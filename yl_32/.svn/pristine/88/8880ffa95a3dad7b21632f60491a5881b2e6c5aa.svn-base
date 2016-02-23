#include "stdafx.h"
#include "GameCenter.h"
#include "AuctionManager.h"


#define TITLE_LEN			32
#define TAX_RATE			10

#define AUCTION_SELL_SUCCEED_STR	"成功"
#define AUCTION_BUYER_STR	"买家"
#define AUCTION_PRICE_STR	"价格"
#define AUCTION_CUSTODY_CHARGES_STR	"charges"
#define AUCTION_TAX_MONEY_STR	"税费"
#define AUCTION_GET_MONEY_STR	"GetMoney"
#define AUCTION_ACTION_NAME_STR	"AuctionName"
#define AUCTION_BID_FAILED_STR	"BidFailed"
#define AUCTION_CANCEL_SUCCEED_STR	"成功取消"
#define AUCTION_BUY_SUCCEED_STR	"成功取消"
#define AUCTION_BUY_IT_NOW_STR	"一口价"
#define AUCTION_TOP_PRICE_STR	"TopPrice"
#define AUCTION_SELLER_STR	"Seller"


#define REGISTER_OPERATION_FUNC(Type, Func) \
    m_OperationFunc[Type] = &KAuctionManager::Func

KAuctionManager::KAuctionManager()
{
    m_bBusy                       = false;
    m_nNextCheckWithdrawSalesTime = 0;

    memset(m_OperationFunc, 0, sizeof(m_OperationFunc));
    REGISTER_OPERATION_FUNC(eaoLookup, ProcessLookupOperation);
    REGISTER_OPERATION_FUNC(eaoBid, ProcessBidOperation);
    REGISTER_OPERATION_FUNC(eaoSell, ProcessSellOperation);
    REGISTER_OPERATION_FUNC(eaoCancel, ProcessCancelOperation);
}

KAuctionManager::~KAuctionManager()
{
}

BOOL KAuctionManager::Init()
{
    return true;
}

void KAuctionManager::UnInit()
{
    void* pvOperation = NULL;

    while (!m_OperationQueue.empty())
    {
        pvOperation = m_OperationQueue.front();
        m_OperationQueue.pop_front();

        SAFE_FREE(pvOperation);
    }
}

void KAuctionManager::Activate()
{
    BYTE*                    pbyData    = NULL;
    size_t                   uDataLen   = 0;
    KAUCTION_OPERATION_HEAD* pHead      = NULL;
    KAUCTION_OPERATION_FUNC  pFunc      = NULL;

    while (!m_bBusy && !m_OperationQueue.empty())
    {
        pHead = (KAUCTION_OPERATION_HEAD*)m_OperationQueue.front();
        LOG_PROCESS_ERROR(pHead);

        pbyData  = (BYTE*)pHead;
        uDataLen = pHead->uSize;

        pFunc = m_OperationFunc[pHead->byType];
        LOG_PROCESS_ERROR(pFunc);

        m_OperationQueue.pop_front();

        (this->*pFunc)(pbyData, uDataLen);

        SAFE_FREE(pHead);

        m_bBusy = true;
    }

    if (g_pGameCenter->m_nTimeNow >= m_nNextCheckWithdrawSalesTime)
    {
        DoCheckDurationRequest();
        m_nNextCheckWithdrawSalesTime = g_pGameCenter->m_nTimeNow + 10 * 60;
    }

EXIT0:
    return;
}

