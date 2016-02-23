
#pragma once

#include "Protocol/KG_CSLogServerProtocal.h"

#define MAX_LOG_BUFFER_SIZE 1024

class KRole;

class KLogServerAgency
{
public:
    KLogServerAgency();

    BOOL Init();
    void UnInit();

    void Activate();
	void LogConsumeAction(int eActionType,DWORD roleId, int nCoin, const char* pszComment);
	void LogAction(PLAYER_ACTION_TYPE eActionType,KRole* pRole, int nCoin, const char* pszComment);
    void LogFreezeCoin(KRole* pRole, int nCoin, const char* pszComment);
    void LogUnfreezeCoin(KRole* pRole, int nCoin, const char* pszComment);
    void LogGameCardSell(KRole* pRole, int nCoin, const char* pszComment);
    void LogGameCardBuy(KRole* pRole, int nGameTime, BYTE byType, const char* pszComment);
    void LogRoleAntiFarmer(KRole* pRole, time_t nAntiSeconds);
    void LogBuyAuctionItem(DWORD dwBuyerID, DWORD dwSellerID, size_t uItemDataLen, BYTE* pbyItemData, int nPrice);
    void LogApexKickRole(KRole* pRole, int nActionID);
    void LogFreezeCoinTimeout(KRole* pRole, int nCoin, int nPrice);
    void LogAccountExchangeTimeout(KRole* pSellerRole, KRole* pBuyyerRole, int nCoin, int nGameTime, int nPrice);
    void LogMentorshipCreate(DWORD dwMentorID, DWORD dwApprenticeID, const char* pszComment);
    void LogMentorshipDelete(DWORD dwMentorID, DWORD dwApprenticeID, int nMentorValue, const char* pszComment);
    void LogAcquireMentorValue(DWORD dwMentorID, DWORD dwApprenticeID, int nValue, const char* pszComment);
    void LogPlayerItemChange(
        KRole* pRole, ITEM_OPERATION_TYPE eOperationType,
        DWORD dwItemTabType, DWORD dwItemIndex, int nStackNum, 
        const char* pcszComment, const char* pcszCommentExt = NULL
    );

private:
    BOOL LoginServer(const char cszIP[], int nPort, const char cszIdentity[]);

    BOOL Ping();

    BOOL LogPlayerAction(
        PLAYER_ACTION_TYPE eActionType, unsigned uActionLevel, const char* pszActionTarget,
        const char* pszAccountName, const char* pszRoleName, size_t uCommentSize,
        BYTE* pbyComment
    );

    BOOL Send(IMemBlock* piBuffer);

public:
    char              m_szIdentity[_NAME_LEN];

private:
    ISocketStream* m_piSocket;
    int               m_nPingCycle;
    time_t            m_nNextPingTime;
    char              m_szTempBuffer[MAX_LOG_BUFFER_SIZE];
};

