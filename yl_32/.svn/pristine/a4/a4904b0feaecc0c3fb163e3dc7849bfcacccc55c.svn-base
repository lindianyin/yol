/* -------------------------------------------------------------------------
//	文件名		：	qmysteryshopmodule.h
//	创建者		：	NiuNiu
//	创建时间	：	2012-10-30 17:13:00
//	功能描述	：  
// -------------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegameworld/kplayer.h"

#include "onlinegameworld/kplayermgr.h"

#include "qsellmgr.h"
#include "qmysteryshopmodule.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//-----------------------------------------------------------------------
template<>
IKModuleInterface* GetModuleInterFace<emMODULE_MYSTERYSHOP>()
{
	static QMysteryShopModule s_m;
	return &s_m;
}

QMysteryShopModule::QMysteryShopModule() : KModuleBase(emMODULE_MYSTERYSHOP)
{

}

BOOL QMysteryShopModule::OnInit()
{
	/*
	BOOL bOK = g_cSellMgr.Init();
	return bOK;
	*/
	return TRUE;
}

BOOL QMysteryShopModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(QC2S_MYSTERY_SHOP_SYNC_BUY_ITEM);
	KD_PTC_FIXED_SIZE_C2S(QC2S_MYSTERY_SHOP_SYNC_REFRESH);

	return TRUE;
}

BOOL QMysteryShopModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	return g_cSellMgr.ProtocolProcess(pbData, uDataLen, *pPlayer);
}
