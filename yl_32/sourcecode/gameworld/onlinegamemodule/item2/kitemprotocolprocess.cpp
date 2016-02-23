#include "stdafx.h"
#include "kitemprotocolprocess.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayermgr.h"
#include "kitemmgr.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "Runtime/TypeDef.h"

BOOL KItemProtocol::c2sSwitchItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	INT nPlayerIdx;
	BOOL bRet	= FALSE;
	KITEM_POS	sPick, sDrop;
	KITEM_C2S_SWITCH_SYNC* pSync;

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	pSync = (KITEM_C2S_SWITCH_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	//pPlayerItem->c2sSwitchItem((KITEM_C2S_SWITCH_SYNC*)pProtocol);

	sPick.eRoom	= (KE_ITEM_ROOM)pSync->byPickRoom;
	sPick.nX	= pSync->byPickX;
	sPick.nY	= pSync->byPickY;
	sDrop.eRoom	= (KE_ITEM_ROOM)pSync->byDropRoom;
	sDrop.nX	= pSync->byDropX;
	sDrop.nY	= pSync->byDropY;
	
	//printf("SwicthItem-----------------------From : %d,%d To : %d,%d\n",pSync->byPickX,pSync->byPickY,pSync->byDropX,pSync->byDropY);

	do
	{
	INT nPickItem = pPlayerItem->GetItemByPos(sPick);
	INT nDropItem = pPlayerItem->GetItemByPos(sDrop);

	// todo [7/13/2012 simon]
	//// 检查该物品是否属于禁止移动的物品，如果属实，则禁止任何操作
	//if (m_rcPlayer.m_bForbidMoveAndThrowItem)
	//{
	//	SyncItem(nPickItem);
	//	SyncItem(nDropItem);
	//	break;
	//}

	KItem *pcPickItem = nPickItem ? &Item[nPickItem] : NULL;
	KItem *pcDropItem = nDropItem ? &Item[nDropItem] : NULL;
	if (pcPickItem && pcPickItem->IsDupedItem() || pcDropItem && pcDropItem->IsDupedItem())
	break;

	if (IS_BZONE_PLAYER(pPlayer->GetId()) && 
		(pcPickItem && !pcPickItem->CanUseInBZone() || pcDropItem && !pcDropItem->CanUseInBZone()))
	{
		break;
	}

		bRet = pPlayerItem->SwitchItem(sPick, sDrop);			// 执行切换
	} while (FALSE);

	// 处理完毕，发送协议
	KITEM_S2C_SWITCH_END sSync;
	nPlayerIdx = pPlayer->GetPlayerIndex();
	g_cItemMgr.GetProtocolProcessor()->SendData(nPlayerIdx, sSync);

	return bRet;
}

BOOL KItemProtocol::c2sPlayerUseItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	INT nPlayerIdx;
	KPLAYER_USEITEM* pSync;

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	pSync = (KPLAYER_USEITEM*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	INT nItemIdx = pPlayerItem->GetItemByPos((KE_ITEM_ROOM)pSync->byRoom, pSync->byX, pSync->byY);
	if (nItemIdx <= 0)
		return FALSE;

	KPLAYER_USEITEM_RESULT sSync;
	sSync.dwID		= Item[nItemIdx].GetID();
	sSync.bResult	= (BYTE)FALSE;

	do 
	{
		if (!pPlayerItem->IsBagRoom((KE_ITEM_ROOM)pSync->byRoom))
			break;										// 只有背包里的东西可以直接使用

		if (nItemIdx <= 0 || (!pPlayerItem->CheckItemUsable(nItemIdx) && !KD_ISEQUIP(Item[nItemIdx].GetGenre())))
			break;

		INT nCDType = Item[nItemIdx].GetCDType();		// 要在UseItem之前获取，道具有可能被删掉

		if (!pPlayerItem->UseItem(nItemIdx))
			break;

		sSync.bResult = (BYTE)TRUE;
		pPlayerItem->m_cCDTime.Start(nCDType);
	} while (FALSE);

	nPlayerIdx = pPlayer->GetPlayerIndex();
	g_cItemMgr.GetProtocolProcessor()->SendData(nPlayerIdx, sSync);

	pPlayer->FireEvent(emKOBJEVENTTYPE_PLAYER_ITEM_OPS, pPlayer->GetId(), 0, 0);

	return TRUE;
}

