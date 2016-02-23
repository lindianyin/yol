/* -------------------------------------------------------------------------
//	文件名		：	KG_NameServer.cpp
//	创建者		：	
//	创建时间	：	2010/5/22 22:20:00
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kg_nameserver.h"
#include "mysql/mysqld_error.h"
#include "protocol/roleserver_protocol.h"
#include "roledb_def.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

static const unsigned long MYSQL5_VERSION_ID = 50000; // MySQL 5 Version ID


// -------------------------------------------------------------------------

KG_NameServer::KG_NameServer()
: m_bRunning(FALSE),
m_uTotalJobs(0),
m_uTotalJobsOk(0),
m_uBytesSend(0)
{
	m_nRef = 0;
	m_pMysqlConn = NULL;
}

KG_NameServer::~KG_NameServer()
{

}

BOOL KG_NameServer::Init(const KROLESVC_DB_PARAM &crParam, const std::string& strName)
{
	int nResult  = false;
	QLogPrintf(LOG_INFO, "[NameServer] %s Init...", strName.c_str());

	nResult = _InitNameServerConnect(crParam);
	LOG_PROCESS_ERROR(nResult && "Init NameServer Connect Failed");

	nResult = CheckDataBaseVariable(m_pMysqlConn);
	LOG_PROCESS_ERROR(nResult && "Check Database Variable Failed");

	nResult = _CreateNameServerDatabase(crParam);
	LOG_PROCESS_ERROR(nResult && "Create NameServer Database Failed");

	ASSERT(m_pMysqlConn);

	m_bRunning = TRUE;
	nResult = m_WorkThread.Create(_WorkThreadFunction, this);
	LOG_PROCESS_ERROR(nResult && "Create Worker Thread Failed");

	m_strWorkerName = strName;
	nResult = true;
EXIT0:
	if ( ! nResult)
		_UnInitNameServerConnect();
	return nResult;
}

BOOL KG_NameServer::Uninit()
{
	QLogPrintf(LOG_INFO, "[NameServer] %s Uninit...", m_strWorkerName.c_str());
	m_bRunning = FALSE;
	m_WorkThread.Wait();
	_UnInitNameServerConnect();
	m_cJobListLock.Unlock();
	m_cResultListLock.Unlock();

	QLogPrintf(LOG_INFO, "[NameServer] %s statics: jobs %u/%u, %llu bytes sent",
		m_strWorkerName.c_str(), m_uTotalJobsOk, m_uTotalJobs, (UINT64)m_uBytesSend);

	for (KProcessingPackageList::iterator it = m_cResultList.begin(); it != m_cResultList.end(); ++it)
	{
		SAFE_RELEASE(it->pAdditional);
		SAFE_RELEASE(it->pData);
	}
	if (m_cResultList.size())
		QLogPrintf(LOG_INFO, "[NameServer] %s discarded %u results", m_strWorkerName.c_str(), m_cResultList.size());

	for (KProcessingPackageList::iterator it = m_cJobList.begin(); it != m_cJobList.end(); ++it)
	{
		SAFE_RELEASE(it->pAdditional);
		SAFE_RELEASE(it->pData);
	}
	if (m_cJobList.size())
		QLogPrintf(LOG_INFO, "[NameServer] %s discarded %u jobs", m_strWorkerName.c_str(), m_cJobList.size());

	Release();
	return true;
}

INT KG_NameServer::_ThreadFunction()
{
	while (m_bRunning)
	{
		INT nRetCode = ProcessJobQueue();
		QCONFIRM(nRetCode && "KG_NameServer::ProcessJobQueue failed");
	}

	QLogPrintf(LOG_INFO, "%s Stopped", m_strWorkerName.c_str());
	return TRUE;
}

void KG_NameServer::_WorkThreadFunction(void *pvParam)
{
	KG_NameServer *pThis = (KG_NameServer *)pvParam;

	ASSERT(pThis);

	pThis->_ThreadFunction();
}

INT KG_NameServer::ProcessJobQueue()
{
	INT nRetCode = TRUE;
	KProcessingPackageList cTempResultList; // 写入m_cResultList前先汇总
	KProcessingPackageList cJobList;

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
		//LOG_PROCESS_ERROR(nRetCode);
		if (nRetCode && sResult.pData)
			m_uTotalJobsOk++;
		if (sResult.pData)
			cTempResultList.push_back(sResult);
		SAFE_RELEASE(it->pData);
		SAFE_RELEASE(it->pAdditional);
	}

	if (cTempResultList.size() > 0) 
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

INT KG_NameServer::ProcessSingleJob(KProcessingPackage& rcRequest, KProcessingPackage& rcResult)
{
	//printf("%s Processing Job [%d]\n", m_strWorkerName.c_str().c_str(), rcPack.u64PackageId);

	BOOL bRet = FALSE;
	IMemBlock* piBuffer = rcRequest.pData;
	KC2RS_HEADER* pReq = (KC2RS_HEADER*)piBuffer->GetData();
	QCONFIRM_RET_FALSE(pReq);

	switch (pReq->byProtocol)
	{
	case emKC2RS_CREATE_ROLE:
		bRet = CreateRoleName(rcRequest, rcResult);
		break;
	default:
		QCONFIRM_RET_FALSE(FALSE);
		break;
	}

	return bRet;
}

INT KG_NameServer::DeleteRoleName(
								  const char cszRoleName[],   unsigned uRoleNameSize
								  )
{
	int nResult  = false;
	int nRetCode = false;
	//    char szEscapedRoleName[ESCAPE_STRING_SIZE];
	//    char szQuery[MAX_QUERY_SIZE];
	//
	//    ASSERT(m_pMysqlConn);
	//    
	//    LOG_PROCESS_ERROR(cszRoleName);
	//    LOG_PROCESS_ERROR(uRoleNameSize > 1);
	//    LOG_PROCESS_ERROR((uRoleNameSize - 1) * 2 < sizeof(szEscapedRoleName));
	//
	//    nRetCode = (int)mysql_real_escape_string(
	//        m_pMysqlConn, szEscapedRoleName, cszRoleName, uRoleNameSize - 1
	//    );
	//    LOG_PROCESS_ERROR(nRetCode > 0);
	//    szEscapedRoleName[nRetCode] = '\0';
	//
	//    nRetCode = snprintf(
	//        szQuery, sizeof(szQuery),
	//        "update rolename set ZoneName = concat('*', ZoneName) "
	//        "where (RoleName = '%s') and (ZoneName not like '*%%')",
	//        szEscapedRoleName
	//    );  // Add prefix '*' before ZoneName means delete.
	//    LOG_PROCESS_ERROR(nRetCode > 0);
	//    LOG_PROCESS_ERROR(nRetCode < sizeof(szQuery));
	//
	//    nRetCode = DoMysqlQuery(m_pMysqlConn, szQuery);
	//    LOG_PROCESS_ERROR(nRetCode);
	//
	//    nRetCode = (int)mysql_affected_rows(m_pMysqlConn);
	//    KG_PROCESS_MYSQL_ERROR((nRetCode == 1), m_pMysqlConn);
	//    
	//    nResult = true;
	//EXIT0:
	return nResult;
}

INT KG_NameServer::_InitNameServerConnect(const KROLESVC_DB_PARAM &crParam)
{
	INT nResult  = false;
	INT nRetCode = false;
	MYSQL *pRetCode = NULL;
	unsigned long ulMySQLClientVersionID = 0;

	m_pMysqlConn = mysql_init(NULL);
	LOG_PROCESS_ERROR(m_pMysqlConn && "[MySQL Error]:mysql_init() failed");

	// enable reconnect if MySQL client version > 5
	ulMySQLClientVersionID = mysql_get_client_version();
	if (ulMySQLClientVersionID >= MYSQL5_VERSION_ID)
	{
		my_bool bReconnectFalg = true;
		nRetCode = mysql_options(m_pMysqlConn, MYSQL_OPT_RECONNECT, &bReconnectFalg);
		KG_PROCESS_MYSQL_ERROR((nRetCode == 0), m_pMysqlConn);
	}

	pRetCode = mysql_real_connect(
		m_pMysqlConn,
		crParam.szServer,
		crParam.szUserName,
		crParam.szPassword,
		NULL,
		KG_DATABASE_PORT,
		NULL,
		CLIENT_FOUND_ROWS
		);
	KG_PROCESS_MYSQL_ERROR((pRetCode == m_pMysqlConn), m_pMysqlConn);

	nResult = true;
EXIT0:
	if (!nResult)
	{
		if (m_pMysqlConn)
		{
			mysql_close(m_pMysqlConn);
			m_pMysqlConn = NULL;
		}
	}
	return nResult;
}

INT KG_NameServer::_UnInitNameServerConnect()
{
	if (m_pMysqlConn)
	{
		mysql_close(m_pMysqlConn);
		m_pMysqlConn = NULL;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
INT KG_NameServer::_CreateNameServerDatabase(const KROLESVC_DB_PARAM &crParam)
{
	int nResult  = false;
	int nRetCode = false;
	char szQuery[MAX_QUERY_SIZE];

	ASSERT(m_pMysqlConn);
	LOG_PROCESS_ERROR(crParam.szDatabase[0]);

	snprintf(
		szQuery, sizeof(szQuery),
		"CREATE DATABASE IF NOT EXISTS %s",
		crParam.szDatabase
		);

	nRetCode = DoMysqlQuery(m_pMysqlConn, szQuery);
	LOG_PROCESS_ERROR(nRetCode);

	nRetCode = mysql_select_db(m_pMysqlConn, crParam.szDatabase);
	KG_PROCESS_MYSQL_ERROR((nRetCode == 0), m_pMysqlConn);

	nRetCode = DoMysqlQuery(m_pMysqlConn, ROLESVC_SQL_INIT_TABLE_ALL_ROLES);
	LOG_PROCESS_ERROR(nRetCode);

	nResult = true;
EXIT0:
	if (!nResult)
	{
		if (m_pMysqlConn)
		{
			mysql_close(m_pMysqlConn);
			m_pMysqlConn = NULL;
		}
	}
	return nResult;
}

///////////////////////////////////////////////////////////////////////////////
INT KG_NameServer::_InsertRoleName(LPCSTR pszAccoutName,
								   LPCSTR pszRoleName,
								   LPCSTR pszDatabaseName,
								   LPCSTR pszGateway,
								   UINT64& ru64RoleId,
								   INT& rnRetCode)
{
	INT nResult = FALSE;
	CHAR szEscapedRoleName[ESCAPE_STRING_SIZE];
	CHAR szEscapedAccountName[ESCAPE_STRING_SIZE];
	CHAR szDateTimeString[KG_TIME_STRING_SIZE];
	CHAR szQuery[MAX_QUERY_SIZE];
	size_t uQueryLen = 0;
	KE_RS2C_ERROR_CODE eCreateRoleResult = emKRS2C_ERROR_DATABASE;
	SIZE_T uRoleNameLen = 0;
	SIZE_T uAccountNameLen = 0;

	assert(pszAccoutName);
	assert(pszRoleName);
	assert(pszDatabaseName);

	uRoleNameLen = strlen(pszRoleName);
	LOG_PROCESS_ERROR(uRoleNameLen * 2 <= ESCAPE_STRING_SIZE);
	uAccountNameLen = strlen(pszAccoutName);
	LOG_PROCESS_ERROR(uAccountNameLen * 2 <= ESCAPE_STRING_SIZE);

	nResult = (INT)mysql_real_escape_string(
		m_pMysqlConn, szEscapedRoleName, pszRoleName, uRoleNameLen
		);
	LOG_PROCESS_ERROR(nResult > 0);
	szEscapedRoleName[nResult] = '\0';

	nResult = (INT)mysql_real_escape_string(
		m_pMysqlConn, szEscapedAccountName, pszAccoutName, uAccountNameLen
		);
	LOG_PROCESS_ERROR(nResult > 0);
	szEscapedAccountName[nResult] = '\0';

	g_GetLocalTimeString(szDateTimeString, sizeof(szDateTimeString));

	uQueryLen = snprintf(
		szQuery, sizeof(szQuery),
		"INSERT INTO %s (%s, %s, %s, %s, %s) VALUES ('%s', '%s', '%s', '%s', 'Initial DB %s')",
		RDB_TABLE_ALL_ROLES,
		RDB_FIELD_ROLE,
		RDB_FIELD_ACCOUNT,
		RDB_FIELD_ALL_ROLES_ZONE,
		RDB_FIELD_ALL_ROLES_CREATE_TIME,
		RDB_FIELD_ALL_ROLES_COMMENT,
		szEscapedRoleName, szEscapedAccountName, pszGateway, szDateTimeString, pszDatabaseName
		);
	LOG_PROCESS_ERROR(uQueryLen > 0);

	nResult = DoMysqlQuery(m_pMysqlConn, szQuery); 
	if (!nResult)
	{
		nResult = mysql_errno(m_pMysqlConn);

		if (nResult == ER_DUP_ENTRY)    // Role name existed
			eCreateRoleResult = emKRS2C_ERROR_NAME_EXISTS;

		//KG_PROCESS_MYSQL_ERROR(false, m_pMysqlConn);
		goto EXIT0;
	}

	ru64RoleId = mysql_insert_id(m_pMysqlConn);
	eCreateRoleResult = emKRS2C_ERROR_SUCCESS;
	m_uBytesSend += uQueryLen;
	nResult = TRUE;
EXIT0:
	QLogPrintf(LOG_INFO,
		"Insert role %llu [%s] for account [%s], code [%d] %s",
		ru64RoleId,
		pszRoleName,
		pszAccoutName,
		eCreateRoleResult,
		eCreateRoleResult == emKRS2C_ERROR_SUCCESS ? "Success" : "Error"
		);

	rnRetCode = eCreateRoleResult;
	return nResult;
}

BOOL KG_NameServer::PushWork( const KProcessingPackage& rPack )
{
	m_cJobListLock.Lock();
	m_cJobList.insert(m_cJobList.end(), rPack);
	m_cJobListLock.Unlock();

	return TRUE;
}

BOOL KG_NameServer::GetResult( KProcessingPackageList& rPacks )
{
	m_cResultListLock.Lock();
	rPacks.insert(rPacks.end(), m_cResultList.begin(), m_cResultList.end());
	m_cResultList.clear();
	m_cResultListLock.Unlock();

	return TRUE;
}

BOOL KG_NameServer::CreateRoleName(KProcessingPackage& rcRequest, KProcessingPackage& rcResult)
{
	// rcPack.pData是网络包
	IMemBlock* piBuffer = rcRequest.pData;
	assert(piBuffer);
	KC2RS_CREATE_ROLE* pReq = (KC2RS_CREATE_ROLE*)piBuffer->GetData();
	assert(pReq);
	// rcPack.pAdditional是GatewayProcess传入的额外参数
	IMemBlock* piAdditionalBuffer = (IMemBlock*)rcRequest.pAdditional;
	assert(piAdditionalBuffer);
	KRequest_CreateRoleName* pAdditional = (KRequest_CreateRoleName*)piAdditionalBuffer->GetData();
	assert(pAdditional);

	// 回复包
	IMemBlock* piReplyBuffer = RS_MEM_ALLOCATOR(sizeof(KResponse_CreateRoleName));
	QCONFIRM_RET_FALSE(piReplyBuffer);
	KResponse_CreateRoleName* pReply = (KResponse_CreateRoleName*)piReplyBuffer->GetData();
	assert(pReply);

	KE_RS2C_ERROR_CODE eCode = emKRS2C_ERROR_SUCCESS;
	UINT64 u64RoleId;
	BOOL bRet = _InsertRoleName(pReq->szAccount,
		pReq->szRole, 
		pAdditional->szDbName,
		pAdditional->szGateway, 
		u64RoleId,
		(INT&)eCode);

	rcResult = rcRequest;
	rcRequest.pData->AddRef();
	rcResult.pAdditional = rcRequest.pData; // 把KRequest_CreateRole包作为附件返还
	rcResult.pData = piReplyBuffer;
	pReply->nDbIdentity = pAdditional->nDbIdentity;
	pReply->bNewAccount = pAdditional->bNewAccount;
	pReply->nErrorCode = eCode;
	pReply->u64RoleId = u64RoleId;

	return TRUE;
}

KG_NameServer* KG_NameServer::CreateInstance()
{
	return new KG_NameServer;
}

INT KG_NameServer::Release()
{
	if (--m_nRef <= 0)
		delete this;
	return TRUE;
}
