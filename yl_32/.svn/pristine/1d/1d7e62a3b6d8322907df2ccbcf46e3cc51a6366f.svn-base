#include "stdafx.h"
#include "kqueueslave.h"
#include "GameCenter.h"
#include "globalserverslave.h"
#include "kqueuehandler.h"
#include "onlinegamebase/kscriptmanager.h"

KQueueSlave g_QueueSlave;

KQueueSlave::KQueueSlave(void)
: GlobalServerSlaveProtocolHandler<KQueueSlave>(m2r_overall_queue_protocol_begin, m2r_overall_queue_protocol_end)
{

}

KQueueSlave::~KQueueSlave(void)
{
}


BOOL KQueueSlave::Init()
{
    g_GlobalServerSlave.RegisterProtocolHandler(this);

	RegisterProtocolProcessFunction(m2r_sync_multi_teams,  &KQueueSlave::OnSyncMultiTeams, sizeof(M2R_SYNC_MULTI_TEAMS));
	RegisterProtocolProcessFunction(m2r_sync_teamops_result,  &KQueueSlave::OnSyncTeamOpsResult, sizeof(M2R_SYNC_OVERALLTEAM_RESULT));
	RegisterProtocolProcessFunction(m2r_sync_player_overall_team,  &KQueueSlave::OnSyncPlayerOverallTeam, sizeof(M2R_SYNC_PLAYER_OVERALL_TEAM));
	RegisterProtocolProcessFunction(m2r_do_start_mission,  &KQueueSlave::OnStartMissionRequest, sizeof(M2R_DO_START_MISSION));
	RegisterProtocolProcessFunction(m2r_member_countdown_notify,  &KQueueSlave::OnMemberCountDownNotify, sizeof(M2R_MEMBER_COUNT_DOWN_NOTIFY));
	RegisterProtocolProcessFunction(m2r_broadcast_overallteam_msg,  &KQueueSlave::OnBroadcastOverallTeamMsg, sizeof(M2R_BROADCAST_OVERALLTEAM_MSG));
	RegisterProtocolProcessFunction(m2r_do_start_mission_remote,  &KQueueSlave::OnStartMissionRemoteRequest, sizeof(M2R_DO_START_MISSION_REMOTE));

	return TRUE;
}

BOOL KQueueSlave::UnInit()
{
	return TRUE;
}

VOID KQueueSlave::Activate()
{

}

