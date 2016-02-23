#include "stdafx.h"
#include "kqueuemaster.h"
#include "GameCenter.h"
#include "globalservermaster.h"

KQueueMaster g_QueueMaster;

KQueueMaster::KQueueMaster(void)
: GlobalServerMasterProtocolHandler<KQueueMaster>(r2m_overall_queue_protocol_begin, r2m_overall_queue_protocol_end)
{

}

KQueueMaster::~KQueueMaster(void)
{
}


BOOL KQueueMaster::Init()
{
	m_mapJoinIndex.clear();
	m_mapAllTeams.clear();
	m_mapCrossSvInfos.clear();

    g_GlobalServerMaster.RegisterProtocolHandler(this);

	RegisterProtocolProcessFunction(r2m_overall_create_team_request,  &KQueueMaster::OnCreateOverallTeamRequest, sizeof(R2M_OVERALL_CREATE_TEAM_REQUEST));
	RegisterProtocolProcessFunction(r2m_overall_join_team_request,  &KQueueMaster::OnJoinOverallTeamRequest, sizeof(R2M_OVERALL_JOIN_TEAM_REQUEST));
	RegisterProtocolProcessFunction(r2m_overall_leave_team_request,  &KQueueMaster::OnLeaveOverallTeamRequest, sizeof(R2M_OVERALL_LEAVE_TEAM_REQUEST));
	RegisterProtocolProcessFunction(r2m_overall_team_prepare_request,  &KQueueMaster::OnMemberPrepareRequest, sizeof(R2M_OVERALL_PREPARE_REQUEST));
	RegisterProtocolProcessFunction(r2m_get_player_overallteam_request,  &KQueueMaster::OnSyncPlayerOverallTeamRequest, sizeof(R2M_GET_PLAYER_OVERALLTEAM_REQUEST));
	RegisterProtocolProcessFunction(r2m_kick_player_request,  &KQueueMaster::OnKickOverallPlayerRequest, sizeof(R2M_KICK_OVERALL_PLAYER_REQUEST));
	RegisterProtocolProcessFunction(r2m_start_mission_request,  &KQueueMaster::OnStartMissionRequest, sizeof(R2M_START_MISSION_REQUEST));
	RegisterProtocolProcessFunction(r2m_member_countdown_request,  &KQueueMaster::OnMemberCountDownRequest, sizeof(R2M_MEMBER_COUNTDOWN_REQUEST));
	RegisterProtocolProcessFunction(r2m_broadcast_overallteam_msg,  &KQueueMaster::OnBroadcastOverallTeamMsg, sizeof(R2M_BROADCAST_OVERALLTEAM_MSG));

	return TRUE;
}

BOOL KQueueMaster::UnInit()
{
	m_mapJoinIndex.clear();
	m_mapAllTeams.clear();
	m_mapCrossSvInfos.clear();

	return TRUE;
}

VOID KQueueMaster::Activate()
{

}

BOOL KQueueMaster::Send(int nConnIndex, IMemBlock* piBuffer)
{
	BOOL bResult = FALSE;

	bResult = g_GlobalServerMaster.Send(nConnIndex, piBuffer);
EXIT0:
	return bResult;
}

VOID KQueueMaster::GetMultiMapInfo(INT nInstanceMapId, MAP_MULTI_TEAM_INFO& mapTeamInfo)
{
	MAP_OVERALL_TEAMS::iterator it = m_mapAllTeams.find(nInstanceMapId);
	if(it == m_mapAllTeams.end())
	{
		MAP_MULTI_TEAM_INFO mapTeams;
		m_mapAllTeams[nInstanceMapId] = mapTeams;
		it = m_mapAllTeams.find(nInstanceMapId);
	}

	mapTeamInfo = it->second;
}

INT KQueueMaster::GetInstanceMapCopyIdx(INT nConnIdx, DWORD dwRoleId)
{
	INT nMapCopyIdx = 0;

	std::map<DWORD, KCROSS_SV_ROLEINFO>::iterator it = m_mapCrossSvInfos.find(DYNAMIC_TEAM_INDEX(dwRoleId, nConnIdx));
	LOG_PROCESS_ERROR(it != m_mapCrossSvInfos.end());

	{
		KCROSS_SV_ROLEINFO& info = it->second;
		nMapCopyIdx = info.nInstanceMapCopyIdx;
	}

EXIT0:
	return nMapCopyIdx;
}

DWORD KQueueMaster::GetInstanceTeamId(INT nConnIdx, DWORD dwRoleId)
{
	DWORD dwTeamId = 0;

	std::map<DWORD, KCROSS_SV_ROLEINFO>::iterator it = m_mapCrossSvInfos.find(DYNAMIC_TEAM_INDEX(dwRoleId, nConnIdx));
	LOG_PROCESS_ERROR(it != m_mapCrossSvInfos.end());

	{
		KCROSS_SV_ROLEINFO& info = it->second;
		dwTeamId = info.dwTeamId;
	}
EXIT0:
	return dwTeamId;
}

