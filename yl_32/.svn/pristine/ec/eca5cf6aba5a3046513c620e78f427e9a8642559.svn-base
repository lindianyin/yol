/* -------------------------------------------------------------------------
//	文件名		：	qsellsetting.h
//	创建者		：	NiuNiu
//	创建时间	：	2012-10-30 17:13:00
//	功能描述	：  神秘商店卖出物品设置
// -------------------------------------------------------------------------*/

#include "stdafx.h"
#include "qsellsetting.h"
#include "item2/kitemmgr.h"

//-------------------------------------神秘商店相关-------------------------
#define GOODS_FILE					"\\setting\\mysteryshop\\goods.txt"

/*列名相关*/
#define GOOD_NAME					"Name"
#define GOOD_ID						"Id"
#define GOOD_GENRE					"Genre"
#define GOOD_DETAIL_TYPE			"DetailType"
#define GOOD_PARTICULAR_TYPE		"ParticularType"
#define GOOD_LEVEL					"Level"
#define GOOD_BIND					"Bind"
#define GOOD_PRICE					"Price"
#define GOOD_CURRENCY_TYPE			"CurrencyType"
#define GOOD_TIME_FRAME_START_DAY	"nTimeFrameStartDay"
#define GOOD_TIME_SALE_START		"timeSaleStart"
#define GOOD_TIME_SALE_CLOSE		"timeSaleClose"
#define GOOD_RATE					"Rate"
#define GOOD_VALUEABLE				"Valueable"
#define GOOD_IS_OPEN				"IsOpen"
#define GOOD_COUNT					"Count"

#ifdef  _DEBUG

#define	new DEBUG_NEW
#undef  THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

QSellSetting::QSellSetting() : m_nWidth(0), m_nHeight(0)
{
	
}

QSellSetting::~QSellSetting()
{
	Release();
}

BOOL QSellSetting::Init()
{
	ASSERT(m_mapGoods.empty());

	QTabFile tabGoods;
	if (!tabGoods.Load(GOODS_FILE))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_nHeight	= tabGoods.GetHeight();
	m_nWidth	= tabGoods.GetWidth();

	if (0 == m_nHeight || 0 == m_nWidth)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	/*计算每个物品被随机到的概率*/
	INT totalRate = 0;
	m_vecAllRates.push_back(totalRate);
	for (INT nRow = 2; nRow <= m_nHeight; ++nRow)
	{
		INT nGoodID = 0; // 商品ID，从1开始
		tabGoods.GetInteger(nRow, "Id", 0, &nGoodID);
		if (nGoodID <= 0)
		{
			ASSERT(FALSE);
			continue;
		}
		
		QMysteryGoods& rcGoods = m_mapGoods[nGoodID];
		rcGoods.SetId(nGoodID);

		tabGoods.GetInteger(nRow, GOOD_GENRE, -1, &rcGoods.m_sIndex.nGenre);
		tabGoods.GetInteger(nRow, GOOD_DETAIL_TYPE, -1, &rcGoods.m_sIndex.nDetailType);
		tabGoods.GetInteger(nRow, GOOD_PARTICULAR_TYPE, -1, &rcGoods.m_sIndex.nParticular);
		tabGoods.GetInteger(nRow, GOOD_LEVEL, -1, &rcGoods.m_sIndex.nLevel);
		tabGoods.GetInteger(nRow, GOOD_BIND, 0, &rcGoods.m_nBind);

		tabGoods.GetInteger(nRow, GOOD_PRICE, -1, &rcGoods.m_nPrice);
		tabGoods.GetInteger(nRow, GOOD_CURRENCY_TYPE, 0, &rcGoods.m_nCurrencyType);
		tabGoods.GetInteger(nRow, GOOD_RATE, 0, &rcGoods.m_nRate);
		tabGoods.GetInteger(nRow, GOOD_VALUEABLE, 0, &rcGoods.m_nValueable);
		tabGoods.GetInteger(nRow, GOOD_IS_OPEN, 0, &rcGoods.m_nIsOpen);
		tabGoods.GetInteger(nRow, GOOD_COUNT, 0, &rcGoods.m_nCount);
		
		totalRate += rcGoods.m_nRate;
		//m_vecAllRates[nRow - 1] = totalRate;

		m_vecAllRates.push_back(totalRate);
		LPCSTR pszName = rcGoods.GetName();
		if (!pszName)
		{
			ASSERT(FALSE);
			continue;
		}
		m_mapGoodsName[pszName]	= nGoodID;			//名字-索引对应表
	}
	return TRUE;
}

std::vector<INT> QSellSetting::GenerateGoodIds(INT nGoodAmount)
{
	std::vector<INT> randIds;
	INT rateCount = m_vecAllRates.size();
	while (randIds.size() < nGoodAmount)
	{
		INT randResult = KSysService::Rand(m_vecAllRates[rateCount - 1]);
		INT goodId = GetIdByRate(randResult);

		if (0 != goodId)
		{
			size_t i = 0;
			for (; i < randIds.size(); ++ i)
			{
				if (goodId == randIds[i])
				{
					break;
				}
			}
			if (i == randIds.size())  //获得的id为可用的， 没有重复
			{
				randIds.push_back(goodId);
			}
		}

	}
	return randIds;
}


BOOL QSellSetting::Release()
{
	m_vecAllRates.clear();
	m_mapGoods.clear();
	m_mapGoodsName.clear();
	m_nHeight = 0;
	m_nWidth = 0;
	return TRUE;
}

QMysteryGoods* QSellSetting::GetGoodByItemIdx(CONST KITEM_INDEX& rcItemIdx)
{
	std::map<INT, QMysteryGoods>::iterator iter;
	for (iter = m_mapGoods.begin(); iter != m_mapGoods.end(); ++ iter)
	{
		QMysteryGoods* pGoods = &iter->second;
		if (pGoods->m_sIndex == rcItemIdx)
		{
			return pGoods;
		}
	}

	return NULL;
}

QMysteryGoods* QSellSetting::GetGoods(INT nId)
{
	if (nId < 0)
		return NULL;

	QMysteryGoods* pGoods = NULL;
	std::map<INT, QMysteryGoods>::iterator iter = m_mapGoods.find(nId);
	if (iter != m_mapGoods.end())
	{
		pGoods = &iter->second;
	}
	return pGoods;
}
INT QSellSetting::GetIdByRate(INT rate)
{
	INT i = 1;
	for (; i < m_vecAllRates.size(); ++ i)
	{
		if (rate <= m_vecAllRates[i])
			return i;
	}
	return 0;
}



