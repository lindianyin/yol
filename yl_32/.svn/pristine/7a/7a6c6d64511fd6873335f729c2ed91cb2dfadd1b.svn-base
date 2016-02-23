#pragma once
#include "qpursedef.h"

class ITabFile;

class QPurseSetting
{
public:
	QPurseSetting(void);
	~QPurseSetting(void);

	BOOL Init();
	BOOL UnInit();

private:
	BOOL ReadTimeSpanConfigs();

	BOOL ReadExchangeRateConfigs();

public:
	CONST INT RequestTimeSpan(INT currentTimes);

	CONST INT RequestExchangeRate(INT nType);

private:
	std::map<INT, INT> m_mapTimeSpan;
	std::map<INT, INT> m_mapExchangeRate;
};

extern QPurseSetting g_cPursesetting;
