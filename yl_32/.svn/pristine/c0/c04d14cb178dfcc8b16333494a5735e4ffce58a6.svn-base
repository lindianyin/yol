#include "stdafx.h"
#include "mysql.h"
#include "errmsg.h"
#include "RoleDB.h"
#include "config/kplayerbasedbdef.h"
#include "Misc/Misc.h"
#include "GameCenter.h"
#include "battlezone/battlezonemaster.h"

void KRoleDB::WorkThreadFunction(void* pvParam)
{
    KRoleDB* pThis = (KRoleDB*)pvParam;

    assert(pThis);
    
    pThis->ThreadFunction();
}

int KRoleDB::ThreadFunction()
{   
    UINT64    uBeginTime          = GetTickCount();
    UINT64    uTimeNow            = 0;
    int       nIdleCount          = 0;

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

    ProcessSaveQueue(INT_MAX);

    QLogPrintf(LOG_INFO, "[KRoleDB] Working thread exit !");

    return 0;
}

void KRoleDB::DBThreadActivate()
{    
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
        static  time_t  nNextReConnectTime  = 0;

        if (g_pGameCenter->m_nTimeNow >= nNextReConnectTime)
        {
            QLogPrintf(LOG_DEBUG, "Reconnecting to the database ... ...\n");

            Connect();
            nNextReConnectTime = g_pGameCenter->m_nTimeNow + 10;

            if (m_pDBHandle)
            {
                QLogPrintf(LOG_DEBUG, "Reconnecting to the database successfully!\n");
            }
        }
    }

    PROCESS_ERROR(m_pDBHandle); // 如果没有链接，不应该执行数据库操作

    // 处理"保存"队列
    ProcessSaveQueue(1);

    // 处理"加载"队列
    ProcessLoadQueue();

    // 处理"创建角色"
    ProcessCreateQueue();

    // 处理"删除角色"
    ProcessDeleteQueue();

    // 处理"取角色基本信息"
    ProcessQueryRoleBaseInfoQueue();

    // 处理"角色更名"
    ProcessRenameQueue();

	// 处理离线玩家数据
	ProcessCreateDelayQueue();

	ProcessSaveDelayQueue();

	ProcessDelayLoadQueue();

EXIT0:
    return;
}

static inline BOOL CheckRoleDataCRC(const BYTE* pbyRoleData, size_t uDataLen)
{
    BOOL                bResult         = false;
    //KRoleDataHeader*    pRoleData       = NULL;
    //DWORD               dwCRC           = 0;

    //assert(pbyRoleData);

    //LOG_PROCESS_ERROR(uDataLen > sizeof(KRoleDataHeader));
    //pRoleData = (KRoleDataHeader*)pbyRoleData;
    //
    //dwCRC = Misc_CRC32(0, pbyRoleData + sizeof(KRoleDataHeader), (DWORD)(uDataLen - sizeof(KRoleDataHeader)));
    //LOG_PROCESS_ERROR(dwCRC == pRoleData->dwCRC);
    //
    bResult = true;
EXIT0:
    return bResult;
}

void KRoleDB::ProcessCreateQueue()
{
    BOOL            bRetCode        = false;
    BYTE            byCode          = 0;
    KCREATE_REQUEST Request;
    KCREATE_RESULT  Result;

    while (m_pDBHandle)
    {       
        bRetCode = GetCreateTask(&Request);
        PROCESS_ERROR(bRetCode);
        
        byCode = CreateRole(&Request.Param, &Result.RoleListInfo);
        
        Result.byCode       = byCode;
        Result.ulIdentity   = Request.ulIdentity;

        m_CreateResultLock.Lock();        
        m_CreateResult.push_back(Result);
        m_CreateResultLock.Unlock();
    }

EXIT0:
    return;
}

void KRoleDB::CacheDelayRoleData(DWORD dwRoleId, IMemBlock* piDelayData)
{
	KDelayRoleData DelayRoleData;

	//LOG_PROCESS_ERROR(sizeof(DelayRoleData) == piDelayData->GetSize());

	memcpy(&DelayRoleData, piDelayData->GetData(), piDelayData->GetSize());

	m_DelayRoleDataTable[dwRoleId] = DelayRoleData;

EXIT0:
	return;
}

void KRoleDB::ProcessCreateDelayQueue()
{
	BOOL            bRetCode        = FALSE;
	KCREATE_DELAY_REQUEST Request;

	memset(&Request, 0, sizeof(Request));

	while (m_pDBHandle)
	{       
		bRetCode = GetCreateDelayTask(&Request);
		PROCESS_ERROR(bRetCode);

		bRetCode = CreateDelayRoleData(Request.dwRoleId, Request.piExtInfo);
		LOG_PROCESS_ERROR(bRetCode);

		CacheDelayRoleData(Request.dwRoleId, Request.piExtInfo);
	}

EXIT0:
	SAFE_RELEASE(Request.piExtInfo);
	return;
}

void KRoleDB::ProcessSaveDelayQueue()
{
	BOOL            bRetCode        = FALSE;
	KSAVE_DELAY_REQUEST Request;

	memset(&Request, 0, sizeof(Request));

	while (m_pDBHandle)
	{       
		bRetCode = GetSaveDelayTask(&Request);
		PROCESS_ERROR(bRetCode);

		bRetCode = SaveDelayRoleData(Request.dwRoleId, Request.piExtInfo);
		LOG_PROCESS_ERROR(bRetCode);

		CacheDelayRoleData(Request.dwRoleId, Request.piExtInfo);
	}

EXIT0:
	SAFE_RELEASE(Request.piExtInfo);
	return;
}

