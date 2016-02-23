#ifndef _KRANKDEF_H_
#define _KRANKDEF_H_

enum KE_RANK_ID
{
	emRANK_ID_BEGIN = 0,

	emRANK_ARENA = 1,	// 竞技场
	emRANK_ROLE_FIGHTSCORE = 2, //个人排行 -> 战斗力榜
	emRANK_PET_LEVEL = 3,	//个人排行  -> 神仙等级榜
	emRANK_PET_FIGHTSCORE = 4,	// 个人排行 -> 神仙战斗力
	emRANK_RIDE_FIGHTSCORE = 5, // 个人排行 -> 坐骑战力榜
	emRANK_MPJJ,	// 门派竞技 个人排行 -> 战神无双榜
	emRANK_LG,		// 炼卦   个人排行 -> 炼卦重数榜
	emRANK_SMZC,	// 神魔战场  个人排行 -> 神魔积分榜

	// 以下是家族排名
	emRANK_TONG_FIGHTSCORE,
	emRANK_TONG_SALARY,

	//开服
	emRANK_ROLE_LEVEL_OPEN_SERVER = 11,//玩家等级  开服活动排行榜  -> 全民冲级
	emRANK_RIDE_LV,     //坐骑等级   开服活动排行榜  -> 万兽之王
	emRANK_AREAN_OS,//竞技场   开服活动排行榜  -> 竞技争锋
	emRANK_PET_LV_OS,     //宠物等级   开服活动排行榜  -> 绝世大神
	emRANK_LG_OS,//炼卦   开服活动排行榜  -> 无敌八卦
	emRANK_TOWER,     //伐魔塔   开服活动排行榜  -> 斩妖伐魔
	emRANK_ROLE_FIGSHTSCORE_OS,//战斗力   开服活动排行榜  -> 战神无双
	emRANK_ID_END,
};

enum KE_RANK_MAX_COUNT
{
	emRANK_ARENA_MAX_COUNT = 500,

	emRANK_MAX_COUNT = 100,		// 个人排行榜最大数量

};

#define emRANK_MAX_COUNT_OPEN_SERVICE 10//开服排行榜发送最大人数
#define RANK_POOL_MAX_COUNT 500

#endif	// _KRANKDEF_H_