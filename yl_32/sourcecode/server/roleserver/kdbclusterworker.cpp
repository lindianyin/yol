/* -------------------------------------------------------------------------
//	文件名		：	KDbClusterWorker.cpp
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:21:45
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kdbclusterworker.h"
#include "protocol/roleserver_protocol.h"
#include "roleserver_cfg.h"
#include "Misc/IDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KDbClusterWorker::PProtocolProcessFunction KDbClusterWorker::ms_afnProtocolFunction[UCHAR_MAX + 1] = { 0 };


// -------------------------------------------------------------------------

KDbClusterWorker::KDbClusterWorker() :
m_nRef(0),
m_bRunning(FALSE),
m_uTotalJobs(0),
m_uTotalJobsOk(0),
m_uBytesRecv(0),
m_uBytesSend(0)
{
	// 初始化协议
	{
		ms_afnProtocolFunction[emKC2RS_QUERY_ROLE_LIST] = &KDbClusterWorker::_QueryRoleList;
		ms_afnProtocolFunction[emKC2RS_CREATE_ROLE] = &KDbClusterWorker::_CreateRole;
		ms_afnProtocolFunction[emKC2RS_GET_ROLE_DATA] = &KDbClusterWorker::_GetRoleData;
		ms_afnProtocolFunction[emKC2RS_SAVE_ROLE_DATA] = &KDbClusterWorker::_SaveRoleData;
	}
}

KDbClusterWorker::~KDbClusterWorker(void)
{
}

BOOL KDbClusterWorker::Init( const KROLESVC_DB_PARAM& rcDatabaseCfg, const std::string& strName )
{
	INT nRetCode = FALSE;
	QLogPrintf(LOG_INFO, "[DbClusterWorker] %s Init...", strName.c_str());

	m_strWorkerName = strName;
	nRetCode = InitDatabase(rcDatabaseCfg);
	QCONFIRM_RET_FALSE(nRetCode);

	m_bRunning = TRUE;
	nRetCode = m_WorkThread.Create(_WorkThreadFunction, this);
	QCONFIRM_RET_FALSE(nRetCode);

	return nRetCode;
}

BOOL KDbClusterWorker::Uninit()
{
	QLogPrintf(LOG_INFO, "[DbClusterWorker] %s Uninit...", m_strWorkerName.c_str());
	m_bRunning = FALSE;
	m_WorkThread.Wait();
	SAFE_RELEASE(m_piDatabase);
	KGD_Uninit(NULL);
	m_cJobListLock.Unlock();
	m_cResultListLock.Unlock();

	QLogPrintf(LOG_INFO, "[DbCluster] %s statics: jobs %u/%u, bytes recv %llu, send %llu",
		m_strWorkerName.c_str(), m_uTotalJobsOk, m_uTotalJobs, (UINT64)m_uBytesRecv, (UINT64)m_uBytesSend);

	for (KProcessingPackageList::iterator it = m_cResultList.begin(); it != m_cResultList.end(); ++it)
	{
		SAFE_RELEASE(it->pAdditional);
		SAFE_RELEASE(it->pData);
	}
	if (m_cResultList.size())
		QLogPrintf(LOG_INFO, "[DbCluster] %s discarded %u results", m_strWorkerName.c_str(), m_cResultList.size());

	for (KProcessingPackageList::iterator it = m_cJobList.begin(); it != m_cJobList.end(); ++it)
	{
		SAFE_RELEASE(it->pAdditional);
		SAFE_RELEASE(it->pData);
	}
	if (m_cJobList.size())
		QLogPrintf(LOG_INFO, "[DbCluster] %s discarded %u jobs", m_strWorkerName.c_str(), m_cJobList.size());

	Release();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
INT KDbClusterWorker::ProcessJobQueue()
{
	INT nRetCode = TRUE;

	KProcessingPackageList cJobList;
	KProcessingPackageList cTempResultList; // 写入m_cResultList前先汇总

	// 获取任务列表
	m_cJobListLock.Lock();
	cJobList.swap(m_cJobList);
	m_cJobListLock.Unlock();

	m_uTotalJobs += cJobList.size();
	for (KProcessingPackageList::iterator it = cJobList.begin(); it != cJobList.end(); ++it)
	{
		if (!m_bRunning)
			break;
		KProcessingPackage sResult;
		nRetCode = ProcessSingleJob(*it, sResult);
		//QCONFIRM(nRetCode && "ProcessSingleJob Failed");
		if (nRetCode && sResult.pData)
			m_uTotalJobsOk++;
		if (sResult.pData)
			cTempResultList.push_back(sResult);
		SAFE_RELEASE(it->pData);
		SAFE_RELEASE(it->pAdditional);
	}

	if (cJobList.size() > 0) 
	{
		// 输出任务结果
		m_cResultListLock.Lock();
		m_cResultList.insert(m_cResultList.end(), cTempResultList.begin(), cTempResultList.end());
		m_cResultListLock.Unlock();
	}
	else
	{
		QThread_Sleep(1); // 空闲中，降低CPU使用率，减少对m_cJobListLock的争抢
	}

	nRetCode = TRUE;
//EXIT0:
	return nRetCode;
}

INT KDbClusterWorker::_ThreadFunction()
{
	int nResult  = false;
	int nRetCode = false;

	while (m_bRunning)
	{
		nRetCode = ProcessJobQueue();
		//LOG_PROCESS_ERROR(nRetCode && "Process Request failed, will stop");
	}

	nResult = true;
//EXIT0:
	SAFE_RELEASE(m_piDatabase);
	QLogPrintf(LOG_INFO, "%s Stopped", m_strWorkerName.c_str());

	return nResult;
}

////////////////////////////////////////////////////////////////////////////////
void KDbClusterWorker::_WorkThreadFunction(void *pvParam)
{
	KDbClusterWorker *pThis = (KDbClusterWorker *)pvParam;

	ASSERT(pThis);

	pThis->_ThreadFunction();
}

INT KDbClusterWorker::_QueryRoleList(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult)
{
	INT nRet = FALSE;
	SIZE_T uRoleListSize = 0;
	INT nRoleCount = 0;
	IMemBlock* piResponseBuffer = NULL;
	IMemBlock* piTempBuffer = NULL;
	LPBYTE pTemp = NULL;
	KRS2C_QUERY_ROLE_LIST_REPLY* pReplyPack = NULL;
	IKGD_Result   *piResult       = NULL;
	KGD_FIELD PreviewInfoField = { RDB_FIELD_ROLE_PREVIEW_INFO };
	KGD_FIELD AccountField  = { RDB_FIELD_ACCOUNT };
	KE_RS2C_ERROR_CODE eErrCode = emKRS2C_ERROR_DATABASE;

	//QLogPrintf(LOG_DEBUG, "%s Enter %s()", m_strWorkerName.c_str(), __FUNCTION__);

	IMemBlock* piRequestBuffer = rcRequest.pData;
	assert(piRequestBuffer);
	KC2RS_QUERY_ROLE_LIST* pReq = (KC2RS_QUERY_ROLE_LIST*)piRequestBuffer->GetData();
	assert(pReq);

	// ---------- query role list item information by account -----------------  
	AccountField.pvData    = pReq->szAccount;
	AccountField.uDataSize = strlen(pReq->szAccount) + 1;
	LOG_PROCESS_ERROR(AccountField.uDataSize > 1);
	LOG_PROCESS_ERROR(AccountField.uDataSize <= sizeof(pReq->szAccount));

	//QLogPrintf(
	//	LOG_INFO, 
	//	"%s Query roles list of account [%s]", 
	//	m_strWorkerName.c_str(), 
	//	AccountField.pvData 
	//	);

	eErrCode = emKRS2C_ERROR_DATABASE;
	ASSERT(m_piDatabase);
	nRet = m_piDatabase->Query(
		RDB_TABLE_ROLE, 
		1, 
		&PreviewInfoField, 
		&AccountField, 
		KGD_OPERATOR_EQUAL,
		NULL, 
		KGD_OPERATOR_NONE
		);
	LOG_PROCESS_ERROR(nRet);

	piResult = m_piDatabase->GetQueryResult(KGD_RESULT_STORE);
	LOG_PROCESS_ERROR(piResult);

	nRoleCount = piResult->GetRowCount();
	LOG_PROCESS_ERROR(nRoleCount >= 0);

	// -------------------- create buffer for reply  --------------------  
	if (nRoleCount > 0)
	{
		piTempBuffer = RS_MEM_ALLOCATOR(g_sRoleSvcCfg.nMaxRoleListBuffer);
		LOG_PROCESS_ERROR(piTempBuffer);

		pTemp = (LPBYTE)piTempBuffer->GetData();
		assert(pTemp);

		for (INT i = 0; i < nRoleCount; ++i)
		{
			nRet = piResult->NextRow();
			LOG_PROCESS_ERROR(nRet);

			nRet = piResult->GetCurrentRowField(0, &PreviewInfoField);
			LOG_PROCESS_ERROR(nRet);

			eErrCode = emKRS2C_ERROR_DATA_TOO_LARGE;
			uRoleListSize += /*sizeof(UINT) + */PreviewInfoField.uDataSize;
			LOG_PROCESS_ERROR(uRoleListSize <= (UINT)g_sRoleSvcCfg.nMaxRoleListBuffer);

			//*(UINT*)pTemp = PreviewInfoField.uDataSize;
			//pTemp += sizeof(UINT);
			memcpy(pTemp, PreviewInfoField.pvData, PreviewInfoField.uDataSize);
			pTemp += PreviewInfoField.uDataSize;
		}
	}

	piResponseBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_QUERY_ROLE_LIST_REPLY) + uRoleListSize);
	LOG_PROCESS_ERROR(piResponseBuffer);

	pReplyPack = (KRS2C_QUERY_ROLE_LIST_REPLY*)piResponseBuffer->GetData();
	assert(pReplyPack);

	pReplyPack->byProtocol = emKRS2C_QUERY_ROLE_LIST_REPLY;
	pReplyPack->byErrorCode = emKRS2C_ERROR_SUCCESS;
	pReplyPack->uPackageId = pReq->uPackageId;
	pReplyPack->nRoleCount = nRoleCount;
	pReplyPack->uRoleListSize = uRoleListSize;
	if (nRoleCount)
		memcpy(pReplyPack->byRoleList, piTempBuffer->GetData(), uRoleListSize);

	m_uBytesSend += piRequestBuffer->GetSize();
	m_uBytesRecv += uRoleListSize;
	nRet = TRUE;
