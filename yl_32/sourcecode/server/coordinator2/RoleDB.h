
#pragma once

#include <map>
#include <list>
#include "DBBase.h"
#include "ProtocolBasic.h"
#include "config/kplayerbasedbdef.h"

#define KG_ID_FIELD_NAME            "ID"
#define KG_NAME_FIELD_NAME          "RoleName"
#define KG_ACCOUNT_FIELD_NAME       "Account"
#define KG_BASE_INFO_FIELD_NAME     "BaseInfo"
#define KG_EXT_INFO_FIELD_NAME      "ExtInfo"
#define KG_LAST_MODIFY_FIELD_NAME   "LastModify"
#define KG_DELETE_TIME_FIELD_NAME   "DeleteTime"

struct KACCOUNT_LOGIN_INFO 
{
    int             nChargeFlag;                // 付费状态
    tagExtPointInfo	ExtPointInfo;			    // 可用的附送点
    DWORD           dwEndTimeOfFee;             // 收费总截止时间
    DWORD           dwCoin;                     // 金币数
    DWORD           dwLimitPlayTimeFlag;        // 防沉迷标志位
    DWORD           dwLimitOnlineSecond;        // 防沉迷累计在线时间
    DWORD           dwLimitOfflineSecond;       // 防沉迷累计离线时间
    int             nIdentity;                  // 请求包的id
};

#define MAX_ROLE_DB_SQL_SIZE    (MAX_ROLE_DATA_SIZE * 2)

class KRoleManager;

class KRoleDB
{
public:
    KRoleDB();
    ~KRoleDB();

    BOOL                Init();
    void                UnInit();
    
    void                Activate();

    float               GetRoleSaveTimeCost() { return m_fRoleSaveTimeCost; } 
    float               GetRoleLoadTimeCost() { return m_fRoleLoadTimeCost; }
    
public:
    void                DoCreateRole(KDB_ROLE_GENERATOR_PARAM* pRoleParam, unsigned long ulIdentity);
    void                DoSaveRoleData(int nGSIndex, DWORD dwPlayerID, const KRoleBaseInfo& crBaseInfo, IMemBlock* piRoleData, int nUserValue);
    void                DoLoadRoleData(int nGSIndex, DWORD dwPlayerID);
    void                DoDeleteRole(DWORD dwRoleID);
    void                DoQueryRoleBaseData(DWORD dwRoleID, int nPlayerIndex, int nRoleIndex, int nRoleCount);
    void                DoRename(DWORD dwRoleID, int nPlayerIndex, char szNewName[]);
	void				DoCreateDelayRoleData(DWORD dwPlayerID, IMemBlock* piRoleData);
	void				DoSaveDelayRoleData(DWORD dwPlayerID, IMemBlock* piRoleData);
	void				DoDelayLoadRoleData(int nGSIndex, INT nModuleId, DWORD dwPlayerID, DWORD dwRequestId);

private:
    static void         WorkThreadFunction(void* pvParam);
    int                 ThreadFunction();
    void                DBThreadActivate();
    
    void                ProcessCreateQueue();
    void                ProcessSaveQueue(int nSaveCount);
    void                ProcessLoadQueue();
    void                ProcessDeleteQueue();
    void                ProcessQueryRoleBaseInfoQueue();
    void                ProcessRenameQueue();
	void				ProcessCreateDelayQueue();
	void				ProcessSaveDelayQueue();
	void				ProcessDelayLoadQueue();

private:
    BOOL                Connect();
    void                Disconnect();

private:    
    void                DealWithLoadRoleDataRespond();
    void                DealWithCreateRoleRespond();
    void                DealWithQueryRoleBaseDataRespond();
    void                DealWithRenameRespond();
	void				DealWithDelayLoadRoleDataRespond();

private:
    MYSQL*              m_pDBHandle;
    time_t              m_nNextPingTime;
    QThread             m_WorkThread;
    BOOL                m_bRunFlag;
    char*               m_pszSQL;

public:
    int                 m_nDBThreadWorkLoop;
    int                 m_nDBThreadIdleLoop;

private:
    // ------------- 创建角色 -------------------
    struct KCREATE_REQUEST
    {
        KDB_ROLE_GENERATOR_PARAM    Param;
        unsigned long               ulIdentity;
    };
    typedef std::list<KCREATE_REQUEST> KCREATE_QUEUE;
    KCREATE_QUEUE       m_CreateQueue;
    QLock              m_CreateQueueLock;

    struct KCREATE_RESULT 
    {
        unsigned long   ulIdentity;
        BYTE            byCode;
        KROLE_LIST_INFO RoleListInfo;
    };
    typedef std::list<KCREATE_RESULT> KCREATE_RESULT_QUEUE;
    KCREATE_RESULT_QUEUE    m_CreateResult;
    QLock                  m_CreateResultLock;
    
