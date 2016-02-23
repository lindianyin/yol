#include "stdafx.h"
#include "buddyfactory.h"
#include "relationfactorylist.h"

KBuddyFactory::KBuddyFactory() : SUPER_CLASS(emKPLAYERRELATION_TYPE_BUDDY)
{
}

KBuddyFactory::~KBuddyFactory()
{
}

BOOL KBuddyFactory::CreateRelation(INT nMaster,
								   INT nTarget,
								   BOOL			bIsMasterActive,
								   IKRelation**	ppRelation)
{
	BOOL							bRetCode			= TRUE;
	KRole*							pMaster				= NULL;
	KRole*							pTarget				= NULL;

	LOG_PROCESS_ERROR(nMaster && nTarget);
	// 检查是否允许建立指定密友关系
	LOG_PROCESS_ERROR(nMaster > 0 && nTarget > 0);

	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
	LOG_PROCESS_ERROR(pMaster);
	LOG_PROCESS_ERROR(pTarget);

	bRetCode = CheckCreation(nMaster, nTarget, pMaster, pTarget);
	PROCESS_ERROR(bRetCode);

	// 建立指定关系
	bRetCode = SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, ppRelation);
	LOG_PROCESS_ERROR(bRetCode);

	// 通知关系建立
// 	CHAR szBuf[256];
// 	INT nMasterSvrId	= pMasterPlayer->GetOnlineServer();
// 	if (nMasterSvrId > 0)
// 	{
// 		sprintf(szBuf, KD_STR_BUDDY, pTargetPlayer->GetPlayerName());
// 
// 		g_ChatChannelManager.PostSystemMessage(nMaster, KD_STR_SYSTEM, szBuf, nMasterSvrId);
// 	}
// 	INT nTargetSvrId	= pTargetPlayer->GetOnlineServer();
// 	if (nTargetSvrId > 0)
// 	{
// 		sprintf(szBuf, KD_STR_BUDDY, pMasterPlayer->GetPlayerName());
// 		g_ChatChannelManager.PostSystemMessage(nTarget, KD_STR_SYSTEM, szBuf, nTargetSvrId);
// 	}
// 	return TRUE;

	bRetCode = TRUE;
EXIT0:
	return bRetCode;

// 	KGC_CONFIRM_RET_FALSE(nMaster && nTarget);
// 	// 检查是否允许建立指定密友关系
// 	KGC_CONFIRM_RET_FALSE(nMaster > 0 && nTarget > 0);
// 	
// 	KGC_Player* pMasterPlayer = g_pPlayerManager->GetPlayer(nMaster);
// 	KGC_Player* pTargetPlayer = g_pPlayerManager->GetPlayer(nTarget);
// 	KGC_CONFIRM_RET_FALSE(pMasterPlayer && pTargetPlayer);
// 	
// 	if(!CheckCreation(nMaster, nTarget, pMasterPlayer, pTargetPlayer))
// 	{
// 		KGC_Error("");
// 		return FALSE;
// 	}
// 	// 建立指定关系
// 	if (!SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, ppRelation))
// 	{
// 		KGC_Error("");
// 		return FALSE;
// 	}
// 	CHAR szBuf[256];
// 	INT nMasterSvrId	= pMasterPlayer->GetOnlineServer();
// 	if (nMasterSvrId > 0)
// 	{
// 		sprintf(szBuf, KD_STR_BUDDY, pTargetPlayer->GetPlayerName());
// 
// 		g_ChatChannelManager.PostSystemMessage(nMaster, KD_STR_SYSTEM, szBuf, nMasterSvrId);
// 	}
// 	INT nTargetSvrId	= pTargetPlayer->GetOnlineServer();
// 	if (nTargetSvrId > 0)
// 	{
// 		sprintf(szBuf, KD_STR_BUDDY, pMasterPlayer->GetPlayerName());
// 		g_ChatChannelManager.PostSystemMessage(nTarget, KD_STR_SYSTEM, szBuf, nTargetSvrId);
// 	}
// 	return TRUE;
}

BOOL KBuddyFactory::CanCreateRelation(INT nMaster,
									  INT nTarget)
{
	BOOL			bRetCode		= FALSE;
	KRole*			pMaster			= NULL;
	KRole*			pTarget			= NULL;

	LOG_PROCESS_ERROR(nMaster > 0 && nTarget > 0);
	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
	LOG_PROCESS_ERROR(pMaster);
	LOG_PROCESS_ERROR(pTarget);

	bRetCode = CheckCreation(nMaster, nTarget, pMaster, pTarget);

EXIT0:
	return bRetCode;
}

BOOL KBuddyFactory::DeleteRelation(INT nPlayerId1, INT nPlayerId2)
{
	if(!SUPER_CLASS::DeleteRelation(nPlayerId1, nPlayerId2))
	{
		ASSERT(0);
		return FALSE;
	}
	return TRUE;
}

