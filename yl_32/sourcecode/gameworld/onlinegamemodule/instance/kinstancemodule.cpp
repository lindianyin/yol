

#include "stdafx.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#include "kinstancemodule.h"
#include "kinstancectrl.h"
#include "overallteam/koverallmanager.h"
#include "overallteam/koverallrelayclient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KInstanceModule g_cInstanceModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_INSTANCE>()
{
	return &g_cInstanceModule;
}

KInstanceModule::KInstanceModule() : KModuleBase(emMODULE_INSTANCE)
{}

BOOL KInstanceModule::OnInit()
{
	return TRUE;
}

BOOL KInstanceModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_OVERALL_TEAMS_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_CREATE_OVERALL_TEAM_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_JOIN_OVERALL_TEAM_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_LEAVE_OVERALL_TEAM_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_MEMBER_PREPARE_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_PLAYER_TEAM_INFO_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_KICK_OVERALL_PLAYER_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_START_MISSION_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_MEMBER_COUNT_DOWN_REQUEST);

	KD_PTC_FIXED_SIZE_C2S(KC2S_ADD_OVERALL_TEAM_WATCH);
	KD_PTC_FIXED_SIZE_C2S(KC2S_DEL_OVERALL_TEAM_WATCH);

	KD_PTC_FIXED_SIZE_C2S(KC2S_BROADCAST_OVERALLTEAM_MSG);

	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_MULTI_INSTANCE_INFO);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_OVERALL_TEAMS);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_PLAYER_OVERALLTEAM);
	KD_PTC_FIXED_SIZE_S2C(KS2C_OVERALL_TEAMOP_RESULT);
	KD_PTC_FIXED_SIZE_S2C(KS2C_MEMBER_COUNTDOWN_NOTIFY);

	return TRUE;
}

BOOL KInstanceModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	BOOL						bResult = FALSE;
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_get_overall_teams_request:
		bResult = c2sOnGetOverallTeamRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_create_overall_team_request:
		bResult = c2sOnCreateOverallTeamRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_join_overall_team_request:
		bResult = c2sOnJoinOverallTeamRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_leave_overall_team_request:
		bResult = c2sOnLeaveOverallTeamRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_member_prepare_request:
		bResult = c2sOnMemberPrepareRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_get_player_overallteam_request:
		bResult = c2sOnGetPlayerTeamInfoRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_add_overallteam_watch:
		bResult = c2sAddPlayerToWatch(pPlayer, pbData, uDataLen);
		break;
	case c2s_del_overallteam_watch:
		bResult = c2sDelPlayerToWatch(pPlayer, pbData, uDataLen);
		break;
	case c2s_kick_overall_player_request:
		bResult = c2sOnKickOverallPlayerRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_start_mission_request:
		bResult = c2sStartMission(pPlayer, pbData, uDataLen);
		break;
	case c2s_member_countdown_request:
		bResult = c2sMemberCountDownRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_broadcast_overallteam_msg:
		bResult = c2sBroadcastOverallTeamMsg(pPlayer, pbData, uDataLen);
		break;
	default:
		QLogPrintf(LOG_ERR, "KInstanceModule::ProcessProtocol never come to here!!!pPlayer=%d unknown type=%d\n", 
			pPlayer->GetPlayerId(), (int)(*(pbData + 1)));
		break;
	}
	
	if(!bResult){
		QLogPrintf(LOG_ERR, "KInstanceModule::ProcessProtocol result error!!!pPlayer=%d type=%d\n", 
			pPlayer->GetPlayerId(), (int)(*(pbData + 1)));
	}

	return TRUE;
}

