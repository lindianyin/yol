#include "stdafx.h"
#include "Gateway.h"
#include <queue>

#ifdef WIN32
#define KG_LoadLibrary LoadLibrary
#define KG_GetProcessAddress GetProcAddress
#define KG_FreeLibrary FreeLibrary
#else
#include <dlfcn.h>
inline void* KG_LoadLibrary(const char* pszLibFileName)
{
	void* pvResult = NULL;

	LOG_PROCESS_ERROR(pszLibFileName);

	pvResult = dlopen(pszLibFileName, RTLD_NOW);
	LOG_PROCESS_ERROR(pvResult);
EXIT0:
	if (!pvResult)
	{
		char* pszErr = dlerror();
		if (pszErr)
		{
			QLogPrintf(LOG_ERR, "[SDOA] %s\n", pszErr);
		}
	}
	return pvResult;
}
#define KG_GetProcessAddress dlsym
#define KG_FreeLibrary dlclose
#endif

#ifdef USE_SDOA
typedef ISDOAUserInfoAuthen* (*KSdoaCreateUserInfoAuthenFunc)(void);
int SNDACALLBACK SdoaUserInfoCallBackFunc(int nMsgType, unsigned long ulUniqueID, ISDOAMsg* piMsg);
#endif // USE_SDOA

typedef std::queue<KTOKEN_VERIFY_RESPOND> KTOKEN_VERIFY_RESPOND_QUEUE;
static KTOKEN_VERIFY_RESPOND_QUEUE g_SDOAVerifyRespondQueue;
static QLock                      g_SDOAVerifyRespondQueueLock;

KSndaAgency::KSndaAgency()
{
#ifdef USE_SDOA
	m_hSDOALibrary      = NULL;
	m_piUserInfoAuthen  = NULL;
#endif // USE_SDOA
	m_pPlayerManager    = NULL;
	m_pPaysysAgency     = NULL;
	m_pGateway          = NULL;
	m_bOpen = false;
}

KSndaAgency::~KSndaAgency()
{
}

BOOL KSndaAgency::Init()
{
	BOOL                            bResult                     = false;
	int                             nRetCode                    = false;
#ifdef USE_SDOA
	KSdoaCreateUserInfoAuthenFunc   SdoaCreateUserInfoAuthen    = NULL;

	nRetCode = LoadConfig();
	LOG_PROCESS_ERROR(nRetCode);

	PROCESS_SUCCESS(!m_bOpen);

	m_hSDOALibrary = KG_LoadLibrary(SDOA_DLL_FILE);
	LOG_PROCESS_ERROR(m_hSDOALibrary);

	SdoaCreateUserInfoAuthen = (KSdoaCreateUserInfoAuthenFunc)KG_GetProcessAddress(m_hSDOALibrary, "sdoaCreateUserInfoAuthen");
	LOG_PROCESS_ERROR(SdoaCreateUserInfoAuthen);

	m_piUserInfoAuthen = SdoaCreateUserInfoAuthen();
	LOG_PROCESS_ERROR(m_piUserInfoAuthen);

	nRetCode = m_piUserInfoAuthen->Initialize(SDOA_CONFIG_FILE, &SdoaUserInfoCallBackFunc);
	if (nRetCode != 0)
	{
		QLogPrintf(LOG_DEBUG, "[SDOA] Initialize failed! RetCode = %d", nRetCode);
		goto EXIT0;
	}

#endif // USE_SDOA
EXIT1:
	bResult = true;
EXIT0:
	if (!bResult)
	{
		QThread_Sleep(10000);
#ifdef USE_SDOA
		SAFE_RELEASE(m_piUserInfoAuthen);

		if (m_hSDOALibrary)
		{
			KG_FreeLibrary(m_hSDOALibrary);
			m_hSDOALibrary = NULL;
		}
#endif // USE_SDOA
	}
	if (m_bOpen)
	{
		QLogPrintf(LOG_INFO, "[SDOA] Initialize %s!", bResult ? "succeed" : "failed");
	}

	return bResult;
}

void KSndaAgency::UnInit()
{
#ifdef USE_SDOA
	SAFE_RELEASE(m_piUserInfoAuthen);

	if (m_hSDOALibrary)
	{
		KG_FreeLibrary(m_hSDOALibrary);
		m_hSDOALibrary = NULL;
	}
#endif // USE_SDOA
}

BOOL KSndaAgency::LoadConfig()
{
	BOOL        bResult     = false;
	BOOL        bRetCode    = false;
	IIniFile*   piIniFile   = NULL;

	piIniFile = g_OpenIniFile(GATEWAY_CONFIG_FILE);
	LOG_PROCESS_ERROR(piIniFile);

	m_bOpen = true;
	bRetCode = piIniFile->IsSectionExist("SDOA");
	if (!bRetCode)
	{
		m_bOpen = false;
		goto EXIT1;
	}

	bRetCode = piIniFile->GetString("SDOA", "AdultIDCard", "not configed", m_szAdultIDCard, sizeof(m_szAdultIDCard)); 
	LOG_PROCESS_ERROR(bRetCode);

EXIT1:
	bResult = true;
EXIT0:
	SAFE_RELEASE(piIniFile);
	return bResult;
}

