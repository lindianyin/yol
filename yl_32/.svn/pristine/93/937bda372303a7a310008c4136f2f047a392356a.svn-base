
#include "stdafx.h"
#include "config/MailDef.h"
#include "MailModule2.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/knpc.h"
#include "onlinegamebase/ksysservice.h"
#include "MailRelayClient.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamemodule/kmoduledef.h"
#include "item2/kplayeritem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMMON_PLAYER_OPERATION_DISTANCE (CELL_LENGTH * 10) // TODO:

#define KMAIL_COST 0

QMailModule g_cMailModule2;

#define REGISTER_EXTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
	m_anProtocolSizeC2S[ProtocolID] = ProtocolSize;}						

template<>
IKModuleInterface* GetModuleInterFace<emKMODULE_MAIL>()
{
	return &g_cMailModule2;
}

BOOL QMailModule::InitProtocol()
{
	// 定长
	KD_PTC_FIXED_SIZE_S2C(S2C_SEND_MAIL_RESPOND);
	KD_PTC_FIXED_SIZE_S2C(S2C_NEW_MAIL_NOTIFY);
	KD_PTC_FIXED_SIZE_S2C(S2C_MAIL_GENERAL_RESPOND);

	// 变长
	KD_PTC_LENGTNED_SIZE_S2C(S2C_GET_MAIL_LIST_RESPOND);
	KD_PTC_LENGTNED_SIZE_S2C(S2C_SYNC_MAIL_CONTENT);

	return TRUE;
}

