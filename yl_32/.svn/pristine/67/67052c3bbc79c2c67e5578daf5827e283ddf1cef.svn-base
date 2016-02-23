#pragma once

#include "onlinegameworld/kprotocol.h"
#include "vipdef.h"

#pragma pack(push, 1)

enum VIP_S2C_PROTOCOL
{
	emVIP_PTC_S2C_SYNC_COUNT,			//同步剩余购买次数
	emVIP_PTC_S2C_SYNC_EXP,				//同步vip经验
	emVIP_PTC_S2C_SYNC_RECEVIE,			//同步是否领取
	emVIP_SYNC_FREE_EXP_TIME,			// 同步vip体验时间
	emVIP_PTC_S2C_SYNC_MSG,              //同步消息
	emVIP_PTC_S2C_COUNT,
	
};

enum VIP_C2S_PROTOCOL
{
	emVIP_PTC_C2S_SYNC,
	emVIP_PTC_C2S_GETGIFTS,
	emVIP_PTC_C2S_BUY_COUNT,
	emVIP_PTC_C2S_GET_OR_SET_MSG,
	emVIP_PTC_C2S_COUNT,
};

struct VIP_BUY_COUNT
{
	INT nId;
	INT nCount;
};

struct VIP_S2C_BUY_COUNT : KLENPTC_HEADER<emVIP_PTC_S2C_SYNC_COUNT>
{
	INT nCount;
	VIP_BUY_COUNT buyCount[0];
};

struct VIP_S2C_EXP : KPTC_HEADER<emVIP_PTC_S2C_SYNC_EXP>
{
	INT nExp;
};

struct VIP_S2C_RECEVIE : KPTC_HEADER<emVIP_PTC_S2C_SYNC_RECEVIE>
{
	INT nRecevie[MAX_VIP_LEVEL];
};

struct VIP_S2C_SYNC_EXP_TIME : KPTC_HEADER<emVIP_SYNC_FREE_EXP_TIME>
{
	DWORD dwExpTime;
};


struct VIP_S2C_SYNC_MSG : KPTC_HEADER<emVIP_PTC_S2C_SYNC_MSG>
{
	CHAR szMsg[256];
};

struct VIP_C2S_SYNC : KPTC_HEADER<emVIP_PTC_C2S_SYNC>
{
	INT nSync;
};




struct VIP_C2S_GETGIFTS : KPTC_HEADER<emVIP_PTC_C2S_GETGIFTS>
{
	INT nVipLevel;		//领取是哪个等级
};

struct VIP_C2S_BUY_COUNT : KPTC_HEADER<emVIP_PTC_C2S_BUY_COUNT>
{
	INT nId;			//
	INT nNum;			//数量
};

struct VIP_C2S_GET_OR_SET_MSG : KPTC_HEADER<emVIP_PTC_C2S_GET_OR_SET_MSG>
{
	INT nType;//0 -> get 1 ->set 
	CHAR szMsg[256];
};

#pragma pack(pop)
