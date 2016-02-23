#include "stdafx.h"
#include "globalserverslave.h"
#include "GameCenter.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/korpgworld.h"

GlobalServerSlave g_GlobalServerSlave;

GlobalServerSlaveBaseProtocolHandler::GlobalServerSlaveBaseProtocolHandler() : GlobalServerSlaveProtocolHandler<GlobalServerSlaveBaseProtocolHandler>(m2r_base_protocol_begin, m2r_base_protocol_end)
{
	RegisterProtocolProcessFunction(m2r_handshake_respond,  &GlobalServerSlaveBaseProtocolHandler::OnHandshakeRespond, sizeof(M2R_HANDSHAKE_RESPOND));
}

GlobalServerSlaveBaseProtocolHandler::~GlobalServerSlaveBaseProtocolHandler()
{

}

BOOL GlobalServerSlaveBaseProtocolHandler::OnHandshakeRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                    bRetCode   = FALSE;
	M2R_HANDSHAKE_RESPOND*  pHandshake = (M2R_HANDSHAKE_RESPOND*)pbyData;
	IMemBlock*             piSendBuffer    = NULL;
	R2S_BZONE_HANDSHAKE_RESPOND* pNotify = NULL;

	//m_nWorldIndex = pHandshake->nWorldIndex;
	g_pGameCenter->m_nBaseTime = pHandshake->nBaseTime;
	g_pGameCenter->m_nWorkLoop = pHandshake->nStartFrame;
	g_pGameCenter->m_bDevMode = pHandshake->nDevMode;

	if (pHandshake->nDevMode)
	{
		QConsoleHelper::DoInfoColor();
		QLogPrintf(LOG_INFO, "Running DevMode, GM Command OPEN", pHandshake->nWorldIndex);
		QConsoleHelper::RestoreColor();
	}

	QLogPrintf(LOG_INFO, "Set world index = %d\n", pHandshake->nWorldIndex);

	piSendBuffer = QCreateMemBlock(sizeof(R2S_BZONE_HANDSHAKE_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (R2S_BZONE_HANDSHAKE_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->nBaseTime = g_pGameCenter->m_nBaseTime;
	pNotify->nDevMode = g_pGameCenter->m_bDevMode;
	pNotify->nStartFrame = g_pGameCenter->m_nWorkLoop;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return TRUE;
}

GlobalServerSlave::GlobalServerSlave()
{
	m_piSocketStream = NULL;
	m_bSendErrorFlag = FALSE;
	m_nPingCycle = 0;
	m_nLastSendPacketTime = 0;
	m_nWorldIndex = 0;

}

GlobalServerSlave::~GlobalServerSlave()
{

}

BOOL GlobalServerSlave::Init()
{
	RegisterProtocolHandler(&m_BaseHander);

	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	struct timeval TimeVal = {8, 0};
	QSocketConnector  Connector;

	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_SLAVE)
	{
		bResult = TRUE;
		goto EXIT0;
	}

	m_piSocketStream = Connector.Connect(g_pGameCenter->m_Settings.m_szGlobalServerAddr, 
		g_pGameCenter->m_Settings.m_nGlobalServerPort);
	LOG_PROCESS_ERROR(m_piSocketStream);

	bRetCode = m_piSocketStream->SetTimeout(&TimeVal);
	LOG_PROCESS_ERROR(bRetCode);

	m_nPingCycle = g_pGameCenter->m_Settings.m_nGlobalServerPingInterval;

	bRetCode = DoHandshakeRequest();
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	if (!bResult)
	{
		SAFE_RELEASE(m_piSocketStream);
	}

	if(g_pGameCenter->m_Settings.m_nGlobalServerMode == emRUNMODE_SLAVE)
	{
		QLogPrintf(
			LOG_INFO, "Connect to MasterCR %s:%d ... [%s]",
			g_pGameCenter->m_Settings.m_szGlobalServerAddr, 
			g_pGameCenter->m_Settings.m_nGlobalServerPort, 
			bResult ? "OK" : "Failed"
			);
	}

	return bResult;
}

BOOL GlobalServerSlave::UnInit()
{
	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_SLAVE)
	{
		return TRUE;
	}

	return TRUE;
}

BOOL GlobalServerSlave::RegisterProtocolHandler(GlobalServerSlaveProtocolHandlerBase* handler)
{
	if(std::find(m_arrHandlers.begin(), m_arrHandlers.end(), handler) == m_arrHandlers.end())
	{
		m_arrHandlers.push_back(handler);
		return TRUE;
	} 
	return FALSE;
}

