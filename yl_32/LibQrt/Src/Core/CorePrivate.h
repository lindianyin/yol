
#pragma once


#ifdef WIN32
	#define	_CRT_SECURE_NO_DEPRECATE
	#define	_CRT_NONSTDC_NO_DEPRECATE
	#define	_USE_32BIT_TIME_T
	//#define DECLSPEC_DEPRECATED

	#define WIN32_LEAN_AND_MEAN
	#define _WIN32_WINNT    0x0502 // windows 2003 sp1 / windows xp sp2

	#include <winsock2.h>
	#include <Windows.h>
	#include <mmsystem.h>
	#include <process.h>

	#define _CRT_SECURE_NO_WARNINGS
#else
	#include <pthread.h>
	#include <signal.h>
	#include <unistd.h>
	#define SOCKET_ERROR -1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "CoreDeclspec.h"
#include "CommonDefine.h"

#include "Core/QLog.h"
#include "Misc/utilities.h"
