/* -------------------------------------------------------------------------
//	文件名		：	kenemyfactory.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "enemyfactory.h"
#include "relationcounter.h"
#include "KRole.h"
#include "GameCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------


KEnemyFactory::KEnemyFactory() : SUPER_CLASS(emKPLAYERRELATION_TYPE_ENEMEY, TRUE, FALSE, TRUE)
{
	
}

KEnemyFactory::~KEnemyFactory()
{

}

BOOL KEnemyFactory::CreateRelation( INT nMaster, INT nTarget, BOOL bIsMasterActive, IKRelation** ppRelation )
{
	BOOL					bRetCode	= FALSE;
	KRole*					pMaster		= NULL;
	KRole*					pTarget		= NULL;

	LOG_PROCESS_ERROR(nMaster && nTarget);

	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	pTarget	= g_pGameCenter->m_RoleManager.GetRole(nTarget);
	LOG_PROCESS_ERROR(pMaster);
	LOG_PROCESS_ERROR(pTarget);

	bRetCode = CanCreateRelation(nMaster, nTarget) && 
		SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, ppRelation);

EXIT0:
	return bRetCode;

// 	KGC_CONFIRM_RET_FALSE(nMaster && nTarget);
// 
// 	KGC_Player* pMasterPlayer = g_pPlayerManager->GetPlayer(nMaster);
// 	KGC_Player* pTargetPlayer = g_pPlayerManager->GetPlayer(nTarget);
// 	KGC_CONFIRM_RET_FALSE(pMasterPlayer && pTargetPlayer);
// 
// 	if (CanCreateRelation(nMaster, nTarget) && 
// 		SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, ppRelation))
// 	{
// 		return TRUE;
// 	}
// 	else
// 	{
// 		KGC_CONFIRM_RET_FALSE(FALSE);
// 	}
}


BOOL KEnemyFactory::CanCreateRelation(INT nMaster, INT nTarget)
{
	if (!KRelationCounter::Instance()->CheckReachMaxCount(emKPLAYERRELATION_TYPE_ENEMEY, nMaster) )
		return FALSE;
	BOOL bRet	= FALSE;
	IKRelation** ppRelation = NULL;
	if (nMaster != nTarget && !FindRelation(nMaster, nTarget, ppRelation))
		bRet	= TRUE;
	return bRet;
}

// 仇人关系，玩家只记录，被谁杀了，不记录杀了谁
BOOL KEnemyFactory::SaveAll(INT nPlayerId, IKRelationSaver* pSaver)
{
	ASSERT(nPlayerId && pSaver);
	SUPER_CLASS::SaveAll(nPlayerId, &m_manPlayerIdMasters, &m_manPlayerIdTargets, TRUE, pSaver);
	return TRUE;
}
