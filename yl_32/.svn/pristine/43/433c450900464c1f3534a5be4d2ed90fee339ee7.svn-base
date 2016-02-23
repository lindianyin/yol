/* -------------------------------------------------------------------------
//	文件名		：	kcouplefactory.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	夫妻关系工厂
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "couplefactory.h"
#include "relationcounter.h"
#include "KRole.h"
#include "GameCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KCoupleFactory::KCoupleFactory() : SUPER_CLASS(emKPLAYERRELATION_TYPE_COUPLE, TRUE, FALSE, TRUE)
{

}

KCoupleFactory::~KCoupleFactory()
{

}

BOOL KCoupleFactory::CreateRelation( INT nMaster, INT nTarget, BOOL bIsMasterActive, IKRelation** ppRelation)
{
	BOOL					bRetCode	= FALSE;
	KRole*					pMaster		= NULL;
	KRole*					pTarget		= NULL;

	LOG_PROCESS_ERROR(nMaster && nTarget);
	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
	LOG_PROCESS_ERROR(pMaster);
	LOG_PROCESS_ERROR(pTarget);

	bRetCode = CanCreateRelation(nMaster, nTarget) && 
		SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, ppRelation);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;

// 	BOOL bRet = FALSE;
// 	do 
// 	{
// 		KGC_CONFIRM_BREAK(nMaster && nTarget);
// 		KGC_Player* pMasterPlayer = g_pPlayerManager->GetPlayer(nMaster);
// 		KGC_Player* pTargetPlayer = g_pPlayerManager->GetPlayer(nTarget);
// 		KGC_CONFIRM_BREAK(pMasterPlayer && pTargetPlayer);
// 
// 		if (CanCreateRelation(nMaster, nTarget) && 
// 			SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, ppRelation))
// 		{
// 			bRet = TRUE;
// 		}
// 	} while (0);
// 	return bRet;
}

BOOL KCoupleFactory::CanCreateRelation( INT nMaster, INT nTarget )
{
	BOOL					bRetCode	= FALSE;
	KRole*					pMaster		= NULL;
	KRole*					pTarget		= NULL;
	IKRelation**			ppRelation	= NULL;

	LOG_PROCESS_ERROR(nMaster && nTarget);
	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
	LOG_PROCESS_ERROR(pMaster);
	LOG_PROCESS_ERROR(pTarget);

	bRetCode = KRelationCounter::Instance()->CheckReachMaxCount(emKPLAYERRELATION_TYPE_COUPLE, nMaster);
	PROCESS_ERROR(bRetCode);
	bRetCode = KRelationCounter::Instance()->CheckReachMaxCount(emKPLAYERRELATION_TYPE_COUPLE, nTarget);
	PROCESS_ERROR(bRetCode);

	bRetCode = pMaster->GetSex() == 0;
	PROCESS_ERROR(bRetCode);
	bRetCode = pTarget->GetSex() != 0;
	PROCESS_ERROR(bRetCode);

	bRetCode = (nMaster != nTarget && !FindRelation(nMaster, nTarget, ppRelation));

EXIT0:
	return bRetCode;

// 	BOOL bRet = FALSE;
// 	do 
// 	{
// 		KGC_CONFIRM_BREAK(KRelationCounter::Instance()->CheckReachMaxCount(emKPLAYERRELATION_TYPE_COUPLE, nMaster));
// 		KGC_CONFIRM_BREAK(KRelationCounter::Instance()->CheckReachMaxCount(emKPLAYERRELATION_TYPE_COUPLE, nTarget));
// 		KGC_Player* pMasterPlayer = g_pPlayerManager->GetPlayer(nMaster);
// 		KGC_Player* pTargetPlayer = g_pPlayerManager->GetPlayer(nTarget);
// 		KGC_CONFIRM_BREAK(pMasterPlayer && pTargetPlayer);
// 
// 		KGC_CONFIRM_BREAK(pMasterPlayer->GetSex() == 0);	// IKPlayer::GetSex()返回0表示男性
// 		KGC_CONFIRM_BREAK(pTargetPlayer->GetSex() != 0);
// 
// 		IKRelation** ppRelation = NULL;
// 		if (nMaster != nTarget && !FindRelation(nMaster, nTarget, ppRelation))
// 			bRet	= TRUE;
// 	} while (0);
// 	return bRet;
}

// BOOL KCoupleFactory::SaveAll( INT nPlayerId, IKRelationSaver* pSaver )
// {
// 	KGC_CONFIRM_RET_FALSE(nPlayerId && pSaver);
// 	SUPER_CLASS::SaveAll(nPlayerId, &m_manPlayerIdMasters, &m_manPlayerIdTargets, TRUE, pSaver);
// 	return TRUE;
// }
// -------------------------------------------------------------------------

