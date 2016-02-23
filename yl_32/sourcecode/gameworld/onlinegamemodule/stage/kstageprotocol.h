
#ifndef	__KSTAGEPROTOCOL_H__
#define __KSTAGEPROTOCOL_H__

#include "onlinegameworld/kprotocol.h"
#include "stagedef.h"

#pragma pack(1)

enum KC2S_STAGE_PROTOCOL
{
	c2s_stage_protocol_start = 0,
	c2s_mop_stage_request,
	c2s_take_mop_award,
	c2s_mop_stage_speedup,
};

enum KS2C_STAGE_PROTOCOL
{
	s2c_sync_stage_info,
	s2c_sync_stage_mop_result,
};

// c2s
struct KC2S_MOP_STAGE_REQUEST : KPTC_HEADER<c2s_mop_stage_request>
{
	INT nDifficulty;
};

struct KC2S_TAKE_MOP_AWARD : KPTC_HEADER<c2s_take_mop_award>
{

};

struct KC2S_MOP_SPEED_UP : KPTC_HEADER<c2s_mop_stage_speedup>
{

};


// s2c
struct KS2C_START_MOP_STAGE_RESULT : KPTC_HEADER<s2c_sync_stage_mop_result>
{
	BYTE bResult;
};

struct KS2C_SYNC_STAGE_INFO : KPTC_HEADER<s2c_sync_stage_info>
{
	// 挑战信息
	INT aryStars[MAX_STAGE_DIFICULTY];		// 四种难度的最高评价
	INT nCurMopStage;	// 当前选择的关卡
	INT nRushState;

	// 扫荡时间
	INT nLeftTime;		// 剩余时间
	INT nState;		// 当前状态
	BYTE bySpeedUp;		// 是否使用过加速
};

#pragma pack()

#endif
