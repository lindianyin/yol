#include "stdafx.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamemodule/qplayerteam.h"
#include "qteamserver.h"
#include "kteammodule.h"
#include "../onlinegameworld/kplayerbaseprocess.h"

#ifdef GAME_SERVER

QTeamServer g_cTeamServer;


QTeamBroadcast::QTeamBroadcast()
{
	m_dwTeamID      = KD_BAD_ID;
	m_dwExceptID    = KD_BAD_ID;
	m_pvData        = NULL;
	m_uSize         = 0;
}

BOOL QTeamBroadcast::operator () (QTEAM_MEMBER_INFO* pMemberInfo)
{
	KPlayer* pMember = g_cPlayerMgr.GetById(pMemberInfo->dwMemberID);
	if (!pMember)
		return true;

	QTeamModule::SendUnknownData(pMember->GetPlayerIndex(), (LPSTR)m_pvData, m_uSize);

	return true;
}

QTeamServer::QTeamServer()
{
}

BOOL QTeamServer::Init()
{
    return true;
}

void QTeamServer::UnInit()
{
}

QTeam* QTeamServer::GetTeam(DWORD dwTeamID)
{
    QTeam*                  pTeam   = NULL;
    QTEAM_TABLE::iterator   itFind;

    itFind = m_TeamTable.find(dwTeamID);
    PROCESS_ERROR(itFind != m_TeamTable.end());

    pTeam = &itFind->second;
EXIT0:
    return pTeam;
}

