#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "kbuysellmgr.h"

struct ShopSaveData
{
	INT nShopId;			//商店id
	INT nCategoryId;		//标签id
	INT nGoodsId;			//物品id
	INT nCount;				//购买总数
	DWORD dwLastTime;		//最后购买时间
};

class PlayerShop : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_SHOP	};

	explicit PlayerShop(KCharacter& rcPlayer);
	~PlayerShop();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	//virtual BOOL Active();

	// 读盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 存盘
	virtual BOOL OnDbSave(LPBYTE pData, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	BOOL DoBuyGoods(INT nShopId, INT nCategoryId, INT nGoodsId, INT nCount);
	
	BOOL SyncMyPurchaseRecord();

	BOOL SyncAllPurchaseRecord();
	
private:
	KPlayer&						m_rcPlayer;

	std::vector<ShopSaveData>		m_vecSaveData;

	//LuaPlayerTitle* m_pLunaObj;

	BOOL IndividualPurchase(KGoods *pGoods, INT nShopId, INT nCategoryId, INT nGoodsId, INT nCount);
};