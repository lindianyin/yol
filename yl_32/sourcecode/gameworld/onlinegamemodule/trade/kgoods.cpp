
#include "stdafx.h"
#include "kgoods.h"
#include "item2/kitemmgr.h"

KGoods::KGoods()
{
	m_sReputeCond.nCamp			= 0;
	m_sReputeCond.nClass		= 0;
	m_sReputeCond.nLevel		= 0;
	m_nGoodsPrice_Money			= -1;
	m_nGoodsPrice_FuYuan		= -1;
	m_nGoodsPrice_AbateCoin 	= -1;
	m_nGoodsPrice_Score			= -1;
	m_nGoodsPrice_OrginCoin		= -1;
	m_nGoodsPrice_Contribution	= -1;
	m_nGoodsPrice_BuildFund		= -1;
	m_bSaleLogRequired			= FALSE;
	m_nId						= -1;
	m_dwTimeout					= 0;

#ifdef GAME_SERVER
	m_sIndex.nGenre		 		= ITEM_INVALID_TYPE;
	m_sIndex.nDetailType 		= ITEM_INVALID_TYPE;
	m_sIndex.nParticular 		= ITEM_INVALID_TYPE;
	m_sIndex.nGenre		 		= 0;
	//m_nSeries			 		= series_none;
#else
	m_nItemIdx			 		= 0;
#endif

	memset(&m_PromotionData, 0, sizeof(m_PromotionData));
	memset(m_dwSaleStats, 0, sizeof(m_dwSaleStats));

	m_nTimeFrameStartDay = 0;
	
	m_timeSaleStart = 0;
	m_timeSaleClose = 0;
}
/*
#ifndef _SERVER
VOID KGoods::GetDescOfPrice(LPSTR pszMsg, KCURRENCYTYPE eCurrencyType)
{
	if (!pszMsg)
		return;
	
	CHAR szPrice[32];

	switch (eCurrencyType)
	{
	case CURRENCYTYPE_MONEY:
		strcat(pszMsg, "<color=White>");
		sprintf(szPrice, G_ITEM_2, GetGoodsPrice_Money(), G_STRING_MONEY);
		break;
	case CURRENCYTYPE_FUYUAN:
		strcat(pszMsg, "<color=Gold>");
		sprintf(szPrice, G_ITEM_2, GetGoodsPrice_FuYuan(), G_STRING_FUYUAN);
		break;
	case CURRENCYTYPE_COIN:
		strcat(pszMsg, "<color=Yellow>");
		sprintf(szPrice, G_ITEM_2, GetGoodsPrice_Coin(), G_STRING_COIN);
		break;
	case CURRENCYTYPE_SCORE:
		strcat(pszMsg, "<color=Fire>");
		sprintf(szPrice, G_ITEM_2, GetGoodsPrice_Score(), G_STRING_SCORE);
		break;
	case CURRENCYTYPE_CONTRIBUTION:
		strcat(pszMsg, "<color=LightGreen>");
		sprintf(szPrice, G_ITEM_2, GetGoodsPrice_Contribution(), G_STRING_CONTRIBUTION);
		break;
	case CURRENCYTYPE_HONOUR:
		strcat(pszMsg, "<color=Green>");
		sprintf(szPrice, G_ITEM_2, GetGoodsPrice_Honour(), G_STRING_HONOUR);
		break;
	}
	
	if (strlen(szPrice) > 0 )
	{
		strcat(pszMsg, szPrice);
		strcat(pszMsg, "<color>");
		strcat(pszMsg, "\n");
	}	
}

VOID KGoods::GetTip(KGameObjDesc* pDesc,
					 CurrencyType eCurrencyType,	// = CURRENCYTYPE_NONE
					 BOOL bPicPath)					// = FALSE
{
	KItem::GetTip(pDesc, eCurrencyType, bPicPath);
	
	LPSTR pszMsg = pDesc->szTitle;
	CHAR szDateInfo[128] = {0};
	//strcat(pszMsg, "\n");

	if(eCurrencyType == CURRENCYTYPE_COIN && GetGoodsPrice_Coin() < GetGoodsPrice_OrginCoin() )
	{
		//原价
		sprintf(szDateInfo, "%s%d%s", G_STR_ORIGINAL_PRICE, GetGoodsPrice_OrginCoin(), G_STRING_COIN);
		sprintf(pszMsg, "%s<color=red>%s<color>\n", pszMsg, szDateInfo);

		//现价
		sprintf(szDateInfo, "%s%d%s", G_STR_ABATE_PRICE, GetGoodsPrice_Coin(), G_STRING_COIN);
		sprintf(pszMsg, "%s<color=Yellow>%s<color>\n\n", pszMsg, szDateInfo);

		CHAR szAbateEndDate[32] = {0};
		CHAR szAbateBegin[32] = {0};
		GetGoods_PromotionDate()->GetBeginAbateDate(szAbateBegin);
		GetGoods_PromotionDate()->GetAbateEndDate(szAbateEndDate);

		//打折日期开始
		if(szAbateEndDate[0] != 0)
		{
			if(szAbateBegin[0] != 0)
			{
				sprintf(szDateInfo, "%s%s %s %s", G_STR_ABATE_BEGIN_DATE, szAbateBegin, G_STR_TO, szAbateEndDate);
				sprintf(pszMsg, "%s<color=Fire>%s<color>\n", pszMsg, szDateInfo);
			}
			else
			{
				sprintf(szDateInfo, "%s%s", G_STR_ABATE_END, szAbateEndDate);
				sprintf(pszMsg, "%s<color=Fire>%s<color>\n", pszMsg, szDateInfo);
			}
		}
	}//end of "if(ct"
	
	// 声望需求
	if (m_sReputeCond.nCamp > 0 &&
		m_sReputeCond.nClass > 0 &&
		m_sReputeCond.nLevel > 0)
	{
		CONST KReputeSetting::KCamp *pCamp = KPlayerSetting::s_cReputeSetting.GetCamp(m_sReputeCond.nCamp);
		CONST KReputeSetting::KClass *pClass = pCamp->GetClass(m_sReputeCond.nClass);
		if (pCamp && pClass)
		{
			LPSTR pszRepute = pszMsg + strlen(pszMsg);
			sprintf(pszRepute, G_STR_NPC_BUYITEM_REPUTE, pCamp->GetName(), pClass->GetName(), m_sReputeCond.nLevel);
		}
	}

	CHAR szSellBegin[32] = {0};
	CHAR szSellEndDate[32] = {0};
	GetGoods_PromotionDate()->GetBeginSellDate(szSellBegin);
	GetGoods_PromotionDate()->GetSellEndDate(szSellEndDate);
	//销售日期开始
	if(szSellEndDate[0] != 0)
	{
		if(szSellBegin[0] != 0)
		{
			sprintf(szDateInfo, "%s%s %s %s", G_STR_SELL_BEGIN_DATE, szSellBegin, G_STR_TO, szSellEndDate);
			sprintf(pszMsg, "%s<color=Fire>%s<color>\n", pszMsg, szDateInfo);
		}
		else
		{
			sprintf(szDateInfo, "%s%s", G_STR_SELL_END, szSellEndDate);
			sprintf(pszMsg, "%s<color=Fire>%s<color>\n", pszMsg, szDateInfo);
		}
	}

	return;
}
#endif //#ifndef _SERVER
*/

