
#pragma once

#ifdef WIN32
#else
    #include <netdb.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <string.h>
    #include <ctype.h>
    #include <time.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/time.h>
    #include <sys/socket.h>
    #include <sys/file.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
    #include <sys/types.h>
    #include <sys/param.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <errno.h>

	#include <time.h>
#if defined(__FreeBSD__) || defined(__APPLE__)
	#include <sys/event.h>
#endif
#ifdef __linux
	#include <sys/epoll.h>
#endif // __linux

#endif	//WIN32

#include <vector>

#include "Misc/MSUnknown.h"
#include "Core/QMemory.h"
#include "Core/QLock.h"


/************************************************************************/
/*     you should call WSAStartup(...) and WSACleanup(...) in win32     */                                                                     
/************************************************************************/

enum ENCODE_DECODE_MODE
{
    ENCODE_DECODE_NONE = -1,
    ENCODE_DECODE = 0,
    ENCODE_DECODE_EASY,
    ENCODE_DECODE_SIMPLE,
    ENCODE_DECODE_DYNAMIC
};

struct ISocketStream : IUnknown
{

    virtual int SetTimeout(const timeval *pTimeout)  = 0;

    // return -1: error, 0: timeout, 1: success
    virtual int CheckCanSend(const timeval *pTimeout) = 0;
    
    // return -1: error, 0: timeout, 1: success, don't send a package > 65500 bytes
    virtual int Send(IMemBlock *piBuffer) = 0;

    // return -1: error, 0: timeout, 1: success
    virtual int CheckCanRecv(const timeval *pTimeout) = 0;

    // return -2: again, -1: error, 0: timeout, 1: success
    virtual int Recv(IMemBlock **piRetBuffer)  = 0;
    
    virtual int IsAlive() = 0;
    
    virtual int GetRemoteAddress(struct in_addr *pRemoteIP, u_short *pusRemotePort) = 0;

    virtual int SetUserData(void *pvUserData)      = 0;

    virtual void *GetUserData() = 0;

	virtual	int GetSOCKET() = 0;
};


class QSocketConnector
{
public:
    QSocketConnector();

    virtual ISocketStream *Connect(
        const char cszIPAddress[],     int nPort
    );

    virtual ISocketStream *ProxyConnect(
        const char cszProxyAddress[],  int nProxyPort, 
        const char cszProxyUsername[], const char cszProxyPassword[], 
        const char cszIPAddress[],     int nPort
    );

    virtual ISocketStream *ConnectSecurity(
        const char cszIPAddress[],     int nPort, 
        ENCODE_DECODE_MODE EncodeDecodeMode
    );

    virtual ISocketStream *ProxyConnectSecurity(
        const char cszProxyAddress[],  int nProxyPort, 
        const char cszProxyUsername[], const char cszProxyPassword[], 
        const char cszIPAddress[],     int nPort, 
        ENCODE_DECODE_MODE EncodeDecodeMode
    );

    // Bind a local IP to use one Network Interface Card. 
    // if cszLocalIPAddress is NULL(or "") then is bind in INADDR_ANY address.
    // Bind must call before connect, or it takes no effect. 
    // If you never call Bind, it means bind all. 
    // Binding to a specific port number other than port 0 is discouraged for client applications, 
    // since there is a danger of conflicting with another socket already using that port number.
    virtual int Bind(
        const char cszLocalIPAddress[], int nPort = 0
    );

protected:
    struct sockaddr_in m_BindLoaclAddr;
};

class QBufferedSocketConnector : public QSocketConnector
{
public:
    virtual ISocketStream *Connect(
        const char cszIPAddress[],     int nPort
    );

    virtual ISocketStream *ProxyConnect(
        const char cszProxyAddress[],  int nProxyPort, 
        const char cszProxyUsername[], const char cszProxyPassword[], 
        const char cszIPAddress[],     int nPort
    );
};

