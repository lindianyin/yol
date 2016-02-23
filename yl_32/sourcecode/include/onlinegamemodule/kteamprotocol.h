
#pragma once

#include "onlinegameworld/kprotocol.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"

#pragma pack(push, 1)

enum
{
	c2s_invite_join_team_request, 
	c2s_invite_join_team_respond, 
	c2s_apply_join_team_request,
	c2s_apply_join_team_respond,
	c2s_team_del_member_request,
	c2s_team_change_authority_request,
	c2s_team_set_distribute_mode_request,
	c2s_team_set_roll_quality_request,
	c2s_team_set_formation_leader_request,
	c2s_team_member_notify_signpost_request,   // 向队友发送小地图上的标记
	c2s_team_set_mark_request,
	c2s_team_level_up_raid_request,
	c2s_team_change_member_group_request,
	c2s_create_team_request,
	c2s_disband_team_request,
	c2s_team_protocol_total
};
enum
{
	s2c_sync_player_team_base_info,
	s2c_invite_join_team_request,     
	s2c_apply_join_team_request,
	s2c_team_add_member_notify,        // 通知队伍所有成员"某人加入了我们的队伍"
	s2c_team_del_member_notify,
	s2c_team_change_authority_notify,
	s2c_team_set_loot_mode_notify,
	s2c_team_set_formation_leader_notify,
	s2c_team_disband_notify,
	s2c_sync_team_member_online_flag,
	s2c_team_member_signpost_notify,   // 通知队友小地图上的标记
	s2c_team_set_mark_respond,
	s2c_team_camp_change,
	s2c_team_level_up_raid_notify,
	s2c_team_change_member_group_notify,

	s2c_sync_team_member_max_lmr,
	s2c_sync_team_member_current_lmr,
	s2c_sync_team_member_misc,
	s2c_sync_team_member_position,

	s2c_team_protocol_total
};


// xxx邀请xxx组队
struct C2S_INVITE_JOIN_TEAM_REQUEST : KPTC_HEADER<c2s_invite_join_team_request>
{
	char szInvitedPlayer[_NAME_LEN]; // 邀请该玩家组队
};

// xxx申请加入xxx的队伍
struct C2S_APPLY_JOIN_TEAM_REQUEST : KPTC_HEADER<c2s_apply_join_team_request>
{
	char  szApplyDst[_NAME_LEN]; // 申请加入这个玩家的队伍
};

struct C2S_CREATE_TEAM_REQUEST : KPTC_HEADER<c2s_create_team_request>
{
	DWORD dwCreateSrc;
};

struct C2S_DISBAND_TEAM_REQUEST : KPTC_HEADER<c2s_disband_team_request>
{
	DWORD dwTeamId;
};

// 客户端收到组队邀请后的应答
struct C2S_INVITE_JOIN_TEAM_RESPOND : KPTC_HEADER<c2s_invite_join_team_respond>
{
	CHAR  szInviteSrc[_NAME_LEN]; // 发出组队请求的玩家    
	BOOL  bAgreeFlag;
};

// 客户端(队长)收到入队申请后的应答
struct C2S_APPLY_JOIN_TEAM_RESPOND : KPTC_HEADER<c2s_apply_join_team_respond>
{
	char  szApplySrc[_NAME_LEN]; // 发出入队申请的玩家
	BOOL  bAgreeFlag;
};

struct C2S_TEAM_DEL_MEMBER_REQUEST : KPTC_HEADER<c2s_team_del_member_request>
{
	DWORD dwMemberID;
};

struct C2S_TEAM_CHANGE_AUTHORITY_REQUEST : KPTC_HEADER<c2s_team_change_authority_request>
{
	BYTE  byType;
	DWORD dwNewLeaderID;
};

struct C2S_TEAM_SET_LOOT_MODE_REQUEST : KPTC_HEADER<c2s_team_set_distribute_mode_request>
{
	int nLootMode;
};

