
#pragma once

// -------------------------------------------------------------------------

#if defined(__AFX_H__) // Uses MFC
#undef TRACE
#undef ASSERT
#undef VERIFY
#endif

#ifdef WIN32
#define	_CRT_SECURE_NO_DEPRECATE
#define	_CRT_NONSTDC_NO_DEPRECATE
#define	_USE_32BIT_TIME_T
//#define DECLSPEC_DEPRECATED

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT    0x0502 // windows 2003 sp1 / windows xp sp2

#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <mmsystem.h>
#endif

#include "Runtime/Debug.h"
#include "Runtime/Stddef.h"
#include "Runtime/QRT.h"


//-----------------------------------------------------------
//	辅助释放对象/缓冲区宏定义
//-----------------------------------------------------------
#ifndef SAFE_FREE
	#define SAFE_FREE(a)	if (a) {free(a); (a)=NULL;}
#endif
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(a)	if (a) {(a)->Release(); (a)=NULL;}
#endif
#ifndef SAFE_DELETE
	#define SAFE_DELETE(a)	if (a) {delete (a); (a)=NULL;}
#endif
#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(a)	if (a) {delete[] (a); (a)=NULL;}
#endif

#define COMBO_2SHORT(a, b)	((long)(((short)((a) & 0xffff)) | ((long)((short)((b) & 0xffff))) << 16))
#define LOSHORT(l)			((short)((l) & 0xffff))
#define HISHORT(l)			((short)((l) >> 16))


#include <assert.h>

#ifndef ASSERT
#define ASSERT assert
#endif

#ifdef WIN32

#include <Unknwn.h>

#else	// #ifndef WIN32

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED

typedef long HRESULT;

#endif // !_HRESULT_DEFINED

#ifndef SUCCEEDED
#define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)
#endif

#ifndef FAILED
#define FAILED(Status) ((HRESULT)(Status) < 0)
#endif

#ifndef _HRESULT_TYPEDEF_
#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)
#endif

#endif	// #ifdef WIN32 #else


#define USE_ARGUMENT(arg) ((void)(arg))

#define PROCESS_ERROR(Condition) \
	do  \
{   \
	if (!(Condition))   \
	goto EXIT0;     \
} while (false)


#define PROCESS_SUCCESS(Condition) \
	do  \
{   \
	if (Condition)      \
	goto EXIT1;     \
} while (false)



#define PROCESS_ERROR_RET_CODE(Condition, Code) \
	do  \
{   \
	if (!(Condition))       \
{                       \
	nResult = Code;     \
	goto EXIT0;         \
}                       \
} while (false)

#define PROCESS_ERROR_RET_COM_CODE(Condition, Code) \
	do  \
{   \
	if (!(Condition))       \
{                       \
	hrResult = Code;     \
	goto EXIT0;         \
}                       \
} while (false)


#define COM_PROCESS_ERROR(Condition) \
	do  \
{   \
	if (FAILED(Condition))  \
	goto EXIT0;         \
} while (false)


#define COM_PROCESS_SUCCESS(Condition)   \
	do  \
{   \
	if (SUCCEEDED(Condition))   \
	goto EXIT1;             \
} while (false)


// KG_COM_PROCESS_ERROR_RETURN_ERROR
#define COM_PROC_ERR_RET_ERR(Condition)  \
	do  \
{   \
	if (FAILED(Condition))      \
{                           \
	hrResult = Condition;   \
	goto EXIT0;             \
}   \
} while (false)


#define COM_PROC_ERROR_RET_CODE(Condition, Code)     \
	do  \
{   \
	if (FAILED(Condition))  \
{                       \
	hrResult = Code;    \
	goto EXIT0;         \
}                       \
} while (false)


// =========================================================================
#define CHECK_REPORT2(result, expr)										\
	if (!(result))															\
{																		\
	(void)(expr);																\
	REPORT_ONCE(L"_CHECK_BOOLEX(" L ## #result L", " L ## #expr L");"); \
	goto EXIT0;														\
}
#define CHECK_REPORT(result)			CHECK_REPORT2(result, NULL)
#define COM_CHECK_REPORT(hr)					CHECK_REPORT(SUCCEEDED(hr))



#if _MSC_VER >= 1200            // >= VS 6.0
#   define Q_LITTLE_ENDIAN	    1234
#   define Q_BIG_ENDIAN	    4321
#   define Q_PDP_ENDIAN	    3412
#   define Q_BYTE_ORDER        _LITTLE_ENDIAN  
#else
//#ifdef __FreeBSD__
//#include <sys/endian.h>
//#elif defined(__APPLE__)
//#include <sys/_endian.h>
//#else
//#include <endian.h>
//#endif
//	#define Q_LITTLE_ENDIAN     __LITTLE_ENDIAN
//	#define Q_BIG_ENDIAN        __BIG_ENDIAN
//	#define Q_PDP_ENDIAN        __PDP_ENDIAN
//	#define Q_BYTE_ORDER        __BYTE_ORDER

#endif

#if defined(__APPLE__) && defined(__MACH__)
#define unix
#endif
