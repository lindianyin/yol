/* -------------------------------------------------------------------------
//	文件名		：	kroleservereyes.cpp
//	创建者		：	simon
//	创建时间	：	2011/8/2 21:13:38
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kroleservereyes.h"
#include "kroleserver.h"
#include "kaccountindexing.h"
#include "kdbclustermanager.h"
#include "kg_nameserver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KRoleServerEyes g_cRoleServerEyes;


// -------------------------------------------------------------------------
#define REGISTER_ROLESERVER_FUNCTION(id, func)              \
	do                                                  \
	{                                                   \
	m_ProtocolFunctions[id] = &KRoleServerEyes::func;  \
	} while(false)


KRoleServerEyes::KRoleServerEyes()
{
	memset(m_ProtocolFunctions, 0, sizeof(m_ProtocolFunctions));
}

KRoleServerEyes::~KRoleServerEyes()
{
}

BOOL KRoleServerEyes::Init()
{
	BOOL    bResult  = false;
	BOOL    bRetCode = false;

	bRetCode = m_Stream.Initialize(emKGUARDSERVER_FIRSTID + emKGUARDSERVER_GODDESS, 200 * 1024);
	LOG_PROCESS_ERROR(bRetCode);

	m_nNextPingTime             = 0;
	m_nNextSendPlayerCountTime  = 0;
	m_nNextSendCustomInfoTime   = 0;

	REGISTER_ROLESERVER_FUNCTION(e2l_exit_def, OnE2LStop);
	REGISTER_ROLESERVER_FUNCTION(e2l_getpid_def, OnE2LGetPid);
	REGISTER_ROLESERVER_FUNCTION(g2l_header_def, OnG2LProtocol);

	bResult = true;
EXIT0:
	return bResult;
}

void KRoleServerEyes::UnInit()
{
	m_Stream.UnInitialize();
}

BOOL KRoleServerEyes::Activate()
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

	//if (nTimeNow > m_nNextSendPlayerCountTime)
	//{
	//	SendPlayerCount();
	//	m_nNextSendPlayerCountTime = nTimeNow + 5;
	//}

	if (nTimeNow > m_nNextSendCustomInfoTime)
	{
		SendCustomInfo();
		m_nNextSendCustomInfoTime = nTimeNow + 10;
	}
	return TRUE;
}

void KRoleServerEyes::OnE2LStop(BYTE* pbyData, size_t uDataLen)
{
	QLogPrintf(LOG_INFO, "Received E2LStop Command");
	g_cRoleSvc.Shutdown();
}

void KRoleServerEyes::OnE2LGetPid(BYTE* pbyData, size_t uDataLen)
{
	l2e_getpid  sMsg;

	sMsg.wProtocol = l2e_getpid_def;
	sMsg.wServer   = 0;
	sMsg.nPid	   = getpid();

	m_Stream.PutPack(&sMsg, sizeof(sMsg));

	return;
}

void KRoleServerEyes::OnG2LProtocol(BYTE* pbyData, size_t uDataLen)
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

void KRoleServerEyes::Ping()
{
	KSubProtocolHead    sPing;

	sPing.wProtocol		= l2e_header_def;
	sPing.wSubProtocol	= l2e_ping_def;
	sPing.wServer		= 0;

	m_Stream.PutPack(&sPing, sizeof(sPing));

	return;
}

void KRoleServerEyes::SendPlayerCount()
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

	pnCount[emKGUARD_PLAYERCOUNT_TOTOAL] = 0; // not acquired now

	m_Stream.PutPack(szBuff, sizeof(szBuff));

	return;
}

#define MAX_CUSTOM_INFO_PAK 64000

void KRoleServerEyes::SendCustomInfo()
{
	l2e_update_custom_info* pInfo               = NULL;
	size_t                  uStrLen             = 0;
	IMemBlock*             piBuffer            = NULL;
	int                     nTotalAccounts = 0;
	std::string strClusterStatics;
	std::string strNameDbStatics;
	time_t timeClock = m_nNextPingTime;
	struct tm tmClock = *localtime(&timeClock);

	nTotalAccounts = g_cAccountIndexing.TotalAccounts();

	piBuffer = QCreateMemBlock(MAX_CUSTOM_INFO_PAK);
	LOG_PROCESS_ERROR(piBuffer);
	
	pInfo = (l2e_update_custom_info*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pInfo);

	pInfo->wProtocol	= l2e_header_def;
	pInfo->wServer		= 0;
	pInfo->wSubProtocol = l2e_update_custom_info_def;

	g_cDbClusterMgr.GetWorkStat(strClusterStatics);
	g_cNameServerMgr.GetWorkStat(strNameDbStatics);

	uStrLen = snprintf(
		(char*)pInfo->byData, MAX_CUSTOM_INFO_PAK - sizeof(l2e_update_custom_info),
		"-------------------------------------\n"
		"Total Accounts .... ... ... %d\n"
		"Last Ping .... ... ... .... %s"
		"-------------------------------------\n"
		"%s"
		"-------------------------------------\n"
		"%s",
		nTotalAccounts,
		asctime(&tmClock),
		strClusterStatics.c_str(),
		strNameDbStatics.c_str()
		);
	LOG_PROCESS_ERROR(uStrLen > 0);
	LOG_PROCESS_ERROR(uStrLen < MAX_CUSTOM_INFO_PAK - sizeof(l2e_update_custom_info));

	pInfo->uDataLen = uStrLen;

	m_Stream.PutPack(pInfo, sizeof(l2e_update_custom_info) + uStrLen);

EXIT0:
	SAFE_RELEASE(piBuffer);
	return;
}
