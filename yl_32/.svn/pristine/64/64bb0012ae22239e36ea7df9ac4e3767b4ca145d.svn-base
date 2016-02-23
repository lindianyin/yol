
#pragma once

#include "onlinegameworld/kgcmodulebase.h"
#include "serverbase/kgcmoduledef.h"
#include "onlinegamemodule/kteamprotocol.h"
// -------------------------------------------------------------------------

class KPlayer;

class QTeamModule : public KGcModuleBase
{
public:
	QTeamModule();
private:
	virtual BOOL InitProtocol();
	virtual BOOL OnInit();
	virtual BOOL OnUnInit();
	virtual BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);
	virtual BOOL ProcessGcProtocol(LPCBYTE pbData, UINT uDataLen);


	static BOOL CanTeamProcess(KPlayer* pPlayer);

	//邀请某玩家组队
	static void OnInviteJoinTeamRequest(KPlayer* pPlayer, char* pData, size_t nSize);
	//组队邀请应答
	static void OnInviteJoinTeamRespond(KPlayer* pPlayer, char* pData, size_t nSize);
	//创建队伍请求
	static void OnCreateTeamRequest(KPlayer* pPlayer, char* pData, size_t nSize);
	//解散队伍请求
	static void OnDisbandTeamRequest(KPlayer* pPlayer, char* pData, size_t nSize);
	//申请加入xxx的队伍
	static void OnApplyJoinTeamRequest(KPlayer* pPlayer, char* pData, size_t nSize);
	//对xxx入队声请的应答
	static void OnApplyJoinTeamRespond(KPlayer* pPlayer, char* pData, size_t nSize);
	//队伍删除成员: 离队或者踢人
	static void OnTeamDelMemberRequest(KPlayer* pPlayer, char* pData, size_t nSize);
	static void OnTeamChangeAuthorityRequest(KPlayer* pPlayer, char* pData, size_t nSize);

	static void OnTeamSetLootMode(KPlayer* pPlayer, char* pData, size_t nSize);
	static void OnTeamSetRollQuality(KPlayer* pPlayer, char* pData, size_t nSize);

	static void OnTeamSetFormationLeader(KPlayer* pPlayer, char* pData, size_t nSize);

	// 发送小地图上的标记给队友
	static void OnTeamMemberNotifySignpost(KPlayer* pPlayer, char* pData, size_t nSize);

	// 收到小队标记
	static void OnTeamSetMarkRequest(KPlayer* pPlayer, char* pData, size_t nSize);

	static void OnTeamLevelUpRaidRequest(KPlayer* pPlayer, char* pData, size_t nSize);
	static void OnTeamChangeMemberGroupRequest(KPlayer* pPlayer, char* pData, size_t nSize);

public:
	// 发送固定长度协议
	static BOOL SendData(INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);
	// 发送变长协议
	static BOOL SendLenData(INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);
	static BOOL SendUnknownData(INT nPlayerIndex, LPSTR pData, UINT uDataSize);

private:
	typedef void (*PROCESS_PROTOCOL_FUNC)(KPlayer* pPlayer, char* pData, size_t uDataLen);
	static PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[c2s_team_protocol_total];
	static size_t m_nProtocolSize[c2s_team_protocol_total];
};


