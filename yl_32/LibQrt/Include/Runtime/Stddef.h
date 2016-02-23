
#pragma once


#if defined(WIN32)

	#ifndef _WINDOWS_ //_WINDEF_
		#define _WIN32_WINNT  0x0502
		#define WIN32_LEAN_AND_MEAN
		#include <winsock2.h>
		#include <mswsock.h>
		#include <windows.h>
	#endif

	#include <objbase.h>
	#include <stdlib.h>

	typedef ULONG REFERENCE;
	#include "TypeDef.h"

#else // if defined(WIN32)

	#include "TypeDef.h"

	#include <stdlib.h>
	#include <string.h>
	#include <wchar.h>

	#if defined(__BORLANDC__)
	typedef LONG REFERENCE;
	#else
	typedef ULONG REFERENCE;
	#endif

#endif

// -------------------------------------------------------------------------

#ifdef __GNUC__
typedef long long		INT64;
#endif

// 补全指针类型定义
typedef const BYTE*		LPCBYTE;
typedef CHAR*			LPCHAR;
typedef CONST CHAR*		LPCCHAR;
typedef UCHAR*			LPUCHAR;
typedef CONST UCHAR*	LPCUCHAR;
typedef SHORT*			LPSHORT;
typedef CONST SHORT*	LPCSHORT;
typedef USHORT*			LPUSHORT;
typedef CONST USHORT*	LPCUSHORT;
typedef CONST WORD*		LPCWORD;
typedef CONST INT*		LPCINT;
typedef UINT*			LPUINT;
typedef CONST UINT*		LPCUINT;
typedef CONST DWORD*	LPCDWORD;
typedef INT64*			LPINT64;
typedef CONST INT64*	LPCINT64;
typedef CONST BOOL*		LPCBOOL;
typedef FLOAT*			LPFLOAT;
typedef CONST FLOAT*	LPCFLOAT;
typedef DOUBLE*			LPDOUBLE;
typedef CONST DOUBLE*	LPCDOUBLE;

// -------------------------------------------------------------------------
// --> Platform(Compiler) Cofiguration

#if defined(__GNUC__)
	//#define __stdcall __attribute__((stdcall))
	#define __stdcall 
	#define __cdecl
#endif

// =========================================================================

#if !defined(EXTERN_C)
#ifdef __cplusplus
#define EXTERN_C				extern "C"
#else
#define EXTERN_C
#endif
#endif

#ifndef APIENTRY
#define APIENTRY				__stdcall
#endif

#ifndef STDAPI
#define STDAPI                  EXTERN_C HRESULT __stdcall
#define STDAPI_(type)           EXTERN_C type __stdcall
#endif

#ifndef STDMETHOD
#define STDMETHODCALLTYPE		__stdcall
#define STDMETHOD(method)       virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method
#define STDMETHODIMP            HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)     type STDMETHODCALLTYPE
#endif

#ifndef INLINEAPI
#define INLINEAPI				inline HRESULT __stdcall
#define INLINEAPI_(type)		inline type __stdcall
#endif

#ifndef CDECLAPI
#define CDECLAPI				inline HRESULT __cdecl
#define CDECLAPI_(type)			inline type __cdecl
#endif

#ifndef EXPORTAPI
#define EXPORTAPI				STDAPI __export
#define EXPORTAPI_(type)		STDAPI_(type) __export
#endif

#ifndef STDPROC
#define STDPROC					virtual HRESULT __stdcall
#define STDPROC_(Type)			virtual Type __stdcall
#endif

#ifndef PURE
#define PURE					= 0
#endif

#ifndef DECLARE_IID
#define DECLARE_IID(Interface)	EXTERN_C const GUID IID_##Interface;
#endif

#ifndef DECLARE_CLSID
#define DECLARE_CLSID(ComClass)	EXTERN_C const GUID CLSID_##ComClass;
#endif

#ifndef	countof
#define countof(array)					(sizeof(array)/sizeof(array[0]))
#endif


#ifndef FillMemory
#define FillMemory(p, len, c)			memset(p, c, len)
#define ZeroMemory(p, len)				memset(p, 0, len)
#define CopyMemory(dst, src, len)		memcpy(dst, src, len)
#define MoveMemory(dst, src, len)		memmove(dst, src, len)
#endif

#ifndef ZeroStruct
#define ZeroStruct(buf)					ZeroMemory(&(buf), sizeof(buf))
#endif

#ifndef TokenVal
#define TokenVal(p, type)				(*((type*&)(p))++)
#endif

#ifndef TokenSize
#define TokenSize(p, size)				(((const char*&)(p)) += (size))
#endif

#ifndef ROUND
#define ROUND(x, y)						(((x)+(y)-1) & ~((y)-1))
#endif

#ifndef MAX
	#define MAX(x, y)					((x) > (y) ? (x) : (y))
	#define MIN(x, y)					((x) < (y) ? (x) : (y))
#endif

#ifndef MAX_PATH
#define MAX_PATH		260
#endif

#ifndef _MAX_PATH
#define _MAX_PATH		MAX_PATH
#define _MAX_FNAME		256
#define _MAX_DRIVE		3
#define _MAX_DIR		256
#define _MAX_EXT		256
#endif

#ifndef _offsetof
#	if defined(__GNUC__)
#		define _offsetof(coclass, member)	( (size_t)&((coclass*)64)->member - 64 )
#	else
#		define _offsetof(coclass, member)	( (size_t)&((coclass*)0)->member )
#	endif
#endif


