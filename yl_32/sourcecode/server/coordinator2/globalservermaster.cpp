#include "stdafx.h"
#include "globalservermaster.h"
#include "GameCenter.h"

GlobalServerMaster g_GlobalServerMaster;

GlobalServerMasterBaseProtocolHandler::GlobalServerMasterBaseProtocolHandler() : GlobalServerMasterProtocolHandler<GlobalServerMasterBaseProtocolHandler>(r2m_base_protocol_begin, r2m_base_protocol_end)
{
	RegisterProtocolProcessFunction(r2m_handshake_request,  &GlobalServerMasterBaseProtocolHandler::OnHandshakeRequest, sizeof(R2M_HANDSHAKE_REQUEST));
	RegisterProtocolProcessFunction(r2m_ping_signal,  &GlobalServerMasterBaseProtocolHandler::OnPingSignal, sizeof(R2M_PING_SIGNAL));
}

GlobalServerMasterBaseProtocolHandler::~GlobalServerMasterBaseProtocolHandler()
{

}

BOOL GlobalServerMasterBaseProtocolHandler::OnHandshakeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bResult = FALSE;
	INT nRetCode = FALSE;
	R2M_HANDSHAKE_REQUEST*  pHandshake = (R2M_HANDSHAKE_REQUEST*)pbyData;
	INT nServerTimeDiff = 0;
	INT nLowerProtocolVersion = 0;
	INT nUpperProtocolVersion = 0;

	g_pGameCenter->m_GameServer.GetGameWorldProtocolVersion(
		&nLowerProtocolVersion, &nUpperProtocolVersion);

	// 方便调试先去掉gameversion限制
	/*
	LOG_PROCESS_ERROR(pHandshake->nGameVersion == nLowerProtocolVersion);
	LOG_PROCESS_ERROR(pHandshake->nGameVersion == nUpperProtocolVersion);
	*/

	nServerTimeDiff = (int)(pHandshake->nServerTime - g_pGameCenter->m_nTimeNow);

	if (abs(nServerTimeDiff) > 1)
	{
		QLogPrintf(LOG_WARNING, "SlaveCR %d Time difference is %d !\n", nConnIndex, nServerTimeDiff);
	}

	//DoHandshakeRespond(nConnIndex);

	bResult = TRUE;
EXIT0:
	if (!bResult)
	{
		g_GlobalServerMaster.CloseConnection(nConnIndex);
	}
	return TRUE;
}

