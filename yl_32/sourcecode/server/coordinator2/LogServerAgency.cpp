#include "stdafx.h"
#include "LogServerAgency.h"
#include "GameCenter.h"
//#include "KItemDef.h"

KLogServerAgency::KLogServerAgency()
{
    m_piSocket      = NULL;
    m_nPingCycle    = 30;
    m_nNextPingTime = 0;
    m_szIdentity[0] = '\0';
}

BOOL KLogServerAgency::Init()
{
    const QGameCenterSettings& rSettings = g_pGameCenter->m_Settings;

    // 无论连上与否,均视为成功
    LoginServer(rSettings.m_szLogServerIP, rSettings.m_nLogServerPort, rSettings.m_szLogServerIdentity);

    return true;
}

void KLogServerAgency::UnInit()
{
    SAFE_RELEASE(m_piSocket);
}

void KLogServerAgency::Activate()
{
    int         nRetCode        = false;
    BOOL        bConnectionLost = false;
    IMemBlock* piBuffer        = NULL;

    while (m_piSocket)
    {
        const struct timeval        TimeVal     = {0, 0};
        tagProtocolHeader2*         pHeader     = NULL;

        nRetCode = m_piSocket->CheckCanRecv(&TimeVal);
        if (nRetCode == -1)
        {
            bConnectionLost = true;
            break;
        }

        if (nRetCode == 0)
        {
            break;
        }

        SAFE_RELEASE(piBuffer);

        nRetCode = m_piSocket->Recv(&piBuffer);
        if (nRetCode != 1)
        {
            bConnectionLost = true;
            break;
        }
    }

    if (bConnectionLost)
    {
        QLogPrintf(LOG_ERR, "Log server connection lost !\n");
        SAFE_RELEASE(m_piSocket);
        goto EXIT0;
    }

    Ping();

EXIT0:
    SAFE_RELEASE(piBuffer);
}

void KLogServerAgency::LogConsumeAction(int eActionType,DWORD roleId, int nCoin, const char* pszComment)
{
    int     nRetCode    = false;
    size_t  uStrLen     = 0;
	char    szCoin[32];
	KRole * pRole       = NULL;
	//QLogPrintf(LOG_INFO,"%s",pszComment);

    //assert(nCoin >= 0);
    //assert(pszComment);
    LOG_PROCESS_ERROR(pszComment);
	pRole = g_pGameCenter->m_RoleManager.GetRole(roleId);
	//assert(pRole);
	 LOG_PROCESS_ERROR(pRole);
	
    nRetCode = snprintf(szCoin, sizeof(szCoin), "%d", nCoin);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szCoin));

    uStrLen = strlen(pszComment) + 1;

    nRetCode = LogPlayerAction(
        (PLAYER_ACTION_TYPE)eActionType, pRole->m_byLevel, szCoin, pRole->m_pszAccount,
        pRole->m_szName, uStrLen, (BYTE*)pszComment
    );
    LOG_PROCESS_ERROR(nRetCode);
EXIT0:
    return;
}




void KLogServerAgency::LogAction(PLAYER_ACTION_TYPE eActionType,KRole* pRole, int nCoin, const char* pszComment)
{
    int     nRetCode    = false;
    size_t  uStrLen     = 0;
	char    szCoin[32];
	
	QLogPrintf(LOG_INFO,"%s",pszComment);
	
	//assert(pRole);
    //assert(nCoin >= 0);
    //assert(pszComment);

	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(nCoin >= 0);
	LOG_PROCESS_ERROR(pszComment);

    nRetCode = snprintf(szCoin, sizeof(szCoin), "%d", nCoin);
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szCoin));

    uStrLen = strlen(pszComment) + 1;

    nRetCode = LogPlayerAction(
        eActionType, pRole->m_byLevel, szCoin, pRole->m_pszAccount,
        pRole->m_szName, uStrLen, (BYTE*)pszComment
    );
    LOG_PROCESS_ERROR(nRetCode);
EXIT0:
    return;
}


void KLogServerAgency::LogFreezeCoin(KRole* pRole, int nCoin, const char* pszComment)
{
    int     nRetCode    = false;
    size_t  uStrLen     = 0;
    char    szCoin[32];

    //assert(pRole);
    //assert(nCoin > 0);
    //assert(pszComment);

    //nRetCode = snprintf(szCoin, sizeof(szCoin), "%d", nCoin);
    //LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szCoin));

    //uStrLen = strlen(pszComment) + 1;

    //nRetCode = LogPlayerAction(
    //    PLAYER_ACTION_FREEZE_COIN, pRole->m_byLevel, szCoin, pRole->m_pszAccount,
    //    pRole->m_szName, uStrLen, (BYTE*)pszComment
    //);
    //LOG_PROCESS_ERROR(nRetCode);

EXIT0:
    return;
}