EXIT0:
	rcResult.u64CallerArg = rcRequest.u64CallerArg;
	rcResult.pAdditional = NULL;
	rcResult.pData = NULL;

	if ( ! nRet && pReq)
	{
		SAFE_RELEASE(piResponseBuffer);
		piResponseBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_PTC_HEADER));
		if (piResponseBuffer)
		{
			KRS2C_PTC_HEADER* pErrorPack = (KRS2C_PTC_HEADER*)piResponseBuffer->GetData();
			assert(pErrorPack);
			pErrorPack->byProtocol = emKRS2C_QUERY_ROLE_LIST_REPLY;
			pErrorPack->byErrorCode = eErrCode;
			pErrorPack->uPackageId = pReq->uPackageId;
		}
		QLogPrintf(LOG_INFO, 
			"%s Query roles list for account [%s], Error %d", 
			m_strWorkerName.c_str(), 
			AccountField.pvData,
			eErrCode
			);
	}
	else
	{
		QLogPrintf(LOG_INFO, 
			"%s Query roles list for account [%s], found %d role(s)", 
			m_strWorkerName.c_str(), 
			AccountField.pvData,
			nRoleCount
			);
	}
	rcResult.pData = piResponseBuffer;
	rcRequest.pData->AddRef();
	rcResult.pAdditional = rcRequest.pData;

	SAFE_RELEASE(piTempBuffer);
	SAFE_RELEASE(piResult);

	//QLogPrintf(LOG_DEBUG, "%s Leave %s()", m_strWorkerName.c_str(), __FUNCTION__);
	return nRet;
}

