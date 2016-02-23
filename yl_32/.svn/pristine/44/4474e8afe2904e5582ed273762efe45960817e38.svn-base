/* -------------------------------------------------------------------------
//	文件名		：	KSocketListener.h
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:23:56
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KSOCKETLISTENER_H__
#define __KSOCKETLISTENER_H__

#include "ksocketinterface.h"

// -------------------------------------------------------------------------

struct KCONNECTION_DATA
{
	KCONNECTION_DATA()/* : SocketPacker(4096)*/
	{
		nConnectId = 0; piSocketStream = NULL;
	}

	INT nConnectId;
	ISocketStream* piSocketStream;
	//KG_Packer SocketPacker;
};

class KSocketListener : public IKSocketInterface
{
public:
	KSocketListener();
	~KSocketListener();
	BOOL Init(IKSocketEventHandler* piEventProcessor);
	// 打开所有连接
	BOOL SocketOpen();
	// 关闭所有连接
	BOOL SocketClose();
	BOOL Disconnect(INT nConnectId);
	// 发送数据、事件
	BOOL Send(INT nConnectId, IMemBlock* piBuffer);
	// 消息循环，接收数据、事件并处理
	BOOL MessageLoop();
	// 获得本机Actived地址
	BOOL GetHostIP(KHOST_ADDRESS& rcAddress);

	BOOL GetLocalIpAddress(DWORD dwInAdapterIdx, DWORD dwOutAdapterIdx, CHAR szInIp[], CHAR szOutIp[]);
protected:
	BOOL CloseConnect(INT nConnectId);
protected:
	QSocketServerAcceptor	m_cSockServer;	// Server Linstener，用于监听GameClient
	QSOCKET_EVENT* m_pSockEvents;	// 监听事件的缓冲区指针
	INT m_nMaxClients;	// 最大连接数
	QIdPool m_cIdPool;	// 连接ID分配器
	KCONNECTION_DATA* m_pConnData;
	IKSocketEventHandler* m_piEventProcessor;
	UINT m_uLastLoopElapse;
private:
	KHOST_ADDRESS	m_cParam;
};

// -------------------------------------------------------------------------

#endif /* __KSOCKETLISTENER_H__ */