INT KQueueMaster::GetInstanceMapId(INT nConnIdx, DWORD dwRoleId)
{
	INT nMapId = 0;

	std::map<DWORD, KCROSS_SV_ROLEINFO>::iterator it = m_mapCrossSvInfos.find(DYNAMIC_TEAM_INDEX(dwRoleId, nConnIdx));
	LOG_PROCESS_ERROR(it != m_mapCrossSvInfos.end());

	{
		KCROSS_SV_ROLEINFO& info = it->second;
		nMapId = info.nInstanceMapId;
	}

EXIT0:
	return nMapId;
}

BOOL KQueueMaster::OnCreateOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	INT nCombinedIdx = 0;
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	KOVERALL_TEAM_INFO sTeamInfo;
	KOVERALL_MEMBER sMember;
	MAP_MULTI_TEAM_INFO::iterator it;
	std::map<DWORD, INT>::iterator it_idx;
	R2M_OVERALL_CREATE_TEAM_REQUEST* pRequest = (R2M_OVERALL_CREATE_TEAM_REQUEST*)pbyData;
	INT nMapId = pRequest->nInstanceMapId;

	QLogPrintf(LOG_INFO, "KQueueMaster::OnCreateOverallTeamRequest nServerId=%d nInstanceMapId=%d nCreaterId=%d szPlayerName=%s byCreaterLevel=%d byCreaterSex=%d nCreaterFightScore=%d nRequireFightScore=%d nConnIndex=%d\n",
		pRequest->nServerId, pRequest->nInstanceMapId, pRequest->nCreaterId, pRequest->szPlayerName, (int)pRequest->byCreaterLevel, (int)pRequest->byCreaterSex, pRequest->nCreaterFightScore, pRequest->nRequireFightScore, nConnIndex);

	MAP_OVERALL_TEAMS::iterator it_overall = m_mapAllTeams.find(nMapId);
	if(it_overall == m_mapAllTeams.end())
	{
		MAP_MULTI_TEAM_INFO mapMultiTeamInfo;
		m_mapAllTeams[nMapId] = mapMultiTeamInfo;
		it_overall = m_mapAllTeams.find(nMapId);
	}

	LOG_PROCESS_ERROR(it_overall != m_mapAllTeams.end());

	{
		MAP_MULTI_TEAM_INFO& mapTeams = it_overall->second;
		nCombinedIdx = DYNAMIC_TEAM_INDEX(pRequest->nCreaterId, nConnIndex);

		it_idx = m_mapJoinIndex.find(nCombinedIdx);
		// 已经创建过队伍的不能重复创建
		LOG_PROCESS_ERROR(it_idx == m_mapJoinIndex.end());

		it = mapTeams.find(nCombinedIdx);
		if(it == mapTeams.end())
		{
			strncpy(sTeamInfo.szCreaterName, pRequest->szPlayerName, MAX_NAME_LEN);
			sTeamInfo.nRequireFightscore = pRequest->nRequireFightScore;
			sTeamInfo.dwLeaderCombinedIdx = nCombinedIdx;
			sTeamInfo.byStarting = 0;

			sMember.nConnectIdx = nConnIndex;
			sMember.nMemberId = pRequest->nCreaterId;
			memcpy(sMember.szMemberName, pRequest->szPlayerName, MAX_NAME_LEN);
			sMember.byLevel = pRequest->byCreaterLevel;
			sMember.bySex = pRequest->byCreaterSex;
			sMember.nFightScore = pRequest->nCreaterFightScore;
			sMember.byPrepare = 1;

			sTeamInfo.listMembers.push_back(sMember);

			mapTeams[nCombinedIdx] = sTeamInfo;

			m_mapJoinIndex[nCombinedIdx] = nMapId;

			bRetCode = DoSyncPlayerOverallTeam(nConnIndex, pRequest->nCreaterId, nCombinedIdx, nMapId, sTeamInfo.nRequireFightscore, sTeamInfo.listMembers);
			LOG_PROCESS_ERROR(bRetCode);
		}
	}

	bRetCode = DoSyncMultiTeams(pRequest->nInstanceMapId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueMaster::OnJoinOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	INT nTeamIdx = 0;
	INT nJoinTeamResult = 0;
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	KOVERALL_MEMBER sMember;
	MAP_MULTI_TEAM_INFO::iterator it;
	R2M_OVERALL_JOIN_TEAM_REQUEST* pRequest = (R2M_OVERALL_JOIN_TEAM_REQUEST*)pbyData;
	INT nMapId = pRequest->nInstanceMapId;

	QLogPrintf(LOG_INFO, "KQueueMaster::OnJoinOverallTeamRequest nMemberId=%d nInstanceMapId=%d szMemberName=%s byMemberLevel=%d byMemberSex=%d nMemberFightScore=%d dwCombinedIdx=%d nConnIndex=%d\n",
		pRequest->nMemberId, pRequest->nInstanceMapId, pRequest->szMemberName, (int)pRequest->byMemberLevel, (int)pRequest->byMemberSex, pRequest->nMemberFightScore, pRequest->dwCombinedIdx, nConnIndex);

	MAP_OVERALL_TEAMS::iterator it_overall = m_mapAllTeams.find(nMapId);
	PROCESS_ERROR(it_overall != m_mapAllTeams.end());

	{
		MAP_MULTI_TEAM_INFO& mapTeams = it_overall->second;

		it = mapTeams.find(pRequest->dwCombinedIdx);
		if(it == mapTeams.end())
		{
			nJoinTeamResult = emOVERALL_JOINTEAM_NOT_EXIST;
		}
		PROCESS_ERROR(it != mapTeams.end());
		{
			KOVERALL_TEAM_INFO& teamInfo = it->second;
			DWORD dwCombinedIdx = DYNAMIC_TEAM_INDEX(pRequest->nMemberId, nConnIndex);
			// 队伍已满员
			if(teamInfo.listMembers.size() >= MAX_OVERALL_TEAM_NUM)
			{
				nJoinTeamResult = emOVERALL_JOINTEAM_FULL;
			}
			PROCESS_ERROR(teamInfo.listMembers.size() < MAX_OVERALL_TEAM_NUM);

			// 队伍已经开始倒数
			if(teamInfo.byStarting == 1)
			{
				nJoinTeamResult = emOVERALL_TEAM_STARTING;
			}
			PROCESS_ERROR(teamInfo.byStarting == 0);

			for(std::list<KOVERALL_MEMBER>::iterator list_it = teamInfo.listMembers.begin(); list_it != teamInfo.listMembers.end(); ++list_it)
			{
				// 不能重复加入
				if(DYNAMIC_TEAM_INDEX(list_it->nMemberId, list_it->nConnectIdx) == DYNAMIC_TEAM_INDEX(pRequest->nMemberId, nConnIndex))
				{
					nJoinTeamResult = emOVERALL_JOINTEAM_ALREADY;
				}
				PROCESS_ERROR(DYNAMIC_TEAM_INDEX(list_it->nMemberId, list_it->nConnectIdx) != DYNAMIC_TEAM_INDEX(pRequest->nMemberId, nConnIndex));
			}
			sMember.byPrepare = 0;
			sMember.nConnectIdx = nConnIndex;
			sMember.nMemberId = pRequest->nMemberId;
			strncpy(sMember.szMemberName, pRequest->szMemberName, MAX_NAME_LEN);
			sMember.byLevel = pRequest->byMemberLevel;
			sMember.bySex = pRequest->byMemberSex;
			sMember.nFightScore = pRequest->nMemberFightScore;
			teamInfo.listMembers.push_back(sMember);

			m_mapJoinIndex[dwCombinedIdx] = nMapId;

			// 向所有队员同步当前队伍信息
			for(std::list<KOVERALL_MEMBER>::iterator list_it = teamInfo.listMembers.begin(); list_it != teamInfo.listMembers.end(); ++list_it)
			{
				bRetCode = DoSyncPlayerOverallTeam(list_it->nConnectIdx, list_it->nMemberId, pRequest->dwCombinedIdx, nMapId, teamInfo.nRequireFightscore, teamInfo.listMembers);
				LOG_PROCESS_ERROR(bRetCode);
			}
		}
	}

	bRetCode = DoSyncMultiTeams(pRequest->nInstanceMapId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	DoSyncTeamOpsResult(nConnIndex, pRequest->nMemberId, nJoinTeamResult, bResult);

	return bResult;
}

BOOL KQueueMaster::DoLeaveOverallTeam(DWORD dwMemberCombinedIdx, DWORD dwLeaderCombinedIdx, BYTE byIsKick)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	INT nLeaveResult = 0;
	std::list<DWORD> listTeamOps;
	MAP_OVERALL_TEAMS::iterator it_overall;
	DWORD dwCombinedIdx = dwMemberCombinedIdx;
	INT nInstanceMapId = 0;

	std::map<DWORD, INT>::iterator it = m_mapJoinIndex.find(dwCombinedIdx);
	PROCESS_ERROR(it != m_mapJoinIndex.end());

	nInstanceMapId = it->second;

	it_overall = m_mapAllTeams.find(nInstanceMapId);
	LOG_PROCESS_ERROR(it_overall != m_mapAllTeams.end());

	{
		MAP_MULTI_TEAM_INFO& mapTeams = it_overall->second;
		MAP_MULTI_TEAM_INFO::iterator it = mapTeams.find(dwLeaderCombinedIdx);
		LOG_PROCESS_ERROR(it != mapTeams.end());
		{
			std::list<KOVERALL_MEMBER>& listMembers = it->second.listMembers;
			// 队长离开直接解散
			if(dwCombinedIdx == it->second.dwLeaderCombinedIdx)
			{
				for(std::list<KOVERALL_MEMBER>::iterator list_it = listMembers.begin(); list_it != listMembers.end(); ++list_it)
				{
					DWORD dwMemberCombinedIdx = DYNAMIC_TEAM_INDEX(list_it->nMemberId, list_it->nConnectIdx);
					listTeamOps.push_back(dwMemberCombinedIdx);
				}
				mapTeams.erase(it);
				if(byIsKick < 2)
				{
					nLeaveResult = emOVERALL_TEAM_DISMISSED;
				}
				else
				{
					nLeaveResult = emOVERALL_LEAVE_NO_TIPS;
				}
				
			}
			else
			{
				for(std::list<KOVERALL_MEMBER>::iterator list_it = listMembers.begin(); list_it != listMembers.end(); ++list_it)
				{
					DWORD dwMemberCombinedIdx = DYNAMIC_TEAM_INDEX(list_it->nMemberId, list_it->nConnectIdx);
					if(dwCombinedIdx == dwMemberCombinedIdx)
					{
						listTeamOps.push_back(dwMemberCombinedIdx);
						listMembers.erase(list_it);
						break;
					}
				}

				// 一人离开向其他队员同步当前队伍信息
				for(std::list<KOVERALL_MEMBER>::iterator list_it = listMembers.begin(); list_it != listMembers.end(); ++list_it)
				{
					DoSyncPlayerOverallTeam(list_it->nConnectIdx, list_it->nMemberId, dwLeaderCombinedIdx, nInstanceMapId, it->second.nRequireFightscore, listMembers);
				}
			}
		}

	}

	bRetCode = DoSyncMultiTeams(nInstanceMapId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	if(listTeamOps.size() <= 0)
	{
		listTeamOps.push_back(dwMemberCombinedIdx);
	}
	for(std::list<DWORD>::iterator it_ops = listTeamOps.begin(); it_ops != listTeamOps.end(); ++it_ops)
	{
		DWORD dwCombinedIdx = *it_ops;
		if(!nLeaveResult)
		{
			if(byIsKick == 0)
			{
				nLeaveResult = emOVERALL_LEAVETEAM_SELF;
			}
			else if(byIsKick == 1)
			{
				nLeaveResult = emOVERALL_BE_KICKED;
			}
			else
			{
				nLeaveResult = emOVERALL_LEAVE_NO_TIPS;
			}
		}
		
		DoSyncTeamOpsResult(GET_COMBINED_CONNIDX(dwCombinedIdx), GET_COMBINED_MEMBER_ID(dwCombinedIdx), nLeaveResult, bResult);
		if(bResult)
		{
			std::map<DWORD, INT>::iterator it = m_mapJoinIndex.find(dwCombinedIdx);
			if(it != m_mapJoinIndex.end())
			{
				m_mapJoinIndex.erase(it);
			}
		}
	}
	return bResult;
}

BOOL KQueueMaster::OnLeaveOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	R2M_OVERALL_LEAVE_TEAM_REQUEST* pRequest = (R2M_OVERALL_LEAVE_TEAM_REQUEST*)pbyData;

	QLogPrintf(LOG_INFO, "KQueueMaster::OnLeaveOverallTeamRequest dwMemberCombinedIdx=%d dwLeaderCombinedIdx=%d byIsKick=%d nConnIndex=%d\n",
		pRequest->dwMemberCombinedIdx, pRequest->dwLeaderCombinedIdx,(int)pRequest->byIsKick, nConnIndex);

	bRetCode = DoLeaveOverallTeam(pRequest->dwMemberCombinedIdx, pRequest->dwLeaderCombinedIdx, pRequest->byIsKick);
	PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueMaster::OnMemberPrepareRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	MAP_OVERALL_TEAMS::iterator it_overall;
	R2M_OVERALL_PREPARE_REQUEST* pRequest = (R2M_OVERALL_PREPARE_REQUEST*)pbyData;
	DWORD dwCombinedIdx = DYNAMIC_TEAM_INDEX(pRequest->nMemberId, nConnIndex);
	INT nInstanceMapId = 0;

	QLogPrintf(LOG_INFO, "KQueueMaster::OnMemberPrepareRequest nMemberId=%d dwLeaderCombinedIdx=%d byPrepare=%d nConnIndex=%d\n",
		pRequest->nMemberId, pRequest->dwLeaderCombinedIdx, (int)pRequest->byPrepare, nConnIndex);

	std::map<DWORD, INT>::iterator it = m_mapJoinIndex.find(dwCombinedIdx);
	PROCESS_ERROR(it != m_mapJoinIndex.end());

	nInstanceMapId = it->second;

	it_overall = m_mapAllTeams.find(nInstanceMapId);
	LOG_PROCESS_ERROR(it_overall != m_mapAllTeams.end());

	{
		MAP_MULTI_TEAM_INFO& mapTeams = it_overall->second;
		MAP_MULTI_TEAM_INFO::iterator it = mapTeams.find(pRequest->dwLeaderCombinedIdx);
		LOG_PROCESS_ERROR(it != mapTeams.end());
		{
			std::list<KOVERALL_MEMBER>& listMembers = it->second.listMembers;
			for(std::list<KOVERALL_MEMBER>::iterator it_list = listMembers.begin(); it_list != listMembers.end(); ++it_list)
			{
				if(it_list->nConnectIdx == nConnIndex && it_list->nMemberId == pRequest->nMemberId)
				{
					it_list->byPrepare = pRequest->byPrepare;
					break;
				}
			}

			for(std::list<KOVERALL_MEMBER>::iterator it_list = listMembers.begin(); it_list != listMembers.end(); ++it_list)
			{
				DoSyncPlayerOverallTeam(it_list->nConnectIdx, it_list->nMemberId, pRequest->dwLeaderCombinedIdx, nInstanceMapId, it->second.nRequireFightscore, listMembers);
			}
		}
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueMaster::OnKickOverallPlayerRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	MAP_OVERALL_TEAMS::iterator it_overall;
	R2M_KICK_OVERALL_PLAYER_REQUEST* pRequest = (R2M_KICK_OVERALL_PLAYER_REQUEST*)pbyData;
	DWORD dwCombinedIdx = pRequest->dwMemberCombinedIdx;
	INT nInstanceMapId = 0;

	QLogPrintf(LOG_INFO, "KQueueMaster::OnKickOverallPlayerRequest dwMemberCombinedIdx=%d dwLeaderCombinedIdx=%d nConnIndex=%d\n",
		pRequest->dwMemberCombinedIdx, pRequest->dwLeaderCombinedIdx, nConnIndex);

	std::map<DWORD, INT>::iterator it = m_mapJoinIndex.find(dwCombinedIdx);
	PROCESS_ERROR(it != m_mapJoinIndex.end());

	nInstanceMapId = it->second;

	it_overall = m_mapAllTeams.find(nInstanceMapId);
	LOG_PROCESS_ERROR(it_overall != m_mapAllTeams.end());

	{
		MAP_MULTI_TEAM_INFO& mapTeams = it_overall->second;
		MAP_MULTI_TEAM_INFO::iterator it = mapTeams.find(pRequest->dwLeaderCombinedIdx);
		LOG_PROCESS_ERROR(it != mapTeams.end());
		{
			std::list<KOVERALL_MEMBER>& listMembers = it->second.listMembers;
			for(std::list<KOVERALL_MEMBER>::iterator it_list = listMembers.begin(); it_list != listMembers.end(); ++it_list)
			{
				if(DYNAMIC_TEAM_INDEX(it_list->nMemberId, it_list->nConnectIdx) == pRequest->dwMemberCombinedIdx)
				{
					listMembers.erase(it_list);
					break;
				}
			}

			for(std::list<KOVERALL_MEMBER>::iterator it_list = listMembers.begin(); it_list != listMembers.end(); ++it_list)
			{
				DoSyncPlayerOverallTeam(it_list->nConnectIdx, it_list->nMemberId, pRequest->dwLeaderCombinedIdx, nInstanceMapId, it->second.nRequireFightscore, listMembers);
			}
		}
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

INT KQueueMaster::GetTeamRequireFightscore(INT nInstanceMapId, DWORD dwLeaderCombinedIdx)
{
	INT nFightScore = 0;
	MAP_OVERALL_TEAMS::iterator it_overall = m_mapAllTeams.find(nInstanceMapId);
	LOG_PROCESS_ERROR(it_overall != m_mapAllTeams.end());

	{
		MAP_MULTI_TEAM_INFO& mapTeams = it_overall->second;
		MAP_MULTI_TEAM_INFO::iterator it = mapTeams.find(dwLeaderCombinedIdx);
		LOG_PROCESS_ERROR(it != mapTeams.end());
		nFightScore = it->second.nRequireFightscore;
	}

EXIT0:
	return nFightScore;
}

BOOL KQueueMaster::OnSyncPlayerOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	std::list<KOVERALL_MEMBER> listMembers;
	R2M_GET_PLAYER_OVERALLTEAM_REQUEST* pRequest = (R2M_GET_PLAYER_OVERALLTEAM_REQUEST*)pbyData;
	DWORD dwCombinedIdx = DYNAMIC_TEAM_INDEX(pRequest->nPlayerId, nConnIndex);
	INT nInstanceMapId = 0;
	INT nRequireFightscore = 0;

	QLogPrintf(LOG_INFO, "KQueueMaster::OnSyncPlayerOverallTeamRequest nPlayerId=%d dwLeaderCombinedIdx=%d nConnIndex=%d\n",
		pRequest->nPlayerId, pRequest->dwLeaderCombinedIdx, nConnIndex);

	std::map<DWORD, INT>::iterator it = m_mapJoinIndex.find(dwCombinedIdx);
	PROCESS_ERROR(it != m_mapJoinIndex.end());

	nInstanceMapId = it->second;

	LOG_PROCESS_ERROR(GetTeamList(nInstanceMapId, pRequest->dwLeaderCombinedIdx, listMembers));

	nRequireFightscore = GetTeamRequireFightscore(nInstanceMapId, pRequest->dwLeaderCombinedIdx);

	bRetCode = DoSyncPlayerOverallTeam(nConnIndex, pRequest->nPlayerId, pRequest->dwLeaderCombinedIdx, nInstanceMapId, nRequireFightscore, listMembers);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueMaster::OnStartMissionRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	std::list<KOVERALL_MEMBER> listMembers;
	R2M_START_MISSION_REQUEST* pRequest = (R2M_START_MISSION_REQUEST*)pbyData;
	DWORD dwCombinedIdx = pRequest->dwLeaderCombinedIdx;
	INT nInstanceMapId = 0;

	QLogPrintf(LOG_INFO, "KQueueMaster::OnStartMissionRequest dwLeaderCombinedIdx=%d nConnIndex=%d\n",
		pRequest->dwLeaderCombinedIdx, nConnIndex);

	std::map<DWORD, INT>::iterator it = m_mapJoinIndex.find(dwCombinedIdx);
	LOG_PROCESS_ERROR(it != m_mapJoinIndex.end());

	nInstanceMapId = it->second;

	LOG_PROCESS_ERROR(GetTeamList(nInstanceMapId, pRequest->dwLeaderCombinedIdx, listMembers));

	for(std::list<KOVERALL_MEMBER>::iterator it = listMembers.begin(); it != listMembers.end(); ++it)
	{
		LOG_PROCESS_ERROR(it->byPrepare == 1);
	}

	// 临时，在本服开
	bRetCode = DoStartMission_Local(nConnIndex, nInstanceMapId, listMembers);
	LOG_PROCESS_ERROR(bRetCode);

	/*
	bRetCode = DoStartMission_Remote(nConnIndex, nInstanceMapId, listMembers);
	LOG_PROCESS_ERROR(bRetCode);
	*/

	// 开始后删除队伍
	bRetCode = DoLeaveOverallTeam(pRequest->dwLeaderCombinedIdx, pRequest->dwLeaderCombinedIdx, 2);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueMaster::OnMemberCountDownRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	std::list<KOVERALL_MEMBER> listMembers;
	MAP_MULTI_TEAM_INFO::iterator it_team;
	R2M_MEMBER_COUNTDOWN_REQUEST* pRequest = (R2M_MEMBER_COUNTDOWN_REQUEST*)pbyData;
	DWORD dwCombinedIdx = pRequest->dwLeaderCombinedIdx;
	INT nInstanceMapId = 0;

	QLogPrintf(LOG_INFO, "KQueueMaster::OnMemberCountDownRequest dwLeaderCombinedIdx=%d nConnIndex=%d\n",
		pRequest->dwLeaderCombinedIdx, nConnIndex);

	std::map<DWORD, INT>::iterator it = m_mapJoinIndex.find(dwCombinedIdx);
	PROCESS_ERROR(it != m_mapJoinIndex.end());

	nInstanceMapId = it->second;

	{
		MAP_OVERALL_TEAMS::iterator it_all = m_mapAllTeams.find(nInstanceMapId);
		LOG_PROCESS_ERROR(it_all != m_mapAllTeams.end());

		it_team = it_all->second.find(dwCombinedIdx);
		LOG_PROCESS_ERROR(it_team != it_all->second.end());

		it_team->second.byStarting = 1;
	}

	LOG_PROCESS_ERROR(GetTeamList(nInstanceMapId, pRequest->dwLeaderCombinedIdx, listMembers));

	for(std::list<KOVERALL_MEMBER>::iterator it = listMembers.begin(); it != listMembers.end(); ++it)
	{
		LOG_PROCESS_ERROR(it->byPrepare == 1);
	}

	for(std::list<KOVERALL_MEMBER>::iterator it = listMembers.begin(); it != listMembers.end(); ++it)
	{
		DoMemberCountDownNotify(it->nConnectIdx, it->nMemberId);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueMaster::OnBroadcastOverallTeamMsg(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	IMemBlock* piPackage = NULL;
	R2M_BROADCAST_OVERALLTEAM_MSG* pRequest = (R2M_BROADCAST_OVERALLTEAM_MSG*)pbyData;
	M2R_BROADCAST_OVERALLTEAM_MSG* pMsg = NULL;


	QLogPrintf(LOG_INFO, "KQueueMaster::OnBroadcastOverallTeamMsg nMapId=%d dwLeaderCombinedIdx=%d szCreaterName=%s nConnIndex=%d\n",
		pRequest->nMapId, pRequest->dwLeaderCombinedIdx, pRequest->szCreaterName, nConnIndex);

	piPackage = QCreateMemBlock(sizeof(M2R_BROADCAST_OVERALLTEAM_MSG));
	LOG_PROCESS_ERROR(piPackage);

	pMsg = (M2R_BROADCAST_OVERALLTEAM_MSG*)piPackage->GetData();
	LOG_PROCESS_ERROR(pMsg);

	pMsg->wProtocolID = m2r_broadcast_overallteam_msg;
	pMsg->nMapId = pRequest->nMapId;
	pMsg->dwLeaderCombinedIdx = pRequest->dwLeaderCombinedIdx;
	strncpy(pMsg->szCreaterName, pRequest->szCreaterName, MAX_NAME_LEN);

	bRetCode = g_GlobalServerMaster.SendGlobal(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}


BOOL KQueueMaster::DoSyncTeamOpsResult(INT nConnIdx, INT nMemberId, INT nTeamOps, BOOL bOpResult)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	IMemBlock* piPackage = NULL;
	M2R_SYNC_OVERALLTEAM_RESULT* pResult = NULL;

	piPackage = QCreateMemBlock(sizeof(M2R_SYNC_OVERALLTEAM_RESULT));
	LOG_PROCESS_ERROR(piPackage);

	pResult = (M2R_SYNC_OVERALLTEAM_RESULT*)piPackage->GetData();
	LOG_PROCESS_ERROR(pResult);

	pResult->wProtocolID = m2r_sync_teamops_result;
	pResult->nMemberId = nMemberId;
	pResult->byTeamOps = (BYTE)nTeamOps;
	pResult->bResult = (BYTE)bOpResult;

	bRetCode = g_GlobalServerMaster.Send(nConnIdx, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueMaster::DoSyncPlayerOverallTeam(INT nConnIdx, INT nPlayerId, DWORD dwLeaderCombinedIdx, INT nMapId, INT nRequireFightscore, std::list<KOVERALL_MEMBER>& listMembers)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	INT nIdx = 0;
	IMemBlock* piPackage = NULL;
	M2R_SYNC_PLAYER_OVERALL_TEAM* pSync = NULL;
	size_t tMemberNum = listMembers.size();

	piPackage = QCreateMemBlock(sizeof(M2R_SYNC_PLAYER_OVERALL_TEAM) + tMemberNum * sizeof(KSYNC_MEMBER_INFO));
	LOG_PROCESS_ERROR(piPackage);

	pSync = (M2R_SYNC_PLAYER_OVERALL_TEAM*)piPackage->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->wProtocolID = m2r_sync_player_overall_team;
	pSync->nPlayerId = nPlayerId;
	pSync->dwLeaderCombinedIdx = dwLeaderCombinedIdx;
	pSync->nMapId = nMapId;
	pSync->byMemberNum = listMembers.size();
	pSync->nRequireFightscore = nRequireFightscore;
	for(std::list<KOVERALL_MEMBER>::iterator it = listMembers.begin(); it != listMembers.end(); ++it)
	{
		pSync->arrMembers[nIdx].byPrepare = it->byPrepare;
		pSync->arrMembers[nIdx].byMemberLevel = it->byLevel;
		pSync->arrMembers[nIdx].byMemberSex = it->bySex;
		pSync->arrMembers[nIdx].nMemberFightScore = it->nFightScore;
		pSync->arrMembers[nIdx].dwMemberCombinedIdx = DYNAMIC_TEAM_INDEX(it->nMemberId, it->nConnectIdx);
		strncpy(pSync->arrMembers[nIdx].szMemberName, it->szMemberName, MAX_NAME_LEN);
		++nIdx;
	}

	bRetCode = g_GlobalServerMaster.Send(nConnIdx, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueMaster::DoMemberCountDownNotify(INT nConnIdx, INT nPlayerId)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	INT nIdx = 0;
	IMemBlock* piPackage = NULL;
	M2R_MEMBER_COUNT_DOWN_NOTIFY* pSync = NULL;

	piPackage = QCreateMemBlock(sizeof(M2R_MEMBER_COUNT_DOWN_NOTIFY));
	LOG_PROCESS_ERROR(piPackage);

	pSync = (M2R_MEMBER_COUNT_DOWN_NOTIFY*)piPackage->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->wProtocolID = m2r_member_countdown_notify;
	pSync->nPlayerId = nPlayerId;

	bRetCode = g_GlobalServerMaster.Send(nConnIdx, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

// 临时，在本服开
BOOL KQueueMaster::DoStartMission_Local(INT nConnIdx, INT nInstanceMapId, std::list<KOVERALL_MEMBER>& listMembers)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	IMemBlock* piPackage = NULL;
	M2R_DO_START_MISSION* pRequest = NULL;
	INT nIdx = 0;

	piPackage = QCreateMemBlock(sizeof(M2R_DO_START_MISSION));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (M2R_DO_START_MISSION*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = m2r_do_start_mission;

	pRequest->nInstanceMapId = nInstanceMapId;

	memset(pRequest->arrMembers, 0, sizeof(pRequest->arrMembers));

	for(std::list<KOVERALL_MEMBER>::iterator it = listMembers.begin(); it != listMembers.end(); ++it)
	{
		pRequest->arrMembers[nIdx++] = DYNAMIC_TEAM_INDEX(it->nMemberId, it->nConnectIdx);
	}

	bRetCode = g_GlobalServerMaster.Send(nConnIdx, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueMaster::GetTeamList(INT nInstanceMapId, DWORD dwCombinedIdx, std::list<KOVERALL_MEMBER>& listMembers)
{
	BOOL bResult = FALSE;
	MAP_MULTI_TEAM_INFO mapMultiTeamInfo;
	MAP_MULTI_TEAM_INFO::iterator it;
	GetMultiMapInfo(nInstanceMapId, mapMultiTeamInfo);

	it = mapMultiTeamInfo.find(dwCombinedIdx);
	PROCESS_ERROR(it != mapMultiTeamInfo.end());

	{
		KOVERALL_TEAM_INFO& teamInfo = it->second;
		listMembers = teamInfo.listMembers;
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KQueueMaster::DoSyncMultiTeams(INT nInstanceMapId)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	IMemBlock* piPackage = NULL;
	M2R_SYNC_MULTI_TEAMS* pSync = NULL;
	MAP_MULTI_TEAM_INFO mapMultiTeamInfo;
	INT nIdx = 0;
	INT nTotalTeamNum = 0;

	GetMultiMapInfo(nInstanceMapId, mapMultiTeamInfo);

	nTotalTeamNum = mapMultiTeamInfo.size();

	piPackage = QCreateMemBlock(sizeof(M2R_SYNC_MULTI_TEAMS) + sizeof(KSYNC_TEAM_INFO) * nTotalTeamNum);
	LOG_PROCESS_ERROR(piPackage);

	pSync = (M2R_SYNC_MULTI_TEAMS*)piPackage->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->wProtocolID = m2r_sync_multi_teams;
	pSync->nInstanceMapId = nInstanceMapId;
	pSync->byTotalTeamNum = (BYTE)nTotalTeamNum;
	for(MAP_MULTI_TEAM_INFO::iterator it = mapMultiTeamInfo.begin(); it != mapMultiTeamInfo.end(); ++it)
	{
		pSync->arrTeams[nIdx].byCurrentNum = it->second.listMembers.size();
		pSync->arrTeams[nIdx].nRequireFightscore = it->second.nRequireFightscore;
		pSync->arrTeams[nIdx].dwCombinedIdx = it->first;
		strncpy(pSync->arrTeams[nIdx].szCreaterName, it->second.szCreaterName, MAX_NAME_LEN);
		++nIdx;
	}

	bRetCode = g_GlobalServerMaster.SendGlobal(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KQueueMaster::DoStartMission_Remote(INT nConnIdx, INT nInstanceMapId, std::list<KOVERALL_MEMBER>& listMembers)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	IMemBlock* piPackage = NULL;
	M2R_DO_START_MISSION_REMOTE* pRequest = NULL;
	INT nIdx = 0;
	DWORD dwTeamId = 0;
	KCROSS_SV_ROLEINFO info;

	INT nMapCopyIndex = 0;
	INT nDestConnIndex = 0;
	KMapInfo* pMapInfo = NULL;
	KMapCopy* pMapCopy = NULL;

	// 创建新的副本地图
	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(nInstanceMapId);
	LOG_PROCESS_ERROR(pMapInfo);

	// 生成新的mapcopy
	{
		UINT64 nNextCopyIndex = 0;

		bRetCode = g_pGameCenter->m_piSequence->GenerateID(pMapInfo->m_szSequenceName, 16, &nNextCopyIndex);
		LOG_PROCESS_ERROR(bRetCode);

		nMapCopyIndex = (int)nNextCopyIndex;
		pMapInfo->m_nSNGenerator++;
	}

	pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(nInstanceMapId, nMapCopyIndex);
	if(pMapCopy == NULL)
	{
		nDestConnIndex = g_pGameCenter->m_GameServer.GetBestGSConnectionForCreateMap(pMapInfo);
		if(!nDestConnIndex)
		{
			printf("ffffffffffffffffffffffffffffffffff\n");
		}
		LOG_PROCESS_ERROR(nDestConnIndex != 0);

		pMapCopy = pMapInfo->CreateMapCopy(nMapCopyIndex, g_pGameCenter->m_nTimeNow, 0, nDestConnIndex, true);
		LOG_PROCESS_ERROR(pMapCopy);
	}

	g_pGameCenter->m_GameServer.DoCreateMapNotify(nDestConnIndex, nInstanceMapId, nMapCopyIndex);
	pMapCopy->m_eState = eMapStateCreating;

	// 创建队伍
	dwTeamId = g_pGameCenter->m_TeamCenter.CreateTeam(TRUE);
	LOG_PROCESS_ERROR(dwTeamId != KD_BAD_ID);

	info.nInstanceMapId = nInstanceMapId;
	info.nInstanceMapCopyIdx = nMapCopyIndex;
	info.dwTeamId = dwTeamId;

	for(std::list<KOVERALL_MEMBER>::iterator it = listMembers.begin(); it != listMembers.end(); ++it)
	{
		DWORD dwCombinedIdx = DYNAMIC_TEAM_INDEX(it->nMemberId, it->nConnectIdx);
		std::map<DWORD, KCROSS_SV_ROLEINFO>::iterator it_cross = m_mapCrossSvInfos.find(dwCombinedIdx);
		if(it_cross != m_mapCrossSvInfos.end())
		{
			m_mapCrossSvInfos.erase(it_cross);
		}
		m_mapCrossSvInfos[dwCombinedIdx] = info;
	}
	

	piPackage = QCreateMemBlock(sizeof(M2R_DO_START_MISSION_REMOTE));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (M2R_DO_START_MISSION_REMOTE*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = m2r_do_start_mission_remote;

	pRequest->nInstanceMapId = nInstanceMapId;
	pRequest->nMapCopyIdx = nMapCopyIndex;

	memset(pRequest->arrMembers, 0, sizeof(pRequest->arrMembers));

	for(std::list<KOVERALL_MEMBER>::iterator it = listMembers.begin(); it != listMembers.end(); ++it)
	{
		pRequest->arrMembers[nIdx++] = DYNAMIC_TEAM_INDEX(it->nMemberId, it->nConnectIdx);
	}

	for(std::list<KOVERALL_MEMBER>::iterator it = listMembers.begin(); it != listMembers.end(); ++it)
	{
		bRetCode = g_GlobalServerMaster.Send(it->nConnectIdx, piPackage);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}