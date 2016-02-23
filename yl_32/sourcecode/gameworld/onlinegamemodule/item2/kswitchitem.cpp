
#include "stdafx.h"
#include "kswitchitem.h"
#include "kplayeritem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

// TODO: xyf 某种意义上这里改成一个接口而不是回调会更好些
KSwitchItem::KPROCPAIR KSwitchItem::ms_asProc[emROOM_NUM] =
{	// pfnCheck						pfnPick						pfnDrop
	{ &KSwitchItem::CheckEquip,		&KSwitchItem::PickEquip,	&KSwitchItem::DropEquip		},			// emROOM_EQUIP
	{ &KSwitchItem::CheckEquip,		&KSwitchItem::PickBase,		&KSwitchItem::DropEquipEx	},			// emROOM_EQUIPEX
	{ &KSwitchItem::CheckBase,		&KSwitchItem::PickBase,		&KSwitchItem::DropBase		},			// emROOM_MAINBAG
	{ &KSwitchItem::CheckExtBagBar,	&KSwitchItem::PickExtBagBar,&KSwitchItem::DropExtBagBar	},			// emROOM_EXTBAGBAR
	{ &KSwitchItem::CheckExtBag,	&KSwitchItem::PickBase,		&KSwitchItem::DropBase		},			// emROOM_EXTBAG
	{ &KSwitchItem::CheckBase,		&KSwitchItem::PickBase,		&KSwitchItem::DropBase		},			// emROOM_MAIL
	{ &KSwitchItem::CheckBase,		&KSwitchItem::PickBase,		&KSwitchItem::DropBase		},			// emROOM_KIN
};

BOOL KSwitchItem::ms_aabSwitchProc[emROOM_NUM][emROOM_NUM];

// -------------------------------------------------------------------------

VOID KSwitchItem::InitProc(VOID)
{
	for (INT i = 0; i < emROOM_NUM; ++i)
	{
		for (INT j = 0; j < emROOM_NUM; ++j)
		{
			ms_aabSwitchProc[i][j] = FALSE;
			ms_aabSwitchProc[j][i] = FALSE;
		}
	}

	// 目前所有切换操作都是双向的，如果以后有需要可以改变这里
#define _REG_PROC(LEFT, RIGHT) \
do {\
	ms_aabSwitchProc[(LEFT)][(RIGHT)] = TRUE;\
	ms_aabSwitchProc[(RIGHT)][(LEFT)] = TRUE;\
}while(0)
#define _REG_PROC_BAG(ROOM) \
do {\
	_REG_PROC((ROOM), emROOM_MAINBAG);\
	_REG_PROC((ROOM), emROOM_EXTBAG);\
}while(0)

	_REG_PROC(emROOM_EQUIP,		emROOM_EQUIPEX);
	_REG_PROC(emROOM_EXTBAGBAR,	emROOM_EXTBAGBAR);

	_REG_PROC_BAG(emROOM_EQUIP);
	_REG_PROC_BAG(emROOM_EQUIPEX);
	_REG_PROC_BAG(emROOM_MAINBAG);
	_REG_PROC_BAG(emROOM_EXTBAG);
	_REG_PROC_BAG(emROOM_EXTBAGBAR);
#undef _REG_PROC_BAG
#undef _REG_PROC

}