BOOL QTeamServer::Create(DWORD dwTeamID)
{
    BOOL    bResult = false;
    QTeam   Team;

    memset(Team.dwAuthority, 0, sizeof(Team.dwAuthority));
    memset(Team.dwMark, 0, sizeof(Team.dwMark));

    Team.bSystem    = false;
    
    for (int i = 0; i < MAX_TEAM_GROUP_NUM; i++)
    {
        Team.MemberGroup[i].dwFormationLeader = KD_BAD_ID;
        Team.MemberGroup[i].MemberList.clear();
    }

    Team.nGroupNum      = MIN_TEAM_GROUP_NUM;
    Team.nLootMode      = ilmGroupLoot;
    Team.nRollQuality   = 2;

	m_TeamTable.insert(std::make_pair(dwTeamID, Team));

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamServer::Destroy(DWORD dwTeamID)
{
    BOOL                    bResult = false;
    QTeam*                  pTeam   = NULL;
    QTEAM_TABLE::iterator   itFind;

    itFind = m_TeamTable.find(dwTeamID);
    LOG_PROCESS_ERROR(itFind != m_TeamTable.end());

    pTeam = &itFind->second;

    for (int i = 0; i < pTeam->nGroupNum; i++)
    {
        for (
            QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
            it != pTeam->MemberGroup[i].MemberList.end(); ++it
        )
        {
            KPlayer* pPlayer = g_cPlayerMgr.GetById(it->dwMemberID);

            if (!pPlayer)
                continue;

			pPlayer->SetPlayerTeamId(pTeam->bSystem, KD_BAD_ID);
        }
    }

    m_TeamTable.erase(itFind);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamServer::AddMember(DWORD dwTeamID, int nGroupIndex, DWORD dwPlayerID, BOOL bSystem /* = 0 */)
{
    BOOL                bResult     = false;
    QTeam*              pTeam       = NULL;
    KPlayer*            pPlayer     = NULL;
    QTEAM_MEMBER_INFO   MemberInfo;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    LOG_PROCESS_ERROR(nGroupIndex < pTeam->nGroupNum);

    LOG_PROCESS_ERROR(pTeam->MemberGroup[nGroupIndex].MemberList.size() < MAX_PLAYER_PER_GROUP);

    memset(&MemberInfo, 0, sizeof(MemberInfo));

    MemberInfo.dwMemberID   = dwPlayerID;
    MemberInfo.bOnlineFlag  = true;
    MemberInfo.nLifePercent = UCHAR_MAX;
    MemberInfo.nManaPercent = UCHAR_MAX;
    MemberInfo.nRagePercent = UCHAR_MAX;

    pTeam->MemberGroup[nGroupIndex].MemberList.push_back(MemberInfo);
	pTeam->bSystem = bSystem;

    pPlayer = g_cPlayerMgr.GetById(dwPlayerID);
    if (pPlayer)
    {
		pPlayer->SetPlayerTeamId(pTeam->bSystem, dwTeamID);
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamServer::DelMember(DWORD dwTeamID, int nGroupIndex, DWORD dwPlayerID)
{
    BOOL     bResult = false;
    KPlayer* pPlayer = NULL;
    QTeam*   pTeam   = NULL;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    LOG_PROCESS_ERROR(nGroupIndex < pTeam->nGroupNum);

    for (
        QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[nGroupIndex].MemberList.begin();
        it != pTeam->MemberGroup[nGroupIndex].MemberList.end(); ++it
    )
    {
        if (it->dwMemberID == dwPlayerID)
        {
            pTeam->MemberGroup[nGroupIndex].MemberList.erase(it);

            pPlayer = g_cPlayerMgr.GetById(dwPlayerID);
            if (pPlayer)
            {
				pPlayer->SetPlayerTeamId(pTeam->bSystem, KD_BAD_ID);
            }

            bResult = true;
            goto EXIT0;
        }
    }
EXIT0:
    return bResult;
}

BOOL QTeamServer::ChangeMember(DWORD dwTeamID, int nSrcGroupIndex, int nDstGroupIndex, DWORD dwSrcPlayerID, DWORD dwDstPlayerID)
{
    BOOL                bResult         = false;
    KPlayer*            pPlayer         = NULL;
    QTeam*              pTeam           = NULL;
    QTEAM_MEMBER_INFO   TmpMemberInfo;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    LOG_PROCESS_ERROR(nSrcGroupIndex < pTeam->nGroupNum);
    LOG_PROCESS_ERROR(nDstGroupIndex < pTeam->nGroupNum);

    memset(&TmpMemberInfo, 0, sizeof(TmpMemberInfo));
    TmpMemberInfo.dwMemberID = KD_BAD_ID;

    for (
        QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[nDstGroupIndex].MemberList.begin();
        it != pTeam->MemberGroup[nDstGroupIndex].MemberList.end(); ++it
    )
    {
        if (it->dwMemberID == dwDstPlayerID)
        {
            TmpMemberInfo = *it;

            pTeam->MemberGroup[nDstGroupIndex].MemberList.erase(it);
            break;
        }
    }

    LOG_PROCESS_ERROR(pTeam->MemberGroup[nDstGroupIndex].MemberList.size() < MAX_PLAYER_PER_GROUP);

    for (
        QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[nSrcGroupIndex].MemberList.begin();
        it != pTeam->MemberGroup[nSrcGroupIndex].MemberList.end(); ++it
    )
    {
        if (it->dwMemberID != dwSrcPlayerID)
            continue;
        
        pTeam->MemberGroup[nDstGroupIndex].MemberList.push_back(*it);
        pTeam->MemberGroup[nSrcGroupIndex].MemberList.erase(it);

        if (TmpMemberInfo.dwMemberID != KD_BAD_ID)
            pTeam->MemberGroup[nSrcGroupIndex].MemberList.push_back(TmpMemberInfo);

        bResult = true;
        goto EXIT0;
    }

EXIT0:
    return bResult;
}

BOOL QTeamServer::IsPlayerInTeam(DWORD dwTeamID, DWORD dwPlayerID)
{
    BOOL                bResult = false;
    QTEAM_MEMBER_INFO*  pMember = NULL;

    pMember = GetMember(dwTeamID, dwPlayerID);
    PROCESS_ERROR(pMember);

    bResult = true;
EXIT0:
    return bResult;
}

QTEAM_MEMBER_INFO* QTeamServer::GetMember(DWORD dwTeamID, DWORD dwPlayerID)
{
    QTEAM_MEMBER_INFO*  pResult = NULL;
    QTeam*              pTeam   = NULL;

    pTeam = GetTeam(dwTeamID);
    PROCESS_ERROR(pTeam);

    for (int i = 0; i < pTeam->nGroupNum; i++)
    {
        for (
            QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
            it != pTeam->MemberGroup[i].MemberList.end(); ++it
        )
        {
            if (it->dwMemberID == dwPlayerID)
            {
                pResult = &(*it);
                goto EXIT0;
            }
        }
    }

EXIT0:
    return pResult;
}

BOOL QTeamServer::SetAuthority(DWORD dwTeamID, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID)
{
    BOOL    bResult = false;
    QTeam*  pTeam   = NULL;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    LOG_PROCESS_ERROR(eType > tatInvalid && eType < tatTotal);

    pTeam->dwAuthority[eType] = dwTargetID;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL QTeamServer::TeamMessageNotifyToAll(DWORD dwTeamID, INT nMsgId, PVOID pData, UINT uDataLen)
{
	KPlayer* pMember = NULL;
	QTeam* pTeam = g_cTeamServer.GetTeam(dwTeamID);
	QCONFIRM_RET_FALSE(pTeam);

	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		for (
			QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
			it != pTeam->MemberGroup[i].MemberList.end(); ++it)
		{
			pMember = g_cPlayerMgr.GetById(it->dwMemberID);
			if(!pMember) 
				continue;
			g_cPlayerBaseProcess.DoMessageNotify(pMember, ectTeamNotifyCode, nMsgId, pData, uDataLen);
		}
	}
	return TRUE;
}

BOOL QTeamServer::SendTeamInfoToAll(DWORD dwTeamID)
{
	KPlayer* pMember = NULL;
	QTeam* pTeam = g_cTeamServer.GetTeam(dwTeamID);
	QCONFIRM_RET_FALSE(pTeam);

	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		for (
			QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
			it != pTeam->MemberGroup[i].MemberList.end(); ++it)
		{
			pMember = g_cPlayerMgr.GetById(it->dwMemberID);
			if(!pMember) 
				continue;
			QPlayerTeam::DoSyncTeamBaseInfo(dwTeamID, pMember, TRUE);
		}
	}

	return TRUE;
}

BOOL QTeamServer::SetFormationLeader(DWORD dwTeamID, int nGroupIndex, DWORD dwTargetID)
{
    BOOL    bResult = false;
    QTeam*  pTeam   = NULL;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);
    
    LOG_PROCESS_ERROR(nGroupIndex < pTeam->nGroupNum);

    for (
        QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[nGroupIndex].MemberList.begin();
        it != pTeam->MemberGroup[nGroupIndex].MemberList.end(); ++it
    )
    {
        if (it->dwMemberID == dwTargetID)
        {
            pTeam->MemberGroup[nGroupIndex].dwFormationLeader = dwTargetID;

            bResult = true;
            goto EXIT0;
        }
    }

EXIT0:
    return bResult;
}

BOOL QTeamServer::SetMark(DWORD dwTeamID, int nMarkType, DWORD dwTargetID)
{
    BOOL    bResult = false;
    QTeam*  pTeam   = NULL;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    for (int i = 0; i < TEAM_MARK_COUNT; i++)
    {
        if (pTeam->dwMark[i] == dwTargetID)
        {
            pTeam->dwMark[i] = 0;
        }
    }

    PROCESS_SUCCESS(nMarkType == 0);
    LOG_PROCESS_ERROR(nMarkType > 0 && nMarkType <= TEAM_MARK_COUNT);
    pTeam->dwMark[nMarkType - 1] = dwTargetID;

EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

int QTeamServer::GetMemberGroupIndex(DWORD dwTeamID, DWORD dwMemberID)
{
    int     nResult = KD_BAD_ID;
    QTeam*  pTeam   = NULL;

    pTeam = GetTeam(dwTeamID);
    LOG_PROCESS_ERROR(pTeam);

    for (int i = 0; i < pTeam->nGroupNum; i++)
    {
        for (
            QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
            it != pTeam->MemberGroup[i].MemberList.end(); ++it
        )
        {
            if (it->dwMemberID == dwMemberID)
            {
                nResult = i;

                goto EXIT0;
            }
        }
    }

EXIT0:
    return nResult;
}

BOOL QGetAllTeamMember::operator ()(QTEAM_MEMBER_INFO* pMemberInfo)
{
    BOOL bResult = false;

    m_vecResult.push_back(pMemberInfo->dwMemberID);

    bResult = true;
EXIT0:
    return bResult;
}
#endif
