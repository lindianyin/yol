#include "stdafx.h"
#include "PayAgency.h"
#include "GameCenter.h"
//#include "KItemDef.h"

#define PACKET_SIZE 1024
#define MAX_EVENT_ACCEPT 1024

#define REGISTER_P2R_FUNCTION(id, func, data)               \
	do                                                      \
	{                                                       \
	m_ProtocolFunctions[id] = &KPayAgency::func;    \
	m_uPakSize[id]          = sizeof(data);             \
	} while(false)

KPayAgency::KPayAgency()
{
    m_piSocket      = NULL;
    m_nPingCycle    = 30;
    m_nNextPingTime = 0;
    m_szIdentity[0] = '\0';
}

BOOL KPayAgency::Init()
{
	BOOL bRet = FALSE;

	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_SLAVE)
	{
		goto EXIT0;
	}

	bRet = m_cSockServer.Init(g_pGameCenter->m_Settings.m_szPayAgencyIP,
		g_pGameCenter->m_Settings.m_nPayAgencyPort,
		MAX_EVENT_ACCEPT,
		PACKET_SIZE,
		PACKET_SIZE,
		ENCODE_DECODE_NONE/*KSG_ENCODE_DECODE*/,
		NULL);

	LOG_PROCESS_ERROR(bRet);

	m_pSockEvents = new QSOCKET_EVENT[MAX_EVENT_ACCEPT];
	memset(m_pSockEvents, 0, sizeof(QSOCKET_EVENT) * MAX_EVENT_ACCEPT);
	/*BOOL bRetCode = m_Acceptor.Open("127.0.0.1", 21212);
	LOG_PROCESS_ERROR(bRetCode);*/

	REGISTER_P2R_FUNCTION(P2R_PROTOCOL_CHECK_NAME, OnCheckName, CHAR);
	REGISTER_P2R_FUNCTION(P2R_PROTOCOL_PAY, OnUserPay, CHAR);
EXIT0:
    return true;
}

void KPayAgency::UnInit()
{
	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_SLAVE)
	{
		return;
	}

    SAFE_RELEASE(m_piSocket);
	m_cSockServer.UnInit(NULL);
	SAFE_DELETE_ARRAY(m_pSockEvents);
	m_Acceptor.Close();
}

void KPayAgency::Activate()
{
	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_SLAVE)
	{
		return;
	}

	ProcessAcceptor();

	//ProcessPackage();

	CheckTimeout();
}

BOOL KPayAgency::OnCheckName(BYTE* pbyData, size_t uDataLen)
{
	BOOL                bResult         = false;
	IMemBlock*         piSendBuffer    = NULL;
	LPCTSTR szContent = (LPCTSTR)pbyData;
	CHAR* pRespond = NULL;
	CHAR szAccount[64];
	strncpy(szAccount, szContent, 64);

	KAccount* pAccount = g_pGameCenter->m_RoleManager.GetAccount(szAccount);

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(CHAR)*4);
	LOG_PROCESS_ERROR(piSendBuffer);
	pRespond = (CHAR*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);
	if (pAccount == NULL)
	{
		strncpy(pRespond, "0", 10);
	}
	else
	{
		strncpy(pRespond, "1", 10);
	}
	
	Send(piSendBuffer);
	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}


BOOL KPayAgency::OnUserPay(BYTE* pbyData, size_t uDataLen)
{
	BOOL                bResult         = false;
	BOOL				bOrder			= false;
	IMemBlock*         piSendBuffer    = NULL;
	CHAR* pRespond = NULL;
	LPCTSTR szContent = (LPCTSTR)pbyData;
	CHAR szAccount[64];
	INT nAddGold = 0;
	CHAR szOrderno[64];
	INT nSize = 0;

	strncpy(szAccount, szContent, 64);
	nSize = strlen(szContent);
	szContent += (nSize + 1);

	nSize = strlen(szContent);
	nAddGold = atoi(szContent);
	szContent += (nSize + 1);

	nSize = strlen(szContent);
	strncpy(szOrderno, szContent, 64);
	szContent += (nSize + 1);

	KRole* pRole = g_pGameCenter->m_RoleManager.GetRoleForAccount(szAccount);

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(CHAR)*4);
	LOG_PROCESS_ERROR(piSendBuffer);
	pRespond = (CHAR*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	if (pRole == NULL)
	{
		strncpy(pRespond, "0", 10);
	}
	else
	{
		bOrder = g_pGameCenter->m_RoleDB.ProcessPayOrder(pRole->GetId(), szOrderno, nAddGold); 
		//订单重复
		if (bOrder == FALSE)
		{
			strncpy(pRespond, "2", 10);
		} 
		else
		{
			strncpy(pRespond, "1", 10);
		}
	}

	Send(piSendBuffer);
	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}


