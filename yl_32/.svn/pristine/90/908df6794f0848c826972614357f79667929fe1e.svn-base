/* -------------------------------------------------------------------------
//	文件名		：	kgcconfig.h
//	创建者		：	luobaohang
//	创建时间	：	2010/6/21 10:51:06
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGCCONFIG_H__
#define __KGCCONFIG_H__
#include "serverbase/kgcconfiginterface.h"
// -------------------------------------------------------------------------
class KGcConfig : public IKGcConfig
{
public:
	KGcConfig();
	static KGcConfig* Inst();
	BOOL LoadConfig();
	//virtual DB_SERVER_PARAM& DbServerParam();
	KGCGW_PARAM& GetGcParam();
private:
	//DB_SERVER_PARAM m_cDbParam;
	KGCGW_PARAM m_cGcParam;
};

// -------------------------------------------------------------------------

#endif /* __KGCCONFIG_H__ */