void KSndaAgency::Activate()
{
	PROCESS_ERROR(m_bOpen);

	g_SDOAVerifyRespondQueueLock.Lock();
	while (!g_SDOAVerifyRespondQueue.empty())
	{
		KTOKEN_VERIFY_RESPOND& rRespond = g_SDOAVerifyRespondQueue.front();
		OnTokenVerifyRespond(&rRespond);

		g_SDOAVerifyRespondQueue.pop();
	}
	g_SDOAVerifyRespondQueueLock.Unlock();

EXIT0:
	return;
}

BOOL KSndaAgency::DoTokenVerifyRequest(KPlayerAgency* pPlayer)
{
	BOOL            bResult     = false;
#ifdef USE_SDOA
	int             nRetCode    = 0;
	const char*     pszIP       = NULL;
	struct in_addr  RemoteIP;
	u_short         uRemotePort;

	assert(pPlayer);
	LOG_PROCESS_ERROR(m_piUserInfoAuthen);
	LOG_PROCESS_ERROR(pPlayer->piSocket);

	nRetCode = pPlayer->piSocket->GetRemoteAddress(&RemoteIP, &uRemotePort);
	LOG_PROCESS_ERROR(nRetCode);

	pszIP = inet_ntoa(RemoteIP);
	LOG_PROCESS_ERROR(pszIP);

	nRetCode = m_piUserInfoAuthen->AsyncGetUserInfo(pPlayer->szSndaToken, pszIP, (unsigned long)pPlayer->nIndex);
	if (nRetCode != 0)
	{
		QLogPrintf(LOG_DEBUG, "[SDOA] AsyncGetUserInfo failed! RetCode = %d", nRetCode);
		goto EXIT0;
	}

	bResult = true;
EXIT0:
#endif // USE_SDOA
	return bResult;
}

void KSndaAgency::OnTokenVerifyRespond(const KTOKEN_VERIFY_RESPOND* pRespond)
{
	int             nRetCode    = 0;
	KPlayerAgency*  pPlayer     = NULL;

	assert(pRespond);

	if (pRespond->nCode != 0)
	{
		QLogPrintf(LOG_DEBUG, "[SDOA] Token verify failed! PlayerIndex = %d, Result = %d\n", pRespond->nPlayerIndex, pRespond->nCode);
		goto EXIT0;
	}

	pPlayer = m_pPlayerManager->GetPlayer(pRespond->nPlayerIndex);
	LOG_PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(pPlayer->nState == easWaitForSDOAVerify);

	nRetCode = strncmp(pPlayer->szAccount, pRespond->szSndaID, sizeof(pPlayer->szAccount));
	LOG_PROCESS_ERROR(nRetCode == 0);

	strncpy(pPlayer->szIDCard, pRespond->szIDCard, sizeof(pPlayer->szIDCard));
	pPlayer->szIDCard[sizeof(pPlayer->szIDCard) - 1] = '\0';

	if (
		(strncmp(pRespond->szIDCard, "NULL", sizeof(pRespond->szIDCard)) == 0) &&
		pRespond->bAdult
		)
	{
		strncpy(pPlayer->szIDCard, m_szAdultIDCard, sizeof(pPlayer->szIDCard));
		pPlayer->szIDCard[sizeof(pPlayer->szIDCard) - 1] = '\0';
	}

	pPlayer->dwVerifyPostTime = GetTickCount();

	pPlayer->nState = easWaitForPaysysVerify;
	m_pPaysysAgency->DoAccountVerifySDRequest(pPlayer);

EXIT0:
	return;
}
#ifdef USE_SDOA
int SNDACALLBACK SdoaUserInfoCallBackFunc(int nMsgType, unsigned long ulUniqueID, ISDOAMsg* piMsg)
{
	KTOKEN_VERIFY_RESPOND Respond;

	Respond.nCode           = nMsgType;
	Respond.nPlayerIndex    = (int)ulUniqueID;
	Respond.szSndaID[0]     = '\0';
	Respond.szIDCard[0]     = '\0';

	if (nMsgType == 0)
	{
		const char* pszValue = NULL;

		pszValue = piMsg->GetValue("sndaID");
		if (pszValue)
		{
			strncpy(Respond.szSndaID, pszValue, sizeof(Respond.szSndaID));
			Respond.szSndaID[sizeof(Respond.szSndaID) - 1] = '\0';
		}

		pszValue = piMsg->GetValue("idCard");
		if (pszValue)
		{
			strncpy(Respond.szIDCard, pszValue, sizeof(Respond.szIDCard));
			Respond.szIDCard[sizeof(Respond.szIDCard) - 1] = '\0';
		}

		pszValue = piMsg->GetValue("adult");

		Respond.bAdult = false;
		if (pszValue && pszValue[0] == 'Y')
		{
			Respond.bAdult = true;
		}
	}

	g_SDOAVerifyRespondQueueLock.Lock();
	g_SDOAVerifyRespondQueue.push(Respond);
	g_SDOAVerifyRespondQueueLock.Unlock();

	return true;
}
#endif // USE_SDOA

