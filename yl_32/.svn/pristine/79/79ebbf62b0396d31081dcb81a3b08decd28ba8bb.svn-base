

#include "stdafx.h"

#include "onlinegamebase/kscriptmanager.h"
#include "onlinegamebase/kjxexunify.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
namespace KUnifyScriptNameSpace
{
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: IsNameWordPass
// @Description		: 检查名字中的单字是否符合要求
// @ReturnCode		: 1 or 0
// @ArgumentFlag	: s
// @ArgumentComment	: 名字
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaIsNameWordPass(QLuaScript& sc)
{
	LPCSTR szName = sc.GetStr(1);
	sc.PushNumber(g_piNameWordFilter->IsNamePass(szName));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: EnableLeakDectect
// @Description		: [Windows及Debug版下有效]开启/关闭内存泄露检测（主线程）
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 非0开启其他关闭
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaEnableLeakDectect(QLuaScript& sc)
{
#if defined _WINDOWS_ && defined _DEBUG
	/*if (sc.GetInt(1))
		VLDEnable();
	else
		VLDDisable();*/
#endif
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: MathRandom
// @Description		: 获得一个随机数
// @ReturnCode		: 随机数
// @ArgumentFlag	: [dd]
// @ArgumentComment	: 如果无参数返回[0,1)，如果一个参数返回[1,max]
// @ArgumentComment	: 如果两个参数返回[min,max]
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaMathRandom(QLuaScript& sc)
{
	INT nParamCount = sc.GetTopIndex();
	if (nParamCount == 0)
	{
		sc.PushNumber(KSysService::Random());
	}
	else if (nParamCount == 1)
	{
		DWORD nRet = 0;
		INT nMax = sc.GetInt(1);
		if (nMax > 0)
		{
			nRet = KSysService::Rand(nMax) + 1;
		}
		sc.PushNumber(nRet);
	}
	else if (nParamCount >= 2)
	{
		INT nMin = sc.GetInt(1);
		INT nMax = sc.GetInt(2);		
		INT nRet = KSysService::Rand(nMin, nMax);
		sc.PushNumber(nRet);
	}
	return 1;
}

#include "kunifyscript_table.hpp"
} // KUnifyScriptNameSpace

BOOL g_RegisterKUnifyScriptFun()
{
	return g_cScriptManager.RegisterTableFuncs("KUnify", KUnifyScriptNameSpace::arFunction, countof(KUnifyScriptNameSpace::arFunction));
}