BOOL KAuctionManager::PostRequestLookup(BYTE byRequestID, DWORD dwPlayerID, KAUCTION_LOOKUP_PARAM* pParam)
{
    BOOL                        bResult     = false;
    BOOL                        bRetCode    = false;
    KAUCTION_LOOKUP_OPERATION*  pOperation  = NULL;

    assert(pParam);

    LOG_PROCESS_ERROR(dwPlayerID != ERROR_ID);

    if (m_OperationQueue.size() > MAX_AUCTION_LOOKUP_COUNT)
    {
        g_pGameCenter->m_GameServer.DoAuctionLookupRespond(dwPlayerID, byRequestID, arcServerBusy, NULL);

        goto EXIT0;
    }

    pParam->szSaleName[sizeof(pParam->szSaleName) - 1] = '\0';
    pParam->szSellerName[sizeof(pParam->szSellerName) - 1] = '\0';

    LOG_PROCESS_ERROR(pParam->nLevelMin == 0 || pParam->nLevelMax == 0 || pParam->nLevelMin <= pParam->nLevelMax);
    LOG_PROCESS_ERROR(pParam->byAuction <= assNoOneBid);
    LOG_PROCESS_ERROR(pParam->nStartIndex >= 0);
    LOG_PROCESS_ERROR(pParam->byOrderType < aotTotal);

    pOperation = (KAUCTION_LOOKUP_OPERATION*)malloc(sizeof(KAUCTION_LOOKUP_OPERATION));
    LOG_PROCESS_ERROR(pOperation);

    pOperation->byType      = eaoLookup;
    pOperation->dwPlayerID  = dwPlayerID;
    pOperation->uSize       = sizeof(KAUCTION_LOOKUP_OPERATION);
    pOperation->byRequestID = byRequestID;
    pOperation->Param       = *pParam;

    m_OperationQueue.push_back(pOperation);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        SAFE_FREE(pOperation);
    }
    return bResult;
}

BOOL KAuctionManager::PostRequestBid(DWORD dwBidderID, DWORD dwSaleID, DWORD dwCRC, int nPrice)
{
    BOOL                    bResult     = false;
    BOOL                    bRetCode    = false;
    KAUCTION_BID_OPERATION* pOperation  = NULL;

    LOG_PROCESS_ERROR(dwBidderID != ERROR_ID);

    pOperation = (KAUCTION_BID_OPERATION*)malloc(sizeof(KAUCTION_BID_OPERATION));
    LOG_PROCESS_ERROR(pOperation);

    pOperation->byType      = eaoBid;
    pOperation->dwPlayerID  = dwBidderID;
    pOperation->uSize       = sizeof(KAUCTION_BID_OPERATION);
    pOperation->dwSaleID    = dwSaleID;
    pOperation->dwCRC       = dwCRC;
    pOperation->nPrice      = nPrice;

    m_OperationQueue.push_back(pOperation);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        SAFE_FREE(pOperation);
    }
    return bResult;
}

BOOL KAuctionManager::PostRequestSell(
    DWORD dwSellerID, char* pszSaleName, int nAucGenre, int nAucSub, int nRequireLevel,
    int nQuality, int nStartPrice, int nBuyItNowPrice, int nCustodyCharges, time_t nDurationTime,
    BYTE* pbyItemData, size_t uDataLen
)
{
    BOOL                        bResult     = false;
    BOOL                        bRetCode    = false;
    KAUCTION_SELL_OPERATION*    pOperation  = NULL;
    KRole*                      pRole       = NULL;

	INT                         nSize       = 0;

    LOG_PROCESS_ERROR(pszSaleName);
    LOG_PROCESS_ERROR(pszSaleName[0] != '\0');
    LOG_PROCESS_ERROR(dwSellerID != ERROR_ID);
    LOG_PROCESS_ERROR(pbyItemData);
    LOG_PROCESS_ERROR(uDataLen > 0);

    pRole = g_pGameCenter->m_RoleManager.GetRole(dwSellerID);
    LOG_PROCESS_ERROR(pRole);
	nSize = sizeof(KAUCTION_SELL_OPERATION) + uDataLen;
    pOperation = (KAUCTION_SELL_OPERATION*)malloc(nSize);
    LOG_PROCESS_ERROR(pOperation);

    pOperation->byType          = eaoSell;
    pOperation->dwPlayerID      = dwSellerID;
    pOperation->uSize           = sizeof(KAUCTION_SELL_OPERATION) + uDataLen;

    strncpy(pOperation->szSaleName, pszSaleName, sizeof(pOperation->szSaleName));
    pOperation->szSaleName[sizeof(pOperation->szSaleName) - 1] = '\0';

    strncpy(pOperation->szSellerName, pRole->m_szName, sizeof(pOperation->szSellerName));
    pOperation->szSellerName[sizeof(pOperation->szSellerName) - 1] = '\0';

    pOperation->nAucGenre       = nAucGenre;
    pOperation->nAucSub         = nAucSub;
    pOperation->nRequireLevel   = nRequireLevel;
    pOperation->nQuality        = nQuality;
    pOperation->nStartPrice     = nStartPrice;
    pOperation->nBuyItNowPrice  = nBuyItNowPrice;
    pOperation->nCustodyCharges = nCustodyCharges;
    pOperation->nDurationTime   = nDurationTime;
    pOperation->uDataLen        = uDataLen;

	LOG_PROCESS_ERROR(nSize >= uDataLen);
    memcpy(pOperation->byItemData, pbyItemData, uDataLen);

    m_OperationQueue.push_back(pOperation);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        SAFE_FREE(pOperation);
    }
    return bResult;
}