BOOL KSwitchItem::Switch(CONST KITEM_POS& rsLeft, CONST KITEM_POS& rsRight)
{
	if (rsLeft.eRoom < 0 || rsLeft.eRoom >= emROOM_NUM || rsRight.eRoom < 0 || rsRight.eRoom >= emROOM_NUM ||
		rsLeft.nX  < 0 || rsLeft.nX  >= m_rcPlayerItem.m_acRoom[rsLeft.eRoom].GetWidth() ||
		rsLeft.nY  < 0 || rsLeft.nY  >= m_rcPlayerItem.m_acRoom[rsLeft.eRoom].GetHeight() ||
		rsRight.nX < 0 || rsRight.nX >= m_rcPlayerItem.m_acRoom[rsRight.eRoom].GetWidth() ||
		rsRight.nY < 0 || rsRight.nY >= m_rcPlayerItem.m_acRoom[rsRight.eRoom].GetHeight())		// 参数合法性检查
		return	FALSE;

	if (rsLeft.eRoom == rsRight.eRoom && rsLeft.nX == rsRight.nX && rsLeft.nY == rsRight.nY)	// 不需要交换
		return	FALSE;

//#ifdef GAME_SERVER
//	if (m_rcPlayerItem.m_rcPlayer.m_cPlayerTrade.IsTrading())
//		return	FALSE;			// 如果正在交易
//#endif

	INT nLeft  = m_rcPlayerItem.GetItemByPos(rsLeft);
	INT nRight = m_rcPlayerItem.GetItemByPos(rsRight);
	INT nLeftList  = 0;
	INT nRightList = 0;

	if (nLeft > 0)
	{
		nLeftList = m_rcPlayerItem.FindSame(nLeft);
		if (nLeftList <= 0)
		{
			ASSERT(FALSE);
			return	FALSE;
		}
	}

	if (nRight > 0)
	{
		nRightList = m_rcPlayerItem.FindSame(nRight);
		if (nRightList <= 0)
		{
			ASSERT(FALSE);
			return	FALSE;
		}
	}

	if (nLeftList == nRightList || (nLeftList <= 0 && nRightList <= 0))
		return	FALSE;			// 两边东西一样或者都没东西，不交换

	if (!ms_aabSwitchProc[rsLeft.eRoom][rsRight.eRoom])
		return	FALSE;			// 没有注册交换回调函数，不能交换

	if (m_rcPlayerItem.m_acRoom[rsLeft.eRoom].GetCapability() <= 0 ||
		m_rcPlayerItem.m_acRoom[rsRight.eRoom].GetCapability() <= 0)
		return	FALSE;			// 未被激活的空间

	INT nCheck = rsLeft.eRoom < rsRight.eRoom ? nLeft : nRight;
	if (!m_rcPlayerItem.CanUnequip() &&	MAX(rsLeft.eRoom, rsRight.eRoom) > emROOM_EQUIPEX &&
		MIN(rsLeft.eRoom, rsRight.eRoom) <= emROOM_EQUIPEX && nCheck > 0)			// 检查是否允许脱下装备
		return FALSE;

	CheckProc pfnLeftCheck  = ms_asProc[rsLeft.eRoom].pfnCheck;
	PickProc  pfnLeftPick   = ms_asProc[rsLeft.eRoom].pfnPick;
	DropProc  pfnLeftDrop   = ms_asProc[rsLeft.eRoom].pfnDrop;
	CheckProc pfnRightCheck = ms_asProc[rsRight.eRoom].pfnCheck;
	PickProc  pfnRightPick  = ms_asProc[rsRight.eRoom].pfnPick;
	DropProc  pfnRightDrop  = ms_asProc[rsRight.eRoom].pfnDrop;

	if (!pfnLeftCheck || !pfnLeftPick || !pfnLeftDrop || !pfnRightCheck || !pfnRightPick || !pfnRightDrop)
		return	FALSE;			// 注册有误

	if (!(this->*pfnLeftCheck)(rsLeft, nLeft, nRight))
		return	FALSE;

	if (!(this->*pfnRightCheck)(rsRight, nRight, nLeft))
		return	FALSE;
	
#ifdef GAME_SERVER
	if (m_rcPlayerItem.StackItem(nLeft, nRight))	// 如果两者能叠加就叠加，不用走交换协议了
		return FALSE;
#endif

	m_SwitchNotify.SwitchBegin();

	(this->*pfnLeftPick)(rsLeft, nLeftList);		// 左侧拿起处理
	m_SwitchNotify.OnPickNotify(rsLeft, nLeftList);
	(this->*pfnRightPick)(rsRight, nRightList);		// 右侧拿起处理
	m_SwitchNotify.OnPickNotify(rsRight, nRightList);
	(this->*pfnLeftDrop)(rsLeft, nRightList);		// 左侧放下处理
	m_SwitchNotify.OnDropNotify(rsLeft, nRightList);
	(this->*pfnRightDrop)(rsRight, nLeftList);		// 右侧放下处理
	m_SwitchNotify.OnDropNotify(rsRight, nLeftList);

	m_SwitchNotify.SwitchEnd();
	return	TRUE;
}

BOOL KSwitchItem::CheckBase(CONST KITEM_POS& rsPos, INT nPick, INT nDrop)
{	// 必须保证一个格子先Pick成功的话之后一定能成功Drop，所以这里只检查是否能Pick
	KItemRoom& rcRoom = m_rcPlayerItem.m_acRoom[rsPos.eRoom];
	if (nPick > 0 && !rcRoom.IsPickable(nPick, rsPos.nX, rsPos.nY))
		return	FALSE;
	return	TRUE;
}

BOOL KSwitchItem::CheckEquip(CONST KITEM_POS& rsPos, INT nPick, INT nDrop)
{
	if (!CheckBase(rsPos, nPick, nDrop))
		return	FALSE;
#ifdef GAME_SERVER
	if (nDrop > 0 && !m_rcPlayerItem.CanEquip(Item[nDrop], rsPos.nX))		// 穿上新装备需要条件
		return	FALSE;
#endif
	return	TRUE;
}

