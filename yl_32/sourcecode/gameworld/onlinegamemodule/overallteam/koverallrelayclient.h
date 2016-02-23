#pragma once

#include "stdafx.h"

class KOverallRelayClient
{
public:
	KOverallRelayClient(void);
	~KOverallRelayClient(void);

public:
	static BOOL DoCreateOverallTeamRequest(INT nPlayerId, INT nInstanceMapId, INT nRequireFightscore);

	static BOOL DoJoinOverallTeamRequest(INT nMemberId, INT nInstanceMapId, DWORD dwLeaderCombinedIdx);

	static BOOL DoLeaveOverallTeamRequest(DWORD dwMemberCombinedIdx, DWORD dwLeaderCombinedIdx, INT nIsKick);

	static BOOL DoMemberPrepareRequest(INT nMemberId, DWORD dwLeaderCombinedIdx, BYTE byPrepare);

	static BOOL DoGetPlayerTeamInfoRequest(INT nPlayerId, DWORD dwLeaderCombinedIdx);

	static BOOL DoKickOverallPlayerRequest(DWORD dwLeaderCombinedIdx, DWORD dwMemberCombinedIdx);

	static BOOL DoBroadcastOverallteamMsg(INT nMapId, DWORD dwLeaderCombinedIdx, INT nRequireFightscore, LPCTSTR szCreaterName);

	static BOOL DoStartMissionRequest(DWORD dwLeaderCombinedIdx);

	static BOOL DoMemberCountDownRequest(DWORD dwLeaderCombinedIdx);

	static BOOL DoCheckOverallOnlineRequest(INT nPlayerId, INT nMapId, DWORD dwMemberCombinedIdx, DWORD dwLeaderCombinedIdx);

	static VOID OnSyncOverallTeamList(BYTE* pbyData, size_t uDataLen);

	static VOID OnSyncOverallTeamOpsResult(BYTE* pbyData, size_t uDataLen);

	static VOID OnSynPlayerOverallTeam(BYTE* pbyData, size_t uDataLen);

	static VOID OnMemberCountDownNotify(BYTE* pbyData, size_t uDataLen);

	static VOID OnCheckMemberOnlineRespond(BYTE* pbyData, size_t uDataLen);

	static VOID OnMultiTransRequest(BYTE* pbyData, size_t uDataLen);

private:
	static inline BOOL Send(IMemBlock* piBuffer);
};
