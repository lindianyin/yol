
#ifndef	__KINSTANCEPROTOCOL_H__
#define __KINSTANCEPROTOCOL_H__

#include "onlinegameworld/kprotocol.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "instancedef.h"

#pragma pack(1)

enum KC2S_INSTANCE_PROTOCOL
{
	c2s_instance_protocol_start = 0,
	c2s_get_overall_teams_request,
	c2s_create_overall_team_request,
	c2s_join_overall_team_request,
	c2s_leave_overall_team_request,
	c2s_member_prepare_request,
	c2s_get_player_overallteam_request,		// 请求得到自己组队信息
	c2s_kick_overall_player_request,
	c2s_start_mission_request,
	c2s_member_countdown_request,

	c2s_add_overallteam_watch,
	c2s_del_overallteam_watch,

	c2s_broadcast_overallteam_msg,		// 发送寻求队友
};

enum KS2C_INSTANCE_PROTOCOL
{
	s2c_sync_multi_instance_info,
	s2c_sync_overall_teams,		// 同步一个副本地图里的所有队伍信息
	s2c_sync_player_overallteam,	// 同步单个人的组队信息
	s2c_overall_teamsop_result,		// 告知加入队伍或者离开队伍的结果
	s2c_member_countdown_notify,
};

// c2s

struct KC2S_GET_OVERALL_TEAMS_REQUEST : KPTC_HEADER<c2s_get_overall_teams_request>
{
	INT nInstanceMapId;
};

struct KC2S_CREATE_OVERALL_TEAM_REQUEST : KPTC_HEADER<c2s_create_overall_team_request>
{
	INT nPlayerId;
	INT nInstanceMapId;
	INT nRequireFightscore;
};

struct KC2S_JOIN_OVERALL_TEAM_REQUEST : KPTC_HEADER<c2s_join_overall_team_request>
{
	INT nPlayerId;
	INT nInstanceMapId;
	DWORD dwLeaderCombinedIdx;
};

struct KC2S_LEAVE_OVERALL_TEAM_REQUEST : KPTC_HEADER<c2s_leave_overall_team_request>
{
	DWORD dwMemberCombinedIdx;
	DWORD dwLeaderCombinedIdx;
	BYTE byIsKick;
};

struct KC2S_MEMBER_PREPARE_REQUEST : KPTC_HEADER<c2s_member_prepare_request>
{
	INT nPlayerId;
	DWORD dwLeaderCombinedIdx;
	BYTE byPrepare;
};

struct KC2S_GET_PLAYER_TEAM_INFO_REQUEST : KPTC_HEADER<c2s_get_player_overallteam_request>
{
	INT nPlayerId;
	DWORD dwLeaderCombinedIdx;
};

struct KC2S_KICK_OVERALL_PLAYER_REQUEST : KPTC_HEADER<c2s_kick_overall_player_request>
{
	DWORD dwLeaderCombinedIdx;
	DWORD dwMemberCombinedIdx;
};

struct KC2S_ADD_OVERALL_TEAM_WATCH : KPTC_HEADER<c2s_add_overallteam_watch>
{
	INT nPlayerId;
	INT nInstanceMapId;
};

struct KC2S_DEL_OVERALL_TEAM_WATCH : KPTC_HEADER<c2s_del_overallteam_watch>
{
	INT nPlayerId;
};

struct KC2S_BROADCAST_OVERALLTEAM_MSG : KPTC_HEADER<c2s_broadcast_overallteam_msg>
{
	INT nMapId;
	DWORD dwLeaderCombinedIdx;
	INT nRequireFightscore;
	CHAR szCreaterName[MAX_NAME_LEN];
};

struct KC2S_START_MISSION_REQUEST : KPTC_HEADER<c2s_start_mission_request>
{
	DWORD dwLeaderCombinedIdx;
};

struct KC2S_MEMBER_COUNT_DOWN_REQUEST : KPTC_HEADER<c2s_member_countdown_request>
{
	DWORD dwLeaderCombinedIdx;
};


// s2c

struct KS2C_SYNC_MULTI_INSTANCE_INFO : KPTC_HEADER<s2c_sync_multi_instance_info>
{
	INT nState;
	INT nRushMapId;
};

struct KS2C_SYNC_OVERALL_TEAMS : KLENPTC_HEADER<s2c_sync_overall_teams>
{
	INT nInstanceMapId;
	BYTE byTeamsNum;
	KTEAM_INFO arrTeams[0];
};

struct KS2C_SYNC_PLAYER_OVERALLTEAM : KLENPTC_HEADER<s2c_sync_player_overallteam>
{
	DWORD dwLeaderCombinedIdx;
	BYTE byMemberNum;
	INT nRequireFightscore;
	KRS_MEMBER_INFO arrMembers[0];
};

struct KS2C_OVERALL_TEAMOP_RESULT : KPTC_HEADER<s2c_overall_teamsop_result>
{
	BYTE byTeamsOp;
	BYTE byResult;
};

struct KS2C_MEMBER_COUNTDOWN_NOTIFY : KPTC_HEADER<s2c_member_countdown_notify>
{

};

#pragma pack()

#endif
