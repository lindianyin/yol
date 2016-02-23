////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG_LogThread.cpp
//  Version     : 1.0
//  Creator     : Wu Caizhong
//  Create Date : 2008-6-12 22:04:22
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG_LogThread.h"

using namespace std;
static const unsigned long MYSQL5_VERSION_ID = 50000; // MySQL 5 Version ID

////////////////////////////////////////////////////////////////////////////
// @brief : Get a string's size in a buffer, limited scan range.
// @param uBufferSize : [in] string buffer max size.
// @param pszStr      : [in] point to a NULL terminal string.
// @param puSize      : [out] string size, include terminal NULL, 
//                            if string's size > uBufferSize,
//                            it will be set to uBufferSize
// @return : true if successful, false if string length > uMaxSize.
static inline int _KG_GetStringSize(
    unsigned uBufferSize, const char *pszStr, unsigned *puSize
)
{
    int nResult = false;
    const char *cpchBegin = NULL;
    const char *cpchEnd   = NULL;

    ASSERT(uBufferSize > 0);
    ASSERT(pszStr);
    ASSERT(puSize);

    cpchBegin = pszStr;
    cpchEnd   = cpchBegin + uBufferSize;

    while (pszStr != cpchEnd)
    {
        PROCESS_SUCCESS(*pszStr == '\0'); 
        ++pszStr;
    }
    PROCESS_ERROR(false);

EXIT1:
    *puSize = (unsigned)(pszStr - cpchBegin) + 1;
    nResult = true;
EXIT0:
    if (!nResult)
    {
        *puSize = uBufferSize;
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
KG_LogThread::KG_LogThread(void):
    m_nExitFlag(false),
    m_nActiveFlag(false),
    m_pRequestQueue(NULL),
    m_pResponseQueue(NULL),
    m_nRequestPerSecond(0),
    m_fRequestConsumeTime(0.0),
    m_pMysqlConn(NULL)
{
    memset(&m_aProtocolProcessFunction, 0, sizeof(m_aProtocolProcessFunction));

    m_aProtocolProcessFunction[LOGC2S_PING]               = &KG_LogThread::_ping;
    m_aProtocolProcessFunction[LOGC2S_LOG_LOGIN]          = &KG_LogThread::_login;
    m_aProtocolProcessFunction[LOGC2S_LOG_ITEM_OPERATION] = &KG_LogThread::_log_item_operation;
    m_aProtocolProcessFunction[LOGC2S_LOG_PLAYER_ACTION]  = &KG_LogThread::_log_player_action;
	m_aProtocolProcessFunction[LOGC2S_LOG_DATA_ANALYSIS]  = &KG_LogThread::_log_data_analysis;

    m_szLogItemTableName[0]   = '\0';
    m_szLogActionTableName[0] = '\0';
	m_szLogDataAnalysisTableName[0] = '\0';

    m_tmLastSwitchTable.tm_year = 0;
    m_tmLastSwitchTable.tm_mon  = 0;
    m_tmLastSwitchTable.tm_mday = 0;
	m_nFailureCount = 0;
}

KG_LogThread::~KG_LogThread(void)
{
    m_GatewayMap.clear();
    m_RoleIndexMap.clear();
    ASSERT(!m_pMysqlConn);
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::Init(
    const KG_LOG_THREAD_PARAM &crParam,
    KG_PACKAGE_LOCK_QUEUE     *pRequestQueue,
    KG_PACKAGE_LOCK_QUEUE     *pResponseQueue
)
{
    int nResult  = false;
    int nRetCode = false;
    int nConnectFlag = false;
    int nPrepareStmtInitFlag = false;
    int nWorkThreadInitFlag = false;

    LOG_PROCESS_ERROR(pRequestQueue);
    LOG_PROCESS_ERROR(pResponseQueue);

    m_Param          = crParam;
    m_pRequestQueue  = pRequestQueue;
    m_pResponseQueue = pResponseQueue;

    nRetCode = _Connect();
    LOG_PROCESS_ERROR(nRetCode && "[Log] Connect to LogServer Failed");
    nConnectFlag = true;

    nRetCode = _CreateLogDatabase();
    LOG_PROCESS_ERROR(nRetCode && "[Log] Create Log Database Failed");

    nRetCode = m_WorkThread.Create(_WorkThreadFunction, this);
    LOG_PROCESS_ERROR(nRetCode);
    nWorkThreadInitFlag = true;

    nResult = true;
EXIT0:
    if (!nResult)
    {
        if (nWorkThreadInitFlag)
        {
            m_WorkThread.Wait();
            nWorkThreadInitFlag = false;
        }
        if (nConnectFlag)
        {
            _DisConnect();
            nConnectFlag = false;
        }
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::UnInit()
{
    int nResult  = false;
    int nRetCode = false;

    SetExitFlag(true);

    m_WorkThread.Wait();
    _DisConnect();

    nResult = true;
//EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::GetStatus(std::string *psRetStatus)
{
    int nResult  = false;
    int nRetCode = false;
    char szBuffer[256];
    double fMaxRequestPerSecond = 0;
    double fLoadCondition = 0;

    LOG_PROCESS_ERROR(psRetStatus);

    if (m_nRequestPerSecond == 0)
        fMaxRequestPerSecond = 0;
    else
        fMaxRequestPerSecond = (double)m_nRequestPerSecond / m_fRequestConsumeTime;

    if ((int)(m_fRequestConsumeTime * 100) >= 100)
        fLoadCondition = 100;
    else
        fLoadCondition = m_fRequestConsumeTime * 100; // m_nRequestPerSecond / fMaxRequestPerSecond * 100

    nRetCode = snprintf(
        szBuffer,
        sizeof(szBuffer) - 1,
        "Cache RoleIndexMap Size : %u\n"
		"Database Load Condition : %d/%.0f (%.2f%%)\n"
		"Failure : %d\n",
        (unsigned)m_RoleIndexMap.size(),
        m_nRequestPerSecond,
        fMaxRequestPerSecond,
        fLoadCondition,
		m_nFailureCount
    );
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szBuffer) - 1));
    szBuffer[sizeof(szBuffer) - 1] = '\0';

    *psRetStatus = szBuffer;

    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
void KG_LogThread::_WorkThreadFunction(void *pvParam)
{
    KG_LogThread *pThis = (KG_LogThread *)pvParam;

    ASSERT(pThis);

    pThis->_ThreadFunction();
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_ThreadFunction()
{
    int nResult  = false;
    int nRetCode = false;
    unsigned long ulLastResetTime = 0;
    unsigned long ulCurrentTime   = 0;
    double fTimeConsumePerRequest = 0;

    m_fRequestConsumeTime = 0;
    m_nRequestPerSecond = 0;

    ulLastResetTime = GetTickCount();

    m_nActiveFlag = true;
    while (!m_nExitFlag)
    {
        ulCurrentTime = GetTickCount();

        if ((ulCurrentTime - ulLastResetTime) >= 1000)
        {
            m_fRequestConsumeTime = 0;
            m_nRequestPerSecond = 0;
            ulLastResetTime = ulCurrentTime;
        }

        if (m_pRequestQueue->Queue.empty())
        {
            QThread_Sleep(10);
        }

        nRetCode = _SwitchTable();
        //LOG_PROCESS_ERROR(nRetCode && "[Log] Update log View Failed");

        nRetCode = m_MicroTimer.Start();
        //PROCESS_ERROR(nRetCode);

        nRetCode = _ProcessRequest();
        //LOG_PROCESS_ERROR(nRetCode);

        nRetCode = m_MicroTimer.Stop();
        //PROCESS_ERROR(nRetCode);

        nRetCode = m_MicroTimer.TimerDiff(&fTimeConsumePerRequest);
        //PROCESS_ERROR(nRetCode);
        m_fRequestConsumeTime += fTimeConsumePerRequest;
    }

    nResult = true;
//EXIT0:
    m_nActiveFlag = false;
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_ProcessRequest()
{
    int nResult  = false;
    int nRetCode = false;
    int nRequestQueueLockFlag = false;
    unsigned char *pbyRequest = NULL;
    unsigned char chProtocal = 0;
    unsigned uRequestSize = 0;
    IMemBlock  *piResponse = NULL;
    unsigned uRequestQueueSize = 0;
    KG_PACKAGE Request;
    KG_PACKAGE Response;
    string *psGatewayName = NULL;
    KG_GATEWAY_INFO_MAP::iterator itGateway;
    std::pair<KG_GATEWAY_INFO_MAP::iterator, bool> pairInsertResult;

    // ------------------------ get request -----------------------------
    Request.uConnectionID = 0;
    Request.piDataBuffer  = NULL;

    m_pRequestQueue->Lock.Lock();
    nRequestQueueLockFlag = true;

    uRequestQueueSize = (unsigned)m_pRequestQueue->Queue.size();
    PROCESS_SUCCESS(uRequestQueueSize == 0);

    ++m_nRequestPerSecond;
 
    Request = m_pRequestQueue->Queue.front();
    m_pRequestQueue->Queue.pop_front();

    m_pRequestQueue->Lock.Unlock();
    nRequestQueueLockFlag = false;

    // ------------------------ process it ------------------------------
    uRequestSize = Request.piDataBuffer->GetSize();
    LOG_PROCESS_ERROR(uRequestSize > 0);

    pbyRequest = (unsigned char *)Request.piDataBuffer->GetData();
    LOG_PROCESS_ERROR(pbyRequest);

    chProtocal = *pbyRequest;
    
    if (!m_aProtocolProcessFunction[chProtocal])
    {
        QLogPrintf(LOG_ERR, "[Log] Unsupported protocol: %u", chProtocal);
        PROCESS_ERROR(false);
    }

    itGateway = m_GatewayMap.find(Request.uConnectionID);
    if (itGateway == m_GatewayMap.end())
    {
        pairInsertResult = m_GatewayMap.insert(
            make_pair(Request.uConnectionID, "")
        );
        LOG_PROCESS_ERROR(pairInsertResult.second);
        psGatewayName = &(pairInsertResult.first->second);
    }
    else
    {
        psGatewayName = &(itGateway->second);
    }
    ASSERT(psGatewayName);

    nRetCode = (this->*m_aProtocolProcessFunction[chProtocal])(
       psGatewayName, &Request, &piResponse
    );
    //LOG_PROCESS_ERROR(nRetCode);

    // ----------------------- put response -----------------------------
    ASSERT(piResponse);
    Response.uConnectionID = Request.uConnectionID;
    Response.piDataBuffer  = piResponse;
    Response.piDataBuffer->AddRef();

    m_pResponseQueue->Lock.Lock();
    m_pResponseQueue->Queue.push_back(Response);
    m_pResponseQueue->Lock.Unlock();

EXIT1:
    nResult = true;
EXIT0:
    SAFE_RELEASE(Request.piDataBuffer);
    SAFE_RELEASE(piResponse);

    if (nRequestQueueLockFlag)
    {
        m_pRequestQueue->Lock.Unlock();
        nRequestQueueLockFlag = false;
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_ping(
    string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse
)
{
    int nResult  = false;
    int nRetCode = false;

    ASSERT(psGatewayName);
    ASSERT(pPackage);
    ASSERT(ppiRetResponse);

    // LOG_PROCESS_ERROR((!psGatewayName->empty()) && "[Log] Must login first");

    *ppiRetResponse = pPackage->piDataBuffer;
    (*ppiRetResponse)->AddRef();

    nResult = true;
EXIT0:
    /*QLogPrintf(
        LOG_INFO, "[%u] Gateway \"%s\" Ping, Result = %d", 
        pPackage->uConnectionID, psGatewayName->c_str(), nResult
    );*/
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_login(
    string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse
)
{
    int nResult  = false;
    int nRetCode = false;
    unsigned uRequestPacketSize = 0;
    unsigned uGatewayNameSize   = 0;
    LOGSERVER_LOGIN *pRequestPacket = NULL;
    char *pszGatewayName = "";
    char szGatewayEscape[128];
    unsigned long ulGatewayEscapeLen = 0;
    unsigned long ulPacketID = ULONG_MAX;

    ASSERT(psGatewayName);
    ASSERT(pPackage);
    ASSERT(ppiRetResponse);

    uRequestPacketSize = pPackage->piDataBuffer->GetSize();
    LOG_PROCESS_ERROR(uRequestPacketSize >= sizeof(LOGSERVER_LOGIN));
    pRequestPacket = (LOGSERVER_LOGIN *)pPackage->piDataBuffer->GetData();
    ASSERT(pRequestPacket);

    ulPacketID = pRequestPacket->ulIdentity;

    LOG_PROCESS_ERROR(psGatewayName->empty() && "Already login!");

    nRetCode = _KG_GetStringSize(
        KG_NAME_MAX_SIZE, pRequestPacket->szGatewayName, &uGatewayNameSize
    );
    LOG_PROCESS_ERROR(nRetCode && "[Log] GatewayName Name too long");
    LOG_PROCESS_ERROR((uGatewayNameSize > 1) && "[Log] GatewayName Name is empty");
    pszGatewayName = pRequestPacket->szGatewayName;

    ulGatewayEscapeLen = mysql_real_escape_string(
        m_pMysqlConn, szGatewayEscape, pszGatewayName, uGatewayNameSize - 1
    );
    MYSQL_PROCESS_ERROR((ulGatewayEscapeLen >= uGatewayNameSize - 1), m_pMysqlConn);

    *psGatewayName = szGatewayEscape;

    nResult = true;
EXIT0:
    QLogPrintf(
        LOG_INFO, 
        "[%u] PacketID:%lu, Gateway \"%s\" Login, Result = %d",
        pPackage->uConnectionID, ulPacketID, pszGatewayName, nResult
    );

    _SetResponsePacket(
        LOGC2S_LOG_LOGIN_RESULT, ulPacketID, nResult, ppiRetResponse
    );
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_log_item_operation(
    string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse
)
{
    int nResult  = false;
    int nRetCode = false;
    LOG_RETURN_CODE eLogReturnCode = LOG_RETURN_FAILED;
    unsigned uRequestPacketSize  = 0;
    KGItemLog *pRequestPacket = NULL;
    char *pszAccountName = "";
    unsigned uAccountNameSize = 0;
    char *pszRoleName = "";
    unsigned uRoleNameSize = 0;
    char *pszItemName = NULL;
    unsigned uItemNameSize = 0;
    char *pszRestoreData = NULL;
    unsigned uRestoreDataSize = 0;
    char *pszComment = NULL;
    unsigned uCommentSize = 0;
    unsigned long ulPacketID = ULONG_MAX;
    unsigned uDateSize = 0;
    IMemBlock *piSqlBuffer = NULL;
    char *pszSqlBuffer = NULL;
    unsigned uBufferSize = 0;
    unsigned long ulSqlLen = 0;
    IMemBlock *piRestoreData = NULL;
    char *pszRestoreDataEscape = NULL;
    unsigned long ulRestoreDataEscape = 0;
    IMemBlock *piComment = NULL;
    char *pszCommentEscape = NULL;
    unsigned long ulCommentEscape = 0;
    char szItemNameEscape[128];
    unsigned long ulItemNameEscape = 0;
    unsigned uRoleIndexID = 0;
    time_t uCurTime = 0;
    struct tm tmNow = { 0 };

    ASSERT(psGatewayName);
    ASSERT(pPackage);
    ASSERT(ppiRetResponse);

    LOG_PROCESS_ERROR((!psGatewayName->empty()) && "[Log] Must login first");

    uRequestPacketSize = pPackage->piDataBuffer->GetSize();
    LOG_PROCESS_ERROR(uRequestPacketSize >= sizeof(KGItemLog));
    pRequestPacket = (KGItemLog *)pPackage->piDataBuffer->GetData();
    ASSERT(pRequestPacket);

    ulPacketID = pRequestPacket->ulIdentity;

    uRestoreDataSize = pRequestPacket->uItemRestoreDataSize;
    LOG_PROCESS_ERROR(
        (uRestoreDataSize > 1) &&  "[Log] Item RestoreData is empty"
    );
    LOG_PROCESS_ERROR(
        (uRestoreDataSize <= LOG_ITEM_RESTORE_DATA_MAX_SIZE) && 
        "[Log] Item RestoreData too long"
    );

    uCommentSize = pRequestPacket->uCommentSize;
    LOG_PROCESS_ERROR(
        (uCommentSize <= LOG_COMMENT_MAX_SIZE) && 
        "[Log] Item Comment Data too long"
    );

    uDateSize = sizeof(KGItemLog) - 1 + uRestoreDataSize + uCommentSize;
    LOG_PROCESS_ERROR(uDateSize == uRequestPacketSize);

    LOG_PROCESS_ERROR(
        (pRequestPacket->eOperationType >= ITEM_OPERATION_TYPE_IN) && 
        (pRequestPacket->eOperationType < ITEM_OPERATION_TYPE_COUNT)
    );

    if (pRequestPacket->uItemValue < m_Param.uMinItemValue)
    {
        eLogReturnCode = LOG_RETURN_FILTER;
        PROCESS_ERROR(false);
    }

    nRetCode = _KG_GetStringSize(
        KG_NAME_MAX_SIZE, pRequestPacket->szAccountName, &uAccountNameSize
    );
    LOG_PROCESS_ERROR(nRetCode && "[Log] Account Name too long");
    LOG_PROCESS_ERROR((uAccountNameSize > 1) && "[Log] Account Name is empty");
    pszAccountName = pRequestPacket->szAccountName;

    nRetCode = _KG_GetStringSize(
        KG_NAME_MAX_SIZE, pRequestPacket->szRoleName, &uRoleNameSize
    );
    LOG_PROCESS_ERROR(nRetCode && "[Log] Role Name too long");
    LOG_PROCESS_ERROR((uRoleNameSize > 1) && "[Log] Role Name is empty");
    pszRoleName = pRequestPacket->szRoleName;

    nRetCode = _KG_GetStringSize(
        KG_NAME_MAX_SIZE, pRequestPacket->szItemName, &uItemNameSize
    );
    LOG_PROCESS_ERROR(nRetCode && "[Log] Item Name too long");
    LOG_PROCESS_ERROR((uItemNameSize > 1) && "[Log] Item Name is empty");
    pszItemName = pRequestPacket->szItemName;

    pszRestoreData = (char *)pRequestPacket->pbyData;
    ASSERT(pszRestoreData[uRestoreDataSize - 1] == '\0');
#ifdef _DEBUG
    nRetCode = (int)strlen(pszRestoreData) + 1;
    LOG_PROCESS_ERROR(
        (nRetCode == uRestoreDataSize) && 
        "[Log] ItemRestoreData is Invalid String"
    );
#endif

    if (uCommentSize > 0)
    {
        pszComment = (char *)pRequestPacket->pbyData + uRestoreDataSize;
        ASSERT(pszComment[uCommentSize - 1] == '\0');
    }

    nRetCode = _GetRoleIndexID(
        pszRoleName, uRoleNameSize,
        pszAccountName, uAccountNameSize,
        psGatewayName, &uRoleIndexID
    );
    LOG_PROCESS_ERROR(nRetCode && "GetRoleIndexID() Failed");
    LOG_PROCESS_ERROR(uRoleIndexID > 0);

    ulItemNameEscape = mysql_real_escape_string(
        m_pMysqlConn, szItemNameEscape, pszItemName, uItemNameSize - 1
    );
    MYSQL_PROCESS_ERROR((ulItemNameEscape >= uItemNameSize - 1), m_pMysqlConn);

    piRestoreData = QCreateMemBlock(uRestoreDataSize * 2);
    LOG_PROCESS_ERROR(piRestoreData);

    pszRestoreDataEscape = (char *)piRestoreData->GetData();
    ASSERT(pszRestoreDataEscape);

    ulRestoreDataEscape = mysql_real_escape_string(
        m_pMysqlConn, pszRestoreDataEscape, pszRestoreData, uRestoreDataSize - 1
    );
    MYSQL_PROCESS_ERROR((ulRestoreDataEscape >= uRestoreDataSize - 1), m_pMysqlConn);

    if (uCommentSize > 0)
    {
        piComment = QCreateMemBlock(uCommentSize * 2);
        LOG_PROCESS_ERROR(piComment);

        pszCommentEscape = (char *)piComment->GetData();
        ASSERT(pszCommentEscape);

        ulCommentEscape = mysql_real_escape_string(
            m_pMysqlConn, pszCommentEscape, pszComment, uCommentSize - 1
        );
        MYSQL_PROCESS_ERROR((ulCommentEscape >= uCommentSize - 1), m_pMysqlConn);
    }

    piSqlBuffer = QCreateMemBlock(1024 * 8);
    LOG_PROCESS_ERROR(piSqlBuffer);

    pszSqlBuffer = (char *)piSqlBuffer->GetData();
    ASSERT(pszSqlBuffer);
    uBufferSize = piSqlBuffer->GetSize();
    ASSERT(uBufferSize > 0);

    uCurTime = time(NULL);
    localtime_r(&uCurTime, &tmNow);

    nRetCode = snprintf(
        pszSqlBuffer,
        uBufferSize - 1,
        "Insert into %s"
        "   (OperationTime, OperationType, RoleIndexID, ItemID, ItemName, ItemValue, ItemRestoreData, Comment)"
        "values"
        "   ('%.4d-%.2d-%.2d %.2d:%.2d:%.2d', %u, %u, %llu, '%s', %u, '%s', '%s')",
        m_szLogItemTableName,
        tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday, 
        tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec, pRequestPacket->eOperationType, 
        uRoleIndexID, pRequestPacket->ullItemID, szItemNameEscape, 
        pRequestPacket->uItemValue, pszRestoreDataEscape, pszCommentEscape
    );
    ASSERT((nRetCode > 0) && (nRetCode <= (int)uBufferSize - 1));
    pszSqlBuffer[uBufferSize - 1] = '\0';
    ulSqlLen = (unsigned long)nRetCode;
    
    nRetCode = DoMysqlQuery(m_pMysqlConn, pszSqlBuffer, ulSqlLen);
    LOG_PROCESS_ERROR(nRetCode);

    eLogReturnCode = LOG_RETURN_SUCCESS;

    nResult = true;
EXIT0:
    SAFE_RELEASE(piRestoreData);
    SAFE_RELEASE(piComment);
    SAFE_RELEASE(piSqlBuffer);

    QLogPrintf(
        LOG_INFO, 
        "[%u] PacketID:%lu, Log Item operation on role \"%s\" of Account \"%s\", Result = %d",
        pPackage->uConnectionID, ulPacketID, pszRoleName, pszAccountName, eLogReturnCode
    );

	if (eLogReturnCode != LOG_RETURN_SUCCESS)
		m_nFailureCount++;

    _SetResponsePacket(
       LOGS2C_LOG_ITEM_OPERATION_RESULT, ulPacketID, eLogReturnCode, ppiRetResponse
    );
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_log_player_action(
    string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse
)
{
    int nResult  = false;
    int nRetCode = false;
    LOG_RETURN_CODE eLogReturnCode = LOG_RETURN_FAILED;
    KGPlayerActionLog *pRequestPacket = NULL;
    unsigned uRequestPacketSize = 0;
    char *pszAccountName = "";
    unsigned uAccountNameSize = 0;
    char *pszRoleName = "";
    unsigned uRoleNameSize = 0;
    char *pszActionTarget = NULL;
    unsigned uActionTargetSize = 0;
    char *pszComment = NULL;
    unsigned uCommentSize = 0;
    unsigned long ulPacketID = ULONG_MAX;
    unsigned uDateSize = 0;
    IMemBlock *piSqlBuffer = NULL;
    unsigned uBufferSize = 0;
    char *pszSqlBuffer = NULL;
    unsigned long ulSqlLen = 0;
    IMemBlock *piComment = NULL;
    char *pszCommentEscape = NULL;
    unsigned long ulCommentEscape = 0;
    char szActionTargetEscape[128];
    unsigned long ulActionTargetEscape = 0;
    unsigned uRoleIndexID = 0;
    time_t uCurTime = 0;
    struct tm tmNow = { 0 };

    ASSERT(psGatewayName);
    ASSERT(pPackage);
    ASSERT(ppiRetResponse);

    LOG_PROCESS_ERROR((!psGatewayName->empty()) && "[Log] Must login first");

    uRequestPacketSize = pPackage->piDataBuffer->GetSize();
    LOG_PROCESS_ERROR(uRequestPacketSize >= sizeof(KGPlayerActionLog));
    pRequestPacket = (KGPlayerActionLog *)pPackage->piDataBuffer->GetData();
    LOG_PROCESS_ERROR(pRequestPacket);

    ulPacketID = pRequestPacket->ulIdentity;

    uCommentSize = pRequestPacket->uCommentSize;
    LOG_PROCESS_ERROR(
        (uCommentSize <= LOG_COMMENT_MAX_SIZE) && 
        "[Log] PlayerAction Comment Data too long"
    );

    if (uCommentSize > 0)
    {
        uDateSize = sizeof(KGPlayerActionLog) - 1 + uCommentSize;
        LOG_PROCESS_ERROR(uDateSize == uRequestPacketSize);
    }

    LOG_PROCESS_ERROR(
        (pRequestPacket->eActionType >= PLAYER_ACTION_PLAYER_LEVELUP) &&
        (pRequestPacket->eActionType < PLAYER_ACTION_TYPE_COUNT)
    );

    if (pRequestPacket->uActionLevel < m_Param.uMinPlayerActionLevel)
    {
        eLogReturnCode = LOG_RETURN_FILTER;
        PROCESS_ERROR(false);
    }

    nRetCode = _KG_GetStringSize(
        KG_NAME_MAX_SIZE, pRequestPacket->szActionTarget, &uActionTargetSize
    );
    LOG_PROCESS_ERROR(nRetCode && "[Log] Action Target Name too long");
    LOG_PROCESS_ERROR((uActionTargetSize > 1) && "[Log] Target Name is empty");
    pszActionTarget = pRequestPacket->szActionTarget;

    nRetCode = _KG_GetStringSize(
        KG_NAME_MAX_SIZE, pRequestPacket->szAccountName, &uAccountNameSize
    );
    LOG_PROCESS_ERROR(nRetCode && "[Log] Account Name too long");
    LOG_PROCESS_ERROR((uAccountNameSize > 1) && "[Log] Account Name is empty");
    pszAccountName = pRequestPacket->szAccountName;

    nRetCode = _KG_GetStringSize(
        KG_NAME_MAX_SIZE, pRequestPacket->szRoleName, &uRoleNameSize
    );
    LOG_PROCESS_ERROR(nRetCode && "[Log] Role Name too long");
    LOG_PROCESS_ERROR((uRoleNameSize > 1) && "[Log] Role Name is empty");
    pszRoleName = pRequestPacket->szRoleName;

    if (uCommentSize > 0)
    {
        pszComment = (char *)pRequestPacket->pbyData;
        ASSERT(pszComment[uCommentSize - 1] == '\0');
    }

    nRetCode = _GetRoleIndexID(
        pszRoleName, uRoleNameSize,
        pszAccountName, uAccountNameSize,
        psGatewayName, &uRoleIndexID
    );
    LOG_PROCESS_ERROR(nRetCode && "_GetRoleIndexID() Failed");
    LOG_PROCESS_ERROR(uRoleIndexID > 0);

    ulActionTargetEscape = mysql_real_escape_string(
        m_pMysqlConn, szActionTargetEscape, pszActionTarget, uActionTargetSize - 1
    );
    MYSQL_PROCESS_ERROR((ulActionTargetEscape >= uActionTargetSize - 1), m_pMysqlConn);

    if (uCommentSize > 0)
    {
        piComment = QCreateMemBlock(uCommentSize * 2);
        LOG_PROCESS_ERROR(piComment);

        pszCommentEscape = (char *)piComment->GetData();
        ASSERT(pszCommentEscape);
        uBufferSize = piComment->GetSize();
        ASSERT(uBufferSize > 0);

        ulCommentEscape = mysql_real_escape_string(
            m_pMysqlConn, pszCommentEscape, pszComment, uCommentSize - 1
        );
        MYSQL_PROCESS_ERROR((ulCommentEscape >= uCommentSize - 1), m_pMysqlConn);
    }

    piSqlBuffer = QCreateMemBlock(1024 * 4);
    LOG_PROCESS_ERROR(piSqlBuffer);

    pszSqlBuffer = (char *)piSqlBuffer->GetData();
    ASSERT(pszSqlBuffer);
    uBufferSize = piSqlBuffer->GetSize();
    ASSERT(uBufferSize > 0);

    uCurTime = time(NULL);
    localtime_r(&uCurTime, &tmNow);

    nRetCode = snprintf(
        pszSqlBuffer,
        uBufferSize - 1,
        "Insert into %s"
        "   (ActionTime, ActionLevel, ActionType, ActionTarget, RoleIndexID, Comment)"
        "values"
        "   ('%.4d-%.2d-%.2d %.2d:%.2d:%.2d', %u, %u, '%s', %u, '%s')",
        m_szLogActionTableName,
        tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday, 
        tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec, 
        pRequestPacket->uActionLevel, pRequestPacket->eActionType, 
        szActionTargetEscape, uRoleIndexID, pszCommentEscape
    );
    ASSERT((nRetCode > 0) && (nRetCode <= (int)uBufferSize - 1));
    pszSqlBuffer[uBufferSize - 1] = '\0';
    ulSqlLen = (unsigned long)nRetCode;

    nRetCode = DoMysqlQuery(m_pMysqlConn, pszSqlBuffer, ulSqlLen);
    LOG_PROCESS_ERROR(nRetCode);

    eLogReturnCode = LOG_RETURN_SUCCESS;

    nResult = true;
EXIT0:
    SAFE_RELEASE(piComment);
    SAFE_RELEASE(piSqlBuffer);

    //QLogPrintf(
    //   LOG_INFO, 
    //    "[%u] PacketID:%lu, Log PlayerAction on role \"%s\" of Account \"%s\", Result = %d",
    //    pPackage->uConnectionID, ulPacketID, pszRoleName, pszAccountName, eLogReturnCode
    //);

	if (eLogReturnCode != LOG_RETURN_SUCCESS)
		m_nFailureCount++;

    _SetResponsePacket(
        LOGS2C_LOG_PLAYER_ACTION_RESULT, ulPacketID, eLogReturnCode, ppiRetResponse
    );
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_log_data_analysis(
	string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse
)
{
	int nResult  = false;
	int nRetCode = false;
	LOG_RETURN_CODE eLogReturnCode = LOG_RETURN_FAILED;
	KGDataAnalysisLog *pRequestPacket = NULL;
	unsigned uRequestPacketSize = 0;
	char *pszComment = NULL;
	unsigned uCommentSize = 0;
	unsigned long ulPacketID = ULONG_MAX;
	unsigned uDateSize = 0;
	IMemBlock *piSqlBuffer = NULL;
	unsigned uBufferSize = 0;
	char *pszSqlBuffer = NULL;
	unsigned long ulSqlLen = 0;
	IMemBlock *piComment = NULL;
	char *pszCommentEscape = NULL;
	unsigned long ulCommentEscape = 0;
	unsigned uRoleIndexID = 0;
	DWORD dwPlayerId = 0;
	unsigned uGameServerId = 0;
	DWORD dwTeamId = 0;
	unsigned uPlayerLevel = 0;
	unsigned uPlayerRank = 0;
	time_t uCurTime = 0;
	struct tm tmNow = { 0 };

	ASSERT(psGatewayName);
	ASSERT(pPackage);
	ASSERT(ppiRetResponse);

	LOG_PROCESS_ERROR((!psGatewayName->empty()) && "[Log] Must login first");

	uRequestPacketSize = pPackage->piDataBuffer->GetSize();
	LOG_PROCESS_ERROR(uRequestPacketSize >= sizeof(KGDataAnalysisLog));
	pRequestPacket = (KGDataAnalysisLog *)pPackage->piDataBuffer->GetData();
	LOG_PROCESS_ERROR(pRequestPacket);

	ulPacketID = pRequestPacket->ulIdentity;

	uCommentSize = pRequestPacket->uCommentSize;
	LOG_PROCESS_ERROR(
		(uCommentSize <= LOG_COMMENT_MAX_SIZE) && 
		"[Log] PlayerAction Comment Data too long"
		);

	if (uCommentSize > 0)
	{
		uDateSize = sizeof(KGDataAnalysisLog) - 1 + uCommentSize;
		LOG_PROCESS_ERROR(uDateSize == uRequestPacketSize);
	}

	LOG_PROCESS_ERROR(
		(pRequestPacket->eLogType >= emDATA_ANALYSIS_LOG_NOMAL) &&
		(pRequestPacket->eLogType < emDATA_ANALYSIS_LOG_TYPE_COUNT)
		);

	if (pRequestPacket->uLogLevel < m_Param.uMinDataAnalysisLevel)
	{
		eLogReturnCode = LOG_RETURN_FILTER;
		PROCESS_ERROR(false);
	}

	uGameServerId = pRequestPacket->uGameServerId;
	dwPlayerId = pRequestPacket->dwPlayerId;
	dwTeamId = pRequestPacket->dwTeamId;
	uPlayerLevel = pRequestPacket->uPlayerLevel;
	uPlayerRank = pRequestPacket->uPlayerRank;

	if (uCommentSize > 0)
	{
		pszComment = (char *)pRequestPacket->pbyData;
		ASSERT(pszComment[uCommentSize - 1] == '\0');
	}

	if (uCommentSize > 0)
	{
		piComment = QCreateMemBlock(uCommentSize * 2);
		LOG_PROCESS_ERROR(piComment);

		pszCommentEscape = (char *)piComment->GetData();
		ASSERT(pszCommentEscape);
		uBufferSize = piComment->GetSize();
		ASSERT(uBufferSize > 0);

		ulCommentEscape = mysql_real_escape_string(
			m_pMysqlConn, pszCommentEscape, pszComment, uCommentSize - 1
			);
		MYSQL_PROCESS_ERROR((ulCommentEscape >= uCommentSize - 1), m_pMysqlConn);
	}

	piSqlBuffer = QCreateMemBlock(1024 * 4);
	LOG_PROCESS_ERROR(piSqlBuffer);

	pszSqlBuffer = (char *)piSqlBuffer->GetData();
	ASSERT(pszSqlBuffer);
	uBufferSize = piSqlBuffer->GetSize();
	ASSERT(uBufferSize > 0);

	uCurTime = time(NULL);
	localtime_r(&uCurTime, &tmNow);

	nRetCode = snprintf(
		pszSqlBuffer,
		uBufferSize - 1,
		"Insert into %s"
		"   (LogTime, LogLevel, LogType, GameServerId, PlayerId, TeamId, PlayerLevel, PlayerRank, Comment)"
		"values"
		"   ('%.4d-%.2d-%.2d %.2d:%.2d:%.2d', %u, %u, %u, %u, %u, %u, %u, '%s')",
		m_szLogDataAnalysisTableName,
		tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday, 
		tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec, 
		pRequestPacket->uLogLevel, pRequestPacket->eLogType, uGameServerId, dwPlayerId, dwTeamId,
		uPlayerLevel, uPlayerRank, pszCommentEscape
		);
	ASSERT((nRetCode > 0) && (nRetCode <= (int)uBufferSize - 1));
	pszSqlBuffer[uBufferSize - 1] = '\0';
	ulSqlLen = (unsigned long)nRetCode;

	nRetCode = DoMysqlQuery(m_pMysqlConn, pszSqlBuffer, ulSqlLen);
	LOG_PROCESS_ERROR(nRetCode);

	eLogReturnCode = LOG_RETURN_SUCCESS;

	nResult = true;
EXIT0:
	SAFE_RELEASE(piComment);
	SAFE_RELEASE(piSqlBuffer);

	QLogPrintf(
		LOG_INFO, 
		"[%u] PacketID:%lu, Log Dada analysis , Result = %d",
		pPackage->uConnectionID, ulPacketID, eLogReturnCode
		);

	if (eLogReturnCode != LOG_RETURN_SUCCESS)
		m_nFailureCount++;

	_SetResponsePacket(
		LOGS2C_LOG_DATA_ANALYSIS_RESULT, ulPacketID, eLogReturnCode, ppiRetResponse
		);
	return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_GetRoleIndexID(
    const char cszRoleName[],     unsigned uRoleNameSize,
    const char cszAccountName[],  unsigned uAccountNameSize,
    string *psGatewayName,
    unsigned *puRetRoleIndexID
)
{
    int nResult  = false;
    int nRetCode = false;
    unsigned uRoleIndexID = 0;
    MYSQL_RES *pResult = NULL;
    MYSQL_ROW  row;
    my_ulonglong ullRowCount = 0;
    char szSql[256];
    unsigned long ulSqlLen = 0;
    char szRoleNameEscape[128];
    unsigned uRoleNameEscapeLen = 0;
    char szAccountNameEscape[128];
    unsigned uAccountNameEscapeLen = 0;
    KG_ROLE_INDEX_MAP::iterator itRoleIndex;
    std::pair<KG_ROLE_INDEX_MAP::iterator, bool> pairInsertResult;
    KG_ROLE_INDEX RoleIndex;

    ASSERT(cszRoleName);
    ASSERT(cszAccountName);
    ASSERT(psGatewayName);
    ASSERT(puRetRoleIndexID);
    ASSERT(m_pMysqlConn);

    LOG_PROCESS_ERROR(!psGatewayName->empty());

    *puRetRoleIndexID = 0;

    RoleIndex.sRoleName    = cszRoleName;
    RoleIndex.sAccountName = cszAccountName;
    RoleIndex.sGatewayName = psGatewayName->c_str();

    itRoleIndex = m_RoleIndexMap.find(RoleIndex);
    if (itRoleIndex != m_RoleIndexMap.end())
    {
        uRoleIndexID = itRoleIndex->second;
        PROCESS_SUCCESS(true);
    }

    uRoleNameEscapeLen = mysql_real_escape_string(
        m_pMysqlConn, szRoleNameEscape, cszRoleName, uRoleNameSize - 1
    );
    MYSQL_PROCESS_ERROR((uRoleNameEscapeLen >= uRoleNameSize - 1), m_pMysqlConn);

    uAccountNameEscapeLen = mysql_real_escape_string(
        m_pMysqlConn, szAccountNameEscape, cszAccountName, uAccountNameSize - 1
    );
    MYSQL_PROCESS_ERROR((uAccountNameEscapeLen >= uAccountNameSize - 1), m_pMysqlConn);

    nRetCode = snprintf(
        szSql,
        sizeof(szSql) - 1,
        "select RoleIndexID from RoleAccountIndex "
        "   where AccountName = '%s' and RoleName = '%s' and GatewayName = '%s'",
        szAccountNameEscape, szRoleNameEscape, psGatewayName->c_str()
    );
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szSql) - 1));
    szSql[sizeof(szSql) - 1] = '\0';
    ulSqlLen = (unsigned long)nRetCode;

    nRetCode = DoMysqlQuery(m_pMysqlConn, szSql, ulSqlLen);
    LOG_PROCESS_ERROR(nRetCode);

    pResult = mysql_store_result(m_pMysqlConn);
    MYSQL_PROCESS_ERROR(pResult, m_pMysqlConn);

    ullRowCount = mysql_num_rows(pResult);
    if (ullRowCount == 1)
    {
        row = mysql_fetch_row(pResult);
        MYSQL_PROCESS_ERROR(row, m_pMysqlConn);

        ASSERT(row[0]);
        uRoleIndexID = strtoul(row[0], NULL, 10);

        pairInsertResult = m_RoleIndexMap.insert(
            make_pair(RoleIndex, uRoleIndexID)
        );
        LOG_PROCESS_ERROR(pairInsertResult.second);
    }
    else
    {
        nRetCode = snprintf(
            szSql,
            sizeof(szSql) - 1,
            "insert into RoleAccountIndex "
            "   (RoleIndexID, RoleName, AccountName, GatewayName) "
            "values"
            "   (null, '%s', '%s', '%s')",
            szRoleNameEscape, szAccountNameEscape, psGatewayName->c_str()
        );
        ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szSql) - 1));
        szSql[sizeof(szSql) - 1] = '\0';
        ulSqlLen = (unsigned long)nRetCode;

        nRetCode = DoMysqlQuery(m_pMysqlConn, szSql, ulSqlLen);
        LOG_PROCESS_ERROR(nRetCode);

        uRoleIndexID = (unsigned)mysql_insert_id(m_pMysqlConn);
    }

EXIT1:
    *puRetRoleIndexID = uRoleIndexID;
    nResult = true;
EXIT0:
    if (pResult)
    {
        mysql_free_result(pResult);
        pResult = NULL;
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_Connect()
{
    int nResult  = false;
    int nRetCode = false;
    MYSQL  *pRetCode = NULL;
    my_bool bReconnectFlag = 0;
    unsigned long ulMySQLClientVersionID = 0;

    ASSERT(!m_pMysqlConn);

    m_pMysqlConn = mysql_init(NULL);
    LOG_PROCESS_ERROR(m_pMysqlConn && "[MySQL Error]:mysql_init() failed");

    // enable reconnect if MySQL client version > 5
    ulMySQLClientVersionID = mysql_get_client_version();
    if (ulMySQLClientVersionID >= MYSQL5_VERSION_ID)
    {
        bReconnectFlag = 1;
        nRetCode = mysql_options(m_pMysqlConn, MYSQL_OPT_RECONNECT, &bReconnectFlag);
        MYSQL_PROCESS_ERROR((nRetCode == 0), m_pMysqlConn);
    }

    pRetCode = mysql_real_connect(
        m_pMysqlConn,
        m_Param.DatabaseParam.szServer,
        m_Param.DatabaseParam.szUsername,
        m_Param.DatabaseParam.szPassword,
        NULL,
        KG_DATABASE_PORT,
        NULL,
        CLIENT_FOUND_ROWS
    );
    MYSQL_PROCESS_ERROR((pRetCode == m_pMysqlConn), m_pMysqlConn);

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

int KG_LogThread::_DisConnect()
{
    if (m_pMysqlConn)
    {
        mysql_close(m_pMysqlConn);
        m_pMysqlConn = NULL;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_CreateLogDatabase()
{
    int  nResult  = false;
    int  nRetCode = false;
    char szCreateDatabaseSql[128];
    unsigned long ulSqlLen = 0;
    const char CREATE_TABLE_SQL[] = 
        "create table if not exists RoleAccountIndex"
        "("
        "   RoleIndexID int unsigned not null auto_increment,"
        "   RoleName    varchar(32) not null,"
        "   AccountName varchar(32) not null,"
        "   GatewayName varchar(32) not null,"
        "   primary key(RoleIndexID),"
        "   index(AccountName),"
		"	registerTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")";

    ASSERT(m_pMysqlConn);

    nRetCode = snprintf(
        szCreateDatabaseSql,
        sizeof(szCreateDatabaseSql) - 1,
        "create database if not exists %s",
        m_Param.DatabaseParam.szDatabase
    );
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szCreateDatabaseSql) - 1));
    szCreateDatabaseSql[sizeof(szCreateDatabaseSql) - 1] = '\0';
    ulSqlLen = (unsigned long)nRetCode;

    nRetCode = DoMysqlQuery(m_pMysqlConn, szCreateDatabaseSql, ulSqlLen);
    LOG_PROCESS_ERROR(nRetCode && "[Log] create database failed");

    nRetCode = mysql_select_db(m_pMysqlConn, m_Param.DatabaseParam.szDatabase);
    MYSQL_PROCESS_ERROR((nRetCode == 0), m_pMysqlConn);

    nRetCode = CheckDataBaseVariable(m_pMysqlConn);
    LOG_PROCESS_ERROR(nRetCode && "[Log] Check Database Variable Failed");

    // -------------------------------------------------
    nRetCode = DoMysqlQuery(m_pMysqlConn, CREATE_TABLE_SQL, sizeof(CREATE_TABLE_SQL) - 1);
    LOG_PROCESS_ERROR(nRetCode && "[Log] Create table RoleAccount Failed");

    // -------------------------------------------------
    nRetCode = _SwitchTable();
    LOG_PROCESS_ERROR(nRetCode && "[Log] Switch to New Table Failed");

    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_SwitchTable()
{
    int nResult  = false;
    int nRetCode = false;
    time_t uCurTime = 0;
    struct tm tmNow = { 0 };
    char szSql[1024];
    unsigned long ulSqlLen = 0;
    char szNewLogItemTable[128];
    char szNewLogActionTable[128];
	char szNewLogDataAnalysisTable[128];

    ASSERT(m_pMysqlConn);

    uCurTime = time(NULL);
    localtime_r(&uCurTime, &tmNow);

    PROCESS_SUCCESS(
        (m_tmLastSwitchTable.tm_mday == tmNow.tm_mday) &&
        (m_tmLastSwitchTable.tm_mon  == tmNow.tm_mon)  &&
        (m_tmLastSwitchTable.tm_year == tmNow.tm_year)
    );

    // ------------------------------------------
    nRetCode = snprintf(
        szNewLogItemTable, sizeof(szNewLogItemTable) - 1,
        "log_item_%.4d%.2d%.2d",
        tmNow.tm_year + 1900,
        tmNow.tm_mon + 1,
        tmNow.tm_mday
    );
    ASSERT((nRetCode > 0) && (nRetCode <= (sizeof(szNewLogItemTable) - 1)));
    szSql[sizeof(szNewLogItemTable) - 1] = '\0';

    nRetCode = snprintf(
        szSql, sizeof(szSql) - 1,
        "create table if not exists %s "
        "("
        "   OperationTime   datetime not null,"
        "   OperationType   int unsigned not null,"
        "   RoleIndexID     int unsigned not null,"
        "   ItemID          bigint unsigned not null,"
        "   ItemName        varchar(32) not null,"
        "   ItemValue       int unsigned not null,"
        "   ItemRestoreData text not null,"
        "   Comment         text,"
        "   Index(RoleIndexID),"
        "   Index(ItemID)"
        ")",
        szNewLogItemTable
    );
    ASSERT((nRetCode > 0) && (nRetCode <= (sizeof(szSql) - 1)));
    szSql[sizeof(szSql) - 1] = '\0';
    ulSqlLen = (unsigned long)nRetCode;

    nRetCode = DoMysqlQuery(m_pMysqlConn, szSql, ulSqlLen);
    LOG_PROCESS_ERROR(nRetCode && "[Log] Create new table Failed");

    strncpy(m_szLogItemTableName, szNewLogItemTable, sizeof(m_szLogItemTableName));
    m_szLogItemTableName[sizeof(m_szLogItemTableName) - 1] = '\0';

    // ------------------------------------------
    nRetCode = snprintf(
        szNewLogActionTable, sizeof(szNewLogActionTable) - 1,
        "log_action_%.4d%.2d%.2d",
        tmNow.tm_year + 1900,
        tmNow.tm_mon + 1,
        tmNow.tm_mday
    );
    ASSERT((nRetCode > 0) && (nRetCode <= (sizeof(szNewLogActionTable) - 1)));
    szSql[sizeof(szNewLogActionTable) - 1] = '\0';

    nRetCode = snprintf(
        szSql, sizeof(szSql) - 1,
        "create table if not exists %s "
        "("
        "   ActionTime      datetime not null,"
        "   ActionLevel     int unsigned not null,"
        "   ActionType      int unsigned not null,"
        "   ActionTarget    varchar(32) not null,"
        "   RoleIndexID     int unsigned not null,"
        "   Comment         text,"
        "   Index(RoleIndexID)"
        ")",
        szNewLogActionTable
    );
    ASSERT((nRetCode > 0) && (nRetCode <= (sizeof(szSql) - 1)));
    szSql[sizeof(szSql) - 1] = '\0';
    ulSqlLen = (unsigned long)nRetCode;

    nRetCode = DoMysqlQuery(m_pMysqlConn, szSql, ulSqlLen);
    LOG_PROCESS_ERROR(nRetCode && "[Log] Create new table Failed");

    strncpy(m_szLogActionTableName, szNewLogActionTable, sizeof(m_szLogActionTableName));
    m_szLogActionTableName[sizeof(m_szLogActionTableName) - 1] = '\0';

    // ------------------------------------------
	nRetCode = snprintf(
		szNewLogDataAnalysisTable, sizeof(szNewLogDataAnalysisTable) - 1,
		"log_dataanalysis_%.4d%.2d%.2d",
		tmNow.tm_year + 1900,
		tmNow.tm_mon + 1,
		tmNow.tm_mday
		);
	ASSERT((nRetCode > 0) && (nRetCode <= (sizeof(szNewLogDataAnalysisTable) - 1)));
	szSql[sizeof(szNewLogDataAnalysisTable) - 1] = '\0';

	nRetCode = snprintf(
		szSql, sizeof(szSql) - 1,
		"create table if not exists %s "
		"("
		"   LogTime         datetime not null,"
		"   LogLevel        int unsigned not null,"
		"   LogType         int unsigned not null,"
		"   GameServerId    int unsigned not null,"
		"   PlayerId        int unsigned not null,"
		"   TeamId          int unsigned not null,"
		"   PlayerLevel     int unsigned not null,"
		"   PlayerRank      int unsigned not null,"
		"   Comment         text"
		")",
		szNewLogDataAnalysisTable
		);
	ASSERT((nRetCode > 0) && (nRetCode <= (sizeof(szSql) - 1)));
	szSql[sizeof(szSql) - 1] = '\0';
	ulSqlLen = (unsigned long)nRetCode;

	nRetCode = DoMysqlQuery(m_pMysqlConn, szSql, ulSqlLen);
	LOG_PROCESS_ERROR(nRetCode && "[Log] Create new table Failed");

	strncpy(m_szLogDataAnalysisTableName, szNewLogDataAnalysisTable, sizeof(m_szLogDataAnalysisTableName));
	m_szLogDataAnalysisTableName[sizeof(m_szLogDataAnalysisTableName) - 1] = '\0';

	// ------------------------------------------
    nRetCode = _ClearExpiredData(uCurTime);
    LOG_PROCESS_ERROR(nRetCode && "[Log] Clear Expired Data Failed");

    m_tmLastSwitchTable = tmNow;
EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_ClearExpiredData(time_t uCurrentTime)
{
    int nResult  = false;
    int nRetCode = false;
    time_t uExpiredDate = 0;
    struct tm tmExpiredDate = { 0 };
    char szSql[128];
    unsigned long ulSqlLen = 0;

    ASSERT(m_pMysqlConn);

    ASSERT(m_Param.nExpiredDay > 0);
    uExpiredDate = uCurrentTime - 60 * 60 * 24 * (m_Param.nExpiredDay + 1);
    localtime_r(&uExpiredDate, &tmExpiredDate);

    //---------------drop expired log_item table--------------------
    nRetCode = snprintf(
        szSql,
        sizeof(szSql) - 1,
        "drop table if exists log_item_%.4d%.2d%.2d",
        tmExpiredDate.tm_year + 1900,
        tmExpiredDate.tm_mon + 1,
        tmExpiredDate.tm_mday
    );
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szSql) - 1));
    szSql[sizeof(szSql) - 1] = '\0';
    ulSqlLen = (unsigned long)nRetCode;
    
    nRetCode = DoMysqlQuery(m_pMysqlConn, szSql, ulSqlLen);
    LOG_PROCESS_ERROR(nRetCode && "[Log] Drop expired table failed");

    //---------------drop expired log_playeraction table------------
    nRetCode = snprintf(
        szSql,
        sizeof(szSql) - 1,
        "drop table if exists log_action_%.4d%.2d%.2d",
        tmExpiredDate.tm_year + 1900,
        tmExpiredDate.tm_mon + 1,
        tmExpiredDate.tm_mday
    );
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szSql) - 1));
    szSql[sizeof(szSql) - 1] = '\0';
    ulSqlLen = (unsigned long)nRetCode;

    nRetCode = DoMysqlQuery(m_pMysqlConn, szSql, ulSqlLen);
    LOG_PROCESS_ERROR(nRetCode && "[Log] Drop expired table failed");

	//---------------drop expired log_dataanalysis table------------
	nRetCode = snprintf(
		szSql,
		sizeof(szSql) - 1,
		"drop table if exists log_dataanalysis_%.4d%.2d%.2d",
		tmExpiredDate.tm_year + 1900,
		tmExpiredDate.tm_mon + 1,
		tmExpiredDate.tm_mday
		);
	ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szSql) - 1));
	szSql[sizeof(szSql) - 1] = '\0';
	ulSqlLen = (unsigned long)nRetCode;

	nRetCode = DoMysqlQuery(m_pMysqlConn, szSql, ulSqlLen);
	LOG_PROCESS_ERROR(nRetCode && "[Log] Drop expired table failed");

    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_LogThread::_SetResponsePacket(
    PROTOCOL_MSG_TYPE   cProtocol, 
    unsigned long       ulIdentity, 
    char                chRetResult,
    IMemBlock        **ppiRetResponse
)
{
    int nResult  = false;
    int nRetCode = false;
    IMemBlock    *piReplyPacket = NULL;
    TProcessData  *pReplyPacket  = NULL;

    ASSERT(ppiRetResponse);

    piReplyPacket = QCreateMemBlock(sizeof(TProcessData));
    LOG_PROCESS_ERROR(piReplyPacket);

    pReplyPacket = (TProcessData *)piReplyPacket->GetData();

    pReplyPacket->nProtoId       = cProtocol;
    pReplyPacket->ulIdentity     = ulIdentity;
    pReplyPacket->nDataLen       = 1;
    pReplyPacket->pDataBuffer[0] = chRetResult;

    *ppiRetResponse = piReplyPacket;
    (*ppiRetResponse)->AddRef();

    nResult = true;
EXIT0:
    SAFE_RELEASE(piReplyPacket);
    return nResult;
}
