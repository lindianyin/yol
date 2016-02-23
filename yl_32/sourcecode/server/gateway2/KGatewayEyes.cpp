#include "stdafx.h"
#include "Gateway.h"

#define REGISTER_GWEYES_FUNCTION(id, func)              \
	do                                                  \
	{                                                   \
	m_ProtocolFunctions[id] = &KGatewayEyes::func;  \
	} while(false)

KGatewayEyes::KGatewayEyes()
{
	m_pPlayerManager = NULL;
	m_pPaysysAgency  = NULL;
	m_pQueueManager  = NULL;

	memset(m_ProtocolFunctions, 0, sizeof(m_ProtocolFunctions));
}

KGatewayEyes::~KGatewayEyes()
{
}

BOOL KGatewayEyes::Init()
{
	BOOL    bResult  = false;
	BOOL    bRetCode = false;

	bRetCode = m_Stream.Initialize(emKGUARDSERVER_FIRSTID + emKGUARDSERVER_GATEWAY, 200 * 1024);
	LOG_PROCESS_ERROR(bRetCode);

	m_nNextPingTime             = 0;
	m_nNextSendPlayerCountTime  = 0;
	m_nNextSendCustomInfoTime   = 0;
	m_bQuitFlag                 = false;

	REGISTER_GWEYES_FUNCTION(e2l_exit_def, OnE2LStop);
	REGISTER_GWEYES_FUNCTION(e2l_getpid_def, OnE2LGetPid);
	REGISTER_GWEYES_FUNCTION(g2l_header_def, OnG2LProtocol);

	bResult = true;
EXIT0:
	return bResult;
}

void KGatewayEyes::UnInit()
{
	m_Stream.UnInitialize();
}

void KGatewayEyes::Activate()
{
	BYTE*               pbyData             = NULL;
	size_t              nPackLen            = 0;
	BYTE                byProtocol          = 0;
	PROTOCOL_FUNCTION   ProtocolFunction    = NULL;
	time_t              nTimeNow            = time(NULL);

	while (true)
	{
		pbyData = (BYTE*)m_Stream.GetPack(nPackLen);

		if (!pbyData)
		{
			break;
		}

		byProtocol = *pbyData;

		ProtocolFunction = m_ProtocolFunctions[byProtocol];

		if (ProtocolFunction)
		{
			(this->*ProtocolFunction)(pbyData, nPackLen);
		}
		else
		{
			QLogPrintf(LOG_DEBUG, "Invalid FSEyes protocol : %d\n", byProtocol);
		}

		m_Stream.PopPack();
	}

	if (nTimeNow >= m_nNextPingTime)
	{
		Ping();
		m_nNextPingTime = nTimeNow + 20;
	}

	if (nTimeNow > m_nNextSendPlayerCountTime)
	{
		SendPlayerCount();
		m_nNextSendPlayerCountTime = nTimeNow + 5;
	}

	if (nTimeNow > m_nNextSendCustomInfoTime)
	{
		SendCustomInfo();
		m_nNextSendCustomInfoTime = nTimeNow + 10;
	}
}

void KGatewayEyes::OnE2LStop(BYTE* pbyData, size_t uDataLen)
{
	QLogPrintf(LOG_INFO, "Received E2LStop Command");
	m_bQuitFlag = true;
}

void KGatewayEyes::OnE2LGetPid(BYTE* pbyData, size_t uDataLen)
{
	l2e_getpid  sMsg;

	sMsg.wProtocol = l2e_getpid_def;
	sMsg.wServer   = 0;
	sMsg.nPid	   = getpid();

	m_Stream.PutPack(&sMsg, sizeof(sMsg));

	return;
}

void KGatewayEyes::OnG2LProtocol(BYTE* pbyData, size_t uDataLen)
{
	KSubProtocolHead*   pHead   = (KSubProtocolHead*)pbyData;

	if (pHead->wSubProtocol == emKPROTOCOL_G2L_PIDREQ)
	{
		KPROTOCOL_L2G_PIDRET sRet;

		LOG_PROCESS_ERROR(uDataLen == sizeof(KSubProtocolHead));

		sRet.wProtocol		= l2g_header_def;
		sRet.wSubProtocol	= emKPROTOCOL_L2G_PIDRET;
		sRet.nPid			= getpid();
		sRet.wServer		= 0;
		m_Stream.PutPack(&sRet, sizeof(sRet));
	}

EXIT0:
	return;
}

void KGatewayEyes::Ping()
{
	KSubProtocolHead    sPing;

	sPing.wProtocol		= l2e_header_def;
	sPing.wSubProtocol	= l2e_ping_def;
	sPing.wServer		= 0;

	m_Stream.PutPack(&sPing, sizeof(sPing));

	return;
}

