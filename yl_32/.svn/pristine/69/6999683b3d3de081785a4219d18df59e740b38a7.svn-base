

#include "stdafx.h"
#include "onlinegamebase/ktimermgr.h"
#include "onlinegamebase/kscriptmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

extern KTimerMgr g_cTimerMgr;


// -------------------------------------------------------------------------

namespace KTimerMgrNamespace
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetFrame
	// @Description		: 获得当前游戏桢数
	// @ReturnCode		: 当前游戏桢数
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetFrame(QLuaScript& sc)
	{	
		sc.PushNumber(g_cTimerMgr.GetCurTime());
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: RegisterTimerPoint
	// @Description		: 注册定时器时间点，Timer类调用
	// @ReturnCode		: 执行时间点
	// @ArgumentFlag	: d
	// @ArgumentComment	: 多少帧后执行
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRegisterTimerPoint(QLuaScript& sc)
	{	
		UINT uFrame			= sc.GetInt(1);
		UINT uActiveFrame	= g_cTimerMgr.AddScriptTimePoint(uFrame);
		sc.PushNumber(uActiveFrame);
		return 1;
	}

#include "kluatimermgr_table.hpp"
}

BOOL g_RegisterTimerMgrScriptFun()
{
	return g_cScriptManager.RegisterTableFuncs("KTimer", KTimerMgrNamespace::arFunction, countof(KTimerMgrNamespace::arFunction));
}