KPromotionSaleDate* KGoods::GetGoods_PromotionDate()
{
	return &m_PromotionData;
}

VOID KGoods::SetGoods_AbateDate(INT nBeginDate, INT nEndDate)
{
	m_PromotionData.nBeginAbateDate = nBeginDate;
	m_PromotionData.nEndAbateDate = nEndDate;
}

VOID KGoods::SetGoods_SaleDate(INT nBeginDate, INT nEndDate)
{
	m_PromotionData.nBeginSaleDate = nBeginDate;
	m_PromotionData.nEndSaleDate = nEndDate;
}

#ifndef _SERVER
INT KGoods::GetGoodsPrice_Money()
{
	//INT nVersion		= KD_GAME_VERSION_CURRENT;
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	CONST KBASICPROP_BASEDATA* pBPTBase = (CONST KBASICPROP_BASEDATA*)pLibBPT->GetBasicProp(m_sIndex);
	if (!pBPTBase || pBPTBase->nPrice == 0)
		return	0;

	return pBPTBase->nPrice;

}
INT KGoods::GetGoodsPrice_BindMoney()
{
	//INT nVersion		= KD_GAME_VERSION_CURRENT;
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	CONST KBASICPROP_BASEDATA* pBPTBase = (CONST KBASICPROP_BASEDATA*)pLibBPT->GetBasicProp(m_sIndex);
	if (!pBPTBase || pBPTBase->nPrice == 0)
		return	0;

	return pBPTBase->nPrice;
}
INT KGoods::GetGoodsPrice_MachineCoin()
{
	//INT nVersion		= KD_GAME_VERSION_CURRENT;
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	CONST KBASICPROP_BASEDATA* pBPTBase = (CONST KBASICPROP_BASEDATA*)pLibBPT->GetBasicProp(m_sIndex);
	if (!pBPTBase || pBPTBase->nPrice == 0)
		return	0;

	return pBPTBase->nPrice;
}
#endif

