

#include "stdafx.h"
#include "kbuysellsetting.h"
#include "item2/kitemmgr.h"
//#include "corefstringtable.h"

//---------------------------- player 交易相关 ------------------------------
#define		BUYSELL_FILE				"\\setting\\shop\\buysell.txt"
#define		GOODS_FILE					"\\setting\\shop\\goods.txt"
#define		SALE_LOG_FILE_NAME			"treasure_item_sale"
#define		ITEMSHOP_FILE				"\\setting\\shop\\ibshop.txt"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

#define  SHOP_FILE_PATH					"\\setting\\shop\\shop.txt"
#define  CATEGORY_FOLD_PATH				"\\setting\\shop\\category\\"
#define  GOODS_FOLD_PATH				"\\setting\\shop\\category\\goods\\"

KBuySellSetting::KBuySellSetting() : m_nWidth(0), m_nHeight(0)
{

}

KBuySellSetting::~KBuySellSetting()
{
	Release();
}

BOOL KBuySellSetting::Init()
{
	ReadShopCfg();
	return TRUE;

	ASSERT(m_mapGoods.empty());
#ifdef GAME_SERVER
	//if (!m_tabItemShop.Load(ITEMSHOP_FILE))
	//	printf("ItemBusinessShop Settings File Is Not Loaded!-> %s\n", ITEMSHOP_FILE);
#endif
	/*
	QTabFile tabGoods;
	if (!tabGoods.Load(GOODS_FILE))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	INT nGoodsHeight = tabGoods.GetHeight();
	INT nGoodsWidth = tabGoods.GetWidth();
	if (nGoodsWidth == 0 || nGoodsHeight == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	QTabFile tabBuySell;
	if (!tabBuySell.Load(BUYSELL_FILE))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	INT nBuySellHeight = tabBuySell.GetHeight();
	INT nBuySellWidth = tabBuySell.GetWidth();
	if (nBuySellWidth == 0 || nBuySellHeight == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_nHeight = nBuySellHeight;
	m_nWidth  = nBuySellWidth;

	for (INT nRow = 2; nRow <= nGoodsHeight; ++nRow)
	{
		INT nGoodsID = 0;	// 商品ID，从1开始
		tabGoods.GetInteger(nRow, "Id", 0, &nGoodsID);
		if (nGoodsID <= 0)
		{
			ASSERT(FALSE);
			continue;
		}

		KGoods& rcGoods = m_mapGoods[nGoodsID];
		rcGoods.SetId(nGoodsID);

		INT nGenre		= ITEM_INVALID_TYPE;
		INT nDetailType	= ITEM_INVALID_TYPE;
		INT nParticular	= ITEM_INVALID_TYPE;
		INT nLevel		= 0;
		INT nSeries		= series_none;
		INT nBind		= 0;

		tabGoods.GetInteger(nRow, "Genre", -1, &nGenre);
		tabGoods.GetInteger(nRow, "DetailType", -1, &nDetailType);
		tabGoods.GetInteger(nRow, "ParticularType", -1, &nParticular);
		tabGoods.GetInteger(nRow, "Level", -1, &nLevel);
		tabGoods.GetInteger(nRow, "Bind", 0, &nBind);

		rcGoods.m_nBind				 = nBind;
		rcGoods.m_sIndex.nGenre		 = nGenre;
		rcGoods.m_sIndex.nDetailType = nDetailType;
		rcGoods.m_sIndex.nParticular = nParticular;
		rcGoods.m_sIndex.nLevel		 = nLevel;

		// 读取时间控制条件，时间轴和绝对时间两者是且的关系，必须同时满足

		// 开服多少天开始卖，未设置或者设置的值<=0视为无效（此时保持默认值0，表示未设置）
		INT nTempTimeFrameDay;
		if(tabGoods.GetInteger(nRow, "nTimeFrameStartDay", 0, &nTempTimeFrameDay) && nTempTimeFrameDay > 0)
			rcGoods.SetTimeFrameStartDay(nTempTimeFrameDay);

		// 绝对时间（年月日时分，格式XXXX/XX/XX XX:XX, XX之间的分隔符可以是其他的非数字的符号)售卖的时间段,未设置或者设置的值不是指定的时间格式视为无效（此时保持默认值0，表示未设置）
		time_t timeTemp;
		if (TryGetDateTime(tabGoods, nRow, "timeSaleStart", timeTemp))
			rcGoods.SetTimeSaleStart(timeTemp);
		if (TryGetDateTime(tabGoods, nRow, "timeSaleClose", timeTemp))
			rcGoods.SetTimeSaleClose(timeTemp);

		
        if (!tabGoods.GetInteger(nRow, "Price", -1, &rcGoods.m_nPrice))
		{
			ASSERT(FALSE);
		}
		if (!tabGoods.GetInteger(nRow, "CurrencyType", 0, &rcGoods.m_nCurrencyType))
		{
			ASSERT(FALSE);
		}
#if 0
		BOOL bSaleLogRequired;
		// 读取声望条件
		KGoods::KReputeCondition sReputeCond;
		tabGoods.GetInteger(nRow, "ReputeCamp", -1, &sReputeCond.nCamp);
		tabGoods.GetInteger(nRow, "ReputeClass", -1, &sReputeCond.nClass);
		tabGoods.GetInteger(nRow, "ReputeLevel", -1, &sReputeCond.nLevel);
		tabGoods.GetInteger(nRow, "TradeLog", 0, &bSaleLogRequired);

		rcGoods.SetGoods_Repute(&sReputeCond);
		
		rcGoods.SetIfSaleLog(bSaleLogRequired);
		//支持荣誉购买条件
		INT nGoodsPrice_Honour;
		tabGoods.GetInteger(nRow, "honor", -1, &nGoodsPrice_Honour);
		rcGoods.SetGoodsPrice_Honour(nGoodsPrice_Honour);
		//增加时效
		INT nGoodsTimeout;
		tabGoods.GetInteger(nRow, "timeout", 0, &nGoodsTimeout);
		rcGoods.SetTimeout(nGoodsTimeout > 0 ? nGoodsTimeout : 0);	
		
		// 购买官衔等级条件
		INT nOfficialLevel;
		tabGoods.GetInteger(nRow, "OfficialLevel", -1, &nOfficialLevel);
		rcGoods.SetGood_OfficialLevel(nOfficialLevel);	

		// 帮会建设资金购买条件
		INT nGoodsPrice_TongFund;
		tabGoods.GetInteger(nRow, "tongfund", -1, &nGoodsPrice_TongFund);
		rcGoods.SetGoodsPrice_TongFund(nGoodsPrice_TongFund);
		// 帮会建设资金购买条件
		INT nGoodsPrice_TongEnergy;
		tabGoods.GetInteger(nRow, "tongenergy", -1, &nGoodsPrice_TongEnergy);
		rcGoods.SetGoodsPrice_TongEnergy(nGoodsPrice_TongEnergy);

		// 记录金币替代物品的购买条件（目前只有魂石，月影和情花3种）
		INT nGoodsIndex = -1;	// 这里的index 是金币替代物品在gamesetting配置文件中的索引
		tabGoods.GetInteger(nRow, "goodsindex", -1, &nGoodsIndex);
		INT nGoodsPrice = -1;
		tabGoods.GetInteger(nRow, "goodsprice", -1, &nGoodsPrice);
		rcGoods.SetGoodsPrice_CoinItem(nGoodsIndex, nGoodsPrice);

		// 数值货币的购买条件
		INT nValueCoinIndex = -1;
		tabGoods.GetInteger(nRow, "valuecoinindex", -1, &nValueCoinIndex);
		INT nValueCoinPrice = -1;
		tabGoods.GetInteger(nRow, "valuecoinprice", -1, &nValueCoinPrice);
		rcGoods.SetGoodsPrice_ValueCoin(nValueCoinIndex, nValueCoinPrice);

		// 总战斗力排行榜条件
		INT nGoodsPrice_FightPowerRank;
		tabGoods.GetInteger(nRow, "fightpowerrank", 0, &nGoodsPrice_FightPowerRank);
		rcGoods.SetGoodsPrice_FightPowerRank(nGoodsPrice_FightPowerRank);		
#endif

		LPCSTR pszName = rcGoods.GetName();
		if (!pszName)
		{
			ASSERT(FALSE);
			continue;
		}

		m_mapGoodsName[pszName] = nGoodsID;		// 名字-索引对应表
		//m_nMaxGoods++;
	}


	for (INT nRow = 2; nRow <= m_nHeight; ++nRow)
	{
		INT nShopId = 0;
		if (!tabBuySell.GetInteger(nRow, "ShopId", 0, &nShopId) ||
			nShopId <= 0)
		{
			ASSERT(FALSE);
			continue;
		}
		INT nItemIdx = 1;
		INT nGoodsId = 0;
		CHAR szCol[32];
		INT* pGoodsId = new INT[emMAX_ITEMCOUNT];
		memset(pGoodsId, -1, sizeof(INT) * emMAX_ITEMCOUNT);
		sprintf(szCol, "Item%d", nItemIdx);
		while (nItemIdx <= emMAX_ITEMCOUNT && tabBuySell.GetInteger(nRow, szCol, 0, &nGoodsId))
		{
			if (nGoodsId <= 0)
			{
				ASSERT(FALSE);
				continue;
			}
			pGoodsId[nItemIdx - 1] = nGoodsId;
			sprintf(szCol, "Item%d", ++nItemIdx);
		}
		m_mapShopGoods[nShopId] = pGoodsId;
	}*/
	return TRUE;
}

