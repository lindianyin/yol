#include "stdafx.h"
#include "kqueuehandler.h"
#include "GameCenter.h"

BOOL KOverallQueueHandler::DoSyncOverallTeams(INT nInstanceMapId, LIST_SLAVE_TEAMS& listSlaveTeams)
{
	BOOL bResult	 = FALSE;
	R2S_SYNC_OVERALL_TEAM*	pSync = NULL;
	IMemBlock* piPackage = NULL;
	INT	nSize = listSlaveTeams.size();;
	INT nIdx = 0;

	piPackage = QCreateMemBlock(sizeof(R2S_SYNC_OVERALL_TEAM) + sizeof(KTEAM_INFO) * nSize);
	pSync = (R2S_SYNC_OVERALL_TEAM*)piPackage->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->wProtocolID = r2s_sync_overall_teams;
	pSync->nInstanceMapId = nInstanceMapId;
	pSync->byTeamsNum = (BYTE)nSize;
	for(LIST_SLAVE_TEAMS::iterator it = listSlaveTeams.begin(); it != listSlaveTeams.end(); ++it)
	{
		strncpy(pSync->arrTeams[nIdx].szCreaterName, it->szCreaterName, MAX_NAME_LEN);
		pSync->arrTeams[nIdx].dwLeaderCombinedIdx = it->dwCombinedIdx;
		pSync->arrTeams[nIdx].byCurrentMemberNum = it->byCurrentMemberNum;
		pSync->arrTeams[nIdx].nRequireFightscore = it->nRequireFightscore;
		++nIdx;
	}

	g_pGameCenter->m_GameServer.Broadcast(piPackage);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallQueueHandler::DoSyncTeamOpsResult(INT nMemberId, BYTE byTeamOps, BYTE byResult)
{
	BOOL bResult	 = FALSE;
	R2S_OVERALL_TEAM_OPS_RESULT*	pResult = NULL;
	IMemBlock* piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_OVERALL_TEAM_OPS_RESULT));
	pResult = (R2S_OVERALL_TEAM_OPS_RESULT*)piPackage->GetData();
	LOG_PROCESS_ERROR(pResult);

	pResult->wProtocolID = r2s_sync_overall_teamops_result;
	pResult->nMemberId = nMemberId;
	pResult->byTeamOps = byTeamOps;
	pResult->byResult = byResult;

	g_pGameCenter->m_GameServer.Broadcast(piPackage);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallQueueHandler::DoSyncPlayerOverallTeam(INT nPlayerId, INT nMemberNum, DWORD dwLeaderCombinedIdx, INT nMapId, INT nRequireFightscore, KSYNC_MEMBER_INFO* arrMembers)
{
	BOOL bResult	 = FALSE;
	R2S_SYNC_PLAYER_OVERALL_TEAM*	pSync = NULL;
	IMemBlock* piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_SYNC_PLAYER_OVERALL_TEAM) + nMemberNum * sizeof(KRS_MEMBER_INFO));
	pSync = (R2S_SYNC_PLAYER_OVERALL_TEAM*)piPackage->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->wProtocolID = r2s_sync_player_overall_team;
	pSync->nPlayerId = nPlayerId;
	pSync->dwLeaderCombinedIdx = dwLeaderCombinedIdx;
	pSync->nMapId = nMapId;
	pSync->byMemberNum = (BYTE)nMemberNum;
	pSync->nRequireFightscore = nRequireFightscore;
	memcpy(pSync->arrMembers, arrMembers, sizeof(KRS_MEMBER_INFO) * nMemberNum);

	g_pGameCenter->m_GameServer.Broadcast(piPackage);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallQueueHandler::DoMemberCountDownNotify(INT nPlayerId)
{
	BOOL bResult	 = FALSE;
	R2S_MEMBER_COUNTDOWN_NOTIFY*	pNotify = NULL;
	IMemBlock* piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_MEMBER_COUNTDOWN_NOTIFY));
	pNotify = (R2S_MEMBER_COUNTDOWN_NOTIFY*)piPackage->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID = r2s_member_countdown_notify;
	pNotify->nPlayerId = nPlayerId;

	g_pGameCenter->m_GameServer.Broadcast(piPackage);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KOverallQueueHandler::DoMultiTransRequest(INT nMapId, INT nMapCopyIdx, DWORD* arrMembers)
{
	BOOL bResult	 = FALSE;
	R2S_MULTI_TRANS_REQUEST*	pResult = NULL;
	IMemBlock* piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_MULTI_TRANS_REQUEST));
	pResult = (R2S_MULTI_TRANS_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pResult);

	pResult->wProtocolID = r2s_multi_trans_request;
	pResult->nMapId = nMapId;
	pResult->nMapCopyIdx = nMapCopyIdx;
	for(INT i = 0; i < 3; ++i)
	{
		pResult->arrMembers[i] = GET_COMBINED_MEMBER_ID(arrMembers[i]);
	}

	g_pGameCenter->m_GameServer.Broadcast(piPackage);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

