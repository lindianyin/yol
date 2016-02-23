#include "stdafx.h"
#include <time.h>
#include <set>
#include <list>
#include <string>
#include "mysql.h"
#include "errmsg.h"
#include "MiscDB.h"
#include "KRole.h"
#include "rank/krankpool.h"
#include "GameCenter.h"

#define REGISTER_DB_RESPOND_FUNC(ProtocolID, FuncName, ProtocolStruct)	                    \
    {                                                                                       \
        assert(ProtocolID < sizeof(m_ProcessRespondFuns) / sizeof(m_ProcessRespondFuns[0]));\
        m_ProcessRespondFuns[ProtocolID]    = &KMiscDB::FuncName;                           \
        m_uRespondProtocolSize[ProtocolID]  = sizeof(ProtocolStruct);                       \
    }

KMiscDB::KMiscDB()
{
    m_pDBHandle                 = NULL;
    m_nNextPingTime             = 0;
    m_pszSQL                    = NULL;
    
    m_nDBThreadWorkLoop         = 0;
    m_nDBThreadIdleLoop         = 0;
    m_bRequestProcessing        = false;
    m_nOperationSpeed           = 0;
    m_fOperationSpeed           = 0.0f;
	
    memset(m_nOperationCount, 0, sizeof(m_nOperationCount));
    memset(m_ProcessRequestFuns, 0, sizeof(m_ProcessRequestFuns));
    memset(m_ProcessRespondFuns, 0, sizeof(m_ProcessRespondFuns));
    memset(m_uRequestProtocolSize, 0, sizeof(m_uRequestProtocolSize));
    memset(m_uRespondProtocolSize, 0, sizeof(m_uRespondProtocolSize));

    REGISTER_DB_RESPOND_FUNC(emrpLoadRelationRespond, OnLoadRelationData, KLOAD_RELATION_DATA_RESPOND);
    REGISTER_DB_RESPOND_FUNC(emrpLoadMailBoxRespond, OnLoadMailBox, KLOAD_MAIL_BOX_RESPOND);
    REGISTER_DB_RESPOND_FUNC(emrpCheckWithdrawMailRespond, OnCheckWithdrawMail, KCHECK_WITHDRAW_MAIL_RESPOND);
    //REGISTER_DB_RESPOND_FUNC(emrpLoadPQListRespond, OnLoadPQList, KLOAD_PQ_LIST_RESPOND);
    REGISTER_DB_RESPOND_FUNC(emrpAuctionLookupRespond, OnAuctionLookup, KAUCTION_LOOKUP_DB_RESPOND);
    REGISTER_DB_RESPOND_FUNC(emrpAuctionBidRespond, OnAuctionBid, KAUCTION_BID_DB_RESPOND);
    REGISTER_DB_RESPOND_FUNC(emrpAuctionSellRespond, OnAuctionSell, KAUCTION_SELL_DB_RESPOND);
    REGISTER_DB_RESPOND_FUNC(emrpAuctionCancelRespond, OnAuctionCancel, KAUCTION_CANCEL_DB_RESPOND);
    REGISTER_DB_RESPOND_FUNC(emrpAuctionCheckDurationRespond, OnAuctionCheckDuration, KAUCTION_CHECK_DURATION_RESPOND);

	REGISTER_DB_RESPOND_FUNC(emrpLoadArenaRecordRespond, OnLoadArenaRecord, KLOAD_ARENA_RECORD_RESPOND);
	REGISTER_DB_RESPOND_FUNC(emrpLoadServerRecordRespond, OnLoadServerRecord, KLOAD_SERVER_RECORD_RESPOND);
    //REGISTER_DB_RESPOND_FUNC(emrpGameCardBuyRespond, OnGameCardBuyRespond, KGAME_CARD_BUY_RESPOND);
    //REGISTER_DB_RESPOND_FUNC(emrpGameCardLookupRespond, OnGameCardLookupRespond, KGAME_CARD_LOOKUP_RESPOND);
    //REGISTER_DB_RESPOND_FUNC(emrpGameCardCancelRespond, OnGameCardCancelRespond, KGAME_CARD_CANCEL_RESPOND);
    //REGISTER_DB_RESPOND_FUNC(emrpGameCardCheckDurationRespond, OnGameCardCheckDurationRespond, KGAME_CARD_CHECK_DURATION_RESPOND);
}

BOOL KMiscDB::Init()
{
    BOOL    bResult         = FALSE;
    int     nRetCode        = FALSE;

    m_pszSQL = new char[MAX_MISC_DB_SQL_SIZE];
    LOG_PROCESS_ERROR(m_pszSQL);

    m_pDBHandle = MySQL_Connect(
        g_pGameCenter->m_Settings.m_szDBIP, 
        g_pGameCenter->m_Settings.m_szDBName, 
        g_pGameCenter->m_Settings.m_szDBAcc, 
        g_pGameCenter->m_Settings.m_szDBPsw
    );
    LOG_PROCESS_ERROR(m_pDBHandle);

    m_bRunFlag = TRUE;

    nRetCode = m_WorkThread.Create(WorkThreadFunction, this);
    LOG_PROCESS_ERROR(nRetCode);

    bResult = TRUE;
EXIT0:
    if (!bResult)
    {
        if (m_pDBHandle)
        {
            MySQL_Disconnect(m_pDBHandle);
            m_pDBHandle = NULL;
        }

        SAFE_DELETE_ARRAY(m_pszSQL);
    }
    return bResult;
}

void KMiscDB::UnInit()
{
    IMemBlock* piBuffer = NULL;

    m_bRunFlag = false;
    m_WorkThread.Wait();

    while(!m_RequestQueue.empty())
    {
        piBuffer = m_RequestQueue.front();

        KMISC_DB_REQUEST* pHeader = (KMISC_DB_REQUEST*)piBuffer->GetData();

        QLogPrintf(LOG_ERR, "Unprocessed DB request: %d", pHeader->nProtocolID);

        SAFE_RELEASE(piBuffer);
        m_RequestQueue.pop_front();
    }

    while(!m_RespondQueue.empty())
    {
        piBuffer = m_RespondQueue.front();

        KMISC_DB_RESPOND* pHeader = (KMISC_DB_RESPOND*)piBuffer->GetData();

        QLogPrintf(LOG_ERR, "Unprocessed DB respond: %d", pHeader->nProtocolID);

        SAFE_RELEASE(piBuffer);
        m_RespondQueue.pop_front();
    }

    if (m_pDBHandle)
    {
        MySQL_Disconnect(m_pDBHandle);
        m_pDBHandle = NULL;
    }

    SAFE_DELETE_ARRAY(m_pszSQL);
}