BOOL KBuySellSetting::Release()
{
	ClearShopMap();
//#ifndef _SERVER
//	for (KGoodsMap::const_iterator it = m_mapGoods.begin(); it != m_mapGoods.end(); ++it)
//	{
//		CONST KGoods& rcGoods = it->second;
//		VERIFY(g_cItemMgr.Remove(rcGoods.m_nItemIdx));
//	}
//#endif

	for (KShopGoodsMap::const_iterator it = m_mapShopGoods.begin(); it != m_mapShopGoods.end(); ++it)
	{
		LPINT pGoodsId = it->second;
		ASSERT(pGoodsId);
		delete[] pGoodsId;
	}

	m_mapGoods.clear();
	m_mapShopGoods.clear();
	m_nHeight = 0;
	m_nWidth = 0;
	return TRUE;
}

//KGoods* KBuySellSetting::GetGoods(LPCSTR pszItemName)
//{
//	ASSERT(pszItemName && pszItemName[0] != 0);	
//	MAP_GOODSNAME2INDEX::iterator it = m_mapGoodsName.find(pszItemName);
//	if (m_mapGoodsName.end() != it)
//	{
//		INT nGoodsId = it->second;
//		std::map<int, KGoods>::iterator iter = m_mapGoods.find(nGoodsId);
//		if (iter != m_mapGoods.end())
//		{
//			return &iter->second;
//		}
//	}
//	return NULL;
//}
//
//KGoods* KBuySellSetting::GetGoodsByItemIdx(CONST KITEM_INDEX& crItemIdx)
//{
//	std::map<int, KGoods>::iterator iter;
//	for (iter = m_mapGoods.begin(); iter != m_mapGoods.end(); ++iter)
//	{
//		KGoods* pGoods = &iter->second;
//		if (pGoods->m_sIndex == crItemIdx)
//		{
//			return pGoods;
//		}
//	}
//
//	return NULL;
//}
//
//KGoods* KBuySellSetting::GetGoods(INT nId)
//{
//	//ASSERT(m_pGoods);
//	if (nId < 0)
//		return NULL;
//	/*
//#ifndef _SERVER
//	if (Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_eCurrencyType == CURRENCYTYPE_COIN &&
//		m_pGoods[nIndex].GetGoodsPrice_Coin() == (INT)INVALID_PRICE)
//	{
//		return NULL;
//	}
//#endif
//	*/
//	KGoods* pGoods = NULL;
//	std::map<int, KGoods>::iterator it = m_mapGoods.find(nId);
//	if (it != m_mapGoods.end())
//	{
//		pGoods = &it->second;
//	}
//	return pGoods;
//}
//
//INT KBuySellSetting::GetGoodsIndex(INT nShop, INT nIndex)
//{
//	INT nGoodsId = -1;
//	std::map<INT, INT*>::iterator it = m_mapShopGoods.find(nShop);
//	if (it != m_mapShopGoods.end())
//	{
//		INT* pGoodsId = it->second;
//		if (nIndex >= 0 && nIndex < emMAX_ITEMCOUNT)
//		{
//			nGoodsId = pGoodsId[nIndex];
//		}
//	}
//	return nGoodsId;
//	// TODO: here
//	/*
//	ASSERT(m_ppShopGoods);
//	if (nShop < 0 || nShop >= m_nHeight || nIndex < 0 || nIndex >= m_nWidth)
//		return -1;
//	if (!m_ppShopGoods[nShop])
//		return -1;
//	INT nGoodsIdx = m_ppShopGoods[nShop][nIndex];
//	if (nGoodsIdx > m_nMaxGoods)
//		return -1;
//	return nGoodsIdx;
//	*/
//}

