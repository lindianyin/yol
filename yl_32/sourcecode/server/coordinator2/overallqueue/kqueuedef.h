
#pragma once

#define MAX_OVERALL_TEAM_NUM 3	// 最大组队3人

// 根据玩家id和connidx合成动态索引
#define DYNAMIC_TEAM_INDEX(nPlayerId, nConnIdx) ((nPlayerId) | (nConnIdx) << 16)

#define GET_COMBINED_MEMBER_ID(nCombineIdx) ((nCombineIdx) & 0xFFFF)
#define GET_COMBINED_CONNIDX(nCombineIdx) ( (0xFFFF0000 & (nCombineIdx)) >> 16)

#pragma	pack(1)

// master 存储队伍结构

struct KOVERALL_MEMBER
{
	INT nConnectIdx;
	INT nMemberId;
	CHAR szMemberName[MAX_NAME_LEN];
	BYTE byLevel;
	BYTE bySex;
	INT nFightScore;
	BYTE byPrepare;
};

struct KOVERALL_TEAM_INFO
{
	CHAR szCreaterName[MAX_NAME_LEN];
	INT nRequireFightscore;
	DWORD dwLeaderCombinedIdx;
	BYTE byStarting;		// 是否已经开始倒数

	std::list<KOVERALL_MEMBER> listMembers;
};


// 玩家跨服过来所需要的信息
struct KCROSS_SV_ROLEINFO
{
	INT nInstanceMapId;
	INT nInstanceMapCopyIdx;
	DWORD dwTeamId;
};

typedef std::map<DWORD, KOVERALL_TEAM_INFO> MAP_MULTI_TEAM_INFO;		// 存储一个地图id里的排队信息

typedef std::map<INT, MAP_MULTI_TEAM_INFO> MAP_OVERALL_TEAMS;		// 存储了所有排队信息


// slave 存储队伍结构
struct KSLAVE_TEAM_INFO
{
	CHAR szCreaterName[MAX_NAME_LEN];
	INT nRequireFightscore;
	DWORD dwCombinedIdx;
	BYTE byCurrentMemberNum;
};

typedef std::list<KSLAVE_TEAM_INFO> LIST_SLAVE_TEAMS;
typedef std::map<INT, LIST_SLAVE_TEAMS> MAP_SLAVE_TEAMS;

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

#pragma pack()
