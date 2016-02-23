
#pragma once

#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"

struct QTEAM_MEMBER_INFO;
class KPlayer;

class QPlayerTeam : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emPLAYER_CTRL_TEAM	};

	explicit QPlayerTeam(KCharacter& rcPlayer);
	virtual ~QPlayerTeam();
public:
	virtual BOOL Init();
	virtual BOOL UnInit();
	virtual BOOL OnLogin();
	//virtual BOOL OnLogout();
	virtual BOOL Active();
	//virtual BOOL OnClientReady();

	static BOOL CanTeamProcess(KPlayer* pDstPlayer);

	static BOOL DoSyncTeamMemberMaxLMR(DWORD dwTeamID, QTEAM_MEMBER_INFO* pMember);
	static BOOL DoSyncTeamMemberCurrentLMRGlobal(DWORD dwTeamID, QTEAM_MEMBER_INFO* pMember);
	static BOOL DoSyncTeamMemberCurrentLMRLocal(KPlayer* pPlayer);
	static BOOL DoSyncTeamMemberMisc(DWORD dwTeamID, QTEAM_MEMBER_INFO* pMember);
	static BOOL DoSyncTeamMemberPositionGlobal(DWORD dwTeamID, QTEAM_MEMBER_INFO* pMember);
	static BOOL DoSyncTeamMemberPositionLocal(KPlayer* pPlayer);

	// 玩家登陆服务器时同步队伍数据
	static BOOL DoLoginTeamSync(KPlayer* pPlayer);
	// 同步玩家所在队伍的基本信息
	static BOOL DoSyncTeamBaseInfo(DWORD dwTeamID, KPlayer* pPlayer, BOOL bAddTeamMemberFlag = false);
	// 将组队邀请发送到目标玩家
	static BOOL DoInvitePlayerJoinTeamRequest(DWORD dwInviteDst, const char cszInviteSrc[]);
	// 将Relay转发过来的入队申请,发送到队长客户端
	static BOOL DoApplyJoinTeamRequest(DWORD dwLeader, const char cszApplySrc[]);
	// 队伍新添成员的通知
	static BOOL DoTeamAddMemberNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwMemberID);
	// xxx离开队伍的通知
	static BOOL DoTeamDelMemberNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwMemberID);
	static BOOL DoTeamChangeAuthorityNotify(DWORD dwTeamID, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID);
	static BOOL DoTeamSetLootModeNotify(DWORD dwTeamID, int nLootMode, int nRollQuality);

	static BOOL DoTeamSetFormationLeaderNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwNewFormationLeader);

	// 队伍解散通知
	static BOOL DoTeamDisbandNotify(DWORD dwTeamID);
	// 同步成员在线状态的改变
	static BOOL DoSyncTeamMemberOnlineFlag(DWORD dwTeamID, DWORD dwMemberID, BOOL bOnlineFlag);
	// 同步玩家状态信息
	static BOOL DoSyncPlayerStateInfo(KPlayer* pPlayer);

	// 队伍标记
	static BOOL DoTeamSetMarkRespond(DWORD dwTeamID, int nMarkType, DWORD dwTargetID);

	// 组队阵营发生变化
	//static BOOL DoTeamCampChange(DWORD dwTeamID, KCAMP eCamp);

	static BOOL DoTeamLevelUpRaidNotify(DWORD dwTeamID, int nGroupNum);

	static BOOL DoTeamChangeMemberGroupNotify(
		DWORD dwTeamID, int nSrcGroupIndex, int nDstGroupIndex, DWORD dwDstMemberID, DWORD dwMemberID
		);


	void ResetTeamLastSyncParamRecord();

	// 同场景队友上一次同步数值
	int     m_nLastSyncLocalTeamLifePercent;
	int     m_nLastSyncLocalTeamManaPercent;
	int     m_nLastSyncLocalTeamRagePercent;
	DWORD   m_dwLastSyncLocalTeamMemberMapID;
	int     m_nLastSyncLocalTeamMemberMapCopyIndex;
	int     m_nLastSyncLocalTeamMemberPosX;
	int     m_nLastSyncLocalTeamMemberPosY;
	// 跨服同步数据的上一次同步数值
	int     m_nLastSyncGlobalTeamLifePercent;
	int     m_nLastSyncGlobalTeamManaPercent;
	int     m_nLastSyncGlobalTeamRagePercent;
	DWORD   m_dwLastSyncGlobalTeamMemberMapID;
	int     m_nLastSyncGlobalTeamMemberMapCopyIndex;
	int     m_nLastSyncGlobalTeamMemberPosX;
	int     m_nLastSyncGlobalTeamMemberPosY;

	static BYTE	m_byTempData[32*1024];
protected:
	static void TeamBroadcast(DWORD dwTeamID, void* pvData, size_t uSize);
protected:
	KPlayer& m_rPlayer;
};
