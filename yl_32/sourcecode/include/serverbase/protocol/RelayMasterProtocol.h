
#pragma once

#include "ProtocolBasic.h"
#include "config/kplayerbasedbdef.h"


#define REGISTER_EXTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
	m_nProtocolSize[ProtocolID] = ProtocolSize;}						

#define REGISTER_INTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
	m_uProtocolSize[ProtocolID] = ProtocolSize;}


// 上行协议定义
enum R2M_PROTOCOL
{
    r2m_protocol_begin,

	//--- 基础协议 ---
	r2m_base_protocol_begin,

	r2m_handshake_request,
    r2m_ping_signal,

	r2m_base_protocol_end,

	//--- 战场相关协议 ---
	r2m_bzone_protocol_begin,

	r2m_bzone_transfer_request,
	r2m_bzone_transfer_respond,
	r2m_bzone_return_respond,

	r2m_load_role_data_request,
	r2m_sync_role_data,

	r2m_bzone_protocol_end,

	// 跨服组队协议
	r2m_overall_queue_protocol_begin,

	r2m_overall_create_team_request,			// 申请创建跨服队伍
	r2m_overall_join_team_request,
	r2m_overall_leave_team_request,
	r2m_overall_team_prepare_request,
	r2m_get_player_overallteam_request,		// 请求得到自己的队伍信息
	r2m_kick_player_request,
	r2m_start_mission_request,
	r2m_member_countdown_request,
	r2m_broadcast_overallteam_msg,

	r2m_overall_queue_protocol_end,

    r2m_protocol_end
};

// 下行协议定义
enum M2R_PROTOCOL
{
	m2r_protocol_begin = 0,

	//--- 基础协议 ---
	m2r_base_protocol_begin,

	m2r_handshake_respond,
    m2r_quit_notify,

	m2r_base_protocol_end,

	//--- 战场相关协议 ---
	m2r_bzone_protocol_begin,

	m2r_bzone_transfer_request,
	m2r_bzone_transfer_respond,
	m2r_bzone_return_request,

	m2r_bzone_protocol_end,

	// 跨服排队相关
	m2r_overall_queue_protocol_begin,

	m2r_sync_multi_teams,		// 同步跨服组队信息
	m2r_sync_teamops_result,
	m2r_sync_player_overall_team,
	m2r_member_countdown_notify,
	
	m2r_do_start_mission,
	m2r_do_start_mission_remote,

	m2r_broadcast_overallteam_msg,

	m2r_overall_queue_protocol_end,

	m2r_protocol_end
};


#pragma	pack(1)

// 上行协议结构定义

struct R2M_HANDSHAKE_REQUEST : IDENTITY_HEADER
{
    int     nGameVersion;
    time_t  nServerTime;
};

struct R2M_PING_SIGNAL : IDENTITY_HEADER
{
	DWORD dwReserved;
};

struct R2M_BZONE_TRANSFER_REQUEST : IDENTITY_HEADER
{
    DWORD           dwRoleID;
	CHAR            szRoleName[_NAME_LEN];
	CHAR            szAccount[_NAME_LEN];
	INT				nPlayerIdx;

    KRoleBaseInfo   RoleBaseInfo;
	DWORD           dwSourceRoleID;

	DWORD  uRoleDataLen;
	BYTE    byData[0];
};

struct M2R_BZONE_RETURN_REQUEST : IDENTITY_HEADER
{
	DWORD           dwRoleID;			// 在slave上登录的角色id
	DWORD			dwSourceRoleID;		// 在master上的角色id
	INT				nExValue;
	KRoleBaseInfo   RoleBaseInfo;

	DWORD uExDataLen;
	BYTE byExData[0];
};

// 玩家跨服时,目标服务器的响应消息
struct R2M_TRANSFER_PLAYER_RESPOND : IDENTITY_HEADER
{
	DWORD		dwPlayerID;
	BOOL		bSucceed;
	DWORD	    dwAddress;
	WORD		wPort;
	GUID		Guid;
};

struct R2M_LOAD_ROLE_DATA_REQUEST : IDENTITY_HEADER
{
	DWORD   dwPlayerID;
};

