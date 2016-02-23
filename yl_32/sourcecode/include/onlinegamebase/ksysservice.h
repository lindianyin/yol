
#pragma once

#include "Core/QTime.h"

#include "kplayerback_interface.h"
#include "rancombi.h"

class KSysService
{
public:
	// Declare object of combined generator
	static TRandomCombined RG; // 联合随机数发生器

	static VOID SetService(IKSysService* pService)
	{
		m_pService = pService;
	};
	static IKSysService* GetService()
	{
		return m_pService;
	};
	static time_t Time(time_t* pTimer)
	{
		return time(pTimer);
	}
	static time_t GameTime(time_t* pTimer)
	{
		return m_pService ? m_pService->GameTime(pTimer) : time(pTimer);
	}
	static void SetGameTime(time_t* pTimer)
	{
		m_pService ? m_pService->SetGameTime(pTimer) : (void)NULL;
	}
	static DWORD GetTickCount()
	{
		return m_pService ? m_pService->GetTickCount() : GetTickCount();
	}
	static time_t StringToDateTime(CHAR* str)
	{
		int year, month, day, hour, minute, second;
		sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);

		tm tm_;
		memset(&tm_, 0, sizeof(tm));
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;

		time_t t_ = mktime(&tm_);
		return t_;
		
	}
	// 返回0~(nMax - 1)的随机数，如果nMax <= 0，返回均匀分布的32位随机数
	static DWORD Rand(INT nMax = 0)
	{
		return RG.IRandomMax(nMax);
		/*return m_pService != NULL ?
			m_pService->Rand(nMax) : (nMax <= 0 ? rand() :
			(DWORD)((INT64)rand() * nMax / RAND_MAX));*/
	}
    static DWORD Rand(INT nMin, INT nMax)
	{
		return RG.IRandomRange(nMin, nMax);
	}
	// 返回[0, 1)区间内随机小数
	static DOUBLE Random()
	{
		return RG.Random();
		/*return m_pService != NULL ?
			m_pService->Random() : (DOUBLE)rand() / RAND_MAX;*/
	}

	static IKBadwordFilter* CreateBadWordFilter()
	{
		return m_pService != NULL ?
			m_pService->CreateBadWordFilter() : NULL;
	}

	static INT ValidateStringGBK(CHAR* pString)
	{
		BYTE byChar;
		while ((byChar = *pString++))
		{
			if (byChar == 0xFF)
				return FALSE;
			if (byChar > 0x80)
			{
				BYTE bySecond = *pString++;
				if (bySecond == 0)
					break;
				if (bySecond == 0x7F)
					return FALSE; // 屏蔽0x7F扫描线
				if (bySecond < 0x40 || bySecond == 0xFF)
					return FALSE;
				// 禁止GBK用户自定义1/2/3区
				if (byChar >= 0xAA && byChar <= 0xAF && bySecond >= 0xA1 && bySecond <= 0xFE)
					return FALSE;
				if (byChar >= 0xF8 && byChar <= 0xFE && bySecond >= 0xA1 && bySecond <= 0xFE)
					return FALSE;
				if (byChar >= 0xA1 && byChar <= 0xA7 && bySecond >= 0x40 && bySecond <= 0xA0)
					return FALSE;
			}
			else
			{
				if ( ! isalnum(byChar))
					return FALSE;
			}
		}
		return TRUE;

	}
	//获取两个时间戳之间相隔的天数
	static INT GetDayDiff(time_t tBegin,time_t tEnd)
	{
		return m_pService->GetTimeDiffOfDay( tBegin, tEnd);
	}

	
private:
	static IKSysService* m_pService;
};
