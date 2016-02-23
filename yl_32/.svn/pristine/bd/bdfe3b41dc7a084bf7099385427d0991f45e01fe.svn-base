
#pragma once


#ifdef WIN32

//  [9/29/2010 simon]
// 以下三行不能调换顺序也别插入其他语句，否则要么导致_malloca、_freea出问题，要么不显示内存泄露文件名
#define	_CRTDBG_MAP_ALLOC	// 显示内存泄露所在的文件名和行数
#include <malloc.h>
#include <crtdbg.h>

#ifndef ASSERT
	#define ASSERT(x)	_ASSERTE(x)
#endif

#else

#endif

#ifndef DEBUG_NEW
#define DEBUG_NEW		new
#endif

// -------------------------------------------------------------------------
#if !defined(_DEBUG)

#ifndef ASSERT_ONCE2
#define ASSERT_ONCE2(e, msg)	(void)(e)
#endif

#ifndef ASSERT2
#define ASSERT2(e, msg)		(void)(e)
#endif

#ifndef ASSERT_
#define ASSERT_						NULLFN
#endif

#ifndef ASSERT_ONCE_
#define ASSERT_ONCE_				NULLFN
#endif

#ifndef REPORT_
#define REPORT_						NULLFN
#endif

#ifndef REPORT_ONCE_
#define REPORT_ONCE_				NULLFN
#endif

#else

#if defined(WIN32)
	#include <crtdbg.h>
	#ifndef ASSERT2
	#define ASSERT2(e, msg)												\
		do { if (!(e) &&												\
		(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL))) \
		_CrtDbgBreak(); } while (0)
	#endif //ASSERT2 - windows

#else //defined(WIN32)

#ifndef ASSERT2
#define ASSERT2(e, msg)		INL_ASSERT(e)
#endif

#endif // !defined(_DEBUG)

#ifndef ASSERT_ONCE2
#define ASSERT_ONCE2(e, msg)												\
	do {																	\
	if (!(e))															\
		{																	\
		static int fReported = 0;										\
		ASSERT2(fReported, msg);										\
		fReported = 1;													\
		}																	\
	} while(0)
#endif

#endif // defined(Release_Assert)
// -------------------------------------------------------------------------

#ifndef VERIFY
#	if defined(_DEBUG)
#		define VERIFY(e)			ASSERT(e)
#	else
#		define VERIFY(e)			e
#	endif
#endif

#ifndef VERIFY_OK
#define VERIFY_OK(hr)				VERIFY( SUCCEEDED(hr) )
#endif

#ifndef _DEBUG_ONLY
#	if defined(_DEBUG)
#		define _DEBUG_ONLY(e)		e
#	else
#		define _DEBUG_ONLY(e)		NULL
#	endif
#endif

#ifndef DEBUG_ONLY
#define DEBUG_ONLY(e)				_DEBUG_ONLY(e)
#endif

#ifndef ASSERT
#define ASSERT(e)					ASSERT2(e, NULL)
#endif

#ifndef ASSERT_OK
#define ASSERT_OK(hr)				ASSERT( SUCCEEDED(hr) )
#endif

#ifndef ASSERT_ONCE
#define ASSERT_ONCE(e)				ASSERT_ONCE2(e, #e)
#endif

#ifndef REPORT
#define REPORT(msg)					ASSERT2(FALSE, msg)
#endif

#ifndef REPORT_ONCE
#define REPORT_ONCE(msg)			ASSERT_ONCE2(FALSE, msg)
#endif

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

#if !defined(INL_ASSERT)
#	if !defined(_DEBUG)
#		define INL_ASSERT(e)				(void)NULL

#	elif defined(__GNUC__)
#		include <stdio.h> // <assert.h>
#		define INL_ASSERT(e)												\
	do { if (!(e))													\
	printf("!!!!!!!!!!!!!!Assert %s(%d)\n", __FILE__, __LINE__); } while(0)
#	else
#		define INL_ASSERT			ASSERT
#	endif
#endif


CORE_API_C void g_DebugLog(const char* Fmt, ...);

#ifdef _DEBUG
	#define QASSERT(x)	if (!(x)) g_DebugLog("Assert failed in %s, line = %i", __FILE__, __LINE__)
#else
	#define QASSERT(x)	(void)0
#endif

#ifndef _ASSERT
	#define _ASSERT(f)	QASSERT(f)
#endif


