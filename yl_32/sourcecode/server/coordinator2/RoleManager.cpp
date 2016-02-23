
#include "stdafx.h"
#include "RoleManager.h"
#include "GameCenter.h"
#include "config/kplayerbasedbdef.h"
#include "GameCenter.h"

#define ACTIVE_PLAYER_TIME 7 * 24 * 3600

KRoleManager::KRoleManager()
{
}

BOOL KRoleManager::Init()
{
    // do nothing !
    return true;
}

void KRoleManager::UnInit()
{
	//m_lock.Lock();
    for (
        KACCOUNT_NAME_INDEX::iterator it = m_AccountNameIndex.begin(); 
        it != m_AccountNameIndex.end(); ++it
    )
    {
        //KMemory::Delete(it->second);
		delete it->second; 
    }
    m_AccountNameIndex.clear();
    m_RoleNameIndex.clear();
    m_RoleTable.clear();
	//m_lock.Unlock();
}

void KRoleManager::Activate()
{
	//m_lock.Lock();
    for (KROLE_BLACK_LIST::iterator it = m_RoleBlackList.begin(); it != m_RoleBlackList.end(); NULL)
    {
        if (g_pGameCenter->m_nTimeNow > it->second)
        {
            g_pGameCenter->m_MiscDB.DoUpdateRoleBlackList(it->first, it->second);

            m_RoleBlackList.erase(it++);

            continue;
        }

        ++it;
    }


    if (g_pGameCenter->m_nWorkLoop % (GAME_FPS * 30) == 0)
    {
        ProcessDeleteList();
    }

	
    //if (g_pGameCenter->m_nWorkLoop % (GAME_FPS * 3600) == 0)
    //{
    //    CalculateCampActivePlayerCount();
    //}
    //m_lock.Unlock();
}

KAccount* KRoleManager::GetAccount(const char cszAccount[])
{
    KACCOUNT_NAME_INDEX::iterator it = m_AccountNameIndex.find(cszAccount);
    if (it != m_AccountNameIndex.end())
    {
        return it->second;
    }
    return NULL;
}