////////////////////////////////////////////////////////////////////////////////
INT KDbClusterWorker::_CreateRole(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult)
{
	INT nRet = FALSE;
	CHAR szTime[KG_TIME_STRING_SIZE] = "";
	KGD_FIELD aInsertFields[] = 
	{
		{ RDB_FIELD_ROLE_ID }, 
		{ RDB_FIELD_ROLE }, 
		{ RDB_FIELD_ACCOUNT }, 
		{ RDB_FIELD_ROLE_PREVIEW_INFO },
		{ RDB_FIELD_ROLE_DATA },
		{ RDB_FIELD_ROLE_LAST_SAVE_TIME }
	};

	//QLogPrintf(LOG_DEBUG, "%s Enter %s()", m_strWorkerName.c_str(), __FUNCTION__);

	IMemBlock* piReplyBuffer = NULL;
	IMemBlock* piAdditionalBuffer = NULL;
	KResponse_CreateRoleName* pForwardReply = NULL;
	IMemBlock* piRequestBuffer = rcRequest.pData;
	assert(piRequestBuffer);
	KC2RS_CREATE_ROLE* pReq = (KC2RS_CREATE_ROLE*)piRequestBuffer->GetData();
	assert(pReq);
	piAdditionalBuffer = (IMemBlock*)rcRequest.pAdditional;
	assert(piAdditionalBuffer);
	pForwardReply = (KResponse_CreateRoleName*)piAdditionalBuffer->GetData();
	assert(pForwardReply);

	piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_CREATE_ROLE_REPLY));
	LOG_PROCESS_ERROR(piReplyBuffer && "low mem!");

	nRet = g_GetLocalTimeString(szTime, sizeof(szTime));
	LOG_PROCESS_ERROR(nRet);

	//QLogPrintf(LOG_DEBUG, "Creating RoleDataSize %d", pReq->uRoleDataSize);
	//printf("Data ");
	//for(int i = 0; i < pReq->uRoleDataSize; ++i)
	//	printf("%02x ", pReq->abyData[pReq->uRoleListSize + i]);

	// 设置字段值
	aInsertFields[0].pvData    = &pForwardReply->u64RoleId;
	aInsertFields[0].uDataSize = KGD_UNSIGNED_INT64_SIZE;

	aInsertFields[1].pvData    = pReq->szRole;
	aInsertFields[1].uDataSize = strlen(pReq->szRole) + 1;

	aInsertFields[2].pvData    = pReq->szAccount;
	aInsertFields[2].uDataSize = strlen(pReq->szAccount) + 1;

	aInsertFields[3].pvData    = pReq->abyData;
	aInsertFields[3].uDataSize = pReq->uRoleListSize;

	aInsertFields[4].pvData    = PVOID(pReq->abyData + pReq->uRoleListSize);
	aInsertFields[4].uDataSize = pReq->uRoleDataSize;

	aInsertFields[5].pvData    = szTime;
	aInsertFields[5].uDataSize = strlen(szTime) + 1;  

	nRet = m_piDatabase->Insert(
		RDB_TABLE_ROLE, 
		sizeof(aInsertFields) / sizeof(aInsertFields[0]), 
		aInsertFields
		);
	LOG_PROCESS_ERROR(nRet);

	nRet = m_piDatabase->GetAffectedRow();
	LOG_PROCESS_ERROR(nRet == 1); // 成功写入数据库

	// 向账号库添加
	if (pForwardReply->bNewAccount)
	{
		KGD_FIELD aInsertFieldsAccount[] = 
		{
			{ RDB_FIELD_ACCOUNT }, 
			{ RDB_FIELD_ACCOUNT_INIT_DB },
			//{ ROLESVC_FIELD_NAME_ACCOUNT_EXT },
		};
		// 设置字段值
		aInsertFieldsAccount[0].pvData    = pReq->szAccount;
		aInsertFieldsAccount[0].uDataSize = strlen(pReq->szAccount) + 1;

		aInsertFieldsAccount[1].pvData    = &pForwardReply->nDbIdentity;
		aInsertFieldsAccount[1].uDataSize = KGD_INT32_SIZE;

		//aInsertFieldsAccount[2].pvData    = "Test账户扩展数据";
		//aInsertFieldsAccount[2].uDataSize = strlen("Test账户扩展数据") + 1;

		nRet = m_piDatabase->Insert(
			RDB_TABLE_ACCOUNT, 
			countof(aInsertFieldsAccount), 
			aInsertFieldsAccount
			);
		LOG_PROCESS_ERROR(nRet);

		nRet = m_piDatabase->GetAffectedRow();
		LOG_PROCESS_ERROR(nRet == 1); // 成功写入数据库
	}

	m_uBytesSend += piRequestBuffer->GetSize();
	nRet = TRUE;
