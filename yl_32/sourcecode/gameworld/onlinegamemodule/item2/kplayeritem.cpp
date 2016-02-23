
#include "stdafx.h"
#include "kplayeritem.h"

#include "kitemmgr.h"
#include "itemenv.h"
#include "kmagicattrib.h"
#include "kluaitem.h"
#include <algorithm>

#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamebase/ksysservice.h"
#include "kmagicattribmanager.h"
#include "kluaplayeritem.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "../trade/qsellmgr.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "Runtime/TypeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// 如果玩家使用一个道具失败，在接下来的4帧里都不接收再使用该道具的协议并做计数
INT g_nUseFailedFrameInterval		= 4;
// 连续失败计数超过5，则将该玩家T下线
INT g_nUseFailedLimitTime			= 5;


#define KD_ADDMONEY_PERSEC_TRACE	500	// 平均每秒增加多少金钱时继续追踪金钱增加情况，否则重新开始
#define KD_ADDMONEY_FILTER_SEC	10	// 加金钱间隔超过多少秒时判断KD_ADDMONEY_PERSEC_TRACE


// 金钱报警级数
enum KD_ADDMONEY_ALM_LEVEL
{
	emKADDMONEY_ALM_1 = 500000,
	emKADDMONEY_ALM_2 = 1000000,
	emKADDMONEY_ALM_MAX = 2000000,
};

// 增加金钱次数级数
enum KE_ADDMONEY_FREQ_LEVEL
{
	emKADDMONEY_FREQ_1 = 10,
	emKADDMONEY_FREQ_2 = 50,
	emKADDMONEY_FREQ_3 = 100,
};
//----------------------------------------------------------------------
extern QLogFile	g_MoneyLogFile;
extern QLogFile	g_ItemLogFile;
extern QLogFile	g_NewLogFile;
extern QLogFile	g_SysAlarmLogFile;

#define KD_ITEM_SWITCH_TIMELIMIT			3		// 装备切换间隔限制为1分钟

CONST INT g_anEquipPos[equip_detailnum + 1] =		// 根据装备类型获得装备的位置---玩家装备
{
	-1,												// nothing
	emEQUIPPOS_SWORD,
	emEQUIPPOS_SPEAR,
	emEQUIPPOS_NECKLACE,			// 项链
	emEQUIPPOS_RING,				// 戒指
	emEQUIPPOS_PENDANT,				// 腰坠
	emEQUIPPOS_HEAD,
	emEQUIPPOS_ARMOR,				// 衣服
	emEQUIPPOS_BELT,				// 腰带
	emEQUIPPOS_CLOAK,				// 披风
	emEQUIPPOS_FOOT,				// 鞋子

	emEQUIPPOS_SUIT,				// 套装
	emEQUIPPOS_BOTTOM,				// 脚下
	emEQUIPPOS_BACK,				// 背
};

CONST KE_ITEM_ROOM g_aeExtBagPos2Room[] =
{
	emROOM_EXTBAG,									// emEXTBAGPOS_BAG1
};

CONST KE_ITEM_ROOM g_aeBagRoom[] =
{
	emROOM_MAINBAG,
	emROOM_EXTBAG,
};

KPlayerItem::CHECKCALLBACK KPlayerItem::m_apfnLimitUseCheck[emKLIMITUSE_COUNT] =
{
	NULL,
	&KPlayerItem::CheckMapForbid,
	&KPlayerItem::CheckMapProperUse,
};
//----------------------------------------------------------------------

KPlayerItem::KPlayerItem(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer)), m_cSwitch(*this)
{
	m_pLuna = NULL;
	Release();
}

KPlayerItem::~KPlayerItem()
{
	ClearSuiteMass();
	Release();
}

BOOL KPlayerItem::Init()
{
	m_pLuna = new KLuaPlayerItem(&m_rcPlayer);
	m_BuyInfo.Clear();
	m_cTimeoutCtrl.Init(this);
	m_cCDTime.Init();
	m_nMaxCarryMoney = 2000000000;	//临时用
	return TRUE;
}

BOOL KPlayerItem::OnClientReady()
{
	//#ifdef GAME_SERVER
	//	INT nPlayerId = m_rcPlayer.GetPlayerID();
	//	KGC_Player* pPlayer = g_PlayerManager.GetPlayer(nPlayerId);
	//	QCONFIRM_RET_FALSE(pPlayer);
	//	m_uAddedMoney = (UINT)pPlayer->GetAddedMoney();
	//	m_uAddedMoneyTime = (UINT)pPlayer->GetAddedMoneyTime();
	//	m_uAddedTimes = (UINT)pPlayer->GetAddedTimes();
	//
	//	INT nTaskValue	= m_rcPlayer.m_cPlayerTask.GetTaskValue(TASKVALUE_CHECKMONEY);
	//
	//	CheckMoney("Loading");				// 检查载入过程中是否保持未变
	//
	//	m_dwCheckMoney	= (DWORD)(nTaskValue - 1);	// 恢复到上次任务变量保存时
	//	if (m_dwCheckMoney != (DWORD)-1)
	//		CheckMoney("OnLoaded");			// 检查当前和上次任务变量保存的是否一致
	//	else
	//		SetCheckMoney(GetTotalMoney());
	//#endif
	return TRUE;
}

BOOL KPlayerItem::Release()
{
	ZeroMemory(m_szGiftTable, sizeof(m_szGiftTable));
	ZeroMemory(m_aMoney, sizeof(m_aMoney));
	ZeroMemory(&m_sActiveSpState, sizeof(m_sActiveSpState));

	m_nExtRepState	= 0;
	m_nListCurIdx	= 0;

	m_FreeIdx.Init(KD_MAX_PLAYER_ITEM);
	m_UseIdx.Init(KD_MAX_PLAYER_ITEM);

	for (INT i = KD_MAX_PLAYER_ITEM - 1; i > 0 ; i--)
		m_FreeIdx.Insert(i);

	for (INT i = 0; i < emROOM_NUM; ++i)		// 以防万一，先清掉所有空间
		m_acRoom[i].Release();

	m_acRoom[emROOM_EQUIP].Init(KD_ROOM_EQUIP_WIDTH, KD_ROOM_EQUIP_HEIGHT);
	m_acRoom[emROOM_EQUIPEX].Init(KD_ROOM_EQUIPEX_WIDTH, KD_ROOM_EQUIPEX_HEIGHT);
	m_acRoom[emROOM_MAINBAG].Init(KD_ROOM_MAINBAG_WIDTH, KD_ROOM_MAINBAG_HEIGHT);
	m_acRoom[emROOM_EXTBAGBAR].Init(KD_ROOM_EXTBAGBAR_WIDTH, KD_ROOM_EXTBAGBAR_HEIGHT);
	m_acRoom[emROOM_EXTBAG].Init(KD_ROOM_EXTBAG1_MAX_WIDTH, KD_ROOM_EXTBAG1_MAX_HEIGHT, 0, 0);
	m_acRoom[emROOM_KIN].Init(KD_ROOM_KIN_WIDTH, KD_ROOM_KIN_HEIGHT);

#ifdef GAME_SERVER
	m_nLastSwithTime	= 0;
	m_bAbrade			= FALSE;
	m_nViewEquipTime	= 0;
	m_nPrePayMoney		= 0;
	m_uAddedMoney		= 0;
	m_uAddedMoneyTime	= 0;
	m_uAddedTimes		= 0;
	m_dwCheckMoney		= 0;
	m_nNextAlarmLevel	= emKADDMONEY_ALM_1;
	m_nForbidUse		= 0;

	m_bStoreAcutionSearchRet = FALSE;

#else

#endif
	m_cTimeoutCtrl.UnInit();

	SAFE_DELETE(m_pLuna);
	return TRUE;
}
/*首次登陆时刷新神秘商店里的商品 并记录时间*/
BOOL KPlayerItem::OnLogin()
{
	// added by liyun 玩家登录时要检查装备，如果家族ID为空则要删除所有借来的装备
	if (m_rcPlayer.m_dwKinId == 0)
	{
		RemoveAllRentItem();
	}

	// 	INT nPlayerIdx = m_rcPlayer.GetPlayerIndex();
	// 	time_t lastUpdateTime = KSysService::GameTime(NULL);
	// 	m_rcPlayer.RawSetLastUpdateMysteryShopTime(lastUpdateTime);
	// 	if (g_cSellMgr.GenrateGoods(nPlayerIdx));
	// 	{
	// 		return TRUE;
	// 	}
	// 	return FALSE;
	return FALSE;
}

BOOL KPlayerItem::UnInit()
{
	RemoveAll();
	Release();
	return TRUE;
}

#ifdef GAME_SERVER

BOOL KPlayerItem::SyncItem(INT nItemIdx)
{
	INT nListIdx = FindSame(nItemIdx);
	if (nListIdx <= 0)
		return	FALSE;

	KLISTITEM& rsListItem = m_asList[nListIdx];
	SyncItem(rsListItem);
	return	TRUE;
}

VOID KPlayerItem::SyncItem(CONST KLISTITEM& rsListItem) CONST
{
	KITEM_SYNC sItem;
	sItem.byProtocol = (BYTE)s2c_syncitem;

	// 生成同步物品基本数据
	KItem* pcItem = &Item[rsListItem.nIdx];
	INT nViewInfoUseSize = pcItem->MakeSyncInfo(sItem);
	// 物品同步数据
	sItem.byRoom	= (BYTE)rsListItem.sPos.eRoom;
	sItem.byX		= (BYTE)rsListItem.sPos.nX;
	sItem.byY		= (BYTE)rsListItem.sPos.nY;

	sItem.wDataLen = sizeof(sItem) - sizeof(KVIEWITEMINFO) + nViewInfoUseSize;
	g_cItemMgr.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), sItem, sItem.wDataLen);
}

VOID KPlayerItem::SyncLockState(INT nItem)
{
	//if (nItem <= 0 || nItem >= MAX_ITEM)
	//{
	//	_ASSERT(FALSE);
	//	return;
	//}

	//ITEM_LOCK_SYNC data;
	//data.cProtocol = s2c_synclock;
	//data.dwID = Item[nItem].GetID();

	//if (Item[nItem].IsLock())
	//	data.dwUnlockTime = Item[nItem].GetUnlockTime();
	//else
	//	data.dwUnlockTime = 0UL;

	//g_pCoreServer->SendData(m_rcPlayer.m_nNetConnectIdx, &data, sizeof(data));
}

BOOL KPlayerItem::CheckMoney(LPCTSTR pszReason)
{
	//DWORD dwNowMoney	= (DWORD)GetTotalMoney();
	//
	//if (m_dwCheckMoney == dwNowMoney)
	//	return TRUE;

	//m_rcPlayer.CallMainScript("Player", "OnMoneyErr", 0, "sdd", pszReason, m_dwCheckMoney, dwNowMoney);

	//SetCheckMoney(dwNowMoney);

	return FALSE;
}

BOOL KPlayerItem::SetCheckMoney(DWORD dwNowMoney)
{
	//m_dwCheckMoney	= dwNowMoney;

	//m_rcPlayer.m_cPlayerTask.SetTaskValue(TASKVALUE_CHECKMONEY, dwNowMoney + 1);

	return FALSE;
}

// 这个只是给脚本调上来的接口使用的，程序中不使用，所以这里只需要判断100以上的就可以了
INT KPlayerItem::GetDataRecordWay(KE_ADD_ITEM_WAY eWay1 /* = emKADDITEM_NONE */, KE_LOSE_ITEM_WAY eWay2 /* = emKLOSEITEM_NONE */)
{
	KE_ITEM_DATARECORD_WAY eWay = emITEM_DATARECORD_NORMAL;
	if (eWay1 != emKADDITEM_NONE)
	{
		if (eWay1 == emKADDITEM_BYSCRIPT_COMPOSE || eWay1 == emKADDITEM_BYSCRIPT_CYSTAL_COMPOSE ||
			eWay1 == emKADDITEM_BYSCRIPT_PEEL_EQUIP || eWay1 == emKADDITEM_BYSCRIPT_PEEL_PARTNER)
			eWay = emITEM_DATARECORD_ROLLOUT;
		else
			eWay = emITEM_DATARECORD_SYSTEMADD;
	}
	else if (eWay2 != emKLOSEITEM_NONE)
	{
		if (eWay2 == emKLOSEITEM_SCRIPT_COMPOSE || eWay2 == emKLOSEITEM_SCRIPT_ENHANCE ||
			eWay2 == emKLOSEITEM_SCRIPT_STRENGTHEN || eWay2 == emKLOSEITEM_SCRIPT_PARTNER_TALENT ||
			eWay2 == emKLOSEITEM_SCRIPT_BREAKUP	|| eWay2 == emKLOSEITEM_SCRIPT_CYSTAL_COMPOSE ||
			eWay2 == emKLOSEITEM_SCRIPT_MANTLE_SHOP)
			eWay = emITEM_DATARECORD_ROLLIN;
		else
			eWay = emITEM_DATARECORD_REMOVE;
	}

	return eWay;
}
#else	// #ifdef GAME_SERVER

#endif	// #ifndef GAME_SERVER

INT KPlayerItem::Add(INT			nIdx,
					 KE_ITEM_ROOM	eRoom,
					 KE_ADD_ITEM_WAY eWay,
					 INT			nX,			// = 0
					 INT			nY,			// = 0
					 INT			nPrice,		// = 0
					 INT			nCount,		// = 0
					 BOOL			bIsSell,	// = 0
					 LPCSTR			pszCurrencyType)//  = NULL
{
	KITEM_POS sPos;
	sPos.eRoom = eRoom;
	sPos.nX    = nX;
	sPos.nY	   = nY;
	return Add(nIdx, sPos, eWay, nPrice, nCount, bIsSell, pszCurrencyType);
}

INT	KPlayerItem::Add(INT nIdx, CONST KITEM_POS& rsPos, KE_ADD_ITEM_WAY eWay, INT nPrice /* = 0 */, INT nCount /* = 0 */, BOOL bIsSell /* = 0 */, LPCSTR pszCurrencyType /*= NULL*/)
{
	if (nIdx <= 0 || rsPos.eRoom < 0 || rsPos.eRoom >= emROOM_NUM || !Item[nIdx].GetID())
		return	0;

	KItem* pItem = &Item[nIdx];

	//// 加载，拆分，摆摊返还时获得的道具不需要重置解锁时间
	//if (pItem->GetLockIntervale() && eWay != emKADDITEM_NONE && eWay != emKADDITEM_LOAD
	//	&& eWay != emKADDITEM_SPLIT && eWay != emKADDITEM_STALL_RETURN)
	//{
	//	pItem->SetLockState(TRUE,  g_ServerTime.GetCurLocalTime() + pItem->GetLockIntervale());
	//}

#ifdef GAME_SERVER
	INT nBelongPlayerIdx = pItem->GetBelong();
	if (nBelongPlayerIdx > 0)	// 属于某个玩家
	{
		_ASSERT(FALSE);
		//g_SysAlarmLogFile.LogRecordVar(
		//	LOG_LEVEL_ERROR,
		//	L_ItemList_0,
		//	m_rcPlayer.GetAccount(),
		//	m_rcPlayer.GetPlayerName(),
		//	Player[nBelongPlayerIdx].GetAccount(),
		//	Player[nBelongPlayerIdx].GetPlayerName(),
		//	pItem->GetName(),
		//	pItem->GetGenre(),
		//	pItem->GetDetailType(),
		//	pItem->GetParticular()
		//);
		return 0;
	}


	//pszCurrencyType = pszCurrencyType ? pszCurrencyType : "";
	//if ((eWay != emKADDITEM_NONE && eWay != emKADDITEM_LOAD && pItem->GetValue() >= 50000) || 
	//	(emKADDITEM_IBCOIN == eWay || emKADDITEM_IBSHOP == eWay))
	//{
	//	// 给玩家行为日志上也加上价值量，这样就能知道个数了
	//	KD_COURSE_LOG(m_rcPlayer, emKCOURSE_PLAYER_ADDITEM, "AddItem\t" << pItem->GetName() << 
	//		"[" << pItem->GetGuidString() << "]" <<  "(" << pItem->GetGenTime() << ")\tWay" 
	//		<< (INT)eWay << "\tValue" << pItem->GetValue() << pszCurrencyType);

	//	// 获得物品log
	//	if (eWay != emKADDITEM_SPLIT) // 如果是拆分道具，玩家实际上没有获得新道具，不需要记物品流向LOG
	//	{
	//		KLogWritter* pLog = KLogWritter::GetInstance();
	//		CHAR szValue[256];
	//		sprintf(szValue, "%d\t%s",eWay, pszCurrencyType);
	//		pLog->Goddess_ItemLog(
	//			m_rcPlayer.GetAccount(),
	//			m_rcPlayer.GetPlayerName(),
	//			pItem,
	//			TRUE,
	//			emKITEMLOG_TYPE_ADDITEM,
	//			szValue);
	//	}
	//}

#endif

	INT nListIdx = FindFree();
	if (!nListIdx)
		return	0;

	switch (rsPos.eRoom)						// TODO: xyf 这边的写法不太好，以后统一处理
	{
	case emROOM_EQUIP:
		if (!AddEquip(nIdx, nListIdx, rsPos.nX))
			return	0;
		break;
	case emROOM_EXTBAGBAR:
		if (!AddExtBag(nIdx, nListIdx, rsPos.nX))
			return	0;
		break;
	case emROOM_EQUIPEX:
		if (!AddEquipEx(nIdx, nListIdx, rsPos.nX))
			return 0;
		break;
	default:
		if (!AddBase(nIdx, nListIdx, rsPos))
			return	0;
		break;
	}

	// 限时控制
	CONST KITEM_TIMEOUT* pTimeout = pItem->GetTimeout();
	if (pTimeout->dwTimeout > 0)
		m_cTimeoutCtrl.AddTimeout(nIdx);

	m_FreeIdx.Remove(nListIdx);
	m_UseIdx.Insert(nListIdx);

	Item[nIdx].SetBelong(m_rcPlayer.GetPlayerIndex());

#ifdef GAME_SERVER
	SyncItem(m_asList[nListIdx]);
#else
	::g_CoreEventNotify(emCOREEVENT_SYNC_ITEM, "ddd", rsPos.eRoom, rsPos.nX, rsPos.nY); 
	if (IsBagRoom(rsPos.eRoom))
		Player[CLIENT_PLAYER_INDEX].Refect(PLAYER_ADD_ITEM, nListIdx);
#endif

	return	nListIdx;
}

BOOL KPlayerItem::RemoveOnly(INT nItemIdx, KE_LOSE_ITEM_WAY eWay)
{
	if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		return FALSE;

	INT nListIdx = FindSame(nItemIdx);
	if (!nListIdx)
		return FALSE;

	CONST KITEM_POS& rsPos = m_asList[nListIdx].sPos;
	KItem& rcItem = Item[nItemIdx];

#ifdef GAME_SERVER

	INT nPlayerIdx = rcItem.GetBelong();
	//if (nPlayerIdx != m_rcPlayer.GetPlayerIndex())
	//{
	//	_ASSERT(FALSE);
	//	if (nPlayerIdx < 0 || nPlayerIdx >= MAX_PLAYER)
	//		nPlayerIdx = 0;
	//	g_SysAlarmLogFile.LogRecordVar(
	//		LOG_LEVEL_ERROR,
	//		L_ItemList_2,
	//		m_rcPlayer.GetAccount(),
	//		m_rcPlayer.GetPlayerName(),
	//		rcItem.GetName(),
	//		rcItem.GetGenre(),
	//		rcItem.GetDetailType(),
	//		rcItem.GetParticular(),
	//		Player[nPlayerIdx].GetAccount(),
	//		Player[nPlayerIdx].GetPlayerName()
	//	);
	//	return	FALSE;
	//}
	//m_rcPlayer.CumulateValue(&rcItem);
	//if (eWay != emKLOSEITEM_NONE && rcItem.GetValue() >= 50000) 		
	//{
	//	// 给玩家行为日志上也加上价值量，这样就能知道个数了
	//	KD_COURSE_LOG(m_rcPlayer, emKCOURSE_PLAYER_REMOVEITEM, "RemoveItem\t" << rcItem.GetName() << 
	//		"[" <<  rcItem.GetGuidString() << "]" << "(" << rcItem.GetGenTime() << ")\tWay" 
	//		<< (INT)eWay << "\tValue" << rcItem.GetValue());

	//	//失去物品log
	//	if (eWay != emKLOSEITEM_STACK)   // 如果是叠加操作的话，玩家实际上是没有失去物品的，不需要记物品流向LOG
	//	{
	//		CHAR szValue[256];
	//		sprintf(szValue, "%d",eWay);
	//		KLogWritter* pLog = KLogWritter::GetInstance();
	//		pLog->Goddess_ItemLog(
	//			m_rcPlayer.GetAccount(),
	//			m_rcPlayer.GetPlayerName(),
	//			&rcItem,
	//			FALSE,
	//			emKITEMLOG_TYPE_REMOVEITEM,
	//			szValue);
	//	}
	//}
#endif

	switch (rsPos.eRoom)						// TODO: xyf 这边的写法不太好，以后统一处理
	{
	case emROOM_EQUIP:
		RemoveEquip(nListIdx);
		break;
	case emROOM_EXTBAGBAR:
		RemoveExtBag(nListIdx);
		break;
	default:
		RemoveBase(nListIdx);
		break;
	}

	// 超时控制
	CONST KITEM_TIMEOUT* pTimeout = rcItem.GetTimeout();
	if (pTimeout->dwTimeout > 0)
		m_cTimeoutCtrl.DelTimeout(nItemIdx);

	rcItem.SetBelong(0);	// 清除所有者

	m_FreeIdx.Insert(nListIdx);
	m_UseIdx.Remove(nListIdx);
	return TRUE;
}

BOOL KPlayerItem::Remove(INT nItemIdx, KE_LOSE_ITEM_WAY eWay)
{
	KITEM_POS sPos;
	if (!GetItemPos(nItemIdx, sPos))
		return FALSE;
#ifndef GAME_SERVER
	if (sPos.eRoom == emROOM_MAINBAG)
		m_rcPlayer.m_cPlayerStall.DelOfferPrice(nItemIdx);
#endif
	if (!RemoveOnly(nItemIdx, eWay))
		return FALSE;

#ifdef GAME_SERVER
	KITEM_REMOVE_SYNC sSync;
	sSync.dwID = Item[nItemIdx].GetID();
	g_cItemMgr.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
#else

	m_rcPlayer.Refect(PLAYER_ITEM_REMOVE, nItemIdx);
	g_cItemMgr.Remove(nItemIdx);		// 客户端从人身上删除道具时则同时从整个客户端删除
	g_CoreEventNotify(emCOREEVENT_SYNC_ITEM, "ddd", sPos.eRoom, sPos.nX, sPos.nY);
#endif

	return TRUE;
}

INT KPlayerItem::FindFree(VOID) CONST
{
	return m_FreeIdx.GetNext(0);
}

INT KPlayerItem::FindSame(INT nGameIdx) CONST
{
	if (nGameIdx <= 0 || nGameIdx >= MAX_ITEM)
		return	0;

	INT nIdx = 0;

	while (true)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;

		if (m_asList[nIdx].nIdx == nGameIdx)
			return nIdx;
	}

	return 0;
}

BOOL KPlayerItem::Active()
{
	INT nInterval = GAME_FPS / 2;
#ifdef GAME_SERVER
	if (!(g_cOrpgWorld.m_dwLogicFrames % nInterval))
		CheckTimeout(KSysService::GameTime(NULL), nInterval);
#endif
	return TRUE;
}