//解锁背包
BOOL KItemProtocol::c2sResizeExtBag(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	BOOL bRet = FALSE;
	KPLAYER_RESIZE_EXTBAG* pSync = NULL;
	KPlayerItem* pPlayerItem = NULL;

	pSync = (KPLAYER_RESIZE_EXTBAG*)pProtocol;
	LOG_PROCESS_ERROR(pSync);

	LOG_PROCESS_ERROR(pSync->byRoom == emROOM_EXTBAG);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	bRet = pPlayerItem->CanResizeExtBag((KE_ITEM_ROOM)pSync->byRoom, pSync->byDestWidth, pSync->byDestHeight);
	PROCESS_ERROR(bRet);

	bRet = pPlayerItem->ResizeExtBag((KE_ITEM_ROOM)pSync->byRoom, pSync->byDestWidth, pSync->byDestHeight);
	LOG_PROCESS_ERROR(bRet);

	bRet = TRUE;
EXIT0:
	return bRet;
}

BOOL KItemProtocol::c2sOnReApplyPendants(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	BOOL bRet = FALSE;
	KPlayerItem* pPlayerItem = NULL;

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	pPlayerItem->ReApplyEquipRes();

	bRet = TRUE;
EXIT0:
	return bRet;
}

BOOL KItemProtocol::c2sSplitItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	KITEM_C2S_SPLIT_ITEM* pSync = (KITEM_C2S_SPLIT_ITEM*)pProtocol;

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	pPlayerItem->c2sSplitItem(pSync->dwItemId, pSync->bySplitCount);

	return TRUE;
}

BOOL KItemProtocol::c2sArrangeBagItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	pPlayerItem->ArrangeBagItem();

	// 处理完毕，发送协议
	KITEM_S2C_ARRANGE_END sSync;
	INT nPlayerIdx = pPlayer->GetPlayerIndex();
	g_cItemMgr.GetProtocolProcessor()->SendData(nPlayerIdx, sSync);

	return TRUE;
}

BOOL KItemProtocol::c2sPlayerThrowAwayItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	//if (Player[nIndex].m_cPlayerTrade.IsTrading())
	//	return;
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);
	KITEM_THROWAWAY_SYNC* psSync = (KITEM_THROWAWAY_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(psSync);
	pPlayerItem->ThrowAwayItem(psSync->dwId);
	return TRUE;
}

BOOL KItemProtocol::c2sViewEquip(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KITEM_C2S_VIEW_EQUIP* pView = (KITEM_C2S_VIEW_EQUIP*)pProtocol;
	QCONFIRM_RET_FALSE(pView);

	INT nTgPlayerIdx = pView->m_dwPlayerIdx;
	pPlayerItem->SendEquipItemInfo(nTgPlayerIdx);
	return TRUE;
}

