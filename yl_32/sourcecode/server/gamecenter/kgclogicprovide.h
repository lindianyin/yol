/* -------------------------------------------------------------------------
//	文件名		：	kgclogicprovide.h
//	创建者		：	luobaohang
//	创建时间	：	2010/7/14 9:55:27
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGCLOGICPROVIDE_H__
#define __KGCLOGICPROVIDE_H__

#include "serverbase/kgclogicbaseinterface.h"
// -------------------------------------------------------------------------
class KGcLogicProvide : public IKGcLogicProvide
{
public:
	static KGcLogicProvide* Inst();
	KGcLogicProvide(void);
	~KGcLogicProvide(void);
private:
	// 注册逻辑层扩展接口（提供给gc模块Query调用）
	virtual BOOL RegisterExtInterface(REFIID rrid, LPVOID pvObject);
	// nConnectId：Gameserver连接号，-1表示发送给所有
	virtual BOOL Send(INT nConnectId, LPVOID pvData, UINT uDataSize);
	virtual BOOL RegisterGsProcessor(IKProcessServer* pIServer);
	virtual BOOL Send2OtherGc(INT nModuleId, LPVOID pData, UINT uDataSize);
};

// -------------------------------------------------------------------------

#endif /* __KGCLOGICPROVIDE_H__ */
