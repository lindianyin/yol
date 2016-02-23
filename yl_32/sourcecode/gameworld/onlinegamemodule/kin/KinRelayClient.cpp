
#include "stdafx.h"
#include "KinRelayClient.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "KinModule.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "../onlinegameworld/KRelayClient.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "onlinegamemodule/item2/itemdef.h"
#include "../onlinegamemodule/item2/kplayeritem.h"
#include "../onlinegamemodule/item2/kitemmgr.h"
#include "onlinegamebase/ksysservice.h"
#include "../onlinegamemodule/rank/rankdef.h"
#include "../onlinegamemodule/rank/krankmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KinRelayClient::KinRelayClient(void)
{
}

KinRelayClient::~KinRelayClient(void)
{
}


void KinRelayClient::OnSyncTongMemberInfo(BYTE* pbyData, size_t uDataLen)
{   
	R2S_SYNC_TONG_MEMBER_INFO* pSyncRespond = (R2S_SYNC_TONG_MEMBER_INFO*)pbyData;

	g_KinModule.DoSyncTongMemberInfo(pSyncRespond->dwPlayerID, pSyncRespond->MemberInfo);
}

void KinRelayClient::OnDeleteTongMemberNotify(BYTE* pbyData, size_t uDataLen)
{
	R2S_DELETE_TONG_MEMBER_NOTIFY* pPackage = (R2S_DELETE_TONG_MEMBER_NOTIFY*)pbyData;

	g_KinModule.DoTongDeleteMemberNotify(pPackage->dwPlayerID, pPackage->dwMemberID);
}

void KinRelayClient::OnApplyTongInfoRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_APPLY_TONG_INFO_RESPOND* pSyncRespond = (R2S_APPLY_TONG_INFO_RESPOND*)pbyData;

	g_KinModule.DoApplyTongInfoRespond(
		pSyncRespond->dwPlayerID, pSyncRespond->byInfoType, 
		pSyncRespond->byData, uDataLen - sizeof(R2S_APPLY_TONG_INFO_RESPOND)
		);
}

void KinRelayClient::OnUpdateTongClientDataVersion(BYTE* pbyData, size_t uDataLen)
{
	R2S_UPDATE_TONG_CLIENT_DATA_VERSION* pUpdateTongData = (R2S_UPDATE_TONG_CLIENT_DATA_VERSION*)pbyData;

	g_KinModule.DoUpdateTongClientDataVersion(
		pUpdateTongData->dwPlayerID, (KTONG_UPDATE_FLAG)pUpdateTongData->byCountType,
		pUpdateTongData->nUpdateCount);
}

void KinRelayClient::OnSyncTongRepertoryPageRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                  bResult = FALSE;
	R2S_SYNC_TONG_REPERTORY_PAGE_RESPOND* pRespond = (R2S_SYNC_TONG_REPERTORY_PAGE_RESPOND*)pbyData;
	BYTE*                                 pbyRepertoryOffset = NULL;
	BYTE*                                 pbyItemSyncOffset = NULL;
	TDBItemData*                          pDBItemData = NULL;
	KPlayer*                              pPlayer    = NULL;
	KPlayerItem*                          pPlayerItem = NULL;
	KItem*                                pItem       = NULL;
	KTONG_ITEM_SYNC_DATA*                 pTongRepertorySynData = NULL;
	KIN_REPERTORY_GRID_SYNC_DATA*         pItemSyncData = NULL;
	IMemBlock*                            piBuffer    = NULL;
	INT                                   nUpdateDataLen = 0;
	INT                                   nInvalidDataLen = 0;

	nUpdateDataLen = sizeof(KIN_REPERTORY_GRID_SYNC_DATA) * pRespond->byUpdateItemCount;
	nInvalidDataLen = sizeof(BYTE) * pRespond->byInvailedItemCount;
	piBuffer = QCreateMemBlock(1 + nUpdateDataLen + nInvalidDataLen);

	if (pRespond->byUpdateItemCount + pRespond->byInvailedItemCount)
	{
		PROCESS_ERROR(piBuffer);

		pbyItemSyncOffset = (BYTE*)piBuffer->GetData();
		pbyRepertoryOffset = pRespond->byData;

		// 根据TDBItemData生成Item
		for (int i = 0; i < pRespond->byUpdateItemCount; ++i)
		{
			pTongRepertorySynData = (KTONG_ITEM_SYNC_DATA*)pbyRepertoryOffset;
			PROCESS_ERROR(sizeof(TDBItemData) == pTongRepertorySynData->byDataLen);
			pDBItemData = (TDBItemData*)pTongRepertorySynData->byData;
			pItemSyncData = (KIN_REPERTORY_GRID_SYNC_DATA*)pbyItemSyncOffset;

			pItemSyncData->byPos                = pTongRepertorySynData->byPos;
			pItemSyncData->bRent                = pTongRepertorySynData->byRent;
			pItemSyncData->bRemoveOnExpire      = pTongRepertorySynData->byRemoveOnExpire;
			pItemSyncData->nExpireTime          = pTongRepertorySynData->nExpireTime;
			strncpy(pItemSyncData->szRenterName, pTongRepertorySynData->szRenterName, sizeof(pItemSyncData->szRenterName));
			strncpy(pItemSyncData->szPutterName, pTongRepertorySynData->szPutterName, sizeof(pItemSyncData->szPutterName));

			pItemSyncData->dwID = 0;
			pItemSyncData->byGenre       = (BYTE)pDBItemData->wGenre;
			pItemSyncData->wDetail       = (WORD)pDBItemData->wDetailType;
			pItemSyncData->wParticular   = pDBItemData->wParticularType;
			pItemSyncData->dwLevel       = pDBItemData->byLevel;
			pItemSyncData->dwRandomSeed  = pDBItemData->dwRandSeed;
			pItemSyncData->dwRandomSeedEx = pDBItemData->dwRandSeedEx;
			pItemSyncData->wCount        = (WORD)pDBItemData->dwCount;
			pItemSyncData->byEnhance     = pDBItemData->byEnhance;
			// pItemSyncData->byStoneInfo = pDBItemData->arydwStoneInfo;
			memset(&pItemSyncData->byStoneInfo[0], 0, sizeof(pItemSyncData->byStoneInfo));
			pItemSyncData->bBind         = pDBItemData->byStateFlags & (0x01 << 1);
			pItemSyncData->byTimeoutType = pDBItemData->byTimeoutType;
			pItemSyncData->dwTimeout     = pDBItemData->dwTimeout;
			pItemSyncData->sGuid         = pDBItemData->sGuid;

			pbyItemSyncOffset += sizeof(KIN_REPERTORY_GRID_SYNC_DATA);
			pbyRepertoryOffset += sizeof(KTONG_ITEM_SYNC_DATA) + pTongRepertorySynData->byDataLen;
		}

		memcpy(pbyItemSyncOffset, pbyRepertoryOffset, sizeof(BYTE) * nInvalidDataLen);

		pbyItemSyncOffset  = NULL;
		pbyRepertoryOffset = NULL;
	}

	g_KinModule.DoSyncTongRepertoryPageRespond(
		pRespond->dwPlayerID, pRespond->byPageIndex, pRespond->nServerCounter,
		pRespond->byGetRight, pRespond->byPutRight,
		pRespond->byUpdateItemCount, pRespond->byInvailedItemCount,
		nUpdateDataLen + nInvalidDataLen, (BYTE*)piBuffer->GetData()
		);

EXIT0:
	SAFE_RELEASE(piBuffer);
	return;
}

void KinRelayClient::OnInvitePlayerJoinTongRequest(BYTE* pbyData, size_t uDataLen)
{
	R2S_INVITE_PLAYER_JOIN_TONG_REQUEST* pRequest = (R2S_INVITE_PLAYER_JOIN_TONG_REQUEST*)pbyData;

	g_KinModule.DoInvitePlayerJoinTongRequest(
		pRequest->dwPlayerID, pRequest->dwInviterID, pRequest->dwTongID, pRequest->byTongCamp, pRequest->szInviterName, pRequest->szTongName
		); 
}

