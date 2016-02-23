#include "stdafx.h"
#include "MapCopy.h"
#include "GameCenter.h"

KMapCopy::KMapCopy()
{
    m_nCopyIndex            = 0;
    m_eState                = eMapStateInvalid;	        
    m_bAutoDelete           = false;      
    m_nCreateTime           = 0;
    m_nUnloadTime           = 0;
    m_nRefreshTime          = 0;
    m_nLastSaveTime         = 0;    
    m_nConnIndex            = 0;		
    m_nPlayerCount          = 0;	
    m_nMinPlayerCount       = 0;
    m_dwOwnerID             = 0;		
    m_piSceneData           = NULL;
    m_bIsChanged            = false;       
    m_dwQueueID             = INVALID_QUEUE_ID;
    m_nLastTryLoginTime     = 0;
}

KMapCopy::~KMapCopy()
{
    SAFE_RELEASE(m_piSceneData);
}

void KMapCopy::Activate()
{
    // 处理切换地图超时的
    for (
        KCREATE_MAP_CG_CALLBACK_LIST::iterator it = m_CGCallbackList.begin();
        it != m_CGCallbackList.end(); ++it
    )
    {
        if (it->nStartWaitTime == 0)
            continue;

        if (g_pGameCenter->m_nTimeNow <= it->nStartWaitTime + 300)
            continue;

        it->nStartWaitTime = 0;

        KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(it->dwPlayerID);
        LOG_PROCESS_ERROR(pRole);

        assert(pRole->GetRoleState() == rsCG_WaitForCreateMap);

        g_pGameCenter->m_GameServer.DoSearchMapRespond(
            pRole->m_nConnIndex, pRole->m_dwPlayerID, smrcMapCreating, false, 
            0, 0, 0, 0, 0
        );

        pRole->SetRoleState(rsOnline);
    }

    if (m_dwQueueID != INVALID_QUEUE_ID)
    {
        // 处理场景登陆排队: 如果30秒没有人来登录,则清除这个队列
        if (g_pGameCenter->m_nTimeNow > m_nLastTryLoginTime + 30)
        {
            QLogPrintf(LOG_DEBUG, "QueueID cleared, MapCopyIndex = %d", m_nCopyIndex);

            m_dwQueueID = INVALID_QUEUE_ID;
        }
    }

    // 处理掉线玩家保持表的超时
    if (g_pGameCenter->m_nWorkLoop % GAME_FPS == 0)
    {
        for (KMAP_COPY_PLAYER_KEEP_LIST::iterator it = m_KeepList.begin(); it != m_KeepList.end(); NULL)
        {
            if (g_pGameCenter->m_nTimeNow > it->second)
            {
                m_nPlayerCount--;
                m_KeepList.erase(it++);
                continue;
            }
            ++it;
        }
    }

EXIT0:
    return;
}

BOOL KMapCopy::RegisterCGCallbackPlayer(DWORD dwPlayerID, int nX, int nY, int nZ)
{
    KCreateMapCGCallbackInfo    CallbackInfo;

    for (KCREATE_MAP_CG_CALLBACK_LIST::iterator it = m_CGCallbackList.begin(); it != m_CGCallbackList.end(); ++it)
    {
        if (it->dwPlayerID == dwPlayerID)
        {
            it->nX                = nX;
            it->nY                = nY;
            it->nZ                = nZ;
            it->nStartWaitTime    = g_pGameCenter->m_nTimeNow;

            goto EXIT0;
        }
    }

    CallbackInfo.dwPlayerID         = dwPlayerID;
    CallbackInfo.nX                 = nX;
    CallbackInfo.nY                 = nY;
    CallbackInfo.nZ                 = nZ;
    CallbackInfo.nStartWaitTime     = g_pGameCenter->m_nTimeNow;

    m_CGCallbackList.push_back(CallbackInfo);

EXIT0:
    return true;
}

BOOL KMapCopy::UnregisterCGCallbackPlayer(DWORD dwPlayerID)
{
    assert(m_eState == eMapStateCreating);

    for (
        KCREATE_MAP_CG_CALLBACK_LIST::iterator it = m_CGCallbackList.begin();
        it != m_CGCallbackList.end();
        ++it
    )
    {
        if (it->dwPlayerID == dwPlayerID)
        {
            m_CGCallbackList.erase(it);

            return true;
        }
    }
    return false;
}

