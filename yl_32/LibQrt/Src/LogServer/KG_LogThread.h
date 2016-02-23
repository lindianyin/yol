////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG_LogThread.h
//  Version     : 1.0
//  Creator     : Wu Caizhong
//  Create Date : 2008-6-12 22:04:11
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG_LOGTHREAD_H_
#define _INCLUDE_KG_LOGTHREAD_H_

#include <string>
#include <map>
#include "KG_Queue.h"
#include "KG_Database.h"
#include "KG_MicroTimer.h"
#include "Protocol/KG_CSLogServerProtocal.h"

////////////////////////////////////////////////////////////////////////////////
struct KG_LOG_THREAD_PARAM 
{
    unsigned uMinItemValue;
    unsigned uMinPlayerActionLevel;
	unsigned uMinDataAnalysisLevel;
    int      nExpiredDay;
    KG_DATABASE_PARAM DatabaseParam;
};

////////////////////////////////////////////////////////////////////////////////
class KG_LogThread
{
	typedef std::string string;;
public:
    KG_LogThread(void);
    ~KG_LogThread(void);

    int Init(
        const KG_LOG_THREAD_PARAM &crParam,
        KG_PACKAGE_LOCK_QUEUE     *pRequestQueue,
        KG_PACKAGE_LOCK_QUEUE     *pResponseQueue
    );
    int UnInit();

    int GetStatus(std::string *psRetStatus);

    int IsActive()              { return m_nActiveFlag;             };
    int GetExitFlag()           { return m_nExitFlag;               };
    int SetExitFlag(int nFlag)  { m_nExitFlag = nFlag; return true; };

private:
    typedef std::map<unsigned, std::string> KG_GATEWAY_INFO_MAP;

    struct KG_ROLE_INDEX 
    {
        string sRoleName;
        string sAccountName;
        string sGatewayName;
    };

    class _KG_ROLE_INDEX_StructCompare
    {
    public:
        bool operator()(
            const KG_ROLE_INDEX &crLeft,
            const KG_ROLE_INDEX &crRight
        ) const
        {
            if (crLeft.sRoleName != crRight.sRoleName)
                return crLeft.sRoleName < crRight.sRoleName;
            else if (crLeft.sAccountName != crRight.sAccountName)
                return crLeft.sAccountName < crRight.sAccountName;
            else
                return crLeft.sGatewayName < crRight.sGatewayName;
        }
    };

    typedef std::map<KG_ROLE_INDEX, unsigned, _KG_ROLE_INDEX_StructCompare> KG_ROLE_INDEX_MAP;

private:
    static void _WorkThreadFunction(void *pvParam); 
    int _ThreadFunction();

    int _ProcessRequest();

    int _Connect();
    int _DisConnect();

    int _CreateLogDatabase();

    int _SwitchTable();

    int _ClearExpiredData(time_t tmNow);

    int _GetRoleIndexID(
        const char cszRoleName[],       unsigned uRoleNameSize,
        const char cszAccountName[],    unsigned uAccountNameSize,
        string *psGatewayName,
        unsigned *puRetRoleIndexID
    );

private:
    typedef int (KG_LogThread::*PROTOCOL_PROCESS_FUNCTION)(
        string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse
    ); 

    // Protocol just use 1 byte, so we define a 256 array for each protocol
    PROTOCOL_PROCESS_FUNCTION m_aProtocolProcessFunction[256]; 

    int _SetResponsePacket(
        PROTOCOL_MSG_TYPE cProtocol, 
        unsigned long ulIdentity, 
        char chRetResult,
        IMemBlock **ppiRetResponse
    );

    // Protocol Process
    int _ping(string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse);
    int _login(string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse);
    int _log_item_operation(string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse);
    int _log_player_action(string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse);
	int _log_data_analysis(string *psGatewayName, KG_PACKAGE *pPackage, IMemBlock **ppiRetResponse);

private:
    int m_nExitFlag;
    int m_nActiveFlag;
    int m_nRequestPerSecond;
    KG_LOG_THREAD_PARAM m_Param;

    KG_PACKAGE_LOCK_QUEUE *m_pRequestQueue;
    KG_PACKAGE_LOCK_QUEUE *m_pResponseQueue;

    QThread             m_WorkThread;
    KG_GATEWAY_INFO_MAP m_GatewayMap;
    KG_ROLE_INDEX_MAP   m_RoleIndexMap;
    char m_szLogItemTableName[KG_NAME_MAX_SIZE];
    char m_szLogActionTableName[KG_NAME_MAX_SIZE];
	char m_szLogDataAnalysisTableName[KG_NAME_MAX_SIZE];


    MYSQL *m_pMysqlConn;

    KG_MicroTimer m_MicroTimer;
    double        m_fRequestConsumeTime;
    struct tm     m_tmLastSwitchTable;
	INT m_nFailureCount;
};


#endif //_INCLUDE_KG_LOGTHREAD_H_
