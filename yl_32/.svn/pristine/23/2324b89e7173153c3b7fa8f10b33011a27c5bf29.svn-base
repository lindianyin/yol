
#pragma once

// 邮件缓存调度管理器

#include <list>
#include "MailBox.h"
#include "MailOperation.h"

#define MAX_MAILBOX_DB_DATA_SIZE    (1024 * 1024 * 4)
#define MAX_CACHE_MAIL_COUNT        160000

class KMailManager
{
public:
    KMailManager();
    ~KMailManager();

    BOOL Init();
    void UnInit();

    void Activate();

    void SaveAll();

    BOOL LoadGlobalMail(int nVersion, DWORD dwMailID, time_t nEndTime, BYTE* pbyData, size_t uDataLen);

    BOOL PostRequestSendMail(int nRequestID, DWORD dwMailBoxID, KMail* pMail, size_t uMailLen);
    BOOL PostRequestSendMail(
        int nRequestID, DWORD dwMailBoxID, int nDelayTime, BYTE byType,
        const char* pcszSenderName,
        const char* pcszTitle, 
        int nMoney,int nSilver,int nEnergy,
        BYTE* pbyTextData, size_t uTextLen,
        BYTE* pbyItemData, size_t uItemDataLen, int nItemPrice
    );

    BOOL PostRequestGetMailList(DWORD dwMailBoxID, DWORD dwStartID);
    BOOL PostRequestDeleteMail(DWORD dwMailBoxID, DWORD dwMailID);
    BOOL PostRequestQueryMailContent(DWORD dwMailBoxID, DWORD dwMailID, KMAIL_TYPE eMailType);
    BOOL PostRequestAcquireMailMoney(DWORD dwMailBoxID, DWORD dwMailID);
    BOOL PostRequestAcquireMailItem(DWORD dwMailBoxID, DWORD dwMailID, int nIndex);

    BOOL PostRequestReleaseMailMoney(DWORD dwMailBoxID, DWORD dwMailID);
    BOOL PostRequestReleaseMailItem(DWORD dwMailBoxID, DWORD dwMailID, int nIndex);
    BOOL PostRequestUnLockMail(DWORD dwMailBoxID, DWORD dwMailID);
    BOOL PostRequestSetMailRead(DWORD dwMailBoxID, DWORD dwMailID);
    BOOL PostRequestUserReturnMail(DWORD dwMailBoxID, DWORD dwMailID);
    BOOL PostRequestWithdrawMail(DWORD dwMailBoxID);
    BOOL PostRequestDeleteAll(DWORD dwMailBoxID);

    BOOL PostRequestLoadMailCount(DWORD dwMailBoxID);

    int  GetCacheHitRatio(){ return m_nCacheHitRatio; }
    int  GetCacheMissRatio(){ return m_nCacheMissRatio; }

    BOOL SendGlobalMail(time_t nEndTime, KMail* pMailInfo, size_t uMailLen);
    BOOL SendGlobalMail(
        int nLifeTime, BYTE byType,
        const char* pcszSenderName,
        const char* pcszTitle, 
        int nMoney,
        BYTE* pbyTextData, size_t uTextLen,
        BYTE* pbyItemData, size_t uItemDataLen, int nItemPrice
    );
    BOOL DeleteGlobalMail(DWORD dwMailID);
    void DeleteAllGlobalMail();

	void SyncMailList(DWORD dwPlayerId);

public:
    BOOL OnLoadMailBoxRespond(
        BOOL bSucceed, DWORD dwMailBoxID, time_t nBaseTime,
        BYTE* pbyMailBoxInfo, size_t uMailBoxLen
    );

    void OnCheckWithdrawMailRespond(int nMailBoxCount, DWORD* pdwMailBoxIDList);

    BOOL GetCachedMailCount(DWORD dwMailBoxID, KMailCount* pMailCount);
    void UnLoadMailCount(DWORD dwMailBoxID);

private:
    BOOL OnSendMailOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnGetMailListOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnDeleteMailOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnQueryMailContentOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnAcquireMailMoneyOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnAcquireMailItemOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnReleaseMailMoneyOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnReleaseMailItemOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnUnLockMailOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnSetMailReadOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnUserReturnMailOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnWithdrawMailOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnDeleteAllOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);
    BOOL OnLoadMailCountOperation(KMailBox* pMailBox, BYTE* pbyData, size_t uDataLen);

    void UpdateMailCount(DWORD dwID, const KMailCount& crCount);
    BOOL ExtractContentFromMail(KMail* pMail, KMailContent** ppContent, size_t* puContentLen);

private:
    typedef BOOL (KMailManager::*KMAIL_OPERATION_FUNC)(KMailBox*, BYTE*, size_t);
    KMAIL_OPERATION_FUNC    m_MailOperationFunc[emoTotal];

    typedef std::list<void*> KMAIL_OPERATION_QUEUE;
    KMAIL_OPERATION_QUEUE   m_OperationQueue;

    typedef std::list<DWORD> KWITHDRAW_MAIL_QUEUE;
    KWITHDRAW_MAIL_QUEUE    m_WithdrawMailQueue;

    // 全局群发邮件支持:
    struct KGlobalMail 
    {
        time_t  nEndTime;
        KMail*  pMail;
    };

    typedef std::map<DWORD, KGlobalMail, std::less<DWORD> > KGLOBAL_TABLE;
    KGLOBAL_TABLE   m_GlobalMailTable;

    BOOL    m_bCacheReady;
    time_t  m_nNextSaveTime;
    time_t  m_nNextWithdrawTime;
    time_t  m_nNextCheckGlobalMailLifeTime;
    BYTE*   m_pbyBuffer;

    int     m_nCacheHitRatio;
    int     m_nCacheMissRatio;
    int     m_nCacheHitCount;
    int     m_nCacheMissCount;

    typedef std::map<DWORD, KMailCount, std::less<DWORD> > KMailCountTable;
    KMailCountTable m_MailCountTable;   // 注意: 这个邮件数量不是精确的,会有时间差

private:
    // cache manage
    void      CheckCache();
    KMailBox* GetMailBox(DWORD dwID);
    KMailBox* NewMailBox(DWORD dwID);

    struct KCacheNode;

    typedef std::map<DWORD, KCacheNode*, std::less<DWORD> > KCacheTable;

    struct KCacheNode : QNode
    {
        KMailBox    MailBox;
        KCacheTable::iterator CacheIter;
    };

    QList              m_MailQueue;
    KCacheTable        m_CacheTable;
    int                m_nMaxCache;
};

