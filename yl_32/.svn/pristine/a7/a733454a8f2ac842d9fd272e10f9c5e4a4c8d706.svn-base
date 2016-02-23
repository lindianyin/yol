/* -------------------------------------------------------------------------
//	文件名		：	nsworld/knsbaserequire.h
//	创建者		：	luobaohang
//	创建时间	：	2009/12/30 16:09:15
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __NSWORLD_KNSBASEREQUIRE_H__
#define __NSWORLD_KNSBASEREQUIRE_H__

#include "onlinegameworld/krequireinterface.h"
// -------------------------------------------------------------------------
class KNsBaseRequire : public IKNsBaseRequire
{
public:
	BOOL Init();
	// 获取环境配置参数
	virtual const KENV_CONFIG* GetEnvConfig();
	virtual IKModuleInterface** GetModuleList();

private:
	KENV_CONFIG	m_sEnvCfg;
};

KNsBaseRequire* g_InitNsBaseRequire();
// -------------------------------------------------------------------------

#endif /* __NSWORLD_KNSBASEREQUIRE_H__ */
