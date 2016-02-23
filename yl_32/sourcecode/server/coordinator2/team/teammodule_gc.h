
#pragma once

#include "config/gamedef.h"
#include "TeamCenter.h"


class QCenterTeamModule/* : public IKGC_Module*/
{
public:
	static BOOL DoSyncTeamInfo(int nConnectionIndex, DWORD dwTeamID, KTeam* pTeam);
	static BOOL DoApplyJoinTeamRequest(KRole *pApplyDst, KRole *pApplySrc);
	static BOOL DoTeamCreateNotify(DWORD dwTeamID);
	static BOOL DoTeamChangeAuthorityNotify(DWORD dwTeamID, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID);
	static BOOL DoTeamSetLootModeNotify(DWORD dwTeamID, int nLootMode, int nRollQuality);
	static BOOL DoTeamAddMemberNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwMemberID, BOOL bSystem = 0);
	static BOOL DoTeamDelMemberNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwMemberID, CONST CHAR* szMemberName);
	static BOOL DoTeamDisbandNotify(DWORD dwTeamID);
	static BOOL DoSyncTeamMemberOnlineFlag(DWORD dwTeamID, DWORD dwMemberID, BOOL bOnlineFlag);
	static BOOL DoTeamMessageNotify(int nConnIndex, DWORD dwPlayerID, int nCode, const char* pszText = NULL);
	static BOOL DoTeamSetFormationLeaderNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwNewFormationLeader);

	static BOOL DoTeamSetMarkRespond(DWORD dwTeamID, int nMarkType, DWORD dwTargetID);
	static BOOL DoTeamLevelUpRaidNotify(DWORD dwTeamID, int nGroupNum);
	static BOOL DoTeamChangeMemberGroupNotify(
		DWORD dwTeamID, DWORD dwMemberID, int nOldGroupIndex, DWORD dwDstMemberID, int nNewGroupIndex
		);


	// 邀请某人组队
	static void OnInvitePlayerJoinTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	// 处理: 玩家收到组队邀请后的应答
	static void OnInvitePlayerJoinTeamRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	// 处理: 玩家创建队伍
	static void OnCreateTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	// 处理玩家的入队申请
	static void OnApplyJoinTeamRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	// 处理队长对入队申请的应答
	static void OnApplyJoinTeamRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	// 队伍删除(通常是将xxx踢出队伍)成员的请求
	static void OnTeamDelMemberRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	// 队伍更换队长
	static void OnTeamChangeAuthorityRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnTeamSetLootModeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnTeamSetRollQualityRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnTeamSetFormationLeader(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	// 请求解散队伍
	static void OnTeamDisbandRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static void OnTeamSyncMemberMaxLMR(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnTeamSyncMemberCurrentLMR(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnTeamSyncMemberMisc(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static void OnTeamSyncMemberPosition(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static void OnTeamSetMarkRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static void OnTeamLevelRaidRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static void OnTeamChangeMemberGroupRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

};

extern QCenterTeamModule g_cGcTeamModule;