VOID KOverallQueueHandler::OnCreateOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_CREATE_OVERALL_TEAM_REQUEST*	pRequest	= (S2R_CREATE_OVERALL_TEAM_REQUEST*)pbyData;
	KRole* pRole = NULL;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnCreateOverallTeamRequest nPlayerId=%d nInstanceMapId=%d nRequireFightscore=%d nConnIndex=%d",
		pRequest->nPlayerId, pRequest->nInstanceMapId, pRequest->nRequireFightscore, nConnIndex);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	bRetCode = g_QueueSlave.DoCreateOverallTeamRequest(pRequest->nPlayerId, pRole->getName(), pRole->GetLevel(), pRole->GetSex(), pRole->m_nFightScore, pRequest->nInstanceMapId, pRequest->nRequireFightscore);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

VOID KOverallQueueHandler::OnJoinOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_JOIN_OVERALL_TEAM_REQUEST*	pRequest	= (S2R_JOIN_OVERALL_TEAM_REQUEST*)pbyData;
	KRole* pRole = NULL;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnJoinOverallTeamRequest nMemberId=%d nInstanceMapId=%d dwCombinedIdx=%d nConnIndex=%d",
		pRequest->nMemberId, pRequest->nInstanceMapId, pRequest->dwCombinedIdx, nConnIndex);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->nMemberId);
	LOG_PROCESS_ERROR(pRole);


	bRetCode = g_QueueSlave.DoJoinOverallTeamRequest(pRequest->nMemberId, pRole->getName(), pRole->GetLevel(), pRole->GetSex(), pRole->m_nFightScore, pRequest->nInstanceMapId, pRequest->dwCombinedIdx);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

VOID KOverallQueueHandler::OnLeaveOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_LEAVE_OVERALL_TEAM_REQUEST*	pRequest	= (S2R_LEAVE_OVERALL_TEAM_REQUEST*)pbyData;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnLeaveOverallTeamRequest dwMemberCombinedIdx=%d dwLeaderCombinedIdx=%d byIsKick=%d nConnIndex=%d",
		pRequest->dwMemberCombinedIdx, pRequest->dwLeaderCombinedIdx, (int)pRequest->byIsKick, nConnIndex);

	bRetCode = g_QueueSlave.DoLeaveOverallTeamRequest(pRequest->dwMemberCombinedIdx, pRequest->dwLeaderCombinedIdx, pRequest->byIsKick);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

VOID KOverallQueueHandler::OnMemberPrepareRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_MEMBER_PREPARE_REQUEST*	pRequest	= (S2R_MEMBER_PREPARE_REQUEST*)pbyData;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnMemberPrepareRequest nMemberId=%d dwLeaderCombinedIdx=%d byPrepare=%d nConnIndex=%d",
		pRequest->nMemberId, pRequest->dwLeaderCombinedIdx, (int)pRequest->byPrepare, nConnIndex);

	bRetCode = g_QueueSlave.DoMemberPrepareRequest(pRequest->nMemberId, pRequest->dwLeaderCombinedIdx, pRequest->byPrepare);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

VOID KOverallQueueHandler::OnGetPlayerTeamInfoRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_GET_PLAYER_TEAMINFO_REQUEST*	pRequest	= (S2R_GET_PLAYER_TEAMINFO_REQUEST*)pbyData;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnGetPlayerTeamInfoRequest nPlayerId=%d dwLeaderCombinedIdx=%d nConnIndex=%d",
		pRequest->nPlayerId, pRequest->dwLeaderCombinedIdx, nConnIndex);

	bRetCode = g_QueueSlave.DoGetPlayerOverallTeamRequest(pRequest->nPlayerId, pRequest->dwLeaderCombinedIdx);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