void KMiscDB::Activate()
{
    IMemBlock*             piPackage   = NULL;
    BYTE*                   pbyData     = NULL;
    size_t                  uDataLen    = 0;
    KMISC_DB_RESPOND*       pHeader     = NULL;
    PROCESS_RESPOND_FUNC    ProcessFunc = NULL;

    while (true)
    {
        SAFE_RELEASE(piPackage);

        piPackage = PopRespond();
        PROCESS_ERROR(piPackage);

        pbyData = (BYTE*)piPackage->GetData();
        LOG_PROCESS_ERROR(pbyData);

        pHeader = (KMISC_DB_RESPOND*)pbyData;
        LOG_PROCESS_ERROR(pHeader);

        LOG_PROCESS_ERROR(pHeader->nProtocolID > emrpRespondBegin);
        LOG_PROCESS_ERROR(pHeader->nProtocolID < emrpRespondEnd);

        uDataLen = piPackage->GetSize();
        LOG_PROCESS_ERROR(uDataLen >= m_uRespondProtocolSize[pHeader->nProtocolID]);

        ProcessFunc = m_ProcessRespondFuns[pHeader->nProtocolID];
        LOG_PROCESS_ERROR(ProcessFunc);

        (this->*ProcessFunc)(pbyData, uDataLen);
    }

EXIT0:
    SAFE_RELEASE(piPackage);
    return;
}

BOOL KMiscDB::IsAllPackageComplete()
{
    BOOL bResult            = false;
    BOOL bRequestQueueEmpty = false;
    BOOL bRespondQueueEmpty = false;

    m_RequestQueueLock.Lock();
    bRequestQueueEmpty = m_RequestQueue.empty();
    m_RequestQueueLock.Unlock();
    PROCESS_ERROR(bRequestQueueEmpty);

    m_RespondQueueLock.Lock();
    bRespondQueueEmpty = m_RespondQueue.empty();
    m_RespondQueueLock.Unlock();
    PROCESS_ERROR(bRespondQueueEmpty);

    PROCESS_ERROR(!m_bRequestProcessing);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMiscDB::PushRequest(IMemBlock* piRequest)
{
    assert(piRequest);

    m_RequestQueueLock.Lock();

    piRequest->AddRef();

    m_RequestQueue.push_back(piRequest);

    m_RequestQueueLock.Unlock();

    return true;
}
 
IMemBlock* KMiscDB::PopRespond()
{
    IMemBlock* piBuffer = NULL;

    m_RespondQueueLock.Lock();

    if (!m_RespondQueue.empty())
    {
        piBuffer = m_RespondQueue.front();
        m_RespondQueue.pop_front();
    }

    m_RespondQueueLock.Unlock();

    return piBuffer;
}

BOOL KMiscDB::DoSaveRelationData(DWORD dwPlayerID, BYTE* pbyFellowData, size_t uDataLen)
{
    BOOL                            bResult     = false;
    BOOL                            bRetCode    = false;
    IMemBlock*						piBuffer    = NULL;
    KSAVE_RELATION_DATA_REQUEST*	pResquest   = NULL;
    
	LOG_PROCESS_ERROR(dwPlayerID > 0);
	LOG_PROCESS_ERROR(pbyFellowData);
	LOG_PROCESS_ERROR(uDataLen > 0);

    piBuffer = QCreateMemBlock((unsigned)(sizeof(KSAVE_RELATION_DATA_REQUEST) + uDataLen));
    LOG_PROCESS_ERROR(piBuffer);

    pResquest = (KSAVE_RELATION_DATA_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pResquest);

    pResquest->nProtocolID  = emrpSaveRelationRequest;
    pResquest->dwPlayerID   = dwPlayerID;
    pResquest->uDataLen     = uDataLen;

    memcpy(pResquest->byData, pbyFellowData, uDataLen);

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoLoadRelationData(DWORD dwPlayerID)
{
    BOOL                            bResult     = false;
    BOOL                            bRetCode    = false;
    IMemBlock*                     piBuffer    = NULL;
    KLOAD_RELATION_DATA_REQUEST*  pResquest   = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KLOAD_RELATION_DATA_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pResquest = (KLOAD_RELATION_DATA_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pResquest);

    pResquest->nProtocolID = emrpLoadRelationRequest;
    pResquest->dwPlayerID  = dwPlayerID;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveMapCopyInfo(DWORD dwMapID, int nMapCopyIndex)
{
    BOOL                            bResult                 = false;
    BOOL                            bRetCode                = false;
    size_t                          uPakSize                = 0;
    IMemBlock*                     piBuffer                = NULL;
    IMemBlock_Resizable*              piReSize                = NULL;
    KMapInfo*                       pMapInfo                = NULL;
    KMapCopy*                       pMapCopy                = NULL;
    KSAVE_MAP_COPY_INFO_REQUEST*    pRequest                = NULL;

    pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(dwMapID);
    LOG_PROCESS_ERROR(pMapInfo);

    pMapCopy = pMapInfo->GetMapCopy(nMapCopyIndex);
    LOG_PROCESS_ERROR(pMapCopy);

    piBuffer = QCreateMemBlock((unsigned)(sizeof(KSAVE_MAP_COPY_INFO_REQUEST) + MAX_MAP_COPY_DATA_LEN));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_MAP_COPY_INFO_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID      = emrpSaveMapCopyInfoRequest;
    pRequest->dwMapID          = dwMapID;
    pRequest->nMapCopyIndex    = nMapCopyIndex;
    pRequest->dwOwnerID        = pMapCopy->m_dwOwnerID;
    pRequest->nCreateTime      = pMapCopy->m_nCreateTime;
    pRequest->nLastSaveTime    = g_pGameCenter->m_nTimeNow;
    
    bRetCode = pMapCopy->Save(pRequest->byData, MAX_MAP_COPY_DATA_LEN, &pRequest->uDataLen);
    LOG_PROCESS_ERROR(bRetCode);

    piBuffer->QueryInterface(IID_IMemBlock_Resizable, (void**)&piReSize);
    LOG_PROCESS_ERROR(piReSize);
    
    uPakSize = sizeof(KSAVE_MAP_COPY_INFO_REQUEST) + pRequest->uDataLen;

    bRetCode = piReSize->SetSmallerSize((unsigned)uPakSize);
    LOG_PROCESS_ERROR(bRetCode);
    
    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);
    
    bResult = true;
EXIT0:
    SAFE_RELEASE(piReSize);
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoDeleteMapCopyInfo(DWORD dwMapID, int nMapCopyIndex)
{
    IMemBlock*                     piRequest   = NULL;
    KDELETE_MAP_COPY_INFO_REQUEST*  pRequest    = NULL;
    BOOL                            bRetCode    = false;
    BOOL                            bResult     = false;

    piRequest = QCreateMemBlock(sizeof(KDELETE_MAP_COPY_INFO_REQUEST));
    LOG_PROCESS_ERROR(piRequest);

    pRequest = (KDELETE_MAP_COPY_INFO_REQUEST*)piRequest->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpDeleteMapCopyInfoRequest;
    pRequest->dwMapID       = dwMapID;
    pRequest->nMapCopyIndex = nMapCopyIndex;

    bRetCode = PushRequest(piRequest);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piRequest);
    return bResult;
}

BOOL KMiscDB::DoUpdateRoleBlackList(DWORD dwRoleID, time_t nFreezeTime)
{
    IMemBlock*                     piRequest   = NULL;
    KUPDATE_ROLE_BLACK_LIST*        pRequest    = NULL;
    BOOL                            bRetCode    = false;
    BOOL                            bResult     = false;

    piRequest = QCreateMemBlock((unsigned)sizeof(KUPDATE_ROLE_BLACK_LIST));
    LOG_PROCESS_ERROR(piRequest);

    pRequest = (KUPDATE_ROLE_BLACK_LIST*)piRequest->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpUpdateRoleBlackList;
    pRequest->dwRoleID      = dwRoleID;
    pRequest->nFreezeTime   = nFreezeTime;

    bRetCode = PushRequest(piRequest);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piRequest);
    return bResult;
}

BOOL KMiscDB::DoUpdateRoleDeleteList(DWORD dwID, time_t nEndTime)
{
    IMemBlock*                 piRequest   = NULL;
    KUPDATE_ROLE_DELETE_LIST*   pRequest    = NULL;
    BOOL                        bRetCode    = false;
    BOOL                        bResult     = false;

    piRequest = QCreateMemBlock((unsigned)sizeof(KUPDATE_ROLE_DELETE_LIST));
    LOG_PROCESS_ERROR(piRequest);

    pRequest = (KUPDATE_ROLE_DELETE_LIST*)piRequest->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpUpdateRoleDeleteList;
    pRequest->dwID          = dwID;
    pRequest->nEndTime      = nEndTime;

    bRetCode = PushRequest(piRequest);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piRequest);
    return bResult;
}

