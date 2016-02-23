
#include "stdafx.h"
#include "Core/QGUID.h"
#include "ServerHandler.h"
#include "GameCenter.h"
#include "config/MailDef.h"
#include "onlinegamebase/kscriptmanager.h"
//#include "ItemDef.h"


void KGameServer::OnSendMailRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	int                     nResult      = mrcFailed;
	BOOL                    bRetCode     = false;
	size_t                  uMailLen     = 0;
	int                     nCount       = 0;
	KRole*                  pDstRole     = NULL;
	KRole*                  pSrcRole     = NULL;
	S2R_SEND_MAIL_REQUEST*  pPak         = (S2R_SEND_MAIL_REQUEST*)pbyData;
	KMail*                  pMail        = (KMail*)pPak->byData;
	int                     nRequestID   = 0;

	nRequestID = pPak->byRequestID;

	LOG_PROCESS_ERROR(uDataLen >= sizeof(S2R_SEND_MAIL_REQUEST) + sizeof(KMail));

	uMailLen = sizeof(KMail) + pMail->wTextLen;
	for (int i = 0; i < KMAIL_MAX_ITEM_COUNT; i++)
	{
		uMailLen += pMail->ItemDesc[i].byDataLen;
	}
	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_SEND_MAIL_REQUEST) + uMailLen);

	pMail->szSenderName[sizeof(pMail->szSenderName) - 1] = '\0';
	pMail->szTitle[sizeof(pMail->szTitle) - 1] = '\0';
	pPak->szReceiverName[sizeof(pPak->szReceiverName) - 1] = '\0';

	if (pMail->byType == eMailType_Player)
	{
		KMailCount  Count;
		KROLE_STATE eRoleState = rsInvalid;

		pSrcRole = g_pGameCenter->m_RoleManager.GetRole(pMail->szSenderName);
		LOG_PROCESS_ERROR(pSrcRole);

		eRoleState = pSrcRole->GetRoleState();
		PROCESS_ERROR(eRoleState == rsOnline);

		// 上线的时候会加载其邮件数量,所以,发送者邮件数量不大可能取不到
		bRetCode  = g_pGameCenter->m_MailManager.GetCachedMailCount(pSrcRole->m_dwPlayerID, &Count);
		LOG_PROCESS_ERROR(bRetCode);

		PROCESS_ERROR_RET_CODE(Count.nSystemMail < g_pGameCenter->m_Settings.m_nMaxSystemMailCount, mrcSelfMailBoxFull);
	}

	pDstRole = g_pGameCenter->m_RoleManager.GetRole(pPak->szReceiverName);
	PROCESS_ERROR_RET_CODE(pDstRole, mrcDstNotExist);
	
	//目标玩家不在线时无法做黑名单判断
	//PROCESS_ERROR(pDstRole->GetRoleState() == rsOnline);
	//黑名单处理
	if(g_pGameCenter->m_RelationManager.HasRelation(pDstRole->GetId(),pSrcRole->GetId(),emKPLAYERRELATION_TYPE_BLACKLIST))
	{
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunction("Relation", "NotifyAddedBlack_GC", 0, "ds", pSrcRole->GetId(), pDstRole->GetPlayerName());
		//这里没有把bRetCode置为false
		LOG_PROCESS_ERROR(FALSE);
	}

	bRetCode = g_pGameCenter->m_MailManager.PostRequestSendMail(nRequestID, pDstRole->m_dwPlayerID, pMail, uMailLen);
	LOG_PROCESS_ERROR(bRetCode);

	nResult = mrcSucceed;
EXIT0:
	if (nResult != mrcSucceed && pMail && pMail->byType == eMailType_Player)
	{
		if (pSrcRole)
		{
			DoSendMailRespond(nRequestID, pSrcRole, (BYTE)nResult, pMail, uMailLen, pPak->szReceiverName); 
		}
	}
	return;
}