KRole* KRoleManager::AddRole(
    const char cszAccountName[], const char cszRoleName[], 
    DWORD dwID, BYTE byLevel,  BYTE byType, /*KCAMP eCamp, */BYTE byForceID, INT nFightScore, time_t nLastSaveTime, time_t nCreateTime
)
{
	m_lock.Lock();
    KRole*      pResult         = NULL;
    size_t      uNameLen        = 0;
    KAccount*   pAccount        = NULL;
    KRole*      pRole           = NULL;
    BOOL        bRoleInsert     = false;
    BOOL        bNameInsert     = false;
    BOOL        bNewAccount     = false;
    KACCOUNT_NAME_INDEX::iterator AccIter;
	std::pair<KROLE_TABLE::iterator, BOOL> RoleInsRet;
    std::pair<KROLE_NAME_INDEX::iterator, BOOL> NameIndexInsRet;
	//m_lock.Lock();
    RoleInsRet = m_RoleTable.insert(std::make_pair(dwID, KRole()));
	//m_lock.Unlock();
	LOG_PROCESS_ERROR(RoleInsRet.second);
    bRoleInsert = true;

    pRole = &RoleInsRet.first->second;

    pRole->m_dwPlayerID     = dwID;
    pRole->m_byLevel        = byLevel;
    pRole->m_byType         = byType;
    //pRole->m_eCamp          = eCamp;
    pRole->m_dwForceID      = byForceID;
    pRole->m_nLastSaveTime  = nLastSaveTime;
    pRole->m_nCreateTime    = nCreateTime;
	pRole->m_nFightScore    = nFightScore;

	pRole->SetId(dwID);
	pRole->SetPlayerName(cszRoleName);
	pRole->SetSex(byType);
	pRole->SetLevel(byLevel);
	pRole->SetPrimaryFaction(byForceID);
	
    uNameLen = strlen(cszRoleName);
    LOG_PROCESS_ERROR(uNameLen < sizeof(pRole->m_szName));
    strcpy(pRole->m_szName, cszRoleName);

    NameIndexInsRet = m_RoleNameIndex.insert(std::make_pair(pRole->m_szName, pRole));
    LOG_PROCESS_ERROR(RoleInsRet.second);
    bNameInsert = true;

    AccIter = m_AccountNameIndex.find(cszAccountName);
    if (AccIter != m_AccountNameIndex.end())
    {
        pAccount = AccIter->second;
    }
    else
    {
		std::pair<KACCOUNT_NAME_INDEX::iterator, BOOL> AccInsRet;

        //pAccount = KMemory::New<KAccount>();
		pAccount = new KAccount();
        LOG_PROCESS_ERROR(pAccount);
        bNewAccount = true;

        uNameLen = strlen(cszAccountName);
        LOG_PROCESS_ERROR(uNameLen < sizeof(pAccount->szName));
        strcpy(pAccount->szName, cszAccountName);

        AccInsRet = m_AccountNameIndex.insert(std::make_pair(pAccount->szName, pAccount));
        LOG_PROCESS_ERROR(AccInsRet.second);        
    }

    pAccount->vRoleList.push_back(pRole);
    pRole->m_pszAccount = pAccount->szName;

    pResult = pRole;
	
EXIT0:
    if (pResult == NULL)
    {
        AccIter = m_AccountNameIndex.find(cszAccountName);
        if (AccIter != m_AccountNameIndex.end())
        {
            int nRoleCount = 0;

            pAccount = AccIter->second;

            nRoleCount = (int)pAccount->vRoleList.size();

            for (int i = 0; i < nRoleCount; i++)
            {
                if (pAccount->vRoleList[i] == pRole)
                {
                    pAccount->vRoleList[i] = pAccount->vRoleList[nRoleCount - 1];
                    pAccount->vRoleList.resize(nRoleCount-1);
                    nRoleCount--;
                    break;
                }
            }
        }

        if (bNameInsert)
        {
            m_RoleNameIndex.erase(cszRoleName);
        }

        if (bRoleInsert)
        {
        	//m_lock.Lock();
            m_RoleTable.erase(dwID);
			//m_lock.Unlock();
		}

        if (bNewAccount)
        {
            //KMemory::Delete(pAccount);
			delete pAccount;
        }
    }

	m_lock.Unlock();
    return pResult;
}

KRole* KRoleManager::GetRole(DWORD dwID)
{
    KROLE_TABLE::iterator it = m_RoleTable.find(dwID);
    if (it != m_RoleTable.end())
    {
        return &it->second;
    }
    return NULL;
}

BOOL KRoleManager::GetRoleIds(std::vector<DWORD>& playerIds)
{
	KROLE_TABLE::iterator it;
	for (it = m_RoleTable.begin(); it != m_RoleTable.end(); ++it)
	{
        playerIds.push_back(it->first);
	}
	return TRUE;
}

BOOL KRoleManager::GetOnlineRoleIds(std::vector<DWORD>& playerIds)
{
	KROLE_TABLE::iterator it;
	for (it = m_RoleTable.begin(); it != m_RoleTable.end(); ++it)
	{
		if(it->second.IsOnline())
		{
			playerIds.push_back(it->first);
		}        
	}
	return TRUE;
}





KRole* KRoleManager::GetRole(const char cszName[])
{
    KROLE_NAME_INDEX::iterator it = m_RoleNameIndex.find(cszName);
    if (it != m_RoleNameIndex.end())
    {
        return it->second;
    }
    return NULL;
}

KRole* KRoleManager::GetRoleForAccount(const char cszAccount[])
{
	KRole*      pResult     = NULL;
	KAccount*   pAccount    = NULL;
	KACCOUNT_NAME_INDEX::iterator AccIter = m_AccountNameIndex.find(cszAccount);

	PROCESS_ERROR(AccIter != m_AccountNameIndex.end());

	pAccount = AccIter->second;

	for (KACC_ROLE_TAB::iterator it = pAccount->vRoleList.begin(); it != pAccount->vRoleList.end(); ++it)
	{
		pResult   = *it;
		break;
	}

EXIT0:
	return pResult;
}

