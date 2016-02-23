
#pragma once

#include <string>
#include <time.h>
#include "kpursedata.h"
#include "onlinegamemodule/item2/itemdef.h"
//#include "kservertime.h"
#include "stdlib.h"

class KPromotionSaleDate
{

public:

	INT nBeginSaleDate;			// 开始出售日期20051202
	INT nEndSaleDate;			// 出售截止日期
	INT nBeginAbateDate;		// 开始打折日期
	INT nEndAbateDate;			// 打折截止日期

#ifndef _SERVER

	//开始打折日期
	LPSTR GetBeginAbateDate(LPSTR szDate)
	{
		InteagerToDateStr(nBeginAbateDate, szDate);
		return szDate;
	}

	//打折截止日期字符串
	LPSTR GetAbateEndDate(LPSTR szDate)
	{
		InteagerToDateStr(nEndAbateDate, szDate);
		return szDate;
	}

	//开始出售日期（下架日期）
	LPSTR GetSellEndDate(LPSTR szDate)
	{
		InteagerToDateStr(nEndSaleDate, szDate);
		return szDate;
	}

	//出售截止日期（下架日期）
	LPSTR GetBeginSellDate(LPSTR szDate)
	{
		InteagerToDateStr(nBeginSaleDate, szDate);
		return szDate;
	}

#endif

private:

#ifndef _SERVER

	BOOL PickUpDate(INT nDate, std::string& year, std::string& month, std::string& day)
	{
		if(nDate <= 0)
			return FALSE;

		CHAR szTmpDate[32] = {0};
		sprintf(szTmpDate, "%d", nDate);
		//itoa(nDate, szTmpDate, 10);

		std::string strTemp = szTmpDate;

		if(strTemp.length() != 8)
		{
			ASSERT(0 == "Time format of table Goods is illegal, it should be like“20051202”");
			return FALSE;
		}

		year = strTemp.substr(0, 4);
		month = strTemp.substr(4, 2);
		day = strTemp.substr(6, 7);

		return TRUE;
	}

	VOID InteagerToDateStr(INT nDate, LPSTR szDate)
	{
		if(szDate == NULL)
			return;

		std::string year, month, day;
		if(!PickUpDate(nDate, year, month, day))
			return;

		sprintf(szDate, "%s-%s-%s", year.c_str(), month.c_str(), day.c_str());

		return;
	}

	VOID InteagerToStandardDate(INT nDate, tm& tmDate)
	{
		std::string year, month, day;
		if(!PickUpDate(nDate, year, month, day))
			return;

		INT nYear = 0, nMonth = 0, nDay = 0;
		nYear = atoi(year.c_str()) - 1900;
		nMonth = atoi(month.c_str());
		nDay = atoi(day.c_str());

		if(nYear < 0 || nMonth <= 0 || nDay <= 0 || nMonth > 12 || nDay > 31)
		{
			ASSERT(0);
			return;
		}

		memset(&tmDate, 0,  sizeof(tmDate));
		tmDate.tm_year = nYear;
		tmDate.tm_mon = nMonth;
		tmDate.tm_mday = nDay;
	}

#endif

};

class KGoods
{

public:
	enum enumStatType
	{
		StatType_LateCache = 0,									// 最近缓存
		StatType_TotalForStart,									// 从服务器启动后的总数
		StatType_End,
	};

	struct KReputeCondition										// 声望条件
	{
		INT nCamp;												// 阵营
		INT nClass;												// 类别
		INT nLevel;												// 等级
	};

public:

