

#include "stdafx.h"
#include "onlinegamebase/kjxexunify.h"
#include "onlinegamebase/ksysservice.h"
#include "kdatetimecontrol.h"
#include "onlinegamebase/ikbadwordfilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KDF_NAME_WORD_FILTER "\\setting\\namewordfilter.txt"

INameFilter* g_piNameWordFilter = NULL;
KDateTimeControl g_cDataTimeControl;
IKSysService* KSysService::m_pService = NULL;
TRandomCombined KSysService::RG;

// -----------------------------------------------------------------------------

class KSysServiceImpl : public IKSysService
{
	KSysServiceImpl() {}
public:
	static IKSysService* Create()
	{
		return new KSysServiceImpl();
	}
	virtual void Release()
	{
		delete this;
	}
	virtual time_t Time(time_t* pTimer)
	{
		return g_cDataTimeControl.GetSysTime(pTimer);
	}
	virtual time_t GameTime(time_t* pTimer)
	{
		return g_cDataTimeControl.GetGameTime(pTimer);
	}
	virtual void SetGameTime(time_t* pTimer)
	{
		g_cDataTimeControl.SetGameTime(*pTimer);
	}
	// 跟GetElapse一样
	virtual DWORD GetTickCount()
	{
		return g_cDataTimeControl.GetGameElapse();
	}
	// 返回0~(nMax - 1)的随机数，如果nMax <= 0，返回32位随机数
	virtual DWORD Rand(INT nMax = 0)
	{
		return nMax <= 0 ? g_cDataTimeControl.Rand() :
			(DWORD)((INT64)g_cDataTimeControl.Rand() * nMax / 0x100000000LL);
	}
	// 返回[0, 1)区间内随机小数
	virtual DOUBLE Random()
	{
		return (DOUBLE)g_cDataTimeControl.Rand() / 0x100000000LL;
	}

	virtual IKBadwordFilter*	CreateBadWordFilter()
	{
		return ::CreateBadWordFilter();
	}

	//IsLeap函数判断一个年份是否为闰年，方法如下:
	bool IsLeap(int year)
	{
		return (year % 4 ==0 || year % 400 ==0) && (year % 100 !=0);
	}

	//DayInYear能根据给定的日期，求出它在该年的第几天，代码如下
	int DayInYear(int year, int month, int day)
	{
		//int _day = 0;
		int DAY[12]={31,28,31,30,31,30,31,31,30,31,30,31};
		if(IsLeap(year))
		{
			DAY[1] = 29;
		}
		for(int i=0; i< month - 1; ++i)
		{
			day += DAY[i];
		}
		return day;
	}
	
	//获得连个时间戳自己相隔的天数
	virtual INT	GetTimeDiffOfDay(time_t tBegin,time_t tEnd)
	{
		//取出日期中的年月日
		int year1, month1, day1;
		int year2, month2, day2;

		struct tm tmBegin;
		localtime_r(&tBegin,&tmBegin);

		struct tm tmEnd;
		localtime_r(&tEnd,&tmEnd);
		year1 = tmBegin.tm_year + 1900;
		month1 = tmBegin.tm_mon + 1;
		day1 = tmBegin.tm_mday;

		year2 = tmEnd.tm_year + 1900;
		month2 = tmEnd.tm_mon + 1;
		day2 = tmEnd.tm_mday;


		if(year1 == year2 && month1 == month2)
		{
			return day1 > day2 ? day1 - day2 : day2 - day1;

			//如果年相同
		}
		else if(year1 == year2)
		{
			int d1, d2;
			d1 = DayInYear(year1, month1, day1);
			d2 = DayInYear(year2, month2, day2);
			return d1 > d2 ? d1 - d2 : d2 - d1;
			//年月都不相同
		}
		else{
			//确保year1年份比year2早
			if(year1 > year2)
			{
				//swap进行两个值的交换
				std::swap(year1, year2);
				std::swap(month1, month2);
				std::swap(day1, day2);
			}
			int d1,d2,d3;
			if(IsLeap(year1))
				d1 = 366 - DayInYear(year1,month1, day1); //取得这个日期在该年还于下多少天
			else
				d1 = 365 - DayInYear(year1,month1, day1);
			d2 = DayInYear(year2,month2,day2); //取得在当年中的第几天
			//cout<<"d1:"<<d1<<", d2:"<<d2;

			d3 = 0;
			for(int year = year1 + 1; year < year2; year++)
			{
				if(IsLeap(year))
					d3 += 366;
				else
					d3 += 365;
			}
			return d1 + d2 + d3;
		}

	}

};

// -------------------------------------------------------------------------
BOOL g_UnifyInit() 
{
	INT nResult = 0;

	g_cDataTimeControl.Init();

	KSysService::SetService(KSysServiceImpl::Create());

	CreateNameFilter(&g_piNameWordFilter);
	LOG_PROCESS_ERROR(g_piNameWordFilter);

	// nRetCode = g_piNameWordFilter->InitFromFile(KDF_NAME_WORD_FILTER);
	// LOG_PROCESS_ERROR(nRetCode);

	nResult = 1;
EXIT0:
	return nResult;
}

BOOL g_UnifyUnInit() 
{
	if (g_piNameWordFilter)
		SAFE_RELEASE(g_piNameWordFilter);

	IKSysService* piSysService = KSysService::GetService();
	KSysService::SetService(NULL);

	if (piSysService)
		piSysService->Release();

	return TRUE;
}

BOOL g_UnifyLoop()
{
	g_cDataTimeControl.Loop();

	return TRUE;
}

