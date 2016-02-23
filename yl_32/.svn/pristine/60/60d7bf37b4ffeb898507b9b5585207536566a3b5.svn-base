
#include "CorePrivate.h"

#include <errno.h>
#include <string>

#ifdef __GNUC__
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#else
#define socklen_t int
#endif

#include "Misc/MSUnknown.h"
#include "Network/SocketCryptKey.h"
#include "Network/QSocket.h"
#include "Network/QSocketPack.h"
#include "Core/EncodeDecode.h"
#include "Crypto/easycrypt.h"
//#include "Base64.h"
#include "Core/QLock.h"
#include "Core/QLog.h"
#include "Core/QThread.h"


//#define USE_SOCKET_INFO_LOGGER



#if (defined(_MSC_VER) || defined(__ICL))   //WIN32
class CWinSockInit
{
public:
    CWinSockInit()
    {
        WORD    wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        int     nRetCode = 0;
        nRetCode = WSAStartup(wVersionRequested, &wsaData);
        ASSERT((!nRetCode) && "WSAStartup failed!");
    }
    ~CWinSockInit()
    {
        WSACleanup();
    }
} g_WinSockInitor;
#endif

class QSocketStream : public ISocketStream
{
public:

    QSocketStream(int nSocket, struct sockaddr_in *pRemoteAddr);

    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject
        ) { return -1; };
    virtual ULONG STDMETHODCALLTYPE AddRef(void)    {  return InterlockedIncrement((long *)&m_ulRefCount); };
    virtual ULONG STDMETHODCALLTYPE Release(void);

    virtual int SetTimeout(const timeval *pTimeout);

    virtual int CheckCanSend(const timeval *pTimeout);
    
    virtual int Send(IMemBlock *piBuffer);

    virtual int CheckCanRecv(const timeval *pTimeout);

    virtual int Recv(IMemBlock **ppiRetBuffer);

    virtual int IsAlive();

    virtual int GetRemoteAddress(struct in_addr *pRemoteIP, u_short *pusRemotePort);

    virtual int SetUserData(void *pvUserData) { m_pvUserData = pvUserData; return true; }

    virtual void *GetUserData() { return m_pvUserData; }

	virtual	int GetSOCKET() { return m_nSocketHandle; }
protected:
    volatile unsigned long  m_ulRefCount;
    int                 m_nSocketHandle;
    timeval             m_Timeout;
    struct sockaddr_in  m_RemoteAddress;
    void               *m_pvUserData;
};

class QBufferedSocketStream : public QSocketStream
{
public:
    QBufferedSocketStream(int nSocket, struct sockaddr_in *pRemoteAddr);

    virtual int Recv(IMemBlock **ppiRetBuffer);

protected:
    unsigned            m_uRecvTailPos;
    unsigned char       m_byRecvBuffer[USHRT_MAX];
};

typedef int SOCKET_STREAM_ENCODE_DECODE_FUNCTION(size_t uSize, unsigned char *pbyBuf, unsigned *puKey);

class QSecuritySocketStream : public ISocketStream
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject
        ) { return -1; };
    virtual ULONG STDMETHODCALLTYPE AddRef(void)    {  return InterlockedIncrement((long *)&m_ulRefCount); };
    virtual ULONG STDMETHODCALLTYPE Release(void);
    
    QSecuritySocketStream(
        ISocketStream *pSocketStream, 
        unsigned uSendKey, 
        unsigned uRecvKey, 
        ENCODE_DECODE_MODE EncodeDecodeMode
    );
    virtual int SetTimeout(const timeval *pTimeout)   {  if (m_piSocketStream) return m_piSocketStream->SetTimeout(pTimeout);  return false;}

    virtual int CheckCanSend(const timeval *pTimeout) {  if (m_piSocketStream) return m_piSocketStream->CheckCanSend(pTimeout); return -1;  }
    
    virtual int Send(IMemBlock *piBuffer);

    virtual int CheckCanRecv(const timeval *pTimeout) {  if (m_piSocketStream) return m_piSocketStream->CheckCanRecv(pTimeout); return -1;  }

    virtual int Recv(IMemBlock **ppiRetBuffer);
    
    virtual int IsAlive(){if (m_piSocketStream) return m_piSocketStream->IsAlive(); return false;}

    virtual int GetRemoteAddress(struct in_addr *pRemoteIP, u_short *pusRemotePort) 
    {
        if (m_piSocketStream) 
            return m_piSocketStream->GetRemoteAddress(pRemoteIP, pusRemotePort); 
        return false;
    }

    virtual int SetUserData(void *pvUserData)
    {
        if (m_piSocketStream) 
            return m_piSocketStream->SetUserData(pvUserData); 
        return false;
    }

    virtual void *GetUserData()
    {
        if (m_piSocketStream) 
            return m_piSocketStream->GetUserData(); 
        return NULL;
    }

	virtual	int GetSOCKET()
	{
		if (m_piSocketStream) 
			return m_piSocketStream->GetSOCKET(); 
		return 0;
	}

private:
    volatile unsigned long  m_ulRefCount;

    ISocketStream *m_piSocketStream;
    unsigned          m_uSendKey;
    unsigned          m_uRecvKey;
    SOCKET_STREAM_ENCODE_DECODE_FUNCTION *m_pfnEncodeFunction;
    SOCKET_STREAM_ENCODE_DECODE_FUNCTION *m_pfnDecodeFunction;
};

#ifdef USE_SOCKET_INFO_LOGGER
class _QSocketInfoLogger
{
public:
	_QSocketInfoLogger() : m_nConnectCount(0), m_nDisConnectCount(0) {};

	int  Init();
	void UnInit();

	void OnConnect(ISocketStream *piStream);
	void OnDisConnect(ISocketStream *piStream);

private:
	int   m_nConnectCount;
	int   m_nDisConnectCount;
};

_QSocketInfoLogger g_InfoLogger;
#endif

/******************************** inline functions *******************************************************/


static inline unsigned _Rand()
{
    static unsigned s_holdrand = (unsigned)time(NULL);
    
    s_holdrand = s_holdrand * 214013L + 2531011L;

    return s_holdrand;
}


#if (defined(_MSC_VER) || defined(__ICL))

int _SetSocketNoBlock(int nSocket)
{
    int nResult  = false;
    int nRetCode = 0;
    unsigned long ulOption = 1;

    nRetCode = ioctlsocket(nSocket, FIONBIO, (unsigned long *)&ulOption);
    PROCESS_ERROR(nRetCode == 0);

    nResult = true;
EXIT0:
    return nResult;
}

static inline int _IsSocketCanRestore()
{
	return 0;
    //return (WSAGetLastError() == EINTR);
}

static inline int _GetSocketErrorCode()
{
    return WSAGetLastError();
}


static inline int _IsSocketWouldBlock()
{
    return (
        (WSAEWOULDBLOCK == WSAGetLastError()) ||
        (WSA_IO_PENDING == WSAGetLastError())
    );
}

static inline int _CloseSocket(int nSocket)
{
    struct linger lingerStruct;

    lingerStruct.l_onoff  = 1;
    lingerStruct.l_linger = 0;
		
    setsockopt(
	    nSocket,
	    SOL_SOCKET, SO_LINGER, 
	    (char *)&lingerStruct, sizeof(lingerStruct)
	);
    
    return closesocket(nSocket);
}

// return -1: error, 0: timeout, 1: success
static inline int _CheckCanRecv(int nSocket, const timeval *pcTimeout)
{
    if (nSocket < 0)
        return -1;

    fd_set FDSet;
    FD_ZERO(&FDSet);
    FD_SET(nSocket, &FDSet);

    timeval  TempTimeout;
    timeval *pTempTimeout = NULL;

    if (pcTimeout)
    {
        TempTimeout  = *pcTimeout;
        pTempTimeout = &TempTimeout;
    }

    int nRetCode = select(nSocket + 1, &FDSet, NULL, NULL, pTempTimeout);

    if (nRetCode == 0)
        return 0;

    if (nRetCode > 0)
        return 1;
        
    nRetCode = _GetSocketErrorCode();
    QLogPrintf(
        LOG_DEBUG,
        "nSocket = %d, _CheckCanRecv(), Error = %d\n",
        nSocket, 
        nRetCode
    );

    return -1;
}

// return -1: error, 0: timeout, 1: success
static inline int _CheckCanSend(int nSocket, const timeval *pcTimeout)
{
    if (nSocket < 0)
        return -1;

    fd_set FDSet;
    FD_ZERO(&FDSet); 
    FD_SET(nSocket, &FDSet);

    timeval  TempTimeout;
    timeval *pTempTimeout = NULL;

    if (pcTimeout)
    {
        TempTimeout  = *pcTimeout;
        pTempTimeout = &TempTimeout;
    }
    
    int nRetCode = select(nSocket + 1, NULL, &FDSet, NULL, pTempTimeout);

    if (nRetCode == 0)
        return 0;
    
    if (nRetCode > 0)
        return 1;
        
    nRetCode = _GetSocketErrorCode();
    QLogPrintf(
        LOG_DEBUG,
        "nSocket = %d, _CheckCanSend(), Error = %d\n",
        nSocket, 
        nRetCode
    );
        

    return -1;        
}


#elif defined(__GNUC__) //linux

int _SetSocketNoBlock(int nSocket)
{
    int nResult  = false;
    int nRetCode = 0;
    int nOption  = 0;

    nOption = fcntl(nSocket, F_GETFL, 0);
    nRetCode = fcntl(nSocket, F_SETFL, nOption | O_NONBLOCK);
    PROCESS_ERROR(nRetCode == 0);

    nResult = true;
EXIT0:
    return nResult;
}

static inline int _IsSocketCanRestore()
{
	return 0;
    //return (errno == EINTR);
}

static inline int _GetSocketErrorCode()
{
    return errno;
}

static inline int _IsSocketWouldBlock()
{
    return (EAGAIN == errno || EWOULDBLOCK == errno);
}

static inline int _CloseSocket(int nSocket)
{
    struct linger lingerStruct;

    lingerStruct.l_onoff = 1;
    lingerStruct.l_linger = 0;
		
    setsockopt(
	    nSocket,
	    SOL_SOCKET, SO_LINGER, 
	    (char *)&lingerStruct, sizeof(lingerStruct)
	);

    return close(nSocket);
}

// return -1: error, 0: timeout, 1: success
static inline int _CheckCanRecv(int nSocket, const timeval *pcTimeout)
{
    if (nSocket < 0)
        return -1;

    int nTimeout = -1;
    if (pcTimeout)
    {
        nTimeout = (pcTimeout->tv_sec * 1000) + (pcTimeout->tv_usec / 1000);
    }

    struct pollfd PollFD;
    PollFD.fd = nSocket;
    PollFD.events  = POLLIN;
    PollFD.revents = 0;

    int nRetCode = poll(&PollFD, 1, nTimeout);

    if (nRetCode == 0)
        return 0;
    
    if (nRetCode > 0)
    {
        ASSERT(
            (PollFD.revents & POLLIN ) ||
            (PollFD.revents & POLLERR) ||
            (PollFD.revents & POLLHUP)
        );
        return 1;
    }
    
    nRetCode = _GetSocketErrorCode();
    QLogPrintf(
        LOG_DEBUG,
        "nSocket = %d, _CheckCanRecv(), Error = %d\n",
        nSocket, 
        nRetCode
    );
    

    return -1;        
}

// return -1: error, 0: timeout, 1: success
static inline int _CheckCanSend(int nSocket, const timeval *pcTimeout)
{
    if (nSocket < 0)
        return -1;

    int nTimeout = -1;
    if (pcTimeout)
    {
        nTimeout = (pcTimeout->tv_sec * 1000) + (pcTimeout->tv_usec / 1000);
    }

    struct pollfd PollFD;
    PollFD.fd = nSocket;
    PollFD.events  = POLLOUT;
    PollFD.revents = 0;

    int nRetCode = poll(&PollFD, 1, nTimeout);

    if (nRetCode == 0)
        return 0;
    
    if (nRetCode > 0)
    {
        ASSERT(
            (PollFD.revents & POLLOUT) ||
            (PollFD.revents & POLLERR) ||
            (PollFD.revents & POLLHUP)
        );
        return 1;
    }

    nRetCode = _GetSocketErrorCode();
    QLogPrintf(
        LOG_DEBUG,
        "nSocket = %d, _CheckCanSend(), Error = %d\n",
        nSocket, 
        nRetCode
    );

    return -1;        
}

#endif