BOOL KQueueSlave::DoCreateOverallTeamRequest(INT nPlayerId, LPCTSTR szPlayerName, BYTE byPlayerLevel, BYTE byPlayerSex, INT nFightScore, INT nMapId, INT nRequireFightScore)
{
	BOOL								bResult     = false;
	BOOL								bRetCode    = false;
	IMemBlock*							piPackage   = NULL;
	R2M_OVERALL_CREATE_TEAM_REQUEST*    pRequest    = NULL;
	INT nConnIndex									= 0;

	piPackage = QCreateMemBlock(sizeof(R2M_OVERALL_CREATE_TEAM_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (R2M_OVERALL_CREATE_TEAM_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID        = r2m_overall_create_team_request;
	pRequest->nCreaterId = nPlayerId;
	pRequest->nInstanceMapId = nMapId;
	pRequest->nServerId = 1;		// 先暂定为1，以后每个服一个编号
	pRequest->nRequireFightScore = nRequireFightScore;
	strncpy(pRequest->szPlayerName, szPlayerName, MAX_NAME_LEN);
	pRequest->byCreaterLevel = byPlayerLevel;
	pRequest->byCreaterSex = byPlayerSex;
	pRequest->nCreaterFightScore = nFightScore;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueSlave::DoJoinOverallTeamRequest(INT nMemberId, LPCTSTR szMemberName, BYTE byMemberLevel, BYTE byMemberSex, INT nFightScore, INT nInstanceMapId, DWORD dwCombinedIdx)
{
	BOOL								bResult     = false;
	BOOL								bRetCode    = false;
	IMemBlock*							piPackage   = NULL;
	R2M_OVERALL_JOIN_TEAM_REQUEST*    pRequest    = NULL;

	piPackage = QCreateMemBlock(sizeof(R2M_OVERALL_JOIN_TEAM_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (R2M_OVERALL_JOIN_TEAM_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID        = r2m_overall_join_team_request;
	pRequest->nMemberId = nMemberId;
	strncpy(pRequest->szMemberName, szMemberName, MAX_NAME_LEN);
	pRequest->byMemberLevel = byMemberLevel;
	pRequest->byMemberSex = byMemberSex;
	pRequest->nMemberFightScore = nFightScore;
	pRequest->nInstanceMapId = nInstanceMapId;
	pRequest->dwCombinedIdx = dwCombinedIdx;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueSlave::DoLeaveOverallTeamRequest(DWORD dwMemberCombinedIdx, DWORD dwLeaderCombinedIdx, BYTE byIsKick)
{
	BOOL								bResult     = false;
	BOOL								bRetCode    = false;
	IMemBlock*							piPackage   = NULL;
	R2M_OVERALL_LEAVE_TEAM_REQUEST*    pRequest    = NULL;

	piPackage = QCreateMemBlock(sizeof(R2M_OVERALL_LEAVE_TEAM_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (R2M_OVERALL_LEAVE_TEAM_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID        = r2m_overall_leave_team_request;
	pRequest->dwMemberCombinedIdx = dwMemberCombinedIdx;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;
	pRequest->byIsKick = byIsKick;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueSlave::DoMemberPrepareRequest(INT nMemberId, DWORD dwLeaderCombinedIdx, BYTE byPrepare)
{
	BOOL								bResult     = false;
	BOOL								bRetCode    = false;
	IMemBlock*							piPackage   = NULL;
	R2M_OVERALL_PREPARE_REQUEST*    pRequest    = NULL;

	piPackage = QCreateMemBlock(sizeof(R2M_OVERALL_PREPARE_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (R2M_OVERALL_PREPARE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID        = r2m_overall_team_prepare_request;
	pRequest->nMemberId = nMemberId;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;
	pRequest->byPrepare = byPrepare;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueSlave::DoGetPlayerOverallTeamRequest(INT nPlayerId, DWORD dwLeaderCombinedIdx)
{
	BOOL								bResult     = false;
	BOOL								bRetCode    = false;
	IMemBlock*							piPackage   = NULL;
	R2M_GET_PLAYER_OVERALLTEAM_REQUEST*    pRequest    = NULL;

	piPackage = QCreateMemBlock(sizeof(R2M_GET_PLAYER_OVERALLTEAM_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (R2M_GET_PLAYER_OVERALLTEAM_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID        = r2m_get_player_overallteam_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueSlave::DoStartMissionRequest(DWORD dwLeaderCombinedIdx)
{
	BOOL								bResult     = false;
	BOOL								bRetCode    = false;
	IMemBlock*							piPackage   = NULL;
	R2M_START_MISSION_REQUEST*    pRequest    = NULL;

	piPackage = QCreateMemBlock(sizeof(R2M_START_MISSION_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (R2M_START_MISSION_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID        = r2m_start_mission_request;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueSlave::DoMemberCountDownRequest(DWORD dwLeaderCombinedIdx)
{
	BOOL								bResult     = false;
	BOOL								bRetCode    = false;
	IMemBlock*							piPackage   = NULL;
	R2M_MEMBER_COUNTDOWN_REQUEST*    pNotify    = NULL;

	piPackage = QCreateMemBlock(sizeof(R2M_MEMBER_COUNTDOWN_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pNotify = (R2M_MEMBER_COUNTDOWN_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID        = r2m_member_countdown_request;
	pNotify->dwLeaderCombinedIdx = dwLeaderCombinedIdx;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueSlave::DoKickOverallPlayerRequest(DWORD dwLeaderCombinedIdx, DWORD dwMemberCombinedIdx)
{
	BOOL								bResult     = false;
	BOOL								bRetCode    = false;
	IMemBlock*							piPackage   = NULL;
	R2M_KICK_OVERALL_PLAYER_REQUEST*    pRequest    = NULL;

	piPackage = QCreateMemBlock(sizeof(R2M_KICK_OVERALL_PLAYER_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (R2M_KICK_OVERALL_PLAYER_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID        = r2m_kick_player_request;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;
	pRequest->dwMemberCombinedIdx = dwMemberCombinedIdx;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueSlave::DoBroadcastOverallTeamMsg(INT nMapId, DWORD dwLeaderCombinedIdx, INT nRequireFightscore, LPCTSTR szCreaterName)
{
	/*
	BOOL								bResult     = false;
	BOOL								bRetCode    = false;
	IMemBlock*							piPackage   = NULL;
	R2M_BROADCAST_OVERALLTEAM_MSG*    pRequest    = NULL;

	piPackage = QCreateMemBlock(sizeof(R2M_BROADCAST_OVERALLTEAM_MSG));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (R2M_BROADCAST_OVERALLTEAM_MSG*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID        = r2m_broadcast_overallteam_msg;
	pRequest->nMapId = nMapId;
	pRequest->dwLeaderCombinedIdx = dwLeaderCombinedIdx;
	strncpy(pRequest->szCreaterName, szCreaterName, MAX_NAME_LEN);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
	*/

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction("PVEMatch", "BroadcastOverallTeamMsg", 0, "ddds", nMapId, dwLeaderCombinedIdx, nRequireFightscore, szCreaterName);

	return TRUE;
}

BOOL KQueueSlave::Send(IMemBlock* piBuffer)
{
	return g_GlobalServerSlave.Send(piBuffer);
}

VOID KQueueSlave::GetSlaveMultiTeamInfo(INT nInstanceMapId, LIST_SLAVE_TEAMS& listTeams)
{
	MAP_SLAVE_TEAMS::iterator it = m_mapSlaveTeams.find(nInstanceMapId);
	if(it == m_mapSlaveTeams.end())
	{
		LIST_SLAVE_TEAMS listSlaveTeam;
		m_mapSlaveTeams[nInstanceMapId] = listSlaveTeam;
		it = m_mapSlaveTeams.find(nInstanceMapId);
	}
	listTeams = m_mapSlaveTeams[nInstanceMapId];
}

BOOL KQueueSlave::OnSyncMultiTeams(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bResult     = FALSE;
	BOOL                            bRetCode    = FALSE;
	IMemBlock*                     piPackage   = NULL;
	M2R_SYNC_MULTI_TEAMS*  pSync		= (M2R_SYNC_MULTI_TEAMS*)pbyData;
	INT nInstanceMapId = pSync->nInstanceMapId;
	
	MAP_SLAVE_TEAMS::iterator it = m_mapSlaveTeams.find(nInstanceMapId);
	if(it == m_mapSlaveTeams.end())
	{
		LIST_SLAVE_TEAMS listTeams;
		m_mapSlaveTeams[nInstanceMapId] = listTeams;
		it = m_mapSlaveTeams.find(nInstanceMapId);
	}

	{
		LIST_SLAVE_TEAMS& listSlaveTeams = it->second;

		listSlaveTeams.clear();

		for(INT i = 0; i < pSync->byTotalTeamNum; ++i)
		{
			KSLAVE_TEAM_INFO info;

			strncpy(info.szCreaterName, pSync->arrTeams[i].szCreaterName, MAX_NAME_LEN);
			info.nRequireFightscore = pSync->arrTeams[i].nRequireFightscore;
			info.dwCombinedIdx = pSync->arrTeams[i].dwCombinedIdx;
			info.byCurrentMemberNum = pSync->arrTeams[i].byCurrentNum;

			listSlaveTeams.push_back(info);
		}

		// 向所有gs同步排队信息
		KOverallQueueHandler::DoSyncOverallTeams(nInstanceMapId, listSlaveTeams);
	}

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueSlave::OnSyncTeamOpsResult(BYTE* pbyData, size_t uDataLen)
{
	BOOL bResult     = FALSE;
	M2R_SYNC_OVERALLTEAM_RESULT*  pSync		= (M2R_SYNC_OVERALLTEAM_RESULT*)pbyData;

	KOverallQueueHandler::DoSyncTeamOpsResult(pSync->nMemberId, pSync->byTeamOps, pSync->bResult);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueSlave::OnSyncPlayerOverallTeam(BYTE* pbyData, size_t uDataLen)
{
	BOOL bResult     = FALSE;
	M2R_SYNC_PLAYER_OVERALL_TEAM*  pSync		= (M2R_SYNC_PLAYER_OVERALL_TEAM*)pbyData;

	KOverallQueueHandler::DoSyncPlayerOverallTeam(pSync->nPlayerId, pSync->byMemberNum, pSync->dwLeaderCombinedIdx, pSync->nMapId, pSync->nRequireFightscore, pSync->arrMembers);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueSlave::OnMemberCountDownNotify(BYTE* pbyData, size_t uDataLen)
{
	BOOL bResult     = FALSE;
	M2R_MEMBER_COUNT_DOWN_NOTIFY*  pSync		= (M2R_MEMBER_COUNT_DOWN_NOTIFY*)pbyData;

	KOverallQueueHandler::DoMemberCountDownNotify(pSync->nPlayerId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueSlave::OnStartMissionRequest(BYTE* pbyData, size_t uDataLen)
{
	BOOL bResult     = FALSE;
	M2R_DO_START_MISSION*  pSync		= (M2R_DO_START_MISSION*)pbyData;

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction("PVEMatch", "TestEnterMission", 0, "dddd", GET_COMBINED_MEMBER_ID(pSync->arrMembers[0]), GET_COMBINED_MEMBER_ID(pSync->arrMembers[1]), GET_COMBINED_MEMBER_ID(pSync->arrMembers[2]), pSync->nInstanceMapId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueSlave::OnBroadcastOverallTeamMsg(BYTE* pbyData, size_t uDataLen)
{
	BOOL bResult     = FALSE;
	M2R_BROADCAST_OVERALLTEAM_MSG*  pSync		= (M2R_BROADCAST_OVERALLTEAM_MSG*)pbyData;

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction("PVEMatch", "BroadcastOverallTeamMsg", 0, "dds", pSync->nMapId, pSync->dwLeaderCombinedIdx, pSync->szCreaterName);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueSlave::OnStartMissionRemoteRequest(BYTE* pbyData, size_t uDataLen)
{
	BOOL bResult     = FALSE;
	M2R_DO_START_MISSION_REMOTE*  pSync		= (M2R_DO_START_MISSION_REMOTE*)pbyData;

	KOverallQueueHandler::DoMultiTransRequest(pSync->nInstanceMapId, pSync->nInstanceMapId, pSync->arrMembers);

	bResult = TRUE;
EXIT0:
	return bResult;
}