BOOL KMapCopy::CanReset()
{
    BOOL bResult = false;

    PROCESS_ERROR(m_eState == eMapStateNormal);
    PROCESS_ERROR(m_nPlayerCount == 0);
    PROCESS_ERROR(m_piSceneData == NULL);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMapCopy::Save(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize)
{
    BOOL    bResult         = false;
    BYTE*   pbyOffset       = pbyBuffer;
    size_t  uLeftSize       = uBufferSize;    
    BYTE*   pbySceneData    = NULL;
    size_t  uSceneDataLen   = 0;

    LOG_PROCESS_ERROR(uLeftSize >= sizeof(DWORD));
    *(DWORD*)pbyOffset = (DWORD)m_PlayerList.size();
    pbyOffset += sizeof(DWORD);
    uLeftSize -= sizeof(DWORD);

    for (KMAP_COPY_PLAYER_LIST::iterator it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
    {
        LOG_PROCESS_ERROR(uLeftSize >= sizeof(DWORD));
        *(DWORD*)pbyOffset = *it;

        pbyOffset += sizeof(DWORD);
        uLeftSize -= sizeof(DWORD);
    }

    if (m_piSceneData)
    {
        pbySceneData   = (BYTE*)m_piSceneData->GetData();
        LOG_PROCESS_ERROR(pbySceneData);
        
        uSceneDataLen  = m_piSceneData->GetSize();
    }

    LOG_PROCESS_ERROR(uLeftSize >= sizeof(DWORD));
    *(DWORD*)pbyOffset = (DWORD)uSceneDataLen;

    pbyOffset += sizeof(DWORD);
    uLeftSize -= sizeof(DWORD);

    if (uSceneDataLen > 0)
    {
        LOG_PROCESS_ERROR(uLeftSize >= uSceneDataLen);
        memcpy(pbyOffset, pbySceneData, uSceneDataLen);

        pbyOffset += uSceneDataLen;
        uLeftSize -= uSceneDataLen;
    }
    
    *puUsedSize = (size_t)(pbyOffset - pbyBuffer);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMapCopy::Load(BYTE* pbyData, size_t uDataLen)
{
    BOOL        bResult         = false;
    BOOL        bRetCode        = false;
    BYTE*       pbyOffset       = pbyData;
    size_t      uLeftSize       = uDataLen;
    DWORD       dwPlayerCount   = 0;
    DWORD       dwSceneDataLen  = 0;
    IMemBlock* piSceneData     = NULL;
    BYTE*       pbySceneData    = NULL;
    
    LOG_PROCESS_ERROR(uLeftSize >= sizeof(DWORD));
    dwPlayerCount = *(DWORD*)pbyOffset;

    pbyOffset += sizeof(DWORD);
    uLeftSize -= sizeof(DWORD);
    
    for (DWORD i = 0; i < dwPlayerCount; i++)
    {
        LOG_PROCESS_ERROR(uLeftSize >= sizeof(DWORD));
        m_PlayerList.insert(*(DWORD*)pbyOffset);

        pbyOffset += sizeof(DWORD);
        uLeftSize -= sizeof(DWORD);
    }

    LOG_PROCESS_ERROR(uLeftSize >= sizeof(DWORD));
    dwSceneDataLen = *(DWORD*)pbyOffset;

    pbyOffset += sizeof(DWORD);
    uLeftSize -= sizeof(DWORD);
    
    LOG_PROCESS_ERROR(uLeftSize >= dwSceneDataLen);
    if (dwSceneDataLen > 0)
    {
        piSceneData = QCreateMemBlock((unsigned)dwSceneDataLen);
        LOG_PROCESS_ERROR(piSceneData);

        pbySceneData = (BYTE*)piSceneData->GetData();
        LOG_PROCESS_ERROR(pbySceneData);
        
        memcpy(pbySceneData, pbyOffset, dwSceneDataLen);

        assert(m_piSceneData == NULL);    

        m_piSceneData = piSceneData;
        m_piSceneData->AddRef();
    }

    pbyOffset += dwSceneDataLen;
    uLeftSize -= dwSceneDataLen;

    LOG_PROCESS_ERROR(uLeftSize == 0);
    
    bResult = true;
EXIT0:
    SAFE_RELEASE(piSceneData);
    return bResult;
}

BOOL KMapCopy::RegisterKeepPlayer(DWORD dwPlayerID)
{
    BOOL bResult = false;
	std::pair<KMAP_COPY_PLAYER_KEEP_LIST::iterator, BOOL> InsRet;

    InsRet = m_KeepList.insert(std::make_pair(dwPlayerID,g_pGameCenter->m_nTimeNow + 180));
    PROCESS_ERROR(InsRet.second);

    m_nPlayerCount++;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KMapCopy::UnRegisterKeepPlayer(DWORD dwPlayerID)
{
    KMAP_COPY_PLAYER_KEEP_LIST::iterator it = m_KeepList.find(dwPlayerID);
    if (it != m_KeepList.end())
    {
        m_nPlayerCount--;
        m_KeepList.erase(it);
        return true;
    }
    return false;
}

BOOL KMapCopy::IsPlayerKeeped(DWORD dwPlayerID)
{
    KMAP_COPY_PLAYER_KEEP_LIST::iterator it = m_KeepList.find(dwPlayerID);
    if (it != m_KeepList.end())
    {
        return true;
    }

    return false;
}
//
//DEFINE_LUA_CLASS_BEGIN(KMapCopy)
//
//    REGISTER_LUA_INTEGER_READONLY(KMapCopy, CopyIndex)
//    REGISTER_LUA_TIME_READONLY(KMapCopy, CreateTime)
//    REGISTER_LUA_TIME_READONLY(KMapCopy, UnloadTime)
//    REGISTER_LUA_TIME_READONLY(KMapCopy, RefreshTime)
//    REGISTER_LUA_INTEGER_READONLY(KMapCopy, ConnIndex)
//    REGISTER_LUA_INTEGER_READONLY(KMapCopy, PlayerCount)
//    REGISTER_LUA_INTEGER_READONLY(KMapCopy, MinPlayerCount)
//    REGISTER_LUA_DWORD_READONLY(KMapCopy, OwnerID)
//    REGISTER_LUA_DWORD_READONLY(KMapCopy, QueueID)
//
//DEFINE_LUA_CLASS_END(KMapCopy)