// if nSendFlag == false, then RecvBuffer
// if nSendFlag == true,  then SendBuffer   00002746 
// return -1: error, 0: timeout, 1: success
static inline int _SendOrRecvBuffer(
    const int nSendFlag,
    int nSocket, 
    unsigned char *pbyBuffer, int nSize, 
    const timeval &ProcessTimeout
)
{
    int nResult  = -1;      // error
    int nRetCode = false;

    const timeval *pcTimeout = NULL;
    if (ProcessTimeout.tv_sec != -1)
        pcTimeout = &ProcessTimeout;

    PROCESS_ERROR(nSocket != -1);
    PROCESS_ERROR(pbyBuffer);
    PROCESS_ERROR(nSize > 0);

    while (nSize > 0)
    {
        if (nSendFlag)
        {
            //when timeout disabled,pTimeout was set to NULL.
            nRetCode = _CheckCanSend(nSocket, pcTimeout);
            PROCESS_ERROR_RET_CODE(nRetCode != 0, nRetCode); // if timeout
            if (nRetCode < 0)  //error
            {
                nRetCode = _IsSocketCanRestore();
                if (nRetCode)   // if can restore then continue
                    continue;

                goto EXIT0;
            }
            nRetCode = send(nSocket, (char *)pbyBuffer, nSize, 0);
        }
        else    // recv
        {
            //when timeout disabled,pTimeout was set to NULL.
            nRetCode = _CheckCanRecv(nSocket, pcTimeout);
            PROCESS_ERROR_RET_CODE(nRetCode != 0, nRetCode); // if timeout
            if (nRetCode < 0)   // error
            {
                nRetCode = _IsSocketCanRestore();
                if (nRetCode)   // if can restore then continue
                    continue;

                goto EXIT0;
            }
            nRetCode = recv(nSocket, (char *)pbyBuffer, nSize, 0);
        }

        if (nRetCode == 0)  // Disconnected
            goto EXIT0;

        if (nRetCode < 0)   // Error!
        {
            nRetCode = _IsSocketCanRestore();
            if (nRetCode)   // if can restore then continue
                continue;
                
            nRetCode = _GetSocketErrorCode();
            QLogPrintf(
                LOG_DEBUG,
                "nSocket = %d, nSendFlag = %d, Error = %d\n",
                nSocket, 
                nSendFlag, 
                nRetCode
            );
            goto EXIT0;
        }

        pbyBuffer   += nRetCode;
        nSize       -= nRetCode;
    }

    nResult = 1;    // success
EXIT0:
    return nResult;   
}

int _SetEncodeDecodeFunction(
    ENCODE_DECODE_MODE EncodeDecodeMode, 
    SOCKET_STREAM_ENCODE_DECODE_FUNCTION  **ppfnRetEncodeFunction, 
    SOCKET_STREAM_ENCODE_DECODE_FUNCTION  **ppfnRetDecodeFunction
)
{
    int nResult = false;
    SOCKET_STREAM_ENCODE_DECODE_FUNCTION *pfnEncodeFunction = NULL;
    SOCKET_STREAM_ENCODE_DECODE_FUNCTION *pfnDecodeFunction = NULL;

    PROCESS_ERROR(ppfnRetEncodeFunction);
    PROCESS_ERROR(ppfnRetDecodeFunction);

    switch (EncodeDecodeMode)
    {
    case ENCODE_DECODE_NONE:
        pfnEncodeFunction = NULL;
        pfnDecodeFunction = NULL;
        break;
    case ENCODE_DECODE:
        pfnEncodeFunction = QEncodePSK_Fixed;
        pfnDecodeFunction = QDecodePSK_Fixed;
        break;
    case ENCODE_DECODE_EASY:
        pfnEncodeFunction = Encrypt;
        pfnDecodeFunction = Decrypt;
        break;
    case ENCODE_DECODE_SIMPLE:
        pfnEncodeFunction = QEncodeDynamic_FixedKey;
        pfnDecodeFunction = QDecodeDynamic_FixedKey;
        break;
    case ENCODE_DECODE_DYNAMIC:
        pfnEncodeFunction = QEncodeDynamic;
        pfnDecodeFunction = QDecodeDynamic;
        break;
    default:
        ASSERT(!"Unexpected encode/decode mode for security socket stream !");
        PROCESS_ERROR(false);
    }

//EXIT1:
    *ppfnRetEncodeFunction = pfnEncodeFunction;
    *ppfnRetDecodeFunction = pfnDecodeFunction;

    nResult = true;

EXIT0:
    return nResult;
}

int _MakeSecurityKey(ENCODE_DECODE_MODE EncodeDecodeMode, unsigned *puRetServerKey, unsigned *puRetClientKey, IMemBlock **ppiRetBuffer)
{
    int nResult  = false;
    int nRetCode = false;

    IMemBlock *piBuffer   = NULL;
    unsigned    uServerKey = 0;
    unsigned    uClientKey = 0;

    PROCESS_ERROR(puRetServerKey);
    PROCESS_ERROR(puRetClientKey);
    PROCESS_ERROR(ppiRetBuffer);

    switch (EncodeDecodeMode)
    {
    case ENCODE_DECODE:
    case ENCODE_DECODE_SIMPLE:
    case ENCODE_DECODE_DYNAMIC:
        {
            ACCOUNT_BEGIN *pACCOUNT_BEGIN   = NULL;

            piBuffer = QCreateMemBlock(sizeof(ACCOUNT_BEGIN));
            PROCESS_ERROR(piBuffer);

            pACCOUNT_BEGIN = (ACCOUNT_BEGIN *)piBuffer->GetData();
            PROCESS_ERROR(pACCOUNT_BEGIN);

            uServerKey = _Rand();
            uClientKey = _Rand();
            pACCOUNT_BEGIN->ProtocolType  = CIPHER_PROTOCOL_TYPE;
            pACCOUNT_BEGIN->Mode          = 0;
            pACCOUNT_BEGIN->ServerKey     = NE_MAKEKEY(uServerKey);
            pACCOUNT_BEGIN->ClientKey     = NE_MAKEKEY(uClientKey);
            break;
        }
    case ENCODE_DECODE_EASY:
        {
            EASYCRYPT_ACCOUNT_BEGIN *pEASYCRYPT_ACCOUNT_BEGIN = NULL;

            piBuffer = QCreateMemBlock(sizeof(EASYCRYPT_ACCOUNT_BEGIN));
            PROCESS_ERROR(piBuffer);

            pEASYCRYPT_ACCOUNT_BEGIN = (EASYCRYPT_ACCOUNT_BEGIN *)piBuffer->GetData();
            PROCESS_ERROR(pEASYCRYPT_ACCOUNT_BEGIN);

            uServerKey = _Rand();
            uClientKey = _Rand();
            pEASYCRYPT_ACCOUNT_BEGIN->ProtocolType  = CIPHER_PROTOCOL_TYPE;
            pEASYCRYPT_ACCOUNT_BEGIN->Mode          = 0;
            pEASYCRYPT_ACCOUNT_BEGIN->ServerKey     = EASYCRYPT_MAKEKEY(uServerKey);
            pEASYCRYPT_ACCOUNT_BEGIN->ClientKey     = EASYCRYPT_MAKEKEY(uClientKey);
            break;
        }
    case ENCODE_DECODE_NONE:
        break;
    default:
        ASSERT(!"Unexpected encode/decode mode for security socket stream !");
        goto EXIT0;
    }

//EXIT1:
    *puRetServerKey = uServerKey;
    *puRetClientKey = uClientKey;
    *ppiRetBuffer   = piBuffer;

    nResult = true;

EXIT0:
    if (!nResult)
    {
        SAFE_RELEASE(piBuffer);
    }

    return nResult;
}


int _SendSecurityKey(int nSocket, ENCODE_DECODE_MODE EncodeDecodeMode, unsigned *puRetServerKey, unsigned *puRetClientKey)
{
    int nResult  = false;
    int nRetCode = false;

    IMemBlock *piBuffer     = NULL;
    timeval     SendTimeOut  = {0, 1000};   // 1ms
    unsigned    uBufferSize  = 0;
    unsigned char *pbyBuffer = NULL;

    PROCESS_ERROR(nSocket != -1);
    PROCESS_ERROR(puRetServerKey);
    PROCESS_ERROR(puRetClientKey);

    nRetCode = _MakeSecurityKey(EncodeDecodeMode, puRetServerKey, puRetClientKey, &piBuffer);
    PROCESS_ERROR(nRetCode);

    if (piBuffer)
    {
        ASSERT((piBuffer->GetReserveSize()) > sizeof(WORD));

        pbyBuffer = (unsigned char *)piBuffer->GetData();
        uBufferSize = piBuffer->GetSize();

        pbyBuffer   -= sizeof(WORD);
        uBufferSize += sizeof(WORD);

        *((WORD *)pbyBuffer) = (WORD)uBufferSize;

        nRetCode = _SendOrRecvBuffer(true, nSocket, pbyBuffer, uBufferSize, SendTimeOut);
        PROCESS_ERROR(1 == nRetCode);
    }

    nResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return nResult;
}

ISocketStream *_RecvSecurityKey(ISocketStream *piSocketStream, ENCODE_DECODE_MODE EncodeDecodeMode)
{
    int nResult  = false;
    int nRetCode = false;

    ISocketStream        *piResult                   = NULL;
    IMemBlock              *piKeyBuffer                = NULL;
    ACCOUNT_BEGIN           *pACCOUNT_BEGIN             = NULL;
    EASYCRYPT_ACCOUNT_BEGIN *pEASYCRYPT_ACCOUNT_BEGIN   = NULL;
    timeval                  TimeoutValue               = {20, 0};

    PROCESS_ERROR(piSocketStream);
    piSocketStream->SetTimeout(&TimeoutValue);
    nRetCode = piSocketStream->Recv(&piKeyBuffer);
    TimeoutValue.tv_sec  = -1;
    TimeoutValue.tv_usec = -1;
    piSocketStream->SetTimeout(&TimeoutValue); // set time out value back to default(-1, -1)
    PROCESS_ERROR(nRetCode > 0);
    PROCESS_ERROR(piKeyBuffer);

    switch (EncodeDecodeMode)
    {
    case ENCODE_DECODE:
    case ENCODE_DECODE_SIMPLE:
    case ENCODE_DECODE_DYNAMIC:
        //set key
        PROCESS_ERROR(sizeof(ACCOUNT_BEGIN) == piKeyBuffer->GetSize());
        pACCOUNT_BEGIN = (ACCOUNT_BEGIN *)piKeyBuffer->GetData();
        PROCESS_ERROR(pACCOUNT_BEGIN);
        PROCESS_ERROR(pACCOUNT_BEGIN->ProtocolType == CIPHER_PROTOCOL_TYPE);
        PROCESS_ERROR(pACCOUNT_BEGIN->Mode == 0);
    
        piResult = new QSecuritySocketStream(
            piSocketStream, 
            NE_GETKEY(pACCOUNT_BEGIN->ClientKey),     
            NE_GETKEY(pACCOUNT_BEGIN->ServerKey),
            EncodeDecodeMode
        );
        break;
    case ENCODE_DECODE_EASY:
        //set key
        PROCESS_ERROR(sizeof(EASYCRYPT_ACCOUNT_BEGIN) == piKeyBuffer->GetSize());
        pEASYCRYPT_ACCOUNT_BEGIN = (EASYCRYPT_ACCOUNT_BEGIN *)piKeyBuffer->GetData();
        PROCESS_ERROR(pEASYCRYPT_ACCOUNT_BEGIN);
        PROCESS_ERROR(pEASYCRYPT_ACCOUNT_BEGIN->ProtocolType == CIPHER_PROTOCOL_TYPE);
        PROCESS_ERROR(pEASYCRYPT_ACCOUNT_BEGIN->Mode == 0);

        piResult = new QSecuritySocketStream(
            piSocketStream, 
            EASYCRYPT_GETKEY(pEASYCRYPT_ACCOUNT_BEGIN->ClientKey),     
            EASYCRYPT_GETKEY(pEASYCRYPT_ACCOUNT_BEGIN->ServerKey),
            EncodeDecodeMode
        );
        break;
    default:
        ASSERT(!"Unexpected encode/decode mode for security socket stream !");
        goto EXIT0;
    }

    PROCESS_ERROR(piResult);

    nResult = true;
EXIT0:
    SAFE_RELEASE(piKeyBuffer);

    if (!nResult)
    {
        SAFE_RELEASE(piResult);
    }

    return piResult;
}

int _CreateListenSocket(const char cszIPAddress[], int nPort, int *pnRetListenSocket)
{
    int nResult  = false;
    int nRetCode = false;
    int nOne = 1;
    unsigned long ulAddress = INADDR_ANY;
    int nListenSocket = -1;

    sockaddr_in LocalAddr;

    PROCESS_ERROR(pnRetListenSocket);

    nListenSocket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    PROCESS_ERROR(nListenSocket != -1);

    if (
        (cszIPAddress) &&
        (cszIPAddress[0] != '\0')
    )
    {
        ulAddress = inet_addr(cszIPAddress);
        if (ulAddress == INADDR_NONE)
            ulAddress = INADDR_ANY;
    }

	nRetCode = setsockopt(nListenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&nOne, sizeof(int));
    PROCESS_ERROR(nRetCode >= 0);

	ZeroStruct(LocalAddr);
    LocalAddr.sin_family        = AF_INET;
    LocalAddr.sin_addr.s_addr   = ulAddress;    //not need to htonl
    LocalAddr.sin_port          = htons(nPort);

    nRetCode = bind(nListenSocket, (struct sockaddr *)&LocalAddr, sizeof(LocalAddr));
    PROCESS_ERROR(nRetCode != -1);                                      

    nRetCode = listen(nListenSocket, 8);
    PROCESS_ERROR(nRetCode >= 0);

    nResult = true;   

EXIT0:
    if (!nResult)
    {    
        if (nListenSocket != -1)
        {
            _CloseSocket(nListenSocket);
            nListenSocket = -1;
        }
    }

    if (pnRetListenSocket)
    {
        *pnRetListenSocket = nListenSocket;
    }
    return nResult;
}


