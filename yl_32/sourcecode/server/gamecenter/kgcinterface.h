/* -------------------------------------------------------------------------
//	文件名		：	kgcinterface.h
//	创建者		：	luobaohang
//	创建时间	：	2010/6/9 15:02:12
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGCINTERFACE_H__
#define __KGCINTERFACE_H__

#include "serverbase/kgcmoduleinterface.h"
#include "kgcinterfacedef.h"

// -------------------------------------------------------------------------
class KInterfaceAcceptor : public IKInterfaceAcceptor
{
public:
	KInterfaceAcceptor();
	KInterfaceAcceptor(BYTE byOwnerModuleId);

	virtual BOOL Send(BYTE byConnectorModuleId, LPVOID pvData, UINT uDataSize);
	virtual BOOL IsConnectValid(BYTE byConnectorModuleId);

	BYTE GetOwnerModuleId();
protected:
	BYTE m_byOwnerModuleId;			// Accept 所在的模块
};

class KInterfaceConnector : public IKInterfaceConnector
{
public:
	KInterfaceConnector();
	KInterfaceConnector(BYTE byAcceptorModuleId, BYTE byOwnerModuleId, BOOL bEssencial);
	// 向连接发送数据
	virtual BOOL Send(LPVOID pvData, UINT uDataSize);
	// 检查连接是否有效
	virtual BOOL IsConnectValid(); 

	BYTE GetAcceptorModuleId();
	BYTE GetOwnerModuleId();

	BOOL IsEssencial();
protected:
	BYTE m_byAcceptorModuleId;		// Acceptor模块Id
	BYTE m_byOwnerModuleId;			// Connector 所在的模块
	BOOL m_bEssencial;
};

// -------------------------------------------------------------------------

#endif /* __KINTERFACE_H__ */
