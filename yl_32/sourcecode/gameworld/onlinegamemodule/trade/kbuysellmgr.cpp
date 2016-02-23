#include "stdafx.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/knpc.h"
#include "kgoods.h"
#include "item2/kitemmgr.h"
#include "kbuysellmgr.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "item2/kitem.h"
#include "kpursedata.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "serverbase/RecordDef.h"
#include "../record/RecordRelayClient.h"
#include "Runtime/TypeDef.h"

#ifdef GAME_SERVER
#include "item2/itemenv.h"
#include "onlinegameworld/klogwritter.h"
#include "qshopmodule.h"
#include "qplayershop.h"

#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

#define	SHOP_BOX_WIDTH		5
#define	SHOP_BOX_HEIGHT		8

#define SHOP_INSIDE_SHOP_ID  178

extern QLogFile	g_NewLogFile;
KBuySellMgr	g_cBuySellMgr;

INT64 g_i64BuySellTotal = 0;
INT64 g_i64BuySellTotal_FuYuan = 0;
INT64 g_i64BuySellTotal_Coin = 0;
INT64 g_i64BuySellTotal_Score = 0;
INT64 g_i64BuySellTotal_Contribution = 0;
INT64 g_i64BuySellTotal_Honour = 0;
//KTabFile KBuySellMgr::ms_ItemBusinessShopSetting;

LPCSTR cszCurrencyType[EM_MONEY_MAX] = {0};

KBuySellMgr::KBuySellMgr()
{
	cszCurrencyType[EM_MONEY_COIN]				= "COIN";
	cszCurrencyType[EM_MONEY_SILVER]			= "SILVER";
	cszCurrencyType[EM_MONEY_YLENERGY]			= "CYLENERGY";
	cszCurrencyType[EM_MONEY_SPIRIT]			= "SPIRIT";
	cszCurrencyType[EM_MONEY_PRESTIGE]			= "PRESTIGE";
	cszCurrencyType[EM_MONEY_CONTRIBUTION]		= "CONTRIBUTION";
	cszCurrencyType[EM_MONEY_MPJJ_SCORE]		= "MPJJ_SCORE";
	cszCurrencyType[EM_MONEY_JINGPO]			= "JINGPO";
}

KBuySellMgr::~KBuySellMgr()
{

}

BOOL KBuySellMgr::Init()
{
	RecordRelayClient::DoGetServerRecordRequest(emRecordBuyAllPurchase);
	if (!m_cSetting.Init())
		return FALSE;
#ifdef GAME_SERVER	
	//InitItemSaleLog();
#endif
	return TRUE;
}

BOOL KBuySellMgr::Release()
{
	m_cSetting.Release();
	return TRUE;
}

BOOL KBuySellMgr::OnGetAllBuyPurchaseRecordRespond(BYTE* pRecordData, size_t uRecordDataLen)
{
	QLogPrintf(LOG_INFO,"..KBuySellMgr::OnGetAllBuyPurchaseRecordRespond uRecordDataLen=%d..",uRecordDataLen);
	BOOL		bResult = FALSE;
	BYTE*		pBuffer = pRecordData;
	size_t		uPakSize = uRecordDataLen;

	std::vector<KPlayer*>			vPlayers;

	m_vecBuyAllPurchaseRecord.clear();

	while(uPakSize >= sizeof(KBuyAllPurchaseRecord))
	{
		KBuyAllPurchaseRecord sRecord;
		memcpy((VOID*)&sRecord, pBuffer, sizeof(KBuyAllPurchaseRecord));
		m_vecBuyAllPurchaseRecord.push_back(sRecord);
		pBuffer += sizeof(KBuyAllPurchaseRecord);
		uPakSize -= sizeof(KBuyAllPurchaseRecord);
	}

	g_cPlayerMgr.GetAllPlayers(vPlayers);
	for (std::vector<KPlayer*>::iterator it = vPlayers.begin(); it != vPlayers.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		PlayerShop *pPlayerShop = pPlayer->GetCtrl<PlayerShop>();
		LOG_PROCESS_ERROR(pPlayerShop);
		pPlayerShop->SyncAllPurchaseRecord();
	}
	LOG_PROCESS_ERROR(uPakSize == 0);

	bResult = TRUE;
EXIT0:
	return bResult;
}

KBuyAllPurchaseRecord* KBuySellMgr::GetAllPurchaseRecord(INT nShopId, INT nCategoryId, INT nGoodsId)
{
	KBuyAllPurchaseRecord *pRecord = NULL;
	for (std::vector<KBuyAllPurchaseRecord>::iterator it = m_vecBuyAllPurchaseRecord.begin(); it != m_vecBuyAllPurchaseRecord.end(); ++it)
	{
		if (it->nShopId == nShopId && it->nCategoryId == nCategoryId && it->nGoodsId == nGoodsId)
		{
			pRecord = &(*it);
			break;
		}
	}
	return pRecord;
}


// -------------------------------------------------------------------------
// 函数		: KBuySellMgr::IsGoodsOverdue
// 功能		: 
// 返回值	: BOOL Ture->过期，　FALSE->不过期
// 参数		: KGoods* pGoods
// 附注		: 判断指定的商品是否过期
// -------------------------------------------------------------------------
//BOOL KBuySellMgr::IsGoodsOverdue(KGoods* pGoods)
//{
//	if(NULL == pGoods)
//		return TRUE;
//
//	struct tm* pStm = NULL;
//	time_t t;
//	t = g_ServerTime.GetCurLocalTime();
//	tm _tempTm;
//	g_ServerTime.LocalTime(&t, &_tempTm);
//	pStm = &_tempTm;
//
//}
#ifdef GAME_SERVER