void KinRelayClient::OnChangePlayerTongNotify(BYTE* pbyData, size_t uDataLen)
{
	KPlayer*                        pPlayer = NULL;
	KPlayerItem*                    pPlayerItem = NULL;
	R2S_CHANGE_PLAYER_TONG_NOTIFY*  pNotify = (R2S_CHANGE_PLAYER_TONG_NOTIFY*)pbyData;
	QLogPrintf(LOG_INFO,"..KinRelayClient::OnChangePlayerTongNotify pNotify->byReason=%d",pNotify->byReason);
	pPlayer = g_cPlayerMgr.GetById(pNotify->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	pPlayer->m_dwKinId = pNotify->dwTongID;
	pPlayer->m_dwKinMasterId = pNotify->dwKinMasterId;
	if (pNotify->byReason == ctrFired || pNotify->byReason == ctrQuit) // added by liyun 玩家被T出家族移除所有他从家族仓库借来的装备
	{
		pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
		LOG_PROCESS_ERROR(pPlayerItem);

		pPlayerItem->RemoveAllRentItem();

		// 退出家族贡献度清0
		KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
		LOG_PROCESS_ERROR(pPlayerPurse);
		KE_PURSE_RESULT cResult;
		pPlayerPurse->SetMoney(EM_MONEY_CONTRIBUTION, 0, &cResult);

		pPlayer->SetLastQuitTongTime(KSysService::Time(NULL));
	}
	else if (pNotify->byReason == ctrCreate) // 创建成功，扣除银两
	{
		KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
		LOG_PROCESS_ERROR(pPlayerPurse);

		KE_PURSE_RESULT cResult;
		pPlayerPurse->AddMoney(EM_MONEY_SILVER, -200000, &cResult);
	}

	g_KinModule.DoChangePlayerTongNotify(pPlayer->GetPlayerIndex(), pNotify->dwTongID, pNotify->byReason, pNotify->szTongName);

	g_KinModule.DoBroadcastPlayerChangeTong(pPlayer);

EXIT0:
	return;
}

void KinRelayClient::OnBroadcastTongMessage(BYTE* pbyData, size_t uDataLen)
{
	KPlayer*                    pPlayer     = NULL;
	R2S_TONG_BROADCAST_MESSAGE* pMessage    = (R2S_TONG_BROADCAST_MESSAGE*)pbyData;

	pPlayer = g_cPlayerMgr.GetById(pMessage->dwPlayerID);
	PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(pMessage->byCode > tbmInvalid && pMessage->byCode < tbmTotal);

	g_KinModule.DoBroadcastTongMessage(
		pPlayer->GetPlayerIndex(), pMessage->byCode, pMessage->byData, uDataLen - sizeof(R2S_TONG_BROADCAST_MESSAGE)
		);

EXIT0:
	return;
}

void KinRelayClient::OnGetTongDescriptionRespond(BYTE* pbyData, size_t uDataLen)
{
	KPlayer*                            pPlayer     = NULL;
	R2S_GET_TONG_DESCRIPTION_RESPOND*   pRespond    = (R2S_GET_TONG_DESCRIPTION_RESPOND*)pbyData;
	QKinModule::PLAYER_KIN_INFO info;

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	PROCESS_ERROR(pPlayer);

	pRespond->szTongName[sizeof(pRespond->szTongName) - 1] = '\0';
	
	info.szKinName = pRespond->szTongName;
	info.nKinLevel = pRespond->byTongLevel;
	info.dwKinMasterID = pRespond->dwMasterID;
	info.dwKinId = pRespond->dwTongID;

	if (pPlayer->GetKinId() == info.dwKinId)
	{
        g_KinModule.UpdatePlayerKinInfo(pPlayer->GetId(), info);
	}

	g_KinModule.DoGetTongDescriptionRespond(pPlayer->GetPlayerIndex(), pRespond->dwTongID, pRespond->szTongName, pRespond->dwMasterID);
EXIT0:
	return;
}

void KinRelayClient::OnSyncTongOnlineMessage(BYTE* pbyData, size_t uDataLen)
{
	KPlayer*                        pPlayer = NULL;
	R2S_SYNC_TONG_ONLINE_MESSAGE*   pSync   = (R2S_SYNC_TONG_ONLINE_MESSAGE*)pbyData;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_SYNC_TONG_ONLINE_MESSAGE) + (size_t)pSync->byDataLen);

	pPlayer = g_cPlayerMgr.GetById(pSync->dwPlayerID);
	PROCESS_ERROR(pPlayer);

	g_KinModule.DoSyncTongOnlineMessage(pPlayer->GetPlayerIndex(), pSync->szOnlineMessage, (size_t)pSync->byDataLen);
EXIT0:
	return;
}

void KinRelayClient::OnTakeTongRepertoryItemToPosRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                            bResult         = false;
	BOOL                                            bRetCode        = false;
	int                                             nRetCode        = 0;
	KPlayer*                                        pPlayer         = NULL;
//	ADD_ITEM_RESULT_CODE                            eRetCode        = aircFailed;
//	R2S_TAKE_TONG_REPERTORY_ITEM_TO_POS_RESPOND*    pRespond        = (R2S_TAKE_TONG_REPERTORY_ITEM_TO_POS_RESPOND*)pbyData;
//	KItem*                                          pItem           = NULL;
//	KItemList*                                      pItemList       = NULL;
//	int                                             nBoxSize        = 0;
//	KITEM_LOG_INFO                                  ItemLogInfo;
//
//	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_TAKE_TONG_REPERTORY_ITEM_TO_POS_RESPOND) + pRespond->bySize);
//
//	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
//	LOG_PROCESS_ERROR(pPlayer);
//	pItemList = &pPlayer->m_ItemList;
//
//	LOG_PROCESS_ERROR(pRespond->byBoxIndex >= ibPackage && pRespond->byBoxIndex <= ibPackage4);
//	nBoxSize = pItemList->GetBoxSize(pRespond->byBoxIndex);
//	LOG_PROCESS_ERROR(pRespond->byBoxPos < nBoxSize);
//
//	pItem = g_pWorld->m_ItemManager.GenerateItemFromBinaryData(KD_BAD_ID, pRespond->byData, (size_t)pRespond->bySize);
//	LOG_PROCESS_ERROR(pItem);
//
//	g_LogClient.GetItemLogInfo(pItem, &ItemLogInfo);
//
//	eRetCode = pItemList->AddItemToPos(pItem, pRespond->byBoxIndex, pRespond->byBoxPos);
//	PROCESS_ERROR(eRetCode == aircSuccess);
//	pItem = NULL;
//
//	g_LogClient.LogPlayerItemChange(pPlayer, ITEM_OPERATION_TYPE_IN, &ItemLogInfo, "get from tong");
//
//	bResult = true;
//EXIT0:
//	if (pItem)
//	{
//		g_pWorld->m_ItemManager.FreeItem(pItem);
//		pItem = NULL;
//	}

	//DoTakeTongRepertoryItemRespond(pRespond->dwPlayerID, pRespond->dwTongID, pRespond->byPageIndex, pRespond->byRepertoryPos, bResult);
	return;
}