BOOL KSwitchItem::CheckEquipEx2(CONST KITEM_POS& rsPos, INT nPick, INT nDrop)
{
	if (!CheckBase(rsPos, nPick, nDrop))
		return	FALSE;
#ifdef GAME_SERVER
	if (nDrop > 0 && !m_rcPlayerItem.CanEquip(Item[nDrop], rsPos.nX))		// 穿上新装备需要条件
		return	FALSE;

	if (m_rcPlayerItem.m_BuyInfo.m_nBuyIdx != BuySellInfo::BUY_INDEX_REPOSITORY)	// 看是否处于开箱状态
		return	FALSE;
#endif
	return	TRUE;
}

BOOL KSwitchItem::CheckRepository(CONST KITEM_POS& rsPos, INT nPick, INT nDrop)
{
	if (!CheckBase(rsPos, nPick, nDrop))
		return	FALSE;
#ifdef GAME_SERVER
	if (m_rcPlayerItem.m_BuyInfo.m_nBuyIdx != BuySellInfo::BUY_INDEX_REPOSITORY)	// 看是否处于开箱状态
		return	FALSE;
#endif
	return	TRUE;
}

BOOL KSwitchItem::CheckExtBagBar(CONST KITEM_POS& rsPos, INT nPick, INT nDrop)
{
	if (!CheckBase(rsPos, nPick, nDrop))
		return	FALSE;

	INT nWidth = 0, nHeight = 0;
	KE_ITEM_ROOM eRoom = g_aeExtBagPos2Room[rsPos.nX];

	if (nPick > 0)
	{
		if (!Item[nPick].GetExtBagSize(nWidth, nHeight))
			return	FALSE;
		if (nDrop <= 0 && !m_rcPlayerItem.m_acRoom[eRoom].CanResize(0, 0))	// 如果是交换背包则不检查原背包是不是空的
			return	FALSE;
	}

	if (nDrop > 0)
	{
		if (!Item[nDrop].GetExtBagSize(nWidth, nHeight))
			return	FALSE;
		if (!m_rcPlayerItem.m_acRoom[eRoom].CanResize(nWidth, nHeight))
			return	FALSE;
		if (Item[nDrop].GetBagPosLimit() > 0 && rsPos.nX != Item[nDrop].GetBagPosLimit() - 1)
		{
			return FALSE;
		}
	}

	return	TRUE;
}

BOOL KSwitchItem::CheckExtBag(CONST KITEM_POS& rsPos, INT nPick, INT nDrop)
{
	if (!CheckBase(rsPos, nPick, nDrop))
		return	FALSE;

	if (nDrop > 0)
	{
		KITEM_POS sPos;
		VERIFY(m_rcPlayerItem.GetItemPos(nDrop, sPos));
		if (sPos.eRoom == emROOM_EXTBAGBAR && g_aeExtBagPos2Room[sPos.nX] == rsPos.eRoom)
			return	FALSE;
	}

	return	TRUE;
}

BOOL KSwitchItem::CheckExtRep(CONST KITEM_POS& rsPos, INT nPick, INT nDrop)
{
	if (!CheckBase(rsPos, nPick, nDrop))
		return	FALSE;
#ifdef GAME_SERVER
	if (m_rcPlayerItem.m_BuyInfo.m_nBuyIdx != BuySellInfo::BUY_INDEX_REPOSITORY)	// 看是否处于开箱状态
		return	FALSE;
#endif
	if (nPick > 0 && nDrop == 0)
	{
		return TRUE;
	}
	else if (nDrop > 0)
	{
		return TRUE;
	}
	return	FALSE;
}

VOID KSwitchItem::PickBase(CONST KITEM_POS& rsPos, INT nPickList)
{
	if (nPickList <= 0)
		return;

	KLISTITEM& rsList = m_rcPlayerItem.m_asList[nPickList];
	VERIFY(m_rcPlayerItem.m_acRoom[rsPos.eRoom].PickItemDirectly(rsList.nIdx, rsPos.nX, rsPos.nY));
	rsList.sPos.eRoom  = emROOM_NONE;
	rsList.sPos.nX	   = 0;
	rsList.sPos.nY	   = 0;
}

VOID KSwitchItem::PickEquip(CONST KITEM_POS& rsPos, INT nPickList)
{
	if (nPickList <= 0)
		return;

	KLISTITEM& rsList = m_rcPlayerItem.m_asList[nPickList];
	KItem& rcItem = Item[rsList.nIdx];
	INT nEquipPos = rsPos.nX;

	// todo [7/6/2012 simon]

	BOOL bUseing = TRUE;		// 该装备正在被应用中才能去除其属性应用，不在应用中不去除属性应用
	if (bUseing)
	{
		m_rcPlayerItem.RemoveNpcRes(nEquipPos);					// 移除换装效果
		//m_rcPlayerItem.RemoveFeature(nEquipPos);				// 移除易容效果
		//m_rcPlayerItem.RemoveActiveMAAttrib(nEquipPos);			// 移除该装备的局部易容效果

		m_rcPlayerItem.RemoveBaseAttrib(nEquipPos);				// 移除基础属性
		m_rcPlayerItem.RemoveEnhanceAttrib(nEquipPos);		// 移除强化属性
		m_rcPlayerItem.RemoveAppendAttrib(nEquipPos);
		m_rcPlayerItem.RemoveStoneAttribArr(nEquipPos);			// 移除宝石属性
	}

	PickBase(rsPos, nPickList);
}

