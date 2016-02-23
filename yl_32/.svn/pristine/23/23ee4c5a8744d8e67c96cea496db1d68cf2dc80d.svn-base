

#include "stdafx.h"
#include "teammodule_gc.h"
#include "onlinegamemodule/kteamprotocol.h"
#include "GameCenter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


QCenterTeamModule g_cGcTeamModule;

// -------------------------------------------------------------------------

BOOL QCenterTeamModule::DoSyncTeamInfo(int nConnectionIndex, DWORD dwTeamID, KTeam* pTeam)
{
	BOOL                          bResult               = FALSE;
	BOOL                          bRetCode              = FALSE;
	IMemBlock*                   piTeamCreateNotify   = NULL;
	R2S_TEAM_CREATE_NOTIFY*       pTeamCreateNotify    = NULL;
	int                           nIndex                = 0;
	int                           nMemberCount          = 0;
	size_t                        uPakSize              = 0;

	assert(pTeam);

	nMemberCount = g_pGameCenter->m_TeamCenter.GetTeamMemberCount(dwTeamID);
	uPakSize = sizeof(R2S_TEAM_CREATE_NOTIFY) + (nMemberCount * sizeof(R2S_TEAM_CREATE_NOTIFY::KMEMBER));

	piTeamCreateNotify = QCreateMemBlock((unsigned)uPakSize);
	LOG_PROCESS_ERROR(piTeamCreateNotify);

	pTeamCreateNotify = (R2S_TEAM_CREATE_NOTIFY*)piTeamCreateNotify->GetData();

	pTeamCreateNotify->wProtocolID         = r2s_team_create_notify;
	pTeamCreateNotify->dwTeamID           = dwTeamID;
	pTeamCreateNotify->bSystem             = pTeam->bSystem;
	//pTeamCreateNotify->byCamp              = (BYTE)pTeam->eCamp;
	pTeamCreateNotify->nGroupNum           = pTeam->nGroupNum;
	pTeamCreateNotify->nLootMode           = pTeam->nLootMode;
	pTeamCreateNotify->nRollQuality        = pTeam->nRollQuality;
	pTeamCreateNotify->byMemberCount       = (BYTE)nMemberCount;

	memcpy(pTeamCreateNotify->dwAuthority, pTeam->dwAuthority, sizeof(pTeamCreateNotify->dwAuthority));
	memcpy(pTeamCreateNotify->dwTeamMark, pTeam->dwTeamMark, sizeof(pTeamCreateNotify->dwTeamMark));

	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		for (
			KTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
			it != pTeam->MemberGroup[i].MemberList.end(); ++it
			)
		{
			KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(it->dwMemberID);
			size_t uSize = 0;

			assert(nIndex < nMemberCount);

			if (!pRole)
				continue;

			pTeamCreateNotify->Member[nIndex].dwMemberID   = it->dwMemberID;
			pTeamCreateNotify->Member[nIndex].nGroupIndex  = i;
			pTeamCreateNotify->Member[nIndex].byOnLineFlag = (BYTE)it->bIsOnline;
			pTeamCreateNotify->Member[nIndex].byLevel      = (BYTE)pRole->GetLevel();

			uSize = sizeof(pTeamCreateNotify->Member[nIndex].szMemberName);

			strncpy(pTeamCreateNotify->Member[nIndex].szMemberName, pRole->GetPlayerName(), uSize);
			pTeamCreateNotify->Member[nIndex].szMemberName[uSize - 1] = '\0';

			nIndex++;
		}
	}

	LOG_PROCESS_ERROR(nMemberCount == nIndex);

	bRetCode = g_pGameCenter->m_GameServer.Send(nConnectionIndex, piTeamCreateNotify);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piTeamCreateNotify);
	return bResult;
}