void KinRelayClient::OnTakeTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                    bResult     = false;
	KPlayer*                                pPlayer     = NULL;
	KPlayerItem*                            pPlayerItem = NULL;
	KItem*                                  pItem       = NULL;
	INT                                     nItemIdx    = 0;
	INT                                     nKinItemIdx = 0;
	ADD_ITEM_RESULT_CODE                    eRetCode    = aircFailed;
	R2S_TAKE_TONG_REPERTORY_ITEM_RESPOND*   pRespond    = (R2S_TAKE_TONG_REPERTORY_ITEM_RESPOND*)pbyData;
	TDBItemData*                            pItemData   = NULL;
	INT                                     nX          = 0;
	INT                                     nY          = 0;

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_TAKE_TONG_REPERTORY_ITEM_RESPOND) + pRespond->byItemDataLen);

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsPlaying);
	LOG_PROCESS_ERROR(pRespond->byBoxIndex == emROOM_MAINBAG || pRespond->byBoxIndex == emROOM_EXTBAG);
	LOG_PROCESS_ERROR(pRespond->byBoxPos < KD_ROOM_MAINBAG_SIZE);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	pItemData = (TDBItemData*)pRespond->byItemData;
	if ((pItemData->byStateFlags >> 1) & 0x01)
	{
		if (pRespond->dwPutItemPlayerID == pRespond->dwPlayerID)
		{
			nKinItemIdx = pPlayerItem->SearchGUID(pItemData->sGuid, NULL);
			LOG_PROCESS_ERROR(nKinItemIdx);
		}
		else
		{ // 租用装备，设置超时时间
			pItemData->byStateFlags |= (0x01 << 4);
			pItemData->byTimeoutType = emKITEM_TIMEOUTTYPE_ABSOLUTE;
			pItemData->dwTimeout = KSysService::GameTime(NULL) + pRespond->tRentPeriod; // 单位是秒数
		}
	}

	nItemIdx = pPlayerItem->MakeItem(pItemData);
	LOG_PROCESS_ERROR(nItemIdx);

	nX = pRespond->byBoxPos % KD_ROOM_MAINBAG_WIDTH;
	nY = pRespond->byBoxPos / KD_ROOM_MAINBAG_WIDTH;
	bResult = pPlayerItem->Add(nItemIdx, (KE_ITEM_ROOM)pRespond->byBoxIndex, emKADDITEM_NONE, nX, nY);
	LOG_PROCESS_ERROR(bResult);

	if (nKinItemIdx)
	{
		bResult = pPlayerItem->Remove(nKinItemIdx, emKLOSEITEM_NONE);
		LOG_PROCESS_ERROR(bResult);
	}

// 	pItem = g_pWorld->m_ItemManager.GenerateItemFromBinaryData(KD_BAD_ID, pRespond->byData, (size_t)pRespond->bySize);
// 	LOG_PROCESS_ERROR(pItem);
// 
// 	g_LogClient.GetItemLogInfo(pItem, &ItemLogInfo);
// 
// 	eRetCode = pPlayer->m_ItemList.AddItem(pItem);
// 	PROCESS_ERROR(eRetCode == aircSuccess);
// 	g_LogClient.LogPlayerItemChange(pPlayer, ITEM_OPERATION_TYPE_IN, &ItemLogInfo, "get from tong");

	pItem = NULL;
	nItemIdx = 0;
	bResult = true;
EXIT0:
	if (nItemIdx)
	{
		// g_pWorld->m_ItemManager.FreeItem(pItem);
		g_cItemMgr.Remove(nItemIdx);
		pItem = NULL;
	}

	DoTakeTongRepertoryItemRespond(pRespond->dwPlayerID, pRespond->dwTongID, pRespond->byPageIndex, pRespond->byPagePos, bResult);
	return;
}

void KinRelayClient::OnPutTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                    bRetCode        = false;
	BOOL                                    bSuccess        = false;
	KPlayer*                                pPlayer         = NULL;
	KPlayerItem*							pPlayerItem     = NULL;
	KItem*                                  pItem           = NULL;
	INT                                     nItemIdx        = 0;
	size_t                                  uItemDataLen    = 0;
	R2S_PUT_TONG_REPERTORY_ITEM_RESPOND*    pRespond        = (R2S_PUT_TONG_REPERTORY_ITEM_RESPOND*)pbyData;
	int                                     nBoxSize        = 0;
	TDBItemData                             cItemData;
	KITEM_POS                               cItemPos;
	INT                                     nx              = 0;
	INT                                     ny              = 0;

	LOG_PROCESS_ERROR(pRespond->byBoxIndex == emROOM_MAINBAG || pRespond->byBoxIndex == emROOM_EXTBAG);
	LOG_PROCESS_ERROR(pRespond->byBoxPos <= KD_ROOM_MAINBAG_SIZE);

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	nx = pRespond->byBoxPos % KD_ROOM_MAINBAG_WIDTH;
	ny = pRespond->byBoxPos / KD_ROOM_MAINBAG_WIDTH;
	cItemPos.eRoom = (KE_ITEM_ROOM)pRespond->byBoxIndex;
	cItemPos.nX = nx;
	cItemPos.nY = ny;
	pItem = KPlayerItem::GetItemFromItemPos(&cItemPos, pPlayer);
	PROCESS_ERROR(pItem);

	nItemIdx = pItem->GetGameIndex();
	PROCESS_ERROR(nItemIdx);

	memset(&cItemData, 0, sizeof(cItemData));
	pPlayerItem->SaveItemData(pItem, &cItemData, (INT)pRespond->byBoxIndex, nx, ny);

	bRetCode = pPlayerItem->Remove(nItemIdx, emKLOSEITEM_NONE);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = DoPutTongRepertoryItemRespond(pPlayer->GetId(), 
		pRespond->byPageIndex, pRespond->byPagePos, pRespond->bFindItem, (BYTE*)&cItemData, sizeof(cItemData));
	LOG_PROCESS_ERROR(bRetCode);

	if ((cItemData.byStateFlags >> 1) & 0x01)
	{
		if (!pRespond->bFindItem)
		{ // 玩家要放入装备供其他人借出，放一份装备到玩家的家族包包
			pItem->SetKinReptoryID(pRespond->dwTongID);
			pPlayerItem->AddItemIntoKinBag(nItemIdx);
		}
	}

// 	nBoxSize = pItemList->GetBoxSize(pRespond->byBoxIndex);
// 	LOG_PROCESS_ERROR(pRespond->byBoxPos < nBoxSize);
// 	pItem = pItemList->GetItem(pRespond->byBoxIndex, pRespond->byBoxPos);
// 	PROCESS_ERROR(pItem);
// 
// 	PROCESS_ERROR(pItem->m_dwID == pRespond->dwServerItemID);
// 	bRetCode = pItem->IsBind();
// 	LOG_PROCESS_ERROR(!bRetCode);
// 
// 	bRetCode = DoPutTongRepertoryItemRespond(pItem, pRespond->dwPlayerID, pRespond->dwTongID, pRespond->byPageIndex, pRespond->byPagePos);
// 	LOG_PROCESS_ERROR(bRetCode);
// 
// 	pItem = pItemList->PickUpItem(pRespond->byBoxIndex, pRespond->byBoxPos, true);
// 	assert(pItem);
// 
// 	g_LogClient.LogPlayerItemChange(pPlayer, ITEM_OPERATION_TYPE_OUT, pItem, "put to tong");
// 
// 	// 如果是寄存，则存入玩家的仓库包包
// 	g_pWorld->m_ItemManager.FreeItem(pItem);
// 	pItem = NULL;

	bSuccess = true;
EXIT0:
	if (!bSuccess)
	{
		DoUnlockTongRepertoryGrid(pRespond->dwTongID, pRespond->byPageIndex, pRespond->byPagePos);
	}
	return;
}

void KinRelayClient::OnApplyStackTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                            bRetCode    = false;
	BOOL                                            bSuccess    = false;
	int                                             nRetCode    = 0;
	KPlayer*                                        pPlayer     = NULL;
