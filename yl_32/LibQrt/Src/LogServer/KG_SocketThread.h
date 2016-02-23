
#pragma once

#include "KG_LogThread.h"
#include "KG_Queue.h"
#include <map>

struct KG_SOCKET_THREAD_PARAM
{
    char szListenIP[32];
    int  nListenPort;
    int  nAutoDisconnectTime; // in second
};

////////////////////////////////////////////////////////////////////////////////
class KG_SocketThread
{
public:
    KG_SocketThread(void);
    ~KG_SocketThread(void);

    int Init(
        const KG_SOCKET_THREAD_PARAM &crParam,
        KG_PACKAGE_LOCK_QUEUE        *pRequestQueue,
        KG_PACKAGE_LOCK_QUEUE        *pResponseQueue
    );
    int UnInit();
    
    int GetExitFlag()               { return m_nExitFlag;                   };
    int SetExitFlag(int nFlag)      { m_nExitFlag = nFlag; return true;     };
    int EnableReceive(int nEnable)  { m_nEnableReceiveFlag = nEnable; return true; };   

    unsigned GetConnectionCount() { return (unsigned)m_ConnectionMap.size(); };
    int GetStatus(std::string *psRetStatus);

private:
    static void _WorkThreadFunction(void *pvParam); 
    int _ThreadFunction();
    
    int _DisconnectIdelConnection();
    int _ProcessRequest();
    int _ProcessResponse();

    int _ProcessAcceptEvent(ISocketStream *piSocket);
    int _ProcessDataInEvent(unsigned uConnectionID);

private:
    struct KG_Connection
    {
        ISocketStream *piSocketStream;
        time_t  uLastRecvTime;
        char    szRemoteAddress[sizeof("255.255.255.255:65535")];
    }; 
    typedef std::map<unsigned, KG_Connection> KG_CONNECTION_MAP;

private:
    int      m_nExitFlag;
    int      m_nEnableReceiveFlag;
    QThread  m_WorkThread;

    QSocketServerAcceptor  m_SocketServer;
    QSOCKET_EVENT         *m_pEventArray;
    unsigned                 m_uEventArraySize;

    int                m_nAutoDisconnectTime;
    unsigned           m_uNextConnectionID;
    KG_CONNECTION_MAP  m_ConnectionMap;

    unsigned               m_uNextRequestID;
    KG_PACKAGE_LOCK_QUEUE *m_pRequestQueue;
    KG_PACKAGE_LOCK_QUEUE *m_pResponseQueue;
};


