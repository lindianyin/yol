/* -------------------------------------------------------------------------
//	文件名		：	KGatewayProcess.cpp
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:25:49
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kgatewayprocess.h"
#include "protocol/roleserver_protocol.h"
#include "kdbclustermanager.h"
#include "kg_nameserver.h"
#include "kaccountindexing.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KGatewayProcess g_cGatewayProcess;


// -------------------------------------------------------------------------

KGatewayProcess::KGatewayProcess(void) :
m_nMaxConnId(0),
m_u64PackageId(0),
m_u64TotalSend(0),
m_u64TotalSendOk(0),
m_uBytesRecv(0),
m_uBytesSend(0)
{

	ZeroMemory(m_afnProtocolFunction, sizeof(m_afnProtocolFunction));
	//ZeroMemory(m_afnResultFunction, sizeof(m_afnResultFunction));
	// 初始化协议处理器
	{
		m_afnProtocolFunction[emKC2RS_QUERY_ROLE_LIST] = &KGatewayProcess::OnRequestQueryRoleList;
		m_afnProtocolFunction[emKC2RS_CREATE_ROLE] = &KGatewayProcess::OnRequestCreateRole;
		m_afnProtocolFunction[emKC2RS_GET_ROLE_DATA] = &KGatewayProcess::OnRequestGetRoleData;
		m_afnProtocolFunction[emKC2RS_SAVE_ROLE_DATA] = &KGatewayProcess::OnRequestSaveRoleData;
		m_afnProtocolFunction[emKC2RS_PING] = &KGatewayProcess::OnPing;
	}
	// 初始化结果处理器
	{
		//m_afnResultFunction[emKRESPONSE_CREATE_ROLE_NAME] = &KGatewayProcess::OnResultCreateRoleName;
	}
}

KGatewayProcess::~KGatewayProcess(void)
{
}

BOOL KGatewayProcess::Init( INT nMaxConnectId )
{
	QLogPrintf(LOG_INFO, "[KGatewayProcess] Init...");
	m_nMaxConnId = nMaxConnectId;
	m_vConnId2Session.resize(m_nMaxConnId + 1);

	return TRUE;
}

BOOL KGatewayProcess::ProcessEvent( INT nConnectId, INT nEventId, LPCVOID pData, INT nData )
{
	QCONFIRM_RET_FALSE(nConnectId > 0 && nConnectId <= m_nMaxConnId);
	switch (nEventId)
	{
	case emKSOCKET_EVENT_CREATE:
		{
			static INT s_nSessionIdGenerator = 0;
			m_vConnId2Session[nConnectId].nSessionId = ++s_nSessionIdGenerator;
			m_vConnId2Session[nConnectId].timeLastPing = time(NULL);
			QLogPrintf(LOG_INFO, "New Client[%s:%d] Connection[%d] Session[%d]",
				(LPCSTR)pData, nData, nConnectId, s_nSessionIdGenerator);
		}
		break;
	case emKSOCKET_EVENT_CLOSE:
		{
			QLogPrintf(LOG_INFO, "Connection [%d] Session [%d] Closed!",
				nConnectId, m_vConnId2Session[nConnectId].nSessionId);
			m_vConnId2Session[nConnectId].nSessionId = 0;
			m_vConnId2Session[nConnectId].timeLastPing = 0;
		}
		break;
	case emKSOCKET_EVENT_MASS:
		{
			QLogPrintf(LOG_WARNING, "Massive Package From Connection [%d]!", nConnectId);
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;
}

BOOL KGatewayProcess::ProcessPacket( INT nConnectId, IMemBlock* piPacketBuffer )
{
	m_u64PackageId++;
	m_uBytesRecv = piPacketBuffer->GetSize();
	KProcessingPackage sProcessingPack;
	sProcessingPack.u64CallerArg = (INT64(nConnectId) << 32) | m_vConnId2Session[nConnectId].nSessionId;
	sProcessingPack.pData = piPacketBuffer;

	ASSERT(piPacketBuffer);
	KC2RS_HEADER* pReq = (KC2RS_HEADER*)piPacketBuffer->GetData();
	ASSERT(pReq);

	if (pReq->byProtocol >= emKC2RS_ALL || ! m_afnProtocolFunction[pReq->byProtocol] )
	{
		QLogPrintf(
			LOG_WARNING,
			"[KGatewayProcess::ProcessPacket] unknown protocol [%d] in CommPack [%u] from Gateway [%d]", 
			(INT)pReq->byProtocol,
			(INT)sProcessingPack.u64CallerArg,
			nConnectId
		);
	}
	else
	{
		INT nRetCode = (this->*m_afnProtocolFunction[pReq->byProtocol]) (sProcessingPack);
		//QCONFIRM_RET_FALSE(nRetCode);
	}
	return TRUE;
}

BOOL KGatewayProcess::Activate()
{
	ProcessRoleDbResults();
	ProcessNameServerResults();

	if (g_sRoleSvcCfg.sNetwork.nPingTimeOut)
		CheckSessionAlive();

	return TRUE;
}

BOOL KGatewayProcess::CheckSessionAlive()
{
	static INT nCheckPos = 0;
	nCheckPos = nCheckPos % m_nMaxConnId;
	nCheckPos++;

	KRS_CLIENT_SESSION& rSession = m_vConnId2Session[nCheckPos];
	if (rSession.nSessionId)
	{
		time_t timeNow = time(NULL);
		time_t timeDiff = timeNow - rSession.timeLastPing;
		if (timeDiff > g_sRoleSvcCfg.sNetwork.nPingTimeOut)
		{
			tm tmLocal;
			localtime_r(&rSession.timeLastPing, &tmLocal);
			QLogPrintf(LOG_ERR,
				"Conn[%d] Session[%d] timeout, last ping %s",
				nCheckPos, rSession.nSessionId, asctime(&tmLocal)
				);
			g_piSocketServer->Disconnect(nCheckPos);
		}
	}

	return TRUE;
}

INT KGatewayProcess::OnRequestQueryRoleList(const KProcessingPackage& rcPack)
{
	IMemBlock* piReqBuffer = rcPack.pData;
	ASSERT(piReqBuffer);

	KC2RS_QUERY_ROLE_LIST* pReq = (KC2RS_QUERY_ROLE_LIST*)piReqBuffer->GetData();
	ASSERT(pReq);

	//Q_Printl("OnRequestQueryRoleList " << pReq->szAccount);

	const KAccountInfo* pAccountInfo = g_cAccountIndexing.GetAccountInfo(pReq->szAccount);

	if (pAccountInfo && pAccountInfo->nDatabaseIdentity > 0)
	{
		QLogPrintf(LOG_INFO, "[OnRequestQueryRoleList] %s", pReq->szAccount);
		BOOL bRet = g_cDbClusterMgr.PushWork(pAccountInfo->nDatabaseIdentity, rcPack);
		QCONFIRM_RET_FALSE(bRet);
	}
	else
	{
		IMemBlock* piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_QUERY_ROLE_LIST_REPLY));
		QCONFIRM_RET_FALSE(piReplyBuffer);
		KRS2C_QUERY_ROLE_LIST_REPLY* pResponse = (KRS2C_QUERY_ROLE_LIST_REPLY*)piReplyBuffer->GetData();
		QCONFIRM_RET_FALSE(pResponse);
		pResponse->nRoleCount = 0;
		pResponse->uRoleListSize = 0;
		pResponse->byProtocol = emKRS2C_QUERY_ROLE_LIST_REPLY;
		pResponse->byErrorCode = emKRS2C_ERROR_SUCCESS;
		pResponse->uPackageId = pReq->uPackageId;

		QLogPrintf(LOG_INFO, "[OnRequestQueryRoleList] account[%s] not found", pReq->szAccount);

		SendReplyPackage(rcPack.u64CallerArg, piReplyBuffer);
		SAFE_RELEASE(piReplyBuffer);
	}

	return TRUE;
}

INT KGatewayProcess::OnRequestCreateRole(const KProcessingPackage& rcPack)
{
	INT nRet = FALSE;
	IMemBlock* piAdditionalBuffer = NULL;
	KRequest_CreateRoleName* pForwardRequest = NULL;
	KProcessingPackage sProcessingPack = rcPack;
	INT nDbIdentity = 0;
	const KAccountInfo* pAccountInfo = NULL;

	IMemBlock* piReqBuffer = rcPack.pData;
	ASSERT(piReqBuffer);

	// 过长过短字符串过滤
	KC2RS_CREATE_ROLE* pReq = (KC2RS_CREATE_ROLE*)piReqBuffer->GetData();
	ASSERT(pReq);
	SIZE_T uRoleLen = strnlen(pReq->szRole, sizeof(pReq->szRole));
	SIZE_T uAccountLen = strnlen(pReq->szAccount, sizeof(pReq->szAccount));
	if (uRoleLen == 0 || uRoleLen >= sizeof(pReq->szRole) ||
		uAccountLen == 0 || uAccountLen >= sizeof(pReq->szAccount))
	{
		QLogPrintf(LOG_ERR, "[OnRequestCreateRole] string too long/short: Role [%.*s] Account [%.*s]",
			sizeof(pReq->szRole), pReq->szRole,
			sizeof(pReq->szAccount), pReq->szAccount);

		KRS2C_PTC_HEADER* pResponse = NULL;
		IMemBlock* piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_PTC_HEADER));
		LOG_PROCESS_ERROR(piReplyBuffer);
		pResponse = (KRS2C_PTC_HEADER*)piReplyBuffer->GetData();
		LOG_PROCESS_ERROR(pResponse);

		pResponse->byProtocol = emKRS2C_CREATE_ROLE_REPLY;
		pResponse->byErrorCode = emKRS2C_ERROR_BUG_CHECK;
		pResponse->uPackageId = pReq->uPackageId;

		SendReplyPackage(rcPack.u64CallerArg, piReplyBuffer);
		SAFE_RELEASE(piReplyBuffer);
		PROCESS_ERROR(FALSE);
	}

	//Q_Printl("[OnRequestCreateRole] " << pReq->szAccount);
	pAccountInfo = g_cAccountIndexing.GetAccountInfo(pReq->szAccount);

	if (pAccountInfo)
	{
		nDbIdentity = pAccountInfo->nDatabaseIdentity;
		ASSERT(pAccountInfo->nDatabaseIdentity);
		// 角色个数超限
		if (pAccountInfo->nRoleCount >= g_sRoleSvcCfg.nMaxRolePerAccount)
		{
			KRS2C_PTC_HEADER* pResponse = NULL;
			IMemBlock* piReplyBuffer = NULL;

			QLogPrintf(LOG_ERR,
				"[OnRequestCreateRole] [%s] has %d roles, MaxRolePerAccount %d",
				pReq->szAccount,
				(INT)pAccountInfo->nRoleCount,
				g_sRoleSvcCfg.nMaxRolePerAccount );

			piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_PTC_HEADER));
			LOG_PROCESS_ERROR(piReplyBuffer);
			pResponse = (KRS2C_PTC_HEADER*)piReplyBuffer->GetData();
			LOG_PROCESS_ERROR(pResponse);

			pResponse->byProtocol = emKRS2C_CREATE_ROLE_REPLY;
			pResponse->byErrorCode = emKRS2C_ERROR_TOO_MANY_ROLES;
			pResponse->uPackageId = pReq->uPackageId;

			SendReplyPackage(rcPack.u64CallerArg, piReplyBuffer);
			SAFE_RELEASE(piReplyBuffer);

			PROCESS_ERROR(FALSE);
		}
	}
	else
	{
		nDbIdentity = g_cAccountIndexing.GetSuggestedDb(pReq->szAccount);

		QLogPrintf(LOG_INFO,
			"[OnRequestCreateRole] account[%s] has no roles. Prepare to create on Db[%d]",
			pReq->szAccount,
			nDbIdentity);

		if (! nDbIdentity)
		{
			KRS2C_PTC_HEADER* pResponse = NULL;
			IMemBlock* piReplyBuffer = NULL;
			piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_PTC_HEADER));
			LOG_PROCESS_ERROR(piReplyBuffer);
			pResponse = (KRS2C_PTC_HEADER*)piReplyBuffer->GetData();
			LOG_PROCESS_ERROR(pResponse);

			pResponse->byProtocol = emKRS2C_CREATE_ROLE_REPLY;
			pResponse->byErrorCode = emKRS2C_ERROR_DATABASE;
			pResponse->uPackageId = pReq->uPackageId;

			SendReplyPackage(rcPack.u64CallerArg, piReplyBuffer);
			SAFE_RELEASE(piReplyBuffer);
		}
	}
	LOG_PROCESS_ERROR(nDbIdentity && "Database Cluster is full!!! Can't find a free database.");

	piAdditionalBuffer = RS_MEM_ALLOCATOR(sizeof(KRequest_CreateRoleName));
	LOG_PROCESS_ERROR(piAdditionalBuffer);
	pForwardRequest = (KRequest_CreateRoleName*)piAdditionalBuffer->GetData();
	LOG_PROCESS_ERROR(pForwardRequest);

	pForwardRequest->nDbIdentity = nDbIdentity;
	pForwardRequest->bNewAccount = pAccountInfo ? FALSE : TRUE;
	QStrCpyLen(pForwardRequest->szDbName,
		g_sRoleSvcCfg.pDbNodeParams[nDbIdentity - 1].szDatabase,
		sizeof(pForwardRequest->szDbName)
		);
	QStrCpyLen(pForwardRequest->szGateway,
		"server0101", // TODO server zone id
		sizeof(pForwardRequest->szGateway)
		);

	sProcessingPack.pAdditional = piAdditionalBuffer;
	nRet = g_cNameServerMgr.PushWork(1, sProcessingPack);
	if (nRet)
		g_cAccountIndexing.OnCreateRole(pReq->szAccount, nDbIdentity);

	nRet = TRUE;
EXIT0:
	SAFE_RELEASE(piAdditionalBuffer);
	return nRet;
}

INT KGatewayProcess::OnRequestGetRoleData(const KProcessingPackage& rcPack)
{
	IMemBlock* piReqBuffer = rcPack.pData;
	ASSERT(piReqBuffer);

	KC2RS_GET_ROLE_DATA* pReq = (KC2RS_GET_ROLE_DATA*)piReqBuffer->GetData();
	ASSERT(pReq);

	//Q_Printl("OnRequestGetRoleData " << pReq->szRole);

	const KAccountInfo* pAccountInfo = g_cAccountIndexing.GetAccountInfo(pReq->szAccount);

	if (pAccountInfo && pAccountInfo->nDatabaseIdentity > 0)
	{
		QLogPrintf(LOG_DEBUG,
			"[OnRequestGetRoleData] [%s] for account [%s]",
			pReq->szRole,
			pReq->szAccount);
		BOOL bRet = g_cDbClusterMgr.PushWork(pAccountInfo->nDatabaseIdentity, rcPack);
		QCONFIRM_RET_FALSE(bRet);
	}
	else
	{
		QLogPrintf(LOG_DEBUG,
			"[OnRequestGetRoleData] [%s] for account [%s], failed: account not found.",
			pReq->szRole,
			pReq->szAccount);
		IMemBlock* piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_PTC_HEADER));
		QCONFIRM_RET_FALSE(piReplyBuffer);
		KRS2C_PTC_HEADER* pResponse = (KRS2C_PTC_HEADER*)piReplyBuffer->GetData();
		QCONFIRM_RET_FALSE(pResponse);
		pResponse->byProtocol = emKRS2C_GET_ROLE_DATA_REPLY;
		pResponse->byErrorCode = emKRS2C_ERROR_BUG_CHECK;
		pResponse->uPackageId = pReq->uPackageId;

		SendReplyPackage(rcPack.u64CallerArg, piReplyBuffer);
		SAFE_RELEASE(piReplyBuffer);
	}

	return TRUE;
}

INT KGatewayProcess::OnRequestSaveRoleData(const KProcessingPackage& rcPack)
{
	IMemBlock* piReqBuffer = rcPack.pData;
	ASSERT(piReqBuffer);

	KC2RS_SAVE_ROLE_DATA* pReq = (KC2RS_SAVE_ROLE_DATA*)piReqBuffer->GetData();
	ASSERT(pReq);

	//QLogPrintf(LOG_DEBUG, "OnRequestSaveRoleData %s for %s", pReq->szRole, pReq->szAccount);

	const KAccountInfo* pAccountInfo = g_cAccountIndexing.GetAccountInfo(pReq->szAccount);

	if (pAccountInfo && pAccountInfo->nDatabaseIdentity > 0)
	{
		QLogPrintf(LOG_DEBUG,
			"[OnRequestSaveRoleData] [%s] for account [%s]",
			pReq->szRole,
			pReq->szAccount);
		BOOL bRet = g_cDbClusterMgr.PushWork(pAccountInfo->nDatabaseIdentity, rcPack);
		QCONFIRM_RET_FALSE(bRet);
	}
	else
	{
		QLogPrintf(LOG_DEBUG,
			"[OnRequestSaveRoleData] [%s] for account [%s], failed: account not found.",
			pReq->szRole,
			pReq->szAccount);
		IMemBlock* piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_PTC_HEADER));
		QCONFIRM_RET_FALSE(piReplyBuffer);
		KRS2C_PTC_HEADER* pResponse = (KRS2C_PTC_HEADER*)piReplyBuffer->GetData();
		QCONFIRM_RET_FALSE(pResponse);
		pResponse->byProtocol = emKRS2C_SAVE_ROLE_DATA_REPLY;
		pResponse->byErrorCode = emKRS2C_ERROR_BUG_CHECK;
		pResponse->uPackageId = pReq->uPackageId;

		SendReplyPackage(rcPack.u64CallerArg, piReplyBuffer);
		SAFE_RELEASE(piReplyBuffer);
	}

	return TRUE;
}

INT KGatewayProcess::SendReplyPackage(UINT64 u64ClientId, IMemBlock* piBuffer)
{
	m_u64TotalSend++;
	QCONFIRM_RET_FALSE(piBuffer);
	// only for functionality test
	//if (nClientId == 0)
	//return TRUE;

	INT nConnId = INT(u64ClientId >> 32);
	INT nSessionId = (INT)u64ClientId; // 截断高32位
	INT nCurrentSessionId = m_vConnId2Session[nConnId].nSessionId; // 此连接当前会话Id（判断连接号是否重用过）
	if (nCurrentSessionId && nSessionId != nCurrentSessionId)
	{
		QLogPrintf(LOG_ERR,
			"Invalid Session[%d], Connection[%d] has been recycled with new Session[%d]",
			nSessionId, nConnId, nCurrentSessionId);
		return FALSE;
	}

	BOOL bRet = g_piSocketServer->Send(nConnId, piBuffer);
	m_u64TotalSendOk += (bRet ? 1 : 0);
	m_uBytesSend += (bRet ? piBuffer->GetSize() : 0);
	return bRet;
}

INT KGatewayProcess::ProcessRoleDbResults()
{
	KProcessingPackageList cResultList;
	g_cDbClusterMgr.GetResult(cResultList);

	for (KProcessingPackageList::iterator it = cResultList.begin(); it != cResultList.end(); ++it)
	{
		KProcessingPackage& rPack = *it;
		if (rPack.pData)
		{
			//extern UINT64 g_uMemStreamed;
			//g_uMemStreamed += rPack.pData->GetSize();

			KRS2C_PTC_HEADER* pReplyHeader = (KRS2C_PTC_HEADER*)rPack.pData->GetData();
			ASSERT(pReplyHeader);
			if (pReplyHeader->byProtocol == emKRS2C_QUERY_ROLE_LIST_REPLY)
			{
				KRS2C_QUERY_ROLE_LIST_REPLY* pRoleList = (KRS2C_QUERY_ROLE_LIST_REPLY*)pReplyHeader;
				ASSERT(rPack.pAdditional);
				KC2RS_QUERY_ROLE_LIST* pReq = (KC2RS_QUERY_ROLE_LIST*)((IMemBlock*)rPack.pAdditional)->GetData();
				ASSERT(pReq);
				g_cAccountIndexing.UpdateRoleCount(pReq->szAccount, pRoleList->nRoleCount);
			}

			SendReplyPackage(rPack.u64CallerArg, rPack.pData);
			SAFE_RELEASE(rPack.pData);
			SAFE_RELEASE(rPack.pAdditional);
		}
	}
	return TRUE;
}

INT KGatewayProcess::OnResultCreateRoleName( const KProcessingPackage& rcPack )
{
	INT nRet = FALSE;
	IMemBlock* piBuffer = NULL;
	KResponse_CreateRoleName* pForwardReply = NULL;
	KProcessingPackage sProcessingPack = rcPack;
	KC2RS_CREATE_ROLE* pOriginalReq = NULL;

	LOG_PROCESS_ERROR(rcPack.pAdditional);
	pOriginalReq = (KC2RS_CREATE_ROLE*)((IMemBlock*)rcPack.pAdditional)->GetData();
	LOG_PROCESS_ERROR(pOriginalReq);

	piBuffer = rcPack.pData;
	LOG_PROCESS_ERROR(piBuffer);
	pForwardReply = (KResponse_CreateRoleName*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pForwardReply);

	//Q_Printl("OnResultCreateRoleName " << pOriginalReq->szRole);

	if (pForwardReply->nErrorCode == emKRS2C_ERROR_SUCCESS)
	{
		sProcessingPack.pAdditional = piBuffer; // piBuffer留给_create_role释放
		sProcessingPack.pData = (IMemBlock*)rcPack.pAdditional;
		g_cDbClusterMgr.PushWork(pForwardReply->nDbIdentity, sProcessingPack);
	}
	else
	{
		g_cAccountIndexing.OnCreateRole(pOriginalReq->szAccount, 0); // 此账户下角色数减一
		KRS2C_PTC_HEADER* pResponse = NULL;
		IMemBlock* piReplyBuffer = NULL;

		piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_PTC_HEADER));
		LOG_PROCESS_ERROR(piReplyBuffer);
		pResponse = (KRS2C_PTC_HEADER*)piReplyBuffer->GetData();
		LOG_PROCESS_ERROR(pResponse);

		pResponse->byProtocol = emKRS2C_CREATE_ROLE_REPLY;
		pResponse->byErrorCode = pForwardReply->nErrorCode;
		pResponse->uPackageId = pOriginalReq->uPackageId;

		SendReplyPackage(rcPack.u64CallerArg, piReplyBuffer);
		SAFE_RELEASE(piReplyBuffer);
	}

	nRet = TRUE;
EXIT0:
	return nRet;
}

INT KGatewayProcess::ProcessNameServerResults()
{
	KProcessingPackageList cResultList;
	g_cNameServerMgr.GetResult(cResultList);

	for (KProcessingPackageList::iterator it = cResultList.begin(); it != cResultList.end(); ++it)
	{
		KProcessingPackage& rPack = *it;
		OnResultCreateRoleName(rPack);
		SAFE_RELEASE(it->pData);
		SAFE_RELEASE(it->pAdditional);
	}
	return TRUE;
}

INT KGatewayProcess::OnPing( const KProcessingPackage& rcPack )
{
	//printf("OnPing\n");
	INT nConnId = INT(rcPack.u64CallerArg >> 32);;
	KRS_CLIENT_SESSION& rSession = m_vConnId2Session[nConnId];
	rSession.timeLastPing = time(NULL);
	return TRUE;
}

BOOL KGatewayProcess::Uninit()
{
	QLogPrintf(LOG_INFO, "[KGatewayProcess] statics: total recv %llu, send %llu/%llu, bytes recv %llu, send %llu",
		m_u64PackageId, m_u64TotalSendOk, m_u64TotalSend,
		(UINT64)m_uBytesRecv, (UINT64)m_uBytesSend);
	QLogPrintf(LOG_INFO, "[KGatewayProcess] Uninit...");
	m_vConnId2Session.clear();

	return TRUE;
}