BOOL KAuctionManager::PostRequestCancel(DWORD dwSellerID, DWORD dwSaleID)
{
    BOOL                        bResult     = false;
    BOOL                        bRetCode    = false;
    KAUCTION_CANCEL_OPERATION*  pOperation  = NULL;

    LOG_PROCESS_ERROR(dwSellerID != ERROR_ID);

    pOperation = (KAUCTION_CANCEL_OPERATION*)malloc(sizeof(KAUCTION_CANCEL_OPERATION));
    LOG_PROCESS_ERROR(pOperation);

    pOperation->byType      = eaoCancel;
    pOperation->dwPlayerID  = dwSellerID;
    pOperation->uSize       = sizeof(KAUCTION_CANCEL_OPERATION);
    pOperation->dwSaleID    = dwSaleID;

    m_OperationQueue.push_back(pOperation);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        SAFE_FREE(pOperation);
    }
    return bResult;
}

BOOL KAuctionManager::DoCheckDurationRequest()
{
    g_pGameCenter->m_MiscDB.DoAuctionCheckDuration(g_pGameCenter->m_nTimeNow);
    return true;
}

void KAuctionManager::ProcessLookupOperation(BYTE* pbyData, size_t uDataLen)
{
    KAUCTION_LOOKUP_OPERATION* pOperation = (KAUCTION_LOOKUP_OPERATION*)pbyData;

    g_pGameCenter->m_MiscDB.DoAuctionLookup(
        pOperation->byRequestID, pOperation->dwPlayerID, &pOperation->Param
    );
}

void KAuctionManager::ProcessBidOperation(BYTE* pbyData, size_t uDataLen)
{
    KAUCTION_BID_OPERATION* pOperation = (KAUCTION_BID_OPERATION*)pbyData;

    g_pGameCenter->m_MiscDB.DoAuctionBid(
        pOperation->dwPlayerID, pOperation->dwSaleID, pOperation->dwCRC, pOperation->nPrice
    );
}

void KAuctionManager::ProcessSellOperation(BYTE* pbyData, size_t uDataLen)
{
    KAUCTION_SELL_OPERATION* pOperation = (KAUCTION_SELL_OPERATION*)pbyData;

    g_pGameCenter->m_MiscDB.DoAuctionSell(
        pOperation->dwPlayerID, pOperation->szSaleName, pOperation->szSellerName, pOperation->nAucGenre,
        pOperation->nAucSub, pOperation->nRequireLevel, pOperation->nQuality, pOperation->nStartPrice,
        pOperation->nBuyItNowPrice, pOperation->nCustodyCharges, pOperation->nDurationTime,
        pOperation->byItemData, pOperation->uDataLen
    );
}

