#include "stdafx.h"
#include <set>
#include <list>
#include <string>
#include "mysql.h"
#include "errmsg.h"
#include "MiscDB.h"
#include "KRole.h"
#include "GameCenter.h"
#include "Misc/Misc.h"

#define REGISTER_DB_REQUEST_FUNC(ProtocolID, FuncName, ProtocolStruct)	                    \
    {                                                                                       \
        assert(ProtocolID < sizeof(m_ProcessRequestFuns) / sizeof(m_ProcessRequestFuns[0]));\
        m_ProcessRequestFuns[ProtocolID]    = &KMiscDB::FuncName;		                    \
        m_uRequestProtocolSize[ProtocolID]  = sizeof(ProtocolStruct);                       \
    }

void KMiscDB::WorkThreadFunction(void* pvParam)
{
    KMiscDB* pThis = (KMiscDB*)pvParam;

    assert(pThis);

    pThis->RegisterRequestProcess();
    pThis->ThreadFunction();
}

void KMiscDB::RegisterRequestProcess()
{
    REGISTER_DB_REQUEST_FUNC(emrpSaveRelationRequest, ProcessSaveRelationData, KSAVE_RELATION_DATA_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpLoadRelationRequest, ProcessLoadRelationData, KLOAD_RELATION_DATA_REQUEST);

    REGISTER_DB_REQUEST_FUNC(emrpSaveMapCopyInfoRequest, ProcessSaveMapCopyInfo, KSAVE_MAP_COPY_INFO_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpDeleteMapCopyInfoRequest, ProcessDeleteMapCopyInfo, KDELETE_MAP_COPY_INFO_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpUpdateRoleBlackList, ProcessUpdateRoleBlackList, KUPDATE_ROLE_BLACK_LIST);
    REGISTER_DB_REQUEST_FUNC(emrpUpdateRoleDeleteList, ProcessUpdateRoleDeleteList, KUPDATE_ROLE_DELETE_LIST);

    REGISTER_DB_REQUEST_FUNC(emrpLoadMailBoxRequest, ProcessLoadMailBox, KLOAD_MAIL_BOX_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpSaveMailBoxRequest, ProcessSaveMailBox, KSAVE_MAIL_BOX_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpSaveGlobalMailRequest, ProcessSaveGlobalMail, KSAVE_GLOBAL_MAIL_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpDeleteGlobalMailRequest, ProcessDeleteGlobalMail, KDELETE_GLOBAL_MAIL_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpCheckWithdrawMailRequest, ProcessCheckWithdrawMail, KCHECK_WITHDRAW_MAIL_REQUEST);
    
    REGISTER_DB_REQUEST_FUNC(emrSaveTongDataRequest, ProcessSaveTongData, KSAVE_TONG_DATA_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrRemoveTongDataRequest, ProcessRemoveTongData, KREMOVE_TONG_DATA_REQUEST);

	REGISTER_DB_REQUEST_FUNC(emrSaveRankDataRequest, ProcessSaveRankData, KSAVE_RANK_DATA_REQUEST);

	REGISTER_DB_REQUEST_FUNC(emrSaveArenaRecordRequest, ProcessSaveArenaRecord, KSAVE_ARENA_RECORD_REQUEST);
	REGISTER_DB_REQUEST_FUNC(emrLoadArenaRecordRequest, ProcessLoadArenaRecord, KLOAD_ARENA_RECORD_REQUEST);

	REGISTER_DB_REQUEST_FUNC(emrSaveServerRecordRequest, ProcessSaveServerRecord, KSAVE_SERVER_RECORD_REQUEST);
	REGISTER_DB_REQUEST_FUNC(emrLoadServerRecordRequest, ProcessLoadServerRecord, KLOAD_SERVER_RECORD_REQUEST);
    
    //REGISTER_DB_REQUEST_FUNC(emrpLoadPQListRequest, ProcessLoadPQList, KLOAD_PQ_LIST_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpSavePQRequest, ProcessSavePQ, KSAVE_PQ_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpDeletePQRequest, ProcessDeletePQ, KDELETE_PQ_REQUEST);

    REGISTER_DB_REQUEST_FUNC(emrpAuctionLookupRequest, ProcessAuctionLookup, KAUCTION_LOOKUP_DB_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpAuctionBidRequest, ProcessAuctionBid, KAUCTION_BID_DB_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpAuctionSellRequest, ProcessAuctionSell, KAUCTION_SELL_DB_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpAuctionCancelRequest, ProcessAuctionCancel, KAUCTION_CANCEL_DB_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpAuctionCheckDurationRequest, ProcessAuctionCheckDuration, KAUCTION_CHECK_DURATION_REQUEST);

    //REGISTER_DB_REQUEST_FUNC(emrpSaveGlobalCustomData, ProcessSaveGlobalCustomData, KSAVE_GLOBAL_CUSTOM_DATA_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpDeleteGlobalCustomData, ProcessDeleteGlobalCustomData, KDELETE_GLOBAL_CUSTOM_DATA_REQUEST);

    //REGISTER_DB_REQUEST_FUNC(emrpSaveGlobalSystemValue, ProcessSaveGlobalSystemValue, KSAVE_GLOBAL_SYSTEM_VALUE_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpSaveStatData, ProcessSaveStatData, KSAVE_STAT_DATA_REQUEST);
    REGISTER_DB_REQUEST_FUNC(emrpSaveStatDataName, ProcessSaveStatDataName, KSAVE_STAT_DATA_NAME_REQUEST);

    //REGISTER_DB_REQUEST_FUNC(emrpUpdateActivityData, ProcessUpdateActivityData, KUPDATE_ACTIVITY_DATA_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpDeleteActivityData, ProcessDeleteActivityData, KDELETE_ACTIVITY_DATE_REQUEST);

    //REGISTER_DB_REQUEST_FUNC(emrpGameCardSellRequest, ProcessGameCardSell, KGAME_CARD_SELL_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpGameCardBuyRequest, ProcessGameCardBuy, KGAME_CARD_BUY_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpGameCardLookupRequest, ProcessGameCardLookup, KGAME_CARD_LOOKUP_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpGameCardCancelRequest, ProcessGameCardCancel, KGAME_CARD_CANCEL_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpGameCardCheckDurationRequest, ProcessGameCardCheckDuration, KGAME_CARD_CHECK_DURATION_REQUEST);

    //REGISTER_DB_REQUEST_FUNC(emrpAddFarmerRecordRequest, ProcessAddFarmerRecord, KADD_FARMER_RECORD_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpDeleteFarmerRecordRequest, ProcessDeleteFarmerRecord, KDELETE_FARMER_RECORD_REQUEST);

    //REGISTER_DB_REQUEST_FUNC(emrpSaveMentorRecordRequest, ProcessSaveMentorRecord, KSAVE_MENTOR_RECORD_REQUEST);
    //REGISTER_DB_REQUEST_FUNC(emrpDeleteMentorRecordRequest, ProcessDeleteMentorRecord, KDELETE_MENTOR_RECORD_REQUEST);
}

int KMiscDB::ThreadFunction()
{   
    UINT64    uBeginTime          = GetTickCount();
    UINT64    uTimeNow            = 0;
    int         nIdleCount          = 0;

    while (m_bRunFlag)
    {
        uTimeNow = GetTickCount();

        if ((uTimeNow - uBeginTime) * GAME_FPS < (UINT64)m_nDBThreadWorkLoop * 1000)
        {
            nIdleCount++;
            QThread_Sleep(10);
            continue;
        }

        m_nDBThreadIdleLoop = nIdleCount;
        nIdleCount  = 0;

        DBThreadActivate();
    }

    DumpOperationCount();

    QLogPrintf(LOG_INFO, "[KMiscDB] Working thread exit !");

    return 0;
}

void KMiscDB::DBThreadActivate()
{
    int                     nResult             = false;
    int                     nRetCode            = false;
    IMemBlock*             piBuffer            = NULL;
    BYTE*                   pbyData             = NULL;
    size_t                  uDataLen            = 0;
    KMISC_DB_REQUEST*       pHeader             = NULL;
    PROCESS_REQUEST_FUNC    ProcessFunc         = NULL;
    
    m_nDBThreadWorkLoop++;

    // 定期ping数据库，防止过久没操作数据库导致连接断开
    if (g_pGameCenter->m_nTimeNow > m_nNextPingTime)
    {
        if (m_pDBHandle)
        {
            mysql_ping(m_pDBHandle);
        }
        m_nNextPingTime = g_pGameCenter->m_nTimeNow + PING_CYCLE;
    }

    if (!m_pDBHandle)
    {
        static time_t nNextReConnectTime  = 0;

        if (g_pGameCenter->m_nTimeNow >= nNextReConnectTime)
        {
            QLogPrintf(LOG_DEBUG, "Reconnecting to the database ... ...\n");

            m_pDBHandle = MySQL_Connect(
                g_pGameCenter->m_Settings.m_szDBIP, g_pGameCenter->m_Settings.m_szDBName, 
                g_pGameCenter->m_Settings.m_szDBAcc, g_pGameCenter->m_Settings.m_szDBPsw
            );
            
            nNextReConnectTime = g_pGameCenter->m_nTimeNow + 10;

            if (m_pDBHandle)
            {
                QLogPrintf(LOG_DEBUG, "Reconnecting to the database successfully!\n");
            }
        }
    }

    while (m_pDBHandle)
    {
        SAFE_RELEASE(piBuffer);

        piBuffer = PopRequest();
        PROCESS_ERROR(piBuffer);

        pbyData = (BYTE*)piBuffer->GetData();
        LOG_PROCESS_ERROR(pbyData);

        pHeader = (KMISC_DB_REQUEST*)pbyData;
        LOG_PROCESS_ERROR(pHeader);

        LOG_PROCESS_ERROR(pHeader->nProtocolID > emrpRequestBegin);
        LOG_PROCESS_ERROR(pHeader->nProtocolID < emrpRequestEnd);

        uDataLen = piBuffer->GetSize();
        LOG_PROCESS_ERROR(uDataLen >= m_uRequestProtocolSize[pHeader->nProtocolID]);

        ProcessFunc = m_ProcessRequestFuns[pHeader->nProtocolID];
        LOG_PROCESS_ERROR(ProcessFunc);

        m_nOperationCount[pHeader->nProtocolID]++;
        m_nOperationSpeed++;

        (this->*ProcessFunc)(pbyData, uDataLen);
    }

EXIT0:
    if (m_nDBThreadWorkLoop % GAME_FPS == 0)
    {
        m_fOperationSpeed = m_fOperationSpeed * 0.9f + (float)m_nOperationSpeed * 0.1f;
        m_nOperationSpeed = 0;
    }

    m_bRequestProcessing = false;
    SAFE_RELEASE(piBuffer);
    return;
}

BOOL KMiscDB::PushRespond(IMemBlock *piRespond)
{
    assert(piRespond);

    m_RespondQueueLock.Lock();

    piRespond->AddRef();

    m_RespondQueue.push_back(piRespond);

    m_RespondQueueLock.Unlock();

    return true;
}

IMemBlock* KMiscDB::PopRequest()
{
    IMemBlock* piBuffer = NULL;

    m_RequestQueueLock.Lock();

    if (!m_RequestQueue.empty())
    {
        piBuffer = m_RequestQueue.front();
        m_RequestQueue.pop_front();
        m_bRequestProcessing = true;
    }

    m_RequestQueueLock.Unlock();

    return piBuffer;
}