///////////////////////////////////////////////////////////////////////////////
// class SocketStream
///////////////////////////////////////////////////////////////////////////////
QSocketStream::QSocketStream(int nSocket,  struct sockaddr_in *pRemoteAddr)
{
    ASSERT(nSocket != -1);
    ASSERT(pRemoteAddr);
    
    m_ulRefCount        = 1;

    m_RemoteAddress     = *pRemoteAddr;
    m_nSocketHandle     = nSocket;
    m_Timeout.tv_sec    = -1;
    m_Timeout.tv_usec   = -1;
    m_pvUserData        = NULL;
}

int QSocketStream::IsAlive()
{
    int nRetsult = false;
    int nRetCode = false;
    volatile int nData = false; // no use


    PROCESS_ERROR(m_nSocketHandle != -1);

    nRetCode = send(m_nSocketHandle, (char *)&nData, 0, 0);
    PROCESS_ERROR(nRetCode != -1);

    nRetsult = true;
EXIT0:
    return nRetsult;
}

int QSocketStream::GetRemoteAddress(struct in_addr *pRemoteIP, u_short *pusRemotePort)
{
    int nResult  = false;
    int nRetCode = false;
    
    PROCESS_ERROR(m_nSocketHandle != -1);
    
    if (pRemoteIP)
        *pRemoteIP  = m_RemoteAddress.sin_addr;

    if (pusRemotePort)
        *pusRemotePort = m_RemoteAddress.sin_port;

    nResult = true;
EXIT0:    
    return nResult;
}


ULONG STDMETHODCALLTYPE QSocketStream::Release(void)
{
    unsigned long ulRefCount = InterlockedDecrement((long *)&m_ulRefCount);

    if (ulRefCount)
        return ulRefCount;

    if (m_nSocketHandle != -1)
    {
        _CloseSocket(m_nSocketHandle);
        m_nSocketHandle = -1;
    }

    delete this;
    return 0;
}

int QSocketStream::SetTimeout(const timeval *pTimeout)
{
    if (!pTimeout)
    {
        m_Timeout.tv_sec  = -1;
        m_Timeout.tv_usec = -1;
        return true;
    }

    m_Timeout = *pTimeout;
    return true;
}

// return -1: error, 0: timeout, 1: success
int QSocketStream::CheckCanSend(const timeval *pTimeout)
{
    return _CheckCanSend(m_nSocketHandle, pTimeout);
}


// return -1: error, 0: timeout, 1: success
int QSocketStream::Send(IMemBlock *piBuffer)
{
	//QLogPrintf(LOG_INFO,"..send %d",piBuffer->GetSize());
    int nResult  = -1;  // error
    int nRetCode = false;
    unsigned char *pbyBuffer = NULL;
    unsigned uBufferSize = 0;


    LOG_PROCESS_ERROR(piBuffer);
    piBuffer->AddRef();    
    LOG_PROCESS_ERROR(m_nSocketHandle != -1);
    
    uBufferSize = piBuffer->GetSize();
    LOG_PROCESS_ERROR(uBufferSize <= 65500);

    pbyBuffer = (unsigned char *)piBuffer->GetData();
    ASSERT(pbyBuffer);
    ASSERT((piBuffer->GetReserveSize()) > sizeof(WORD));

    pbyBuffer   -= sizeof(WORD);
    uBufferSize += sizeof(WORD);

    *((WORD *)pbyBuffer) = (WORD)uBufferSize;

    nRetCode = _SendOrRecvBuffer(
        true,   // SendFlag = true
        m_nSocketHandle, 
        pbyBuffer, 
        (int)uBufferSize, 
        m_Timeout
    );
    PROCESS_ERROR_RET_CODE(nRetCode > 0, nRetCode);
    
    nResult = 1;    // success
EXIT0:
    SAFE_RELEASE(piBuffer);

    return nResult;
}

// return -1: error, 0: timeout, 1: success
int QSocketStream::CheckCanRecv(const timeval *pTimeout)
{
    return _CheckCanRecv(m_nSocketHandle, pTimeout);
}


// return -1: error, 0: timeout, 1: success
int QSocketStream::Recv(IMemBlock **ppiRetBuffer)  
{
    int nResult     = -1;   // error
    int nRetCode    = false;
    
    IMemBlock *piBuffer = NULL;
    WORD        wDataLen = 0;

    PROCESS_ERROR(m_nSocketHandle != -1);
    PROCESS_ERROR(ppiRetBuffer);
    
    *ppiRetBuffer = NULL;
    
    nRetCode = _SendOrRecvBuffer(false, m_nSocketHandle, (unsigned char *)&wDataLen, (int)sizeof(wDataLen), m_Timeout);
    PROCESS_ERROR_RET_CODE(nRetCode > 0, -1);

    wDataLen -= sizeof(wDataLen);
    
    piBuffer = QCreateMemBlock((unsigned)wDataLen);
    PROCESS_ERROR(piBuffer);

    nRetCode = _SendOrRecvBuffer(
        false,  // SendFlag = false
        m_nSocketHandle, 
        (unsigned char *)piBuffer->GetData(), 
        (int)piBuffer->GetSize(), 
        m_Timeout
    );
    PROCESS_ERROR_RET_CODE(nRetCode > 0, nRetCode);

    *ppiRetBuffer = piBuffer;
    (*ppiRetBuffer)->AddRef();
    
    nResult = 1;    // success

EXIT0:
    SAFE_RELEASE(piBuffer);

    return nResult;
}

QBufferedSocketStream::QBufferedSocketStream(int nSocket, struct sockaddr_in *pRemoteAddr) : QSocketStream(nSocket, pRemoteAddr)
{
    m_uRecvTailPos = 0;
}

// return -2: timeout, -1: error, 0: timeout, 1: success
int QBufferedSocketStream::Recv(IMemBlock **ppiRetBuffer)  
{
    int         nResult         = -1;   // error
    int         nRetCode        = false;
    unsigned    uRecvDataSize   = 0;
    unsigned    uPackSize       = 0;
    unsigned    uSizeToRecv     = 0;
    IMemBlock *piBuffer        = NULL;

    PROCESS_ERROR(m_nSocketHandle != -1);
    PROCESS_ERROR(ppiRetBuffer);
    *ppiRetBuffer = NULL;

    while (true)
    {
        uRecvDataSize = m_uRecvTailPos;
        uSizeToRecv = sizeof(WORD) - uRecvDataSize; // Recv a WORD for the pack size first. 

        if (uRecvDataSize >= sizeof(WORD))  // We already have the pack size. 
        {
            uPackSize = *((WORD *)m_byRecvBuffer);
            LOG_PROCESS_ERROR(uPackSize > sizeof(WORD));

            // Make sure we have enough space to hold package. 
            LOG_PROCESS_ERROR(uPackSize <= sizeof(m_byRecvBuffer));

            // Then check if we already have package completely received. 
            PROCESS_SUCCESS(uPackSize <= uRecvDataSize);

            uSizeToRecv = uPackSize - uRecvDataSize;
        }

        ASSERT(uSizeToRecv);
        ASSERT(uSizeToRecv <= USHRT_MAX);

        nRetCode = _CheckCanRecv(m_nSocketHandle, &m_Timeout);
        PROCESS_ERROR_RET_CODE(nRetCode, -2);

        // Recv
        while (true)
        {
            nRetCode = recv(
                m_nSocketHandle, 
                (char *)(m_byRecvBuffer + m_uRecvTailPos), 
                uSizeToRecv, 
                0
            );
            if (nRetCode <= 0)  // when 0 is socket close
            {
                nRetCode = _IsSocketCanRestore();
                if (nRetCode)   // if can restore then continue
                    continue;

                nRetCode = _IsSocketWouldBlock();
                if (nRetCode)
                {
                    PROCESS_ERROR_RET_CODE(false, -2);
                }
                goto EXIT0;
            }
            // now nRetCode is recv bytes

            break;
        }

        m_uRecvTailPos += nRetCode;
    }

EXIT1:
    uPackSize -= sizeof(WORD);
    piBuffer = QCreateMemBlock(uPackSize);
    PROCESS_ERROR(piBuffer);

    memcpy(piBuffer->GetData(), m_byRecvBuffer + sizeof(WORD), uPackSize);
    m_uRecvTailPos = 0;

    *ppiRetBuffer = piBuffer;
    piBuffer = NULL;

    nResult = 1;    // success
EXIT0:
    SAFE_RELEASE(piBuffer);

    return nResult;
}

///////////////////////////////////////////////////////////////////////////////
// class SecuritySocketStream
///////////////////////////////////////////////////////////////////////////////
QSecuritySocketStream::QSecuritySocketStream(ISocketStream *pSocketStream, unsigned uSendKey, unsigned uRecvKey, ENCODE_DECODE_MODE EncodeDecodeMode )
{
    int nRetCode = false;

    ASSERT(pSocketStream);
    ASSERT(EncodeDecodeMode != ENCODE_DECODE_NONE);

    m_ulRefCount     = 1;
    m_uSendKey       = uSendKey;
    m_uRecvKey       = uRecvKey;
    m_piSocketStream = pSocketStream;
    m_piSocketStream->AddRef();

    nRetCode = _SetEncodeDecodeFunction(EncodeDecodeMode, &m_pfnEncodeFunction, &m_pfnDecodeFunction);
    ASSERT(nRetCode);
}

ULONG STDMETHODCALLTYPE QSecuritySocketStream::Release(void)
{
    unsigned long ulRefCount = InterlockedDecrement((long *)&m_ulRefCount);

    if (ulRefCount)
        return ulRefCount;

    SAFE_RELEASE(m_piSocketStream);
    
    delete this;
    return 0;
}

// return -1: error, 0: timeout, 1: success
int QSecuritySocketStream::Send(IMemBlock *piBuffer)
{
    int nResult  = false;
    int nRetCode = false;
    int nEncodeFlag      = false;
    unsigned uOldSendKey = m_uSendKey;
    
    PROCESS_ERROR(m_piSocketStream);
    PROCESS_ERROR(piBuffer);
   
    // use m_uSendKey encode
    nRetCode = m_pfnEncodeFunction(piBuffer->GetSize(), (unsigned char *)piBuffer->GetData(), &m_uSendKey);
    PROCESS_ERROR(nRetCode); 
    nEncodeFlag = true;

    nRetCode = m_piSocketStream->Send(piBuffer);
    PROCESS_ERROR_RET_CODE(nRetCode > 0, nRetCode); 
    
    nResult = nRetCode;

EXIT0:
    if ((nResult != 1) && piBuffer && nEncodeFlag)
    {
        if (m_pfnDecodeFunction)
        {
            m_uSendKey = uOldSendKey;
            nRetCode = m_pfnDecodeFunction(piBuffer->GetSize(), (unsigned char *)piBuffer->GetData(), &m_uSendKey);
        }
        m_uSendKey = uOldSendKey;
    }

    #ifdef _DEBUG
    if ((nResult == 1) && (piBuffer))
    {
        memset(piBuffer->GetData(), 0xbd, piBuffer->GetSize());
    }
    #endif

    return nResult;
}

// return -1: error, 0: timeout, 1: success
int QSecuritySocketStream::Recv(IMemBlock **ppiRetBuffer)
{
    int nResult  = false;
    int nRetCode = false;
    IMemBlock *piBuffer = NULL;
    
    PROCESS_ERROR(m_piSocketStream);
    PROCESS_ERROR(ppiRetBuffer);
    *ppiRetBuffer = NULL;
    
    nRetCode = m_piSocketStream->Recv(&piBuffer);
    PROCESS_ERROR_RET_CODE(nRetCode > 0, nRetCode); 

    ASSERT(piBuffer);

    // use m_uRecvKey decode
    nRetCode = m_pfnDecodeFunction(piBuffer->GetSize(), (unsigned char *)piBuffer->GetData(), &m_uRecvKey);
    PROCESS_ERROR_RET_CODE(nRetCode, -1);
    
    *ppiRetBuffer = piBuffer;
    (*ppiRetBuffer)->AddRef();
 
    nResult = true;

EXIT0:
    SAFE_RELEASE(piBuffer);
    
    return nResult;        
}


// =================================================================================================
// class SocketConnector begin

QSocketConnector::QSocketConnector()
{
    memset((void *)&m_BindLoaclAddr, 0, sizeof(sockaddr_in));
    m_BindLoaclAddr.sin_family       = AF_INET;
    m_BindLoaclAddr.sin_addr.s_addr  = INADDR_ANY;
    m_BindLoaclAddr.sin_port         = htons(0);
}

