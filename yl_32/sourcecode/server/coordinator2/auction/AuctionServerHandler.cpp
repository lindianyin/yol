#include "stdafx.h"
#include "Core/QGUID.h"
#include "ServerHandler.h"
#include "GameCenter.h"


BOOL KGameServer::DoAuctionLookupRespond(DWORD dwPlayerID, BYTE byRespondID, BYTE byCode, KAUCTION_PAGE_HEAD* pPage)
{
	BOOL                        bResult     = false;
	BOOL                        bRetcode    = false;
	KRole*                      pRole       = NULL;
	IMemBlock*                 piBuffer    = NULL;
	R2S_AUCTION_LOOKUP_RESPOND* pRespond    = NULL;
	size_t                      uSize       = 0;

	if (pPage)
	{
		uSize = pPage->uSize;
	}

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	piBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_AUCTION_LOOKUP_RESPOND) + uSize));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_AUCTION_LOOKUP_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID       = r2s_auction_lookup_respond;
	pRespond->byRespondID       = byRespondID;
	pRespond->dwPlayerID        = dwPlayerID;
	pRespond->byCode            = byCode; 

	pRespond->Page.nTotalCount  = 0;
	pRespond->Page.uSize        = 0;

	if (pPage)
	{
		memcpy(&pRespond->Page, pPage, sizeof(pRespond->Page) + pPage->uSize);
	}

	bRetcode = Send(pRole->m_nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetcode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KGameServer::DoAuctionBidRespond(DWORD dwPlayerID, BYTE byCode, int nBidPrice)
{
	BOOL                        bResult     = false;
	BOOL                        bRetcode    = false;
	KRole*                      pRole       = NULL;
	IMemBlock*                 piBuffer    = NULL;
	R2S_AUCTION_BID_RESPOND*    pRespond    = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_AUCTION_BID_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_AUCTION_BID_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID       = r2s_auction_bid_respond;
	pRespond->dwPlayerID        = dwPlayerID;
	pRespond->byCode            = byCode;
	pRespond->nBidPrice         = nBidPrice;

	bRetcode = Send(pRole->m_nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetcode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KGameServer::DoAuctionSellRespond(
									   DWORD dwPlayerID, BYTE byCode, int nStartPrice, int nBuyItNowPrice, 
									   int nCustodyCharges, size_t uItemDataLen, BYTE* pbyItemData
									   )
{
	BOOL                        bResult     = false;
	BOOL                        bRetcode    = false;
	KRole*                      pRole       = NULL;
	IMemBlock*                 piBuffer    = NULL;
	R2S_AUCTION_SELL_RESPOND*   pRespond    = NULL;

	assert(uItemDataLen > 0);
	assert(pbyItemData);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	piBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_AUCTION_SELL_RESPOND) + uItemDataLen));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_AUCTION_SELL_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID       = r2s_auction_sell_respond;
	pRespond->dwPlayerID        = dwPlayerID;
	pRespond->byCode            = byCode;
	pRespond->nStartPrice       = nStartPrice;
	pRespond->nBuyItNowPrice    = nBuyItNowPrice;
	pRespond->nCustodyCharges   = nCustodyCharges;
	pRespond->uItemDataLen      = uItemDataLen;

	memcpy(pRespond->byItemData, pbyItemData, uItemDataLen);

	bRetcode = Send(pRole->m_nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetcode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KGameServer::DoAuctionCancelRespond(DWORD dwPlayerID, BYTE byCode)
{
	BOOL                        bResult     = false;
	BOOL                        bRetcode    = false;
	KRole*                      pRole       = NULL;
	IMemBlock*                 piBuffer    = NULL;
	R2S_AUCTION_CANCEL_RESPOND* pRespond    = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_AUCTION_CANCEL_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_AUCTION_CANCEL_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID   = r2s_auction_cancel_respond;
	pRespond->dwPlayerID    = dwPlayerID;
	pRespond->byCode        = byCode;

	bRetcode = Send(pRole->m_nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetcode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KGameServer::DoAuctionMessageNotify(DWORD dwPlayerID, BYTE byCode, const char* pszSaleName, int nPrice)
{
	BOOL                        bResult     = false;
	BOOL                        bRetcode    = false;
	KRole*                      pRole       = NULL;
	IMemBlock*                 piBuffer    = NULL;
	R2S_AUCTION_MESSAGE_NOTIFY* pNotify     = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	PROCESS_ERROR(pRole->IsOnline()); // 只通知在线玩家

	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_AUCTION_MESSAGE_NOTIFY));
	LOG_PROCESS_ERROR(piBuffer);

	pNotify = (R2S_AUCTION_MESSAGE_NOTIFY*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID    = r2s_auction_message_notify;
	pNotify->dwPlayerID     = dwPlayerID;
	pNotify->byCode         = byCode;

	pNotify->szSaleName[0] = '\0';
	if (pszSaleName)
	{
		strncpy(pNotify->szSaleName, pszSaleName, sizeof(pNotify->szSaleName));
		pNotify->szSaleName[sizeof(pNotify->szSaleName) - 1] = '\0';
	}

	pNotify->nPrice         = nPrice;

	bRetcode = Send(pRole->m_nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetcode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}