    DWORD   GetCurrentRoleID();
    BOOL    GetRoleBaseInfo(KDB_ROLE_GENERATOR_PARAM* pParam, DWORD dwRoleID, KRoleBaseInfo* pRetBaseInfo);
    BOOL    GetCreateTask(KCREATE_REQUEST* pTask);
    BOOL    CheckRoleCountInAccount(KDB_ROLE_GENERATOR_PARAM* pCreateRoleParam);
    BYTE    CreateRole(KDB_ROLE_GENERATOR_PARAM* pCreateRoleParam, KROLE_LIST_INFO* pRetRoleListInfo);

private:
    // -------- 角色保存相关数据结构 ----------------------
    struct KSAVE_REQUEST
    {
        int                     nGSIndex;
        int                     nUserValue;
        KRoleBaseInfo           BaseInfo;
        IMemBlock*             piExtInfo;
    };
    
    typedef std::map<DWORD, KSAVE_REQUEST>  KSAVE_DATA_TABLE;
    typedef std::list<DWORD>                KSAVE_QUEUE;
    
    KSAVE_DATA_TABLE    m_SaveDataTable;
    KSAVE_QUEUE         m_SaveQueue;
    QLock              m_SaveQueueLock;

    float               m_fRoleSaveTimeCost;

    BOOL        GetSaveTask(DWORD* pdwRetRoleID, KSAVE_REQUEST* pTask);
    BOOL        SaveRoleData(DWORD dwRetRoleID, const KRoleBaseInfo& crBaseInfo, IMemBlock* piExtInfo);

private:
    // --------- 角色加载相关数据结构 --------------------
    struct KLOAD_REQUEST
    {
        DWORD       dwPlayerID;
        int         nGSIndex;
    };
    typedef std::list<KLOAD_REQUEST>        KLOAD_QUEUE;
    
    KLOAD_QUEUE m_LoadQueue;
    QLock      m_LoadQueueLock;

    struct KLOAD_RESULT
    {
        BOOL        bSucceed;
        DWORD       dwPlayerID;
        int         nGSIndex;
        IMemBlock* piRoleData;
    };
    typedef std::list<KLOAD_RESULT>        KLOAD_RESULT_QUEUE;
    
    KLOAD_RESULT_QUEUE  m_LoadRetQueue;
    QLock              m_LoadRetLock;
    float               m_fRoleLoadTimeCost;

    BOOL        GetLoadTask(KLOAD_REQUEST* pTask);
    BOOL        LoadRoleData(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo);
    BOOL        LoadRoleDataFromSaveQueue(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo);
    BOOL        LoadRoleDataFromDB(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo);

private:
    // -------- 删除角色 -----------------------------
    struct KDELETE_REQUEST 
    {
        DWORD           dwRoleID;
    };
    typedef std::list<KDELETE_REQUEST>      KDELETE_REQUEST_QUEUE;
    KDELETE_REQUEST_QUEUE   m_DeleteQueue;
    QLock                  m_DeleteQueueLock;
    
    BOOL        GetDeleteTask(KDELETE_REQUEST* pTask);
    BOOL        DeleteRole(DWORD dwRoleID);

private:
    // ---------- 查询角色基本信息 -----------------------------
    struct KQUERY_ROLE_BASE_INFO_REQUEST
    {
        DWORD               dwRoleID;
        int                 nPlayerIndex;
        int                 nRoleIndex;
        int                 nRoleCount;
    };
    typedef std::list<KQUERY_ROLE_BASE_INFO_REQUEST> KQUERY_ROLE_BASE_INFO_REQUEST_QUEUE;
    KQUERY_ROLE_BASE_INFO_REQUEST_QUEUE m_QueryRoleBaseInfoQueue;
    QLock                              m_QueryRoleBaseInfoQueueLock;

    struct KQUERY_ROLE_BASE_INFO_RESULT
    {
        DWORD               dwRoleID;
        int                 nPlayerIndex;
        int                 nRoleIndex;
        int                 nRoleCount;
        BOOL                bSucceed;
        KRoleBaseInfo       BaseInfo;
    };

    typedef std::list<KQUERY_ROLE_BASE_INFO_RESULT> KQUERY_ROLE_BASE_INFO_RESULT_QUEUE;
    KQUERY_ROLE_BASE_INFO_RESULT_QUEUE   m_QueryRoleBaseResultQueue;
    QLock                               m_QueryRoleBaseResultQueueLock;

    BOOL        GetQueryRoleBaseInfoTask(KQUERY_ROLE_BASE_INFO_REQUEST* pTask);
    BOOL        QueryRoleBaseInfo(DWORD dwRoleID, KRoleBaseInfo* pBaseInfo);
    BOOL        QueryRoleBaseInfoFromSaveQueue(DWORD dwRoleID, KRoleBaseInfo* pBaseInfo);
    BOOL        QueryRoleBaseInfoFromDB(DWORD dwRoleID, KRoleBaseInfo* pBaseInfo);

private:
    // ---------- 更改角色姓名信息 -----------------------------
    struct KRENAME_REQUEST
    {
        DWORD dwRoleID;
        int   nPlayerIndex;
        char  szNewName[_NAME_LEN];
    };
    typedef std::list<KRENAME_REQUEST> KRENAME_REQUEST_QUEUE;
    KRENAME_REQUEST_QUEUE   m_RenameQueue;
    QLock                  m_RenameQueueLock;