int QSocketConnector::Bind(const char cszLocalIPAddress[], int nPort /* = 0  */)
{
    int nResult  = false;
    int nRetCode = false;
    unsigned long ulAddress = INADDR_ANY;

    if ((cszLocalIPAddress) && (cszLocalIPAddress[0] != '\0'))
    {
        ulAddress = inet_addr(cszLocalIPAddress);
        if (ulAddress == INADDR_NONE)
            ulAddress = INADDR_ANY;
    }

    m_BindLoaclAddr.sin_addr.s_addr = ulAddress;
    m_BindLoaclAddr.sin_port = htons(nPort);

    nResult = true;
//EXIT0:
    return nResult;
}

ISocketStream *QSocketConnector::Connect(const char cszIPAddress[], int nPort)
{
    int nResult  = false;
    int nRetCode = false;
    ISocketStream *piResult = NULL;

    int nSocket = -1; 

    struct sockaddr_in ServerAddr;
    struct hostent *pHost = NULL;

    PROCESS_ERROR(cszIPAddress);

    pHost = gethostbyname(cszIPAddress);
    PROCESS_ERROR(pHost);

    nSocket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    PROCESS_ERROR(nSocket != -1);

    nRetCode = bind(nSocket, (struct sockaddr *)&m_BindLoaclAddr, sizeof(sockaddr_in));
    PROCESS_ERROR(nRetCode != -1);

#ifdef __GNUC__
    // linux
    {
        // 屏蔽SIGPIPE信号。其实不需要每次都做这个操作，但是Connector没有Init函数，所以只能暂时先放这里了。
        typedef void (*SignalHandlerPointer)(int);
        SignalHandlerPointer pSignalHandler = SIG_ERR;

        pSignalHandler = signal(SIGPIPE, SIG_IGN);
        PROCESS_ERROR(pSignalHandler != SIG_ERR);  // write when connection reset.
    }
#endif

    memset((void *)&ServerAddr, 0, sizeof(sockaddr_in));
    ServerAddr.sin_family       = AF_INET;
    ServerAddr.sin_addr.s_addr  = *(unsigned long *)pHost->h_addr_list[0];
    ServerAddr.sin_port         = htons(nPort);

    while (true)
    {
        nRetCode = connect(nSocket, (struct sockaddr *)&ServerAddr, sizeof(sockaddr_in));
        if (nRetCode >= 0)
            break;
            
        nRetCode = _IsSocketCanRestore();
        PROCESS_ERROR(nRetCode);
        // if can restore then continue
    }

    piResult = new QSocketStream(nSocket, &ServerAddr);
    PROCESS_ERROR(piResult);

    nSocket = -1;

    nResult = true;
EXIT0:

    if (!nResult)
    {
        SAFE_RELEASE(piResult);

        if (nSocket != -1)
        {
            _CloseSocket(nSocket);
            nSocket = -1;
        }
    }

    return piResult;
}

ISocketStream *QSocketConnector::ConnectSecurity(
    const char cszIPAddress[], 
    int nPort, 
    ENCODE_DECODE_MODE EncodeDecodeMode
)
{
    int nResult  = false;
    int nRetCode = false;

    ISocketStream *piResult = NULL;    
    ISocketStream *piSocketStream = NULL;
    
    PROCESS_ERROR(cszIPAddress);

    piSocketStream = Connect(cszIPAddress, nPort);
    PROCESS_ERROR(piSocketStream);
    
    piResult = _RecvSecurityKey(piSocketStream, EncodeDecodeMode);
    PROCESS_ERROR(piResult);   

    nResult = true;
EXIT0:

    SAFE_RELEASE(piSocketStream);

    if (!nResult)
    {
        SAFE_RELEASE(piResult);
    }

    return piResult;    
}

/////////////////////// SOCKS5 Proxy  ////////////////////////
#pragma pack(1)

#define SOCKS5_VERSION                          0x05

#define SOCKS5_RESERVED                         0x00
#define SOCKS5_AUTHTYPE_PASSWORD                0x02
#define SOCKS5_ADDRTYPE_IPV4                    0x01
#define SOCKS5_COMMAND_CONNECT                  0x01

#define SOCKS5_REPLY_SUCCEEDED                  0x00
#define SOCKS5_REPLY_GENERAL_FAILURE            0x01
#define SOCKS5_REPLY_CONNECTION_NOT_ALLOWED     0x02
#define SOCKS5_REPLY_NETWORK_UNREACHABLE        0x03
#define SOCKS5_REPLY_HOST_UNREACHABLE           0x04
#define SOCKS5_REPLY_CONNECTION_REFUSED         0x05
#define SOCKS5_REPLY_TTL_EXPIRED                0x06
#define SOCKS5_REPLY_COMMAND_NOT_SUPPORTED      0x07
#define SOCKS5_REPLY_ADDRESS_TYPE_NOT_SUPPORTED 0x08

#define SOCKS5_AUTH_VERSION                     0x01

// RFC1928
struct SOCKS5_BASE
{
    BYTE    bySocksVersion;
};

struct SOCKS5_REQUEST_NEGOTIATION : SOCKS5_BASE
{
    BYTE    byMethodsCount;
    BYTE    byMethods;
};

struct SOCKS5_RESPOND_NEGOTIATION : SOCKS5_BASE 
{
    BYTE    byMethod;
};

struct SOCKS5_ADDRESS_TYPE_IPV4 
{
    DWORD   dwDestAddress;
    WORD    wDestPort;
};

struct SOCKS5_REQUEST_COMMAND : SOCKS5_BASE
{
    BYTE    byCommand;
    BYTE    byReserved;
    BYTE    byAddressType;
};

struct SOCKS5_RESPOND_COMMAND : SOCKS5_BASE
{
    BYTE    byReply;
    BYTE    byReserved;
    BYTE    byAddressType;
};

// RFC1929
struct SOCKS5_AUTH_REQUEST_LOGIN
{
    BYTE    byAuthVersion;
};

struct SOCKS5_AUTH_RESPOND_LOGIN 
{
    BYTE    byAuthVersion;
    BYTE    byStatus;
};

#pragma pack()

static inline int _ProxyNegotiation(int nSocket, const timeval &crTimeOut)
{
    int nResult  = false;
    int nRetCode = false;
    struct SOCKS5_REQUEST_NEGOTIATION RequestNegotiation;
    struct SOCKS5_RESPOND_NEGOTIATION RespondNegotiation;

    PROCESS_ERROR(nSocket != -1);

    RequestNegotiation.bySocksVersion = SOCKS5_VERSION;
    RequestNegotiation.byMethodsCount = 1;
    RequestNegotiation.byMethods      = SOCKS5_AUTHTYPE_PASSWORD;

    nRetCode = _SendOrRecvBuffer(true, nSocket, (unsigned char *)&RequestNegotiation, sizeof(SOCKS5_REQUEST_NEGOTIATION), crTimeOut);
    PROCESS_ERROR(nRetCode == 1);

    nRetCode = _SendOrRecvBuffer(false, nSocket, (unsigned char *)&RespondNegotiation, sizeof(SOCKS5_RESPOND_NEGOTIATION), crTimeOut);
    PROCESS_ERROR(nRetCode == 1);

    PROCESS_ERROR(RespondNegotiation.bySocksVersion == SOCKS5_VERSION);
    PROCESS_ERROR(RespondNegotiation.byMethod == SOCKS5_AUTHTYPE_PASSWORD);

    nResult = true;
EXIT0:
    return nResult;
}

static inline int _ProxyAuthentication(int nSocket, const timeval &crTimeOut, const char cszUser[], const char cszPassword[])
{
    int nResult  = false;
    int nRetCode = false;
    unsigned uUserLen      = 0;
    BYTE     byUserLen     = 0;
    unsigned uPasswordLen  = 0;
    BYTE     byPasswordLen = 0;
    BYTE     byBuffer[1 + 1 + 255 + 1 + 255 + 32];  // 32 is reserve bytes

    struct SOCKS5_AUTH_RESPOND_LOGIN AuthRespondLogin;

    PROCESS_ERROR(nSocket != -1);
    PROCESS_ERROR(cszUser);
    PROCESS_ERROR(cszPassword);

    uUserLen = (unsigned)strlen(cszUser);
    PROCESS_ERROR(uUserLen > 0 && uUserLen < 256);
    byUserLen = (BYTE)uUserLen;

    uPasswordLen = (unsigned)strlen(cszPassword);
    PROCESS_ERROR(uPasswordLen > 0 && uPasswordLen < 256);
    byPasswordLen = (BYTE)uPasswordLen;

    //Send login request
    byBuffer[0] = SOCKS5_AUTH_VERSION;
    byBuffer[1] = byUserLen;
    memcpy(&byBuffer[2], cszUser, uUserLen);
    byBuffer[2 + uUserLen] = byPasswordLen;
    memcpy(&byBuffer[3 + uUserLen], cszPassword, uPasswordLen);

    nRetCode = _SendOrRecvBuffer(true, nSocket, (unsigned char *)&byBuffer, 3 + uUserLen + uPasswordLen, crTimeOut);
    PROCESS_ERROR(nRetCode == 1);

    //Recv respond.
    nRetCode = _SendOrRecvBuffer(false, nSocket, (unsigned char *)&AuthRespondLogin, sizeof(SOCKS5_AUTH_RESPOND_LOGIN), crTimeOut);
    PROCESS_ERROR(nRetCode == 1);

    PROCESS_ERROR(AuthRespondLogin.byAuthVersion == SOCKS5_AUTH_VERSION);
    PROCESS_ERROR(AuthRespondLogin.byStatus == 0);

    nResult = true;
EXIT0:
    return nResult;
}

static inline int _ProxyCreateConnection(int nSocket, const timeval &crTimeOut, long lDestAddress, int nDestPort, long *plRetAddress, int *pnRetPort)
{
    int nResult  = false;
    int nRetCode = false;
    BYTE byBuffer[512]; 
    struct SOCKS5_REQUEST_COMMAND   *pRequestCommand = (SOCKS5_REQUEST_COMMAND *)byBuffer;
    struct SOCKS5_ADDRESS_TYPE_IPV4 *pAddrIPv4 = (SOCKS5_ADDRESS_TYPE_IPV4 *)&byBuffer[sizeof(SOCKS5_REQUEST_COMMAND)];
    struct SOCKS5_RESPOND_COMMAND    RespondCommand;
    struct SOCKS5_ADDRESS_TYPE_IPV4  RespondAddress;

    PROCESS_ERROR(nSocket != -1);
    PROCESS_ERROR(lDestAddress != 0);
    PROCESS_ERROR(plRetAddress);
    PROCESS_ERROR(pnRetPort);

    pRequestCommand->bySocksVersion = SOCKS5_VERSION;
    pRequestCommand->byCommand      = SOCKS5_COMMAND_CONNECT;
    pRequestCommand->byReserved     = SOCKS5_RESERVED;
    pRequestCommand->byAddressType  = SOCKS5_ADDRTYPE_IPV4;

    pAddrIPv4->dwDestAddress = lDestAddress;
    pAddrIPv4->wDestPort = htons(nDestPort);

    nRetCode = _SendOrRecvBuffer(true, nSocket, byBuffer, sizeof(SOCKS5_REQUEST_COMMAND) + sizeof(SOCKS5_ADDRESS_TYPE_IPV4), crTimeOut);
    PROCESS_ERROR(nRetCode == 1);

    nRetCode = _SendOrRecvBuffer(false, nSocket, (unsigned char *)&RespondCommand, sizeof(SOCKS5_RESPOND_COMMAND), crTimeOut);
    PROCESS_ERROR(nRetCode == 1);

    PROCESS_ERROR(RespondCommand.bySocksVersion == SOCKS5_VERSION);
    PROCESS_ERROR(RespondCommand.byReply == SOCKS5_REPLY_SUCCEEDED);
    PROCESS_ERROR(RespondCommand.byAddressType == SOCKS5_ADDRTYPE_IPV4);

    nRetCode = _SendOrRecvBuffer(false, nSocket, (unsigned char *)&RespondAddress, sizeof(SOCKS5_ADDRESS_TYPE_IPV4), crTimeOut);
    PROCESS_ERROR(nRetCode == 1);
    
    *plRetAddress = RespondAddress.dwDestAddress;
    *pnRetPort    = RespondAddress.wDestPort;

    nResult = true;
EXIT0:
    return nResult;
}
//////////////////////////////////////////////////////////////////////////