BOOL KRoleDB::ProcessSavePayRecord()
{
	BOOL bResult							= FALSE;
	IMemBlock* piPackage					= NULL;
	PAY_RECORD* pRecord						= NULL;
	LPTSTR pBuffer = NULL;

	m_PayRecordRetLock.Lock();
	for (ROLE_PAY_MAP::iterator it_role = m_RolePayMap.begin(); it_role != m_RolePayMap.end(); ++it_role)
	{
		ORDER_RECORD_MAP& mapRecord = it_role->second;

		piPackage = QCreateMemBlock(sizeof(PAY_RECORD) * mapRecord.size()); 
		pBuffer = (LPTSTR)piPackage->GetData();

		for (ORDER_RECORD_MAP::iterator it_order = mapRecord.begin(); it_order != mapRecord.end(); ++it_order)
		{
			pRecord = (PAY_RECORD*)pBuffer;
			strncpy(pRecord->szOrderno, it_order->second.szOrderno, 64);
			pRecord->nGold = it_order->second.nGold;
			pRecord->bAdd = it_order->second.bAdd;

			pBuffer += sizeof(PAY_RECORD);
		}

		SavePayRecord(it_role->first, piPackage);

		SAFE_RELEASE(piPackage);
	}

	m_PayRecordRetLock.Unlock();
	
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

void KRoleDB::ProcessSaveQueue(int nSaveCount)
{
    BOOL            bRetCode        = false;
    DWORD           dwPlayerID      = KD_BAD_ID;
    KSAVE_REQUEST   Request;

    memset(&Request, 0, sizeof(Request));

    while (m_pDBHandle && nSaveCount-- > 0)
    {
        SAFE_RELEASE(Request.piExtInfo);

        bRetCode = GetSaveTask(&dwPlayerID, &Request);
        PROCESS_ERROR(bRetCode);

        SaveRoleData(dwPlayerID, Request.BaseInfo, Request.piExtInfo);
    }

EXIT0:
    SAFE_RELEASE(Request.piExtInfo);
    return;
}

void KRoleDB::ProcessLoadQueue()
{
    BOOL                        bRetCode        = false;
    IMemBlock*                  piRoleData      = NULL;
    KLOAD_REQUEST               Request;
    KLOAD_RESULT                Result;

    while (m_pDBHandle)
    {
        bRetCode = GetLoadTask(&Request);
        PROCESS_ERROR(bRetCode);
        
        SAFE_RELEASE(piRoleData);
		if (IS_BZONE_PLAYER(Request.dwPlayerID))
		{
			bRetCode = g_BZoneMaster.GetRoleData(Request.dwPlayerID, &piRoleData);
		}
		else
		{
			bRetCode = LoadRoleData(Request.dwPlayerID, &piRoleData);
		}
        
        m_LoadRetLock.Lock();
        
        Result.bSucceed   = bRetCode;
        Result.piRoleData = piRoleData;
        Result.dwPlayerID = Request.dwPlayerID;
        Result.nGSIndex   = Request.nGSIndex;

        if (Result.piRoleData)
        {
            Result.piRoleData->AddRef();
        }

        m_LoadRetQueue.push_back(Result);

        m_LoadRetLock.Unlock();
    }
    
EXIT0:
    SAFE_RELEASE(piRoleData);
    return;
}

void KRoleDB::ProcessDelayLoadQueue()
{
	BOOL                        bRetCode        = false;
	IMemBlock*                 	piRoleData      = NULL;
	KDELAY_LOAD_REQUEST               Request;
	KDELAY_LOAD_RESULT                Result;

	while (m_pDBHandle)
	{
		bRetCode = GetDelayLoadTask(&Request);
		PROCESS_ERROR(bRetCode);

		SAFE_RELEASE(piRoleData);
		
		bRetCode = LoadDelayRoleData(Request.dwPlayerID, &piRoleData);

		m_DelayLoadRetLock.Lock();

		Result.bSucceed   = bRetCode;
		Result.piRoleData = piRoleData;
		Result.byModuleId = Request.byModuleId;
		Result.dwPlayerID = Request.dwPlayerID;
		Result.dwSrcId = Request.dwSrcId;
		Result.nGSIndex   = Request.nGSIndex;

		if (Result.piRoleData)
		{
			Result.piRoleData->AddRef();
		}

		m_DelayLoadRetQueue.push_back(Result);

		m_DelayLoadRetLock.Unlock();
	}

EXIT0:
	SAFE_RELEASE(piRoleData);
	return;
}

//VOID KRoleDB::ProcessPayRecord(DWORD dwRoleID)
//{
//	BOOL                        bRetCode        = false;
//	IMemBlock*					piRoleData      = NULL;
//	ORDER_RECORD_MAP            sOrder;
//
//	SIZE_T uLoadBuffSize = 0;
//	SIZE_T uBuffSize = 0;
//	ROLE_PAY_MAP::iterator it =  m_RolePayMap.find(dwRoleID);;
//
//	SAFE_RELEASE(piRoleData);
//
//	bRetCode = LoadPayRecordFromDB(dwRoleID, &piRoleData);
//
//	PROCESS_ERROR(bRetCode);
//
//	PROCESS_ERROR(piRoleData);
//
//	m_PayRecordRetLock.Lock();
//	if (it != m_RolePayMap.end())
//		m_RolePayMap.erase(it);
//
//	uBuffSize = piRoleData->GetSize();
//	while(uLoadBuffSize < uBuffSize)
//	{
//		if (uLoadBuffSize + sizeof(PAY_RECORD) > uBuffSize)
//		{
//			QLogPrintf(LOG_ERR, "[KRoleDB::ProcessPayRecord]	The Size is more than uSize");
//			return;
//		}
//
//		PAY_RECORD* pBufData = (PAY_RECORD*)piRoleData;
//
//		sOrder.insert(std::make_pair(pBufData->dwOrderId, *pBufData));
//
//		piRoleData += sizeof(PAY_RECORD);
//		uLoadBuffSize += sizeof(PAY_RECORD);
//	}
//
//	m_PayRecordRetLock.Unlock();
//
//EXIT0:
//	SAFE_RELEASE(piRoleData);
//	return;
//}

BOOL KRoleDB::ProcessPayOrder(DWORD dwRoleID, CHAR* szOrderno, INT nGold)
{
	BOOL bResult = FALSE;
	IMemBlock*					piPayData      = NULL;
	PAY_RECORD*					pPayRecord		= NULL;
	ROLE_PAY_MAP::iterator it = m_RolePayMap.find(dwRoleID);
	if (it == m_RolePayMap.end())
	{
		PAY_RECORD sRecord;
		strncpy(sRecord.szOrderno, szOrderno, 64);
		sRecord.nGold = nGold;
		sRecord.bAdd = 0;
		ORDER_RECORD_MAP recordMap;
		recordMap.insert(std::make_pair(szOrderno, sRecord));
		m_RolePayMap.insert(std::make_pair(dwRoleID, recordMap));

		//插入
		piPayData = QCreateMemBlock(sizeof(PAY_RECORD));
		LOG_PROCESS_ERROR(piPayData);
		pPayRecord = (PAY_RECORD*)piPayData->GetData();
		LOG_PROCESS_ERROR(pPayRecord);
		memcpy(pPayRecord, &(sRecord), sizeof(PAY_RECORD));
		CreatePayRecord(dwRoleID, piPayData);
	}
	else
	{
		ORDER_RECORD_MAP::iterator it_order = it->second.find(szOrderno);
		PROCESS_ERROR(it_order == it->second.end());

		PAY_RECORD sRecord;
		strncpy(sRecord.szOrderno, szOrderno, 64);
		sRecord.nGold = nGold;
		sRecord.bAdd = 0;
		it->second.insert(std::make_pair(szOrderno, sRecord));
		
	}
	SendPlayerOrder(dwRoleID);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPayData);
	return bResult;
}

