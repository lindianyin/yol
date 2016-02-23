
#include "stdafx.h"
#include "DBBase.h"
#include "IDSequence.h"

#include "errmsg.h"
#include "mysqld_error.h"

#define SEQ_TABLE_NAME  "sequence"
#define SEQ_NAME        "seqname"
#define SEQ_CURRENT_ID  "currentid"

static const unsigned long MYSQL5_VERSION_ID = 50000; // MySQL 5 Version ID

#ifdef WIN32
#define strtoull _strtoui64
#endif

class KIDSequence : public IKIDSequence
{
public:
    KIDSequence();
    ~KIDSequence();

    HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject
    )
    {
        USE_ARGUMENT(riid);
        USE_ARGUMENT(ppvObject);
        return E_FAIL;
    }

    ULONG STDMETHODCALLTYPE AddRef(void)
    {
        return (unsigned long)InterlockedIncrement((long *)&m_ulRefCount);
    }

    ULONG STDMETHODCALLTYPE Release(void)
    {
        unsigned long ulCount = InterlockedDecrement((long *)&m_ulRefCount);
        if (ulCount == 0)
        {
            Destroy();
            delete this;
        }
        return ulCount;
    }

    BOOL GenerateID(const char cszSequenceName[], int nPrefetch, UINT64* puRetID);

public:
    BOOL Create(const char cszDBAddr[], const char cszDBName[], const char cszUserName[], const char cszPassword[]);
    void Destroy();
    
private:
    BOOL DoQuery(const char cszSQL[]);

    unsigned long   m_ulRefCount;
    MYSQL*          m_pDBHandle;
    QLock          m_Lock;

    struct KID_PREFETCH_INFO
    {
        UINT64 uIDEnd;
        UINT64 uNextID;
    };

	typedef std::map<std::string, KID_PREFETCH_INFO> KID_PREFETCH_TABLE;
    
    KID_PREFETCH_TABLE m_PrefetchTable;
};

KIDSequence::KIDSequence()
{
    m_ulRefCount    = 1;
    m_pDBHandle     = NULL;
}

KIDSequence::~KIDSequence()
{
   	m_pDBHandle = NULL;
    m_PrefetchTable.clear();
}

BOOL KIDSequence::GenerateID(const char cszSequenceName[], int nPrefetch, UINT64* puRetID)
{
    BOOL                                            bResult     = false;    
    int                                             nRetCode    = false;
    KID_PREFETCH_INFO*                              pPrefetch   = NULL;
    MYSQL_RES*                                      pQueryRes   = NULL;
    UINT64                                        	uStartID    = 0;
    my_ulonglong                                    uRowCount   = 0;
    MYSQL_ROW                                       Row;
    char                                            szQuery[256];
    KID_PREFETCH_TABLE::iterator                    it;
    std::pair<KID_PREFETCH_TABLE::iterator, BOOL>   InsRet;

    LOG_PROCESS_ERROR(cszSequenceName);
    LOG_PROCESS_ERROR(puRetID);
    LOG_PROCESS_ERROR(m_pDBHandle);
    LOG_PROCESS_ERROR(nPrefetch >= 1);

    m_Lock.Lock();
    
    it = m_PrefetchTable.find(cszSequenceName);
    if (it != m_PrefetchTable.end())
    {
        pPrefetch = &it->second;
    }
    else
    {
        InsRet = m_PrefetchTable.insert(std::make_pair(cszSequenceName, KID_PREFETCH_INFO()));
        LOG_PROCESS_ERROR(InsRet.second);
        
        pPrefetch = &InsRet.first->second;

        pPrefetch->uIDEnd   = 0;
        pPrefetch->uNextID  = 0;
    }

    if (pPrefetch->uNextID < pPrefetch->uIDEnd)
        goto EXIT1;

    while(true)
    { 
        nRetCode = snprintf(
            szQuery,  
            sizeof(szQuery),
            "select %s from %s where %s = '%s'",
            SEQ_CURRENT_ID, SEQ_TABLE_NAME, SEQ_NAME,
            cszSequenceName
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szQuery));

        nRetCode = DoQuery(szQuery);
        LOG_PROCESS_ERROR(nRetCode);

        if (pQueryRes)
        {
            mysql_free_result(pQueryRes);
            pQueryRes = NULL;
        }

        pQueryRes = mysql_store_result(m_pDBHandle);
        LOG_PROCESS_ERROR(pQueryRes);

        uRowCount = mysql_num_rows(pQueryRes);
        if (uRowCount == 0)
        {
            nRetCode = snprintf(
                szQuery, sizeof(szQuery),
                "insert into %s (%s, %s) values('%s', 1)",
                SEQ_TABLE_NAME, SEQ_NAME, SEQ_CURRENT_ID,
                cszSequenceName
            );
            LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szQuery));

            nRetCode = DoQuery(szQuery);
            LOG_PROCESS_ERROR(nRetCode);

            continue;
        }

        Row = mysql_fetch_row(pQueryRes);
        LOG_PROCESS_ERROR(Row);
        
        LOG_PROCESS_ERROR(Row[0]);

        uStartID = (UINT64)strtoull(Row[0], NULL, 10);
        
        LOG_PROCESS_ERROR(uStartID < ULLONG_MAX - nPrefetch);

        nRetCode = snprintf(
            szQuery,
            sizeof(szQuery),
            "update %s set %s = %s + %d where %s = '%s' and %s = %llu",
            SEQ_TABLE_NAME, SEQ_CURRENT_ID, SEQ_CURRENT_ID, nPrefetch,
            SEQ_NAME, cszSequenceName, SEQ_CURRENT_ID, uStartID
        );
        LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szQuery));

        nRetCode = DoQuery(szQuery);
        LOG_PROCESS_ERROR(nRetCode);

        nRetCode = (int)mysql_affected_rows(m_pDBHandle);
        if (nRetCode == 1)
            break;
    }
    
    pPrefetch->uIDEnd   = uStartID + nPrefetch;
    pPrefetch->uNextID  = uStartID;