ISocketStream *QSocketConnector::ProxyConnect(
    const char cszProxyAddress[],   int nProxyPort, 
    const char cszProxyUsername[],  const char cszProxyPassword[], 
    const char cszIPAddress[],      int nPort
)
{
    int nResult  = false;
    int nRetCode = false;
    char *pszIP  = NULL;
    timeval TimeOut = {3, 0};
    ISocketStream *piResult = NULL;
    long lJumperAddress = 0;
    int  nJumperPort = 0;

    int nSocket = -1; 
    
    struct sockaddr_in ProxyAddr;
    struct sockaddr_in DestAddr;
    struct sockaddr_in ServerAddr;
    struct hostent *pHost = NULL;

    PROCESS_ERROR(cszProxyAddress);
    PROCESS_ERROR(cszProxyUsername);
    PROCESS_ERROR(cszProxyPassword);
    PROCESS_ERROR(cszIPAddress);
    
    nSocket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    PROCESS_ERROR(nSocket != -1);

    nRetCode = bind(nSocket, (struct sockaddr *)&m_BindLoaclAddr, sizeof(sockaddr_in));
    PROCESS_ERROR(nRetCode != -1);

    pHost = gethostbyname(cszProxyAddress);
    PROCESS_ERROR(pHost);

    memset((void *)&ProxyAddr, 0, sizeof(sockaddr_in));
    ProxyAddr.sin_family       = AF_INET;
    ProxyAddr.sin_addr.s_addr  = *(unsigned long *)pHost->h_addr_list[0];
    ProxyAddr.sin_port         = htons(nProxyPort);

    pHost = gethostbyname(cszIPAddress);
    PROCESS_ERROR(pHost);

    memset((void *)&DestAddr, 0, sizeof(sockaddr_in));
    DestAddr.sin_family       = AF_INET;
    DestAddr.sin_addr.s_addr  = *(unsigned long *)pHost->h_addr_list[0];
    DestAddr.sin_port         = htons(nPort);

    while (true)
    {
        nRetCode = connect(nSocket, (struct sockaddr *)&ProxyAddr, sizeof(sockaddr_in));
        if (nRetCode >= 0)
            break;
            
        nRetCode = _IsSocketCanRestore();
        PROCESS_ERROR(nRetCode);
        // if can restore then continue
    }
    PROCESS_ERROR(nSocket != -1);
    
    nRetCode = _ProxyNegotiation(nSocket, TimeOut);
    PROCESS_ERROR(nRetCode);

    nRetCode = _ProxyAuthentication(nSocket, TimeOut, cszProxyUsername, cszProxyPassword);
    PROCESS_ERROR(nRetCode);

    nRetCode = _ProxyCreateConnection(nSocket, TimeOut, DestAddr.sin_addr.s_addr, nPort, &lJumperAddress, &nJumperPort);
    PROCESS_ERROR(nRetCode);

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = lJumperAddress;
    ServerAddr.sin_port = nJumperPort;

    piResult = new QSocketStream(nSocket, &ServerAddr);
    PROCESS_ERROR(piResult);
    nSocket = -1;

    nResult = true;
EXIT0:

    if (!nResult)
    {
        SAFE_RELEASE(piResult);
    }

    if (nSocket != -1)
    {
        _CloseSocket(nSocket);
        nSocket = -1;
    }

    return piResult;
}

ISocketStream *QSocketConnector::ProxyConnectSecurity(
    const char cszProxyAddress[],   int nProxyPort, 
    const char cszProxyUsername[],  const char cszProxyPassword[], 
    const char cszIPAddress[],      int nPort, 
    ENCODE_DECODE_MODE EncodeDecodeMode
)
{
    int nResult  = false;
    int nRetCode = false;

    ISocketStream *piResult = NULL;
    ISocketStream *piSocketStream = NULL;

    PROCESS_ERROR(cszProxyAddress);
    PROCESS_ERROR(cszProxyUsername);
    PROCESS_ERROR(cszProxyPassword);
    PROCESS_ERROR(cszIPAddress);

    piSocketStream = ProxyConnect(cszProxyAddress, nProxyPort, cszProxyUsername, cszProxyPassword, cszIPAddress, nPort);
    PROCESS_ERROR(piSocketStream);

    piResult = _RecvSecurityKey(piSocketStream, EncodeDecodeMode);
    PROCESS_ERROR(piResult);
    
    nResult = true;
EXIT0:
    
    SAFE_RELEASE(piSocketStream);

    if (!nResult)
    {
        SAFE_RELEASE(piResult);
    }

    return piResult;
}
// class SocketConnector end
// =============================================================================================

ISocketStream *QBufferedSocketConnector::Connect(const char cszIPAddress[], int nPort)
{
    int nResult  = false;
    int nRetCode = false;
    ISocketStream *piResult = NULL;

    int nSocket = -1; 

    struct sockaddr_in ServerAddr;
    struct hostent *pHost = NULL;

    PROCESS_ERROR(cszIPAddress);

    pHost = gethostbyname(cszIPAddress);
    PROCESS_ERROR(pHost);

    nSocket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    PROCESS_ERROR(nSocket != -1);

    nRetCode = bind(nSocket, (struct sockaddr *)&m_BindLoaclAddr, sizeof(sockaddr_in));
    PROCESS_ERROR(nRetCode != -1);

    memset((void *)&ServerAddr, 0, sizeof(sockaddr_in));
    ServerAddr.sin_family       = AF_INET;
    ServerAddr.sin_addr.s_addr  = *(unsigned long *)pHost->h_addr_list[0];
    ServerAddr.sin_port         = htons(nPort);

    while (true)
    {
        nRetCode = connect(nSocket, (struct sockaddr *)&ServerAddr, sizeof(sockaddr_in));
        if (nRetCode >= 0)
            break;

        nRetCode = _IsSocketCanRestore();
        PROCESS_ERROR(nRetCode);
        // if can restore then continue
    }

    piResult = new QBufferedSocketStream(nSocket, &ServerAddr);
    PROCESS_ERROR(piResult);

    nSocket = -1;

    nResult = true;
EXIT0:

    if (!nResult)
    {
        SAFE_RELEASE(piResult);

        if (nSocket != -1)
        {
            _CloseSocket(nSocket);
            nSocket = -1;
        }
    }

    return piResult;
}

ISocketStream *QBufferedSocketConnector::ProxyConnect(
    const char cszProxyAddress[],   int nProxyPort, 
    const char cszProxyUsername[],  const char cszProxyPassword[], 
    const char cszIPAddress[],      int nPort
    )
{
    int nResult  = false;
    int nRetCode = false;
    char *pszIP  = NULL;
    timeval TimeOut = {3, 0};
    ISocketStream *piResult = NULL;
    long lJumperAddress = 0;
    int  nJumperPort = 0;

    int nSocket = -1; 

    struct sockaddr_in ProxyAddr;
    struct sockaddr_in DestAddr;
    struct sockaddr_in ServerAddr;
    struct hostent *pHost = NULL;

    PROCESS_ERROR(cszProxyAddress);
    PROCESS_ERROR(cszProxyUsername);
    PROCESS_ERROR(cszProxyPassword);
    PROCESS_ERROR(cszIPAddress);

    nSocket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    PROCESS_ERROR(nSocket != -1);

    nRetCode = bind(nSocket, (struct sockaddr *)&m_BindLoaclAddr, sizeof(sockaddr_in));
    PROCESS_ERROR(nRetCode != -1);

    pHost = gethostbyname(cszProxyAddress);
    PROCESS_ERROR(pHost);

    memset((void *)&ProxyAddr, 0, sizeof(sockaddr_in));
    ProxyAddr.sin_family       = AF_INET;
    ProxyAddr.sin_addr.s_addr  = *(unsigned long *)pHost->h_addr_list[0];
    ProxyAddr.sin_port         = htons(nProxyPort);

    pHost = gethostbyname(cszIPAddress);
    PROCESS_ERROR(pHost);

    memset((void *)&DestAddr, 0, sizeof(sockaddr_in));
    DestAddr.sin_family       = AF_INET;
    DestAddr.sin_addr.s_addr  = *(unsigned long *)pHost->h_addr_list[0];
    DestAddr.sin_port         = htons(nPort);

    while (true)
    {
        nRetCode = connect(nSocket, (struct sockaddr *)&ProxyAddr, sizeof(sockaddr_in));
        if (nRetCode >= 0)
            break;

        nRetCode = _IsSocketCanRestore();
        PROCESS_ERROR(nRetCode);
        // if can restore then continue
    }
    PROCESS_ERROR(nSocket != -1);

    nRetCode = _ProxyNegotiation(nSocket, TimeOut);
    PROCESS_ERROR(nRetCode);

    nRetCode = _ProxyAuthentication(nSocket, TimeOut, cszProxyUsername, cszProxyPassword);
    PROCESS_ERROR(nRetCode);

    nRetCode = _ProxyCreateConnection(nSocket, TimeOut, DestAddr.sin_addr.s_addr, nPort, &lJumperAddress, &nJumperPort);
    PROCESS_ERROR(nRetCode);

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = lJumperAddress;
    ServerAddr.sin_port = nJumperPort;

    piResult = new QBufferedSocketStream(nSocket, &ServerAddr);
    PROCESS_ERROR(piResult);
    nSocket = -1;

    nResult = true;
EXIT0:

    if (!nResult)
    {
        SAFE_RELEASE(piResult);
    }

    if (nSocket != -1)
    {
        _CloseSocket(nSocket);
        nSocket = -1;
    }

    return piResult;
}

// =============================================================================================
// Class SocketAcceptor begin

QSocketAcceptor::QSocketAcceptor(void)
{
    m_nListenSocket   = -1;
    //m_Timeout.tv_sec  = -1;
    m_Timeout.tv_sec  = 10;//by ldy 设置超时
    m_Timeout.tv_usec = -1;
}

QSocketAcceptor::~QSocketAcceptor(void)
{
    if (m_nListenSocket != -1)
    {
        _CloseSocket(m_nListenSocket);
        m_nListenSocket = -1;
    }
}

int QSocketAcceptor::Open(const char cszIPAddress[], int nPort)
{
    int nResult  = false;
    int nRetCode = false;

#ifdef __GNUC__
    // linux
    {
        typedef void (*SignalHandlerPointer)(int);
        SignalHandlerPointer pSignalHandler = SIG_ERR;

        pSignalHandler = signal(SIGPIPE, SIG_IGN);
        PROCESS_ERROR(pSignalHandler != SIG_ERR);  // write when connection reset.
    }
#endif

    nRetCode = _CreateListenSocket(cszIPAddress, nPort, &m_nListenSocket);
    PROCESS_ERROR(nRetCode);

    nResult = true;   

EXIT0:
    if (!nResult)
    {    
        if (m_nListenSocket != -1)
        {
            _CloseSocket(m_nListenSocket);
            m_nListenSocket = -1;
        }
    }
    return nResult;
}

int QSocketAcceptor::SetTimeout(const timeval *pTimeout)
{
    if (!pTimeout)
    {
        m_Timeout.tv_sec  = -1;
        m_Timeout.tv_usec = -1;
        return true;
    }
    
    m_Timeout = *pTimeout;
    return true;
}

int QSocketAcceptor::Close(void)
{
    if (m_nListenSocket != -1)
    {
        _CloseSocket(m_nListenSocket);
        m_nListenSocket = -1;
    }
    return true;
}

ISocketStream *QSocketAcceptor::Accept(void)
{
    int nResult  = false;
    int nRetCode = -1;
    
    ISocketStream *piResult = NULL;

    const timeval *pcTimeout  = NULL;
    int nClientSocket  = -1;
    struct sockaddr_in RemoteAddr;
    socklen_t nAddrLen       = 0;

	m_AcceptLock.Lock();

    PROCESS_ERROR(m_nListenSocket != -1);

    if (m_Timeout.tv_sec != -1)
        pcTimeout = &m_Timeout;

    while (true)
    {
        nRetCode = _CheckCanRecv(m_nListenSocket, pcTimeout);
        if (nRetCode <= 0)
        {
            nRetCode = _IsSocketCanRestore();
            if (nRetCode)   // if can restore then continue
                continue;
            
            goto EXIT0;
        }

        nAddrLen = sizeof(sockaddr_in);
        memset((void *)&RemoteAddr, 0, sizeof(sockaddr_in));
        
        nClientSocket = (int)accept(m_nListenSocket, (struct sockaddr *)&RemoteAddr, &nAddrLen);
        if (nClientSocket == -1)
        {
            nRetCode = _IsSocketCanRestore();
            if (nRetCode)   // if can restore then continue
                continue;
                
            goto EXIT0;
        }

        break;
    }

 
    piResult = new QSocketStream(nClientSocket, &RemoteAddr);
    PROCESS_ERROR(piResult);
    nClientSocket = -1;
    
    nRetCode = piResult->SetTimeout(&m_Timeout);
    PROCESS_ERROR(nRetCode);

    nResult = true;
EXIT0:

    if (!nResult)
    {
        SAFE_RELEASE(piResult);
        
        if (nClientSocket != -1)
        {
            _CloseSocket(nClientSocket);
            nClientSocket = -1;
        }
    }

	m_AcceptLock.Unlock();
    
    return piResult;
}



ISocketStream *QSocketAcceptor::AcceptSecurity(ENCODE_DECODE_MODE EncodeDecodeMode)
{
    int nResult  = false;
    int nRetCode = false;
    ISocketStream *piResult          = NULL;    
    ISocketStream *piSocketStream    = NULL;
    IMemBlock       *piBuffer          = NULL;    

    unsigned uServerKey = 0;
    unsigned uClientKey = 0;

    piSocketStream = Accept();
    PROCESS_ERROR(piSocketStream);

    nRetCode = _MakeSecurityKey(EncodeDecodeMode, &uServerKey, &uClientKey, &piBuffer);
    PROCESS_ERROR(nRetCode);
    PROCESS_ERROR(piBuffer);

    nRetCode = piSocketStream->Send(piBuffer);
    PROCESS_ERROR(nRetCode > 0);

    piResult = new QSecuritySocketStream(piSocketStream, uServerKey, uClientKey, EncodeDecodeMode);
    PROCESS_ERROR(piResult);

    nResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    SAFE_RELEASE(piSocketStream);

    if (!nResult)
    {
        SAFE_RELEASE(piResult);
    }

    return piResult;
}

