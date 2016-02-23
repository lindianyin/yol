
#include "stdafx.h"
#include "RoleDB.h"
#include "GameCenter.h"

#define MAX_SQL_SIZE 1024
#define PING_CYCLE   30

KRoleDB::KRoleDB()
{
    m_pDBHandle             = NULL;
    m_nNextPingTime         = 0;
    m_bRunFlag              = false;
    m_pszSQL                = NULL;
    m_nDBThreadWorkLoop     = 0;
    m_nDBThreadIdleLoop     = 0;
}

KRoleDB::~KRoleDB()
{
    assert(m_pszSQL == NULL);
}

BOOL KRoleDB::Init()
{
    BOOL    bResult         = false;
    BOOL    bRetCode        = false;
    BOOL    bConnected      = false;

    m_pszSQL = new char[MAX_ROLE_DB_SQL_SIZE];
    LOG_PROCESS_ERROR(m_pszSQL);

    bRetCode = Connect();
    LOG_PROCESS_ERROR(bRetCode);
    bConnected = true;
    
    m_fRoleSaveTimeCost = 0.0f;
    m_fRoleLoadTimeCost = 0.0f;

    m_bRunFlag = true;

    bRetCode = m_WorkThread.Create(WorkThreadFunction, this);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        SAFE_DELETE_ARRAY(m_pszSQL);
        
        if (bConnected)
        {
            Disconnect();
            bConnected = false;
        }
    }
    return bResult;
}


void KRoleDB::UnInit()
{
    while (true)
    {   
        size_t uLeftCount = 0;

        m_SaveQueueLock.Lock();
        uLeftCount = m_SaveQueue.size();
        m_SaveQueueLock.Unlock();

        if (uLeftCount == 0)
            break;

        QLogPrintf(LOG_INFO, "Waiting for save role data: %u\n", uLeftCount);
        QThread_Sleep(1000);
    }

    m_bRunFlag = false;
    m_WorkThread.Wait();
    
    Disconnect();
    
    assert(m_SaveDataTable.empty());

    for (KLOAD_RESULT_QUEUE::iterator it = m_LoadRetQueue.begin(); it != m_LoadRetQueue.end(); ++it)
    {
        SAFE_RELEASE(it->piRoleData);
    }
    
    SAFE_DELETE_ARRAY(m_pszSQL);
}

void KRoleDB::Activate()
{
    DealWithLoadRoleDataRespond();

    DealWithCreateRoleRespond();
    
    DealWithQueryRoleBaseDataRespond();

    DealWithRenameRespond();

	DealWithDelayLoadRoleDataRespond();
}

void KRoleDB::DoCreateRole(KDB_ROLE_GENERATOR_PARAM* pRoleParam, unsigned long ulIdentity)
{
    KCREATE_REQUEST CreateRequest;

    assert(pRoleParam);

    CreateRequest.Param      = *pRoleParam;
    CreateRequest.ulIdentity = ulIdentity;

    m_CreateQueueLock.Lock();
    m_CreateQueue.push_back(CreateRequest);
    m_CreateQueueLock.Unlock();
    
    return;
}

void KRoleDB::DoCreateDelayRoleData(DWORD dwPlayerID, IMemBlock* piRoleData)
{
	assert(piRoleData);
	m_CreateDelayQueueLock.Lock();

	KCREATE_DELAY_REQUEST sCreateRequest;

	sCreateRequest.dwRoleId = dwPlayerID;
	sCreateRequest.piExtInfo = piRoleData;
	sCreateRequest.piExtInfo->AddRef();

	m_CreateDelayQueue.push_back(sCreateRequest);

	m_CreateDelayQueueLock.Unlock();

	CacheDelayRoleData(dwPlayerID, piRoleData);

	return;
}

void KRoleDB::DoSaveDelayRoleData(DWORD dwPlayerID, IMemBlock* piRoleData)
{
	assert(piRoleData);
	m_SaveDelayQueueLock.Lock();

	KSAVE_DELAY_REQUEST sSaveRequest;

	sSaveRequest.dwRoleId = dwPlayerID;
	sSaveRequest.piExtInfo = piRoleData;
	sSaveRequest.piExtInfo->AddRef();

	m_SaveDelayQueue.push_back(sSaveRequest);

	m_SaveDelayQueueLock.Unlock();

	return;
}

