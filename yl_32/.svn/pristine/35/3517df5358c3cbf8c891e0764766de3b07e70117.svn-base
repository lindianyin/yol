
#pragma once

#include "kgoods.h"
#include <map>

// 注意：此枚举不允许更改
enum KE_SERIES_TYPE
{
	series_none		= 0,				// 无系
	series_metal	= 1,				// 金系
	series_wood		= 2,				// 木系
	series_water	= 3,				// 水系
	series_fire		= 4,				// 火系
	series_earth	= 5,				// 土系
	series_num,
};

class KBuySellSetting
{

public:
	enum
	{
		emMAX_ITEMCOUNT = 150,
	};
	KBuySellSetting();
	~KBuySellSetting();

	BOOL	Init();
	BOOL	Release();
	//KGoods* GetGoodsByItemIdx(CONST KITEM_INDEX& crItemIdx);
	//KGoods* GetGoods(LPCSTR pszItemName);
	//KGoods* GetGoods(INT nId);
	//INT		GetGoodsIndex(INT nShop, INT nIndex);
	//INT		GetWidth() CONST	{ return m_nWidth; }
	//INT		GetHeight() CONST	{ return m_nHeight; }

#ifdef GAME_SERVER
	INT		ItemBusinessShopName2ID(LPSTR pszName);
#else
	// 获得商品在对应商店中的位置
	INT	 GetGoodsShopIdx(INT nGoodsId);
#endif

private:

	typedef std::map<std::string, INT>	MAP_GOODSNAME2INDEX;
	typedef std::map<INT, LPINT>		KShopGoodsMap;
	typedef std::map<INT, KGoods>		KGoodsMap;

	MAP_GOODSNAME2INDEX	m_mapGoodsName;		// 名字-下标索引对应表
	KGoodsMap			m_mapGoods;			// 商品表
	KShopGoodsMap		m_mapShopGoods;
	INT					m_nWidth;			// 商店模板的最多商品数（对应Buysell.txt的列数）
	INT					m_nHeight;			// 商店模板的个数（对应g_cBuySellMgr.txt的行数减1，g_cBuySellMgr.txt包含1行列名）

#ifdef GAME_SERVER
	QTabFile			m_tabItemShop;			
#endif

public:
	BOOL TryGetDateTime(ITabFile* pTabFile, INT nRowIdx, LPCSTR pszColName, time_t &t);
	BOOL TryParseTime(LPCSTR pszTime, time_t& t);

public:
	BOOL ReadShopCfg();
	BOOL ClearShopMap();

	KGoods* GetGoodsByGDPL(CONST KITEM_INDEX& crItemIdx);
	KGoods* GetGoodsByIndex(INT nShopIndex, INT nCategoryIndex, INT nGoodsIndex);
	//KGoodsVec* GetGoodsVecByIndex(INT nShopIndex, INT nCategoryIndex);
public:
	typedef std::vector<KGoods*>		KGoodsVec;
	typedef std::map<INT, KGoodsVec*>	KCategoryMap;
	typedef std::map<INT, KCategoryMap*> KShopMap;
private:
	KShopMap m_mapShop;
};