KGoods* KBuySellSetting::GetGoodsByGDPL(CONST KITEM_INDEX& crItemIdx)
{
	for (KShopMap::iterator it_shop = m_mapShop.begin(); it_shop != m_mapShop.end(); ++it_shop)
	{
		for (KCategoryMap::iterator it_category = it_shop->second->begin(); it_category != it_shop->second->end(); ++ it_category)
		{
			for (KGoodsVec::iterator it_goods = it_category->second->begin(); it_goods != it_category->second->end(); ++it_goods)
			{
				if ((*it_goods)->m_sIndex == crItemIdx)
					return (*it_goods);
			}
		}
	}

	return NULL;
}

KGoods* KBuySellSetting::GetGoodsByIndex(INT nShopIndex, INT nCategoryIndex, INT nGoodsIndex)
{
	if (m_mapShop.find(nShopIndex) != m_mapShop.end())
	{
		KCategoryMap* pCategoryMap = m_mapShop[nShopIndex];
		if (pCategoryMap->find(nCategoryIndex) != pCategoryMap->end())
		{
			KGoodsVec* pGoodsVec = (*pCategoryMap)[nCategoryIndex];
			for (KGoodsVec::iterator it = pGoodsVec->begin(); it != pGoodsVec->end(); ++it)
			{
				if ((*it)->GetId() == nGoodsIndex)
					return *it;
			}
		}
	}

	return NULL;
}