VOID KBuySellMgr::SetGoodsTimeout(CONST KGoods* pGoods, INT nItemIdx) CONST
{
	//处理商品时效问题
	DWORD dwTimeout = pGoods->GetTimeout();
	if (dwTimeout > 0)
	{	
		time_t tTime	= KSysService::Time(NULL);
		KITEM_TIMEOUT sTimeOut;
		sTimeOut.dwTimeout	= tTime + dwTimeout;
		sTimeOut.emType		= emKITEM_TIMEOUTTYPE_ABSOLUTE;
		KItem* pItem = &Item[nItemIdx];
		pItem->SetTimeout(&sTimeOut);
	}
}


// 帮会商店特殊购买，需要从gc申请扣帮会建设资金
BOOL KBuySellMgr::Buy_Sync(INT nPlayerIdx, KMONEYTYPE nCurrencyType, INT nBuy, INT nBuyIndex, INT nCost, INT nCount)
{
	_ASSERT(nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER);

/*	KPlayer* pPlayer = g_cPlayerMgr.GetById(nPlayerIdx);

	INT nIdx = GetGoodsIndex(nBuy, nBuyIndex);
	if (nIdx < 0)
		return FALSE; 
	KGoods *pGoods = m_cSetting.GetGoods(nIdx);
	if (!pGoods)
		return FALSE;

	INT nItemIdx = 0;
	INT nResultItemIdx = 0;*/	// 加到身上的物品索引（可能和nItemIdx不同）

#if 0
	switch (nCurrencyType)
	{
	case CURRENCYTYPE_TONG_FUND:
		{
			INT nMaxLoop = nCount;		// 防止死循环
			while (nMaxLoop > 0 && nCount > 0)
			{
				nItemIdx = CreateGoods(pPlayer, pGoods, nCurrencyType, nCount);
				if (nItemIdx <= 0)
					return	FALSE;
				
				Item[nItemIdx].SetGenInfo(KItem::emKITEM_GENINFO_OWNER_TONGID - 1, pPlayer->m_cPlayerKinTong.GetTongId()); // 与帮会绑定

				nResultItemIdx = pPlayer->m_cPlayerItem.AddItemIntoBag(nItemIdx, emKADDITEM_BUY, TRUE);
				if (!nResultItemIdx)
				{	// 加到人身上失败，从游戏世界中删掉
					RemoveGoods(nItemIdx);
					return	FALSE;
				}
				nMaxLoop --;
			}
		}
		break;
	default:
		return FALSE;
	}
#endif

	return TRUE;
}