struct C2S_TEAM_SET_ROLL_QUALITY_REQUEST : KPTC_HEADER<c2s_team_set_roll_quality_request>
{
	int nRollQuality;
};

struct C2S_TEAM_SET_FORMATION_LEADER_REQUEST : KPTC_HEADER<c2s_team_set_formation_leader_request>
{
	DWORD dwNewFormationLeaderID;
	int   nGroupIndex;
};

struct  C2S_TEAM_MEMBER_NOTIFY_SIGNPOST_REQUEST : KPTC_HEADER<c2s_team_member_notify_signpost_request>
{
	int   nX;
	int   nY;
};

struct C2S_TEAM_SET_MARK_REQUEST : KPTC_HEADER<c2s_team_set_mark_request>
{
	int     nMarkType;
	DWORD   dwTargetID;
};

struct C2S_TEAM_LEVEL_UP_RAID_REQUEST : KPTC_HEADER<c2s_team_level_up_raid_request>
{
};

struct C2S_TEAM_CHANGE_MEMBER_GROUP_REQUEST : KPTC_HEADER<c2s_team_change_member_group_request>
{
	DWORD dwMemberID;
	int   nDstGroupIndex;
	DWORD dwDstMemberID;
};
//////////////////////////////////////////////////////////////////////////
// 玩家登陆服务器时同步队伍基本信息
struct S2C_SYNC_PLAYER_TEAM_BASE_INFO : KLENPTC_HEADER<s2c_sync_player_team_base_info>
{
	BOOL  bSystem;
	DWORD dwTeamID;
	DWORD dwLeaderID;
	int   nGroupNum;
	int   nLootMode;
	int   nRollQuality;
	BYTE  byCamp;
	DWORD dwFormationLeader[MAX_TEAM_GROUP_NUM];
	DWORD dwDistributeMan;
	DWORD dwMarkMan;
	BOOL  bAddTeamMemberFlag;
	DWORD dwTeamMark[TEAM_MARK_COUNT];
	BYTE  byMemberCount;
	struct KMEMBER_INFO
	{
		DWORD dwMemberID;
		BYTE  byGroupIndex;
		char  szMemberName[_NAME_LEN];
		//KPORTRAIT_INFO  PortraitInfo;
		DWORD dwMapID;
		int   nMapCopyIndex;
		BYTE  byForceID;
		BYTE  byLevel;
		BYTE  byOnLineFlag;
		int   nMaxLife;
		int   nMaxMana;
		int   nMaxRage;
		BYTE  byLifePercent;
		BYTE  byManaPercent;
		BYTE  byRagePercent;
		int   nPosX;
		int   nPosY;
		BYTE  byRoleType;
		BYTE  byDeathFlag;
	} MemberInfo[0];
};

// xxx邀请你组队
struct S2C_INVITE_JOIN_TEAM_REQUEST : KPTC_HEADER<s2c_invite_join_team_request>
{
	char  szInviteSrc[_NAME_LEN]; // 发出组队请求的玩家
};

// xxx申请加入xxx的队伍
struct S2C_APPLY_JOIN_TEAM_REQUEST : KPTC_HEADER<s2c_apply_join_team_request>
{
	char  szApplySrc[_NAME_LEN]; // 发出入队申请的玩家
};

// 如果A玩家刚刚加入某队伍,通知队伍里面的所有成员"A的加入了队伍"
struct S2C_TEAM_ADD_MEMBER_NOTIFY : KPTC_HEADER<s2c_team_add_member_notify>
{
	DWORD           dwTeamID;
	int             nGroupIndex;
	DWORD           dwMemberID;
	char            szMemberName[_NAME_LEN];
};

// 通知客户端xxx离开xxx的队伍
struct S2C_TEAM_DEL_MEMBER_NOTIFY : KPTC_HEADER<s2c_team_del_member_notify>
{
	DWORD dwTeamID;
	int   nGroupIndex;
	DWORD dwMemberID;
};

