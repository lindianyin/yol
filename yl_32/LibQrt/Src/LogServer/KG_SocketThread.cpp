////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG_SocketThread.cpp
//  Version     : 1.0
//  Creator     : Wu Caizhong
//  Create Date : 2008-6-17 12:00:32
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG_SocketThread.h"
#include "Protocol/KG_CSLogServerProtocal.h"

const unsigned MAX_ACCEPT_EACH_WAIT = 8;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
KG_SocketThread::KG_SocketThread(void) :
    m_nExitFlag(false),
    m_nEnableReceiveFlag(true),
    m_pEventArray(NULL),
    m_uEventArraySize(0),
    m_uNextConnectionID(1),
    m_nAutoDisconnectTime(120),
    m_uNextRequestID(1),
    m_pRequestQueue(NULL),
    m_pResponseQueue(NULL)
{
}

KG_SocketThread::~KG_SocketThread(void)
{
    ASSERT(!m_pEventArray);
}

////////////////////////////////////////////////////////////////////////////////
int KG_SocketThread::Init(
    const KG_SOCKET_THREAD_PARAM &crParam,
    KG_PACKAGE_LOCK_QUEUE        *pRequestQueue,
    KG_PACKAGE_LOCK_QUEUE        *pResponseQueue
)
{
    int nResult  = false;
    int nRetCode = false;
    int nSocketServerInitFlag = false;
    int nWorkThreadInitFlag   = false;

    LOG_PROCESS_ERROR(pRequestQueue);
    LOG_PROCESS_ERROR(pResponseQueue);

    m_nAutoDisconnectTime = crParam.nAutoDisconnectTime;
    m_pRequestQueue       = pRequestQueue;
    m_pResponseQueue      = pResponseQueue;

    m_uEventArraySize = MAX_ACCEPT_EACH_WAIT * 2;
    m_pEventArray = new(nothrow) QSOCKET_EVENT[m_uEventArraySize];
    LOG_PROCESS_ERROR(m_pEventArray);

    nRetCode = m_SocketServer.Init(
        crParam.szListenIP,  crParam.nListenPort,
        MAX_ACCEPT_EACH_WAIT, 
        KG_LOG_SERVER_MAX_PACKET_SIZE,  KG_LOG_SERVER_MAX_PACKET_SIZE,
        ENCODE_DECODE_NONE,
        NULL
    );
    LOG_PROCESS_ERROR(nRetCode && "m_SocketServer->Init");
    nSocketServerInitFlag = true;

    QLogPrintf(
        LOG_INFO, 
        "start listen on %s:%d\n", 
        crParam.szListenIP,  crParam.nListenPort
    );

    nRetCode = m_WorkThread.Create(_WorkThreadFunction, this);
    LOG_PROCESS_ERROR(nRetCode);
    nWorkThreadInitFlag = true;

    nResult = true;
EXIT0:
    if (!nResult)
    {
        if (nWorkThreadInitFlag)
        {
            m_WorkThread.Wait();
            nWorkThreadInitFlag = false;
        }
        if (nSocketServerInitFlag)
        {
            m_SocketServer.UnInit(NULL);
            nSocketServerInitFlag = false;
        }
        SAFE_DELETE_ARRAY(m_pEventArray);
    }
   
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_SocketThread::UnInit()
{
    int nResult  = false;
    int nRetCode = false;

    SetExitFlag(true);

    m_WorkThread.Wait();
    m_SocketServer.UnInit(NULL);
    SAFE_DELETE_ARRAY(m_pEventArray);

    nResult = true;
//EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
void KG_SocketThread::_WorkThreadFunction(void *pvParam)
{
    KG_SocketThread *pThis = (KG_SocketThread *)pvParam;

    ASSERT(pThis);

    pThis->_ThreadFunction();
}

////////////////////////////////////////////////////////////////////////////////
int KG_SocketThread::_ThreadFunction()
{
    int nResult  = false;
    int nRetCode = false;
    unsigned long ulLastResetTime = 0;
    unsigned long ulCurrentTime   = 0;
    KG_CONNECTION_MAP::iterator it;

    while (!m_nExitFlag)
    {
        nRetCode = _DisconnectIdelConnection();
        //LOG_PROCESS_ERROR(nRetCode);

        if (m_nEnableReceiveFlag)
        {
            nRetCode = _ProcessRequest();
            //LOG_PROCESS_ERROR(nRetCode); 
        }

        nRetCode = _ProcessResponse();
        //LOG_PROCESS_ERROR(nRetCode);

        // sleep some time, if not, will cause a high CPU usage. 
        QThread_Sleep(10);
    }

    nResult = true;
//EXIT0:
    for (it = m_ConnectionMap.begin(); it != m_ConnectionMap.end(); ++it)
    {
        QLogPrintf(
            LOG_ERR, 
            "[%u] (%s) force disconnect!", it->first, it->second.szRemoteAddress
        );
        SAFE_RELEASE(it->second.piSocketStream);
    }
    m_ConnectionMap.clear();

    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_SocketThread::_DisconnectIdelConnection()
{
    int nResult  = false;
    int nRetCode = false;
    time_t uCurrentTime = 0;
    int    nIdelTime    = 0;
    KG_CONNECTION_MAP::iterator it;
    KG_Connection *pConnection = NULL;

    uCurrentTime = time(NULL);

    it = m_ConnectionMap.begin(); 
    while (it != m_ConnectionMap.end())
    {
        pConnection = &(it->second);

        // uCurrent may less than pConnection->uLastRecvTime
        // cause by system time synchronism
        nIdelTime = (int)(uCurrentTime - pConnection->uLastRecvTime);

        if (nIdelTime > m_nAutoDisconnectTime)
        {
            QLogPrintf(
                LOG_ERR, 
                "[%u] (%s) There are no data arrive during %u second, auto disconnect!", 
                it->first, pConnection->szRemoteAddress, nIdelTime
            );
            SAFE_RELEASE(pConnection->piSocketStream);

            m_ConnectionMap.erase(it++);
        }
        else
        {
            ++it;
        }
    }

    nResult = true;
// EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_SocketThread::_ProcessRequest()
{
    int nResult  = false;
    int nRetCode = false;
    unsigned uConnectionID = 0;
    unsigned uConnectionCount = 0;
    unsigned uNewEventArraySize = 0;
    QSOCKET_EVENT *pNewEventArray = NULL;
    QSOCKET_EVENT *pEvent = NULL;
    QSOCKET_EVENT *pEventEnd = NULL;
    int nEventCount = 0;
    int nQueueLockFlag = false;

    ASSERT(m_pRequestQueue);

    while (true)
    {
        // make sure each connection can get a event object.
        uConnectionCount = (unsigned)m_ConnectionMap.size();
        if (m_uEventArraySize < (uConnectionCount + MAX_ACCEPT_EACH_WAIT))
        {
            uNewEventArraySize = uConnectionCount + MAX_ACCEPT_EACH_WAIT * 2;
            pNewEventArray = new(nothrow) QSOCKET_EVENT[uNewEventArraySize];
            LOG_PROCESS_ERROR(pNewEventArray);

            SAFE_DELETE_ARRAY(m_pEventArray);
            m_pEventArray = pNewEventArray;
            pNewEventArray = NULL;

            m_uEventArraySize = uNewEventArraySize;
        }

        nRetCode = m_SocketServer.Wait(
            m_uEventArraySize, m_pEventArray, &nEventCount
        );
        LOG_PROCESS_ERROR(nRetCode && "m_SocketServer.Wait");

        if (nEventCount == 0)
            break;  // If no event then break. 

        if (m_pRequestQueue->Queue.size() >= MAX_REQUEST_QUEUE_SIZE)
        {
            QLogPrintf(LOG_WARNING, "request queue is full!");
            break;
        }

        m_pRequestQueue->Lock.Lock();
        nQueueLockFlag = true;
        pEventEnd = m_pEventArray + nEventCount;
        for (pEvent = m_pEventArray; pEvent < pEventEnd; ++pEvent)
        {
            ASSERT(pEvent->piSocket);

            // QSOCKET_EVENT_ACCEPT event must be processed before any other event, 
            if (pEvent->uEventFlag & QSOCKET_EVENT_ACCEPT)
            {
                nRetCode = _ProcessAcceptEvent(pEvent->piSocket);
                //LOG_PROCESS_ERROR(nRetCode);
            }
            else if (pEvent->uEventFlag & QSOCKET_EVENT_IN)
            {
                uConnectionID = (unsigned)(ptrdiff_t)(pEvent->piSocket->GetUserData());
                nRetCode = _ProcessDataInEvent(uConnectionID);
                //LOG_PROCESS_ERROR(nRetCode);
            }

            SAFE_RELEASE(pEvent->piSocket);
        }
        m_pRequestQueue->Lock.Unlock();
        nQueueLockFlag = false;
	}

    nResult = true;
EXIT0:
    if (nQueueLockFlag)
    {
        m_pRequestQueue->Lock.Unlock();
        nQueueLockFlag = false;
    }
    SAFE_DELETE_ARRAY(pNewEventArray);
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_SocketThread::_ProcessResponse()
{
    int nResult  = false;
    int nRetCode = false;
    KG_PACKAGE *pResponse = NULL;
    KG_CONNECTION_MAP::iterator itConnection;
    KG_Connection *pConnection = NULL;
    int nQueueLockFlag = false;
  
    ASSERT(m_pResponseQueue);

    m_pResponseQueue->Lock.Lock();
    nQueueLockFlag = true;

    while (!m_pResponseQueue->Queue.empty())
    {
        pResponse = &(m_pResponseQueue->Queue.front());

        itConnection = m_ConnectionMap.find(pResponse->uConnectionID);
        if (itConnection != m_ConnectionMap.end())
        {
            pConnection = &(itConnection->second);
            nRetCode = pConnection->piSocketStream->Send(pResponse->piDataBuffer);
            if (nRetCode != 1)
            {
                QLogPrintf(
                    LOG_ERR, 
                    "[%u] (%s) Send data error\n", 
                    itConnection->first, pConnection->szRemoteAddress
                );
            }
        }

        SAFE_RELEASE(pResponse->piDataBuffer);
        m_pResponseQueue->Queue.pop_front();
    }

    nResult = true;
//EXIT0:
    if (nQueueLockFlag)
    {
        m_pResponseQueue->Lock.Unlock();
        nQueueLockFlag = false;
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_SocketThread::_ProcessAcceptEvent(ISocketStream *piSocket)
{
    int nResult  = false;
    int nRetCode = false;
    std::pair<KG_CONNECTION_MAP::iterator, bool> pairInsertResult;
    KG_Connection Connection;
    struct in_addr RemoteIP;
    u_short usRemotePort = 0;
    const char *pcszIP = NULL;

    LOG_PROCESS_ERROR(piSocket);

    nRetCode = piSocket->GetRemoteAddress(&RemoteIP, &usRemotePort);
    ASSERT(nRetCode);

    nRetCode = piSocket->SetUserData((void *)(ptrdiff_t)m_uNextConnectionID);
    ASSERT(nRetCode);

    pcszIP = inet_ntoa(RemoteIP);
    nRetCode = snprintf(
        Connection.szRemoteAddress,
        sizeof(Connection.szRemoteAddress) - 1,
        "%s:%u",
        pcszIP, usRemotePort
    );
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(Connection.szRemoteAddress) - 1));
    Connection.szRemoteAddress[sizeof(Connection.szRemoteAddress) - 1] = '\0';

    Connection.piSocketStream = piSocket;
    Connection.piSocketStream->AddRef();

    Connection.uLastRecvTime = time(NULL);

    pairInsertResult = m_ConnectionMap.insert(
        make_pair(m_uNextConnectionID, Connection)
    );
    LOG_PROCESS_ERROR(pairInsertResult.second);

    ++m_uNextConnectionID;

    QLogPrintf(
        LOG_INFO, "[%u] (%s) Connected", m_uNextConnectionID - 1, Connection.szRemoteAddress
    );

    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_SocketThread::_ProcessDataInEvent(unsigned uConnectionID)
{
    int nResult  = false;
    int nRetCode = false;
    KG_Connection *pConnction = NULL;
    KG_CONNECTION_MAP::iterator itConnection;
    KG_PACKAGE Request;
    IMemBlock *piDataBuffer = NULL;

    LOG_PROCESS_ERROR(m_pRequestQueue);

    itConnection = m_ConnectionMap.find(uConnectionID);
    LOG_PROCESS_ERROR(itConnection != m_ConnectionMap.end());
    pConnction = &(itConnection->second);

    while (true)
    {
        SAFE_RELEASE(piDataBuffer);
        nRetCode = pConnction->piSocketStream->Recv(&piDataBuffer);
        PROCESS_SUCCESS(nRetCode == -2);

        if (nRetCode == -1)
        {
            QLogPrintf(
                LOG_ERR, 
                "[%u] (%s) Recv data error, disconnect it.", 
                itConnection->first, pConnction->szRemoteAddress
            );
            SAFE_RELEASE(pConnction->piSocketStream);

            m_ConnectionMap.erase(itConnection);

            break; 
        }

        pConnction->uLastRecvTime = time(NULL);

        Request.uConnectionID = uConnectionID;
        ASSERT(piDataBuffer);
        Request.piDataBuffer = piDataBuffer;
        Request.piDataBuffer->AddRef();

        m_pRequestQueue->Queue.push_back(Request); // should lock in the caller function
    }

EXIT1:
    nResult = true;
EXIT0:
    SAFE_RELEASE(piDataBuffer);
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_SocketThread::GetStatus(std::string *psRetStatus)
{
    int nResult  = false;
    int nRetCode = false;
    char szBuffer[256];

    LOG_PROCESS_ERROR(psRetStatus);

    nRetCode = snprintf(
        szBuffer,
        sizeof(szBuffer) - 1,
        "Total Connections  : %d\n"
        "RequestQueue Size  : %u\n"
        "ResponseQueue Size : %u\n",
        (unsigned)m_ConnectionMap.size(),
        (unsigned)m_pRequestQueue->Queue.size(),
        (unsigned)m_pResponseQueue->Queue.size()
    );
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szBuffer) - 1));
    szBuffer[sizeof(szBuffer) - 1] = '\0';

    *psRetStatus = szBuffer;
    nResult = true;
EXIT0:
    return nResult;
}