EXIT0:
	rcResult.u64CallerArg = rcRequest.u64CallerArg;
	rcResult.pAdditional = NULL;
	rcResult.pData = NULL;

	//if (pForwardReply && pReq)
	//{
	if (piReplyBuffer)
	{
		KRS2C_CREATE_ROLE_REPLY* pReplyPack = (KRS2C_CREATE_ROLE_REPLY*)piReplyBuffer->GetData();
		ASSERT(pReplyPack);
		pReplyPack->byProtocol = emKRS2C_CREATE_ROLE_REPLY;
		pReplyPack->byErrorCode = nRet ? emKRS2C_ERROR_SUCCESS : emKRS2C_ERROR_DATABASE;
		pReplyPack->uPackageId = pReq->uPackageId;
		pReplyPack->dwRoleId   = (DWORD)pForwardReply->u64RoleId;

		rcResult.pData = piReplyBuffer;
	}

	QLogPrintf(
		LOG_INFO, 
		"%s Create role [%s] for Account [%s] %s", 
		m_strWorkerName.c_str(),
		pReq->szRole,
		pReq->szAccount,
		nRet ? "Success" : "Error"
		);
	//}

	//QLogPrintf(LOG_DEBUG, "%s Leave %s()", m_strWorkerName.c_str(), __FUNCTION__);
	return nRet;
}