BOOL KBuySellMgr::Buy(INT nPlayerIdx, KPLAYER_BUY_ITEM_SYNC *pCmd)
{
	_ASSERT(nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	
	INT nBuy = pCmd->nShopId;
	/*INT nIdx = GetGoodsIndex(nBuy, pCmd->byBuyIdx);
	if (nIdx < 0)
		return FALSE; 
	KGoods *pGoods = m_cSetting.GetGoods(nIdx);*/
	KGoods *pGoods = m_cSetting.GetGoodsByIndex(pCmd->nShopId, pCmd->byCategoryId, pCmd->byBuyIdx);
	if (!pGoods)
		return FALSE;

	//KCURRENCYTYPE nCurrencyType = pBuySellInfo->m_eCurrencyType;
	INT nItemIdx = 0;
	INT nPrice = 0;
	INT nSinglePrice = 0;
	INT nCount = pCmd->wCount;
	//INT nDealCount = nCount;
	INT nResultItemIdx = 0;	// 加到身上的物品索引（可能和nItemIdx不同）
	// Added by dengyong 20100304, 物品产出途径修改
	// 默认从商店购买的物品获得方式是emKADDITEM_BUY，但是从内部商店购买的物品产出途径为emKADDITEM_BUY_INNER_SHOP
	KE_ADD_ITEM_WAY eItemAddWay = emKADDITEM_BUY;
	LPCTSTR szGoodsName = pGoods->GetName();	//记录名字，不用每添加一个就发一次获得的消息，减少客户端收报数量

	INT tmpCount = nCount;
	std::vector<INT> vecItemIdx;
	vecItemIdx.clear();
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	//判断背包
	while (nCount > 0)
	{
		INT nPreCount = nCount;
		nItemIdx = CreateGoods(pPlayer, pGoods, (KMONEYTYPE)pGoods->m_nCurrencyType, nCount);
		if (nItemIdx <= 0)
			return	FALSE;
		nPrice = pGoods->m_nPrice * pGoods->m_nDiscount / 100;

		KItem& rItem = Item[nItemIdx];
		
		if (!pPlayerItem->CanAddItemIntoBag(Item[nItemIdx].GetIndex(), Item[nItemIdx].GetBindType(), nPreCount))
		{
			RemoveGoods(nItemIdx);
			g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
				pPlayer->GetId(), "DEBUG", LANG_019);
			return FALSE;
		}

		vecItemIdx.push_back(nItemIdx);
	}

	//判断限购
	INT nOldMoney = pPlayerPurse->GetMoney(pGoods->m_nCurrencyType);
	if (nOldMoney < (nPrice * tmpCount))
		return FALSE;
	PlayerShop *pPlayerShop = pPlayer->GetCtrl<PlayerShop>();
	if (pPlayerShop->DoBuyGoods(pCmd->nShopId, pCmd->byCategoryId, pCmd->byBuyIdx, pCmd->wCount)==FALSE)
		return FALSE;

	//判断货币
	if (tmpCount > 0)
	{
		KE_PURSE_RESULT eResult;
		if (!pPlayerPurse->AddMoney(pGoods->m_nCurrencyType, -(nPrice * tmpCount), &eResult))
		{
			if (pGoods->m_nCurrencyType == (INT)EM_MONEY_COIN)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_020);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_SILVER)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_021);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_SPIRIT)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_022);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_CONTRIBUTION)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_023);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_PRESTIGE)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_024);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_MPJJ_SCORE)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_025);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_JINGPO)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_026);
			}
			else
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_027);
			}

			for (std::vector<INT>::iterator it = vecItemIdx.begin(); it != vecItemIdx.end(); ++it)
				RemoveGoods(*it);
			return FALSE;
		}
		//tmpCount = 0;
	}

	for (std::vector<INT>::iterator it = vecItemIdx.begin(); it != vecItemIdx.end(); ++it)
	{
		//if (pGoods->IsGetBind())
		//	Item[nItemIdx].SetBind(TRUE);
		nResultItemIdx = pPlayerItem->AddItemIntoBag(*it, eItemAddWay, FALSE);

		if (!nResultItemIdx)
		{	// 加到人身上失败，从游戏世界中删掉
			RemoveGoods(*it);
			return	FALSE;
		}

		if (!BuyCallBack(nPlayerIdx, *it, nPrice))
		{
			pPlayerItem->Remove(*it, emKLOSEITEM_BUYFAIL);
			RemoveGoods(*it);
			return FALSE;
		}

		WriteLog(
			nPlayerIdx,
			pGoods,
			nPrice,
			(KMONEYTYPE)pGoods->m_nCurrencyType,
			nBuy + 1,
			BUY, 
			pGoods->m_nCurrencyType == (INT)EM_MONEY_COIN
			);

		//pGoods->OnSale((KMONEYTYPE)pGoods->m_nCurrencyType, nPreCount - nCount);

		g_i64BuySellTotal += nPrice;
	}
	pGoods->OnSale((KMONEYTYPE)pGoods->m_nCurrencyType, tmpCount);
	//g_piStatLog->ModifyAdd(KD_STTAB_JXB, G_STR_KBUYSELLMGR_BUY, G_STR_KBUYSELLMGR_AMOUNT, nPrice);	

	if(tmpCount > 0)
	{
		CHAR szBuffer[MAX_PATH];
		snprintf(szBuffer, sizeof(szBuffer), LANG_028, tmpCount, pGoods->GetName());
		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
			pPlayer->GetId(), "DEBUG", szBuffer);
		KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;

		//商店只统计元宝消耗   by ldy 2015/08/13
		if(pGoods->m_nCurrencyType == EM_MONEY_COIN)
		{
			pPlayerPurse->LogMoney(EM_MONEY_COIN,-(nPrice * tmpCount),50,&nResult,1,(char *)pGoods->GetName());
		}
		CHAR szParam[MAX_PATH];
		ZeroMemory(szParam,sizeof(szParam));
		sprintf(szParam,"%d#%d#%d",(DWORD)pCmd->nShopId,(DWORD)pCmd->byCategoryId,(DWORD)pCmd->byBuyIdx);
		cSafeScript->CallTableFunction( "DailyTasks", "commitTaskValue", 0, "osdd",pPlayer->GetScriptInterface(),szParam,3,tmpCount);
	}

	//nDealCount -= nCount;
	//if (nDealCount > 0 )	//加完道具后统一通知，减少客户端收包数量，加快速度,by Egg
	//{
		//KScriptFunctionCallHelper sfcHelper(*g_pMainScript);
		//sfcHelper.SetMe(pPlayer);
		//g_pMainScript->CallTableFunction("Player","BuyGoodsMsg",0,"ds",nDealCount,szGoodsName);
	//}
	// 记录日志
	/*
	if (nDealCount > 0 && pGoods->IsSaleLogRequired())
	{
		KLogWritter::GetInstance()->WriteLog_BuySell_Buy(
			&Player[nPlayerIdx],
			&Item[nItemIdx],
			nDealCount,
			pBuySellInfo->m_eCurrencyType,
			nPrice
		);
	}
	*/


	return TRUE;
}

