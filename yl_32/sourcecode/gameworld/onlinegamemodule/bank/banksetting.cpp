#include "stdafx.h"
#include "banksetting.h"
#include "bankdef.h"
#include "Runtime/qstring.h"
BankSetting g_cBankSetting;
BankSetting::BankSetting()
{

}

BankSetting::~BankSetting()
{

}

BOOL BankSetting::Init()
{
	ReadHonorRateConfig();
	ReadNormalRateConfig();
	return TRUE;
}

BOOL BankSetting::UnInit()
{
	return TRUE;
}

BOOL BankSetting::ReadHonorRateConfig()
{
	ITabFile* pTabFile = g_OpenTabFile(HONOR_RATE_PATH);
	QCONFIRM_RET_FALSE(pTabFile);

	for (INT i = 3; i <= pTabFile->GetHeight(); i++)
	{
		INT nDays	= 0;
		INT nRate	= 0;

		pTabFile->GetInteger(i, "days", 0, &nDays);
		pTabFile->GetInteger(i, "rate", 0, &nRate);

		m_mapHonorRate[nDays] = nRate;
	}

	return TRUE;
}

BOOL BankSetting::ReadNormalRateConfig()
{
	ITabFile* pTabFile = g_OpenTabFile(NORMAL_RATE_PATH);
	QCONFIRM_RET_FALSE(pTabFile);

	for (INT i = 3; i <= pTabFile->GetHeight(); i++)
	{
		INT nDays	= 0;
		INT nRate	= 0;

		pTabFile->GetInteger(i, "days", 0, &nDays);
		pTabFile->GetInteger(i, "rate", 0, &nRate);

		m_mapNormalRate[nDays] = nRate;
	}

	return TRUE;
}

INT BankSetting::GetHonorRate(INT nDay)
{
	return m_mapHonorRate[nDay];
}

INT BankSetting::GetNoramlRate(INT nDay)
{
	return m_mapNormalRate[nDay];
}