BOOL KMiscDB::DumpOperationCount()
{
    BOOL    bResult = false;
    FILE*   pFile   = NULL;

    pFile = fopen("misc_db_operation.txt", "w");
    LOG_PROCESS_ERROR(pFile);

    fprintf(pFile, "ID\tCount\n");

    for (int i = emrpRequestBegin + 1; i < emrpRequestEnd; i++)
    {
        fprintf(pFile, "%d\t%d\n", i, m_nOperationCount[i]);
    }

    bResult = true;
EXIT0:
    if (pFile)
    {
        fclose(pFile);
        pFile = NULL;
    }
    return bResult;
}

BOOL KMiscDB::ProcessSaveRelationData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult             = false;
    int                             nRetCode            = false;
    KSAVE_RELATION_DATA_REQUEST*  pRequest            = (KSAVE_RELATION_DATA_REQUEST*)pbyData;
    MYSQL_RES*                      pQueryRes           = NULL;
    int                             nParamCount         = 0;
    int                             nRowNum             = 0;
    unsigned long                   ulStrLen            = 0;
    unsigned int                    uRelationDataLen  = 0;
    unsigned int                    uGroupNameLen       = 0;
    my_ulonglong                    lAffectedRow        = 0;
    char*                           pszPos              = NULL;
    size_t                          uLeftBufferSize     = 0;

    LOG_PROCESS_ERROR(uDataLen == sizeof(KSAVE_RELATION_DATA_REQUEST) + pRequest->uDataLen);

    pszPos              = m_pszSQL;
    uLeftBufferSize     = MAX_MISC_DB_SQL_SIZE;

    nRetCode = snprintf(pszPos, uLeftBufferSize, 
        "replace into "
        "    %s "
        "    (PlayerID, RelationData) "
        "values "
        "    (%u, '",
        RELATION_TABLE_NAME, 
        pRequest->dwPlayerID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    uRelationDataLen  = (unsigned)pRequest->uDataLen;
    LOG_PROCESS_ERROR(uLeftBufferSize > uRelationDataLen * 2);

    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (const char *)pRequest->byData, uRelationDataLen);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftBufferSize, "');");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    lAffectedRow = mysql_affected_rows(m_pDBHandle);
    LOG_PROCESS_ERROR(lAffectedRow <= 2);

    bResult = true;
EXIT0:
    if(!bResult)
    {
        QLogPrintf(LOG_ERR, "Save relation data error! PlayerID: %u", pRequest->dwPlayerID);
    }

    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }

    return bResult;
}

BOOL KMiscDB::ProcessLoadRelationData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult         = false;
    KLOAD_RELATION_DATA_REQUEST*  pRequest        = (KLOAD_RELATION_DATA_REQUEST*)pbyData;
    IMemBlock*                     piBuffer        = NULL;
    MYSQL_RES*                      pQueryRes       = NULL;
    unsigned long*                  pulFieldLength  = NULL;
    KLOAD_RELATION_DATA_RESPOND*  pRespond        = NULL;
    int                             nRetCode        = 0;
    int                             nRowNum         = 0;
    int                             nCount          = 0;
    char                            szSQL[1024];
    MYSQL_ROW                       QueryRow;

    nRetCode = snprintf(szSQL, sizeof(szSQL), 
        "select "
        "    RelationData "
        "from "
        "    %s "
        "where "
        "    PlayerID = %u;", 
        RELATION_TABLE_NAME, 
        pRequest->dwPlayerID
    );
    LOG_PROCESS_ERROR(nRetCode < sizeof(szSQL));

    nRetCode = DoQuery(szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pQueryRes = mysql_store_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    nRowNum = (int)mysql_num_rows(pQueryRes);

    if (nRowNum == 0)
    {
        piBuffer = QCreateMemBlock(sizeof(KLOAD_RELATION_DATA_RESPOND));
        LOG_PROCESS_ERROR(piBuffer);

        pRespond = (KLOAD_RELATION_DATA_RESPOND*)piBuffer->GetData();
        LOG_PROCESS_ERROR(pRespond);

        pRespond->nProtocolID = emrpLoadRelationRespond;
        pRespond->dwPlayerID  = pRequest->dwPlayerID;
    }
    else
    {
        LOG_PROCESS_ERROR(nRowNum == 1);

        QueryRow = mysql_fetch_row(pQueryRes);
        LOG_PROCESS_ERROR(QueryRow);

        pulFieldLength = mysql_fetch_lengths(pQueryRes);
        LOG_PROCESS_ERROR(pulFieldLength);

        piBuffer = QCreateMemBlock(sizeof(KLOAD_RELATION_DATA_RESPOND) + pulFieldLength[0]);
        LOG_PROCESS_ERROR(piBuffer);

        pRespond = (KLOAD_RELATION_DATA_RESPOND*)piBuffer->GetData();
        LOG_PROCESS_ERROR(pRespond);

        pRespond->nProtocolID = emrpLoadRelationRespond;
        pRespond->dwPlayerID  = pRequest->dwPlayerID;

        memcpy(pRespond->byData, QueryRow[0], pulFieldLength[0]);
    }

    nRetCode = PushRespond(piBuffer);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    SAFE_RELEASE(piBuffer);

    return bResult;
}

BOOL KMiscDB::ProcessSaveMapCopyInfo(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult         = false;
    int                             nRetCode        = false;
    char*                           pszPos          = NULL;
    size_t                          uLeftBufferSize = 0;
    my_ulonglong                    lAffectedRow    = 0;
    KSAVE_MAP_COPY_INFO_REQUEST*    pRequest        = NULL;

    pRequest = (KSAVE_MAP_COPY_INFO_REQUEST*)pbyData;
    
    LOG_PROCESS_ERROR(pRequest->uDataLen != 0);
    
    pszPos          = m_pszSQL;
    uLeftBufferSize = MAX_MISC_DB_SQL_SIZE;

    nRetCode = snprintf(
        pszPos, uLeftBufferSize,
        "replace into %s "
        "   (MapID, MapCopyIndex, CreateTime, LastModifyTime, SceneData) "
        "values "
        "   (%u, %d, from_unixtime(%lu), from_unixtime(%lu), '",
        MAP_COPY_TABLE_NAME,
        pRequest->dwMapID,
        pRequest->nMapCopyIndex,
        (uint64_t)pRequest->nCreateTime,
        (uint64_t)pRequest->nLastSaveTime
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;
    
    LOG_PROCESS_ERROR(uLeftBufferSize > pRequest->uDataLen * 2);
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (const char *)pRequest->byData, (int)pRequest->uDataLen);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;
    
    nRetCode = snprintf(pszPos, uLeftBufferSize, "'); "); 
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    lAffectedRow = mysql_affected_rows(m_pDBHandle);
    LOG_PROCESS_ERROR(lAffectedRow == 1 || lAffectedRow == 2);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        QLogPrintf(
            LOG_DEBUG, "[KMiscDB]: Save MapCopyInfo[MapID: %u, MapCopyIndex: %d, OwnerID: %u] Failed !", 
            pRequest->dwMapID, 
            pRequest->nMapCopyIndex, 
            pRequest->dwOwnerID
        );
    }
    return bResult;
}

BOOL KMiscDB::ProcessDeleteMapCopyInfo(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult             = false;
    int                             nRetCode            = 0;
    KDELETE_MAP_COPY_INFO_REQUEST*  pRequest            = (KDELETE_MAP_COPY_INFO_REQUEST*)pbyData;
    char                            szSQL[1024];

    nRetCode = snprintf(
        szSQL, sizeof(szSQL),
        "delete from %s where MapID = %u and MapCopyIndex = %d;",
        MAP_COPY_TABLE_NAME,
        pRequest->dwMapID,
        pRequest->nMapCopyIndex
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szSQL));

    nRetCode = DoQuery(szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        QLogPrintf(
            LOG_DEBUG, "[KMiscDB] : Delete MapCopyInfo[MapID: %u, MapCopyIndex: %d] Failed! ", 
            pRequest->dwMapID, 
            pRequest->nMapCopyIndex
        );
    }

    return bResult;
}