BOOL QMailModule::OnInit()
{
	REGISTER_EXTERNAL_FUNC(c2s_send_mail_request, &QMailModule::OnSendMailRequest, -1);
	REGISTER_EXTERNAL_FUNC(c2s_get_maillist_request, &QMailModule::OnGetMaillistRequest, sizeof(C2S_GET_MAILLIST_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_query_mail_content, &QMailModule::OnQueryMailContent, sizeof(C2S_QUERY_MAIL_CONTENT));
	REGISTER_EXTERNAL_FUNC(c2s_acquire_mail_money_request, &QMailModule::OnAcquireMailMoneyRequest, sizeof(C2S_ACQUIRE_MAIL_MONEY_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_acquire_mail_item_request, &QMailModule::OnAcquireMailItemRequest, sizeof(C2S_ACQUIRE_MAIL_ITEM_REQUEST));
	REGISTER_EXTERNAL_FUNC(c2s_set_mail_read, &QMailModule::OnSetMailRead, sizeof(C2S_SET_MAIL_READ));
	REGISTER_EXTERNAL_FUNC(c2s_delete_mail, &QMailModule::OnDeleteMail, sizeof(C2S_DELETE_MAIL));
	REGISTER_EXTERNAL_FUNC(c2s_return_mail, &QMailModule::OnReturnMail, sizeof(C2S_RETURN_MAIL));
	return TRUE;
}

BOOL QMailModule::OnUnInit()
{
	return TRUE;
}

BOOL QMailModule::ProcessProtocol( INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen )
{
	BOOL					bResult		= FALSE;
	PROCESS_PROTOCOL_FUNC	pFunc		= NULL;
	KPTC_HEADER_BASE*		pHeader		= (KPTC_HEADER_BASE*)pbData;
	KPlayer*				pPlayer		= g_cPlayerMgr.GetByIndex(nPlayerIndex);

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pbData);
	LOG_PROCESS_ERROR(uDataLen);

	pFunc = m_ProcessProtocolFuns[pHeader->byProtocol];
	LOG_PROCESS_ERROR(pFunc);

	(this->*pFunc)(pPlayer, (char*)pbData, uDataLen);

	bResult = TRUE;
EXIT0:
	return bResult;
}

QMailModule::QMailModule(void) : KModuleBase(emKMODULE_MAIL)
{
}

QMailModule::~QMailModule(void)
{
}

BOOL QMailModule::DoSendMailRespond( KPlayer* pPlayer, int nRespondID, int nResult )
{
	BOOL						bRetCode	= FALSE;
	S2C_SEND_MAIL_RESPOND*		pRespond	= (S2C_SEND_MAIL_RESPOND*)m_byTempData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(nRespondID);

	pRespond->byProtocol	= s2c_send_mail_respond;
	pRespond->byRespondID	= nRespondID;
	pRespond->byResult		= (BYTE)nResult;

	bRetCode = SendData(pPlayer->GetPlayerIndex(), *pRespond);
	LOG_PROCESS_ERROR(bRetCode);
	
	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL QMailModule::DoGetMailListRespond( KPlayer* pPlayer, KMailListInfo* pMailListInfo, int nMailCount )
{
	BOOL							bRetCode			= FALSE;
	INT								nMailListDataLen	= 0;
	S2C_GET_MAIL_LIST_RESPOND*		pRespond			= (S2C_GET_MAIL_LIST_RESPOND*)m_byTempData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pMailListInfo);

	pRespond->byProtocol	= s2c_get_mail_list_respond;
	pRespond->byMailCount	= nMailCount;

	nMailListDataLen = sizeof(KMailListInfo) * nMailCount;

	if (nMailListDataLen > 0)
	{
		memcpy(pRespond->MailListInfo, pMailListInfo, nMailListDataLen);
	}

	bRetCode = SendLenData(pPlayer->GetPlayerIndex(), *pRespond, sizeof(S2C_GET_MAIL_LIST_RESPOND) + nMailListDataLen);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL QMailModule::DoNewMailNotify( KPlayer* pPlayer, KMailListInfo* pMailListInfo )
{
	BOOL						bRetCode	= FALSE;
	S2C_NEW_MAIL_NOTIFY*		pRespond	= (S2C_NEW_MAIL_NOTIFY*)m_byTempData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pMailListInfo);

	pRespond->byProtocol		= s2c_new_mail_notify;
	pRespond->NewMailListInfo	= *pMailListInfo;

	bRetCode = SendData(pPlayer->GetPlayerIndex(), *pRespond);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL QMailModule::DoSyncMailContent( KPlayer* pPlayer, DWORD dwMailID, int nResult, int nDataLen, BYTE* pbyData)
{
	BOOL					bRetCode		= FALSE;
	INT						nContentLen		= 0;
	S2C_SYNC_MAIL_CONTENT*	pSyncData		= (S2C_SYNC_MAIL_CONTENT*)m_byTempData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(dwMailID);
	LOG_PROCESS_ERROR(pbyData);

	pSyncData->byProtocol	= s2c_sync_mail_content;
	pSyncData->dwMailID		= dwMailID;
	pSyncData->byResult		= nResult;

	memcpy(pSyncData->byData, pbyData, nDataLen);

	bRetCode = SendLenData(pPlayer->GetPlayerIndex(), *pSyncData, sizeof(S2C_SYNC_MAIL_CONTENT) + nDataLen);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL QMailModule::DoMailGeneralRespond( KPlayer* pPlayer, DWORD dwMailID, int nResult )
{
	BOOL							bRetCode		= FALSE;
	S2C_MAIL_GENERAL_RESPOND*		pRespond		= (S2C_MAIL_GENERAL_RESPOND*)m_byTempData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(dwMailID);

	pRespond->byProtocol	= s2c_mail_general_respond;
	pRespond->dwMailID		= dwMailID;
	pRespond->byResult		= (BYTE)nResult;

	bRetCode = SendData(pPlayer->GetPlayerIndex(), *pRespond);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

//==========================================================================================

void QMailModule::OnSendMailRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	QLogPrintf(LOG_INFO,"..QMailModule::OnSendMailRequest..");
	BOOL                    bRetCode        = false;
	KNpc*                   pNpc            = NULL;
	C2S_SEND_MAIL_REQUEST*  pRequest        = (C2S_SEND_MAIL_REQUEST*)pData;
	int                     nRequestID      = pRequest->byRequestID;
	KItem*                  pItem           = NULL;
	int                     nDelayTime      = 0;
	size_t                  uItemDataLen    = 0;
	size_t                  uMailLen        = 0;
	int                     nPlayerMoney    = 0;
	KMail*                  pMail           = NULL;
	BYTE*                   pbyOffset       = NULL;
	BYTE*                   pbyBuffer       = NULL;
	TDBItemData*			pItemData		= NULL;
	KPlayerItem*			pPlayerItem		= NULL;
	INT						nItemIdx		= 0;
	INT						nMoney			= 0;
	KITEM_POS				cItemPos;

	assert(pPlayer);

	pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

// 	pNpc = 0;
// 	LOG_PROCESS_ERROR(pNpc);
// 
// 	bRetCode = g_InRange(pPlayer, pNpc, COMMON_PLAYER_OPERATION_DISTANCE);
// 	LOG_PROCESS_ERROR(bRetCode);

	pRequest->szDstName[sizeof(pRequest->szDstName) - 1] = '\0';
	pRequest->szTitle[sizeof(pRequest->szTitle) - 1] = '\0';

	// TODO call script can send mail

	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_SEND_MAIL_REQUEST) + pRequest->wTextLen);
	LOG_PROCESS_ERROR(pRequest->wTextLen > 0); // 空字符串也有1个字节
	LOG_PROCESS_ERROR(pRequest->wTextLen <= MAX_MAIL_TEXT_LEN);
	pRequest->szText[pRequest->wTextLen - 1] = '\0';

	// 不能给自己发邮件
	if (QStrCaseCmp(pPlayer->GetName(), pRequest->szDstName) == 0)
	{
		DoSendMailRespond(pPlayer, nRequestID, mrcDstNotSelf);
		goto EXIT0;
	}

	if (pRequest->szText[0] != '\0')
	{
		nDelayTime = MAX(nDelayTime, 0/*.nMailDelaySecondOnText*/);
	}

	// 检查金钱，邮件不能携带金钱
	LOG_PROCESS_ERROR(pRequest->nMoney == 0);

	nMoney = pPlayerItem->GetCashMoney();
	if (nMoney < KMAIL_COST)
	{
		DoSendMailRespond(pPlayer, nRequestID, mrcNotEnoughMoney);
		goto EXIT0;
	}
	
	// nPlayerMoney = 0; // TODO pPlayer->GetMoney();
	// LOG_PROCESS_ERROR(nPlayerMoney - KMAIL_COST >= pRequest->nMoney);

// 	if (pRequest->nMoney > 0)
// 	{
// 		nDelayTime = MAX(nDelayTime, 0/*.nMailDelaySecondOnMoney*/);
// 	}

	// 检查装备
	for (int i = 0; i < KMAIL_MAX_ITEM_COUNT; i++)
	{
		if (pRequest->byBox[i] != emROOM_MAINBAG && pRequest->byBox[i] != emROOM_EXTBAG)
			continue;

		cItemPos.eRoom = (KE_ITEM_ROOM)pRequest->byBox[i];
		cItemPos.nX = pRequest->byX[i] % KD_ROOM_MAINBAG_WIDTH;
		cItemPos.nY = pRequest->byX[i] / KD_ROOM_MAINBAG_WIDTH;
		pItem = KPlayerItem::GetItemFromItemPos(&cItemPos, pPlayer);
		LOG_PROCESS_ERROR(pItem);

		// 不能是绑定装备
		//if (pItem->IsBind())
		//{
		//	QLogPrintf(LOG_INFO,"..pItem->IsBind()..");
		//	DoSendMailRespond(pPlayer, nRequestID, mrcItemBeBind);
		//	goto EXIT0;
		//}
	}

	pMail     = (KMail*)m_byTempData;
	pbyOffset = pMail->byData;
	uMailLen  = sizeof(KMail);

	pMail->dwMailID		= 0;
	pMail->bRead        = false;
	pMail->byType       = eMailType_Player;
	pMail->nSendTime    = KSysService::GameTime(NULL);
	pMail->nRecvTime    = pMail->nSendTime + nDelayTime;

	strncpy(pMail->szTitle, pRequest->szTitle, sizeof(pMail->szTitle));
	pMail->szTitle[sizeof(pMail->szTitle) - 1] = '\0';

	strncpy(pMail->szSenderName, pPlayer->GetName(), sizeof(pMail->szSenderName));
	pMail->szSenderName[sizeof(pMail->szSenderName) - 1] = '\0';

	// 先拷了邮件内容信息
	pMail->wTextLen = pRequest->wTextLen;
	pbyBuffer = pbyOffset;
	memcpy(pbyBuffer, pRequest->szText, pRequest->wTextLen);
	pbyOffset += pMail->wTextLen;
	uMailLen  += pMail->wTextLen;

	// 取道具
	for (int i = 0; i < KMAIL_MAX_ITEM_COUNT; i++)
	{
		pMail->ItemDesc[i].bAcquired = false;
		pMail->ItemDesc[i].byDataLen = 0;
		pMail->ItemDesc[i].nPrice    = 0;

		// 必须是主背包或者是扩展背包
		if (pRequest->byBox[i] != emROOM_MAINBAG && pRequest->byBox[i] != emROOM_EXTBAG)
			continue;

		cItemPos.eRoom = (KE_ITEM_ROOM)pRequest->byBox[i];
		cItemPos.nX = pRequest->byX[i] % KD_ROOM_MAINBAG_WIDTH;
		cItemPos.nY = pRequest->byX[i] / KD_ROOM_MAINBAG_WIDTH;
		pItem = KPlayerItem::GetItemFromItemPos(&cItemPos, pPlayer);
		LOG_PROCESS_ERROR(pItem);

		pItemData = (TDBItemData*)pbyOffset;
		uItemDataLen = sizeof(TDBItemData);
		memset(pItemData, 0, sizeof(TDBItemData));
		pPlayerItem->SaveItemData(pItem, pItemData, (INT)cItemPos.eRoom, cItemPos.nX, cItemPos.nY);

		pbyOffset += uItemDataLen;
		uMailLen  += uItemDataLen;

		pMail->ItemDesc[i].byDataLen = (BYTE)uItemDataLen;
		pMail->ItemDesc[i].nPrice    = 0;

		// 从背包中移除玩家的装备
		nItemIdx = pItem->GetGameIndex();
		bRetCode = pPlayerItem->Remove(nItemIdx, emKLOSEITEM_NONE);
		LOG_PROCESS_ERROR(bRetCode);

		//g_LogClient.LogPlayerItemChange(
		//	pPlayer, ITEM_OPERATION_TYPE_OUT, pItem, 
		//	"mail to player: ", pRequest->szDstName
		//	);

		// del item
		pItem = NULL;
	}

	// 扣钱
	pMail->nMoney = 0;
	pMail->nSilver = 0;
	pMail->nEnergy = 0;
	pPlayerItem->DecMoney(KMAIL_COST);

// 	pMail->nMoney = pRequest->nMoney;
// 	bRetCode = 0; //.AddMoney(-(pRequest->nMoney + KMAIL_COST));
// 	assert(bRetCode);

	//g_LogClient.LogPlayerMoneyChange(
	//	-pRequest->nMoney, pPlayer, "mail to player: ", pRequest->szDstName
	//	);

	//g_pWorld->m_StatDataServer.UpdateMoneyStat(pPlayer, -KMAIL_COST, "OTHER");

	MailRelayClient::DoSendMailRequest(nRequestID, pRequest->szDstName, pMail, uMailLen);

EXIT0:
	return;
}

void QMailModule::OnGetMaillistRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                        bRetCode    = false;
	C2S_GET_MAILLIST_REQUEST*   pRequest    = (C2S_GET_MAILLIST_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_GET_MAILLIST_REQUEST));

	QLogPrintf(LOG_INFO,"..QMailModule::OnGetMaillistRequest pPlayer->m_dwId=%d pRequest->dwStartID=%d",
							pPlayer->m_dwId, pRequest->dwStartID);

	bRetCode = MailRelayClient::DoGetMailListRequest(pPlayer->m_dwId, pRequest->dwStartID);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QMailModule::OnQueryMailContent(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                    bRetCode    = false;
	KNpc*                   pNpc        = NULL;
	C2S_QUERY_MAIL_CONTENT* pPak        = (C2S_QUERY_MAIL_CONTENT*)pData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_QUERY_MAIL_CONTENT));
	LOG_PROCESS_ERROR(pPak->dwMailID);