struct S2C_TEAM_CHANGE_AUTHORITY_NOTIFY : KPTC_HEADER<s2c_team_change_authority_notify>
{
	DWORD dwTeamID;
	BYTE  byType;
	DWORD dwNewID;
};

struct S2C_TEAM_SET_LOOT_MODE_NOTIFY : KPTC_HEADER<s2c_team_set_loot_mode_notify>
{
	DWORD dwTeamID;
	int   nLootMode;
	int   nRollQuality;
};

struct S2C_TEAM_SET_FORMATION_LEADER_NOTIFY : KPTC_HEADER<s2c_team_set_formation_leader_notify>
{
	DWORD dwTeamID;
	int   nGroupIndex;
	DWORD dwNewFormationLeader;
};

struct S2C_TEAM_DISBAND_NOTIFY : KPTC_HEADER<s2c_team_disband_notify>
{
	DWORD dwTeamID;
};

struct S2C_SYNC_TEAM_MEMBER_ONLINE_FLAG : KPTC_HEADER<s2c_sync_team_member_online_flag>
{
	DWORD dwTeamID;
	DWORD dwMemberID;
	BOOL  bOnlineFlag;
};

struct  S2C_TEAM_MEMBER_SIGNPOST_NOTIFY : KPTC_HEADER<s2c_team_member_signpost_notify>
{
	int   nX;
	int   nY;
};

struct S2C_TEAM_SET_MARK_RESPOND : KPTC_HEADER<s2c_team_set_mark_respond>
{
	int     nMarkType;
	DWORD   dwTargetID;
};

struct S2C_TEAM_CAMP_CHANGE : KPTC_HEADER<s2c_team_camp_change>
{
	BYTE byCamp;
};

struct S2C_TEAM_LEVEL_UP_RAID_NOTIFY : KPTC_HEADER<s2c_team_level_up_raid_notify>
{
	DWORD dwTeamID;
	BYTE  byGroupNum;
};

struct S2C_TEAM_CHANGE_MEMBER_GROUP_NOTIFY : KPTC_HEADER<s2c_team_change_member_group_notify>
{
	DWORD dwTeamID;
	BYTE  bySrcGroupIndex;
	BYTE  byDstGroupIndex;
	DWORD dwDstMemberID;
	DWORD dwMemberID;
};


struct S2C_SYNC_TEAM_MEMBER_MAX_LMR : KPTC_HEADER<s2c_sync_team_member_max_lmr>
{
	DWORD   dwMemberID;
	int     nMaxLife;
	int     nMaxMana;
	int     nMaxRage;
};

struct S2C_SYNC_TEAM_MEMBER_CURRENT_LMR : KPTC_HEADER<s2c_sync_team_member_current_lmr>
{
	DWORD   dwMemberID;
	BYTE    byLifePercent;
	BYTE    byManaPercent;
	BYTE    byRagePercent;
};

struct S2C_SYNC_TEAM_MEMBER_MISC : KPTC_HEADER<s2c_sync_team_member_misc>
{
	DWORD           dwMemberID;
	//KPORTRAIT_INFO  PortraitInfo;
	DWORD           dwMapID;
	DWORD           dwForceID;
	BYTE            byCamp;
	BYTE            byLevel;  
	BYTE            byRoleType;
	BOOL            bDeathFlag;
};

struct S2C_SYNC_TEAM_MEMBER_POSITION : KPTC_HEADER<s2c_sync_team_member_position>
{
	DWORD dwMemberID;
	DWORD dwMapID;
	int   nMapCopyIndex;
	int   nPosX;
	int   nPosY;
};

//////////////////////////////////////////////////////////////////////////

// Relay将组队邀请转发到被邀请者所在的服务器
struct R2S_INVITE_PLAYER_JOIN_TEAM_REQUEST : SERVER_PROTOCOL<r2s_invite_player_join_team_request>
{
	DWORD dwInviteSrc;
	char  szInviteSrc[_NAME_LEN];
	DWORD dwInviteDst;
	BYTE  bySrcCamp;
};