BOOL KPayAgency::ProcessAcceptor()
{
	BOOL                bResult                     = false;
	BOOL                bRetCode                    = false;
	ISocketStream*   piSocket                    = NULL;
	timeval             IOTimeout                   = {5, 0};
	struct in_addr      RemoteAddr                  = { 0 };
	u_short             wRemotePortNet              = 0;
	u_short             wRemotePortHost             = 0;
	char*               pszRetString                = NULL;

	INT					nEventCount = 0;
	QSOCKET_EVENT* pEvent = NULL;
	PROCESS_ERROR(m_piSocket == NULL);

	// 监听网络事件
	if (!m_cSockServer.Wait(MAX_EVENT_ACCEPT, m_pSockEvents, &nEventCount))
		return FALSE;
	//piSocket = m_Acceptor.Accept();
	//PROCESS_ERROR(piSocket);
	for (INT i = 0; i < nEventCount; i++)
	{
		pEvent = &m_pSockEvents[i];
		piSocket = pEvent->piSocket;
		piSocket->SetTimeout(&IOTimeout);

		piSocket->GetRemoteAddress(&RemoteAddr, &wRemotePortNet);

		wRemotePortHost = ntohs(wRemotePortNet);

		m_piSocket = piSocket;
		m_piSocket->AddRef();
		SAFE_RELEASE(pEvent->piSocket);

		pszRetString = inet_ntoa(RemoteAddr);
		LOG_PROCESS_ERROR(pszRetString);

		m_nLastPingTime = g_pGameCenter->m_nTimeNow;
		PROCESS_ERROR(ProcessPackage());
	}

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSocket);
	return bResult;
}

BOOL KPayAgency::ProcessPackage()
{
	BOOL                    bResult             = false;
	int                     nRetCode            = 0;
	IMemBlock*             piRecvPackage       = NULL;
	size_t                  uBufferSize         = 0;
	LPTSTR pBuffer = NULL;
	INT nProtocol = 0;
	INT nProSize = 0;

	while (m_piSocket)
	{
		timeval                 TimeoutValue        = {0, 0};
		PROTOCOL_FUNCTION       pfnProtocolFunction = NULL;
		int                     nProtocol           = 0;

		/*nRetCode = m_piSocket->CheckCanRecv(&TimeoutValue);
		if (nRetCode == 0)
			break;

		LOG_PROCESS_ERROR(nRetCode == 1);*/

		SAFE_RELEASE(piRecvPackage);

		nRetCode = m_piSocket->Recv(&piRecvPackage);
		PROCESS_ERROR(nRetCode == 1);

		LOG_PROCESS_ERROR(piRecvPackage);

		m_nLastPingTime = g_pGameCenter->m_nTimeNow;

		uBufferSize = (size_t)piRecvPackage->GetSize();
		LOG_PROCESS_ERROR(uBufferSize > PROTOCOL_START_SIZE);
		
		pBuffer = (LPTSTR)piRecvPackage->GetData();

		pBuffer += PROTOCOL_START_SIZE;

		nProSize = strlen(pBuffer);
		nProtocol = atoi(pBuffer);

		pBuffer += (nProSize + 1);
		
		LOG_PROCESS_ERROR(nProtocol > P2R_PROTOCOL_BEGIN);
		LOG_PROCESS_ERROR(nProtocol < P2R_PROTOCOL_TOTAL);

		//LOG_PROCESS_ERROR(uBufferSize >= m_uPakSize[nProtocol]);

		pfnProtocolFunction = m_ProtocolFunctions[nProtocol];
		LOG_PROCESS_ERROR(pfnProtocolFunction);

		(this->*pfnProtocolFunction)((BYTE*)pBuffer, uBufferSize);
	}

	bResult = true;
EXIT0:
	if (!bResult)
	{
		if (m_piSocket)
		{
			QLogPrintf(LOG_INFO, "[Pay] Connection lost !\n");
		}

		SAFE_RELEASE(m_piSocket);
	}
	SAFE_RELEASE(piRecvPackage);
	return bResult;
}

BOOL KPayAgency::CheckTimeout()
{
	DWORD   dwTimeNow = 0;

	PROCESS_ERROR(m_piSocket);

	if (g_pGameCenter->m_nTimeNow > m_nLastPingTime + m_nPingCycle)
	{
		QLogPrintf(LOG_INFO, "[Pay] connection timeout !\n");

		SAFE_RELEASE(m_piSocket);
	}

EXIT0:
	return true;
}


BOOL KPayAgency::Send(IMemBlock* piBuffer)
{
    BOOL bResult  = false;
    int  nRetCode = false;

    PROCESS_ERROR(m_piSocket);

    nRetCode = m_piSocket->Send(piBuffer);
    LOG_PROCESS_ERROR(nRetCode == 1);

    bResult = true;
EXIT0:
    return bResult;
}