////////////////////////////////////////////////////////////////////////////////
INT KDbClusterWorker::_SaveRoleData(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult)
{
	INT nRet = FALSE;
	KGD_FIELD RoleNameField = { RDB_FIELD_ROLE };
	KGD_FIELD RoleIdField = { RDB_FIELD_ROLE_ID };
	//KGD_FIELD AccountField  = {KG_ACCOUNT_FIELD_NAME};
	CHAR szTime[KG_TIME_STRING_SIZE] = "";
	UINT64 llRoleId = 0;
	KGD_FIELD aUpdateFields[] = 
	{
		{RDB_FIELD_ROLE_PREVIEW_INFO  },
		{RDB_FIELD_ROLE_DATA  },
		{RDB_FIELD_ROLE_LAST_SAVE_TIME}
	};

	//QLogPrintf(LOG_DEBUG, "%s Enter %s()", m_strWorkerName.c_str(), __FUNCTION__);

	IMemBlock* piRequestBuffer = rcRequest.pData;
	assert(piRequestBuffer);
	KC2RS_SAVE_ROLE_DATA* pReq = (KC2RS_SAVE_ROLE_DATA*)piRequestBuffer->GetData();
	assert(pReq);

	nRet = g_GetLocalTimeString(szTime, sizeof(szTime));
	LOG_PROCESS_ERROR(nRet);

	RoleNameField.uDataSize = strlen(pReq->szRole) + 1;
	RoleNameField.pvData = pReq->szRole;
	llRoleId = (UINT64)pReq->dwRoleId;
	RoleIdField.uDataSize = sizeof(llRoleId);
	RoleIdField.pvData = &llRoleId;

	// -------------------- save to database --------------------  
	//QLogPrintf(LOG_DEBUG, "Saving RoleDataSize %d", pReq->uRoleDataSize);
	//printf("Data ");
	//for(int i = 0; i < pReq->uRoleDataSize; ++i)
	//	printf("%02x ", pReq->abyData[pReq->uRoleListSize + i]);
	aUpdateFields[0].uDataSize = pReq->uRoleListSize;
	aUpdateFields[0].pvData = pReq->abyData;

	aUpdateFields[1].uDataSize = pReq->uRoleDataSize;
	aUpdateFields[1].pvData = PVOID(pReq->abyData + pReq->uRoleListSize);

	aUpdateFields[2].uDataSize = strlen(szTime) + 1;
	aUpdateFields[2].pvData = szTime;

	nRet = m_piDatabase->Update(
		RDB_TABLE_ROLE, 
		countof(aUpdateFields), 
		aUpdateFields,
		&RoleIdField, 
		KGD_OPERATOR_EQUAL,
		&RoleNameField, 
		KGD_OPERATOR_EQUAL
		);
	LOG_PROCESS_ERROR(nRet);

	nRet = m_piDatabase->GetAffectedRow();
	LOG_PROCESS_ERROR(nRet == 1); // 成功写入数据库

	m_uBytesSend += piRequestBuffer->GetSize();
	nRet = TRUE;
EXIT0:
	rcResult.u64CallerArg = rcRequest.u64CallerArg;
	rcResult.pAdditional = NULL;
	rcResult.pData = NULL;

	// piReplyBuffer不在这里释放
	IMemBlock* piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KRS2C_PTC_HEADER));
	if (piReplyBuffer)
	{
		KRS2C_PTC_HEADER* pReplyPack = (KRS2C_PTC_HEADER*)piReplyBuffer->GetData();
		assert(pReplyPack);
		pReplyPack->byProtocol = emKRS2C_SAVE_ROLE_DATA_REPLY;
		pReplyPack->byErrorCode = nRet ? emKRS2C_ERROR_SUCCESS : emKRS2C_ERROR_DATABASE;
		pReplyPack->uPackageId = pReq->uPackageId;

		rcResult.pData = piReplyBuffer;
	}
	else
	{
		QLogPrintf(LOG_ERR, "Can't Alloc %d bytes", sizeof(KRS2C_PTC_HEADER));
	}

	QLogPrintf(LOG_INFO, 
		"%s Save role [%u] [%s] for account [%s] %s", 
		m_strWorkerName.c_str(), pReq->dwRoleId, pReq->szRole, pReq->szAccount,
		nRet ? "Success" : "Error"
		);
	return nRet;
}