BOOL KPlayerItem::SatisfyRequire(KE_ITEM_REQUIREMENT eRequire, INT nValue) CONST
{
	switch (eRequire)
	{
	case emEQUIP_REQ_ROUTE:			// 路线
		if (m_rcPlayer.GetCurrentRoute() != nValue)
			return FALSE;
		break;
	case emEQUIP_REQ_LEVEL:			// 级别
		if (m_rcPlayer.GetLevel() < nValue)
			return FALSE;
		break;
	case emEQUIP_REQ_FACTION:		// 门派
		if (m_rcPlayer.GetPrimaryFaction() != nValue)
			return FALSE;
		break;
	case emEQUIP_REQ_SEX:			// 性别
		if (m_rcPlayer.GetSex() != nValue)
			return FALSE;
		break;
	case emEQUIP_REQ_NONE:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;
}

VOID KPlayerItem::ReApplyEquipRes()
{
	CONST KE_EQUIP_POSITION emEquipPos[] =
	{
		emEQUIPPOS_SWORD,
		emEQUIPPOS_HEAD,
		emEQUIPPOS_BACK,
		emEQUIPPOS_BOTTOM,
		emEQUIPPOS_ARMOR,
		emEQUIPPOS_SUIT,
		emEQUIPPOS_CLOAK,
	};

	for(INT i = 0; i < countof(emEquipPos); ++i)
	{
		INT nEquipPos = emEquipPos[i];
		INT nItemIdx = GetItemByPos(emROOM_EQUIP, nEquipPos, 0);
		if(nItemIdx > 0)
		{
			EquipResForNPC(nEquipPos);
		}
		else
		{
			switch(nEquipPos)
			{
			case emEQUIPPOS_SWORD:
				m_rcPlayer.SetRightWeaponRepresentId(0);
				m_rcPlayer.SetLeftWeaponRepresentId(0);
				break;
			case emEQUIPPOS_HEAD:
				m_rcPlayer.SetHeadPendantRepresentId(0);
				if(m_rcPlayer.GetSex() == emROLETYPE_MALE)
				{
					m_rcPlayer.SetHatRepresentId(emHEAD_MAN);
				}
				else
				{
					m_rcPlayer.SetHatRepresentId(emHEAD_WOMEN);
				}
				break;
			case emEQUIPPOS_BACK:
				m_rcPlayer.SetBackPendantRepresentId(0);
				break;
			case emEQUIPPOS_BOTTOM:
				m_rcPlayer.SetBottomPendantRepresentId(0);
				break;
			case emEQUIPPOS_ARMOR:
				if(m_rcPlayer.GetSex() == emROLETYPE_MALE)
				{
					m_rcPlayer.SetBodyRepresentId(emBODY_MAN);
				}
				else
				{
					m_rcPlayer.SetBodyRepresentId(emBODY_WOMAN);
				}
				break;
			case emEQUIPPOS_CLOAK:
				m_rcPlayer.SetLeftShoulderRepresentId(0);
				break;
			default:
				break;
			}
		}
	}
}

BOOL KPlayerItem::EquipResForNPC(INT nEquipPos)
{
	INT nEquip = GetEquip(nEquipPos);
	if (nEquip <= 0)
		return FALSE;

	KE_NPCRES_PART ePart = (KE_NPCRES_PART)(-1);
	switch (nEquipPos)
	{
	case emEQUIPPOS_SWORD:
		ePart = emNPCRES_PART_WEAPON;
		break;
	case emEQUIPPOS_HEAD:
		ePart = emNPCRES_PART_HEAD;
		break;
	case emEQUIPPOS_BACK:
		ePart = emNPCRES_PART_BACK;
		break;
	case emEQUIPPOS_BOTTOM:
		ePart = emNPCRES_PART_BOTTEM;
		break;
	case emEQUIPPOS_ARMOR:
		ePart = emNPCRES_PART_BODY;
		break;
	case emEQUIPPOS_CLOAK:
		ePart = emNPCRES_PART_CLOAK;
		break;
	case emEQUIPPOS_SUIT:
		ePart = emNPCRES_PART_SUIT;
		break;
	default:
		return TRUE;
	}

	// todo [7/13/2012 simon]
	/*
	KNpc* pNpc = &Npc[m_rcPlayer.m_nIndex];
	KItem* pItem = &Item[nEquip];

	pNpc->ModifyEquipRes(ePart, FALSE, pItem);
	*/
	KItem* pItem = &Item[nEquip];
	ModifyEquipRes(&m_rcPlayer, ePart, pItem, TRUE);

	return TRUE;
}

// 计算方法：当前值除以最大值，得到一个百分比，多个属性的平均值
INT KPlayerItem::CalcEquipScore(KItem* pEquip)
{
	INT nValue = 0;
	INT nMaxValue = 0;
	INT nMinValue = 0;
	INT nScore = 0;

	INT nTotalScore = 0;
	INT nTotalAttr = 0;
	QCONFIRM_RET_NULL(KD_ISEQUIP(pEquip->GetGenre()));

	CONST KLibOfBPT* pLibBPT = g_cItemMgr.GetGenerator()->GetLibOfBPT();
	QCONFIRM_RET_FALSE(pLibBPT);

	KITEM_INDEX sIndex = { item_equip_general, pEquip->GetDetailType(), pEquip->GetParticular()};
	CONST KBASICPROP_EQUIPMENT* pEq = static_cast<CONST KBASICPROP_EQUIPMENT*>(pLibBPT->GetBasicProp(sIndex));
	QCONFIRM_RET_FALSE(pEq);

	for (INT i = 0; i < emITEM_COUNT_BASE; i++)
	{
		for(INT j = 0; j < KD_MAGIC_VALUE_NUM; ++j)
		{
			nValue = pEquip->m_aryBaseAttrib[i].nValue[j];
			nMaxValue = pEq->aryBasic[i].aryRange[j].nMax;
			nMinValue = pEq->aryBasic[i].aryRange[j].nMin;
			if(!nMaxValue || !nValue) 
				continue;
			if(nMaxValue == nMinValue)
			{
				nScore = 40;
			}
			else
			{
				nScore = (nValue - nMinValue) * 40 / (nMaxValue - nMinValue);
			}

			nTotalScore += nScore;
			nTotalAttr++;
		}
	}

	INT nAveScore = nTotalScore / nTotalAttr + 60;
	QCONFIRM_RET_NULL(nAveScore > 0 && nAveScore <100);

	return nAveScore;
}

BOOL KPlayerItem::GetStoneAttrib(KMagicAttrib* pAttrib, INT nParticular, INT nLevel, INT i)
{
	CONST KLibOfBPT* pLibBPT = g_cItemMgr.GetGenerator()->GetLibOfBPT();
	QCONFIRM_RET_FALSE(pLibBPT);

	KITEM_INDEX sIndex = { item_script, item_script_stone, nParticular, nLevel };
	CONST KBASICPROP_STONE* pSt = static_cast<CONST KBASICPROP_STONE*>(pLibBPT->GetBasicProp(sIndex));
	//QCONFIRM_RET_FALSE(pSt);
	if(!pSt)
	{
		return FALSE;
	}
	pAttrib->nAttribType = pSt->aryAttrib[i].nAttrib;
	// 目前宝石属性只有1个参数
	for(INT j = 0; j < KD_MAGIC_VALUE_NUM; ++j)
	{
		pAttrib->nValue[j] = pSt->aryAttrib[i].aryParam;
	}

	return TRUE;
}

INT KPlayerItem::CalcEquipFightScore(KItem* pEquip)
{
	INT nValue = 0;
	INT nScore = 0;
	CONST KMagicAttrib *pAttrib = NULL;

	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	QCONFIRM_RET_NULL(pItemGen);

	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	QCONFIRM_RET_NULL(pLibBPT);

	QCONFIRM_RET_NULL(KD_ISEQUIP(pEquip->GetGenre()));

	// 基础属性
	for (INT i = 0; i < emITEM_COUNT_BASE; i++)
	{
		pAttrib = pEquip->GetBaseAttrib(i);
		nValue = pAttrib->nValue[0];
		nScore += nValue * g_cItemMgr.m_cSetting.GetAttribScore(pAttrib->nAttribType);
	}

	// 强化属性
	pAttrib = pEquip->GetEnhAttribArr();
	nValue = pAttrib->nValue[0];
	nScore += nValue * g_cItemMgr.m_cSetting.GetAttribScore(pAttrib->nAttribType);

	// 宝石属性
	for(INT i = 0; i < pEquip->GetStoneSlot(); ++i)
	{
		INT nParticular = pEquip->m_aryStoneInfo[i].byStoneId;
		if(!nParticular || nParticular == emSTONE_LOCKED_ID) 
			continue;

		INT nLevel = pEquip->m_aryStoneInfo[i].byStoneLevel;
		if(nLevel >= 1 && nLevel <= emSTONE_MAX_LV)
			continue;

		KMagicAttrib aryStoneAttr[emSTONE_COUNT_ATTRIB];
		for(INT j = 0; j < emSTONE_COUNT_ATTRIB; ++j)
		{
			BOOL bRet = GetStoneAttrib(&aryStoneAttr[j], nParticular, nLevel, j);
			QCONFIRM_RET_NULL(bRet);
			pAttrib = &aryStoneAttr[j];
			nValue = pAttrib->nValue[0];
			nScore += nValue * g_cItemMgr.m_cSetting.GetAttribScore(pAttrib->nAttribType);
		}
	}

	nScore = nScore / 100;

	pEquip->SetFightScore(nScore);

	return nScore;
}

INT KPlayerItem::CalcStoneFightScore(KItem* pStone)
{
	INT nValue = 0;
	INT nScore = 0;
	CONST KMagicAttrib *pAttrib = NULL;

	QCONFIRM_RET_NULL(KD_ISSTONE(pStone->GetGenre(), pStone->GetDetailType()));

	KMagicAttrib aryStoneAttr[emSTONE_COUNT_ATTRIB];
	for(INT i = 0; i < emSTONE_COUNT_ATTRIB; ++i)
	{
		BOOL bRet = GetStoneAttrib(&aryStoneAttr[i], pStone->GetParticular(), pStone->GetLevel(), i);
		pAttrib = &aryStoneAttr[i];
		nValue = (pAttrib->nValue[0] + pAttrib->nValue[1]) / 2;
		nScore += nValue * g_cItemMgr.m_cSetting.GetStoneAttribScore(pAttrib->nAttribType);
	}

	nScore = nScore / 100;

	pStone->SetFightScore(nScore);

	return nScore;
}

INT KPlayerItem::GetEquipByDetail(INT nDetail)
{
	KE_ITEM_ROOM eRoom = emROOM_EQUIP;
	INT nX = g_anEquipPos[nDetail];
	INT nY = 0;

	INT nItem = GetItemByPos(eRoom, nX, nY);

	return nItem;
}

VOID KPlayerItem::ModifyExRepresent(KPlayer* pPlayer, KItem* pItem, BOOL bPutOn)
{
	INT nStoneCount = 0;
	INT nExPendant = 0;
	for(INT i = 0; i < emSTONE_MAX_SLOT; ++i)
	{
		if(pItem->HasStone(i + 1))
		{
			++nStoneCount;
		}
	}
	QCONFIRM_RET(nStoneCount >= 0 && nStoneCount <= emITEM_COUNT_EX_PENDANT, );

	if(nStoneCount > 0)
	{
		nExPendant = pItem->m_arrExPandents[nStoneCount - 1];
	}

	if(nExPendant > 0 && bPutOn)
	{
		pPlayer->SetLeftWeaponRepresentId(nExPendant);
	}
	else
	{
		pPlayer->SetLeftWeaponRepresentId(0);
	}
}

VOID KPlayerItem::ModifyBodyRepresent(KPlayer* pPlayer, KItem* pItem, BOOL bPutOn)
{
	INT nPandent1 = 0;
	INT nPandent2 = 0;
	INT nItemIdx = 0;

	// 套装效果优先
	if(pItem->GetDetailType() == equip_suit)
	{
		INT nItemIdx = GetEquipByDetail(equip_armor);
		if(bPutOn)
		{
			nPandent1 = pItem->m_nPandent1;
			nPandent2 = pItem->m_nPandent2;
		}
		else
		{
			// 如果有穿衣服，那就恢复衣服的挂件，否则恢复主身体挂件
			if(nItemIdx > 0)
			{
				KItem* pArmor = &Item[nItemIdx];
				nPandent1 = pArmor->m_nPandent1;
				nPandent2 = pArmor->m_nPandent2;
			}
			else
			{
				if(pPlayer->GetSex() == emROLETYPE_MALE)
				{
					nPandent1 = emBODY_MAN;
					nPandent2 = emBODY_MAN;
				}
				else
				{
					nPandent1 = emBODY_WOMAN;
					nPandent2 = emBODY_WOMAN;
				}
			}
		}
	}
	else if(pItem->GetDetailType() == equip_armor)
	{
		INT nItemIdx = GetEquipByDetail(equip_suit);
		if(bPutOn)
		{
			// 如果没穿套装就设置衣服的挂件
			if(!nItemIdx)
			{
				nPandent1 = pItem->m_nPandent1;
				nPandent2 = pItem->m_nPandent2;
			}
			else
			{
				KItem* pSuit = &Item[nItemIdx];
				nPandent1 = pSuit->m_nPandent1;
				nPandent2 = pSuit->m_nPandent2;
			}
		}
		else
		{
			// 脱衣服时如果有穿套装就设置套装挂件，否则恢复主身体
			if(nItemIdx)
			{
				KItem* pSuit = &Item[nItemIdx];
				nPandent1 = pSuit->m_nPandent1;
				nPandent2 = pSuit->m_nPandent2;
			}
			else
			{
				if(pPlayer->GetSex() == emROLETYPE_MALE)
				{
					nPandent1 = emBODY_MAN;
					nPandent2 = emBODY_MAN;
				}
				else
				{
					nPandent1 = emBODY_WOMAN;
					nPandent2 = emBODY_WOMAN;
				}
			}
		}
	}

	if(pPlayer->GetSex() == emROLETYPE_MALE)
	{
		pPlayer->SetBodyRepresentId(nPandent1);
	}
	else
	{
		pPlayer->SetBodyRepresentId(nPandent2);
	}
}

VOID KPlayerItem::ModifyHeadRepresent(KPlayer* pPlayer, KItem* pItem, BOOL bPutOn)
{
	INT nPandent1 = 0;
	INT nPandent2 = 0;
	INT nItemIdx = 0;

	// 套装效果优先
	if(pItem->GetDetailType() == equip_suit)
	{
		INT nItemIdx = GetEquipByDetail(equip_head);
		if(bPutOn)
		{
			nPandent1 = pItem->m_arrSuitExPandents[0];
			nPandent2 = pItem->m_arrSuitExPandents[1];
		}
		else
		{
			// 如果有戴头盔，那就恢复头盔的挂件，否则恢复主身体挂件
			if(nItemIdx > 0)
			{
				KItem* pHead = &Item[nItemIdx];
				nPandent1 = pHead->m_nPandent1;
				nPandent2 = pHead->m_nPandent2;
			}
			else
			{
				if(pPlayer->GetSex() == emROLETYPE_MALE)
				{
					nPandent1 = emHEAD_MAN;
					nPandent2 = emHEAD_MAN;
				}
				else
				{
					nPandent1 = emHEAD_WOMEN;
					nPandent2 = emHEAD_WOMEN;
				}
			}
		}
	}
	else if(pItem->GetDetailType() == equip_head)
	{
		INT nItemIdx = GetEquipByDetail(equip_suit);
		if(bPutOn)
		{
			// 如果没穿套装就设置衣服的挂件
			if(!nItemIdx)
			{
				nPandent1 = pItem->m_nPandent1;
				nPandent2 = pItem->m_nPandent2;
			}
			else
			{
				KItem* pSuit = &Item[nItemIdx];
				nPandent1 = pSuit->m_arrSuitExPandents[0];
				nPandent2 = pSuit->m_arrSuitExPandents[1];
			}
		}
		else
		{
			// 脱衣服时如果有穿套装就设置套装挂件，否则恢复主身体
			if(nItemIdx)
			{
				KItem* pSuit = &Item[nItemIdx];
				nPandent1 = pSuit->m_arrSuitExPandents[0];
				nPandent2 = pSuit->m_arrSuitExPandents[1];
			}
			else
			{
				if(pPlayer->GetSex() == emROLETYPE_MALE)
				{
					nPandent1 = emHEAD_MAN;
					nPandent2 = emHEAD_MAN;
				}
				else
				{
					nPandent1 = emHEAD_WOMEN;
					nPandent2 = emHEAD_WOMEN;
				}
			}
		}
	}

	if(pPlayer->GetSex() == emROLETYPE_MALE)
	{
		pPlayer->SetHatRepresentId(nPandent1);
		pPlayer->SetHeadPendantRepresentId(0);
	}
	else
	{
		pPlayer->SetHatRepresentId(nPandent2);
		pPlayer->SetHeadPendantRepresentId(0);
	}
}

VOID KPlayerItem::ModifyShoulderRepresent(KPlayer* pPlayer, KItem* pItem, BOOL bPutOn)
{
	INT nPandent1 = 0;
	INT nPandent2 = 0;
	INT nItemIdx = 0;

	// 套装效果优先
	if(pItem->GetDetailType() == equip_suit)
	{
		INT nItemIdx = GetEquipByDetail(equip_cloak);
		if(bPutOn)
		{
			nPandent1 = pItem->m_arrSuitExPandents[2];
			nPandent2 = pItem->m_arrSuitExPandents[3];
		}
		else
		{
			// 如果有戴头盔，那就恢复头盔的挂件，否则恢复主身体挂件
			if(nItemIdx > 0)
			{
				KItem* pBack = &Item[nItemIdx];
				nPandent1 = pBack->m_nPandent1;
				nPandent2 = pBack->m_nPandent2;
			}
			else
			{
				nPandent1 = 0;
				nPandent2 = 0;
			}
		}
	}
	else if(pItem->GetDetailType() == equip_cloak)
	{
		INT nItemIdx = GetEquipByDetail(equip_suit);
		if(bPutOn)
		{
			// 如果没穿套装就设置衣服的挂件
			if(!nItemIdx)
			{
				nPandent1 = pItem->m_nPandent1;
				nPandent2 = pItem->m_nPandent2;
			}
			else
			{
				KItem* pBack = &Item[nItemIdx];
				nPandent1 = pBack->m_arrSuitExPandents[2];
				nPandent2 = pBack->m_arrSuitExPandents[3];
			}
		}
		else
		{
			// 脱衣服时如果有穿套装就设置套装挂件，否则恢复主身体
			if(nItemIdx)
			{
				KItem* pBack = &Item[nItemIdx];
				nPandent1 = pBack->m_arrSuitExPandents[2];
				nPandent2 = pBack->m_arrSuitExPandents[3];
			}
			else
			{
				nPandent1 = 0;
				nPandent2 = 0;
			}
		}
	}

	if(pPlayer->GetSex() == emROLETYPE_MALE)
	{
		pPlayer->SetLeftShoulderRepresentId(nPandent1);
	}
	else
	{
		pPlayer->SetLeftShoulderRepresentId(nPandent2);
	}
}

// 应用套装效果
VOID KPlayerItem::ModifySuitRepresent(KPlayer* pPlayer, KItem* pSuit, BOOL bPutOn)
{
	// 身体
	ModifyBodyRepresent(pPlayer, pSuit, bPutOn);

	// 头部
	ModifyHeadRepresent(pPlayer, pSuit, bPutOn);

	// 背部
	ModifyShoulderRepresent(pPlayer, pSuit, bPutOn);
}


VOID KPlayerItem::ModifyEquipRes(KPlayer* pPlayer, KE_NPCRES_PART ePart, KItem* pItem, BOOL bPutOn)
{
	INT nPandent1 = 0;
	INT nPandent2 = 0;
	if(bPutOn)
	{
		nPandent1 = pItem->m_nPandent1;
		nPandent2 = pItem->m_nPandent2;
	}

	switch(ePart)
	{
	case emNPCRES_PART_WEAPON:
		if(pPlayer->GetSex() == emROLETYPE_MALE)
		{
			pPlayer->SetRightWeaponRepresentId(nPandent1);
		}
		else
		{
			pPlayer->SetRightWeaponRepresentId(nPandent2);
		}

		ModifyExRepresent(pPlayer, pItem, bPutOn);

		break;
	case emNPCRES_PART_HEAD:
		ModifyHeadRepresent(pPlayer, pItem, bPutOn);
		break;
	case emNPCRES_PART_SHOULDER:
		pPlayer->SetLeftShoulderRepresentId(nPandent1);
		pPlayer->SetRightShoulerRepresentId(nPandent2);
		break;
	case emNPCRES_PART_BACK:
		if(pPlayer->GetSex() == emROLETYPE_MALE)
		{
			pPlayer->SetBackPendantRepresentId(nPandent1);
		}
		else
		{
			pPlayer->SetBackPendantRepresentId(nPandent2);
		}
		
		break;
	case emNPCRES_PART_BOTTEM:
		if(pPlayer->GetSex() == emROLETYPE_MALE)
		{
			pPlayer->SetBottomPendantRepresentId(nPandent1);
		}
		else
		{
			pPlayer->SetBottomPendantRepresentId(nPandent2);
		}
		
		break;
	case emNPCRES_PART_BODY:
		ModifyBodyRepresent(pPlayer, pItem, bPutOn);
		break;
	case emNPCRES_PART_SUIT:
		ModifySuitRepresent(pPlayer, pItem, bPutOn);
		break;
	case emNPCRES_PART_CLOAK:
		ModifyShoulderRepresent(pPlayer, pItem, bPutOn);
		break;
	default:
		break;
	}
}

BOOL KPlayerItem::CleanStone(KItem* pEquip)
{
	INT nTmp = 0;
	QCONFIRM_RET_FALSE(KD_ISEQUIP(pEquip->GetGenre()));

	for(INT nSlot = pEquip->GetStoneSlot(); nSlot > 1; --nSlot)
	{
		INT nPreSlot = nSlot - 1;
		if(!pEquip->HasStone(nSlot)) continue;
		if(pEquip->HasStone(nPreSlot)) break;

		nTmp = pEquip->m_aryStoneInfo[nSlot-1].byStoneId;
		pEquip->m_aryStoneInfo[nSlot-1].byStoneId = pEquip->m_aryStoneInfo[nPreSlot-1].byStoneId;
		pEquip->m_aryStoneInfo[nPreSlot-1].byStoneId = nTmp;

		nTmp = pEquip->m_aryStoneInfo[nSlot-1].byStoneLevel;
		pEquip->m_aryStoneInfo[nSlot-1].byStoneLevel = pEquip->m_aryStoneInfo[nPreSlot-1].byStoneLevel;
		pEquip->m_aryStoneInfo[nPreSlot-1].byStoneLevel = nTmp;
	}
	return TRUE;
}

BOOL KPlayerItem::StonePunch(KItem* pEquip, INT nPos)
{
	ASSERT(pEquip->HasStoneLocked(nPos+1) && !pEquip->HasStone(nPos+1));

	pEquip->m_aryStoneInfo[nPos].byStoneId = 0;
	pEquip->m_aryStoneInfo[nPos].byStoneLevel = 0;

	return TRUE;
}

VOID KPlayerItem::RestoreAttrib(VOID)
{
	for (INT i = 0; i < emEQUIPPOS_NUM; ++i)
		ApplyBaseAttrib(i);

	RefreshSuiteState();

	CalcActiveMA();
}

INT KPlayerItem::GetEquipPos(INT nType)
{
	if (nType <= 0 || nType > equip_detailnum)
		return	-1;

	return	g_anEquipPos[nType];
}

VOID KPlayerItem::RefreshSuiteState(VOID)
{
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();

	m_mapGoldCount.clear();
	m_mapGreenCount.clear();
	ClearSuiteMass();

	// 计算套装件数
	for (INT i = 0; i < emEQUIPPOS_NUM; i++)
	{
		//if (GetEquip(i) <= 0)
		//	continue;

		//CONST KItem* pEquip = &Item[GetEquip(i)];

		//if (!KD_ISSUITE(pEquip->GetGenre()) || pEquip->GetSuiteID() <= 0)
		//	continue;					// 不是套装装备

		//if (pEquip->IsInvalid())		// 无效装备不参与计数
		//	continue;

		//KITEM_INDEX sEqIdx = pEquip->GetIndex();

		//switch (sEqIdx.nGenre)
		//{
		//case item_equip_gold:			// 黄金装备
		//	{
		//		CONST KBASICPROP_GOLDEQUIP* pEquip = pLibBPT->m_cGoldEquip.GetData(sEqIdx);
		//		if (!pEquip || pEquip->nSuiteID <= 0)
		//			continue;
		//		m_mapGoldCount[pEquip->nSuiteID]++;
		//	}
		//	continue;
		//case item_equip_green:			// 绿色装备
		//	{
		//		CONST KBASICPROP_GREENSUITE* pEquip = pLibBPT->m_cGreenEquip.GetData(sEqIdx);
		//		if (!pEquip || pEquip->nSuiteID <= 0)
		//			continue;
		//		m_mapGreenCount[pEquip->nSuiteID]++;
		//	}
		//	continue;
		//}
	}

	/*for (KITEM_SUITE_COUNT_MAP::iterator it = m_mapBaseActiveSuit.begin(); it != m_mapBaseActiveSuit.end(); ++it)
	{
	m_mapGreenCount[it->first] += it->second;
	}*/

	// 初始化黄金装备激活属性
	KITEM_SUITE_COUNT_MAP::const_iterator itGold = m_mapGoldCount.begin();
	for (; itGold != m_mapGoldCount.end(); ++itGold)
	{
		if (itGold->first <= KD_ITEM_INVALID_SUITEID)
			continue;
		if (itGold->second <= 0)
			continue;

		CONST KSUITE_INFO* pInfo = pLibBPT->GetGoldSuiteInfo(itGold->first);
		if (!pInfo)
		{
			_ASSERT(FALSE);
			continue;
		}

		KMASS_Suite* pMASS = new KMASS_Suite(*pInfo, emITEM_SUITE_GOLD);
		pMASS->Init();
		m_vecGoldMASS.push_back(pMASS);
	}

	// 初始化绿色装备激活属性
	KITEM_SUITE_COUNT_MAP::const_iterator itGreen = m_mapGreenCount.begin();
	for (; itGreen != m_mapGreenCount.end(); ++itGreen)
	{
		if (itGreen->first <= KD_ITEM_INVALID_SUITEID)
			continue;
		if (itGreen->second <= 0)
			continue;

		CONST KSUITE_INFO* pInfo = pLibBPT->GetGreenSuiteInfo(itGreen->first);
		if (!pInfo)
		{
			_ASSERT(FALSE);
			continue;
		}

		KMASS_Suite* pMASS = new KMASS_Suite(*pInfo, emITEM_SUITE_GREEN);
		pMASS->Init();
		m_vecGreenMASS.push_back(pMASS);
	}
}

KPlayerItem::KE_HOLDSTATUS KPlayerItem::CheckItemHoldStatus(CONST KITEM_INDEX& sItemIdx)
{
	// 检查身上是否拥有某种装备
	for (INT i = 0; i < emEQUIPPOS_NUM; ++i)
	{
		if (GetEquip(i) <= 0)
			continue;

		CONST KItem* pItem = &Item[GetEquip(i)];
		KITEM_INDEX sCurIndex = pItem->GetIndex();

		if (sItemIdx == sCurIndex)
		{
			if (pItem->IsInvalid())
				return	emHOLDSTATUS_ROOM;		// 虽然穿在身上但无效，视为放在包裹里
			return	emHOLDSTATUS_EQUIP;
		}
	}

	// 检查箱子里是否放着某种物品
	for (INT i = 0; i < KD_MAX_PLAYER_ITEM; ++i)
	{
		if (m_asList[i].nIdx <= 0)
			continue;

		KITEM_INDEX sCurIndex = Item[m_asList[i].nIdx].GetIndex();
		if (sItemIdx == sCurIndex)
			return	emHOLDSTATUS_ROOM;
	}

	return emHOLDSTATUS_NONE;
}

BOOL KPlayerItem::GetItemPos(INT nGameIndex, KITEM_POS& rsItemPos)
{
	if (nGameIndex <= 0 || nGameIndex >= MAX_ITEM)
		return	FALSE;

	INT nIndexInList = FindSame(nGameIndex);
	if (nIndexInList > 0 && nIndexInList < KD_MAX_PLAYER_ITEM)
	{
		rsItemPos = m_asList[nIndexInList].sPos;
		return	TRUE;
	}
	return	FALSE;
}

INT	KPlayerItem::SearchID(DWORD dwID, LPINT pnListIdx) CONST
{
	if (m_rcPlayer.GetPlayerIndex() <= 0 || !dwID)
		return 0;
	INT nIdx = 0;
	while (TRUE)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;
		INT nItemIdx = m_asList[nIdx].nIdx;
		if (Item[nItemIdx].GetID() == dwID)
		{
			if (pnListIdx)
				*pnListIdx = nIdx;
			return nItemIdx;
		}
	}
	return 0;
}


INT KPlayerItem::SearchGUID( const GUID& guid, LPINT pnListIdx /*= NULL*/ ) CONST
{
	if (m_rcPlayer.GetPlayerIndex() <= 0)
		return 0;
	INT nIdx = 0;
	while (TRUE)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;
		INT nItemIdx = m_asList[nIdx].nIdx;
		if (Item[nItemIdx].GetGuid() == guid)
		{
			if (pnListIdx)
				*pnListIdx = nIdx;
			return nItemIdx;
		}
	}
	return 0;
}

BOOL KPlayerItem::ValidateIndex(INT nItemIdx)
{
	if (nItemIdx <= 0)
		return	FALSE;

	if (FindSame(nItemIdx))
		return	TRUE;
	if (nItemIdx > 0)
		return	TRUE;

	return	FALSE;
}

INT KPlayerItem::GetTotalMoney(VOID) CONST
{
	INT nMoney = 0;
	for (INT i = 0; i < emPURSE_NUM; ++i)
		nMoney += m_aMoney[i];
	return	nMoney;
}

BOOL KPlayerItem::AddMoney(KE_PURSE_TYPE ePurse, INT nMoney)
{
	if (ePurse < 0 || ePurse >= emPURSE_NUM)
		return	FALSE;
	INT nTotalAmount = m_aMoney[ePurse] + nMoney;
	if (nTotalAmount < 0 || nTotalAmount > m_nMaxCarryMoney)
	{
#ifdef GAME_SERVER
		g_ChatChannelManager.ProcessRespond(trLocalSys,
			emKSYSTEMMSG_NORMAL,
			m_rcPlayer.GetId(),
			"系统",
			LANG_007, emQCHAT_PANEL
			);
#else
		KPlayerChat::NotifySystemMessage(MSG_ITEM_TAKE_MONEY_ERR);
#endif
		return FALSE;
	}
	m_aMoney[ePurse] += nMoney;
#ifdef GAME_SERVER
	//m_rcPlayer.CumulateValue(abs(nMoney));
	SendMoneySync();
#endif
	return	TRUE;
}