void KRoleDB::DoSaveRoleData(int nGSIndex, DWORD dwPlayerID, const KRoleBaseInfo& crBaseInfo, IMemBlock* piRoleData, int nUserValue)
{
    
    assert(piRoleData);

    m_SaveQueueLock.Lock();

    KSAVE_DATA_TABLE::iterator it = m_SaveDataTable.find(dwPlayerID);

    if (it != m_SaveDataTable.end())
    {
        KSAVE_REQUEST* pRecord = &it->second;

        pRecord->nGSIndex   = nGSIndex;
        pRecord->nUserValue = nUserValue;
        pRecord->BaseInfo   = crBaseInfo;

        SAFE_RELEASE(pRecord->piExtInfo);

        pRecord->piExtInfo = piRoleData;
        pRecord->piExtInfo->AddRef();
    }
    else
    {
        KSAVE_REQUEST SaveRequest;

        SaveRequest.nGSIndex    = nGSIndex;
        SaveRequest.nUserValue  = nUserValue;
        SaveRequest.BaseInfo    = crBaseInfo;

        SaveRequest.piExtInfo = piRoleData;
        SaveRequest.piExtInfo->AddRef();

        m_SaveDataTable[dwPlayerID] = SaveRequest;
        m_SaveQueue.push_back(dwPlayerID);
    }

    m_SaveQueueLock.Unlock();
    
    return;
}

void KRoleDB::DoLoadRoleData(int nGSIndex, DWORD dwPlayerID)
{
    KLOAD_REQUEST   Request;
    
    Request.dwPlayerID  = dwPlayerID;
    Request.nGSIndex    = nGSIndex;

    m_LoadQueueLock.Lock();
    m_LoadQueue.push_back(Request);
    m_LoadQueueLock.Unlock();

    return;
}

void KRoleDB::DoDelayLoadRoleData(int nGSIndex, INT nModuleId, DWORD dwPlayerID, DWORD dwSrcId)
{
	m_DelayLoadQueueLock.Lock();
	//LOG_PROCESS_ERROR(m_DelayLoadQueue.size() < 1000);
	if(m_DelayLoadQueue.size() > 10000)
	{
		QLogPrintf(LOG_ERR,"..m_DelayLoadQueue.size() > 10000");
	}
	KDELAY_LOAD_REQUEST   Request;
	Request.dwPlayerID  = dwPlayerID;
	Request.dwSrcId = dwSrcId;
	Request.byModuleId = (BYTE)nModuleId;
	Request.nGSIndex    = nGSIndex;
	m_DelayLoadQueue.push_back(Request);
EXIT0:
	m_DelayLoadQueueLock.Unlock();
	return;
}

void KRoleDB::DoDeleteRole(DWORD dwRoleID)
{
    KDELETE_REQUEST   Request;
        
    Request.dwRoleID   = dwRoleID;

    m_DeleteQueueLock.Lock();
    m_DeleteQueue.push_back(Request);
    m_DeleteQueueLock.Unlock();

    g_pGameCenter->m_StatDataManager.UpdataCreateOrDeleteRole(false);

    return;
}


void KRoleDB::DoQueryRoleBaseData(DWORD dwRoleID, int nPlayerIndex, int nRoleIndex, int nRoleCount)
{
    KQUERY_ROLE_BASE_INFO_REQUEST Request;
    
    Request.dwRoleID        = dwRoleID;
    Request.nPlayerIndex    = nPlayerIndex;
    Request.nRoleIndex      = nRoleIndex;
    Request.nRoleCount      = nRoleCount;

    m_QueryRoleBaseInfoQueueLock.Lock();
    m_QueryRoleBaseInfoQueue.push_back(Request);
    m_QueryRoleBaseInfoQueueLock.Unlock();

    return;
}

void KRoleDB::DoRename(DWORD dwRoleID, int nPlayerIndex, char szNewName[])
{
    KRENAME_REQUEST Request;

    Request.dwRoleID        = dwRoleID;
    Request.nPlayerIndex    = nPlayerIndex;

    strncpy(Request.szNewName, szNewName, sizeof(Request.szNewName));
    Request.szNewName[sizeof(Request.szNewName) - 1] = '\0';

    m_RenameQueueLock.Lock();
    m_RenameQueue.push_back(Request);
    m_RenameQueueLock.Unlock();

    return;
}

BOOL KRoleDB::Connect()
{
    BOOL    bResult    = false;
    
    LOG_PROCESS_ERROR(m_pDBHandle == NULL);
    m_pDBHandle = MySQL_Connect(
        g_pGameCenter->m_Settings.m_szDBIP,
        g_pGameCenter->m_Settings.m_szDBName, 
        g_pGameCenter->m_Settings.m_szDBAcc,
        g_pGameCenter->m_Settings.m_szDBPsw    
    );
    PROCESS_ERROR(m_pDBHandle);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        if (m_pDBHandle)
        {
            MySQL_Disconnect(m_pDBHandle);
            m_pDBHandle = NULL;
        }
    }
    return bResult;
}

void KRoleDB::Disconnect()
{
    if (m_pDBHandle)
    {
        MySQL_Disconnect(m_pDBHandle);
        m_pDBHandle = NULL;
    }
}