BOOL QCenterTeamModule::DoApplyJoinTeamRequest(KRole* pApplyDst, KRole* pApplySrc)
{
	BOOL                            bResult         = false;
	int                             nRetCode        = false;
	BOOL                            bIsNotFull      = false;
	KTeam*                          pTeam           = NULL;
	KRole*                          pTeamLeader     = NULL;
	IMemBlock*                     piSendBuffer    = NULL;
	R2S_APPLY_JOIN_TEAM_REQUEST*    pApplyToDst     = NULL;

	assert(pApplyDst);
	assert(pApplySrc);

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pApplyDst->GetTeamId());
	LOG_PROCESS_ERROR(pTeam);

	// 得到该队伍的队长
	pTeamLeader = g_pGameCenter->m_RoleManager.GetRole(pTeam->dwAuthority[tatLeader]);
	LOG_PROCESS_ERROR(pTeamLeader);

	// 检查申请折的阵营是否和队伍阵营一致
	// if (pTeam->eCamp != cNeutral && pApplySrc->m_eCamp && pTeam->eCamp != pApplySrc->m_eCamp)
	// {
	//     DoTeamMessageNotify(
	//         pApplySrc->GetOnlineServer(), pApplySrc->GetId(), 
	//         epncCampError, pTeamLeader->GetPlayerName()
	//     );
	//     goto EXIT0;
	// }

	// 检查队伍是否满员
	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		if (pTeam->MemberGroup[i].MemberList.size() < MAX_PLAYER_PER_GROUP)
		{
			bIsNotFull = true;
			break;
		}
	}

	if (!bIsNotFull)
	{
		DoTeamMessageNotify(
			pApplySrc->m_nConnIndex, pApplySrc->GetId(), 
			epncDestTeamIsFull, pApplyDst->GetPlayerName()
			);
		goto EXIT0;
	}

	// 队长必须在线
	LOG_PROCESS_ERROR(pTeamLeader->IsOnline());

	g_pGameCenter->m_TeamCenter.RegisterApply(pApplySrc->GetId(), pTeamLeader->GetId());

	// 转发入队申请到目标玩家
	piSendBuffer = QCreateMemBlock(sizeof(R2S_APPLY_JOIN_TEAM_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pApplyToDst = (R2S_APPLY_JOIN_TEAM_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pApplyToDst);

	pApplyToDst->wProtocolID        = r2s_apply_join_team_request;
	pApplyToDst->dwApplyDst         = pTeamLeader->GetId();
	pApplyToDst->dwApplySrc         = pApplySrc->GetId();
	pApplyToDst->bySrcLevel         = (BYTE)pApplySrc->GetLevel();
	pApplyToDst->bySrcForceID       = (BYTE)pApplySrc->m_dwForceID;
	//pApplyToDst->bySrcCamp          = (BYTE)pApplySrc->m_eCamp;
	pApplyToDst->dwSrcTongID        = pApplySrc->m_dwTongID;
	pApplyToDst->dwMapID            = pApplySrc->m_dwMapID;

	strncpy(pApplyToDst->szApplySrc, pApplySrc->GetPlayerName(), sizeof(pApplyToDst->szApplySrc));
	pApplyToDst->szApplySrc[sizeof(pApplyToDst->szApplySrc) - 1] = '\0';

	nRetCode = g_pGameCenter->m_GameServer.Send(pTeamLeader->m_nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamCreateNotify(DWORD dwTeamID)
{
	BOOL                    bResult         = FALSE;
	BOOL                    bRetCode        = FALSE;
	KRole*                  pLeader         = NULL;
	R2S_TEAM_CREATE_NOTIFY* pNotify         = NULL;
	IMemBlock*             piSendBuffer    = NULL;
	KTeam*                  pTeam           = NULL;

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	piSendBuffer = QCreateMemBlock(sizeof(R2S_TEAM_CREATE_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (R2S_TEAM_CREATE_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID    = r2s_team_create_notify;
	pNotify->dwTeamID       = dwTeamID;
	pNotify->bSystem        = pTeam->bSystem;
	//pNotify->byCamp         = (BYTE)pTeam->eCamp;
	pNotify->nGroupNum      = pTeam->nGroupNum;
	pNotify->nLootMode      = pTeam->nLootMode;
	pNotify->nRollQuality   = pTeam->nRollQuality;
	pNotify->byMemberCount  = 0;

	memcpy(pNotify->dwAuthority, pTeam->dwAuthority, sizeof(pNotify->dwAuthority));
	memcpy(pNotify->dwTeamMark, pTeam->dwTeamMark, sizeof(pNotify->dwTeamMark));

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamChangeAuthorityNotify(DWORD dwTeamID, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID)
{
	BOOL                                bResult                     = FALSE;
	BOOL                                bRetCode                    = FALSE;
	IMemBlock*                         piSendBuffer                = NULL;
	R2S_TEAM_CHANGE_AUTHORITY_NOTIFY*   pTeamChangeLeaderNotify    = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(R2S_TEAM_CHANGE_AUTHORITY_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pTeamChangeLeaderNotify = (R2S_TEAM_CHANGE_AUTHORITY_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pTeamChangeLeaderNotify);

	pTeamChangeLeaderNotify->wProtocolID   = r2s_team_change_authority_notify;
	pTeamChangeLeaderNotify->dwTeamID     = dwTeamID;
	pTeamChangeLeaderNotify->byType        = (BYTE)eType;
	pTeamChangeLeaderNotify->dwNewID       = dwTargetID;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamSetLootModeNotify(DWORD dwTeamID, int nLootMode, int nRollQuality)
{
	BOOL                            bResult         = FALSE;
	BOOL                            bRetCode        = FALSE;
	IMemBlock*                     piSendBuffer    = NULL;
	R2S_TEAM_SET_LOOT_MODE_NOTIFY*  pNotify         = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(R2S_TEAM_SET_LOOT_MODE_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (R2S_TEAM_SET_LOOT_MODE_NOTIFY *)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID      = r2s_team_set_loot_mode_notify;
	pNotify->dwTeamID        = dwTeamID;
	pNotify->nLootMode		  = nLootMode;
	pNotify->nRollQuality	  = nRollQuality;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamAddMemberNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwMemberID, BOOL bSystem /* = 0 */)
{
	BOOL                        bResult             = false;
	IMemBlock*                 piSendBuffer        = NULL;
	R2S_TEAM_ADD_MEMBER_NOTIFY* pAddMemberNotify    = NULL;
	KRole*                      pRole               = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_TEAM_ADD_MEMBER_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pAddMemberNotify = (R2S_TEAM_ADD_MEMBER_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pAddMemberNotify);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwMemberID);
	LOG_PROCESS_ERROR(pRole);

	pAddMemberNotify->wProtocolID   = r2s_team_add_member_notify;
	pAddMemberNotify->dwTeamID     = dwTeamID;
	pAddMemberNotify->nGroupIndex   = nGroupIndex;
	pAddMemberNotify->dwMemberID    = dwMemberID;
	pAddMemberNotify->bySystem = (BYTE)bSystem;

	strncpy(pAddMemberNotify->szMemberName, pRole->GetPlayerName(), sizeof(pAddMemberNotify->szMemberName));
	pAddMemberNotify->szMemberName[sizeof(pAddMemberNotify->szMemberName) - 1] = '\0';

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamDelMemberNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwMemberID, CONST CHAR* szMemberName)
{
	BOOL                            bResult             = FALSE;
	BOOL                            bRetCode            = FALSE;
	IMemBlock*                     piSendBuffer        = NULL;
	R2S_TEAM_DEL_MEMBER_NOTIFY*     pDelMemberNotify    = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(R2S_TEAM_DEL_MEMBER_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pDelMemberNotify = (R2S_TEAM_DEL_MEMBER_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pDelMemberNotify);

	pDelMemberNotify->wProtocolID       = r2s_team_del_member_notify;
	pDelMemberNotify->dwTeamID         = dwTeamID;
	pDelMemberNotify->nGroupIndex       = nGroupIndex;
	pDelMemberNotify->dwMemberID        = dwMemberID;
	strncpy(pDelMemberNotify->szMemberName, szMemberName, sizeof(pDelMemberNotify->szMemberName));

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamDisbandNotify(DWORD dwTeamID)
{
	BOOL                            bResult             = FALSE;
	BOOL                            bRetCode            = FALSE;
	IMemBlock*                     piSendBuffer        = NULL;
	R2S_TEAM_DISBAND_NOTIFY*        pTeamDisbandNotify = NULL;

	// 队伍解散通知
	piSendBuffer = QCreateMemBlock(sizeof(R2S_TEAM_DISBAND_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pTeamDisbandNotify = (R2S_TEAM_DISBAND_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pTeamDisbandNotify);

	pTeamDisbandNotify->wProtocolID       = r2s_team_disband_notify;
	pTeamDisbandNotify->dwTeamID         = dwTeamID;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoSyncTeamMemberOnlineFlag(DWORD dwTeamID, DWORD dwMemberID, BOOL bOnlineFlag)
{
	BOOL                                bResult             = false;
	BOOL                                bRetCode            = false;
	IMemBlock*                         piSendBuffer        = NULL;
	R2S_SYNC_TEAM_MEMBER_ONLINE_FLAG*   pSyncOnlineInfo     = NULL;

	// 同步队员在线状态
	piSendBuffer = QCreateMemBlock(sizeof(R2S_SYNC_TEAM_MEMBER_ONLINE_FLAG));
	LOG_PROCESS_ERROR(piSendBuffer);

	pSyncOnlineInfo = (R2S_SYNC_TEAM_MEMBER_ONLINE_FLAG*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pSyncOnlineInfo);

	pSyncOnlineInfo->wProtocolID       = r2s_sync_team_member_online_flag;
	pSyncOnlineInfo->dwTeamID         = dwTeamID;
	pSyncOnlineInfo->dwMemberID        = dwMemberID;
	pSyncOnlineInfo->bOnlineFlag       = bOnlineFlag;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamMessageNotify(int nConnIndex, DWORD dwPlayerID, int nCode, const char* pszText)
{
	BOOL                                bResult             = false;
	BOOL                                bRetCode            = false;
	size_t                              uTextLen            = 0;
	IMemBlock*                         piSendBuffer        = NULL;
	R2S_PARTY_MESSAGE_NOTIFY*           pMsg                = NULL;

	if (pszText)
	{
		uTextLen = strlen(pszText) + 1;
	}

	// 同步队员在线状态
	piSendBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_PARTY_MESSAGE_NOTIFY) + uTextLen));
	LOG_PROCESS_ERROR(piSendBuffer);

	pMsg = (R2S_PARTY_MESSAGE_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pMsg);

	pMsg->wProtocolID = r2s_party_message_notify;
	pMsg->dwPlayerID  = dwPlayerID;
	pMsg->nCode       = nCode;

	if (pszText)
	{
		memcpy(pMsg->byData, pszText, uTextLen + 1);
	}

	bRetCode = g_pGameCenter->m_GameServer.Send(nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamSetFormationLeaderNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwNewFormationLeader)
{
	/*
	BOOL                                    bResult             = false;
	BOOL                                    bRetCode            = false;
	IMemBlock*                             piSendBuffer        = NULL;
	R2S_TEAM_SET_FORMATION_LEADER_NOTIFY*   pNotify             = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_TEAM_SET_FORMATION_LEADER_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (R2S_TEAM_SET_FORMATION_LEADER_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID            = r2s_team_set_formation_leader_notify;
	pNotify->dwTeamID              = dwTeamID;
	pNotify->nGroupIndex            = nGroupIndex;
	pNotify->dwNewFormationLeader   = dwNewFormationLeader;

	Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
	*/
	return TRUE;
}

BOOL QCenterTeamModule::DoTeamSetMarkRespond(DWORD dwTeamID, int nMarkType, DWORD dwTargetID)
{
	BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	IMemBlock*                 piSendBuffer    = NULL;
	R2S_TEAM_SET_MARK_RESPOND*  pNotify         = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(R2S_TEAM_SET_MARK_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (R2S_TEAM_SET_MARK_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID    = r2s_team_set_mark_respond;
	pNotify->dwTeamID      = dwTeamID;
	pNotify->nMarkType      = nMarkType;
	pNotify->dwTarget       = dwTargetID;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamLevelUpRaidNotify(DWORD dwTeamID, int nGroupNum)
{
	BOOL                            bResult         = false;
	IMemBlock*                     piSendBuffer    = NULL;
	R2S_TEAM_LEVEL_UP_RAID_NOTIFY*  pNotify         = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_TEAM_LEVEL_UP_RAID_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (R2S_TEAM_LEVEL_UP_RAID_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID = r2s_team_level_up_raid_notify;
	pNotify->dwTeamID    = dwTeamID;
	pNotify->nGroupNum   = nGroupNum;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QCenterTeamModule::DoTeamChangeMemberGroupNotify(
	DWORD dwTeamID, DWORD dwMemberID, int nOldGroupIndex, DWORD dwDstMemberID, int nNewGroupIndex
	)
{
	BOOL                                    bResult      = false;
	IMemBlock*                             piSendBuffer = NULL;
	R2S_TEAM_CHANGE_MEMBER_GROUP_NOTIFY*    pNotify      = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_TEAM_CHANGE_MEMBER_GROUP_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (R2S_TEAM_CHANGE_MEMBER_GROUP_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID    = r2s_team_change_member_group_notify;
	pNotify->dwTeamID       = dwTeamID;
	pNotify->dwMemberID     = dwMemberID;
	pNotify->nOldGroupIndex = nOldGroupIndex;
	pNotify->dwDstMemberID  = dwDstMemberID;
	pNotify->nNewGroupIndex = nNewGroupIndex;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

void QCenterTeamModule::OnInvitePlayerJoinTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                  bRetCode               = false;
	S2R_INVITE_PLAYER_JOIN_TEAM_REQUEST*  pInvitePlayerJoinTeam  = (S2R_INVITE_PLAYER_JOIN_TEAM_REQUEST*)pbyData;
	KRole*                                pInviteSrc             = NULL;
	KRole*                                pInviteDst             = NULL;
	IMemBlock*                           piSendBuffer           = NULL;
	R2S_INVITE_PLAYER_JOIN_TEAM_REQUEST*  pTeamInvitation       = NULL;

	pInvitePlayerJoinTeam->szInviteRole[sizeof(pInvitePlayerJoinTeam->szInviteRole) - 1] = '\0';

	pInviteSrc = g_pGameCenter->m_RoleManager.GetRole(pInvitePlayerJoinTeam->dwInviteSrc);
	LOG_PROCESS_ERROR(pInviteSrc);

	pInviteDst = g_pGameCenter->m_RoleManager.GetRole(pInvitePlayerJoinTeam->szInviteRole);
	if (!pInviteDst) 
	{
		DoTeamMessageNotify(
			nConnIndex, pInvitePlayerJoinTeam->dwInviteSrc, 
			epncPlayerInviteNotExist, pInvitePlayerJoinTeam->szInviteRole
			);
		goto EXIT0;
	}

	// 不能自己邀请自己组队
	LOG_PROCESS_ERROR(pInviteSrc != pInviteDst);

	if (pInviteSrc->GetTeamId() != KD_BAD_ID)
	{
		// 邀请发起者已经处于组队状态
		// 1. 检查发起者是不是队长; 2. 检查队伍是否满员
		BOOL   bIsFulled    = true;

		KTeam* pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pInviteSrc->GetTeamId());
		LOG_PROCESS_ERROR(pTeam);

		// 必须是队长
		PROCESS_ERROR(pInviteSrc->GetId() == pTeam->dwAuthority[tatLeader]);
		// 满员?
		for (int i = 0; i < pTeam->nGroupNum; i++)
		{
			if (pTeam->MemberGroup[i].MemberList.size() < MAX_PLAYER_PER_GROUP)
			{
				bIsFulled = false;
				break;
			}
		}

		if (bIsFulled)
		{
			DoTeamMessageNotify(
				nConnIndex, pInvitePlayerJoinTeam->dwInviteSrc, 
				epncYourTeamIsFull, pInvitePlayerJoinTeam->szInviteRole
				);
			goto EXIT0;
		}

		// 阵营不符合
		// if (pTeam->eCamp != cNeutral && pInviteDst->m_eCamp && pTeam->eCamp != pInviteDst->m_eCamp)
		// {
		//     DoTeamMessageNotify(
		//         nConnIndex, pInvitePlayerJoinTeam->dwInviteSrc, 
		//         epncCampError, pInvitePlayerJoinTeam->szInviteRole
		//     );
		//     goto EXIT0;
		// }
	}

	// 如果src没有组队,dest在队伍里面,那么src会申请加入dest的队伍
	if (pInviteSrc->GetTeamId() == KD_BAD_ID && pInviteDst->GetTeamId() != KD_BAD_ID)
	{
		DoApplyJoinTeamRequest(pInviteDst, pInviteSrc);
		goto EXIT0;
	}

	if (pInviteDst->GetTeamId() != KD_BAD_ID) 
	{
		if (pInviteDst->GetTeamId() == pInviteSrc->GetTeamId())
		{
			DoTeamMessageNotify(
				nConnIndex, pInvitePlayerJoinTeam->dwInviteSrc, 
				epncPlayerAlreadInYourTeam, pInviteDst->GetPlayerName()
				);
		}
		else
		{
			DoTeamMessageNotify(
				nConnIndex, pInvitePlayerJoinTeam->dwInviteSrc, 
				epncPlayerAlreadInOtherTeam, pInviteDst->GetPlayerName()
				);
		}
		goto EXIT0;
	}

	//if (pInviteDst->GetRoleState() != rsOnline) 
	if (!pInviteDst->IsOnline()) 
	{
		DoTeamMessageNotify(
			nConnIndex, pInvitePlayerJoinTeam->dwInviteSrc, 
			epncPlayerNotOnline, pInviteDst->GetPlayerName()
			);
		goto EXIT0;
	}

	bRetCode = g_pGameCenter->m_TeamCenter.IsInviteExist(pInviteSrc->GetId(), pInviteDst->GetId());
	if (bRetCode)
	{
		DoTeamMessageNotify(
			nConnIndex, pInvitePlayerJoinTeam->dwInviteSrc, 
			epncPlayerIsBusy, pInviteDst->GetPlayerName()
			);
		goto EXIT0;
	}

	// if (pInviteSrc->m_eCamp != cNeutral && pInviteDst->m_eCamp != cNeutral && pInviteSrc->m_eCamp != pInviteDst->m_eCamp)
	// {
	//     DoTeamMessageNotify(
	//         nConnIndex, pInvitePlayerJoinTeam->dwInviteSrc, 
	//         epncCampError, pInvitePlayerJoinTeam->szInviteRole
	//     );
	//     goto EXIT0;
	// }

	g_pGameCenter->m_TeamCenter.RegisterInvite(
		pInvitePlayerJoinTeam->dwInviteSrc, pInviteDst->GetId()
		);

	// 将邀请发给被邀请玩家所在的服务器
	piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_INVITE_PLAYER_JOIN_TEAM_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pTeamInvitation = (R2S_INVITE_PLAYER_JOIN_TEAM_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pTeamInvitation);

	pTeamInvitation->wProtocolID      = r2s_invite_player_join_team_request;
	pTeamInvitation->dwInviteDst      = pInviteDst->GetId();
	pTeamInvitation->dwInviteSrc      = pInviteSrc->GetId();
	//pTeamInvitation->bySrcCamp        = (BYTE)pInviteSrc->m_eCamp;

	strncpy(pTeamInvitation->szInviteSrc, pInviteSrc->GetPlayerName(), sizeof(pTeamInvitation->szInviteSrc));
	pTeamInvitation->szInviteSrc[sizeof(pTeamInvitation->szInviteSrc) - 1] = '\0';

	bRetCode = g_pGameCenter->m_GameServer.Send(pInviteDst->m_nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piSendBuffer);
}

void QCenterTeamModule::OnCreateTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                bRetCode	        = FALSE;
	KRole*								pCreateSrc			= NULL;
	S2R_CREATE_TEAM_REQUEST*			pSync				= (S2R_CREATE_TEAM_REQUEST*)pbyData;
	IMemBlock*                          piSendBuffer		= NULL;
	KTeam*                              pTeam				= NULL;
	DWORD                               dwTeamID			= KD_BAD_ID;

	QLogPrintf(LOG_INFO, "QCenterTeamModule::OnCreateTeamRequest dwCreateSrc=%d bySystem=%d\n", pSync->dwCreateSrc, pSync->bySystem);

	pCreateSrc = g_pGameCenter->m_RoleManager.GetRole(pSync->dwCreateSrc);
	LOG_PROCESS_ERROR(pCreateSrc);

	// 已经有队伍
	if (pCreateSrc->GetTeamId() != KD_BAD_ID)
	{
		DoTeamMessageNotify(
			nConnIndex, pCreateSrc->GetId(), 
			epncPlayerAlreadInYourTeam, pCreateSrc->GetPlayerName()
			);
		goto EXIT0;
	}

	// 创建队伍:
	dwTeamID = g_pGameCenter->m_TeamCenter.CreateTeam(false);
	LOG_PROCESS_ERROR(dwTeamID != KD_BAD_ID);

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	// 设置队长标记
	g_pGameCenter->m_TeamCenter.SetAuthority(dwTeamID, tatLeader, pCreateSrc->GetId());

	bRetCode = g_pGameCenter->m_TeamCenter.AddMember(dwTeamID, pCreateSrc->GetId(), pSync->bySystem);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QCenterTeamModule::OnInvitePlayerJoinTeamRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                  bRetCode          = FALSE;
	S2R_INVITE_PLAYER_JOIN_TEAM_RESPOND*  pInviteRespond    = (S2R_INVITE_PLAYER_JOIN_TEAM_RESPOND*)pbyData;
	KRole*                                pInviteSrc        = NULL;
	KRole*                                pInviteDst        = NULL;
	KTeam*                                pTeam             = NULL;
	DWORD                                 dwTeamID          = KD_BAD_ID;

	pInviteRespond->szInviteSrc[sizeof(pInviteRespond->szInviteSrc) - 1] = '\0';

	pInviteSrc = g_pGameCenter->m_RoleManager.GetRole(pInviteRespond->szInviteSrc);
	LOG_PROCESS_ERROR(pInviteSrc);

	pInviteDst = g_pGameCenter->m_RoleManager.GetRole(pInviteRespond->dwInviteDst);
	LOG_PROCESS_ERROR(pInviteDst);

	// 核对是否真的发起过邀请，或者邀请是否仍然有效
	bRetCode = g_pGameCenter->m_TeamCenter.IsInviteExist(
		pInviteSrc->GetId(), pInviteDst->GetId()
		);
	PROCESS_ERROR(bRetCode);

	// 被邀请者可能已加入其他队伍
	PROCESS_ERROR(pInviteDst->GetTeamId() == KD_BAD_ID);

	// 检查邀请发起者是否仍然在线
	if (!pInviteSrc->IsOnline())
	{
		DoTeamMessageNotify(
			nConnIndex, pInviteDst->GetId(), 
			epncPlayerNotOnline, pInviteSrc->GetPlayerName()
			);
		goto EXIT0;
	}

	if (!pInviteRespond->bAgreeFlag) 
	{
		DoTeamMessageNotify(
			pInviteSrc->m_nConnIndex, pInviteSrc->GetId(), 
			epncInvitationDenied, pInviteDst->GetPlayerName()
			);
		goto EXIT0;
	}

	// 检查邀请发起者的队伍状态: 要么不在任何队伍中，要么是队长
	if (pInviteSrc->GetTeamId() == KD_BAD_ID)
	{
		// 创建队伍:
		dwTeamID = g_pGameCenter->m_TeamCenter.CreateTeam(false);
		LOG_PROCESS_ERROR(dwTeamID != KD_BAD_ID);

		pTeam = g_pGameCenter->m_TeamCenter.GetTeam(dwTeamID);
		LOG_PROCESS_ERROR(pTeam);

		// 设置队长标记
		g_pGameCenter->m_TeamCenter.SetAuthority(dwTeamID, tatLeader, pInviteSrc->GetId());

		bRetCode = g_pGameCenter->m_TeamCenter.AddMember(dwTeamID, pInviteSrc->GetId());
		LOG_PROCESS_ERROR(bRetCode);
	}
	else
	{
		BOOL   bIsNotFull = false;

		pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pInviteSrc->GetTeamId());
		LOG_PROCESS_ERROR(pTeam);

		dwTeamID = pInviteSrc->GetTeamId();

		// 确认他还是队长
		if (pTeam->dwAuthority[tatLeader] != pInviteSrc->GetId())
		{
			DoTeamMessageNotify(
				nConnIndex, pInviteDst->GetId(),
				epncInvitationOutOfDate, pInviteSrc->GetPlayerName()
				);
			goto EXIT0;
		}

		// 确认是否满员
		for (int i = 0; i < pTeam->nGroupNum; i++)
		{
			if (pTeam->MemberGroup[i].MemberList.size() < MAX_PLAYER_PER_GROUP)
			{
				bIsNotFull = true;
				break;
			}
		}

		if (!bIsNotFull)
		{
			DoTeamMessageNotify(
				nConnIndex, pInviteDst->GetId(), 
				epncDestTeamIsFull, pInviteSrc->GetPlayerName()
				);
			goto EXIT0;
		}
	}

	// if (pTeam->eCamp != cNeutral && pInviteDst->m_eCamp != cNeutral && pTeam->eCamp != pInviteDst->m_eCamp)
	// {
	//     DoTeamMessageNotify(
	//         nConnIndex, pInviteDst->GetId(), 
	//         epncCampError, pInviteSrc->GetPlayerName()
	//     );
	//     goto EXIT0;
	// }

	// 队伍添加成员
	bRetCode = g_pGameCenter->m_TeamCenter.AddMember(dwTeamID, pInviteRespond->dwInviteDst);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	// no matter player agreed or denied, no matter respond to which invatation, clean the "being in invited" flag !
	if (pInviteSrc && pInviteDst)
	{
		g_pGameCenter->m_TeamCenter.UnregisterInvite(pInviteSrc->GetId(), pInviteDst->GetId());
	}
}

void QCenterTeamModule::OnApplyJoinTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	int                             nRetCode                = false;
	S2R_APPLY_JOIN_TEAM_REQUEST*    pApplyJoinTeamRequest   = (S2R_APPLY_JOIN_TEAM_REQUEST*)pbyData;
	KRole*                          pApplySrc               = NULL;
	KRole*                          pApplyDst               = NULL;
	R2S_APPLY_JOIN_TEAM_REQUEST*    pApplyToDst             = NULL;

	pApplyJoinTeamRequest->szApplyDst[sizeof(pApplyJoinTeamRequest->szApplyDst) - 1] = '\0';

	pApplySrc = g_pGameCenter->m_RoleManager.GetRole(pApplyJoinTeamRequest->dwApplySrc);
	LOG_PROCESS_ERROR(pApplySrc);

	pApplyDst = g_pGameCenter->m_RoleManager.GetRole(pApplyJoinTeamRequest->szApplyDst);
	if (!pApplyDst)
	{
		DoTeamMessageNotify(
			nConnIndex, pApplySrc->GetId(), 
			epncPlayerApplyNotExist, pApplyJoinTeamRequest->szApplyDst
			);
		goto EXIT0;
	}

	// 不能自己申请加入自己的队伍
	LOG_PROCESS_ERROR(pApplySrc != pApplyDst); 

	// 目标必须在线
	if (!pApplyDst->IsOnline())
	{
		DoTeamMessageNotify(
			nConnIndex, pApplySrc->GetId(), 
			epncPlayerNotOnline, pApplyDst->GetPlayerName()
			);
		goto EXIT0;
	}

	// 检查申请者是否已经处于组队状态
	if (pApplySrc->GetTeamId() != KD_BAD_ID)
	{
		DoTeamMessageNotify(
			nConnIndex, pApplySrc->GetId(), 
			epncYouAlreadInTeamState, pApplyDst->GetPlayerName()
			);
		goto EXIT0;
	}

	// 检查对方在队伍中
	PROCESS_ERROR(pApplyDst->GetTeamId() != KD_BAD_ID);

	nRetCode = DoApplyJoinTeamRequest(pApplyDst, pApplySrc);
	LOG_PROCESS_ERROR(nRetCode);

EXIT0:
	return;
}

void QCenterTeamModule::OnApplyJoinTeamRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	int                             nRetCode            = false;
	S2R_APPLY_JOIN_TEAM_RESPOND*    pApplyRespond       = (S2R_APPLY_JOIN_TEAM_RESPOND*)pbyData;
	BOOL                            bIsNotFull          = false;
	KRole*                          pTeamLeader        = NULL;
	KRole*                          pApplyPlayer        = NULL;
	KTeam*                          pTeam               = NULL;

	pApplyRespond->szApplySrc[sizeof(pApplyRespond->szApplySrc) - 1] = '\0';

	pTeamLeader = g_pGameCenter->m_RoleManager.GetRole(pApplyRespond->dwApplyDst);
	LOG_PROCESS_ERROR(pTeamLeader);

	pApplyPlayer = g_pGameCenter->m_RoleManager.GetRole(pApplyRespond->szApplySrc);
	LOG_PROCESS_ERROR(pApplyPlayer);

	// 核对玩家是否真的发起过申请，或者是不是已经覆盖(取消)
	nRetCode = g_pGameCenter->m_TeamCenter.IsApplyExist(pApplyPlayer->GetId(), pTeamLeader->GetId());
	if (!nRetCode)
	{
		DoTeamMessageNotify(
			nConnIndex, pTeamLeader->GetId(), 
			epncPlayerApplyNotExist, pApplyPlayer->GetPlayerName()
			);
		goto EXIT0;
	}

	if (!pApplyPlayer->IsOnline())
	{
		DoTeamMessageNotify(
			nConnIndex, pTeamLeader->GetId(), 
			epncPlayerNotOnline, pApplyPlayer->GetPlayerName()
			);
		goto EXIT0;
	}

	// 检查申请者是否已经处于组队状态
	if (pApplyPlayer->GetTeamId() != KD_BAD_ID)
	{
		DoTeamMessageNotify(
			nConnIndex, pTeamLeader->GetId(), 
			epncPlayerAlreadInOtherTeam, pApplyPlayer->GetPlayerName()
			);
		goto EXIT0;
	}

	// 检查队长是否真的是队长
	PROCESS_ERROR(pTeamLeader->GetTeamId() != KD_BAD_ID);

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pTeamLeader->GetTeamId());
	LOG_PROCESS_ERROR(pTeam);

	PROCESS_ERROR(pTeamLeader->GetId() == pTeam->dwAuthority[tatLeader]);

	if (!pApplyRespond->bAgreeFlag) 
	{
		// 告诉玩家,对方拒绝了入队申请
		DoTeamMessageNotify(
			pApplyPlayer->m_nConnIndex, pApplyPlayer->GetId(), 
			epncApplicationDenied, pTeamLeader->GetPlayerName()
			);
		goto EXIT0;
	}

	// 确认队伍是否已经满员
	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		if (pTeam->MemberGroup[i].MemberList.size() < MAX_PLAYER_PER_GROUP)
		{
			bIsNotFull = true;
			break;
		}
	}

	if (!bIsNotFull)
	{
		// 告诉申请者,目标队伍已经满员
		DoTeamMessageNotify(
			pApplyPlayer->m_nConnIndex, pApplyPlayer->GetId(), 
			epncDestTeamIsFull, pTeamLeader->GetPlayerName()
			);
		goto EXIT0;
	}

	// if (pTeam->eCamp != cNeutral && pApplyPlayer->m_eCamp != cNeutral && pTeam->eCamp != pApplyPlayer->m_eCamp)
	// {
	//     DoTeamMessageNotify(
	//         pApplyPlayer->GetOnlineServer(), pApplyPlayer->GetId(), 
	//         epncCampError, pTeamLeader->GetPlayerName()
	//     );
	//     goto EXIT0;
	// }

	// 队伍添加成员
	nRetCode = g_pGameCenter->m_TeamCenter.AddMember(pTeamLeader->GetTeamId(), pApplyPlayer->GetId());
	LOG_PROCESS_ERROR(nRetCode);

EXIT0:
	if (pApplyPlayer && pTeamLeader)
	{
		g_pGameCenter->m_TeamCenter.UnregisterApply(pApplyPlayer->GetId(), pTeamLeader->GetId());
	}
}

void QCenterTeamModule::OnTeamDelMemberRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode            = FALSE;
	S2R_TEAM_DEL_MEMBER_REQUEST*    pDelMemberRequest   = (S2R_TEAM_DEL_MEMBER_REQUEST*)pbyData;
	KRole*                          pDelRole            = NULL;
	KTeam*                          pTeam               = NULL;
	int                             nTeamCout           = 0;
	
	QLogPrintf(LOG_INFO,"..QCenterTeamModule::OnTeamDelMemberRequest pDelMemberRequest->dwTeamID=%d pDelMemberRequest->dwMemberID=%d pDelMemberRequest->dwRequestSrc=%d",
						pDelMemberRequest->dwTeamID,pDelMemberRequest->dwMemberID,pDelMemberRequest->dwRequestSrc);
	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pDelMemberRequest->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	pDelRole = g_pGameCenter->m_RoleManager.GetRole(pDelMemberRequest->dwMemberID);
	LOG_PROCESS_ERROR(pDelRole);

	if (pDelMemberRequest->dwRequestSrc != pTeam->dwAuthority[tatLeader]) // 除了队长，只有自己能删除自己
	{
		LOG_PROCESS_ERROR(pDelMemberRequest->dwMemberID == pDelMemberRequest->dwRequestSrc);
	}

	//LOG_PROCESS_ERROR(!pTeam->bSystem); // 允许退出系统队伍

	bRetCode = g_pGameCenter->m_TeamCenter.DelMember(
		pDelMemberRequest->dwTeamID, pDelMemberRequest->dwMemberID
		);
	LOG_PROCESS_ERROR(bRetCode);

	pDelRole->m_dwTeamID = KD_BAD_ID;

EXIT0:
	return;
}

void QCenterTeamModule::OnTeamChangeAuthorityRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                bRetCode                = FALSE;
	S2R_TEAM_CHANGE_AUTHORITY_REQUEST*  pChangeAuthority        = (S2R_TEAM_CHANGE_AUTHORITY_REQUEST*)pbyData;
	KRole*                              pRequestRole            = NULL;
	KRole*                              pTargetRole             = NULL;
	KTeam*                              pTeam                   = NULL;

	pRequestRole = g_pGameCenter->m_RoleManager.GetRole(pChangeAuthority->dwRequestSrc);
	LOG_PROCESS_ERROR(pRequestRole);

	pTargetRole = g_pGameCenter->m_RoleManager.GetRole(pChangeAuthority->dwTargetID);
	LOG_PROCESS_ERROR(pTargetRole);

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pChangeAuthority->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	// 只有队长或者权限所有者能变更对应的权限
	PROCESS_ERROR(
		pRequestRole->GetId() == pTeam->dwAuthority[tatLeader] ||
		pRequestRole->GetId() == pTeam->dwAuthority[pChangeAuthority->byType]
	);

	// 新队长必须在队伍里
	bRetCode = g_pGameCenter->m_TeamCenter.IsRoleInTeam(
		pChangeAuthority->dwTeamID, pChangeAuthority->dwTargetID
		);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_pGameCenter->m_TeamCenter.SetAuthority(
		pChangeAuthority->dwTeamID, (TEAM_AUTHORITY_TYPE)pChangeAuthority->byType, pChangeAuthority->dwTargetID
		);
	LOG_PROCESS_ERROR(bRetCode);

	if (pChangeAuthority->byType == tatLeader)
	{
		pTargetRole->UpdateMapCopyOwnerForTeamLeader(); // 副本所属更新
	}

EXIT0:
	return;
}

void QCenterTeamModule::OnTeamSetLootModeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode    = FALSE;
	S2R_TEAM_SET_LOOT_MODE_REQUEST* pRequest    = (S2R_TEAM_SET_LOOT_MODE_REQUEST*)pbyData;
	KTeam*                          pTeam       = NULL;
	KRole*                          pRole       = NULL;

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pRequest->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRequestSrc);
	LOG_PROCESS_ERROR(pRole);

	LOG_PROCESS_ERROR(!pTeam->bSystem);

	PROCESS_ERROR(pRole->GetId() == pTeam->dwAuthority[tatDistribute]);

	PROCESS_ERROR(pRequest->nLootMode > ilmInvalid && pRequest->nLootMode < ilmTotal); 

	pTeam->nLootMode = pRequest->nLootMode;

	DoTeamSetLootModeNotify(pRequest->dwTeamID, pTeam->nLootMode, pTeam->nRollQuality);

EXIT0:
	return;
}

void QCenterTeamModule::OnTeamSetRollQualityRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                bRetCode = false;
	S2R_TEAM_SET_ROLL_QUALITY_REQUEST*  pRequest = (S2R_TEAM_SET_ROLL_QUALITY_REQUEST*)pbyData;
	KTeam*                          pTeam       = NULL;
	KRole*                          pRole       = NULL;

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pRequest->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRequestSrc);
	LOG_PROCESS_ERROR(pRole);

	LOG_PROCESS_ERROR(!pTeam->bSystem);

	PROCESS_ERROR(pRole->GetId() == pTeam->dwAuthority[tatDistribute]);

	pTeam->nRollQuality = pRequest->nRollQuality;

	DoTeamSetLootModeNotify(pRequest->dwTeamID, pTeam->nLootMode, pTeam->nRollQuality);

EXIT0:
	return;
}

void QCenterTeamModule::OnTeamSetFormationLeader(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetCode = false;
	S2R_TEAM_SET_FORMATION_LEADER_REQUEST*  pRequest = (S2R_TEAM_SET_FORMATION_LEADER_REQUEST*)pbyData;
	KRole*                                  pRole    = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwNewFormationLeader);
	LOG_PROCESS_ERROR(pRole);
	PROCESS_ERROR(pRole->IsOnline());

	bRetCode = g_pGameCenter->m_TeamCenter.SetFormationLeader(
		pRequest->dwTeamID, pRequest->nGroupIndex, pRequest->dwRequestSrc, pRequest->dwNewFormationLeader
		);
	PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QCenterTeamModule::OnTeamDisbandRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                        bRetCode            = false;
	S2R_TEAM_DISBAND_REQUEST*   pTeamDisbandRequest = NULL;
	KTeam*                      pTeam               = NULL;

	// 处理解散队伍的请求
	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_TEAM_DISBAND_REQUEST));
	pTeamDisbandRequest = (S2R_TEAM_DISBAND_REQUEST*)pbyData;
	LOG_PROCESS_ERROR(pTeamDisbandRequest);

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pTeamDisbandRequest->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);
	if(!pTeam->bSystem)
	{
		LOG_PROCESS_ERROR(pTeamDisbandRequest->dwLeaderID == pTeam->dwAuthority[tatLeader]);
	}
	//LOG_PROCESS_ERROR(!pTeam->bSystem);
	

	bRetCode = g_pGameCenter->m_TeamCenter.DestroyTeam(pTeamDisbandRequest->dwTeamID);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QCenterTeamModule::OnTeamSyncMemberMaxLMR(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode        = FALSE;
	S2R_SYNC_TEAM_MEMBER_MAX_LMR*   pS2RPak         = NULL;
	IMemBlock*                     piSendBuffer    = NULL;
	R2S_SYNC_TEAM_MEMBER_MAX_LMR*   pR2SPak         = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_SYNC_TEAM_MEMBER_MAX_LMR));

	pS2RPak = (S2R_SYNC_TEAM_MEMBER_MAX_LMR*)pbyData;
	LOG_PROCESS_ERROR(pS2RPak);

	piSendBuffer = QCreateMemBlock(sizeof(R2S_SYNC_TEAM_MEMBER_MAX_LMR));
	LOG_PROCESS_ERROR(piSendBuffer);

	pR2SPak = (R2S_SYNC_TEAM_MEMBER_MAX_LMR*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pR2SPak);

	pR2SPak->wProtocolID      = r2s_sync_team_member_max_lmr;
	pR2SPak->dwTeamID         = pS2RPak->dwTeamID;
	pR2SPak->dwMemberID       = pS2RPak->dwMemberID;
	pR2SPak->nMaxLife         = pS2RPak->nMaxLife;
	pR2SPak->nMaxMana         = pS2RPak->nMaxMana;
	pR2SPak->nMaxRage         = pS2RPak->nMaxRage;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return ;
}

void QCenterTeamModule::OnTeamSyncMemberCurrentLMR(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                bRetCode        = FALSE;
	S2R_SYNC_TEAM_MEMBER_CURRENT_LMR*   pS2RPak         = NULL;
	IMemBlock*                         piSendBuffer    = NULL;
	R2S_SYNC_TEAM_MEMBER_CURRENT_LMR*   pR2SPak         = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_SYNC_TEAM_MEMBER_CURRENT_LMR));

	pS2RPak = (S2R_SYNC_TEAM_MEMBER_CURRENT_LMR*)pbyData;
	LOG_PROCESS_ERROR(pS2RPak);

	piSendBuffer = QCreateMemBlock(sizeof(R2S_SYNC_TEAM_MEMBER_CURRENT_LMR));
	LOG_PROCESS_ERROR(piSendBuffer);

	pR2SPak = (R2S_SYNC_TEAM_MEMBER_CURRENT_LMR*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pR2SPak);

	pR2SPak->wProtocolID      = r2s_sync_team_member_current_lmr;
	pR2SPak->dwTeamID        = pS2RPak->dwTeamID;
	pR2SPak->dwMemberID       = pS2RPak->dwMemberID;
	pR2SPak->byLifePercent    = pS2RPak->byLifePercent;
	pR2SPak->byManaPercent    = pS2RPak->byManaPercent;
	pR2SPak->byRagePercent    = pS2RPak->byRagePercent;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return ;
}