void KLogServerAgency::LogUnfreezeCoin(KRole* pRole, int nCoin, const char* pszComment)
{
    int     nRetCode    = false;
    size_t  uStrLen     = 0;
    char    szCoin[32];

    //assert(pRole);
    //assert(nCoin > 0);
    //assert(pszComment);

    //nRetCode = snprintf(szCoin, sizeof(szCoin), "%d", nCoin);
    //LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szCoin));

    //uStrLen = strlen(pszComment) + 1;

    //nRetCode = LogPlayerAction(
    //    PLAYER_ACTION_UNFREEZE_COIN, pRole->m_byLevel, szCoin, pRole->m_pszAccount,
    //    pRole->m_szName, uStrLen, (BYTE*)pszComment
    //);
    //LOG_PROCESS_ERROR(nRetCode);

EXIT0:
    return;
}

void KLogServerAgency::LogGameCardSell(KRole* pRole, int nCoin, const char* pszComment)
{
    int     nRetCode    = false;
    size_t  uStrLen     = 0;
    char    szCoin[32];

    //assert(pRole);
    //assert(nCoin > 0);
    //assert(pszComment);

    //nRetCode = snprintf(szCoin, sizeof(szCoin), "%d", nCoin);
    //LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szCoin));

    //uStrLen = strlen(pszComment) + 1;

    //nRetCode = LogPlayerAction(
    //    PLAYER_ACTION_GAME_CARD_SELL, pRole->m_byLevel, szCoin, pRole->m_pszAccount,
    //    pRole->m_szName, uStrLen, (BYTE*)pszComment
    //);
    //LOG_PROCESS_ERROR(nRetCode);

EXIT0:
    return;
}

void KLogServerAgency::LogGameCardBuy(KRole* pRole, int nGameTime, BYTE byType, const char* pszComment)
{
    int     nRetCode    = false;
    size_t  uStrLen     = 0;
    char    szTarget[32];

    //assert(pRole);
    //assert(nGameTime > 0);
    //assert(pszComment);

    //nRetCode = snprintf(szTarget, sizeof(szTarget), "%d %u", nGameTime, (unsigned)byType);
    //LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szTarget));

    //uStrLen = strlen(pszComment) + 1;

    //nRetCode = LogPlayerAction(
    //    PLAYER_ACTION_GAME_CARD_BUY, pRole->m_byLevel, szTarget, pRole->m_pszAccount,
    //    pRole->m_szName, uStrLen, (BYTE*)pszComment
    //);
    //LOG_PROCESS_ERROR(nRetCode);

EXIT0:
    return;
}

void KLogServerAgency::LogRoleAntiFarmer(KRole* pRole, time_t nAntiSeconds)
{
    BOOL    bRetCode        = false;
    char    szComment[64];
    /*
    snprintf(szComment, sizeof(szComment), "anti time : %d seconds", (int)nAntiSeconds);
    szComment[sizeof(szComment) - 1] = '\0';
    
    bRetCode = LogPlayerAction(PLAYER_ACTION_ANTI_FARMER, pRole->m_byLevel, szComment, pRole->m_pszAccount, pRole->m_szName, 0, NULL);
    LOG_PROCESS_ERROR(bRetCode);*/

EXIT0:
    return;
}

void KLogServerAgency::LogBuyAuctionItem(
    DWORD dwBuyerID, DWORD dwSellerID, size_t uItemDataLen, BYTE* pbyItemData, int nPrice
)
{
    BOOL                bRetCode        = false;
    KRole*              pBuyer          = NULL;
    KRole*              pSeller         = NULL;
    TDBItemData*  pItemData       = NULL;
    size_t              uStrLen         = 0;
    char                szComment[256];
/*
    assert(pbyItemData);
    
    pBuyer = g_pGameCenter->m_RoleManager.GetRole(dwBuyerID);
    LOG_PROCESS_ERROR(pBuyer);
    
    LOG_PROCESS_ERROR(uItemDataLen >= sizeof(TDBItemData));
    pItemData = (TDBItemData*)pbyItemData;
    
    pSeller = g_pGameCenter->m_RoleManager.GetRole(dwSellerID);
    if (pSeller) // seller maybe deleted
    {
        snprintf(
            szComment, sizeof(szComment), "Seller acc : %s, name : %s, item tabtype : %d, index: %d, durability : %d", 
            pSeller->m_pszAccount, pSeller->m_szName, pItemData->byTabType, pItemData->wTabIndex, pItemData->wDurability
        );
        szComment[sizeof(szComment) - 1] = '\0';
    }
    else
    {
        snprintf(
            szComment, sizeof(szComment), "Seller role id : %lu, item tabtype : %d, index: %d, durability : %d", 
            dwSellerID, pItemData->byTabType, pItemData->wTabIndex, pItemData->wDurability
        );
        szComment[sizeof(szComment) - 1] = '\0';
    }

    uStrLen = strlen(szComment) + 1;

    bRetCode = LogPlayerAction(
        PLAYER_ACTION_BUY_AUCTION_ITEM, nPrice, "BUY AUCTION ITEM", pBuyer->m_pszAccount,
        pBuyer->m_szName, uStrLen, (BYTE*)&szComment[0]
    );
    LOG_PROCESS_ERROR(bRetCode);
*/
EXIT0:
    return;
}