//	R2S_APPLY_STACK_TONG_REPERTORY_ITEM_RESPOND*    pRespond    = (R2S_APPLY_STACK_TONG_REPERTORY_ITEM_RESPOND*)pbyData;
//	KItemList*                                      pItemList   = NULL;
//	KItem*                                          pItem       = NULL;
//	DWORD                                           dwTabType   = 0;
//	DWORD                                           dwTabIndex  = 0;
//	int                                             nBoxSize    = 0;
//	int                                             nStackNum   = 0;
//	int                                             nMaxStackNum = 0;
//
//	assert(pRespond);
//
//	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
//	PROCESS_ERROR(pPlayer);
//
//	LOG_PROCESS_ERROR(pRespond->dwTongID == pPlayer->m_dwKinId);
//
//	LOG_PROCESS_ERROR(pRespond->byBoxIndex >= ibPackage  && pRespond->byBoxIndex <= ibPackage4);
//
//	pItemList = &pPlayer->m_ItemList;
//	assert(pItemList);
//
//	nBoxSize = pItemList->GetBoxSize(pRespond->byBoxIndex);
//	LOG_PROCESS_ERROR(pRespond->byBoxPos < nBoxSize);
//
//	pItem = pItemList->GetItem(pRespond->byBoxIndex, pRespond->byBoxPos);
//	PROCESS_ERROR(pItem);
//	PROCESS_ERROR(pItem->m_dwID == pRespond->dwItemID);    
//	PROCESS_ERROR(pRespond->wStackNum <= pItem->m_nStackNum);
//	nMaxStackNum = pItem->GetMaxStackNum();
//
//	dwTabType = pItem->m_GenParam.dwTabType;
//	dwTabIndex = pItem->m_GenParam.dwIndex;
//	nStackNum = pItem->GetStackNum();
//	nStackNum -= pRespond->wStackNum;
//
//	bRetCode = pItem->SetStackNum(nStackNum);
//	LOG_PROCESS_ERROR(bRetCode);
//
//	if (pItem->m_nStackNum == 0)
//	{
//		nRetCode = pItemList->DestroyItem(pItem, true);
//		LOG_CHECK_ERROR(nRetCode == ircSuccess);
//		pItem = NULL;
//	}
//	else
//	{
//		bRetCode = g_KinModule.DoUpdateItemAmount(
//			pPlayer->GetPlayerIndex(), pRespond->byBoxIndex,
//			pRespond->byBoxPos, nStackNum
//			);
//		LOG_CHECK_ERROR(bRetCode);
//	}
//
//	bRetCode = DoStackTongRepertoryItemRequest(
//		pRespond->dwPlayerID, pRespond->dwTongID,
//		pRespond->byPageIndex, pRespond->byPagePos, 
//		(BYTE)dwTabType, (WORD)dwTabIndex,
//		pRespond->wStackNum, (WORD)nMaxStackNum
//		);
//	LOG_PROCESS_ERROR(bRetCode);
//
//	bSuccess = true;
//EXIT0:
//	if(!bSuccess)
//	{
//		DoUnlockTongRepertoryGrid(pRespond->dwTongID, pRespond->byPageIndex, pRespond->byPagePos);
//	}
	return;
}

void KinRelayClient::OnApplyOpenTongRepertoryRespond(BYTE* pbyData, size_t uDataLen)
{
	KPlayer*                                pPlayer     = NULL;
	R2S_APPLY_OPEN_TONG_REPERTPRY_RESPOND*  pRespond    = (R2S_APPLY_OPEN_TONG_REPERTPRY_RESPOND*)pbyData;

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	g_KinModule.DoApplyOpenTongRepertoryRespond(
		pPlayer->GetPlayerIndex(), pRespond->dwPlayerID, pRespond->szTongName, pRespond->byRepertoryPageNum
		);
EXIT0:
	return;
}

void KinRelayClient::OnSyncTongActivityOpenTimes(BYTE* pbyData, size_t uDataLen)
{
	R2S_SYNC_KIN_ACTIVITY_OPENTIMES* pSyncTongOpenTimes = (R2S_SYNC_KIN_ACTIVITY_OPENTIMES*)pbyData;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(pSyncTongOpenTimes->dwPlayerId);
	LOG_CHECK_ERROR(pPlayer);
	if(pPlayer){
		g_KinModule.DoSyncTongActivityOpenTimes(pPlayer->GetPlayerIndex(), pSyncTongOpenTimes->byOpenTimesData, pSyncTongOpenTimes->nCount);
	}
}

void KinRelayClient::OnSyncTongApplyJoinList(BYTE* pbyData, size_t uDataLen)
{
    R2S_SYNC_TONG_APPLY_JOIN_LIST* pSyncTongApplyJoinList = (R2S_SYNC_TONG_APPLY_JOIN_LIST*)pbyData;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(pSyncTongApplyJoinList->dwPlayerId);
	LOG_CHECK_ERROR(pPlayer);
	if (pPlayer){
 		g_KinModule.DoSyncTongApplyJoinList(pPlayer->GetPlayerIndex(), pSyncTongApplyJoinList->byData, pSyncTongApplyJoinList->dwCount);
	}
}

void KinRelayClient::OnGetTongSalaryRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	KPlayer*                        pPlayer     = NULL;
	int                             nMoney      = 0;
	R2S_GET_TONG_SALARY_RESPOND*    pRespond    = (R2S_GET_TONG_SALARY_RESPOND*)pbyData;
	KPlayerPurse*					pPlayerPurse = NULL;
	KE_PURSE_RESULT					pResult		= emKPURSE_E_FAILED;

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(pRespond->nSilver >= 0);
	LOG_PROCESS_ERROR(INT_MAX / MONEY_CONVERSION_SILVER_RATE >= pRespond->nSilver);

	nMoney = pRespond->nSilver * MONEY_CONVERSION_SILVER_RATE;

	pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	LOG_PROCESS_ERROR(pPlayerPurse);

	pPlayerPurse->AddMoney(EM_MONEY_SILVER, nMoney, &pResult);
	LOG_PROCESS_ERROR(bRetCode);

	// g_LogClient.LogPlayerMoneyChange(nMoney, pPlayer, "get tong salary");

	bResult = true;
EXIT0:
	if (!bResult)
	{
		DoGetTongSalaryFailRespond(pRespond->dwPlayerID, pRespond->dwTongID, pRespond->nSilver);
	}
	return;
}

void KinRelayClient::OnGetTongRankInfoRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bRetCode    = false;
	R2S_SYNC_TONG_RANK_INFO*    pRespond    = (R2S_SYNC_TONG_RANK_INFO*)pbyData;

	switch(pRespond->nRankId)
	{
	case emRANK_TONG_FIGHTSCORE:
		g_cRankManager.OnGetTongFsRankInfoRespond(pRespond->dwTongId, pRespond->nTongMemberNum, pRespond->szTongName, pRespond->szMasterName);
		break;
	case emRANK_TONG_SALARY:
		g_cRankManager.OnGetTongSrRankInfoRespond(pRespond->dwTongId, pRespond->nTongMemberNum, pRespond->szTongName, pRespond->szMasterName);
		break;
	default:
		break;
	}

EXIT0:
	return;
}


void KinRelayClient::OnSyncTongHistoryRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	KPlayer*                        pPlayer     = NULL;
	R2S_SYNC_TONG_HISTORY_RESPOND*  pRespond    = (R2S_SYNC_TONG_HISTORY_RESPOND*)pbyData;

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(pRespond->byCount > 0);
	LOG_PROCESS_ERROR(uDataLen > sizeof(R2S_SYNC_TONG_HISTORY_RESPOND));

	g_KinModule.DoSyncTongHistoryRespond(
		pPlayer->GetPlayerIndex(), pRespond->byType, pRespond->dwStartIndex, (int)pRespond->byCount, 
		pRespond->byData, uDataLen - sizeof(R2S_SYNC_TONG_HISTORY_RESPOND)
		);
EXIT0:
	return;
}

void KinRelayClient::OnKinMessageNotify(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	KPlayer*                        pPlayer     = NULL;
	R2S_KIN_MESSAGE_NOTIFY*			pRespond	= (R2S_KIN_MESSAGE_NOTIFY*)pbyData;

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(pRespond->nCode > 0);
	LOG_PROCESS_ERROR(uDataLen >= sizeof(R2S_KIN_MESSAGE_NOTIFY));

	g_KinModule.DoKinMessageNotify(pPlayer->GetPlayerIndex(), pRespond->nCode, pRespond->byData, uDataLen - sizeof(R2S_KIN_MESSAGE_NOTIFY));

EXIT0:
	return;
}