// class SocketAcceptor end
// =================================================================================================



//////////////////////////////////////////////////////////////////////////
// class AsyncSocketStream  begin
class QAsyncSocketStream : public ISocketStream
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject
        ) { return -1; };
    virtual ULONG STDMETHODCALLTYPE AddRef(void)    {  return InterlockedIncrement((long *)&m_ulRefCount); };
    virtual ULONG STDMETHODCALLTYPE Release(void);

    virtual int SetTimeout(const timeval *pTimeout);

    virtual int CheckCanSend(const timeval *pTimeout) { return -1; }

    virtual int Send(IMemBlock *piBuffer);

    virtual int CheckCanRecv(const timeval *pTimeout) { return -1; }

    virtual int Recv(IMemBlock **ppiRetBuffer);

    virtual int IsAlive();

    virtual int GetRemoteAddress(struct in_addr *pRemoteIP, u_short *pusRemotePort);

    virtual int SetUserData(void *pvUserData) { m_pvUserData = pvUserData; return true; }

    virtual void *GetUserData() { return m_pvUserData; }

	virtual	int GetSOCKET() { return m_nSocketHandle; }
public:
    QAsyncSocketStream();
    ~QAsyncSocketStream();

    int Init(
        int nSocket, struct sockaddr_in *pRemoteAddr, 
        unsigned uRecvBufferSize, unsigned uSendBufferSize, 
        unsigned uSendKey, unsigned uRecvKey, ENCODE_DECODE_MODE EncodeDecodeMode, 
        void *pvContext
    );

    int UnInit(void *pvContext);

    int Destory();

private:
    volatile unsigned long  m_ulRefCount;
    int                 m_nSocketHandle;
    timeval             m_Timeout;
    struct sockaddr_in  m_RemoteAddress;
    void               *m_pvUserData;
    
    IMemBlock         *m_piRecvBuffer;
    unsigned            m_uRecvBufferSize;
    unsigned char      *m_pbyRecvBuffer;
    unsigned            m_uRecvHeadPos;
    unsigned            m_uRecvTailPos;

    unsigned            m_uSendKey;
    unsigned            m_uRecvKey;
    SOCKET_STREAM_ENCODE_DECODE_FUNCTION *m_pfnEncodeFunction;
    SOCKET_STREAM_ENCODE_DECODE_FUNCTION *m_pfnDecodeFunction;

#ifdef WIN32
public:
    int GetDelayDestoryFlag()   { return m_nDelayDestoryFlag;  }
    int GetRecvCompletedFlag()  { return m_nRecvCompletedFlag; }
    int GetWaitNotifyFlag()     { return m_nWaitNotifyFlag; }

    int OnRecvCompleted(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);
    int OnWaitNotifyed();

private:
    int                 m_nDelayDestoryFlag;
    int                 m_nWaitNotifyFlag;
    int                 m_nRecvCompletedFlag;
    int                 m_nRecvCompletedErrorCode;
    int                 m_nRecvCompletedSize;
    WSABUF              m_wsBuf;
    DWORD               m_dwWsFlag;

public:
    WSAOVERLAPPED       m_RecvWSOVL;
#endif
};



ULONG STDMETHODCALLTYPE QAsyncSocketStream::Release(void)
{
    unsigned long ulRefCount = InterlockedDecrement((long *)&m_ulRefCount);

    if (ulRefCount)
        return ulRefCount;

    #ifdef WIN32
    // notify IoCompletionCallback to exit, and call Destory in  _SocketServerAcceptor::Wait
    m_nDelayDestoryFlag = true;

    if (m_nSocketHandle != -1)
    {
        _CloseSocket(m_nSocketHandle);
        m_nSocketHandle = -1;
    }

    return 0;
    #endif

    Destory();

    return 0;
}

QAsyncSocketStream::QAsyncSocketStream()
{
    m_ulRefCount        = 1;

    memset(&m_RemoteAddress, 0, sizeof(m_RemoteAddress));
    m_nSocketHandle     = -1;
    //m_Timeout.tv_sec    = -1;
	m_Timeout.tv_sec    = 1;
    m_Timeout.tv_usec   = -1;
    m_pvUserData        = NULL;
    
    m_piRecvBuffer      = NULL;
    m_uRecvBufferSize   = 0;
    m_pbyRecvBuffer     = NULL;
    m_uRecvHeadPos      = 0;
    m_uRecvTailPos      = 0;

    m_uSendKey          = 0;
    m_uRecvKey          = 0;
    m_pfnEncodeFunction = NULL;
    m_pfnDecodeFunction = NULL;
    
    #ifdef WIN32
        m_nDelayDestoryFlag         = false;
        m_nWaitNotifyFlag           = true;
        m_nRecvCompletedFlag        = true;     // first time must be true, so can call WSARecv in Recv()
        m_nRecvCompletedErrorCode   = ERROR_SUCCESS;
        m_nRecvCompletedSize        = 0;
    #endif
}

QAsyncSocketStream::~QAsyncSocketStream()
{
    ASSERT(m_nSocketHandle == -1);
    ASSERT(m_piRecvBuffer == NULL);
    ASSERT(m_uRecvBufferSize == 0);
    ASSERT(m_pbyRecvBuffer == NULL);
    
}

int QAsyncSocketStream::Init(
    int nSocket, struct sockaddr_in *pRemoteAddr, 
    unsigned uRecvBufferSize, unsigned uSendBufferSize, 
    unsigned uSendKey, unsigned uRecvKey, ENCODE_DECODE_MODE EncodeDecodeMode, 
    void *pvContext
)
{
    int nResult  = false;
    int nRetCode = false;
    int nSockOptSndBufValue = 0;
    int nSockOptValueSize   = 0;

    PROCESS_ERROR(nSocket != -1);
    PROCESS_ERROR(pRemoteAddr);
    PROCESS_ERROR(uRecvBufferSize > 0);
    PROCESS_ERROR(uSendBufferSize > 0);
    USE_ARGUMENT(pvContext);
    
    m_RemoteAddress     = *pRemoteAddr;
    m_Timeout.tv_sec    = -1;
    m_Timeout.tv_usec   = -1;
    m_uRecvHeadPos      = 0;
    m_uRecvTailPos      = 0;

    m_uSendKey          = uSendKey;
    m_uRecvKey          = uRecvKey;

    nRetCode = _SetSocketNoBlock(nSocket);
    PROCESS_ERROR(nRetCode);

    m_piRecvBuffer = QCreateMemBlock(uRecvBufferSize);
    PROCESS_ERROR(m_piRecvBuffer);

    m_uRecvBufferSize = m_piRecvBuffer->GetSize();
    m_pbyRecvBuffer   = (unsigned char *)m_piRecvBuffer->GetData();

    nRetCode = _SetEncodeDecodeFunction(EncodeDecodeMode, &m_pfnEncodeFunction, &m_pfnDecodeFunction);
    PROCESS_ERROR(nRetCode);

    nRetCode = setsockopt(nSocket, SOL_SOCKET, SO_SNDBUF, (char *)&uSendBufferSize, sizeof(uSendBufferSize));
    PROCESS_ERROR(-1 != nRetCode);
   
    #ifdef WIN32
        m_nDelayDestoryFlag         = false;
        m_nWaitNotifyFlag           = true;
        m_nRecvCompletedFlag        = true;     // first time must be true, so can call WSARecv in Recv()
        m_nRecvCompletedErrorCode   = ERROR_SUCCESS;
        m_nRecvCompletedSize        = 0;
    #endif
    
    m_nSocketHandle     = nSocket;

#ifdef USE_SOCKET_INFO_LOGGER
	g_InfoLogger.OnConnect(this);
#endif

    nResult = true;
EXIT0:    
    if (!nResult)
    {
        SAFE_RELEASE(m_piRecvBuffer);
        m_uRecvBufferSize = 0;
        m_pbyRecvBuffer   = NULL;
    }
    return nResult;
}


int QAsyncSocketStream::UnInit(void *pvContext)
{
    int nResult = false;

#ifdef USE_SOCKET_INFO_LOGGER
	g_InfoLogger.OnDisConnect(this);
#endif

	nResult = true;
//EXIT0:
	m_uSendKey          = 0;
	m_uRecvKey          = 0;
	m_pfnEncodeFunction = NULL;
	m_pfnDecodeFunction = NULL;

	SAFE_RELEASE(m_piRecvBuffer);
	m_uRecvBufferSize   = 0;
	m_pbyRecvBuffer     = NULL;

	m_uRecvHeadPos = 0;
	m_uRecvTailPos = 0;

	if (m_nSocketHandle != -1)
	{
		_CloseSocket(m_nSocketHandle);
		m_nSocketHandle = -1;
	}
    return nResult;
}


int QAsyncSocketStream::Destory()
{
    int nResult = false;

    UnInit(NULL);

    delete this;

    nResult = true;
//EXIT0:
    return nResult;
}

int QAsyncSocketStream::SetTimeout(const timeval *pTimeout)
{
    if (!pTimeout)
    {
        m_Timeout.tv_sec  = -1;
        m_Timeout.tv_usec = -1;
        return true;
    }

    m_Timeout = *pTimeout;
    return true;
}

// return -1: error, 0: timeout, 1: success
int QAsyncSocketStream::Send(IMemBlock *piBuffer)
{
    int nResult  = -1;
    int nRetCode = false;
    int nErrorNumber = 0;
    unsigned char *pbyBuffer = NULL;
    unsigned uBufferSize = 0;
    int nEncodeFlag      = false;
    unsigned uOldSendKey = m_uSendKey;
    int nSockOptSndBufValue = 0;
    int nSockOptValueSize   = 0;
	//int nRetryCount = 1024;
	
    PROCESS_ERROR(piBuffer);
    piBuffer->AddRef();
    PROCESS_ERROR(m_nSocketHandle != -1);

    uBufferSize = piBuffer->GetSize();
    LOG_PROCESS_ERROR(uBufferSize <= 65500);

    pbyBuffer = (unsigned char *)piBuffer->GetData();
    ASSERT(pbyBuffer);
    ASSERT((piBuffer->GetReserveSize()) > sizeof(WORD));

    //If we have a encrypt function.
    if (m_pfnEncodeFunction)
    {
        nRetCode = m_pfnEncodeFunction(uBufferSize, pbyBuffer, &m_uSendKey);
        PROCESS_ERROR(nRetCode);
        nEncodeFlag = true;
    }

    pbyBuffer   -= sizeof(WORD);
    uBufferSize += sizeof(WORD);

    *((WORD *)pbyBuffer) = (WORD)uBufferSize;

    while (uBufferSize > 0)
    {
        nRetCode = send(m_nSocketHandle, (const char *)pbyBuffer, uBufferSize, 0);
		
        if (nRetCode >= 0)
        {
            uBufferSize -= nRetCode;
            pbyBuffer   += nRetCode;
            continue;
        }

		
		int nErrorCode = _GetSocketErrorCode();
		
        nRetCode = _IsSocketCanRestore();
        if (nRetCode)  // A signal interrupted send() before any data was transmitted.
            continue;  // Just try again.
		
        nRetCode = _IsSocketWouldBlock();
		
        if (nRetCode)
        {
            QLogPrintf(
                LOG_DEBUG,
                "piSocketStream = %p, nSocket = %d, errorCode= %d, Send buffer full! Please use \"sysctl -a | grep net.ipv4.tcp_wmem\" to check your system settings. \n",
                this,
                m_nSocketHandle,
				nErrorCode
            );

			#ifndef WIN32
			QLogPrintf(LOG_DEBUG, "error str = %s\n", strerror(nErrorCode));
			#endif
			
            LOG_PROCESS_ERROR(false);
        }
		
        LOG_PROCESS_ERROR(false); // Return error when we got other errors. 
    }

    nResult = 1;

EXIT0:
    if ((nResult != 1) && piBuffer && nEncodeFlag)
    {
        if (m_pfnDecodeFunction)
        {
            m_uSendKey = uOldSendKey;
            m_pfnDecodeFunction(piBuffer->GetSize(), (unsigned char *)piBuffer->GetData(), &m_uSendKey);
        }
        m_uSendKey = uOldSendKey;
    }

    #ifdef _DEBUG
    if ((nResult == 1) && (piBuffer) && m_pfnEncodeFunction)
    {
        memset(piBuffer->GetData(), 0xbd, piBuffer->GetSize());
    }
    #endif

    SAFE_RELEASE(piBuffer);
    return nResult;
}

