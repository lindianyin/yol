
#include "stdafx.h"
#include "kluaplayeritem.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegamemodule/item2/itemdef.h"
#include "./kitem.h"
#include "./kplayeritem.h"
#include "./kluaitem.h"
#include "onlinegamebase/ksysservice.h"
#include "./kitemmgr.h"
#include "onlinegameworld/kluanpc.h"


KLuaPlayerItem::KLuaPlayerItem(KPlayer* pPlayer) : m_pMe(pPlayer)
{
	m_pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	assert(m_pPlayerItem);
}

KPlayer* KLuaPlayerItem::GetMe(QLuaScript& sc, INT nIndex)
{
	KLuaPlayerItem* pLuaPlayer = dynamic_cast<KLuaPlayerItem *>(GetCObj(sc.GetLuaState(), nIndex));
	if (pLuaPlayer)
		return pLuaPlayer->m_pMe;

	return NULL;
}

INT KLuaPlayerItem::RegisterToPlayer(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLuaPlayer = static_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_NULL(NULL != pLuaPlayer);

	KPlayer* pPlayer = pLuaPlayer->GetHim();
	QCONFIRM_RET_NULL(NULL != pPlayer);

	KPlayerItem* pPlayerCtrl = pPlayer->GetCtrl<KPlayerItem>();
	sc.PushObj(pPlayerCtrl->GetScriptInterface());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetItem
// @Description		: 获得角色指定位置的道具
// @ReturnCode		: 返回道具对象，失败返回nil
// @ArgumentFlag	: ddd
// @ArgumentComment	: nRoom:道具所在空间
// @ArgumentComment	: nX:横坐标
// @ArgumentComment	: nY:纵坐标
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaGetItem(QLuaScript& sc)
{
	KE_ITEM_ROOM eRoom = (KE_ITEM_ROOM)sc.GetInt(1);
	INT nX = sc.GetInt(2);
	INT nY = sc.GetInt(3);

	INT nItem = m_pPlayerItem->GetItemByPos(eRoom, nX, nY);
	if (nItem <= 0)
		return	0;

	sc.PushObj(Item[nItem].m_pLuna);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetEquipByDetail
// @Description		: 根据D值获得角色身上装备
// @ReturnCode		: 返回道具对象，失败返回nil
// @ArgumentFlag	: d
// @ArgumentComment	: nDetail
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaGetEquipByDetail(QLuaScript& sc)
{
	INT nDetail = sc.GetInt(1);
	KE_ITEM_ROOM eRoom = emROOM_EQUIP;
	INT nX = g_anEquipPos[nDetail];
	INT nY = 0;

	INT nItem = m_pPlayerItem->GetItemByPos(eRoom, nX, nY);
	if (nItem <= 0)
		return	0;

	sc.PushObj(Item[nItem].m_pLuna);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetItemCount
// @Description		: 获取指定空间中物品的数量
// @ReturnCode		: 返回总数量，空间不存在返回nil
// @ArgumentFlag	: d[?????]
// @ArgumentComment	: nRoom
// @ArgumentComment	: nGenre
// @ArgumentComment	: nDetail
// @ArgumentComment	: nParticular
// @ArgumentComment	: nBind
// @ArgumentComment	: nTimeOut
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaGetItemCount(QLuaScript& sc)
{
	KE_ITEM_ROOM eRoom	= (KE_ITEM_ROOM)sc.GetInt(1);
	INT nGenre			= ITEM_INVALID_TYPE;
	INT nDetail			= ITEM_INVALID_TYPE;
	INT nParticular		= ITEM_INVALID_TYPE;
	INT nBind			= KD_ITEM_NO_BIND_LIMIT;
	INT	nTimeOut		= KD_ITEM_NO_TIMEOUT_LIMIT;

	if (eRoom < 0 || eRoom > emROOM_NUM)
		return	0;
	if (sc.IsNumber(2))
		nGenre = sc.GetInt(2);
	if (sc.IsNumber(3))
		nDetail = sc.GetInt(3);
	if (sc.IsNumber(4))
		nParticular	= sc.GetInt(4);
	if (sc.IsNumber(7))
		nBind = sc.GetInt(7);
	if (sc.IsNumber(8))
		nTimeOut = sc.GetInt(8);

	sc.PushNumber(m_pPlayerItem->GetItemCount(eRoom, nGenre, nDetail, nParticular, nBind, nTimeOut));
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: HaveItem
// @Description		: 看一个人是否拥有某件道具，并获得所在空间和坐标
// @ReturnCode		: 没找到返回nil，否则返回一个Table：{ nRoom = 道具所在空间, nX = 横坐标, nY = 纵坐标 }
// @ArgumentFlag	: o
// @ArgumentComment	: pItem:目标道具的对象
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaHaveItem(QLuaScript& sc)
{
	KLuaItem* pItem = dynamic_cast<KLuaItem*>(sc.GetObj(1));
	if (!pItem)
		return	0;		// 参数不对

	KITEM_POS sPos;
	INT nIndex = pItem->GetIt()->GetGameIndex();

	if (!m_pPlayerItem->GetItemPos(nIndex, sPos))
		return	0;

	sc.PushTable();
	sc.PushNumber(sPos.eRoom);
	sc.SetTableField("nRoom");
	sc.PushNumber(sPos.nX);
	sc.SetTableField("nX");
	sc.PushNumber(sPos.nY);
	sc.SetTableField("nY");
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: CountFreeBagCell
// @Description		: 获取所有背包中空闲格子的数目
// @ReturnCode		: 空闲格子数
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaCountFreeBagCell(QLuaScript& sc)
{
	sc.PushNumber(m_pPlayerItem->CountBagFreeCell());
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: CalcFreeItemCountInBags
// @Description		: 计算所有背包空间中还可放置某个物品的数量
// @ReturnCode		: 还可放置物品的数量
// @ArgumentFlag	: dddd
// @ArgumentComment	: nGenre
// @ArgumentComment	: nDetail
// @ArgumentComment	: nParticular
// @ArgumentComment	: bBind
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayerItem::LuaCalcFreeItemCountInBags(QLuaScript& rcScript)
{
	INT nCount = 0;
	KITEM_INDEX sIndex;
	sIndex.nGenre			= rcScript.GetInt(1);
	sIndex.nDetailType		= rcScript.GetInt(2);
	sIndex.nParticular		= rcScript.GetInt(3);
	BOOL bBind				= (BOOL)rcScript.GetInt(6);
	nCount = m_pPlayerItem->CalcFreeItemCountInBags(sIndex, bBind);
	rcScript.PushNumber(nCount);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: FindItem
// @Description		: 在指定空间里查找指定类型的道具
// @ReturnCode		: 返回找到的道具列表，nil表示失败（参数不正确），空Table表示没找到
// @ArgumentFlag	: d[????]
// @ArgumentComment	: nRoom
// @ArgumentComment	: nGenre/szClass
// @ArgumentComment	: nDetail
// @ArgumentComment	: nParticular
// @ArgumentComment	: nLevel
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaFindItem(QLuaScript& sc)
{
	KE_ITEM_ROOM eRoom	= (KE_ITEM_ROOM)sc.GetInt(1);
	INT nGenre			= ITEM_INVALID_TYPE;
	INT nDetail			= ITEM_INVALID_TYPE;
	INT nParticular		= ITEM_INVALID_TYPE;
	INT nLevel			= ITEM_INVALID_TYPE;

	if (eRoom < 0 || eRoom >= emROOM_NUM)
		return	0;

	if (sc.IsNumber(2))
		nGenre = sc.GetInt(2);
	if (sc.IsNumber(3))
		nDetail = sc.GetInt(3);
	if (sc.IsNumber(4))
		nParticular	= sc.GetInt(4);
	if (sc.IsNumber(5))
		nLevel	= sc.GetInt(5);

	KItemRoom& rcRoom = m_pPlayerItem->m_acRoom[eRoom];
	KItemRoom::KFinder cFind = rcRoom.GetFinder();

	KRoomFindCondition::SameIndexSeries cCond(nGenre, nDetail, nParticular, nLevel);
	sc.PushTable();
	INT i = 0;
	while (cFind.Next(cCond))
	{
		sc.PushTable();
		sc.PushObj(Item[cFind.Index()].m_pLuna);
		sc.SetTableField("pItem");
		sc.PushNumber(cFind.Pos().nX);
		sc.SetTableField("nX");
		sc.PushNumber(cFind.Pos().nY);
		sc.SetTableField("nY");
		sc.SetTableIndex(++i);
	}

	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: FindItemInBags
// @Description		: 在主背包和扩展背包里查找指定物品
// @ReturnCode		: 返回找到的道具列表，nil表示失败（参数不正确），空Table表示没找到
// @ArgumentFlag	: ddd[?]
// @ArgumentComment	: nGenre/szClass
// @ArgumentComment	: nDetail
// @ArgumentComment	: nParticular
// @ArgumentComment	: nLevel
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaFindItemInBags(QLuaScript& sc)
{
	INT nIdx			= 0;
	INT nGenre			= sc.GetInt(1);
	INT nDetail			= sc.GetInt(2);
	INT nParticular		= sc.GetInt(3);
	INT nLevel			= 0;
	if (sc.IsNumber(4))
		nLevel = sc.GetInt(4);

	CONST KE_ITEM_ROOM emBagRoom[] =
	{
		emROOM_MAINBAG,
		emROOM_EXTBAG,
	};

	sc.PushTable();
	for (INT i = 0; i < countof(emBagRoom); ++i)
	{
		// 找空间放这个物品
		KItemRoom& rcRoom = m_pPlayerItem->m_acRoom[emBagRoom[i]];
		KItemRoom::KFinder cFind = rcRoom.GetFinder();

		KRoomFindCondition::SameIndexSeries cCond(nGenre, nDetail, nParticular, nLevel);
		while (cFind.Next(cCond))
		{
			sc.PushObj(Item[cFind.Index()].m_pLuna);
			sc.SetTableIndex(++nIdx);
		}
	}

	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: FindItemId
// @Description		: 在指定空间里查找指定类型的道具
// @ReturnCode		: 返回nRoom,nX,nY,失败返回nil
// @ArgumentFlag	: d
// @ArgumentComment	: nItemId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaFindItemId(QLuaScript& sc)
{
	INT nRet = 0;
	INT nItemId = sc.GetInt(1);
	INT nListIdx = 0;
	KLISTITEM* pListItem = NULL;
	PROCESS_ERROR(nItemId);
	m_pPlayerItem->SearchID(nItemId, &nListIdx);
	pListItem = m_pPlayerItem->GetItem(nListIdx);
	PROCESS_ERROR(pListItem);
	nRet += sc.PushNumber(pListItem->sPos.eRoom);
	nRet += sc.PushNumber(pListItem->sPos.nX);
	nRet += sc.PushNumber(pListItem->sPos.nY);
EXIT0:
	return nRet;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: FindClassItem
// @Description		: 在指定空间里查找指定Class的道具
// @ReturnCode		: 返回找到的道具列表，nil表示失败（参数不正确），空Table表示没找到
// @ArgumentFlag	: ds
// @ArgumentComment	: nRoom
// @ArgumentComment	: szClass
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaFindClassItem(QLuaScript& sc)
{
	KE_ITEM_ROOM eRoom	= (KE_ITEM_ROOM)sc.GetInt(1);
	LPCSTR pszClass = sc.GetStr(2);
	if (eRoom < 0 || eRoom > emROOM_NUM || !*pszClass)
		return	0;

	KItemRoom& rcRoom = m_pPlayerItem->m_acRoom[eRoom];
	KItemRoom::KFinder cFind = rcRoom.GetFinder();

	KRoomFindCondition::SameClass cCond(pszClass);
	sc.PushTable();
	INT i = 0;
	while (cFind.Next(cCond))
	{
		sc.PushTable();
		sc.PushObj(Item[cFind.Index()].m_pLuna);
		sc.SetTableField("pItem");
		sc.PushNumber(cFind.Pos().nX);
		sc.SetTableField("nX");
		sc.PushNumber(cFind.Pos().nY);
		sc.SetTableField("nY");
		sc.SetTableIndex(++i);
	}

	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: CanAddItemIntoBag
// @Description		: 检查当前背包是否能够放得下指定的东西（会将各种复杂的叠加情况也计算在内）
// @ReturnCode		: 1放得下
// @ArgumentFlag	: ?
// @ArgumentComment	: 道具列表，每一项的格式：{ nGenre, nDetail, nParticular, bBind, nCount [bTimeOut]}，项数不限
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaCanAddItemIntoBag(QLuaScript& sc)
{
	KPlayerItem::KHoldItemList lstHoldItem;

	for (INT i = 1, c = sc.GetTopIndex(); i <= c; ++i)
	{
		if (!sc.IsTable(i))
			break;

		KPlayerItem::KHOLD_ITEM sHold;
		INT nTopIndex = sc.GetTopIndex();
		ScriptSafe cSafeScript(sc); // 每次循环时还原脚本TopIndex

		sc.GetTableField(i, "nGenre");
		sc.GetTableField(i, "nDetail");
		sc.GetTableField(i, "nParticular");
		sc.GetTableField(i, "bBind");
		sc.GetTableField(i, "nCount");
		sc.GetTableField(i, "bTimeOut");

		sHold.sIndex.nGenre		 = sc.GetInt(nTopIndex + 1);
		sHold.sIndex.nDetailType = sc.GetInt(nTopIndex + 2);
		sHold.sIndex.nParticular = sc.GetInt(nTopIndex + 3);
		sHold.bBind				 = (BOOL)sc.GetInt(nTopIndex + 4);
		sHold.nCount			 = sc.GetInt(nTopIndex + 5);
		sHold.bTimeOut			 = sc.GetInt(nTopIndex + 6);

		lstHoldItem.push_back(sHold);
	}

	sc.PushNumber(m_pPlayerItem->CanAddItemIntoBag(lstHoldItem));
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetItemAbsTimeout
// @Description		: 获取物品绝对超时时间z
// @ReturnCode		: 返回绝对超时时间表{年，月，日，时，分}或者nil
// @ArgumentFlag	: o
// @ArgumentComment	: 物品对象
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayerItem::LuaGetItemAbsTimeout(QLuaScript& rcScript)
{
	KLuaItem *pLuaItem = dynamic_cast<KLuaItem*>(rcScript.GetObj(1));
	do
	{
		KItem* pItem = pLuaItem->GetIt();
		if (!pItem)
		{
			ASSERT(FALSE);
			rcScript.PushNull();
			break;
		}
		CONST KITEM_TIMEOUT* pTimeout = pItem->GetTimeout();
		if (pTimeout->emType != emKITEM_TIMEOUTTYPE_ABSOLUTE ||
			pTimeout->dwTimeout == 0)
		{
			rcScript.PushNull();
			break;
		}

		time_t timeout_tmp = pTimeout->dwTimeout;
		tm* pTime = localtime(&timeout_tmp);
		if (!pTime)
		{
			ASSERT(FALSE);
			rcScript.PushNull();
			break;
		}
		INT nIndex = 1;
		rcScript.PushTable();
		rcScript.PushNumber(pTime->tm_year + 1900);
		rcScript.SetTableIndex(nIndex++);
		rcScript.PushNumber(pTime->tm_mon + 1);
		rcScript.SetTableIndex(nIndex++);
		rcScript.PushNumber(pTime->tm_mday);
		rcScript.SetTableIndex(nIndex++);
		rcScript.PushNumber(pTime->tm_hour);
		rcScript.SetTableIndex(nIndex++);
		rcScript.PushNumber(pTime->tm_min);
		rcScript.SetTableIndex(nIndex++);
	}
	while (0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetItemRelTimeout
// @Description		: 获取物品相对超时时间
// @ReturnCode		: 返回相对超时时间表{天，时，分}或者nil
// @ArgumentFlag	: o
// @ArgumentComment	: 物品对象
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayerItem::LuaGetItemRelTimeout(QLuaScript& rcScript)
{
	KLuaItem *pLuaItem = dynamic_cast<KLuaItem*>(rcScript.GetObj(1));
	do
	{
		KItem* pItem = pLuaItem->GetIt();
		if (!pItem)
		{
			ASSERT(FALSE);
			rcScript.PushNull();
			break;
		}
		CONST KITEM_TIMEOUT* pTimeout = pItem->GetTimeout();
		if (pTimeout->emType != emKITEM_TIMEOUTTYPE_RELATIVE ||
			pTimeout->dwTimeout == 0)
		{
			rcScript.PushNull();
			break;
		}
		INT nTime = (INT)pTimeout->dwTimeout;
		nTime /= GAME_FPS;
		INT nDayCount = nTime / (24 * 3600);
		nTime %= 24 * 3600;
		INT nHourCount = nTime / 3600;
		nTime %= 3600;
		INT nMinCount = nTime / 60;

		INT nIndex = 1;
		rcScript.PushTable();
		rcScript.PushNumber(nDayCount);
		rcScript.SetTableIndex(nIndex++);
		rcScript.PushNumber(nHourCount);
		rcScript.SetTableIndex(nIndex++);
		rcScript.PushNumber(nMinCount);
		rcScript.SetTableIndex(nIndex++);
	}
	while (0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetJbCoin
// @Description		: 获取金币数
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaGetJbCoin(QLuaScript& sc)
{
	return 0;
//#ifdef GAME_SERVER
//	return  sc.PushNumber(m_pMe->GetJbCoin());
//#else
//	return sc.PushNumber(m_pPlayerItem->GetCoin());
//#endif
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetMaxCarryMoney
// @Description		: 获取玩家携带银两上限
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaGetMaxCarryMoney(QLuaScript& sc)
{
	return sc.PushNumber(m_pPlayerItem->GetMaxCarryMoney());
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetCashCoin
// @Description		: 获取玩家金币替代物品的数量
// @ArgumentFlag	: d
// @ArgumentComment	: nItemCoinIndex 实物货币的索引
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaGetCashCoin(QLuaScript& sc)
{
	//INT nItemCoinIndex = sc.GetInt(1);
	//sc.PushNumber(m_pPlayerItem->GetCashCoin(nItemCoinIndex));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetBindMoney
// @Description		: 获取玩家绑定银两
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaGetBindMoney(QLuaScript& sc)
{
	//return sc.PushNumber(m_pPlayerItem->GetBindMoney());
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: GetItemPos
// @Description		: 根据已知道具对象返回它在自己身上的位置
// @ReturnCode		: { nRoom, nX, nY }，失败返回nil
// @ArgumentFlag	: o
// @ArgumentComment	: pItem:要查找的道具对象
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaGetItemPos(QLuaScript& sc)
{
	KLuaItem* pLuaItem = dynamic_cast<KLuaItem*>(sc.GetObj(1));
	if (!pLuaItem)
		return	0;

	KITEM_POS sPos;
	if (!m_pPlayerItem->GetItemPos(pLuaItem->GetIt()->GetGameIndex(), sPos))
		return	0;

	sc.PushTable();
	sc.PushNumber(sPos.eRoom);
	sc.SetTableField("nRoom");
	sc.PushNumber(sPos.nX);
	sc.SetTableField("nX");
	sc.PushNumber(sPos.nY);
	sc.SetTableField("nY");
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: ThrowAway
// @Description		: 扔掉身上的指定物品
// @ReturnCode		: 1成功
// @ArgumentFlag	: d[dd]
// @ArgumentComment	: nRoom:要扔的物品所在空间
// @ArgumentComment	: nX:要扔的物品所在X
// @ArgumentComment	: nY:要扔的物品所在Y
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaThrowAway(QLuaScript& sc)
{
	KE_ITEM_ROOM eRoom = (KE_ITEM_ROOM)sc.GetInt(1);
	INT nX = sc.GetInt(2);
	INT nY = sc.GetInt(3);
	INT nItemIdx = m_pPlayerItem->GetItemByPos(eRoom, nX, nY);
	if (nItemIdx <= 0)
		return 0;

	sc.PushNumber(m_pPlayerItem->ThrowAwayItem(Item[nItemIdx].GetID()));
	return	1;
}

#ifdef GAME_SERVER

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: DropRateItem
// @Description		: 按照指定的DropRate掉落道具
// @ReturnCode		: 无
// @ArgumentFlag	: s[do]
// @ArgumentComment	: szDropRateFile:掉落率配置文件
// @ArgumentComment	: nCount:掉落数量，默认为1
// @ArgumentComment	: pNpc:掉落位置以哪个npc为中心，默认为当前玩家
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaDropRateItem(QLuaScript& sc)
{
	//LPCSTR	pszDropRateFile = NULL;
	//INT	nCount	= 1;
	//INT	nLucky	= -1;
	//INT	nSeries	= -1;
	//sc.ReadList(1, "sd[dd]", &pszDropRateFile, &nCount, &nLucky, &nSeries);

	//KItemDropRate* pRate	= g_GetItemDropRate(pszDropRateFile);
	//if (!pRate)
	//	return 0;

	//if (nSeries < 0)
	//	nSeries	= KD_RAND_SERIES();

	//KLuaNpc* pLuaNpc	= dynamic_cast<KLuaNpc*>(sc.GetObj(5));
	//KNpc* pNpc	= pLuaNpc ? pLuaNpc->GetHim() : m_pMe->GetNpcObject();

	//if (pNpc)
	//{
	//	pNpc->LoseItem(m_pMe->GetPlayerID(), nCount, nLucky, pRate, pszDropRateFile, nSeries);
	//}

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: OpenRepository
// @Description		: 打开储物箱
// @ReturnCode		: 1成功
// @ArgumentFlag	: o
// @ArgumentComment	: pNpc:打开储物箱的NPC
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaOpenRepository(QLuaScript& sc)
{
	KPlayerItem* pPlayerItem = m_pPlayerItem;
	QCONFIRM_RET_FALSE(pPlayerItem);
	KLuaNpc* pLuaNpc = dynamic_cast<KLuaNpc*>(sc.GetObj(1));
	QCONFIRM_RET_FALSE(pLuaNpc);
	KNpc* pcNpc = pLuaNpc->GetHim();
	QCONFIRM_RET_FALSE(pcNpc);

	//if (pcMeNpc->m_FightMode)
	//	return 0;			// 战斗状态不可以使用储物箱

	m_pMe->SetMoveState(cmsOnStand);

	pPlayerItem->m_BuyInfo.m_nBuyIdx = BuySellInfo::BUY_INDEX_REPOSITORY;
	pPlayerItem->m_BuyInfo.m_dwSceneId = pcNpc->GetSceneId();
	pcNpc->GetPosition(pPlayerItem->m_BuyInfo.m_nPosX, pPlayerItem->m_BuyInfo.m_nPosY, pPlayerItem->m_BuyInfo.m_nPosZ);

	S2C_OPEN_REPOSITORY sSync;
	g_cItemMgr.GetProtocolProcessor()->SendData(m_pMe->GetPlayerIndex(), sSync);
	sc.PushNumber(TRUE);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: SetExtRepState
// @Description		: 设置扩展储物箱激活状态
// @ReturnCode		: --
// @ArgumentFlag	: d
// @ArgumentComment	: nState
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaSetExtRepState(QLuaScript& sc)
{
	INT nState = sc.GetInt(1);
	if (nState == m_pPlayerItem->GetExtRepState())
		return	0;
	if (!m_pPlayerItem->SetExtRepState(nState))
		return	0;

	S2C_SYNC_EXT_REPO_STATE sSync;
	sSync.byState	= m_pPlayerItem->GetExtRepState();
	g_cItemMgr.GetProtocolProcessor()->SendData(m_pMe->GetPlayerIndex(), sSync);
	return	0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: ConsumeItemInBags
// @Description		: 消耗减少一定数量的指定类型的物品（个数为物品叠放数），物品必须在背包中
// @ReturnCode		: 返回请求消费道具数目 - 实际消费数目
// @ArgumentFlag	: d[????]
// @ArgumentComment	: nCount:请求数目
// @ArgumentComment	: nGenre
// @ArgumentComment	: nDetail
// @ArgumentComment	: nParticular
// @ArgumentComment	: nLevel
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaConsumeItemInBags(QLuaScript& rcScript)
{
	INT nCount		= rcScript.GetInt(1);
	INT nGenre		= ITEM_INVALID_TYPE;
	INT nDetail		= ITEM_INVALID_TYPE;
	INT nParticular	= ITEM_INVALID_TYPE;
	INT nLevel		= KD_ITEM_NO_LEVEL_LIMIT;
	
	if (rcScript.IsNumber(2))
		nGenre = rcScript.GetInt(2);
	if (rcScript.IsNumber(3))
		nDetail = rcScript.GetInt(3);
	if (rcScript.IsNumber(4))
		nParticular	= rcScript.GetInt(4);
	if(rcScript.IsNumber(5))
		nLevel = rcScript.GetInt(5);


	rcScript.PushNumber(m_pPlayerItem->ConsumeItemInBags(nCount, nGenre, nDetail, nParticular, nLevel));
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: SetItemTimeout
// @Description		: 设置物品超时时间
// @ReturnCode		: 成功返回1，失败返回0
// @ArgumentFlag	: ??[d]
// @ArgumentComment	: 物品对象或者物品索引
// @ArgumentComment	: 绝对时间（字符串）形如'YYYY-MM-DD HH:MM:SS'，或者分钟数（整数）
// @ArgumentComment	: BOOL:1表示相对时间，0表示绝对时间，第二个参数为整数时有效，缺省为1
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayerItem::LuaSetItemTimeout(QLuaScript& rcScript)
{
	INT nRet = 0;
	do
	{
		INT nParamCount = rcScript.GetTopIndex();
		KItem* pItem = NULL;
		if (rcScript.IsNumber(1))
		{
			INT nItemIdx = rcScript.GetInt(1);
			if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
			{
				ASSERT(FALSE);
				break;
			}
			pItem = &Item[nItemIdx];
		}
		else if (rcScript.IsUser(1))
		{
			KLuaItem *pLuaItem = dynamic_cast<KLuaItem*>(rcScript.GetObj(1));
			if (!pLuaItem)
			{
				ASSERT(FALSE);
				break;
			}
			pItem = pLuaItem->GetIt();
		}
		else
		{
			ASSERT(FALSE);
			break;
		}
		if (!pItem || pItem->GetBelong() != m_pMe->GetPlayerIndex())
		{
			ASSERT(FALSE);
			break;
		}
		INT nItemIdx = pItem->GetGameIndex();
		if (rcScript.IsNumber(2))
		{
			INT nTimeout = rcScript.GetInt(2);
			if (nTimeout <= 0)
			{
				ASSERT(FALSE);
				break;
			}
			BOOL bRelative = TRUE;	// 缺省为相对时间
			if (nParamCount >= 3)
				bRelative = rcScript.GetInt(3) > 0;
			KITEM_TIMEOUT sTimeout;
			if (bRelative)
			{
				sTimeout.emType = emKITEM_TIMEOUTTYPE_RELATIVE;
				sTimeout.dwTimeout = (DWORD)nTimeout * 60 * GAME_FPS;	// 把分钟转成帧数
			}
			else
			{
				time_t tNow = KSysService::Time(NULL);
				sTimeout.emType = emKITEM_TIMEOUTTYPE_ABSOLUTE;
				sTimeout.dwTimeout = tNow + nTimeout * 60;				// 把相对时间转成绝对时间
			}
			m_pPlayerItem->SetTimeout(nItemIdx, &sTimeout);
			m_pPlayerItem->SyncItem(nItemIdx);
			nRet = 1;
		}
		else if (rcScript.IsString(2))
		{
			LPCSTR pszTimeout = rcScript.GetStr(2);
			if (!pszTimeout)
			{
				ASSERT(FALSE);
				break;
			}
#pragma pack(push, 1)
			struct KTimeString
			{
				CHAR szYear[4];
				CHAR chSep1;
				CHAR szMon[2];
				CHAR chSep2;
				CHAR szDay[2];
				CHAR chSep3;
				CHAR szHour[2];
				CHAR chSep4;
				CHAR szMin[2];
				CHAR chSep5;
				CHAR szSec[2];
			};
#pragma pack(pop)
			if (strlen(pszTimeout) != sizeof(KTimeString))
			{
				ASSERT(FALSE);
				break;
			}
			KTimeString sTime;
			memcpy(&sTime, pszTimeout, sizeof(sTime));
			if (sTime.chSep1 != '/' ||
				sTime.chSep2 != '/' ||
				sTime.chSep3 != '/' ||
				sTime.chSep4 != '/' ||
				sTime.chSep5 != '/')
			{
				ASSERT(FALSE);
				break;
			}
			sTime.chSep1 = '\0';
			sTime.chSep2 = '\0';
			sTime.chSep3 = '\0';
			sTime.chSep4 = '\0';
			sTime.chSep5 = '\0';
			tm t;
			memset(&t, 0, sizeof(t));
			t.tm_year = atoi(sTime.szYear) - 1900;
			t.tm_mon  = atoi(sTime.szMon) - 1;
			t.tm_mday = atoi(sTime.szDay);
			t.tm_hour = atoi(sTime.szHour);
			t.tm_min  = atoi(sTime.szMin);
			t.tm_sec  = atoi(sTime.szSec);
			KITEM_TIMEOUT sTimeout;
			sTimeout.emType = emKITEM_TIMEOUTTYPE_ABSOLUTE;
			sTimeout.dwTimeout = mktime(&t);
			m_pPlayerItem->SetTimeout(nItemIdx, &sTimeout);
			m_pPlayerItem->SyncItem(nItemIdx);
			nRet = 1;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	while (0);
	rcScript.PushNumber(nRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: DelItemTimeout
// @Description		: 取消物品超时时间
// @ReturnCode		: 成功返回1，失败返回0
// @ArgumentFlag	: ?
// @ArgumentComment	: 物品对象或者物品索引
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayerItem::LuaDelItemTimeout(QLuaScript& rcScript)
{
	BOOL bResult	= FALSE;
	INT nItemIdx	= 0;

	if (rcScript.IsNumber(1))
	{
		nItemIdx	= rcScript.GetInt(1);
	}
	else if (rcScript.IsUser(1))
	{
		KLuaItem* pLuaItem = dynamic_cast<KLuaItem*>(rcScript.GetObj(1));
		LOG_PROCESS_ERROR(pLuaItem);

		KItem* pcItem	= pLuaItem->GetIt();
		LOG_PROCESS_ERROR(pcItem);
		
		nItemIdx	= pcItem->GetGameIndex();
	}
	else
	{
		LOG_PROCESS_ERROR(FALSE);
	}

	LOG_PROCESS_ERROR(nItemIdx > 0);
	LOG_PROCESS_ERROR(nItemIdx < MAX_ITEM);

	LOG_PROCESS_ERROR(Item[nItemIdx].GetBelong() == m_pMe->GetPlayerIndex());

	LOG_PROCESS_ERROR(m_pPlayerItem->DelTimeout(nItemIdx));
	m_pPlayerItem->SyncItem(nItemIdx);

	bResult	= TRUE;

EXIT0:
	rcScript.PushNumber(bResult);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: CostMoney
// @Description		: 扣除剑侠币
// @ReturnCode		: 扣除成功与否 
// @ArgumentFlag	: dd
// @ArgumentComment	: 扣除的数目
// @ArgumentComment	: 扣除途径
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaCostMoney(QLuaScript& sc)
{
	INT nCostCount = sc.GetInt(1);
	INT nCostWay = sc.GetInt(2);
	INT nCostRet = m_pPlayerItem->Pay(nCostCount, nCostWay);
	sc.PushNumber(nCostRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: CostBindMoney
// @Description		: 扣除绑定银两
// @ReturnCode		: 扣除成功与否
// @ArgumentFlag	: d[d]
// @ArgumentComment	: nCostCount 扣除的数目
// @ArgumentComment	: nCostWay 扣除的途径
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaCostBindMoney(QLuaScript& sc)
{
	INT nCostCount = sc.GetInt(1);
	INT nCostWay = 100;		// 从脚本调上来的，从100开始
	if (sc.GetTopIndex() >= 2)
	{
		nCostWay = sc.GetInt(2);
	}
	INT nCostRet = m_pPlayerItem->CostBindMoney(nCostCount, nCostWay);
	sc.PushNumber(nCostRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: IsMyItem
// @Description		: 制定物品的是否属于自己的
// @ReturnCode		: 0：不是，1：是的
// @ArgumentFlag	: d
// @ArgumentComment	: nItemIndex：物品索引
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaIsMyItem(QLuaScript& sc)
{
	INT nItemIndex = sc.GetInt(1);

	if (nItemIndex <= 0 || nItemIndex >= MAX_ITEM)
	{
		sc.PushNumber(0);
		return 1;
	}

	INT bIsMyItem = 0;
	bIsMyItem = m_pPlayerItem->IsMyItem(nItemIndex);
	sc.PushNumber(bIsMyItem);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: CalcFreeItemCellCount
// @Description		: 获取当前玩家背包中空着的格子数
// @ReturnCode		: 背包中空着的格子数
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaCalcFreeItemCellCount(QLuaScript& sc)
{
	sc.PushNumber(m_pPlayerItem->m_acRoom[emROOM_MAINBAG].CountFree());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: ReApplyEquipRes
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaReApplyEquipRes(QLuaScript& sc)
{
	m_pPlayerItem->ReApplyEquipRes();

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: ThrowAllItem
// @Description		: 抛弃背包里的所有物品
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaThrowAllItem(QLuaScript& sc)
{
	KItemRoom& room = m_pPlayerItem->m_acRoom[emROOM_MAINBAG];

	KItemRoom::KFinder cFind = room.GetFinder();

	//KLogWritter* pLog = KLogWritter::GetInstance();
	// 遍历本空间所有物品
	while (cFind.Next(KRoomFindCondition::Used))
	{
		INT nItemIdx = cFind.Index();
		if (m_pPlayerItem->Remove(nItemIdx, emKLOSEITEM_SCRIPT))
		{
			/*
			// 日志
			pLog->ItemHistory_RemoveItem(
				&Item[nItemIdx],
				m_pMe,
				emKLOG_REASON_REMOVEITEM_SYSTEM);
			*/
			g_cItemMgr.Remove(nItemIdx, emITEM_DATARECORD_REMOVE, emITEM_CONSUMEMODE_ERRORLOST_THROWALLITEM, m_pMe);
		}
	}
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: FindAllItem
// @Description		: 查找指定空间所有物品指定空间所有物品
// @ReturnCode		: {}
// @ArgumentFlag	: d
// @ArgumentComment	: nRoom 指定空间
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaFindAllItem(QLuaScript& sc)
{
	INT nRoom = sc.GetInt(1);
	KItemRoom& room = m_pPlayerItem->m_acRoom[nRoom];
	KItemRoom::KFinder cFind = room.GetFinder();
	INT nIndex		= 1;
	
	// 遍历本空间所有物品
	sc.PushTable();
	while (cFind.Next(KRoomFindCondition::Used))
	{
		INT nItemIdx = cFind.Index();
		sc.PushNumber(nItemIdx);
		sc.SetTableIndex(nIndex++);	
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: ItemLog
// @Description		: 物品流向日志（发往Goddess）
// @ReturnCode		: 
// @ArgumentFlag	: odd[sd]
// @ArgumentComment	: pItem: 物品
// @ArgumentComment	: BOOL：是否获得物品
// @ArgumentComment	: nType：行为类型，见KE_PLAYERLOG_TYPE@klogtype.h
// @ArgumentComment	: szComment：注释
// @ArgumentComment	: bForce：是否强制写日志,缺省为FALSE
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayerItem::LuaItemLog(QLuaScript& sc)
{
	//KLuaItem* pLuaItem = dynamic_cast<KLuaItem*>(sc.GetObj(1));
	//if (!pLuaItem)
	//{
	//	ASSERT(FALSE);
	//	return 0;
	//}
	//KItem* pItem = pLuaItem->GetIt();
	//if (!pItem)
	//{
	//	ASSERT(FALSE);
	//	return 0;
	//}
	//BOOL bGain = sc.GetInt(2) != 0;
	//INT nType = sc.GetInt(3);
	//LPCSTR pszComment = NULL;
	//BOOL bForce = FALSE;
	//if (sc.GetTopIndex() >= 4)
	//	pszComment = sc.GetStr(4);
	//if (sc.GetTopIndex() >= 5)
	//	bForce = sc.GetInt(5) != 0;
	//if (nType < 0 || nType >= emKITEMLOG_TYPE_COUNT)
	//{
	//	ASSERT(FALSE);
	//	return 0;
	//}
	//if (!bForce)
	//{
	//	KLogWritter::GetInstance()->Goddess_ItemLog(
	//		m_pMe,
	//		pItem,
	//		bGain,
	//		static_cast<KE_ITEMLOG_TYPE>(nType),
	//		pszComment);
	//}
	//else
	//{
	//	KLogWritter::GetInstance()->Goddess_ForceItemLog(
	//		m_pMe->GetAccount(),
	//		m_pMe->GetPlayerName(),
	//		pItem,
	//		bGain,
	//		static_cast<KE_ITEMLOG_TYPE>(nType),
	//		pszComment);
	//}
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: AddBindCoin
// @Description		: 对绑定金币的操作
// @ReturnCode		: 1：成功，0：失败
// @ArgumentFlag	: d[d]
// @ArgumentComment	: nBindCoin
// @ArgumentComment	: nWay
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaAddBindCoin(QLuaScript& sc)
{
	INT bRet	= 0;
	INT nValue	= sc.GetInt(1);
	INT nWay	= 100;
	if (sc.GetTopIndex() >= 2)
	{
		nWay = sc.GetInt(2);
	}
	bRet	=	m_pPlayerItem->AddBindCoin(nValue, nWay);
	return sc.PushNumber(bRet);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: AutoEquip
// @Description		: 自动装备道具
// @ReturnCode		: 成功返回1
// @ArgumentFlag	: ?
// @ArgumentComment	: nItemIndex/pItem
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaAutoEquip(QLuaScript& sc)
{
	INT nItemIdx = 0;

	if (sc.IsNumber(1))
	{
		nItemIdx = sc.GetInt(1);
	}
	else
	{
		KLuaItem *pLuaItem = dynamic_cast<KLuaItem *>(sc.GetObj(1));
		if (pLuaItem)
			nItemIdx = pLuaItem->GetIt()->GetGameIndex();
	}

	if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		return	0;

	sc.PushNumber(m_pPlayerItem->AutoEquip(nItemIdx));
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerItem
// @LuaApiName		: AutoUse
// @Description		: 自动使用
// @ReturnCode		: 成功返回1
// @ArgumentFlag	: ?
// @ArgumentComment	: nItemIndex/pItem
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerItem::LuaAutoUse(QLuaScript& sc)
{
	INT nItemIdx = 0;

	if (sc.IsNumber(1))
	{
		nItemIdx = sc.GetInt(1);
	}
	else
	{
		KLuaItem *pLuaItem = dynamic_cast<KLuaItem *>(sc.GetObj(1));
		if (pLuaItem)
			nItemIdx = pLuaItem->GetIt()->GetGameIndex();
	}

	if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		return	0;

	sc.PushNumber(m_pPlayerItem->UseItem(nItemIdx));
	return	1;
}

#endif

#include "kluaplayeritem_table.hpp"

// -------------------------------------------------------------------------


