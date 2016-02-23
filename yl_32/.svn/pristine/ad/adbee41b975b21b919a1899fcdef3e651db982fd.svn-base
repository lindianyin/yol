#ifndef _STAGEDEF_H__
#define _STAGEDEF_H__

#define MAX_STAGE_DIFICULTY  4		// 最高4种难度
#define MAX_STAGE_STAR 5		// 最高5星评价

// 挑战关卡信息
struct KStageRushInfo
{
	INT aryStars[MAX_STAGE_DIFICULTY];		// 四种难度的最高评价
	INT nRushLayer;
	DWORD dwStartTime;		// 挑战开始时间
	INT nStageDifficulty;		// 挑战难度
	INT nState;	// 挑战状态
};

struct KMopStageInfo
{
	INT nLeftTime;		// 剩余时间
	INT nState;		// 当前状态
	INT nMopStage;		// 当前扫荡的难度
	BOOL bSpeedUp;	// 是否用了加速
};

#endif // _STAGEDEF_H__