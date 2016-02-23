#pragma once

#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)

enum OPENSE_S2C_PROTOCOL
{
	emOPENSE_PTC_S2C_SYNC,			//同步目标
	emOPENSE_PTC_S2C_ALL_FINISH,	//目标全部完成
	emOPENSE_PTC_S2C_COUNT,
};

struct OPENSE_INFO
{
	BYTE byDay;		//第几天
	BYTE byId;		//id， 为0表示是完成当天的总奖励
	BYTE byFinish;	//
	BYTE byGive;	//是否发奖励
	INT nCount;		//次数
};

struct OPENSE_S2C_SYNC : KLENPTC_HEADER<emOPENSE_PTC_S2C_SYNC>
{
	INT day;
	INT one;
	INT two;
	INT nCount;
	OPENSE_INFO arrInfo[0];
};


struct OPENSE_S2C_ALL_FINISH : KPTC_HEADER<emOPENSE_PTC_S2C_ALL_FINISH>
{
	INT nFinish;
};

enum OPENSE_C2S_PROTOCOL
{
	emOPENSE_PTC_C2S_SYNC,
	emOPENSE_PTC_C2S_GETAWARD,

	emOPENSE_PTC_C2S_COUNT
};

struct OPENSE_C2S_SYNC : KPTC_HEADER<emOPENSE_PTC_C2S_SYNC>
{
	INT nSync;
};

struct OPENSE_C2S_GETAWARD : KPTC_HEADER<emOPENSE_PTC_C2S_GETAWARD>
{
	INT nDay;
	INT nId;
};

#pragma pack(pop)