//KGoodsVec* KBuySellSetting::GetGoodsVecByIndex(INT nShopIndex, INT nCategoryIndex)
//{
//	return NULL;
//}

#ifdef GAME_SERVER

INT KBuySellSetting::ItemBusinessShopName2ID(LPSTR pszShopName)
{
	ASSERT(pszShopName);
	INT nShopID = 0;
	INT nSaleRow = m_tabItemShop.FindRow(pszShopName);
	if (nSaleRow > 0)
		m_tabItemShop.GetInteger(nSaleRow, 2, 0, &nShopID);
	return nShopID;
}

#else

INT KBuySellSetting::GetGoodsShopIdx(INT nGoodsId)
{
	if (nGoodsId <= 0)
		return	NULL;

	for (INT i = 0; i < GetWidth(); i++)
	{
		INT nId = GetGoodsIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, i);
		if (nId <= 0)
			continue;
		KGoods* pGoods = GetGoods(nId);
		if (pGoods && pGoods->GetId() == nGoodsId)
		{
			return i;
		}
	}
	return -1;
}

#endif

BOOL KBuySellSetting::TryParseTime(LPCSTR pszTime, time_t& t)
{
	BOOL bRet = FALSE;
	do
	{
		INT nYear = 0;
		INT nMon = 0;
		INT nDay = 0;
		INT nHour = 0;
		INT nMin = 0;
		INT nSec = 0;
		if (sscanf(pszTime, "%d%*c%d%*c%d%*c%d%*c%d%*c%d", &nYear, &nMon, &nDay, &nHour, &nMin, &nSec) != 6 &&
			sscanf(pszTime, "%d%*c%d%*c%d%*c%d%*c%d", &nYear, &nMon, &nDay, &nHour, &nMin) != 5)
		{
			break;
		}

		tm sTime;
		memset(&sTime, 0, sizeof(sTime));
		sTime.tm_year = nYear - 1900;
		sTime.tm_mon  = nMon - 1;
		sTime.tm_mday = nDay;
		sTime.tm_hour = nHour;
		sTime.tm_min  = nMin;
		sTime.tm_sec  = nSec;
		t = mktime(&sTime);
		bRet = TRUE;
	}
	while(0);
	return bRet;
}