BOOL KMiscDB::ProcessUpdateRoleBlackList(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult     = false;
    int                             nRetCode    = 0;
    KUPDATE_ROLE_BLACK_LIST*        pRequest    = (KUPDATE_ROLE_BLACK_LIST*)pbyData;
    char                            szSQL[1024];

    if (pRequest->nFreezeTime > g_pGameCenter->m_nTimeNow)
    {
        nRetCode = snprintf(
              szSQL, sizeof(szSQL),
              "replace into %s "
              "(ID, FreezeTime) "
              "values(%u, FROM_UNIXTIME(%lu))",
              ROLE_BLACK_LIST_TABLE_NAME,
              pRequest->dwRoleID,
              (uint64_t)pRequest->nFreezeTime
        );
    }
    else
    {
        nRetCode = snprintf(
              szSQL, sizeof(szSQL),
              "delete from %s "
              "where ID = %u ",
              ROLE_BLACK_LIST_TABLE_NAME,
              pRequest->dwRoleID
        );
    }
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szSQL));

    nRetCode = DoQuery(szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessUpdateRoleDeleteList(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult     = false;
    int                         nRetCode    = 0;
    KUPDATE_ROLE_DELETE_LIST*   pRequest    = (KUPDATE_ROLE_DELETE_LIST*)pbyData;

    if (pRequest->nEndTime > g_pGameCenter->m_nTimeNow)
    {
        nRetCode = snprintf(
            m_pszSQL, MAX_MISC_DB_SQL_SIZE,
            " replace into %s (ID, EndTime) values(%u, FROM_UNIXTIME(%lu)); ",
            ROLE_DELETE_LIST_TABLE_NAME, pRequest->dwID, (uint64_t)pRequest->nEndTime
        );
    }
    else
    {
        nRetCode = snprintf(
            m_pszSQL, MAX_MISC_DB_SQL_SIZE,
            " delete from %s where ID = %u; ",
            ROLE_DELETE_LIST_TABLE_NAME, pRequest->dwID
        );
    }
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessLoadMailBox(BYTE* pbyData, size_t uDataLen)
{
    BOOL                    bResult         = false;
    int                     nRetCode        = 0;
    KLOAD_MAIL_BOX_REQUEST* pRequest        = (KLOAD_MAIL_BOX_REQUEST*)pbyData;
    IMemBlock*             piBuffer        = NULL;
    KLOAD_MAIL_BOX_RESPOND* pRespond        = NULL;
    unsigned long           uMailBoxLen     = 0;
    BOOL                    bSucceed        = false;
    time_t                  nBaseTime       = 0;
    MYSQL_RES*              pQueryRes       = NULL;
    int                     nRowNum         = 0;
    unsigned long*          pulFieldLength  = NULL;
    MYSQL_ROW               QueryRow        = NULL;

    
    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "select "
        "   UNIX_TIMESTAMP(BaseTime), MailBoxInfo "
        "from "
        "   %s "
        "where "
        "   ID = %u;",
        MAIL_TABLE_NAME,
        pRequest->dwMailBoxID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    if (!nRetCode)
    {
        goto EXIT1;
    }

    pQueryRes = mysql_store_result(m_pDBHandle);
    if (pQueryRes == NULL)
    {
        goto EXIT1;
    }

    nRowNum = (int)mysql_num_rows(pQueryRes);
    if (nRowNum == 1)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (QueryRow == NULL)
        {
            goto EXIT1;
        }

        pulFieldLength = mysql_fetch_lengths(pQueryRes);
        if (pulFieldLength == NULL)
        {
            goto EXIT1;
        }

        uMailBoxLen = pulFieldLength[1];

        nBaseTime = strtol(QueryRow[0], NULL, 10);
    }

    bSucceed = true;
EXIT1:
    piBuffer = QCreateMemBlock((unsigned)(sizeof(KLOAD_MAIL_BOX_RESPOND) + uMailBoxLen));
    LOG_PROCESS_ERROR(piBuffer);

    pRespond = (KLOAD_MAIL_BOX_RESPOND*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRespond);

    pRespond->nProtocolID = emrpLoadMailBoxRespond;
    pRespond->dwMailBoxID = pRequest->dwMailBoxID;
    pRespond->bSucceed    = bSucceed;
    pRespond->nBaseTime   = nBaseTime;
    pRespond->uMailBoxLen = uMailBoxLen;

    if (uMailBoxLen > 0)
    {
        memcpy(pRespond->MailBoxInfo, QueryRow[1], uMailBoxLen);
    }

    nRetCode = PushRespond(piBuffer);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::ProcessSaveMailBox(BYTE* pbyData, size_t uDataLen)
{
    BOOL                    bResult         = false;
    int                     nRetCode        = 0;
    KSAVE_MAIL_BOX_REQUEST* pRequest        = (KSAVE_MAIL_BOX_REQUEST*)pbyData;

    if (pRequest->nMailCount == 0)
    {
        nRetCode = snprintf(
            m_pszSQL, MAX_MISC_DB_SQL_SIZE,
            "delete from %s "
            "where ID = %u;",
            MAIL_TABLE_NAME,
            pRequest->dwMailBoxID
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
    }
    else
    {
        char*   pszPos      = m_pszSQL;
        size_t  uLeftSize   = MAX_MISC_DB_SQL_SIZE;

        nRetCode = snprintf(
            pszPos, uLeftSize,
            "replace into %s "
            "   (ID, BaseTime, MailBoxInfo) "
            "values "
            "   (%u, FROM_UNIXTIME(%lu), '",
            MAIL_TABLE_NAME,
            pRequest->dwMailBoxID,
            (uint64_t)pRequest->nBaseTime
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
        pszPos      += nRetCode;
        uLeftSize   -= nRetCode;

        LOG_PROCESS_ERROR(uLeftSize > pRequest->uMailBoxLen * 2);
        nRetCode = mysql_real_escape_string(
            m_pDBHandle, pszPos, (const char*)pRequest->MailBoxInfo, (unsigned long)pRequest->uMailBoxLen
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
        pszPos      += nRetCode;
        uLeftSize   -= nRetCode;

        nRetCode = snprintf(pszPos, uLeftSize, "');");
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    }

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessCheckWithdrawMail(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult         = false;
    int                             nRetCode        = 0;
    KCHECK_WITHDRAW_MAIL_REQUEST*   pRequest        = (KCHECK_WITHDRAW_MAIL_REQUEST*)pbyData;
    MYSQL_RES*                      pQueryRes       = NULL;
    int                             nRowNum         = 0;
    unsigned long*                  pulFieldLength  = NULL;
    MYSQL_ROW                       QueryRow        = NULL;
    IMemBlock*                     piBuffer        = NULL;
    KCHECK_WITHDRAW_MAIL_RESPOND*   pRespond        = NULL;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "select ID from %s where BaseTime < FROM_UNIXTIME(%ld);",
        MAIL_TABLE_NAME,
        pRequest->nDeadLineTime
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pQueryRes = mysql_store_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    nRowNum = (int)mysql_num_rows(pQueryRes);
    PROCESS_SUCCESS(nRowNum == 0);

    piBuffer = QCreateMemBlock((unsigned)sizeof(KCHECK_WITHDRAW_MAIL_RESPOND) + nRowNum * sizeof(DWORD));
    LOG_PROCESS_ERROR(piBuffer);

    pRespond = (KCHECK_WITHDRAW_MAIL_RESPOND*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRespond);

    pRespond->nProtocolID   = emrpCheckWithdrawMailRespond;
    pRespond->nMailBoxCount = nRowNum;

    for (int i = 0; i < nRowNum; i++)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        LOG_PROCESS_ERROR(QueryRow);

        pRespond->dwMailBoxIDList[i] = strtol(QueryRow[0], NULL, 10);
    }

    nRetCode = PushRespond(piBuffer);
    LOG_PROCESS_ERROR(nRetCode);

EXIT1:
    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

BOOL KMiscDB::ProcessSaveGlobalMail(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult         = false;
    int                             nRetCode        = 0;
    KSAVE_GLOBAL_MAIL_REQUEST*      pRequest        = (KSAVE_GLOBAL_MAIL_REQUEST*)pbyData;
    KMail*                          pMail           = (KMail*)pRequest->byData;
    size_t                          uMailLen        = 0;
    char*                           pszPos          = m_pszSQL;
    size_t                          uLeftSize       = MAX_MISC_DB_SQL_SIZE;

    LOG_PROCESS_ERROR(uDataLen >= sizeof(KSAVE_GLOBAL_MAIL_REQUEST) +sizeof(KMail));
    uMailLen = sizeof(KMail) + pMail->wTextLen;
    for (int i = 0; i < KMAIL_MAX_ITEM_COUNT; i++)
    {
        uMailLen += pMail->ItemDesc[i].byDataLen;
    }
    LOG_PROCESS_ERROR(uDataLen == sizeof(KSAVE_GLOBAL_MAIL_REQUEST) + uMailLen);

    nRetCode = snprintf(
        pszPos, uLeftSize,
        " replace into %s "
        "   (Version, MailIndex, EndTime, MailInfo) "
        " values "
        "   (%d, %u, FROM_UNIXTIME(%lu), '",
        GLOBAL_MAIL_TABLE_NAME,
        pRequest->nVersion,
        pRequest->dwMailIndex,
        (uint64_t)pRequest->nEndTime
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    LOG_PROCESS_ERROR(uLeftSize > uMailLen * 2);
    nRetCode = mysql_real_escape_string(
        m_pDBHandle, pszPos, (const char*)pMail, (unsigned long)uMailLen
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftSize, "');");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessDeleteGlobalMail(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult  = false;
    int                             nRetCode = 0;
    KDELETE_GLOBAL_MAIL_REQUEST*    pRequest = (KDELETE_GLOBAL_MAIL_REQUEST*)pbyData;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "delete from %s where MailIndex = %u",
        GLOBAL_MAIL_TABLE_NAME,
        pRequest->dwMailIndex
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessLoadPQList(BYTE* pbyData, size_t uDataLen)
{
    BOOL                    bResult         = false;
    int                     nRetCode        = 0;
    IMemBlock*             piBuffer        = NULL;
    KLOAD_PQ_LIST_RESPOND*  pRespond        = NULL;
    MYSQL_RES*              pQueryRes       = NULL;
    int                     nRowNum         = 0;
    unsigned long*          pulFieldLength  = NULL;
    MYSQL_ROW               QueryRow        = NULL;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "select "
        "   ID, Data "
        "from "
        "   %s "
        ";",
        PQ_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pQueryRes = mysql_store_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    nRowNum = (int)mysql_num_rows(pQueryRes);

    piBuffer = QCreateMemBlock((unsigned)(sizeof(KLOAD_PQ_LIST_RESPOND) + sizeof(KGPQ_DB_DATA) * nRowNum));
    LOG_PROCESS_ERROR(piBuffer);

    pRespond = (KLOAD_PQ_LIST_RESPOND*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRespond);

    pRespond->nProtocolID   = emrpLoadPQListRespond;
    pRespond->nCount        = nRowNum;

    for (int i = 0; i < nRowNum; i++)
    {
        size_t  uSize   = 0;
        WORD*   pwSize  = NULL;

        QueryRow = mysql_fetch_row(pQueryRes);
        if (!QueryRow)
            break;

        pulFieldLength = mysql_fetch_lengths(pQueryRes);
        LOG_PROCESS_ERROR(pulFieldLength[1] == (unsigned long)sizeof(KGPQ_DB_DATA));

        memcpy(&pRespond->PQData[i], QueryRow[1], sizeof(pRespond->PQData[i]));
    }

    nRetCode = PushRespond(piBuffer);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::ProcessSavePQ(BYTE* pbyData, size_t uDataLen)
{
    BOOL                bResult     = false;
    int                 nRetCode    = 0;
    KSAVE_PQ_REQUEST*   pRequest    = (KSAVE_PQ_REQUEST*)pbyData;
    char*               pszPos      = m_pszSQL;
    size_t              uLeftSize   = MAX_MISC_DB_SQL_SIZE;

    nRetCode = snprintf(
        pszPos, uLeftSize,
        "replace into"
        "    %s "
        "values "
        "    (%u, '", 
        PQ_TABLE_NAME, pRequest->PQData.dwPQID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
    pszPos += nRetCode;
    uLeftSize -= nRetCode;

    LOG_PROCESS_ERROR(uLeftSize > sizeof(pRequest->PQData) * 2);
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (const char *)&pRequest->PQData, (unsigned long)sizeof(pRequest->PQData));
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos += nRetCode;
    uLeftSize -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftSize, "');");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessDeletePQ(BYTE* pbyData, size_t uDataLen)
{
    BOOL                bResult     = false;
    int                 nRetCode    = 0;
    KDELETE_PQ_REQUEST* pRequest    = (KDELETE_PQ_REQUEST*)pbyData;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "delete "
        "from "
        "   %s "
        "where "
        "   ID = %u "
        ";",
        PQ_TABLE_NAME, pRequest->dwPQID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessSaveTongData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult     = false;
    int                         nRetCode    = 0;   
    char*                       pszPos      = m_pszSQL;
    size_t                      uLeftSize   = MAX_MISC_DB_SQL_SIZE;
    KSAVE_TONG_DATA_REQUEST*    pRequest    = NULL;
    DWORD                       dwTongID    = KD_BAD_ID;
    unsigned long               ulStrLen    = 0;

    LOG_PROCESS_ERROR(pbyData);
    pRequest = (KSAVE_TONG_DATA_REQUEST*)pbyData;
    
    dwTongID = pRequest->dwTongID;
    LOG_PROCESS_ERROR(dwTongID);

    nRetCode = snprintf(
        pszPos, uLeftSize,
        "replace into %s (ID, Data) values (%u, '",
        TONG_TABLE_NAME,
        dwTongID   
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    LOG_PROCESS_ERROR(uLeftSize > pRequest->uDataLen * 2);
    nRetCode = mysql_real_escape_string(
        m_pDBHandle, pszPos, (const char*)pRequest->byData, (unsigned long)pRequest->uDataLen
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftSize, "');");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessRemoveTongData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult     = false;
    int                         nRetCode    = 0;
    KREMOVE_TONG_DATA_REQUEST*  pRequest    = (KREMOVE_TONG_DATA_REQUEST*)pbyData;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "delete from %s where ID = %u ;",
        TONG_TABLE_NAME, pRequest->dwTongID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessSaveRankData(BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bResult     = false;
	int                         nRetCode    = 0;   
	char*                       pszPos      = m_pszSQL;
	size_t                      uLeftSize   = MAX_MISC_DB_SQL_SIZE;
	KSAVE_RANK_DATA_REQUEST*    pRequest    = NULL;
	unsigned long               ulStrLen    = 0;

	LOG_PROCESS_ERROR(pbyData);
	pRequest = (KSAVE_RANK_DATA_REQUEST*)pbyData;

	nRetCode = snprintf(
		pszPos, uLeftSize,
		"replace into %s (RankID, Data) values (%d, '",
		RANKPOOL_TABLE_NAME,
		pRequest->byRankId   
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
	pszPos      += nRetCode;
	uLeftSize   -= nRetCode;

	LOG_PROCESS_ERROR(uLeftSize > pRequest->uDataLen * 2);
	nRetCode = mysql_real_escape_string(
		m_pDBHandle, pszPos, (const char*)pRequest->byData, (unsigned long)pRequest->uDataLen
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
	pszPos      += nRetCode;
	uLeftSize   -= nRetCode;

	nRetCode = snprintf(pszPos, uLeftSize, "');");
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
	pszPos      += nRetCode;
	uLeftSize   -= nRetCode;

	nRetCode = DoQuery(m_pszSQL);
	LOG_PROCESS_ERROR(nRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KMiscDB::ProcessSaveArenaRecord(BYTE* pbyData, size_t uDataLen)
{
	BOOL							bResult     = false;
	int								nRetCode    = 0;   
	char*							pszPos      = m_pszSQL;
	size_t							uLeftSize   = MAX_MISC_DB_SQL_SIZE;
	KSAVE_ARENA_RECORD_REQUEST*    pRequest    = NULL;
	unsigned long					ulStrLen    = 0;

	LOG_PROCESS_ERROR(pbyData);
	pRequest = (KSAVE_ARENA_RECORD_REQUEST*)pbyData;

	nRetCode = snprintf(
		pszPos, uLeftSize,
		"replace into %s (PlayerID, Record) values (%d, '",
		ARENA_RECORD_TABLE_NAME,
		pRequest->dwRoleId   
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
	pszPos      += nRetCode;
	uLeftSize   -= nRetCode;

	LOG_PROCESS_ERROR(uLeftSize > pRequest->uDataLen * 2);
	nRetCode = mysql_real_escape_string(
		m_pDBHandle, pszPos, (const char*)pRequest->byData, (unsigned long)pRequest->uDataLen
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
	pszPos      += nRetCode;
	uLeftSize   -= nRetCode;

	nRetCode = snprintf(pszPos, uLeftSize, "');");
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
	pszPos      += nRetCode;
	uLeftSize   -= nRetCode;

	nRetCode = DoQuery(m_pszSQL);
	LOG_PROCESS_ERROR(nRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KMiscDB::ProcessLoadArenaRecord(BYTE* pbyData, size_t uDataLen)
{
	BOOL							bResult         = false;
	int								nRetCode        = 0;
	KLOAD_ARENA_RECORD_REQUEST*		pRequest        = (KLOAD_ARENA_RECORD_REQUEST*)pbyData;
	IMemBlock*						piBuffer        = NULL;
	KLOAD_ARENA_RECORD_RESPOND*		pRespond        = NULL;
	unsigned long					uRecordDataLen  = 0;
	MYSQL_RES*						pQueryRes       = NULL;
	int								nRowNum         = 0;
	unsigned long*					pulFieldLength  = NULL;
	MYSQL_ROW						QueryRow        = NULL;


	nRetCode = snprintf(m_pszSQL, MAX_MISC_DB_SQL_SIZE, "select Record from %s where PlayerID = %u;", 
		ARENA_RECORD_TABLE_NAME, pRequest->dwRoleId);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

	nRetCode = DoQuery(m_pszSQL);
	if (!nRetCode)
	{
		goto EXIT1;
	}

	pQueryRes = mysql_store_result(m_pDBHandle);
	if (pQueryRes == NULL)
	{
		goto EXIT1;
	}

	nRowNum = (int)mysql_num_rows(pQueryRes);
	if(nRowNum != 1)
	{
		goto EXIT1;
	}
	
	QueryRow = mysql_fetch_row(pQueryRes);
	if (QueryRow == NULL)
	{
		goto EXIT1;
	}

	pulFieldLength = mysql_fetch_lengths(pQueryRes);
	if (pulFieldLength == NULL)
	{
		goto EXIT1;
	}
	uRecordDataLen = *pulFieldLength;

EXIT1:
	piBuffer = QCreateMemBlock((unsigned)(sizeof(KLOAD_ARENA_RECORD_RESPOND) + uRecordDataLen));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (KLOAD_ARENA_RECORD_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->nProtocolID = emrpLoadArenaRecordRespond;
	pRespond->nConnIndex = pRequest->nConnIndex;
	pRespond->dwRoleId = pRequest->dwRoleId;
	pRespond->uRecordDataLen = uRecordDataLen;

	if (uRecordDataLen > 0)
	{
		memcpy(pRespond->byRecordData, QueryRow[0], uRecordDataLen);
	}

	nRetCode = PushRespond(piBuffer);
	LOG_PROCESS_ERROR(nRetCode);

	bResult = true;
EXIT0:
	if (pQueryRes)
	{
		mysql_free_result(pQueryRes);
		pQueryRes = NULL;
	}
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KMiscDB::ProcessSaveServerRecord(BYTE* pbyData, size_t uDataLen)
{
	BOOL							bResult     = false;
	int								nRetCode    = 0;   
	char*							pszPos      = m_pszSQL;
	size_t							uLeftSize   = MAX_MISC_DB_SQL_SIZE;
	KSAVE_SERVER_RECORD_REQUEST*    pRequest    = NULL;
	unsigned long					ulStrLen    = 0;

	LOG_PROCESS_ERROR(pbyData);
	pRequest = (KSAVE_SERVER_RECORD_REQUEST*)pbyData;

	nRetCode = snprintf(
		pszPos, uLeftSize,
		"replace into %s (RecordID, Record) values (%d, '",
		SERVER_RECORD_TABLE_NAME,
		pRequest->nRecordId   
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
	pszPos      += nRetCode;
	uLeftSize   -= nRetCode;

	LOG_PROCESS_ERROR(uLeftSize > pRequest->uDataLen * 2);
	nRetCode = mysql_real_escape_string(
		m_pDBHandle, pszPos, (const char*)pRequest->byData, (unsigned long)pRequest->uDataLen
		);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
	pszPos      += nRetCode;
	uLeftSize   -= nRetCode;

	nRetCode = snprintf(pszPos, uLeftSize, "');");
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
	pszPos      += nRetCode;
	uLeftSize   -= nRetCode;

	nRetCode = DoQuery(m_pszSQL);
	LOG_PROCESS_ERROR(nRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KMiscDB::ProcessLoadServerRecord(BYTE* pbyData, size_t uDataLen)
{
	BOOL							bResult         = false;
	int								nRetCode        = 0;
	KLOAD_SERVER_RECORD_REQUEST*	pRequest		= (KLOAD_SERVER_RECORD_REQUEST*)pbyData;
	IMemBlock*						piBuffer        = NULL;
	KLOAD_SERVER_RECORD_RESPOND*	pRespond        = NULL;
	unsigned long					uRecordDataLen  = 0;
	MYSQL_RES*						pQueryRes       = NULL;
	int								nRowNum         = 0;
	unsigned long*					pulFieldLength  = NULL;
	MYSQL_ROW						QueryRow        = NULL;


	nRetCode = snprintf(m_pszSQL, MAX_MISC_DB_SQL_SIZE, "select Record from %s where RecordID = %u;", 
		SERVER_RECORD_TABLE_NAME, pRequest->nRecordId);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

	nRetCode = DoQuery(m_pszSQL);
	if (!nRetCode)
	{
		goto EXIT1;
	}

	pQueryRes = mysql_store_result(m_pDBHandle);
	if (pQueryRes == NULL)
	{
		goto EXIT1;
	}

	nRowNum = (int)mysql_num_rows(pQueryRes);
	if(nRowNum != 1)
	{
		goto EXIT1;
	}

	QueryRow = mysql_fetch_row(pQueryRes);
	if (QueryRow == NULL)
	{
		goto EXIT1;
	}

	pulFieldLength = mysql_fetch_lengths(pQueryRes);
	if (pulFieldLength == NULL)
	{
		goto EXIT1;
	}
	uRecordDataLen = *pulFieldLength;

EXIT1:
	piBuffer = QCreateMemBlock((unsigned)(sizeof(KLOAD_SERVER_RECORD_RESPOND) + uRecordDataLen));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (KLOAD_SERVER_RECORD_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->nProtocolID = emrpLoadServerRecordRespond;
	pRespond->nRecordId = pRequest->nRecordId;
	pRespond->uRecordDataLen = uRecordDataLen;

	if (uRecordDataLen > 0)
	{
		memcpy(pRespond->byRecordData, QueryRow[0], uRecordDataLen);
	}

	nRetCode = PushRespond(piBuffer);
	LOG_PROCESS_ERROR(nRetCode);

	bResult = true;
EXIT0:
	if (pQueryRes)
	{
		mysql_free_result(pQueryRes);
		pQueryRes = NULL;
	}
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KMiscDB::ProcessAuctionLookup(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult             = false;
    int                             nRetCode            = 0;
    KAUCTION_LOOKUP_DB_REQUEST*     pRequest            = (KAUCTION_LOOKUP_DB_REQUEST*)pbyData;
    MYSQL_RES*                      pQueryRes           = NULL;
    int                             nRowNum             = 0;
    unsigned long*                  pulFieldLength      = NULL;
    MYSQL_ROW                       QueryRow            = NULL;
    IMemBlock*                     piBuffer            = NULL;
    KAUCTION_LOOKUP_DB_RESPOND*     pRespond            = NULL;
    char*                           pszPos              = NULL;
    size_t                          uLeftBufferSize     = 0;
    BYTE*                           pbyOffset           = NULL;
    size_t                          uLeftSize           = 0;
    const char*                     apszWord[2]         = {"where", "and"};
    int                             nWordIndex          = 0;
    const char*                     apszOrder[aotTotal] = {"Quality", "RequireLevel", "DurationTime", "Price", "BuyItNowPrice"};

    pszPos          = m_pszSQL;
    uLeftBufferSize = MAX_MISC_DB_SQL_SIZE;

    nRetCode = snprintf(
        pszPos, uLeftBufferSize,
        "select sql_calc_found_rows "
        "   ID, SellerID, Price, BuyItNowPrice, BidderID, UNIX_TIMESTAMP(DurationTime), ItemData "
        "from %s ",
        AUCTION_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    if (pRequest->Param.szSaleName[0] != '\0')
    {
        size_t uSaleNameLen = 0;

        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s Name like '%c",
            apszWord[nWordIndex], '%'
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        uSaleNameLen = strlen(pRequest->Param.szSaleName);
        LOG_PROCESS_ERROR(uLeftBufferSize >= uSaleNameLen * 2 + 1);
        nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, pRequest->Param.szSaleName, (unsigned long)uSaleNameLen);
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nRetCode = snprintf(pszPos, uLeftBufferSize, "%c' ", '%');
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.nAucGenre != 0)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s AucGenre = %d ",
            apszWord[nWordIndex], pRequest->Param.nAucGenre
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.nAucSub != 0)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s AucSub = %d ",
            apszWord[nWordIndex], pRequest->Param.nAucSub
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.nLevelMin != 0)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s RequireLevel >= %d ",
            apszWord[nWordIndex], pRequest->Param.nLevelMin
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.nLevelMax != 0)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s RequireLevel <= %d ",
            apszWord[nWordIndex], pRequest->Param.nLevelMax
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.nQuality != -1)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s Quality <= %d ",
            apszWord[nWordIndex], pRequest->Param.nQuality
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.szSellerName[0] != '\0')
    {
        size_t uSellerNameLen = 0;

        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s SellerName like '%c",
            apszWord[nWordIndex], '%'
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        uSellerNameLen = strlen(pRequest->Param.szSellerName);
        LOG_PROCESS_ERROR(uLeftBufferSize >= uSellerNameLen * 2 + 1);
        nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, pRequest->Param.szSellerName, (unsigned long)uSellerNameLen);
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nRetCode = snprintf(pszPos, uLeftBufferSize, "%c' ", '%');
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.dwSellerID != KD_BAD_ID)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s SellerID = %u ",
            apszWord[nWordIndex], pRequest->Param.dwSellerID
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.dwBidderID != KD_BAD_ID)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s BidderID = %u ",
            apszWord[nWordIndex], pRequest->Param.dwBidderID
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.nTopPrice != 0)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s Price <= %d ",
            apszWord[nWordIndex], pRequest->Param.nTopPrice
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    if (pRequest->Param.byAuction == assSomeoneBid)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s BidderID != 0 ",
            apszWord[nWordIndex]
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }
    else if (pRequest->Param.byAuction == assNoOneBid)
    {
        nRetCode = snprintf(
            pszPos, uLeftBufferSize,
            "%s BidderID = 0 ",
            apszWord[nWordIndex]
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
        pszPos          += nRetCode;
        uLeftBufferSize -= nRetCode;

        nWordIndex = 1;
    }

    nRetCode = snprintf(
        pszPos, uLeftBufferSize,
        " order by %s %s limit %d, %d;",
        apszOrder[pRequest->Param.byOrderType], pRequest->Param.byDesc ? "desc" : "",
        pRequest->Param.nStartIndex, SYNC_PAGE_MAX_COUNT
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);

    nRetCode = DoQuery(m_pszSQL);
    if (!nRetCode)
    {
        goto EXIT1;
    }

    pQueryRes = mysql_store_result(m_pDBHandle);
    if (pQueryRes == NULL)
    {
        goto EXIT1;
    }

    nRowNum = (int)mysql_num_rows(pQueryRes);

EXIT1:
    piBuffer = QCreateMemBlock(SYNC_SALE_LIST_MAX_LEN);
    LOG_PROCESS_ERROR(piBuffer);

    pbyOffset = (BYTE*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pbyOffset);

    uLeftSize = (size_t)piBuffer->GetSize();

    LOG_PROCESS_ERROR(uLeftSize >= sizeof(KAUCTION_LOOKUP_DB_RESPOND));
    pRespond = (KAUCTION_LOOKUP_DB_RESPOND*)pbyOffset;
    pbyOffset += sizeof(KAUCTION_LOOKUP_DB_RESPOND);
    uLeftSize -= sizeof(KAUCTION_LOOKUP_DB_RESPOND);

    for (int i = 0; i < nRowNum; i++)
    {
        KAUCTION_PAGE_NODE* pNode       = NULL;
        size_t              uItemSize   = 0;
        int                 nLeftTime   = 0;

        QueryRow = mysql_fetch_row(pQueryRes);
        if (QueryRow == NULL)
        {
            break;
        }

        pulFieldLength = mysql_fetch_lengths(pQueryRes);
        if (pulFieldLength == NULL)
        {
            break;
        }

        uItemSize = pulFieldLength[6];

        LOG_PROCESS_ERROR(uLeftSize >= sizeof(KAUCTION_PAGE_NODE) + uItemSize);
        pNode = (KAUCTION_PAGE_NODE*)pbyOffset;
        pbyOffset += sizeof(KAUCTION_PAGE_NODE) + uItemSize;
        uLeftSize -= sizeof(KAUCTION_PAGE_NODE) + uItemSize;

        pNode->dwSaleID         = strtoul(QueryRow[0], NULL, 10);
        pNode->dwSellerID       = strtoul(QueryRow[1], NULL, 10);
        pNode->szSellerName[0]  = '\0';
        pNode->nPrice           = strtol(QueryRow[2], NULL, 10);
        pNode->nBuyItNowPrice   = strtol(QueryRow[3], NULL, 10);
        pNode->dwBidderID       = strtoul(QueryRow[4], NULL, 10);
        pNode->szBidderName[0]  = '\0';

        nLeftTime               = strtol(QueryRow[5], NULL, 10) - g_pGameCenter->m_nTimeNow;
        pNode->nLeftTime        = MAX(nLeftTime, 0);

        pNode->uItemSize        = uItemSize;
        memcpy(pNode->byItemData, QueryRow[6], pNode->uItemSize);
    }

    pRespond->nProtocolID       = emrpAuctionLookupRespond;
    pRespond->byRespondID       = pRequest->byRequestID;
    pRespond->dwPlayerID        = pRequest->dwPlayerID;
    pRespond->Page.uSize        = SYNC_SALE_LIST_MAX_LEN - uLeftSize - sizeof(KAUCTION_LOOKUP_DB_RESPOND);
    pRespond->Page.nTotalCount  = SelectFoundRows();

    nRetCode = PushRespond(piBuffer);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

BOOL KMiscDB::ProcessAuctionBid(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult         = false;
    int                         nRetCode        = 0;
    KAUCTION_BID_DB_REQUEST*    pRequest        = (KAUCTION_BID_DB_REQUEST*)pbyData;
    MYSQL_RES*                  pQueryRes       = NULL;
    int                         nRowNum         = 0;
    unsigned long*              pulFieldLength  = NULL;
    MYSQL_ROW                   QueryRow        = NULL;
    IMemBlock*                 piBuffer        = NULL;
    KAUCTION_BID_DB_RESPOND*    pRespond        = NULL;
    BOOL                        bSelect         = false;
    BYTE                        byCode          = arcUnknownError;
    char*                       pszSaleName     = NULL;
    DWORD                       dwSellerID      = KD_BAD_ID;
    int                         nPrice          = 0;
    int                         nBuyItNowPrice  = 0;
    DWORD                       dwBidderID      = 0;
    int                         nCustodyCharges = 0;
    BYTE*                       pbyItemData     = NULL;
    size_t                      uItemDataLen    = 0;
    DWORD                       dwCRC           = 0;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "select "
        "   Name, SellerID, Price, BuyItNowPrice, BidderID, CustodyCharges, ItemData "
        "from %s "
        "where "
        "   ID = %u; ",
        AUCTION_TABLE_NAME,
        pRequest->dwSaleID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    if (!nRetCode)
    {
        goto EXIT1;
    }

    pQueryRes = mysql_store_result(m_pDBHandle);
    if (pQueryRes == NULL)
    {
        goto EXIT1;
    }

    nRowNum = (int)mysql_num_rows(pQueryRes);
    if (nRowNum != 1 && nRowNum != 0)
    {
        goto EXIT1;
    }

    if (nRowNum == 1)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (QueryRow == NULL)
        {
            goto EXIT1;
        }

        pulFieldLength = mysql_fetch_lengths(pQueryRes);
        if (pulFieldLength == NULL)
        {
            goto EXIT1;
        }

        pszSaleName     = QueryRow[0];

        dwSellerID      = strtoul(QueryRow[1], NULL, 10);
        nPrice          = strtol(QueryRow[2], NULL, 10);
        nBuyItNowPrice  = strtol(QueryRow[3], NULL, 10);
        dwBidderID      = strtoul(QueryRow[4], NULL, 10);
        nCustodyCharges = strtol(QueryRow[5], NULL, 10);
        pbyItemData     = (BYTE*)QueryRow[6];
        uItemDataLen    = pulFieldLength[6];
        //dwCRC           = (DWORD)Misc_CRC32(0, pbyItemData, (unsigned)uItemDataLen);
		dwCRC = ngx_crc32_long((unsigned char*)pbyItemData, uItemDataLen);

        bSelect = true;
    }

    if (!bSelect)
    {
        byCode = arcItemNotExist;
        goto EXIT1;
    }

    if (pRequest->dwCRC != dwCRC)
    {
        byCode = arcUnknownError;
        goto EXIT1;
    }

    if (
        (pRequest->nPrice < nPrice) ||
        (dwBidderID != KD_BAD_ID && pRequest->nPrice == nPrice))
    {
        byCode = arcPriceTooLow;
        goto EXIT1;
    }

    if (pRequest->nPrice >= nBuyItNowPrice)
    {
        nRetCode = snprintf(
            m_pszSQL, MAX_MISC_DB_SQL_SIZE,
            " delete from %s where ID = %u; ",
            AUCTION_TABLE_NAME, pRequest->dwSaleID
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
    }
    else
    {
        nRetCode = snprintf(
            m_pszSQL, MAX_MISC_DB_SQL_SIZE,
            " update %s set Price = %d, BidderID = %u where ID = %u; ",
            AUCTION_TABLE_NAME, pRequest->nPrice, pRequest->dwBidderID, pRequest->dwSaleID
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
    }

    nRetCode = DoQuery(m_pszSQL);
    if (!nRetCode)
    {
        goto EXIT1;
    }

    byCode = arcSucceed;
EXIT1:
    piBuffer = QCreateMemBlock((unsigned)(sizeof(KAUCTION_BID_DB_RESPOND) + uItemDataLen));
    LOG_PROCESS_ERROR(piBuffer);

    pRespond = (KAUCTION_BID_DB_RESPOND*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRespond);

    pRespond->nProtocolID       = emrpAuctionBidRespond;
    pRespond->byCode            = byCode;
    pRespond->dwSellerID        = dwSellerID;
    pRespond->nSellPrice        = nBuyItNowPrice;
    pRespond->dwBidderID        = pRequest->dwBidderID;
    pRespond->nBidPrice         = pRequest->nPrice;
    pRespond->dwOldBidderID     = dwBidderID;
    pRespond->nOldBidPrice      = nPrice;

    pRespond->szSaleName[0]     = '\0';
    if (pszSaleName)
    {
        strncpy(pRespond->szSaleName, pszSaleName, sizeof(pRespond->szSaleName));
        pRespond->szSaleName[sizeof(pRespond->szSaleName) - 1] = '\0';
    }

    pRespond->nCustodyCharges   = nCustodyCharges;
    pRespond->uItemDataLen      = uItemDataLen;
    if (pbyItemData != NULL)
    {
        memcpy(pRespond->byItemData, pbyItemData, uItemDataLen);
    }

    nRetCode = PushRespond(piBuffer);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::ProcessAuctionSell(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult     = false;
    int                         nRetCode    = false;
    BYTE                        byCode      = arcUnknownError;
    KAUCTION_SELL_DB_REQUEST*   pRequest    = (KAUCTION_SELL_DB_REQUEST*)pbyData;
    IMemBlock*                 piBuffer    = NULL;
    KAUCTION_SELL_DB_RESPOND*   pRespond    = NULL;
    char*                       pszPos      = NULL;
    size_t                      uLeftSize   = 0;
    unsigned long               uLen        = 0;

    LOG_PROCESS_ERROR(pRequest->uItemDataLen > 0);

    pszPos      = m_pszSQL;
    uLeftSize   = MAX_MISC_DB_SQL_SIZE;

    nRetCode = snprintf(
        pszPos, uLeftSize,
        " insert into %s "
        "   (Name, AucGenre, AucSub, RequireLevel, Quality, SellerID, SellerName, Price, BuyItNowPrice, "
        "   BidderID, CustodyCharges, DurationTime, ItemData) "
        " values "
        "   ('",
        AUCTION_TABLE_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    uLen = (unsigned long)strlen(pRequest->szSaleName);

    LOG_PROCESS_ERROR(uLeftSize > uLen * 2);
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, pRequest->szSaleName, uLen);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = snprintf(
        pszPos, uLeftSize,
        "', %d, %d, %d, %d, %u, '",
        pRequest->nAucGenre, pRequest->nAucSub, pRequest->nRequireLevel,
        pRequest->nQuality, pRequest->dwSellerID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    uLen = (unsigned long)strlen(pRequest->szSellerName);

    LOG_PROCESS_ERROR(uLeftSize > uLen * 2);
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, pRequest->szSellerName, uLen);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = snprintf(
        pszPos, uLeftSize,
        "', %d, %d, %u, %d, FROM_UNIXTIME(%ld), '",
        pRequest->nStartPrice, pRequest->nBuyItNowPrice,
        KD_BAD_ID, pRequest->nCustodyCharges, pRequest->nDurationTime
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    LOG_PROCESS_ERROR(uLeftSize > pRequest->uItemDataLen * 2);
    nRetCode = mysql_real_escape_string(
        m_pDBHandle, pszPos, (const char*)pRequest->byItemData, (unsigned long)pRequest->uItemDataLen
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftSize, "');");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);

    nRetCode = DoQuery(m_pszSQL);
    if (!nRetCode)
    {
        QLogPrintf(LOG_ERR, "[Auction]Role %s sell %s failed!", pRequest->szSellerName, pRequest->szSaleName);
        goto EXIT1;
    }

    byCode = arcSucceed;
EXIT1:
    piBuffer = QCreateMemBlock((unsigned)sizeof(KAUCTION_SELL_DB_RESPOND));
    LOG_PROCESS_ERROR(piBuffer);

    pRespond = (KAUCTION_SELL_DB_RESPOND*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRespond);

    pRespond->nProtocolID   = emrpAuctionSellRespond;
    pRespond->byCode        = byCode;

    nRetCode = PushRespond(piBuffer);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::ProcessAuctionCancel(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult         = false;
    int                         nRetCode        = false;
    KAUCTION_CANCEL_DB_REQUEST* pRequest        = (KAUCTION_CANCEL_DB_REQUEST*)pbyData;
    IMemBlock*                 piBuffer        = NULL;
    KAUCTION_CANCEL_DB_RESPOND* pRespond        = NULL;
    MYSQL_RES*                  pQueryRes       = NULL;
    int                         nRowNum         = 0;
    unsigned long*              pulFieldLength  = NULL;
    MYSQL_ROW                   QueryRow        = NULL;
    BYTE                        byCode          = arcUnknownError;
    DWORD                       dwSellerID      = KD_BAD_ID;
    char*                       pszSaleName     = NULL;
    DWORD                       dwBidderID      = KD_BAD_ID;
    BYTE*                       pbyItemData     = NULL;
    size_t                      uItemDataLen    = 0;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        " select "
        "   Name, SellerID, BidderID, ItemData "
        " from %s "
        " where ID = %u;",
        AUCTION_TABLE_NAME, pRequest->dwSaleID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    if (!nRetCode)
    {
        goto EXIT1;
    }

    pQueryRes = mysql_store_result(m_pDBHandle);
    if (pQueryRes == NULL)
    {
        goto EXIT1;
    }

    nRowNum = (int)mysql_num_rows(pQueryRes);
    if (nRowNum != 0 && nRowNum != 1)
    {
        goto EXIT1;
    }

    if (nRowNum == 0)
    {
        byCode = arcItemNotExist;
        goto EXIT1;
    }

    QueryRow = mysql_fetch_row(pQueryRes);
    if (QueryRow == NULL)
    {
        goto EXIT1;
    }

    pulFieldLength = mysql_fetch_lengths(pQueryRes);
    if (pulFieldLength == NULL)
    {
        goto EXIT1;
    }
    
    pszSaleName     = QueryRow[0];
    dwSellerID      = strtoul(QueryRow[1], NULL, 10);
    dwBidderID      = strtoul(QueryRow[2], NULL, 10);
    pbyItemData     = (BYTE*)QueryRow[3];
    uItemDataLen    = pulFieldLength[3];

    if (dwSellerID != pRequest->dwSellerID)
    {
        goto EXIT1;
    }

    if (dwBidderID != KD_BAD_ID)
    {
         byCode = arcCannotCancel;
         goto EXIT1;
    }

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "delete from %s where ID = %u; ", AUCTION_TABLE_NAME, pRequest->dwSaleID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    if (!nRetCode)
    {
        goto EXIT1;
    }

    byCode = arcSucceed;
EXIT1:
    piBuffer = QCreateMemBlock((unsigned)(sizeof(KAUCTION_CANCEL_DB_RESPOND) + uItemDataLen));
    LOG_PROCESS_ERROR(piBuffer);

    pRespond = (KAUCTION_CANCEL_DB_RESPOND*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRespond);

    pRespond->nProtocolID   = emrpAuctionCancelRespond;
    pRespond->byCode        = byCode;
    pRespond->dwSellerID    = pRequest->dwSellerID;

    pRespond->szSaleName[0] = '\0';
    if (pszSaleName)
    {
        strncpy(pRespond->szSaleName, pszSaleName, sizeof(pRespond->szSaleName));
        pRespond->szSaleName[sizeof(pRespond->szSaleName) - 1] = '\0';
    }

    pRespond->uItemDataLen  = uItemDataLen;

    if (uItemDataLen > 0)
    {
        memcpy(pRespond->byItemData, pbyItemData, uItemDataLen);
    }

    nRetCode = PushRespond(piBuffer);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::ProcessAuctionCheckDuration(BYTE* pbyData, size_t uDataLen)
{
    BOOL                                bResult         = false;
    int                                 nRetCode        = false;
    KAUCTION_CHECK_DURATION_REQUEST*    pRequest        = (KAUCTION_CHECK_DURATION_REQUEST*)pbyData;
    IMemBlock*                         piBuffer        = NULL;
    KAUCTION_CHECK_DURATION_RESPOND*    pRespond        = NULL;
    MYSQL_RES*                          pQueryRes       = NULL;
    MYSQL_ROW                           QueryRow        = NULL;
    unsigned long*                      pulFieldLength  = NULL;
    char*                               pszSaleName     = NULL;
    size_t                              uItemDataLen    = 0;
    BYTE*                               pbyItemData     = NULL;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        " select "
        "   Name, SellerID, Price, BuyItNowPrice, BidderID, CustodyCharges, ItemData "
        " from %s "
        " where "
        "   DurationTime <= FROM_UNIXTIME(%ld); ",
        AUCTION_TABLE_NAME, pRequest->nTimeNow
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pQueryRes = mysql_use_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    while (true)
    {
        QueryRow = mysql_fetch_row(pQueryRes);
        if (QueryRow == NULL)
        {
            break;
        }

        pulFieldLength = mysql_fetch_lengths(pQueryRes);
        LOG_PROCESS_ERROR(pulFieldLength);

        uItemDataLen = pulFieldLength[6];

        SAFE_RELEASE(piBuffer);

        piBuffer = QCreateMemBlock((unsigned)(sizeof(KAUCTION_CHECK_DURATION_RESPOND) + uItemDataLen));
        LOG_PROCESS_ERROR(piBuffer);

        pRespond = (KAUCTION_CHECK_DURATION_RESPOND*)piBuffer->GetData();
        LOG_PROCESS_ERROR(pRespond);

        pRespond->nProtocolID       = emrpAuctionCheckDurationRespond;

        strncpy(pRespond->szSaleName, QueryRow[0], sizeof(pRespond->szSaleName));
        pRespond->szSaleName[sizeof(pRespond->szSaleName) - 1] = '\0';

        pRespond->dwSellerID        = strtoul(QueryRow[1], NULL, 10);
        pRespond->nPrice            = strtol(QueryRow[2], NULL, 10);
        pRespond->nBuyItNowPrice    = strtol(QueryRow[3], NULL, 10);
        pRespond->dwBidderID        = strtoul(QueryRow[4], NULL, 10);
        pRespond->nCustodyCharges   = strtol(QueryRow[5], NULL, 10);
        pRespond->uItemDataLen      = uItemDataLen;

        memcpy(pRespond->byItemData, QueryRow[6], uItemDataLen);

        nRetCode = PushRespond(piBuffer);
        LOG_PROCESS_ERROR(nRetCode);
    }

    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        " delete from %s where DurationTime <= FROM_UNIXTIME(%ld); ",
        AUCTION_TABLE_NAME, pRequest->nTimeNow
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::ProcessSaveGlobalCustomData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                                bResult         = false;
    int                                 nRetCode        = 0;   
    char*                               pszPos          = m_pszSQL;
    size_t                              uLeftSize       = MAX_MISC_DB_SQL_SIZE;
    KSAVE_GLOBAL_CUSTOM_DATA_REQUEST*   pRequest        = (KSAVE_GLOBAL_CUSTOM_DATA_REQUEST*)pbyData;
    size_t                              uCustomDataLen  = 0;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "replace into %s (Name, Value) values (\"%s\", '",
        GLOBAL_CUSTOM_DATA_TABLE_NAME,
        pRequest->szName
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    uCustomDataLen = uDataLen - sizeof(KSAVE_GLOBAL_CUSTOM_DATA_REQUEST);
    LOG_PROCESS_ERROR(uLeftSize > uCustomDataLen * 2);
    nRetCode = mysql_real_escape_string(
        m_pDBHandle, pszPos, (const char*)pRequest->byData, (unsigned long)uCustomDataLen
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftSize, "');");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult; 
}

BOOL KMiscDB::ProcessDeleteGlobalCustomData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                                bResult  = false;
    int                                 nRetCode = 0;
    KDELETE_GLOBAL_CUSTOM_DATA_REQUEST* pRequest = (KDELETE_GLOBAL_CUSTOM_DATA_REQUEST*)pbyData;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "delete from %s where Name = \"%s\"",
        GLOBAL_CUSTOM_DATA_TABLE_NAME, 
        pRequest->szName
	);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::ProcessSaveGlobalSystemValue(BYTE* pbyData, size_t uDataLen)
{
    BOOL                                bResult  = false;
    int                                 nRetCode = 0;
    KSAVE_GLOBAL_SYSTEM_VALUE_REQUEST*  pRequest = (KSAVE_GLOBAL_SYSTEM_VALUE_REQUEST*)pbyData;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        " replace into %s "
        "   (Name, Value) "
        " values "
        "   ('%s', %d); ",
        GLOBAL_SYSTEM_VALUE_TABLE_NAME, 
        pRequest->szName, pRequest->nValue
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

// 统计数据
BOOL KMiscDB::ProcessSaveStatData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult         = false;
    int                         nRetCode        = 0;
    char*                       pszPos          = m_pszSQL;
    size_t                      uLeftSize       = MAX_MISC_DB_SQL_SIZE;
    size_t                      uStatDataLen    = 0;
    KSAVE_STAT_DATA_REQUEST*    pRequest        = (KSAVE_STAT_DATA_REQUEST*)pbyData;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "replace into %s (StatTime, StatData) values (FROM_UNIXTIME(%ld), '",
        STAT_DATA_TABLE_NAME,
        pRequest->nStatTime
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    uStatDataLen = uDataLen - sizeof(KSAVE_STAT_DATA_REQUEST);
    LOG_PROCESS_ERROR(uLeftSize > uStatDataLen * 2);

    nRetCode = mysql_real_escape_string(
        m_pDBHandle, pszPos, (const char*)pRequest->byDataDB, (unsigned long)uStatDataLen
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszPos      += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftSize, "');");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult; 
}

BOOL KMiscDB::ProcessSaveStatDataName(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult         = false;
    int                             nRetCode        = 0;
    KSAVE_STAT_DATA_NAME_REQUEST*   pRequest        = (KSAVE_STAT_DATA_NAME_REQUEST*)pbyData;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        "insert into %s (Name, ID) values (\"%s\", \"%d\");",
        STAT_DATA_NAME_INDEX_TABLE_NAME,
        pRequest->szName,
        pRequest->nNameID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult; 
}

//BOOL KMiscDB::ProcessUpdateActivityData(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL                            bResult         = false;
//    int                             nRetCode        = 0;
//    KUPDATE_ACTIVITY_DATA_REQUEST*  pRequest        = (KUPDATE_ACTIVITY_DATA_REQUEST*)pbyData;
//
//    nRetCode = snprintf(
//        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
//        " replace into %s "
//        "   (ID, Type, Time, Value0, Value1, Value2, Value3, Value4, Value5) "
//        " values "
//        "   (%lu, %d, FROM_UNIXTIME(%ld), %d, %d, %d, %d, %d, %d);",
//        ACTIVITY_TABLE_NAME,
//        pRequest->Data.dwRoleID, pRequest->Data.nType, pRequest->Data.nEndTime,
//        pRequest->Data.anValue[0], pRequest->Data.anValue[1], pRequest->Data.anValue[2],
//        pRequest->Data.anValue[3], pRequest->Data.anValue[4], pRequest->Data.anValue[5]
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
//
//    nRetCode = DoQuery(m_pszSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    bResult = true;
//EXIT0:
//    return bResult; 
//}

BOOL KMiscDB::ProcessDeleteActivityData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult         = false;
    int                             nRetCode        = 0;
    KDELETE_ACTIVITY_DATE_REQUEST*  pRequest        = (KDELETE_ACTIVITY_DATE_REQUEST*)pbyData;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        " delete from %s where ID = %u and Type = %d; ",
        ACTIVITY_TABLE_NAME,
        pRequest->dwRoleID, pRequest->nType
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult; 
}

BOOL KMiscDB::ProcessGameCardSell(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult     = false;
    int                         nRetCode    = 0;
    KGAME_CARD_SELL_REQUEST*    pRequest    = (KGAME_CARD_SELL_REQUEST*)pbyData;

    nRetCode = snprintf(
        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
        " insert into %s "
        "   (SellerID, Coin, GameTime, Type, Price, EndTime) "
        " values "
        "   (%u, %d, %d, %d, %d, FROM_UNIXTIME(%lu)); ",
        GAME_CARD_TABLE_NAME,
        pRequest->dwSellerID, pRequest->nCoin, pRequest->nGameTime,
        (int)pRequest->byType, pRequest->nPrice, (uint64_t)pRequest->nEndTime
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    return bResult; 
}

//BOOL KMiscDB::ProcessGameCardBuy(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL                        bResult         = false;
//    int                         nRetCode        = 0;
//    KGAME_CARD_BUY_REQUEST*     pRequest        = (KGAME_CARD_BUY_REQUEST*)pbyData;
//    MYSQL_RES*                  pQueryRes       = NULL;
//    int                         nRowNum         = 0;
//    unsigned long*              pulFieldLength  = NULL;
//    MYSQL_ROW                   QueryRow        = NULL;
//    IMemBlock*                 piBuffer        = NULL;
//    KGAME_CARD_BUY_RESPOND*     pRespond        = NULL;
//    BYTE                        byCode          = gorUnknownError;
//    DWORD                       dwSellerID      = KD_BAD_ID;
//    int                         nCoin           = 0;
//    int                         nGameTime       = 0;
//    BYTE                        byType          = 0;
//    int                         nPrice          = 0;
//
//    nRetCode = snprintf(
//        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
//        " select "
//        "   SellerID, Coin, GameTime, Type, Price "
//        " from %s where ID = %lu; ",
//        GAME_CARD_TABLE_NAME, pRequest->dwID
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
//
//    nRetCode = DoQuery(m_pszSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    pQueryRes = mysql_store_result(m_pDBHandle);
//    LOG_PROCESS_ERROR(pQueryRes);
//
//    nRowNum = (int)mysql_num_rows(pQueryRes);
//    LOG_PROCESS_ERROR(nRowNum == 1 || nRowNum == 0);
//
//    if (nRowNum == 1)
//    {
//        QueryRow = mysql_fetch_row(pQueryRes);
//        LOG_PROCESS_ERROR(QueryRow);
//
//        pulFieldLength = mysql_fetch_lengths(pQueryRes);
//        LOG_PROCESS_ERROR(pulFieldLength);
//
//        dwSellerID  = strtoul(QueryRow[0], NULL, 10);
//        nCoin       = strtol(QueryRow[1], NULL, 10);
//        nGameTime   = strtol(QueryRow[2], NULL, 10);
//        byType      = (BYTE)strtoul(QueryRow[3], NULL, 10);
//        nPrice      = strtol(QueryRow[4], NULL, 10);
//
//        if (nPrice == pRequest->nPrice)
//        {
//            byCode = gorSucceed;
//        }
//    }
//
//    if (byCode == gorSucceed)
//    {
//        nRetCode = snprintf(
//            m_pszSQL, MAX_MISC_DB_SQL_SIZE,
//            " delete from %s where ID = %ld; ",
//            GAME_CARD_TABLE_NAME, pRequest->dwID
//        );
//        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
//
//        nRetCode = DoQuery(m_pszSQL);
//        LOG_PROCESS_ERROR(nRetCode);
//    }
//
//    piBuffer = QCreateMemBlock((unsigned)sizeof(KGAME_CARD_BUY_RESPOND));
//    LOG_PROCESS_ERROR(piBuffer);
//
//    pRespond = (KGAME_CARD_BUY_RESPOND*)piBuffer->GetData();
//    LOG_PROCESS_ERROR(pRespond);
//
//    pRespond->nProtocolID   = emrpGameCardBuyRespond;
//    pRespond->byCode        = byCode;
//    pRespond->dwSellerID    = dwSellerID;
//    pRespond->nCoin         = nCoin;
//    pRespond->nGameTime     = nGameTime;
//    pRespond->byType        = byType;
//    pRespond->nPrice        = pRequest->nPrice;
//    pRespond->dwBuyerID     = pRequest->dwBuyerID;
//
//    nRetCode = PushRespond(piBuffer);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    bResult = true;
//EXIT0:
//    if (pQueryRes)
//    {
//        mysql_free_result(pQueryRes);
//        pQueryRes = NULL;
//    }
//    SAFE_RELEASE(piBuffer);
//    return bResult; 
//}
//
//BOOL KMiscDB::ProcessGameCardLookup(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL                        bResult         = false;
//    int                         nRetCode        = false;
//    KGAME_CARD_LOOKUP_REQUEST*  pRequest        = (KGAME_CARD_LOOKUP_REQUEST*)pbyData;
//    MYSQL_RES*                  pQueryRes       = NULL;
//    int                         nRowNum         = 0;
//    unsigned long*              pulFieldLength  = NULL;
//    MYSQL_ROW                   QueryRow        = NULL;
//    KGAME_CARD_LOOKUP_RESPOND*  pRespond        = NULL;
//    IMemBlock*                 piBuffer        = NULL;
//    KGAME_CARD*                 pCardList       = NULL;
//    const char*                 apszOrder[gotTotal] = {"GameTime", "EndTime", "Price"};
//    
//    LOG_PROCESS_ERROR(pRequest->byOrderType < gotTotal);
//
//    nRetCode = snprintf(
//        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
//        " select sql_calc_found_rows "
//        "   ID, SellerID, GameTime, Type, Price, UNIX_TIMESTAMP(EndTime) "
//        " from %s "
//        " where Type = %d "
//        " order by %s %s "
//        " limit %d, %d;",
//        GAME_CARD_TABLE_NAME,
//        pRequest->byCardType,
//        apszOrder[pRequest->byOrderType], pRequest->bDesc ? "desc" : "",
//        pRequest->nStartIndex, MAX_SYNC_GAME_CARD_COUNT
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
//
//    nRetCode = DoQuery(m_pszSQL);
//    if (!nRetCode)
//    {
//        goto EXIT1;
//    }
//
//    pQueryRes = mysql_store_result(m_pDBHandle);
//    if (pQueryRes == NULL)
//    {
//        goto EXIT1;
//    }
//
//    nRowNum = (int)mysql_num_rows(pQueryRes);
//
//EXIT1:
//    piBuffer = QCreateMemBlock((unsigned)sizeof(KGAME_CARD_LOOKUP_RESPOND) + sizeof(KGAME_CARD) * nRowNum);
//    LOG_PROCESS_ERROR(piBuffer);
//
//    pRespond = (KGAME_CARD_LOOKUP_RESPOND*)piBuffer->GetData();
//    LOG_PROCESS_ERROR(pRespond);
//
//    pCardList = pRespond->CardList;
//
//    for (int i = 0; i < nRowNum; i++)
//    {
//        time_t  nEndTime    = 0;
//
//        QueryRow = mysql_fetch_row(pQueryRes);
//        LOG_PROCESS_ERROR(QueryRow);
//
//        pulFieldLength = mysql_fetch_lengths(pQueryRes);
//        LOG_PROCESS_ERROR(pulFieldLength);
//
//        pCardList[i].dwID = strtoul(QueryRow[0], NULL, 10);
//
//        pCardList[i].dwSellerID = strtoul(QueryRow[1], NULL, 10);
//        pCardList[i].szSellerName[0] = '\0';
//        pCardList[i].nGameTime = strtol(QueryRow[2], NULL, 10);
//        pCardList[i].byType = (BYTE)strtoul(QueryRow[3], NULL, 10);
//        pCardList[i].nPrice = strtol(QueryRow[4], NULL, 10);
//
//        nEndTime = (time_t)strtol(QueryRow[5], NULL, 10);
//        pCardList[i].nLeftTime = 0;
//        if (nEndTime > g_pGameCenter->m_nTimeNow)
//        {
//            pCardList[i].nLeftTime = (int)(nEndTime - g_pGameCenter->m_nTimeNow);
//        }
//    }
//
//    pRespond->nProtocolID   = emrpGameCardLookupRespond;
//    pRespond->dwPlayerID    = pRequest->dwPlayerID;
//    pRespond->nTotalCount   = SelectFoundRows();
//    pRespond->nCount        = nRowNum;
//    
//    nRetCode = PushRespond(piBuffer);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    bResult = true;
//EXIT0:
//    if (pQueryRes)
//    {
//        mysql_free_result(pQueryRes);
//        pQueryRes = NULL;
//    }
//    SAFE_RELEASE(piBuffer);
//    return bResult;
//}
//
//BOOL KMiscDB::ProcessGameCardCancel(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL                        bResult         = false;
//    int                         nRetCode        = 0;
//    KGAME_CARD_CANCEL_REQUEST*  pRequest        = (KGAME_CARD_CANCEL_REQUEST*)pbyData;
//    IMemBlock*                 piBuffer        = NULL;
//    KGAME_CARD_CANCEL_RESPOND*  pRespond        = NULL;
//    MYSQL_RES*                  pQueryRes       = NULL;
//    int                         nRowNum         = 0;
//    unsigned long*              pulFieldLength  = NULL;
//    MYSQL_ROW                   QueryRow        = NULL;
//    BYTE                        byCode          = gorUnknownError;
//    int                         nCoin           = 0;
//
//    nRetCode = snprintf(
//        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
//        " select Coin from %s where ID = %lu and SellerID = %lu; ",
//        GAME_CARD_TABLE_NAME,
//        pRequest->dwID, pRequest->dwPlayerID
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
//
//    nRetCode = DoQuery(m_pszSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    pQueryRes = mysql_store_result(m_pDBHandle);
//    LOG_PROCESS_ERROR(pQueryRes);
//
//    nRowNum = (int)mysql_num_rows(pQueryRes);
//    LOG_PROCESS_ERROR(nRowNum == 1 || nRowNum == 0);
//
//    if (nRowNum == 1)
//    {
//        QueryRow = mysql_fetch_row(pQueryRes);
//        LOG_PROCESS_ERROR(QueryRow);
//
//        pulFieldLength = mysql_fetch_lengths(pQueryRes);
//        LOG_PROCESS_ERROR(pulFieldLength);
//
//        byCode  = gorSucceed;
//        nCoin   = strtol(QueryRow[0], NULL, 10);
//    }
//
//    piBuffer = QCreateMemBlock((unsigned)sizeof(KGAME_CARD_CANCEL_RESPOND));
//    LOG_PROCESS_ERROR(piBuffer);
//
//    pRespond = (KGAME_CARD_CANCEL_RESPOND*)piBuffer->GetData();
//    LOG_PROCESS_ERROR(pRespond);
//
//    pRespond->nProtocolID   = emrpGameCardCancelRespond;
//    pRespond->byCode        = byCode;
//    pRespond->dwPlayerID    = pRequest->dwPlayerID;
//    pRespond->nCoin         = nCoin;
//
//    if (byCode == gorSucceed)
//    {
//        nRetCode = snprintf(
//            m_pszSQL, MAX_MISC_DB_SQL_SIZE,
//            " delete from %s where ID = %lu; ",
//            GAME_CARD_TABLE_NAME, pRequest->dwID
//        );
//        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
//
//        nRetCode = DoQuery(m_pszSQL);
//        LOG_PROCESS_ERROR(nRetCode);
//    }
//
//    nRetCode = PushRespond(piBuffer);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    bResult = true;
//EXIT0:
//    if (pQueryRes)
//    {
//        mysql_free_result(pQueryRes);
//        pQueryRes = NULL;
//    }
//    SAFE_RELEASE(piBuffer);
//    return bResult;
//}
//
//BOOL KMiscDB::ProcessGameCardCheckDuration(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL                                bResult         = false;
//    int                                 nRetCode        = false;
//    KGAME_CARD_CHECK_DURATION_REQUEST*  pRequest        = (KGAME_CARD_CHECK_DURATION_REQUEST*)pbyData;
//    IMemBlock*                         piBuffer        = NULL;
//    KGAME_CARD_CHECK_DURATION_RESPOND*  pRespond        = NULL;
//    MYSQL_RES*                          pQueryRes       = NULL;
//    int                                 nRowNum         = 0;
//    MYSQL_ROW                           QueryRow        = NULL;
//    unsigned long*                      pulFieldLength  = NULL;
//
//    nRetCode = snprintf(
//        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
//        " select SellerID, Coin from %s where EndTime <= FROM_UNIXTIME(%ld); ",
//        GAME_CARD_TABLE_NAME, pRequest->nTimeNow
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
//
//    nRetCode = DoQuery(m_pszSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    pQueryRes = mysql_store_result(m_pDBHandle);
//    LOG_PROCESS_ERROR(pQueryRes);
//
//    nRowNum = (int)mysql_num_rows(pQueryRes);
//
//    for (int i = 0; i < nRowNum; i++)
//    {
//        QueryRow = mysql_fetch_row(pQueryRes);
//        LOG_PROCESS_ERROR(QueryRow);
//
//        pulFieldLength = mysql_fetch_lengths(pQueryRes);
//        LOG_PROCESS_ERROR(pulFieldLength);
//
//        SAFE_RELEASE(piBuffer);
//
//        piBuffer = QCreateMemBlock((unsigned)sizeof(KGAME_CARD_CHECK_DURATION_RESPOND));
//        LOG_PROCESS_ERROR(piBuffer);
//
//        pRespond = (KGAME_CARD_CHECK_DURATION_RESPOND*)piBuffer->GetData();
//        LOG_PROCESS_ERROR(pRespond);
//
//        pRespond->nProtocolID   = emrpGameCardCheckDurationRespond;
//        pRespond->dwPlayerID    = strtoul(QueryRow[0], NULL, 10);
//        pRespond->nCoin         = strtol(QueryRow[1], NULL, 10);
//
//        nRetCode = PushRespond(piBuffer);
//        LOG_PROCESS_ERROR(nRetCode);
//    }
//
//    nRetCode = snprintf(
//        m_pszSQL, MAX_MISC_DB_SQL_SIZE,
//        " delete from %s where EndTime <= FROM_UNIXTIME(%ld); ",
//        GAME_CARD_TABLE_NAME, pRequest->nTimeNow
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
//
//    nRetCode = DoQuery(m_pszSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    bResult = true;
//EXIT0:
//    if (pQueryRes)
//    {
//        mysql_free_result(pQueryRes);
//        pQueryRes = NULL;
//    }
//    SAFE_RELEASE(piBuffer);
//    return bResult;
//}
//
//BOOL KMiscDB::ProcessAddFarmerRecord(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL                        bResult     = false;
//    int                         nRetCode    = 0;
//    KADD_FARMER_RECORD_REQUEST* pRequest    = (KADD_FARMER_RECORD_REQUEST*)pbyData;
//    size_t                      uLeftSize   = MAX_MISC_DB_SQL_SIZE;
//    size_t                      uStrLen     = 0;
//    char*                       pszPos      = m_pszSQL;
//
//    nRetCode = snprintf(
//        pszPos, uLeftSize,
//        " insert into %s "
//        "   (Account, PunishEndTime) "
//        " values "
//        "   ('",
//        ANTI_FARMER_TABLE_NAME
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);    
//    pszPos      += nRetCode;
//    uLeftSize   -= nRetCode;
//
//    uStrLen = strlen(pRequest->szAccount);
//    LOG_PROCESS_ERROR(uLeftSize > uStrLen * 2);
//    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, pRequest->szAccount, (unsigned long)uStrLen);
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
//    pszPos      += nRetCode;
//    uLeftSize   -= nRetCode;
//
//    nRetCode = snprintf(pszPos, uLeftSize, "', from_unixtime(%ld));", pRequest->nPunishEndTime);
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
//    pszPos      += nRetCode;
//    uLeftSize   -= nRetCode;
//
//    nRetCode = DoQuery(m_pszSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    bResult = true;
//EXIT0:
//    return bResult;
//}
//
//BOOL KMiscDB::ProcessDeleteFarmerRecord(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL                            bResult     = false;
//    int                             nRetCode    = 0;
//    KDELETE_FARMER_RECORD_REQUEST*  pRequest    = (KDELETE_FARMER_RECORD_REQUEST*)pbyData;
//    size_t                          uLeftSize   = MAX_MISC_DB_SQL_SIZE;
//    size_t                          uStrLen     = 0;
//    char*                           pszPos      = m_pszSQL;
//
//    nRetCode = snprintf(
//        pszPos, uLeftSize,
//        " delete from %s where Account = '",
//        ANTI_FARMER_TABLE_NAME
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
//    pszPos      += nRetCode;
//    uLeftSize   -= nRetCode;
//
//    uStrLen = strlen(pRequest->szAccount);
//    LOG_PROCESS_ERROR(uLeftSize > uStrLen * 2);
//
//    nRetCode = mysql_real_escape_string(
//        m_pDBHandle, pszPos, pRequest->szAccount, (unsigned long)uStrLen
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
//    pszPos      += nRetCode;
//    uLeftSize   -= nRetCode;
//
//    nRetCode = snprintf(pszPos, uLeftSize, "';");
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
//    pszPos      += nRetCode;
//    uLeftSize   -= nRetCode;
//
//    nRetCode = DoQuery(m_pszSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    bResult = true;
//EXIT0:
//    return bResult;
//}
//
//BOOL KMiscDB::ProcessSaveMentorRecord(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL                            bResult             = false;
//    int                             nRetCode            = false;
//    KSAVE_MENTOR_RECORD_REQUEST*    pRequest            = (KSAVE_MENTOR_RECORD_REQUEST*)pbyData;
//    MYSQL_RES*                      pQueryRes           = NULL;
//    unsigned int                    uMenterDataLen      = 0;
//    char*                           pszPos              = m_pszSQL;
//    size_t                          uLeftBufferSize     = MAX_MISC_DB_SQL_SIZE;
//
//    LOG_PROCESS_ERROR(uDataLen == sizeof(KSAVE_MENTOR_RECORD_REQUEST) + pRequest->uDataLen);
//
//    nRetCode = snprintf(
//        pszPos, uLeftBufferSize, 
//        " replace into %s"
//        "   (MentorID, ApprenticeID, Data) "
//        " values "
//        "   (%lu, %lu, '",
//        MENTOR_TABLE_NAME, 
//        pRequest->dwMentorID, pRequest->dwApprenticeID
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
//    pszPos          += nRetCode;
//    uLeftBufferSize -= nRetCode;
//
//    uMenterDataLen  = (unsigned)pRequest->uDataLen;
//    LOG_PROCESS_ERROR(uLeftBufferSize > uMenterDataLen * 2);
//
//    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (const char*)pRequest->byMentorData, uMenterDataLen);
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
//    pszPos          += nRetCode;
//    uLeftBufferSize -= nRetCode;
//
//    nRetCode = snprintf(pszPos, uLeftBufferSize, "');");
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
//
//    nRetCode = DoQuery(m_pszSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    bResult = true;
//EXIT0:
//    return bResult;
//}
//
//BOOL KMiscDB::ProcessDeleteMentorRecord(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL                            bResult             = false;
//    int                             nRetCode            = false;
//    KDELETE_MENTOR_RECORD_REQUEST*  pRequest            = (KDELETE_MENTOR_RECORD_REQUEST*)pbyData;
//
//    LOG_PROCESS_ERROR(uDataLen == sizeof(KDELETE_MENTOR_RECORD_REQUEST));
//
//    nRetCode = snprintf(
//        m_pszSQL, MAX_MISC_DB_SQL_SIZE, 
//        " delete from %s"
//        "   where MentorID = %lu and ApprenticeID = %lu;",
//        MENTOR_TABLE_NAME, pRequest->dwMentorID, pRequest->dwApprenticeID
//    );
//    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);
//
//    nRetCode = DoQuery(m_pszSQL);
//    LOG_PROCESS_ERROR(nRetCode);
//
//    bResult = true;
//EXIT0:
//    return bResult;
//}

int KMiscDB::SelectFoundRows()
{
    int                             nResult             = 0;
    int                             nRetCode            = 0;
    MYSQL_RES*                      pQueryRes           = NULL;
    int                             nRowNum             = 0;
    MYSQL_ROW                       QueryRow            = NULL;
    int                             nTotalCount         = 0;

    nRetCode = snprintf(m_pszSQL, MAX_MISC_DB_SQL_SIZE, "select found_rows();");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < MAX_MISC_DB_SQL_SIZE);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pQueryRes = mysql_store_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    nRowNum = (int)mysql_num_rows(pQueryRes);
    LOG_PROCESS_ERROR(nRowNum == 1);

    QueryRow = mysql_fetch_row(pQueryRes);
    LOG_PROCESS_ERROR(QueryRow);

    nResult = strtol(QueryRow[0], NULL, 10);
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return nResult;
}

BOOL KMiscDB::DoQuery(const char cszSQL[])
{
    BOOL     bResult            = FALSE;
    BOOL     bRetCode           = FALSE;
    unsigned ulMySQLErrorCode   = 0;

    LOG_PROCESS_ERROR(cszSQL);
    LOG_PROCESS_ERROR(m_pDBHandle);
    
    bRetCode = MySQL_Query(m_pDBHandle, cszSQL, &ulMySQLErrorCode);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = TRUE;
EXIT0:
    if (ulMySQLErrorCode != 0 && ulMySQLErrorCode != ER_DUP_ENTRY) // 主键冲突不用断开链接
    {
        MySQL_Disconnect(m_pDBHandle);
        m_pDBHandle = NULL;
    }
    return bResult;
}