void QCenterTeamModule::OnTeamSyncMemberMisc(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                bRetCode        = FALSE;
	S2R_SYNC_TEAM_MEMBER_MISC*         pS2RPak         = NULL;
	IMemBlock*                         piSendBuffer    = NULL;
	R2S_SYNC_TEAM_MEMBER_MISC*         pR2SPak         = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_SYNC_TEAM_MEMBER_MISC));

	pS2RPak = (S2R_SYNC_TEAM_MEMBER_MISC*)pbyData;
	LOG_PROCESS_ERROR(pS2RPak);

	piSendBuffer = QCreateMemBlock(sizeof(R2S_SYNC_TEAM_MEMBER_MISC));
	LOG_PROCESS_ERROR(piSendBuffer);

	pR2SPak = (R2S_SYNC_TEAM_MEMBER_MISC*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pR2SPak);

	pR2SPak->wProtocolID      = r2s_sync_team_member_misc;
	pR2SPak->dwTeamID        = pS2RPak->dwTeamID;
	pR2SPak->dwMemberID       = pS2RPak->dwMemberID;

	//pR2SPak->PortraitInfo     = pS2RPak->PortraitInfo;

	pR2SPak->dwForceID        = pS2RPak->dwForceID;
	pR2SPak->byCamp           = pS2RPak->byCamp;
	pR2SPak->byLevel          = pS2RPak->byLevel;
	pR2SPak->byRoleType       = pS2RPak->byRoleType;
	pR2SPak->byDeathFlag       = pS2RPak->byDeathFlag;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return ;
}