BOOL KPlayerItem::CostBindMoney(INT nMoney, INT nCostWay)
{
	BOOL bRet = FALSE;
	//	INT nCurMoney	= m_rcPlayer.m_cPlayerTask.GetTaskValue(TASKVALUE_BINDMONEY);
	//	PROCESS_ERROR(nCurMoney >= nMoney && nMoney >= 0);
	//	m_rcPlayer.m_cPlayerTask.SetTaskValue(TASKVALUE_BINDMONEY, nCurMoney - nMoney);
	//#ifdef GAME_SERVER
	//	if (nMoney > 0 && nCostWay != emKPAY_NONE && (nCostWay != emKPAY_SHOP || nMoney >= 5000))
	//		KD_COURSE_LOG(m_rcPlayer, emKCOURSE_BIND_JXB_CHANGE, "CostBindMoney\tCostWay" << nCostWay << "\tCount\t" << nMoney);
	//
	//	if (m_rcPlayer.m_eRecordLogType & KPlayer::recordlog_money)
	//		m_rcPlayer.WriteLog("AddBindMoney[%d]\t-%d\t=%d", nCostWay, nMoney, nCurMoney - nMoney);
	//
	//	OnCoinChanged(3, -nMoney);	// 3表示绑银类型，数量减少，第二个参数为负
	//	m_rcPlayer.CumulateValue(abs(nMoney));
	//	//SendMoneySync();
	//#endif
	//	bRet = TRUE;
	//EXIT0:
	return bRet;
}

BOOL KPlayerItem::CostMoney(INT nMoney, INT nPayType)
{
	if (nMoney > GetCashMoney())
		return FALSE;
	if (!AddMoney(emPURSE_CASH, -nMoney))
		return FALSE;
#ifdef GAME_SERVER
	if (nPayType != emKPAY_PLAYER_TRADE && nPayType != emKPAY_GIVE && nPayType != emKPAY_SENDMAIL && nPayType != emKPAY_AUCTION_PRICE
		&& nPayType != emKPAY_SCRIPT_JBEXCHANGE && nPayType != emKPAY_SCRIPT_TONGFUD && nPayType != emKPAY_SCRIPT_PEEL
		&& nPayType != emKPAY_SCRIPT_RESTOREBANK)
	{
		OnCoinChanged(2, -nMoney);	// 2表示类型是银两，这里是减少，第二个参数应该为负
	}
#endif
	return TRUE;
}
//INT KPlayerItem::GetBankStoreCoin() CONST
//{
//	return m_rcPlayer.m_cPlayerTask.GetTaskValue(TASKVALUE_BANKSTORECOIN);
//}
//
//INT	KPlayerItem::GetBindMoney()
//{
//	return m_rcPlayer.m_cPlayerTask.GetTaskValue(TASKVALUE_BINDMONEY);
//}

BOOL KPlayerItem::DecMoney(INT nMoney)
{
	if (nMoney < 0)
		return FALSE;

	if (nMoney > GetCashMoney())
	{
		nMoney -= GetCashMoney();
		SetMoney(emPURSE_CASH, 0);
		if (nMoney > GetSaveMoney())
			SetMoney(emPURSE_SAVE, 0);
		else
			AddMoney(emPURSE_SAVE, -nMoney);
	}
	else
	{
		AddMoney(emPURSE_CASH, -nMoney);
	}

#ifdef GAME_SERVER
	SendMoneySync();
#endif

	return TRUE;
}

#ifdef GAME_SERVER

VOID KPlayerItem::SendMoneySync()
{
	//KPLAYER_MONEY_SYNC sMoney;
	//sMoney.ProtocolType = s2c_syncmoney;
	//sMoney.dwCash = (DWORD)GetCashMoney();
	//sMoney.dwSave = (DWORD)GetSaveMoney();

	//g_cItemMgr.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), &sMoney);
}

#endif

VOID KPlayerItem::SetMoney(INT nCash, INT nSave)
{
	if (nCash >= 0)
		m_aMoney[emPURSE_CASH] = nCash;
	if (nSave >= 0)
		m_aMoney[emPURSE_SAVE] = nSave;
#ifdef GAME_SERVER
	//if (m_rcPlayer.m_eRecordLogType & KPlayer::recordlog_money)
	//	m_rcPlayer.WriteLog("SetMoney\t%d+%d\t=%d", nCash, nSave, nCash + nSave);
	m_dwCheckMoney	= nCash + nSave;	// 载入数据时临时使用此变量保存载入时数据
#else
	::g_CoreEventNotify(emCOREEVENT_MONEY_CHANGED, "dd", nCash, nSave);
#endif
}

VOID KPlayerItem::SetMoney(KE_PURSE_TYPE ePurse, INT nMoney)
{
	if (ePurse < 0 || ePurse >= emPURSE_NUM || nMoney < 0)
		return;
	m_aMoney[ePurse] = nMoney;
}

INT KPlayerItem::GetItemByPos(CONST KITEM_POS& rsPos) CONST
{
	INT nItemIdx = 0;
	KPlayer* pMe = &m_rcPlayer;
	KItem* pItem = GetItemFromItemPos(&rsPos, pMe);
	if (pItem)
		nItemIdx = pItem->GetGameIndex();
	return nItemIdx;
}

INT KPlayerItem::GetItemByPos(KE_ITEM_ROOM eRoom, INT nX /* = 0 */, INT nY /* = 0 */) CONST
{
	KITEM_POS sPos;
	sPos.eRoom	= eRoom;
	sPos.nX		= nX;
	sPos.nY		= nY;
	return GetItemByPos(sPos);
}

KLISTITEM* KPlayerItem::GetItem(INT nListIdx /* = 0 */)
{
	if (nListIdx < 0 || nListIdx >= KD_MAX_PLAYER_ITEM)
		return	NULL;

	return m_asList + nListIdx;
}

KItem* KPlayerItem::GetItemById(DWORD dwItemId)
{
	INT nItemIdx = SearchID(dwItemId);
	KItem *pItem = nItemIdx ? &Item[nItemIdx] : NULL;
	return pItem;
}

KLISTITEM* KPlayerItem::GetFirstItem(VOID)
{
	m_nListCurIdx = m_UseIdx.GetNext(0);
	return	&m_asList[m_nListCurIdx];
}

KLISTITEM* KPlayerItem::GetNextItem(VOID)
{
	if (!m_nListCurIdx)
		return	NULL;
	m_nListCurIdx = m_UseIdx.GetNext(m_nListCurIdx);
	return	&m_asList[m_nListCurIdx];
}

VOID KPlayerItem::ClearRoom(KE_ITEM_ROOM eRoom)
{
	if (eRoom >= 0 && eRoom < emROOM_NUM)
		m_acRoom[eRoom].Clear();
}

VOID KPlayerItem::RemoveAll(VOID)
{
	INT nIdx = m_UseIdx.GetNext(0);
	INT nIdx1 = 0;
	while(nIdx)
	{
		nIdx1 = m_UseIdx.GetNext(nIdx);
		INT nGameIdx = m_asList[nIdx].nIdx;
		if (!Remove(nGameIdx, emKLOSEITEM_NONE))
		{
			m_UseIdx.Remove(nIdx);
			m_FreeIdx.Insert(nIdx);
		}
#ifdef GAME_SERVER
		// 客户端在上面KPlayerItem::Remove()已经做了g_cItemMgr.Remove()
		else 
		{
			g_cItemMgr.Remove(nGameIdx);
		}
#endif
		nIdx = nIdx1;
	}
}

#ifdef GAME_SERVER

BOOL KPlayerItem::IsItemExist(INT nGenreType, INT nDetailType, INT nParticularType)
{
	INT nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		INT nGameIdx = m_asList[nIdx].nIdx;
		if (nGenreType != Item[nGameIdx].GetGenre())
			continue;

		if (((nDetailType == -1) || nDetailType == Item[nGameIdx].GetDetailType()) && 
			((nParticularType == -1) || (nParticularType == Item[nGameIdx].GetParticular())))
		{
			return	TRUE;
		}
	}
	return	FALSE;
}

BOOL KPlayerItem::RemoveItem(INT nGenreType, INT nDetailType, INT nParticularType)
{
	INT nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		INT nGameIdx = m_asList[nIdx].nIdx;
		if (nGenreType != Item[nGameIdx].GetGenre())
			continue;

		if (Item[nGameIdx].ForBitForDupedItem())		// 复制品不算(不能使用)
			continue;

		if (((nDetailType == -1) || nDetailType == Item[nGameIdx].GetDetailType()) && 
			((nParticularType == -1) || (nParticularType == Item[nGameIdx].GetParticular())))
		{
			if (!Remove(nGameIdx, emKLOSEITEM_BYGDP))
				return FALSE;

			g_cItemMgr.Remove(nGameIdx);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL KPlayerItem::IsMyItem(INT nItemIndex)
{
	if (nItemIndex <= 0 || nItemIndex >= MAX_ITEM)
		return FALSE;

	INT nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (m_asList[nIdx].nIdx == nItemIndex)
			return TRUE;
	}

	return FALSE;
}

#endif

#ifdef GAME_SERVER

BOOL KPlayerItem::StackItem(INT& nStackPick, INT nStackDrop, KE_ADD_ITEM_WAY eWay,INT nItemCount/*= 1*/)
{
	if (nStackDrop <= 0 || nStackDrop >= MAX_ITEM || nStackPick <= 0 || nStackPick >= MAX_ITEM ||
		nStackDrop == nStackPick || !Item[nStackDrop].CanStackWith(Item[nStackPick]))
		return	FALSE;

	INT nListIdx = FindSame(nStackDrop);
	if (nListIdx <= 0)
		return FALSE;

	INT nItemToStackCount		= Item[nStackDrop].GetCount();
	INT nItemStackWithCount		= Item[nStackPick].GetCount();
	INT nItemToStackCount_Max	= Item[nStackDrop].GetMaxCount();
	INT nItemStackWithCount_Max	= Item[nStackPick].GetMaxCount();

	if(nItemCount > 1){
		nItemStackWithCount = nItemCount;//定额数量道具
	}

	if (nItemToStackCount < 1 || nItemStackWithCount < 1 || nItemToStackCount_Max != nItemStackWithCount_Max)
		return FALSE;

	INT nTotalItemCount = nItemToStackCount + nItemStackWithCount;
	INT nMaxStackCount = nItemToStackCount_Max;

	if (nTotalItemCount <= nMaxStackCount)
	{	// nStackPick物品有可能不在ItemList里，比如捡起的物品自动叠放时，故不在此判断是否在ItemList删除成功
		// 日志：只记录自己身上被叠加删除的物品
		if (Item[nStackPick].GetBelong() == m_rcPlayer.GetPlayerIndex())
		{
			/*
			KLogWritter::GetInstance()->ItemHistory_RemoveItem(
			&Item[nStackPick],
			&m_rcPlayer,
			emKLOG_REASON_REMOVEITEM_WRAP);
			*/
		}

		Remove(nStackPick, emKLOSEITEM_STACK);
		if (!g_cItemMgr.Remove(nStackPick, emITEM_DATARECORD_NORMAL, emITEM_CONSUMEMODE_STACK, &m_rcPlayer))
		{
			ASSERT(FALSE);
			return FALSE;
		}
		nStackPick = 0;
		// 这时应该是添加物品
		if (!SetItemStackCount(nStackDrop, nTotalItemCount, emITEM_DATARECORD_NORMAL, emKLOSEITEM_NONE, eWay))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	else
	{
		if (!SetItemStackCount(nStackPick, nTotalItemCount - nMaxStackCount, emITEM_DATARECORD_NORMAL, emKLOSEITEM_NONE, eWay))
		{
			ASSERT(FALSE);
			return FALSE;
		}
		if (!SetItemStackCount(nStackDrop, nMaxStackCount, emITEM_DATARECORD_NORMAL, emKLOSEITEM_NONE, eWay))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL KPlayerItem::SplitItem(INT nItemIdx, INT nSplitCount)
{
	INT nListIdx = FindSame(nItemIdx);
	if (nListIdx <= 0)
		return FALSE;

	KItem& rcItem  = Item[nItemIdx];
	INT nOrgCount  = rcItem.GetCount();
	INT nLeftCount = nOrgCount - nSplitCount;
	if (rcItem.GetMaxCount() <= 1 || nOrgCount <= 1 || nSplitCount <= 0 || nLeftCount <= 0)
		return FALSE;

	KITEM_POS sPos;
	if (!GetBagFreeCell(sPos))			// 剩余格子不够
		return	FALSE;

	INT nSplitItemIdx = g_cItemMgr.Add(
		rcItem.GetGenre(),
		rcItem.GetDetailType(),
		rcItem.GetParticular(),
		rcItem.GetLevel(),
		rcItem.GetEnhanceTimes(),
		emITEM_DATARECORD_NORMAL,		//拆分道具，并不是真正的产出，不记录
		rcItem.m_aryStoneInfo,
		rcItem.m_aryAppendAttrib,
		rcItem.GetRandSeed(),
		rcItem.GetRandSeedEx()
		);

	if (nSplitItemIdx <= 0)
		return FALSE;

	KItem& rcSplit = Item[nSplitItemIdx];
	VERIFY(rcItem.SetCount(nLeftCount));
	VERIFY(rcSplit.SetCount(nSplitCount));
	if (rcItem.IsBind())
		rcSplit.Bind(TRUE);				// 拆分物品保持绑定状态

	if (!Add(nSplitItemIdx, sPos, emKADDITEM_SPLIT))
	{
		rcItem.SetCount(nOrgCount);
		g_cItemMgr.Remove(nSplitItemIdx);
		return FALSE;
	}

	KITEM_COUNT_SYNC sSync;
	//sSync.cProtocol	= (PROTOCOL_MSG_TYPE)s2c_itemcountsync;
	sSync.dwId		= rcItem.GetID();
	sSync.wCount	= (WORD)rcItem.GetCount();

	g_cItemMgr.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	return TRUE;
}

INT KPlayerItem::ConsumeItemInBags(INT nCount, CONST KITEM_INDEX& sIndex)
{
	if (nCount < 0)
		return	-1;

	INT nLeft = nCount;

	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		KE_ITEM_ROOM eRoom = g_aeBagRoom[i];
		nLeft = ConsumeRoomItem(eRoom, nLeft, sIndex);
		if (nLeft == 0)
			return	0;
	}

	return	nLeft;
}

INT KPlayerItem::ConsumeItemInBags(INT nCount,
								   INT nGenre,			// = KD_ITEM_INVALID_TYPE
								   INT nDetailType,		// = KD_ITEM_INVALID_TYPE
								   INT nParticular,		// = KD_ITEM_INVALID_TYPE
								   INT nLevel			// = KD_ITEM_NO_LEVEL_LIMIT
								   )
{
	KITEM_INDEX sIndex;
	sIndex.nGenre		= nGenre;
	sIndex.nDetailType	= nDetailType;
	sIndex.nParticular	= nParticular;
	sIndex.nLevel		= nLevel;
	return	ConsumeItemInBags(nCount, sIndex);
}

//按照先消耗绑定物品,再消耗非绑定物品顺序进行支付
INT KPlayerItem::ConsumeItemInBagsByBindOrder(INT nCount, 
											  INT* pnCosumeBindCount,
											  INT nGenre,			// = KD_ITEM_INVALID_TYPE
											  INT nDetailType,		// = KD_ITEM_INVALID_TYPE
											  INT nParticular		// = KD_ITEM_INVALID_TYPE
											  )
{
	KITEM_INDEX sIndex;
	sIndex.nGenre		= nGenre;
	sIndex.nDetailType	= nDetailType;
	sIndex.nParticular	= nParticular;

	if (nCount < 0)
		return	-1;

	INT nLeft = nCount;
	INT nCosumeBindCount = 0;
	INT nRemain = 0;
	//先消耗绑定物品
	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		KE_ITEM_ROOM eRoom = g_aeBagRoom[i];
		nRemain = ConsumeRoomItem(eRoom, nLeft, sIndex, emCONSUME_BIND_ONLY);
		nCosumeBindCount += nLeft - nRemain; //本次循环消耗的物品个数
		nLeft = nRemain;
		*pnCosumeBindCount = nCosumeBindCount;
		if (nLeft == 0)
			return	0;
	}
	//再消耗非绑定物品
	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		KE_ITEM_ROOM eRoom = g_aeBagRoom[i];
		nLeft = ConsumeRoomItem(eRoom, nLeft, sIndex, emCONSUME_NOBIND_ONLY);
		if (nLeft == 0)
			return	0;
	}
	return	nLeft;
}

INT KPlayerItem::ConsumeRoomItem(KE_ITEM_ROOM eRoom, INT nCount, CONST KITEM_INDEX& sIndex, KE_CONSUME_TYPE eConsumeType/* = emCONSUME_ALL*/)
{
	if (eRoom < 0 || eRoom >= emROOM_NUM)
		return	nCount;

	INT nLeft = nCount;
	KItemRoom& rcRoom = m_acRoom[eRoom];
	KItemRoom::KFinder cFind = rcRoom.GetFinder();
	KRoomFindCondition::SameIndexSeries cCond(sIndex, eConsumeType);
	//KLogWritter* pLog = KLogWritter::GetInstance();
	while (cFind.Next(cCond) && nLeft > 0)
	{
		INT nItemIdx = cFind.Index();
		KItem& rcItem = Item[nItemIdx];
		INT nCurCount = rcItem.GetCount();

		// 一叠道具全扣完必须用g_cItemMgr.Remove，没扣完必须用SetItemStackCount，否则推广员消耗会重复计算
		if (nCurCount <= nLeft)
		{
			if (Remove(nItemIdx, emKLOSEITEM_CONSUME))
			{
				/*
				// 日志
				pLog->ItemHistory_RemoveItem(
				&Item[nItemIdx],
				&m_rcPlayer,
				emKLOG_REASON_REMOVEITEM_CONSUME);
				*/
				g_cItemMgr.Remove(nItemIdx, emITEM_DATARECORD_REMOVE, emITEM_CONSUMEMODE_CONSUME, &m_rcPlayer);
				nLeft -= nCurCount;
			}
		}
		else
		{
			BOOL bStackOk = SetItemStackCount(nItemIdx, rcItem.GetCount() - nLeft, emITEM_DATARECORD_REMOVE, emKLOSEITEM_CONSUME, emKADDITEM_NONE);

			nLeft = 0;
		}
	}

	return	nLeft;		// 返回 请求消费道具数目 - 实际消费数目
}

BOOL KPlayerItem::PayCoin(INT nPayNum, INT nItemIndex /*= 1*/)
{
	//if (GetCashCoin(nItemIndex) >= nPayNum)
	//{
	//	INT nPayBindNum = 0;
	//	std::vector<INT> vctCoinItemParam = g_cMiscMgr.m_cGlbVal.m_mapCoinItemParam[nItemIndex];
	//	INT nRet = ConsumeItemInBagsByBindOrder( //返回结果为0时,表示消耗成功
	//		nPayNum,
	//		&nPayBindNum,
	//		vctCoinItemParam[0],
	//		vctCoinItemParam[1],
	//		vctCoinItemParam[2],
	//		vctCoinItemParam[3]);
	//	
	//	if (!nRet)
	//	{
	//		// 发消息给玩家
	//		SHOW_MSG_SYNC sMsg;
	//		sMsg.ProtocolType = s2c_msgshow;
	//		sMsg.m_wMsgID = enumMSG_ID_TRADE_COSUMEINFO;

	//		// -------------------------------------------------------------------------
	//		// 占用32位：genre-8位，detailtype/particular-12位
	//		sMsg.adwParam[0] = MAKE_ITEM_TYPEID(vctCoinItemParam[0], vctCoinItemParam[1], vctCoinItemParam[2]);
	//		sMsg.adwParam[1] = vctCoinItemParam[3]; //nLevel
	//		*((LPDWORD)sMsg.szParam + 2) = (DWORD)nPayBindNum;			//绑定物品的消耗数量
	//		*((LPDWORD)sMsg.szParam + 3) = (DWORD)(nPayNum - nPayBindNum);  //非绑定物品的消耗数量
	//		//sMsg.adwParam[1] = nPayBindNum | ((WORD)(nPayNum - nPayBindNum) << 16);//低位保存绑定物品的消耗数量,高位保存非绑定物品的消耗数量
	//		sMsg.m_wLength = (sMsg.MainSize() + sizeof(DWORD) * 4) - 1;
	//		//g_pCoreServer->SendData(m_rcPlayer.m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	//		g_cItemMgr.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), &sMsg);
	//	}

	//	return !nRet;
	//}
	return FALSE;
}

BOOL KPlayerItem::PayValueCoin(INT nPayNum, INT nValueCoinIndex)
{
	BOOL bRet = FALSE;
	//do 
	//{
	//	INT nCurValueCoin = GetValueCoin(nValueCoinIndex);
	//	if (nCurValueCoin < nPayNum)
	//		break;

	//	KGlobalValue::KPARAM_VALUECOIN sValueCoinParam = g_cMiscMgr.m_cGlbVal.m_mapValueCoinParam[nValueCoinIndex];

	//	m_rcPlayer.m_cPlayerTask.SetTaskValue(MAKELONG(sValueCoinParam.nTaskId, sValueCoinParam.nTaskGroup),
	//		nCurValueCoin - nPayNum);
	//	bRet = TRUE;
	//} while (0);
	return bRet;
}

#endif //#ifdef GAME_SERVER

//INT KPlayerItem::GetCashCoin(INT nCoinItemIndex)
//{
//	if (nCoinItemIndex == -1)
//	{
//		return KPlayerStall::INVALIDPRICE;
//	}
//
//	std::vector<INT> vctCoinItemParam = g_cMiscMgr.m_cGlbVal.m_mapCoinItemParam[nCoinItemIndex];
//	return GetItemCountInBags(
//		vctCoinItemParam[0],
//		vctCoinItemParam[1],
//		vctCoinItemParam[2],
//		vctCoinItemParam[3]
//		);
//}

//INT KPlayerItem::GetValueCoin(INT nValueCoinIndex)
//{
//	if (nValueCoinIndex == -1)
//	{
//		return KPlayerStall::INVALIDPRICE;
//	}
//
//	KGlobalValue::KPARAM_VALUECOIN sValueCoinParam = g_cMiscMgr.m_cGlbVal.m_mapValueCoinParam[nValueCoinIndex];
//	return m_rcPlayer.m_cPlayerTask.GetTaskValue(MAKELONG(sValueCoinParam.nTaskId, sValueCoinParam.nTaskGroup));
//}

#ifdef GAME_SERVER

INT KPlayerItem::Add_AutoStack(KE_ITEM_ROOM eRoom, INT nItemIdx, KE_ADD_ITEM_WAY eWay, BOOL bStack /*= TRUE*/)
{
	if (eRoom < 0 || eRoom >= emROOM_NUM || nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		return	0;

	KItemRoom& rcRoom = m_acRoom[eRoom];
	INT nX = 0, nY = 0;

	if (!Item[nItemIdx].IsStackable() || !bStack)
	{	// 如果不能叠放则寻找空格放置
		if (!rcRoom.GetFreeCell(nX, nY))
			return	0;
		if (Add(nItemIdx, eRoom, eWay, nX, nY) <= 0)
			return	0;
		return	nItemIdx;
	}

	KItemRoom::KFinder cFind = rcRoom.GetFinder();
	while (cFind.Next(KRoomFindCondition::StackWith(Item[nItemIdx])))
	{	// 叠放成功且nItemIdx被置0，表明新物品已被完全叠放到现有物品并自动删除了，没有产生新物品，自动叠放完成
		if (StackItem(nItemIdx, cFind.Index(), eWay) && !nItemIdx)
			return	cFind.Index();
	}

	if (rcRoom.GetFreeCell(nX, nY) && Add(nItemIdx, eRoom, eWay, nX, nY) > 0)
		return	nItemIdx;

	return	0;
}

INT KPlayerItem::FindAndStackInBag(INT nItemIdx, KE_ADD_ITEM_WAY eWay, INT nItemCount/*= 1*/)
{
	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		KItemRoom& rcRoom = m_acRoom[g_aeBagRoom[i]];
		INT nX = 0, nY = 0;
		KItemRoom::KFinder cFind = rcRoom.GetFinder();
		while (cFind.Next(KRoomFindCondition::StackWith(Item[nItemIdx])))
		{	// 叠放成功且nItemIdx被置0，表明新物品已被完全叠放到现有物品并自动删除了，没有产生新物品，自动叠放完成
			if (StackItem(nItemIdx, cFind.Index(), eWay, nItemCount) && !nItemIdx)
			{
				return	cFind.Index();
			}
		}
	}
	return 0;
}

INT KPlayerItem::FindStackRoom(INT nItemIdx, KE_ADD_ITEM_WAY eWay, INT nGenre)
{
	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		KItemRoom& rcRoom = m_acRoom[g_aeBagRoom[i]];
		INT nX = 0, nY = 0;
		KItemRoom::KFinder cFind = rcRoom.GetFinder();
		while (cFind.Next(KRoomFindCondition::StackWith(Item[nItemIdx])))
		{
			return cFind.Index();
		}
	}

	return FALSE;
}

VOID KPlayerItem::Abrade(VOID)
{
	//if (g_cMiscMgr.m_cGlbVal.m_nItemAbradeType == KGlobalValue::type_abrade_noabrade)
	//{
	//	m_bAbrade = FALSE;
	//	return;
	//}

	m_bAbrade = TRUE;
}

BOOL KPlayerItem::CompareItems(CONST KINDEX_SET& lhs, CONST KINDEX_SET& rhs)
{
	if (lhs.size() != rhs.size())
		return FALSE;
	KINDEX_SET::const_iterator it1 = lhs.begin();
	KINDEX_SET::const_iterator it2 = rhs.begin();
	while (it1 != lhs.end())
	{
		if (*it1++ != *it2++)
			return FALSE;
	}
	return TRUE;
}

BOOL KPlayerItem::CheckRoomItems(KE_ITEM_ROOM eRoom)
{
	KINDEX_SET setItemIndex;
	KItemRoom *pRoom = &m_acRoom[eRoom];
	KItemRoom::KFinder cFind = pRoom->GetFinder();
	KPlayer* pSelf = &m_rcPlayer;
	while (cFind.Next(KRoomFindCondition::Used))
	{
		KItem* pObj = &Item[cFind.Index()];

		if (setItemIndex.find(cFind.Index()) != setItemIndex.end())	// 在Item数组中重复索引的物品
		{
			_ASSERT(FALSE);
			//g_SysAlarmLogFile.LogRecordVar(LOG_LEVEL_ERROR,
			//	"[ITEM]\t%s(%s) Find duplication item[%s](RandSeed:%d, GenTime:%u)!",
			//	pSelf->GetAccount(),
			//	pSelf->GetPlayerName(),
			//	pObj->GetName(),
			//	pObj->GetRandSeed(),
			//	pObj->GetGenTime()
			//);
			return FALSE;
		}

		if (pObj->GetBelong() != m_rcPlayer.GetPlayerIndex())				// 错误的物品拥有者
		{
			_ASSERT(FALSE);
			//g_SysAlarmLogFile.LogRecordVar(LOG_LEVEL_ERROR,
			//	"[ITEM]\t%s(%s) Find misowned item[%s](RandSeed:%d, GenTime:%u)!",
			//	pSelf->GetAccount(),
			//	pSelf->GetPlayerName(),
			//	pObj->GetName(),
			//	pObj->GetRandSeed(),
			//	pObj->GetGenTime()
			//);
			return FALSE;
		}

		setItemIndex.insert(cFind.Index());
	}
	return TRUE;
}

#endif