BOOL KInstanceModule::c2sOnGetOverallTeamRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_GET_OVERALL_TEAMS_REQUEST*	pSync					= (KC2S_GET_OVERALL_TEAMS_REQUEST*)pProtocol;
	KInstanceCtrl* pInstanceCtrl = NULL;
	LIST_OVERALL_TEAMS listTeams;


	QLogPrintf(LOG_INFO, "KInstanceModule::c2sOnGetOverallTeamRequest FromPlayerId=%d nInstanceMapId= %d\n", pPlayer->GetPlayerId(), pSync->nInstanceMapId);

	pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
	LOG_PROCESS_ERROR(pInstanceCtrl);

	LOG_PROCESS_ERROR(g_OverallManager.GetOverallTeamList(pSync->nInstanceMapId, listTeams));

	LOG_PROCESS_ERROR(pInstanceCtrl->SyncOverallTeamList(listTeams));

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KInstanceModule::c2sOnCreateOverallTeamRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_CREATE_OVERALL_TEAM_REQUEST*	pRequest			= (KC2S_CREATE_OVERALL_TEAM_REQUEST*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sOnCreateOverallTeamRequest FromPlayerId=%d PlayerId=%d nInstanceMapId=%d nRequireFightscore=%d\n", 
		pPlayer->GetPlayerId(), pRequest->nPlayerId, pRequest->nInstanceMapId, pRequest->nRequireFightscore);

	return KOverallRelayClient::DoCreateOverallTeamRequest(pRequest->nPlayerId, pRequest->nInstanceMapId, pRequest->nRequireFightscore);

}

BOOL KInstanceModule::c2sOnJoinOverallTeamRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_JOIN_OVERALL_TEAM_REQUEST*	pRequest			= (KC2S_JOIN_OVERALL_TEAM_REQUEST*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sOnJoinOverallTeamRequest FromPlayerId=%d PlayerId=%d nInstanceMapId=%d dwLeaderCombinedIdx=%d\n",
		pPlayer->GetPlayerId(), pRequest->nPlayerId, pRequest->nInstanceMapId, pRequest->dwLeaderCombinedIdx);

	return KOverallRelayClient::DoJoinOverallTeamRequest(pRequest->nPlayerId, pRequest->nInstanceMapId, pRequest->dwLeaderCombinedIdx);
}

BOOL KInstanceModule::c2sOnLeaveOverallTeamRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_LEAVE_OVERALL_TEAM_REQUEST*	pRequest			= (KC2S_LEAVE_OVERALL_TEAM_REQUEST*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sOnLeaveOverallTeamRequest FromPlayerId=%d dwMemberCombinedIdx=%d dwLeaderCombinedIdx=%d byIsKick=%d\n",
		pPlayer->GetPlayerId(), pRequest->dwMemberCombinedIdx, pRequest->dwLeaderCombinedIdx, pRequest->byIsKick);

	return  KOverallRelayClient::DoLeaveOverallTeamRequest(pRequest->dwMemberCombinedIdx, pRequest->dwLeaderCombinedIdx, pRequest->byIsKick);
}

BOOL KInstanceModule::c2sOnMemberPrepareRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_MEMBER_PREPARE_REQUEST*	pRequest			= (KC2S_MEMBER_PREPARE_REQUEST*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sOnMemberPrepareRequest FromPlayerId=%d nPlayerId=%d dwLeaderCombinedIdx=%d byPrepare=%d\n",
		pPlayer->GetPlayerId(), pRequest->nPlayerId, pRequest->dwLeaderCombinedIdx, pRequest->byPrepare);

	return KOverallRelayClient::DoMemberPrepareRequest(pRequest->nPlayerId, pRequest->dwLeaderCombinedIdx, pRequest->byPrepare);
}

BOOL KInstanceModule::c2sOnGetPlayerTeamInfoRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_GET_PLAYER_TEAM_INFO_REQUEST*	pRequest			= (KC2S_GET_PLAYER_TEAM_INFO_REQUEST*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sOnGetPlayerTeamInfoRequest FromPlayerId=%d nPlayerId=%d dwLeaderCombinedIdx=%d\n",
		pPlayer->GetPlayerId(), pRequest->nPlayerId, pRequest->dwLeaderCombinedIdx);

	return KOverallRelayClient::DoGetPlayerTeamInfoRequest(pRequest->nPlayerId, pRequest->dwLeaderCombinedIdx);
}

