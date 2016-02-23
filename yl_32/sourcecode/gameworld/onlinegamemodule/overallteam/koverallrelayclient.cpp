#include "koverallrelayclient.h"
#include "../onlinegameworld/KRelayClient.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "koverallmanager.h"
#include "onlinegameworld/kplayermgr.h"
#include "../onlinegamemodule/instance/kinstancectrl.h"

KOverallRelayClient::KOverallRelayClient(void)
{

}	

KOverallRelayClient::~KOverallRelayClient(void)
{
	
}

BOOL KOverallRelayClient::Send( IMemBlock* piBuffer )
{
	return g_RelayClient.Send(piBuffer);
}

BOOL KOverallRelayClient::DoCreateOverallTeamRequest(INT nPlayerId, INT nInstanceMapId, INT nRequireFightScore)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_CREATE_OVERALL_TEAM_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_CREATE_OVERALL_TEAM_REQUEST));
	pRequest = (S2R_CREATE_OVERALL_TEAM_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_create_overall_team_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->nInstanceMapId = nInstanceMapId;
	pRequest->nRequireFightscore = nRequireFightScore;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallRelayClient::DoJoinOverallTeamRequest(INT nMemberId, INT nInstanceMapId, DWORD dwCombinedIdx)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_JOIN_OVERALL_TEAM_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_JOIN_OVERALL_TEAM_REQUEST));
	pRequest = (S2R_JOIN_OVERALL_TEAM_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_join_overall_team_request;
	pRequest->nMemberId = nMemberId;
	pRequest->nInstanceMapId = nInstanceMapId;
	pRequest->dwCombinedIdx = dwCombinedIdx;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallRelayClient::DoLeaveOverallTeamRequest(DWORD dwMemberCombinedIdx, DWORD dwLeaderCombinedIdx, INT nIsKick)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_LEAVE_OVERALL_TEAM_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_LEAVE_OVERALL_TEAM_REQUEST));
	pRequest = (S2R_LEAVE_OVERALL_TEAM_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_leave_overall_team_request;
	pRequest->dwMemberCombinedIdx = dwMemberCombinedIdx;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;
	pRequest->byIsKick = (BYTE)nIsKick;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallRelayClient::DoCheckOverallOnlineRequest(INT nPlayerId, INT nMapId, DWORD dwMemberCombinedIdx, DWORD dwLeaderCombinedIdx)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_CHECK_OVERALL_ONLINE_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_CHECK_OVERALL_ONLINE_REQUEST));
	pRequest = (S2R_CHECK_OVERALL_ONLINE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_check_online_leave_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->nMapId = nMapId;
	pRequest->dwMemberCombinedIdx = dwMemberCombinedIdx;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallRelayClient::DoMemberPrepareRequest(INT nMemberId, DWORD dwLeaderCombinedIdx, BYTE byPrepare)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_MEMBER_PREPARE_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_MEMBER_PREPARE_REQUEST));
	pRequest = (S2R_MEMBER_PREPARE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_member_prepare_request;
	pRequest->nMemberId = nMemberId;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;
	pRequest->byPrepare = byPrepare;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallRelayClient::DoGetPlayerTeamInfoRequest(INT nPlayerId, DWORD dwLeaderCombinedIdx)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_GET_PLAYER_TEAMINFO_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_GET_PLAYER_TEAMINFO_REQUEST));
	pRequest = (S2R_GET_PLAYER_TEAMINFO_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_get_player_teaminfo_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallRelayClient::DoStartMissionRequest(DWORD dwLeaderCombinedIdx)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_START_MISSION_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_START_MISSION_REQUEST));
	pRequest = (S2R_START_MISSION_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_start_mission_request;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallRelayClient::DoMemberCountDownRequest(DWORD dwLeaderCombinedIdx)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_MEMBER_COUNTDOWN_REQUEST*	pNotify = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_MEMBER_COUNTDOWN_REQUEST));
	pNotify = (S2R_MEMBER_COUNTDOWN_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID = s2r_member_countdown_request;
	pNotify->dwLeaderCombinedIdx = dwLeaderCombinedIdx;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallRelayClient::DoKickOverallPlayerRequest(DWORD dwLeaderCombinedIdx, DWORD dwMemberCombinedIdx)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_KICK_OVERALL_PLAYER_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_KICK_OVERALL_PLAYER_REQUEST));
	pRequest = (S2R_KICK_OVERALL_PLAYER_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_kick_overall_player_request;
	pRequest->dwMemberCombinedIdx = dwMemberCombinedIdx;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallRelayClient::DoBroadcastOverallteamMsg(INT nMapId, DWORD dwLeaderCombinedIdx, INT nRequireFightscore, LPCTSTR szCreaterName)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_BROADCAST_OVERALLTEAM_MSG*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_BROADCAST_OVERALLTEAM_MSG));
	pRequest = (S2R_BROADCAST_OVERALLTEAM_MSG*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_broadcast_overallteam_msg;
	pRequest->nMapId = nMapId;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;
	pRequest->nRequireFightscore = nRequireFightscore;
	strncpy(pRequest->szCreaterName, szCreaterName, MAX_NAME_LEN);

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

VOID KOverallRelayClient::OnSyncOverallTeamList(BYTE* pbyData, size_t uDataLen)
{
	R2S_SYNC_OVERALL_TEAM* pSync = NULL;
	pSync = (R2S_SYNC_OVERALL_TEAM*)pbyData;
	LOG_PROCESS_ERROR(pSync);

	g_OverallManager.OnSyncOverallTeam(pSync->nInstanceMapId, pSync->byTeamsNum, pSync->arrTeams);

EXIT0:
	return;
}

VOID KOverallRelayClient::OnSyncOverallTeamOpsResult(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	KInstanceCtrl* pInstanceCtrl = NULL;
	R2S_OVERALL_TEAM_OPS_RESULT* pSync = NULL;
	pSync = (R2S_OVERALL_TEAM_OPS_RESULT*)pbyData;
	LOG_PROCESS_ERROR(pSync);

	// TODO:告知客户端失败或者成功

	pPlayer = g_cPlayerMgr.GetById(pSync->nMemberId);
	PROCESS_ERROR(pPlayer);

	pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
	LOG_PROCESS_ERROR(pInstanceCtrl);

	pInstanceCtrl->DoSyncOverallTeamopResult(pSync->byTeamOps, pSync->byResult);

	if((pSync->byTeamOps == emOVERALL_LEAVETEAM_SELF || pSync->byTeamOps == emOVERALL_BE_KICKED || pSync->byTeamOps == emOVERALL_LEAVE_NO_TIPS
		|| pSync->byTeamOps == emOVERALL_TEAM_DISMISSED) && pSync->byResult == 1)
	{
		pPlayer->SetOverallTeamIdx(0);
	}

EXIT0:
	return;
}

VOID KOverallRelayClient::OnSynPlayerOverallTeam(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	KInstanceCtrl* pInstanceCtrl = NULL;
	R2S_SYNC_PLAYER_OVERALL_TEAM* pSync = NULL;
	pSync = (R2S_SYNC_PLAYER_OVERALL_TEAM*)pbyData;
	LOG_PROCESS_ERROR(pSync);

	pPlayer = g_cPlayerMgr.GetById(pSync->nPlayerId);
	PROCESS_ERROR(pPlayer);

	pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
	LOG_PROCESS_ERROR(pInstanceCtrl);

	pPlayer->SetOverallTeamIdx(pSync->dwLeaderCombinedIdx);
	pPlayer->SetMultiMapId(pSync->nMapId);

	for(INT i = 0; i < pSync->byMemberNum; ++i)
	{
		if(strncmp(pPlayer->GetName(), pSync->arrMembers[i].szMemberName, MAX_NAME_LEN) == 0)
		{
			pInstanceCtrl->m_dwOverallCombinedIdx = pSync->arrMembers[i].dwMemberCombinedIdx;
		}
	}
	
	pInstanceCtrl->SyncPlayerOverallTeam(pSync->dwLeaderCombinedIdx, pSync->byMemberNum, pSync->nRequireFightscore, pSync->arrMembers);

EXIT0:
	return;
}

VOID KOverallRelayClient::OnMemberCountDownNotify(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	KInstanceCtrl* pInstanceCtrl = NULL;
	R2S_MEMBER_COUNTDOWN_NOTIFY* pNotify = NULL;
	pNotify = (R2S_MEMBER_COUNTDOWN_NOTIFY*)pbyData;
	LOG_PROCESS_ERROR(pNotify);

	pPlayer = g_cPlayerMgr.GetById(pNotify->nPlayerId);
	PROCESS_ERROR(pPlayer);

	pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
	LOG_PROCESS_ERROR(pInstanceCtrl);

	pInstanceCtrl->DoMemberCountDownNotify();

EXIT0:
	return;
}

VOID KOverallRelayClient::OnCheckMemberOnlineRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_CHECK_MEMBER_ONLINE_RESPOND* pRespond = NULL;
	
	pRespond = (R2S_CHECK_MEMBER_ONLINE_RESPOND*)pbyData;
	LOG_PROCESS_ERROR(pRespond);

	if(!pRespond->byOnline)
	{
		KOverallRelayClient::DoLeaveOverallTeamRequest(pRespond->dwMemberCombinedIdx, pRespond->dwLeaderCombinedIdx, 0);
	}
	else
	{
		KPlayer* pPlayer = g_cPlayerMgr.GetById(pRespond->nPlayerId);
		PROCESS_ERROR(pPlayer);
		pPlayer->SetMultiMapId(pRespond->nMapId);
		pPlayer->SetOverallTeamIdx(pRespond->dwLeaderCombinedIdx);
	}

EXIT0:
	return;
}

VOID KOverallRelayClient::OnMultiTransRequest(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	KInstanceCtrl* pInstanceCtrl = NULL;
	R2S_MULTI_TRANS_REQUEST* pRequest = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	pRequest = (R2S_MULTI_TRANS_REQUEST*)pbyData;
	LOG_PROCESS_ERROR(pRequest);

	for(INT i = 0; i < 3; ++i)
	{
		pPlayer = g_cPlayerMgr.GetById(pRequest->arrMembers[i]);
		if(pPlayer)
		{
			cSafeScript->CallTableFunction("MultiInstance", "DoEnterInstance", 0, "od", pPlayer->GetScriptInterface(), pRequest->nMapId);
			//cSafeScript->CallTableFunction("MultiInstance", "PrepareToTransfer", 0, "od", pPlayer->GetScriptInterface(), pRequest->nMapId);
		}
	}

EXIT0:
	return;
}