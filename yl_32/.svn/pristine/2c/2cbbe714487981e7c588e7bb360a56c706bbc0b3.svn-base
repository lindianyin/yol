#pragma once

#include "onlinegameworld/kprotocol.h"
#include "bankdef.h"

#pragma pack(push, 1)
enum KC2S_BANK_PROTOCOL
{
	c2s_get_income_request,	//获取收益
	c2s_invest_request,		//投资
};

struct KC2S_GET_INCOME_REQUEST : KPTC_HEADER<c2s_get_income_request>
{
	INT nId;
};

struct KC2S_INVEST_REQUEST : KPTC_HEADER<c2s_invest_request>
{
	INT nId;
	INT nCoins;
};

enum KS2C_BANK_PROTOCOL
{
	s2c_sync_all_invest_info,
	s2c_sync_my_invest_info,
};

struct KAllInvestInfo
{
	INT nCoins;
	DWORD dwTime;
	CHAR szName[MAX_NAME_LEN];
};

struct KS2C_ALL_INVEST_INFO : KLENPTC_HEADER<s2c_sync_all_invest_info>
{
	INT nCount;
	KAllInvestInfo arrInfo[0];
};

struct KMyInvestInfo
{
	INT nId;
	INT nCoins;
	INT nGetCoins;
	DWORD dwTime;
};

struct KS2C_MY_INVEST_INFO : KLENPTC_HEADER<s2c_sync_my_invest_info>
{
	INT nCount;
	KMyInvestInfo arrInfo[0];;
};


#pragma pack(pop)