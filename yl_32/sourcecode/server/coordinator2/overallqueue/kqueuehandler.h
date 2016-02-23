
#pragma once

#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "kqueueslave.h"

class KOverallQueueHandler
{
public:
	static BOOL DoSyncOverallTeams(INT nInstanceMapId, LIST_SLAVE_TEAMS& listSlaveTeams);

	static BOOL DoSyncTeamOpsResult(INT nMemberId, BYTE byTeamOps, BYTE byResult);

	static BOOL DoSyncPlayerOverallTeam(INT nPlayerId, INT nMemberNum, DWORD dwLeaderCombinedIdx, INT nMapId, INT nRequireFightscore, KSYNC_MEMBER_INFO* arrMembers);

	static BOOL DoMemberCountDownNotify(INT nPlayerId);

	static BOOL DoMultiTransRequest(INT nMapId, INT nMapCopyIdx, DWORD* arrMembers);

	static VOID OnCreateOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnJoinOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnLeaveOverallTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnMemberPrepareRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnGetPlayerTeamInfoRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnKickOverllPlayerRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnBroadcastOverallteamMsg(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnStartMissionRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnMemberCountDownRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnCheckOnlineLeaveRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

private:
	static inline BOOL Send(int nConnIndex, IMemBlock* piBuffer);
};
