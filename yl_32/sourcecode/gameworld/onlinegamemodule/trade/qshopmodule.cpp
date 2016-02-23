/* -------------------------------------------------------------------------
//	文件名		：	qshopmodule.cpp
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-10-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegameworld/kplayer.h"

#include "onlinegameworld/kplayermgr.h"

#include "kbuysellmgr.h"
#include "qshopmodule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
QShopModule g_cShopModule;
template<>
IKModuleInterface* GetModuleInterFace<emMODULE_SHOP>()
{
	//QShopModule& _m = GetShopModule();
	//return &_m;
	return &g_cShopModule;
}

QShopModule::QShopModule() : KModuleBase(emMODULE_SHOP)
{}

BOOL QShopModule::OnInit()
{
	
	BOOL bOK = g_cBuySellMgr.Init();
	//return bOK;
	
	return TRUE;
}

BOOL QShopModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(KPLAYER_BUY_ITEM_SYNC);
	KD_PTC_FIXED_SIZE_C2S(KPLAYER_SELL_ITEM_SYNC);
	KD_PTC_FIXED_SIZE_C2S(KPLAYER_RESIZE_EXTBAG);

	KD_PTC_FIXED_SIZE_C2S(KSHOP_C2S_DISCOUNT_SYNC);
	KD_PTC_LENGTNED_SIZE_S2C(KSHOP_S2C_DISCOUNT_SYNC);
	KD_PTC_LENGTNED_SIZE_S2C(KSHOP_S2C_ALL_PURCHASE_SYNC);
	KD_PTC_LENGTNED_SIZE_S2C(KSHOP_S2C_MY_PURCHASE_SYNC);

	return TRUE;
}


#ifdef GAME_SERVER

BOOL QShopModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	return g_cBuySellMgr.ProtocolProcess(pbData, uDataLen, *pPlayer);
}

#endif

// -------------------------------------------------------------------------