void KGatewayEyes::SendPlayerCount()
{
	char                szBuff[sizeof(l2e_PlayerCount) + sizeof(int) * emKGUARD_PLAYERCOUNT_COUNT];
	l2e_PlayerCount*    pProtocol   = (l2e_PlayerCount*)szBuff;
	int*                pnCount     = NULL;

	pProtocol->wProtocol	= l2e_header_def;
	pProtocol->wServer		= 0;
	pProtocol->wSubProtocol = l2e_PlayerCount_def;
	pProtocol->uCount		= emKGUARD_PLAYERCOUNT_COUNT;

	pnCount = (int*)(pProtocol + 1);

	for (int i = 0; i < emKGUARD_PLAYERCOUNT_COUNT; i++)
	{
		pnCount[i] = -1;
	}

	pnCount[emKGUARD_PLAYERCOUNT_TOTOAL] = m_pPlayerManager->GetConnectionCount();

	m_Stream.PutPack(szBuff, sizeof(szBuff));

	return;
}
#define MAX_CUSTOM_INFO_PAK 64000

/*
-------------------------------------
Connection total ... .... %d
Waiting queue ... ... ... %d
Account verify respond .. %u ms
-------------------------------------
*/

void KGatewayEyes::SendCustomInfo()
{
	l2e_update_custom_info* pInfo               = NULL;
	size_t                  uStrLen             = 0;
	IMemBlock*             piBuffer            = NULL;
	int                     nCurrentConnections    = 0;
	DWORD                   dwAccountVerifyTime = 0;
	int                     nWaitingQueuePlayer = 0;

	nCurrentConnections    = m_pPlayerManager->GetConnectionCount();
	dwAccountVerifyTime = m_pPaysysAgency->GetAccountVerifyTimeCost();
	nWaitingQueuePlayer = m_pQueueManager->GetWaitingQueuePlayerCount();

	piBuffer = QCreateMemBlock(MAX_CUSTOM_INFO_PAK);
	LOG_PROCESS_ERROR(piBuffer);

	pInfo = (l2e_update_custom_info*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pInfo);

	pInfo->wProtocol	= l2e_header_def;
	pInfo->wServer		= 0;
	pInfo->wSubProtocol = l2e_update_custom_info_def;

	uStrLen = snprintf(
		(char*)pInfo->byData, MAX_CUSTOM_INFO_PAK - sizeof(l2e_update_custom_info),
		"-------------------------------------\n"
		"Connexion historique . ... %d\n"
		"Connection current ... ... %d\n"
		"Waiting queue .... ... ... %d\n"
		"Account verify respond ... %u ms\n"
		"Paysys disconnect count .. %d\n"
		"-------------------------------------\n",
		m_pPlayerManager->TotalConnectionCount(),
		nCurrentConnections,
		nWaitingQueuePlayer,
		dwAccountVerifyTime,
		m_pPaysysAgency->m_nPaysysDisconnectCount
		);
	LOG_PROCESS_ERROR(uStrLen > 0);
	LOG_PROCESS_ERROR(uStrLen < MAX_CUSTOM_INFO_PAK - sizeof(l2e_update_custom_info));

	pInfo->uDataLen = uStrLen;

	m_Stream.PutPack(pInfo, sizeof(l2e_update_custom_info) + uStrLen);

EXIT0:
	SAFE_RELEASE(piBuffer);
	return;
}

void KGatewayEyes::SendWarningInfo(int nLevel, const char* pszInfo)
{
//	l2e_warning_info*       pInfo               = NULL;
//	size_t                  uInfoLen            = 0;
//	size_t                  uPackLen            = 0;
//	IMemBlock*             piBuffer            = NULL;
//
//	LOG_PROCESS_ERROR(pszInfo);
//
//	uInfoLen = strlen(pszInfo) + 1;
//	uPackLen = sizeof(l2e_warning_info) + uInfoLen;
//
//	piBuffer = QCreateMemBlock((unsigned)uPackLen);
//	LOG_PROCESS_ERROR(piBuffer);
//
//	pInfo = (l2e_warning_info*)piBuffer->GetData();
//	LOG_PROCESS_ERROR(pInfo);
//
//	pInfo->wProtocol	= l2e_header_def;
//	pInfo->wServer		= 0;
//	pInfo->wSubProtocol = l2e_warning_info_def;
//
//	pInfo->nLevel   = nLevel;
//	pInfo->uDataLen = uInfoLen;
//
//	memcpy(pInfo->byData, pszInfo, uInfoLen);
//
//	m_Stream.PutPack(pInfo, uPackLen);
//
//EXIT0:
//	SAFE_RELEASE(piBuffer);
//	return;
}