void KRoleDB::DealWithLoadRoleDataRespond()
{
    m_LoadRetLock.Lock();

    for (KLOAD_RESULT_QUEUE::iterator it = m_LoadRetQueue.begin(); it != m_LoadRetQueue.end(); ++it)
    {
        BYTE*       pbyRoleData     = NULL;
        unsigned    uRoleDataLen    = 0;

        if (it->piRoleData != NULL)
        {
            pbyRoleData     = (BYTE*)it->piRoleData->GetData();
            uRoleDataLen    = it->piRoleData->GetSize();
        }

		g_pGameCenter->m_GameServer.LoadRoleData(
			it->nGSIndex, it->dwPlayerID, it->bSucceed, pbyRoleData, uRoleDataLen
			);

        SAFE_RELEASE(it->piRoleData);
    }

    m_LoadRetQueue.clear();
    m_LoadRetLock.Unlock();

    return;
}

void KRoleDB::DealWithDelayLoadRoleDataRespond()
{
	m_DelayLoadRetLock.Lock();

	for (KDELAY_LOAD_RESULT_QUEUE::iterator it = m_DelayLoadRetQueue.begin(); it != m_DelayLoadRetQueue.end(); ++it)
	{
		BYTE*       pbyRoleData     = NULL;
		unsigned    uRoleDataLen    = 0;

		if (it->piRoleData != NULL)
		{
			pbyRoleData     = (BYTE*)it->piRoleData->GetData();
			uRoleDataLen    = it->piRoleData->GetSize();
		}

		g_pGameCenter->m_GameServer.DoDelayLoadRoleData(
			it->nGSIndex, it->byModuleId, it->dwPlayerID, it->dwSrcId, it->bSucceed, pbyRoleData, uRoleDataLen
			);
			
		SAFE_RELEASE(it->piRoleData);
	}

	m_DelayLoadRetQueue.clear();
	m_DelayLoadRetLock.Unlock();

	return;
}

void KRoleDB::DealWithCreateRoleRespond()
{
    m_CreateResultLock.Lock();
	KRole *pRole = NULL;

	   
    for (KCREATE_RESULT_QUEUE::iterator it = m_CreateResult.begin(); it != m_CreateResult.end(); ++it)
    {
        if (it->byCode == eCreateRoleSucceed)
        {
			INT nFightScore = *((INT*)it->RoleListInfo.BaseInfo.byReserved);
            pRole = g_pGameCenter->m_RoleManager.AddRole(
                it->RoleListInfo.szAccount,
                it->RoleListInfo.szRoleName,
                it->RoleListInfo.dwPlayerID,
                it->RoleListInfo.BaseInfo.byLevel,
                (BYTE)it->RoleListInfo.BaseInfo.cRoleType,
                /*(KCAMP)it->RoleListInfo.BaseInfo.byCamp,*/
                it->RoleListInfo.BaseInfo.byFaction,
				nFightScore,
                it->RoleListInfo.BaseInfo.nLastSaveTime,
                it->RoleListInfo.BaseInfo.nCreateTime
            );

            g_pGameCenter->m_StatDataManager.UpdataCreateOrDeleteRole(true);


			//ldy:统计创建角色
			g_pGameCenter->m_LogServerAgency.LogAction(PLAYER_ACTION_CREATE_ROLE,pRole,0,"CreateRole");
			
        }

        g_pGameCenter->m_Gateway.DoCreateRoleRespond(it->byCode, it->ulIdentity, &it->RoleListInfo);
    }
    m_CreateResult.clear();
    m_CreateResultLock.Unlock();

    return;
}

void KRoleDB::DealWithQueryRoleBaseDataRespond()
{
    m_QueryRoleBaseResultQueueLock.Lock();

    for (
        KQUERY_ROLE_BASE_INFO_RESULT_QUEUE::iterator it = m_QueryRoleBaseResultQueue.begin(); 
        it != m_QueryRoleBaseResultQueue.end(); ++it
    )
    {
        KRoleBaseInfo* pBaseInfo = NULL;

        if (it->bSucceed)
        {
            pBaseInfo = &it->BaseInfo;
        }

        g_pGameCenter->m_Gateway.DoSyncRoleList(
            it->nPlayerIndex, it->nRoleIndex, it->nRoleCount, it->dwRoleID, pBaseInfo
        );
    }

    m_QueryRoleBaseResultQueue.clear();

    m_QueryRoleBaseResultQueueLock.Unlock();
    return;
}

void KRoleDB::DealWithRenameRespond()
{
    m_RenameResultQueueLock.Lock();

    for (
        KRENAME_RESULT_QUEUE::iterator it = m_RenameResultQueue.begin();
        it != m_RenameResultQueue.end(); ++it
    )
    {
        KROLE_LIST_INFO* pRoleListInfo = NULL;

        if (it->byCode == eRenameSucceed)
        {
            pRoleListInfo = &it->RoleListInfo;

            g_pGameCenter->m_RoleManager.RoleRename(it->dwRoleID, pRoleListInfo->szRoleName);

            g_pGameCenter->m_RoleManager.RemoveRenameRole(it->dwRoleID);
        }

        g_pGameCenter->m_Gateway.DoRenameRespond(
            it->byCode, it->nPlayerIndex, it->dwRoleID, pRoleListInfo
        );
    }

    m_RenameResultQueue.clear();

    m_RenameResultQueueLock.Unlock();
    return;
}
