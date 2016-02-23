/* -------------------------------------------------------------------------
//	文件名		：	kconnectpipe.h
//	创建者		：	luobaohang
//	创建时间	：	2010/7/28 9:59:16
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KCONNECTPIPE_H__
#define __KCONNECTPIPE_H__

#include "serverbase/kgcmoduleinterface.h"
#include "kgcinterfacedef.h"
// -------------------------------------------------------------------------

class KConnectPipe : public KConnectBase
{
public:
	KConnectPipe(BYTE byLoaderId);
	// 实现KConnectBase接口
	virtual BOOL Send(BYTE byAcceptorModuleId, BYTE byConnectId, LPVOID pvData, UINT uDataSize);
	virtual BOOL IsValid() { return m_bIsValid; }
	VOID SetValid(BOOL bValid) { m_bIsValid = bValid; }
private:
	BYTE m_byLoaderId; // 连接对象的LoaderId
	BOOL m_bIsValid;
};

class KConnectPipeMgr
{
public:
	static KConnectPipeMgr* Inst();
	KConnectPipe* NewConnect(BYTE byLoaderId);
private:
	std::list<KConnectPipe> m_lstConnect;	// 连接对象
};
// -------------------------------------------------------------------------

#endif /* __KCONNECTPIPE_H__ */
