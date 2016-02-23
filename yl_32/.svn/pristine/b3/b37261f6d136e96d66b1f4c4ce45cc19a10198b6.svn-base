/* -------------------------------------------------------------------------
//	文件名		：	kchatprotocolprocess_gc.h
//	创建者		：	liuchang
//	创建时间	：	2010-6-24 11:14:49
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KCHATPROTOCOLPROCESS_GC_H__
#define __KCHATPROTOCOLPROCESS_GC_H__

// -------------------------------------------------------------------------

#include "serverbase/kgcmoduleinterface.h"
#include "kchatmodule_gc.h"

// -------------------------------------------------------------------------
// 处理GS协议的接收器
class KChatClientMsgProcessor : public IKProcessServer
{
public:
	virtual BOOL OnConnectClose(INT nConnectId);
	virtual BOOL OnConnected(INT nConnectId);
	virtual BOOL ProcessData(INT nConnectId, LPBYTE pData, UINT uSize);

private:
	BOOL ProcessReceiveMsg(INT nConnectId, LPBYTE pData, UINT uSize);
	BOOL ProcessSendMsgToPlayer(INT nConnectId, LPBYTE pData, UINT uSize);
};
extern KChatClientMsgProcessor g_ChatProcessAcceptor;


// -------------------------------------------------------------------------
// 处理GC的Team模块消息
class KChatMsgDispatcher_ForTeam : public IKProcessConnector
{
public:
	virtual BOOL OnConnected();
	virtual BOOL OnConnectClose();
	virtual BOOL ProcessData(LPBYTE pData, UINT uSize);
};

// -------------------------------------------------------------------------
// 处理GC的Kin模块消息
class KChatMsgDispatcher_ForKin : public IKProcessConnector
{
public:
	virtual BOOL OnConnected();
	virtual BOOL OnConnectClose();
	virtual BOOL ProcessData(LPBYTE pData, UINT uSize);
};


// -------------------------------------------------------------------------
// 处理GC的Group模块消息
class KChatMsgDispatcher_ForGroup : public IKProcessConnector
{
public:
	virtual BOOL OnConnected();
	virtual BOOL OnConnectClose();
	virtual BOOL ProcessData(LPBYTE pData, UINT uSize);
};


// -------------------------------------------------------------------------
// Chat模块对其它GC模块的连接工厂
class KChatModuleConnectorFactory : public IKModuleConnectorFactory
{
public:
	virtual IKProcessConnector* GetModuleChatProcess(KE_GC_MODULE_ID emModuleId);
};
extern KChatModuleConnectorFactory g_cChatModuleConnectorFactory;

// -------------------------------------------------------------------------

#endif /* __KCHATPROTOCOLPROCESS_GC_H__ */