void KAuctionManager::ProcessCancelOperation(BYTE* pbyData, size_t uDataLen)
{
    KAUCTION_CANCEL_OPERATION* pOperation = (KAUCTION_CANCEL_OPERATION*)pbyData;

    g_pGameCenter->m_MiscDB.DoAuctionCancel(pOperation->dwPlayerID, pOperation->dwSaleID);

}

void KAuctionManager::OnLookupRespond(BYTE byRespondID, DWORD dwPlayerID, KAUCTION_PAGE_HEAD* pPage)
{
    BOOL                bRetCode    = false;
    KRole*              pSellerRole = NULL;
    KRole*              pBidderRole = NULL;
    BYTE*               pbyOffset   = NULL;
    size_t              uLeftSize   = 0;

    m_bBusy = false;

    assert(pPage);

    pbyOffset = pPage->bySaleList;
    uLeftSize = pPage->uSize;

    while (uLeftSize > 0)
    {
        KAUCTION_PAGE_NODE* pNode       = (KAUCTION_PAGE_NODE*)pbyOffset;
        size_t              uSaleSize   = sizeof(KAUCTION_PAGE_NODE) + pNode->uItemSize;

        LOG_PROCESS_ERROR(uLeftSize >= uSaleSize);
        pbyOffset += uSaleSize;
        uLeftSize -= uSaleSize;

        pSellerRole = g_pGameCenter->m_RoleManager.GetRole(pNode->dwSellerID);
        if (pSellerRole)
        {
            strncpy(pNode->szSellerName, pSellerRole->m_szName, sizeof(pNode->szSellerName));
            pNode->szSellerName[sizeof(pNode->szSellerName) - 1] = '\0';
        }

        pBidderRole = g_pGameCenter->m_RoleManager.GetRole(pNode->dwBidderID);
        if (pBidderRole)
        {
            strncpy(pNode->szBidderName, pBidderRole->m_szName, sizeof(pNode->szBidderName));
            pNode->szBidderName[sizeof(pNode->szBidderName) - 1] = '\0';
        }
    }

    bRetCode = g_pGameCenter->m_GameServer.DoAuctionLookupRespond(dwPlayerID, byRespondID, arcSucceed, pPage);
    LOG_PROCESS_ERROR(bRetCode);

EXIT0:
    return;
}

void KAuctionManager::OnBidRespond(
    BYTE byCode, DWORD dwSellerID, int nSellPrice, DWORD dwBidderID, int nBidPrice,
    DWORD dwOldBidderID, int nOldBidPrice, char* pszSaleName, int nCustodyCharges,
    size_t uItemDataLen, BYTE* pbyItemData
)
{
    BOOL    bRetCode    = false;

    m_bBusy = false;

    if (byCode != arcSucceed)
    {
        goto Exit1;
    }

    if (nBidPrice >= nSellPrice)
    {
        // 1 把商品和找的钱 mailto 买家 通知买家获得道具
        SendItemToBidder(dwBidderID, dwSellerID, pszSaleName, nBidPrice, nSellPrice, uItemDataLen, pbyItemData);

        g_pGameCenter->m_GameServer.DoAuctionMessageNotify(dwBidderID, amcAcquireItem, pszSaleName, nSellPrice);

        // 2 把卖的钱 mailto 卖家 通知卖家道具已卖出
        SendMoneyToSeller(dwSellerID, dwBidderID, pszSaleName, nSellPrice, nCustodyCharges);

        g_pGameCenter->m_GameServer.DoAuctionMessageNotify(dwSellerID, amcItemSold, pszSaleName, nSellPrice);

        g_pGameCenter->m_LogServerAgency.LogBuyAuctionItem(dwBidderID, dwSellerID, uItemDataLen, pbyItemData, nSellPrice);
    }
    else
    {
        // 通知卖家道具被出价
        g_pGameCenter->m_GameServer.DoAuctionMessageNotify(dwSellerID, amcSomeoneBid, pszSaleName, nBidPrice);
    }

    // 把上一个bidder的钱退还 通知上一个出价人出价被超过
    if (dwOldBidderID != ERROR_ID)
    {
        SendMoneyToBidder(dwOldBidderID, pszSaleName, nOldBidPrice);

        g_pGameCenter->m_GameServer.DoAuctionMessageNotify(dwOldBidderID, amcBidLost, pszSaleName, nOldBidPrice);
    }

Exit1:
    bRetCode = g_pGameCenter->m_GameServer.DoAuctionBidRespond(dwBidderID, byCode, nBidPrice);
    LOG_PROCESS_ERROR(bRetCode);

EXIT0:
    return;
}