GlobalServerSlaveProtocolHandlerBase* GlobalServerSlave::FindHandler(WORD protocol)
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

VOID GlobalServerSlave::Activate()
{
	ProcessPackage();
}

BOOL GlobalServerSlave::DoHandshakeRequest()
{
	BOOL                    bResult         = false;
	BOOL                    bRetCode        = false;
	IMemBlock*             piPackage       = NULL;
	R2M_HANDSHAKE_REQUEST*  pHandshake      = NULL;

	piPackage = QCreateMemBlock((unsigned)sizeof(S2R_HANDSHAKE_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pHandshake = (R2M_HANDSHAKE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pHandshake);

	pHandshake->wProtocolID = r2m_handshake_request;
	pHandshake->nGameVersion  = KD_PROTOCOL_VERSION;
	pHandshake->nServerTime  = KSysService::GameTime(NULL);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}


BOOL GlobalServerSlave::DoPingSignal()
{
	BOOL						bResult				= FALSE;
	BOOL						bRetCode			= FALSE;
	IMemBlock*                 piSendBuffer        = NULL;
	R2M_PING_SIGNAL*			pPingSingal			= NULL;

	piSendBuffer = QCreateMemBlock(sizeof(R2M_PING_SIGNAL));
	LOG_PROCESS_ERROR(piSendBuffer);

	pPingSingal = (R2M_PING_SIGNAL*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pPingSingal);

	pPingSingal->wProtocolID       = r2m_ping_signal;
	pPingSingal->dwReserved        = 0;

	bRetCode = Send(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);   

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL GlobalServerSlave::ProcessPackage()
{
	BOOL bResult = FALSE;
	INT nRetCode = FALSE;
	INT nConnectionAlive = TRUE;
	IMemBlock* piBuffer = NULL;

	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_SLAVE)
	{
		bResult = TRUE;
		goto EXIT0;
	}

	assert(m_piSocketStream);

	while (TRUE)
	{
		const struct timeval        TimeVal     = {0, 0};
		INTERNAL_PROTOCOL_HEADER*   pHeader     = NULL;
		size_t                      uPakSize    = 0;

		if (KSysService::GameTime(NULL) - m_nLastSendPacketTime > m_nPingCycle)
		{
			DoPingSignal();
		}

		nRetCode = m_piSocketStream->CheckCanRecv(&TimeVal);
		if (nRetCode == -1)
		{
			nConnectionAlive = FALSE;
			goto EXIT0;
		}
		if (nRetCode == 0)
		{
			break;
		}

		LOG_PROCESS_ERROR(nRetCode == 1);

		SAFE_RELEASE(piBuffer);

		nRetCode = m_piSocketStream->Recv(&piBuffer);
		if (nRetCode == -1)
		{
			nConnectionAlive = FALSE;
			goto EXIT0;
		}
		LOG_PROCESS_ERROR(nRetCode == 1);

		pHeader = (INTERNAL_PROTOCOL_HEADER*)piBuffer->GetData();
		LOG_PROCESS_ERROR(pHeader);
   
		GlobalServerSlaveProtocolHandlerBase* handler = FindHandler(pHeader->wProtocolID);
		LOG_PROCESS_ERROR(handler);

		bResult = handler->ProcessData(piBuffer->GetData(), piBuffer->GetSize());
	}

	bResult = TRUE;

EXIT0:
	if (m_piSocketStream && !nConnectionAlive)
	{
		QLogPrintf(LOG_DEBUG, "[SlaveCR] LastPakTime = %u\n", m_nLastSendPacketTime);
		QLogPrintf(LOG_DEBUG, "[SlaveCR] CurrentTime = %u\n", KSysService::GameTime(NULL));

		QLogPrintf(LOG_ERR, "Coordinator lost, shutdown !\n");

		//TODO zhaoyu:shutdown?
	}
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL GlobalServerSlave::Send(IMemBlock* piBuffer)
{
	BOOL bResult  = FALSE;
	INT nRetCode = FALSE;

	assert(piBuffer);
	LOG_PROCESS_ERROR(m_piSocketStream);

	nRetCode = m_piSocketStream->Send(piBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	m_nLastSendPacketTime = KSysService::GameTime(NULL);

	bResult = TRUE;
EXIT0:
	if (!bResult)
		m_bSendErrorFlag = TRUE;

	return bResult;
}
