#include "stdafx.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/knpc.h"
#include "qmysterygoods.h"
#include "item2/kitemmgr.h"
#include "qsellmgr.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "item2/kitem.h"
#include "kpursedata.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "onlinegameworld/korpgworld.h"

#ifdef GAME_SERVER
#include "item2/itemenv.h"
#include "onlinegameworld/klogwritter.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

#define	MYSTERYSHOP_BOX_WIDTH		5
#define	MYSTERYSHOP_BOX_HEIGHT		8

extern QLogFile g_NewLogFile;

QSellMgr g_cSellMgr;


QSellMgr::QSellMgr()
{
}

QSellMgr::~QSellMgr()
{

}

BOOL QSellMgr::Init()
{
	if (!m_cSetting.Init())
		return FALSE;

	return TRUE;
}

BOOL QSellMgr::Release()
{
	m_cSetting.Release();
	return TRUE;
}


#ifdef GAME_SERVER

BOOL QSellMgr::GenrateGoods(INT nPlayerIdx)
{
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
	QCONFIRM_RET_FALSE(pPlayer);

	KPlayerItem* pPlayerItem = (pPlayer->GetCtrl<KPlayerItem>());

	std::vector<INT> randIds = m_cSetting.GenerateGoodIds(QMYSTERY_GOOD_MAX);


	/*遍历删除神秘商店的物品*/
	KE_LOSE_ITEM_WAY nWay = emKLOSEITEM_NONE;
	KE_ITEM_DATARECORD_WAY eRecordWay = emITEM_DATARECORD_REMOVE;
	for (INT i = 0; i < KD_ROOM_STERY_SHOP_WIDTH; ++ i)
	{
		for (INT j = 0; j < KD_ROOM_STERY_SHOP_HEIGHT; ++ j)
		{
			INT nIdx = pPlayerItem->GetItemByPos(emROOM_MYSTERY_SHOP, i, j);
			if (nIdx > 0)
			{
				if (!pPlayerItem->Remove(nIdx, nWay))			//从玩家身上删除
					return 0;
				if (!g_cItemMgr.Remove(nIdx, eRecordWay))		//从游戏世界删除
					return 0;

			}
		}
	}
	/*for (size_t i = 0; i < randIds.size(); ++ i)
	{
		QMysteryGoods* pGood = m_cSetting.GetGoods(randIds[i]);
		m_mapGoodsInShop[pGood->GetId()] = *pGood;
		INT nCount = pGood->m_nCount;
		INT nPrice = pGood->m_nPrice;
		INT nItemIdx = g_cItemMgr.Add(pGood->m_sIndex.nGenre, pGood->m_sIndex.nDetailType,
			pGood->m_sIndex.nParticular, pGood->m_sIndex.nLevel, 0, emITEM_DATARECORD_SYSTEMADD);

		if (pPlayerItem->AddItemIntoSteryShop(nItemIdx, nCount, nPrice) < 0)
			return FALSE;


	}*/
	return TRUE;
}

#endif


BOOL QSellMgr::ProtocolProcess(LPCBYTE pData, INT nDataLen, KPlayer& rcPlayer)
{
	ASSERT(pData && nDataLen > 0);
	switch (*(pData + 1))
	{
	default:
		ASSERT(FALSE);
		return FALSE;
		break;
	}
	return TRUE;
}