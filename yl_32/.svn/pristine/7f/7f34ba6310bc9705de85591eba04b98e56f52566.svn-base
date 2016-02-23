

#include "stdafx.h"
#include "onlinegamemodule/kmoduledef.h"
#include "serverbase/kgcmoduledef.h"
#include "kteammodule.h"
#include "onlinegamemodule/kteamprotocol.h"
#include "onlinegameworld/kplayer.h"
#include "qteamgchandler.h"
#include "qteamserver.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/ikscenemgr.h"
#include "../onlinegameworld/scene/kscene.h"
#include "onlinegamemodule/chat/kchatmodule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define REGISTER_EXTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
	m_nProtocolSize[ProtocolID] = ProtocolSize;}						


QTeamModule g_TeamModule;
template<>
IKModuleInterface* GetModuleInterFace<emMODULE_TEAM>()
{
	return &g_TeamModule;
}

QTeamModule::PROCESS_PROTOCOL_FUNC	QTeamModule::m_ProcessProtocolFuns[c2s_team_protocol_total] = {0};
size_t QTeamModule::m_nProtocolSize[c2s_team_protocol_total] = {0};

// -------------------------------------------------------------------------
QTeamModule::QTeamModule() : KGcModuleBase(emMODULE_TEAM, emKGCMODULE_TEAM)
{
}

