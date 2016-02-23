
#include "stdafx.h"
#include "kgc_coordinatorclient.h"
#include "kmodulesetting.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamebase/kscriptmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

#define REGISTER_PROTOCOL_HANDLER(Protocol, Func, DataStruct)          \
do                                                                                  \
{                                                                                   \
	m_PakProcessor[Protocol] = &KGC_CoordinatorClient::Func;                             \
	m_uPakSize[Protocol] = sizeof(tagProtocolHeader) + sizeof(DataStruct);   \
} while (false)

// -------------------------------------------------------------------------

KGC_CoordinatorClient& KGC_CoordinatorClient::Inst()
{
	static KGC_CoordinatorClient _s_CoordinatorClient;
	return _s_CoordinatorClient;
}

KGC_CoordinatorClient::KGC_CoordinatorClient()
{
	m_piSocket = NULL;
	m_nCoordinatorPort = 0;
	ZeroStruct(m_PakProcessor);
	ZeroStruct(m_uPakSize);

	REGISTER_PROTOCOL_HANDLER(l2s_sync_time, OnSyncTime, L2S_SYNC_TIME);
	REGISTER_PROTOCOL_HANDLER(l2s_gc_intercomm, OnGcIntercomm, L2S_GC_INTERCOMM);
}

BOOL KGC_CoordinatorClient::Init(PCSTR pszIp, INT nPort)
{
	INT nRet = FALSE;
	LOG_PROCESS_ERROR(pszIp);

	m_strCoordinatorIp = pszIp;
	m_nCoordinatorPort = nPort;
	m_piSocket = NULL;

	nRet = Connect(pszIp, nPort);
	PROCESS_ERROR(nRet);

EXIT0:
	return nRet;
}

BOOL KGC_CoordinatorClient::Uninit()
{
	SAFE_RELEASE(m_piSocket);
	return TRUE;
}

INT KGC_CoordinatorClient::WaitForModuleInfo()
{
	INT nResult = FALSE;
	IMemBlock* piBuffer = NULL;
	L2S_GC_MODULE_LIST* pModuleList = NULL;

	nResult = SendModuleInfoRequest(m_piSocket);
	LOG_PROCESS_ERROR(nResult);

	QLogPrintf(LOG_INFO, "Waiting for module list...");
	nResult = m_piSocket->Recv(&piBuffer);
	LOG_PROCESS_ERROR(nResult == 1);

	pModuleList = (L2S_GC_MODULE_LIST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pModuleList);
	LOG_PROCESS_ERROR(piBuffer->GetSize() >= sizeof(*pModuleList));
	LOG_PROCESS_ERROR(pModuleList->byProtocol == l2s_sync_game_center_module);

	KModuleSetting::Inst()->m_nDevModeFlag = pModuleList->nDevModeFlag;
	nResult = KModuleSetting::Inst()->SetModuleInfo(pModuleList->nModuleCount,
		(KGC_MODULE_INFO*)pModuleList->abyModuleInfo);
	LOG_PROCESS_ERROR(nResult);

	nResult = TRUE;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return nResult;
}

