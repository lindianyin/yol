#ifndef _INSTANCEDEF_H__
#define _INSTANCEDEF_H__

#include "serverbase/protocol/Relay_GS_Protocol.h"

typedef std::list<KTEAM_INFO> LIST_OVERALL_TEAMS;
typedef std::map<INT, LIST_OVERALL_TEAMS> MAP_OVERALL_TEAMS;

// 多人副本信息
struct KMultiInstanceInfo
{
	DWORD dwStartTime;		// 挑战开始时间
	INT nState;				// 挑战状态
	INT nRushStar;			// 挑战评星
	INT nRushMapId;			// 选择挑战的mapId
};

enum emOVERALL_TEAM_RESULT
{
	emOVERALL_JOINTEAM_FULL = 1, // 队伍满员，加入失败
	emOVERALL_JOINTEAM_NOT_EXIST, // 队伍不存在加入失败
	emOVERALL_JOINTEAM_ALREADY,	// 重复加入
	emOVERALL_LEAVETEAM_SELF, // 自己离开队伍
	emOVERALL_BE_KICKED,	// 被队长踢出队伍
	emOVERALL_LEAVE_NO_TIPS,	// 队伍开始后自动删除
	emOVERALL_TEAM_DISMISSED,	// 队伍解散
	emOVERALL_TEAM_STARTING,	// 队伍已经倒数
};

#endif // _INSTANCEDEF_H__