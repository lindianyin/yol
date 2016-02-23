/* -------------------------------------------------------------------------
//	文件名		：	kscriptenv.h
//	创建者		：	liuchang
//	创建时间	：	2010-6-24 9:56:10
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KSCRIPTENV_H__
#define __KSCRIPTENV_H__

#include "onlinegamebase/kscriptmanager.h"
// -------------------------------------------------------------------------
class KGcScriptEnv : public IKGcScriptEnv
{
public:
	KScriptManager& GetScriptManager()
	{
		return g_cScriptManager;
	}

	static KGcScriptEnv* Inst()
	{
		static KGcScriptEnv s_Inst;
		return &s_Inst;
	}
};
// -------------------------------------------------------------------------

#endif /* __KSCRIPTENV_H__ */
