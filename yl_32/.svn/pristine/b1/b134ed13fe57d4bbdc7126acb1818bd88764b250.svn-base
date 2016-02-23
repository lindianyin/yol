
#pragma once

#include "Misc/namefilter_i.h"
#include "ksysservice.h"

// -------------------------------------------------------------------------
// 统一初始化
BOOL g_UnifyInit();
// 统一反初始化
BOOL g_UnifyUnInit();
// 每循环执行
BOOL g_UnifyLoop();

extern BOOL g_RegisterKUnifyScriptFun();

// 名字单词过滤
extern INameFilter* g_piNameWordFilter;

#if defined _WINDOWS_ && defined _DEBUG

VOID g_RunTimeMemLeakDetect(UINT uInterval);
INT g_RunTimeMemLeakDetectInit();

#endif