//return -2: again, -1: error, 0: timeout, 1: success
int QAsyncSocketStream::Recv(IMemBlock **ppiRetBuffer)
{
    int nResult             = -1;   //error
    int nRetCode            = false;
    unsigned uRecvDataSize  = 0;
    unsigned uPackSize      = 0;

    PROCESS_ERROR(m_nSocketHandle != -1);
    PROCESS_ERROR(m_piRecvBuffer);
    PROCESS_ERROR(ppiRetBuffer);

    *ppiRetBuffer = NULL;

    while (true)
    {
        #ifdef WIN32
            if (!m_nRecvCompletedFlag)
            {
                PROCESS_ERROR_RET_CODE(false, -2);
            }
            // when Io Completion
            PROCESS_ERROR(m_nRecvCompletedErrorCode == ERROR_SUCCESS);
            m_uRecvTailPos += m_nRecvCompletedSize;
            m_nRecvCompletedSize = 0;
        #endif

        //Already have whole pack, just return it.
        uRecvDataSize = m_uRecvTailPos - m_uRecvHeadPos;
        if (uRecvDataSize >= sizeof(WORD))
        {
            uPackSize = *((WORD *)(m_pbyRecvBuffer + m_uRecvHeadPos));
            LOG_PROCESS_ERROR(uPackSize > sizeof(WORD));

            // Make sure we have enough space to hold package.
            LOG_PROCESS_ERROR(uPackSize <= m_uRecvBufferSize);

            // Then check if we already have package completely received.
            PROCESS_SUCCESS(uPackSize <= uRecvDataSize);
        }

        //If we have reach the end, then move the rest data to the beginning of the internal buffer.
        if (m_uRecvTailPos == m_uRecvBufferSize)
        {
            uRecvDataSize = m_uRecvTailPos - m_uRecvHeadPos;
            memmove(m_pbyRecvBuffer, m_pbyRecvBuffer + m_uRecvHeadPos, uRecvDataSize);
            m_uRecvHeadPos = 0;
            m_uRecvTailPos = m_uRecvHeadPos + uRecvDataSize;
        }

        while (true)
        {
            #ifdef WIN32
                DWORD dwProcessBytes = 0;
                m_wsBuf.len = m_uRecvBufferSize - m_uRecvTailPos;
                m_wsBuf.buf = (char *)(m_pbyRecvBuffer + m_uRecvTailPos);
                m_dwWsFlag = 0;
                memset(&m_RecvWSOVL, 0, sizeof(m_RecvWSOVL));

                m_nRecvCompletedErrorCode = ERROR_SUCCESS;
                m_nRecvCompletedSize = 0;

                m_nRecvCompletedFlag = false;
                nRetCode = WSARecv(m_nSocketHandle, &m_wsBuf, 1, &dwProcessBytes, &m_dwWsFlag, &m_RecvWSOVL, NULL);
                if (nRetCode < 0)   // when 0 is success
                {
                    nRetCode = _IsSocketCanRestore();
                    if (nRetCode)   // if can restore then continue
                        continue;

                    nRetCode = _IsSocketWouldBlock();
                    if (nRetCode)
                    {
                        PROCESS_ERROR_RET_CODE(false, -2);
                    }

                    m_nRecvCompletedFlag = true;
                    goto EXIT0;
                }
                // when success also need to wait for IoCompletionCallBack
                nRetCode = 0;  // set to zero recv bytes
                nResult = -2;
                goto EXIT0;
            #else
                // GNU Linux
                nRetCode = recv(
                    m_nSocketHandle, 
                    (char *)(m_pbyRecvBuffer + m_uRecvTailPos), 
                    m_uRecvBufferSize - m_uRecvTailPos, 
                    0
                );
                if (nRetCode == 0)      // when 0 is socket close
                    goto EXIT0;

                if (nRetCode < 0)
                {
                    nRetCode = _IsSocketCanRestore();
                    if (nRetCode)   // if can restore then continue
                        continue;

                    nRetCode = _IsSocketWouldBlock();
                    if (nRetCode)
                        PROCESS_ERROR_RET_CODE(false, -2);

                    goto EXIT0;
                }
                // now nRetCode is recv bytes
            #endif

            break;
        }
        
        m_uRecvTailPos += nRetCode;
    }
    
EXIT1:
    uPackSize -= sizeof(WORD);
    m_uRecvHeadPos += sizeof(WORD);
    *ppiRetBuffer = QCreateMemBlock(uPackSize);
    PROCESS_ERROR(*ppiRetBuffer);

    memcpy((*ppiRetBuffer)->GetData(), m_pbyRecvBuffer + m_uRecvHeadPos, uPackSize);
    m_uRecvHeadPos += uPackSize;

    //If we have a decrypt function.
    if (m_pfnDecodeFunction)
    {
        nRetCode = m_pfnDecodeFunction((*ppiRetBuffer)->GetSize(), (unsigned char *)(*ppiRetBuffer)->GetData(), &m_uRecvKey);
        LOG_PROCESS_ERROR(nRetCode);
    }

    nResult = 1;

EXIT0:    
    if (1 != nResult)
    {
        if (ppiRetBuffer)
        {
            SAFE_RELEASE(*ppiRetBuffer);
        }
    }

    return nResult;
}


int QAsyncSocketStream::IsAlive()
{
    int nRetsult = false;
    int nRetCode = false;
    volatile int nData = false; // no use


    PROCESS_ERROR(m_nSocketHandle != -1);

    nRetCode = send(m_nSocketHandle, (char *)&nData, 0, 0);
    PROCESS_ERROR(nRetCode != -1);

    nRetsult = true;
EXIT0:
    return nRetsult;
}

int QAsyncSocketStream::GetRemoteAddress(struct in_addr *pRemoteIP, u_short *pusRemotePort)
{
    int nResult  = false;
    int nRetCode = false;

    PROCESS_ERROR(m_nSocketHandle != -1);

    if (pRemoteIP)
        *pRemoteIP  = m_RemoteAddress.sin_addr;

    if (pusRemotePort)
        *pusRemotePort = m_RemoteAddress.sin_port;

    nResult = true;
EXIT0:    
    return nResult;
}

#ifdef WIN32
int QAsyncSocketStream::OnRecvCompleted(
    DWORD dwErrorCode, 
    DWORD dwNumberOfBytesTransfered, 
    LPOVERLAPPED lpOverlapped
)
{
    int nResult = false;

    m_nRecvCompletedErrorCode   = dwErrorCode;
    m_nRecvCompletedSize        = dwNumberOfBytesTransfered;
    m_nRecvCompletedFlag        = true;
    m_nWaitNotifyFlag           = true;

    nResult = true;
//EXIT0:
    return nResult;
}

int QAsyncSocketStream::OnWaitNotifyed()
{
    int nResult = false;

    m_nWaitNotifyFlag = false;

    nResult = true;
//EXIT0:
    return nResult;
}
#endif

// class _AsyncSocketStream end
//////////////////////////////////////////////////////////////////////////

// =============================================================================================
// Class _SocketServerAcceptor begin
int QSocketServerAcceptor::Init(
    const char cszIPAddress[], int nPort,
    int nMaxAcceptEachWait,
    int nMaxRecvBufSizePerSocket,
    int nMaxSendBufSizePerSocket,
    ENCODE_DECODE_MODE EncodeDecodeMode, 
    void *pvContext
)
{
    int nResult  = false;
    int nRetCode = false;

    PROCESS_ERROR(nMaxAcceptEachWait > 0);
    PROCESS_ERROR(nMaxRecvBufSizePerSocket > 0);
    PROCESS_ERROR(nMaxSendBufSizePerSocket > 0);
    USE_ARGUMENT(pvContext);

    m_nMaxAcceptEachWait       = nMaxAcceptEachWait;
    m_nMaxRecvBufSizePerSocket = nMaxRecvBufSizePerSocket;
    m_nMaxSendBufSizePerSocket = nMaxSendBufSizePerSocket;
    m_nListenSocket            = 0;

    m_EncodeDecodeMode = EncodeDecodeMode;

    nRetCode = _CreateListenSocket(cszIPAddress, nPort, &m_nListenSocket);
    PROCESS_ERROR(nRetCode);

    nRetCode = _SetSocketNoBlock(m_nListenSocket);
    PROCESS_ERROR(nRetCode);

#if  defined(WIN32)
	m_nLastWaitPos = 0;
	m_SocketVector.clear();
#elif defined(__FreeBSD__) || defined(__APPLE__)
	{
		typedef void (*SignalHandlerPointer)(int);
		SignalHandlerPointer pSignalHandler = SIG_ERR;

		m_nEpollHandle = -1;
		m_SocketEventVector.clear();

		pSignalHandler = signal(SIGPIPE, SIG_IGN);
		PROCESS_ERROR(pSignalHandler != SIG_ERR);  // write when connection reset.

		m_nEpollHandle = kqueue();
		PROCESS_ERROR(m_nEpollHandle != -1);
	}
#elif defined(__linux)
    // linux
    {
        typedef void (*SignalHandlerPointer)(int);
        SignalHandlerPointer pSignalHandler = SIG_ERR;

        m_nEpollHandle = -1;
        m_SocketEventVector.clear();

        pSignalHandler = signal(SIGPIPE, SIG_IGN);
        PROCESS_ERROR(pSignalHandler != SIG_ERR);  // write when connection reset.

        m_nEpollHandle = epoll_create(256);
        PROCESS_ERROR(m_nEpollHandle != -1);
    }
#endif

#ifdef USE_SOCKET_INFO_LOGGER
	g_InfoLogger.Init();
#endif

    nResult = true;
EXIT0:
    if (!nResult)
    {
        #ifdef WIN32
        #else
            // linux
            if (m_nEpollHandle != -1)
            {
                close(m_nEpollHandle);
                m_nEpollHandle = -1;
            }
        #endif

        if (m_nListenSocket != -1)
        {
            _CloseSocket(m_nListenSocket);
            m_nListenSocket = -1;
        }
    }

    return nResult;
}
                                                      
int QSocketServerAcceptor::UnInit(void *pvContext)
{
    int nResult  = false;
    int nRetCode = false;


    #ifdef WIN32
        nRetCode = _ProcessSocketVectorDestory();
        ASSERT(nRetCode);
    #else
        // linux
        if (m_nEpollHandle != -1)
        {
            close(m_nEpollHandle);
            m_nEpollHandle = -1;
        }
    #endif

    if (m_nListenSocket != -1)
    {
        _CloseSocket(m_nListenSocket);
        m_nListenSocket = -1;
    }

#ifdef USE_SOCKET_INFO_LOGGER
	g_InfoLogger.UnInit();
#endif

    nResult = true;
//EXIT0:
    return nResult;
}

#ifdef WIN32

static VOID WINAPI IOCompletionCallBack(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped
)
{
    int nRetCode = false;
    QAsyncSocketStream *pSocketStream = CONTAINING_RECORD(lpOverlapped, QAsyncSocketStream, m_RecvWSOVL);
    ASSERT(pSocketStream);

    if (dwNumberOfBytesTransfered == 0) // For byte streams, zero bytes having been read (as indicated by a zero
    {                                   // return value to indicate success, and lpNumberOfBytesRecvd value of
        dwErrorCode = WSAEDISCON;       // zero) indicates graceful closure and that no more bytes will ever be
    }                                   // read. (See MSDN:WSARecv() for more information)

    nRetCode = pSocketStream->OnRecvCompleted(dwErrorCode, dwNumberOfBytesTransfered, lpOverlapped);
    ASSERT(nRetCode);
}

#endif