void KGameServer::OnSendGlobalMailRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode     = false;
	S2R_SEND_GLOBAL_MAIL_REQUEST*   pMailData    = (S2R_SEND_GLOBAL_MAIL_REQUEST*)pbyData;
	KMail*                          pMail        = (KMail*)pMailData->byData;
	size_t                          uMailLen     = 0;

	LOG_PROCESS_ERROR(uDataLen >= sizeof(S2R_SEND_GLOBAL_MAIL_REQUEST) + sizeof(KMail));
	uMailLen = sizeof(KMail) + pMail->wTextLen;
	for (int i = 0; i < KMAIL_MAX_ITEM_COUNT; i++)
	{
		uMailLen += pMail->ItemDesc[i].byDataLen;
	}
	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_SEND_GLOBAL_MAIL_REQUEST) + uMailLen);

	bRetCode = g_pGameCenter->m_MailManager.SendGlobalMail(
		pMailData->nLifeTime, pMail, uMailLen
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KGameServer::OnGetMailListRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                        bRetCode    = false;
	DWORD                       dwPlayerID  = 0;
	KRole*                      pRole       = NULL;
	S2R_GET_MAILLIST_REQUEST*   pPak        = (S2R_GET_MAILLIST_REQUEST*)pbyData;
	KROLE_STATE                 eRoleState  = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pPak->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);
	QLogPrintf(LOG_INFO,"..KGameServer::OnGetMailListRequest pPak->dwPlayerID=%d pPak->dwStartID=%d",
				pPak->dwPlayerID,pPak->dwStartID);
	eRoleState = pRole->GetRoleState();
	LOG_PROCESS_ERROR(eRoleState == rsOnline);

	bRetCode = g_pGameCenter->m_MailManager.PostRequestGetMailList(pPak->dwPlayerID, pPak->dwStartID);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KGameServer::OnQueryMailContent(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                    bRetCode        = false;
	S2R_QUERY_MAIL_CONTENT* pPak            = (S2R_QUERY_MAIL_CONTENT*)pbyData;
	KRole*                  pRole           = NULL;
	KROLE_STATE             eRoleState      = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pPak->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	eRoleState = pRole->GetRoleState();
	PROCESS_ERROR(eRoleState == rsOnline);

	bRetCode = g_pGameCenter->m_MailManager.PostRequestQueryMailContent(
		pPak->dwPlayerID, pPak->dwMailID, (KMAIL_TYPE)pPak->byMailType
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KGameServer::OnAcquireMailMoneyRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode    = false;
	S2R_ACQUIRE_MAIL_MONEY_REQUEST* pPak        = (S2R_ACQUIRE_MAIL_MONEY_REQUEST*)pbyData;
	KRole*                          pRole       = NULL;
	KROLE_STATE                     eRoleState  = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pPak->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	eRoleState = pRole->GetRoleState();
	PROCESS_ERROR(eRoleState == rsOnline);

	bRetCode = g_pGameCenter->m_MailManager.PostRequestAcquireMailMoney(
		pPak->dwPlayerID, pPak->dwMailID 
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KGameServer::OnAcquireMailItemRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode    = false;
	S2R_ACQUIRE_MAIL_ITEM_REQUEST*  pPak        = (S2R_ACQUIRE_MAIL_ITEM_REQUEST*)pbyData;
	KRole*                          pRole       = NULL;
	KROLE_STATE                     eRoleState  = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pPak->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	eRoleState = pRole->GetRoleState();
	PROCESS_ERROR(eRoleState == rsOnline);

	bRetCode = g_pGameCenter->m_MailManager.PostRequestAcquireMailItem(
		pPak->dwPlayerID, pPak->dwMailID, pPak->byIndex
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KGameServer::OnSetMailRead(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                bRetCode    = false;
	S2R_SET_MAIL_READ*  pPak        = (S2R_SET_MAIL_READ*)pbyData;
	KRole*              pRole       = NULL;
	KROLE_STATE         eRoleState  = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pPak->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	eRoleState = pRole->GetRoleState();
	PROCESS_ERROR(eRoleState == rsOnline);

	bRetCode = g_pGameCenter->m_MailManager.PostRequestSetMailRead(
		pPak->dwPlayerID, pPak->dwMailID
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KGameServer::OnDeleteMail(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                bRetCode    = false;
	S2R_DELETE_MAIL*    pPak        = (S2R_DELETE_MAIL*)pbyData;
	KRole*              pRole       = NULL;
	KROLE_STATE         eRoleState  = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pPak->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	eRoleState = pRole->GetRoleState();
	PROCESS_ERROR(eRoleState == rsOnline);

	bRetCode = g_pGameCenter->m_MailManager.PostRequestDeleteMail(
		pPak->dwPlayerID, pPak->dwMailID
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KGameServer::OnReturnMail(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                bRetCode    = false;
	unsigned            uPakSize    = 0;
	S2R_RETURN_MAIL*    pPak        = (S2R_RETURN_MAIL*)pbyData;
	KRole*              pRole       = NULL;
	KROLE_STATE         eRoleState  = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pPak->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	eRoleState = pRole->GetRoleState();
	PROCESS_ERROR(eRoleState == rsOnline);

	bRetCode = g_pGameCenter->m_MailManager.PostRequestUserReturnMail(
		pPak->dwPlayerID, pPak->dwMailID
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

void KGameServer::OnGiveMailMoneyToPlayerRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetCode            = false;
	S2R_GIVE_MAIL_MONEY_TO_PLAYER_RESPOND*  pPak = (S2R_GIVE_MAIL_MONEY_TO_PLAYER_RESPOND*)pbyData;

	if (pPak->byResult == mrcSucceed)
	{
		bRetCode = g_pGameCenter->m_MailManager.PostRequestReleaseMailMoney(
			pPak->dwPlayerID, pPak->dwMailID
			);
		LOG_PROCESS_ERROR(bRetCode);
	}
	else
	{
		bRetCode = g_pGameCenter->m_MailManager.PostRequestUnLockMail(
			pPak->dwPlayerID, pPak->dwMailID
			);
		LOG_PROCESS_ERROR(bRetCode);

		DoMailGeneralRespond(nConnIndex, pPak->dwPlayerID, pPak->dwMailID, pPak->byResult);
	}

EXIT0:
	return;
}

void KGameServer::OnGiveMailItemToPlayerRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                    bRetCode    = false;
	S2R_GIVE_MAIL_ITEM_TO_PLAYER_RESPOND*   pPak        = (S2R_GIVE_MAIL_ITEM_TO_PLAYER_RESPOND*)pbyData;

	if (pPak->byResult == mrcSucceed)
	{
		bRetCode = g_pGameCenter->m_MailManager.PostRequestReleaseMailItem(
			pPak->dwPlayerID, pPak->dwMailID, pPak->byIndex
			);
		LOG_PROCESS_ERROR(bRetCode);
	}
	else
	{
		bRetCode = g_pGameCenter->m_MailManager.PostRequestUnLockMail(
			pPak->dwPlayerID, pPak->dwMailID
			);
		LOG_PROCESS_ERROR(bRetCode);

		DoMailGeneralRespond(nConnIndex, pPak->dwPlayerID, pPak->dwMailID, pPak->byResult);
	}

EXIT0:
	return;
}

// ---------------------------------------------------------------------------
BOOL KGameServer::DoSendMailRespond(int nRespondID, KRole* pRole, int nResult, KMail* pMail, size_t uMailLen, const char cszReceiver[])
{
	BOOL                    bResult         = false;
	BOOL                    bRetCode        = false;
	IMemBlock*             piSendBuffer    = NULL;
	R2S_SEND_MAIL_RESPOND*  pPak            = NULL;
	size_t                  uPakSize        = sizeof(R2S_SEND_MAIL_RESPOND);

	assert(pRole);

	uPakSize += uMailLen;
// 	if (nResult != mrcSucceed)
// 	{
// 		uPakSize += uMailLen;
// 		assert(pMail);
// 	}

	piSendBuffer = QCreateMemBlock((unsigned)uPakSize);
	LOG_PROCESS_ERROR(piSendBuffer);

	pPak = (R2S_SEND_MAIL_RESPOND*)piSendBuffer->GetData();

	pPak->wProtocolID      = r2s_send_mail_respond;
	pPak->byRespondID      = (BYTE)nRespondID;
	pPak->dwSrcID          = pRole->m_dwPlayerID;
	pPak->byResult         = (BYTE)nResult;

	strncpy(pPak->szReceiver, cszReceiver, sizeof(pPak->szReceiver));
	pPak->szReceiver[sizeof(pPak->szReceiver) - 1] = '\0';

	// if (nResult != mrcSucceed)
	memcpy(pPak->byData, pMail, uMailLen);

	bRetCode = Send(pRole->m_nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KGameServer::DoGetMailListRespond(DWORD dwPlayerID, KMailListInfo MailList[], int nMailCount)
{
	BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	IMemBlock*                 piSendBuffer    = NULL;
	R2S_GET_MAILLIST_RESPOND*   pRespond        = NULL;
	KRole*                      pRole           = NULL;

	assert(nMailCount >= 0);

	piSendBuffer = QCreateMemBlock(
		(unsigned)(sizeof(R2S_GET_MAILLIST_RESPOND) + sizeof(KMailListInfo) * nMailCount)
		);
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (R2S_GET_MAILLIST_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_get_maillist_respond;
	pRespond->dwPlayerID  = dwPlayerID;
	pRespond->nMailCount  = nMailCount;
	
	if (nMailCount > 0)
	{
		memcpy(pRespond->MailList, MailList, sizeof(KMailListInfo) * nMailCount);
	}

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	bRetCode = Send(pRole->m_nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KGameServer::DoSyncMailContent(
									int nConnIndex, DWORD dwPlayerID, DWORD dwMailID, BYTE byResult, 
									KMailContent* pContent, size_t uContentLen
									)
{
	BOOL                    bResult         = false;
	BOOL                    bRetCode        = false;
	IMemBlock*             piSendBuffer    = NULL;
	R2S_SYNC_MAIL_CONTENT*  pRespond        = NULL;

	piSendBuffer = QCreateMemBlock(
		(unsigned)(sizeof(R2S_SYNC_MAIL_CONTENT) + uContentLen)
		);
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (R2S_SYNC_MAIL_CONTENT*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = r2s_sync_mail_content;
	pRespond->dwPlayerID    = dwPlayerID;
	pRespond->dwMailID      = dwMailID;
	pRespond->byResult      = byResult;

	if (pContent)
	{
		memcpy(pRespond->byData, pContent, uContentLen);
	}

	bRetCode = Send(nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KGameServer::DoGiveMailMoneyToPlayer(
	int nConnIndex, DWORD dwPlayerID, DWORD dwMailID, BYTE byMailType, int nMoney,int nSilver,int nEnergy, const char cszSender[]
)
{
	BOOL                            bResult      = false;
	BOOL                            bRetCode     = false;
	IMemBlock*                      piSendBuffer = NULL;
	R2S_GIVE_MAIL_MONEY_TO_PLAYER*  pRespond     = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_GIVE_MAIL_MONEY_TO_PLAYER));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (R2S_GIVE_MAIL_MONEY_TO_PLAYER*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = r2s_give_mail_money_to_player;
	pRespond->dwPlayerID    = dwPlayerID;
	pRespond->dwMailID      = dwMailID;
	pRespond->byMailType    = byMailType;
	pRespond->nMoney        = nMoney;
	pRespond->nSilver       = nSilver;
	pRespond->nEnergy       = nEnergy;
	strncpy(pRespond->szSender, cszSender, sizeof(pRespond->szSender));
	pRespond->szSender[sizeof(pRespond->szSender) - 1] = '\0';

	bRetCode = Send(nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KGameServer::DoGiveMailItemToPlayer(
	int nConnIndex, DWORD dwPlayerID, DWORD dwMailID, BYTE byMailType, BYTE byResult,
	int nItemIndex, int nItemPrice, BYTE* pbyItemData, size_t uItemDataLen, const char cszSender[]
)
{
	BOOL                            bResult      = false;
	BOOL                            bRetCode     = false;
	IMemBlock*                     piSendBuffer = NULL;
	R2S_GIVE_MAIL_ITEM_TO_PLAYER*   pRespond     = NULL;

	assert(pbyItemData);

	piSendBuffer = QCreateMemBlock(
		(unsigned)(sizeof(R2S_GIVE_MAIL_ITEM_TO_PLAYER) + uItemDataLen)
		);
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (R2S_GIVE_MAIL_ITEM_TO_PLAYER*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID  = r2s_give_mail_item_to_player;
	pRespond->dwPlayerID   = dwPlayerID;
	pRespond->dwMailID     = dwMailID;
	pRespond->byMailType   = byMailType;
	pRespond->byResult     = byResult;
	pRespond->byItemIndex  = (BYTE)nItemIndex;
	pRespond->nItemPrice   = nItemPrice;
	pRespond->byItemLen    = (BYTE)uItemDataLen;

	strncpy(pRespond->szSender, cszSender, sizeof(pRespond->szSender));
	pRespond->szSender[sizeof(pRespond->szSender) - 1] = '\0';

	memcpy(pRespond->byData, pbyItemData, uItemDataLen);

	bRetCode = Send(nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KGameServer::DoNewMailNotity(int nConnIndex, DWORD dwPlayerID, KMailListInfo* pMailListInfo)
{
	BOOL                 bResult        = false;
	BOOL                 bRetCode       = false;
	IMemBlock*          piSendBuffer   = NULL;
	R2S_NEW_MAIL_NOTIFY* pNotify        = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_NEW_MAIL_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (R2S_NEW_MAIL_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID      = r2s_new_mail_notify;
	pNotify->dwPlayerID       = dwPlayerID;

	memcpy(&(pNotify->NewMailListInfo), pMailListInfo, sizeof(KMailListInfo));

	bRetCode = Send(nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KGameServer::DoMailGeneralRespond(int nConnIndex, DWORD dwPlayerID, DWORD dwMailID, BYTE byResult)
{
	BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	IMemBlock*                 piSendBuffer    = NULL;
	R2S_MAIL_GENERAL_RESPOND*   pRespond        = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_MAIL_GENERAL_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (R2S_MAIL_GENERAL_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID       = r2s_mail_general_respond;
	pRespond->dwPlayerID        = dwPlayerID;
	pRespond->dwMailID          = dwMailID;
	pRespond->byRespond         = byResult;

	bRetCode = Send(nConnIndex, piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}