BOOL KPlayerItem::SetItemStackCount(INT nItemGameIdx, INT nCount, INT eRecordWay, KE_LOSE_ITEM_WAY eWay1, KE_ADD_ITEM_WAY eWay2)
{
	if (nItemGameIdx <= 0 || nItemGameIdx >= MAX_ITEM || Item[nItemGameIdx].GetMaxCount() <= 1 ||
		nCount < 0 || nCount > (INT)Item[nItemGameIdx].GetMaxCount())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CHAR szKey[32];
	::sprintf(szKey, "%d_%d_%d", Item[nItemGameIdx].GetGenre(), Item[nItemGameIdx].GetDetailType(), 
		Item[nItemGameIdx].GetParticular());

	if (nCount > 0)
	{
		INT nPreCount = Item[nItemGameIdx].GetCount();
		BOOL bOK = Item[nItemGameIdx].SetCount(nCount);
		if (!bOK)
			return FALSE;

#ifdef GAME_SERVER
		// 设置成功，判断要不要记录LOG
		INT nDelValue = Item[nItemGameIdx].GetSingleValue() * (nPreCount - nCount);
		if (nPreCount > nCount && eWay1 != emKLOSEITEM_NONE)			// 扣除物品的LOG
		{
			//// 给玩家行为日志上也加上价值量，这样就能知道个数了
			//KD_COURSE_LOG(m_rcPlayer, emKCOURSE_PLAYER_REMOVEITEM, "RemoveItem\t" << Item[nItemGameIdx].GetName()
			//	<< "[" <<  Item[nItemGameIdx].GetGuidString() << "]"<< "(" << Item[nItemGameIdx].GetGenTime() << ")\tWay"
			//	<< (INT)eWay1 << "\tValue" << nDelValue);

			//// 失去物品log
			//// 价值量变化大于50000，记录到物品流向LOG
			//if (nDelValue >= 50000)
			//{
			//	CHAR szValue[256];
			//	sprintf(szValue, "%d",eWay1);
			//	KLogWritter* pLog = KLogWritter::GetInstance();
			//	pLog->Goddess_ItemLog(
			//		m_rcPlayer.GetAccount(),
			//		m_rcPlayer.GetPlayerName(),
			//		&Item[nItemGameIdx],
			//		FALSE,
			//		emKITEMLOG_TYPE_REMOVEITEM,
			//		szValue,
			//		nDelValue);			
			//}

			//// 调用脚本，做数据进点数据的记录
			//if( (eWay1 == emKLOSEITEM_SELL || eWay1 == emKLOSEITEM_USE || eWay1 == emKLOSEITEM_THROW || eWay1 == emKLOSEITEM_TIMEOUT || 
			//	eWay1 == emKLOSEITEM_CONSUME || eWay1 == emKLOSEITEM_SCRIPT_EVENTUSE || eWay1 == emKLOSEITEM_SCRIPT_TASKUSE || 
			//	eWay1 == emKLOSEITEM_SCRIPT_BREAKUP || eWay1 == emKLOSEITEM_SCRIPT_CHANGE_HUN || emKLOSEITEM_SCRIPT_REPAIR ||
			//	eWay1 == emKLOSEITEM_SCRIPT_SIGNET || eWay1 == emKLOSEITEM_SCRIPT_KILLER || eWay1 == emKLOSEITEM_SCRIPT_MANTLE_SHOP || 
			//	eWay1 == emKLOSEITEM_SCRIPT_PARTNER_TALENT ||  eWay1 == emKLOSEITEM_SCRIPT) && 
			//	(g_cItemMgr.m_mpDataRecordList.find(szKey) != g_cItemMgr.m_mpDataRecordList.end() && eRecordWay != emITEM_DATARECORD_NORMAL)
			//	)
			//{
			//	KScriptFunctionCallHelper sfcHelper(*g_pMainScript);
			//	sfcHelper.SetIt(&Item[nItemGameIdx]);

			//	g_pMainScript->CallTableFunction("Item",
			//		"OnItemRemove",
			//		0, 
			//		"dd",
			//		nPreCount - nCount,
			//		eRecordWay);
			//}
		}	
		else if (nPreCount < nCount  && eWay2 != emKADDITEM_NONE)	 // 添加物品的LOG
		{
			//INT nAddValue = -nDelValue;
			//// 给玩家行为日志上也加上价值量，这样就能知道个数了
			//KD_COURSE_LOG(m_rcPlayer, emKCOURSE_PLAYER_ADDITEM, "AddItem\t" << Item[nItemGameIdx].GetName() 
			//	<< "[" <<  Item[nItemGameIdx].GetGuidString() << "]" << "(" << Item[nItemGameIdx].GetGenTime() << ")\tWay" 
			//	<< (INT)eWay2 << "\tValue" << nAddValue);
			//
			//// 价值量变化大于50000，记录到物品流向LOG
			//if (nAddValue >= 50000)		
			//{
			//	CHAR szValue[256];
			//	sprintf(szValue, "%d",eWay2);
			//	KLogWritter* pLog = KLogWritter::GetInstance();
			//	pLog->Goddess_ItemLog(
			//		m_rcPlayer.GetAccount(),
			//		m_rcPlayer.GetPlayerName(),
			//		&Item[nItemGameIdx],
			//		FALSE,
			//		emKITEMLOG_TYPE_ADDITEM,
			//		szValue, 
			//		nAddValue);	
			//}

			//// 调用脚本，做数据进点数据的记录
			//if ((eWay2 == emKADDITEM_BUY || eWay2 == emKADDITEM_IBSHOP || eWay2 == emKADDITEM_IBCOIN || eWay2 == emKADDITEM_TEAMSHARE ||
			//	eWay2 == emKADDITEM_BYSCRIPT_PRODUCE || eWay2 == emKADDITEM_BYSCRIPT_FINISHTASK || eWay2 == emKADDITEM_BYSCRIPT) &&
			//	(eRecordWay != emITEM_DATARECORD_NORMAL && g_cItemMgr.m_mpDataRecordList.find(szKey) != g_cItemMgr.m_mpDataRecordList.end()))
			//{
			//	KScriptFunctionCallHelper sfcHelper(*g_pMainScript);
			//	sfcHelper.SetIt(&Item[nItemGameIdx]);

			//	g_pMainScript->CallTableFunction("Item",
			//		"OnItemAdd",
			//		0, 
			//		"dd",
			//		nCount - nPreCount,
			//		eRecordWay);
			//}
		}

		KITEM_COUNT_SYNC sSync;
		//sSync.cProtocol	= (PROTOCOL_MSG_TYPE)s2c_itemcountsync;
		sSync.dwId		= Item[nItemGameIdx].GetID();
		sSync.wCount	= (WORD)nCount;

		g_cItemMgr.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
#endif
	}
	else
	{
		if (!Remove(nItemGameIdx, emKLOSEITEM_SET_STACK))
		{
			_ASSERT(FALSE);
			return FALSE;
		}
		/*
		#ifdef GAME_SERVER
		// 日志：异常情况？wangbin 2008-03-20
		KLogWritter::GetInstance()->ItemHistory_RemoveItem(
		&Item[nItemGameIdx],
		&m_rcPlayer,
		emKLOG_REASON_REMOVEITEM_ZEROCOUNT);
		#endif
		*/
		g_cItemMgr.Remove(nItemGameIdx, emITEM_DATARECORD_NORMAL, emITEM_CONSUMEMODE_ERRORLOST_STACK, &m_rcPlayer);
	}

	return TRUE;
}

KItem* KPlayerItem::GetItemFromItemPos(CONST KITEM_POS* pPos, KPlayer* pPlayer)
{
	if (!pPlayer)
		return 0;

	KE_ITEM_ROOM eRoom = pPos->eRoom;
	KPlayerItem *pList = pPlayer->GetCtrl<KPlayerItem>();

	if (eRoom < 0 || eRoom >= emROOM_NUM)
		return	NULL;

	INT nItemIdx = pList->m_acRoom[eRoom].GetCell(pPos->nX, pPos->nY);
	if (nItemIdx <= 0)
		return	NULL;

	return	&Item[nItemIdx];
}

BOOL KPlayerItem::CanAddItemIntoBag(CONST KITEM_INDEX& rsIndex, BOOL bBind /* = FALSE */,
									INT nCount /* = 1 */, BOOL bTimeOut /*= = FALSE*/) CONST
{
	KHOLD_ITEM sHold;
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	CONST KBASICPROP_BASEDATA* pBPTBase = (CONST KBASICPROP_BASEDATA*)pLibBPT->GetBasicProp(rsIndex);
	if (pBPTBase)
		bBind |= KItem::IsBindByBindType((KE_ITEM_BINDTYPE)pBPTBase->nBindType) ;
	sHold.sIndex  = rsIndex;
	sHold.bBind   = bBind;
	sHold.nCount  = nCount;
	sHold.bTimeOut = bTimeOut;

	KHoldItemList lstHold;
	lstHold.push_back(sHold);
	return	CanAddItemIntoBag(lstHold);
}

BOOL KPlayerItem::CanAddItemIntoBag(CONST KItem& rcItem) CONST
{
	KItemList lstItem;
	lstItem.push_back(&rcItem);

	return	CanAddItemIntoBag(lstItem);
}

BOOL KPlayerItem::CanAddItemIntoBag(CONST KItemList& rlstItem) CONST
{
	KHoldItemList lstHoldItem;
	for (KItemList::const_iterator it = rlstItem.begin(); it != rlstItem.end(); ++it)
	{
		const KItem* pcItem = *it;
		if (!pcItem)
		{
			ASSERT(FALSE);
			return	FALSE;
		}

		KHOLD_ITEM sHold;
		ZeroStruct(sHold);
		sHold.sIndex	= pcItem->GetIndex();
		sHold.nCount	= pcItem->GetCount();
		sHold.bBind		= (pcItem->IsBind() || KItem::IsBindByBindType((KE_ITEM_BINDTYPE)pcItem->GetBindType()));
		sHold.bTimeOut  = pcItem->GetTimeout()->dwTimeout > 0 ? TRUE : FALSE;
		lstHoldItem.push_back(sHold);
	}

	return	CanAddItemIntoBag(lstHoldItem);
}

BOOL KPlayerItem::CanAddItemIntoBag(CONST KHoldItemList& rlstHoldItem) CONST
{
	INT nFreeCell = CountBagFreeCell();
	KHoldItemList lstVirtual;

	for (KHoldItemList::const_iterator it = rlstHoldItem.begin(); it != rlstHoldItem.end(); ++it)
	{
		CONST KHOLD_ITEM& rsHold = *it;
		CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
		CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
		CONST KBASICPROP_BASEDATA* psBase = pLibBPT->GetBasicProp(rsHold.sIndex);

		if (!psBase)
		{	// 无效的道具类型索引
			ASSERT(FALSE);
			return	FALSE;
		}

		if (KD_ISEQUIP(psBase->nItemGenre) || rsHold.bTimeOut)
		{	// 装备一定不可叠加, 会超时物一定不可叠加
			nFreeCell -= rsHold.nCount;
		}
		else
		{	// 非装备，检查叠加类型
			KBASICPROP_OTHER* psOther = (KBASICPROP_OTHER*)psBase;
			if (psOther->nStackMax > 1)		// 叠加道具
				CheckHoldStatckItemInBags(rsHold, psOther->nStackMax, lstVirtual, nFreeCell);
			else							// 非叠加道具
				nFreeCell -= rsHold.nCount;
		}

		if (nFreeCell < 0)
			return	FALSE;					// 放不下了
	}

	return	TRUE;
}

VOID KPlayerItem::CheckHoldStatckItemInBags(const KHOLD_ITEM& rsHold, INT nStackMax, KHoldItemList& rlstVirtual, INT& rnFreeCell) CONST
{
	typedef std::list<KHOLD_ITEM*> KFoundList;

	KFoundList lstFound;
	INT nCurCount = rsHold.nCount;
	if (nCurCount <= 0)
		return;

	for (KHoldItemList::iterator it = rlstVirtual.begin(); it != rlstVirtual.end(); ++it)
	{	// 在rvecVirtual中找
		KHOLD_ITEM& rsVirtual = *it;
		if (rsVirtual.sIndex == rsHold.sIndex && rsVirtual.bBind == rsHold.bBind)
			lstFound.push_back(&rsVirtual);
	}

	if (!lstFound.size())
	{	// rvecVirtual中没有，则到背包里找
		for (INT i = 0; i < countof(g_aeBagRoom); ++i)
		{
			const KItemRoom& rcRoom = m_acRoom[g_aeBagRoom[i]];
			KItemRoom::KFinder cFind = rcRoom.GetFinder();
			KRoomFindCondition::StackWith cCond(rsHold.sIndex, rsHold.bBind);
			while (cFind.Next(cCond))
			{
				KItem& rcItem = Item[cFind.Index()];
				if (rcItem.GetCount() >= rcItem.GetMaxCount())
					continue;							// 已经满了的道具就不要了

				KHOLD_ITEM sVirtual;
				sVirtual.sIndex		= rcItem.GetIndex();
				sVirtual.nCount		= rcItem.GetCount();
				rlstVirtual.push_back(sVirtual);		// 注册到rvecVirtual
				lstFound.push_back(&rlstVirtual.back());
			}
		}
	}

	for (KFoundList::const_iterator it = lstFound.begin(); it != lstFound.end(); ++it)
	{	// 看看rvecVirtual中能不能放下
		KHOLD_ITEM* psVirtual = *it;
		ASSERT(psVirtual);
		INT nCap = nStackMax - psVirtual->nCount;

		if (nCap >= nCurCount)
		{	// 放得下
			psVirtual->nCount += nCurCount;
			return;
		}

		// 放不下
		nCurCount -= nStackMax - psVirtual->nCount;
		psVirtual->nCount = nStackMax;
	}

	// 依然有放不下的道具
	INT nNeedCell = nCurCount / nStackMax;	// 计算需要占用的格子总数
	rnFreeCell -= nNeedCell;
	if (nCurCount % nStackMax > 0)
	{
		// 将新占用的格子注册到rvecVirtual
		KHOLD_ITEM sVirtual = rsHold;
		sVirtual.nCount = nCurCount % nStackMax;			// 忽略掉已满的道具
		rlstVirtual.push_back(sVirtual);
		rnFreeCell--;
	}
}

INT KPlayerItem::GetItemCount(KE_ITEM_ROOM eRoom, CONST KITEM_INDEX &sIndex, 
							  INT nBind /*= KD_ITEM_NO_BIND_LIMIT*/,
							  INT nTimeOut /*= KD_ITEM_NO_TIMEOUT_LIMIT*/)
{
	ASSERT(eRoom >= 0 && eRoom < emROOM_NUM);
	INT nCount = 0;
	KItemRoom* pInv = &m_acRoom[eRoom];
	KItemRoom::KFinder cFind = pInv->GetFinder();
	KRoomFindCondition::SameIndexSeries cCond(sIndex, nBind);
	while (cFind.Next(cCond))
	{
		INT nIdx = cFind.Index();
		if (nTimeOut == KD_ITEM_NO_TIMEOUT_LIMIT ||
			(Item[nIdx].GetTimeout()->dwTimeout > 0 && nTimeOut) ||
			(Item[nIdx].GetTimeout()->dwTimeout == 0 && !nTimeOut))
			nCount += Item[nIdx].GetCount();
	}
	return nCount;
}

INT KPlayerItem::GetItemCountInBags(CONST KITEM_INDEX &sIndex, INT nBind /*= KD_ITEM_NO_BIND_LIMIT*/)
{
	INT nCount = 0;

	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		KE_ITEM_ROOM eRoom = g_aeBagRoom[i];
		KItemRoom::KFinder cFind = m_acRoom[eRoom].GetFinder();
		KRoomFindCondition::SameIndexSeries cCond(sIndex, nBind);
		while (cFind.Next(cCond))
			nCount += Item[cFind.Index()].GetCount();
	}

	return nCount;
}

INT KPlayerItem::GetItemCount(KE_ITEM_ROOM	eRoom,
							  INT 			nGenre,				// = KD_ITEM_INVALID_TYPE
							  INT 			nDetail,			// = KD_ITEM_INVALID_TYPE
							  INT 			nParticular,		// = KD_ITEM_INVALID_TYPE
							  INT				nBind,				// = KD_ITEM_NO_BIND_LIMIT
							  INT				nTimeOut			// = KD_ITEM_NO_TIMEOUT_LIMIT
							  )
{
	KITEM_INDEX sIndex;
	sIndex.nGenre		= nGenre;
	sIndex.nDetailType	= nDetail;
	sIndex.nParticular	= nParticular;
	sIndex.nLevel = 0;
	return	GetItemCount(eRoom, sIndex, nBind, nTimeOut);
}

INT KPlayerItem::GetItemCountInBags(INT nGenre,			// = KD_ITEM_INVALID_TYPE
									INT nDetail,		// = KD_ITEM_INVALID_TYPE
									INT nParticular,	// = KD_ITEM_INVALID_TYPE
									INT nBind			// = KD_ITEM_NO_BIND_LIMIT
									)
{
	KITEM_INDEX sIndex;
	sIndex.nGenre		= nGenre;
	sIndex.nDetailType	= nDetail;
	sIndex.nParticular	= nParticular;
	sIndex.nLevel = 0;
	return	GetItemCountInBags(sIndex, nBind);
}

BOOL KPlayerItem::FindRoom(KE_ITEM_ROOM eRoom, KItem* pItem, KPos &sPos, INT &nIdx)
{	// TODO: xyf 这个函数似乎有点毛病，最好避免这种用法
	ASSERT(pItem && eRoom >= 0 && eRoom < emROOM_NUM);
	KItemRoom *pRoom = &m_acRoom[eRoom];
	KItemRoom::KFinder cFind = pRoom->GetFinder();

	if (pItem->IsStackable())
	{
		KRoomFindCondition::StackWith cCond(*pItem);
		while (cFind.Next(cCond))
		{
			nIdx = cFind.Index();
			sPos = cFind.Pos();
			return TRUE;
		}
	}

	nIdx = 0;
	return	pRoom->GetFreeCell(sPos.nX, sPos.nY);
}

BOOL KPlayerItem::CheckRoom(KE_ITEM_ROOM eRoom, KItem* pItem, INT nCount)
{
	ASSERT(eRoom >= 0 && eRoom < emROOM_NUM && nCount > 0);
	KItemRoom *pRoom = &m_acRoom[eRoom];

	if (nCount <= pRoom->CountFree())
		return	TRUE;	// 只要剩余格子够就一定能放下

	if (!pItem->IsStackable())
		return	FALSE;	// 非叠加物品，放不下

	KItemRoom::KFinder cFind = pRoom->GetFinder();

	if (cFind.Next(KRoomFindCondition::StackWith(*pItem)))
	{	// 先看有没有可以与之叠加的物品
		KItem* pStackItem = &Item[cFind.Index()];
		nCount -= pStackItem->GetMaxCount() - pStackItem->GetCount();
		if (nCount <= 0)
			return TRUE;
	}

	while (cFind.Next(KRoomFindCondition::Free))
	{	// 还有剩的，找空位放试试
		nCount -= pItem->GetMaxCount();
		if (nCount <= 0)
			return TRUE;
	}

	return FALSE;
}

VOID KPlayerItem::CalcActiveMA(VOID)
{
	/*
	for (INT i = 0; i < emEQUIPPOS_NUM; ++i)		// 加上同伴装备
	{
		INT nIdx = (i >= emEQUIPPOS_NUM) ? GetPartnerEquip(i - emEQUIPPOS_NUM) : GetEquip(i);
		if (nIdx <= 0)
			continue;

		KItem* pItem = &Item[nIdx];

		// 激活判断
		if (m_sActiveSpState.bActiveOrnament > 0  && KD_ISORNAMENT(KPlayerItem::GetEquipPos(pItem->GetDetailType())))
			pItem->m_massRandom.UpdateState(m_rcPlayer.GetPlayerIndex(), TRUE);
		else
			pItem->m_massRandom.UpdateState(m_rcPlayer.GetPlayerIndex());
		for (KMassSuiteVec::const_iterator it = m_vecGoldMASS.begin(); it < m_vecGoldMASS.end(); ++it)
			(*it)->UpdateState(m_rcPlayer.GetPlayerIndex());
		for (KMassSuiteVec::const_iterator it = m_vecGreenMASS.begin(); it < m_vecGreenMASS.end(); ++it)
			(*it)->UpdateState(m_rcPlayer.GetPlayerIndex());

		// 应用新的激活属性
		ApplyActiveMA(&pItem->m_massRandom);

	}

	for (KMassSuiteVec::const_iterator it = m_vecGoldMASS.begin(); it < m_vecGoldMASS.end(); ++it)
		ApplyActiveMA(*it);
	for (KMassSuiteVec::const_iterator it = m_vecGreenMASS.begin(); it < m_vecGreenMASS.end(); ++it)
		ApplyActiveMA(*it);
	*/
}

// 获取道具限制使用的规则组合
// 返回的是该种类型道具检查的优先级最高的规则组合
BOOL KPlayerItem::GetLimitUseRule(LPCSTR szItemForbitType, vecLimitUseRule& rcRule)
{
	LPCSTR aszItemClass[2] = {szItemForbitType, "ALL_ITEM"};
	for (INT i = emKLIMIT_PRIORITY_count -1; i >= emKLIMIT_PRIORITY_forbid; --i)
	{
		if (g_cItemMgr.m_mpLimitPri.find((KE_LIMIT_PRIORITY)i) != g_cItemMgr.m_mpLimitPri.end())
		{
			vecLimitUseRule vecRule = g_cItemMgr.m_mpLimitPri.find((KE_LIMIT_PRIORITY)i)->second;
			for (INT j = 0; j < 2; ++j)
			{
				std::map<std::string, KItemMgr::KMP_LIMITUSE>::iterator it = g_cItemMgr.m_mpLimitUseData.find(aszItemClass[j]);
				if (it == g_cItemMgr.m_mpLimitUseData.end())
					continue;

				KItemMgr::KMP_LIMITUSE mpLimitUse = it->second;

				for (vecLimitUseRule::iterator ite = vecRule.begin(); ite != vecRule.end(); ++ite)
				{
					if (mpLimitUse.find(*ite) != mpLimitUse.end())
					{
						rcRule.push_back(*ite);
					}					
				}		
			}
		}

		if (rcRule.size() > 0 )
			break;
	}

	return TRUE;
}

BOOL KPlayerItem::CheckUseLimited(LPCSTR szItemForbitType, DWORD dwMapId, CHAR* szRet /* = NULL */)
{
	BOOL bRet = FALSE;
	vecLimitUseRule sRule;
	PROCESS_ERROR(GetLimitUseRule(szItemForbitType, sRule));

	{
		std::vector<KE_LIMITUSE_TYPE>::iterator it = sRule.begin();
		for (;it != sRule.end(); ++it)
		{
			CHECKCALLBACK pFun = m_apfnLimitUseCheck[*it];
			if (pFun)
			{
				PROCESS_ERROR((*pFun)(szItemForbitType, dwMapId, szRet));
			}
		}
		bRet = TRUE;
	}
EXIT0:
	return bRet;	
}

// 地图使用限制检查回调：FALSE表示不能使用，TRUE表示能使用
BOOL KPlayerItem::CheckMapForbid(LPCSTR szItemForbitType, DWORD dwMapId, CHAR* szRet)
{
	//	CHAR szMapType[64] = {0};
	//#ifdef GAME_SERVER
	//	// 服务端把可能的动态地图ID转为模板ID
	//	INT	nWorldIndex = g_SubWorldSet.SearchWorld(dwMapId);
	//	if (nWorldIndex < MAX_SUBWORLD && nWorldIndex >= 0)
	//		dwMapId = SubWorld[nWorldIndex].m_MapTemplateID;
	//#endif
	//	if (!g_SubWorldSet.GetMapType(dwMapId, szMapType))
	//		return FALSE;
	//
	BOOL bRet = TRUE;
	//	LPCSTR aszItemClass[2] = {szItemForbitType, "ALL_ITEM"};
	//	for (INT i = 0; i < 2; ++i)
	//	{
	//		std::map<std::string, KItemMgr::KMP_LIMITUSE>::iterator it = g_cItemMgr.m_mpLimitUseData.find(aszItemClass[i]);
	//		if (it == g_cItemMgr.m_mpLimitUseData.end())
	//			continue;
	//
	//		KItemMgr::KMP_LIMITUSE mpLimitUse = it->second;
	//		KItemMgr::KMP_LIMITUSE::iterator it2 = mpLimitUse.find(emKLIMITUSE_MAPFORBID);
	//		if (it2 == mpLimitUse.end())
	//			continue;
	//
	//		KItemMgr::KMP_DATA mpLimitData = it2->second;
	//		KItemMgr::KMP_DATA::iterator it3 = mpLimitData.find(szMapType);
	//		if (it3 != mpLimitData.end())
	//		{
	//			bRet = FALSE;
	//			if (szRet)
	//				strcpy(szRet, G_ItemLimiUse_Map);
	//			break;
	//		}
	//	}

	return bRet;
}

// 地图专属道具检查回调：FALSE表示不能使用，TRUE表示能使用
// ALL_ITEM不能成为地图专属道具，所以这个接口里不需要做ALL_ITEM检查
BOOL KPlayerItem::CheckMapProperUse(LPCSTR szItemForbitType, DWORD dwMapId, CHAR* szRet)
{
	//	std::map<std::string, KItemMgr::KMP_LIMITUSE>::iterator it = g_cItemMgr.m_mpLimitUseData.find(szItemForbitType);
	//	if (it == g_cItemMgr.m_mpLimitUseData.end())
	//		return TRUE;
	//
	//	KItemMgr::KMP_LIMITUSE mpLimitUse = it->second;
	//	KItemMgr::KMP_LIMITUSE::iterator it2 = mpLimitUse.find(emKLIMITUSE_MAPPROPER);
	//	if (it2 == mpLimitUse.end())
	//		return TRUE;
	//
	//	CHAR szMapType[64] = {0};
	//#ifdef GAME_SERVER
	//	// 服务端把可能的动态地图ID转为模板ID
	//	INT	nWorldIndex = g_SubWorldSet.SearchWorld(dwMapId);
	//	if (nWorldIndex < MAX_SUBWORLD && nWorldIndex >= 0)
	//		dwMapId = SubWorld[nWorldIndex].m_MapTemplateID;
	//#endif
	//	if (!g_SubWorldSet.GetMapType(dwMapId, szMapType))
	//		return FALSE;
	//
	//	KItemMgr::KMP_DATA mpLimitData = it2->second;
	//	// 是专属于本地图的才能全用，因为一种道具只能专属于一种地图，所以这里加个size为1的判断
	//	if (mpLimitData.size() == 1 && mpLimitData.find(szMapType) != mpLimitData.end())
	//		return TRUE;
	//
	//	//strcpy(szRet, "该道具禁止在本地图使用");
	//	if (szRet)
	//		strcpy(szRet, G_ItemLimiUse_Map);
	return FALSE;
}

VOID KPlayerItem::UpdateEquipInvalid(BOOL bForce /* = FALSE */)
{
	//KNpc* pNpc = &Npc[m_rcPlayer.m_nIndex];
	BOOL aryInvalid[emEQUIPPOS_NUM];
	INT32 bmChanged = 0;	// 记录失效状态是否有改变的位图，其位意义对应KE_EQUIP_POSITION枚举，注意最大只有32位

	for (INT i = 0; i < emEQUIPPOS_NUM; i++)
	{
		if (GetEquip(i) <= 0)
			continue;

		KItem* pItem = &Item[GetEquip(i)];
		aryInvalid[i] = TestInvalid(*pItem);	// 失效判断

		if (pItem->IsInvalid() != aryInvalid[i])
		{
			bmChanged |= 1 << i;				// 失效状态改变
			if (aryInvalid[i])
			{	// 从有效变为失效
				RemoveNpcRes(i);
				// todo [7/13/2012 simon]
				//if (i == emEQUIPPOS_HORSE)
				//	m_rcPlayer.DoRideHorse(FALSE);
			}
			else
			{	// 从失效变为有效
				ApplyNpcRes(i);
			}
		}
	}

	if (!bmChanged && !bForce)			// 失效状态都没有变，返回
		return;

	for (INT i = 0; i < emEQUIPPOS_NUM; ++i)
	{	// 逐一更新身上装备的有效状态
		INT nItem = GetEquip(i);
		if (nItem <= 0)
			continue;
		RemoveBaseAttrib(i);
		Item[nItem].SetInvalid(aryInvalid[i]);
		ApplyBaseAttrib(i);
	}

	RefreshSuiteState();				// 刷新套装持有信息
	CalcActiveMA();						// 重新应用激活魔法属性

	// todo [7/13/2012 simon]
	//if (bmChanged & (1 << emEQUIPPOS_WEAPON))	// 如果武器失效状态改变
	//	pNpc->m_SkillList.UpdateWeaponSkill();
}