void KAuctionManager::OnSellRespond(BYTE byCode)
{
    m_bBusy = false;

    LOG_PROCESS_ERROR(byCode == arcSucceed);

EXIT0:
    return;
}

void KAuctionManager::OnCancelRespond(
    BYTE byCode, DWORD dwSellerID, char* pszSaleName, size_t uItemDataLen, BYTE* pbyItemData
)
{
    BOOL    bRetCode    = false;

    m_bBusy = false;

    if (byCode != arcSucceed)
    {
        goto Exit1;
    }

    // 发邮件给卖家退货
    SendItemToSeller(dwSellerID, pszSaleName, uItemDataLen, pbyItemData);

Exit1:
    bRetCode = g_pGameCenter->m_GameServer.DoAuctionCancelRespond(dwSellerID, byCode);
    LOG_PROCESS_ERROR(bRetCode);

EXIT0:
    return;
}

void KAuctionManager::OnCheckDurationRespond(
    char* pszSaleName, DWORD dwSellerID, DWORD dwBidderID, int nPrice, int nBuyItNowPrice,
    int nCustodyCharges, size_t uItemDataLen, BYTE* pbyItemData
)
{
    KRole* pSellerRole = NULL;

    if (dwBidderID == ERROR_ID)
    {
        // 退回给卖家，过期无人出价
        SendItemToSeller(dwSellerID, pszSaleName, uItemDataLen, pbyItemData);

        g_pGameCenter->m_GameServer.DoAuctionMessageNotify(dwSellerID, amcTimeOver, pszSaleName, nPrice);

        pSellerRole = g_pGameCenter->m_RoleManager.GetRole(dwSellerID);
        if (pSellerRole)
        {
            g_pGameCenter->m_StatDataManager.UpdateMoneyStat(pSellerRole, -nCustodyCharges, "OTHER");
        }
    }
    else
    {
        // 寄给买家商品
        SendItemToBidder(dwBidderID, dwSellerID, pszSaleName, nPrice, nBuyItNowPrice, uItemDataLen, pbyItemData);

        g_pGameCenter->m_GameServer.DoAuctionMessageNotify(dwBidderID, amcAcquireItem, pszSaleName, nPrice);

        // 寄钱给卖家
        SendMoneyToSeller(dwSellerID, dwBidderID, pszSaleName, nPrice, nCustodyCharges);

        g_pGameCenter->m_GameServer.DoAuctionMessageNotify(dwSellerID, amcItemSold, pszSaleName, nPrice);
    }
}

inline int _FormatMoney(char* pszDst, size_t uLeftSize, const char* pszName, int nMoney)
{
    int nRetCode = 0;

    nRetCode = snprintf(
        pszDst, uLeftSize,
        "%s%4d%s%2d%s%2d%s\n",
        pszName,
        nMoney / 10000, g_pGameCenter->m_Settings.m_szGold,
        (nMoney / 100) % 100, g_pGameCenter->m_Settings.m_szSilver,
        nMoney % 100, g_pGameCenter->m_Settings.m_szCopper
    );

    return nRetCode;
}

