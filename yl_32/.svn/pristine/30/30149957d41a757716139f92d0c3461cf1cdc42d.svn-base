#pragma once
#include "bankdef.h"

#define  HONOR_RATE_PATH		"\\setting\\bank\\honor_rate.txt"
#define  NORMAL_RATE_PATH		"\\setting\\bank\\normal_rate.txt"

class BankSetting 
{
public:
	BankSetting();
	~BankSetting();

	BOOL Init();
	BOOL UnInit();

	BOOL ReadHonorRateConfig();

	BOOL ReadNormalRateConfig();

	INT GetHonorRate(INT nDay);

	INT GetNoramlRate(INT nDay);

private:
	std::map<INT, INT> m_mapHonorRate;
	std::map<INT, INT> m_mapNormalRate;

};

extern BankSetting g_cBankSetting;