VOID KPlayerItem::ApplyAppendAttrib(INT nEquipPos)
{
	INT nIdx = GetEquip(nEquipPos);
	if (nIdx <= 0)
		return;

	KItem* pItem = &Item[nIdx];

	if (pItem->IsInvalid())
		return;

	KCharacter * pPlayer = (KCharacter*)g_cPlayerMgr.GetByIndex(m_rcPlayer.GetPlayerIndex());
	if(!pPlayer) 
		return;

	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		if(pItem->m_aryAppendAttrib[i].nAttribType == 0)
			break;
		g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, pItem->m_aryAppendAttrib[i], FALSE);
	}
}

VOID KPlayerItem::RemoveAppendAttrib(INT nEquipPos)
{
	INT nIdx = GetEquip(nEquipPos);
	if (nIdx <= 0)
		return;

	KItem* pItem = &Item[nIdx];

	if (pItem->IsInvalid())
		return;

	KCharacter * pPlayer = (KCharacter*)g_cPlayerMgr.GetByIndex(m_rcPlayer.GetPlayerIndex());
	if(!pPlayer) 
		return;

	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		if(pItem->m_aryAppendAttrib[i].nAttribType == 0)
			break;
		g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, pItem->m_aryAppendAttrib[i], TRUE);
	}
}

VOID KPlayerItem::ApplyBaseAttrib(INT nEquipPos)
{
	INT nIdx = GetEquip(nEquipPos);
	if (nIdx <= 0)
		return;

	KItem* pItem = &Item[nIdx];
	//KNpc* pNpc   = &Npc[m_rcPlayer.m_nIndex];

	if (pItem->IsInvalid())
		return;
	KCharacter * pPlayer = (KCharacter*)g_cPlayerMgr.GetByIndex(m_rcPlayer.GetPlayerIndex());
	if(!pPlayer) return;

	//if (nEquipPos != emEQUIPPOS_HORSE || pNpc->GetRideHorse())
	//{	// 如果是马，有图形资源才有效果
	for (INT i = 0; i < emITEM_COUNT_BASE; i++)
	{
		CONST KMagicAttrib *pMA = pItem->GetBaseAttrib(i);
		//if (pMA->nAttribType == magic_equip_active_all_ornament)	// 激活所有首饰
		//	m_sActiveSpState.bActiveOrnament++;
		//else if (pMA->nAttribType == magic_equip_active_suit)
		//	ApplyMagicActiveSuit(pMA->nValue[0], pMA->nValue[1]);
		//else if (pMA->nAttribType == magic_equip_disguise_base || pMA->nAttribType == magic_equip_disguise_effect)
		//	ApplyDisguiseMA(pMA);
		////else
		{
			g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, *pMA, FALSE);
		}
	}
	//}

	// 暂时屏蔽掉 modify by zhaoyu 2012-08-03
	/*
	ApplyEnhanceAttrib(nEquipPos);
	ApplyStrengthenAttrib(nEquipPos);
	*/
}

VOID KPlayerItem::RemoveBaseAttrib(INT nEquipPos)
{
	INT nIdx = GetEquip(nEquipPos);
	if (nIdx <= 0)
		return;

	KItem* pItem = &Item[nIdx];
	//KNpc* pNpc   = &Npc[m_rcPlayer.m_nIndex];

	if (pItem->IsInvalid())
		return;
	KCharacter * pPlayer = (KCharacter*)g_cPlayerMgr.GetByIndex(m_rcPlayer.GetPlayerIndex());
	if(!pPlayer) return;

	// 移除该装备的基础属性对NPC的属性调整
	//if (nEquipPos != emEQUIPPOS_HORSE || pNpc->GetRideHorse())
	//{	// 如果是马，要真的骑着才移去效果
	for (INT i = 0; i < emITEM_COUNT_BASE; i++)
	{
		CONST KMagicAttrib *pMA = pItem->GetBaseAttrib(i);
		//if (pMA->nAttribType == magic_equip_active_all_ornament)	// 清除激活属性
		//	m_sActiveSpState.bActiveOrnament--;
		//else if (pMA->nAttribType == magic_equip_active_suit)
		//	RemoveMagicActiveSuit(pMA->nValue[0], pMA->nValue[1]);
		//else if (pMA->nAttribType == magic_equip_disguise_base || pMA->nAttribType == magic_equip_disguise_effect)
		//	RemoveDisguiseMA(pMA, nEquipPos);
		////else
		{
			g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, *pMA, TRUE);
		}
	}
	//}
	// 暂时屏蔽掉 modify by zhaoyu 2012-08-03
	/*
	RemoveEnhanceAttrib(nEquipPos);
	RemoveStrengthenAttrib(nEquipPos);
	*/
}

VOID KPlayerItem::ApplyMagicActiveSuit(INT nSuitId, INT nActiveNum)
{
	if (m_mapBaseActiveSuit.find(nSuitId) == m_mapBaseActiveSuit.end())
		m_mapBaseActiveSuit[nSuitId] = nActiveNum;
	else
		m_mapBaseActiveSuit[nSuitId] += nActiveNum;
}

VOID KPlayerItem::RemoveMagicActiveSuit(INT nSuitId, INT nActiveNum)
{
	if (m_mapBaseActiveSuit.find(nSuitId) == m_mapBaseActiveSuit.end())
		return;
	m_mapBaseActiveSuit[nSuitId] -= nActiveNum;
	if (m_mapBaseActiveSuit[nSuitId] <= 0)
		m_mapBaseActiveSuit.erase(nSuitId);
}

INT KPlayerItem::GetMagicActiveSuit(INT nSuitId)
{
	INT nRet = 0;
	if (m_mapBaseActiveSuit.find(nSuitId) != m_mapBaseActiveSuit.end())
		return m_mapBaseActiveSuit[nSuitId];
	return nRet;
}

VOID KPlayerItem::ApplyEnhanceAttrib(INT nEquipPos)
{
	INT nIdx = GetEquip(nEquipPos);
	if (nIdx <= 0)
		return;

	KItem* pItem = &Item[nIdx];
	KCharacter * pPlayer = (KCharacter*)g_cPlayerMgr.GetByIndex(m_rcPlayer.GetPlayerIndex());
	if(!pPlayer) return;

	CONST KMagicAttrib *pMA = pItem->GetEnhAttribArr();
	g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, *pMA, FALSE);
}

VOID KPlayerItem::ModifyStoneAttribArr(INT nEquipPos, BOOL bIsEndProcess)
{
	INT nIdx = GetEquip(nEquipPos);
	if (nIdx <= 0) 
		return;

	KItem* pItem = &Item[nIdx];
	KCharacter * pPlayer = (KCharacter*)g_cPlayerMgr.GetByIndex(m_rcPlayer.GetPlayerIndex());
	if(!pPlayer) return;

	for(INT i = 0; i < pItem->GetStoneSlot(); ++i)
	{
		INT nParticular = pItem->m_aryStoneInfo[i].byStoneId;
		if(nParticular == 0 || nParticular == emSTONE_LOCKED_ID)
			continue;

		INT nLevel = pItem->m_aryStoneInfo[i].byStoneLevel;
		if(nLevel < 1 || nLevel > emSTONE_MAX_LV)
			continue;

		KMagicAttrib aryStoneAttr[emSTONE_COUNT_ATTRIB];
		for (INT j = 0; j < emSTONE_COUNT_ATTRIB; j++)
		{
			BOOL bRet = GetStoneAttrib(&aryStoneAttr[j], nParticular, nLevel, j);
			if(!bRet)
			{
				return;
			}
			g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, aryStoneAttr[j], bIsEndProcess);
		}
	}
}

VOID KPlayerItem::ApplyStoneAttribArr(INT nEquipPos)
{
	ModifyStoneAttribArr(nEquipPos, FALSE);
}

VOID KPlayerItem::RemoveStoneAttribArr(INT nEquipPos)
{
	ModifyStoneAttribArr(nEquipPos, TRUE);
}

VOID KPlayerItem::RemoveEnhanceAttrib(INT nEquipPos)
{
	INT nIdx = GetEquip(nEquipPos);
	if (nIdx <= 0)
		return;
	KItem* pItem = &Item[nIdx];

	KCharacter * pPlayer = (KCharacter*)g_cPlayerMgr.GetByIndex(m_rcPlayer.GetPlayerIndex());
	if(!pPlayer) return;

	CONST KMagicAttrib *pMA = pItem->GetEnhAttribArr();
	g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, *pMA, TRUE);
	
}

VOID KPlayerItem::ApplyActiveMA(CONST KMASocketSet* pMASS)
{
	ASSERT(pMASS);
	//KNpc* pNpc = &Npc[m_rcPlayer.m_nIndex];
	KCharacter* pCharacter = (KCharacter*)g_cPlayerMgr.GetByIndex(m_rcPlayer.GetPlayerIndex());
	for (INT i = 0; i < pMASS->GetCount(); ++i)
	{
		CONST KMagicAttrib *pMA = pMASS->GetMagicAttrib(i);
		//if (pMA->nAttribType == magic_equip_active_all_ornament && pMASS->IsActive(i))	// 激活所有首饰
		//	m_sActiveSpState.bActiveOrnament++;
		//else if (pMASS->IsActive(i))		// 应用激活魔法属性
		//	pNpc->m_SkillData.ApplyMagicAttrib(pMA, TRUE); // todo [7/13/2012 simon]
		if (pMASS->IsActive(i) && pMA->nAttribType > magic_begin && pMA->nAttribType < magic_end)
		{
			//ApplyRideAttribArr(emEQUIPPOS_HORSE);
			g_cMagicAttribMgr.ModifyAttrib(pCharacter, pCharacter, 0, *pMA, FALSE);
		}
	}
}

VOID KPlayerItem::RemoveActiveMA(CONST KMASocketSet* pMASS)
{
	ASSERT(pMASS);
	//KNpc* pNpc = &Npc[m_rcPlayer.m_nIndex];
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(m_rcPlayer.GetPlayerIndex());
	for (INT i = 0; i < pMASS->GetCount(); ++i)
	{
		CONST KMagicAttrib *pMA = pMASS->GetMagicAttrib(i);
		//if (pMA->nAttribType == magic_equip_active_all_ornament && pMASS->IsActive(i))	// 清除激活属性
		//	m_sActiveSpState.bActiveOrnament--;
		//else if (pMASS->IsActive(i))		// 清除激活魔法属性
		//	pNpc->m_SkillData.RemoveMagicAttrib(pMA, TRUE); // todo [7/13/2012 simon]
		if (pMASS->IsActive(i) && pMA->nAttribType > magic_begin && pMA->nAttribType < magic_end)
		{
			//	RemoveRideAttribArr(emEQUIPPOS_HORSE);
			g_cMagicAttribMgr.ModifyAttrib(pPlayer, pPlayer, 0, *pMA, TRUE);
		}
	}
}

VOID KPlayerItem::ApplyNpcRes(INT nEquipPos)
{
	// 魔人状态下暂时不应用换装
	if(m_rcPlayer.GetAngerLeftTime() > 0)
	{
		return;
	}

	INT nIdx = GetEquip(nEquipPos);
	if (nIdx <= 0)
		return;

	EquipResForNPC(nEquipPos);
}

VOID KPlayerItem::RemoveNpcRes(INT nEquipPos, BOOL bForce /*= FALSE*/)
{
	// 魔人状态下暂时不应用换装
	if(m_rcPlayer.GetAngerLeftTime() > 0)
	{
		return;
	}

	INT nIdx = GetEquip(nEquipPos);
	if (nIdx <= 0 && !bForce)
		return;

	KE_NPCRES_PART ePart = (KE_NPCRES_PART)(-1);
	switch (nEquipPos)
	{
	case emEQUIPPOS_SWORD:
		ePart = emNPCRES_PART_WEAPON;
		break;
	case emEQUIPPOS_HEAD:
		ePart = emNPCRES_PART_HEAD;
		break;
	case emEQUIPPOS_BACK:
		ePart = emNPCRES_PART_BACK;
		break;
	case emEQUIPPOS_BOTTOM:
		ePart = emNPCRES_PART_BOTTEM;
		break;
	case emEQUIPPOS_ARMOR:
		ePart = emNPCRES_PART_BODY;
		break;
	case emEQUIPPOS_SUIT:
		ePart = emNPCRES_PART_SUIT;
		break;
	case emEQUIPPOS_CLOAK:
		ePart = emNPCRES_PART_CLOAK;
		break;
	default:
		return;
	}

	KItem* pItem = &Item[nIdx];
	ModifyEquipRes(&m_rcPlayer, ePart, pItem, FALSE);

	//pNpc->ModifyEquipRes(ePart, bForce); // todo [7/13/2012 simon]
}

#ifdef GAME_SERVER

INT KPlayerItem::AddBankStoreCoin(INT nValue)
{
	BOOL bRet	= FALSE;
	//INT nBankCoin	= m_rcPlayer.m_cPlayerTask.GetTaskValue(TASKVALUE_BANKSTORECOIN);
	//INT nLeft	= nBankCoin + nValue;
	//if (nLeft >= 0)
	//{
	//	m_rcPlayer.m_cPlayerTask.SetTaskValue(TASKVALUE_BANKSTORECOIN, nLeft);
	//	bRet	= TRUE;
	//	m_rcPlayer.CumulateValue(abs(nValue) * KD_VALUE_JB_TO_VALUE);
	//}
	return bRet;
}
INT KPlayerItem::AddBindCoin(INT nValue, INT nWay)
{
	//INT nBindCoin	= m_rcPlayer.m_cPlayerTask.GetTaskValue(TASKVALUE_BINDCOIN);
	//INT nLeft	= nBindCoin + nValue;
	BOOL bRet	= FALSE;
	//if (nLeft >= 0)
	//{
	//	m_rcPlayer.m_cPlayerTask.SetTaskValue(TASKVALUE_BINDCOIN, nLeft);
	//	bRet	= TRUE;
	//	m_rcPlayer.CumulateValue(abs(nValue) * KD_VALUE_JB_TO_VALUE);
	//
	//	if (nValue > 0)			// 是获得了绑金
	//	{
	//		KD_COURSE_LOG(m_rcPlayer, emKCOURSE_BIND_COIN_CHANGE, "AddBindCoin\tAddWay" << nWay << "\tCount\t" << nValue);
	//	}
	//	else					// 是消耗了绑金
	//	{
	//		KD_COURSE_LOG(m_rcPlayer, emKCOURSE_BIND_COIN_CHANGE, "CostBindCoin\tCostWay" << nWay << "\tCount\t" << -nValue);
	//	}

	//	OnCoinChanged(1, nValue);	// 1表示类型是绑金， 第二个参数自己已经能表示正负，这里不需要调整了。
	//	CHAR szMsg[64] = {0};
	//	if (nValue > 0)
	//	{
	//		sprintf(szMsg, MSG_ITEM_ADD_BIND_COIN, nValue);
	//		KPlayerChat::SendSystemInfo(
	//			emKSEND_MSG_PERSONAL,
	//			m_rcPlayer.GetPlayerIndex(),
	//			"",
	//			szMsg,
	//			strlen(szMsg)
	//			);
	//	}
	//}
	return bRet;
}

BOOL KPlayerItem::AddBindMoney(INT nMoney, INT nAddWay, BOOL bNoNeedMsg)
{
	//if (nMoney == 0)
	//	return FALSE;

	//INT nCurMoney	= m_rcPlayer.m_cPlayerTask.GetTaskValue(TASKVALUE_BINDMONEY);
	//INT nLeft		= nCurMoney + nMoney;
	//if (nLeft > GetMaxCarryMoney())
	//{

	//	KPlayerChat::SendSystemInfo(
	//		emKSEND_MSG_PERSONAL,
	//		m_rcPlayer.GetPlayerIndex(),
	//		G_PLAYER_13,
	//		MSG_ITEM_TAKE_BIND_MONEY_ERR,
	//		strlen(MSG_ITEM_TAKE_BIND_MONEY_ERR)
	//		);
	//	return FALSE;
	//}
	//if (bNoNeedMsg != 1)
	//{
	//	CHAR szMsg[64] = {0};
	//	sprintf(szMsg, MSG_ITEM_ADD_BIND_MONEY, nMoney);
	//	KPlayerChat::SendSystemInfo(
	//		emKSEND_MSG_PERSONAL,
	//		m_rcPlayer.GetPlayerIndex(),
	//		"",
	//		szMsg,
	//		strlen(szMsg)
	//		);
	//}
	//// 这里加个最低数量限制，防止频繁的记录琐碎的LOG，打怪得的也不记LOG
	//if (nMoney > 0 && (emKEARN_FARM != nAddWay || nMoney >= 500))
	//	KD_COURSE_LOG(m_rcPlayer, emKCOURSE_BIND_JXB_CHANGE, "AddBindMoney\tAddWay" << nAddWay << "\tCount\t" << nMoney);

	//m_rcPlayer.m_cPlayerTask.SetTaskValue(TASKVALUE_BINDMONEY, nLeft);
	//OnCoinChanged(3, nMoney);	// 3表示绑银类型
	//m_rcPlayer.CumulateValue(abs(nMoney));
	//if (m_rcPlayer.m_eRecordLogType & KPlayer::recordlog_money && nMoney > 500)
	//	m_rcPlayer.WriteLog("AddBindMoney[%d]\t+%d\t=%d", nAddWay, nMoney, nLeft);

	return TRUE;	
}

// nCoinType ：1表示绑金，2表示银两，3表示绑银
// nCount : 大于0表示增加，小于0表示减少
BOOL KPlayerItem::OnCoinChanged(INT nCoinType, INT nCount)
{
	//KScriptFunctionCallHelper sfcHelper(*g_pMainScript);

	//g_pMainScript->CallTableFunction("Item",
	//	"OnCoinChanged",
	//	0, 
	//	"dd",
	//	nCoinType,
	//	nCount);

	return TRUE;
}

BOOL KPlayerItem::c2sApplyEnhance(CONST KITEM_C2S_ENHANCE_APPLY_SYNC* pSync)
{
	//INT nParamCount = 0;
	//INT nEquipIdx = SearchID(pSync->dwEquipId);
	//KScriptFunctionCallHelper cSfcHelp(*g_pMainScript);
	//cSfcHelp.SetMe(&m_rcPlayer);

	//switch (pSync->byMode)
	//{
	//	// 逻辑有很多
	//case emENHANCE_MODE_ENHANCE:
	//	if (nEquipIdx <= 0)
	//		return FALSE;
	//	if (!pSync->byItemCount || pSync->byItemCount > KD_ROOM_ENHANCE_ITEM_SIZE)
	//		return FALSE;
	//	if (pSync->wProtocolSize + PROTOCOL_MSG_SIZE != sizeof(KITEM_C2S_ENHANCE_APPLY_SYNC) - (countof(pSync->adwItemId) - pSync->byItemCount) * sizeof(DWORD))
	//		return FALSE;
	//	if (m_rcPlayer.m_BuyInfo.m_nBuyIdx != BuySellInfo::BUY_INDEX_GIFT)
	//		return FALSE;
	//	g_pMainScript->GetGlobalF("Item:Enhance");
	//	g_pMainScript->PushObj(Item[nEquipIdx].m_pLuna);
	//	g_pMainScript->PushTable();
	//	for (INT i = 0; i < pSync->byItemCount; ++i)
	//	{
	//		INT nListIdx = 0;
	//		INT nItemIdx = SearchID(pSync->adwItemId[i], &nListIdx);
	//		if (nItemIdx <= 0)
	//			return FALSE;
	//		// 不在身上的存储空间内
	//		if (m_asList[nListIdx].sPos.eRoom != emROOM_MAINBAG &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG2 &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG3)
	//			return FALSE;
	//		g_pMainScript->PushObj(Item[nItemIdx].m_pLuna);
	//		g_pMainScript->SetTableIndex(i + 1);
	//	}
	//	g_pMainScript->PushNumber(pSync->byMoneyType);
	//	g_pMainScript->PushNumber(pSync->byParam);
	//	VERIFY(g_pMainScript->DoCall(1, 5));
	//	break;
	//case emENHANCE_MODE_PEEL:
	//	if (nEquipIdx <= 0)
	//		return FALSE;
	//	if (m_rcPlayer.m_BuyInfo.m_nBuyIdx != BuySellInfo::BUY_INDEX_GIFT)
	//		return FALSE;
	//	VERIFY(g_pMainScript->CallTableFunction("Item", "Peel", 1, "od", Item[nEquipIdx].m_pLuna, pSync->byParam));
	//	break;
	//case emENHANCE_MODE_COMPOSE:
	//case emENHANCE_MODE_REFINE:
	//	if (!pSync->byItemCount || pSync->byItemCount > KD_ROOM_ENHANCE_ITEM_SIZE)
	//		return FALSE;
	//	if (pSync->wProtocolSize + PROTOCOL_MSG_SIZE != sizeof(KITEM_C2S_ENHANCE_APPLY_SYNC) - (countof(pSync->adwItemId) - pSync->byItemCount) * sizeof(DWORD))
	//		return FALSE;
	//	if (m_rcPlayer.m_BuyInfo.m_nBuyIdx != BuySellInfo::BUY_INDEX_GIFT)
	//		return FALSE;
	//	if (pSync->byMode == emENHANCE_MODE_COMPOSE)
	//		g_pMainScript->GetGlobalF("Item:Compose");
	//	else
	//		g_pMainScript->GetGlobalF("Item:DoRefine");
	//	nParamCount++;
	//	nParamCount += g_pMainScript->PushTable();
	//	for (INT i = 0; i < pSync->byItemCount; ++i)
	//	{
	//		INT nListIdx = 0;
	//		INT nItemIdx = SearchID(pSync->adwItemId[i], &nListIdx);
	//		if (nItemIdx <= 0)
	//			return FALSE;
	//		// 不在身上的存储空间内
	//		if (m_asList[nListIdx].sPos.eRoom != emROOM_MAINBAG &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG2 &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG3 &&		// 不在身上
	//			!KD_ISEQUIP_ROOM(m_asList[nListIdx].sPos.eRoom))		// 不在装备栏上
	//			return FALSE;

	//		g_pMainScript->PushObj(Item[nItemIdx].m_pLuna);
	//		g_pMainScript->SetTableIndex(i + 1);
	//	}
	//	nParamCount += g_pMainScript->PushNumber(pSync->byMoneyType);
	//	nParamCount += g_pMainScript->PushNumber(pSync->byParam);
	//	VERIFY(g_pMainScript->DoCall(1, nParamCount));
	//	break;
	//case emENHANCE_MODE_UPGRADE:
	//	if (!pSync->byItemCount || pSync->byItemCount > KD_ROOM_ENHANCE_ITEM_SIZE)
	//		return FALSE;
	//	if (pSync->wProtocolSize + PROTOCOL_MSG_SIZE != sizeof(KITEM_C2S_ENHANCE_APPLY_SYNC) - (countof(pSync->adwItemId) - pSync->byItemCount) * sizeof(DWORD))
	//		return FALSE;
	//	if (m_rcPlayer.m_BuyInfo.m_nBuyIdx != BuySellInfo::BUY_INDEX_GIFT)
	//		return FALSE;
	//	g_pMainScript->GetGlobalF("Item:UpgradeSignet");
	//	g_pMainScript->PushObj(Item[nEquipIdx].m_pLuna);
	//	g_pMainScript->PushTable();
	//	for (INT i = 0; i < pSync->byItemCount; ++i)
	//	{
	//		INT nListIdx = 0;
	//		INT nItemIdx = SearchID(pSync->adwItemId[i], &nListIdx);
	//		if (nItemIdx <= 0)
	//			return FALSE;
	//		// 不在身上的存储空间内
	//		if (m_asList[nListIdx].sPos.eRoom != emROOM_MAINBAG &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG2 &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG3)
	//			return FALSE;

	//		g_pMainScript->PushObj(Item[nItemIdx].m_pLuna);
	//		g_pMainScript->SetTableIndex(i + 1);
	//	}
	//	g_pMainScript->PushNumber(pSync->byParam);
	//	VERIFY(g_pMainScript->DoCall(1, 4));
	//	break;
	//case emENHANCE_MODE_STRENGTHEN:
	//	if (nEquipIdx <= 0)
	//		return FALSE;
	//	if (!pSync->byItemCount || pSync->byItemCount > KD_ROOM_ENHANCE_ITEM_SIZE)
	//		return FALSE;
	//	if (pSync->wProtocolSize + PROTOCOL_MSG_SIZE != sizeof(KITEM_C2S_ENHANCE_APPLY_SYNC) - (countof(pSync->adwItemId) - pSync->byItemCount) * sizeof(DWORD))
	//		return FALSE;
	//	if (m_rcPlayer.m_BuyInfo.m_nBuyIdx != BuySellInfo::BUY_INDEX_GIFT)
	//		return FALSE;
	//	g_pMainScript->GetGlobalF("Item:Strengthen");
	//	g_pMainScript->PushObj(Item[nEquipIdx].m_pLuna);
	//	g_pMainScript->PushTable();
	//	for (INT i = 0; i < pSync->byItemCount; ++i)
	//	{
	//		INT nListIdx = 0;
	//		INT nItemIdx = SearchID(pSync->adwItemId[i], &nListIdx);
	//		if (nItemIdx <= 0)
	//			return FALSE;
	//		// 不在身上的存储空间内
	//		if (m_asList[nListIdx].sPos.eRoom != emROOM_MAINBAG &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG2 &&
	//			m_asList[nListIdx].sPos.eRoom != emROOM_EXTBAG3)
	//			return FALSE;
	//		g_pMainScript->PushObj(Item[nItemIdx].m_pLuna);
	//		g_pMainScript->SetTableIndex(i + 1);
	//	}
	//	g_pMainScript->PushNumber(pSync->byMoneyType);
	//	g_pMainScript->PushNumber(pSync->byParam);
	//	VERIFY(g_pMainScript->DoCall(1, 5));
	//	break;
	//default:
	//	if (m_rcPlayer.m_BuyInfo.m_nBuyIdx == BuySellInfo::BUY_INDEX_GIFT)
	//		m_rcPlayer.m_BuyInfo.Clear();			// 结束强化/剥离操作
	//	return FALSE;
	//}

	//INT nRet = g_pMainScript->GetInt(-1);
	//KITEM_S2C_ENHANCE_RESULT_SYNC sSync;
	//sSync.cProtocol = (PROTOCOL_MSG_TYPE)s2c_enhance_result;
	//sSync.byMode	= pSync->byMode;
	//sSync.byResult  = (BYTE)nRet;
	//g_pCoreServer->SendData(m_rcPlayer.m_nNetConnectIdx, &sSync, sizeof(sSync));
	//return (nRet == 1) ? TRUE : FALSE;
	return 0;
}

#else	// #ifdef GAME_SERVER

#endif	// #ifdef GAME_SERVER #else

VOID KPlayerItem::PrepareItemRepair(DWORD dwUseItemID)
{
#ifdef GAME_SERVER
	KSYNC_PREPARE_ITEMREPAIR sSync;
	sSync.dwUseItemID	= dwUseItemID;
	g_cItemMgr.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
#else
	m_nRepairUseItem = SearchID(dwUseItemID);
	if (m_nRepairUseItem > 0)
		::g_CoreEventNotify(emCOREEVENT_PREPARE_ITEMREPAIR);
#endif
}