BOOL KRoleDB::SendPlayerOrder(DWORD dwRoleID)
{
	BOOL bResult = FALSE;
	KRole *pRole = NULL;
	R2S_PAY_ORDER_RESPOND* pPayRecord = NULL;
	IMemBlock*         piSendBuffer    = NULL;
	ROLE_PAY_MAP::iterator it = m_RolePayMap.find(dwRoleID);
	PROCESS_ERROR(it != m_RolePayMap.end());
	pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleID);
	PROCESS_ERROR(pRole != NULL && pRole->IsOnline() == TRUE);

	piSendBuffer = QCreateMemBlock(sizeof(R2S_PAY_ORDER_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);
	pPayRecord = (R2S_PAY_ORDER_RESPOND*)piSendBuffer->GetData();
	for (ORDER_RECORD_MAP::iterator it_order = it->second.begin(); it_order != it->second.end(); ++it_order)
	{
		if (it_order->second.bAdd == 0)
		{
			pPayRecord->wProtocolID = r2s_pay_order_respond;
			pPayRecord->dwRoleId = dwRoleID;
			strncpy(pPayRecord->szOrderno, it_order->second.szOrderno, 64);
			pPayRecord->dwGold = it_order->second.nGold;
			bResult = g_pGameCenter->m_GameServer.Send(pRole->m_nConnIndex, piSendBuffer);
		}
	}

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KRoleDB::DoOrderFromGS(DWORD dwRoleID, CHAR* szOrderno)
{
	BOOL bResult = FALSE;
	ROLE_PAY_MAP::iterator it = m_RolePayMap.find(dwRoleID);
	LOG_PROCESS_ERROR(it != m_RolePayMap.end());

	it->second[szOrderno].bAdd = 1;
	strncpy(it->second[szOrderno].szOrderno, szOrderno, 64);

	bResult = TRUE;
EXIT0:
	return bResult;

}

void KRoleDB::ProcessDeleteQueue()
{
    BOOL            bRetCode = false;
    KDELETE_REQUEST Request;
    
    while (m_pDBHandle)
    {
        bRetCode = GetDeleteTask(&Request);
        PROCESS_ERROR(bRetCode);

        DeleteRole(Request.dwRoleID);
    }

EXIT0:
    return;
}

void KRoleDB::ProcessQueryRoleBaseInfoQueue()
{
    BOOL                            bRetCode        = false;
    KQUERY_ROLE_BASE_INFO_REQUEST   Request;
    KQUERY_ROLE_BASE_INFO_RESULT    Respond;

    while (m_pDBHandle)
    {
        bRetCode = GetQueryRoleBaseInfoTask(&Request);
        PROCESS_ERROR(bRetCode);
        
        ZeroMemory(&Respond, sizeof(Respond));

        bRetCode = QueryRoleBaseInfo(Request.dwRoleID, &Respond.BaseInfo);
        
        Respond.dwRoleID        = Request.dwRoleID;
        Respond.nPlayerIndex    = Request.nPlayerIndex;
        Respond.nRoleIndex      = Request.nRoleIndex;
        Respond.nRoleCount      = Request.nRoleCount;
        Respond.bSucceed        = bRetCode;

        m_QueryRoleBaseResultQueueLock.Lock();
        m_QueryRoleBaseResultQueue.push_back(Respond);
        m_QueryRoleBaseResultQueueLock.Unlock();
    }

EXIT0:
    return;
}

void KRoleDB::ProcessRenameQueue()
{
    BOOL            bRetCode    = false;
    KRole*          pRole       = NULL;
    KRENAME_REQUEST Request;
    KRENAME_RESULT  Respond;

    while (m_pDBHandle)
    {
        bRetCode = GetRenameTask(&Request);
        PROCESS_ERROR(bRetCode);

        ZeroMemory(&Respond, sizeof(Respond));

        Respond.byCode = Rename(
            Request.dwRoleID, Request.szNewName
        );

        bRetCode = QueryRoleBaseInfoFromDB(
            Request.dwRoleID, &Respond.RoleListInfo.BaseInfo
        );
        PROCESS_ERROR(bRetCode);

        pRole = g_pGameCenter->m_RoleManager.GetRole(Request.dwRoleID);
        LOG_PROCESS_ERROR(pRole);

        Respond.RoleListInfo.dwPlayerID   = Request.dwRoleID;

        strncpy(Respond.RoleListInfo.szAccount, pRole->m_pszAccount, sizeof(Respond.RoleListInfo.szAccount));
        Respond.RoleListInfo.szAccount[sizeof(Respond.RoleListInfo.szAccount) - 1] = '\0';

        strncpy(Respond.RoleListInfo.szRoleName, Request.szNewName, sizeof(Respond.RoleListInfo.szRoleName));
        Respond.RoleListInfo.szRoleName[sizeof(Respond.RoleListInfo.szRoleName) - 1] = '\0';

        Respond.nPlayerIndex = Request.nPlayerIndex;
        Respond.dwRoleID     = Request.dwRoleID;

        m_RenameResultQueueLock.Lock();
        m_RenameResultQueue.push_back(Respond);
        m_RenameResultQueueLock.Unlock();
    }

EXIT0:
    return;
}

DWORD KRoleDB::GetCurrentRoleID()
{
    DWORD               dwRetRoleID         = KD_BAD_ID;
    BOOL                bRetCode            = false;
    unsigned long long  ullCurrentRoleID    = 0;
    
    bRetCode = g_pGameCenter->m_piSequence->GenerateID("RoleID", 1, &ullCurrentRoleID);
    LOG_PROCESS_ERROR(bRetCode);

    LOG_PROCESS_ERROR((ullCurrentRoleID > 0) && "ID Error!\n");
    LOG_PROCESS_ERROR(ullCurrentRoleID < UINT_MAX);

    dwRetRoleID = (DWORD)ullCurrentRoleID;
EXIT0:
    return dwRetRoleID;
}

BOOL KRoleDB::GetRoleBaseInfo(KDB_ROLE_GENERATOR_PARAM* pParam, DWORD dwRoleID, KRoleBaseInfo* pRetBaseInfo)
{
    BOOL      bResult  = false;

    assert(pParam);
    assert(pRetBaseInfo);

    // ---------------- Set Base Information ----------------->
    LOG_PROCESS_ERROR(/*pParam->UserParam.cRoleType > rtInvalid &&*/ pParam->UserParam.cRoleType < emROLETYPE_TOTAL);
    
    ZeroMemory(pRetBaseInfo, sizeof(KRoleBaseInfo));

    pRetBaseInfo->cRoleType                    = pParam->UserParam.cRoleType;
    pRetBaseInfo->CurrentPos.dwMapID           = pParam->UserParam.dwMapID;
    pRetBaseInfo->CurrentPos.nMapCopyIndex     = pParam->UserParam.nMapCopyIndex;
    pRetBaseInfo->CurrentPos.nX                = pParam->nX;
    pRetBaseInfo->CurrentPos.nY                = pParam->nY;
    pRetBaseInfo->CurrentPos.nZ                = pParam->nZ;

    pRetBaseInfo->byLevel                      = 1;
    pRetBaseInfo->byCamp                       = 0/*cNeutral*/;
    pRetBaseInfo->byFaction                    = pParam->UserParam.byFaction;
    pRetBaseInfo->CurrentPos.byFaceDirection   = pParam->byDirection;
    pRetBaseInfo->nCreateTime                  = g_pGameCenter->m_nTimeNow;

    memcpy(pRetBaseInfo->wRepresentId, pParam->UserParam.wRepresentId, sizeof(pRetBaseInfo->wRepresentId));
    
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KRoleDB::GetCreateTask(KCREATE_REQUEST* pTask)
{
    BOOL bResult = false;

    assert(pTask);

    m_CreateQueueLock.Lock();

    PROCESS_ERROR(!m_CreateQueue.empty());

    *pTask = m_CreateQueue.front();
    m_CreateQueue.pop_front();

    bResult = true;
EXIT0:
    m_CreateQueueLock.Unlock();

    return bResult;
}

BOOL KRoleDB::GetCreateDelayTask(KCREATE_DELAY_REQUEST* pTask)
{
	BOOL bResult = false;

	assert(pTask);

	m_CreateDelayQueueLock.Lock();

	PROCESS_ERROR(!m_CreateDelayQueue.empty());

	*pTask = m_CreateDelayQueue.front();
	m_CreateDelayQueue.pop_front();

	bResult = true;
EXIT0:
	m_CreateDelayQueueLock.Unlock();

	return bResult;
}

BOOL KRoleDB::GetSaveDelayTask(KSAVE_DELAY_REQUEST* pTask)
{
	BOOL bResult = false;

	assert(pTask);

	m_SaveDelayQueueLock.Lock();

	PROCESS_ERROR(!m_SaveDelayQueue.empty());

	*pTask = m_SaveDelayQueue.front();
	m_SaveDelayQueue.pop_front();

	bResult = true;
EXIT0:
	m_SaveDelayQueueLock.Unlock();

	return bResult;
}

BOOL KRoleDB::CheckRoleCountInAccount(KDB_ROLE_GENERATOR_PARAM* pCreateRoleParam)
{
    BOOL            bResult         = false;
    BOOL            bRetCode        = false;
    int             nRetCode        = 0;
    my_ulonglong    ullRowCount     = 0;             
    unsigned long   ulLength        = 0;
    char*           pszPos          = NULL;
    size_t          uLeftBufferSize = 0;
    MYSQL_RES*      pQueryRes       = NULL;   
    MYSQL_ROW       QueryRow;
    
    pszPos          = m_pszSQL;
    uLeftBufferSize = MAX_ROLE_DB_SQL_SIZE;

    nRetCode = snprintf(
        m_pszSQL, uLeftBufferSize, "select count(*) from %s where %s = '",
        ROLE_TABLE_NAME, KG_ACCOUNT_FIELD_NAME
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    ulLength = (unsigned long)(strlen(pCreateRoleParam->szAccountName));
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, pCreateRoleParam->szAccountName, ulLength);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;  

    nRetCode = snprintf(pszPos, uLeftBufferSize, "' ;");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);

    bRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(bRetCode);
    
    pQueryRes = mysql_store_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    nRetCode = (int)mysql_num_rows(pQueryRes);
    PROCESS_ERROR(nRetCode == 1);

    QueryRow = mysql_fetch_row(pQueryRes);
    PROCESS_ERROR(QueryRow);

#ifdef WIN32
    ullRowCount = (my_ulonglong)_atoi64(QueryRow[0]);
#else
    ullRowCount = (my_ulonglong)atoll(QueryRow[0]);
#endif
    
    assert(g_pGameCenter);
    LOG_PROCESS_ERROR(ullRowCount < (my_ulonglong)(g_pGameCenter->m_Settings.m_nMaxRoleCount));

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }

    return bResult;      
}

