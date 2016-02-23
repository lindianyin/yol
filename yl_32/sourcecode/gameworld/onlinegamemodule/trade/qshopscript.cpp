#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "kbuysellmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

namespace QShopScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: TestBuyGood
	// @Description		: 测试购买商品
	// @ReturnCode		: 1、成功；nil、失败
	// @ArgumentFlag	: dddd
	// @ArgumentComment	: nPlayerIndex
	// @ArgumentComment	: nShopId
	// @ArgumentComment	: nGoodId
	// @ArgumentComment	: nNum
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaTestBuyGood(QLuaScript& sc)
	{
		INT nPlayerIndex = sc.GetInt(1);
		INT nShopId	= sc.GetInt(2);
		INT nGoodId	= sc.GetInt(3);
		INT nNum	= sc.GetInt(4);

		KPLAYER_BUY_ITEM_SYNC cBuyItem;
		cBuyItem.nShopId = nShopId;
		cBuyItem.byBuyIdx = (BYTE)nGoodId;
		cBuyItem.wCount = (WORD)nNum;
		BOOL bRet = g_cBuySellMgr.Buy(nPlayerIndex, &cBuyItem);

		sc.PushNumber(bRet);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: TestSellGood
	// @Description		: 测试销售商品
	// @ReturnCode		: 1、成功；nil、失败
	// @ArgumentFlag	: dddd
	// @ArgumentComment	: nPlayerIndex
	// @ArgumentComment	: nShopId
	// @ArgumentComment	: nGoodId
	// @ArgumentComment	: nNum
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaTestSellGood(QLuaScript& sc)
	{
		INT nPlayerIndex = sc.GetInt(1);
		INT nShopId	= sc.GetInt(2);
		INT nGoodId	= sc.GetInt(3);
		INT nNum	= sc.GetInt(4);

		KPLAYER_SELL_ITEM_SYNC cSellItem;
		cSellItem.nShopId = nShopId;
		cSellItem.dwId = (BYTE)nGoodId;
		cSellItem.wCount = (WORD)nNum;
		BOOL bRet = g_cBuySellMgr.Sell(nPlayerIndex, &cSellItem);

		sc.PushNumber(bRet);

		return 1;
	}

	#include "qshopscript_table.hpp"
}

BOOL g_RegisterShopScript()
{
	return g_cScriptManager.RegisterTableFuncs("QShop", QShopScript::arFunction, countof(QShopScript::arFunction));
}