BOOL KPlayerItem::SwitchItem(CONST KITEM_POS& sPickPos, CONST KITEM_POS& sDropPos)
{
	INT nPick = GetItemByPos(sPickPos);
	INT nDrop = GetItemByPos(sDropPos);
	BOOL bRet = FALSE;

#ifdef GAME_SERVER
	bRet = m_cSwitch.Switch(sPickPos, sDropPos);

	// todo
	//if (bRet)
	//{
	//	m_rcPlayer.RegisterProcessEvent(KTaskTimerList::EVENT_ARRANGEITEM);
	//	m_rcPlayer.RegisterProcessEvent(KTaskTimerList::EVENT_CHANGEEQUIP);
	//}

#else

	bRet = m_cSwitch.Switch(sPickPos, sDropPos);		// 执行交换

	// 通知UI更新
	::g_CoreEventNotify(emCOREEVENT_SYNC_ITEM, "ddd", sPickPos.eRoom, sPickPos.nX, sPickPos.nY);
	::g_CoreEventNotify(emCOREEVENT_SYNC_ITEM, "ddd", sDropPos.eRoom, sDropPos.nX, sDropPos.nY);

#endif

	PROCESS_ERROR(bRet);

	if (m_rcPlayer.GetMoveState() == cmsOnSit)
		m_rcPlayer.SetMoveState(cmsOnStand);

	if(sPickPos.eRoom == emROOM_EQUIP || sDropPos.eRoom == emROOM_EQUIP ||
		sPickPos.eRoom == emROOM_EQUIPEX || sDropPos.eRoom == emROOM_EQUIPEX)
	{
		g_cCharacterHelper.CalcFightScore(m_rcPlayer);
		m_rcPlayer.FireEvent(emKOBJEVENTTYPE_PLAYER_EQUIP_OPS, m_rcPlayer.GetId(), 0, 0);
	}

	m_rcPlayer.FireEvent(emKOBJEVENTTYPE_PLAYER_ITEM_OPS, m_rcPlayer.GetId(), 0, 0);

EXIT0:
#ifdef GAME_SERVER
	KITEM_S2C_SWITCH_SYNC sSync;
	sSync.bSuccess		= (BYTE)bRet;
	sSync.byPickRoom	= (BYTE)sPickPos.eRoom;
	sSync.byPickX		= (BYTE)sPickPos.nX;
	sSync.byPickY		= (BYTE)sPickPos.nY;
	sSync.byDropRoom	= (BYTE)sDropPos.eRoom;
	sSync.byDropX 		= (BYTE)sDropPos.nX;
	sSync.byDropY 		= (BYTE)sDropPos.nY;
	g_cItemMgr.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
#endif

	return	bRet;
}

#ifdef GAME_SERVER

BOOL KPlayerItem::AutoEquip(INT nItemIdx)
{
	KITEM_POS sPickPos;
	KITEM_POS sDropPos;
	if (!IsMyItem(nItemIdx))
		return	FALSE;

	INT nDetail = Item[nItemIdx].GetDetailType();
	INT nEquipPos = GetEquipPos(nDetail);
	_ASSERT(nEquipPos >= 0);

	if (!CanEquip(Item[nItemIdx], nEquipPos))
		return	FALSE;

	if (nEquipPos < emEQUIPPOS_NUM)  // 玩家装备
	{
		sDropPos.eRoom = emROOM_EQUIP;
		sDropPos.nX = nEquipPos;
		sDropPos.nY = 0;
	}

	INT nListIdx = FindSame(nItemIdx);
	memcpy(&sPickPos, &m_asList[nListIdx].sPos, sizeof(sPickPos));
	SwitchItem(sPickPos, sDropPos);

	/*
	INT nX = 0, nY = 0;
	INT nOldEquipIdx = GetEquip(nEquipPos);
	if (nOldEquipIdx > 0)
	{	// 如果欲装备位置已有东西，卸下来
	_ASSERT(nOldEquipIdx != nItemIdx);
	KITEM_POS sPos;
	::memset(&sPos, 0, sizeof(sPos));
	if (!GetBagFreeCell(sPos))
	return	FALSE;
	Remove(nOldEquipIdx);
	Add(nOldEquipIdx, sPos.eRoom, sPos.nX, sPos.nY);
	}

	Remove(nItemIdx);

	if (!Add(nItemIdx, emROOM_EQUIP, nEquipPos, 0))
	{	// 添加失败，还得放回去
	KITEM_POS sPos;
	if (!GetBagFreeCell(sPos))
	return	FALSE;
	Add(nItemIdx, sPos.eRoom, sPos.nX, sPos.nY);
	}*/

	return TRUE;
}

VOID KPlayerItem::c2sSyncSwitchEquip(CONST KSWITCH_EQUIP_PROTOCOL* pProtocol)
{
	// 切换间隔时间检测
	time_t nCurTime = ::KSysService::Time(NULL);
	time_t nDiffTime = nCurTime - m_nLastSwithTime;
	if (nDiffTime < KD_ITEM_SWITCH_TIMELIMIT)
	{
		// 短时间内切换装备，切换失败
		CHAR szBuf[128];
		::sprintf(szBuf, "切换装备太频繁，请过 %ld 秒再试！", (int64_t)(KD_ITEM_SWITCH_TIMELIMIT - nDiffTime));
		g_ChatChannelManager.ProcessRespond(trLocalSys,
			emKSYSTEMMSG_NORMAL,
			m_rcPlayer.GetId(),
			"系统",
			szBuf, emQCHAT_PANEL
			);
		return;
	}
	m_nLastSwithTime = nCurTime;

	// TODO: xyf 地图切换限制检测

	for (INT i = 0; i < pProtocol->byCount; ++i)
	{
		KITEM_POS sEqPos, sEqExPos;
		sEqPos.eRoom	= emROOM_EQUIP;
		sEqPos.nX		= pProtocol->asPair[i].byEquipPos;
		sEqPos.nY		= 0;
		sEqExPos.eRoom	= emROOM_EQUIPEX;
		sEqExPos.nX		= pProtocol->asPair[i].byEquipExPos;
		sEqExPos.nY		= 0;

		SwitchItem(sEqPos, sEqExPos);
	}
}

#endif	// #ifdef GAME_SERVER

BOOL KPlayerItem::ResizeRoom(KE_ITEM_ROOM eRoom, INT nWidth, INT nHeight)
{
	if (eRoom < 0 || eRoom >= emROOM_NUM)
	{
		_ASSERT(FALSE);
		return	FALSE;
	}

	KItemRoom &rcRoom = m_acRoom[eRoom];
	if (!rcRoom.Resize(nWidth, nHeight))		// 先变包裹空间大小，此时有可能部分道具不能被外部访问
		return	FALSE;

	for (INT i = 0; i < KD_MAX_PLAYER_ITEM; ++i)
	{
		KLISTITEM *psListItem = &m_asList[i];
		if (psListItem->nIdx <= 0 || psListItem->sPos.eRoom != eRoom)
			continue;			// 没东西或者不在指定空间内，不处理
		if (psListItem->sPos.nX < rcRoom.GetWidth() && psListItem->sPos.nY < rcRoom.GetHeight())
			continue;			// 东西在新旧空间的交集中，不处理

		// 旧空间比新空间多出来的东西，要移动
		INT nX = 0, nY = 0;
		if (!rcRoom.GetFreeCell(nX, nY))
		{	// 绝对不应该失败，否则可能导致数据错误
			printf(
				"[ERR] Resize room FATAL ERROR : List(%d) Room(%d) nX(%d) nY(%d) ItemIdx(%d)",
				i,
				eRoom,
				psListItem->sPos.nX,
				psListItem->sPos.nY,
				psListItem->nIdx
				);
			_ASSERT(FALSE);
			return	FALSE;
		}

		// 移动道具
		rcRoom.PickItemDirectly(psListItem->nIdx, psListItem->sPos.nX, psListItem->sPos.nY);
#ifndef GAME_SERVER
		::g_CoreEventNotify(emCOREEVENT_SYNC_ITEM, "ddd", eRoom, psListItem->sPos.nX, psListItem->sPos.nY);
#endif
		psListItem->sPos.nX = nX;
		psListItem->sPos.nY = nY;
		rcRoom.DropItem(psListItem->nIdx, nX, nY);
#ifndef GAME_SERVER
		::g_CoreEventNotify(emCOREEVENT_SYNC_ITEM, "ddd", eRoom, nX, nY);
#endif
	}

	return	TRUE;
}


BOOL KPlayerItem::TestInvalid(CONST KItem& rcItem) CONST
{
	if (rcItem.GetCount() <= 0)
		return	TRUE;	// 耐久为0或者叠加数目为0，视为失效

	for (INT j = 0; j < emITEM_COUNT_REQ; j++)
	{	// 判断是否满足需求条件
		if (SatisfyRequire(rcItem.m_aryReqAttrib[j].eRequire, rcItem.m_aryReqAttrib[j].nValue))
			continue;
		return	TRUE;		// 不满足需求条件，失效
	}

	return	FALSE;
}

BOOL KPlayerItem::CheckItemUsable(INT nIdx) CONST
{
	KItem& rcItem = Item[nIdx];
	KE_MOVE_STATE eMoveState = m_rcPlayer.GetMoveState();
	BOOL bRet = FALSE;

	CHAR szRet[128] = {0};	// 道具使用检查输出信息
#ifdef GAME_SERVER
	DWORD nCurFrame = g_cOrpgWorld.m_dwLogicFrames;
	// 如果玩家使用一个道具失败，在接下来的g_nUseFailedFrameInterval帧里都不接收再使用该道具的协议，直接跳转到出口
	PROCESS_ERROR(nCurFrame - rcItem.m_dwLastFailedFrame > (DWORD)g_nUseFailedFrameInterval);
#endif	// #ifdef GAME_SERVER

#ifdef GAME_SERVER
	PROCESS_ERROR(m_nForbidUse != 1);
	//	PROCESS_ERROR(!m_rcPlayer.m_cPlayerTrade.IsTrading()); // 正在交易	
#endif

	PROCESS_ERROR(nIdx > 0 && nIdx < MAX_ITEM);

	//PROCESS_ERROR(m_rcPlayer.m_cPlayerStall.GetStallState() == KPlayerStall::emKSTAT_NORMAL); // 正在摆摊/收购/给予物品			

	PROCESS_ERROR(FindSame(nIdx) > 0); // 不在身上			

	PROCESS_ERROR(CanUseItem(rcItem)); // 不可使用道具			

	PROCESS_ERROR(eMoveState != cmsOnDeath/* && eMoveState != cmsOnRevive*/); // 不是活人不能用道具

	PROCESS_ERROR(m_cCDTime.IsTimeUp(rcItem.GetCDType()));	// 检查CD时间

	//PROCESS_ERROR(Npc[m_rcPlayer.m_nIndex].IsCanUse()); // 是否禁止使用道具
	{
		g_cScriptManager.SetMe((QLunaBase*)m_rcPlayer.GetScriptInterface());
		g_cScriptManager.SetIt(rcItem.m_pLuna);
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

		IKScene* pScene = m_rcPlayer.GetScene();
		if (pScene)
			PROCESS_ERROR(CheckUseLimited(rcItem.GetForbidType(), pScene->GetTemplateId(), szRet));

		//保护栈
		int nTopIdx = cSafeScript->GetTopIndex();
		cSafeScript->SafeCallBegin(nTopIdx);
		VERIFY(cSafeScript->CallTableFunction("Item", "CheckUsable", 1, "s", rcItem.GetClass()));
		bRet = cSafeScript->GetInt(-1);
		cSafeScript->SafeCallEnd(nTopIdx);
	}	
EXIT0:
#ifdef GAME_SERVER
	if (bRet)	// 使用成功，将道具的计数重置
	{
		rcItem.m_nUseFailedTimes = 0;
		rcItem.m_dwLastFailedFrame = 0;
	}
	else	// 如果玩家使用一个道具失败，在接下来的g_nUseFailedFrameInterval帧里都不接收再使用该道具的协议并做计数
	{
		if (rcItem.m_dwLastFailedFrame == 0)   // 第一次开始计数或重新开始计数
			rcItem.m_nUseFailedTimes = 1;
		else if (nCurFrame - rcItem.m_dwLastFailedFrame <= (DWORD)g_nUseFailedFrameInterval)  // 符合条件，计数
			rcItem.m_nUseFailedTimes++;
		else    // 这次失败距上次失败大于间隔时间了，重新计数
			rcItem.m_nUseFailedTimes = 1;

		rcItem.m_dwLastFailedFrame = nCurFrame;	// 更新上次失败时间

		if (rcItem.m_nUseFailedTimes >= g_nUseFailedLimitTime)
		{
			// 如果限制次数为0，则只是在每次非法使用道具后输出，否则在达到次数限制之后，将玩家踢下线
			if (g_nUseFailedLimitTime != 0)
			{
				printf("%s连续使用%s失败%d次，疑似外挂！\n",
					m_rcPlayer.GetName(), rcItem.GetName(), rcItem.m_nUseFailedTimes);
				// m_rcPlayer.KickOut();
			}
			else if (g_nUseFailedLimitTime == 0 && rcItem.m_nUseFailedTimes == 1)
			{
				// 当限制次数为0时，连续失败的情况只做一次输出
				printf("%s: %s,%s,%d\n", "illegal use item low", m_rcPlayer.GetName(), rcItem.GetName(), rcItem.m_nUseFailedTimes);
			}			
		}
	}
#endif	// #ifdef GAME_SERVER
	if (szRet && szRet[0])
	{
#ifdef GAME_SERVER
		g_ChatChannelManager.RequestSendMsg(
			trLocalSys,
			emKSYSTEMMSG_NORMAL,
			m_rcPlayer.GetId(),
			"系统",
			szRet,
			emQCHAT_PANEL
			);
#else
		::g_CoreEventNotify(emCOREEVENT_SYSTEM_MESSAGE, "s", szRet);
#endif
	}

	return	bRet;
}

BOOL KPlayerItem::UseItem(INT nIdx, INT nParam /* = 0 */)
{
#ifdef GAME_SERVER

	if (nIdx <= 0)
		return	FALSE;

	KItem* pItem = &Item[nIdx];
	ScriptSafe rcScriptSafe = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)m_rcPlayer.GetScriptInterface());
	g_cScriptManager.SetIt(pItem->m_pLuna);

	// todo todo [7/13/2012 simon]
	//// 记录玩家使用物品事件
	//m_rcPlayer.RegisterProcessEvent(KTaskTimerList::EVENT_USEITEM);
	if (!rcScriptSafe->CallTableFunction("Item", "OnUse", 1, "sd", pItem->GetClass(), nParam))
		return	FALSE;

	//if (pItem->GetGenre() != item_medicine)
	//	KD_COURSE_LOG(m_rcPlayer, emKCOURSE_PLAYER_USEITEM, "UseItem\t" << pItem->GetName());

	if (rcScriptSafe->GetInt(-1))
	{
		// 扣除物品
		if (pItem->GetMaxCount() > 1 && pItem->GetCount() > 1)
		{
			SetItemStackCount(nIdx, pItem->GetCount() - 1, emITEM_DATARECORD_REMOVE, emKLOSEITEM_USE, emKADDITEM_NONE);
		}
		else
		{
			if (!Remove(nIdx, emKLOSEITEM_USE))
			{
				_ASSERT(FALSE);
				return	FALSE;
			}
			// 日志
			//KLogWritter::GetInstance()->Goddess_ItemLog(
			//	&m_rcPlayer,
			//	pItem,
			//	FALSE,
			//	emKITEMLOG_TYPE_USE);
			g_cItemMgr.Remove(nIdx, emITEM_DATARECORD_REMOVE, emITEM_CONSUMEMODE_EAT, &m_rcPlayer);
		}
	}

#endif	// #ifdef GAME_SERVER

	if (m_rcPlayer.GetMoveState() == cmsOnSit)
		m_rcPlayer.SetMoveState(cmsOnStand); // 强制站立

	return TRUE;
}

BOOL KPlayerItem::IsBagRoom(KE_ITEM_ROOM eRoom)
{
	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		if (eRoom == g_aeBagRoom[i])
			return	TRUE;
	}
	return	FALSE;
}

BOOL KPlayerItem::GetBagFreeCell(KITEM_POS& sPos) CONST
{
	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		KE_ITEM_ROOM eRoom = g_aeBagRoom[i];
		INT nX = 0, nY = 0;
		if (!m_acRoom[eRoom].GetFreeCell(nX, nY))
			continue;
		sPos.nX = nX;
		sPos.nY = nY;
		sPos.eRoom = eRoom;
		return	TRUE;
	}

	return	FALSE;
}

INT KPlayerItem::CountBagFreeCell(VOID) CONST
{
	INT nCount = 0;
	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
		nCount += m_acRoom[g_aeBagRoom[i]].CountFree();

	return nCount;
}

BOOL KPlayerItem::CheckAutoGive(INT nGenre, INT nDetail, INT nParticular, INT nLevel, INT nAmount)
{
	BOOL bResult = FALSE;
	KPlayerPurse* pPlayerPurse = NULL;
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;

	pPlayerPurse = m_rcPlayer.GetCtrl<KPlayerPurse>();
	LOG_PROCESS_ERROR(pPlayerPurse);
	if(nGenre == 18 && nDetail == 1 && nParticular == 276)
	{
		pPlayerPurse->AddMoney(EM_MONEY_PRESTIGE, nAmount, &nResult);
		LOG_PROCESS_ERROR(nResult == emKPURSE_S_SUCCESS);
		bResult = TRUE;
	}
	else if(nGenre == 18 && nDetail == 1 && nParticular == 217)
	{
		pPlayerPurse->AddMoney(EM_MONEY_JINGPO, nAmount, &nResult);
		LOG_PROCESS_ERROR(nResult == emKPURSE_S_SUCCESS);
		bResult = TRUE;
	}
	else if(nGenre == 18 && nDetail == 1 && nParticular == 277)
	{
		pPlayerPurse->AddMoney(EM_MONEY_SILVER, nAmount, &nResult);
		LOG_PROCESS_ERROR(nResult == emKPURSE_S_SUCCESS);
		bResult = TRUE;
	}
	else if(nGenre == 18 && nDetail == 1 && nParticular == 278)
	{
		pPlayerPurse->AddMoney(EM_MONEY_YLENERGY, nAmount, &nResult);
		LOG_PROCESS_ERROR(nResult == emKPURSE_S_SUCCESS);
		bResult = TRUE;
	}
	else if(nGenre == 18 && nDetail == 1 && nParticular == 222)
	{
		//m_rcPlayer.AddExp(nAmount);
		if (m_rcPlayer.GetLevel()<=(MAX_PLAYER_LEVEL-1))
		{
			m_rcPlayer.AddExp(nAmount);
		}
		else
		{

		//by ldy
//			DWORD nLvUpNeedExp = g_cCharacterHelper.GetNextLvExp(MAX_PLAYER_LEVEL);
//			if (m_rcPlayer.GetExp() +	nAmount < nLvUpNeedExp)//经验卡在升级经验少一点
//			{
//				m_rcPlayer.AddExp(nAmount);
//			}
//			else
//			{
//				m_rcPlayer.AddExp(nLvUpNeedExp - m_rcPlayer.GetExp() - 1);
//			}

		}
		bResult = TRUE;
	}
	else if(nGenre == 18 && nDetail == 1 && nParticular == 507)
	{
		pPlayerPurse->AddMoney(EM_MONEY_COIN, nAmount, &nResult);
		LOG_PROCESS_ERROR(nResult == emKPURSE_S_SUCCESS);
		bResult = TRUE;
	}
	
EXIT0:
	return bResult;
}

BOOL KPlayerItem::GetRepFreeCell(KITEM_POS& sPos)
{
	INT nX = 0, nY = 0;
	if (!m_acRoom[sPos.eRoom].GetFreeCell(nX, nY))
		return	FALSE;
	sPos.nX = nX;
	sPos.nY = nY;
	return	TRUE;
}


INT KPlayerItem::CountRepFreeCell(KE_ITEM_ROOM eRoom) CONST
{
	INT nCount = 0;
	nCount += m_acRoom[eRoom].CountFree();
	return nCount;
}


INT KPlayerItem::CalcFreeItemCountInBags(CONST KITEM_INDEX& rsIndex, BOOL bBind) CONST
{
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	CONST KBASICPROP_BASEDATA* psBase = pLibBPT->GetBasicProp(rsIndex);
	if (!psBase)
		return	0;

	if (KD_ISEQUIP(rsIndex.nGenre))
		return	CountBagFreeCell();

	CONST KBASICPROP_OTHER* psOther = static_cast<CONST KBASICPROP_OTHER*>(psBase);
	INT nMaxCount = psOther->nStackMax;
	INT nCount = CountBagFreeCell() * nMaxCount;

	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		CONST KItemRoom& rcRoom = m_acRoom[g_aeBagRoom[i]];
		KItemRoom::KFinder cFind = rcRoom.GetFinder();
		KRoomFindCondition::StackWith cCond(rsIndex, bBind);
		while (cFind.Next(cCond))
		{
			INT nCurCount = Item[cFind.Index()].GetCount();
			ASSERT(nCurCount > 0 && nCurCount <= nMaxCount);
			nCount += nMaxCount - nCurCount;
		}
	}

	return nCount;
}

INT	KPlayerItem::CalcFreeItemCountInBags(KItem& rcItem) CONST
{
	return	CalcFreeItemCountInBags(rcItem.GetIndex(), rcItem.IsBind());
}

BOOL KPlayerItem::ClearItemSellPrice(INT nItemIdx)
{
	BOOL bRet = FALSE;
	//	PROCESS_ERROR(nItemIdx > 0 && nItemIdx < MAX_ITEM);
	//	bRet = m_rcPlayer.m_cPlayerStall.AddStallInfo(nItemIdx, 0, 0, FALSE);
	//#ifdef GAME_SERVER
	//	m_rcPlayer.m_cPlayerStall.SyncStallInfo();
	//#endif
	//EXIT0:
	return bRet;
}

BOOL KPlayerItem::IsInBags(INT nItemIdx) CONST
{
	if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		return	FALSE;

	INT nListIdx = FindSame(nItemIdx);
	if (nListIdx <= 0)
		return	FALSE;

	return	IsBagRoom(m_asList[nListIdx].sPos.eRoom);
}

BOOL KPlayerItem::SetExtRepState(INT nState)
{
	if (nState < 0 || nState > emEXTREPPOS_NUM)
		return	FALSE;
	if (nState == m_nExtRepState)
		return	TRUE;

	m_nExtRepState = nState;

	//for (INT i = 0; i < m_nExtRepState; ++i)
	//	VERIFY(m_acRoom[g_aeExtRepPos2Room[i]].Resize(KD_ROOM_EXTREP_WIDTH, KD_ROOM_EXTREP_HEIGHT));
	//for (INT i = m_nExtRepState; i < emEXTREPPOS_NUM; ++i)
	//	VERIFY(m_acRoom[g_aeExtRepPos2Room[i]].Resize(0, 0));		// TODO: xyf 当扩展储物箱里有东西时这里会失败, 以后再说

#ifndef GAME_SERVER
	::g_CoreEventNotify(emCOREEVENT_EXTREPSTATE_CHANGED, "d", m_nExtRepState);
#endif

	return	TRUE;
}

BOOL KPlayerItem::ThrowAwayItem(DWORD dwId)
{
#ifdef GAME_SERVER
	//// 检查该物品是否属于禁止移动的物品，如果属实，则禁止任何操作
	//if (m_rcPlayer.m_bForbidMoveAndThrowItem)
	//	return	FALSE;
#endif
	INT nItemIdx = SearchID(dwId);
	if (nItemIdx <= 0)
		return	FALSE;

	INT nListIdx = FindSame(nItemIdx);
	if (nListIdx <= 0)
		return	FALSE;

	if(IS_BZONE_PLAYER(m_rcPlayer.GetId()) && !Item[nItemIdx].CanUseInBZone())
		return FALSE;

	switch (m_asList[nListIdx].sPos.eRoom)
	{
	case emROOM_EQUIP:
	case emROOM_EQUIPEX:
	case emROOM_MAINBAG:
	case emROOM_EXTBAG:
		break;
	default:
		return	FALSE;
	}
	//	if (m_rcPlayer.CheckPlayerAccountLockState() && (!Item[nItemIdx].IsNewPick() || Item[nItemIdx].IsStackable()))
	//	{
	//#ifndef GAME_SERVER
	//		gShowSystemMessage(MSG_ACCOUNT_LOCK_NOTHROW);
	//		g_CoreEventNotify(emCOREEVENT_SET_POPTIP, "d", 44);
	//#endif
	//		return FALSE;
	//	}
#ifdef GAME_SERVER

	if (Item[nItemIdx].IsForbitThrow())
	{
		return	FALSE;
	}

	if (Remove(nItemIdx, emKLOSEITEM_THROW))
	{
		g_cItemMgr.Remove(nItemIdx, emITEM_DATARECORD_REMOVE, emITEM_CONSUMEMODE_ERRORLOST_THROWALLITEM, &m_rcPlayer);
	}

#else

	if (Item[nItemIdx].IsForbitThrow())
	{
		gShowSystemMessage(MSG_ITEM_NOTHROW);
		return	FALSE;
	}

	KITEM_THROWAWAY_SYNC sSync;
	sSync.cProtocol	= (PROTOCOL_MSG_TYPE)c2s_playerthrowawayitem;
	sSync.dwId		= dwId;
	g_pCoreClient->SendData(&sSync, sizeof(sSync));

#endif

	//#ifdef GAME_SERVER
	//	m_rcPlayer.RegisterProcessEvent(KTaskTimerList::EVENT_DROPITEM);
	//#endif

	return	TRUE;
}

VOID KPlayerItem::ClearSuiteMass(VOID)
{
	for (KMassSuiteVec::iterator it = m_vecGoldMASS.begin(); it != m_vecGoldMASS.end(); ++it)
		delete *it;
	for (KMassSuiteVec::iterator it = m_vecGreenMASS.begin(); it != m_vecGreenMASS.end(); ++it)
		delete *it;
	m_vecGoldMASS.clear();
	m_vecGreenMASS.clear();
}

#ifdef GAME_SERVER
VOID KPlayerItem::c2sSplitItem(DWORD dwItemID, INT nSplitCount)
{
	INT nItemIdx = g_cItemMgr.SearchID(dwItemID);
	if (nItemIdx <= 0)
		return;
	SplitItem(nItemIdx, nSplitCount);
}

