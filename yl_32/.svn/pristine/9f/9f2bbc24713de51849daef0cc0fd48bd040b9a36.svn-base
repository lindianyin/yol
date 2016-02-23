/* -------------------------------------------------------------------------
//	文件名		：	qsellsetting.h
//	创建者		：	NiuNiu
//	创建时间	：	2012-10-30 17:13:00
//	功能描述	：  神秘商店卖出物品设置
// -------------------------------------------------------------------------*/

#ifndef __QSELLSETTING_H__
#define	__QSELLSETTING_H__

#include "qmysterygoods.h"
#include "onlinegamebase/ksysservice.h"
#include <map>
#include <vector>

class QSellSetting
{
public:
	enum
	{
		emMAX_ITEMCOUNT = 150,
	};
	QSellSetting();
	~QSellSetting();

	BOOL			Init();
	BOOL			Release();
	std::vector<INT> GenerateGoodIds(INT nGoodAmount);
	QMysteryGoods*	GetGoodByItemIdx( CONST KITEM_INDEX& crItemIdx );
//	QMysteryGoods*	GetGoods( LPCSTR pszItemName );
	QMysteryGoods*	GetGoods(INT nId);

//	INT				GetGoodsId(INT nIndex);
	INT				GetWidth() CONST { return m_nWidth; }
	INT				GetHeight() CONST { return m_nHeight; }
	std::vector<INT> GetAllGoodRates() { return m_vecAllRates; }
	INT				GetIdByRate(INT rate);


private:
	typedef std::map<std::string, INT>		MAP_GOODSNAME2INDEX;
	typedef std::map<INT, QMysteryGoods>	QGoodsMap;
	typedef std::map<INT, LPINT>			QShopGoodsMap;

	MAP_GOODSNAME2INDEX		m_mapGoodsName;		// 名字-下标索引对应表
	QGoodsMap				m_mapGoods;			// 商品表
	QShopGoodsMap			m_mapShopGoods;
	INT						m_nWidth;
	INT						m_nHeight;

	std::vector<INT>		m_vecAllRates;
#ifndef GAME_SEVER
	QTabFile				m_tabItemShop;
#endif
};

#endif