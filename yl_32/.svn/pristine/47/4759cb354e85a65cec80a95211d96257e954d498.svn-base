#pragma once

#include "stdafx.h"

#define ARENA_PRE_RANK_NUM 5
#define ARENA_MAP_ID  232
// 挑战CD时间

#define ARENA_CHALLENGE_DEFAULT_TIME  20  // 20次挑战
#define ARENA_OPEN_LEVEL              18  // 竞技场开放等级
#define ARENA_CHALLENGE_CD_TIME    (5 * 60 * GAME_FPS)
#define MODULE_OFFLINE_FIGHT          128   // 离线战斗时取角色数据的id

#pragma pack(push, 1)
struct KATTACK_RECORD
{
	BYTE bySkillId;             // 放的技能id 

	BYTE bAttack;				// 自己攻击还是敌人攻击

	BYTE bIsCrit;				// 是否暴击
	BYTE bIsMiss;				// 是否被闪避

	INT nHurtValue;				// 自己伤害值

	INT nPetHurtValue;			// 神仙伤害值
};

#pragma pack(pop)

enum QE_OFFLINE_FIGHT_RESULT
{
	emOffline_Fighting, // 战斗中
	emOffline_Success,  // 战斗成功
	emOffline_Failed   //  战斗失败
};

struct QOFFLINE_FIGHT_STATE
{
	INT     nFightID;
	DWORD   dwStartRoleId;            // 发起战斗的角色对应的Id 
	DWORD   dwOfflineRoleId;          // 离线角色对应的id
	DWORD   dwStartRoleMapNpcId;      // 发起战斗的角色对应的npc Id 
	DWORD   dwOfflineRoleMapNpcId;    // 离线角色对应的npc id
	DWORD   dwStartPetMapNpcId;       // 发起战斗的角色神仙对应的npc id
	DWORD   dwOfflinePetMapNpcId;     // 离线角色神仙对应的npc id
	QE_OFFLINE_FIGHT_RESULT eState;   // 当前状态
};