BOOL GlobalServerMasterBaseProtocolHandler::DoHandshakeRespond(int nConnIndex)
{
	BOOL                    bResult         = false;
	int                     nRetCode        = false;
	IMemBlock*             piPackage       = NULL;
	M2R_HANDSHAKE_RESPOND*  pRespond        = NULL;
//	KSLAVECR_INFO*          pSlaveInfo       = NULL;

	//pSlaveInfo = GetSlaveConnectionByIdx(nConnIndex);
	//LOG_PROCESS_ERROR(pSlaveInfo);

	piPackage = QCreateMemBlock((unsigned)sizeof(M2R_HANDSHAKE_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespond = (M2R_HANDSHAKE_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID        = m2r_handshake_respond;
	pRespond->nBaseTime          = g_pGameCenter->m_nBaseTime;
	//pRespond->nWorldIndex        = pSlaveInfo->nWorldIndex;
	pRespond->nStartFrame        = g_pGameCenter->m_nWorkLoop;
	pRespond->nDevMode			= g_pGameCenter->m_bDevMode;

	nRetCode = g_GlobalServerMaster.Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(nRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL GlobalServerMasterBaseProtocolHandler::OnPingSignal(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	return TRUE;
}


GlobalServerMaster::GlobalServerMaster()
{
	m_nLastConnIndex = 1;
	m_nConnectionTimeout = 0;
	memset(m_SlaveCRList, 0, sizeof(m_SlaveCRList));
}

GlobalServerMaster::~GlobalServerMaster()
{
   
}

BOOL GlobalServerMaster::Init()
{
	RegisterProtocolHandler(&m_BaseHandler);

	BOOL nRetCode = FALSE;
	struct timeval  TimeVal  = {0, 0};

	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_MASTER)
	{
		return TRUE;
	}

	m_nConnectionTimeout = g_pGameCenter->m_Settings.m_nConnectionTimeout;

	nRetCode = m_SocketAcceptor.Open("", g_pGameCenter->m_Settings.m_nGlobalServerPort);
	QCONFIRM_RET_FALSE(nRetCode);

	m_SocketAcceptor.SetTimeout(&TimeVal);

	QLogPrintf(LOG_INFO, "Global Server Started....");

	return TRUE;
}

BOOL GlobalServerMaster::UnInit()
{
	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_MASTER)
	{
		return TRUE;
	}

	for (int i = 0; i < MAX_SLAVE_CR_COUNT; i++)
	{
		if (m_SlaveCRList[i].piSocket != NULL)
		{
			CloseConnection(m_SlaveCRList[i].nConnIndex);
		}
	}

	m_SocketAcceptor.Close();

	return TRUE;
}

BOOL GlobalServerMaster::RegisterProtocolHandler(GlobalServerMasterProtocolHandlerBase* handler)
{
	if(std::find(m_arrHandlers.begin(), m_arrHandlers.end(), handler) == m_arrHandlers.end())
	{
	    m_arrHandlers.push_back(handler);
		return TRUE;
	} 
	return FALSE;
}

VOID GlobalServerMaster::Activate()
{
	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_MASTER)
	{
		return;
	}

	ProcessAccepter();

	for(INT i = 0; i < MAX_SLAVE_CR_COUNT; ++i)
	{
		QSLAVECR_INFO* pSlaveInfo = &m_SlaveCRList[i];
		if(pSlaveInfo->piSocket == NULL)
			continue;

		ProcessConnection(pSlaveInfo->nConnIndex, pSlaveInfo->piSocket);
	}
}

BOOL GlobalServerMaster::Send(int nConnIndex, IMemBlock* piBuffer)
{
	BOOL bResult = FALSE;
	INT nRetCode = FALSE;
	QSLAVECR_INFO* pSlaveInfo = NULL;

	PROCESS_SUCCESS(nConnIndex == 0);

	pSlaveInfo = GetSlaveConnectionByIdx(nConnIndex);
	LOG_PROCESS_ERROR(pSlaveInfo);    

	LOG_PROCESS_ERROR(pSlaveInfo->piSocket);    

	nRetCode = pSlaveInfo->piSocket->Send(piBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

EXIT1:
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL GlobalServerMaster::SendGlobal(IMemBlock* piBuffer)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	QSLAVECR_INFO* pSlaveInfo = NULL;

	for(INT i = 0; i < MAX_SLAVE_CR_COUNT; ++i)
	{
		if(m_SlaveCRList[i].nConnIndex == 0)
		{
			continue;
		}

		pSlaveInfo = GetSlaveConnectionByIdx(m_SlaveCRList[i].nConnIndex);
		LOG_PROCESS_ERROR(pSlaveInfo);    

		LOG_PROCESS_ERROR(pSlaveInfo->piSocket);    

		bRetCode = pSlaveInfo->piSocket->Send(piBuffer);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL GlobalServerMaster::ProcessConnection(int nConnIndex, ISocketStream* piSocket)
{
	BOOL nRetCode = FALSE;
	BOOL bResult = FALSE;
	QSLAVECR_INFO* pSlaveInfo = NULL;
	IMemBlock* piPackage = NULL;
	size_t uPakSize = 0;
	INT nIdleTime = 0;
	time_t nCurrentTime = g_pGameCenter->m_nTimeNow;

	assert(piSocket);
	piSocket->AddRef();

	pSlaveInfo = GetSlaveConnectionByIdx(nConnIndex);
	LOG_PROCESS_ERROR(pSlaveInfo);

	while(TRUE)
	{
		struct timeval TimeVal = {0, 0};
		INTERNAL_PROTOCOL_HEADER* pHeader = NULL;

		nRetCode = piSocket->CheckCanRecv(&TimeVal);
		if (nRetCode == 0)
			break;

		PROCESS_ERROR(nRetCode == 1);

		SAFE_RELEASE(piPackage);

		nRetCode = piSocket->Recv(&piPackage);
		PROCESS_ERROR(nRetCode == 1);

		LOG_PROCESS_ERROR(piPackage);

		pSlaveInfo->nLastRecvPacketTime = nCurrentTime;

		VOID* pData = piPackage->GetData();
		LOG_PROCESS_ERROR(pData);
		pHeader = (INTERNAL_PROTOCOL_HEADER*)pData;
		LOG_PROCESS_ERROR(pHeader);
		
		GlobalServerMasterProtocolHandlerBase* handler = FindHandler(pHeader->wProtocolID);
		if(!handler)
		{
			QLogPrintf(LOG_ERR,"..pHeader->wProtocolID=%d",(int)pHeader->wProtocolID);
		}
		
		LOG_PROCESS_ERROR(handler);

		bResult = handler->ProcessData(pData, piPackage->GetSize(), nConnIndex);
	}

	nIdleTime = (int)(nCurrentTime - pSlaveInfo->nLastRecvPacketTime);
	if (nIdleTime > m_nConnectionTimeout)
	{
		QLogPrintf(LOG_DEBUG, "[SlaveCR%d] LastPakTime = %u\n", nConnIndex, pSlaveInfo->nLastRecvPacketTime);
		QLogPrintf(LOG_DEBUG, "[SlaveCR%d] CurrentTime = %u\n", nConnIndex, nCurrentTime);
		QLogPrintf(LOG_ERR, "SlaveCR %d timeout(%d)\n", nConnIndex, nIdleTime);
		goto EXIT0;
	}

	bResult = TRUE;

EXIT0:

	SAFE_RELEASE(piSocket);

	if (!bResult)
	{
		CloseConnection(nConnIndex);
	}

	SAFE_RELEASE(piPackage);
	return bResult;
}

GlobalServerMasterProtocolHandlerBase* GlobalServerMaster::FindHandler(WORD protocol)
{
	HANDLER_ARRAY::iterator it;
	for (it = m_arrHandlers.begin(); it != m_arrHandlers.end(); ++it)
	{
		if (protocol > (*it)->m_eProtocolBegin && protocol < (*it)->m_eProtocolEnd)
		{
			return (*it);
		}
	}
	return NULL;
}

VOID GlobalServerMaster::ProcessAccepter()
{
	INT nWorldIdx = 0;
	ISocketStream* piSocket = NULL;
	QSLAVECR_INFO* pSlaveInfo = NULL;
	struct timeval TimeVal = {3, 0};
	struct in_addr RemoteAddr = { 0 };
	u_short wRemotePortNet  = 0;
	u_short wRemotePortHost = 0;
	char* pszRetString    = NULL;

	piSocket = m_SocketAcceptor.Accept();
	PROCESS_ERROR(piSocket);

	piSocket->SetTimeout(&TimeVal);

	for(INT i = 0; i < MAX_SLAVE_CR_COUNT; ++i)
	{
		if(m_SlaveCRList[i].piSocket == NULL)
		{
			nWorldIdx = i + 1;
			pSlaveInfo = &m_SlaveCRList[i];
			break;
		}
	}

	LOG_PROCESS_ERROR(pSlaveInfo);

	pSlaveInfo->piSocket = piSocket;
	pSlaveInfo->piSocket->AddRef();
	pSlaveInfo->nConnIndex = m_nLastConnIndex++;
	pSlaveInfo->nLastRecvPacketTime  = g_pGameCenter->m_nTimeNow;
	pSlaveInfo->nWorldIndex = nWorldIdx;

	piSocket->GetRemoteAddress(&RemoteAddr, &wRemotePortNet);
	wRemotePortHost = ntohs(wRemotePortNet);

	pszRetString = inet_ntoa(RemoteAddr);
	LOG_PROCESS_ERROR(pszRetString);

	QLogPrintf(
		LOG_INFO, "CR %d connected from %s:%u, WorldIndex = %d\n", 
		pSlaveInfo->nConnIndex, pszRetString, wRemotePortHost, nWorldIdx
		);

EXIT0:
	SAFE_RELEASE(piSocket);
	return;
}

GlobalServerMaster::QSLAVECR_INFO* GlobalServerMaster::GetSlaveConnectionByIdx(INT nConnIndex)
{
	for(INT i = 0; i < MAX_SLAVE_CR_COUNT; ++i)
	{
		if(m_SlaveCRList[i].nConnIndex == nConnIndex)
			return &m_SlaveCRList[i];
	}

	return NULL;
}


BOOL GlobalServerMaster::CloseConnection(INT nConnIndex)
{
	BOOL bResult = FALSE;
	QSLAVECR_INFO* pSlaveInfo = GetSlaveConnectionByIdx(nConnIndex);

	LOG_PROCESS_ERROR(pSlaveInfo);

	QLogPrintf(LOG_INFO, "Slave %d disconnected !\n", nConnIndex);

	SAFE_RELEASE(pSlaveInfo->piSocket);

	pSlaveInfo->nConnIndex = 0;

	bResult = TRUE;
EXIT0:
	return bResult;
}

