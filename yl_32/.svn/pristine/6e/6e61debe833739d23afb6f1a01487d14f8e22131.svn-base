
#include "stdafx.h"
#include "MailRelayClient.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "../onlinegameworld/KRelayClient.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "MailModule2.h"
#include "../onlinegamemodule/item2/kplayeritem.h"
#include "onlinegamemodule/trade/kplayerpurse.h"


MailRelayClient::MailRelayClient(void)
{
}

MailRelayClient::~MailRelayClient(void)
{
}

BOOL MailRelayClient::DoSendMailRequest( INT nRequestID, const CHAR cszDstName[], KMail* pMail, size_t uMailLen )
{
	BOOL						bRetCode	= FALSE;
	IMemBlock*					piMemBlock	= NULL;
	S2R_SEND_MAIL_REQUEST*		pRequest	= NULL;

	LOG_PROCESS_ERROR(nRequestID);
	LOG_PROCESS_ERROR(cszDstName);
	LOG_PROCESS_ERROR(pMail);
	LOG_PROCESS_ERROR(uMailLen >= sizeof(KMail));

	piMemBlock = QCreateMemBlock(sizeof(S2R_SEND_MAIL_REQUEST) + uMailLen);
	LOG_PROCESS_ERROR(piMemBlock);

	pRequest = (S2R_SEND_MAIL_REQUEST*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_send_mail_request;
	pRequest->byRequestID = nRequestID;
	QStrCpyLen(pRequest->szReceiverName, cszDstName, sizeof(pRequest->szReceiverName));
	memcpy(pRequest->byData, pMail, uMailLen);

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoSendGlobalMailRequest( KMail* pMail, size_t uMailLen, INT nLifeTime )
{
	BOOL							bRetCode	= FALSE;
	IMemBlock*						piMemBlock	= NULL;
	S2R_SEND_GLOBAL_MAIL_REQUEST*	pRequest	= NULL;

	LOG_PROCESS_ERROR(pMail);
	LOG_PROCESS_ERROR(uMailLen >= sizeof(KMail));

	piMemBlock = QCreateMemBlock(sizeof(S2R_SEND_GLOBAL_MAIL_REQUEST) + uMailLen);
	LOG_PROCESS_ERROR(piMemBlock);

	pRequest = (S2R_SEND_GLOBAL_MAIL_REQUEST*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID	= s2r_send_global_mail_request;
	pRequest->nLifeTime		= nLifeTime;
	memcpy(pRequest->byData, pMail, uMailLen);

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoGetMailListRequest( DWORD dwPlayerID, DWORD dwStartID )
{
	BOOL						bRetCode	= FALSE;
	IMemBlock*					piMemBlock = NULL;
	S2R_GET_MAILLIST_REQUEST*	pRequest	= NULL;

	LOG_PROCESS_ERROR(dwPlayerID);
	// LOG_PROCESS_ERROR(dwStartID);

	piMemBlock = QCreateMemBlock(sizeof(S2R_GET_MAILLIST_REQUEST));
	LOG_PROCESS_ERROR(piMemBlock);

	pRequest = (S2R_GET_MAILLIST_REQUEST*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID	= s2r_get_maillist_request;
	pRequest->dwPlayerID	= dwPlayerID;
	pRequest->dwStartID		= dwStartID;

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoQueryMailContent( DWORD dwPlayerID, DWORD dwMailID, INT nMailType )
{
	BOOL						bRetCode	= FALSE;
	IMemBlock*					piMemBlock = NULL;
	S2R_QUERY_MAIL_CONTENT*		pRequest	= NULL;

	LOG_PROCESS_ERROR(dwPlayerID);
	LOG_PROCESS_ERROR(dwMailID);

	piMemBlock = QCreateMemBlock(sizeof(S2R_QUERY_MAIL_CONTENT));
	LOG_PROCESS_ERROR(piMemBlock);

	pRequest = (S2R_QUERY_MAIL_CONTENT*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID	= s2r_query_mail_content;
	pRequest->dwPlayerID	= dwPlayerID;
	pRequest->dwMailID		= dwMailID;
	pRequest->byMailType	= (BYTE)nMailType;

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoAcquireMailMoneyRequest( DWORD dwPlayerID, DWORD dwMailID )
{
	BOOL								bRetCode	= FALSE;
	IMemBlock*							piMemBlock	= NULL;
	S2R_ACQUIRE_MAIL_MONEY_REQUEST*		pRequest	= NULL;

	LOG_PROCESS_ERROR(dwPlayerID);
	LOG_PROCESS_ERROR(dwMailID);

	piMemBlock = QCreateMemBlock(sizeof(S2R_ACQUIRE_MAIL_MONEY_REQUEST));
	LOG_PROCESS_ERROR(piMemBlock);

	pRequest = (S2R_ACQUIRE_MAIL_MONEY_REQUEST*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID	= s2r_acquire_mail_money_request;
	pRequest->dwPlayerID	= dwPlayerID;
	pRequest->dwMailID		= dwMailID;

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoAcquireMailItemRequest( DWORD dwPlayerID, DWORD dwMailID, INT nIndex )
{
	BOOL							bRetCode	= FALSE;
	IMemBlock*						piMemBlock	= NULL;
	S2R_ACQUIRE_MAIL_ITEM_REQUEST*	pRequest	= NULL;

	LOG_PROCESS_ERROR(dwPlayerID);
	LOG_PROCESS_ERROR(dwMailID);

	piMemBlock = QCreateMemBlock(sizeof(S2R_ACQUIRE_MAIL_ITEM_REQUEST));
	LOG_PROCESS_ERROR(piMemBlock);

	pRequest = (S2R_ACQUIRE_MAIL_ITEM_REQUEST *)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID	= s2r_acquire_mail_item_request;
	pRequest->dwPlayerID	= dwPlayerID;
	pRequest->dwMailID		= dwMailID;
	pRequest->byIndex		= (BYTE)nIndex;

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoGiveMailMoneyToPlayerRespond( DWORD dwPlayerID, DWORD dwMailID, KMAIL_RESULT_CODE eResult )
{
 	BOOL									bRetCode	= FALSE;
	IMemBlock*								piMemBlock	= NULL;
	S2R_GIVE_MAIL_MONEY_TO_PLAYER_RESPOND*	pRespond	= NULL;

	LOG_PROCESS_ERROR(dwPlayerID);
	LOG_PROCESS_ERROR(dwMailID);

	piMemBlock = QCreateMemBlock(sizeof(S2R_GIVE_MAIL_MONEY_TO_PLAYER_RESPOND));
	LOG_PROCESS_ERROR(piMemBlock);

	pRespond = (S2R_GIVE_MAIL_MONEY_TO_PLAYER_RESPOND*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID	= s2r_give_mail_money_to_player;
	pRespond->dwPlayerID	= dwPlayerID;
	pRespond->dwMailID		= dwMailID;
	pRespond->byResult		= (BYTE)eResult;

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoGiveMailItemToPlayerRespond( DWORD dwPlayerID, DWORD dwMailID, INT nIndex, KMAIL_RESULT_CODE eResult )
{
	BOOL									bRetCode	= FALSE;
	IMemBlock*								piMemBlock	= NULL;
	S2R_GIVE_MAIL_ITEM_TO_PLAYER_RESPOND*	pRespond	= NULL;

	LOG_PROCESS_ERROR(dwPlayerID);
	LOG_PROCESS_ERROR(dwMailID);

	piMemBlock = QCreateMemBlock(sizeof(S2R_GIVE_MAIL_ITEM_TO_PLAYER_RESPOND));
	LOG_PROCESS_ERROR(piMemBlock);

	pRespond = (S2R_GIVE_MAIL_ITEM_TO_PLAYER_RESPOND*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID	= s2r_give_mail_item_to_player;
	pRespond->dwPlayerID	= dwPlayerID;
	pRespond->dwMailID		= dwMailID;
	pRespond->byIndex		= (BYTE)nIndex;
	pRespond->byResult		= (BYTE)eResult;

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoSetMailRead( DWORD dwSrcID, DWORD dwMailID )
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMemBlock	= NULL;
	S2R_SET_MAIL_READ*	pRequest	= NULL;

	LOG_PROCESS_ERROR(dwSrcID);
	LOG_PROCESS_ERROR(dwMailID);

	piMemBlock = QCreateMemBlock(sizeof(S2R_SET_MAIL_READ));
	LOG_PROCESS_ERROR(piMemBlock);

	pRequest = (S2R_SET_MAIL_READ*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID	= s2r_set_mail_read;
	pRequest->dwPlayerID	= dwSrcID;
	pRequest->dwMailID		= dwMailID;

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoDeleteMail( DWORD dwSrcID, DWORD dwMailID )
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMemBlock = NULL;
	S2R_DELETE_MAIL*	pRequest	= NULL;

	LOG_PROCESS_ERROR(dwSrcID);
	LOG_PROCESS_ERROR(dwMailID);

	piMemBlock = QCreateMemBlock(sizeof(S2R_DELETE_MAIL));
	LOG_PROCESS_ERROR(piMemBlock);

	pRequest = (S2R_DELETE_MAIL*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID	= s2r_delete_mail;
	pRequest->dwPlayerID	= dwSrcID;
	pRequest->dwMailID		= dwMailID;

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

BOOL MailRelayClient::DoReturnMail( DWORD dwSrcID, DWORD dwMailID )
{
	BOOL				bRetCode	= FALSE;
	IMemBlock*			piMemBlock = NULL;
	S2R_RETURN_MAIL*	pRequest	= NULL;

	LOG_PROCESS_ERROR(dwSrcID);
	LOG_PROCESS_ERROR(dwMailID);

	piMemBlock = QCreateMemBlock(sizeof(S2R_RETURN_MAIL));
	LOG_PROCESS_ERROR(piMemBlock);

	pRequest = (S2R_RETURN_MAIL*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID	= s2r_return_mail;
	pRequest->dwPlayerID	= dwSrcID;
	pRequest->dwMailID		= dwMailID;

	bRetCode = g_RelayClient.Send(piMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;
}

// ====================================================================

void MailRelayClient::OnSendMailRespond( BYTE* pbyData, size_t uDataLen )
{
	BOOL					bRetCode	= FALSE;
	KPlayer*				pPlayer		= NULL;
	KPlayerItem*			pPlayerItem	= NULL;
	TDBItemData*			pItemData	= NULL;
	INT						nItemIndex	= 0;
	KPlayer*				pReceiver	= NULL;
	R2S_SEND_MAIL_RESPOND*	pRespond	= (R2S_SEND_MAIL_RESPOND*)pbyData;
	KMail*					pMail		= NULL;
	KMailListInfo			newMailInfo;
	KITEM_POS				cItemPos;

	LOG_PROCESS_ERROR(pbyData);
	pMail = (KMail*)pRespond->byData;

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwSrcID);
	LOG_PROCESS_ERROR(pPlayer);	

	switch (pRespond->byResult) 
	{
	case mrcMailBoxFull:
	case mrcFailed:
		{
			// 发送失败，需要将玩家的装备给加上
			pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
			LOG_PROCESS_ERROR(pPlayerItem);

			BYTE* pbyData = pMail->byData;
			pbyData += pMail->wTextLen;
			for (int i = 0; i < KMAIL_MAX_ITEM_COUNT; ++i)
			{
				if (pMail->ItemDesc[i].byDataLen > 0)
				{
					bRetCode = pPlayerItem->GetBagFreeCell(cItemPos);
					LOG_PROCESS_ERROR(bRetCode);

					pItemData = (TDBItemData*)pbyData;
					nItemIndex = pPlayerItem->MakeItem(pItemData);
					LOG_PROCESS_ERROR(nItemIndex);

					bRetCode = pPlayerItem->Add(nItemIndex, cItemPos, emKADDITEM_MAIL);
					LOG_PROCESS_ERROR(bRetCode);

					pbyData += pMail->ItemDesc[i].byDataLen;
				}
			}
		}
		break;
	}

	if (pRespond->byResult == mrcSucceed)
	{
		pReceiver = g_cPlayerMgr.GetByName(pRespond->szReceiver);
		if (pReceiver)
		{
			memset(&newMailInfo, 0, sizeof(KMailListInfo));

			newMailInfo.byType		= pMail->byType;
			newMailInfo.dwMailID	= pMail->dwMailID;

			QStrCpyLen(newMailInfo.szSender, pMail->szSenderName, sizeof(pMail->szSenderName));
			newMailInfo.szSender[_NAME_LEN - 1] = '\0';

			QStrCpyLen(newMailInfo.szTitle, pMail->szTitle, sizeof(pMail->szTitle));
			newMailInfo.szTitle[MAIL_TITLE_LEN - 1] = '\0';

			newMailInfo.nSendTime	= pMail->nSendTime;
			newMailInfo.nRecvTime	= pMail->nRecvTime;
			newMailInfo.bReadFlag	= pMail->bRead;
			newMailInfo.bMoneyFlag	= (pMail->nMoney > 0 || pMail->nSilver > 0 || pMail ->nEnergy) ? TRUE : FALSE;
			newMailInfo.bItemFlag	= false;
			for (int i = 0; i < KMAIL_MAX_ITEM_COUNT; ++i)
			{
				if (pMail->ItemDesc[i].byDataLen > 0)
				{
					newMailInfo.bItemFlag = true;
					break;
				}
			}
			g_cMailModule2.DoNewMailNotify(pReceiver, &newMailInfo);
		}
	}

	g_cMailModule2.DoSendMailRespond(
		pPlayer, 
		pRespond->byRespondID, 
		pRespond->byResult
		);

EXIT0:
	return;
}

void MailRelayClient::OnGetMailListRespond( BYTE* pbyData, size_t uDataLen )
{
	KPlayer*					pPlayer		= NULL;
	R2S_GET_MAILLIST_RESPOND*	pRespond	= (R2S_GET_MAILLIST_RESPOND*)pbyData;

	LOG_PROCESS_ERROR(pbyData);

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	g_cMailModule2.DoGetMailListRespond(
		pPlayer, 
		pRespond->MailList, 
		pRespond->nMailCount
		);

EXIT0:
	return;
}

void MailRelayClient::OnSyncMailContent( BYTE* pbyData, size_t uDataLen )
{
	KPlayer*				pPlayer	= NULL;
	R2S_SYNC_MAIL_CONTENT*	pContent	= (R2S_SYNC_MAIL_CONTENT*)pbyData;
	KVIEWITEMINFO*			pViewInfo		= NULL;
	TDBItemData*			pItemData		= NULL;
	KMailContent*			pMailContent	= NULL;
	KMailContent*			pSyncContent	= NULL;
	IMemBlock*				piMemBlock		= NULL;
	INT						nMaxContentLen	= 0;
	INT						nSyncContentLen	= 0;
	BYTE*					pbySyncItemOffset		= NULL;
	BYTE*					pbyContentItemOffset	= NULL;

	LOG_PROCESS_ERROR(pbyData);

	pPlayer = g_cPlayerMgr.GetById(pContent->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	pMailContent = (KMailContent*)pContent->byData;

	// |KmailContent|Text|ItemViewInfo|ItemViewInfo|..
	nMaxContentLen = sizeof(KMailContent) + pMailContent->wTextLen + KMAIL_MAX_ITEM_COUNT * sizeof(KVIEWITEMINFO);
	nSyncContentLen = sizeof(KMailContent) + pMailContent->wTextLen;

	piMemBlock = QCreateMemBlock(nMaxContentLen);
	LOG_PROCESS_ERROR(piMemBlock);

	pSyncContent = (KMailContent*)piMemBlock->GetData();
	LOG_PROCESS_ERROR(pSyncContent);

	// 邮件文本信息
	pSyncContent->nMoney	= pMailContent->nMoney;
	pSyncContent->nSilver   = pMailContent->nSilver;
	pSyncContent->nEnergy   = pMailContent->nEnergy;
	
	pSyncContent->wTextLen	= pMailContent->wTextLen;

	memcpy(pSyncContent->byData, pMailContent->byData, pMailContent->wTextLen);

	// 装备道具信息
	pbySyncItemOffset = pSyncContent->byData + pSyncContent->wTextLen;
	pbyContentItemOffset = pMailContent->byData + pMailContent->wTextLen;
	for (int i = 0; i < KMAIL_MAX_ITEM_COUNT; ++i)
	{
		pSyncContent->ItemDesc[i].bAcquired = false;
		pSyncContent->ItemDesc[i].byDataLen = 0;

		if (pMailContent->ItemDesc[i].byDataLen == 0)
			continue;;

		pSyncContent->ItemDesc[i].byDataLen = sizeof(KVIEWITEMINFO);
		pSyncContent->ItemDesc[i].bAcquired = pMailContent->ItemDesc[i].bAcquired;

		pViewInfo = (KVIEWITEMINFO*)pbySyncItemOffset;//
		memset(pViewInfo, 0, sizeof(KVIEWITEMINFO));
		pItemData = (TDBItemData*)pbyContentItemOffset;

		pViewInfo->dwID = 0;
		pViewInfo->byGenre       = (BYTE)pItemData->wGenre;
		pViewInfo->wDetail       = pItemData->wDetailType;
		pViewInfo->wParticular   = pItemData->wParticularType;
		pViewInfo->dwLevel       = pItemData->byLevel;
		pViewInfo->dwRandomSeed  = pItemData->dwRandSeed;
		pViewInfo->dwRandomSeedEx = pItemData->dwRandSeedEx;
		pViewInfo->wCount        = (WORD)pItemData->dwCount;
		pViewInfo->byEnhance     = pItemData->byEnhance;
		// pViewInfo->byStoneInfo = pItemData->arydwStoneInfo;
		memset(&pViewInfo->byStoneInfo[0], 0, sizeof(pViewInfo->byStoneInfo));
		pViewInfo->bBind         = pItemData->byStateFlags & (0x01 << 1);
		pViewInfo->byTimeoutType = pItemData->byTimeoutType;
		pViewInfo->dwTimeout     = pItemData->dwTimeout;

		nSyncContentLen += sizeof(KVIEWITEMINFO);

		pbySyncItemOffset += sizeof(KVIEWITEMINFO);
		pbyContentItemOffset += sizeof(TDBItemData);
	}

	g_cMailModule2.DoSyncMailContent(
		pPlayer, 
		pContent->dwMailID, 
		pContent->byResult, 
		nSyncContentLen,
		(BYTE*)pSyncContent
		);

EXIT0:
	SAFE_RELEASE(piMemBlock);
	return;
}

void MailRelayClient::OnGiveMailMoneyToPlayer( BYTE* pbyData, size_t uDataLen )
{
	BOOL 							bResult = FALSE;
	KPlayerPurse* 					pPlayerPurse = NULL;
	KE_PURSE_RESULT 				nResult	= emKPURSE_S_SUCCESS;
	KPlayer*						pPlayer	= NULL;
	R2S_GIVE_MAIL_MONEY_TO_PLAYER*	pRespond	= (R2S_GIVE_MAIL_MONEY_TO_PLAYER*)pbyData;
	KPlayerItem*					pPlayerItem = NULL;
	KMAIL_RESULT_CODE				mailErrorCode	= mrcFailed;

 	LOG_PROCESS_ERROR(pbyData);
 
	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);
 
	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	LOG_PROCESS_ERROR(pPlayerPurse);

	if (pRespond->nMoney > 0)
	{
		pPlayerPurse->AddMoney(EM_MONEY_COIN, pRespond->nMoney, &nResult);
		LOG_PROCESS_ERROR(nResult == emKPURSE_S_SUCCESS);
	}
	
	if (pRespond->nSilver > 0)
	{
		pPlayerPurse->AddMoney(EM_MONEY_SILVER, pRespond->nSilver, &nResult);
		LOG_PROCESS_ERROR(nResult == emKPURSE_S_SUCCESS);
	}

	if (pRespond->nEnergy > 0)
	{
		pPlayerPurse->AddMoney(EM_MONEY_YLENERGY, pRespond->nEnergy, &nResult);
		LOG_PROCESS_ERROR(nResult == emKPURSE_S_SUCCESS);
	}
	
	mailErrorCode = mrcSucceed;
EXIT0:
	DoGiveMailMoneyToPlayerRespond(pRespond->dwPlayerID, pRespond->dwMailID, mailErrorCode);
	return;
}

void MailRelayClient::OnGiveMailItemToPlayer( BYTE* pbyData, size_t uDataLen )
{
	BOOL							bRetCode	= FALSE;
	KPlayer*						pPlayer		= NULL;
	R2S_GIVE_MAIL_ITEM_TO_PLAYER*	pRespond	= (R2S_GIVE_MAIL_ITEM_TO_PLAYER*)pbyData;
	TDBItemData*					pItemData	= NULL;
	KPlayerItem*					pPlayerItem = NULL;
	INT								nItemIdx	= 0;
	KMAIL_RESULT_CODE				mailErrorCode	= mrcFailed;
	KITEM_POS						cItemPos;

	LOG_PROCESS_ERROR(pbyData);
	LOG_PROCESS_ERROR(pRespond->byItemLen == sizeof(TDBItemData));

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	pItemData = (TDBItemData*)pRespond->byData;

	bRetCode = pPlayerItem->GetBagFreeCell(cItemPos);

	if (!bRetCode)
	{
		mailErrorCode = mrcNotEnoughRoom;
		goto EXIT0;
	}

	nItemIdx = pPlayerItem->MakeItem(pItemData);
	LOG_PROCESS_ERROR(nItemIdx);

	bRetCode = pPlayerItem->Add(nItemIdx, cItemPos, emKADDITEM_MAIL);
	LOG_PROCESS_ERROR(bRetCode);

	mailErrorCode = mrcSucceed;

EXIT0:
	DoGiveMailItemToPlayerRespond(pRespond->dwPlayerID, pRespond->dwMailID, pRespond->byItemIndex, mailErrorCode);
	return;
}

void MailRelayClient::OnNewMailNotify( BYTE* pbyData, size_t uDataLen )
{
	KPlayer*				pPlayer = NULL;
	R2S_NEW_MAIL_NOTIFY*	pNotify = (R2S_NEW_MAIL_NOTIFY*)pbyData;

	LOG_PROCESS_ERROR(pbyData);

	pPlayer = g_cPlayerMgr.GetById(pNotify->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	g_cMailModule2.DoNewMailNotify(
		pPlayer,
		&pNotify->NewMailListInfo
		);

EXIT0:
	return;
}

void MailRelayClient::OnMailGeneralRespond( BYTE* pbyData, size_t uDataLen )
{
	KPlayer*					pPlayer = NULL;
	R2S_MAIL_GENERAL_RESPOND*	pRespond = (R2S_MAIL_GENERAL_RESPOND*)pbyData;

	LOG_PROCESS_ERROR(pbyData);

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	g_cMailModule2.DoMailGeneralRespond(
		pPlayer,
		pRespond->dwMailID,
		pRespond->byRespond
		);

EXIT0:
	return;
}