//处理全服限购，从cr返回时调用
BOOL KBuySellMgr::Buy_Purchase_Limit(INT nPlayerIdx, R2S_BUY_ALL_PURCHASE_REQUEST *pCmd)
{	
	QLogPrintf(LOG_INFO,"..KBuySellMgr::Buy_Purchase_Limit(INT nPlayerIdx=%d, R2S_BUY_ALL_PURCHASE_REQUEST *pCmd->nCount=%d)",
		nPlayerIdx,pCmd->nCount);
	_ASSERT(nPlayerIdx > 0 && nPlayerIdx < MAX_PLAYER);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);

	INT nBuy = pCmd->nShopId;
	KGoods *pGoods = m_cSetting.GetGoodsByIndex(pCmd->nShopId, pCmd->nCategoryId, pCmd->nGoodsId);
	if (!pGoods)
		return FALSE;

	if (pGoods->m_maxCount < pCmd->nCount + pCmd->nBuyCount)
	{
		return FALSE;
	}
	

	INT nItemIdx = 0;
	INT nPrice = 0;
	INT nSinglePrice = 0;
	INT nCount = pCmd->nBuyCount;
	//INT nDealCount = nCount;
	INT nResultItemIdx = 0;	// 加到身上的物品索引（可能和nItemIdx不同）
	// Added by dengyong 20100304, 物品产出途径修改
	// 默认从商店购买的物品获得方式是emKADDITEM_BUY，但是从内部商店购买的物品产出途径为emKADDITEM_BUY_INNER_SHOP
	KE_ADD_ITEM_WAY eItemAddWay = emKADDITEM_BUY;
	LPCTSTR szGoodsName = pGoods->GetName();	//记录名字，不用每添加一个就发一次获得的消息，减少客户端收报数量

	INT tmpCount = nCount;
	std::vector<INT> vecItemIdx;
	vecItemIdx.clear();
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	//判断背包
	while (nCount > 0)
	{
		INT nPreCount = nCount;
		nItemIdx = CreateGoods(pPlayer, pGoods, (KMONEYTYPE)pGoods->m_nCurrencyType, nCount);
		if (nItemIdx <= 0)
			return	FALSE;
		nPrice = pGoods->m_nPrice * pGoods->m_nDiscount / 100;

		KItem& rItem = Item[nItemIdx];
		
		if (!pPlayerItem->CanAddItemIntoBag(Item[nItemIdx].GetIndex(), Item[nItemIdx].GetBindType(), nPreCount))
		{
			RemoveGoods(nItemIdx);
			g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
				pPlayer->GetId(), "DEBUG", LANG_019);
			return FALSE;
		}

		vecItemIdx.push_back(nItemIdx);
	}

	//判断货币
	if (tmpCount > 0)
	{
		KE_PURSE_RESULT eResult;
		if (!pPlayerPurse->AddMoney(pGoods->m_nCurrencyType, -(nPrice * tmpCount), &eResult))
		{
			if (pGoods->m_nCurrencyType == (INT)EM_MONEY_COIN)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_020);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_SILVER)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_021);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_SPIRIT)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_022);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_CONTRIBUTION)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_023);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_PRESTIGE)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_024);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_MPJJ_SCORE)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_025);
			}
			else if (pGoods->m_nCurrencyType == (INT)EM_MONEY_JINGPO)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_026);
			}
			else
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
					pPlayer->GetId(), "DEBUG", LANG_027);
			}

			for (std::vector<INT>::iterator it = vecItemIdx.begin(); it != vecItemIdx.end(); ++it)
				RemoveGoods(*it);
			return FALSE;
		}
	}
	for (std::vector<INT>::iterator it = vecItemIdx.begin(); it != vecItemIdx.end(); ++it)
	{
		nResultItemIdx = pPlayerItem->AddItemIntoBag(*it, eItemAddWay, FALSE);

		if (!nResultItemIdx)
		{	// 加到人身上失败，从游戏世界中删掉
			RemoveGoods(*it);
			return	FALSE;
		}

		if (!BuyCallBack(nPlayerIdx, *it, nPrice))
		{
			pPlayerItem->Remove(*it, emKLOSEITEM_BUYFAIL);
			RemoveGoods(*it);
			return FALSE;
		}

		WriteLog(
			nPlayerIdx,
			pGoods,
			nPrice,
			(KMONEYTYPE)pGoods->m_nCurrencyType,
			nBuy + 1,
			BUY, 
			pGoods->m_nCurrencyType == (INT)EM_MONEY_COIN
			);
		g_i64BuySellTotal += nPrice;
	}
	pGoods->OnSale((KMONEYTYPE)pGoods->m_nCurrencyType, tmpCount);

	if(tmpCount > 0)
	{
		CHAR szBuffer[MAX_PATH];
		snprintf(szBuffer, sizeof(szBuffer), LANG_028, tmpCount, pGoods->GetName());
		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL,
			pPlayer->GetId(), "DEBUG", szBuffer);
	}

//	std::vector<KBuyAllPurchaseRecord> vecRecord = g_cBuySellMgr.GetAllPurchaseRecord();
//	for (std::vector<KBuyAllPurchaseRecord>::iterator it = vecRecord.begin(); it != vecRecord.end(); ++it)
//	{
//		if(pCmd->nShopId == it->nShopId && pCmd->nCategoryId == it->nCategoryId  && pCmd->nGoodsId == it->nGoodsId)
//		{
//				it->nCount += pCmd->nCount;
//		}
//	}
	
	return TRUE;
}

BOOL KBuySellMgr::BuyCallBack(INT nPlayerIdx, INT nItemIdx, INT nPrice)
{
#if 0
	KPlayer& player	= Player[nPlayerIdx];
	if (!player.m_BuyInfo.m_szCallBackFunc[0])
		return TRUE; //无回调函数直接返回
	BOOL bResult = FALSE;
	if (player.OnEventScript("ssdd", "OnBuy", player.m_BuyInfo.m_szCallBackFunc[0], nItemIdx, nPrice))
		bResult = player.GetScriptResultNumber();
#endif
	return TRUE;
}