void KinRelayClient::OnGetBackItemFromRepertoryRespond( BYTE* pbyData, size_t uDataLen )
{
	BOOL                                      bResult     = false;
	BOOL                                      bRetCode    = false;
	KPlayer*                                  pPlayer     = NULL;
	KPlayerItem*                              pPlayerItem = NULL;
	KItem*                                    pItem       = NULL;
	KItem*                                    pNewItem    = NULL;
	DWORD                                     dwItemIdx   = KD_BAD_ID;
	DWORD                                     dwNewItemIdx = KD_BAD_ID;
	R2S_GET_BACK_ITEM_FROM_REPERTORY_RESPOND* pRespond	  = (R2S_GET_BACK_ITEM_FROM_REPERTORY_RESPOND*)pbyData;
	TDBItemData                               cItemData;
	KITEM_POS                                 cItemPos;

	LOG_PROCESS_ERROR(uDataLen >= sizeof(R2S_GET_BACK_ITEM_FROM_REPERTORY_RESPOND));
	PROCESS_ERROR(pRespond->bySuccess);

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	PROCESS_ERROR(pPlayer);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	// 从家族背包中删除，加到玩家主背包
	memset(&cItemPos, 0, sizeof(cItemPos));
	cItemPos.eRoom = emROOM_KIN;
	cItemPos.nX = pRespond->byBoxPos % KD_ROOM_KIN_WIDTH;
	cItemPos.nY = pRespond->byBoxPos / KD_ROOM_KIN_WIDTH;
	pItem = KPlayerItem::GetItemFromItemPos(&cItemPos, pPlayer);
	LOG_PROCESS_ERROR(pItem);

	memset(&cItemData, 0, sizeof(cItemData));
	pPlayerItem->SaveItemData(pItem, &cItemData);

	cItemData.dwKinRepteroryId = 0;
	dwNewItemIdx = pPlayerItem->MakeItem(&cItemData);
	LOG_PROCESS_ERROR(dwNewItemIdx);

	bRetCode = pPlayerItem->Remove(pItem->GetGameIndex(), emKLOSEITEM_NONE);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pPlayerItem->AddItemIntoBag(dwNewItemIdx, emKADDITEM_KIN);
	LOG_PROCESS_ERROR(bRetCode);

	dwNewItemIdx = KD_BAD_ID;
EXIT0:
	if (dwNewItemIdx)
	{
		// g_pWorld->m_ItemManager.FreeItem(pItem);
		g_cItemMgr.Remove(dwNewItemIdx);
		pItem = NULL;
	}

	return;
}

void KinRelayClient::OnGetFreeJoinTongListRespond( BYTE* pbyData, size_t uDataLen )
{
	BOOL								bRetCode	= FALSE;
	KPlayer*							pPlayer		= NULL;
	R2S_GET_FREEJOIN_TONG_LIST_RESPOND*	pRespond	= (R2S_GET_FREEJOIN_TONG_LIST_RESPOND*)pbyData;

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(uDataLen >= sizeof(R2S_GET_FREEJOIN_TONG_LIST_RESPOND));
	bRetCode = g_KinModule.DoGetFreeJoinTongListRespond(pPlayer->GetPlayerIndex(), pRespond->byTongNum, pRespond->nUpdateCounter, pRespond->byData, uDataLen - sizeof(R2S_GET_FREEJOIN_TONG_LIST_RESPOND));
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}