KRole* KRoleManager::GetOnlineRoleForAccount(const char cszAccount[])
{
    KRole*      pResult     = NULL;
    KAccount*   pAccount    = NULL;
    KACCOUNT_NAME_INDEX::iterator AccIter = m_AccountNameIndex.find(cszAccount);

    PROCESS_ERROR(AccIter != m_AccountNameIndex.end());
            
    pAccount = AccIter->second;

    for (KACC_ROLE_TAB::iterator it = pAccount->vRoleList.begin(); it != pAccount->vRoleList.end(); ++it)
    {
        KRole*      pRole   = *it;
        KROLE_STATE eState  = rsInvalid;

        //assert(pRole);
		LOG_PROCESS_ERROR(pRole);
        eState = pRole->GetRoleState();
        if (eState != rsOffline)
        {
            pResult = pRole;
            break;
        }
    }

EXIT0:
    return pResult;
}

void KRoleManager::OnGSCrash(int nConnIndex)
{
	KROLE_TABLE::iterator it = m_RoleTable.begin();

	while (it != m_RoleTable.end())
	{
        KRole* pRole = &it->second;

		if (pRole->m_nConnIndex == nConnIndex || pRole->m_nDestConnIndex == nConnIndex)
        {
            pRole->OnLeaveGS(nConnIndex);
        }

		++it;
	}
}

BOOL KRoleManager::IsRoleBelongToAccount(DWORD dwRoleID, const char cszAccountName[])
{
    BOOL                            bResult     = false;
    KAccount*                       pAccount    = NULL;
    KACCOUNT_NAME_INDEX::iterator   AccIter     = m_AccountNameIndex.find(cszAccountName);
    KRole*                          pUserRole   = GetRole(dwRoleID);

    LOG_PROCESS_ERROR(AccIter != m_AccountNameIndex.end());
    LOG_PROCESS_ERROR(pUserRole != NULL);

    pAccount = AccIter->second;

    for (KACC_ROLE_TAB::iterator it = pAccount->vRoleList.begin(); it != pAccount->vRoleList.end(); ++it)
    {
        if (*it == pUserRole)
        {
            bResult = true; 
            break;
        }
    }

EXIT0:
    return bResult;
}

BOOL KRoleManager::FreezeRole(DWORD dwRoleID, time_t nFreezeEndTime)
{
	//m_lock.Lock();
    m_RoleBlackList[dwRoleID] = nFreezeEndTime;
	//m_lock.Unlock();
    return true;
}

time_t KRoleManager::GetRoleFreezeTime(DWORD dwRoleID)
{
    KROLE_BLACK_LIST::iterator  it = m_RoleBlackList.find(dwRoleID);

    if (it != m_RoleBlackList.end())
    {
        return it->second;
    }

    return 0;
}

void KRoleManager::RegisterDelRole(DWORD dwID, time_t nEndTime)
{
	//m_lock.Lock();
    m_RoleDeleteList[dwID] = nEndTime;
	//m_lock.Unlock();
}

void KRoleManager::UnregisterDelRole(DWORD dwID)
{
	//m_lock.Lock();
    KROLE_DELETE_LIST::iterator it = m_RoleDeleteList.find(dwID);
    if (it != m_RoleDeleteList.end())
    {
        m_RoleDeleteList.erase(it);
        g_pGameCenter->m_MiscDB.DoUpdateRoleDeleteList(dwID, 0);
    }
	//m_lock.Unlock();
}

time_t KRoleManager::GetRoleDeleteTime(DWORD dwID)
{
    KROLE_DELETE_LIST::iterator it = m_RoleDeleteList.find(dwID);

    if (it != m_RoleDeleteList.end())
    {
        return it->second;
    }

    return 0;
}