VOID KSwitchItem::PickExtBagBar(CONST KITEM_POS& rsPos, INT nPickList)
{
	if (nPickList <= 0)
		return;
	KLISTITEM& rsList = m_rcPlayerItem.m_asList[nPickList];
	m_rcPlayerItem.ResizeRoom(g_aeExtBagPos2Room[rsPos.nX], 0, 0);		// 如果是交换背包，可能会失败，但是在这里是正常的！！
	PickBase(rsPos, nPickList);
}

VOID KSwitchItem::DropBase(CONST KITEM_POS& rsPos, INT nDropList)
{
	if (nDropList <= 0)
		return;

	KLISTITEM& rsList = m_rcPlayerItem.m_asList[nDropList];
	VERIFY(m_rcPlayerItem.m_acRoom[rsPos.eRoom].DropItemDirectly(rsList.nIdx, rsPos.nX, rsPos.nY));
	rsList.sPos.eRoom  = rsPos.eRoom;
	rsList.sPos.nX	   = rsPos.nX;
	rsList.sPos.nY	   = rsPos.nY;
}

VOID KSwitchItem::DropEquip(CONST KITEM_POS& rsPos, INT nDropList)
{
	if (nDropList <= 0)
		return;

	KLISTITEM& rsList = m_rcPlayerItem.m_asList[nDropList];
	KItem& rcEquip = Item[rsList.nIdx];
	INT nEquipPos = rsPos.nX;

	BOOL bUseing = TRUE;		// 该装备正在被应用中才能去除其属性应用，不在应用中不去除属性应用

	if (emITEM_BIND_EQUIP == rcEquip.GetBindType())
	{
		rcEquip.Bind();										// 装备时绑定
	}

	if (bUseing)
	{
		m_rcPlayerItem.ClearItemSellPrice(rsList.nIdx);			// 穿上装备把贩卖价格去掉
	}	

	DropBase(rsPos, nDropList);
	if (bUseing)
	{
		m_rcPlayerItem.ApplyBaseAttrib(nEquipPos);				// 应用该装备的基础属性

		m_rcPlayerItem.ApplyEnhanceAttrib(nEquipPos);		// 应用该装备的强化属性

		m_rcPlayerItem.ApplyAppendAttrib(nEquipPos);

		m_rcPlayerItem.ApplyStoneAttribArr(nEquipPos);			// 应用该装备的宝石属性

		m_rcPlayerItem.ApplyNpcRes(nEquipPos);					// 应用换装效果
	}
}

VOID KSwitchItem::DropEquipEx(CONST KITEM_POS& rsPos, INT nDropList)
{
	if (nDropList <= 0)
		return;

	KItem& rcEquip = Item[m_rcPlayerItem.m_asList[nDropList].nIdx];
	if (emITEM_BIND_EQUIP == rcEquip.GetBindType())
		rcEquip.Bind();							// 装备时绑定

	DropBase(rsPos, nDropList);
}

VOID KSwitchItem::DropExtBagBar(CONST KITEM_POS& rsPos, INT nDropList)
{
	if (nDropList <= 0)
		return;

	KLISTITEM& rsList = m_rcPlayerItem.m_asList[nDropList];
	KItem& rcExtBag = Item[rsList.nIdx];
	KE_ITEM_ROOM eRoom = g_aeExtBagPos2Room[rsPos.nX];
	INT nWidth = 0, nHeight = 0;

	VERIFY(rcExtBag.GetExtBagSize(nWidth, nHeight));
	VERIFY(m_rcPlayerItem.ResizeRoom(eRoom, nWidth, nHeight));

	if (emITEM_BIND_EQUIP == rcExtBag.GetBindType())
		rcExtBag.Bind();										// 装备时绑定

	DropBase(rsPos, nDropList);
}

// -------------------------------------------------------------------------

BOOL KSwitchNotify::SwitchBegin() 
{
	return TRUE;
}

BOOL KSwitchNotify::SwitchEnd() 
{
	return TRUE;
}

BOOL KSwitchNotify::OnPickNotify(CONST KITEM_POS& rsPos, INT nPickList) 
{
	return TRUE;
}

BOOL KSwitchNotify::OnDropNotify(CONST KITEM_POS& rsPos, INT nPickList) 
{
	return TRUE;
}

// -------------------------------------------------------------------------