BOOL KinRelayClient::DoApplyTongRosterRequest(DWORD dwPlayerID, int nLastUpdateFrame, DWORD dwMemberList[], int nMemberCount)
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	IMemBlock*                     piPackage       = NULL;
	S2R_APPLY_TONG_ROSTER_REQUEST*  pRequest        = NULL;

	LOG_PROCESS_ERROR(nMemberCount >= 0);
	LOG_PROCESS_ERROR(nMemberCount <= MAX_TONG_MEMBER_COUNT);

	piPackage =  QCreateMemBlock((unsigned)(sizeof(S2R_APPLY_TONG_ROSTER_REQUEST) + sizeof(DWORD) * nMemberCount));
	LOG_PROCESS_ERROR(piPackage);

	pRequest  = (S2R_APPLY_TONG_ROSTER_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID       = s2r_apply_tong_roster_request;
	pRequest->dwPlayerID        = dwPlayerID;
	pRequest->nLastUpdateFrame  = nLastUpdateFrame;
	pRequest->nMemberCount      = nMemberCount;

	if (nMemberCount > 0)
	{
		memcpy(pRequest->dwMemberList, dwMemberList, sizeof(DWORD) * nMemberCount);
	}

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinRelayClient::DoApplyTongInfoRequest(DWORD dwPlayerID, int nLastUpdateFrame)
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	IMemBlock*                     piPackage       = NULL;
	S2R_APPLY_TONG_INFO_REQUEST*    pApplyRequest   = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_APPLY_TONG_INFO_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pApplyRequest = (S2R_APPLY_TONG_INFO_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pApplyRequest);

	pApplyRequest->wProtocolID      = s2r_apply_tong_info_request;
	pApplyRequest->dwPlayerID       = dwPlayerID;
	pApplyRequest->nLastUpdateFrame = nLastUpdateFrame;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinRelayClient::DoApplyCreateTongRequest(DWORD dwPlayerID, const char cszTongName[], int nTemplateIndex)
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	IMemBlock*                     piPackage       = NULL;
	S2R_APPLY_CREATE_TONG_REQUEST*  pApplyRequest   = NULL;
	INT nKinCamp = 0;
	QLogPrintf(LOG_INFO,"..KinRelayClient::DoApplyCreateTongRequest(DWORD dwPlayerID=%d, const char cszTongName[]=%s, int nTemplateIndex=%d)",
					dwPlayerID,cszTongName,nTemplateIndex);
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

    nKinCamp = pPlayer->GetKinCamp();
	LOG_PROCESS_ERROR(nKinCamp == campXian || nKinCamp == campFo);

	piPackage = QCreateMemBlock(sizeof(S2R_APPLY_CREATE_TONG_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pApplyRequest = (S2R_APPLY_CREATE_TONG_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pApplyRequest);

	pApplyRequest->wProtocolID      = s2r_apply_create_tong_request;
	pApplyRequest->dwPlayerID       = dwPlayerID;
	pApplyRequest->byTemplateIndex  = (BYTE)nTemplateIndex;
	pApplyRequest->byCamp           = nKinCamp;

	strncpy(pApplyRequest->szTongName, cszTongName, sizeof(pApplyRequest->szTongName));
	pApplyRequest->szTongName[sizeof(pApplyRequest->szTongName) - 1] = '\0';

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinRelayClient::DoInvitePlayerJoinTongRequest(DWORD dwPlayerID, const char cszNewMemberName[])
{
	BOOL                                    bResult         = false;
	BOOL                                    bRetCode        = false;
	IMemBlock*                             piPackage       = NULL;
	S2R_INVITE_PLAYER_JOIN_TONG_REQUEST*    pInvitRequest   = NULL;

	piPackage = QCreateMemBlock((unsigned)sizeof(S2R_INVITE_PLAYER_JOIN_TONG_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pInvitRequest = (S2R_INVITE_PLAYER_JOIN_TONG_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pInvitRequest);

	pInvitRequest->wProtocolID      = s2r_invite_player_join_tong_request;
	pInvitRequest->dwPlayerID       = dwPlayerID;

	strncpy(pInvitRequest->szNewMemberName, cszNewMemberName, sizeof(pInvitRequest->szNewMemberName));
	pInvitRequest->szNewMemberName[sizeof(pInvitRequest->szNewMemberName) - 1] = '\0';

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinRelayClient::DoInvitePlayerJoinTongRespond(DWORD dwPlayerID, DWORD dwInviterID, DWORD dwTongID, INVITE_TONG_RESULT_CODE eAccept)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	IMemBlock*                             piPackage   = NULL;
	S2R_INVITE_PLAYER_JOIN_TONG_RESPOND*    pRespond    = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_INVITE_PLAYER_JOIN_TONG_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespond = (S2R_INVITE_PLAYER_JOIN_TONG_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = s2r_invite_player_join_tong_respond;
	pRespond->dwPlayerID    = dwPlayerID;
	pRespond->dwInviterID   = dwInviterID;
	pRespond->dwTongID      = dwTongID;
	pRespond->byAccept      = (BYTE)eAccept;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinRelayClient::DoApplyKickOutTongMemberRequest(DWORD dwPlayerID, DWORD dwMemberID)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piPackage   = NULL;
	S2R_APPLY_KICK_OUT_TONG_MEMBER* pRequest    = NULL;   

	piPackage = QCreateMemBlock((unsigned)sizeof(S2R_APPLY_KICK_OUT_TONG_MEMBER));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (S2R_APPLY_KICK_OUT_TONG_MEMBER*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_apply_kick_out_tong_member_request;
	pRequest->dwPlayerID  = dwPlayerID;
	pRequest->dwMemberID  = dwMemberID;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinRelayClient::DoModifyTongInfoRequest(DWORD dwPlayerID, BYTE byInfoType, BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piBuffer    = NULL;
	S2R_MODIFY_TONG_INFO_REQUEST*   pRequest    = NULL;

	LOG_PROCESS_ERROR(byInfoType > tmrBegin && byInfoType < tmrEnd);

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_MODIFY_TONG_INFO_REQUEST) + uDataLen);
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_MODIFY_TONG_INFO_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_modify_tong_info_request;
	pRequest->dwPlayerID    = dwPlayerID;
	pRequest->byInfoType    = byInfoType;
	pRequest->wDataLen      = (WORD)uDataLen;

	memcpy(pRequest->byData, pbyData, uDataLen);

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoModifyTongSchemaRequest(DWORD dwPlayerID, BYTE byGroupIndex, BYTE byModifyType, BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piBuffer    = NULL;
	S2R_MODIFY_TONG_SCHEMA_REQUEST* pRequest    = NULL;

	LOG_PROCESS_ERROR(byModifyType > tmsBegin && byModifyType < tmsEnd);

	piBuffer = QCreateMemBlock((unsigned)(sizeof(S2R_MODIFY_TONG_SCHEMA_REQUEST) + uDataLen));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_MODIFY_TONG_SCHEMA_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_modify_tong_schema_request;
	pRequest->dwPlayerID    = dwPlayerID;
	pRequest->byGroupIndex  = byGroupIndex;
	pRequest->byModifyType  = byModifyType;

	memcpy(pRequest->byData, pbyData, uDataLen);

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoApplyQuitTongRequest(DWORD dwPlayerID)
{
	BOOL                            bResult  = false;
	BOOL                            bRetCode = false;
	IMemBlock*                     piBuffer = NULL;
	S2R_APPLY_QUIT_TONG_REQUEST*    pRequest = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_APPLY_QUIT_TONG_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_APPLY_QUIT_TONG_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_apply_quit_tong_request;
	pRequest->dwPlayerID  = dwPlayerID;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoChangeTongMemberGroupRequest(DWORD dwPlayerID, DWORD dwTargetMemberID, int nTargetGroup)
{
	BOOL                                    bResult  = false;
	BOOL                                    bRetCode = false;
	IMemBlock*                             piBuffer = NULL;
	S2R_CHANGE_TONG_MEMBER_GROUP_REQUEST*   pRequest = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_CHANGE_TONG_MEMBER_GROUP_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_CHANGE_TONG_MEMBER_GROUP_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID       = s2r_change_tong_member_group_request;
	pRequest->dwPlayerID        = dwPlayerID;
	pRequest->dwTargetMemberID  = dwTargetMemberID;
	pRequest->byTargetGroup     = (BYTE)nTargetGroup;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoChangeTongMasterRequest(DWORD dwPlayerID, DWORD dwNewMasterID)
{
	BOOL                             bResult    = false;
	BOOL                             bRetCode   = false;
	IMemBlock*                      piBuffer   = NULL;
	S2R_CHANGE_TONG_MASTER_REQUEST*  pRequest   = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_CHANGE_TONG_MASTER_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_CHANGE_TONG_MASTER_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_change_tong_master_request;
	pRequest->dwPlayerID    = dwPlayerID;
	pRequest->dwNewMasterID = dwNewMasterID;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoChangeTongMemberRemarkRequest(DWORD dwPlayerID, DWORD dwTargetMemberID, char szRemark[])
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	IMemBlock*                             piBuffer    = NULL;
	S2R_CHANGE_TONG_MEMBER_REMARK_REQUEST*  pRequest    = NULL; 

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_CHANGE_TONG_MEMBER_REMARK_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_CHANGE_TONG_MEMBER_REMARK_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID       = s2r_change_tong_member_remark_request;
	pRequest->dwPlayerID        = dwPlayerID;
	pRequest->dwTargetMemberID  = dwTargetMemberID;

	strncpy(pRequest->szRemark, szRemark, sizeof(pRequest->szRemark));
	pRequest->szRemark[sizeof(pRequest->szRemark) - 1] = '\0';

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoGetTongDescriptionRequest(DWORD dwPlayerID, DWORD dwTongID)
{
	BOOL                                bResult     = false;
	BOOL                                bRetCode    = false;
	S2R_GET_TONG_DESCRIPTION_REQUEST*   pRequest    = NULL;
	IMemBlock*                         piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_GET_TONG_DESCRIPTION_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_GET_TONG_DESCRIPTION_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_get_tong_description_request;
	pRequest->dwPlayerID    = dwPlayerID;
	pRequest->dwTongID      = dwTongID;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoGetTongRankInfoRequest(DWORD dwTongId, INT nRankId)
{
	BOOL                                bResult     = false;
	BOOL                                bRetCode    = false;
	S2R_GET_TONG_RANK_INFO_REQUEST*		pRequest    = NULL;
	IMemBlock*                         piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_GET_TONG_RANK_INFO_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_GET_TONG_RANK_INFO_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_get_tong_rank_info_request;
	pRequest->dwTongId      = dwTongId;
	pRequest->nRankId		= nRankId;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoSaveMoneyInTongRequest(DWORD dwPlayerID, DWORD dwPealNum, DWORD dwJadeiteNum, DWORD dwAgateNum, DWORD dwDiamondNum, DWORD dwContributionAdd)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	S2R_SAVE_MONEY_IN_TONG_REQUEST* pRequest    = NULL;
	IMemBlock*                     piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_SAVE_MONEY_IN_TONG_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_SAVE_MONEY_IN_TONG_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_save_money_in_tong_request;
	pRequest->dwPlayerID    = dwPlayerID;
	pRequest->dwPealNum     = dwPealNum;
	pRequest->dwJadeiteNum  = dwJadeiteNum;
	pRequest->dwAgateNum    = dwAgateNum;
	pRequest->dwDiamondNum  = dwDiamondNum;
	pRequest->dwContributionAdd = dwContributionAdd;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoPayTongSalaryRequest(DWORD dwPlayerID, int nTotalSalary)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	S2R_PAY_TONG_SALARY_REQUEST*    pRequest    = NULL;
	IMemBlock*                     piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_PAY_TONG_SALARY_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_PAY_TONG_SALARY_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_pay_tong_salary_request;
	pRequest->dwPlayerID    = dwPlayerID;
	pRequest->nTotalSalary  = nTotalSalary;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoGetTongSalaryRequest(DWORD dwPlayerID)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	S2R_GET_TONG_SALARY_REQUEST*    pRequest    = NULL;
	IMemBlock*                     piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_GET_TONG_SALARY_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_GET_TONG_SALARY_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_get_tong_salary_request;
	pRequest->dwPlayerID    = dwPlayerID;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoGetTongSalaryFailRespond(DWORD dwPlayerID, DWORD dwTongID, int nSilver)
{
	BOOL                                bResult     = false;
	BOOL                                bRetCode    = false;
	S2R_GET_TONG_SALARY_FAIL_RESPOND*   pRespond    = NULL;
	IMemBlock*                         piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_GET_TONG_SALARY_FAIL_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (S2R_GET_TONG_SALARY_FAIL_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = s2r_get_tong_salary_fail_respond;
	pRespond->dwPlayerID    = dwPlayerID;
	pRespond->dwTongID      = dwTongID;
	pRespond->nSilver       = nSilver;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoGetFreeJoinTongListRequest( DWORD dwPlayerID, INT nLastUpdateCounter )
{
	BOOL                                bResult     = FALSE;
	BOOL                                bRetCode    = FALSE;
	S2R_GET_FREEJOIN_TONG_LIST_REQUEST* pRequest	= NULL;
	IMemBlock*							piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_GET_FREEJOIN_TONG_LIST_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_GET_FREEJOIN_TONG_LIST_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID			= s2r_get_freejoin_tong_list_request;
	pRequest->dwPlayerID			= dwPlayerID;
	pRequest->nLastUpdateCounter	= nLastUpdateCounter;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoApplyJoinTongRequest( DWORD dwPlayerID, DWORD dwTongID )
{
	BOOL                                bResult     = FALSE;
	BOOL                                bRetCode    = FALSE;
	S2R_APPLY_JOIN_TONG_REQUEST*		pRequest	= NULL;
	IMemBlock*							piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_APPLY_JOIN_TONG_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_APPLY_JOIN_TONG_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID			= s2r_apply_join_tong_request;
	pRequest->dwPlayerID			= dwPlayerID;
	pRequest->dwTongID				= dwTongID;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoSetTongFreeJoinRequest( DWORD dwPlayerID, DWORD dwTongID, BOOL bValue )
{
	BOOL                                bResult     = FALSE;
	BOOL                                bRetCode    = FALSE;
	S2R_SET_TONG_FREEJOIN_REQUEST*		pRequest	= NULL;
	IMemBlock*							piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_SET_TONG_FREEJOIN_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_SET_TONG_FREEJOIN_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID			= s2r_set_tong_freejoin_request;
	pRequest->dwPlayerID			= dwPlayerID;
	pRequest->dwTongID				= dwTongID;
	pRequest->bValue				= bValue;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoSetTongActivityOpenTimeRequest(DWORD dwPlayerID, DWORD dwTongID, DWORD dwTongActivityIndex, DWORD dwTongActivityOpenTime)
{
	BOOL                                        bResult     = FALSE;
	BOOL                                        bRetCode    = FALSE;
	S2R_SET_TONG_ACTIVITY_OPENTIME_REQUEST*		pRequest	= NULL;
	IMemBlock*							        piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_SET_TONG_ACTIVITY_OPENTIME_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_SET_TONG_ACTIVITY_OPENTIME_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID			   = s2r_set_tong_activity_opentime_request;
	pRequest->dwPlayerId               = dwPlayerID;
	pRequest->dwTongId	    		   = dwTongID;
	pRequest->dwTongActivityIndex	   = dwTongActivityIndex;
	pRequest->dwTongActivityOpenTime   = dwTongActivityOpenTime;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoApplyTongRepertoryPageRequest(DWORD dwPlayerID, BYTE byPageIndex, int nLastUpdateCounter)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	IMemBlock*                             piPackage   = NULL;
	S2R_APPLY_TONG_REPERTORY_PAGE_REQUEST*  pApply      = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_APPLY_TONG_REPERTORY_PAGE_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pApply = (S2R_APPLY_TONG_REPERTORY_PAGE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pApply);

	pApply->wProtocolID         = s2r_apply_tong_repertory_page_request;
	pApply->dwPlayerID          = dwPlayerID;
	pApply->byPageIndex         = byPageIndex;
	pApply->nLastUpdateCounter  = nLastUpdateCounter;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KinRelayClient::DoApplyOpenTongRepertoryRequest(DWORD dwPlayerID)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	S2R_APPLY_OPEN_TONG_REPERTORY_REQUEST*  pRequest    = NULL;
	IMemBlock*                             piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_APPLY_OPEN_TONG_REPERTORY_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_APPLY_OPEN_TONG_REPERTORY_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_apply_open_tong_repertory_request;
	pRequest->dwPlayerID    = dwPlayerID;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoTakeTongRepertoryItemToPosRequest(
	DWORD dwPlayerID, BYTE byPageIndex, BYTE byRepertoryPos,
	BYTE byBoxIndex, BYTE byBoxPos, int nLastUpdateCounter
	)
{
	BOOL                                            bResult     = false;
	BOOL                                            bRetCode    = false;
//	S2R_TAKE_TONG_REPERTORY_ITEM_TO_POS_REQUEST*    pRequest    = NULL;
//	IMemBlock*                                     piBuffer    = NULL;
//
//	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_TAKE_TONG_REPERTORY_ITEM_TO_POS_REQUEST));
//	LOG_PROCESS_ERROR(piBuffer);
//
//	pRequest = (S2R_TAKE_TONG_REPERTORY_ITEM_TO_POS_REQUEST*)piBuffer->GetData();
//	LOG_PROCESS_ERROR(pRequest);
//
//	pRequest->wProtocolID       = s2r_take_tong_repertory_item_to_pos_request;
//	pRequest->dwPlayerID        = dwPlayerID;
//	pRequest->byPageIndex       = byPageIndex;
//	pRequest->byRepertoryPos    = byRepertoryPos;
//	pRequest->byBoxIndex        = byBoxIndex;
//	pRequest->byBoxPos          = byBoxPos;
//	pRequest->nLastUpdateCounter = nLastUpdateCounter;
//
//	bRetCode = Send(piBuffer);
//	LOG_PROCESS_ERROR(bRetCode);
//
//	bResult= true;
//EXIT0:
//	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoTakeTongRepertoryItemRequest(DWORD dwPlayerID, BYTE byPageIndex, BYTE byRepertoryPos, BYTE byBoxIndex, BYTE byBoxPos, int nLastUpdateCounter)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	S2R_TAKE_TONG_REPERTORY_ITEM_REQUEST*   pRequest    = NULL;
	IMemBlock*                             piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_TAKE_TONG_REPERTORY_ITEM_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_TAKE_TONG_REPERTORY_ITEM_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID       = s2r_take_tong_repertory_item_request;
	pRequest->dwPlayerID        = dwPlayerID;
	pRequest->byPageIndex       = byPageIndex;
	pRequest->byRepertoryPos    = byRepertoryPos;
	pRequest->byBoxIndex        = byBoxIndex;
	pRequest->byBoxPos          = byBoxPos;
	pRequest->nLastUpdateCounter = nLastUpdateCounter;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult= true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoTakeTongRepertoryItemRespond(
	DWORD dwPlayerID, DWORD dwTongID, BYTE byPageIndex, BYTE byRepertoryPos, BOOL bSuccess
	)
{
	BOOL                                    bResult  = false;
	BOOL                                    bRetCode = false;
	S2R_TAKE_TONG_REPERTORY_ITEM_RESPOND*   pRespond = NULL;
	IMemBlock*                             piBuffer = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_TAKE_TONG_REPERTORY_ITEM_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (S2R_TAKE_TONG_REPERTORY_ITEM_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID     = s2r_take_tong_repertory_item_respond;
	pRespond->dwPlayerID      = dwPlayerID;
	pRespond->dwTongID        = dwTongID;
	pRespond->byPageIndex     = byPageIndex;
	pRespond->byPagePos       = byRepertoryPos;
	pRespond->bSuccess        = bSuccess;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = false;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoPutTongRepertoryItemToPosRequest(
	DWORD dwPlayerID, DWORD dwItemID, BYTE byBoxIndex, BYTE byBoxPos, BYTE byPageIndex, BYTE byPagePos
	)
{
	BOOL                                        bResult     = false;
	BOOL                                        bRetCode    = false;
//	S2R_PUT_TONG_REPERTORY_ITEM_TO_POS_REQUEST* pRequest    = NULL;
//	IMemBlock*                                 piBuffer    = NULL;
//
//	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_PUT_TONG_REPERTORY_ITEM_TO_POS_REQUEST));
//	LOG_PROCESS_ERROR(piBuffer);
//
//	pRequest = (S2R_PUT_TONG_REPERTORY_ITEM_TO_POS_REQUEST*)piBuffer->GetData();
//	LOG_PROCESS_ERROR(pRequest);
//
//	pRequest->wProtocolID       = s2r_put_tong_repertory_item_to_pos_request;
//	pRequest->dwPlayerID        = dwPlayerID;
//	pRequest->dwServerItemID    = dwItemID;
//	pRequest->byBoxIndex        = byBoxIndex;
//	pRequest->byBoxPos          = byBoxPos;
//	pRequest->byPageIndex       = byPageIndex;
//	pRequest->byPagePos         = byPagePos;
//
//	bRetCode = Send(piBuffer);
//	LOG_PROCESS_ERROR(bRetCode);
//
//	bResult = true;
//EXIT0:
//	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoPutTongRepertoryItemRequest(DWORD dwPlayerID, DWORD dwItemID, BYTE byBoxIndex, BYTE byBoxPos, BYTE byPageIndex, BYTE byPagePos, INT nItemDataLen, BYTE* pbyItemData)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	S2R_PUT_TONG_REPERTORY_ITEM_REQUEST*    pRequest    = NULL;
	IMemBlock*                             piBuffer    = NULL;

	LOG_PROCESS_ERROR(dwItemID > 0);

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_PUT_TONG_REPERTORY_ITEM_REQUEST) + nItemDataLen);
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_PUT_TONG_REPERTORY_ITEM_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID       = s2r_put_tong_repertory_item_request;
	pRequest->dwPlayerID        = dwPlayerID;
	pRequest->byBoxIndex        = byBoxIndex;
	pRequest->byBoxPos          = byBoxPos;
	pRequest->byPageIndex       = byPageIndex;
	pRequest->byPagePos         = byPagePos;
	pRequest->dwItemIdx         = dwItemID;
	pRequest->byItemDataLen     = nItemDataLen;
	
	memcpy(pRequest->byItemData, pbyItemData, nItemDataLen);

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoPutTongRepertoryItemRespond(DWORD dwPlayerID, BYTE byPageIndex, BYTE byPagePos, BOOL bFindItem, BYTE* pbyItemData, BYTE byItemDataLen)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	S2R_PUT_TONG_REPERTORY_ITEM_RESPOND*    pRespond    = NULL;
	IMemBlock*                              piBuffer    = NULL;
	size_t                                  uDataSize   = 0;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_PUT_TONG_REPERTORY_ITEM_RESPOND) + byItemDataLen);
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (S2R_PUT_TONG_REPERTORY_ITEM_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = s2r_put_tong_repertory_item_respond;
	pRespond->dwPlayerID    = dwPlayerID;
	pRespond->bFindItem     = bFindItem;
	pRespond->byPageIndex   = byPageIndex;
	pRespond->byPagePos     = byPagePos;
	pRespond->byItemDataLen = byItemDataLen;
	memcpy(pRespond->byItemData, pbyItemData, byItemDataLen);

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoUnlockTongRepertoryGrid(DWORD dwTongID, BYTE byPageIndex, BYTE byPagePos)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
//	S2R_UNLOCK_TONG_REPERTORY_GRID* pRespond    = NULL;
//	IMemBlock*                     piBuffer    = NULL;
//
//	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_UNLOCK_TONG_REPERTORY_GRID));
//	LOG_PROCESS_ERROR(piBuffer);
//
//	pRespond = (S2R_UNLOCK_TONG_REPERTORY_GRID*)piBuffer->GetData();
//	LOG_PROCESS_ERROR(pRespond);
//
//	pRespond->wProtocolID = s2r_unlock_tong_repertory_grid;
//	pRespond->dwTongID    = dwTongID;
//	pRespond->byPageIndex = byPageIndex;
//	pRespond->byPagePos   = byPagePos;
//
//	bRetCode = Send(piBuffer);
//	LOG_PROCESS_ERROR(bRetCode);
//
//	bResult = true;
//EXIT0:
//	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoExchangeTongRepertoryItemPosRequest(
	DWORD dwPlayerID, BYTE bySrcPageIndex, BYTE bySrcPagePos, int nSrcUpdateCounter,
	BYTE byDstPageIndex, BYTE byDstPagePos, int nDstUpdateCounter
	)
{
	BOOL                                            bResult     = false;
	BOOL                                            bRetCode    = false;
	IMemBlock*                                     piBuffer    = NULL;
	S2R_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST*   pRequest    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID       = s2r_exchange_tong_repertory_item_pos_request;
	pRequest->dwPlayerID        = dwPlayerID;
	pRequest->bySrcPageIndex    = bySrcPageIndex;
	pRequest->bySrcPagePos      = bySrcPagePos;
	pRequest->byDstPageIndex    = byDstPageIndex;
	pRequest->byDstPagePos      = byDstPagePos;
	pRequest->nSrcUpdateCounter = nSrcUpdateCounter;
	pRequest->nDstUpdateCounter = nDstUpdateCounter;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoApplyStackTongRepertoryItemRequest(
	DWORD dwPlayerID, DWORD dwTongID,
	BYTE byPageIndex, BYTE byPagePos, BYTE byBoxIndex, BYTE byBoxPos, DWORD dwItemID,
	BYTE byTabType, WORD wTabIndex, WORD wStackNum, WORD wMaxStackNum, int nLastUpdateCounter
	)
{
	BOOL                                            bResult     = false;
	BOOL                                            bRetCode    = false;
	IMemBlock*                                     piBuffer    = NULL;
//	S2R_APPLY_STACK_TONG_REPERTORY_ITEM_REQUEST*    pRequest = NULL;
//
//	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_APPLY_STACK_TONG_REPERTORY_ITEM_REQUEST));
//	LOG_PROCESS_ERROR(piBuffer);
//
//	pRequest = (S2R_APPLY_STACK_TONG_REPERTORY_ITEM_REQUEST*)piBuffer->GetData();
//	LOG_PROCESS_ERROR(pRequest);
//
//	pRequest->wProtocolID   = s2r_apply_stack_tong_repertory_item_request;
//	pRequest->dwPlayerID    = dwPlayerID;
//	pRequest->dwTongID      = dwTongID;
//	pRequest->byPageIndex   = byPageIndex;
//	pRequest->byPagePos     = byPagePos;
//	pRequest->byBoxIndex    = byBoxIndex;
//	pRequest->byBoxPos      = byBoxPos;
//	pRequest->dwItemID      = dwItemID;
//	pRequest->byTabType     = byTabType;
//	pRequest->wTabIndex     = wTabIndex;
//	pRequest->wStackNum     = wStackNum;
//	pRequest->wMaxStackNum  = wMaxStackNum;
//	pRequest->nLastUpdateCounter = nLastUpdateCounter;
//
//	bRetCode = Send(piBuffer);
//	LOG_PROCESS_ERROR(bRetCode);
//
//	bResult = true;
//EXIT0:
//	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoStackTongRepertoryItemRequest(
	DWORD dwPlayerID, DWORD dwTongID,
	BYTE byPageIndex, BYTE byPagePos, BYTE byTabType, WORD wTabIndex,
	WORD wStackNum, WORD wMaxStackNum
	)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	IMemBlock*                             piBuffer    = NULL;
	S2R_STACK_TONG_REPERTORY_ITEM_REQUEST*  pRequest = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_STACK_TONG_REPERTORY_ITEM_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_STACK_TONG_REPERTORY_ITEM_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_stack_tong_repertory_item_request;
	pRequest->dwPlayerID    = dwPlayerID;
	pRequest->dwTongID      = dwTongID;
	pRequest->byPageIndex   = byPageIndex;
	pRequest->byPagePos     = byPagePos;
	pRequest->byTabType     = byTabType;
	pRequest->wTabIndex     = wTabIndex;
	pRequest->wStackNum     = wStackNum;
	pRequest->wMaxStackNum  = wMaxStackNum;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}


BOOL KinRelayClient::DoRemoveTongRepertoryItemRequest( DWORD dwPlayerID, BYTE byPageIndex, BYTE byRepertoryPos, int nLastUpdateCounter )
{
	BOOL                                     bResult     = false;
	BOOL                                     bRetCode    = false;
	S2R_REMOVE_TONG_REPERTORY_ITEM_REQUEST*  pRequest    = NULL;
	IMemBlock*                               piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_REMOVE_TONG_REPERTORY_ITEM_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_REMOVE_TONG_REPERTORY_ITEM_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID        = s2r_remove_tong_repertory_item_request;
	pRequest->dwPlayerID         = dwPlayerID;
	pRequest->byPageIndex        = byPageIndex;
	pRequest->byPagePos          = byRepertoryPos;
	pRequest->nLastUpdateCounter = nLastUpdateCounter;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult= true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::DoGetBackItemFromRepertoryRequest( DWORD dwPlayerID, DWORD dwTongID, BYTE byBoxIndex, BYTE byBoxPos, const GUID& sGuid, INT nLastUpdateCounter)
{
	BOOL                                      bResult     = false;
	BOOL                                      bRetCode    = false;
	S2R_GET_BACK_ITEM_FROM_REPERTORY_REQUEST* pRequest    = NULL;
	IMemBlock*                                piBuffer    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_GET_BACK_ITEM_FROM_REPERTORY_REQUEST));
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_GET_BACK_ITEM_FROM_REPERTORY_REQUEST*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_get_back_item_from_repertory_request;
	pRequest->dwPlayerID = dwPlayerID;
	pRequest->dwTongID = dwTongID;
	pRequest->byBoxIndex = byBoxIndex;
	pRequest->byBoxPos = byBoxPos;
	pRequest->sGuid = sGuid;
	pRequest->nLastUpdateCounter = nLastUpdateCounter;

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult= true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KinRelayClient::Send( IMemBlock* piBuffer )
{
	return g_RelayClient.Send(piBuffer);
}