////////////////////////////////////////////////////////////////////////////////
INT KDbClusterWorker::_DeleteRole(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult)
{
	int nResult  = false;
	int nRetCode = false;  
//	unsigned uRequestPacketSize  = 0;
//	TProcessData *pRequestPacket = NULL;
//	IKGD_Result *piResult        = NULL;
//	KGD_FIELD AccountField  = {KG_ACCOUNT_FIELD_NAME  };
//	KGD_FIELD RoleNameField = {KG_NAME_FIELD_NAME     };
//	char *pszRoleName = "";
//	char *pszAccountName = "";
//	unsigned long ulPacketID = ULONG_MAX;
//	ASSERT(piRequestPacket);  
//	QLogPrintf(LOG_DEBUG, "%s Enter %s()", m_szServerName, __FUNCTION__);
//
//	uRequestPacketSize = piRequestPacket->GetSize();
//	LOG_PROCESS_ERROR(uRequestPacketSize >= sizeof(TProcessData));
//	pRequestPacket = (TProcessData *)piRequestPacket->GetData();
//	LOG_PROCESS_ERROR(pRequestPacket);
//
//	ulPacketID = pRequestPacket->ulIdentity;
//
//	RoleNameField.pvData    = pRequestPacket->pDataBuffer;
//	RoleNameField.uDataSize = (unsigned)pRequestPacket->nDataLen;
//	LOG_PROCESS_ERROR(RoleNameField.uDataSize > 1);
//	LOG_PROCESS_ERROR(RoleNameField.uDataSize <= KG_NAME_MAX_SIZE);
//	pszRoleName = (char *)RoleNameField.pvData;
//
//	nRetCode = m_piDatabase->Query(
//		KG_ROLE_TABLE_NAME, 
//		1, 
//		&AccountField, 
//		&RoleNameField, 
//		KGD_OPERATOR_EQUAL,
//		NULL, 
//		KGD_OPERATOR_NONE
//		);
//	LOG_PROCESS_ERROR(nRetCode);
//
//	piResult = m_piDatabase->GetQueryResult(KGD_RESULT_STORE);
//	LOG_PROCESS_ERROR(piResult);
//
//	nRetCode = piResult->GetRowCount();
//	LOG_PROCESS_ERROR(nRetCode == 1);
//
//	nRetCode = piResult->NextRow();
//	LOG_PROCESS_ERROR(nRetCode);
//
//	nRetCode = piResult->GetCurrentRowField(0, &AccountField);
//	LOG_PROCESS_ERROR(nRetCode);
//
//	pszAccountName = (char *)AccountField.pvData;
//
//	nRetCode = m_piDatabase->Delete(
//		KG_ROLE_TABLE_NAME,
//		&RoleNameField,
//		KGD_OPERATOR_EQUAL,
//		NULL,
//		KGD_OPERATOR_NONE
//		);
//	LOG_PROCESS_ERROR(nRetCode);
//
//	nRetCode = m_piDatabase->GetAffectedRow();
//	LOG_PROCESS_ERROR(nRetCode == 1); // only one row affected
//
//	// Delete role name in NameServer
//	nRetCode = m_pGoddessServerManager->DeleteRoleName(
//		(char *)RoleNameField.pvData, RoleNameField.uDataSize
//		);
//	LOG_PROCESS_ERROR(nRetCode && "Fail to delete role name in NameServer");
//
//	// 角色删除事件
//	{
//		KGD_FIELD sField;
//		IKGD_Result* pResult = NULL;
//		if (_get_role_data(pszRoleName, &sField, &pResult))
//		{
//			ASSERT(pResult);
//			m_piRoleProcessor->OnRoleDeleted(sField.uDataSize, sField.pvData);
//			SAFE_RELEASE(pResult);
//		}
//	}
//
//	//EXIT1:
//	nResult = true;
//EXIT0:
//	QLogPrintf(
//		LOG_INFO, 
//		"%s PacketID : %lu, Delete role \"%s\" of Account \"%s\", Result = %d",
//		m_szServerName, 
//		ulPacketID,
//		pszRoleName,
//		pszAccountName,
//		nResult
//		);
//
//	SAFE_RELEASE(piResult);
//
//	if (uRequestPacketSize >= sizeof(TProcessData))
//	{
//		_SendSimpleReply(
//			GDS2C_DELETE_ROLE_RESULT, 
//			pRequestPacket->ulIdentity, 
//			(nResult) ? 1 : -1
//			);
//	}
//	QLogPrintf(LOG_DEBUG, "%s Leave %s()", m_szServerName, __FUNCTION__);
	return nResult;
}