struct R2M_SYNC_ROLE_DATA : IDENTITY_HEADER
{
	DWORD   dwRoleID;
	BYTE	bSuccess;
	BYTE    byData[];
};

// -------------------- 跨服组队协议 --------------------

// 创建队伍
struct R2M_OVERALL_CREATE_TEAM_REQUEST : IDENTITY_HEADER
{
	INT nServerId;		// 服务器id
	INT nInstanceMapId;		// 副本地图id
	INT nCreaterId;
	CHAR szPlayerName[MAX_NAME_LEN];
	BYTE byCreaterLevel;
	BYTE byCreaterSex;
	INT nCreaterFightScore;
	INT nRequireFightScore;		// 战斗力需求
};

struct R2M_OVERALL_JOIN_TEAM_REQUEST : IDENTITY_HEADER
{
	INT nMemberId;
	INT nInstanceMapId;
	CHAR szMemberName[MAX_NAME_LEN];
	BYTE byMemberLevel;
	BYTE byMemberSex;
	INT nMemberFightScore;
	DWORD dwCombinedIdx;
};

struct R2M_OVERALL_LEAVE_TEAM_REQUEST : IDENTITY_HEADER
{
	DWORD dwMemberCombinedIdx;
	DWORD dwLeaderCombinedIdx;
	BYTE byIsKick;
};

struct R2M_KICK_OVERALL_PLAYER_REQUEST : IDENTITY_HEADER
{
	DWORD dwMemberCombinedIdx;
	DWORD dwLeaderCombinedIdx;
};

struct R2M_OVERALL_PREPARE_REQUEST : IDENTITY_HEADER
{
	INT nMemberId;
	DWORD dwLeaderCombinedIdx;
	BYTE byPrepare;
};

struct R2M_GET_PLAYER_OVERALLTEAM_REQUEST : IDENTITY_HEADER
{
	INT nPlayerId;
	DWORD dwLeaderCombinedIdx;
};

struct R2M_START_MISSION_REQUEST : IDENTITY_HEADER
{
	DWORD dwLeaderCombinedIdx;
};

struct R2M_MEMBER_COUNTDOWN_REQUEST : IDENTITY_HEADER
{
	DWORD dwLeaderCombinedIdx;
};

struct R2M_BROADCAST_OVERALLTEAM_MSG : IDENTITY_HEADER
{
	INT nMapId;
	DWORD dwLeaderCombinedIdx;
	CHAR szCreaterName[MAX_NAME_LEN];
};

struct KSYNC_TEAM_INFO
{
	CHAR szCreaterName[MAX_NAME_LEN];
	DWORD dwCombinedIdx;
	INT nRequireFightscore;
	BYTE byCurrentNum;	// 当前人数
};

// 同步一个副本的组队信息
struct M2R_SYNC_MULTI_TEAMS : IDENTITY_HEADER
{
	INT nInstanceMapId;
	BYTE byTotalTeamNum;
	KSYNC_TEAM_INFO arrTeams[0];
};

struct M2R_SYNC_OVERALLTEAM_RESULT : IDENTITY_HEADER
{
	INT nMemberId;
	BYTE byTeamOps;		// 队伍操作 1:加入队伍 2:离开队伍
	BYTE bResult;
};

struct M2R_DO_START_MISSION : IDENTITY_HEADER
{
	INT nInstanceMapId;
	DWORD arrMembers[3];
};

struct M2R_DO_START_MISSION_REMOTE : IDENTITY_HEADER
{
	INT nInstanceMapId;
	INT nMapCopyIdx;
	DWORD arrMembers[3];
};

struct KSYNC_MEMBER_INFO
{
	CHAR szMemberName[MAX_NAME_LEN];
	BYTE byMemberLevel;
	BYTE byMemberSex;
	INT nMemberFightScore;
	DWORD dwMemberCombinedIdx;
	BYTE byPrepare;
};

struct M2R_SYNC_PLAYER_OVERALL_TEAM : IDENTITY_HEADER
{
	INT nPlayerId;
	DWORD dwLeaderCombinedIdx;
	INT nMapId;
	BYTE byMemberNum;
	INT nRequireFightscore;
	KSYNC_MEMBER_INFO arrMembers[0];
};