// Relay对入队申请稍加处理后，转发到目标玩家所在的服务器
struct R2S_APPLY_JOIN_TEAM_REQUEST : SERVER_PROTOCOL<r2s_apply_join_team_request>
{
	DWORD dwApplyDst;
	DWORD dwApplySrc;
	char  szApplySrc[_NAME_LEN];
	BYTE  bySrcLevel;
	BYTE  bySrcCamp;
	BYTE  bySrcForceID;
	DWORD dwSrcTongID;
	DWORD dwMapID;
};

// 队伍解散消息广播
struct R2S_TEAM_DISBAND_NOTIFY : SERVER_PROTOCOL<r2s_team_disband_notify>
{
	DWORD dwTeamID;
};

// 队员上下线通知
struct R2S_SYNC_TEAM_MEMBER_ONLINE_FLAG : SERVER_PROTOCOL<r2s_sync_team_member_online_flag>
{
	DWORD dwTeamID;
	DWORD dwMemberID;
	BOOL  bOnlineFlag;
};

struct R2S_PARTY_MESSAGE_NOTIFY : SERVER_PROTOCOL<r2s_party_message_notify>
{
	DWORD dwPlayerID;
	int   nCode;
	BYTE  byData[0];
};

struct R2S_TEAM_SET_FORMATION_LEADER_NOTIFY : SERVER_PROTOCOL<r2s_team_set_formation_leader_notify>
{
	DWORD dwTeamID;
	int   nGroupIndex;
	DWORD dwNewFormationLeader;
};

struct R2S_TEAM_SET_MARK_RESPOND : SERVER_PROTOCOL<r2s_team_set_mark_respond>
{
	DWORD dwTeamID;
	int   nMarkType;
	DWORD dwTarget;
};

struct R2S_TEAM_CAMP_CHANGE : SERVER_PROTOCOL<r2s_team_camp_change>
{
	DWORD dwTeamID;
	BYTE  byCamp;
};

struct R2S_TEAM_LEVEL_UP_RAID_NOTIFY : SERVER_PROTOCOL<r2s_team_level_up_raid_notify>
{
	DWORD dwTeamID;
	int   nGroupNum;
};

struct R2S_TEAM_CHANGE_MEMBER_GROUP_NOTIFY : SERVER_PROTOCOL<r2s_team_change_member_group_notify>
{
	DWORD dwTeamID;
	DWORD dwMemberID;
	int   nOldGroupIndex;
	DWORD dwDstMemberID;
	int   nNewGroupIndex;
};

struct R2S_TEAM_CREATE_NOTIFY : SERVER_PROTOCOL<r2s_team_create_notify>
{
	DWORD dwTeamID;
	BOOL  bSystem;
	BYTE  byCamp;
	int   nLootMode;
	int   nRollQuality;
	int   nGroupNum;
	DWORD dwAuthority[tatTotal];
	DWORD dwTeamMark[TEAM_MARK_COUNT];
	BYTE  byMemberCount;
	struct KMEMBER
	{
		int     nGroupIndex;
		DWORD   dwMemberID;
		char    szMemberName[_NAME_LEN];
		BYTE    byOnLineFlag;
		BYTE    byLevel;
	} Member[0];
};

// 队伍添加成员消息广播
struct R2S_TEAM_ADD_MEMBER_NOTIFY : SERVER_PROTOCOL<r2s_team_add_member_notify>
{
	DWORD dwTeamID;
	int   nGroupIndex;
	DWORD dwMemberID;
	BYTE bySystem;
	char  szMemberName[_NAME_LEN];
};

// 队伍删除成员消息广播
struct R2S_TEAM_DEL_MEMBER_NOTIFY : SERVER_PROTOCOL<r2s_team_del_member_notify>
{
	DWORD dwTeamID;
	int   nGroupIndex;
	DWORD dwMemberID;
	char szMemberName[_NAME_LEN];
};

