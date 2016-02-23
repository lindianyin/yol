#include "stdafx.h"
#include "TeamCenter.h"
#include "teammodule_gc.h"
#include "gclogicbase/kgcplayercommon.h"
#include "gclogicbase/kgclogicextinterface.h"
#include "onlinegamebase/ksysservice.h"
#include "GameCenter.h"

QTeamCenter::QTeamCenter()
{
}

BOOL QTeamCenter::Init()
{
    m_dwTeamSNGenerator = KD_BAD_ID;

    return true;
}

void QTeamCenter::UnInit()
{
}

void QTeamCenter::Activate()
{
    for (KTEAM_TABLE::iterator it = m_TeamTable.begin(); it != m_TeamTable.end(); NULL)
    {
        if ((g_pGameCenter->m_nWorkLoop + (int)it->first) % GAME_FPS)
        {
            ProcessTeam(it->first, &it->second);
        }

        if (it->second.bDelete)
        {
            KTeam* pTeam = &it->second;

            g_cGcTeamModule.DoTeamDisbandNotify(it->first);

            for (int i = 0; i < pTeam->nGroupNum; i++)
            {
                for (
                    KTEAM_MEMBER_LIST::iterator ListIter = pTeam->MemberGroup[i].MemberList.begin();
                    ListIter != pTeam->MemberGroup[i].MemberList.end(); ++ListIter
                )
                {
                    KRole* pRole = NULL;

                    pRole = g_pGameCenter->m_RoleManager.GetRole(ListIter->dwMemberID);
                    if(!pRole)
                        continue;

                    SetRoleTeamID(pRole, pTeam->bSystem, KD_BAD_ID);
                }
            }

            m_TeamTable.erase(it++);
            continue;
        }

        ++it;
    }

    if (g_pGameCenter->m_nWorkLoop & (GAME_FPS * 2))
    {
        ProcessInviteAndApplyTimeOut();
    }
}

DWORD QTeamCenter::CreateTeam(BOOL bSystem)
{
    DWORD dwResult  = KD_BAD_ID;
    DWORD dwTeamID  = ++m_dwTeamSNGenerator;
    KTeam Team;

    std::pair<KTEAM_TABLE::iterator, BOOL> InsertRet;

    Team.bDelete            = false;
    memset(Team.dwAuthority, 0, sizeof(Team.dwAuthority));
    memset(Team.dwTeamMark, 0, sizeof(Team.dwTeamMark));
    //Team.eCamp              = cNeutral;
    Team.nLootMode          = ilmGroupLoot;
    Team.nRollQuality       = 2;
    Team.nGroupNum          = MIN_TEAM_GROUP_NUM;
	Team.nLastActiveTime    = KSysService::GameTime(NULL);
    Team.bSystem            = bSystem;

    for (int i = 0; i < MAX_TEAM_GROUP_NUM; i++)
    {
        Team.MemberGroup[i].dwFormationLeader = KD_BAD_ID;
        Team.MemberGroup[i].MemberList.clear();
    }

	InsertRet = m_TeamTable.insert(std::make_pair(dwTeamID, Team));
    LOG_PROCESS_ERROR(InsertRet.second);

    g_cGcTeamModule.DoTeamCreateNotify(dwTeamID);

    dwResult = dwTeamID;
EXIT0:
	if (dwResult != KD_BAD_ID)
	{
		QLogPrintf(LOG_INFO, "QTeamCenter::CreateTeam succeed! team_id=%d\n", dwTeamID);
	}
	else
	{
		QLogPrintf(LOG_ERR, "QTeamCenter::CreateTeam failed! team_id=%d\n", dwTeamID);
	}
    return dwResult;
}

BOOL QTeamCenter::DestroyTeam(DWORD dwTeamID)
{
    BOOL    bResult = false;
    KTeam*  pTeam   = NULL;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    pTeam->bDelete = true;

    bResult = true;
EXIT0:
	if (bResult)
	{
		QLogPrintf(LOG_INFO, "QTeamCenter::DestroyTeam succeed! team_id=%d\n", dwTeamID);
	}
	else
	{
		QLogPrintf(LOG_ERR, "QTeamCenter::DestroyTeam failed! team_id=%d\n", dwTeamID);
	}
    return bResult;
}