BOOL KRoleDB::CreateDelayRoleData(DWORD dwRoleId, IMemBlock* piDelayData)
{
	int				nRetCode			= 0;
	unsigned        uExtInfoDataLen     = 0;
	unsigned long   ulLength            = 0;
	char*			pszPos				= NULL;
	BOOL			bRet				= false;
	size_t          uLeftBufferSize = 0;

	pszPos = m_pszSQL;
	uLeftBufferSize = MAX_ROLE_DB_SQL_SIZE;

	nRetCode = snprintf(pszPos, MAX_ROLE_DB_SQL_SIZE, "insert into %s (PlayerID, DelayLoadData) "
		"values (%u, '", DELAY_LOAD_TABLE_NAME, dwRoleId);

	pszPos += nRetCode;
	uLeftBufferSize -= nRetCode;

	uExtInfoDataLen = piDelayData->GetSize();
	ulLength = (unsigned long)uExtInfoDataLen;
	nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (const char*)piDelayData->GetData(), ulLength);

	pszPos += nRetCode;
	uLeftBufferSize -= nRetCode;

	nRetCode = snprintf(pszPos, uLeftBufferSize, "');");
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);

	pszPos += nRetCode;

	bRet = DoQuery(m_pszSQL);

EXIT0:
	return bRet;
}

BOOL KRoleDB::SaveDelayRoleData(DWORD dwRoleId, IMemBlock* piDelayData)
{
	int				nRetCode			= 0;
	unsigned        uExtInfoDataLen     = 0;
	unsigned long   ulLength            = 0;
	char*			pszPos				= NULL;
	BOOL			bRet				= false;
	size_t          uLeftBufferSize = 0;

	pszPos = m_pszSQL;
	uLeftBufferSize = MAX_ROLE_DB_SQL_SIZE;

	nRetCode = snprintf(pszPos, MAX_ROLE_DB_SQL_SIZE, "update %s set DelayLoadData = '", DELAY_LOAD_TABLE_NAME);

	pszPos += nRetCode;
	uLeftBufferSize -= nRetCode;

	uExtInfoDataLen = piDelayData->GetSize();
	ulLength = (unsigned long)uExtInfoDataLen;
	nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (char *)piDelayData->GetData(), ulLength);

	pszPos += nRetCode;
	uLeftBufferSize -= nRetCode;

	nRetCode = snprintf(pszPos, uLeftBufferSize, "' where PlayerID = %u;", dwRoleId);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);

	pszPos += nRetCode;

	bRet = DoQuery(m_pszSQL);

EXIT0:
	return bRet;
}

BOOL KRoleDB::CreatePayRecord(DWORD dwRoleID, IMemBlock* piDelayData)
{
	int				nRetCode			= 0;
	unsigned        uExtInfoDataLen     = 0;
	unsigned long   ulLength            = 0;
	char*			pszPos				= NULL;
	BOOL			bRet				= false;
	size_t          uLeftBufferSize = 0;

	pszPos = m_pszSQL;
	uLeftBufferSize = MAX_ROLE_DB_SQL_SIZE;

	nRetCode = snprintf(pszPos, MAX_ROLE_DB_SQL_SIZE, "insert into %s (PlayerID, PayRecordData) "
		"values (%u, '", PAY_RECORD_TABLE_NAME, dwRoleID);

	pszPos += nRetCode;
	uLeftBufferSize -= nRetCode;

	uExtInfoDataLen = piDelayData->GetSize();
	ulLength = (unsigned long)uExtInfoDataLen;
	nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (const char*)piDelayData->GetData(), ulLength);

	pszPos += nRetCode;
	uLeftBufferSize -= nRetCode;

	nRetCode = snprintf(pszPos, uLeftBufferSize, "');");
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);

	pszPos += nRetCode;

	bRet = DoQuery(m_pszSQL);

EXIT0:
	return bRet;
}

