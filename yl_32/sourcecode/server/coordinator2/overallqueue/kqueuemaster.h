#pragma once

#include "serverbase/protocol/RelayMasterProtocol.h"
#include "globalserverprotocolhandler.h"
#include "kqueuedef.h"

class KQueueMaster : public GlobalServerMasterProtocolHandler<KQueueMaster>
{
public:
	KQueueMaster(void);
	~KQueueMaster(void);

	BOOL Init();

	BOOL UnInit();

	VOID Activate();

	BOOL Send(int nConnIndex, IMemBlock* piBuffer);

private:
	// r2m
	BOOL OnCreateOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnJoinOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnLeaveOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnMemberPrepareRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnSyncPlayerOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnKickOverallPlayerRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);		// 队长踢人

	BOOL OnStartMissionRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnMemberCountDownRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnBroadcastOverallTeamMsg(BYTE* pbyData, size_t uDataLen, int nConnIndex);


	// m2r
	BOOL DoSyncMultiTeams(INT nInstanceMapId);		// 向所有连接的服务器同步当前地图组队信息

	BOOL DoSyncTeamOpsResult(INT nConnIdx, INT nMemberId, INT nTeamOps, BOOL bResult);

	BOOL DoSyncPlayerOverallTeam(INT nConnIdx, INT nPlayerId, DWORD dwLeaderCombinedIdx, INT nMapId, INT nRequireFightscore, std::list<KOVERALL_MEMBER>& listMembers);

	BOOL DoMemberCountDownNotify(INT nConnIdx, INT nPlayerId);

	BOOL DoStartMission_Local(INT nConnIdx, INT nInstanceMapId, std::list<KOVERALL_MEMBER>& listMembers);

	BOOL DoStartMission_Remote(INT nConnIdx, INT nInstanceMapId, std::list<KOVERALL_MEMBER>& listMembers);

private:
	VOID GetMultiMapInfo(INT nInstanceMapId, MAP_MULTI_TEAM_INFO& mapTeamInfo);

	BOOL GetTeamList(INT nInstanceMapId, DWORD dwCombinedIdx, std::list<KOVERALL_MEMBER>& listMembers);

	BOOL DoLeaveOverallTeam(DWORD dwMemberCombinedIdx, DWORD dwLeaderCombinedIdx, BYTE byIsKick);

	INT GetTeamRequireFightscore(INT nInstanceMapId, DWORD dwLeaderCombinedIdx);

private:
	MAP_OVERALL_TEAMS m_mapAllTeams;

	std::map<DWORD, INT> m_mapJoinIndex;	// <nCombinedIdx, nInstanceMapId> 玩家报名的索引

	std::map<DWORD, KCROSS_SV_ROLEINFO> m_mapCrossSvInfos;		// 跨服信息 <nCombinedIdx, 跨服信息>

public:
	INT GetInstanceMapCopyIdx(INT nConnIdx, DWORD dwRoleId);

	INT GetInstanceMapId(INT nConnIdx, DWORD dwRoleId);

	DWORD GetInstanceTeamId(INT nConnIdx, DWORD dwRoleId);
};

extern KQueueMaster g_QueueMaster;