BOOL QTeamModule::OnInit()
{
	QTeamGCHandler::InitProtocol();

	REGISTER_EXTERNAL_FUNC(c2s_invite_join_team_request, &QTeamModule::OnInviteJoinTeamRequest, sizeof(C2S_INVITE_JOIN_TEAM_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_invite_join_team_respond, &QTeamModule::OnInviteJoinTeamRespond, sizeof(C2S_INVITE_JOIN_TEAM_RESPOND));
	REGISTER_EXTERNAL_FUNC(c2s_apply_join_team_request, &QTeamModule::OnApplyJoinTeamRequest, sizeof(C2S_APPLY_JOIN_TEAM_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_apply_join_team_respond, &QTeamModule::OnApplyJoinTeamRespond, sizeof(C2S_APPLY_JOIN_TEAM_RESPOND));
	REGISTER_EXTERNAL_FUNC(c2s_team_del_member_request, &QTeamModule::OnTeamDelMemberRequest, sizeof(C2S_TEAM_DEL_MEMBER_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_team_change_authority_request, &QTeamModule::OnTeamChangeAuthorityRequest, sizeof(C2S_TEAM_CHANGE_AUTHORITY_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_team_set_distribute_mode_request, &QTeamModule::OnTeamSetLootMode, sizeof(C2S_TEAM_SET_LOOT_MODE_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_team_set_roll_quality_request, &QTeamModule::OnTeamSetRollQuality, sizeof(C2S_TEAM_SET_ROLL_QUALITY_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_team_set_formation_leader_request, &QTeamModule::OnTeamSetFormationLeader, sizeof(C2S_TEAM_SET_FORMATION_LEADER_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_team_member_notify_signpost_request, &QTeamModule::OnTeamMemberNotifySignpost, sizeof(C2S_TEAM_MEMBER_NOTIFY_SIGNPOST_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_team_set_mark_request, &QTeamModule::OnTeamSetMarkRequest, sizeof(C2S_TEAM_SET_MARK_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_team_level_up_raid_request, &QTeamModule::OnTeamLevelUpRaidRequest, sizeof(C2S_TEAM_LEVEL_UP_RAID_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_team_change_member_group_request, &QTeamModule::OnTeamChangeMemberGroupRequest, sizeof(C2S_TEAM_CHANGE_MEMBER_GROUP_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_create_team_request, &QTeamModule::OnCreateTeamRequest, sizeof(C2S_CREATE_TEAM_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_disband_team_request, &QTeamModule::OnDisbandTeamRequest, sizeof(C2S_DISBAND_TEAM_REQUEST));

	return TRUE;
}

BOOL QTeamModule::OnUnInit()
{
	return TRUE;
}

BOOL QTeamModule::ProcessProtocol( INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen )
{
	BOOL bResult = FALSE;
	PROCESS_PROTOCOL_FUNC       pFunc       = NULL;
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pbData;
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pbData);
	LOG_PROCESS_ERROR(uDataLen);

	pFunc = m_ProcessProtocolFuns[pHeader->byProtocol];
	LOG_PROCESS_ERROR(pFunc);

	(pFunc)(pPlayer, (CHAR*)pbData, uDataLen);


	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL QTeamModule::ProcessGcProtocol( LPCBYTE pbData, UINT uDataLen )
{
	return QTeamGCHandler::ProcessGcProtocol(pbData, uDataLen);
}

BOOL QTeamModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData )
{
	return g_TeamModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL QTeamModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_TeamModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}

BOOL QTeamModule::SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize )
{
	return g_TeamModule.GetProtocolProcessor()->SendUnknownData(nPlayerIndex, pData, uDataSize);
}

// 组队操作前的判断
BOOL QTeamModule::CanTeamProcess(KPlayer* pPlayer)
{
	BOOL bRet = FALSE;
	KScene* pScene = NULL;
	INT nMapId = 0;

	pScene = (KScene*)pPlayer->GetScene();
	LOG_PROCESS_ERROR(pScene);

	nMapId = pScene->GetTemplateId();

	if(g_pSceneMgr->CanTeam(nMapId) == FALSE)
	{
		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, pPlayer->GetId(), "DEBUG", LANG_045);
		goto EXIT0;
	}

	bRet = TRUE;
EXIT0:
	return bRet;
}

// client protocol handlers
void QTeamModule::OnInviteJoinTeamRequest(KPlayer* pPlayer, char* pData, size_t nSize)
{
	BOOL                            bRetCode                = false;
	C2S_INVITE_JOIN_TEAM_REQUEST*   pRequest  = (C2S_INVITE_JOIN_TEAM_REQUEST*)pData;
	QLogPrintf(LOG_INFO, "QTeamModule::OnInviteJoinTeamRequest szInvitedPlayer=%s playerId=%d\n", pRequest->szInvitedPlayer, pPlayer->GetId());

	LOG_PROCESS_ERROR(CanTeamProcess(pPlayer));

	pRequest->szInvitedPlayer[sizeof(pRequest->szInvitedPlayer) - 1] = '\0';
	QTeamGCHandler::DoInviteJoinTeamRequest(pPlayer->GetId(), pRequest->szInvitedPlayer);

EXIT0:
	return;
}

void QTeamModule::OnInviteJoinTeamRespond(KPlayer* pPlayer, char* pData, size_t nSize)
{
	BOOL                                  bRetCode                = false;
	C2S_INVITE_JOIN_TEAM_RESPOND*         pRespond = (C2S_INVITE_JOIN_TEAM_RESPOND*)pData;

	QLogPrintf(LOG_INFO, "QTeamModule::OnInviteJoinTeamRespond szInviteSrc=%s bAgreeFlag=%d playerId=%d\n",
		pRespond->szInviteSrc, (int)pRespond->bAgreeFlag, pPlayer->GetId());

	pRespond->szInviteSrc[sizeof(pRespond->szInviteSrc) - 1] = '\0';

	QTeamGCHandler::DoInviteJoinTeamRespond(
		pRespond->szInviteSrc, pPlayer->GetId(), pRespond->bAgreeFlag
		);

EXIT0:
	return;
}

void QTeamModule::OnApplyJoinTeamRequest(KPlayer* pPlayer, char* pData, size_t nSize)
{
	C2S_APPLY_JOIN_TEAM_REQUEST*  pRequest  = (C2S_APPLY_JOIN_TEAM_REQUEST*)pData;

	pRequest->szApplyDst[sizeof(pRequest->szApplyDst) - 1] = '\0';

	QLogPrintf(LOG_INFO, "QTeamModule::OnApplyJoinTeamRequest szApplyDst=%s playerId=%d\n",
		pRequest->szApplyDst, pPlayer->GetId());

	LOG_PROCESS_ERROR(CanTeamProcess(pPlayer));

	// 交给Relay去处理
	QTeamGCHandler::DoApplyJoinTeamRequest(pPlayer->GetId(), pRequest->szApplyDst);
EXIT0:
	return;
}

void QTeamModule::OnCreateTeamRequest(KPlayer* pPlayer, char* pData, size_t nSize)
{
	C2S_CREATE_TEAM_REQUEST*  pRequest  = (C2S_CREATE_TEAM_REQUEST*)pData;

	QLogPrintf(LOG_INFO, "QTeamModule::OnCreateTeamRequest dwCreateSrc=%d playerId=%d\n", pRequest->dwCreateSrc, pPlayer->GetId());

	LOG_PROCESS_ERROR(CanTeamProcess(pPlayer));

	QTeamGCHandler::DoCreateTeamRequest(pPlayer->GetId(), pRequest->dwCreateSrc);

EXIT0:
	return;
}

void QTeamModule::OnDisbandTeamRequest(KPlayer* pPlayer, char* pData, size_t nSize)
{
	C2S_DISBAND_TEAM_REQUEST*  pRequest  = (C2S_DISBAND_TEAM_REQUEST*)pData;

	QLogPrintf(LOG_INFO, "QTeamModule::OnDisbandTeamRequest dwTeamId=%d playerId=%d\n", pRequest->dwTeamId, pPlayer->GetId());

	LOG_PROCESS_ERROR(CanTeamProcess(pPlayer));

	QTeamGCHandler::DoTeamDisbandRequest(pPlayer->GetId(), pRequest->dwTeamId);

EXIT0:
	return;
}

void QTeamModule::OnApplyJoinTeamRespond(KPlayer* pPlayer, char* pData, size_t nSize)
{
	C2S_APPLY_JOIN_TEAM_RESPOND* pRespond = (C2S_APPLY_JOIN_TEAM_RESPOND*)pData;

	pRespond->szApplySrc[sizeof(pRespond->szApplySrc) - 1] = '\0';

	QLogPrintf(LOG_INFO, "QTeamModule::OnApplyJoinTeamRespond szApplySrc=%s bAgreeFlag=%d playerId=%d\n", 
		pRespond->szApplySrc, (int)pRespond->bAgreeFlag, pPlayer->GetId());

	QTeamGCHandler::DoApplyJoinTeamRespond(
		pPlayer->GetId(), pRespond->szApplySrc, pRespond->bAgreeFlag
		);
EXIT0:
	return;
}


void QTeamModule::OnTeamDelMemberRequest(KPlayer* pPlayer, char* pData, size_t nSize)
{
	BOOL                          bRetCode               = false;
	C2S_TEAM_DEL_MEMBER_REQUEST*  pRequest = (C2S_TEAM_DEL_MEMBER_REQUEST*)pData;
	QTeam*                        pTeam                  = NULL;
	QLogPrintf(LOG_INFO,"QTeamModule::OnTeamDelMemberRequest dwMemberID=%d playerId=%d\n", 
		pRequest->dwMemberID, pPlayer->GetId());

	LOG_PROCESS_ERROR(pPlayer->GetTeamId() != KD_BAD_ID);

	LOG_PROCESS_ERROR(CanTeamProcess(pPlayer));

	pTeam = g_cTeamServer.GetTeam(pPlayer->GetTeamId());
	LOG_PROCESS_ERROR(pTeam);
	//LOG_PROCESS_ERROR(!pTeam->bSystem); // 允许退出系统队伍

	bRetCode = QTeamGCHandler::DoTeamDelMemberRequest(
		pPlayer->GetTeamId(), pPlayer->GetId(), pRequest->dwMemberID
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QTeamModule::OnTeamChangeAuthorityRequest(KPlayer* pPlayer, char* pData, size_t nSize)
{
	BOOL                                bRetCode             = false;
	C2S_TEAM_CHANGE_AUTHORITY_REQUEST*  pRequest = (C2S_TEAM_CHANGE_AUTHORITY_REQUEST*)pData;
	QTeam*                              pTeam                = NULL;
	DWORD                               dwTeamID             = KD_BAD_ID;
	QLogPrintf(LOG_INFO,"QTeamModule::OnTeamChangeAuthorityRequest byType=%d dwNewLeaderID=%d PlayerID=%d\n",
		(int)pRequest->byType, pRequest->dwNewLeaderID, pPlayer->GetId());

	// 注: 移交的目标玩家的状态的正确性Relay会核准的

	dwTeamID = pPlayer->GetTeamId();

	pTeam = g_cTeamServer.GetTeam(dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	LOG_PROCESS_ERROR(CanTeamProcess(pPlayer));

	bRetCode = QTeamGCHandler::DoTeamChangeAuthorityRequest(
		dwTeamID, pPlayer->GetId(), (TEAM_AUTHORITY_TYPE)pRequest->byType, pRequest->dwNewLeaderID
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QTeamModule::OnTeamSetLootMode(KPlayer* pPlayer, char* pData, size_t nSize)
{
	BOOL                                bRetCode     = false;
	C2S_TEAM_SET_LOOT_MODE_REQUEST*     pRequest     = (C2S_TEAM_SET_LOOT_MODE_REQUEST*)pData;
	QTeam*                              pTeam        = NULL;

	QLogPrintf(LOG_INFO, "QTeamModule::OnTeamSetLootMode nLootMode=%d playerId=%d\n",
		pRequest->nLootMode, pPlayer->GetId());

	LOG_PROCESS_ERROR(pPlayer->GetTeamId() != KD_BAD_ID);

	pTeam = g_cTeamServer.GetTeam(pPlayer->GetTeamId());
	LOG_PROCESS_ERROR(pTeam);
	LOG_PROCESS_ERROR(!pTeam->bSystem);

	LOG_PROCESS_ERROR(pPlayer->GetId() == pTeam->dwAuthority[tatDistribute]); //只有分配者才能修改分配模式

	bRetCode = QTeamGCHandler::DoTeamSetLootModeRequest(pPlayer->GetTeamId(), pPlayer->GetId(), pRequest->nLootMode);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QTeamModule::OnTeamSetRollQuality(KPlayer* pPlayer, char* pData, size_t nSize)
{
	BOOL	                            bRetCode    = false;
	C2S_TEAM_SET_ROLL_QUALITY_REQUEST*  pRequest    = (C2S_TEAM_SET_ROLL_QUALITY_REQUEST*)pData;
	QTeam*                              pTeam       = NULL;

	QLogPrintf(LOG_INFO, "QTeamModule::OnTeamSetRollQuality nRollQuality=%d playerId=%d\n",
		pRequest->nRollQuality, pPlayer->GetId());

	pTeam = g_cTeamServer.GetTeam(pPlayer->GetTeamId());
	LOG_PROCESS_ERROR(pTeam);
	LOG_PROCESS_ERROR(!pTeam->bSystem);

	LOG_PROCESS_ERROR(pPlayer->GetId() == pTeam->dwAuthority[tatDistribute]); //只有分配者才能修改分配模式

	QTeamGCHandler::DoTeamSetRollQualityRequest(pPlayer->GetTeamId(), pPlayer->GetId(), pRequest->nRollQuality);

EXIT0:
	return;
}

void QTeamModule::OnTeamSetFormationLeader(KPlayer* pPlayer, char* pData, size_t nSize)
{
	BOOL	                                bRetCode            = false;
	C2S_TEAM_SET_FORMATION_LEADER_REQUEST*  pRequest            = (C2S_TEAM_SET_FORMATION_LEADER_REQUEST*)pData;
	BOOL                                    bIsFormationLeader  = false;
	DWORD                                   dwTeamID            = KD_BAD_ID;
	QTeam*                                  pTeam               = NULL;

	QLogPrintf(LOG_INFO, "QTeamModule::OnTeamSetFormationLeader dwNewFormationLeaderID=%d nGroupIndex=%d playerId=%d\n",
		pRequest->dwNewFormationLeaderID, pRequest->nGroupIndex, pPlayer->GetId());

	dwTeamID = pPlayer->GetTeamId();
	LOG_PROCESS_ERROR(dwTeamID != KD_BAD_ID);

	pTeam = g_cTeamServer.GetTeam(dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	LOG_PROCESS_ERROR(pRequest->nGroupIndex >= 0);
	LOG_PROCESS_ERROR(pRequest->nGroupIndex < pTeam->nGroupNum);

	LOG_PROCESS_ERROR(
		pTeam->MemberGroup[pRequest->nGroupIndex].dwFormationLeader == pPlayer->GetId() ||
		pPlayer->GetId() == pTeam->dwAuthority[tatLeader]
	);

	QTeamGCHandler::DoTeamSetFormationLeader(
		dwTeamID, pPlayer->GetId(), pRequest->dwNewFormationLeaderID, pRequest->nGroupIndex
		);

EXIT0:
	return;
}

void QTeamModule::OnTeamMemberNotifySignpost(KPlayer* pPlayer, char* pData, size_t nSize)
{
	BOOL                                        bRetcode         = false;
	KPlayer*                                    pMemberPlayer    = NULL;
	DWORD                                       dwTeamID         = KD_BAD_ID;
	QTeam*                                      pTeam            = NULL;
	C2S_TEAM_MEMBER_NOTIFY_SIGNPOST_REQUEST*    pRequest         = (C2S_TEAM_MEMBER_NOTIFY_SIGNPOST_REQUEST*)pData;
	S2C_TEAM_MEMBER_SIGNPOST_NOTIFY             Response;

	QLogPrintf(LOG_INFO, "QTeamModule::OnTeamMemberNotifySignpost nX=%d nY=%d playerId=%d\n",
		pRequest->nX, pRequest->nY, pPlayer->GetId());

	dwTeamID = pPlayer->GetTeamId();
	LOG_PROCESS_ERROR(dwTeamID != KD_BAD_ID);
	LOG_PROCESS_ERROR(pPlayer->GetScene() != NULL);

	Response.nX           = pRequest->nX;
	Response.nY           = pRequest->nY;

	pTeam = g_cTeamServer.GetTeam(dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		for (
			QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
			it != pTeam->MemberGroup[i].MemberList.end(); ++it
			)
		{
			if (it->dwMemberID == pPlayer->GetId())
			{
				continue;
			}

			pMemberPlayer = g_cPlayerMgr.GetById(it->dwMemberID);
			if (pMemberPlayer == NULL)
			{
				continue;
			}

			if (pMemberPlayer->GetScene() == NULL)
			{
				continue;
			}

			// 只发给在同一地图内的队友
			if (pPlayer->GetScene() != pMemberPlayer->GetScene())
			{
				continue;
			}

			SendData(pMemberPlayer->GetPlayerIndex(), Response);
		}
	}

EXIT0:
	return;
}

void QTeamModule::OnTeamSetMarkRequest(KPlayer* pPlayer, char* pData, size_t nSize)
{
	C2S_TEAM_SET_MARK_REQUEST*  pRequest = (C2S_TEAM_SET_MARK_REQUEST*)pData;
	DWORD                       dwTeamID             = KD_BAD_ID;
	QTeam*                      pTeam                = NULL;

	QLogPrintf(LOG_INFO, "QTeamModule::OnTeamSetMarkRequest nMarkType=%d dwTargetID=%d playerId=%d\n",
		pRequest->nMarkType, pRequest->dwTargetID, pPlayer->GetId());

	dwTeamID = pPlayer->GetTeamId();
	LOG_PROCESS_ERROR(dwTeamID != KD_BAD_ID);

	pTeam = g_cTeamServer.GetTeam(dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	LOG_PROCESS_ERROR(pTeam->dwAuthority[tatMark] == pPlayer->GetId());

	QTeamGCHandler::DoTeamSetMarkRequest(
		dwTeamID, pPlayer->GetId(), pRequest->nMarkType, pRequest->dwTargetID
		);

EXIT0:
	return;
}

void QTeamModule::OnTeamLevelUpRaidRequest(KPlayer* pPlayer, char* pData, size_t nSize)
{
	C2S_TEAM_LEVEL_UP_RAID_REQUEST* pRequest = (C2S_TEAM_LEVEL_UP_RAID_REQUEST*)pData;
	QTeam*                          pTeam    = NULL;

	QLogPrintf(LOG_INFO, "QTeamModule::OnTeamLevelUpRaidRequest playerId=%d\n", pPlayer->GetId());

	LOG_PROCESS_ERROR(pPlayer->GetTeamId() != KD_BAD_ID);

	pTeam = g_cTeamServer.GetTeam(pPlayer->GetTeamId());
	LOG_PROCESS_ERROR(pTeam);
	LOG_PROCESS_ERROR(!pTeam->bSystem);

	PROCESS_ERROR(pTeam->dwAuthority[tatLeader] == pPlayer->GetId());
	LOG_PROCESS_ERROR(pTeam->nGroupNum == 1);

	QTeamGCHandler::DoTeamLevelUpRequest(pPlayer->GetTeamId(), pPlayer->GetId());
EXIT0:
	return;
}

void QTeamModule::OnTeamChangeMemberGroupRequest(KPlayer* pPlayer, char* pData, size_t nSize)
{
	C2S_TEAM_CHANGE_MEMBER_GROUP_REQUEST*   pRequest = (C2S_TEAM_CHANGE_MEMBER_GROUP_REQUEST*)pData;
	DWORD                                   dwTeamID = KD_BAD_ID;
	QTeam*                                  pTeam    = NULL;

	QLogPrintf(LOG_INFO, "QTeamModule::OnTeamChangeMemberGroupRequest dwMemberID=%d nDstGroupIndex=%d dwDstMemberID=%d playerId=%d\n",
		pRequest->dwMemberID, pRequest->nDstGroupIndex, pRequest->dwDstMemberID, pPlayer->GetId());

	dwTeamID = pPlayer->GetTeamId();
	LOG_PROCESS_ERROR(dwTeamID != KD_BAD_ID);

	pTeam = g_cTeamServer.GetTeam(dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	LOG_PROCESS_ERROR(pTeam->dwAuthority[tatLeader] == pPlayer->GetId());
	LOG_PROCESS_ERROR(pRequest->nDstGroupIndex >= 0 && pRequest->nDstGroupIndex <= pTeam->nGroupNum);

	QTeamGCHandler::DoTeamChangeMemberGroupRequest(
		dwTeamID, pRequest->dwMemberID, pRequest->nDstGroupIndex, pRequest->dwDstMemberID, pPlayer->GetId()
		);
EXIT0:
	return;
}


