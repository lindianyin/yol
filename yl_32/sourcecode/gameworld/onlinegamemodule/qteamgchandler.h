#pragma once

#include "onlinegamemodule/kteamprotocol.h"

class KPlayer;

class QTeamGCHandler
{
public:
	// Relay转发过来的玩家组队邀请
	static void OnInvitePlayerJoinTeamRequest(BYTE* pbyData, size_t uDataLen);
	// Relay处理玩家的入队申请后，转发到这里
	static void OnApplyJoinTeamRequest(BYTE* pbyData, size_t uDataLen);
	// Relay通知: 创建队伍
	static void OnTeamCreateNotify(BYTE* pbyData, size_t uDataLen);
	// Relay通知: 将玩家加入到队伍
	static void OnTeamAddMemberNotify(BYTE* pbyData, size_t uDataLen);
	// Relay通知: 将玩家从队伍删除
	static void OnTeamDelMemberNotify(BYTE* pbyData, size_t uDataLen);
	// Relay通知: 队伍解散
	static void OnTeamDisbandNotify(BYTE* pbyData, size_t uDataLen);
	// Relay通知: 设置队长
	static void OnTeamChangeAuthorityNotify(BYTE* pbyData, size_t uDataLen);  
	// 队伍物品分配模式变更
	static void OnTeamSetLootModeNotify(BYTE* pbyData, size_t uDataLen);
	// 上下线通知
	static void OnSyncTeamMemberOnlineFlag(BYTE* pbyData, size_t uDataLen);
	// 队伍操作响应码
	static void OnTeamMessageNotify(BYTE* pbyData, size_t uDataLen);
	//static void OnSetFormationLeaderNotify(BYTE* pbyData, size_t uDataLen);

	static void OnTeamSyncMemberMaxLMR(BYTE* pbyData, size_t uDataLen);
	static void OnTeamSyncMemberCurrentLMR(BYTE* pbyData, size_t uDataLen);
	static void OnTeamSyncMemberMisc(BYTE* pbyData, size_t uDataLen);
	static void OnTeamSyncMemberPosition(BYTE* pbyData, size_t uDataLen);

	//static void OnTeamSetMarkRespond(BYTE* pbyData, size_t uDataLen);

	//static void OnTeamCampChange(BYTE* pbyData, size_t uDataLen);

	static void OnTeamLevelUpRaidNotify(BYTE* pbyData, size_t uDataLen);

	//static void OnTeamChangeMemberGroupNotify(BYTE* pbyData, size_t uDataLen);
//////////////////////////////////////////////////////////////////////////

	// 向Relay转发邀请组队的请求
	static BOOL DoInviteJoinTeamRequest(DWORD dwInviteSrc, const char cszInviteDst[]);
	// 向Relay转发玩家对组队邀请的应答
	static BOOL DoInviteJoinTeamRespond(const char cszInviteSrc[], DWORD dwInviteDst, BOOL bAgreeFlag);
	static BOOL DoCreateTeamRequest(DWORD dwPlayerID, DWORD dwCreateSrc, BOOL bSystem = 0);
	// 向Relay转发玩家的入队申请
	static BOOL DoApplyJoinTeamRequest(DWORD dwPlayerID, const char cszApplyDst[]);
	// 向Relay转发队长对入队申请的应答
	static BOOL DoApplyJoinTeamRespond(DWORD dwLeaderID, const char cszApplySrc[], BOOL bAgreeFlag);
	// 向Relay发请求: 解散队伍
	static BOOL DoTeamDisbandRequest(DWORD dwLeaderID, DWORD dwTeamID);
	// 向Relay发请求: 将玩家从队伍删除(通常是将xxx踢出队伍)
	static BOOL DoTeamDelMemberRequest(DWORD dwTeamID, DWORD dwRequestSrc, DWORD dwPlayerID);
	// 向Relay发请求: 设置队长
	static BOOL DoTeamChangeAuthorityRequest(DWORD dwTeamID, DWORD dwRequestSrc, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID);  

	static BOOL DoTeamSetLootModeRequest(DWORD dwTeamID, DWORD dwRequestSrc, int nLootMode);
	static BOOL DoTeamSetRollQualityRequest(DWORD dwTeamID, DWORD dwRequestSrc, int nRollQuality);

	static BOOL DoTeamSetFormationLeader(DWORD dwTeamID, DWORD dwRequestSrc, DWORD dwNewFormationLeaderID, int nGroupIndex);

	static BOOL DoTeamSyncMemberMaxLMR(KPlayer* pPlayer);
	static BOOL DoTeamSyncMemberCurrentLMR(KPlayer* pPlayer);
	static BOOL DoTeamSyncMemberMisc(KPlayer* pPlayer);
	static BOOL DoSyncTeamMemberPosition(KPlayer* pPlayer);

	static BOOL DoTeamSetMarkRequest(DWORD dwTeamID, DWORD dwPlayerID, int nMarkType, DWORD dwTargetID);

	static BOOL DoTeamLevelUpRequest(DWORD dwTeamID, DWORD dwRequestSrc);

	static BOOL DoTeamChangeMemberGroupRequest(
		DWORD dwTeamID, DWORD dwMemberID, int nDstGroupIndex, DWORD dwDstMemberID, DWORD dwRequestSrc
		);

	static BOOL SendData(IMemBlock* pSendBuff);
	static BOOL ProcessGcProtocol(LPCBYTE pbData, UINT uDataLen);
	static BOOL InitProtocol();


	//typedef void (*PROCESS_PROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen);
	//static PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[r2s_team_protocol_total];
	//static size_t m_uProtocolSize[r2s_team_protocol_total];
};
