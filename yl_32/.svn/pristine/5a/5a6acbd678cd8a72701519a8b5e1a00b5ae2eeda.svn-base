#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "qpursesetting.h"
namespace KPurseScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetExchangeRate
	// @Description		: ∂“ªª¿‡–Õ
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: ª„¬ 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetExchangeRate(QLuaScript &sc)
	{
		INT nType = sc.GetInt(1);
		CONST INT nRate = g_cPursesetting.RequestExchangeRate(nType);
		sc.PushNumber(nRate);

		return 1;
	}


#include "kpursescript_table.hpp"
}

BOOL g_RegisterPurseScript()
{
	return g_cScriptManager.RegisterTableFuncs("KPurse", KPurseScript::arFunction, countof(KPurseScript::arFunction));
}