struct M2R_MEMBER_COUNT_DOWN_NOTIFY : IDENTITY_HEADER
{
	INT nPlayerId;
};

struct M2R_BROADCAST_OVERALLTEAM_MSG : IDENTITY_HEADER
{
	INT nMapId;
	DWORD dwLeaderCombinedIdx;
	CHAR szCreaterName[MAX_NAME_LEN];
};

// ---------------------- 下行协议结构定义 ---------------------------------------

struct M2R_HANDSHAKE_RESPOND : IDENTITY_HEADER
{
	int     nWorldIndex;
    time_t  nBaseTime;
	int     nStartFrame;
	int     nDevMode;
};

struct M2R_QUIT_NOTIFY : IDENTITY_HEADER
{
    int nNothing;
};

// 玩家登录请求
struct M2R_PLAYER_LOGIN_REQUEST : IDENTITY_HEADER
{
    DWORD           dwRoleID;
    char            szRoleName[_NAME_LEN];
    char            szAccount[_NAME_LEN];
    int             nChargeFlag;            // 收费状态
	tagExtPointInfo	ExtPointInfo;			// 可用的附送点
    time_t          nEndTimeOfFee;          // 收费总截止时间
    int             nCoin;                  // 金币数
    DWORD           dwSystemTeamID;
    DWORD           dwTeamID;
    int             nBattleSide;
    DWORD           dwTongID;
	BYTE            byFarmerLimit;
	KRoleBaseInfo   BaseInfo;
};

struct M2R_CONFIRM_PLAYER_LOGIN_RESPOND : IDENTITY_HEADER
{
    DWORD dwPlayerID;
    BOOL  bPermit;
};

struct M2R_TRANSFER_PLAYER_REQUEST : IDENTITY_HEADER
{
    DWORD           dwRoleID;
    char            szRoleName[_NAME_LEN];
    char            szAccount[_NAME_LEN];
    BOOL            bChargeFlag;
	tagExtPointInfo	ExtPointInfo;
    BOOL            bExtPointLock;
    int             nLastExtPointIndex;
    int             nLastExtPointValue;
    time_t          nEndTimeOfFee;
    int             nCoin;
    DWORD           dwSystemTeamID;
    DWORD           dwTeamID;
    int             nBattleFieldSide;
    DWORD           dwTongID;
	BYTE            byFarmerLimit;
	KRoleBaseInfo   BaseInfo;
};

// 玩家跨服时,GameCenter转发目标服务器的响应消息
struct M2R_TRANSFER_PLAYER_RESPOND : IDENTITY_HEADER
{
	DWORD		dwDstPlayerID;
	DWORD		dwPlayerID;
	BOOL		bSucceed;
	DWORD	    dwAddress;
	WORD		wPort;
	GUID		Guid;
};

// 玩家跨服时,GameCenter转发目标服务器的响应消息
struct R2M_BZONE_RETURN_RESPOND : IDENTITY_HEADER
{
	DWORD		dwDstPlayerID;
	DWORD		dwPlayerID;
	BOOL		bSucceed;
	DWORD	    dwAddress;
	WORD		wPort;
	GUID		Guid;
};

struct M2R_KICK_ACCOUNT_NOTIFY : IDENTITY_HEADER
{
	DWORD       dwPlayerID;
};

// --------------------- 角色操作相关 -----------------------------

struct M2R_SAVE_ROLE_DATA_RESPOND : IDENTITY_HEADER
{
    DWORD dwPlayerID;
    BOOL  bSucceed;
	int   nUserValue;
	int   nUserValue2;
};

struct M2R_SYNC_ROLE_DATA : IDENTITY_HEADER
{
    DWORD   dwRoleID;
    size_t  uOffset;
    BYTE    byData[0];
};

struct M2R_LOAD_ROLE_DATA : IDENTITY_HEADER
{
    DWORD   dwRoleID;
    BOOL    bSucceed;
    size_t  uRoleDataLen;
    BYTE    byRoleData[0];
};

#pragma pack()