void KRoleManager::ProcessDeleteList()
{
	//m_lock.Lock();
    for (KROLE_DELETE_LIST::iterator it = m_RoleDeleteList.begin(); it != m_RoleDeleteList.end(); NULL)
    {
        if (g_pGameCenter->m_nTimeNow > it->second)
        {
            DelRole(it->first);

            g_pGameCenter->m_MiscDB.DoUpdateRoleDeleteList(it->first, it->second);
            m_RoleDeleteList.erase(it++);

            continue;
        }

        ++it;
    }
	//m_lock.Unlock();
}

BOOL KRoleManager::DelRole(DWORD dwID)
{
	m_lock.Lock();
    BOOL                            bResult         = false;
    int                             nRetCode        = false;
    KAccount*                       pAccount        = NULL;
    KRole*                          pRole           = NULL;
    KTong*                          pTong           = NULL; 
    KROLE_STATE                     eState          = rsInvalid;
    int                             nRoleCount      = 0;
    time_t                          nFreezeTime     = 0;
    KROLE_TABLE::iterator           RoleIter;
    KACCOUNT_NAME_INDEX::iterator   AccIter;
    KROLE_NAME_INDEX::iterator      NameIter;

    RoleIter = m_RoleTable.find(dwID);
    LOG_PROCESS_ERROR(RoleIter != m_RoleTable.end());

    pRole = &RoleIter->second;

    LOG_PROCESS_ERROR(pRole->GetRoleState() == rsOffline);

    // 冻结中的角色不允许删除
    nFreezeTime = g_pGameCenter->m_RoleManager.GetRoleFreezeTime(dwID);
    PROCESS_ERROR(g_pGameCenter->m_nTimeNow >= nFreezeTime);

    // 从帮会中移除成员，如果是帮主会操作失败
    if (pRole->m_dwTongID != KD_BAD_ID)
    {
        pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
        LOG_PROCESS_ERROR(pTong);

        nRetCode = pTong->RemoveMember(pRole);
        PROCESS_ERROR(nRetCode);
    }

    // 如果玩家属于某个队伍，要先从队伍删除
    if (pRole->m_dwTeamID != KD_BAD_ID)
    {
        nRetCode = g_pGameCenter->m_TeamCenter.DelMember(
            pRole->m_dwTeamID, pRole->m_dwPlayerID
        );
        LOG_PROCESS_ERROR(nRetCode);
    }

    NameIter = m_RoleNameIndex.find(pRole->m_szName);
    LOG_PROCESS_ERROR(NameIter != m_RoleNameIndex.end());

    AccIter = m_AccountNameIndex.find(pRole->m_pszAccount);
    LOG_PROCESS_ERROR(AccIter != m_AccountNameIndex.end());

    pRole->m_pszAccount = NULL;
    pAccount = AccIter->second;

    nRoleCount = (int)pAccount->vRoleList.size();
    for (int i = 0; i < nRoleCount; i++)
    {
        if (pAccount->vRoleList[i] == pRole)
        {
            pAccount->vRoleList[i] = pAccount->vRoleList[nRoleCount - 1];
            pAccount->vRoleList.resize(nRoleCount - 1);
            nRoleCount--;
            break;
        }
    }

    if (nRoleCount == 0)
    {
		SAFE_DELETE(pAccount);
        m_AccountNameIndex.erase(AccIter);
    }

    m_RoleNameIndex.erase(NameIter);
    m_RoleTable.erase(RoleIter);
    g_pGameCenter->m_RoleDB.DoDeleteRole(pRole->m_dwPlayerID);

    g_pGameCenter->m_MailManager.PostRequestDeleteAll(pRole->m_dwPlayerID);

    bResult = true;
EXIT0:
	m_lock.Unlock();
    return bResult;
}