EXIT1:
    *puRetID = pPrefetch->uNextID++;

    bResult = true;
EXIT0:
    if (pQueryRes)
    {
        mysql_free_result(pQueryRes);
        pQueryRes = NULL;
    }

    m_Lock.Unlock();
    return bResult;
}

BOOL KIDSequence::Create(const char cszDBAddr[], const char cszDBName[], const char cszUserName[], const char cszPassword[])
{
    BOOL        bResult     = false;
    MYSQL*      pHandle     = NULL;
    int         nRetCode    = false;
    MYSQL_RES*  pQueryRes   = NULL;
    unsigned    uRowCount   = 0;
    char        szQuery[256];

    pHandle = MySQL_Connect(cszDBAddr, cszDBName, cszUserName, cszPassword);
    LOG_PROCESS_ERROR(pHandle);
    
    m_pDBHandle = pHandle;
    pHandle = NULL;

    nRetCode = snprintf(
        szQuery,
        sizeof(szQuery), 
        "create table if not exists %s (%s char(64) primary key, %s bigint unsigned)",
        SEQ_TABLE_NAME, SEQ_NAME, SEQ_CURRENT_ID
    );
    LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)sizeof(szQuery));
    
    nRetCode = DoQuery(szQuery);
    PROCESS_ERROR(nRetCode);
    
    bResult = true;
EXIT0:
    if (!bResult)
    {
        if (pHandle)
        {
            MySQL_Disconnect(pHandle);
            pHandle = NULL;
        }
    }
    return bResult;
}

void KIDSequence::Destroy()
{
    if (m_pDBHandle)
    {
        MySQL_Disconnect(m_pDBHandle);
        m_pDBHandle = NULL;
    }
    m_PrefetchTable.clear();
}

BOOL KIDSequence::DoQuery(const char cszSQL[])
{
    BOOL     bResult            = false;
    BOOL     bRetCode           = false;
    unsigned ulMySQLErrorCode   = 0;

    LOG_PROCESS_ERROR(cszSQL);
    LOG_PROCESS_ERROR(m_pDBHandle);
    
    bRetCode = MySQL_Query(m_pDBHandle, cszSQL, &ulMySQLErrorCode);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    if (ulMySQLErrorCode != 0 && ulMySQLErrorCode != ER_DUP_ENTRY) // 主键冲突不用断开链接
    {
        MySQL_Disconnect(m_pDBHandle);
        m_pDBHandle = NULL;
    }
    return bResult;
}

IKIDSequence* CreateIDSequence(
    const char cszDBAddr[], const char cszDBName[], const char cszUserName[], const char cszPassword[]
)
{
    IKIDSequence*   piResult    = NULL;
    KIDSequence*    pIDSequence = NULL;
    BOOL            bRetCode    = false;

    LOG_PROCESS_ERROR(cszDBAddr);
    LOG_PROCESS_ERROR(cszDBName);
    LOG_PROCESS_ERROR(cszUserName);
    LOG_PROCESS_ERROR(cszPassword);

    pIDSequence = new KIDSequence;
    LOG_PROCESS_ERROR(pIDSequence);

    bRetCode = pIDSequence->Create(cszDBAddr, cszDBName, cszUserName, cszPassword);
    LOG_PROCESS_ERROR(bRetCode);

    piResult = pIDSequence;
EXIT0:
    if (!piResult)
    {
        SAFE_RELEASE(pIDSequence);
    }
    return piResult;
}
