/* -------------------------------------------------------------------------
//	文件名		：	kfriendfactory.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	普通好友工厂
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "friendfactory.h"
#include "relationfactorylist.h"
#include "relationmanager.h"

KFriendFactory::KFriendFactory() : SUPER_CLASS(emKPLAYERRELATION_TYPE_BINDFRIEND)
{
}

KFriendFactory::~KFriendFactory()
{
}

BOOL KFriendFactory::DeleteRelation(INT nMaster, INT nTarget)
{
	BOOL							bRetCode			= FALSE;
	IKRelationFactory*				pThisFactory		= NULL;
	IKRelationFactory*				pFactory			= NULL;
	KEPLAYERRELATION_TYPE			emType				= (KEPLAYERRELATION_TYPE)0;
	KRelationRuleManager*			pRuleManager		= NULL;
	KRelationRuleManager::KPOLICY	sOldPolicy			= {0};
	KRelationRuleManager::KPOLICY	sNewPolicy			= {0};

	LOG_PROCESS_ERROR(nMaster && nTarget);

	pThisFactory = this;
	if (pThisFactory->FindRelation(nMaster, nTarget))
	{
		// 删除普通好友关系
		bRetCode = SUPER_CLASS::DeleteRelation(nMaster, nTarget);
		LOG_PROCESS_ERROR(bRetCode);

		// 加为临时好友
		bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TMPFRIEND, &pFactory);
		LOG_PROCESS_ERROR(bRetCode);
		LOG_PROCESS_ERROR(pFactory);
		
		// 不通知被加为临时好友的玩家
		emType = emKPLAYERRELATION_TYPE_TMPFRIEND;
		pRuleManager = KRelationRuleManager::Instance();
		sOldPolicy = *pRuleManager->GetPolicy(emType);
		sNewPolicy = sOldPolicy;
		sNewPolicy.bNotifyTargetCreation = FALSE;
		pRuleManager->SetPolicy(emType, &sNewPolicy);
		bRetCode = pFactory->CreateRelation(nTarget, nMaster);
		pRuleManager->SetPolicy(emType, &sOldPolicy);
		LOG_PROCESS_ERROR(bRetCode);
	}
	else
	{
		// 如果普通好友关系不存在，则试着删除临时好友
		bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TMPFRIEND, &pFactory);
		LOG_PROCESS_ERROR(bRetCode);

		bRetCode = pFactory->DeleteRelation(nMaster, nTarget);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;

// 	assert(nMaster && nTarget);
// 	IKRelationFactory *pThisFactory = this;
// 	if (pThisFactory->FindRelation(nMaster, nTarget))
// 	{
// 		// 删除普通好友关系
// 		if (!SUPER_CLASS::DeleteRelation(nMaster, nTarget))
// 		{
// 			assert(0);
// 			return FALSE;
// 		}
// 		// 加为临时好友
// 		IKRelationFactory *pFactory = NULL;
// 		if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TMPFRIEND, &pFactory))
// 		{
// 			assert(0);
// 			return FALSE;
// 		}
// 
// 		// 不通知被加为临时好友的玩家
// 		KEPLAYERRELATION_TYPE emType = emKPLAYERRELATION_TYPE_TMPFRIEND;
// 		KRelationRuleManager* pRuleManager = KRelationRuleManager::Instance();
// 		KRelationRuleManager::KPOLICY sOldPolicy = *pRuleManager->GetPolicy(emType);
// 		KRelationRuleManager::KPOLICY sNewPolicy = sOldPolicy;
// 		sNewPolicy.bNotifyTargetCreation = FALSE;
// 		pRuleManager->SetPolicy(emType, &sNewPolicy);
// 		BOOL bRet = pFactory->CreateRelation(nTarget, nMaster);
// 		pRuleManager->SetPolicy(emType, &sOldPolicy);
// 		assert(bRet);
// 	}
// 	else
// 	{
// 		// 如果普通好友关系不存在，则试着删除临时好友
// 		IKRelationFactory *pFactory = NULL;
// 		if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TMPFRIEND, &pFactory) ||
// 			!pFactory->DeleteRelation(nMaster, nTarget))
// 		{
// 			assert(0);
// 			return FALSE;
// 		}
// 	}
// 	return TRUE;
}

BOOL KFriendFactory::CreateRelation(INT		nMaster,
									INT		nTarget,
									BOOL			bIsMasterActive,
									IKRelation**	ppRelation)
{
	BOOL							bRetCode = FALSE;
	IKRelation*						pRelation = NULL;
	IKBidFriendRelation*			pFriend = NULL;
	R2S_RELATION_FRIEND_INFO_RESPOND* pRespond = NULL;
	IMemBlock*						pMemBlock = NULL;
	KRole*							pMaster = NULL;
	KRole*							pTarget = NULL;
	INT								nFavor = 0;
	DWORD							dwLastTalkTime = 0;

	LOG_PROCESS_ERROR(nMaster && nTarget);

	bRetCode = SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, &pRelation);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pFriend);

	pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO));
	LOG_PROCESS_ERROR(pMemBlock);
	pRespond = (R2S_RELATION_FRIEND_INFO_RESPOND*)pMemBlock->GetData();
	pRespond->wProtocolID = r2s_relation_friend_info_respond;
	// pRespond->nPlayerID = nMaster;
	pRespond->nCount = 1;

	nFavor = pFriend->GetFavor();
	dwLastTalkTime = pFriend->GetLastTalkTime();
	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
	LOG_PROCESS_ERROR(pMaster);
	LOG_PROCESS_ERROR(pTarget);

	if (pMaster->m_nConnIndex)
	{
		pRespond->nPlayerID = pMaster->m_dwPlayerID;
		KRelationManager_GC::InitRelationInfo(pMaster, pTarget, nFavor, dwLastTalkTime, pRespond->aryInfo);
		bRetCode = g_pGameCenter->m_GameServer.Send(pMaster->m_nConnIndex, pMemBlock);
		LOG_PROCESS_ERROR(bRetCode);
	}
	if (pTarget->m_nConnIndex)
	{
		pRespond->nPlayerID = pTarget->m_dwPlayerID;
		KRelationManager_GC::InitRelationInfo(pTarget, pMaster, nFavor, dwLastTalkTime, pRespond->aryInfo);
		bRetCode = g_pGameCenter->m_GameServer.Send(pTarget->m_nConnIndex, pMemBlock);
		LOG_PROCESS_ERROR(bRetCode);
	}

	if (ppRelation)
	{
		*ppRelation = pRelation;
	}

	g_pGameCenter->m_RelationManager.DoFrindFavorAward(pMaster, pTarget, nMaster, nTarget, 1);
	SUPER_CLASS::AfterCreateRelation(nMaster, nTarget, emKPLAYERRELATION_TYPE_BINDFRIEND);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(pMemBlock);
	return bRetCode;

// 	assert(nMaster && nTarget);
// 	KGC_Player* pMaster = g_pPlayerManager->GetPlayer(nMaster);
// 	KGC_Player* pTarget = g_pPlayerManager->GetPlayer(nTarget);
// 	KGC_CONFIRM_RET_FALSE(nMaster && nTarget);
// 	
// 	IKRelation* pRelation = NULL;
// 	if (!SUPER_CLASS::CreateRelation(nMaster, nTarget, bIsMasterActive, &pRelation))
// 		return FALSE;
// 	IKBidFriendRelation* pFriend = NULL;
// 	if (!pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend))
// 	{
// 		KGC_Error("");
// 		return FALSE;
// 	}
// 	KGC_CONFIRM_RET_FALSE(pFriend);
// 	// 同步好友信息
// 	CONST INT nBufLen = sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO);
// 	CHAR szBuf[nBufLen];
// 	R2S_RELATION_FRIEND_INFO_RESPOND* pCmd = (R2S_RELATION_FRIEND_INFO_RESPOND*)szBuf;
// 	pCmd->ProtocolType = r2s_relation_friend_info_respond;
// 	pCmd->nCount = 1;
// 	INT nFavor = pFriend->GetFavor();
// 	INT nMasterSvrID = pMaster->GetOnlineServer();
// 	if (nMasterSvrID > 0)
// 	{
// 		pCmd->nPlayerID = nMaster;
// 		KRelationManager_GC::InitRelationInfo(pTarget, nFavor, pCmd->aryInfo);
// 		g_pGameCenter->m_RelationManager.SendData(pCmd, nBufLen, nMasterSvrID);
// 
// 	}
// 	INT nTargetSvrID = pTarget->GetOnlineServer();
// 	if (nTargetSvrID > 0)
// 	{
// 		pCmd->nPlayerID = nTarget;
// 		KRelationManager_GC::InitRelationInfo(pMaster, nFavor, pCmd->aryInfo);
// 		g_pGameCenter->m_RelationManager.SendData(pCmd, nBufLen, nTargetSvrID);
// 	}
// 	if (ppRelation)
// 		*ppRelation = pRelation;
// 	// 对第一次建立双向好友的玩家，添加亲密度
// 	g_pGameCenter->m_RelationManager.DoFrindFavorAward(pMaster, pTarget, nMaster, nTarget, 1);
// 	SUPER_CLASS::AfterCreateRelation(nMaster, nTarget, emKPLAYERRELATION_TYPE_BINDFRIEND);
// 
// 	return TRUE;
}