// 	if (pPak->byMailType != eMailType_GmMessage && pPak->byMailType != eMailType_PlayerMessage)
// 	{
// 		pNpc = 0; //  TODO get by id(pPak->dwNpcID);
// 		LOG_PROCESS_ERROR(pNpc);
// 		//LOG_PROCESS_ERROR(pNpc is Mailbox);
// 
// 		bRetCode = g_InRange(pPlayer, pNpc, COMMON_PLAYER_OPERATION_DISTANCE);
// 		LOG_PROCESS_ERROR(bRetCode);
// 	}
//
//	PROCESS_ERROR(pPlayer->m_eMoveState  == cmsOnStand);

	bRetCode = MailRelayClient::DoQueryMailContent(pPlayer->m_dwId, pPak->dwMailID, pPak->byMailType);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QMailModule::OnAcquireMailMoneyRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	QLogPrintf(LOG_INFO,"....QMailModule::OnAcquireMailMoneyRequest....");
	BOOL                            bRetCode    = false;
	C2S_ACQUIRE_MAIL_MONEY_REQUEST* pPak        = (C2S_ACQUIRE_MAIL_MONEY_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_ACQUIRE_MAIL_MONEY_REQUEST));
	LOG_PROCESS_ERROR(pPak->dwMailID);

	LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsPlaying);
	//PROCESS_ERROR(pPlayer->m_eMoveState  == cmsOnStand);

	bRetCode = MailRelayClient::DoAcquireMailMoneyRequest(pPlayer->m_dwId, pPak->dwMailID);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QMailModule::OnAcquireMailItemRequest(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	QLogPrintf(LOG_INFO,"....QMailModule::OnAcquireMailItemRequest....");
	BOOL                            bRetCode    = false;
	C2S_ACQUIRE_MAIL_ITEM_REQUEST*  pPak        = (C2S_ACQUIRE_MAIL_ITEM_REQUEST*)pData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_ACQUIRE_MAIL_ITEM_REQUEST));
	LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsPlaying);
	//PROCESS_ERROR(pPlayer->m_eMoveState  == cmsOnStand);

	bRetCode = MailRelayClient::DoAcquireMailItemRequest(pPlayer->m_dwId, pPak->dwMailID, pPak->byIndex);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QMailModule::OnSetMailRead(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                bRetCode    = false;
	C2S_SET_MAIL_READ*  pPak        = (C2S_SET_MAIL_READ*)pData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_SET_MAIL_READ));

	bRetCode = MailRelayClient::DoSetMailRead(pPlayer->m_dwId, pPak->dwMailID);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QMailModule::OnDeleteMail(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                bRetCode    = false;
	C2S_DELETE_MAIL*    pPak        = (C2S_DELETE_MAIL*)pData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_DELETE_MAIL));

	bRetCode = MailRelayClient::DoDeleteMail(pPlayer->m_dwId, pPak->dwMailID);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void QMailModule::OnReturnMail(KPlayer* pPlayer, char* pData, size_t uDataLen)
{
	BOOL                bRetCode    = false;
	C2S_RETURN_MAIL*    pPak        = (C2S_RETURN_MAIL*)pData;

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(uDataLen == sizeof(C2S_RETURN_MAIL));

	bRetCode = MailRelayClient::DoReturnMail(pPlayer->m_dwId, pPak->dwMailID);
	PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

BOOL QMailModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData )
{
	return m_cProtocolProcessor.SendData(nPlayerIndex, rPtcData);
}

BOOL QMailModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return m_cProtocolProcessor.SendLenData(nPlayerIndex, rPtcData, uDataSize);
}

BOOL QMailModule::SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize )
{
	return m_cProtocolProcessor.SendUnknownData(nPlayerIndex, pData, uDataSize);
}