KTeam* QTeamCenter::GetTeam(DWORD dwTeamID)
{
    KTeam*                  pTeam   = NULL;
    KTEAM_TABLE::iterator   itFind;

    itFind = m_TeamTable.find(dwTeamID);
    PROCESS_ERROR(itFind != m_TeamTable.end());

    pTeam = &itFind->second;
EXIT0:
    return pTeam;
}

BOOL QTeamCenter::AddMember(DWORD dwTeamID, DWORD dwRoleID, BOOL bSystem /* = 0 */)
{
    BOOL                    bResult     = false;
    KTeam*                  pTeam       = NULL;
    KRole*                  pRole       = NULL;
    int                     nGroupIndex = 0;
    KTEAM_MEMBER_INFO       MemberInfo;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    for (nGroupIndex = 0; nGroupIndex < pTeam->nGroupNum; nGroupIndex++)
    {
        if (pTeam->MemberGroup[nGroupIndex].MemberList.size() < MAX_PLAYER_PER_GROUP)
            break;
    }

    LOG_PROCESS_ERROR(nGroupIndex < pTeam->nGroupNum); // 可能是队伍人数满了

    MemberInfo.dwMemberID = dwRoleID;
    MemberInfo.bIsOnline  = true;

    pTeam->MemberGroup[nGroupIndex].MemberList.push_back(MemberInfo);

    g_cGcTeamModule.DoTeamAddMemberNotify(dwTeamID, nGroupIndex, dwRoleID, bSystem);

    pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleID);
    LOG_PROCESS_ERROR(pRole);
	pRole->m_nLastTeamActiveTime = KSysService::GameTime(NULL);

    SetRoleTeamID(pRole, pTeam->bSystem, dwTeamID);

    bResult = true;
EXIT0:
	if (bResult)
	{
		QLogPrintf(LOG_INFO, "QTeamCenter::AddMember succeed! team_id=%d, role_id=%d\n", dwTeamID, dwRoleID);
	}
	else
	{
		QLogPrintf(LOG_ERR, "QTeamCenter::AddMember failed! team_id=%d, role_id=%d\n", dwTeamID, dwRoleID);
	}
    return bResult;
}

BOOL QTeamCenter::DelMember(DWORD dwTeamID, DWORD dwRoleID)
{
    BOOL                    bResult     = false;
    KTeam*                  pTeam       = NULL;
    KRole*                  pRole       = NULL;
    int                     nGroupIndex = 0;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    for (nGroupIndex = 0; nGroupIndex < pTeam->nGroupNum; nGroupIndex++)
    {
        for (
            KTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[nGroupIndex].MemberList.begin();
            it != pTeam->MemberGroup[nGroupIndex].MemberList.end(); ++it
        )
        {
            if (it->dwMemberID == dwRoleID)
            {
                pTeam->MemberGroup[nGroupIndex].MemberList.erase(it);

                pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleID);
                LOG_PROCESS_ERROR(pRole);

                SetRoleTeamID(pRole, pTeam->bSystem, KD_BAD_ID);

                g_cGcTeamModule.DoTeamDelMemberNotify(dwTeamID, nGroupIndex, dwRoleID, pRole->getName());

                bResult = true;
                goto EXIT0;
            }
        }
    }

EXIT0:
	if (bResult)
	{
		QLogPrintf(LOG_INFO, "QTeamCenter::DelMember succeed! team_id=%d, role_id=%d\n", dwTeamID, dwRoleID);
	}
	else
	{
		QLogPrintf(LOG_ERR, "QTeamCenter::DelMember error! team_id=%d, role_id=%d\n", dwTeamID, dwRoleID);
	}
    return bResult;
}