BOOL KBuySellMgr::Sell(DWORD dwPlayerIdx, KPLAYER_SELL_ITEM_SYNC *pCmd)
{
	ASSERT(dwPlayerIdx > 0 && dwPlayerIdx < MAX_PLAYER && pCmd);

	INT nBuy = pCmd->nShopId;
	if (nBuy <= 0)
		return FALSE;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(dwPlayerIdx);
	ASSERT(pPlayer);
	KPlayerItem& rcPlayerItem = (*pPlayer->GetCtrl<KPlayerItem>());
	INT nItemIdx = rcPlayerItem.SearchID(pCmd->dwId);
	if (!nItemIdx)
		return FALSE;

	KItem* pcItem = &Item[nItemIdx];
	if (pcItem->IsForbitSell())
		return FALSE;

	//if (KD_ISEQUIP(pcItem->GetGenre()))		// 装备不能批量卖店
	//	pCmd->wCount = 1;

	/*if (pcItem->GetEnhanceTimes() > 0)		// 密码锁定不允许卖强化过的装备
	{
		return;
	}*/

	//KE_MONEY_TYPE eMoneyType = emNORMAL_MONEY;
	//if (pcItem->IsBind() == 1)
	//	eMoneyType = emBIND_MONEY;

	INT nCount = pCmd->wCount;
	CONST INT nMaxCount = rcPlayerItem.GetItemCountInBags(pcItem->GetIndex(), pcItem->IsBind());
	if (nMaxCount < nCount)
		return FALSE;
	
	/*if (nCount * pcItem->GetUnitSalePrice() + rcPlayerItem.GetCashMoney() > rcPlayerItem.GetMaxCarryMoney())
	{
		return;
	}*/

	KITEM_INDEX		sIndex  = pcItem->GetIndex();
	BYTE currencyType = pcItem->m_sBaseData.byCurrencyType;
	//KE_SERIES_TYPE  eSeries = pcItem->GetSeries();
	INT				nBind	= pcItem->IsBind();

	INT nEarn = SellPlayerItem(*pPlayer, nItemIdx, nCount);	// 先从指定的道具开始

	if (nCount > 0)
	{
		for (INT i = 0; i < 4; ++i)		// TODO: xyf 先写常数，以后尽量改
		{
			KItemRoom& rcRoom = rcPlayerItem.m_acRoom[g_aeBagRoom[i]];
			KItemRoom::KFinder cFind = rcRoom.GetFinder();
			KRoomFindCondition::SameIndexSeries cCond(sIndex, nBind);
			while (cFind.Next(cCond) && nCount > 0)
				nEarn += SellPlayerItem(*pPlayer, cFind.Index(), nCount);
		}
	}
	if (nEarn > 0)
	{
		KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
		ASSERT(pPlayerPurse);
		/*KGoods* pGoods = m_cSetting.GetGoodsByItemIdx(sIndex);
		ASSERT(pGoods);*/
		//pPlayerPurse->AddMoney(pGoods->m_nCurrencyType, nEarn, NULL);
		KE_PURSE_RESULT eResult = emKPURSE_S_SUCCESS;
		// 只能卖出一半价格
		pPlayerPurse->AddMoney(currencyType, nEarn / 2, &eResult);
		ASSERT(eResult == emKPURSE_S_SUCCESS);
	}

	return TRUE;
}