INT KGoods::GetGoodsPrice_FuYuan()
{
	return (m_nGoodsPrice_FuYuan > 0) ? m_nGoodsPrice_FuYuan : INVALID_PRICE;
}

INT KGoods::GetGoodsPrice_Coin()
{
	return (m_nPrice_ItemCoin > 0) ? m_nPrice_ItemCoin : INVALID_PRICE;
}

INT KGoods::GetGoodsPrice_OrginCoin()
{
	return ( m_nGoodsPrice_OrginCoin > 0 ) ? m_nGoodsPrice_OrginCoin : INVALID_PRICE;
}

INT KGoods::GetGoodsPrice_AbateCoin()
{
	return ( m_nGoodsPrice_AbateCoin > 0 ) ? m_nGoodsPrice_AbateCoin : INVALID_PRICE;
}

INT KGoods::GetGoodsPrice_Score()
{
	return ( m_nGoodsPrice_Score > 0 ) ? m_nGoodsPrice_Score : INVALID_PRICE;
}

INT KGoods::GetGoodsPrice_Contribution()
{
	return ( m_nGoodsPrice_Contribution > 0 ) ? m_nGoodsPrice_Contribution : INVALID_PRICE;
}

INT KGoods::GetGoodsPrice_TongFund()
{
	return ( m_nGoodsPrice_TongFund > 0 ) ? m_nGoodsPrice_TongFund : INVALID_PRICE;
}

INT KGoods::GetGoodsPrice_CoinItem()
{
	return (m_nPrice_ItemCoin > 0) ? m_nPrice_ItemCoin : INVALID_PRICE;
}

INT KGoods::GetGoodsPrice_ValueCoin()
{
	return (m_nPrice_ValueCoin > 0) ? m_nPrice_ValueCoin : INVALID_PRICE;
}

INT KGoods::GetItemCoinIndex()
{
	return (m_nItemCoinIndex > 0) ? m_nItemCoinIndex : -1;
}

INT KGoods::GetValueCoinIndex()
{
	return (m_nValueCoinIndex > 0) ? m_nValueCoinIndex : -1;
}

INT KGoods::GetGoodsPrice_BuildFund()
{
	return ( m_nGoodsPrice_BuildFund > 0 ) ? m_nGoodsPrice_BuildFund : INVALID_PRICE;
}

BOOL KGoods::IsGetBind()
{
	return 1 == m_nBind;
}

VOID KGoods::SetGoodsPrice_Contribution(INT nNewPrice)
{
	m_nGoodsPrice_Contribution = nNewPrice;
}

VOID KGoods::SetGoodsPrice_TongFund(INT nNewPrice)
{
	m_nGoodsPrice_TongFund = nNewPrice;
}

VOID KGoods::SetGoodsPrice_BuildFund(INT nNewPrice)
{
	m_nGoodsPrice_BuildFund = nNewPrice;
}

INT KGoods::GetGoodsPrice_Honour()
{
	return m_nGoodsPrice_Honour;
}

VOID KGoods::SetGoodsPrice_Honour( INT nNewPrice )
{
	m_nGoodsPrice_Honour = nNewPrice;
}

INT KGoods::GetGoodsPrice_FightPowerRank()
{
	return m_nGoodsPrice_FightPowerRank;
}

VOID KGoods::SetGoodsPrice_FightPowerRank(INT nNewPrice)
{
	m_nGoodsPrice_FightPowerRank = nNewPrice;
}

VOID KGoods::SetGoodsPrice_CoinItem(INT nGoodsIndex, INT nNewPrice)
{
	m_nItemCoinIndex = nGoodsIndex;
	m_nPrice_ItemCoin = nNewPrice;
}

VOID KGoods::SetGoodsPrice_ValueCoin(INT nGoodsInidex, INT nNewPrice)
{
	m_nValueCoinIndex = nGoodsInidex;
	m_nPrice_ValueCoin = nNewPrice;
}