void KLogServerAgency::LogApexKickRole(KRole* pRole, int nActionID)
{
    BOOL    nRetCode        = 0;
    size_t  uCommentSize    = 0;
    char    szComment[256];

    //nRetCode = snprintf(szComment, sizeof(szComment), "%d", nActionID);
    //LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < sizeof(szComment));

    //nRetCode = LogPlayerAction(
    //    PLAYER_ACTION_CHEAT_KICKED, 100, "KICK_ROLE", pRole->m_pszAccount, pRole->m_szName,
    //    uCommentSize, (BYTE*)szComment
    //);
    //LOG_PROCESS_ERROR(nRetCode);

EXIT0:
    return;
}

void KLogServerAgency::LogFreezeCoinTimeout(KRole* pRole, int nCoin, int nPrice)
{
    int     nRetCode = 0;
    char    szTarget[32];

    //assert(pRole);

    //nRetCode = snprintf(szTarget, sizeof(szTarget), "%d", nCoin);
    //LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < sizeof(szTarget));

    //nRetCode = LogPlayerAction(
    //    PLAYER_ACTION_FREEZE_COIN_TIMEOUT, nPrice, szTarget,
    //    pRole->m_pszAccount, pRole->m_szName, 0, NULL
    //);
    //LOG_PROCESS_ERROR(nRetCode);

EXIT0:
    return;
}

void KLogServerAgency::LogAccountExchangeTimeout(KRole* pSellerRole, KRole* pBuyyerRole, int nCoin, int nGameTime, int nPrice)
{
    int     nRetCode    = 0;
    size_t  uStrLen     = 0;
    char    szTarget[32];
    char    szComment[64];

    //assert(pSellerRole);
    //assert(pBuyyerRole);

    //nRetCode = snprintf(szTarget, sizeof(szTarget), "%d %d", nCoin, nGameTime);
    //LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < sizeof(szTarget));

    //nRetCode = snprintf(szComment, sizeof(szComment), "Seller:%s(%s)", pSellerRole->m_szName, pSellerRole->m_pszAccount);
    //LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < sizeof(szComment));

    //uStrLen = strlen(szComment) + 1;

    //nRetCode = LogPlayerAction(
    //    PLAYER_ACTION_ACCOUNT_EXCHANGE_TIMEOUT, nPrice, szTarget,
    //    pBuyyerRole->m_pszAccount, pBuyyerRole->m_szName, uStrLen, (BYTE*)szComment
    //);
    //LOG_PROCESS_ERROR(nRetCode);

EXIT0:
    return;
}

void KLogServerAgency::LogMentorshipCreate(DWORD dwMentorID, DWORD dwApprenticeID, const char* pszComment)
{
    KRole*  pARole      = g_pGameCenter->m_RoleManager.GetRole(dwApprenticeID);
    size_t  uCommentLen = 0;
    char    szMentorID[32];

    //LOG_PROCESS_ERROR(pARole);

    //snprintf(szMentorID, sizeof(szMentorID), "%lu", dwMentorID);
    //szMentorID[sizeof(szMentorID) - 1] = '\0';

    //uCommentLen = strlen(pszComment) + 1;

    //LogPlayerAction(
    //    PLAYER_ACTION_MENTORSHIP_CREATE, pARole->m_byLevel, szMentorID,
    //    pARole->m_pszAccount, pARole->m_szName, uCommentLen, (BYTE*)pszComment
    //);

EXIT0:
    return;
}