INT KPlayerItem::AddItemOnBody(INT nItemIdx, KE_ADD_ITEM_WAY eWay, BOOL bDropIfFull /* = TRUE */, BOOL bMsg2Player /* = FALSE */, BOOL bStack /*= TRUE*/, INT nItemCount /*= 1*/)
{
	if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		return	0;

	KItem& rcItem = Item[nItemIdx];

	nItemIdx = AddItemIntoBag(nItemIdx, eWay, bMsg2Player, bStack, NULL, nItemCount);

	if (nItemIdx > 0)
		return	nItemIdx;

	if (!bDropIfFull)
		return	0;

	//KMAPPOS sMapPos;
	//m_rcPlayer.GetAboutPos(&sMapPos);

	//KObjMgr::KITEMINFO sInfo;
	//KObjMgr::MakeObjItemInfo(nItemIdx, sInfo);

	//INT nObj = g_cObjMgr.Add(Item[nItemIdx].GetObjIdx(), sMapPos, sInfo);
	//if (nObj >= 0)
	//{
	//	Object[nObj].SetItemBelong(m_rcPlayer.GetPlayerID());

	//	INT nBindType = Item[nItemIdx].GetBindType();
	//	if (emITEM_BIND_GET == nBindType || emITEM_BIND_OWN == nBindType)
	//	{
	//		Item[nItemIdx].SetOnlyBelongPick(TRUE);
	//	}
	//}
	//else
	//{
	//	/*
	//	// 日志
	//	KLogWritter::GetInstance()->ItemHistory_RemoveItem(
	//		&Item[nItemIdx],
	//		&m_rcPlayer,
	//		emKLOG_REASON_REMOVEITEM_SYSTEM
	//	);
	//	*/
	//	g_LogItemWhenObjFailed(nItemIdx, m_rcPlayer.GetPlayerIndex());
	g_cItemMgr.Remove(nItemIdx, emITEM_DATARECORD_NORMAL, emITEM_CONSUMEMODE_ERRORLOST_ADDONBODY, &m_rcPlayer);
	//	return	0;
	//}
	return	TRUE;
}

INT KPlayerItem::AddItemIntoBag(INT nItemIdx, KE_ADD_ITEM_WAY eWay, BOOL bMsg2Player /* = FALSE */,
								BOOL bStack /*= TRUE*/, LPCSTR pszCurrencyType/* = NULL*/, INT nItemCount /*= 1*/)
{
	if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
		return 0;
	INT newItemCount = nItemCount;
	KItem& rcItem = Item[nItemIdx];
	LPCSTR szItemName = rcItem.GetOrgName();
	INT nCount = rcItem.GetCount();
	INT nBindType = rcItem.GetBindType();
	if (emITEM_BIND_GET == nBindType || emITEM_BIND_OWN == nBindType)
		rcItem.Bind();						// 获得时绑定

	if(nCount > 1 && newItemCount == 1)
		newItemCount = nCount;

	INT nStack = 0;
	INT nFlag  = 0;
	if (Item[nItemIdx].IsStackable() && bStack)
	{	// 先尝试叠放
		if(newItemCount > (INT)Item[nItemIdx].GetMaxCount())
		{
			if(FindStackRoom(nItemIdx, eWay, rcItem.GetGenre()))
				newItemCount -= Item[nItemIdx].GetMaxCount();
			nStack = FindAndStackInBag(nItemIdx, eWay, Item[nItemIdx].GetMaxCount() + 1);
			nFlag = 2;//新创不止一个格子
		}
		else
		{
			INT nDropItem = FindStackRoom(nItemIdx, eWay, rcItem.GetGenre());

			if(nDropItem)
				nFlag = 3;//找到了可叠加，有可能增加多一个格子，也可能没有
			else
				nFlag = 1;//没找到可叠加，一定新创一个格子
			nStack = FindAndStackInBag(nItemIdx, eWay, newItemCount);
			

			if(nDropItem>0 && Item[nDropItem].GetCount())
				newItemCount -= Item[nDropItem].GetCount();

		}

		//printf("Stack status:%d\n",nStack);
	}
	PROCESS_SUCCESS(nStack); // 叠加成功，跳到最后

	for (INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		// 找空间放这个物品
		KItemRoom& rcRoom = m_acRoom[g_aeBagRoom[i]];
		INT nX = 0, nY = 0;
		if (rcRoom.GetFreeCell(nX, nY) && Add(nItemIdx, g_aeBagRoom[i], eWay, nX, nY, 0, 0, 0, pszCurrencyType) > 0)
		{
			nStack = nItemIdx;
			break;
		}
	}

	if(newItemCount >= 1 && newItemCount <= (INT)Item[nItemIdx].GetMaxCount()  && Item[nItemIdx].IsStackable() && nStack && nFlag == 1)
	{
		if(!SetItemStackCount(nItemIdx, newItemCount, emITEM_DATARECORD_NORMAL, emKLOSEITEM_NONE, eWay))
			return FALSE;
	}
	else if(newItemCount >= 1 && Item[nItemIdx].IsStackable() && nStack && nFlag != 1 && nFlag != 3)
	{
		newItemCount--;
		INT nItemSum = newItemCount/Item[nItemIdx].GetMaxCount() + (newItemCount%Item[nItemIdx].GetMaxCount()?1:0);//需要格子总数，删除一个已经被添加的
		for(INT nCountNum = 0;nCountNum < nItemSum ;nCountNum++)
		{
			KItem& rcItem = Item[m_nItemIdxArr[nCountNum]];
			INT nCount = rcItem.GetCount();
			INT nBindType = rcItem.GetBindType();
			if (emITEM_BIND_GET == nBindType || emITEM_BIND_OWN == nBindType)
				rcItem.Bind();						// 获得时绑定

 			if(newItemCount > (INT)Item[nItemIdx].GetMaxCount())
				nStack = FindAndStackInBag(m_nItemIdxArr[nCountNum], eWay, Item[nItemIdx].GetMaxCount());
			else
				nStack = FindAndStackInBag(m_nItemIdxArr[nCountNum], eWay, newItemCount);
			for (INT i = 0; i < countof(g_aeBagRoom); ++i)
			{
				KItemRoom& rcRoom = m_acRoom[g_aeBagRoom[i]];
				INT nX = 0, nY = 0;
				if (rcRoom.GetFreeCell(nX, nY) && Add(m_nItemIdxArr[nCountNum], g_aeBagRoom[i], eWay, nX, nY, 0, 0, 0, pszCurrencyType) > 0)
				{
					nStack = m_nItemIdxArr[nCountNum];
					break;
				}
			}
			newItemCount -= Item[nItemIdx].GetMaxCount();
		}
	}
	else if(nFlag == 0 && nItemCount > 1 && !Item[nItemIdx].IsStackable())
	{
		newItemCount--;
		INT nItemSum = newItemCount/Item[nItemIdx].GetMaxCount() + (newItemCount%Item[nItemIdx].GetMaxCount()?1:0);//需要格子总数，删除一个已经被添加的
		for(INT nCountNum = 0;nCountNum < nItemSum ;nCountNum++)
		{	
			KItem& rcItem = Item[m_nItemIdxArr[nCountNum]];
			INT nCount = rcItem.GetCount();
			INT nBindType = rcItem.GetBindType();
			if (emITEM_BIND_GET == nBindType || emITEM_BIND_OWN == nBindType)
				rcItem.Bind();						// 获得时绑定

			for (INT i = 0; i < countof(g_aeBagRoom); ++i)
			{
				KItemRoom& rcRoom = m_acRoom[g_aeBagRoom[i]];
				INT nX = 0, nY = 0;
				if (rcRoom.GetFreeCell(nX, nY) && Add(m_nItemIdxArr[nCountNum], g_aeBagRoom[i], eWay, nX, nY, 0, 0, 0, pszCurrencyType) > 0)
				{
					nStack = m_nItemIdxArr[nCountNum];
					break;
				}
			}
			newItemCount -= Item[nItemIdx].GetMaxCount();
		}
	}
EXIT1:
	if (nStack > 0)
	{
		//m_rcPlayer.CumulateValue(&rcItem);
		if (bMsg2Player)
		{
			//	// 发消息给玩家
			//SHOW_MSG_SYNC sMsg;
			//sMsg.ProtocolType = s2c_msgshow;
			//sMsg.m_wMsgID = enumMSG_ID_GET_ITEM;
			//sMsg.adwParam[0] = Item[nStack].GetID();
			//sMsg.adwParam[1] = nCount | ((INT)eWay << 16);
			//sMsg.m_wLength = sMsg.MainSize() + sizeof(DWORD) * 2 - 1;
			//g_pCoreServer->SendData(m_rcPlayer.m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			CHAR szBuffer[MAX_PATH];
			snprintf(szBuffer, sizeof(szBuffer), LANG_001, nItemCount, szItemName);
			g_ChatChannelManager.ProcessRespond(trLocalSys,
				emKSYSTEMMSG_NORMAL,
				m_rcPlayer.GetId(),
				LANG_044,
				szBuffer
				);
		}
		return nStack;
	}
	else
	{
		g_cPlayerBaseProcess.DoSyncBlackMsg(&m_rcPlayer, LANG_006);
	}
	return	nStack;
}

BOOL _Index_Comp(const INT nItemIdx1, const INT nItemIdx2)
{
	KItem* pItem1 = &Item[nItemIdx1];
	KItem* pItem2 = &Item[nItemIdx2];

	return pItem1->GetIndex() < pItem2->GetIndex();
}

// 得到背包里所有物品nIdx list
VOID KPlayerItem::GetBagUsedItem(std::vector<INT>& vecUsedItem)
{
	INT nItemIdx = 0;

	vecUsedItem.clear();

	for(INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		KItemRoom& rcRoom = m_acRoom[g_aeBagRoom[i]];
		KItemRoom::KFinder cFind = rcRoom.GetFinder();
		// 遍历本空间所有物品
		while (cFind.Next(KRoomFindCondition::Used))
		{
			nItemIdx = cFind.Index();
			vecUsedItem.push_back(nItemIdx);
		}
	}
}

// 整理背包
VOID KPlayerItem::ArrangeBagItem()
{
	INT nItemIdx = 0;
	INT nIdx = 0;
	KItem* pItem = NULL;
	std::vector<INT> vecUsedItem;

	GetBagUsedItem(vecUsedItem);

	// 能叠加的先叠加到一起
	for(INT i = vecUsedItem.size() - 1; i > 0; --i)
	{
		for(INT j = i - 1; j >= 0; --j)
		{
			KItem* pLeftItem = &Item[vecUsedItem[j]];
			KItem* pRightItem = &Item[vecUsedItem[i]];
			if(pLeftItem->GetIndex() == pRightItem->GetIndex() && pLeftItem->GetMaxCount() > 1)
			{
				KITEM_POS sLeftPos, sRightPos;
				GetItemPos(vecUsedItem[j], sLeftPos);
				GetItemPos(vecUsedItem[i], sRightPos);
				SwitchItem(sRightPos, sLeftPos);
				break;
			}
		}
	}

	GetBagUsedItem(vecUsedItem);

	PROCESS_ERROR(vecUsedItem.size() > 0);

	// 根据道具的index做排序
	sort(vecUsedItem.begin(), vecUsedItem.end(), _Index_Comp);

	for(INT i = 0; i < countof(g_aeBagRoom); ++i)
	{
		KItemRoom& rcRoom = m_acRoom[g_aeBagRoom[i]];
		KItemRoom::KFinder cFind = rcRoom.GetFinder();

		while (cFind.Next(KRoomFindCondition::Any))
		{
			CONST KPos& sPos = cFind.Pos();
			nItemIdx = vecUsedItem[nIdx++];

			KITEM_POS rsTargetPos;
			KITEM_POS rsItemPos;

			rsTargetPos.eRoom = g_aeBagRoom[i];
			rsTargetPos.nX = sPos.nX;
			rsTargetPos.nY = sPos.nY;

			GetItemPos(nItemIdx, rsItemPos);

			if(rsItemPos != rsTargetPos)
			{
				SwitchItem(rsItemPos, rsTargetPos);
			}

			if(nIdx >= (INT)vecUsedItem.size())
				goto EXIT0;
		}
	}

EXIT0:
	return;
}

INT KPlayerItem::AddItemIntoKinBag( INT nItemIdx )
{
	if (nItemIdx <= 0)
		return 0;

	KItem& rcItem = Item[nItemIdx];

	// 寻找一个可用的空间
	INT nX = 0;
	INT nY = 0;
	if (!m_acRoom[emROOM_KIN].GetFreeCell(nX, nY))
		return 0;
	INT nListIdx = Add(nItemIdx, emROOM_KIN, emKADDITEM_KIN, nX, nY);

	return nListIdx;
}

#endif	// #ifdef GAME_SERVER

#ifdef GAME_SERVER

VOID KPlayerItem::SendEquipItemInfo(INT nTargetPlayer)
{
	ASSERT(g_cPlayerMgr.GetByIndex(nTargetPlayer));

	KVIEWITEMINFO* pInfo = NULL;
	// 一个足够大的BUFFER，刚好能装入欲同步的玩家信息和玩家所有的装备
	CHAR szBuffer[sizeof(KVIEW_EQUIP_SYNC) + sizeof(KVIEWITEMINFO) * emEQUIPPOS_NUM];
	KVIEW_EQUIP_SYNC* pSync = (KVIEW_EQUIP_SYNC*)szBuffer;
	pSync->byProtocol = s2c_viewequip;
	GetEquipView(&pSync->sView);

	pSync->wDataLen = sizeof(KVIEW_EQUIP_SYNC) + sizeof(KVIEWITEMINFO) * pSync->sView.byItemCount;
	g_cItemMgr.GetProtocolProcessor()->SendLenData(nTargetPlayer, *pSync, pSync->wDataLen);

	//KVIEW_EQUIP_SYNC sView;
	//GetEquipView(&sView.sView);
	//sView.ProtocolType = s2c_viewequip;
	//INT nSize = sizeof(sView) - sizeof(KVIEWITEMINFO) * (emEQUIPPOS_NUM + emPARTNER_NUM - sView.sView.nItemCount);
	//g_pCoreServer->SendData(Player[nTargetPlayer].m_nNetConnectIdx, &sView, nSize);
}

// 获取装备信息
VOID KPlayerItem::GetEquipView(KPLAYER_EQUIPVIEW* pView)
{
	ASSERT(pView);

	pView->bySex	 = (BYTE)m_rcPlayer.GetSex();
	pView->nPortrait = 0;
	pView->byFaction = (BYTE)m_rcPlayer.GetPrimaryFaction();
	pView->byPKValue = 0;
	pView->byLevel	 = (BYTE)m_rcPlayer.GetLevel();

	strncpy(pView->szRoleName, m_rcPlayer.GetName(), sizeof(pView->szRoleName) - 1);
	pView->szRoleName[sizeof(pView->szRoleName) - 1] = 0;

	KVIEWITEMINFO* pInfo = (KVIEWITEMINFO*)(pView->szBuffer);
	pView->byItemCount = 0;
	for (INT i = 0; i < emEQUIPPOS_NUM/* + emPARTNER_NUM*/; i++)
	{
		INT nIdx = 0;
		nIdx = GetEquip(i);

		if (nIdx > 0)
		{
			Item[nIdx].MakeSyncInfo(*pInfo);
			++pInfo;
			++pView->byItemCount;
		}
		//if (nIdx <= 0)
		//{
		//	memset(pInfo, 0, sizeof(KVIEWITEMINFO));
		//	continue;
		//}
		//Item[nIdx].MakeSyncInfo(*pInfo);		// 生成同步物品基本信息
	}
}

BOOL KPlayerItem::Pay(INT nMoney, INT nPayType)
{
	if (nMoney < 0)
		return FALSE;
	//CheckMoney("BeforePay");
	//if (nMoney > 0 && nPayType != emKPAY_NONE && (nPayType != emKPAY_SHOP || nMoney >= 5000))
	//	KD_COURSE_LOG(m_rcPlayer, emKCOURSE_JXB_CHANGE, "PayWay" << nPayType << "\tCount\t" << nMoney);
	//g_cMOneyMoniLog.OnPay(nMoney);
	BOOL bOk	= CostMoney(nMoney, nPayType);
	DWORD dwNowMoney	= GetTotalMoney();
	SetCheckMoney(dwNowMoney);
	//if (nMoney > 0 && m_rcPlayer.m_eRecordLogType & KPlayer::recordlog_money)
	//	m_rcPlayer.WriteLog("PayMoney[%d]\t-%d\t=%d", nPayType, nMoney, dwNowMoney);
	return bOk;
}