BOOL KMiscDB::DoLoadMailBox(DWORD dwMailBoxID)
{
    BOOL                    bResult  = false;
    BOOL                    bRetCode = false;
    IMemBlock*             piBuffer = NULL;
    KLOAD_MAIL_BOX_REQUEST* pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KLOAD_MAIL_BOX_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KLOAD_MAIL_BOX_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID = emrpLoadMailBoxRequest;
    pRequest->dwMailBoxID = dwMailBoxID;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveMailBox(
    DWORD dwMailBoxID, time_t nBaseTime, int nMailCount, BYTE* pbyMailBoxInfo, size_t uMailBoxLen
)
{
    BOOL                    bResult  = false;
    BOOL                    bRetCode = false;
    IMemBlock*             piBuffer = NULL;
    KSAVE_MAIL_BOX_REQUEST* pRequest = NULL;

    assert(pbyMailBoxInfo);
    assert(uMailBoxLen > 0);

    piBuffer = QCreateMemBlock((unsigned)(sizeof(KSAVE_MAIL_BOX_REQUEST) + uMailBoxLen));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_MAIL_BOX_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpSaveMailBoxRequest;
    pRequest->dwMailBoxID   = dwMailBoxID;
    pRequest->nBaseTime     = nBaseTime;
    pRequest->nMailCount    = nMailCount;
    pRequest->uMailBoxLen   = uMailBoxLen;
    memcpy(pRequest->MailBoxInfo, pbyMailBoxInfo, uMailBoxLen);

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoCheckWithdrawMail(time_t nTimeNow, time_t nSurvivalTime)
{
    BOOL                            bResult  = false;
    BOOL                            bRetCode = false;
    IMemBlock*                     piBuffer = NULL;
    KCHECK_WITHDRAW_MAIL_REQUEST*   pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KCHECK_WITHDRAW_MAIL_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KCHECK_WITHDRAW_MAIL_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpCheckWithdrawMailRequest;
    pRequest->nDeadLineTime = nTimeNow > nSurvivalTime ? nTimeNow - nSurvivalTime : 0;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveGlobalMail(time_t nEndTime, DWORD dwMailIndex, KMail* pMail, size_t uMailLen)
{
    BOOL                            bResult      = false;
    BOOL                            bRetCode     = false;
    IMemBlock*                     piBuffer     = NULL;
    KSAVE_GLOBAL_MAIL_REQUEST*      pRequest     = NULL;

    assert(pMail);

    piBuffer = QCreateMemBlock((unsigned)(sizeof(KSAVE_GLOBAL_MAIL_REQUEST) + uMailLen));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_GLOBAL_MAIL_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpSaveGlobalMailRequest;
    pRequest->nVersion      = MAILSYS_VERSION;
    pRequest->nEndTime      = nEndTime;
    pRequest->dwMailIndex   = dwMailIndex;

    memcpy(pRequest->byData, pMail, uMailLen);

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoDeleteGlobalMail(DWORD dwMailID)
{
    BOOL                            bResult  = false;
    BOOL                            bRetCode = false;
    IMemBlock*                     piBuffer = NULL;
    KDELETE_GLOBAL_MAIL_REQUEST*    pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)(sizeof(KDELETE_GLOBAL_MAIL_REQUEST)));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KDELETE_GLOBAL_MAIL_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpDeleteGlobalMailRequest;
    pRequest->dwMailIndex   = dwMailID;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoLoadPQList()
{
    BOOL                    bResult  = false;
    BOOL                    bRetCode = false;
    IMemBlock*             piBuffer = NULL;
    KLOAD_PQ_LIST_REQUEST*  pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KLOAD_PQ_LIST_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KLOAD_PQ_LIST_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID  = emrpLoadPQListRequest;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSavePQ(const KGPQ_DB_DATA& crPQData)
{
    BOOL                bResult  = false;
    BOOL                bRetCode = false;
    IMemBlock*         piBuffer = NULL;
    KSAVE_PQ_REQUEST*   pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KSAVE_PQ_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_PQ_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpSavePQRequest;
    pRequest->PQData        = crPQData;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoDeletePQ(DWORD dwPQID)
{
    BOOL                bResult  = false;
    BOOL                bRetCode = false;
    IMemBlock*         piBuffer = NULL;
    KDELETE_PQ_REQUEST* pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KDELETE_PQ_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KDELETE_PQ_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpDeletePQRequest;
    pRequest->dwPQID        = dwPQID;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveTongData(DWORD dwTongID, KTong* pTong)
{
    BOOL                        bResult         = false;
    BOOL                        bRetCode        = false;
    size_t                      uDataUsedSize   = 0;
    HRESULT                     hRetCode        = E_FAIL;
    IMemBlock*                 piBuffer        = NULL;
    IMemBlock_Resizable*          piResize        = NULL;
    KSAVE_TONG_DATA_REQUEST*    pRequest        = NULL;

    assert(pTong);

    piBuffer = QCreateMemBlock((unsigned)(sizeof(KSAVE_TONG_DATA_REQUEST) + MAX_TONG_DATA_LEN));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_TONG_DATA_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    bRetCode = pTong->Save(&uDataUsedSize, pRequest->byData, MAX_TONG_DATA_LEN);
    LOG_PROCESS_ERROR(bRetCode);

    pRequest->nProtocolID   = emrSaveTongDataRequest;
    pRequest->dwTongID      = dwTongID;
    pRequest->uDataLen      = uDataUsedSize;

    hRetCode = piBuffer->QueryInterface(IID_IMemBlock_Resizable, (void**)&piResize);
    LOG_COM_PROCESS_ERROR(hRetCode);

    bRetCode = piResize->SetSmallerSize((unsigned)(sizeof(KSAVE_TONG_DATA_REQUEST) + pRequest->uDataLen));
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piResize);
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoRemoveTongData(DWORD dwTongID)
{
    BOOL                        bResult     = false;
    BOOL                        bRetCode    = false;
    IMemBlock*                 piBuffer    = NULL;
    KREMOVE_TONG_DATA_REQUEST*  pRequest    = NULL;

    piBuffer = QCreateMemBlock(sizeof(KREMOVE_TONG_DATA_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KREMOVE_TONG_DATA_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrRemoveTongDataRequest;
    pRequest->dwTongID      = dwTongID;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveRankData(INT nRankId, KRankPool* pRankPool)
{
	BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	size_t                      uDataUsedSize   = 0;
	HRESULT                     hRetCode        = E_FAIL;
	IMemBlock*                 piBuffer        = NULL;
	KSAVE_RANK_DATA_REQUEST*    pRequest        = NULL;
	size_t						uBufferSize = 0;

	ASSERT(pRankPool);

	uBufferSize = sizeof(KSAVE_RANK_DATA_REQUEST) + pRankPool->GetSaveSize();

	piBuffer = QCreateMemBlock((unsigned)(uBufferSize));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (KSAVE_RANK_DATA_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	bRetCode = pRankPool->Save(pRequest->byData, uBufferSize, &uDataUsedSize);
	LOG_PROCESS_ERROR(bRetCode);

	pRequest->nProtocolID   = emrSaveRankDataRequest;
	pRequest->byRankId = (BYTE)nRankId;
	pRequest->uDataLen      = uDataUsedSize;

	bRetCode = PushRequest(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KMiscDB::DoSaveArenaRecord(DWORD dwRoleId, IMemBlock* piSaveData)
{
	BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	IMemBlock*					piBuffer        = NULL;
	KSAVE_ARENA_RECORD_REQUEST*    pRequest        = NULL;
	size_t						uBufferSize = 0;

	uBufferSize = sizeof(KSAVE_ARENA_RECORD_REQUEST) + piSaveData->GetSize();

	piBuffer = QCreateMemBlock((unsigned)(uBufferSize));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (KSAVE_ARENA_RECORD_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->nProtocolID   = emrSaveArenaRecordRequest;
	pRequest->dwRoleId		= dwRoleId;
	pRequest->uDataLen      = piSaveData->GetSize();
	memcpy(pRequest->byData, piSaveData->GetData(), piSaveData->GetSize());

	bRetCode = PushRequest(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KMiscDB::DoLoadArenaRecord(INT nConnIndex, DWORD dwRoleId)
{
	BOOL                    bResult  = false;
	BOOL                    bRetCode = false;
	IMemBlock*             piBuffer = NULL;
	KLOAD_ARENA_RECORD_REQUEST* pRequest = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(KLOAD_ARENA_RECORD_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (KLOAD_ARENA_RECORD_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->nProtocolID = emrLoadArenaRecordRequest;
	pRequest->nConnIndex = nConnIndex;
	pRequest->dwRoleId = dwRoleId;

	bRetCode = PushRequest(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

void KMiscDB::OnLoadArenaRecord(BYTE* pbyData, size_t uDataLen)
{
	KLOAD_ARENA_RECORD_RESPOND* pRespond = (KLOAD_ARENA_RECORD_RESPOND*)pbyData;

	g_pGameCenter->m_ArenaManager.OnLoadRecordFromDB(pRespond->nConnIndex, 
		pRespond->dwRoleId, 
		pRespond->byRecordData, 
		pRespond->uRecordDataLen);
}

BOOL KMiscDB::DoSaveServerRecord(INT nRecordId, IMemBlock* piSaveData)
{
	BOOL							bResult         = false;
	BOOL							bRetCode        = false;
	IMemBlock*						piBuffer        = NULL;
	KSAVE_SERVER_RECORD_REQUEST*    pRequest        = NULL;
	size_t							uBufferSize = 0;

	uBufferSize = sizeof(KSAVE_SERVER_RECORD_REQUEST) + piSaveData->GetSize();

	piBuffer = QCreateMemBlock((unsigned)(uBufferSize));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (KSAVE_SERVER_RECORD_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->nProtocolID   = emrSaveServerRecordRequest;
	pRequest->nRecordId		= nRecordId;
	pRequest->uDataLen      = piSaveData->GetSize();
	memcpy(pRequest->byData, piSaveData->GetData(), piSaveData->GetSize());

	bRetCode = PushRequest(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KMiscDB::DoLoadServerRecord(INT nRecordId)
{
	BOOL							bResult  = false;
	BOOL							bRetCode = false;
	IMemBlock*						piBuffer = NULL;
	KLOAD_SERVER_RECORD_REQUEST*	pRequest = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(KLOAD_SERVER_RECORD_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (KLOAD_SERVER_RECORD_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->nProtocolID = emrLoadServerRecordRequest;
	pRequest->nRecordId = nRecordId;

	bRetCode = PushRequest(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

void KMiscDB::OnLoadServerRecord(BYTE* pbyData, size_t uDataLen)
{
	KLOAD_SERVER_RECORD_RESPOND* pRespond = (KLOAD_SERVER_RECORD_RESPOND*)pbyData;

	g_pGameCenter->m_RecordManager.OnLoadRecordFromDB(pRespond->nRecordId,
		pRespond->byRecordData, 
		pRespond->uRecordDataLen);
}


BOOL KMiscDB::DoAuctionLookup(BYTE byRequestID, DWORD dwPlayerID, KAUCTION_LOOKUP_PARAM* pParam)
{
    BOOL                        bResult  = false;
    BOOL                        bRetCode = false;
    IMemBlock*                 piBuffer = NULL;
    KAUCTION_LOOKUP_DB_REQUEST* pRequest = NULL;

    assert(pParam);

    piBuffer = QCreateMemBlock((unsigned)sizeof(KAUCTION_LOOKUP_DB_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KAUCTION_LOOKUP_DB_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpAuctionLookupRequest;
    pRequest->byRequestID   = byRequestID;
    pRequest->dwPlayerID    = dwPlayerID;
    pRequest->Param         = *pParam;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoAuctionBid(DWORD dwBidderID, DWORD dwSaleID, DWORD dwCRC, int nPrice)
{
    BOOL                        bResult  = false;
    BOOL                        bRetCode = false;
    IMemBlock*                 piBuffer = NULL;
    KAUCTION_BID_DB_REQUEST*    pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KAUCTION_BID_DB_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KAUCTION_BID_DB_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpAuctionBidRequest;
    pRequest->dwBidderID    = dwBidderID;
    pRequest->dwSaleID      = dwSaleID;
    pRequest->dwCRC         = dwCRC;
    pRequest->nPrice        = nPrice;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoAuctionSell(
    DWORD dwSellerID, char* pszSaleName, char* pszSellerName, int nAucGenre, int nAucSub, int nRequireLevel,
    int nQuality, int nStartPrice, int nBuyItNowPrice, int nCustodyCharges, time_t nDurationTime,
    BYTE* pbyItemData, size_t uDataLen
)
{
    BOOL                        bResult  = false;
    BOOL                        bRetCode = false;
    IMemBlock*                 piBuffer = NULL;
    KAUCTION_SELL_DB_REQUEST*   pRequest = NULL;

    assert(dwSellerID);
    assert(pszSaleName);
    assert(pszSellerName);

    piBuffer = QCreateMemBlock((unsigned)(sizeof(KAUCTION_SELL_DB_REQUEST) + uDataLen));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KAUCTION_SELL_DB_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID       = emrpAuctionSellRequest;
    pRequest->dwSellerID        = dwSellerID;

    strncpy(pRequest->szSaleName, pszSaleName, sizeof(pRequest->szSaleName));
    pRequest->szSaleName[sizeof(pRequest->szSaleName) - 1] = '\0';

    strncpy(pRequest->szSellerName, pszSellerName, sizeof(pRequest->szSellerName));
    pRequest->szSellerName[sizeof(pRequest->szSellerName) - 1] = '\0';

    pRequest->nAucGenre         = nAucGenre;
    pRequest->nAucSub           = nAucSub;
    pRequest->nRequireLevel     = nRequireLevel;
    pRequest->nQuality          = nQuality;
    pRequest->nStartPrice       = nStartPrice;
    pRequest->nBuyItNowPrice    = nBuyItNowPrice;
    pRequest->nCustodyCharges   = nCustodyCharges;
    pRequest->nDurationTime     = nDurationTime;
    pRequest->uItemDataLen      = uDataLen;

    if (pbyItemData)
    {
        memcpy(pRequest->byItemData, pbyItemData, uDataLen);
    }
                    
    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoAuctionCancel(DWORD dwSellerID, DWORD dwSaleID)
{
    BOOL                        bResult  = false;
    BOOL                        bRetCode = false;
    IMemBlock*                 piBuffer = NULL;
    KAUCTION_CANCEL_DB_REQUEST* pRequest = NULL;

    assert(dwSellerID);

    piBuffer = QCreateMemBlock((unsigned)sizeof(KAUCTION_CANCEL_DB_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KAUCTION_CANCEL_DB_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpAuctionCancelRequest;
    pRequest->dwSellerID    = dwSellerID;
    pRequest->dwSaleID      = dwSaleID;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoAuctionCheckDuration(time_t nTimeNow)
{
    BOOL                                bResult  = false;
    BOOL                                bRetCode = false;
    IMemBlock*                         piBuffer = NULL;
    KAUCTION_CHECK_DURATION_REQUEST*    pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KAUCTION_CHECK_DURATION_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KAUCTION_CHECK_DURATION_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpAuctionCheckDurationRequest;
    pRequest->nTimeNow      = nTimeNow;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveGlobalCustomData(const char cszName[], BYTE* pbyData, size_t uDataLen)
{
    BOOL                                bResult  = false;
    BOOL                                bRetCode = false;
    IMemBlock*                         piBuffer = NULL;
    KSAVE_GLOBAL_CUSTOM_DATA_REQUEST*   pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KSAVE_GLOBAL_CUSTOM_DATA_REQUEST) + uDataLen);
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_GLOBAL_CUSTOM_DATA_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpSaveGlobalCustomData;

    strncpy(pRequest->szName, cszName, sizeof(pRequest->szName));
    pRequest->szName[sizeof(pRequest->szName) - 1] = '\0';

    memcpy(pRequest->byData, pbyData, uDataLen);

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoDeleteGlobalCustomData(const char cszName[])
{
    BOOL                                bResult  = false;
    BOOL                                bRetCode = false;
    IMemBlock*                         piBuffer = NULL;
    KDELETE_GLOBAL_CUSTOM_DATA_REQUEST* pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KDELETE_GLOBAL_CUSTOM_DATA_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KDELETE_GLOBAL_CUSTOM_DATA_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpDeleteGlobalCustomData;

    strncpy(pRequest->szName, cszName, sizeof(pRequest->szName));
    pRequest->szName[sizeof(pRequest->szName) - 1] = '\0';

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveGlobalSystemValue(const char* pszName, int nValue)
{
    BOOL                                bResult  = false;
    BOOL                                bRetCode = false;
    IMemBlock*                         piBuffer = NULL;
    KSAVE_GLOBAL_SYSTEM_VALUE_REQUEST*  pRequest = NULL;

    assert(pszName);

    piBuffer = QCreateMemBlock((unsigned)sizeof(KSAVE_GLOBAL_SYSTEM_VALUE_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_GLOBAL_SYSTEM_VALUE_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpSaveGlobalSystemValue;
    pRequest->nValue        = nValue;

    strncpy(pRequest->szName, pszName, sizeof(pRequest->szName));
    pRequest->szName[sizeof(pRequest->szName) - 1] = '\0';

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveStatData(time_t nStatTime, int nDataCount, INT64* pnData)
{
    BOOL                        bResult     = false;
    BOOL                        bRetCode    = false;
    IMemBlock*                 piBuffer    = NULL;
    KSAVE_STAT_DATA_REQUEST*    pRequest    = NULL;
    KStatData_DB*               pStatDB     = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KSAVE_STAT_DATA_REQUEST) + sizeof(KStatData_DB) + sizeof(INT64) * nDataCount);
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_STAT_DATA_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID = emrpSaveStatData;
    pRequest->nStatTime   = nStatTime;

    pStatDB = (KStatData_DB*)pRequest->byDataDB;

    pStatDB->nDataCount = nDataCount;
    for (int i = 0; i < nDataCount; i++)
        pStatDB->nData[i] = pnData[i];

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveStatDataName(char* pszName, int nNameID)
{
    BOOL                            bResult  = false;
    BOOL                            bRetCode = false;
    IMemBlock*                     piBuffer = NULL;
    KSAVE_STAT_DATA_NAME_REQUEST*   pRequest = NULL;

    LOG_PROCESS_ERROR(pszName);

    piBuffer = QCreateMemBlock((unsigned)sizeof(KSAVE_STAT_DATA_NAME_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_STAT_DATA_NAME_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID = emrpSaveStatDataName;
    pRequest->nNameID     = nNameID;

    strncpy(pRequest->szName, pszName, sizeof(pRequest->szName));
    pRequest->szName[sizeof(pRequest->szName) - 1] = '\0';

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

//BOOL KMiscDB::DoUpdateActivityData(const KActivityRecord& rRecord)
//{
//    BOOL                            bResult  = false;
//    BOOL                            bRetCode = false;
//    IMemBlock*                     piBuffer = NULL;
//    KUPDATE_ACTIVITY_DATA_REQUEST*  pRequest = NULL;
//
//    piBuffer = QCreateMemBlock((unsigned)sizeof(KUPDATE_ACTIVITY_DATA_REQUEST));
//    LOG_PROCESS_ERROR(piBuffer);
//
//    pRequest = (KUPDATE_ACTIVITY_DATA_REQUEST*)piBuffer->GetData();
//    LOG_PROCESS_ERROR(pRequest);
//
//    pRequest->nProtocolID   = emrpUpdateActivityData;
//    pRequest->Data          = rRecord;
//
//    bRetCode = PushRequest(piBuffer);
//    LOG_PROCESS_ERROR(bRetCode);
//
//    bResult = true;
//EXIT0:
//    SAFE_RELEASE(piBuffer);
//    return bResult;
//}

BOOL KMiscDB::DoDeleteActivityData(DWORD dwRoleID, int nType)
{
    BOOL                            bResult  = false;
    BOOL                            bRetCode = false;
    IMemBlock*                     piBuffer = NULL;
    KDELETE_ACTIVITY_DATE_REQUEST*  pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KDELETE_ACTIVITY_DATE_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KDELETE_ACTIVITY_DATE_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpDeleteActivityData;
    pRequest->dwRoleID      = dwRoleID;
    pRequest->nType         = nType;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoGameCardSellRequest(
    DWORD dwSellerID, int nCoin, int nGameTime, BYTE byType, int nPrice, time_t nEndTime
)
{
    BOOL                        bResult  = false;
    BOOL                        bRetCode = false;
    IMemBlock*                 piBuffer = NULL;
    KGAME_CARD_SELL_REQUEST*    pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KGAME_CARD_SELL_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KGAME_CARD_SELL_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpGameCardSellRequest;
    pRequest->dwSellerID    = dwSellerID;
    pRequest->nCoin         = nCoin;
    pRequest->nGameTime     = nGameTime;
    pRequest->byType        = byType;
    pRequest->nPrice        = nPrice;
    pRequest->nEndTime      = nEndTime;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoGameCardBuyRequest(DWORD dwID, DWORD dwBuyerID, int nPrice)
{
    BOOL                    bResult  = false;
    BOOL                    bRetCode = false;
    IMemBlock*             piBuffer = NULL;
    KGAME_CARD_BUY_REQUEST* pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KGAME_CARD_BUY_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KGAME_CARD_BUY_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpGameCardBuyRequest;
    pRequest->dwID          = dwID;
    pRequest->dwBuyerID     = dwBuyerID;
    pRequest->nPrice        = nPrice;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoGameCardLookupRequest(DWORD dwPlayerID, BYTE byCardType, int nStartIndex, BYTE byOrderType, BOOL bDesc)
{
    BOOL                        bResult  = false;
    BOOL                        bRetCode = false;
    IMemBlock*                 piBuffer = NULL;
    KGAME_CARD_LOOKUP_REQUEST*  pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KGAME_CARD_LOOKUP_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KGAME_CARD_LOOKUP_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpGameCardLookupRequest;
    pRequest->dwPlayerID    = dwPlayerID;
    pRequest->byCardType    = byCardType;
    pRequest->nStartIndex   = nStartIndex;
    pRequest->byOrderType   = byOrderType;
    pRequest->bDesc         = bDesc;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoGameCardCancelRequest(DWORD dwPlayerID, DWORD dwID)
{
    BOOL                        bResult  = false;
    BOOL                        bRetCode = false;
    IMemBlock*                 piBuffer = NULL;
    KGAME_CARD_CANCEL_REQUEST*  pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KGAME_CARD_CANCEL_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KGAME_CARD_CANCEL_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpGameCardCancelRequest;
    pRequest->dwPlayerID    = dwPlayerID;
    pRequest->dwID          = dwID;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoGameCardCheckDuration(time_t nTimeNow)
{
    BOOL                                bResult  = false;
    BOOL                                bRetCode = false;
    IMemBlock*                         piBuffer = NULL;
    KGAME_CARD_CHECK_DURATION_REQUEST*  pRequest = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KGAME_CARD_CHECK_DURATION_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KGAME_CARD_CHECK_DURATION_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID   = emrpGameCardCheckDurationRequest;
    pRequest->nTimeNow      = nTimeNow;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoAddFarmerRecordRequest(const char cszAccount[], time_t nPunishEndTime)
{
    BOOL                        bResult     = false;
    BOOL                        bRetCode    = false;
    IMemBlock*                 piBuffer    = NULL;
    KADD_FARMER_RECORD_REQUEST* pRequest    = NULL;
    size_t                      uStrLen     = 0;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KADD_FARMER_RECORD_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KADD_FARMER_RECORD_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID = emrpAddFarmerRecordRequest;
    
    uStrLen = strlen(cszAccount);
    LOG_PROCESS_ERROR(uStrLen < sizeof(pRequest->szAccount));
    strcpy(pRequest->szAccount, cszAccount);

    pRequest->nPunishEndTime    = nPunishEndTime;
    
    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoDeleteFarmerRecordRequest(const char cszAccount[])
{
    BOOL                            bResult     = false;
    BOOL                            bRetCode    = false;
    IMemBlock*                     piBuffer    = NULL;
    KDELETE_FARMER_RECORD_REQUEST*  pRequest    = NULL;
    size_t                          uStrLen     = 0;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KDELETE_FARMER_RECORD_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KDELETE_FARMER_RECORD_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID = emrpDeleteFarmerRecordRequest;

    uStrLen = strlen(cszAccount);
    LOG_PROCESS_ERROR(uStrLen < sizeof(pRequest->szAccount));
    strcpy(pRequest->szAccount, cszAccount);

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoSaveMentorRecordRequest(DWORD dwMentorID, DWORD dwApprenticeID, BYTE* pbyMentorData, size_t uDataLen)
{
    BOOL                            bResult     = false;
    BOOL                            bRetCode    = false;
    IMemBlock*                     piBuffer    = NULL;
    KSAVE_MENTOR_RECORD_REQUEST*    pRequest    = NULL;

    piBuffer = QCreateMemBlock((unsigned)(sizeof(KSAVE_MENTOR_RECORD_REQUEST) + uDataLen));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KSAVE_MENTOR_RECORD_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID    = emrpSaveMentorRecordRequest;
    pRequest->dwMentorID     = dwMentorID;
    pRequest->dwApprenticeID = dwApprenticeID;
    pRequest->uDataLen       = uDataLen;

    memcpy(pRequest->byMentorData, pbyMentorData, uDataLen);

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KMiscDB::DoDeleteMentorRecordRequest(DWORD dwMentorID, DWORD dwApprenticeID)
{
    BOOL                            bResult     = false;
    BOOL                            bRetCode    = false;
    IMemBlock*                     piBuffer    = NULL;
    KDELETE_MENTOR_RECORD_REQUEST*  pRequest    = NULL;

    piBuffer = QCreateMemBlock((unsigned)sizeof(KDELETE_MENTOR_RECORD_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (KDELETE_MENTOR_RECORD_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->nProtocolID    = emrpDeleteMentorRecordRequest;
    pRequest->dwMentorID     = dwMentorID;
    pRequest->dwApprenticeID = dwApprenticeID;

    bRetCode = PushRequest(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

void KMiscDB::OnLoadRelationData(BYTE* pbyData, size_t uDataLen)
{
   KLOAD_RELATION_DATA_RESPOND* pRespond = (KLOAD_RELATION_DATA_RESPOND*) pbyData;

   g_pGameCenter->m_RelationManager.OnLoadPlayerData(pRespond->dwPlayerID, 
	   pRespond->byData, uDataLen - sizeof(KLOAD_RELATION_DATA_RESPOND));

//    g_pGameCenter->m_FellowshipCenter.OnLoadFellowshipData(
//        pRespond->dwPlayerID,
//        pRespond->byData,
//        uDataLen - sizeof(KLOAD_FELLOWSHIP_DATA_RESPOND)
//    );
// EXIT0:
   return;
}

void KMiscDB::OnLoadMailBox(BYTE* pbyData, size_t uDataLen)
{
    KLOAD_MAIL_BOX_RESPOND* pRespond = (KLOAD_MAIL_BOX_RESPOND*)pbyData;

    g_pGameCenter->m_MailManager.OnLoadMailBoxRespond(
        pRespond->bSucceed, pRespond->dwMailBoxID, pRespond->nBaseTime,
        pRespond->MailBoxInfo, pRespond->uMailBoxLen
    );
}

void KMiscDB::OnCheckWithdrawMail(BYTE* pbyData, size_t uDataLen)
{
    KCHECK_WITHDRAW_MAIL_RESPOND* pRespond = (KCHECK_WITHDRAW_MAIL_RESPOND*)pbyData;

    g_pGameCenter->m_MailManager.OnCheckWithdrawMailRespond(
        pRespond->nMailBoxCount, pRespond->dwMailBoxIDList
    );
}

//void KMiscDB::OnLoadPQList(BYTE* pbyData, size_t uDataLen)
//{
//    KLOAD_PQ_LIST_RESPOND* pRespond = (KLOAD_PQ_LIST_RESPOND*)pbyData;
//
//    for (int i = 0; i < pRespond->nCount; i++)
//    {
//        BOOL                bRetCode = false;
//        const KGPQ_DB_DATA& crDBData = pRespond->PQData[i];
//        KGPQTemplateInfo*   pPQTemplate = g_pGameCenter->m_PQCenter.GetPQTemplate(crDBData.dwPQTemplateID);
//        KGPQ                PQ;
//
//        if (!pPQTemplate)
//        {
//            QLogPrintf(LOG_ERR, 
//                "[PQ] Load PQ from DB failed! PQTemplate not found, PQID: %lu, PQTemplateID: %lu.\n", 
//                crDBData.dwPQID, crDBData.dwPQTemplateID
//            );
//            continue;
//        }
//
//        PQ.m_dwPQID         = crDBData.dwPQID;
//        PQ.m_pPQTemplate    = pPQTemplate;
//
//        PQ.m_nBeginTime     = crDBData.nBeginTime;
//        PQ.m_nExpiration    = PQ.m_nBeginTime + PQ.m_pPQTemplate->nExpiration;
//
//        if (PQ.m_pPQTemplate->nEndTime)
//            PQ.m_nEndTime       = PQ.m_nBeginTime + PQ.m_pPQTemplate->nEndTime;
//
//        memcpy(&PQ.m_nPQValue, &crDBData.nPQValue, sizeof(PQ.m_nPQValue));
//        PQ.m_bFinished      = crDBData.bFinished;
//
//        PQ.m_eOwnerType     = (KGPQ_OWNER_TYPE)crDBData.byOnwerType;
//        PQ.m_dwOwnerID1     = crDBData.dwOnwerID1;
//        PQ.m_dwOwnerID2     = crDBData.dwOnwerID2;
//        PQ.m_dwOwnerID3     = crDBData.dwOnwerID3;
//
//        bRetCode = g_pGameCenter->m_PQCenter.LoadPQ(PQ);
//        if (!bRetCode)
//        {
//            QLogPrintf(LOG_ERR, 
//                "[PQ] Load PQ from DB failed! PQID: %lu, PQTemplateID: %lu.\n", 
//                crDBData.dwPQID, crDBData.dwPQTemplateID
//            );
//            continue;
//        }
//    }
//
//    g_pGameCenter->m_PQCenter.m_bLoadCompleted = true;
//    g_pGameCenter->CallMainScript("OnPQLoadComplete");
//}

void KMiscDB::OnAuctionLookup(BYTE* pbyData, size_t uDataLen)
{
    KAUCTION_LOOKUP_DB_RESPOND* pRespond = (KAUCTION_LOOKUP_DB_RESPOND*)pbyData;

    g_pGameCenter->m_AuctionManager.OnLookupRespond(pRespond->byRespondID, pRespond->dwPlayerID, &pRespond->Page);
}

void KMiscDB::OnAuctionBid(BYTE* pbyData, size_t uDataLen)
{
    KAUCTION_BID_DB_RESPOND*    pRespond = (KAUCTION_BID_DB_RESPOND*)pbyData;

    g_pGameCenter->m_AuctionManager.OnBidRespond(
        pRespond->byCode, pRespond->dwSellerID, pRespond->nSellPrice, pRespond->dwBidderID,
        pRespond->nBidPrice, pRespond->dwOldBidderID, pRespond->nOldBidPrice,pRespond->szSaleName,
        pRespond->nCustodyCharges, pRespond->uItemDataLen, pRespond->byItemData
    );
}

void KMiscDB::OnAuctionSell(BYTE* pbyData, size_t uDataLen)
{
    KAUCTION_SELL_DB_RESPOND* pRespond = (KAUCTION_SELL_DB_RESPOND*)pbyData;

    g_pGameCenter->m_AuctionManager.OnSellRespond(pRespond->byCode);
}

void KMiscDB::OnAuctionCancel(BYTE* pbyData, size_t uDataLen)
{
    KAUCTION_CANCEL_DB_RESPOND* pRespond = (KAUCTION_CANCEL_DB_RESPOND*)pbyData;

    g_pGameCenter->m_AuctionManager.OnCancelRespond(
        pRespond->byCode, pRespond->dwSellerID, pRespond->szSaleName, pRespond->uItemDataLen, pRespond->byItemData
    );
}

void KMiscDB::OnAuctionCheckDuration(BYTE* pbyData, size_t uDataLen)
{
    KAUCTION_CHECK_DURATION_RESPOND* pRespond = (KAUCTION_CHECK_DURATION_RESPOND*)pbyData;
    
    g_pGameCenter->m_AuctionManager.OnCheckDurationRespond(
        pRespond->szSaleName, pRespond->dwSellerID, pRespond->dwBidderID, pRespond->nPrice, pRespond->nBuyItNowPrice,
        pRespond->nCustodyCharges, pRespond->uItemDataLen, pRespond->byItemData
    );
}

//void KMiscDB::OnGameCardBuyRespond(BYTE* pbyData, size_t uDataLen)
//{
//    KGAME_CARD_BUY_RESPOND* pRespond = (KGAME_CARD_BUY_RESPOND*)pbyData;
//
//    if (!g_pGameCenter->m_Gateway.IsAvailable())
//    {
//        g_pGameCenter->m_GameServer.DoGameCardBuyRespond(pRespond->dwBuyerID, gorSystemError, pRespond->nPrice);
//        goto EXIT0;
//    }
//
//    g_pGameCenter->m_GameCardManager.OnDBBuyRespond(
//        pRespond->byCode, pRespond->dwSellerID, pRespond->nCoin, pRespond->nGameTime,
//        pRespond->byType, pRespond->nPrice, pRespond->dwBuyerID
//    );
//
//EXIT0:
//    return;
//}
//
//void KMiscDB::OnGameCardLookupRespond(BYTE* pbyData, size_t uDataLen)
//{
//    KGAME_CARD_LOOKUP_RESPOND* pRespond = (KGAME_CARD_LOOKUP_RESPOND*)pbyData;
//
//    g_pGameCenter->m_GameCardManager.OnDBLookupRespond(
//        pRespond->dwPlayerID, pRespond->nTotalCount, pRespond->nCount, pRespond->CardList
//    );
//}

void KMiscDB::OnGameCardCancelRespond(BYTE* pbyData, size_t uDataLen)
{
    KGAME_CARD_CANCEL_RESPOND*  pRespond    = (KGAME_CARD_CANCEL_RESPOND*)pbyData;
    KRole*                      pRole       = NULL;

    pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

    g_pGameCenter->m_Gateway.DoFreezeCoinRequest(pRole->m_dwPlayerID, pRole->m_pszAccount, false, pRespond->nCoin);

EXIT0:
    return;
}

void KMiscDB::OnGameCardCheckDurationRespond(BYTE* pbyData, size_t uDataLen)
{
    KGAME_CARD_CHECK_DURATION_RESPOND*  pRespond    = (KGAME_CARD_CHECK_DURATION_RESPOND*)pbyData;
    KRole*                              pRole       = NULL;

    pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

    g_pGameCenter->m_Gateway.DoFreezeCoinRequest(pRole->m_dwPlayerID, pRole->m_pszAccount, false, pRespond->nCoin);

EXIT0:
    return;
}