BOOL KAuctionManager::SendMoneyToSeller(
    DWORD dwSellerID, DWORD dwBuyerID, const char* pszSaleName, int nPrice, int nCustodyCharges
)
{
    BOOL                            bResult     = false;
    int                             nRetCode    = 0;
    KRole*                          pBuyerRole  = NULL;
    KRole*                          pSellerRole = NULL;
    int                             nTaxMoney   = 0;
    int                             nGetMoney   = 0;
    char                            szText[MAX_MAIL_TEXT_LEN];
    char*                           pszText     = NULL;
    size_t                          uLeftSize   = 0;
    char                            szTitle[TITLE_LEN];
    size_t                          uTextLen    = 0;
    char                            szBuyerName[_NAME_LEN];

    assert(pszSaleName);
    LOG_PROCESS_ERROR(dwSellerID != ERROR_ID);

    pBuyerRole = g_pGameCenter->m_RoleManager.GetRole(dwBuyerID);
    szBuyerName[0] = '\0';
    if (pBuyerRole)
    {
        strncpy(szBuyerName,  pBuyerRole->m_szName, sizeof(szBuyerName));
        szBuyerName[sizeof(szBuyerName) - 1] = '\0';
    }

    nTaxMoney = (int)((double)nPrice * TAX_RATE / 1000);
    nGetMoney = nPrice + nCustodyCharges - nTaxMoney;
    snprintf(szTitle, sizeof(szTitle), "%s%s", AUCTION_SELL_SUCCEED_STR, pszSaleName);
    szTitle[sizeof(szTitle) - 1] = '\0';

    pszText     = szText;
    uLeftSize   = sizeof(szText);

    nRetCode = snprintf(
        pszText, uLeftSize,
        "%s%s\n"
        "%s%s\n",
        AUCTION_SELL_SUCCEED_STR, pszSaleName,
        AUCTION_BUYER_STR, szBuyerName
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszText     += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = _FormatMoney(pszText, uLeftSize, AUCTION_PRICE_STR, nPrice);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszText     += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = _FormatMoney(pszText, uLeftSize, AUCTION_CUSTODY_CHARGES_STR, nCustodyCharges);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszText     += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = _FormatMoney(pszText, uLeftSize, AUCTION_TAX_MONEY_STR, nTaxMoney);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszText     += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = _FormatMoney(pszText, uLeftSize, AUCTION_GET_MONEY_STR, nGetMoney);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszText     += nRetCode;
    uLeftSize   -= nRetCode;

    szText[sizeof(szText) - 1] = '\0';
    uTextLen = strlen(szText) + 1;

    g_pGameCenter->m_MailManager.PostRequestSendMail(
        0, dwSellerID, 0, eMailType_Auction, 
        AUCTION_ACTION_NAME_STR, szTitle, 
        nGetMoney, 0,0,
        (BYTE*)szText, uTextLen, 
        NULL, 0, 0
    );

    pSellerRole = g_pGameCenter->m_RoleManager.GetRole(dwSellerID);
    LOG_PROCESS_ERROR(pSellerRole);

    g_pGameCenter->m_StatDataManager.UpdateMoneyStat(pSellerRole, -nTaxMoney, "OTHER");

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KAuctionManager::SendItemToSeller(
    DWORD dwSellerID, const char* pszSaleName, size_t uItemDataLen, BYTE* pbyItemData
)
{
    BOOL                            bResult     = false;
    BOOL                            bRetCode    = false;
    char                            szTitle[TITLE_LEN];
    char                            szText[]    = "";
    size_t                          uTextLen    = strlen(szText) + 1;

    assert(pszSaleName);
    assert(pbyItemData);
    LOG_PROCESS_ERROR(dwSellerID != ERROR_ID);

    snprintf(szTitle, sizeof(szTitle), "%s%s", AUCTION_SELL_SUCCEED_STR, pszSaleName);
    szTitle[sizeof(szTitle) - 1] = '\0';

    g_pGameCenter->m_MailManager.PostRequestSendMail(
        0, dwSellerID, 0, eMailType_Auction, 
        AUCTION_ACTION_NAME_STR, szTitle, 
        0, 0,0,
        (BYTE*)szText, uTextLen, 
        pbyItemData, uItemDataLen, 0
    );

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KAuctionManager::SendMoneyToBidder(DWORD dwBidderID, const char* pszSaleName, int nMoney)
{
    BOOL                            bResult     = false;
    BOOL                            bRetCode    = false;
    char                            szTitle[TITLE_LEN];
    char                            szText[]    = "";
    size_t                          uTextLen    = strlen(szText) + 1;

    assert(pszSaleName);
    LOG_PROCESS_ERROR(dwBidderID != ERROR_ID);

    snprintf(szTitle, sizeof(szTitle), "%s%s", AUCTION_BID_FAILED_STR, pszSaleName);
    szTitle[sizeof(szTitle) - 1] = '\0';

    g_pGameCenter->m_MailManager.PostRequestSendMail(
        0, dwBidderID, 0, eMailType_Auction, 
        AUCTION_ACTION_NAME_STR, szTitle, 
        nMoney, 0,0,
        (BYTE*)szText, uTextLen, 
        NULL, 0, 0
    );

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KAuctionManager::SendItemToBidder(
    DWORD dwBidderID, DWORD dwSellerID, const char* pszSaleName, int nBidPrice,
    int nBuyItNowPrice, size_t uItemDataLen, BYTE* pbyItemData
)
{
    BOOL                            bResult         = false;
    int                             nRetCode        = 0;
    int                             nSellPrice      = 0;
    KRole*                          pSellerRole     = NULL;
    bool                            bBIN            = false;
    char                            szTitle[TITLE_LEN];
    char*                           pszText         = NULL;
    size_t                          uLeftSize       = 0;
    size_t                          uTextLen        = 0;
    char                            szText[MAX_MAIL_TEXT_LEN];
    char                            szSellerName[_NAME_LEN];
    TDBItemData*              pCommonItemData = (TDBItemData*)pbyItemData;

    assert(pszSaleName);
    assert(pbyItemData);
    LOG_PROCESS_ERROR(dwBidderID != ERROR_ID);

    nSellPrice = MIN(nBidPrice, nBuyItNowPrice);
    bBIN = nBidPrice >= nBuyItNowPrice ? true : false;

    pSellerRole = g_pGameCenter->m_RoleManager.GetRole(dwSellerID);
    szSellerName[0] = '\0';
    if (pSellerRole)
    {
        strncpy(szSellerName, pSellerRole->m_szName, sizeof(szSellerName));
        szSellerName[sizeof(szSellerName) - 1] = '\0';
    }

    snprintf(szTitle, sizeof(szTitle), "%s%s", AUCTION_BUY_SUCCEED_STR, pszSaleName);
    szTitle[sizeof(szTitle) - 1] = '\0';

    pszText     = szText;
    uLeftSize   = sizeof(szText);

    nRetCode = snprintf(
        pszText, uLeftSize,
        "%s%s%s\n"
        "%s%s\n",
        AUCTION_BUY_SUCCEED_STR, pszSaleName, (nBidPrice >= nBuyItNowPrice ? AUCTION_BUY_IT_NOW_STR : AUCTION_TOP_PRICE_STR),
        AUCTION_SELLER_STR, szSellerName
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszText     += nRetCode;
    uLeftSize   -= nRetCode;

    nRetCode = _FormatMoney(pszText, uLeftSize, AUCTION_PRICE_STR, nSellPrice);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uLeftSize);
    pszText     += nRetCode;
    uLeftSize   -= nRetCode;

    szText[sizeof(szText) - 1] = '\0';
    uTextLen = strlen(szText) + 1;

    g_pGameCenter->m_MailManager.PostRequestSendMail(
        0, dwBidderID, 0, eMailType_Auction, 
        AUCTION_ACTION_NAME_STR, szTitle, 
        nBidPrice - nSellPrice, 0,0,
        (BYTE*)szText, uTextLen,
        pbyItemData, uItemDataLen, 0
    );

    //g_pGameCenter->m_StatDataManager.UpdataAuctionItemStat(
    //    pCommonItemData->byTabType, pCommonItemData->wTabIndex, nSellPrice
    //);

    bResult = true;
EXIT0:
    return bResult;
}

