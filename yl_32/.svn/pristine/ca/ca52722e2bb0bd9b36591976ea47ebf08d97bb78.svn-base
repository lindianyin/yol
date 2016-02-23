#ifndef	_KBUYSELLMGR_H__
#define	_KBUYSELLMGR_H__

//#include "api/gamedatadef.h"
#include "kgoods.h"
#include "qshopprotocol.h"
#include "item2/kmanager_i.h"

#include "kbuysellsetting.h"
#include <map>
#include <string>
#include "serverbase/protocol/Relay_GS_Protocol.h"

extern BOOL g_bGlobalServer;

class KItemRoom;

class KBuySellMgr : public IKManager
{
public:	
	enum SALE_BEHAVIOR
	{
		BUY,
		SELL
	};
private:
	KBuySellSetting m_cSetting;
	std::vector<KBuyAllPurchaseRecord> m_vecBuyAllPurchaseRecord;
#ifdef GAME_SERVER
	QLogFile		m_SaleLogFile;						//物品买卖的Log文件
#endif

public:
	KBuySellMgr();
	~KBuySellMgr();

	LPCSTR	GetName() CONST	{ return "BuySell"; };

	BOOL Init();
	BOOL Release();
	//INT GetWidth() CONST
	//{
	//	return m_cSetting.GetWidth();
	//}
	//INT GetHeight() CONST
	//{
	//	return m_cSetting.GetHeight();
	//}
	//KGoods* GetGoods(LPCSTR pszItemName)
	//{
	//	return m_cSetting.GetGoods(pszItemName);
	//}
	//KGoods* GetGoods(INT nIndex)
	//{
	//	return m_cSetting.GetGoods(nIndex);
	//}
	KGoods* GetGoodsByIndex(INT nShopIndex, INT nCategoryIndex, INT nGoodsIndex)
	{
		return m_cSetting.GetGoodsByIndex(nShopIndex, nCategoryIndex, nGoodsIndex);
	}
	//BOOL GetBindType(INT nIndex)					// 是否为获取绑定类型
	//{
	//	BOOL bRet	= FALSE;
	//	KGoods* pGood	= m_cSetting.GetGoods(nIndex);
	//	if(pGood)
	//		bRet	= pGood->IsGetBind();
	//	return bRet;
	//}
	//BOOL			IsGoodsOverdue(KGoods* pGoods);//判断一个商品是否过期

	//全服限购商品
	BOOL OnGetAllBuyPurchaseRecordRespond(BYTE* pRecordData, size_t uRecordDataLen);

	KBuyAllPurchaseRecord* GetAllPurchaseRecord(INT nShopId, INT nCategoryId, INT nGoodsId);

	CONST std::vector<KBuyAllPurchaseRecord>& GetAllPurchaseRecord(){ return m_vecBuyAllPurchaseRecord; }
	
#ifdef GAME_SERVER
	//VOID	OpenSale(INT nPlayerIdx, INT nShop, KCURRENCYTYPE eCurrencyType, INT nScale = 100, INT nPlayerSeries = -1, LPCSTR szCallBack = NULL);
	VOID	OpenItemBussinessSale(INT nPlayerIdx, INT nShop, KMONEYTYPE eCurrencyType, INT nScale /*= 100*/, LPCSTR szShopName);
	BOOL	Buy(INT nPlayerIndex, KPLAYER_BUY_ITEM_SYNC *pCmd);
	BOOL	Buy_Sync(INT nPlayerIdx, KMONEYTYPE nCurrencyType, INT nBuy, INT nBuyIndex, INT nCost, INT nCount);
	BOOL	Sell(DWORD dwPlayerIndex, KPLAYER_SELL_ITEM_SYNC *pCmd);
	BOOL	DiscountSync(INT nPlayerIdx, INT nShopId, INT nCategoryId);
	BOOL	Buy_Purchase_Limit(INT nPlayerIndex, R2S_BUY_ALL_PURCHASE_REQUEST *pCmd);

	//写交易日志记录
	BOOL	WriteLog(DWORD			dwPlayerIndex,
					 KGoods*		pGoods,
					 INT			nPrice,
					 KMONEYTYPE	eCurrencyType, 
					 INT			nShop,
					 SALE_BEHAVIOR	eSaleBehavior,
					 BOOL			bFoceWriting = FALSE);
	BOOL	WriteLog(DWORD dwPlayerIndex, LPSTR pszMsg);
	INT ItemBusinessShopName2ID(LPSTR pszShopName)
	{
		return m_cSetting.ItemBusinessShopName2ID(pszShopName);
	}
#endif

	BOOL ProtocolProcess(LPCBYTE pData, INT nDataLen, KPlayer& rcPlayer);

private:
#ifdef GAME_SERVER
	VOID	PlayerSellItem(KPlayer* pPlayer, LPCBYTE pData, INT nDataLen);
	VOID	PlayerBuyItem(KPlayer* pPlayer, LPCBYTE pData, INT nDataLen);
	// GS/Client之间处理协议，返回协议是否正确（出错时踢玩家下线）
	// 注册协议处理接口，ppHandlers是一个长度为256的IKManager指针数组
	//virtual VOID RegisterProtocol(IKManager** ppHandlers);
	//BOOL	InitItemSaleLog();					//初始化物品交易Log文件
	BOOL	BuyCallBack(INT nPlayerIdx, INT nItemIdx, INT nPrice);
	INT		SellPlayerItem(KPlayer& rcPlayer, INT nItemIdx, INT& rnCount);
	INT		CreateGoods(KPlayer* pPlayer, CONST KGoods* pGoods, KMONEYTYPE eCurrencyType, INT& rnCount);// CONST;	// 建立商品对象
	VOID	RemoveGoods(INT nItemIdx) CONST;	// 删除添加失败的商品
	VOID	SetGoodsTimeout(CONST KGoods* pGoods, INT nItemIdx) CONST; //设置物品的时效
#endif

public:
	// 指定索引的商品是否在销售状态
	BOOL IsGoodsOnSale(INT nIndex);
	// 设置商品的有效时间, szType是列名
	BOOL SetGoodsTime(INT nId, LPCSTR szType, LPCSTR szTime);

private:
	// 获取服务器开服时间
	BOOL TryGetServerStartTime(INT &nServerStartTime);
	// 获得开服第nDayIndex天对应的绝对时间
	BOOL TryGetAbsoluteTime(INT nDayIndex, INT& nAbsoluteTime);
};

extern KBuySellMgr	g_cBuySellMgr;

#endif //_KBUYSELLMGR_H__