// 队伍队长变更通知
struct R2S_TEAM_CHANGE_AUTHORITY_NOTIFY : SERVER_PROTOCOL<r2s_team_change_authority_notify>
{
	DWORD dwTeamID;
	BYTE  byType;
	DWORD dwNewID;
};

// 物品分配模式变更通知
struct R2S_TEAM_SET_LOOT_MODE_NOTIFY : SERVER_PROTOCOL<r2s_team_set_loot_mode_notify>
{
	DWORD dwTeamID;
	int   nLootMode;
	int   nRollQuality;
};

struct R2S_SYNC_TEAM_MEMBER_MAX_LMR : SERVER_PROTOCOL<r2s_sync_team_member_max_lmr>
{
	DWORD 	dwTeamID;
	DWORD 	dwMemberID;
	int   	nMaxLife; 
	int   	nMaxMana; 
	int     nMaxRage;
};

struct R2S_SYNC_TEAM_MEMBER_CURRENT_LMR : SERVER_PROTOCOL<r2s_sync_team_member_current_lmr>
{
	DWORD 	dwTeamID;
	DWORD 	dwMemberID;
	BYTE   	byLifePercent; 
	BYTE   	byManaPercent; 
	BYTE    byRagePercent;
};

struct R2S_SYNC_TEAM_MEMBER_MISC : SERVER_PROTOCOL<r2s_sync_team_member_misc>
{
	DWORD 	dwTeamID;
	DWORD 	dwMemberID;

	// 头像相关数据
	//KPORTRAIT_INFO PortraitInfo;

	DWORD   dwForceID;
	BYTE    byCamp;
	BYTE    byLevel;
	BYTE    byRoleType;
	BYTE    byDeathFlag;
};

struct R2S_SYNC_TEAM_MEMBER_POSITION : SERVER_PROTOCOL<r2s_sync_team_member_position>
{
	DWORD 	dwTeamID;
	DWORD 	dwMemberID;
	DWORD   dwMapID;
	int     nMapCopyIndex;
	int     nPosX;
	int     nPosY;
};
//////////////////////////////////////////////////////////////////////////

// 从GS转发过来的玩家组队邀请
struct S2R_INVITE_PLAYER_JOIN_TEAM_REQUEST : SERVER_PROTOCOL<s2r_invite_player_join_team_request>
{
	DWORD dwInviteSrc;
	char  szInviteRole[_NAME_LEN];
};

struct S2R_CREATE_TEAM_REQUEST : SERVER_PROTOCOL<s2r_create_team_request>
{
	DWORD dwCreateSrc;
	BYTE bySystem;
};

// 被邀请玩家的应答消息GS==>Relay
struct S2R_INVITE_PLAYER_JOIN_TEAM_RESPOND : SERVER_PROTOCOL<s2r_invite_player_join_team_respond>
{
	char  szInviteSrc[_NAME_LEN];
	DWORD dwInviteDst;
	BOOL  bAgreeFlag;
};

// 转发玩家的入队申请到Relay
struct S2R_APPLY_JOIN_TEAM_REQUEST : SERVER_PROTOCOL<s2r_apply_join_team_request>
{
	DWORD dwApplySrc; // 发出入队申请的玩家
	char  szApplyDst[_NAME_LEN]; // 申请加入这个玩家的队伍
};

// 将队长对入队申请的应答转发到Relay
struct S2R_APPLY_JOIN_TEAM_RESPOND : SERVER_PROTOCOL<s2r_apply_join_team_respond>
{
	char  szApplySrc[_NAME_LEN];
	DWORD dwApplyDst;
	BOOL  bAgreeFlag;
};

// 队伍解散请求
struct S2R_TEAM_DISBAND_REQUEST : SERVER_PROTOCOL<s2r_team_disband_request>
{
	DWORD dwLeaderID;
	DWORD dwTeamID;
};