// return -2: again, -1: error, 0: timeout, 1: success
static int _AcceptToAsyncSocketStream(
    int nListenSocket, int nMaxRecvSizePerSocket, int nMaxSendSizePerSocket, 
    ENCODE_DECODE_MODE EncodeDecodeMode,
    int nEpollHandle, 
    QAsyncSocketStream **ppRetSocketStream
)
{
    int         nResult         = -1;
    int         nRetCode        = false;
    int         nSocket         = -1;
    socklen_t   nAddrLen        = 0;
    unsigned    uServerKey      = 0;
    unsigned    uClientKey      = 0;
    struct sockaddr_in RemoteAddr;
    int nSocketStreamInitFlag   = false;
    QAsyncSocketStream *pSocketStream = NULL;

    PROCESS_ERROR(nListenSocket != -1);
    PROCESS_ERROR(nMaxRecvSizePerSocket > 0);
    PROCESS_ERROR(nMaxSendSizePerSocket > 0);
    PROCESS_ERROR(ppRetSocketStream);
    #ifdef WIN32
        PROCESS_ERROR(nEpollHandle == -1);
    #else
        // linux
        PROCESS_ERROR(nEpollHandle != -1);
    #endif

    while (true)
    {
        nAddrLen = sizeof(sockaddr_in);
        memset((void *)&RemoteAddr, 0, sizeof(sockaddr_in));
        
        nSocket = (int)accept(nListenSocket, (struct sockaddr *)&RemoteAddr, &nAddrLen);
        if (nSocket == -1)
        {
            nRetCode = _IsSocketCanRestore();
            if (nRetCode)   // if can restore then continue
                continue;
                
            nRetCode = _IsSocketWouldBlock();
            if (nRetCode)
            {
                PROCESS_ERROR_RET_CODE(false, -2);
            }

            goto EXIT0;
        }

        nRetCode = _SendSecurityKey(nSocket, EncodeDecodeMode, &uServerKey, &uClientKey);
        if (1 != nRetCode)
        {
            if (nSocket != -1)
            {
                _CloseSocket(nSocket);
                nSocket = -1;
            }
            continue;
        }

        pSocketStream = new QAsyncSocketStream();
        PROCESS_ERROR(pSocketStream);

#if defined(WIN32)
        nRetCode = BindIoCompletionCallback((HANDLE)nSocket, IOCompletionCallBack, 0);
        PROCESS_ERROR(nRetCode);
#elif defined(__FreeBSD__) || defined(__APPLE__)
		{
			struct kevent ev;
			EV_SET(&ev, nSocket, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, pSocketStream);

			nRetCode = kevent(nEpollHandle, &ev, 1, NULL, 0, NULL);
			PROCESS_ERROR(nRetCode >= 0);
		}
#elif defined(__linux)
		// linux
		{
    		epoll_event ev;
    		ev.events   = EPOLLIN | EPOLLET;
    		ev.data.ptr = (void *)pSocketStream;

			nRetCode = epoll_ctl(nEpollHandle, EPOLL_CTL_ADD, nSocket, &ev);
			PROCESS_ERROR(nRetCode >= 0);
		}
#endif

        nRetCode = pSocketStream->Init(
            nSocket, &RemoteAddr, 
            nMaxRecvSizePerSocket, nMaxSendSizePerSocket, 
            uServerKey, uClientKey, EncodeDecodeMode, 
            NULL
        );
        PROCESS_ERROR(nRetCode);
        nSocket = -1;
        nSocketStreamInitFlag = true;
        break;
    }
    
    nResult = 1;
EXIT0:
    if (nResult == -1)
    {
        if (nSocketStreamInitFlag)
        {
            pSocketStream->UnInit(NULL);
            nSocketStreamInitFlag = false;
        }

        SAFE_DELETE(pSocketStream);
    }

    if (nSocket != -1)
    {
        _CloseSocket(nSocket);
        nSocket = -1;
    }

    if (ppRetSocketStream)
    {
        *ppRetSocketStream = pSocketStream;
    }

    return nResult;
}


int QSocketServerAcceptor::_WaitProcessAccept(int nEventCount, QSOCKET_EVENT *pEvent, int *pnRetEventCount)
{
    int nResult  = false;
    int nRetCode = false;
    int nProcessEventCount = 0;
    QAsyncSocketStream *pSocketStream = NULL;
    int nEpollHandle = -1;

    PROCESS_ERROR(nEventCount > 0);
    PROCESS_ERROR(pEvent);
    PROCESS_ERROR(pnRetEventCount);
    PROCESS_ERROR(*pnRetEventCount <= nEventCount);
    PROCESS_SUCCESS(*pnRetEventCount == nEventCount);

    #ifdef WIN32
        nEpollHandle = -1;
    #else
        // linux
        nEpollHandle = m_nEpollHandle;
    #endif

    while (*pnRetEventCount < nEventCount)
    {
        nRetCode = _AcceptToAsyncSocketStream(
            m_nListenSocket, 
            m_nMaxRecvBufSizePerSocket,     m_nMaxSendBufSizePerSocket, 
            m_EncodeDecodeMode,
            nEpollHandle, 
            &pSocketStream
        );
        if (nRetCode == -2)     // AGAIN
            break;
        PROCESS_ERROR(nRetCode == 1);

        ASSERT(pSocketStream);
        #ifdef WIN32
            m_SocketVector.push_back(pSocketStream);
        #endif

        pEvent[*pnRetEventCount].uEventFlag = QSOCKET_EVENT_ACCEPT;
        pEvent[*pnRetEventCount].piSocket   = pSocketStream;
        pSocketStream = NULL;

        nProcessEventCount++;
        (*pnRetEventCount)++;
        if (nProcessEventCount >= m_nMaxAcceptEachWait)
            break;
    }

EXIT1:
    nResult = true;
EXIT0:

    if (!nResult)
    {
        SAFE_RELEASE(pSocketStream);

        while (nProcessEventCount--)
        {
            (*pnRetEventCount)--;
            pEvent[*pnRetEventCount].uEventFlag = 0;
            pSocketStream = static_cast<QAsyncSocketStream *>(pEvent[*pnRetEventCount].piSocket);
            pEvent[*pnRetEventCount].piSocket   = NULL;

            SAFE_RELEASE(pSocketStream);
        }
    }
    return nResult;
}

#ifdef WIN32

int QSocketServerAcceptor::_WaitProcessRecvOrDestory(int nEventCount, QSOCKET_EVENT *pEvent, int *pnRetEventCount)
{
    int nResult  = false;
    int nRetCode = false;
    int nProcessCount = 0;
    QSOCKET_VECTOR::iterator itBegin;
    QSOCKET_VECTOR::iterator itEnd;
    QSOCKET_VECTOR::iterator it;
    QSOCKET_VECTOR::iterator itNext;

    PROCESS_ERROR(nEventCount > 0);
    PROCESS_ERROR(pEvent);
    PROCESS_ERROR(pnRetEventCount);
    PROCESS_ERROR(*pnRetEventCount <= nEventCount);
    PROCESS_SUCCESS(*pnRetEventCount == nEventCount);

    nProcessCount = (int)m_SocketVector.size();
    if (m_nLastWaitPos >= nProcessCount)
        m_nLastWaitPos = 0;

    itBegin = m_SocketVector.begin();
    itEnd   = m_SocketVector.end();
    itNext  = itBegin;
    advance(itNext, m_nLastWaitPos);

    while (nProcessCount--)
    {
        if (itNext == itEnd)
            itNext = itBegin;
        it = itNext++;
        
        QAsyncSocketStream *pSocketStream = static_cast<QAsyncSocketStream *>(*it);
        ASSERT(pSocketStream);

        //If not completed, then skip.
        nRetCode = pSocketStream->GetRecvCompletedFlag();
        if (!nRetCode)
            continue;

        //If should be delete, then delete it.
        nRetCode = pSocketStream->GetDelayDestoryFlag();
        if (nRetCode)
        {
            itNext  = m_SocketVector.erase(it);
            itBegin = m_SocketVector.begin();
            itEnd   = m_SocketVector.end();

            pSocketStream->Destory();
            pSocketStream = NULL;
            continue;
        }

        nRetCode = pSocketStream->GetWaitNotifyFlag();
        if (!nRetCode)
            continue;

        //If we have reach the max events count, skip.
        if ((*pnRetEventCount) >= nEventCount)
            continue;

        pSocketStream->OnWaitNotifyed();

        pEvent[*pnRetEventCount].uEventFlag = QSOCKET_EVENT_IN;
        pEvent[*pnRetEventCount].piSocket   = pSocketStream;
        pEvent[*pnRetEventCount].piSocket->AddRef();

        pSocketStream = NULL;

        (*pnRetEventCount)++;
    }
    
    m_nLastWaitPos = (int)distance(itBegin, itNext) + 1;

EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

int QSocketServerAcceptor::_ProcessSocketVectorDestory()
{
    int nResult  = false;
    int nRetCode = false;
    QSOCKET_VECTOR::iterator it;
    int nDestoryCount = 0;

    // To avoid the difference of erase the current item from vector and list, and avoid 
    // movement of existing container elements when erasures take place, we choose not 
    // erase item in loop, but make it a NULL value, then clear all of them at once just 
    // before function's return.
    while (true)
    {
        for (it = m_SocketVector.begin(); it != m_SocketVector.end(); ++it)
        {
            if (!(*it))
                continue;

            QAsyncSocketStream *pSocketStream = static_cast<QAsyncSocketStream *>(*it);
            ASSERT(pSocketStream);

            nRetCode = pSocketStream->GetDelayDestoryFlag();
            if (!nRetCode)
            {
                QLogPrintf(
                    LOG_DEBUG,
                    "UnRelease piSocketStream = %p\n",
                    pSocketStream
                );
                SAFE_RELEASE(pSocketStream);  // Release make sure to exit
                continue;
            }

            nRetCode = pSocketStream->GetRecvCompletedFlag();
            if (!nRetCode)
                continue;


            pSocketStream->Destory();
            pSocketStream = NULL;
            *it = NULL;
            ++nDestoryCount;
        }

        if (nDestoryCount == m_SocketVector.size())
            break;

        QThread_Sleep(50);
    }

    nResult = true;
//EXIT0:
    m_SocketVector.clear();
    return nResult;
}

#else

int QSocketServerAcceptor::_WaitProcessRecv(int nEventCount, QSOCKET_EVENT *pEvent, int *pnRetEventCount)
{
	int nResult   = false;
	int nRetCount = 0;
	int nRemainEventCount = 0;
#if defined(__FreeBSD__) || defined(__APPLE__)
	struct kevent *pEpollEvent     = NULL;
	struct timespec tmWait = { 0, 0 };
#else
	struct epoll_event* pEpollEvent = NULL;
#endif

	PROCESS_ERROR(m_nEpollHandle != -1);

	PROCESS_ERROR(nEventCount > 0);
	PROCESS_ERROR(pEvent);
	PROCESS_ERROR(pnRetEventCount);
	PROCESS_ERROR(*pnRetEventCount <= nEventCount);
	PROCESS_SUCCESS(*pnRetEventCount == nEventCount);

	nRemainEventCount = nEventCount - (*pnRetEventCount);

	m_SocketEventVector.resize(nRemainEventCount);

	pEpollEvent = &m_SocketEventVector[0];
#if defined(__FreeBSD__) || defined(__APPLE__)
	nRetCount = kevent(m_nEpollHandle, NULL, 0, pEpollEvent, nRemainEventCount, &tmWait);
#else // linux
	nRetCount = epoll_wait(m_nEpollHandle, pEpollEvent, nRemainEventCount, 0);
#endif
	PROCESS_ERROR(nRetCount >= 0);

	while (nRetCount--)
	{
#if defined(__FreeBSD__) || defined(__APPLE__)
		pEvent[*pnRetEventCount].uEventFlag = pEpollEvent->flags;
		pEvent[*pnRetEventCount].piSocket   = (ISocketStream *)pEpollEvent->udata;

		pEvent[*pnRetEventCount].uEventFlag |= QSOCKET_EVENT_IN;
#else // linux
		pEvent[*pnRetEventCount].uEventFlag = pEpollEvent->events;
		pEvent[*pnRetEventCount].piSocket   = (ISocketStream *)((pEpollEvent->data).ptr);
#endif
		pEvent[*pnRetEventCount].piSocket->AddRef();

		++pEpollEvent;
		++(*pnRetEventCount);
	}

EXIT1:
	nResult = true;
EXIT0:
	return nResult;
}

#endif // WIN32

int QSocketServerAcceptor::Wait(int nEventCount, QSOCKET_EVENT *pEvent, int *pnRetEventCount)
{
    int nResult  = false;
    int nRetCode = false;

    PROCESS_ERROR(nEventCount > 0);
    PROCESS_ERROR(pEvent);
    PROCESS_ERROR(pnRetEventCount);

    *pnRetEventCount = 0;

    nRetCode = _WaitProcessAccept(nEventCount, pEvent, pnRetEventCount);
    PROCESS_ERROR(nRetCode);

    #if defined(WIN32)
        nRetCode = _WaitProcessRecvOrDestory(nEventCount, pEvent, pnRetEventCount);
        PROCESS_ERROR(nRetCode);
    #else
        //linux
        nRetCode = _WaitProcessRecv(nEventCount, pEvent, pnRetEventCount);
        PROCESS_ERROR(nRetCode);
    #endif

    nResult = true;
EXIT0:
    return nResult;
}
// Class _SocketServerAcceptor end
// =============================================================================================

#ifdef USE_SOCKET_INFO_LOGGER
int _QSocketInfoLogger::Init()
{
	int nResult  = false;
	int nRetCode = false;

    m_nConnectCount = 0;
    m_nDisConnectCount = 0;

    QLogPrintf(LOG_INFO, "[*START*]");

	nResult = true;
//EXIT0:
	return nResult;
}

void _QSocketInfoLogger::UnInit()
{
	QLogPrintf(LOG_INFO, "[**END**] Connect %d time(s), disconnect %d time(s)", m_nConnectCount, m_nDisConnectCount);
}

void _QSocketInfoLogger::OnConnect(ISocketStream *piStream)
{
	struct in_addr RemoteIP;
	u_short usRemotePort;

	PROCESS_ERROR(piStream);

	piStream->GetRemoteAddress(&RemoteIP, &usRemotePort);
	QLogPrintf(LOG_INFO, "[CONNECT] 0x%p with %s:%u", piStream, inet_ntoa(RemoteIP), usRemotePort);
	m_nConnectCount++;

EXIT0:
	return;
}

void _QSocketInfoLogger::OnDisConnect(ISocketStream *piStream)
{
	PROCESS_ERROR(piStream);

	QLogPrintf(LOG_INFO, "[DISCONN] 0x%p", piStream);
	m_nDisConnectCount++;

EXIT0:
	return;
}
#endif
