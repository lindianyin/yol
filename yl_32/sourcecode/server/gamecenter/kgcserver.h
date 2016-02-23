/* -------------------------------------------------------------------------
//	文件名		：	kgcserver.h
//	创建者		：	luobaohang
//	创建时间	：	2010/6/1 16:51:38
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGCSERVER_H__
#define __KGCSERVER_H__
#include "serverbase/kgcmoduleinterface.h"
#include "serverbase/kg_censerver.h"
#include "kgcinterface.h"
// -------------------------------------------------------------------------
//一个GC模块的Server端对象
class KGcServer : public IKGcServer
{
public:
	KGcServer(BYTE byModuleId);
	// nConnectId：Gameserver连接号，-1表示发送给所有
	virtual BOOL Send(INT nConnectId, LPVOID pvData, SIZE_T uDataSize);
	// send 2 random one
	virtual BOOL Send2Random(LPVOID pvData, UINT uDataSize);
	// 注册对Gameserver的协议处理
	virtual BOOL RegisterGsProcessor(IKProcessServer* piProcessor);
	// 注册本模块开放的Gc间连接接口（每模块只开放一个），并指定协议处理接口，须在模块初始化时调用
	virtual IKInterfaceAcceptor* RegisterGcAcceptor(IKProcessAcceptor* pIServer);
	// 注册一个跟其他Gc模块的连接，须在模块初始化时调用
	virtual IKInterfaceConnector* RegisterGcConnector(BYTE byModuleId,
		IKProcessConnector* pIClient,
		BOOL bIsEssencial);
protected:
	BYTE m_byModuleId;
};

// -------------------------------------------------------------------------

#endif /* __KGCSERVER_H__ */
