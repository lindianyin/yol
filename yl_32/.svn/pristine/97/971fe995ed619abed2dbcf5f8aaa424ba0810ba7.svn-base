/* -------------------------------------------------------------------------
//	文件名		：	kgcinterfacedef.h
//	创建者		：	luobaohang
//	创建时间	：	2010/6/9 16:08:54
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGCINTERFACEDEF_H__
#define __KGCINTERFACEDEF_H__

#include "serverbase/kinterfacedef.h"
#include "serverbase/kgcmoduledef.h"

enum KE_CONNECT_TYPE
{
	emKGC_CONNECT_LOCAL,
	emKGC_CONNECT_PIPE,
	emKGC_CONNECT_SOCKET,
};
// -------------------------------------------------------------------------
class KConnectBase
{
public:
	KConnectBase(KE_CONNECT_TYPE eConnectType):	m_eConnectType(eConnectType){}

	virtual BOOL Send(BYTE byAcceptorModuleId, BYTE byConnectId, LPVOID pvData, UINT uDataSize) = 0;

	virtual BOOL IsValid() = 0;
public:	
	KE_CONNECT_TYPE m_eConnectType; // 连接的类型
};

// -------------------------------------------------------------------------

#endif /* __KGCINTERFACEDEF_H__ */
