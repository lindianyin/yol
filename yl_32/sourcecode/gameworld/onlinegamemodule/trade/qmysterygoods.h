// -------------------------------------------------------------------------
//	文件名		：	qmysterygoods.h神秘商品
//	创建者		：	NiuNiu
//	创建时间	：	2012-10-30
// --------------------------------------------------------------------------

#ifndef __QMYSTERY_GOODS_H__
#define __QMYSTERY_GOODS_H__

#include <string>
#include "stdlib.h"
#include "kpursedata.h"
#include "onlinegamemodule/item2/itemdef.h"

class QMysteryGoods
{
public:
	QMysteryGoods();
	
	KITEM_INDEX GetIndex(VOID) CONST;
	INT		GetBindType(VOID) CONST;
	INT		GetPrice(VOID) CONST;


	BOOL	IsGetBind();											//	是否获取绑定
	DWORD	GetTimeout() CONST;										//	获取商品的实效
	VOID	SetTimeout(DWORD dwTimeout);							//	设置商品的实效	

	INT		GetId(VOID) CONST;										//	获得商品的索引
	VOID	SetId(INT nId);											//	设置商品的索引
	LPCSTR  GetName(VOID) CONST;									//  获取商品的名称
	INT		GetGenre(VOID) CONST;
	
	
	INT		GetTimeFrameStartDay();
	VOID	SetTimeFrameStartDay(int nTimeFrameStartDay);

	time_t	GetTimeSaleStart();
	VOID	SetTimeSaleStart(time_t timeSaleStart);

	time_t  GetTimeSaleClose();
	VOID	SetTimeSaleClose(time_t timeSaleClose);
protected:
private:

public:
	KITEM_INDEX	m_sIndex;											// 神秘商品的道具索引
	
	INT			m_nBind;											// 是否绑定
	INT			m_nCurrencyType;									// 此神秘商品消耗的金币类型
	INT			m_nPrice;											// 此神秘商品的价格
	INT			m_nRate;											// 商品的随机概率
	INT			m_nValueable;										// 是否贵重商品
	INT			m_nIsOpen;											// 商品的开关
	INT			m_nCount;											// 商品的个数

private:
	INT			m_nId;												// 商品的索引
	INT			m_nTimeFrameStartDay;								// 开服多少天开始卖(默认值0表示忽略此字段）
	DWORD		m_dwTimeout;										// 商品的实效
	// 商品是否在销售状态的绝对时间控制
	time_t		m_timeSaleStart;									// 什么时候开始卖（绝对时间，默认值0表示忽略此字段）
	time_t		m_timeSaleClose;									// 卖到什么时候结束（绝对时间，默认值0表示忽略此字段）
};

#endif