void QCenterTeamModule::OnTeamSyncMemberPosition(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode        = false;
	S2R_SYNC_TEAM_MEMBER_POSITION*  pS2RPak         = NULL;
	IMemBlock*                     piSendBuffer    = NULL;
	R2S_SYNC_TEAM_MEMBER_POSITION*  pR2SPak         = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_SYNC_TEAM_MEMBER_POSITION));

	pS2RPak = (S2R_SYNC_TEAM_MEMBER_POSITION*)pbyData;
	LOG_PROCESS_ERROR(pS2RPak);

	piSendBuffer = QCreateMemBlock(sizeof(R2S_SYNC_TEAM_MEMBER_POSITION));
	LOG_PROCESS_ERROR(piSendBuffer);

	pR2SPak = (R2S_SYNC_TEAM_MEMBER_POSITION*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pR2SPak);

	pR2SPak->wProtocolID      = r2s_sync_team_member_position;
	pR2SPak->dwTeamID        = pS2RPak->dwTeamID;
	pR2SPak->dwMemberID       = pS2RPak->dwMemberID;
	pR2SPak->dwMapID          = pS2RPak->dwMapID;
	pR2SPak->nMapCopyIndex    = pS2RPak->nMapCopyIndex;
	pR2SPak->nPosX            = pS2RPak->nPosX;
	pR2SPak->nPosY            = pS2RPak->nPosY;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return ;
}

