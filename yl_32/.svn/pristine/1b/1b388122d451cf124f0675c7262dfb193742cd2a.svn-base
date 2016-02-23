#include "stdafx.h"
#include "qpursesetting.h"

QPurseSetting g_cPursesetting;
QPurseSetting::QPurseSetting(void)
{
}

QPurseSetting::~QPurseSetting(void)
{
	UnInit();
}

BOOL QPurseSetting::Init()
{
	ReadTimeSpanConfigs();
	ReadExchangeRateConfigs();

	return TRUE;
}

BOOL QPurseSetting::UnInit()
{
	m_mapTimeSpan.clear();
	m_mapExchangeRate.clear();

	return TRUE;
}

BOOL QPurseSetting::ReadTimeSpanConfigs()
{
	ITabFile* pTabFile = g_OpenTabFile(TIMESPAN_TABLE_FILE);
	QCONFIRM_RET_FALSE(pTabFile);

	for (INT i = 3; i <= pTabFile->GetHeight(); i++)
	{
		INT nFreeTimes	= 0;
		INT nTimeSpan	= 0;

		pTabFile->GetInteger(i, "FreeTimes", 0, &nFreeTimes);
		pTabFile->GetInteger(i, "TimeSpan", 0, &nTimeSpan);

		m_mapTimeSpan[nFreeTimes] = nTimeSpan;
	}
	
	return TRUE;
}

BOOL QPurseSetting::ReadExchangeRateConfigs()
{
	ITabFile* pTabFile = g_OpenTabFile(EXCHANGE_RATE);
	QCONFIRM_RET_FALSE(pTabFile);

	for (INT i = 3; i <= pTabFile->GetHeight(); i++)
	{
		INT nType	= 0;
		INT nRate	= 0;

		pTabFile->GetInteger(i, "Type", 0, &nType);
		pTabFile->GetInteger(i, "Rate", 0, &nRate);

		m_mapExchangeRate[nType] = nRate;
	}

	return TRUE;
}

CONST INT QPurseSetting::RequestTimeSpan(INT nCurrentTimes)
{
	INT nTimeSpan = 0;

	std::map<INT, INT>::iterator it = m_mapTimeSpan.find(nCurrentTimes);
	if (it != m_mapTimeSpan.end())
		nTimeSpan = it->second;

	//如果当前次数没有且不为0，则返回最大间隔
	if (nCurrentTimes > 0 && nTimeSpan == 0)
		nTimeSpan = MAX_BOWL_TIME_SPAN;

	return nTimeSpan;
}

CONST INT QPurseSetting::RequestExchangeRate(INT nType)
{
	INT nRate = 0;

	std::map<INT, INT>::iterator it = m_mapExchangeRate.find(nType);
	if (it != m_mapExchangeRate.end())
		nRate = it->second;

	return nRate;
}
