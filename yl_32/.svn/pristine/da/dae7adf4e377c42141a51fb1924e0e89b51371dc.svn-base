/* -------------------------------------------------------------------------
//	文件名		：	kcondchecker.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/27 17:09:03
//	功能描述	：	条件检测
//
// -----------------------------------------------------------------------*/
#ifndef __KCONDCHECKER_H__
#define __KCONDCHECKER_H__

// -------------------------------------------------------------------------
#ifdef GAME_CENTER

#include "kmaildatabase.h"
#include "gclogicbase/kgcplayercommon.h"

class KCondChecker
{
public:
	typedef BOOL (KCondChecker::*fnCheckCondition)(KGcPlayerCommon*, CONST KMAIL_CONDHEAD*, INT);
	struct KCOND_INFO
	{
		INT					nSize;
		fnCheckCondition	pfnCheckCondition;
	};

public:
	KCondChecker();
	~KCondChecker();
	static KCondChecker* Instance() {return &ms_inst;}
	BOOL CheckCondition(KGcPlayerCommon* pPlayer, CONST KMAIL_CONDHEAD* pCond, INT nCondLen);

private:
	// 检查玩家是否符合条件
	BOOL CheckCond_SexLevel(KGcPlayerCommon* pPlayer, CONST KMAIL_CONDHEAD* pHead, INT nCondLen);
	BOOL CheckCond_SendTime(KGcPlayerCommon* pPlayer, CONST KMAIL_CONDHEAD* pHead, INT nCondLen);

private:
	static KCondChecker		ms_inst;
	KCOND_INFO				m_aryCondition[256];		// 注册的条件检查函数
};

#endif /* GAME_CENTER */
// -------------------------------------------------------------------------

#endif /* __KCONDCHECKER_H__ */
