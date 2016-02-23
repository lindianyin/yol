/* -------------------------------------------------------------------------
//	文件名		：	bufferdef.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-7 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __BUFFER_DEF_H__
#define __BUFFER_DEF_H__

// 技能触发位置
enum BUFFER_POSITION
{
	BUFFER_POSITION_SELF,    // 自身
	BUFFER_POSITION_TARGET // 目标或目标点
};

#define BUFFER_TABLE_FILE                 "\\setting\\buffer_\\buffer.txt"
#define BUFFER_TIMER_EFFECT_TABLE_FILE    "\\setting\\buffer_\\e_timer.txt"
#define BUFFER_TRIGGER_EFFECT_TABLE_FILE  "\\setting\\buffer_\\e_trigger.txt"
#define BUFFER_NUMBER_TABLE_FILE		  "\\setting\\buffer_\\buff_numbers.txt"

#define BUFFER_POOL_MAX_COUNT	50000	//当前服务器最多的玩家buff数量

#define GROUND_BUFFER_POOL_MAX_COUNT	10000	//当前服务器最多的地面buff数量

#define SCRIPT_MAX_LENGTH 1024	//执行脚本的最大长度

#define BUFF_TIMER_EFFECT_COUNT 5	//时间效果的数量

#define BUFF_TRIGGER_EFFECT_COUNT 5	//条件效果的数量

enum TimerEffectLaunchType	//时间触发类型
{
	UpperLaunch = 1,//上沿触发
	LowerLaunch,	//下沿触发
	BothLaunch,		//上下沿都触发
	IntervalLaunch, //间隔触发
};

enum BufferGroupType //buff类型group,先这样定义，确定数值出来再进行修改
{
	NormalBuffer,
	SpecialBuffer
};

enum BufferStackMode	//buff的叠加模式
{
	emOverWrite,		//相同StackCategory，高等级直接覆盖低等级的，叠加层数不变，时间归零
	emUpdateInterval,	//与原来buff共存，添加新Buff
	emForbidStack,		//不可叠加
	emStackInterval,	//替换原来buff，时间叠加，叠加层数+1
	emNoneStackBuff,	//没有叠加关系
};

enum BufferStackDetail
{
	emUpdate = 1,
	emReplace = 2,
	emForbid = 3,
	emStackTime = 4
};

enum BufferType // buff类型
{
	emNoneEffectBuff,
	emNormalDeBuff,		// 其他有害buff
	emHurtOnceDebuff,	// 一次性伤害
	emBleedDebuff,		// 流血
	emControlDebuff,	// 控制
	emNiceBuff,			// 增益
};

#define IS_DEBUFF(nBuffType) ((nBuffType) != emNiceBuff)

#endif