BOOL KBuddyFactory::CheckCreation(INT			nMaster,
								  INT			nTarget,
								  KRole*		pMaster,
								  KRole*		pTarget)
{
	BOOL					bRetCode				= FALSE;
	IKRelationFactory*		pFriendFactory			= NULL;
	IKRelation*				pFriend					= NULL;
	IKBidFriendRelation*	pFriendRelation			= NULL;
	IKRelationFactory*		pTrainingFactory		= NULL;
	IKRelationFactory*		pTrainedFactory			= NULL;
	IKRelationFactory*		pIntroductionFactory	= NULL;

	LOG_PROCESS_ERROR(pMaster && pTarget);

	// 先关闭密友条件
	bRetCode = SUPER_CLASS::CanCreateRelation(nMaster, nTarget);
	goto EXIT0;

	// 检查级别
	if (pMaster->GetLevel() < emKBUDDY_MINLEVEL ||
		pTarget->GetLevel() < emKBUDDY_MINLEVEL)
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	// 检查密友数目
	if (GetBuddyCount(nMaster) >= emKBUDDY_MAXCOUNT ||
		GetBuddyCount(nTarget) >= emKBUDDY_MAXCOUNT)
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	// 必须是普通好友且亲密度达到一定等级
	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFriendFactory);
	LOG_PROCESS_ERROR(bRetCode);
	if (!pFriendFactory->FindRelation(nMaster, nTarget, &pFriend))
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFriendFactory) ||
// 		!pFriendFactory->FindRelation(nMaster, nTarget, &pFriend) ||
// 		!pFriend->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (void**)&pFriendRelation) ||
// 		pFriendRelation->GetFavor() < emKBUDDY_MINFAVOR)
// 	{
// 		KGC_Error("");
// 		return FALSE;
// 	}

	// 已经是师徒不能成为指定密友
	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINING, &pTrainingFactory);
	LOG_PROCESS_ERROR(bRetCode);

	if (pTrainingFactory->FindRelation(nMaster, nTarget) ||
		pTrainingFactory->FindRelation(nTarget, nMaster))
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pTrainedFactory);
	LOG_PROCESS_ERROR(bRetCode);

	if (pTrainedFactory->FindRelation(nMaster, nTarget) ||
		pTrainedFactory->FindRelation(nTarget, nMaster))
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	// 已经是介绍人不能成为指定密友
	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_INTRODUCTION, &pIntroductionFactory);
	LOG_PROCESS_ERROR(bRetCode);

	if (pIntroductionFactory->FindRelation(nMaster, nTarget) ||
		pIntroductionFactory->FindRelation(nTarget, nMaster))
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	bRetCode = SUPER_CLASS::CanCreateRelation(nMaster, nTarget);
EXIT0:
	return bRetCode;

// 	KGC_CONFIRM_RET_FALSE(pMaster && pTarget);
// 	// 检查级别
// 	if (pMaster->GetLevel() < emKBUDDY_MINLEVEL ||
// 		pTarget->GetLevel() < emKBUDDY_MINLEVEL)
// 	{
// 		return FALSE;
// 	}
// 	// 检查密友数目
// 	if (GetBuddyCount(nMaster) >= emKBUDDY_MAXCOUNT ||
// 		GetBuddyCount(nTarget) >= emKBUDDY_MAXCOUNT)
// 	{
// 		return FALSE;
// 	}
// 	// 必须是普通好友且亲密度达到一定等级
// 	IKRelationFactory* pFriendFactory = NULL;
// 	IKRelation* pFriend = NULL;
// 	IKBidFriendRelation* pFriendRelation = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFriendFactory) ||
// 		!pFriendFactory->FindRelation(nMaster, nTarget, &pFriend) ||
// 		!pFriend->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (void**)&pFriendRelation) ||
// 		pFriendRelation->GetFavor() < emKBUDDY_MINFAVOR)
// 	{
// 		KGC_Error("");
// 		return FALSE;
// 	}
// 	// 已经是师徒不能成为指定密友
// 	IKRelationFactory* pTrainingFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINING, &pTrainingFactory))
// 	{
// 		KGC_Error("");
// 		return FALSE;
// 	}
// 	if (pTrainingFactory->FindRelation(nMaster, nTarget) ||
// 		pTrainingFactory->FindRelation(nTarget, nMaster))
// 	{
// 		return FALSE;
// 	}
// 	IKRelationFactory* pTrainedFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pTrainedFactory))
// 	{
// 		KGC_Error("");
// 		return FALSE;
// 	}
// 	if (pTrainedFactory->FindRelation(nMaster, nTarget) ||
// 		pTrainedFactory->FindRelation(nTarget, nMaster))
// 	{
// 		return FALSE;
// 	}
// 	// 已经是介绍人不能成为指定密友
// 	IKRelationFactory* pIntroductionFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_INTRODUCTION, &pIntroductionFactory))
// 	{
// 		KGC_Error("");
// 		return FALSE;
// 	}
// 	if (pIntroductionFactory->FindRelation(nMaster, nTarget) ||
// 		pIntroductionFactory->FindRelation(nTarget, nMaster))
// 	{
// 		return FALSE;
// 	}
// 	if (SUPER_CLASS::CanCreateRelation(nMaster, nTarget))
// 		return TRUE;
// 	return FALSE;
}

INT KBuddyFactory::GetBuddyCount(INT nPlayerId)
{
	ASSERT(nPlayerId);
	IKEnumRelation* pEnum = NULL;
	if (!GetAllRelations(TRUE, nPlayerId, &pEnum))
		return 0;
	return pEnum->GetCount();
}
