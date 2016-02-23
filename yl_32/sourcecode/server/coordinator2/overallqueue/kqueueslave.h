#pragma once

#include "serverbase/protocol/RelayMasterProtocol.h"
#include "globalserverprotocolhandler.h"
#include "kqueuedef.h"

class KQueueSlave : public GlobalServerSlaveProtocolHandler<KQueueSlave>
{
public:
	KQueueSlave(void);
	~KQueueSlave(void);

	BOOL Init();

	BOOL UnInit();

	VOID Activate();

private:
	BOOL Send(IMemBlock* piBuffer);

private:
	VOID GetSlaveMultiTeamInfo(INT nInstanceMapId, LIST_SLAVE_TEAMS& listTeams);

	BOOL OnSyncMultiTeams(BYTE* pbyData, size_t uDataLen);

	BOOL OnSyncTeamOpsResult(BYTE* pbyData, size_t uDataLen);

	BOOL OnSyncPlayerOverallTeam(BYTE* pbyData, size_t uDataLen);

	BOOL OnStartMissionRequest(BYTE* pbyData, size_t uDataLen);

	BOOL OnMemberCountDownNotify(BYTE* pbyData, size_t uDataLen);

	BOOL OnBroadcastOverallTeamMsg(BYTE* pbyData, size_t uDataLen);

	BOOL OnStartMissionRemoteRequest(BYTE* pbyData, size_t uDataLen);

public:

	// 申请创建跨服队伍
	BOOL DoCreateOverallTeamRequest(INT nPlayerId, LPCTSTR szPlayerName, BYTE byPlayerLevel,  BYTE byPlayerSex, INT nFightScore, INT nMapId, INT nRequireFightScore);

	// 申请加入
	BOOL DoJoinOverallTeamRequest(INT nMemberId, LPCTSTR szMemberName, BYTE byMemberLevel, BYTE byMemberSex, INT nFightScore, INT nInstanceMapId, DWORD dwCombinedIdx);

	BOOL DoLeaveOverallTeamRequest(DWORD dwMemberCombinedIdx, DWORD dwLeaderCombinedIdx, BYTE byIsKick);

	BOOL DoMemberPrepareRequest(INT nMemberId, DWORD dwLeaderCombinedIdx, BYTE byPrepare);

	BOOL DoGetPlayerOverallTeamRequest(INT nPlayerId, DWORD dwLeaderCombinedIdx);

	BOOL DoStartMissionRequest(DWORD dwLeaderCombinedIdx);

	BOOL DoMemberCountDownRequest(DWORD dwLeaderCombinedIdx);

	BOOL DoKickOverallPlayerRequest(DWORD dwLeaderCombinedIdx, DWORD dwMemberCombinedIdx);

	BOOL DoBroadcastOverallTeamMsg(INT nMapId, DWORD dwLeaderCombinedIdx, INT nRequireFightscore, LPCTSTR szCreaterName);

private:
	MAP_SLAVE_TEAMS m_mapSlaveTeams;

};

extern KQueueSlave g_QueueSlave;
