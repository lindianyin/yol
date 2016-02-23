#ifndef _ESCORTDEF_H__
#define _ESCORTDEF_H__

#pragma pack(1)

#define MAX_ESCORTPAGE_PLAYER_NUM 11	// 每页修仙人数

#define MAX_ESCORT_POS_NUM 2000		// 最大修仙承载人数
#ifdef VIETNAM
#define ESCORT_TIME (60*15)		// 越南修仙时间15分钟
#else
#define ESCORT_TIME (60)		// 修仙时间1分钟
#endif
enum ESCORT_STATE
{
	emESCORT_STATE_NONE,
	emESCORT_STATE_NORMAL,		// 修仙中
	emESCORT_STATE_BEHIT,		// 被劫驾
	emESCORT_STATE_COMPLETE_NORMAL,	// 正常完成
	emESCORT_STATE_COMPLETE_BEHIT, // 被劫驾完成
};

// 玩家修仙结构
struct KPlayerEscort
{
	INT nPlayerId;
	BYTE byPlayerLevel;
	CHAR szPlayerName[_NAME_LEN];
	DWORD dwFightScore;
	BYTE byEscortLevel;		// 座驾等级 1 - 5
	DWORD dwProtectRoleId;
	CHAR szProtecterName[_NAME_LEN];
	DWORD dwRoberId;		// 劫驾角色id
	DWORD dwRoberFightscore;
	CHAR szRoberName[_NAME_LEN];
	INT nLeftTime;
	BYTE byIfSpeedUp;
	BYTE byEscortState;
};

// 劫驾结构
struct KRobEscort
{
	INT nRobPlayerId;
	INT nRoberId;	// 抢劫者
	CHAR szPlayerName[_NAME_LEN];
	INT nLeftTime;
	BYTE byPlayerLevel;
	BYTE byEscortLevel;
	BYTE byCanTakeAward;
};

#pragma pack()


#endif // _ESCORTDEF_H__