void KLogServerAgency::LogMentorshipDelete(DWORD dwMentorID, DWORD dwApprenticeID, int nMentorValue, const char* pszComment)
{
    KRole*  pARole      = g_pGameCenter->m_RoleManager.GetRole(dwApprenticeID);
    size_t  uCommentLen = 0;
    char    szMentorID[32];

    //PROCESS_ERROR(pARole);

    //snprintf(szMentorID, sizeof(szMentorID), "%lu", dwMentorID);
    //szMentorID[sizeof(szMentorID) - 1] = '\0';

    //uCommentLen = strlen(pszComment) + 1;

    //LogPlayerAction(
    //    PLAYER_ACTION_MENTORSHIP_DELETE, nMentorValue, szMentorID,
    //    pARole->m_pszAccount, pARole->m_szName, uCommentLen, (BYTE*)pszComment
    //);

EXIT0:
    return;
}

void KLogServerAgency::LogAcquireMentorValue(DWORD dwMentorID, DWORD dwApprenticeID, int nValue, const char* pszComment)
{
    KRole*  pMRole      = g_pGameCenter->m_RoleManager.GetRole(dwMentorID);
    size_t  uCommentLen = 0;
    char    szApprenticeID[32];

    //PROCESS_ERROR(pMRole);

    //snprintf(szApprenticeID, sizeof(szApprenticeID), "%lu", dwApprenticeID);
    //szApprenticeID[sizeof(szApprenticeID) - 1] = '\0';

    //uCommentLen = strlen(pszComment) + 1;

    //LogPlayerAction(
    //    PLAYER_ACTION_ACQUIRE_MENTOR_VALUE, nValue, szApprenticeID,
    //    pMRole->m_pszAccount, pMRole->m_szName, uCommentLen, (BYTE*)pszComment
    //);

EXIT0:
    return;
}

void KLogServerAgency::LogPlayerItemChange(
    KRole* pRole, ITEM_OPERATION_TYPE eOperationType,
    DWORD dwItemTabType, DWORD dwItemIndex, int nStackNum, 
    const char* pcszComment, const char* pcszCommentExt
)
{
    BOOL                bRetCode            = false;
    int                 nRetCode            = 0;
    IMemBlock*         piSendBuffer        = NULL;
    KGItemLog*          pItemLog            = NULL;
    size_t              uRestoreDataSize    = 0;
    size_t              uCommentSize        = 0;
    size_t              uCommentExtSize     = 0;
    char                szRestoreData[128];

    assert(pRole);
    assert(pcszComment);

    nRetCode = snprintf(
        szRestoreData, sizeof(szRestoreData), 
        "dwTabType: %u, dwIndex: %u, nDurability: %d", 
        dwItemIndex, dwItemIndex, nStackNum
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < sizeof(szRestoreData));
    uRestoreDataSize = nRetCode + 1;

    uCommentSize = strlen(pcszComment) + 1;
    if (pcszCommentExt)
    {
        uCommentExtSize = strlen(pcszCommentExt) + 1;
    }

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(KGItemLog) - 1 + uRestoreDataSize + uCommentSize + uCommentExtSize);
    LOG_PROCESS_ERROR(piSendBuffer);

    pItemLog = (KGItemLog*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pItemLog);

    pItemLog->cProtocol         = LOGC2S_LOG_ITEM_OPERATION;
    pItemLog->ulIdentity        = 0;
    pItemLog->eOperationType    = eOperationType;

    strncpy(pItemLog->szAccountName, pRole->m_pszAccount, sizeof(pItemLog->szAccountName));
    pItemLog->szAccountName[sizeof(pItemLog->szAccountName) - 1] = '\0';

    strncpy(pItemLog->szRoleName, pRole->m_szName, sizeof(pItemLog->szRoleName));
    pItemLog->szRoleName[sizeof(pItemLog->szRoleName) - 1] = '\0';

    pItemLog->ullItemID = 0;
    pItemLog->szItemName[0] = '\0';         // 无法取得Item名称
    pItemLog->uItemValue = 0;               // 无法取得品质
    pItemLog->uItemRestoreDataSize = (unsigned)uRestoreDataSize;
    pItemLog->uCommentSize = (unsigned)(uCommentSize + uCommentExtSize);

    memcpy(pItemLog->pbyData, szRestoreData, uRestoreDataSize);
    memcpy(pItemLog->pbyData + uRestoreDataSize, pcszComment, uCommentSize);
    if (pcszCommentExt)
    {
        pItemLog->pbyData[uRestoreDataSize + uCommentSize - 1] = ' ';
        memcpy(pItemLog->pbyData + uRestoreDataSize + uCommentSize, pcszCommentExt, uCommentExtSize);
    }

    bRetCode = Send(piSendBuffer);
    PROCESS_ERROR(bRetCode);

EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return;
}