BOOL KRoleDB::SavePayRecord(DWORD dwRoleID, IMemBlock* piDelayData)
{
	int				nRetCode			= 0;
	unsigned        uExtInfoDataLen     = 0;
	unsigned long   ulLength            = 0;
	char*			pszPos				= NULL;
	BOOL			bRet				= false;
	size_t          uLeftBufferSize = 0;

	pszPos = m_pszSQL;
	uLeftBufferSize = MAX_ROLE_DB_SQL_SIZE;

	nRetCode = snprintf(pszPos, MAX_ROLE_DB_SQL_SIZE, "update %s set PayRecordData = '", PAY_RECORD_TABLE_NAME);

	pszPos += nRetCode;
	uLeftBufferSize -= nRetCode;

	uExtInfoDataLen = piDelayData->GetSize();
	ulLength = (unsigned long)uExtInfoDataLen;
	nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (char *)piDelayData->GetData(), ulLength);

	pszPos += nRetCode;
	uLeftBufferSize -= nRetCode;

	nRetCode = snprintf(pszPos, uLeftBufferSize, "' where PlayerID = %u;", dwRoleID);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);

	pszPos += nRetCode;

	bRet = DoQuery(m_pszSQL);

EXIT0:
	return bRet;
}


BYTE KRoleDB::CreateRole(
    KDB_ROLE_GENERATOR_PARAM* pCreateRoleParam, KROLE_LIST_INFO* pRetRoleListInfo
)
{
    BYTE            byResult        = eCreateRoleUnableToCreate;
    BOOL            bRetCode        = false;
    int             nRetCode        = 0;
    DWORD           dwRoleID        = KD_BAD_ID;
    time_t          nCurrentTime    = time(NULL);
    my_ulonglong    lAffectedRow    = 0;
    char*           pszPos          = NULL;
    size_t          uLeftBufferSize = 0;
    unsigned long   ulLength        = 0;
    KRoleBaseInfo   BaseInfo;

    assert(pCreateRoleParam);
    assert(pRetRoleListInfo);
    
    bRetCode = CheckRoleCountInAccount(pCreateRoleParam);
    PROCESS_ERROR(bRetCode);

    dwRoleID = GetCurrentRoleID();
    LOG_PROCESS_ERROR(dwRoleID != KD_BAD_ID);

    bRetCode = GetRoleBaseInfo(pCreateRoleParam, dwRoleID, &BaseInfo);
    LOG_PROCESS_ERROR(bRetCode);

    uLeftBufferSize = MAX_ROLE_DB_SQL_SIZE;
    pszPos = m_pszSQL;

    nRetCode = snprintf(
        pszPos, uLeftBufferSize, "insert into %s (%s, %s, %s, %s, %s, %s) "
        "values (%u, from_unixtime(%lu), '",
        ROLE_TABLE_NAME, KG_ID_FIELD_NAME, KG_LAST_MODIFY_FIELD_NAME, KG_NAME_FIELD_NAME, KG_ACCOUNT_FIELD_NAME, 
        KG_BASE_INFO_FIELD_NAME, KG_EXT_INFO_FIELD_NAME,
        dwRoleID, (uint64_t)nCurrentTime
    );

    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    ulLength = (unsigned long)strlen(pCreateRoleParam->UserParam.szRoleName);
    LOG_PROCESS_ERROR(uLeftBufferSize > ulLength * 2);
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, pCreateRoleParam->UserParam.szRoleName, ulLength);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftBufferSize, "', '");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    ulLength = (unsigned long)strlen(pCreateRoleParam->szAccountName);
    LOG_PROCESS_ERROR(uLeftBufferSize > ulLength * 2);
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, pCreateRoleParam->szAccountName, ulLength);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftBufferSize, "', '");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    ulLength = (unsigned long)sizeof(KRoleBaseInfo);
    LOG_PROCESS_ERROR(uLeftBufferSize > ulLength * 2);
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (const char*)&BaseInfo, ulLength);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftBufferSize, "', null);");
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;
    
    bRetCode = DoQuery(m_pszSQL);
    if (!bRetCode) 
    {
        if (m_pDBHandle) // 执行失败的情况下且数据库连接没有断开
        {
            byResult = eCreateRoleNameAlreadyExist;
        }
        goto EXIT0;
    }

    lAffectedRow = mysql_affected_rows(m_pDBHandle);
    LOG_PROCESS_ERROR(lAffectedRow == 1);
    
    pRetRoleListInfo->BaseInfo       = BaseInfo;
    pRetRoleListInfo->nFreezeTime    = 0;
    pRetRoleListInfo->nDeleteTime    = 0;
    pRetRoleListInfo->dwPlayerID     = dwRoleID;

    strncpy(pRetRoleListInfo->szRoleName, pCreateRoleParam->UserParam.szRoleName, sizeof(pRetRoleListInfo->szRoleName));
    pRetRoleListInfo->szRoleName[sizeof(pRetRoleListInfo->szRoleName) - 1] = '\0';

    strncpy(pRetRoleListInfo->szAccount, pCreateRoleParam->szAccountName, sizeof(pRetRoleListInfo->szAccount));
    pRetRoleListInfo->szAccount[sizeof(pRetRoleListInfo->szAccount) - 1] = '\0';

    QLogPrintf(
        LOG_INFO, "[KRoleDB] Created role %s, RoleID=%u succeed\n",
        pCreateRoleParam->UserParam.szRoleName, dwRoleID
    );

    byResult = eCreateRoleSucceed;
EXIT0:
    return byResult;      
}

BOOL KRoleDB::GetSaveTask(DWORD* pdwRetRoleID, KSAVE_REQUEST* pTask)
{
    BOOL                        bResult     = false;
    DWORD                       dwPlayerID  = KD_BAD_ID;
    KSAVE_DATA_TABLE::iterator  it;
    
    assert(pTask);
    assert(pdwRetRoleID);

    m_SaveQueueLock.Lock();
    
    PROCESS_ERROR(!m_SaveQueue.empty());

    dwPlayerID = m_SaveQueue.front();
    m_SaveQueue.pop_front();

    it = m_SaveDataTable.find(dwPlayerID);
    LOG_PROCESS_ERROR(it != m_SaveDataTable.end());

    *pTask = it->second;
    m_SaveDataTable.erase(it);

    *pdwRetRoleID = dwPlayerID;

    bResult = true;
EXIT0:
    m_SaveQueueLock.Unlock();
    return bResult;
}