BOOL KPlayerItem::PrePay(INT nMoney)
{
	if (nMoney < 0)
		return FALSE;

	if (Pay(nMoney, emKPAY_PREPAY))
	{
		m_nPrePayMoney = nMoney;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static INT _CalcAddedLevel(UINT uAddedMoney)
{
	if (uAddedMoney >= emKADDMONEY_ALM_MAX * 2)
		return 3;
	if (uAddedMoney >= emKADDMONEY_ALM_MAX)
		return 2;
	if (uAddedMoney >= emKADDMONEY_ALM_2)
		return 1;
	return 0;
}

static INT _CalcAddedTimesLevel(INT nTimes)
{
	if (nTimes >= emKADDMONEY_FREQ_3)
		return 3;
	if (nTimes >= emKADDMONEY_FREQ_2)
		return 2;
	if (nTimes >= emKADDMONEY_FREQ_1)
		return 1;
	return 0;
}

BOOL KPlayerItem::Earn(INT nMoney, INT nEarnType)
{
	if (nMoney <= 0)
		return FALSE;
	CheckMoney("BeforeEarn");
	//if (nMoney > 0 && (emKEARN_FARM != nEarnType || nMoney >= 500))
	//	KD_COURSE_LOG(m_rcPlayer, emKCOURSE_JXB_CHANGE, "EarnWay" << nEarnType << "\tCount\t" << nMoney);
	UINT uCurTime = KSysService::Time(NULL);
	INT nInterVal = (INT)(uCurTime - m_uAddedMoneyTime);
	// 之前的加钱频率 < KD_ADDMONEY_PERSEC_TRACE，归0，重新计时
	if (nInterVal > KD_ADDMONEY_FILTER_SEC && m_uAddedMoney / nInterVal < KD_ADDMONEY_PERSEC_TRACE)
	{
		m_uAddedMoney = 0;
		m_uAddedTimes = 0;
		m_uAddedMoneyTime = uCurTime;
		nInterVal = 0;
		m_nNextAlarmLevel = emKADDMONEY_ALM_1;
	}
	m_uAddedMoney += nMoney;
	m_uAddedTimes++;
	if (m_uAddedMoney >= (UINT)m_nNextAlarmLevel)
	{
		const static LPCTSTR aszWarningLvl[] = {
			"!\t", "!!\t", "!!!\t", "!!!!\t"
		};
		// 设置下一告警等级
		SetAddedMoney((INT)m_uAddedMoney);
		INT nAddedLevel = _CalcAddedLevel(m_uAddedMoney);
		INT nAddedTimesLevel = _CalcAddedTimesLevel(m_uAddedTimes);
		//// 输出告警
		//KD_COURSE_LOG(m_rcPlayer, emKCOURSE_JXB_CHANGE, "AddedMoney" << aszWarningLvl[nAddedLevel] <<
		//	m_uAddedMoney << "\tAddedTimes" << aszWarningLvl[nAddedTimesLevel] << m_uAddedTimes <<
		//	"\tStartTime:" << m_uAddedMoneyTime << "\tInterVal:" << nInterVal);
	}
	//g_cMOneyMoniLog.OnEarn(nMoney, uCurTime);
	BOOL bOk	= AddMoney(emPURSE_CASH, nMoney);
	if (nEarnType !=  emKEARN_PLAYER_TRADE && nEarnType != emKEARN_MAIL && nEarnType != emKEARN_JBEXCHANGE_CASH
		&& nEarnType != emKEARN_JBEXCHANGE_BACK && nEarnType != emKEARN_STALL_SELL && nEarnType != emKEARN_STALL_BUY
		&& nEarnType != emKEARN_GIVE && nEarnType != emKEARN_SCRIPT_TONG_FUN && nEarnType != emKEARN_SCRIPT_TONG_DISPAND
		&& nEarnType != emKEARN_SCRIPT_EXCHANGE_BUYFAIL && nEarnType != emKEARN_SCRIPT_DRAWBANK)
	{
		OnCoinChanged(2, nMoney);	// 2表示类型是银两
	}

	DWORD dwNowMoney	= GetTotalMoney();
	SetCheckMoney(dwNowMoney);
	//if (m_rcPlayer.m_eRecordLogType & KPlayer::recordlog_money)
	//	m_rcPlayer.WriteLog("EarnMoney[%d]\t+%d\t=%d", nEarnType, nMoney, dwNowMoney);
	return bOk;
}

#endif	// #ifdef GAME_SERVER

BOOL KPlayerItem::AddBase(INT nItem, INT nListIdx, CONST KITEM_POS& sPos)
{
	if (nListIdx <= 0 || nItem <= 0 || nItem >= MAX_ITEM)
		return	FALSE;

	m_asList[nListIdx].nIdx = nItem;
	m_cSwitch.DropBase(sPos, nListIdx);
	return	TRUE;
}

BOOL KPlayerItem::AddEquip(INT nItem, INT nListIdx, INT nPos /* = -1 */)
{
	if (nListIdx <= 0 || nItem <= 0 || nItem >= MAX_ITEM)
		return	FALSE;

	if (nPos < 0)
	{	// 自动匹配位置
		KItem& rcItem = Item[nItem];
		INT nDetail = rcItem.GetDetailType();
		if (!KD_ISEQUIP(rcItem.GetGenre()) || nDetail <= ITEM_INVALID_TYPE || nDetail >= equip_detailnum)
			return	FALSE;
		nPos = g_anEquipPos[nDetail];
	}
	else if (nPos >= emEQUIPPOS_NUM)
	{
		return	FALSE;
	}

	m_asList[nListIdx].nIdx = nItem;
	KITEM_POS sPos;
	sPos.eRoom	= emROOM_EQUIP;
	sPos.nX		= nPos;
	sPos.nY		= 0;
	m_cSwitch.DropEquip(sPos, nListIdx);
	// 因为装备物品时有时调switch，有时调这里，无语，先这样了，为了实现阵法改变时通知
	m_cSwitch.m_SwitchNotify.SwitchBegin();
	m_cSwitch.m_SwitchNotify.OnPickNotify(m_asList[nListIdx].sPos, nListIdx);
	m_cSwitch.m_SwitchNotify.SwitchEnd();

	return	TRUE;
}

BOOL KPlayerItem::AddEquipEx(INT nItem, INT nListIdx, INT nPos)
{
	BOOL bResult = FALSE;
	m_asList[nListIdx].nIdx = nItem;
	KITEM_POS sPos;
	sPos.eRoom	= emROOM_EQUIPEX;
	sPos.nX		= nPos;
	sPos.nY		= 0;
	PROCESS_ERROR(nPos >= 0 && nPos < emEQUIPPOS_NUM);
	PROCESS_ERROR(nListIdx > 0);
	PROCESS_ERROR(nItem > 0 && nItem < MAX_ITEM);
	m_cSwitch.DropEquipEx(sPos, nListIdx);
	m_cSwitch.m_SwitchNotify.SwitchBegin();
	m_cSwitch.m_SwitchNotify.OnPickNotify(m_asList[nListIdx].sPos, nListIdx);
	m_cSwitch.m_SwitchNotify.SwitchEnd();
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerItem::AddExtBag(INT nItem, INT nListIdx, INT nPos /* = -1 */)
{
	if (nListIdx <= 0 || nItem <= 0 || nItem >= MAX_ITEM)
		return	FALSE;

	if (nPos < 0)
	{	// 自动找空位放置
		INT n = 0;
		if (!m_acRoom[emROOM_EXTBAGBAR].GetFreeCell(nPos, n))
			return	FALSE;		// 满了
	}
	else if (nPos >= emEXTBAGPOS_NUM)
	{
		return	FALSE;
	}

	m_asList[nListIdx].nIdx = nItem;
	KITEM_POS sPos;
	sPos.eRoom = emROOM_EXTBAGBAR;
	sPos.nX	= nPos;
	sPos.nY = 0;
	m_cSwitch.DropExtBagBar(sPos, nListIdx);
	return	TRUE;
}

BOOL KPlayerItem::CanResizeExtBag(KE_ITEM_ROOM emRoom, INT nWidth, INT nHeight)
{
	BOOL bRet = FALSE;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	bRet = m_acRoom[emRoom].CanResize(nWidth, nHeight);
	QCONFIRM_RET_FALSE(bRet);

	g_cScriptManager.SetMe((QLunaBase*)m_rcPlayer.GetScriptInterface());

	// 扣钱等操作
	cSafeScript->CallTableFunction( "Item", "CanResizeExtBag", 1, "oddd",  m_rcPlayer.GetScriptInterface(), emRoom, nWidth, nHeight);
	bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KPlayerItem::ResizeExtBag(KE_ITEM_ROOM emRoom, INT nWidth, INT nHeight)
{
	BOOL bRet = FALSE;
	ASSERT(m_acRoom[emRoom].CanResize(nWidth,nHeight));

	LOG_PROCESS_ERROR(m_acRoom[emRoom].Resize(nWidth, nHeight));

	m_rcPlayer.SetExtBag(nWidth);

	bRet = TRUE;
EXIT0:
	return bRet;
}

BOOL KPlayerItem::RemoveBase(INT nListIdx, INT nX /* = -1 */, INT nY /* = -1 */)
{
	if (nListIdx <= 0)
		return	FALSE;

	INT nItem = m_asList[nListIdx].nIdx;
	if (nItem <= 0 || nItem >= MAX_ITEM)
		return	FALSE;

	if (nX >= 0 && m_asList[nListIdx].sPos.nX != nX || nY >= 0 && m_asList[nListIdx].sPos.nY != nY)
		return	FALSE;

	m_cSwitch.PickBase(m_asList[nListIdx].sPos, nListIdx);
	m_asList[nListIdx].nIdx = 0;
	return	TRUE;
}

BOOL KPlayerItem::RemoveEquip(INT nListIdx, INT nPos /* = -1 */)
{
	if (nListIdx <= 0)
		return	FALSE;

	INT nItem = m_asList[nListIdx].nIdx;
	if (nItem <= 0 || nItem >= MAX_ITEM)
		return	FALSE;

	if (nPos >= 0 && (m_asList[nListIdx].sPos.nX != nPos || m_asList[nListIdx].sPos.nY != 0))
		return	FALSE;

	m_cSwitch.PickEquip(m_asList[nListIdx].sPos, nListIdx);

	m_cSwitch.m_SwitchNotify.SwitchBegin();
	m_cSwitch.m_SwitchNotify.OnPickNotify(m_asList[nListIdx].sPos, nListIdx);
	m_cSwitch.m_SwitchNotify.SwitchEnd();

	m_asList[nListIdx].nIdx = 0;

	return	TRUE;
}

BOOL KPlayerItem::RemoveExtBag(INT nListIdx, INT nPos /* = -1 */)
{
	if (nListIdx <= 0)
		return	FALSE;

	INT nItem = m_asList[nListIdx].nIdx;
	if (nItem <= 0 || nItem >= MAX_ITEM)
		return	FALSE;

	if (nPos >= 0 && (m_asList[nListIdx].sPos.nX != nPos || m_asList[nListIdx].sPos.nY != 0))
		return	FALSE;

	m_cSwitch.PickExtBagBar(m_asList[nListIdx].sPos, nListIdx);
	m_asList[nListIdx].nIdx = 0;
	return	TRUE;
}

BOOL KPlayerItem::CanUseItem(CONST KItem& rcItem) CONST
{
	if (rcItem.IsInvalid())
		return	FALSE;		// 失效道具不能使用

	if (IS_BZONE_PLAYER(m_rcPlayer.GetId()) && !rcItem.CanUseInBZone())
		return FALSE;		// 跨服状态且非跨服道具不能使用

	if (KD_ISEQUIP(rcItem.GetGenre()))
	{	// 装备检查是否可以装备
		if (!CanEquip(rcItem))
			return	FALSE;
	}
	else
	{	// 非装备要检查ReqLevel
		if (m_rcPlayer.GetLevel() < rcItem.GetReqLevel())
			return	FALSE;
	}

	return	TRUE;
}

BOOL KPlayerItem::CanUnequip() CONST
{
	//if (m_rcPlayer.CheckPlayerAccountLockState())
	//	return FALSE;
	return TRUE;
}

BOOL KPlayerItem::CanEquip(CONST KItem &rcEquip, INT nEquipPos /* = -1 */) CONST
{
	if (!KD_ISEQUIP(rcEquip.GetGenre()))
		return	FALSE;

	if (nEquipPos < 0)
	{
		nEquipPos = GetEquipPos(rcEquip.GetDetailType());
	}

	if (!Fit(rcEquip, nEquipPos))
		return	FALSE;

	return	!TestInvalid(rcEquip);		// 如果无效则不能装备
}

BOOL KPlayerItem::Fit(CONST KItem& rcEquip, INT nEquipPos) CONST
{
	if (!KD_ISEQUIP(rcEquip.GetGenre()))
		return	FALSE;

	INT nType = rcEquip.GetDetailType();
	if (nType <= ITEM_INVALID_TYPE || nType > equip_detailnum)
		return	FALSE;

	return	nEquipPos == g_anEquipPos[nType];
}

VOID KPlayerItem::SetTimeout(INT nItemIdx, CONST KITEM_TIMEOUT* pTimeout)
{
	ASSERT(nItemIdx > 0 && pTimeout && pTimeout->dwTimeout > 0);
	m_cTimeoutCtrl.SetTimeout(nItemIdx, pTimeout);
}
BOOL KPlayerItem::DelTimeout(INT nItemIdx)
{
	BOOL bResult	= FALSE;
	CONST KITEM_TIMEOUT sTimeOut	= {emKITEM_TIMEOUTTYPE_ABSOLUTE, 0};

	LOG_PROCESS_ERROR(nItemIdx > 0);
	LOG_PROCESS_ERROR(nItemIdx < MAX_ITEM);

	m_cTimeoutCtrl.DelTimeout(nItemIdx);
	Item[nItemIdx].SetTimeout(&sTimeOut);

	bResult	= TRUE;

EXIT0:
	return bResult;
}

CONST KITEM_TIMEOUT* KPlayerItem::GetTimeout(INT nItemIdx) CONST
{
	ASSERT(nItemIdx > 0);
	return Item[nItemIdx].GetTimeout();
}

VOID KPlayerItem::CheckTimeout(time_t tNow, INT nInterval)
{
	//ASSERT(tNow > 0 && nInterval > 0);
	m_cTimeoutCtrl.CheckTimeout(tNow, nInterval);
}

VOID KPlayerItem::SetRecycleItemDate(INT nListIdx, WORD wRecycleDate)
{
	if (nListIdx < 0 && nListIdx >= KD_MAX_PLAYER_ITEM)
		return;
	m_asList[nListIdx].wRecycleDate = wRecycleDate;
}

INT KPlayerItem::GetCoin(VOID) CONST
{
	//#ifdef GAME_SERVER
	//	return m_rcPlayer.GetJbCoin();
	//#else
	//	return m_nJbCoin;
	//#endif
	return 0;
}

//INT KPlayerItem::GetBindCoin() CONST
//{
//	return m_rcPlayer.m_cPlayerTask.GetTaskValue(TASKVALUE_BINDCOIN);
//}

BOOL KPlayerItem::CheckItemInBag(KItem* pItem)
{
	KITEM_POS sPos;
	INT nItemIdx = pItem->GetGameIndex();
	if (!GetItemPos(nItemIdx, sPos)) return FALSE;
	if(!IsBagRoom(sPos.eRoom)) return FALSE;
	return TRUE;
}

INT KPlayerItem::CalcAllEquipValue(VOID) 
{
	INT nValueTotal = 0;
	KItemRoom::KFinder cFinder = m_acRoom[emROOM_EQUIP].GetFinder();
	while (cFinder.Next(KRoomFindCondition::Used))
		nValueTotal += Item[cFinder.Index()].GetValue();
	return nValueTotal;
}

#ifdef GAME_SERVER
VOID KPlayerItem::SetAddedMoney( INT nValue )
{
	m_uAddedMoney = (UINT)nValue;
	if (m_uAddedMoney >= emKADDMONEY_ALM_MAX)
		m_nNextAlarmLevel = (m_uAddedMoney / emKADDMONEY_ALM_MAX + 1) * emKADDMONEY_ALM_MAX;
	else if (m_uAddedMoney >= emKADDMONEY_ALM_2)
		m_nNextAlarmLevel = emKADDMONEY_ALM_MAX;
	else if (m_uAddedMoney >= emKADDMONEY_ALM_1)
		m_nNextAlarmLevel = emKADDMONEY_ALM_2;
	else
		m_nNextAlarmLevel = emKADDMONEY_ALM_1;
}

INT	KPlayerItem::MakeItem(CONST TDBItemData *pItemData)
{
	ASSERT(pItemData);

	BOOL bResult = FALSE;
	KITEM_INDEX sIndex;
	sIndex.nGenre		= pItemData->wGenre;
	sIndex.nDetailType	= pItemData->wDetailType;
	sIndex.nParticular	= pItemData->wParticularType;
	sIndex.nLevel		= pItemData->byLevel;

	KItem cItem;
	cItem.m_sBaseData.sIndex				= sIndex;
	cItem.m_GeneratorParam.uRandomSeed		= pItemData->dwRandSeed;
	cItem.m_GeneratorParam.uRandomSeedEx	= pItemData->dwRandSeedEx;
	cItem.m_dwCurCount						= pItemData->wStackCount;
	cItem.m_nEnhTimes						= pItemData->byEnhance;
	cItem.SetBuyPrice(pItemData->dwBuyPrice);

	memset(cItem.m_sBaseData.aryExtParam, 0, sizeof(cItem.m_sBaseData.aryExtParam));
	for (INT i = 0; i < 3; ++i)
		cItem.SetExtParam(i, pItemData->arydwExtParam[i]);

	memcpy(cItem.m_aryStoneInfo, pItemData->arydwStoneInfo, sizeof(cItem.m_aryStoneInfo));

	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		cItem.m_aryAppendAttrib[i].nAttribType = pItemData->arywAppendData[2 * i];
		cItem.m_aryAppendAttrib[i].nValue[0] = pItemData->arywAppendData[2 * i + 1];
		cItem.m_aryAppendAttrib[i].nValue[1] = cItem.m_aryAppendAttrib[i].nValue[0];
	}

	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		cItem.m_arySaveAppendAttrib[i].nAttribType = pItemData->arywSaveAppendData[2 * i];
		cItem.m_arySaveAppendAttrib[i].nValue[0] = pItemData->arywSaveAppendData[2 * i + 1];
		cItem.m_arySaveAppendAttrib[i].nValue[1] = cItem.m_arySaveAppendAttrib[i].nValue[0];
	}

	memcpy(cItem.m_aryAppendQuality, pItemData->aryAppendQuality, sizeof(pItemData->aryAppendQuality));
	memcpy(cItem.m_arySaveAppendQuality, pItemData->arySaveAppendQuality, sizeof(pItemData->arySaveAppendQuality));

	KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();

	CONST KItem::KSTONE_INFO* pStoneInfo = cItem.m_aryStoneInfo;
	CONST KMagicAttrib*	pAppendAttr = cItem.m_aryAppendAttrib;

	bResult = pItemGen->Gen_Item(
		&cItem,
		cItem.GetGenre(),
		cItem.GetDetailType(),
		cItem.GetParticular(),
		cItem.GetLevel(),
		cItem.GetEnhanceTimes(),
		pStoneInfo,
		pAppendAttr,
		NULL,
		cItem.GetRandSeed(),
		cItem.GetRandSeedEx(),
		FALSE,
		FALSE,
		&pItemData->sGuid
		);
	//}

	// 生成物品失败应返回错误中止
	if (!bResult)
	{
		_ASSERT(FALSE);
		g_SysAlarmLogFile.LogRecordVar(
			LOG_LEVEL_ERROR, 
			"DBLoading: Generating Item Failed!, G:%d, D:%d, P:%d, Level:%d\n", 
			pItemData->wGenre, 
			pItemData->wDetailType,
			pItemData->wParticularType,
			pItemData->byLevel
			);
		return 0;
	}

	// 绑定
	if (pItemData->byStateFlags & 0x02)
		cItem.Bind(TRUE);

	// 从家族仓库借来
	if (pItemData->byStateFlags & (0x01 << 4))
		cItem.SetRentFromKin(TRUE);

	// added by liyun 记录装备被借出到哪个家族仓库了
	cItem.SetKinReptoryID(pItemData->dwKinRepteroryId);

	cItem.SetCustom(
		(KItem::KE_CUSTOM_TYPE)pItemData->byCustomType,
		(LPCSTR)pItemData->abyCustom,
		sizeof(pItemData->abyCustom)
		);	// 自定义字符串

	if (cItem.GetMaxCount() > 1)
		cItem.SetCount(pItemData->wStackCount);
	else
		cItem.SetCount(1);

	cItem.SetGenTime(pItemData->dwGenTime);
	cItem.m_GeneratorParam.uRandomSeed = pItemData->dwRandSeed;
	cItem.m_GeneratorParam.uRandomSeedEx = pItemData->dwRandSeedEx;

	DupedDef::OP_RESULT op_ret = DupedDef::ret_normal;
	INT nIndex = g_cItemMgr.Add(&cItem, m_rcPlayer.GetPlayerIndex(), g_dupedItemSetting.GetOpLevel(), op_ret);

	// 复制物品，已被删除，不再加入到玩家物品列表中
	if (DupedDef::ret_del_all == op_ret || DupedDef::ret_del_bymark == op_ret)
		return 0;

	if (nIndex <= 0) 
	{
		if (bResult)
		{
			QLogPrintf(LOG_INFO,"g_cItemMgr:%d, Load item failed!\n", g_cItemMgr.GetItemCount());
			return 0;
		}
	}
	else
	{
		// 超时限制
		if (pItemData->dwTimeout > 0 && pItemData->byTimeoutType < emKITEM_TIMEOUTTYPE_COUNT)
		{
			KITEM_TIMEOUT sTimeout;
			sTimeout.emType = static_cast<KE_ITEM_TIMEOUTTYPE>(pItemData->byTimeoutType);
			sTimeout.dwTimeout = pItemData->dwTimeout;
			Item[nIndex].SetTimeout(&sTimeout);
		}
	}

	return nIndex;
}

BOOL KPlayerItem::LoadItemData(CONST TDBItemData *pPlayerItem, INT nCount)
{
	//pPlayerItem->_.Check();
	KPlayer* pPlayer = NULL;
	DWORD dwExtBagSize = 0;
	ASSERT(pPlayerItem && nCount > 0);

	// 初始化扩展背包1
	pPlayer = &m_rcPlayer;
	LOG_PROCESS_ERROR(pPlayer);
	dwExtBagSize = pPlayer->GetExtBag();
	if (dwExtBagSize > 0)
	{
		m_acRoom[g_aeExtBagPos2Room[0]].Init(KD_ROOM_EXTBAG1_MAX_WIDTH, KD_ROOM_EXTBAG1_MAX_HEIGHT, (INT)dwExtBagSize, KD_ROOM_EXTBAG1_MAX_HEIGHT);
	}

	for (INT i = 0; i < nCount; i ++)
	{
		CONST TDBItemData* pItemData = &pPlayerItem[i];

		KE_ITEM_ROOM eRoom = (KE_ITEM_ROOM)pItemData->byPlaceRoom;

		INT nIndex = MakeItem(pItemData);

		if (nIndex <= 0)
		{
			LPCSTR pszName = "";
			KITEM_INDEX sIndex;
			sIndex.nGenre = pItemData->wGenre;
			sIndex.nDetailType = pItemData->wDetailType;
			sIndex.nParticular = pItemData->wParticularType;
			sIndex.nLevel = pItemData->byLevel;
			CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
			CONST KLibOfBPT *pLibBPT = pItemGen->GetLibOfBPT();
			CONST KBASICPROP_BASEDATA *pBaseData = pLibBPT->GetBasicProp(sIndex);
			if (pBaseData)
				pszName = pBaseData->szName;

			g_SysAlarmLogFile.LogRecordVar(
				LOG_LEVEL_ERROR,
				"[ITEM]\tLoad Error: <%s(%d,%d,%d)>, Pos(%d, %d, %d), CurPlayer(%s,%s)",
				pszName,
				pItemData->wGenre,
				pItemData->wDetailType,
				pItemData->wParticularType,
				pItemData->byPlaceRoom,
				pItemData->byPlaceX,
				pItemData->byPlaceY,
				m_rcPlayer.m_cPlayerServer.GetAccount(),
				m_rcPlayer.GetName()
				);

			return FALSE;				// 失败则不允许玩家上线，以免道具丢失
		}


		if (eRoom < 0)
		{
			ASSERT(FALSE);
			continue;
		}

		switch (eRoom)
		{
		case emROOM_MAIL:
			ASSERT(FALSE);
			break;
		default:
			break;
		}

		INT nListIdx = Add(nIndex, (KE_ITEM_ROOM)pItemData->byPlaceRoom, emKADDITEM_LOAD, pItemData->byPlaceX, pItemData->byPlaceY,
			pItemData->dwPrice,pItemData->dwCount, ((pItemData->byStateFlags>>2)&0x01));

		if (nListIdx <= 0)
		{
			g_cItemMgr.Remove(nIndex);

			LPCSTR pszName = "";
			KITEM_INDEX sIndex;
			sIndex.nGenre = pItemData->wGenre;
			sIndex.nDetailType = pItemData->wDetailType;
			sIndex.nParticular = pItemData->wParticularType;
			sIndex.nLevel = pItemData->byLevel;
			CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
			CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
			CONST KBASICPROP_BASEDATA* pBaseData = pLibBPT->GetBasicProp(sIndex);
			if (pBaseData)
				pszName = pBaseData->szName;

			g_SysAlarmLogFile.LogRecordVar(
				LOG_LEVEL_ERROR,
				"[ITEM]\tLoad Error: <%s(%d,%d,%d)>, Pos(%d, %d, %d), CurPlayer(%s,%s)",
				pszName,
				pItemData->wGenre,
				pItemData->wDetailType,
				pItemData->wParticularType,
				pItemData->byPlaceRoom,
				pItemData->byPlaceX,
				pItemData->byPlaceY,
				m_rcPlayer.m_cPlayerServer.GetAccount(),
				m_rcPlayer.GetName()
				);

			return FALSE;				// 失败则不允许玩家上线，以免道具丢失
		}
	}
	return TRUE;
EXIT0:
	return FALSE;
}

BOOL KPlayerItem::OnDbLoad( LPCBYTE pData, SIZE_T uSize )
{
	ASSERT(uSize % sizeof(TDBItemData) == 0);
	INT nItemCount = uSize / sizeof(TDBItemData);
	CONST TDBItemData* pItemData = (TDBItemData*)pData;

	if (NULL == pData)
		return	TRUE;
	if (nItemCount == 0)
		return	TRUE;

	// 玩家身上的复制装备查找并执行删除
	m_dupedItemDeal.ClearAll();		// 先清空
	m_dupedItemDeal.SetMarkFlag(DupedDef::mark_dupe_limit);	// 与自己身上的物品有雷同复制的标记
	if (!LoadItemData(pItemData, nItemCount))
	{
		RemoveAll();	// TODO: 这样做也不够安全 如果后续装载失败 仍然会泄露Item 并且可能有别的资源泄露现象
		return FALSE;
	}

	m_vtrLoadItemBuffer.clear();

	return TRUE;
}

VOID KPlayerItem::SaveItemData(CONST KItem *pItem, TDBItemData *pItemData, KLISTITEM* pListItem)
{
	SaveItemData(pItem, pItemData, pListItem->sPos.eRoom, pListItem->sPos.nX, pListItem->sPos.nY);
}

VOID KPlayerItem::SaveItemData(CONST KItem *pItem, TDBItemData *pItemData, INT nPlace, INT nX, INT nY)
{
	//玩家物件基本信息
	pItemData->wGenre				= (WORD)pItem->GetGenre();
	pItemData->wDetailType			= (WORD)pItem->GetDetailType();
	pItemData->wParticularType		= (WORD)pItem->GetParticular();

	pItemData->dwGenTime			= (DWORD)pItem->GetGenTime();
	pItemData->dwRandSeed			= (DWORD)pItem->GetRandSeed();
	pItemData->dwRandSeedEx			= (DWORD)pItem->GetRandSeedEx();
	pItemData->wStackCount			= (WORD)pItem->GetCount();
	pItemData->byPlaceRoom			= (BYTE)nPlace;	// m_cPlayerItem.m_asList[nIdx].nPlace;
	pItemData->byPlaceX 			= (BYTE)nX;		// m_cPlayerItem.m_asList[nIdx].nX;
	pItemData->byPlaceY 			= (BYTE)nY;		// m_cPlayerItem.m_asList[nIdx].nY;

	pItemData->byLevel				= (BYTE)pItem->GetLevel();
	pItemData->byEnhance			= (BYTE)pItem->GetEnhanceTimes();
	pItemData->byStateFlags			= 0;
	CONST KITEM_TIMEOUT* pTimeout	= pItem->GetTimeout();
	pItemData->byTimeoutType		= (BYTE)pTimeout->emType;
	pItemData->dwTimeout			= pTimeout->dwTimeout;
	pItemData->byCustomType			= (BYTE)pItem->GetCustomType();
	pItemData->sGuid				= pItem->GetGuid();
	pItemData->dwBuyPrice			= pItem->GetBuyPrice();
	memcpy(pItemData->abyCustom, pItem->GetCustomString(), sizeof(pItemData->abyCustom));
	memcpy(pItemData->arydwStoneInfo, pItem->m_aryStoneInfo, sizeof(pItem->m_aryStoneInfo));

	for(INT i = 0; i < emITEM_COUNT_APPEND; ++i)
	{
		pItemData->arywAppendData[i * 2] = pItem->m_aryAppendAttrib[i].nAttribType;
		pItemData->arywAppendData[i * 2 + 1] = pItem->m_aryAppendAttrib[i].nValue[0];

		pItemData->arywSaveAppendData[i * 2] = pItem->m_arySaveAppendAttrib[i].nAttribType;
		pItemData->arywSaveAppendData[i * 2 + 1] = pItem->m_arySaveAppendAttrib[i].nValue[0];
	}

	memcpy(pItemData->aryAppendQuality, pItem->m_aryAppendQuality, sizeof(pItem->m_aryAppendQuality));
	memcpy(pItemData->arySaveAppendQuality, pItem->m_arySaveAppendQuality, sizeof(pItem->m_arySaveAppendQuality));

	pItemData->dwCount	= 0;
	pItemData->dwPrice	= 0;
	INT nIdx = pItem->GetGameIndex();

	//if (!pItem->IsForbitTrade() && nIdx > 0) // 优化：只对可以交易物品做检测
	//{
	//	DWORD dwItemId	= pItem->GetID();
	//	KPlayerStall::KStallInfo* pStallInfo =  m_cPlayerStall.GetOfferReq(dwItemId);
	//	if (pStallInfo)
	//	{
	//		pItemData->dwCount	= pStallInfo->nCount;
	//		pItemData->dwPrice	= pStallInfo->nPrice;
	//		if (pStallInfo->bIsSell)
	//			pItemData->byStateFlags |= 4;
	//	}
	//}

	if (pItem->IsBind())
		pItemData->byStateFlags |= 2;

	// added by liyun
	if (pItem->IsRentFromKin())
		pItemData->byStateFlags |= (0x01 << 4);

	// added by liyun
	pItemData->dwKinRepteroryId = pItem->GetKinReptoryID();

	for (INT i = 0; i < 3; ++i)
		pItemData->arydwExtParam[i] = pItem->GetExtParam(i);

	//pItemData->_.Clear();
}

BOOL KPlayerItem::OnDbSave( LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize )
{
	_ASSERT(pBuffer && uBuffSize > 0);
	TDBItemData * pItemData = (TDBItemData*)pBuffer;
	INT	nItemCount = 0;

	typedef std::map<INT, INT> _CheckItemDupedMap;
	typedef std::pair<_CheckItemDupedMap::iterator, bool> _DupedInsert;
	_CheckItemDupedMap mapItemDuped;
	INT nIdx = 0;

	while (TRUE)
	{
		if ((LPBYTE)(pItemData + 1) - pBuffer > (INT)uBuffSize)
		{
			// 缓冲区空间不足
			ASSERT(FALSE);
			break;
		}

		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;

		INT nItemIndex = m_asList[nIdx].nIdx;
		KE_ITEM_ROOM eRoom = m_asList[nIdx].sPos.eRoom;
		if (nItemIndex <= 0 || nItemIndex >= MAX_ITEM || eRoom < 0)
		{
			_ASSERT(FALSE);
			continue;
		}

		switch (eRoom)
		{
		case emROOM_MAIL:
			continue;
			break;
		default:
			break;
		}

		KItem *pItem = &Item[nItemIndex];

		INT nItemBelongPlayerIdx = pItem->GetBelong();
		if (pItem->GetID() < 100 || m_rcPlayer.GetPlayerIndex() != nItemBelongPlayerIdx)	// 不属于玩家自身的物品，不允许保存!!
		{
			if (nItemBelongPlayerIdx <= 0 || nItemBelongPlayerIdx >=  MAX_PLAYER)
				nItemBelongPlayerIdx = 0;

			// todo [7/16/2012 simon]
			//g_SysAlarmLogFile.LogRecordVar(
			//	LOG_LEVEL_ERROR,
			//	"[ITEM]\tSave Error: <%s(%d,%d,%d)>, ID(%d), Pos(%d), TradeState(%d),"
			//	"CurPlayer(%s,%s), BelongPlayer(%s,%s)",
			//	pItem->GetName(), 
			//	pItem->GetGenre(),
			//	pItem->GetDetailType(),
			//	pItem->GetParticular(),
			//	pItem->GetID(),
			//	eRoom,
			//	m_cMenuState.m_nState,
			//	m_AccountName,
			//	m_PlayerName, 
			//	Player[nItemBelongPlayerIdx].m_AccountName,
			//	Player[nItemBelongPlayerIdx].GetPlayerName()
			//	);

			continue;
		}

		//临时物品不存档
		if (pItem->IsTemporary())
			continue;

		// ---------------------------------------
		// 若在存盘时，出现是竟是同一编号的贵重物品，复制的！！
		if (pItem->IsValuable())
		{
			_DupedInsert ret = mapItemDuped.insert(_CheckItemDupedMap::value_type(nItemIndex, 1));

			// Map中原本已存在此Key(nItemIndex)，没有复制装备，按正常流程走
			if (false == ret.second)
			{
				//pItemData->btexparam1 = 0;
				//pItemData->btexparam1 = 1;
				//pItemData->wexparam = 0;

				// todo [7/16/2012 simon]
				//g_SysAlarmLogFile.LogRecordVar(
				//	LOG_LEVEL_ERROR,
				//	"[ITEM]\tSame Item In Player(%s, %s): <%s(%d,%d,%d)>, GenTime:%u, RandSeed:%u",
				//	m_AccountName,
				//	m_PlayerName,
				//	pItem->GetName(),
				//	pItem->GetGenre(),
				//	pItem->GetDetailType(),
				//	pItem->GetParticular(),
				//	pItem->GetGenTime(),
				//	pItem->GetRandSeed()
				//	);
				// 不再保存此物品
				continue;
			}
		}
		// ---------------------------------------
		KLISTITEM* pListItem = &m_asList[nIdx];
		SaveItemData(pItem, pItemData, pListItem);
		pItemData++;
		nItemCount++;
	}

	uUsedSize = nItemCount * sizeof(TDBItemData);

	//pItemData->_.Clear();
	return TRUE;
}

void KPlayerItem::RemoveAllRentItem()
{
	KItem*                                  pItem       = NULL;
	KE_ITEM_ROOM                            nItemRoom   = emROOM_NONE;
	INT                                     nRoomWidth  = 0;
	INT                                     nRoomHeight = 0;
	KITEM_POS                               cItemPos;

	// 3个数字为1组，第1个值为背包空间类型，第2个值为空间宽度，第3个值为空间高度
	INT                                     nItemRoomAry[] = {
		emROOM_EQUIP,          KD_ROOM_EQUIP_WIDTH,           KD_ROOM_EQUIP_HEIGHT,
		emROOM_EQUIPEX,        KD_ROOM_EQUIPEX_WIDTH,         KD_ROOM_EQUIPEX_HEIGHT,
		emROOM_MAINBAG,        KD_ROOM_MAINBAG_WIDTH,         KD_ROOM_MAINBAG_HEIGHT,
		emROOM_EXTBAGBAR,      KD_ROOM_EXTBAGBAR_WIDTH,       KD_ROOM_EXTBAGBAR_HEIGHT,
		// emROOM_EXTBAG,        KD_ROOM_EXTBAG_WIDTH,          KD_ROOM_EXTBAG_HEIGHT,
		emROOM_MAIL,           KD_ROOM_MAIL_WIDTH,            KD_ROOM_MAIL_HEIGHT,
		emROOM_KIN,            KD_ROOM_KIN_WIDTH,             KD_ROOM_KIN_WIDTH,			

		-1 // 结束标记
	};

	for (int i = 0; i < sizeof(nItemRoomAry) / sizeof(nItemRoomAry[0]); i += 3)
	{
		if (nItemRoomAry[i] < 0) break;

		nItemRoom = (KE_ITEM_ROOM)nItemRoomAry[i];
		nRoomWidth = nItemRoomAry[i + 1];
		nRoomHeight = nItemRoomAry[i + 2];

		for (int j = 0; j < nRoomWidth; ++j)
		{
			for (int k = 0; k < nRoomHeight; ++k)
			{
				cItemPos.eRoom = nItemRoom;
				cItemPos.nX = j;
				cItemPos.nY = k;

				pItem = KPlayerItem::GetItemFromItemPos(&cItemPos, &m_rcPlayer);
				if (pItem && pItem->IsRentFromKin())
				{
					Remove(pItem->GetGameIndex(), emKLOSEITEM_RETURN_TO_KIN_REPERTORY);
				}
			}
		}
	}
}

// 存储装备离线数据
BOOL KPlayerItem::SaveEquipDelayData(KDelayRoleData* pDelayData)
{
	KDelayEquipData* pData = NULL;
	for (INT i = 0; i < emEQUIPPOS_NUM; i++)
	{
		INT nIdx = GetEquip(i);
		if (nIdx <= 0)
		{
			memset(&(pDelayData->aryEquips[i]), 0, sizeof(KDelayEquipData));
			continue;
		}

		pData = &(pDelayData->aryEquips[i]);
		KItem* pEquip = &Item[nIdx];

		pData->nDetail = pEquip->GetDetailType();
		pData->nParticular = pEquip->GetParticular();
		pData->byLevel = (BYTE)pEquip->GetLevel();

		pData->nFightScore = pEquip->GetFightScore();
		pData->byEnhTimes = (BYTE)pEquip->GetEnhanceTimes();

		pData->sBaseAttr.byAttr = (BYTE)pEquip->m_aryBaseAttrib[0].nAttribType;
		pData->sBaseAttr.nValue = pEquip->m_aryBaseAttrib[0].nValue[0] + pEquip->m_sEnhAttrib.nValue[0];

		for(int j = 0; j < 5; ++j)
		{
			pData->aryAppendAttrs[j].byAttr = (BYTE)pEquip->m_aryAppendAttrib[j].nAttribType;
			pData->aryAppendAttrs[j].nValue = pEquip->m_aryAppendAttrib[j].nValue[0];
		}

		memcpy(pData->aryAppendQuality, pEquip->m_aryAppendQuality, sizeof(pEquip->m_aryAppendQuality));

		memcpy(pData->aryStones, pEquip->m_aryStoneInfo, sizeof(pData->aryStones));
	}

	return TRUE;
}


#endif