BOOL KLogServerAgency::LoginServer(const char cszIP[], int nPort, const char cszIdentity[])
{
    BOOL                bResult      = false;
    BOOL                bRetCode     = false;
    IMemBlock*         piSendBuffer = NULL;
    LOGSERVER_LOGIN* pLogin       = NULL;
    QSocketConnector  Connector;
    struct timeval      TimeVal;

    assert(m_piSocket == NULL);

    m_piSocket = Connector.Connect(cszIP, nPort);
    PROCESS_ERROR(m_piSocket);

    TimeVal.tv_sec  = 2;
    TimeVal.tv_usec = 0;

    bRetCode = m_piSocket->SetTimeout(&TimeVal);
    LOG_PROCESS_ERROR(bRetCode);

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(LOGSERVER_LOGIN));
    LOG_PROCESS_ERROR(piSendBuffer);

    pLogin = (LOGSERVER_LOGIN*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pLogin);

    pLogin->ProtocolType = LOGC2S_LOG_LOGIN;
    pLogin->ulIdentity   = 0;

    strncpy(pLogin->szGatewayName, cszIdentity, sizeof(pLogin->szGatewayName));
    pLogin->szGatewayName[sizeof(pLogin->szGatewayName) - 1] = '\0';

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    QLogPrintf(
        LOG_INFO, "Connect to log server %s:%d ... ... [%s]",
        cszIP, nPort, bResult ? "OK" : "Failed"
    );
    if (!bResult)
    {
        SAFE_RELEASE(m_piSocket);
    }
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KLogServerAgency::Ping()
{
    BOOL                bResult      = false;
    BOOL                bRetCode     = false;
    IMemBlock*         piSendBuffer = NULL;
    tagProtocolHeader2* pPing        = NULL;

    PROCESS_ERROR(g_pGameCenter->m_nTimeNow >= m_nNextPingTime);

    m_nNextPingTime = g_pGameCenter->m_nTimeNow + m_nPingCycle;

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(tagProtocolHeader2));
    LOG_PROCESS_ERROR(piSendBuffer);

    pPing = (tagProtocolHeader2*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pPing);

    pPing->ProtocolType = LOGC2S_PING;
    pPing->ulIdentity   = 0;

    bRetCode = Send(piSendBuffer);
    PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KLogServerAgency::LogPlayerAction(
    PLAYER_ACTION_TYPE eActionType, unsigned uActionLevel, const char* pszActionTarget,
    const char* pszAccountName, const char* pszRoleName, size_t uCommentSize,
    BYTE* pbyComment
)
{
    BOOL                bResult      = false;
    BOOL                bRetCode     = false;
    size_t              uExtendSize  = 0;
    IMemBlock*         piSendBuffer = NULL;
    KGPlayerActionLog*  pLog         = NULL;

    assert(pszActionTarget);
    assert(pszAccountName);
    assert(pszRoleName);

    uExtendSize = uCommentSize ? uCommentSize - 1 : 0;

    piSendBuffer = QCreateMemBlock((unsigned)(sizeof(KGPlayerActionLog) + uExtendSize));
    LOG_PROCESS_ERROR(piSendBuffer);

    pLog = (KGPlayerActionLog*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pLog);

    pLog->ProtocolType = LOGC2S_LOG_PLAYER_ACTION;
    pLog->ulIdentity   = 0;
    pLog->uActionLevel = uActionLevel;
    pLog->eActionType  = eActionType;

    strncpy(pLog->szActionTarget, pszActionTarget, sizeof(pLog->szActionTarget));
    pLog->szActionTarget[sizeof(pLog->szActionTarget) - 1] = '\0';

    strncpy(pLog->szAccountName, pszAccountName, sizeof(pLog->szAccountName));
    pLog->szAccountName[sizeof(pLog->szAccountName) - 1] = '\0';

    strncpy(pLog->szRoleName, pszRoleName, sizeof(pLog->szRoleName));
    pLog->szRoleName[sizeof(pLog->szRoleName) - 1] = '\0';

    pLog->uCommentSize = (unsigned)uCommentSize;
    if (pbyComment)
    {
        memcpy(pLog->pbyData, pbyComment, uCommentSize);
    }

    bRetCode = Send(piSendBuffer);
    PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KLogServerAgency::Send(IMemBlock* piBuffer)
{
    BOOL bResult  = false;
    int  nRetCode = false;

    PROCESS_ERROR(m_piSocket);

    nRetCode = m_piSocket->Send(piBuffer);
    LOG_PROCESS_ERROR(nRetCode == 1);

    bResult = true;
EXIT0:
    return bResult;
}