void QCenterTeamModule::OnTeamSetMarkRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                        bRetCode    = false;
	S2R_TEAM_SET_MARK_REQUEST*  pS2RPak     = NULL;
	KTeam*                      pTeam       = NULL;
	KRole*                      pRole       = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_TEAM_SET_MARK_REQUEST));

	pS2RPak = (S2R_TEAM_SET_MARK_REQUEST*)pbyData;
	LOG_PROCESS_ERROR(pS2RPak);

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pS2RPak->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pS2RPak->dwMemberID);
	LOG_PROCESS_ERROR(pRole);

	PROCESS_ERROR(pTeam->dwAuthority[tatMark] == pRole->GetId());

	bRetCode = g_pGameCenter->m_TeamCenter.SetMark(
		pS2RPak->dwTeamID, pS2RPak->nMarkType, pS2RPak->dwTargetID
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QCenterTeamModule::OnTeamLevelRaidRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode    = false;
	S2R_TEAM_LEVEL_UP_RAID_REQUEST* pRequest    = (S2R_TEAM_LEVEL_UP_RAID_REQUEST*)pbyData;
	KRole*                          pRole       = NULL;
	KTeam*                          pTeam       = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_TEAM_LEVEL_UP_RAID_REQUEST));

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pRequest->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	LOG_PROCESS_ERROR(!pTeam->bSystem);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRequestSrc);
	LOG_PROCESS_ERROR(pRole);

	LOG_PROCESS_ERROR(pRole->GetId() == pTeam->dwAuthority[tatLeader]);
	LOG_PROCESS_ERROR(pTeam->nGroupNum == 1);

	pTeam->nGroupNum = MAX_TEAM_GROUP_NUM;

	DoTeamLevelUpRaidNotify(pRequest->dwTeamID, pTeam->nGroupNum);

EXIT0:
	return;
}

void QCenterTeamModule::OnTeamChangeMemberGroupRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetCode     = false;
	S2R_TEAM_CHANGE_MEMBER_GROUP_REQUEST*   pRequest     = (S2R_TEAM_CHANGE_MEMBER_GROUP_REQUEST*)pbyData;
	KRole*                                  pMemebrRole  = NULL;
	KRole*                                  pRequestRole = NULL;
	KTeam*                                  pTeam        = NULL;

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_TEAM_CHANGE_MEMBER_GROUP_REQUEST));

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pRequest->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	pRequestRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRequestSrc);
	LOG_PROCESS_ERROR(pRequestRole);

	pMemebrRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwMemberID);
	LOG_PROCESS_ERROR(pMemebrRole);

	LOG_PROCESS_ERROR(pRequestRole->GetId() == pTeam->dwAuthority[tatLeader]);

	bRetCode = g_pGameCenter->m_TeamCenter.ChangeGroup(
		pRequest->dwTeamID, pRequest->nDstGroupIndex, pRequest->dwDstMemberID, pRequest->dwMemberID
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}