INT KDbClusterWorker::_GetRoleData(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult)
{
	INT nRet = FALSE;
	IKGD_Result *piResult = NULL;
	IMemBlock *piReplyPacket    = NULL;
	IMemBlock* piRequestBuffer = rcRequest.pData;
	KGD_FIELD RoleNameField  = { RDB_FIELD_ROLE };
	KE_RS2C_ERROR_CODE eErrCode = emKRS2C_ERROR_DATABASE;
	KGD_FIELD aQueryResult[]  = 
	{
		{RDB_FIELD_ROLE_ID},
		{RDB_FIELD_ROLE_DATA},
	};

	assert(piRequestBuffer);
	//QLogPrintf(LOG_DEBUG, "%s Enter %s()", m_strWorkerName.c_str(), __FUNCTION__);

	KC2RS_GET_ROLE_DATA* pReq = (KC2RS_GET_ROLE_DATA*)piRequestBuffer->GetData();
	assert(pReq);

	RoleNameField.uDataSize = strlen(pReq->szRole) + 1;
	RoleNameField.pvData    = pReq->szRole;
	LOG_PROCESS_ERROR(RoleNameField.uDataSize > 1);
	LOG_PROCESS_ERROR(RoleNameField.uDataSize <= KD_MAX_NAME_LEN);

	eErrCode = emKRS2C_ERROR_DATABASE;
	// --------------------  query role by name -------------------- 
	nRet = m_piDatabase->Query(
		RDB_TABLE_ROLE, 
		sizeof(aQueryResult) / sizeof(aQueryResult[0]), 
		aQueryResult, 
		&RoleNameField, 
		KGD_OPERATOR_EQUAL,
		NULL, 
		KGD_OPERATOR_NONE
		);
	LOG_PROCESS_ERROR(nRet);

	piResult = m_piDatabase->GetQueryResult(KGD_RESULT_STORE);
	LOG_PROCESS_ERROR(piResult);

	nRet = piResult->GetRowCount();
	LOG_PROCESS_ERROR(nRet == 1);

	nRet = piResult->NextRow();
	LOG_PROCESS_ERROR(nRet);

	nRet = piResult->GetCurrentRowField(0, &aQueryResult[0]);
	LOG_PROCESS_ERROR(nRet);

	nRet = piResult->GetCurrentRowField(1, &aQueryResult[1]);
	LOG_PROCESS_ERROR(nRet);

	//QLogPrintf(LOG_DEBUG, "Loaded RoleDataSize %d", aQueryResult[1].uDataSize);
	//printf("Data ");
	//for(int i = 0; i < aQueryResult[0].uDataSize; ++i)
	//	printf("%02x ", ((BYTE*)aQueryResult[0].pvData)[i]);
//EXIT1:
	m_uBytesSend += piRequestBuffer->GetSize();
	m_uBytesRecv += aQueryResult[0].uDataSize + aQueryResult[1].uDataSize;
	nRet = TRUE;
EXIT0:
	SIZE_T uReplyPackSize = 0;
	IMemBlock* piReplyBuffer = NULL;	// piReplyBuffer不在这里释放
	rcResult.u64CallerArg = rcRequest.u64CallerArg;
	rcResult.pAdditional = NULL;
	rcResult.pData = NULL;

	if (nRet)
	{
		uReplyPackSize = sizeof(KRS2C_GET_ROLE_DATA) + aQueryResult[1].uDataSize;
		piReplyBuffer = RS_MEM_ALLOCATOR(uReplyPackSize);
		if (piReplyBuffer)
		{
			KRS2C_GET_ROLE_DATA* pReplyPack = (KRS2C_GET_ROLE_DATA*)piReplyBuffer->GetData();
			assert(pReplyPack);
			pReplyPack->byProtocol  = emKRS2C_GET_ROLE_DATA_REPLY;			
			pReplyPack->uPackageId  = pReq->uPackageId;
			pReplyPack->byErrorCode = emKRS2C_ERROR_SUCCESS;
			pReplyPack->dwRoleId  = (DWORD)(*((UINT64*)aQueryResult[0].pvData));
			pReplyPack->uDataSize = aQueryResult[1].uDataSize;
			memcpy(pReplyPack->abyData, aQueryResult[1].pvData, aQueryResult[1].uDataSize);
		}
	}
	else
	{
		uReplyPackSize = sizeof(KRS2C_PTC_HEADER);
		piReplyBuffer = RS_MEM_ALLOCATOR(uReplyPackSize);
		if (piReplyBuffer)
		{
			KRS2C_PTC_HEADER* pErrorPack = (KRS2C_PTC_HEADER*)piReplyBuffer->GetData();
			assert(pErrorPack);
			pErrorPack->byProtocol = emKRS2C_GET_ROLE_DATA_REPLY;
			pErrorPack->byErrorCode = eErrCode;
			pErrorPack->uPackageId = pReq->uPackageId;
		}
	}
	rcResult.pData = piReplyBuffer;

	QLogPrintf(LOG_INFO, 
		"%s Get role [%s] for account [%s] %s",
		m_strWorkerName.c_str(),
		pReq->szRole,
		pReq->szAccount,
		nRet ? "Success" : "Error"
		);
	SAFE_RELEASE(piResult);

	return nRet;
}