BOOL KItemProtocol::c2sRepairItem(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);
	KITEM_REPAIR_SYNC* pSync = (KITEM_REPAIR_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	KE_REPAIR_TYPE eType = (KE_REPAIR_TYPE)pSync->byType;

	BYTE byCount = pSync->byCount;
	if (byCount <= 0)
	{
		return FALSE;
	}
	std::vector<INT> vctItemIndex;
	for (INT i = 0; i != byCount; ++i)
	{
		INT nItemIdx = pPlayerItem->SearchID(pSync->dwItemID[i]);
		if (nItemIdx > 0)
		{
			vctItemIndex.push_back(nItemIdx);
		}
	}

	if (pPlayerItem->m_BuyInfo.m_nBuyIdx <= 0 && eType != emREPAIR_ITEM)		// zhengyuhua:不是用道具修理则必须在商店内修理
		return FALSE;

	switch (eType)
	{
	case emREPAIR_COMMON:
	case emREPAIR_SPECIAL:
		//pPlayerItem->RepairEquip(vctItemIndex, eType);
		return TRUE;
	case emREPAIR_ITEM:
		{
			INT nIdx = pPlayerItem->SearchID(pSync->dwUseItemID);
			if (nIdx <= 0)
				return FALSE;
			//pPlayerItem->RepairEquip(vctItemIndex, eType, "o", Item[nIdx].m_pLuna);
		}
		return TRUE;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL KItemProtocol::c2sCloseRepository(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	return TRUE;
}

BOOL KItemProtocol::c2sApplyEnhance(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	// 跨服状态下不能强化
	if(IS_BZONE_PLAYER(pPlayer->GetId()))
		return FALSE;

	KITEM_C2S_ENHANCE_APPLY_SYNC* pSync;
	pSync = (KITEM_C2S_ENHANCE_APPLY_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	INT nParamCount = 0;
	KItem* pEquip = pPlayerItem->GetItemById(pSync->dwEquipId);
	QCONFIRM_RET_FALSE(pEquip);

	QCONFIRM_RET_FALSE(KD_ISEQUIP(pEquip->GetGenre()));

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	if(pSync->byMode == emENHANCE_ONCE)
	{
		cSafeScript->CallTableFunction( "Item", "Enhance", 1, "od", pEquip->m_pLuna, pEquip->GetEnhanceCost());
	}
	else
	{
		// 一键强化
		cSafeScript->CallTableFunction( "Item", "EnhanceEx", 1, "o", pEquip->m_pLuna);
	}
	
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KItemProtocol::c2sUpgrade(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	KITEM_C2S_UPGRADE_APPLY_SYNC* pSync;
	pSync = (KITEM_C2S_UPGRADE_APPLY_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KItem* pEquip = pPlayerItem->GetItemById(pSync->dwEquipId);
	QCONFIRM_RET_FALSE(pEquip);

	QCONFIRM_RET_FALSE(KD_ISEQUIP(pEquip->GetGenre()));

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Item", "Upgrade", 1, "o", pEquip->m_pLuna);
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KItemProtocol::c2sRefine(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	KITEM_C2S_REFINE_APPLY_SYNC* pSync;
	pSync = (KITEM_C2S_REFINE_APPLY_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KItem* pEquip = pPlayerItem->GetItemById(pSync->dwEquipId);
	QCONFIRM_RET_FALSE(pEquip);

	QCONFIRM_RET_FALSE(KD_ISEQUIP(pEquip->GetGenre()));

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Item", "Refine", 1, "odddddd", pEquip->m_pLuna, pSync->byMoneyType, 
		pSync->aryLock[0], pSync->aryLock[1], pSync->aryLock[2], pSync->aryLock[3], pSync->aryLock[4]);
	INT bRet = cSafeScript->GetInt(-1);
	QCONFIRM_RET_FALSE(bRet);
	return bRet;
}

BOOL KItemProtocol::c2sSaveAppendAttr(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	//QLogPrintf(LOG_INFO,"KItemProtocol::c2sSaveAppendAttr");
	KITEM_C2S_SAVE_REFINE_ATTR* pSync;
	pSync = (KITEM_C2S_SAVE_REFINE_ATTR*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);
	//Q_Printl("..pSync->bySaveOriginal="<< pSync->bySaveOriginal << ",pSync->dwEquipId=" << pSync->dwEquipId);
	//QLogPrintf(LOG_INFO,"..pSync->bySaveOriginal=%d",pSync->bySaveOriginal);
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KItem* pEquip = pPlayerItem->GetItemById(pSync->dwEquipId);
	QCONFIRM_RET_FALSE(pEquip);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Item", "SaveAppendAttr", 1, "od", pEquip->m_pLuna, pSync->bySaveOriginal);
	INT bRet = cSafeScript->GetInt(-1);

	//pEquip->SaveAppendAttr(pSync->bySaveOriginal);

	if(!pSync->bySaveOriginal)
	{
		g_cCharacterHelper.CalcFightScore(*pPlayer);
	}
	QCONFIRM_RET_FALSE(bRet);
	return bRet;
}

BOOL KItemProtocol::c2sEquipIdentify(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	// 跨服状态下不能鉴定
	if(IS_BZONE_PLAYER(pPlayer->GetId()))
		return FALSE;

	KITEM_C2S_EQUIP_IDENTIFY* pSync;
	pSync = (KITEM_C2S_EQUIP_IDENTIFY*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);


	KItem* pEquip = pPlayerItem->GetItemById(pSync->dwEquipId);
	QCONFIRM_RET_FALSE(pEquip);

	QCONFIRM_RET_FALSE(KD_ISEQUIP(pEquip->GetGenre()));

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Item", "EquipIdentify", 1, "o", pEquip->m_pLuna );
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KItemProtocol::c2sStoneMix(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	// 跨服状态下不能宝石合成
	if(IS_BZONE_PLAYER(pPlayer->GetId()))
		return FALSE;

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KITEM_C2S_STONE_MIX_SYNC* pSync;
	pSync = (KITEM_C2S_STONE_MIX_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	QCONFIRM_RET_FALSE(pSync->byStoneCount > 0 && pSync->byStoneCount <= emSTONE_MIX_MAX_COUNT);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());

	cSafeScript->GetGlobalF("Item:StoneMix");
	cSafeScript->PushTable();
	for (INT i = 0; i < pSync->byStoneCount; ++i)
	{
		KItem* pStone = pPlayerItem->GetItemById(pSync->adwStoneId[i]);
		QCONFIRM_RET_FALSE(pStone);
		if (!KD_ISSTONE(pStone->GetGenre(), pStone->GetDetailType())) return FALSE;
		if(!pPlayerItem->CheckItemInBag(pStone)) return FALSE;

		cSafeScript->PushObj((QLunaBase*)pStone->m_pLuna);
		cSafeScript->SetTableIndex(i+1);
	};
	cSafeScript->DoCall(0, 2);
	return TRUE;
}

BOOL KItemProtocol::c2sSellItems(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	KITEM_C2S_SELL_ITEMS* pSync;
	pSync = (KITEM_C2S_SELL_ITEMS*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	INT nLeftDataLen = nDataLen - sizeof(KITEM_C2S_SELL_ITEMS);
	QCONFIRM_RET_FALSE(nLeftDataLen > 0 && nLeftDataLen % sizeof(DWORD) == 0);

	INT nItemCount = nLeftDataLen / sizeof(DWORD);

	static BYTE _sbyBuffer[USHRT_MAX];
	ZeroMemory(_sbyBuffer,sizeof(_sbyBuffer));
	KS2C_BLACK_MSG* pSync_msg = (KS2C_BLACK_MSG*)_sbyBuffer;
	*pSync_msg = KS2C_BLACK_MSG();
	LPCSTR pszString = LANG_029;
	//INT nLen = sizeof(pszString);
	INT nLen = strlen(pszString);
	QLogPrintf(LOG_INFO,"...size of LANG_029=%d",nLen);
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->GetGlobalF("Item:SellItems");
	cSafeScript->PushTable();

	BOOL MARK = FALSE;
	for(INT i = 0; i < nItemCount; ++i)
	{
		KItem* pItem = pPlayerItem->GetItemById(pSync->adwItemIds[i]);
		
		QCONFIRM_RET_FALSE(pItem && pPlayerItem->CheckItemInBag(pItem));

		//equip不能出售，防止玩家出售
		if (strcmp(pItem->m_sBaseData.pszClass,"equip") == 0)
		{
			MARK = TRUE;
			continue;
		}
		cSafeScript->PushObj((QLunaBase*)pItem->m_pLuna);
		cSafeScript->SetTableIndex(i+1);
	}
	if (MARK)
	{
		strncpy((CHAR*)pSync_msg->arBuffer, pszString, nLen);
		//pSync_msg->arBuffer[nLen - 1] = 0;
		pSync_msg->arBuffer[nLen] = 0;

		g_pBaseSender->SendLenData(pPlayer->GetPlayerIndex(), *pSync_msg, sizeof(*pSync_msg) + nLen);
	}
	
	cSafeScript->DoCall(0, 2);

	// 处理完毕，发送协议
	KITEM_S2C_SELLITEMS_END sSync;
	INT nPlayerIdx = pPlayer->GetPlayerIndex();
	g_cItemMgr.GetProtocolProcessor()->SendData(nPlayerIdx, sSync);

	return TRUE;
}

BOOL KItemProtocol::c2sStoneMount(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	// 跨服状态下不能宝石镶嵌
	if(IS_BZONE_PLAYER(pPlayer->GetId()))
		return FALSE;

	KITEM_POS sPos;
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KITEM_C2S_STONE_MOUNT_SYNC* pSync;
	pSync = (KITEM_C2S_STONE_MOUNT_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);
	INT nLeftDataLen = nDataLen - sizeof(KITEM_C2S_STONE_MOUNT_SYNC);
	QCONFIRM_RET_FALSE(nLeftDataLen > 0 && nLeftDataLen % sizeof(DWORD) == 0);

	// 装备和宝石的检查
	KItem* pEquip = pPlayerItem->GetItemById(pSync->dwEquipId);
	if(!KD_ISEQUIP(pEquip->GetGenre())) return FALSE;

	INT nStoneCount = nLeftDataLen / sizeof(DWORD);
	QCONFIRM_RET_FALSE(nStoneCount > 0 && nStoneCount <= pEquip->GetStoneSlot());

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->GetGlobalF("Item:StoneMount");
	cSafeScript->PushObj((QLunaBase*)pEquip->m_pLuna);
	cSafeScript->PushTable();

	for(INT i = 0; i < nStoneCount; ++i)
	{
		KItem* pStone = pPlayerItem->GetItemById(pSync->adwStoneId[i]);
		QCONFIRM_RET_FALSE(pStone);
		QCONFIRM_RET_FALSE(KD_ISSTONE(pStone->GetGenre(), pStone->GetDetailType()));
		QCONFIRM_RET_FALSE(pPlayerItem->CheckItemInBag(pStone));

		cSafeScript->PushObj((QLunaBase*)pStone->m_pLuna);
		cSafeScript->SetTableIndex(i+1);
	}
	
	cSafeScript->DoCall(0, 3);

	return TRUE;
}

BOOL KItemProtocol::c2sStoneRemove(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	if(IS_BZONE_PLAYER(pPlayer->GetId()))
		return FALSE;

	KITEM_POS sPos;
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KITEM_C2S_STONE_REMOVE_SYNC* pSync;
	pSync = (KITEM_C2S_STONE_REMOVE_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	INT nLeftDataLen = nDataLen - sizeof(KITEM_C2S_STONE_REMOVE_SYNC);
	QCONFIRM_RET_FALSE(nLeftDataLen > 0 && nLeftDataLen % sizeof(BYTE) == 0);

	KItem* pEquip = pPlayerItem->GetItemById(pSync->dwEquipId);
	if(!KD_ISEQUIP(pEquip->GetGenre())) return FALSE;

	INT nSlotCount = nLeftDataLen / sizeof(BYTE);
	QCONFIRM_RET_FALSE(nSlotCount > 0 && nSlotCount <= pEquip->GetStoneSlot());

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());

	cSafeScript->GetGlobalF("Item:RemoveStone");
	cSafeScript->PushObj((QLunaBase*)pEquip->m_pLuna);
	cSafeScript->PushTable();
	for(INT i = 0; i < nSlotCount; ++i)
	{
		BYTE bySlot = pSync->abySlot[i];

		QCONFIRM_RET_FALSE(pEquip->HasStone(bySlot+1));

		cSafeScript->PushNumber(bySlot+1);
		cSafeScript->SetTableIndex(i+1);
	}
	cSafeScript->DoCall(0, 3);

	return TRUE;
}

BOOL KItemProtocol::c2sStonePunch(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KITEM_C2S_STONE_PUNCH* pSync;
	pSync = (KITEM_C2S_STONE_PUNCH*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	KItem* pEquip = pPlayerItem->GetItemById(pSync->dwEquipId);
	QCONFIRM_RET_FALSE(pEquip);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	INT nRet = cSafeScript->CallTableFunction("Item", "StonePunch", 1, "od", pEquip->m_pLuna, pSync->byPos);

	return nRet;
}

BOOL KItemProtocol::c2sEquipRerand(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	if(IS_BZONE_PLAYER(pPlayer->GetId()))
		return FALSE;

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KITEM_C2S_EQUIP_RERAND_SYNC* pSync;
	pSync = (KITEM_C2S_EQUIP_RERAND_SYNC*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);
	QCONFIRM_RET_FALSE(pSync->byMode == 0 || pSync->byMode == 1);

	KItem* pEquip = pPlayerItem->GetItemById(pSync->dwEquipId);
	QCONFIRM_RET_FALSE(pEquip);

	if(!KD_ISEQUIP(pEquip->GetGenre())) 
		return FALSE;

	if(pSync->byMode == 1)
		QCONFIRM_RET_FALSE(pSync->byNeedScore > pPlayerItem->CalcEquipScore(pEquip) && pSync->byNeedScore <= 100);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->GetGlobalF("Item:EquipRerand");
	cSafeScript->PushObj((QLunaBase*)pEquip->m_pLuna);
	cSafeScript->PushNumber(pSync->byMode);
	cSafeScript->PushNumber(pSync->byNeedScore);
	cSafeScript->DoCall(0, 4);

	return TRUE;
}

BOOL KItemProtocol::ProcessProtocol(INT nPlayerIdx, LPCBYTE pProtocol, UINT nDataLen)
{
	QCONFIRM_RET_FALSE(pProtocol);
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIdx);
	QCONFIRM_RET_FALSE(pPlayer);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pProtocol;
	QCONFIRM_RET_FALSE(pData);
	switch (pData->byProtocol)
	{
	case c2s_switchitem:
		c2sSwitchItem(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_playeruseitem:
		c2sPlayerUseItem(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_resizeextbag:
		c2sResizeExtBag(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_arrangebag:
		c2sArrangeBagItem(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_splititem:
		c2sSplitItem(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_playerthrowawayitem:
		c2sPlayerThrowAwayItem(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_viewequip:
		c2sViewEquip(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_repairitem:
		c2sRepairItem(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_closerepository:
		c2sCloseRepository(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_applyenhance:
		c2sApplyEnhance(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_upgrade:
		c2sUpgrade(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_refine:
		c2sRefine(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_save_appendattr:
		c2sSaveAppendAttr(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_stone_mix:
		c2sStoneMix(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_stone_mount:
		c2sStoneMount(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_stone_remove:
		c2sStoneRemove(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_stone_punch:
		c2sStonePunch(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_equip_rerand:
		c2sEquipRerand(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_equip_identify:
		c2sEquipIdentify(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_playersellitem:
		c2sSellItems(pPlayer, pProtocol, nDataLen);
		break;
	case c2s_reapply_pendant_request:
		c2sOnReApplyPendants(pPlayer, pProtocol, nDataLen);
		break;
	default:
		QCONFIRM_RET_FALSE(FALSE);
	}

	return TRUE;
}
