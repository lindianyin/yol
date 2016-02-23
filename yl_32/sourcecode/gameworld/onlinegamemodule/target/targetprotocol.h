#pragma once

#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)

enum TARGET_S2C_PROTOCOL
{
	emTARGET_PTC_S2C_SYNC,			//同步目标
	emTARGET_PTC_S2C_ALL_FINISH,	//目标全部完成

	emTARGET_PTC_S2C_COUNT,
};

struct TARGET_INFO
{
	BYTE byDay;		//第几天
	BYTE byId;		//id， 为0表示是完成当天的总奖励
	BYTE byFinish;	//
	BYTE byGive;	//是否发奖励
	INT nCount;		//次数
};

struct TARGET_S2C_SYNC : KLENPTC_HEADER<emTARGET_PTC_S2C_SYNC>
{
	INT nCount;
	TARGET_INFO arrInfo[0];
};


struct TARGET_S2C_ALL_FINISH : KPTC_HEADER<emTARGET_PTC_S2C_ALL_FINISH>
{
	INT nFinish;
};

enum TARGET_C2S_PROTOCOL
{
	emTARGET_PTC_C2S_SYNC,
	emTARGET_PTC_C2S_GETAWARD,

	emTARGET_PTC_C2S_COUNT
};

struct TARGET_C2S_SYNC : KPTC_HEADER<emTARGET_PTC_C2S_SYNC>
{
	INT nSync;
};

struct TARGET_C2S_GETAWARD: KPTC_HEADER<emTARGET_PTC_C2S_GETAWARD>
{
	INT nDay;
	INT nId;
};

#pragma pack(pop)