/*
VOID KGoods::SetGoodsPrice_Money( INT nNewPrice )
{
	m_nGoodsPrice_Money = nNewPrice;
}
*/

VOID KGoods::SetGoodsPrice_FuYuan( INT nNewPrice )
{
	m_nGoodsPrice_FuYuan = nNewPrice;
}

VOID KGoods::SetGoodsPrice_AbateCoin( INT nNewPrice )
{
	m_nGoodsPrice_AbateCoin = nNewPrice;
}

VOID KGoods::SetGoodsPrice_OrginCoin( INT nNewPrice )
{
	m_nGoodsPrice_OrginCoin = nNewPrice;
}

VOID KGoods::SetGoodsPrice_Score( INT nNewPrice )
{
	m_nGoodsPrice_Score = nNewPrice;
}

BOOL KGoods::IsSaleLogRequired()
{
	return m_bSaleLogRequired;
}

VOID KGoods::SetIfSaleLog( BOOL bLogRequired )
{
	m_bSaleLogRequired = bLogRequired;
}

BOOL KGoods::OnSale(KMONEYTYPE type, DWORD dwCount)
{
	m_dwSaleStats[type][StatType_LateCache]		+= dwCount;
	m_dwSaleStats[type][StatType_TotalForStart]	+= dwCount;
	return TRUE;
}

DWORD KGoods::GetSaleStat(KMONEYTYPE currType, enumStatType statType)
{ 
	return m_dwSaleStats[currType][statType];
}

VOID KGoods::CleanSaleStat(KMONEYTYPE currType, enumStatType statType)
{
	m_dwSaleStats[currType][statType] = 0;
}

VOID KGoods::SetGoods_Repute(CONST KReputeCondition *pCond)
{
	m_sReputeCond = *pCond;
}

INT KGoods::GetId(VOID) CONST
{
	return	m_nId;
}

VOID KGoods::SetId(INT nId)
{
	if (nId < 0)
		m_nId = -1;
	else
		m_nId = nId;
}

LPCSTR KGoods::GetName(VOID) CONST
{
#ifdef GAME_SERVER
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	if (pItemGen == NULL) 
		return NULL;
	CONST KLibOfBPT *pLibBPT = pItemGen->GetLibOfBPT();
	if (pLibBPT == NULL) 
		return NULL;
	CONST KBASICPROP_BASEDATA *pBaseData = pLibBPT->GetBasicProp(m_sIndex);
	if (!pBaseData)
		return	NULL;
	return	pBaseData->szName;
#else
	if (m_nItemIdx <= 0 || m_nItemIdx >= MAX_ITEM)
		return	NULL;
	return	Item[m_nItemIdx].GetName();
#endif
}

DWORD	KGoods::GetTimeout() CONST										// 获取商品的时效
{
	return m_dwTimeout;
}

VOID KGoods::SetTimeout( DWORD dwTimeout )
{
	m_dwTimeout = dwTimeout;
}

time_t KGoods::GetTimeSaleStart()
{
	return m_timeSaleStart;
}

VOID KGoods::SetTimeSaleStart( time_t timeSaleStart )
{
	m_timeSaleStart = timeSaleStart;
}

time_t KGoods::GetTimeSaleClose()
{
	return m_timeSaleClose;
}

VOID KGoods::SetTimeSaleClose( time_t timeSaleClose )
{
	m_timeSaleClose = timeSaleClose;
}

INT KGoods::GetTimeFrameStartDay()
{
	return m_nTimeFrameStartDay;
}

VOID KGoods::SetTimeFrameStartDay( int nTimeFrameStartDay )
{
	m_nTimeFrameStartDay = nTimeFrameStartDay;
}

INT KGoods::GetStock()
{
	return m_nStock;
}

VOID KGoods::SetStock(INT nStock)
{
	m_nStock = nStock;
}

INT KGoods::GetPurchaseLimit()
{
	return m_nPurchaseLimit;
}

VOID KGoods::SetPurchaseLimit(INT nPursechaseLimit)
{
	m_nPurchaseLimit = nPursechaseLimit;
}

INT KGoods::GetVip()
{
	return m_nVip;
}

VOID KGoods::SetVip(INT nVip)
{
	m_nVip = nVip;
}

INT KGoods::GetDiscount()
{
	return m_nDiscount;
}

VOID KGoods::SetDiscount(INT nDiscount)
{
	m_nDiscount = nDiscount;
}