BOOL QTeamCenter::ChangeGroup(DWORD dwTeamID, int nDstGroupIndex, DWORD dwDstRoleID, DWORD dwRoleID)
{
    BOOL                    bResult     = false;
    KTeam*                  pTeam       = NULL;
    KTEAM_MEMBER_INFO       TmpMemberInfo;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    LOG_PROCESS_ERROR(nDstGroupIndex >= 0 && nDstGroupIndex < pTeam->nGroupNum);

    TmpMemberInfo.dwMemberID = KD_BAD_ID;
    TmpMemberInfo.bIsOnline  = false;

    for (
        KTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[nDstGroupIndex].MemberList.begin();
        it != pTeam->MemberGroup[nDstGroupIndex].MemberList.end(); ++it
    )
    {
        if (it->dwMemberID == dwDstRoleID)
        {
            TmpMemberInfo = *it;

            pTeam->MemberGroup[nDstGroupIndex].MemberList.erase(it);
            break;
        }
    }

    PROCESS_ERROR(pTeam->MemberGroup[nDstGroupIndex].MemberList.size() < MAX_PLAYER_PER_GROUP);

    for (int nGroupIndex = 0; nGroupIndex < pTeam->nGroupNum; nGroupIndex++)
    {
        for (
            KTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[nGroupIndex].MemberList.begin();
            it != pTeam->MemberGroup[nGroupIndex].MemberList.end(); ++it
        )
        {
            if (it->dwMemberID != dwRoleID)
                continue;

            pTeam->MemberGroup[nDstGroupIndex].MemberList.push_back(*it);
            pTeam->MemberGroup[nGroupIndex].MemberList.erase(it);

            if (TmpMemberInfo.dwMemberID != KD_BAD_ID)
                pTeam->MemberGroup[nGroupIndex].MemberList.push_back(TmpMemberInfo);

            g_cGcTeamModule.DoTeamChangeMemberGroupNotify(
                dwTeamID, dwRoleID, nGroupIndex, dwDstRoleID, nDstGroupIndex
            );

            bResult = true;
            goto EXIT0;
        }
    }
EXIT0:
    return bResult;
}

