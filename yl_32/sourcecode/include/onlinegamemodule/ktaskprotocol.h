
#ifndef __KTASKPROTOCOL_H__
#define __KTASKPROTOCOL_H__
#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)
// -------------------------------------------------------------------------

enum
{
	emKTASK_PTC_BEGIN,
	emLENPROT_S2C_SYNC_TASKGROUP,

	emPROT_SYNC_TASK, // c2s && s2c
	emPROT_SYNC_TASK_EVENT,

	emKTASK_PTC_COUNT,
};

//各类任务相关的动作
enum
{
	emKTASK_ACCEPT,
	emKTASK_AWARD,
	emKTASK_REFRESH,
	emKTASK_GIVEUP,
	emKTASK_SHARE,
	emKTASK_GENERAL_AWARD,
	emKTASK_USE_SHORTCUT,
	emKTASK_STEP_START,
	emKTASK_SET_BLACKSKY,
	emKTASK_SETENV_CALLBACK,
};

struct TASK_SYNC_PAIR
{
	WORD	wTaskID;
	INT		nValue;
};

struct S2C_SYNC_TASKGROUP : KLENPTC_HEADER<emLENPROT_S2C_SYNC_TASKGROUP>
{
	enum	{ KE_MAX_COUNT = 200 };
	WORD			wTaskCount;
	WORD			wTaskGroupID;
	BYTE			byFirst;
	TASK_SYNC_PAIR	atpPairs[KE_MAX_COUNT];

	INT GetSize() CONST
	{
		return sizeof(S2C_SYNC_TASKGROUP) - sizeof(TASK_SYNC_PAIR) * (KE_MAX_COUNT - wTaskCount);
	}
};

struct TASK_SYNC_TASK : KPTC_HEADER<emPROT_SYNC_TASK>
{
	INT		nTaskID;
	INT		nValue;
};

// 向客户端发送邀请信息
struct TASK_SYNC_EVENT : KPTC_HEADER<emPROT_SYNC_TASK_EVENT>
{
	BYTE	byType;
	INT		nTaskID;
	INT		nSubTaskIdx;
	INT		nParam;
};

#pragma pack(pop)
// -------------------------------------------------------------------------

#endif /* __KTASKPROTOCOL_H__ */