	KGoods();
#ifndef _SERVER	// 此函数只在客户端使用，服务器端商品价格从KItem::GetPrice()获取
	INT 	GetGoodsPrice_Money();								// 获取商品的剑侠币售价
	INT		GetGoodsPrice_BindMoney();							// 获取商品的绑定银两售价
	INT		GetGoodsPrice_MachineCoin();						// 获取商品的机关力耐久度
#endif
	INT 	GetGoodsPrice_FuYuan();								// 获取商品的福缘售价
	INT 	GetGoodsPrice_Coin();								// 获取商品的铜钱售价 转为 获取商品的五行魂石兑换数量
	INT 	GetGoodsPrice_Score();								// 获取商品的积分售价
	INT 	GetGoodsPrice_Honour();								// 获取商品的荣誉点售价
	INT		GetGoodsPrice_FightPowerRank();						// 获取商品的战斗力排名价格
	INT 	GetGoodsPrice_OrginCoin();
	INT 	GetGoodsPrice_AbateCoin();
	INT 	GetGoodsPrice_Contribution();						// 获取商品的贡献度售价
	INT		GetGoodsPrice_TongFund();						// 获取商品售价(帮会建设资金)
	INT		GetGoodsPrice_CoinItem();							// 获取商店的金币替代物品售价
	INT		GetGoodsPrice_ValueCoin();							// 获取商品的数值货币售价
	INT 	GetGoodsPrice_BuildFund();							// 获取商品的建设度售价
	INT		GetGoodsPrice_TongEnergy(VOID)	{return m_nGoodsPrice_Energy; }
	DWORD	GetTimeout() CONST;										// 获取商品的时效
	BOOL	IsGetBind();											// 是否为获取绑定
	INT		GetItemCoinIndex();									// 获取金币替代物品在gamesetting中的index
	INT		GetValueCoinIndex();								// 获取实物货币在gamesetting中的index
	

	//VOID 	SetGoodsPrice_Money(INT nNewPrice);					// 设置商品的剑侠币售价
	VOID 	SetGoodsPrice_FuYuan(INT nNewPrice);				// 设置商品的福缘售价
	VOID 	SetGoodsPrice_AbateCoin(INT nNewPrice);				// 设置商品的铜钱售价
	VOID 	SetGoodsPrice_OrginCoin(INT nNewPrice);				// 设置商品的铜钱售价
	VOID 	SetGoodsPrice_Score(INT nNewPrice);					// 设置商品的积分售价
	VOID 	SetGoodsPrice_Contribution(INT nNewPrice);			// 获取商品的贡献度售价
	VOID 	SetGoodsPrice_TongFund(INT nNewPrice);			// 获取商品售价(帮会建设资金)
	VOID 	SetGoodsPrice_BuildFund(INT nNewPrice);				// 获取商品的建设度售价
	VOID 	SetGoodsPrice_Honour(INT nNewPrice);				// 设置商品的荣誉点售价
	VOID	SetGoodsPrice_FightPowerRank(INT nNewPrice);		// 设置商品的战斗力排名价格
	VOID	SetGoodsPrice_CoinItem(INT nGoodsIndex, INT nNewPrice);	// 设置商品的金币替代物品售价
	VOID	SetGoodsPrice_ValueCoin(INT nGoodsInidex, INT nNewPrice);	// 设置商品的数值货币售价
	VOID	SetGoodsPrice_TongEnergy(INT nEnergy){ m_nGoodsPrice_Energy = nEnergy;}
	VOID 	SetTimeout(DWORD dwTimeout);						// 设置商品的时效
	BOOL 	IsSaleLogRequired();								// 判断是否需要做交易日志
	VOID 	SetIfSaleLog(BOOL bLogRequired);					// 设置是否需要做交易日志
	VOID 	SetGoods_SaleDate(INT nBeginDate, INT nEndDate);
	VOID 	SetGoods_AbateDate(INT nBeginDate, INT nEndDate);

	INT		GetId(VOID) CONST;									// 获得商品索引
	VOID	SetId(INT nId);										// 设定商品索引
	LPCSTR	GetName(VOID) CONST;								// 获得商品名称
	VOID	SetGoods_Repute(CONST KReputeCondition *pCond);		// 设置声望需求
	BOOL	OnSale(KMONEYTYPE type, DWORD dwCount);
	DWORD	GetSaleStat(KMONEYTYPE currType, enumStatType statType);
	VOID	CleanSaleStat(KMONEYTYPE currType, enumStatType statType);