BOOL KInstanceModule::c2sOnKickOverallPlayerRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_KICK_OVERALL_PLAYER_REQUEST*	pRequest			= (KC2S_KICK_OVERALL_PLAYER_REQUEST*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sOnKickOverallPlayerRequest FromPlayerId=%d dwMemberCombinedIdx=%d dwMemberCombinedIdx=%d\n",
		pPlayer->GetPlayerId(), pRequest->dwMemberCombinedIdx, pRequest->dwMemberCombinedIdx);

	return KOverallRelayClient::DoKickOverallPlayerRequest(pRequest->dwLeaderCombinedIdx, pRequest->dwMemberCombinedIdx);
}

BOOL KInstanceModule::c2sAddPlayerToWatch(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_ADD_OVERALL_TEAM_WATCH*	pRequest			= (KC2S_ADD_OVERALL_TEAM_WATCH*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sAddPlayerToWatch FromPlayerId=%d nPlayerId=%d nInstanceMapId=%d\n",
		pPlayer->GetPlayerId(), pRequest->nPlayerId, pRequest->nInstanceMapId);

	g_OverallManager.AddPlayerToWatchTable(pRequest->nPlayerId, pRequest->nInstanceMapId);

	return TRUE;
}

BOOL KInstanceModule::c2sDelPlayerToWatch(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_DEL_OVERALL_TEAM_WATCH*	pRequest			= (KC2S_DEL_OVERALL_TEAM_WATCH*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sDelPlayerToWatch FromPlayerId=%d nPlayerId=%d \n",
		pPlayer->GetPlayerId(), pRequest->nPlayerId);

	g_OverallManager.RemovePlayerFromWatchTable(pRequest->nPlayerId);

	return TRUE;
}

BOOL KInstanceModule::c2sBroadcastOverallTeamMsg(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_BROADCAST_OVERALLTEAM_MSG*	pRequest			= (KC2S_BROADCAST_OVERALLTEAM_MSG*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sBroadcastOverallTeamMsg FromPlayerId=%d nMapId=%d dwLeaderCombinedIdx=%d nRequireFightscore=%d szCreaterName=%s\n",
		pPlayer->GetPlayerId(), pRequest->nMapId, pRequest->dwLeaderCombinedIdx, pRequest->nRequireFightscore, pRequest->szCreaterName);

	return KOverallRelayClient::DoBroadcastOverallteamMsg(pRequest->nMapId, pRequest->dwLeaderCombinedIdx, pRequest->nRequireFightscore, pRequest->szCreaterName);
}

BOOL KInstanceModule::c2sStartMission(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_START_MISSION_REQUEST*	pRequest			= (KC2S_START_MISSION_REQUEST*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sStartMission FromPlayerId=%d dwLeaderCombinedIdx=%d\n",
		pPlayer->GetPlayerId(), pRequest->dwLeaderCombinedIdx);

	return KOverallRelayClient::DoStartMissionRequest(pRequest->dwLeaderCombinedIdx);
}

BOOL KInstanceModule::c2sMemberCountDownRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_MEMBER_COUNT_DOWN_REQUEST*	pRequest			= (KC2S_MEMBER_COUNT_DOWN_REQUEST*)pProtocol;

	QLogPrintf(LOG_INFO, "KInstanceModule::c2sMemberCountDownRequest FromPlayerId=%d dwLeaderCombinedIdx=%d\n",
		pPlayer->GetPlayerId(), pRequest->dwLeaderCombinedIdx);

	return KOverallRelayClient::DoMemberCountDownRequest(pRequest->dwLeaderCombinedIdx);
}


BOOL KInstanceModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cInstanceModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KInstanceModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cInstanceModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}