/* -------------------------------------------------------------------------
//	文件名		：	serverbase/kgcbaseinterface.h
//	创建者		：	luobaohang
//	创建时间	：	2010/7/14 11:14:23
//	功能描述	：	gc基础接口定义
//
// -----------------------------------------------------------------------*/
#ifndef __SERVERBASE_KGCBASEINTERFACE_H__
#define __SERVERBASE_KGCBASEINTERFACE_H__

// -------------------------------------------------------------------------
// 虚拟连接接口（改为无连接）
interface IKInterfaceAcceptor
{
	// 向连接发送数据，nConnectId为-1时向所有连接发送
	virtual BOOL Send(BYTE byConnectorModuleId, LPVOID pvData, UINT uDataSize) = 0;
	// 检查连接是否有效，（即Connector所在模块是否已准备好，目前该接口无效，均返回TRUE）
	virtual BOOL IsConnectValid(BYTE byConnectorModuleId) = 0;
};

// 虚拟连接接口（改为无连接）
interface IKInterfaceConnector
{
	// 向接收端发送数据
	virtual BOOL Send(LPVOID pvData, UINT uDataSize) = 0;
	// 检查连接是否有效（即Acceptro所在模块是否已准备好，目前该接口无效，均返回TRUE）
	virtual BOOL IsConnectValid() = 0;
};

// Acceptor处理（无连接）
interface IKProcessAcceptor
{
	// 数据处理
	virtual BOOL ProcessData(BYTE byConnectorModuleId, LPBYTE pData, UINT uSize) PURE;
};

// Connector处理（无连接）
interface IKProcessConnector
{
	// 数据处理
	virtual BOOL ProcessData(LPBYTE pData, UINT uSize) PURE;
};

// 有连接服务端处理
interface IKProcessServer
{
	// 连接关闭
	virtual BOOL OnConnectClose(INT nConnectId) PURE;
	// 建立连接
	virtual BOOL OnConnected(INT nConnectId) PURE;
	// 数据处理
	virtual BOOL ProcessData(INT nConnectId, LPBYTE pData, UINT uSize) PURE;
};

// 有连接客户端处理
interface IKProcessClient
{
	// 建立连接
	virtual BOOL OnConnected() PURE;
	// 连接关闭
	virtual BOOL OnConnectClose() PURE;
	// 数据处理
	virtual BOOL ProcessData(LPBYTE pData, UINT uSize) PURE;
};
// -------------------------------------------------------------------------

#endif /* __SERVERBASE_KGCBASEINTERFACE_H__ */