//struct KCalculateCampActivePlayerCount
//{
//    BOOL operator () (DWORD dwID, KRole& rRole)
//    {
//        int nLevel = 0;
//
//        if (g_pGameCenter->m_nTimeNow - rRole.m_nLastSaveTime > ACTIVE_PLAYER_TIME)
//        {
//            return true;
//        }
//
//        assert(rRole.m_eCamp == cNeutral || rRole.m_eCamp == cEvil || rRole.m_eCamp == cGood);
//
//        ++m_nActivePlayerCount[rRole.m_eCamp];
//
//        return true;
//    }
//
//    int m_nActivePlayerCount[cTotal];
//};
//
//void KRoleManager::CalculateCampActivePlayerCount()
//{
//    KCalculateCampActivePlayerCount Calculate;
//
//    memset(Calculate.m_nActivePlayerCount, 0, sizeof(Calculate.m_nActivePlayerCount));
//
//    Traverse(Calculate);
//    
//    memcpy(
//        g_pGameCenter->m_CampManager.m_nActivePlayerCount, 
//        Calculate.m_nActivePlayerCount, 
//        sizeof(g_pGameCenter->m_CampManager.m_nActivePlayerCount)
//    );
//
//    g_pGameCenter->m_GameServer.DoSyncCampInfo(-1);
//}

BOOL KRoleManager::RoleRename(DWORD dwID, char szNewName[])
{
	//m_lock.Lock();
    BOOL                                    bResult = false;
    KRole*                                  pRole   = NULL;
    KROLE_NAME_INDEX::iterator              itFind;
    KROLE_NAME_INDEX::iterator              itFindNew;
	std::pair<KROLE_NAME_INDEX::iterator, BOOL>  NameIndexInsRet;

    pRole = GetRole(dwID);
    LOG_PROCESS_ERROR(pRole);

    itFind = m_RoleNameIndex.find(pRole->m_szName);
    LOG_PROCESS_ERROR(itFind != m_RoleNameIndex.end());

    itFindNew = m_RoleNameIndex.find(szNewName);
    LOG_PROCESS_ERROR(itFindNew == m_RoleNameIndex.end());

    m_RoleNameIndex.erase(itFind);

    strncpy(pRole->m_szName, szNewName, sizeof(pRole->m_szName));
    pRole->m_szName[sizeof(pRole->m_szName) - 1] = '\0';

    NameIndexInsRet = m_RoleNameIndex.insert(std::make_pair(pRole->m_szName, pRole));
    LOG_PROCESS_ERROR(NameIndexInsRet.second);

    bResult = true;
	
EXIT0:
	//m_lock.Unlock();
    return bResult;
}

void KRoleManager::RecordRenameRole(DWORD dwRoleID)
{
	//m_lock.Lock();
    m_RenameRoleTable.insert(dwRoleID);
	//m_lock.Unlock();
}

BOOL KRoleManager::IsCanRename(DWORD dwRoleID)
{
    BOOL                            bResult     = false;
    KRENAME_ROLE_TABLE::iterator    itFind;
    itFind = m_RenameRoleTable.find(dwRoleID);
    PROCESS_ERROR(itFind != m_RenameRoleTable.end());
    bResult = true;
EXIT0:
    return bResult;
}

void KRoleManager::RemoveRenameRole(DWORD dwRoleID)
{
	//m_lock.Lock();
    KRENAME_ROLE_TABLE::iterator    itFind;
    itFind = m_RenameRoleTable.find(dwRoleID);
    LOG_PROCESS_ERROR(itFind != m_RenameRoleTable.end());
    m_RenameRoleTable.erase(itFind);
EXIT0:
	//m_lock.Unlock();
    return;
}

BOOL KRoleManager::OnSaveRoleData(DWORD dwId, const KRoleBaseInfo& rsBaseInfo)
{
	BOOL bResult = FALSE;
	KRole* pRole = GetRole(dwId);
	LOG_PROCESS_ERROR(pRole);

	pRole->SetLevel(rsBaseInfo.byLevel);

	bResult = TRUE;
EXIT0:
	return bResult;
}