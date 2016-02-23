#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "qsellmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace QMysteryShopScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: TestGenrateGoods
	// @Description		: ≤‚ ‘π∫¬Ú…Ã∆∑
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: ÕÊº“id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaTestGenrateGoods(QLuaScript& sc)
	{
		INT nPlayerIdx = sc.GetInt(1);
		g_cSellMgr.GenrateGoods(nPlayerIdx);
		return 1;
	}

	#include "qmysteryshopscript_table.hpp"
};

BOOL g_RegisterMysteryShopScript()
{
	return g_cScriptManager.RegisterTableFuncs("QMysteryShop", QMysteryShopScript::arFunction, countof(QMysteryShopScript::arFunction));
}
