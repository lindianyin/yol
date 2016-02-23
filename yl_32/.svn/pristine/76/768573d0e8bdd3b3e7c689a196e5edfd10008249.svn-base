/* -------------------------------------------------------------------------
//	文件名		：	kawardprotocol.h
//	创建者		：	zhaoyu
//	创建时间	：	2013-08-14
//	功能描述	：  
// -------------------------------------------------------------------------*/

#pragma once

#include "awarddef.h"
#include "onlinegameworld/kprotocol.h"

#pragma pack(1)

enum KS2C_AWARD_PROTOCOL
{
	s2c_award_begin = 0, 

	s2c_sync_award,
	s2c_take_award_respond,
};

enum KC2S_AWARD_PROTOCOL
{
	c2s_award_begin = 0,

	c2s_take_award,
};

// s2c
struct KS2C_SYNC_AWARD : KLENPTC_HEADER<s2c_sync_award>
{
	DWORD dwId;

	INT nRankIdx;

	CHAR szModuleName[MAX_NAME_LEN];
	INT nLevel;

	INT nSilver;             // 银两奖励数量
	INT nEnergy;                // 真气奖励数量
	INT nFightingSpirit;     // 战魂奖励数量
	INT nPrestige;           // 声望数量

	INT nItemNum;

	KAwardItem Items[0];
};

struct KS2C_TAKE_AWARD_RESPOND : KPTC_HEADER<s2c_take_award_respond>
{
	DWORD dwId;
	BYTE bSucceed;
};


// c2s
struct KC2S_TAKE_AWARD : KPTC_HEADER<c2s_take_award>
{
	DWORD dwAwardId;
	CHAR  szModuleName[MAX_NAME_LEN];
};

#pragma pack()