VOID KOverallQueueHandler::OnStartMissionRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_START_MISSION_REQUEST*	pRequest	= (S2R_START_MISSION_REQUEST*)pbyData;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnStartMissionRequest dwLeaderCombinedIdx=%d nConnIndex=%d",
		pRequest->dwLeaderCombinedIdx, nConnIndex);

	bRetCode = g_QueueSlave.DoStartMissionRequest(pRequest->dwLeaderCombinedIdx);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

VOID KOverallQueueHandler::OnMemberCountDownRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_MEMBER_COUNTDOWN_REQUEST*	pRequest	= (S2R_MEMBER_COUNTDOWN_REQUEST*)pbyData;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnMemberCountDownRequest dwLeaderCombinedIdx=%d nConnIndex=%d",
		pRequest->dwLeaderCombinedIdx, nConnIndex);

	bRetCode = g_QueueSlave.DoMemberCountDownRequest(pRequest->dwLeaderCombinedIdx);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

VOID KOverallQueueHandler::OnKickOverllPlayerRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_KICK_OVERALL_PLAYER_REQUEST*	pRequest	= (S2R_KICK_OVERALL_PLAYER_REQUEST*)pbyData;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnKickOverllPlayerRequest dwLeaderCombinedIdx=%d dwMemberCombinedIdx=%d,nConnIndex=%d",
		pRequest->dwLeaderCombinedIdx, pRequest->dwMemberCombinedIdx, nConnIndex);

	bRetCode = g_QueueSlave.DoKickOverallPlayerRequest(pRequest->dwLeaderCombinedIdx, pRequest->dwLeaderCombinedIdx);
	LOG_PROCESS_ERROR(bRetCode);
EXIT0:
	return;
}

VOID KOverallQueueHandler::OnBroadcastOverallteamMsg(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_BROADCAST_OVERALLTEAM_MSG*	pRequest	= (S2R_BROADCAST_OVERALLTEAM_MSG*)pbyData;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnBroadcastOverallteamMsg nMapId=%d dwLeaderCombinedIdx=%d nRequireFightscore=%d szCreaterName=%s nConnIndex=%d",
		pRequest->nMapId, pRequest->dwLeaderCombinedIdx, pRequest->nRequireFightscore, pRequest->szCreaterName, nConnIndex);

	bRetCode = g_QueueSlave.DoBroadcastOverallTeamMsg(pRequest->nMapId, pRequest->dwLeaderCombinedIdx, pRequest->nRequireFightscore, pRequest->szCreaterName);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

VOID KOverallQueueHandler::OnCheckOnlineLeaveRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	S2R_CHECK_OVERALL_ONLINE_REQUEST*	pRequest	= (S2R_CHECK_OVERALL_ONLINE_REQUEST*)pbyData;
	R2S_CHECK_MEMBER_ONLINE_RESPOND* pRespond = NULL;
	IMemBlock* piPackage = NULL;
	KRole* pRole = NULL;

	QLogPrintf(LOG_INFO, "KOverallQueueHandler::OnCheckOnlineLeaveRequest nPlayerId=%d nMapId=%d dwMemberCombinedIdx=%d dwLeaderCombinedIdx=%d nConnIndex=%d",
		pRequest->nPlayerId, pRequest->nMapId, pRequest->dwMemberCombinedIdx, pRequest->dwLeaderCombinedIdx, nConnIndex);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	piPackage = QCreateMemBlock(sizeof(R2S_CHECK_MEMBER_ONLINE_RESPOND));
	pRespond = (R2S_CHECK_MEMBER_ONLINE_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_check_member_online_respond;
	pRespond->nPlayerId = pRequest->nPlayerId;
	pRespond->nMapId = pRequest->nMapId;
	pRespond->dwMemberCombinedIdx = pRequest->dwMemberCombinedIdx;
	pRespond->dwLeaderCombinedIdx = pRequest->dwLeaderCombinedIdx;
	pRespond->byOnline = (BYTE)pRole->IsOnline();

	g_pGameCenter->m_GameServer.Broadcast(piPackage);

EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}

BOOL KOverallQueueHandler::Send( int nConnIndex, IMemBlock* piBuffer )
{
	return g_pGameCenter->m_GameServer.Send(nConnIndex, piBuffer);
}