BOOL KRoleDB::SaveRoleData(DWORD dwPlayerID, const KRoleBaseInfo& crBaseInfo, IMemBlock* piExtInfo)
{
    BOOL            bResult             = false;
    int             nRetCode            = false;
    BYTE*           pbyExtInfo          = NULL;
    unsigned        uExtInfoDataLen     = 0;
    time_t          nCurrentTime        = time(NULL);
    my_ulonglong    lAffectedRow        = 0;
    unsigned long   ulLength            = 0;
    char*           pszPos              = NULL;
    size_t          uLeftBufferSize     = 0;
    DWORD           dwTime              = 0;

    //assert(piExtInfo);
    LOG_PROCESS_ERROR(piExtInfo);
	
    dwTime = GetTickCount();
    
    pbyExtInfo = (BYTE*)piExtInfo->GetData();
    LOG_PROCESS_ERROR(pbyExtInfo);
    
    uExtInfoDataLen = piExtInfo->GetSize();

    nRetCode = CheckRoleDataCRC(pbyExtInfo, uExtInfoDataLen);
    LOG_PROCESS_ERROR(nRetCode);

    uLeftBufferSize = MAX_ROLE_DB_SQL_SIZE;
    pszPos = m_pszSQL;

    nRetCode = snprintf(
        pszPos, uLeftBufferSize, "update %s set %s = from_unixtime(%ld), %s = '",
        ROLE_TABLE_NAME, KG_LAST_MODIFY_FIELD_NAME, nCurrentTime, KG_BASE_INFO_FIELD_NAME
    );

    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    ulLength = (unsigned long)sizeof(KRoleBaseInfo);
    LOG_PROCESS_ERROR(uLeftBufferSize > ulLength * 2);
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (const char*)&crBaseInfo, ulLength);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftBufferSize, "', %s = '", KG_EXT_INFO_FIELD_NAME);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    ulLength = (unsigned long)uExtInfoDataLen;
    LOG_PROCESS_ERROR(uLeftBufferSize > ulLength * 2);
    nRetCode = mysql_real_escape_string(m_pDBHandle, pszPos, (const char*)pbyExtInfo, ulLength);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);
    pszPos          += nRetCode;
    uLeftBufferSize -= nRetCode;

    nRetCode = snprintf(pszPos, uLeftBufferSize, "' where %s = %u", KG_ID_FIELD_NAME, dwPlayerID);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftBufferSize);

    nRetCode = DoQuery(m_pszSQL);
    LOG_PROCESS_ERROR(nRetCode);

    lAffectedRow = mysql_affected_rows(m_pDBHandle);
    LOG_PROCESS_ERROR(lAffectedRow <= 1);

    dwTime  = GetTickCount() - dwTime;

    m_fRoleSaveTimeCost = m_fRoleSaveTimeCost * 0.95f + (float)dwTime * 0.05f;

    bResult = true;
EXIT0:
    if (!bResult)
    {
        QLogPrintf(LOG_ERR, "Save role data failed: ID = %u", dwPlayerID);
    }
    return bResult;
}


BOOL KRoleDB::GetLoadTask(KLOAD_REQUEST* pTask)
{
    BOOL bResult = false;

    assert(pTask);

    m_LoadQueueLock.Lock();
    
    PROCESS_ERROR(!m_LoadQueue.empty());

    *pTask = m_LoadQueue.front();
    m_LoadQueue.pop_front();

    bResult = true;
EXIT0:
    m_LoadQueueLock.Unlock();
    return bResult;
}

BOOL KRoleDB::GetDelayLoadTask(KDELAY_LOAD_REQUEST* pTask)
{
	BOOL bResult = false;

	assert(pTask);

	m_DelayLoadQueueLock.Lock();

	PROCESS_ERROR(!m_DelayLoadQueue.empty());

	*pTask = m_DelayLoadQueue.front();
	m_DelayLoadQueue.pop_front();

	bResult = true;
EXIT0:
	m_DelayLoadQueueLock.Unlock();
	return bResult;
}

