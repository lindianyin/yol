
#pragma once

#include "onlinegameworld/kprotocol.h"
#include "onlinegamemodule/trade/kpurseprotocoldef.h"
// -------------------------------------------------------------------------

#define MAX_SYNC_BUFF_SIZE	1024

#pragma pack(push, 1)

struct S2C_PURSE_SYNC_CURRENCY : KPTC_HEADER<emPURSE_PTC_S2C_SYNC_CURRENCY>
{
	BYTE byCurrencyType;
	INT  nCurrency;
};

struct S2C_PURSE_SYNC_REGIST_CURRENCYTYPE : KPTC_HEADER<emPURSE_PTC_S2C_SYNC_REGIST_CURRENCYTYPE>
{
	BYTE byCurrencyType;
};

struct S2C_PURSE_SYNC_MONEY : KPTC_HEADER<emPURSE_PTC_S2C_SYNC_MONEY>
{
	BYTE byMoneyType;
	INT  nMoney;
};

struct S2C_PURSE_SYNC_REGIST_MONEYTYPE : KPTC_HEADER<emPURSE_PTC_S2C_SYNC_REGIST_MONEYTYPE>
{
	BYTE byMoneyType;
};

struct S2C_PURSE_SYNC_COIN : KPTC_HEADER<emPURSE_PTC_S2C_SYNC_COIN>
{
	INT nJbCoin;
};

struct S2C_PURSE_SYNC_AL_CHEMY : KPTC_HEADER<emPURSE_PTC_S2C_SYNC_AL_CHEMY>
{
	INT nNeedCoin;						//元宝获取需要的元宝
	INT nChangeMoney;					//兑换的银两数量
	INT nChangeEnergy;					//兑换的金币数量
	INT nLeaveCoinTimes;				//剩余元宝兑换次数
	INT nNeedCoinTen;					//兑换10次需要的元宝
	INT nfreeTimeSpan;					//免费获取时间间隔
};

struct C2S_PURSE_SYNC_AL_CHEMY : KPTC_HEADER<emPURSE_PTC_C2S_SYNC_AL_CHEMY>
{
	INT nRefresh;						// 同步方式
};

#pragma pack(pop)