INT KBuySellMgr::SellPlayerItem(KPlayer& rcPlayer, INT nItemIdx, INT& rnCount)
{
	KPlayerItem& rcPlayerItem = (*rcPlayer.GetCtrl<KPlayerItem>());
	INT nEarn = 0;
	KItem& rcItem = Item[nItemIdx];
	INT nCurCount = rcItem.GetCount();
	KE_ITEM_DATARECORD_WAY eWay = emITEM_DATARECORD_NORMAL;

	do 
	{
		if (nCurCount <= rnCount)
		{
			INT nPrice = nCurCount * rcItem.m_sBaseData.nPrice;
			INT nAmount = nEarn + nPrice + rcPlayerItem.GetCashMoney();
			/*if (nAmount > rcPlayerItem.GetMaxCarryMoney() || nAmount < 0)
			{
				KPlayerChat::SendSystemInfo(
					emKSEND_MSG_PERSONAL,
					rcPlayer.GetPlayerIndex(),
					G_PLAYER_13,
					MSG_ITEM_TAKE_MONEY_ERR,
					strlen(MSG_ITEM_TAKE_MONEY_ERR)
					);
				break;
			}*/
			if (rcPlayerItem.Remove(nItemIdx, emKLOSEITEM_SELL))
			{
				/*
				// 日志
				KLogWritter::GetInstance()->ItemHistory_RemoveItem(
					&Item[nItemIdx],
					&rcPlayer,
					emKLOG_REASON_REMOVEITEM_NPCSHOP);
				*/
				eWay = emITEM_DATARECORD_REMOVE;

				if (g_cItemMgr.Remove(nItemIdx, eWay, emITEM_CONSUMEMODE_SELL, &rcPlayer))
				{
					rnCount -= nCurCount;
					nEarn += nPrice;
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			INT nPrice = rnCount * rcItem.m_sBaseData.nPrice;
			/*INT nAmount = nEarn + nPrice + rcPlayerItem.GetCashMoney();
			if (nAmount > rcPlayerItem.GetMaxCarryMoney() || nAmount < 0)
			{
				KPlayerChat::SendSystemInfo(
					emKSEND_MSG_PERSONAL,
					rcPlayer.GetPlayerIndex(),
					G_PLAYER_13,
					MSG_ITEM_TAKE_MONEY_ERR,
					strlen(MSG_ITEM_TAKE_MONEY_ERR)
					);
				break;
			}*/
			eWay = emITEM_DATARECORD_REMOVE;
			rcPlayerItem.SetItemStackCount(nItemIdx, nCurCount - rnCount, eWay, emKLOSEITEM_STALL_SELL, emKADDITEM_NONE);
			nEarn += nPrice;
			rnCount = 0;
		}
		/*rcPlayer.m_cPlayerTrace.OnBuySellAtShop(FALSE, rcPlayer.m_BuyInfo.m_nBuyIdx, rcPlayer.m_BuyInfo.m_eCurrencyType, 
			rcItem, rnCount, rcItem.GetUnitPrice());*/
	}
	while(0);
	return	nEarn;
}
#else	// #ifdef GAME_SERVER

VOID KBuySellMgr::OpenSale(INT nShop, KMONEYTYPE eCurrencyType, INT nScale)
{
	if (nShop < 0 || nScale < 0)
	{
		ASSERT(FALSE);
		return;
	}
	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx = nShop;
	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nScale = nScale;
	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_eCurrencyType = eCurrencyType;
	::g_CoreEventNotify(emCOREEVENT_NPC_TRADE, "d", TRUE);
}
#endif //#ifndef GAME_SERVER

#if 0
#ifdef GAME_SERVER
VOID KBuySellMgr::OpenSale(INT nPlayerIdx, INT nShop, KCURRENCYTYPE eCurrencyType, INT nScale /*= 100*/, INT nPlayerSeries /*= -1*/, LPCSTR szCallBack /*= NULL*/)
{
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
	{
		return;
	}

	Player[nPlayerIdx].m_BuyInfo.m_nBuyIdx = nShop;
	Player[nPlayerIdx].m_BuyInfo.m_nScale = nScale;
	Player[nPlayerIdx].m_BuyInfo.m_nPlayerSeries = nPlayerSeries;
	Player[nPlayerIdx].m_BuyInfo.m_SubWorldID = Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex;
	Player[nPlayerIdx].m_BuyInfo.m_eCurrencyType = eCurrencyType;
	if (szCallBack) //回调脚本函数
	{	
		if (strlen(szCallBack) < sizeof(Player[nPlayerIdx].m_BuyInfo.m_szCallBackFunc))
		{
			strcpy(Player[nPlayerIdx].m_BuyInfo.m_szCallBackFunc, szCallBack);
		}
		else
		{
			_ASSERT("!Too long Callback function！");
			Player[nPlayerIdx].m_BuyInfo.m_szCallBackFunc[0]	= '\0';
		}
	}
	else
	{
		Player[nPlayerIdx].m_BuyInfo.m_szCallBackFunc[0]	= '\0';
	}
	Npc[Player[nPlayerIdx].m_nIndex].GetMpsPos(
		&Player[nPlayerIdx].m_BuyInfo.m_nMpsX,
		&Player[nPlayerIdx].m_BuyInfo.m_nMpsY);

	SALE_BOX_SYNC saleSync;
	saleSync.ProtocolType = s2c_opensalebox;
	saleSync.nShopIndex = nShop;
	saleSync.nScale = nScale;
	saleSync.nCurrencyType = (INT)eCurrencyType;
	g_pCoreServer->SendData(Player[nPlayerIdx].m_nNetConnectIdx, &saleSync, sizeof(SALE_BOX_SYNC));
	Npc[Player[nPlayerIdx].m_nIndex].SendCommand(do_stand, 0, 0);
}

VOID KBuySellMgr::OpenItemBussinessSale(INT nPlayerIdx, INT nShop, KCURRENCYTYPE eCurrencyType, INT nScale /*= 100*/, LPCSTR szShopName)
{
	if (nShop < 0)
		return;
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
		return;

	Player[nPlayerIdx].m_BuyInfo.m_szCallBackFunc[0]	= '\0';
	Player[nPlayerIdx].m_BuyInfo.m_nBuyIdx = nShop;
	Player[nPlayerIdx].m_BuyInfo.m_nScale = nScale;
	Player[nPlayerIdx].m_BuyInfo.m_SubWorldID = Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex;
	Player[nPlayerIdx].m_BuyInfo.m_eCurrencyType = eCurrencyType;
	Npc[Player[nPlayerIdx].m_nIndex].GetMpsPos(
		&Player[nPlayerIdx].m_BuyInfo.m_nMpsX,
		&Player[nPlayerIdx].m_BuyInfo.m_nMpsY
	);
	
	if (Player[nPlayerIdx].CallMainScript("Shop", "CheckCanUse", 1, "d", nShop))
	{
		INT nReturn = Player[nPlayerIdx].GetScriptResultNumber();
		//如果脚本返回值为1,表示不要删除当前的物品。如果脚本返回值为0表示需要删除当前物品
		if (!nReturn)
			return ;
	}
	
	PLAYER_UISCRIPT_SYNC UiInfo;
	::memset(&UiInfo, 0, sizeof(UiInfo));
	UiInfo.ProtocolType = s2c_playeruiscript;
	UiInfo.m_bUIId = UI_OPENITEMBUSSINESS;
	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nBufferLen = sizeof(SALE_ITEMBUSSINESS);
	UiInfo.m_bParam2 = 1;
	UiInfo.m_bParam1 = 1;
	SALE_ITEMBUSSINESS * pSaleSync = (SALE_ITEMBUSSINESS *) &UiInfo.m_pContent;
	
	pSaleSync->ProtocolType = s2c_opensalebox;
	pSaleSync->nShopIndex = nShop;
	pSaleSync->nScale = nScale;
	pSaleSync->nCurrencyType = (INT)eCurrencyType;

	if (szShopName)
		strcpy(pSaleSync->szShopName, szShopName);
	else
		strcpy(pSaleSync->szShopName, "SHOP");

	UiInfo.wProtocolSize = sizeof(PLAYER_UISCRIPT_SYNC) - KD_UISCIRPT_BUFFER_MAX + UiInfo.m_nBufferLen - 1;
	g_pCoreServer->SendData(Player[nPlayerIdx].m_nNetConnectIdx, (LPBYTE)&UiInfo, UiInfo.wProtocolSize + 1 );
}
#endif

BOOL KBuySellMgr::InitItemSaleLog()
{
	return m_SaleLogFile.InitialLogFile(F_LOG_PATH_ROOT, SALE_LOG_FILE_NAME);
}

VOID KBuySellMgr::RegisterProtocol(IKManager** ppHandlers)
{
	ASSERT(ppHandlers);
	INT aryProtocol[] = {
		c2s_playersellitem,
		c2s_playerbuyitem
	};
	for (INT i = 0; i < countof(aryProtocol); i++)
	{
		BYTE byProtocol = aryProtocol[i];
		ASSERT(ppHandlers[byProtocol] == NULL);
		ppHandlers[byProtocol] = this;
	}
}
#endif
BOOL KBuySellMgr::ProtocolProcess(LPCBYTE pData, INT nDataLen, KPlayer& rcPlayer)
{
	ASSERT(pData && nDataLen > 0);
	switch (*(pData + 1))
	{
	case emSHOP_PTC_C2S_SYNC_SELLITEM:
		PlayerSellItem(&rcPlayer, pData, nDataLen);
		break;
	case emSHOP_PTC_C2S_SYNC_BUYITEM:
		PlayerBuyItem(&rcPlayer, pData, nDataLen);
		break;
	default:
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

VOID KBuySellMgr::PlayerSellItem(KPlayer* pPlayer, LPCBYTE pData, INT nDataLen)
{
	KPLAYER_SELL_ITEM_SYNC* pCommand = (KPLAYER_SELL_ITEM_SYNC*)pData;
	if (nDataLen != sizeof(KPLAYER_SELL_ITEM_SYNC) ||
		pCommand->dwId <= 0 ||
		pCommand->wCount <= 0)
	{
		return;
	}
	Sell(pPlayer->GetPlayerIndex(), pCommand);
}

VOID KBuySellMgr::PlayerBuyItem(KPlayer* pPlayer, LPCBYTE pData, INT nDataLen)
{
	KPLAYER_BUY_ITEM_SYNC* psSync = (KPLAYER_BUY_ITEM_SYNC*)pData;
	if (!Buy(pPlayer->GetPlayerIndex(), psSync))
		printf("%s buy shop Item failed!\n", pPlayer->GetName());
	pPlayer->FireEvent(emKOBJEVENTTYPE_PLAYER_ITEM_OPS, pPlayer->GetId(), 0, 0);
}

BOOL KBuySellMgr::WriteLog(DWORD dwPlayerIndex, KGoods* pGoods, INT nPrice, KMONEYTYPE eCurrencyType, INT nShop, 
						SALE_BEHAVIOR eSaleBehavior, BOOL bFoceWriting /* FALSE */ )
{
	if (!pGoods || (!bFoceWriting && !pGoods->IsSaleLogRequired()))
		return FALSE;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(dwPlayerIndex);
	if (pPlayer == NULL)
		return FALSE;
	CONST static CHAR *cszBehavior[] = {"BUY", "SEL"};
	LPCSTR szBehavior =  "";
	switch( eSaleBehavior )
	{
	case BUY:
		szBehavior = cszBehavior[0];
		break;
	case SELL:
		szBehavior = cszBehavior[1];
		break;
	default:
		break;
	}

	LPCSTR szCurrencyType = "";
	if (eCurrencyType >= 0 && eCurrencyType < sizeof(cszCurrencyType)/sizeof(cszCurrencyType[0]))
		szCurrencyType	= cszCurrencyType[eCurrencyType];
	else
		szCurrencyType	= cszCurrencyType[EM_MONEY_MAX - 1];
	if (!szCurrencyType)
	{
		_ASSERT(FALSE);
		szCurrencyType	= "";
	}
	
	m_SaleLogFile.LogRecordVar((LOG_RECORD_REMIND_LEVEL)0x1, "%s(%s) %s %d %s(%d,%d,%d,%d,%d) %d(%s) at Shop[%d]",
		pPlayer->GetName(), pPlayer->GetName(),
		szBehavior, 1, pGoods->GetName(),
		pGoods->m_sIndex.nGenre, pGoods->m_sIndex.nDetailType, pGoods->m_sIndex.nParticular,
		pGoods->m_sIndex.nLevel, pGoods->m_nSeries, nPrice, szCurrencyType, nShop);

	return TRUE;
}

BOOL KBuySellMgr::WriteLog(DWORD dwPlayerIndex, LPSTR szMsg )
{
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwPlayerIndex);
	m_SaleLogFile.LogRecordVar((LOG_RECORD_REMIND_LEVEL)0x1, "%s(%s) %s", pPlayer->GetName(), pPlayer->GetName(), szMsg);
	return TRUE;
}

INT KBuySellMgr::CreateGoods(KPlayer* pPlayer, CONST KGoods* pGoods, KMONEYTYPE eCurrencyType, INT& rnCount)// CONST
{
	ASSERT(rnCount > 0);
	if (!pPlayer || !pGoods)
		return	0;

	//没有开始卖
	if (pGoods->m_timeSaleStart > 0 && pGoods->m_timeSaleClose > 0)
	{
		TIME_T tNow = KSysService::GameTime(NULL);
		if (pGoods->m_timeSaleStart > tNow || tNow > pGoods->m_timeSaleClose)
			return 0;
	}

	DupedDef::OP_RESULT op_ret = DupedDef::ret_normal;
	KItemEnvBuySell itemEnv(pPlayer);
	itemEnv.CurrencyType(eCurrencyType);

	INT nItemIdx = g_cItemMgr.Add(
		pGoods->m_sIndex.nGenre,
		pGoods->m_sIndex.nDetailType,
		pGoods->m_sIndex.nParticular,
		pGoods->m_sIndex.nLevel,
		0,
		emITEM_DATARECORD_SYSTEMADD
	);

	if (nItemIdx <= 0)
		return	0;

	if (Item[nItemIdx].IsStackable())
	{
		INT nMaxCount = Item[nItemIdx].GetMaxCount();
		if (rnCount > nMaxCount)
		{
			rnCount -= nMaxCount;
			Item[nItemIdx].SetCount(nMaxCount);
		}
		else
		{
			Item[nItemIdx].SetCount(rnCount);
			rnCount = 0;
		}
	}
	else
	{
		--rnCount;
	}

	SetGoodsTimeout(pGoods, nItemIdx);	//设置时效，要在GOODS表填写对应的时效长度(timeout字段, 分钟为单位)
										//zhengyuhua:移动到这里，所有类型货币都能使用，不再是魂石商店专用了
	return	nItemIdx;
}

VOID KBuySellMgr::RemoveGoods(INT nItemIdx) CONST
{
	if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		return;
	printf("KBuySellMgr::RemoveGoods remove %s nCount: %d\n", Item[nItemIdx].GetName(), Item[nItemIdx].GetCount());
	g_cItemMgr.Remove(nItemIdx);
}

#if 0
// 指定索引的商品是否在销售状态
BOOL KBuySellMgr::IsGoodsOnSale(INT nIndex)
{
	INT nTempTimeFrameDay		= 0;
	INT nAbsoluteTime			= 0;
	time_t timeNow				= 0;
	time_t timeTemp				= 0;
	KGoods *pGoods				= NULL;
	
	pGoods = m_cSetting.GetGoods(nIndex);
	if (!m_cSetting.GetGoods(nIndex))
		return FALSE;

	timeNow = g_ServerTime.GetCurLocalTime();

	// 绝对时间检查
	timeTemp = pGoods->GetTimeSaleStart();
	if (timeTemp!= 0 && timeNow < timeTemp)
		return FALSE;

	timeTemp = pGoods->GetTimeSaleClose();
	if (timeTemp != 0 && timeNow > timeTemp)
		return FALSE;

	// 时间轴检查
	nTempTimeFrameDay = pGoods->GetTimeFrameStartDay();
	if (nTempTimeFrameDay != 0)
	{
		if (!TryGetAbsoluteTime(nTempTimeFrameDay, nAbsoluteTime))
			return FALSE;
		if (timeNow < nAbsoluteTime)
			return FALSE;
	}

	return TRUE;
}
#endif

// 获取服务器开服时间
BOOL KBuySellMgr::TryGetServerStartTime(INT &nServerStartTime)
{
	static INT nStartTime		= 0; // 开服时间
	
	if (nStartTime)
	{
		nServerStartTime = nStartTime;
		return TRUE;
	}

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	if(cSafeScript->GetGlobalF("KGblTask.SCGetDbTaskInt") != 1)
		return FALSE;

	if(!cSafeScript->PushNumber(16))
		return FALSE;

	if(!cSafeScript->DoCall(1, 1))
		return FALSE;

	if (!cSafeScript->IsNumber(-1))
		return FALSE;

	nStartTime = cSafeScript->GetInt(-1);
	nServerStartTime = nStartTime;

	return TRUE;
}

// 获得开服第nDayIndex天对应的绝对时间
BOOL KBuySellMgr::TryGetAbsoluteTime(INT nDayIndex, INT& nAbsoluteTime)
{
	if (nDayIndex <= 0)
		return FALSE;

	int nServerStartTime = 0;
	if (!TryGetServerStartTime(nServerStartTime))
		return FALSE;

	nAbsoluteTime = nServerStartTime + (nDayIndex - 1) * 86400; // 86400是每天的秒数
	return TRUE;
}

BOOL KBuySellMgr::SetGoodsTime(INT nId, LPCSTR szType, LPCSTR szTime )
{
	//KGoods *pGoods = m_cSetting.GetGoods(nId);
	KGoods *pGoods = NULL;
	if (pGoods == NULL)
		return FALSE;

	if (strcmp(szType, "nTimeFrameStartDay") == 0)
	{
		INT nTimeFrameStartDay;
		if (szTime[0] == '\0' || strcmp(szTime, "0") == 0)
		{
			nTimeFrameStartDay = 0;
		}
		else
		{
			if (sscanf(szTime, "%d", &nTimeFrameStartDay) != 1)
				return FALSE;

			if (nTimeFrameStartDay < 0)
				return FALSE;
		}


		pGoods->SetTimeFrameStartDay(nTimeFrameStartDay);
		return TRUE;
	}

	if (strcmp(szType, "timeSaleStart") == 0)
	{
		time_t t;
		if (szTime[0] == '\0' || strcmp(szTime, "0") == 0)
		{
			t = 0;
		}
		else
		{
			if (!m_cSetting.TryParseTime(szTime, t))
				return FALSE;
		}

		pGoods->SetTimeSaleStart(t);
		return TRUE;
	}

	if (strcmp(szType, "timeSaleClose") == 0)
	{
		time_t t;
		if (szTime[0] == '\0' || strcmp(szTime, "0") == 0)
		{
			t = 0;
		}
		else
		{
			if (!m_cSetting.TryParseTime(szTime, t))
				return FALSE;
		}

		pGoods->SetTimeSaleClose(t);
		return TRUE;
	}

	return FALSE;
}

BOOL KBuySellMgr::DiscountSync(INT nPlayerIdx, INT nShopId, INT nCategoryId)
{
	IMemBlock* piPackage					= NULL;
	KSHOP_S2C_DISCOUNT_SYNC* ptcDiscount	= NULL;
	std::vector<KGoods*> *goodsVec			= NULL;
	piPackage = QCreateMemBlock(sizeof(KSHOP_S2C_DISCOUNT_SYNC) + sizeof(KSHOP_DISCOUNT_INFO));
	ptcDiscount = (KSHOP_S2C_DISCOUNT_SYNC*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcDiscount);

	ptcDiscount->byProtocol					= (BYTE)emSHOP_PTC_S2C_SYNC_DISCOUNT;
	memcpy(ptcDiscount->arrInfo, ptcDiscount->arrInfo, sizeof(KSHOP_DISCOUNT_INFO) * ptcDiscount->nCount);
	g_cShopModule.GetProtocolProcessor()->SendLenData(nPlayerIdx, *ptcDiscount, sizeof(KSHOP_S2C_DISCOUNT_SYNC)+sizeof(KSHOP_DISCOUNT_INFO) * ptcDiscount->nCount);


EXIT0:
	SAFE_RELEASE(piPackage);
	return TRUE;
}