	KPromotionSaleDate* GetGoods_PromotionDate();		
	CONST KReputeCondition *GetGoods_Repute() CONST { return &m_sReputeCond; }

	VOID	SetGood_OfficialLevel(INT nOfficialLevel){ m_nOfficialLevel = nOfficialLevel;}
	INT		GetGood_OfficialLevel(VOID)	 {return m_nOfficialLevel;}

	INT		GetTimeFrameStartDay();
	VOID	SetTimeFrameStartDay(int nTimeFrameStartDay);

	time_t	GetTimeSaleStart();
	VOID	SetTimeSaleStart(time_t timeSaleStart);
	time_t  GetTimeSaleClose();
	VOID	SetTimeSaleClose(time_t timeSaleClose);

	INT		GetStock();
	VOID	SetStock(INT nStock);
	INT		GetPurchaseLimit();
	VOID	SetPurchaseLimit(INT nPursechaseLimit);
	INT		GetVip();
	VOID	SetVip(INT nVip);
	INT		GetDiscount();
	VOID	SetDiscount(INT nDiscount);

public:

#ifdef GAME_SERVER
	KITEM_INDEX	m_sIndex;										// 商品的道具索引
	INT			m_nSeries;										// 商品的五行
#else
	INT			m_nItemIdx;										// 商品对应的道具Index
#endif
	INT			m_nId;		
	INT			m_nBind;										// 是否绑定
	INT         m_nCurrencyType;                                // 此商品消耗的货币类型
	INT         m_nPrice;                                       // 价格
	INT			m_nStock;											//库存
	INT			m_nPurchaseLimit;								//限购
	INT			m_nVip;											//vip
	INT			m_nDiscount;									//折扣
	DWORD		m_dwTimeout;									// 时效
	INT			m_nLimitType;									//限购类型
	INT			m_nLimitTime;									//限购的时间间隔

	// 商品是否在销售状态的控制，由时间轴和绝对时间两部分限制，只有这两个条件都满足的时候才可以卖

	// 商品是否在销售状态的时间轴控制
	INT			m_nTimeFrameStartDay;								// 开服多少天开始卖(默认值0表示忽略此字段）

	// 商品是否在销售状态的绝对时间控制
	time_t		m_timeSaleStart;									// 什么时候开始卖（绝对时间，默认值0表示忽略此字段）
	time_t		m_timeSaleClose;									// 卖到什么时候结束（绝对时间，默认值0表示忽略此字段）
	INT         m_maxCount;											//全服限购最大数量
private:
									// 商品索引
	INT			m_nGoodsPrice_Money;							// 向NPC买时的剑侠币价格
	INT			m_nGoodsPrice_FuYuan;							// 福缘价格
	INT			m_nGoodsPrice_AbateCoin;						// 铜钱价格
	INT			m_nGoodsPrice_OrginCoin;						
	INT			m_nGoodsPrice_Score;							// 积分价格
	INT			m_nGoodsPrice_Contribution;						// 贡献度价格
	INT			m_nGoodsPrice_TongFund;							// 帮会建设资金价格
	INT			m_nGoodsPrice_Energy;							// 帮会行动力
	INT			m_nGoodsPrice_BuildFund;						// 建设度价格
	INT			m_nGoodsPrice_Honour;							// 荣誉点价格
	INT			m_nGoodsPrice_FightPowerRank;					// 战斗力排名价格
	BOOL		m_bSaleLogRequired;								// 是否需要做交易日志
	DWORD		m_dwSaleStats[EM_MONEY_MAX - 2][StatType_End];	// 物品销售统计(多种货币)
	INT			m_nOfficialLevel;								// 官衔条件

	INT			m_nItemCoinIndex;								// 实物货币的索引
	INT			m_nPrice_ItemCoin;								// 实物货币的价格

	INT			m_nValueCoinIndex;								// 数值货币的索引
	INT			m_nPrice_ValueCoin;								// 数值货币的价格

	KReputeCondition	m_sReputeCond;							// 声望条件
	KPromotionSaleDate	m_PromotionData;						// TODO: xyf 暂时无视打折问题

};