BOOL QTeamCenter::SetAuthority(DWORD dwTeamID, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID)
{
    BOOL                    bResult     = false;
    KTeam*                  pTeam       = NULL;
	KRole*					pRole		= NULL;
	QLogPrintf(LOG_INFO, "QTeamCenter::SetAuthority(DWORD dwTeamID=%d, TEAM_AUTHORITY_TYPE eType=%d, DWORD dwTargetID=%d) "
							,dwTeamID,eType,dwTargetID);
    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    LOG_PROCESS_ERROR(eType > tatInvalid && eType < tatTotal);

    pTeam->dwAuthority[eType] = dwTargetID;
	if (eType == tatLeader)
	{
		pRole = g_pGameCenter->m_RoleManager.GetRole(dwTargetID);
		LOG_PROCESS_ERROR(pRole);

		pRole->m_dwTeamID = dwTeamID;
	}

	if(eType == tatLeader)
	{
		g_cGcTeamModule.DoTeamChangeAuthorityNotify(dwTeamID, eType, dwTargetID);
	}

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamCenter::SetFormationLeader(DWORD dwTeamID, int nGroupIndex, DWORD dwRequestSrcID, DWORD dwTargetID)
{
    BOOL                    bResult     = false;
    KTeam*                  pTeam       = NULL;
    KTEAM_TABLE::iterator   itFind;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    PROCESS_ERROR(nGroupIndex >= 0 && nGroupIndex < pTeam->nGroupNum);

    PROCESS_ERROR(
        dwRequestSrcID == pTeam->dwAuthority[tatLeader] ||
        dwRequestSrcID == pTeam->MemberGroup[nGroupIndex].dwFormationLeader
    );

    for (
        KTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[nGroupIndex].MemberList.begin();
        it != pTeam->MemberGroup[nGroupIndex].MemberList.end(); ++it
    )
    {
        if (it->dwMemberID == dwTargetID)
        {
            pTeam->MemberGroup[nGroupIndex].dwFormationLeader = dwTargetID;

            g_cGcTeamModule.DoTeamSetFormationLeaderNotify(dwTeamID, nGroupIndex, dwTargetID);

            bResult = true;
            goto EXIT0;
        }
    }

EXIT0:
    return bResult;
}

BOOL QTeamCenter::SetMark(DWORD dwTeamID, int nMarkType, DWORD dwTargetID)
{
    BOOL    bResult = false;
    KTeam*  pTeam   = NULL;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    for (int i = 0; i < TEAM_MARK_COUNT; i++)
    {
        if (pTeam->dwTeamMark[i] == dwTargetID)
        {
            pTeam->dwTeamMark[i] = 0;
        }
    }

    PROCESS_SUCCESS(nMarkType == 0); // 等于0的时候表示取消标记
    LOG_PROCESS_ERROR(nMarkType > 0 && nMarkType <= TEAM_MARK_COUNT);

    pTeam->dwTeamMark[nMarkType - 1] = dwTargetID;

EXIT1:
    g_cGcTeamModule.DoTeamSetMarkRespond(dwTeamID, nMarkType, dwTargetID);

    bResult = true;
EXIT0:
    return bResult;
}

void QTeamCenter::SyncNewServer(int nConnIndex)
{
    for (KTEAM_TABLE::iterator it = m_TeamTable.begin(); it != m_TeamTable.end(); ++it)
    {
        g_cGcTeamModule.DoSyncTeamInfo(nConnIndex, it->first, &it->second);
    }
}

int QTeamCenter::GetTeamMemberCount(DWORD dwTeamID)
{
    int     nCount = 0;
    KTeam*  pTeam  = NULL;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    for (int i = 0; i < pTeam->nGroupNum; i++)
    {
        nCount += (int)pTeam->MemberGroup[i].MemberList.size();
    }

EXIT0:
    return nCount;
}

BOOL QTeamCenter::IsRoleInTeam(DWORD dwTeamID, DWORD dwRoleID)
{
    BOOL    bResult = false;
    KTeam*  pTeam   = NULL;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    for (int i = 0; i < pTeam->nGroupNum; i++)
    {
        for (
            KTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
            it != pTeam->MemberGroup[i].MemberList.end(); ++it
        )
        {
            if (dwRoleID == it->dwMemberID)
            {
                bResult = true;
                goto EXIT0;
            }
        }
    }

EXIT0:
    return bResult;
}

BOOL QTeamCenter::RegisterInvite(DWORD dwInviteSrc, DWORD dwInviteDst)
{
    BOOL                    bResult = false;
    INT64                 nKey    = MAKE_INT64(dwInviteSrc, dwInviteDst);
    KINVITE_TABLE::iterator itFind;

    itFind = m_InviteTable.find(nKey);
    PROCESS_ERROR(itFind == m_InviteTable.end());

	m_InviteTable.insert(std::make_pair(nKey, KSysService::GameTime(NULL)));

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamCenter::UnregisterInvite(DWORD dwInviteSrc, DWORD dwInviteDst)
{
    BOOL                    bResult = false;
    INT64                 nKey    = MAKE_INT64(dwInviteSrc, dwInviteDst);
    KINVITE_TABLE::iterator itFind;

    itFind = m_InviteTable.find(nKey);
    PROCESS_ERROR(itFind != m_InviteTable.end());

    m_InviteTable.erase(itFind);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamCenter::ClearInvite(DWORD dwInviteSrc)
{
    BOOL                    bResult  = false;
    INT64                 nKeyLow  = MAKE_INT64(dwInviteSrc, 0);
    INT64                 nKeyHigh = MAKE_INT64(dwInviteSrc + 1, 0);
    KINVITE_TABLE::iterator itLow;
    KINVITE_TABLE::iterator itHigh;
    KINVITE_TABLE::iterator it;

    PROCESS_ERROR(nKeyLow < nKeyHigh);
    itLow  = m_InviteTable.lower_bound(nKeyLow);
    itHigh = m_InviteTable.upper_bound(nKeyHigh);

    for (it = itLow; it != itHigh;)
    {
        m_InviteTable.erase(it++);
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamCenter::IsInviteExist(DWORD dwInviteSrc, DWORD dwInviteDst)
{
    BOOL                    bResult = false;
    INT64                 nKey    = MAKE_INT64(dwInviteSrc, dwInviteDst);
    KINVITE_TABLE::iterator itFind;
    
    itFind = m_InviteTable.find(nKey);
    PROCESS_ERROR(itFind != m_InviteTable.end());

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamCenter::RegisterApply(DWORD dwApplySrc, DWORD dwApplyDst)
{
    BOOL                    bResult = false;
    INT64                 nKey    = MAKE_INT64(dwApplySrc, dwApplyDst);
    KAPPLY_TABLE::iterator itFind;

    itFind = m_ApplyTable.find(nKey);
    PROCESS_ERROR(itFind == m_ApplyTable.end());

	m_ApplyTable.insert(std::make_pair(nKey, KSysService::GameTime(NULL)));

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamCenter::UnregisterApply(DWORD dwApplySrc, DWORD dwApplyDst)
{
    BOOL                    bResult = false;
    INT64                 nKey    = MAKE_INT64(dwApplySrc, dwApplyDst);
    KAPPLY_TABLE::iterator itFind;

    itFind = m_ApplyTable.find(nKey);
    PROCESS_ERROR(itFind != m_ApplyTable.end());

    m_ApplyTable.erase(itFind);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamCenter::ClearApply(DWORD dwApplySrc)
{
    BOOL                    bResult  = false;
    INT64                 nKeyLow  = MAKE_INT64(dwApplySrc, 0);
    INT64                 nKeyHigh = MAKE_INT64(dwApplySrc + 1, 0);
    KAPPLY_TABLE::iterator  itLow;
    KAPPLY_TABLE::iterator  itHigh;
    KAPPLY_TABLE::iterator  it;

    PROCESS_ERROR(nKeyLow < nKeyHigh);
    itLow  = m_ApplyTable.lower_bound(nKeyLow);
    itHigh = m_ApplyTable.upper_bound(nKeyHigh);

    for (it = itLow; it != itHigh;)
    {
        m_InviteTable.erase(it++);
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamCenter::IsApplyExist(DWORD dwApplySrc, DWORD dwApplyDst)
{
    BOOL                    bResult = false;
    INT64                 nKey    = MAKE_INT64(dwApplySrc, dwApplyDst);
    KAPPLY_TABLE::iterator  itFind;

    itFind = m_ApplyTable.find(nKey);
    PROCESS_ERROR(itFind != m_ApplyTable.end());

    bResult = true;
EXIT0:
    return bResult;
}

int QTeamCenter::GetTeamCount()
{
    return (int)m_TeamTable.size();
}

void QTeamCenter::ProcessTeam(DWORD dwTeamID, KTeam* pTeam)
{
    ProcessAuthority(dwTeamID, pTeam);

    ProcessFormation(dwTeamID, pTeam);

    ProcessMember(dwTeamID, pTeam);
}

void QTeamCenter::ProcessAuthority(DWORD dwTeamID, KTeam* pTeam)
{
    // ----------------- 如果拥有权限的人没有在线或不在队伍中，则回收他的权限----------->>
    for (int i = tatInvalid + 1; i < tatTotal; i++)
    {
        KRole* pRole = NULL;

        if (pTeam->dwAuthority[i] == KD_BAD_ID)
            continue;

        pRole = g_pGameCenter->m_RoleManager.GetRole(pTeam->dwAuthority[i]);
        if (
            !pRole || !pRole->IsOnline() ||
            (pRole->GetTeamId() != dwTeamID && pRole->m_dwSystemTeamID != dwTeamID)
        )
        {
            pTeam->dwAuthority[i] = KD_BAD_ID;
        }
    }
    // <<---------------------------------------------------------------------------------

    // ----------------- 如果队长权限没有人拥有，则按规则分配 -------------------------->>
    if (pTeam->dwAuthority[tatLeader] == KD_BAD_ID && pTeam->dwAuthority[tatDistribute] != KD_BAD_ID)
    {
        SetAuthority(dwTeamID, tatLeader, pTeam->dwAuthority[tatDistribute]);

        OnChangeLeader(pTeam->dwAuthority[tatLeader]);
    }

    if (pTeam->dwAuthority[tatLeader] == KD_BAD_ID && pTeam->dwAuthority[tatMark] != KD_BAD_ID)
    {
        SetAuthority(dwTeamID, tatLeader, pTeam->dwAuthority[tatMark]);

        OnChangeLeader(pTeam->dwAuthority[tatLeader]);
    }

    if (pTeam->dwAuthority[tatLeader] != KD_BAD_ID)
    {
        if (pTeam->dwAuthority[tatDistribute] == KD_BAD_ID)
        {
            SetAuthority(dwTeamID, tatDistribute, pTeam->dwAuthority[tatLeader]);
        }

        if (pTeam->dwAuthority[tatMark] == KD_BAD_ID)
        {
            SetAuthority(dwTeamID, tatMark, pTeam->dwAuthority[tatLeader]);
        }
    }
    // <<---------------------------------------------------------------------------------
}

void QTeamCenter::ProcessFormation(DWORD dwTeamID, KTeam* pTeam)
{
    for (int i = 0; i < pTeam->nGroupNum; i++)
    {
        KRole* pRole        = NULL;
        BOOL   bIsInGroup   = false;

        pRole = g_pGameCenter->m_RoleManager.GetRole(pTeam->MemberGroup[i].dwFormationLeader);

        for (
            KTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
            it != pTeam->MemberGroup[i].MemberList.end(); ++it
        )
        {
            if (it->dwMemberID == pTeam->MemberGroup[i].dwFormationLeader)
            {
                bIsInGroup = true;
                break;
            }
        }

        // ------------- 如果阵眼不在线或者不在队伍中，则收回他的权限。----------------->>
        if (!pRole || !pRole->IsOnline() || !bIsInGroup)
        {
            pTeam->MemberGroup[i].dwFormationLeader = KD_BAD_ID;
        }
        // <<-----------------------------------------------------------------------------
    }
}

void QTeamCenter::ProcessMember(DWORD dwTeamID, KTeam* pTeam)
{
    int     nActiveMemberCount  = 0;
    int     nMemberCount        = GetTeamMemberCount(dwTeamID);
    // BOOL    bNotNeutral         = false;

    // ----------------- 如果队伍所有人下线超过一定时间或者所有人都退出了，则解散掉 ---->>
    if (
        !pTeam->bSystem &&
        (KSysService::GameTime(NULL) - pTeam->nLastActiveTime > EMPTY_TEAM_TIME_OUT /*|| nMemberCount < 2*/)
    )
    {
        DestroyTeam(dwTeamID);
    }
    // <<---------------------------------------------------------------------------------

    for (int nGroupIndex = 0; nGroupIndex < pTeam->nGroupNum; nGroupIndex++)
    {
        for (
            KTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[nGroupIndex].MemberList.begin();
            it != pTeam->MemberGroup[nGroupIndex].MemberList.end(); NULL
        )
        {
            KRole* pRole = NULL;

            pRole = g_pGameCenter->m_RoleManager.GetRole(it->dwMemberID);

            if (!pRole)
            {
                QLogPrintf(LOG_ERR, "[TEAM] Error Role in team.\n");

                pTeam->MemberGroup[nGroupIndex].MemberList.erase(it++);
                continue;
            }

            // if (pRole->m_eCamp != cNeutral)
            // {
            //     if (pTeam->eCamp == cNeutral)
            //     {
            //         pTeam->eCamp = pRole->m_eCamp;
            // 
            //         g_cGcTeamModule.DoTeamCampChange(dwTeamID, pTeam->eCamp);
            //     }
            // 
            //     bNotNeutral = true;
            // }

            // --------- 如果队伍和该成员的阵营是冲突的，则该成员离开队伍 -------------->>
            // if (pRole->m_eCamp != cNeutral && pTeam->eCamp != cNeutral && pRole->m_eCamp != pTeam->eCamp)
            // {
            //     QLogPrintf(LOG_ERR, "[TEAM] Camp error, in Team(%lu), Role:%s\n", dwTeamID, pRole->m_szName);
            // 
            //     pTeam->MemberGroup[nGroupIndex].MemberList.erase(it++);
            // 
            //     if (pTeam->bSystem)
            //     {
            //         pRole->m_dwSystemTeamID = KD_BAD_ID;
            //     }
            //     else
            //     {
            //         pRole->m_dwTeamID = KD_BAD_ID;
            //     }
            // 
            //     g_cGcTeamModule.DoTeamDelMemberNotify(dwTeamID, nGroupIndex, pRole->GetId());
            // }
            // <<-------------------------------------------------------------------------

            if (!pRole->IsOnline())
            {
				if (it->bIsOnline)
              {
                 it->bIsOnline = false;
                 g_cGcTeamModule.DoSyncTeamMemberOnlineFlag(dwTeamID, it->dwMemberID, false);
					++it;
              }
				else
				{
					++it;
					if (KSysService::GameTime(NULL) - pRole->m_nLastTeamActiveTime > LOGOUT_TEAM_TIME_OUT)
					{
						DelMember(dwTeamID, pRole->GetId());
					}
				} 
              continue;
            }

            if (pRole->IsOnline() && !it->bIsOnline)
            {
                it->bIsOnline = true;
                g_cGcTeamModule.DoSyncTeamMemberOnlineFlag(
                    dwTeamID, it->dwMemberID, true
                );
            }

            nActiveMemberCount++;
			  pRole->m_nLastTeamActiveTime = KSysService::GameTime(NULL);

            if (pTeam->dwAuthority[tatLeader] == KD_BAD_ID) // 如果这时候队长权限还没分配出去，就给这个角色
            {
                SetAuthority(dwTeamID, tatLeader, pRole->GetId());

                OnChangeLeader(pTeam->dwAuthority[tatLeader]);
            }

            if (pTeam->MemberGroup[nGroupIndex].dwFormationLeader == KD_BAD_ID)
            {
                pTeam->MemberGroup[nGroupIndex].dwFormationLeader = pRole->GetId();

                g_cGcTeamModule.DoTeamSetFormationLeaderNotify(
                    dwTeamID, nGroupIndex, pTeam->MemberGroup[nGroupIndex].dwFormationLeader
                );
            }

            ++it;
        }
    }

    if (nMemberCount > 0 && pTeam->dwAuthority[tatLeader] == KD_BAD_ID)
    {
        for (int nGroupIndex = 0; nGroupIndex < pTeam->nGroupNum; nGroupIndex++)
        {
            if (pTeam->MemberGroup[nGroupIndex].MemberList.empty())
                continue;

            pTeam->dwAuthority[tatLeader] = pTeam->MemberGroup[nGroupIndex].MemberList.front().dwMemberID;
            break;
        }
    }

    if (nActiveMemberCount > 0)
    {
        pTeam->nLastActiveTime = KSysService::GameTime(NULL);
    }

    // if (!bNotNeutral && pTeam->eCamp != cNeutral)
    // {
    //     pTeam->eCamp = cNeutral;
    //     g_cGcTeamModule.DoTeamCampChange(dwTeamID, pTeam->eCamp);
    // }
}

void QTeamCenter::ProcessInviteAndApplyTimeOut()
{
    for (KINVITE_TABLE::iterator it = m_InviteTable.begin(); it != m_InviteTable.end(); NULL)
    {
        if (KSysService::GameTime(NULL) - it->second > INVITE_AND_APPLY_TIME_OUT)
        {
            m_InviteTable.erase(it++);
            continue;
        }

        ++it;
    }

    for (KAPPLY_TABLE::iterator it = m_ApplyTable.begin(); it != m_ApplyTable.end(); NULL)
    {
        if (KSysService::GameTime(NULL) - it->second > INVITE_AND_APPLY_TIME_OUT)
        {
            m_ApplyTable.erase(it++);
            continue;
        }

        ++it;
    }
}

void QTeamCenter::OnChangeLeader(DWORD dwLeaderID)
{
    BOOL        bRetCode    = false;
    KRole*      pLeader     = NULL;
    KMapInfo*   pMapInfo    = NULL;

    pLeader = g_pGameCenter->m_RoleManager.GetRole(dwLeaderID);
    LOG_PROCESS_ERROR(pLeader);

    pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pLeader->m_dwMapID);
    LOG_PROCESS_ERROR(pMapInfo);

    LOG_PROCESS_ERROR(pMapInfo->m_nType == emtDungeon);

    bRetCode = g_pGameCenter->m_MapManager.SetMapCopyOwner(pLeader->m_dwMapID, pLeader->m_nMapCopyIndex, dwLeaderID);
    LOG_PROCESS_ERROR(bRetCode);

EXIT0:
    return;
}

void QTeamCenter::SetRoleTeamID(KRole* pRole, BOOL bSystem, DWORD dwTeamID)
{
	QLogPrintf(LOG_INFO,"..QTeamCenter::SetRoleTeamID(KRole* pRole, BOOL bSystem=%d, DWORD dwTeamID=%d)"
						,bSystem,dwTeamID);
    KTeam* pTeam = NULL;

    assert(pRole);

    if (bSystem)
    {
        pRole->m_dwSystemTeamID = dwTeamID;
    }
    else
    {
        pRole->m_dwTeamID = dwTeamID;
    }
}
