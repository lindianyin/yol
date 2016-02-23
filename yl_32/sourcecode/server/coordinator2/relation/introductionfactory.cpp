/* -------------------------------------------------------------------------
//	文件名		：	kintroductionfactory.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	介绍人好友工厂
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "introductionfactory.h"
#include "relationfactorylist.h"


KIntroductionFactory::KIntroductionFactory() : SUPER_CLASS(emKPLAYERRELATION_TYPE_INTRODUCTION, TRUE, FALSE, FALSE)
{
}

KIntroductionFactory::~KIntroductionFactory()
{
}

BOOL KIntroductionFactory::CreateRelation(INT nMaster,
										  INT nTarget,
										  BOOL			bIsMasterActive,
										  IKRelation**	ppRelation)
{
	BOOL					bRetCode		= FALSE;
	KRole*					pMaster			= NULL;
	KRole*					pTarget			= NULL;

	LOG_PROCESS_ERROR(nMaster && nTarget);
	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
	LOG_PROCESS_ERROR(pMaster);
	LOG_PROCESS_ERROR(pTarget);

	if (!SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, ppRelation))
	{
		DeleteRelation(nMaster, nTarget);
		bRetCode = FALSE;
		goto EXIT0;
	}

	MsgToShow(nMaster, nTarget, pMaster, pTarget, emADDINTRODUCTION_SUCCESS);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;

// 	KGC_CONFIRM_RET_FALSE(nMaster && nTarget);
// 	
// 	KGC_Player* pMasterPlayer = g_pPlayerManager->GetPlayer(nMaster);
// 	KGC_Player* pTargetPlayer = g_pPlayerManager->GetPlayer(nTarget);
// 	KGC_CONFIRM_RET_FALSE(pMasterPlayer && pTargetPlayer);
// 	
// 	// 建立介绍人关系
// 	if (!SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, ppRelation))
// 	{
// 		KGC_Error("");
// 		DeleteRelation(nMaster, nTarget);
// 		return FALSE;
// 	}
// 	return MsgToShow(nMaster, nTarget, pMasterPlayer, pTargetPlayer, emADDINTRODUCTION_SUCCESS);
}

BOOL KIntroductionFactory::CanCreateRelation(INT nMaster,
											 INT nTarget)
{
	if (!nMaster) return FALSE;
	if (!nTarget) return FALSE;

	KRole* pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	if (!pMaster) return FALSE;

	KRole* pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
	if (!pTarget) return FALSE;

	if (!CheckCreation(nMaster, nTarget, pMaster, pTarget))
		return FALSE;
	return TRUE;
}

BOOL KIntroductionFactory::CheckCreation(INT			nMaster,
										 INT			nTarget,
										 KRole*	pMaster,
										 KRole*	pTarget)
{
	if (nMaster <= 0) return FALSE;
	if (nMaster <= 0) return FALSE;
	if (!pMaster) return FALSE;
	if (!pTarget) return FALSE;
	
	// 检查级别
	INT nMasterLevel = pMaster->GetLevel();
	INT nTargetLevel = pTarget->GetLevel();
	if (nTargetLevel > emKINTRODUCED_MAXLEVEL)
		return FALSE;
	if (nMasterLevel - nTargetLevel < emKINTRODUCER_DIFLEVEL)
		return FALSE;
	// 被介绍人只能有一个介绍人
	IKEnumRelation* pEnum = NULL;
	if (GetAllRelations(FALSE, nTarget, &pEnum) &&
		pEnum->GetCount() > 0)
	{
		return FALSE;
	}
	return SUPER_CLASS::CanCreateRelation(nMaster, nTarget);
}

BOOL KIntroductionFactory::MsgToShow(INT		 nMaster, 
									 INT		 nTarget, 
									 KRole* pMasterPlayer, 
									 KRole* pTargetPlayer, 
									 BYTE		 btMsgType)
{
	// 系统提示
// 	char szBufToMaster[512];
// 	char szBufToTarget[512];
// 
// 	switch(btMsgType)
// 	{
// 	case emADDINTRODUCTION_SUCCESS:
// 		sprintf(szBufToMaster, KD_STR_INTRODUCED, pTargetPlayer->GetPlayerName());
// 		sprintf(szBufToTarget, KD_STR_INTRODUCER, pMasterPlayer->GetPlayerName());
// 		break;
// 	case emINTRODUCTION_OVERMAX_NUMBER:
// 		sprintf(szBufToMaster, KD_STR_INTRODUCED_MAXNUMBER,	pTargetPlayer->GetPlayerName());
// 		sprintf(szBufToTarget, KD_STR_INTRODUCER_MAXNUMBER);
// 		break;
// 	default:
// 		KGC_CONFIRM_RET_FALSE(FALSE);
// 	}
// 	
// 	INT nMasterSvrID = pMasterPlayer->GetOnlineServer();
// 	if (nMasterSvrID > 0)
// 	{
// 		g_ChatChannelManager.PostSystemMessage(nMaster, KD_STR_SYSTEM, szBufToTarget, nMasterSvrID);
// 	}
// 	INT nTargetSvrID = pTargetPlayer->GetOnlineServer();
// 	if (nTargetSvrID > 0)
// 	{
// 		g_ChatChannelManager.PostSystemMessage(nTarget, KD_STR_SYSTEM, szBufToTarget, nTargetSvrID);
// 	}
	return TRUE;
}
