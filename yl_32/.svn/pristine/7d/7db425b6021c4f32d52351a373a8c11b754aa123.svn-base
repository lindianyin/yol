#include "stdafx.h"
//#include "Global.h"
#include "GameCenterSettings.h"

#define GCS_SECTION_GLOBAL      "Global"
#define GCS_SECTION_GATEWAY     "Gateway"
#define GCS_SECTION_GAMESERVER  "GameServer"
#define GCS_SECTION_MYSQL       "MySQL"
#define GCS_SECTION_MAIL        "Mail"
#define GCS_SECTION_FELLOWSHIP  "Fellowship"
#define GCS_SECTION_AUCTION     "Auction"
#define GCS_SECTION_ROLE        "Role"
#define GCS_SECTION_CAMP        "Camp"
#define GCS_SECTION_TONG        "Tong"
#define GCS_SECTION_APEXPROXY   "ApexProxy"
#define GCS_SECTION_LOGSERVER   "LogServer"
#define GCS_SECTION_ANTI_FARMER "AntiFarmer"
#define GCS_SECTION_GLOBALSERVER	"GlobalServer"
#define GCS_SECTION_PAY_AGENCY	"PayAgency"

QGameCenterSettings::QGameCenterSettings(void)
{
    m_szLocale[0]                                = '\0';

    m_nMemoryLimitForCreateMap                   = 1400;
    m_fAveragePerformanceLimitForCreateMap       = 6.0f;
    m_fImmediatePerformanceLimitForCreateMap     = 8.0f;
    m_fAveragePerformanceLimitForNewPlayer       = 2.0f;
    m_fImmediatePerformanceLimitForNewPlayer     = 4.0f;
}

BOOL QGameCenterSettings::Load()
{
    BOOL bResult    = false;
    BOOL bRetCode   = false;

    bRetCode = LoadRelaySettings();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = LoadCenterConst();
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;

}
BOOL QGameCenterSettings::LoadRelaySettings()
{
    BOOL        bResult     = false;
    int         nRetCode    = false;	
    IIniFile*	piIniFile   = NULL;
    int			nValue      = 0;

    piIniFile = g_OpenIniFile(COORDINATOR_CFG_INI);
    LOG_PROCESS_ERROR(piIniFile);

    // Global
    nRetCode = piIniFile->GetString(GCS_SECTION_GLOBAL, "Locale", "", m_szLocale, sizeof(m_szLocale));
    LOG_PROCESS_ERROR(nRetCode);

    // Gateway config
    nRetCode = piIniFile->GetString(GCS_SECTION_GATEWAY, "IP", "localhost", m_szGatewayIP, sizeof(m_szGatewayIP));
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_GATEWAY, "Port", 9001, &m_nGatewayPort);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_GATEWAY, "PingCycle", 30, (int*)&m_nGatewayPingCycle);
    LOG_PROCESS_ERROR(nRetCode);

    // GS config
    nRetCode = piIniFile->GetInteger(GCS_SECTION_GAMESERVER, "Port", 5003, &m_nGameServerPort);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_GAMESERVER, "ConnectionTimeout", 20, &m_nConnectionTimeout);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_GAMESERVER, "MemoryLimitForCreateMap", 1400, &m_nMemoryLimitForCreateMap);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetFloat(GCS_SECTION_GAMESERVER, "AveragePerformanceLimitForCreateMap", 6.0f, &m_fAveragePerformanceLimitForCreateMap);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetFloat(GCS_SECTION_GAMESERVER, "ImmediatePerformanceLimitForCreateMap", 8.0f, &m_fImmediatePerformanceLimitForCreateMap);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetFloat(GCS_SECTION_GAMESERVER, "AveragePerformanceLimitForNewPlayer", 4.0f, &m_fAveragePerformanceLimitForNewPlayer);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetFloat(GCS_SECTION_GAMESERVER, "ImmediatePerformanceLimitForNewPlayer", 6.0f, &m_fImmediatePerformanceLimitForNewPlayer);
    LOG_PROCESS_ERROR(nRetCode);

    // MySQL
    nRetCode = piIniFile->GetString(GCS_SECTION_MYSQL, "IP", "127.0.0.1", m_szDBIP, sizeof(m_szDBIP));
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_MYSQL, "Port", 3306, &m_nDBPort);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetString(GCS_SECTION_MYSQL, "Account", "", m_szDBAcc, sizeof(m_szDBAcc));
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetString(GCS_SECTION_MYSQL, "Password", "", m_szDBPsw, sizeof(m_szDBPsw));
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetString(GCS_SECTION_MYSQL, "Database", "", m_szDBName, sizeof(m_szDBName));
    LOG_PROCESS_ERROR(nRetCode);

    // Apex Proxy
    nRetCode = piIniFile->GetString(
        GCS_SECTION_APEXPROXY, "IP", "127.0.0.1", m_szApexServerIP, sizeof(m_szApexServerIP)
    );
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = piIniFile->GetInteger(GCS_SECTION_APEXPROXY, "Port", 10001, &m_nApexServerPort);
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = piIniFile->GetInteger(GCS_SECTION_APEXPROXY, "PingCycle", 5, &m_nApexPingCycle);
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = piIniFile->GetInteger(GCS_SECTION_APEXPROXY, "MaxKickCount", 1, &m_nMaxKickCount);
    LOG_PROCESS_ERROR(nRetCode);

    // LogServer
    nRetCode = piIniFile->GetString(GCS_SECTION_LOGSERVER, "IP", "127.0.0.1", m_szLogServerIP, sizeof(m_szLogServerIP));
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = piIniFile->GetInteger(GCS_SECTION_LOGSERVER, "Port", 5100, &m_nLogServerPort);
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = piIniFile->GetInteger(GCS_SECTION_LOGSERVER, "PingCycle", 30, &m_nLogServerPingCycle);
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = piIniFile->GetString(GCS_SECTION_LOGSERVER, "Identity", "", m_szLogServerIdentity, sizeof(m_szLogServerIdentity));
    LOG_PROCESS_ERROR(nRetCode);

	//pay agency
	nRetCode = piIniFile->GetInteger(GCS_SECTION_PAY_AGENCY, "Port", 21212, &m_nPayAgencyPort);
	LOG_PROCESS_ERROR(nRetCode);

	nRetCode = piIniFile->GetString(GCS_SECTION_PAY_AGENCY, "IP", "127.0.0.1", m_szPayAgencyIP, sizeof(m_szPayAgencyIP));
	LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piIniFile);
    return bResult;
}

