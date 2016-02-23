
#pragma once

#include "Misc/randomgen.h"
// -------------------------------------------------------------------------
// 时间管理，用于控制游戏时间，游戏时间可以动态设定，不再受操作系统时间控制
class KDateTimeControl
{
public:
	KDateTimeControl();
	BOOL Init();
	// 获取从启动到现在毫秒数，49.7天后溢出，只是计算短时间内（24.5天内）差值时可用这个
	UINT GetGameElapse();
	// 获取从启动到现在真实毫秒数，不溢出
	INT64 GetRealGameElapse();
	// 获取游戏内time()值
	time_t GetGameTime(time_t* pTime);
	// 获取真正的系统时间time()值
	time_t GetSysTime(time_t* );
	// 每帧执行
	BOOL Loop();
	// 设置游戏时间
	BOOL SetGameTime(const time_t& tTime);
	// 获取随机数（32位）
	DWORD Rand();
protected:
	UINT m_uStartElapse;
	UINT m_uCurElapse;
	UINT m_uCurElapseHigh;

	time_t m_tTimeAdjust; // 时间校准（与外部时间源的差值）
	time_t m_tCurGameTime;
	QRandGentor m_cRandGentor;
private:
};