// 获取日期时间
BOOL KBuySellSetting::TryGetDateTime(ITabFile* pTabFile, INT nRow, LPCSTR pszColName, time_t &t)
{
	CHAR szBuf[MAX_PATH];
	QCONFIRM_RET_FALSE(pTabFile);
	if(!pTabFile->GetString(nRow, pszColName, "", szBuf, sizeof(szBuf)))
		return FALSE;

	szBuf[sizeof(szBuf) - 1] = '\0';
	if (szBuf[0] == '\0')
		return FALSE;

	return TryParseTime(szBuf, t);
EXITO:
	return FALSE;
}

BOOL KBuySellSetting::ReadShopCfg()
{
	m_mapShop.clear();

	CHAR szCategroyFilePath[_MAX_PATH];
	QString szCategroyFileTemp;
	ITabFile* pCategroyFile = NULL;

	CHAR szGoodsFilePath[_MAX_PATH];
	QString szGoodsFileTemp;
	ITabFile* pGoodsFile = NULL;

	ITabFile* pShopFile = g_OpenTabFile(SHOP_FILE_PATH);
	QCONFIRM_RET_FALSE(pShopFile);

	for (INT i = 2; i <= pShopFile->GetHeight(); i++)
	{
		INT nShopId = 0;
		pShopFile->GetInteger(i, "shopId", 0, &nShopId);
		pShopFile->GetString(i, "categoryPath", "", szCategroyFilePath, _MAX_PATH);

		KCategoryMap *pMapCategory = new KCategoryMap();
		szCategroyFileTemp = CATEGORY_FOLD_PATH;
		szCategroyFileTemp = szCategroyFileTemp + szCategroyFilePath;
		pCategroyFile = g_OpenTabFile(szCategroyFileTemp.CStr());
		QCONFIRM_RET_FALSE(pCategroyFile);
		for (int j = 2; j <= pCategroyFile->GetHeight(); j++)
		{
			INT nCategroyId = 0;
			pCategroyFile->GetInteger(j, "categoryId", 0, &nCategroyId);
			pCategroyFile->GetString(j, "goodsPath", "", szGoodsFilePath, _MAX_PATH);

			KGoodsVec* pVecGoods = new KGoodsVec();
			szGoodsFileTemp = GOODS_FOLD_PATH;
			szGoodsFileTemp = szGoodsFileTemp + szGoodsFilePath;
			pGoodsFile = g_OpenTabFile(szGoodsFileTemp.CStr());
			QCONFIRM_RET_FALSE(pGoodsFile);
			for (INT k = 2; k <= pGoodsFile->GetHeight(); k++)
			{
				KGoods *pGoods = new KGoods();
				pGoodsFile->GetInteger(k, "Id", 0, &pGoods->m_nId);
				pGoodsFile->GetInteger(k, "Genre", -1, &pGoods->m_sIndex.nGenre);
				pGoodsFile->GetInteger(k, "DetailType", -1, &pGoods->m_sIndex.nDetailType);
				pGoodsFile->GetInteger(k, "ParticularType", -1, &pGoods->m_sIndex.nParticular);
				pGoodsFile->GetInteger(k, "Level", -1, &pGoods->m_sIndex.nLevel);
				pGoodsFile->GetInteger(k, "Price", -1, &pGoods->m_nPrice);
				pGoodsFile->GetInteger(k, "stock", -1, &pGoods->m_nStock);
				pGoodsFile->GetInteger(k, "purchaseLimit", -1, &pGoods->m_nPurchaseLimit);
				pGoodsFile->GetInteger(k, "Bind", 0, &pGoods->m_nBind);
				pGoodsFile->GetInteger(k, "CurrencyType", 0, &pGoods->m_nCurrencyType);
				pGoodsFile->GetInteger(k, "vip", 0, &pGoods->m_nVip);
				pGoodsFile->GetInteger(k, "discount", 0, &pGoods->m_nDiscount);
				pGoodsFile->GetInteger(k, "limitType", 0, &pGoods->m_nLimitType);
				pGoodsFile->GetInteger(k, "limitTime", 0, &pGoods->m_nLimitTime);

				INT nTimeOut = 0;
				pGoodsFile->GetInteger(k, "effectiveTime", 0, &nTimeOut);
				pGoods->SetTimeout(nTimeOut);
				// 读取时间控制条件，时间轴和绝对时间两者是且的关系，必须同时满足

				// 开服多少天开始卖，未设置或者设置的值<=0视为无效（此时保持默认值0，表示未设置）
				INT nTempTimeFrameDay;
				if(pGoodsFile->GetInteger(k, "nTimeFrameStartDay", 0, &nTempTimeFrameDay) && nTempTimeFrameDay > 0)
					pGoods->SetTimeFrameStartDay(nTempTimeFrameDay);

				// 绝对时间（年月日时分，格式XXXX/XX/XX XX:XX, XX之间的分隔符可以是其他的非数字的符号)售卖的时间段,未设置或者设置的值不是指定的时间格式视为无效（此时保持默认值0，表示未设置）
				time_t timeTemp;
				if (TryGetDateTime(pGoodsFile, k, "timeSaleStart", timeTemp))
					pGoods->SetTimeSaleStart(timeTemp);
				if (TryGetDateTime(pGoodsFile, k, "timeSaleClose", timeTemp))
					pGoods->SetTimeSaleClose(timeTemp);

				//pGoodsFile->GetInteger(k, "maxCount", 0, &pGoods->m_maxCount);
				pGoods->m_maxCount = 100;

				
				pVecGoods->push_back(pGoods);
			}
			pMapCategory->insert(make_pair(nCategroyId, pVecGoods));
		}
		m_mapShop.insert(make_pair(nShopId, pMapCategory));
	}

EXITO:
	return TRUE;
}

BOOL KBuySellSetting::ClearShopMap()
{
	for (KShopMap::iterator it_shop = m_mapShop.begin(); it_shop != m_mapShop.end(); ++it_shop)
	{
		KCategoryMap* pCategoryMap = it_shop->second;
		for (KCategoryMap::iterator it_category = pCategoryMap->begin(); it_category != pCategoryMap->end(); ++ it_category)
		{
			KGoodsVec* pGoodsVec = it_category->second;
			for (KGoodsVec::iterator it_goods = pGoodsVec->begin(); it_goods != pGoodsVec->end(); ++it_goods)
			{
				SAFE_DELETE(*it_goods);
			}
			pGoodsVec->clear();
			SAFE_DELETE(it_category->second);
		}
		pCategoryMap->clear();
		SAFE_DELETE(it_shop->second);
	}
	m_mapShop.clear();

	return TRUE;
}