BOOL KRoleDB::LoadRoleData(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo)
{
    BOOL      bResult     = false;
    BOOL      bRetCode    = false;
    DWORD     dwTime      = 0;
    
    //assert(ppiRoleExtInfo);
	LOG_PROCESS_ERROR(ppiRoleExtInfo);
	
    bRetCode = LoadRoleDataFromSaveQueue(dwRoleID, ppiRoleExtInfo);
    PROCESS_SUCCESS(bRetCode);

    dwTime = GetTickCount();

    bRetCode = LoadRoleDataFromDB(dwRoleID, ppiRoleExtInfo);
    LOG_PROCESS_ERROR(bRetCode);

    dwTime = GetTickCount() - dwTime;

    m_fRoleLoadTimeCost  = m_fRoleLoadTimeCost * 0.95f + (float)dwTime * 0.05f;

EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KRoleDB::LoadDelayRoleData(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo)
{
	BOOL      bResult     = false;
	BOOL      bRetCode    = false;
	DWORD     dwTime      = 0;

	//assert(ppiRoleExtInfo);
	LOG_PROCESS_ERROR(ppiRoleExtInfo);

	bRetCode = LoadDelayRoleDataFromCache(dwRoleID, ppiRoleExtInfo);
	PROCESS_SUCCESS(bRetCode);

	bRetCode = LoadDelayRoleDataFromDB(dwRoleID, ppiRoleExtInfo);
	LOG_PROCESS_ERROR(bRetCode);

	CacheDelayRoleData(dwRoleID, *ppiRoleExtInfo);

EXIT1:
	bResult = true;
EXIT0:
	return bResult;
}

BOOL KRoleDB::LoadRoleDataFromSaveQueue(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo)
{
    BOOL                        bResult             = FALSE;
    IMemBlock*                 	piExtInfo           = NULL;
    KSAVE_DATA_TABLE::iterator  it;

    //assert(ppiRoleExtInfo);
	LOG_PROCESS_ERROR(ppiRoleExtInfo);
    m_SaveQueueLock.Lock();
    it = m_SaveDataTable.find(dwRoleID);
    if (it != m_SaveDataTable.end())
    {
        piExtInfo = it->second.piExtInfo;
        piExtInfo->AddRef();
    }
    m_SaveQueueLock.Unlock();

    PROCESS_ERROR(piExtInfo);

    *ppiRoleExtInfo = piExtInfo;
    //(*ppiRoleExtInfo)->AddRef();

    bResult = TRUE;
EXIT0:
    //SAFE_RELEASE(piExtInfo);
    return bResult;
}

BOOL KRoleDB::LoadDelayRoleDataFromCache(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo)
{
	BOOL                        bResult				= FALSE;
	IMemBlock*                 piExtInfo			= NULL;
	KDELAY_ROLE_DATA_TABLE::iterator	it;
	IMemBlock*                 piRoleExtInfo		= NULL;
	void*                       pvRoleExtInfo		= NULL;

	//assert(ppiRoleExtInfo);
	m_DelayTableLock.Lock();
	LOG_PROCESS_ERROR(ppiRoleExtInfo);

	it = m_DelayRoleDataTable.find(dwRoleID);
	if (it != m_DelayRoleDataTable.end())
	{
		piRoleExtInfo = QCreateMemBlock(sizeof(KDelayRoleData));
		LOG_PROCESS_ERROR(piRoleExtInfo);

		pvRoleExtInfo = piRoleExtInfo->GetData();
		LOG_PROCESS_ERROR(pvRoleExtInfo);

		memcpy(pvRoleExtInfo, &(it->second), sizeof(KDelayRoleData));
	}

	PROCESS_ERROR(piRoleExtInfo);
	*ppiRoleExtInfo = piRoleExtInfo;
	//(*ppiRoleExtInfo)->AddRef();

	bResult = TRUE;
EXIT0:
	if(!bResult)
	{
		SAFE_RELEASE(piRoleExtInfo);
	}
	m_DelayTableLock.Unlock();
	return bResult;
}

BOOL KRoleDB::LoadRoleDataFromDB(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo)
{
    BOOL                        bResult         = false;
    int                         nRetCode        = false;
    HRESULT                     hRetCode        = E_FAIL;
    unsigned long long          ullRoleCount    = 0;
    IMemBlock*                 	piRoleExtInfo   = NULL;
    void*                       pvRoleExtInfo  	= NULL;
    unsigned long*              pulRoleDataSize = NULL;
    MYSQL_RES*                  pQueryRes       = NULL;
    MYSQL_ROW                   QueryRow;
    char                        szSQL[256];
    
    //assert(ppiRoleExtInfo);
	LOG_PROCESS_ERROR(ppiRoleExtInfo);
	ZeroMemory(szSQL,sizeof(szSQL));
    nRetCode =  snprintf(
        szSQL, sizeof(szSQL), "select %s from %s where %s = %u;",
        KG_EXT_INFO_FIELD_NAME, ROLE_TABLE_NAME, KG_ID_FIELD_NAME, dwRoleID
    );
	
	Q_Printl("..sql " << szSQL);
	LOG_PROCESS_ERROR(nRetCode < (int)sizeof(szSQL));
	
    nRetCode = DoQuery(szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pQueryRes = mysql_store_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);

    ullRoleCount = mysql_num_rows(pQueryRes);
    LOG_PROCESS_ERROR(ullRoleCount == 1);

    QueryRow = mysql_fetch_row(pQueryRes);
    LOG_PROCESS_ERROR(QueryRow);
    
    *ppiRoleExtInfo = NULL;

    if (QueryRow[0] == NULL) // 刚创建的角色
    {
        goto EXIT1;
    }
    
    pulRoleDataSize = mysql_fetch_lengths(pQueryRes);
    LOG_PROCESS_ERROR(pulRoleDataSize);

    piRoleExtInfo = QCreateMemBlock(*pulRoleDataSize);
    LOG_PROCESS_ERROR(piRoleExtInfo);

    pvRoleExtInfo = piRoleExtInfo->GetData();
    LOG_PROCESS_ERROR(pvRoleExtInfo);

    memcpy(pvRoleExtInfo, QueryRow[0], *pulRoleDataSize);

    *ppiRoleExtInfo = piRoleExtInfo;
    (*ppiRoleExtInfo)->AddRef();

EXIT1:
    bResult = TRUE;
EXIT0:
    SAFE_RELEASE(piRoleExtInfo);

    if(pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

BOOL KRoleDB::LoadDelayRoleDataFromDB(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo)
{
	BOOL                        bResult         = false;
	int                         nRetCode        = false;
	HRESULT                     hRetCode        = E_FAIL;
	unsigned long long          ullRoleCount    = 0;
	IMemBlock*                 	piRoleExtInfo   = NULL;
	void*                       pvRoleExtInfo  	= NULL;
	unsigned long*              pulRoleDataSize = NULL;
	MYSQL_RES*                  pQueryRes       = NULL;
	MYSQL_ROW                   QueryRow;
	char                        szSQL[256];

	//assert(ppiRoleExtInfo);
	LOG_PROCESS_ERROR(ppiRoleExtInfo);

	nRetCode =  snprintf(
		szSQL, sizeof(szSQL), "select DelayLoadData from %s where PlayerID = %u;",
		DELAY_LOAD_TABLE_NAME, dwRoleID
		);
	LOG_PROCESS_ERROR(nRetCode < (int)sizeof(szSQL));

	nRetCode = DoQuery(szSQL);
	LOG_PROCESS_ERROR(nRetCode);

	pQueryRes = mysql_store_result(m_pDBHandle);
	LOG_PROCESS_ERROR(pQueryRes);

	ullRoleCount = mysql_num_rows(pQueryRes);
	LOG_PROCESS_ERROR(ullRoleCount == 1);

	QueryRow = mysql_fetch_row(pQueryRes);
	LOG_PROCESS_ERROR(QueryRow);

	*ppiRoleExtInfo = NULL;

	if (QueryRow[0] == NULL) // 刚创建的角色
	{
		goto EXIT1;
	}

	pulRoleDataSize = mysql_fetch_lengths(pQueryRes);
	LOG_PROCESS_ERROR(pulRoleDataSize);

	piRoleExtInfo = QCreateMemBlock(*pulRoleDataSize);
	LOG_PROCESS_ERROR(piRoleExtInfo);

	pvRoleExtInfo = piRoleExtInfo->GetData();
	LOG_PROCESS_ERROR(pvRoleExtInfo);

	memcpy(pvRoleExtInfo, QueryRow[0], *pulRoleDataSize);

	*ppiRoleExtInfo = piRoleExtInfo;
	//(*ppiRoleExtInfo)->AddRef();

EXIT1:
	bResult = TRUE;
EXIT0:
	if (!bResult)
		SAFE_RELEASE(piRoleExtInfo);

	if(pQueryRes)
	{
		mysql_free_result(pQueryRes);
		pQueryRes = NULL;
	}
	return bResult;
}


BOOL KRoleDB::LoadPayRecord(DWORD dwRoleID, BYTE* pData, size_t uDataLen)
{
	BOOL bResult = TRUE;
	PAY_RECORD* pRecord = NULL;
	ROLE_PAY_MAP::iterator it = m_RolePayMap.find(dwRoleID);
	ORDER_RECORD_MAP recordMap;
	PROCESS_ERROR(it == m_RolePayMap.end());

	pRecord = (PAY_RECORD*)pData;

	recordMap.insert(std::make_pair(pRecord->szOrderno, *pRecord));
	m_RolePayMap.insert(std::make_pair(dwRoleID, recordMap));

EXIT0:
	return bResult;
}


BOOL KRoleDB::GetDeleteTask(KDELETE_REQUEST* pTask)
{
    BOOL bResult = FALSE;

    assert(pTask);

    m_DeleteQueueLock.Lock();

    PROCESS_ERROR(!m_DeleteQueue.empty());

    *pTask = m_DeleteQueue.front();
    m_DeleteQueue.pop_front();

    bResult = true;
EXIT0:
    m_DeleteQueueLock.Unlock();
    return bResult;
}

BOOL KRoleDB::DeleteRole(DWORD dwRoleID)
{
    BOOL                bResult         = FALSE;
    int                 nRetCode        = FALSE;
    unsigned long       ulLength        = 0;
    my_ulonglong        lAffectedRow    = 0;
    time_t              nCurrentTime    = time(NULL);
    char                szSQL[1024];

    nRetCode = snprintf(
        szSQL, sizeof(szSQL), 
        "insert into %s (%s,%s,%s,%s,%s) "
        "   (select %s, %s, %s, %s, from_unixtime(%lu) from %s where %s = %u);",
        RESTORE_ROLE_TABLE_NAME, 
        KG_NAME_FIELD_NAME, KG_ACCOUNT_FIELD_NAME, KG_BASE_INFO_FIELD_NAME, KG_EXT_INFO_FIELD_NAME, KG_DELETE_TIME_FIELD_NAME,
        KG_NAME_FIELD_NAME, KG_ACCOUNT_FIELD_NAME, KG_BASE_INFO_FIELD_NAME, KG_EXT_INFO_FIELD_NAME, (uint64_t)nCurrentTime, 
        ROLE_TABLE_NAME, KG_ID_FIELD_NAME, dwRoleID
    );
    LOG_PROCESS_ERROR(nRetCode < (int)sizeof(szSQL));
    
    nRetCode = DoQuery(szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    lAffectedRow = mysql_affected_rows(m_pDBHandle);
    LOG_PROCESS_ERROR(lAffectedRow == 1);
    
    nRetCode = snprintf(
        szSQL, sizeof(szSQL), "delete from %s where %s = %u;",
        ROLE_TABLE_NAME, KG_ID_FIELD_NAME, dwRoleID
    );
    LOG_PROCESS_ERROR(nRetCode < (int)sizeof(szSQL));
    
    nRetCode = DoQuery(szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    lAffectedRow = mysql_affected_rows(m_pDBHandle);
    LOG_PROCESS_ERROR(lAffectedRow == 1);

    QLogPrintf(LOG_DEBUG, "Delete role %u successfully.", dwRoleID);

    bResult = TRUE;
EXIT0:
    return bResult;
}

BOOL KRoleDB::GetQueryRoleBaseInfoTask(KQUERY_ROLE_BASE_INFO_REQUEST* pTask)
{
    BOOL bResult = FALSE;

    assert(pTask);

    m_QueryRoleBaseInfoQueueLock.Lock();

    PROCESS_ERROR(!m_QueryRoleBaseInfoQueue.empty());

    *pTask = m_QueryRoleBaseInfoQueue.front();
    m_QueryRoleBaseInfoQueue.pop_front();

    bResult = TRUE;
EXIT0:
    m_QueryRoleBaseInfoQueueLock.Unlock();
    return bResult;
}

BOOL KRoleDB::QueryRoleBaseInfo(DWORD dwRoleID, KRoleBaseInfo* pRetBaseInfo)
{
    BOOL    bResult     = FALSE;
    BOOL    bRetCode    = FALSE;

    assert(pRetBaseInfo);
    
    bRetCode = QueryRoleBaseInfoFromSaveQueue(dwRoleID, pRetBaseInfo);
    PROCESS_SUCCESS(bRetCode);
    
    bRetCode = QueryRoleBaseInfoFromDB(dwRoleID, pRetBaseInfo);
    LOG_PROCESS_ERROR(bRetCode);
    
EXIT1:
    bResult = TRUE;
EXIT0:
    return bResult;
}

BOOL KRoleDB::QueryRoleBaseInfoFromSaveQueue(DWORD dwRoleID, KRoleBaseInfo* pBaseInfo)
{
    BOOL                        bResult         = FALSE;
    KSAVE_DATA_TABLE::iterator  it;

    m_SaveQueueLock.Lock();

    it = m_SaveDataTable.find(dwRoleID);
    PROCESS_ERROR(it != m_SaveDataTable.end());
    
    *pBaseInfo = it->second.BaseInfo;

    bResult = TRUE;
EXIT0:
    m_SaveQueueLock.Unlock();
    return bResult;
}

BOOL KRoleDB::QueryRoleBaseInfoFromDB(DWORD dwRoleID, KRoleBaseInfo* pBaseInfo)
{
    BOOL                        bResult         = FALSE;
    int                         nRetCode        = FALSE;
    unsigned long long          ullRecordCount  = 0;
    unsigned long               ulLength        = 0;
    MYSQL_RES*                  pQueryRes       = NULL;
    MYSQL_ROW                   QueryRow        = NULL;
    unsigned long*              QueryLen        = NULL;
    KRole*                      pRole           = NULL;
    char                        szSQL[1024];

    //assert(pBaseInfo);
    ZeroMemory(szSQL,sizeof(szSQL));
	LOG_PROCESS_ERROR(pBaseInfo);
	
    nRetCode = snprintf(
        szSQL, sizeof(szSQL), "select %s from %s where %s = %u",
        KG_BASE_INFO_FIELD_NAME, ROLE_TABLE_NAME, KG_ID_FIELD_NAME, dwRoleID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szSQL));

    nRetCode = DoQuery(szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pQueryRes = mysql_store_result(m_pDBHandle);
    LOG_PROCESS_ERROR(pQueryRes);
    
    ullRecordCount = mysql_num_rows(pQueryRes);
    LOG_PROCESS_ERROR(ullRecordCount == 1);

    QueryRow = mysql_fetch_row(pQueryRes);
    PROCESS_ERROR(QueryRow);

    QueryLen = mysql_fetch_lengths(pQueryRes);
    PROCESS_ERROR(QueryLen);

    LOG_PROCESS_ERROR(QueryLen[0] == sizeof(KRoleBaseInfo));

    memcpy(pBaseInfo, QueryRow[0], QueryLen[0]);

    bResult = TRUE;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }
    return bResult;
}

BOOL KRoleDB::GetRenameTask(KRENAME_REQUEST* pTask)
{
    BOOL    bResult = FALSE;

    assert(pTask);

    m_RenameQueueLock.Lock();

    PROCESS_ERROR(!m_RenameQueue.empty());

    *pTask = m_RenameQueue.front();
    m_RenameQueue.pop_front();

    bResult = TRUE;
EXIT0:
    m_RenameQueueLock.Unlock();
    return bResult;
}

BYTE KRoleDB::Rename(DWORD dwRoleID, char szNewName[])
{
    BYTE                        byResult        = eRenameUnknownError;
    int                         nRetCode        = FALSE;
    KRole*                      pRole           = NULL;
    char                        szSQL[1024];

    nRetCode = snprintf(
        szSQL, sizeof(szSQL), "update %s set %s = '%s' where %s = %u",
        ROLE_TABLE_NAME, KG_NAME_FIELD_NAME, szNewName, KG_ID_FIELD_NAME, dwRoleID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szSQL));

    nRetCode = DoQuery(szSQL);
    if (!nRetCode)
    {
        if (m_pDBHandle)
        {
            byResult = eRenameNameAlreadyExist;
        }
        goto EXIT0;
    }

    byResult = eRenameSucceed;
EXIT0:
    return byResult;
}

BOOL KRoleDB::DoQuery(const char cszSQL[])
{
    BOOL     bResult            = FALSE;
    BOOL     bRetCode           = FALSE;
    unsigned ulMySQLErrorCode   = 0;

    assert(cszSQL);
    assert(m_pDBHandle);
    
    bRetCode = MySQL_Query(m_pDBHandle, cszSQL, &ulMySQLErrorCode);
    LOG_PROCESS_ERROR(bRetCode);
    bResult = TRUE;
EXIT0:
	if(!bResult)
	{
		QLogPrintf(LOG_ERR,"%s %s",mysql_error(m_pDBHandle),cszSQL);
	}
    if (ulMySQLErrorCode != 0 && ulMySQLErrorCode != ER_DUP_ENTRY) // 主键冲突不用断开链接
    {
        //MySQL_Disconnect(m_pDBHandle);
        //m_pDBHandle = NULL;
    }
    return bResult;
}