class QSocketAcceptor
{
public:
    QSocketAcceptor();
    ~QSocketAcceptor();

    // cszIPAddress: local ip. if cszIPAddress is NULL(or "") then is bind in INADDR_ANY address
    virtual int Open(const char cszIPAddress[], int nPort);    //when succeed, return true,otherwise return false;
                                                      //ulAddress default is INADDR_ANY
    virtual int SetTimeout(const timeval *pTimeout);

    virtual ISocketStream *Accept();

    virtual ISocketStream *AcceptSecurity(ENCODE_DECODE_MODE EncodeDecodeMode);

    virtual int Close();

private:
    int     m_nListenSocket;
    timeval m_Timeout;

	QLock m_AcceptLock;
};


#ifdef EPOLLIN
#define QSOCKET_EVENT_IN      EPOLLIN
#else
#define QSOCKET_EVENT_IN      0x001
#endif

#define QSOCKET_EVENT_ACCEPT  (1 << 23)   

struct QSOCKET_EVENT
{
    unsigned          uEventFlag;
    ISocketStream *piSocket;
};


class ISocketServerAcceptor
{
public:
    // cszIPAddress: local ip. if cszIPAddress is NULL(or "") then is bind in INADDR_ANY address
    virtual int Init(
        const char cszIPAddress[], int nPort,
        int nMaxAcceptEachWait,
        int nMaxRecvBufSizePerSocket,
        int nMaxSendBufSizePerSocket,
        ENCODE_DECODE_MODE EncodeDecodeMode, 
        void *pvContext
    ) = 0;    //when succeed, return true,otherwise return false;
                                                      
    //when succeed, return true,otherwise return false;
    virtual int UnInit(void *pvContext) = 0;    

    //when succeed, return true,otherwise return false;
    virtual int Wait(int nEventCount, QSOCKET_EVENT *pEvent, int *pnRetEventCount) = 0;
};


#if defined(WIN32)
typedef std::vector<ISocketStream *>    QSOCKET_VECTOR;
#elif defined(__FreeBSD__) || defined(__APPLE__)
typedef std::vector<struct kevent>  QSOCKET_EVENT_VECTOR;
#elif defined(__linux)
// linux
typedef std::vector<struct epoll_event>  QSOCKET_EVENT_VECTOR;
#endif

class QSocketServerAcceptor : public ISocketServerAcceptor
{
public:
    virtual int Init(
        const char cszIPAddress[], int nPort,
        int nMaxAcceptEachWait,
        int nMaxRecvBufSizePerSocket,
        int nMaxSendBufSizePerSocket,
        ENCODE_DECODE_MODE EncodeDecodeMode, 
        void *pvContext
    );

    virtual int UnInit(void *pvContext);

    virtual int Wait(int nEventCount, QSOCKET_EVENT *pEvent, int *pnRetEventCount);

private:
    int             _WaitProcessAccept(int nEventCount, QSOCKET_EVENT *pEvent, int *pnRetEventCount);
    #ifdef WIN32
    int             _WaitProcessRecvOrDestory(int nEventCount, QSOCKET_EVENT *pEvent, int *pnRetEventCount);
    int             _ProcessSocketVectorDestory();
    #else
    // linux
    int             _WaitProcessRecv(int nEventCount, QSOCKET_EVENT *pEvent, int *pnRetEventCount);
    #endif


private:
    int                     m_nMaxAcceptEachWait;
    int                     m_nMaxRecvBufSizePerSocket;
    int                     m_nMaxSendBufSizePerSocket;

    int                     m_nListenSocket;
    ENCODE_DECODE_MODE      m_EncodeDecodeMode;

    #ifdef WIN32
    int                     m_nLastWaitPos;
    QSOCKET_VECTOR        	m_SocketVector;
    #else
    // linux
    int                     m_nEpollHandle;
    QSOCKET_EVENT_VECTOR 	m_SocketEventVector;
    #endif
};