// 队伍删除成员请求
struct S2R_TEAM_DEL_MEMBER_REQUEST : SERVER_PROTOCOL<s2r_team_del_member_request>
{
	DWORD dwTeamID;
	DWORD dwRequestSrc; // 请求发起者的ID
	DWORD dwMemberID;
};

// 设置队伍队长请求
struct S2R_TEAM_CHANGE_AUTHORITY_REQUEST : SERVER_PROTOCOL<s2r_team_change_authority_request>
{
	DWORD dwTeamID;
	DWORD dwRequestSrc; // 请求发起者的ID
	BYTE  byType;
	DWORD dwTargetID;
};

// 设置队伍物品分配方式
struct S2R_TEAM_SET_LOOT_MODE_REQUEST : SERVER_PROTOCOL<s2r_team_set_loot_mode_request>
{
	DWORD dwTeamID;
	DWORD dwRequestSrc;
	int   nLootMode;
};

struct S2R_TEAM_SET_ROLL_QUALITY_REQUEST : SERVER_PROTOCOL<s2r_team_set_roll_quality_request>
{
	DWORD dwTeamID;
	DWORD dwRequestSrc;
	int   nRollQuality;
};

struct S2R_TEAM_SET_FORMATION_LEADER_REQUEST : SERVER_PROTOCOL<s2r_team_set_formation_leader_request>
{
	DWORD dwTeamID;
	DWORD dwRequestSrc;
	DWORD dwNewFormationLeader;
	int   nGroupIndex;
};

struct S2R_SYNC_TEAM_MEMBER_MAX_LMR : SERVER_PROTOCOL<s2r_sync_team_member_max_lmr>
{
	DWORD 	dwTeamID;
	DWORD 	dwMemberID;
	int   	nMaxLife; 
	int   	nMaxMana; 
	int     nMaxRage;
};

struct S2R_SYNC_TEAM_MEMBER_CURRENT_LMR : SERVER_PROTOCOL<s2r_sync_team_member_current_lmr>
{
	DWORD 	dwTeamID;
	DWORD 	dwMemberID;
	BYTE   	byLifePercent; 
	BYTE   	byManaPercent; 
	BYTE   	byRagePercent; 
};

struct S2R_SYNC_TEAM_MEMBER_MISC : SERVER_PROTOCOL<s2r_sync_team_member_misc>
{
	DWORD 	dwTeamID;
	DWORD 	dwMemberID;

	// 头像相关数据
	//KPORTRAIT_INFO PortraitInfo;

	DWORD   dwForceID;
	BYTE    byLevel;
	BYTE    byCamp;
	BYTE    byRoleType;
	BYTE    byDeathFlag;
};

struct S2R_SYNC_TEAM_MEMBER_POSITION : SERVER_PROTOCOL<s2r_sync_team_member_position>
{
	DWORD 	dwTeamID;
	DWORD 	dwMemberID;
	DWORD   dwMapID;
	int     nMapCopyIndex;
	int     nPosX;
	int     nPosY;
};

struct S2R_TEAM_SET_MARK_REQUEST : SERVER_PROTOCOL<s2r_team_set_mark_request>
{
	DWORD   dwTeamID;
	DWORD   dwMemberID;
	int     nMarkType;
	DWORD   dwTargetID;
};

struct S2R_TEAM_LEVEL_UP_RAID_REQUEST : SERVER_PROTOCOL<s2r_team_level_up_raid_request>
{
	DWORD dwTeamID;
	DWORD dwRequestSrc;
};

struct S2R_TEAM_CHANGE_MEMBER_GROUP_REQUEST : SERVER_PROTOCOL<s2r_team_change_member_group_request>
{
	DWORD dwTeamID;
	DWORD dwMemberID;
	int   nDstGroupIndex;
	DWORD dwDstMemberID;
	DWORD dwRequestSrc;
};

// -------------------------------------------------------------------------
#pragma pack(pop)