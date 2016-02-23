/* -------------------------------------------------------------------------
//	文件名		：	qmysteryshopprotocol.h
//	创建者		：	NiuNiu
//	创建时间	：	2012-10-30 17:13:00
//	功能描述	：  神秘商店卖出物品协议处理
// -------------------------------------------------------------------------*/

#ifndef __GAMEWORLD_RPG_ONLINEGAMEMODULE_TRADE_MYSTERYSHOP_H__
#define __GAMEWORLD_RPG_ONLINEGAMEMODULE_TRADE_MYSTERYSHOP_H__

#define QMYSTERY_GOOD_MAX			9		//神秘商店最大的商品数量

#include "onlinegameworld/kprotocol.h"

enum QE_MYSTERY_SHOP_C2S_PROTOCOL
{
	c2s_mystery_shop_sync_buy_item,	// 购买道具
	c2s_mystery_shop_sync_refresh,		// 打开
	c2s_mystery_shop_count,			// 协议数量
};

#pragma pack(push, 1)		/*VS为了查找效率采用四字节， 此处为了减少协议数据包， 关闭此种方式 */

struct  QC2S_MYSTERY_SHOP_SYNC_BUY_ITEM : KPTC_HEADER<c2s_mystery_shop_sync_buy_item> 
{
	DWORD dwID;			// 购买物品的Id
	WORD  wCount;		// 购买的数量
	BYTE  bByIndex;		// 是否通过GDPL购买
};

struct QC2S_MYSTERY_SHOP_SYNC_REFRESH : KPTC_HEADER<c2s_mystery_shop_sync_refresh>
 {
	BYTE byRefresh;	//是否刷新， 此处用byte 代替bool
 };
#pragma pack(pop)

#endif