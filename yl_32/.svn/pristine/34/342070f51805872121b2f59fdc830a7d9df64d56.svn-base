
#ifndef __KGCMONITORCLIENT_H__
#define __KGCMONITORCLIENT_H__

#include "stdafx.h"
// -------------------------------------------------------------------------
// TODO: 将socketinterface移植过来取代这个

// SOCKET逻辑事件定义
enum KE_SOCKET_CONNECT_EVENT
{
	emKSOCKET_EVENT_BEGIN = 0,
	emKSOCKET_EVENT_CREATE,
	emKSOCKET_EVENT_LOST,
	emKSOCKET_EVENT_CLOSE, // 链接主动关闭
	emKSOCKET_EVENT_MASS, //  数据包太多
};

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
	virtual BOOL Connect(LPCSTR pszIp, USHORT uPort) = 0;
	virtual BOOL Send(LPCVOID pData, UINT uDataLen) = 0;
	virtual BOOL ShutDown() = 0;
};

class KSocketClient : public IKSocketClientInterface
{
public:

	KSocketClient();
	~KSocketClient();
	BOOL	Init(IKSocketEventProcessor_Client* piEventProcessor);
	BOOL	Activate();

	BOOL	Connect(LPCSTR pszIp, USHORT uPort);
	BOOL	ShutDown(); // 主动断开同Server的连接
	BOOL	Send(LPCVOID pData, UINT uDataLen);
private:
	VOID	NotifyGameSvrDisconnect(VOID);
	INT		SocketThreadFunction(VOID); // 工作线程（接收）
	BOOL	ProcessPacket(IMemBlock* piBuffer);
	static VOID WorkThreadFunction(LPVOID pThis);

private:
	QBufferedSocketConnector m_sockConnector;
	IMemBlock*	 m_pGameSvrStream;
	QThread m_cSocketThread; // 网络线程

	BOOL m_bRunStatus;
	BOOL m_bLostConnection; // 是否需要通知 连接断开
	IKSocketEventProcessor_Client* m_pProcessor;

	QLock m_cShutdownLock;	// 网络连接关闭锁
	// 读取数据包改成无锁方式 QMutex m_cPacketLock;
	std::vector<IMemBlock *> m_vecPackagReceive[2];
	INT m_nReceivePos;
	INT m_nReading;	// 读取标志
};

// -------------------------------------------------------------------------

#endif /* __KGCMONITORCLIENT_H__ */