INT KGC_CoordinatorClient::SendModuleInfoRequest( ISocketStream* piSocket )
{
	INT nResult = FALSE;
	S2L_GAMECENTER_MODULE_REQUEST* pReq = NULL;

	ASSERT(piSocket);
	IMemBlock* piBuffer = QCreateMemBlock(sizeof(S2L_GAMECENTER_MODULE_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pReq = (S2L_GAMECENTER_MODULE_REQUEST*)piBuffer->GetData();
	pReq->byProtocol = s2l_gamecenter_module_request;
	pReq->dwLoaderIp = 0; // ÔÝÊ±²»ÓÃ
	pReq->nServerId = 0;
	pReq->nZoneId = 0;

	nResult = piSocket->Send(piBuffer);
	LOG_PROCESS_ERROR(nResult == 1);
	nResult = TRUE;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return nResult;
}

INT KGC_CoordinatorClient::Activate()
{
	if (!m_piSocket)
		return Connect(m_strCoordinatorIp.c_str(), m_nCoordinatorPort);

	ProcessPackage();

	return TRUE;
}

BOOL KGC_CoordinatorClient::ProcessPackage()
{
	BOOL bResult = FALSE;
	INT nRetCode = false;
	IMemBlock* piPackage = NULL;
	PPROTOCOL_FUNC ProtocolFunc = NULL;
	BYTE* pbyData = NULL;
	size_t uDataLen = 0;
	KGR_PROTOCOL_HEADER* pHeader = NULL;
	BOOL bSocketError = FALSE;

	PROCESS_ERROR(m_piSocket);

	while (true)
	{
		timeval TimeoutValue = {0, 0};

		nRetCode = m_piSocket->CheckCanRecv(&TimeoutValue);
		if (nRetCode == -1)
		{
			bSocketError = true;
			goto EXIT0;
		}

		if (nRetCode == 0)
			break;

		SAFE_RELEASE(piPackage);

		nRetCode = m_piSocket->Recv(&piPackage);
		if (nRetCode != 1)
		{
			bSocketError = true;
			goto EXIT0;
		}

		LOG_PROCESS_ERROR(piPackage);

		pbyData = (BYTE*)piPackage->GetData();
		LOG_PROCESS_ERROR(pbyData);

		uDataLen = piPackage->GetSize();
		LOG_PROCESS_ERROR(uDataLen >= sizeof(KGR_PROTOCOL_HEADER));

		pHeader = (KGR_PROTOCOL_HEADER*)pbyData;

		ProtocolFunc = m_PakProcessor[pHeader->byProtocol];
		if (ProtocolFunc == NULL)
			continue;

		(this->*ProtocolFunc)(pbyData, uDataLen);
	}

	bResult = true;
EXIT0:
	if (m_piSocket && bSocketError)
	{
		QLogPrintf(LOG_ERR, "[Coordinator] Connection lost !\n");
		SAFE_RELEASE(m_piSocket);
	}
	SAFE_RELEASE(piPackage);
	return bResult;
}

void KGC_CoordinatorClient::OnSyncTime( BYTE* pbyData, size_t uDataLen )
{
	L2S_SYNC_TIME* pPack = (L2S_SYNC_TIME*)pbyData;

	time_t tCoordinatorTime = pPack->dwTime;
	KSysService::SetGameTime(&tCoordinatorTime);
}

void KGC_CoordinatorClient::OnGcIntercomm( BYTE* pbyData, size_t uDataLen )
{
	L2S_GC_INTERCOMM* pPack = (L2S_GC_INTERCOMM*)pbyData;

	ScriptSafe cScriptSafe = g_cScriptManager.GetSafeScript();

	if (cScriptSafe->LoadBuffer2Value(pPack->abyBuffer, pPack->uSize))
	{
		cScriptSafe->CallTableFunction("GCEvent", "OnIntercomm",
			0, "v", cScriptSafe.GetBeginTopIndex() + 1);
	}
}

INT KGC_CoordinatorClient::Send( IMemBlock* piBuffer )
{
	INT nRet = FALSE;
	LOG_PROCESS_ERROR(m_piSocket);

	nRet = m_piSocket->Send(piBuffer);
	LOG_PROCESS_ERROR(nRet);

EXIT0:
	return nRet;
}

INT KGC_CoordinatorClient::Connect( PCSTR pszIp, INT nPort )
{
	INT nRet = FALSE;
	QSocketConnector Connector;
	//timeval TimeoutValue;

	QLogPrintf(LOG_INFO, "Connecting coordinator %s:%d", pszIp, nPort);
	LOG_PROCESS_ERROR(pszIp);

	m_piSocket = Connector.Connect(pszIp, nPort);
	LOG_PROCESS_ERROR(m_piSocket);

	//TimeoutValue.tv_sec  = 1;
	//TimeoutValue.tv_usec = 0;
	nRet = m_piSocket->SetTimeout(NULL/*&TimeoutValue*/);
	LOG_PROCESS_ERROR(nRet);

	nRet = TRUE;
EXIT0:
	return nRet;
}

