/* -------------------------------------------------------------------------
//	文件名		：	ksocketinterface.h
//	创建者		：	luobaohang
//	创建时间	：	2009-7-6 14:58:11
//	功能描述	：	Socket层与逻辑层的接口
//
// -----------------------------------------------------------------------*/
#ifndef __KSOCKETINTERFACE_H__
#define __KSOCKETINTERFACE_H__

struct IMemBlock;

class IKSocketEventHandler
{
public:
	virtual BOOL Init(INT nMaxConnectId) = 0;
	virtual BOOL ProcessEvent(INT nConnectId, INT nEventId, LPCVOID pData, INT nData) = 0;
	virtual BOOL ProcessPacket(INT nConnectId, IMemBlock* piPacketBuffer) = 0;
	virtual BOOL Activate() PURE;
};

#define KD_IPADDRESS_LEN 16
struct KHOST_ADDRESS
{
	CHAR szIntranetIp[KD_IPADDRESS_LEN]; // 内网Ip
	CHAR szInternetIp[KD_IPADDRESS_LEN]; // 外网Ip
	INT nListenPort;
};

// 定义程序与外部数据交互的统一接口
class IKSocketInterface
{
public:
	virtual BOOL Init(IKSocketEventHandler* piEventProcessor) = 0;
	// 打开所有连接
	virtual BOOL SocketOpen() = 0;
	// 关闭所有连接
	virtual BOOL SocketClose() = 0;
	// 断开某个链接
	virtual BOOL Disconnect(INT nConnectId) = 0;
	// 发送数据、事件
	virtual BOOL Send(INT nConnectId, IMemBlock* piBuffer) = 0;
	// 消息循环，接收数据、事件并处理
	virtual BOOL MessageLoop() = 0;
	// 获得本机监听的地址(内网和外网)
	virtual BOOL GetHostIP(KHOST_ADDRESS& rcAddress) = 0;
};

// SOCKET逻辑事件定义
enum KE_SOCKET_CONNECT_EVENT
{
	emKSOCKET_EVENT_BEGIN = 0,
	emKSOCKET_EVENT_CREATE,
	emKSOCKET_EVENT_LOST,
	emKSOCKET_EVENT_CLOSE, // 链接主动关闭
	emKSOCKET_EVENT_MASS, //  数据包太多
};

extern IKSocketInterface* g_piSocketServer; // gameserver与客户端连接（服务）

class IKSocketEventProcessor_Client
{
public:
	virtual BOOL ProcessEvent(INT nEventId, LPCVOID pData, INT nData) = 0;
	virtual BOOL ProcessPacket(LPCBYTE pData, UINT uDataLen) = 0;
};

class IKSocketClientInterface
{
public:
	virtual BOOL Init(IKSocketEventProcessor_Client* piEventProcessor) = 0;
	virtual BOOL Activate() PURE;
	virtual BOOL Connect(DWORD dwIpAddress, USHORT uPort, const GUID* pGuid, BOOL bHidden) = 0;
	virtual BOOL Send(LPCVOID pData, UINT uDataLen) = 0;
	virtual BOOL ShutDown() = 0;
};

#endif /* __KSOCKETINTERFACE_H__ */
