/* -------------------------------------------------------------------------
//	文件名		：	kconnectmgr.h
//	创建者		：	luobaohang
//	创建时间	：	2010/9/10 15:29:36
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KCONNECTMGR_H__
#define __KCONNECTMGR_H__

#include "kgcinterfacedef.h"
// -------------------------------------------------------------------------
class KConnectMgr
{
public:
	KConnectMgr(void);
	static KConnectMgr* Inst();
	BOOL SetConnect(BYTE byConnectorModuleId, KConnectBase* pConnect);
	KConnectBase* GetConnect(BYTE byConnectorModuleId);
private:
	KConnectBase* m_aConnect[256];
};

// -------------------------------------------------------------------------

#endif /* __KCONNECTMGR_H__ */