BOOL KDbClusterWorker::InitDatabase( const KROLESVC_DB_PARAM& rcParam )
{
	INT nRet = KGD_Init(NULL);
	LOG_PROCESS_ERROR(nRet);

	// connect to database
	m_piDatabase = KGD_Connect(
		KGD_ENGINE_DEFAULT, 
		rcParam.szServer,
		rcParam.nPort,
		rcParam.szUserName,
		rcParam.szPassword,
		NULL
		);
	LOG_PROCESS_ERROR(m_piDatabase);

	nRet = m_piDatabase->CreateDatabase(
		rcParam.szDatabase,  KGD_CREATE_IF_NOT_EXIST
		);
	LOG_PROCESS_ERROR(nRet);

	nRet = m_piDatabase->SetCurrentDatabase(rcParam.szDatabase);
	LOG_PROCESS_ERROR(nRet);

	// 建立Role表
	nRet = m_piDatabase->CreateTable(
		RDB_TABLE_ROLE, 
		countof(g_caRoleTableFields), 
		g_caRoleTableFields, 
		KGD_CREATE_IF_NOT_EXIST
		);
	LOG_PROCESS_ERROR(nRet);

	// 建立Account表
	nRet = m_piDatabase->CreateTable(
		RDB_TABLE_ACCOUNT, 
		countof(g_caAccountTableFields), 
		g_caAccountTableFields, 
		KGD_CREATE_IF_NOT_EXIST
		);
	LOG_PROCESS_ERROR(nRet);

	nRet = TRUE;
EXIT0:
	if ( ! nRet)
	{
		KGD_Uninit(NULL);
		SAFE_RELEASE(m_piDatabase);
	}
	return nRet;
}

BOOL KDbClusterWorker::PushWork( const KProcessingPackage& rcPack )
{
	m_cJobListLock.Lock();
	m_cJobList.insert(m_cJobList.end(), rcPack);
	m_cJobListLock.Unlock();

	return TRUE;
}

BOOL KDbClusterWorker::GetResult( KProcessingPackageList& rPacks )
{
	m_cResultListLock.Lock();
	rPacks.insert(rPacks.end(), m_cResultList.begin(), m_cResultList.end());
	m_cResultList.clear();
	m_cResultListLock.Unlock();

	return TRUE;
}

INT KDbClusterWorker::ProcessSingleJob(KProcessingPackage& rcRequest, KProcessingPackage& rcResult)
{
	//printf("%s Processing Job [%d]\n", m_strWorkerName.c_str().c_str(), rcPack.u64PackageId);
	ASSERT(rcRequest.pData);

//	SAFE_RELEASE(rcPack.pData);
//	IMemBlock* pBuffer = QCreateMemBlock(100000);
//	KProcessingPackage pack1;
//	pack1.pData = pBuffer;
//	m_cTempResultList.insert(m_cTempResultList.end(), pack1);
//return TRUE;

	IMemBlock* piBuffer = rcRequest.pData;
	KC2RS_HEADER* pReq = (KC2RS_HEADER*)piBuffer->GetData();
	QCONFIRM_RET_FALSE(pReq);

	if ( ! ms_afnProtocolFunction[pReq->byProtocol] )
	{
		QLogPrintf(
			LOG_WARNING,
			"%s encountered an unsupported protocol [%u] in pack [%u]", 
			m_strWorkerName.c_str(), 
			pReq->byProtocol,
			(UINT)pReq->uPackageId
		);
	}
	else
	{
		INT nRetCode = (this->*ms_afnProtocolFunction[pReq->byProtocol]) (rcRequest, rcResult);
		//QCONFIRM_RET_FALSE(nRetCode);
	}

	return TRUE;
}

BOOL KDbClusterWorker::TestWork( const KProcessingPackage& rPacks )
{

	return TRUE;
}
