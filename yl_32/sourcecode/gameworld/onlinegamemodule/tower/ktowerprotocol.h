
#ifndef	__KTOWERPROTOCOL_H__
#define __KTOWERPROTOCOL_H__

#include "onlinegameworld/kprotocol.h"

#pragma pack(1)

enum KC2S_TOWER_PROTOCOL
{
	c2s_tower_protocol_start = 0,
	c2s_start_mop_tower,		// 开始扫荡
	c2s_stop_mop_tower,
	c2s_take_mop_award,
	c2s_mop_speedup,
	c2s_finish_now,		// 直接完成
};

enum KS2C_TOWER_PROTOCOL
{
	s2c_start_mop_result,  // 客户端点击开始扫荡的结果
	s2c_sync_mop_info,		// 同步当前扫荡信息
};

struct KS2C_START_MOP_RESULT : KPTC_HEADER<s2c_start_mop_result>
{
	BYTE bResult;
};

struct KS2C_SYNC_MOP_INFO : KPTC_HEADER<s2c_sync_mop_info>
{
	INT nLeftTime;		// 剩余时间
	INT nState;		// 当前状态
	BOOL bSpeedUp;	// 是否用了加速
};

struct QC2S_START_MOP : KPTC_HEADER<c2s_start_mop_tower>
{

};

struct QC2S_STOP_MOP : KPTC_HEADER<c2s_stop_mop_tower>
{

};

struct QC2S_TAKE_MOP_AWARD : KPTC_HEADER<c2s_take_mop_award>
{

};

struct QC2S_MOP_SPEED_UP : KPTC_HEADER<c2s_mop_speedup>
{

};

struct QC2S_MOP_FINISH_NOW : KPTC_HEADER<c2s_finish_now>
{

};

#pragma pack()

#endif