BOOL QGameCenterSettings::LoadCenterConst()
{
    BOOL        bResult     = false;
	int         nRetCode    = false;	
    IIniFile*	piIniFile   = NULL;
	int			nValue      = 0;

	piIniFile = g_OpenIniFile(COORDINATOR_CFG_INI);
	LOG_PROCESS_ERROR(piIniFile);

    // Mail System config
    nRetCode = piIniFile->GetInteger(GCS_SECTION_MAIL, "SurvivalTime", 3600, &m_nMailSurvivalTime);
    LOG_PROCESS_ERROR(nRetCode && m_nMailSurvivalTime > 0);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_MAIL, "MaxPlayerMailCount", 100, &m_nMaxPlayerMailCount);
    LOG_PROCESS_ERROR(nRetCode && m_nMaxPlayerMailCount > 0);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_MAIL, "MaxAuctionMailCount", 100, &m_nMaxAuctionMailCount);
    LOG_PROCESS_ERROR(nRetCode && m_nMaxAuctionMailCount > 0);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_MAIL, "MaxSystemMailCount", 100, &m_nMaxSystemMailCount);
    LOG_PROCESS_ERROR(nRetCode && m_nMaxSystemMailCount > 0);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_MAIL, "MaxGmMsgMailCount", 50, &m_nMaxGmMsgMailCount);
    LOG_PROCESS_ERROR(nRetCode && m_nMaxGmMsgMailCount > 0);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_MAIL, "MaxPlayerMsgMailCount", 50, &m_nMaxPlayerMsgMailCount);
    LOG_PROCESS_ERROR(nRetCode && m_nMaxPlayerMsgMailCount > 0);
    nRetCode = piIniFile->GetString(GCS_SECTION_MAIL, "SendFailedName", "System", m_szMailSendFailedName, sizeof(m_szMailSendFailedName));
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetString(GCS_SECTION_MAIL, "WithdrawName", "System", m_szMailWithdrawName, sizeof(m_szMailWithdrawName));
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetString(GCS_SECTION_MAIL, "UserReturnTitle", "Return", m_szMailUserReturnTitle, sizeof(m_szMailUserReturnTitle));
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetString(GCS_SECTION_MAIL, "SystemMailSender", "[SYSTEM]", m_szSystemMailSender, sizeof(m_szSystemMailSender));
    LOG_PROCESS_ERROR(nRetCode);

    // PlayerRelation System config
    nRetCode = piIniFile->GetInteger(GCS_SECTION_FELLOWSHIP, "SaveIntervalFrame", 60, &m_nFellowshipSaveInterval);
    LOG_PROCESS_ERROR(nRetCode && m_nFellowshipSaveInterval > 0);
    //
    //// Auction config
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "AuctionName", "Auction", m_szAuctionName, sizeof(m_szAuctionName));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "Gold", "Gold", m_szGold, sizeof(m_szGold));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "Silver", "Silver", m_szSilver, sizeof(m_szSilver));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "Copper", "Copper", m_szCopper, sizeof(m_szCopper));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "SellSucceed", "SellSucceed", m_szSellSucceed, sizeof(m_szSellSucceed));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "Buyyer", "Buyyer", m_szBuyyer, sizeof(m_szBuyyer));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "Price", "Price", m_szPrice, sizeof(m_szPrice));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "CustodyCharges", "CustodyCharges", m_szCustodyCharges, sizeof(m_szCustodyCharges));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "TaxMoney", "TaxMoney", m_szTaxMoney, sizeof(m_szTaxMoney));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "GetMoney", "GetMoney", m_szGetMoney, sizeof(m_szGetMoney));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "CancelSucceed", "CancelSucceed", m_szCancelSucceed, sizeof(m_szCancelSucceed));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "BidFailed", "BidFailed", m_szBidFailed, sizeof(m_szBidFailed));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "BuySucceed", "BuySucceed", m_szBuySucceed, sizeof(m_szBuySucceed));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "BuyItNowPrice", "BuyItNowPrice", m_szBuyItNowPrice, sizeof(m_szBuyItNowPrice));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "TopPrice", "TopPrice", m_szTopPrice, sizeof(m_szTopPrice));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "Seller", "Seller", m_szSeller, sizeof(m_szSeller));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetString(GCS_SECTION_AUCTION, "Coin", "Coin", m_szCoin, sizeof(m_szCoin));
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetInteger(GCS_SECTION_AUCTION, "AuctionTaxRate", 5, &m_nAuctionTaxRate);
    //LOG_PROCESS_ERROR(nRetCode);
    //nRetCode = piIniFile->GetInteger(GCS_SECTION_AUCTION, "GameCardTaxRate", 5, &m_nGameCardTaxRate);
    //LOG_PROCESS_ERROR(nRetCode);

    // Role config
    nRetCode = piIniFile->GetInteger(GCS_SECTION_ROLE, "MaxNameLen", 0, &m_nMaxRoleNameLength);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_ROLE, "MinNameLen", 0, &m_nMinRoleNameLength);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_ROLE, "MaxRoleCount", 0, &m_nMaxRoleCount);
    LOG_PROCESS_ERROR(nRetCode);

	// BattleZone config
	nRetCode = piIniFile->GetInteger(GCS_SECTION_GLOBALSERVER, "Mode", 2, &m_nGlobalServerMode);
	LOG_PROCESS_ERROR(nRetCode);
	nRetCode = piIniFile->GetInteger(GCS_SECTION_GLOBALSERVER, "Port", 8000, &m_nGlobalServerPort);
	LOG_PROCESS_ERROR(nRetCode);
	nRetCode = piIniFile->GetString(GCS_SECTION_GLOBALSERVER, "IP", "127.0.0.1", m_szGlobalServerAddr, 16);
	LOG_PROCESS_ERROR(nRetCode);
	nRetCode = piIniFile->GetInteger(GCS_SECTION_GLOBALSERVER, "PingInterval", 20, &m_nGlobalServerPingInterval);
	LOG_PROCESS_ERROR(nRetCode);

    /*
    nRetCode = piIniFile->GetInteger(GCS_SECTION_ROLE, "DeleteProtectLevel", 0, &m_nDeleteProtectLevel);
    LOG_PROCESS_ERROR(nRetCode);
    nRetCode = piIniFile->GetInteger(GCS_SECTION_ROLE, "DeleteDelayTime", 0, &m_nDeleteDelayTime);
    LOG_PROCESS_ERROR(nRetCode);
    */
    m_nDeleteProtectLevel   = 20;
    m_nDeleteDelayTime      = 7 * 24 * 60 * 60;

    //// Tong
    //nRetCode = piIniFile->GetString(
    //    GCS_SECTION_TONG, "TongReturnMoneyMailTitle", "tong system return money", 
    //    m_szTongReturnMoneyMailTitle, sizeof(m_szTongReturnMoneyMailTitle)
    //);
    //LOG_PROCESS_ERROR(nRetCode);

    //// AntiFarmer
    //nRetCode = piIniFile->GetBool(GCS_SECTION_ANTI_FARMER, "IsEnable", &m_bAntiFarmerEnable);
    //LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piIniFile);
	return bResult;
}