    struct KRENAME_RESULT
    {
        BYTE            byCode;
        DWORD           dwRoleID;
        int             nPlayerIndex;
        KROLE_LIST_INFO RoleListInfo;
    };
    typedef std::list<KRENAME_RESULT> KRENAME_RESULT_QUEUE;
    KRENAME_RESULT_QUEUE    m_RenameResultQueue;
    QLock                  m_RenameResultQueueLock;

    BOOL        GetRenameTask(KRENAME_REQUEST* pTask);
    BYTE        Rename(DWORD dwRoleID, char szNewName[]);

    BOOL        DoQuery(const char cszSQL[]);

private:
	// -------- 离线角色相关数据结构 ----------------------
	// 创建
	struct KCREATE_DELAY_REQUEST
	{
		DWORD					dwRoleId;
		IMemBlock*             piExtInfo;
	};

	typedef std::list<KCREATE_DELAY_REQUEST> KCREATE_DELAY_REQUEST_QUEUE;

	KCREATE_DELAY_REQUEST_QUEUE   m_CreateDelayQueue;
	QLock					 m_CreateDelayQueueLock;

	BOOL GetCreateDelayTask(KCREATE_DELAY_REQUEST* pTask);
	BOOL CreateDelayRoleData(DWORD dwRoleId, IMemBlock* piDelayData);

	// 存储
	struct KSAVE_DELAY_REQUEST
	{
		DWORD					dwRoleId;
		IMemBlock*             piExtInfo;
	};

	typedef std::list<KSAVE_DELAY_REQUEST> KSAVE_DELAY_REQUEST_QUEUE;

	KSAVE_DELAY_REQUEST_QUEUE   m_SaveDelayQueue;
	QLock					 m_SaveDelayQueueLock;

	BOOL GetSaveDelayTask(KSAVE_DELAY_REQUEST* pTask);
	BOOL SaveDelayRoleData(DWORD dwRoleId, IMemBlock* piDelayData);

	// 取数据
	struct KDELAY_LOAD_REQUEST
	{
		DWORD       dwPlayerID;
		DWORD		dwSrcId;
		BYTE		byModuleId;
		int         nGSIndex;
	};
	typedef std::list<KDELAY_LOAD_REQUEST>        KDELAY_LOAD_QUEUE;

	KDELAY_LOAD_QUEUE m_DelayLoadQueue;
	QLock      m_DelayLoadQueueLock;

	struct KDELAY_LOAD_RESULT
	{
		BOOL        bSucceed;
		BYTE		byModuleId;
		DWORD       dwPlayerID;
		DWORD		dwSrcId;
		int         nGSIndex;
		IMemBlock* piRoleData;
	};
	typedef std::list<KDELAY_LOAD_RESULT>        KDELAY_LOAD_RESULT_QUEUE;

	KDELAY_LOAD_RESULT_QUEUE  m_DelayLoadRetQueue;
	QLock              m_DelayLoadRetLock;

	BOOL        GetDelayLoadTask(KDELAY_LOAD_REQUEST* pTask);
	BOOL		LoadDelayRoleData(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo);
	BOOL		LoadDelayRoleDataFromDB(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo);

	// 数据缓存
	typedef std::map<DWORD, KDelayRoleData>  KDELAY_ROLE_DATA_TABLE;

	KDELAY_ROLE_DATA_TABLE    m_DelayRoleDataTable;
	QLock					  m_DelayTableLock;

	void CacheDelayRoleData(DWORD dwRoleId, IMemBlock* piDelayData);
	BOOL LoadDelayRoleDataFromCache(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo);

public:
	/**************************角色订单相关**********************************************************************/
	BOOL CreatePayRecord(DWORD dwRoleID, IMemBlock* piDelayData);

	BOOL SavePayRecord(DWORD dwRoleID, IMemBlock* piDelayData);

	BOOL ProcessSavePayRecord();

	//BOOL LoadPayRecordFromDB(DWORD dwRoleID, IMemBlock** ppiRoleExtInfo);

	BOOL LoadPayRecord(DWORD dwRoleID, BYTE* pData, size_t uDataLen);

	//VOID ProcessPayRecord(DWORD dwRoleID);

	BOOL ProcessPayOrder(DWORD dwRoleID, CHAR* szOrderno, INT nGold);

	BOOL SendPlayerOrder(DWORD dwRoleID);

	BOOL DoOrderFromGS(DWORD dwRoleID, CHAR* szOrderno);

private:
	struct PAY_RECORD
	{
		//DWORD dwRoleId;
		CHAR szOrderno[64];
		INT nGold;
		INT bAdd;
	};

	typedef std::map<QString, PAY_RECORD> ORDER_RECORD_MAP;
	typedef std::map<DWORD, ORDER_RECORD_MAP> ROLE_PAY_MAP;

	ROLE_PAY_MAP		m_RolePayMap;
	QLock				m_PayRecordRetLock;

};

