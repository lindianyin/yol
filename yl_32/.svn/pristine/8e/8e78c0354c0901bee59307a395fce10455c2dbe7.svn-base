
#pragma once

#include <iostream>

// -------------------------------------------------------------------------

#define NOTHING	0

#if defined(_DEBUG) && defined(_WINDOWS_)
#define _KDbg(statement)				statement
#else
#define _KDbg(statement)				NOTHING
#endif

#define _KDbgDump						_KDbg(std::cout) //这个不能直接用

/////////////////// 调试输出宏 //////////////////////////////////////

#define KTrace(stream)					_KDbg(_KDbgDump << stream << std::endl)
#define KTraceInline(stream)			_KDbg(_KDbgDump << __FILE__ << '(' << __LINE__ << "): " << stream << std::endl)

#ifdef __linux
#define KTraceback(stream)									\
	do														\
	{														\
		KTrace(stream);										\
		KTRACEBACK_INFO sInfo;								\
		KD_TRACEBACK(sInfo);								\
		for (INT i = 0; i < sInfo.nSize; ++i)				\
			_KDbgDump << sInfo.ppszString[i] << std::endl;	\
	}														\
	while (0)
#else
#define KTraceback(stream)				KTrace(stream)		/* TODO: WIN32版未实现 */
#endif

////////////////// 计时宏 ////////////////////////////////

#define CALC_T			INT64

inline CALC_T  _GetCycleCount()	//获取计时次数
{
#ifdef _WINDOWS_
	LARGE_INTEGER li;
	if (QueryPerformanceCounter(&li))
		return CALC_T(li.QuadPart);
#endif
	return (CALC_T)(0);
}

inline CALC_T  _GetFreq()	//获取计时频率
{
#ifdef _WINDOWS_
	LARGE_INTEGER li;
	if (QueryPerformanceFrequency(&li))
		return CALC_T(li.QuadPart);
#endif
	return (CALC_T)(1);
}

#define DECLARE_COUNTER(name)			_KDbg(CALC_T name;)
#define EXTERN_COUNTER(name)			_KDbg(extern CALC_T name;)
#define INIT_COUNTER(name)				_KDbg(name = _GetCycleCount())
#define COUNTER_GET(name)				_KDbg((UINT)(_GetCycleCount() - name))	//获取从INIT_COUNTER到COUNTER_GET的计数差
#define COUNTER_TIME_GET(name)			_KDbg(COUNTER_GET(name) / (DOUBLE)_GetFreq())	//获取时间差(秒)
//使用下面的宏一次过完成定义和初始化（通常与COUNTER_GET配合用在同一局部域）
#define COUNTER_START(name)				_KDbg(CALC_T name; INIT_COUNTER(name);)

#ifdef __linux

// 注：
//   想要带符号输出则需要在gcc编译选项中加上-rdynamic项，否则将只输出首地址；
//   在仅有地址的情况下，可以使用 nm jxex_linux | sort > symbols.txt 来获得符号表；
//   用grep等工具可以快速的定位符号；
//   符号是以C形式输出的，对于C++函数的显示会比较凌乱。

#include <execinfo.h>

#define KD_TRACEBACK(INFO)	__Traceback(INFO, 20)

struct KTRACEBACK_INFO
{
	KTRACEBACK_INFO() : ppszString(NULL), nSize(0) {}
	~KTRACEBACK_INFO() { free(ppszString); }
	LPSTR*	ppszString;
	INT		nSize;
};

inline VOID __Traceback(KTRACEBACK_INFO& rsInfo, INT nLevel)
{
	LPVOID apTmp[256];
	if (nLevel > countof(apTmp))
		nLevel = countof(apTmp);
	rsInfo.nSize = backtrace(apTmp, nLevel);
	rsInfo.ppszString = backtrace_symbols(apTmp, rsInfo.nSize);
}

#else

#define KD_TRACEBACK(INFO)		// TODO: WIN32版本未实现

struct KTRACEBACK_INFO
{
	// TODO: WIN32版本未实现
};

#endif

// -------------------------------------------------------------------------

