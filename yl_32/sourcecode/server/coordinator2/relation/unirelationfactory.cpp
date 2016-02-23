/* -------------------------------------------------------------------------
//	文件名		：	unirelationfactory.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	单向关系工厂（黑名单等）
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "unirelationfactory.h"
#include "relationcounter.h"
#include "relation.h"
#include "relationfactorylist.h"
#include "KRole.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "GameCenter.h"
#include "onlinegamebase/kscriptmanager.h"

KUniRelationFactory::KUniRelationFactory(KEPLAYERRELATION_TYPE nType) :
	m_nType(nType),
	m_pRelation(NULL),
	m_pEnum(NULL)
{
	ASSERT(nType >= 0 && nType < emKPLAYERRELATION_TYPE_COUNT);
	m_pRelation = new KRelation(nType, this);
	m_pEnum = new KEnumUniRelation(m_pRelation);
}

KUniRelationFactory::~KUniRelationFactory()
{
	ASSERT(m_pRelation && m_pEnum);
	delete m_pRelation;
	m_pRelation = NULL;
	delete m_pEnum;
	m_pEnum = NULL;
}

BOOL KUniRelationFactory::Load(INT nMaster, INT nTarget, LPVOID pData, INT nLen)
{
	// KGC_CONFIRM_RET_FALSE(nLen == 0);
	
	BOOL bRetCode = FALSE;
	RELATION_SET *pSet = &m_mapRelationList[nMaster];
	RELATION_SET::const_iterator it = pSet->find(nTarget);
	if (it == pSet->end())
	{
		pSet->insert(nTarget);
		AddReverseRelation(nTarget, nMaster);
	}

	LOG_PROCESS_ERROR(CountRelation(&m_mapRelationList) == CountRelation(&m_mapReverseRelationList));

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL KUniRelationFactory::CreateRelation(INT	nMaster,
										 INT	nTarget,
										 BOOL			bIsMasterActive,
										 IKRelation**	ppRelation)
{
	BOOL									bRetCode	= FALSE;
	RELATION_SET*							pSet		= NULL;
	RELATION_SET::const_iterator			it;
	CONST KRelationRuleManager::KPOLICY*	pPolicy		= NULL;
	KRole*									pMaster		= NULL;
	KRole*									pTarget		= NULL;
	IMemBlock*								pMemBlock	= NULL;
	R2S_RELATION_SYNC_LIST_RESPOND*			pSync		= NULL;
	KRELATION_INFO*							pInfo		= NULL;
	R2S_RELATION_NOTIFY_RESPOND*			pRespond	= NULL;

	LOG_PROCESS_ERROR(nMaster && nTarget);

	pSet = &m_mapRelationList[nMaster];
	it = pSet->find(nTarget);
	LOG_PROCESS_ERROR(it == pSet->end());

	pSet->insert(nTarget);
	AddReverseRelation(nTarget, nMaster);
	LOG_PROCESS_ERROR(CountRelation(&m_mapRelationList) == CountRelation(&m_mapReverseRelationList));

	m_pRelation->Init(nMaster, nTarget);
	if (ppRelation)
	{
		*ppRelation = m_pRelation;
	}
	
	// 同步新建关系
	LOG_PROCESS_ERROR(nMaster > 0 && nTarget > 0);
	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
	LOG_PROCESS_ERROR(pMaster);
	LOG_PROCESS_ERROR(pTarget);
	g_pGameCenter->m_RelationManager.AddDirty(nMaster);
	// m_pHelper->Dirty(nMaster, 0);

	pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
	LOG_PROCESS_ERROR(pPolicy);
	if (pPolicy->bSyncToServerAsMaster && pMaster->m_nConnIndex)
	{
		pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + sizeof(KRELATION_INFO));
		LOG_PROCESS_ERROR(pMemBlock);

		pSync = (R2S_RELATION_SYNC_LIST_RESPOND*)pMemBlock->GetData();
		pSync->wProtocolID		= r2s_relation_sync_list_respond;
		pSync->nCount			= 1;
		pSync->nType			= m_nType;
		pSync->nRole			= 0;
		pSync->nPlayerID		= nMaster;
		pSync->byNotifyClient	= pPolicy->bSyncToClientAsMaster;
		pSync->byCreate			= TRUE;

		pInfo					= pSync->aryInfo;
		memcpy(pInfo->szRelation, pTarget->m_szName, sizeof(pTarget->m_szName));
		pInfo->nOnline			= pTarget->IsOnline();
		pInfo->nSex				= pTarget->GetSex();
		pInfo->nLevel			= pTarget->GetLevel();
		pInfo->nPlayerId		= pTarget->GetId();
		bRetCode = g_pGameCenter->m_GameServer.Send(pMaster->m_nConnIndex, pMemBlock);
		LOG_PROCESS_ERROR(bRetCode);
		SAFE_RELEASE(pMemBlock);

// 		INT nMasterSvrID = pGCPlayerMaster->GetOnlineServer();
// 		if (nMasterSvrID > 0)
// 		{
// 			CHAR szBuf[sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + sizeof(KRELATION_INFO)];
// 			R2S_RELATION_SYNC_LIST_RESPOND *pSync = (R2S_RELATION_SYNC_LIST_RESPOND*)szBuf;
// 			pSync->nCount			= 1;
// 			pSync->nType			= m_nType;
// 			pSync->nRole			= 0;
// 			pSync->nPlayerID		= nMaster;
// 			pSync->byNotifyClient	= pPolicy->bSyncToClientAsMaster;
// 			pSync->ProtocolType		= r2s_relation_sync_list_respond;
// 			KRELATION_INFO *pInfo = pSync->aryInfo;
// 			strncpy(pInfo->szRelation, pGCPlayerTarget->GetPlayerName(), sizeof(pInfo->szRelation) - 1);
// 			pInfo->szRelation[sizeof(pInfo->szRelation) - 1] = 0;
// 			pInfo->nOnline = pGCPlayerTarget->GetOnlineServer() > 0 ? 1 : 0;
// 			g_pGameCenter->m_GameServer.Send(pRoleMaster->m_nConnIndex, )
// 			m_pSender->SendData(szBuf, sizeof(szBuf), nMasterSvrID);
// 		}
	}
	if (pPolicy->bNotifyTargetCreation && pTarget->m_nConnIndex)
	{
		pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_NOTIFY_RESPOND));
		LOG_PROCESS_ERROR(pMemBlock);

		pRespond = (R2S_RELATION_NOTIFY_RESPOND*)pMemBlock->GetData();
		pRespond->wProtocolID	= r2s_relation_notify_respond;
		pRespond->nType			= m_nType;
		pRespond->nRole			= 0;
		pRespond->nPlayerID		= nTarget;
		memcpy(pRespond->szRelation, pMaster->m_szName, sizeof(pMaster->m_szName));
		bRetCode = g_pGameCenter->m_GameServer.Send(pTarget->m_nConnIndex, pMemBlock);
		LOG_PROCESS_ERROR(bRetCode);
		SAFE_RELEASE(pMemBlock);

		// 通知被加关系的玩家
// 		INT nTargetSvrID = pGCPlayerTarget->GetOnlineServer();
// 		if (nTargetSvrID > 0)
// 		{
// 			R2S_RELATION_NOTIFY_RESPOND sNotify;
// 			sNotify.ProtocolType = r2s_relation_notify_respond;
// 			sNotify.nType = m_nType;
// 			sNotify.nRole = 0;
// 			sNotify.nPlayerID = nTarget;
// 			strncpy(sNotify.szRelation, pGCPlayerMaster->GetPlayerName(), sizeof(sNotify.szRelation) - 1);
// 			sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 			m_pSender->SendData(&sNotify, sizeof(sNotify), nTargetSvrID);
// 		}
	}

	AfterCreateRelation(nMaster, nTarget, m_nType);
	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(pMemBlock);
	return bRetCode;
}

BOOL KUniRelationFactory::CanCreateRelation(INT	nMaster,
											INT	nTarget)
{
	BOOL						bRetCode = FALSE;
	RELATION_MAP::iterator		it1;
	RELATION_SET*				pSet = NULL;
	RELATION_SET::iterator		it2;

	LOG_PROCESS_ERROR(nMaster && nTarget);
	if(!KRelationCounter::Instance()->CheckReachMaxCount(m_nType, nMaster))
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	it1 = m_mapRelationList.find(nMaster);
	if (it1 == m_mapRelationList.end())
	{
		bRetCode = TRUE;
		goto EXIT0;
	}

	pSet = &it1->second;
	it2 = pSet->find(nTarget);
	bRetCode = (it2 == pSet->end());
EXIT0:
	return bRetCode;
}

BOOL KUniRelationFactory::GetAllRelations(BOOL		bAsMaster,
										  INT		nPlayerId,
										  IKEnumRelation**	ppEnum)
{
	BOOL					bRetCode = FALSE;
	RELATION_MAP*			pMap = NULL;
	RELATION_MAP::iterator	it;

	LOG_PROCESS_ERROR(nPlayerId && ppEnum);
	pMap = bAsMaster ? &m_mapRelationList : &m_mapReverseRelationList;
	it = pMap->find(nPlayerId);
	if (it == pMap->end())
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	m_pEnum->Init(nPlayerId, bAsMaster, &it->second);
	m_pEnum->Reset();
	*ppEnum = m_pEnum;

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL KUniRelationFactory::FindRelation(INT	nMaster,
									   INT	nTarget,
									   IKRelation**	ppRelation)
{
	BOOL						bRetCode = FALSE;
	RELATION_MAP::iterator		it1;
	RELATION_SET*				pSet = NULL;
	RELATION_SET::iterator		it2;

	LOG_PROCESS_ERROR(nMaster && nTarget);
	it1 = m_mapRelationList.find(nMaster);
	if (it1 == m_mapRelationList.end())
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	pSet = &it1->second;
	it2 = pSet->find(nTarget);
	if (it2 == pSet->end())
	{
		bRetCode = FALSE;
		goto EXIT0;
	}
	if (ppRelation)
	{
		m_pRelation->Init(it1->first, *it2);
		*ppRelation = m_pRelation;
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL KUniRelationFactory::DeleteRelation(INT nMaster, INT nTarget)
{
	BOOL									bRetCode		= FALSE;
	RELATION_SET*							pSet			= NULL;
	RELATION_SET::iterator					it2;
	CONST KRelationRuleManager::KPOLICY*	pPolicy			= NULL;
	IMemBlock*								pMemBlock		= NULL;
	R2S_RELATION_DEL_RELATION_RESPOND*		pRespond		= NULL;
	RELATION_MAP::iterator					it1;
	KRole*									pMaster			= NULL;
	KRole*									pTarget			= NULL;

	LOG_PROCESS_ERROR(nMaster && nTarget);
	it1 = m_mapRelationList.find(nMaster);
	LOG_PROCESS_ERROR(it1 != m_mapRelationList.end());
	
	pSet = &it1->second;
	it2 = pSet->find(nTarget);
	LOG_PROCESS_ERROR(it2 != pSet->end());
	
	if (pSet->size() == 1)
		m_mapRelationList.erase(it1);
	else
		pSet->erase(it2);
	DelReverseRelation(nTarget, nMaster);
	LOG_PROCESS_ERROR(CountRelation(&m_mapRelationList) == CountRelation(&m_mapReverseRelationList));

	// 减少计数
//	KRelationCounter::Instance()->DelRelationCount(nMaster);
	// 删除数据
	//if (m_pDatabase)
	//	m_pDatabase->DelFromDatabase(m_nType, nMaster, nTarget);

	pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
	LOG_PROCESS_ERROR(pPolicy->bNotifyDeleteToMaster);
	
	// 通知关系删除
	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
	LOG_PROCESS_ERROR(pMaster && pTarget);
	g_pGameCenter->m_RelationManager.AddDirty(nMaster);
	if (pMaster->m_nConnIndex)
	{
		pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_DEL_RELATION_RESPOND));
		LOG_PROCESS_ERROR(pMemBlock);

		pRespond = (R2S_RELATION_DEL_RELATION_RESPOND*)pMemBlock->GetData();
		pRespond->wProtocolID	= r2s_relation_del_relation_respond;
		pRespond->nType			= m_nType;
		pRespond->nPlayerID		= nMaster;
		pRespond->nRelationRole = 0;
		memcpy(pRespond->szRelation, pTarget->m_szName, sizeof(pTarget->m_szName));
		bRetCode = g_pGameCenter->m_GameServer.Send(pMaster->m_nConnIndex, pMemBlock);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(pMemBlock);
	return bRetCode;

// 	KGC_Player* pGCPlayerMaster = g_pPlayerManager->GetPlayer(nMaster);
// 	KGC_Player* pGCPlayerTarget = g_pPlayerManager->GetPlayer(nTarget);
// 	KGC_CONFIRM_RET_FALSE(pGCPlayerMaster && pGCPlayerTarget);
// 	m_pHelper->Dirty(nMaster, 0);
// 
// 	INT nMasterSvrID = pGCPlayerMaster->GetOnlineServer();
// 	KGC_CONFIRM_RET_FALSE(nMasterSvrID > 0);
// 	R2S_RELATION_DEL_RELATION_RESPOND sNotify;
// 	sNotify.ProtocolType = r2s_relation_del_relation_respond;
// 	sNotify.nType = m_nType;
// 	sNotify.nPlayerID = nMaster;
// 	sNotify.nRelationRole = 0;
// 	strncpy(sNotify.szRelation, pGCPlayerTarget->GetPlayerName(), sizeof(sNotify.szRelation) - 1);
// 	sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 	m_pSender->SendData(&sNotify, sizeof(sNotify), nMasterSvrID);
// 	return TRUE;
}

BOOL KUniRelationFactory::Remove(INT nPlayerId)
{
	BOOL						bRetCode = FALSE;
	RELATION_MAP::iterator		it;
	RELATION_SET*				pSet = NULL;
	RELATION_SET::iterator		iter;

	LOG_PROCESS_ERROR(nPlayerId);
	it = m_mapRelationList.find(nPlayerId);
	if (it == m_mapRelationList.end())
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	pSet = &it->second;
	iter = pSet->begin();
	for (; iter != pSet->end(); iter++)
	{
		DelReverseRelation(*iter, nPlayerId);
	}
	m_mapRelationList.erase(it);
	//pDatabase->Remove(nPlayerId);

	bRetCode = CountRelation(&m_mapRelationList) == CountRelation(&m_mapReverseRelationList);
EXIT0:
	return bRetCode;
}

BOOL KUniRelationFactory::Unload(INT nPlayerId)
{
	BOOL					bRetCode = FALSE;
	RELATION_MAP::iterator	it1;
	RELATION_SET*			pSet = NULL;
	RELATION_SET::iterator	it3;

	LOG_PROCESS_ERROR(nPlayerId);
	it1 = m_mapRelationList.find(nPlayerId);
	if (it1 == m_mapRelationList.end())
	{
		bRetCode = FALSE;
		goto EXIT0;
	}
	//if (m_pDatabase)
	//{
	//	RELATION_SET *pSet = &it1->second;
	//	RELATION_SET::iterator it2 = pSet->begin();
	//	for (; it2 != pSet->end(); it2++)
	//	{
	//		m_pDatabase->Update(m_nType, nPlayerId, &(*it2));
	//	}
	//}
	pSet = &it1->second;
	it3 = pSet->begin();
	for (; it3 != pSet->end(); it3++)
	{
		DelReverseRelation(*it3, nPlayerId);
	}
	m_mapRelationList.erase(it1);

	bRetCode = CountRelation(&m_mapRelationList) == CountRelation(&m_mapReverseRelationList);
EXIT0:
	return bRetCode;
}

BOOL KUniRelationFactory::SaveAll(INT nPlayerId, IKRelationSaver* pSaver)
{
	BOOL						bRetCode = FALSE;
	RELATION_MAP::iterator		it1;
	RELATION_SET*				pSet = NULL;
	RELATION_SET::iterator		it2;

	LOG_PROCESS_ERROR(nPlayerId);
	it1 = m_mapRelationList.find(nPlayerId);
	if (it1 == m_mapRelationList.end())
	{
		bRetCode = TRUE;
		goto EXIT0;
	}

	pSet = &it1->second;
	LOG_PROCESS_ERROR(!pSet->empty());

	pSaver->SaveBegin(m_nType, 0, TRUE);
	it2 = pSet->begin();
	for (; it2 != pSet->end(); it2++)
	{
		pSaver->AddRecord(*it2);
	}
	pSaver->SaveEnd(m_nType);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL KUniRelationFactory::AddReverseRelation(INT nPlayerId1, INT nPlayerId2)
{
	BOOL bRetCode = FALSE;
	LOG_PROCESS_ERROR(nPlayerId1 && nPlayerId2);
	m_mapReverseRelationList[nPlayerId1].insert(nPlayerId2);
	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL KUniRelationFactory::DelReverseRelation(INT nPlayerId1, INT nPlayerId2)
{
	BOOL					bRetCode = FALSE;
	RELATION_MAP::iterator	it;
	RELATION_SET*			pSet = NULL;

	LOG_PROCESS_ERROR(nPlayerId1 && nPlayerId2);
	it = m_mapReverseRelationList.find(nPlayerId1);
	LOG_PROCESS_ERROR(it != m_mapReverseRelationList.end());
	pSet = &it->second;
	pSet->erase(nPlayerId2);
	if (pSet->empty())
	{
		m_mapReverseRelationList.erase(it);
	}
	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

INT KUniRelationFactory::CountRelation(CONST RELATION_MAP *pMap)
{
	if (!pMap)
	{
		ASSERT(0);
		return 0;
	}

	INT nCount = 0;
	RELATION_MAP::const_iterator it = pMap->begin();
	for (; it != pMap->end(); it++)
	{
		nCount += (INT)it->second.size();
	}
	return nCount;
}

VOID KUniRelationFactory::AfterCreateRelation( INT nMasterId, INT nTargetId, INT nType )
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction("Relation", "AfterAddRelation_GC", 0, 
		"ddd", nMasterId, nTargetId, nType);
}

KEnumUniRelation::KEnumUniRelation(KRelation *pRelation) :
	m_pRelationSet(NULL),
	m_pRelation(pRelation),
	m_bAsMaster(FALSE)
{
	ASSERT(pRelation);
}

BOOL KEnumUniRelation::Init(INT nPlayerId, BOOL bAsMaster, RELATION_SET *pSet)
{
	if (!(nPlayerId && pSet))
	{
		ASSERT(0);
		return FALSE;
	}
	m_pRelationSet = pSet;
	m_itSet = pSet->begin();
	m_bAsMaster = bAsMaster;
	m_nPlayerId = nPlayerId;
	return TRUE;
}

VOID KEnumUniRelation::Reset()
{
	if (!m_pRelationSet)
	{
		ASSERT(0);
		return;
	}
	m_itSet = m_pRelationSet->begin();
}

BOOL KEnumUniRelation::Next(IKRelation **ppRelation)
{
	if (!(ppRelation && m_pRelationSet))
	{
		ASSERT(0);
		return FALSE;
	}

	if (m_itSet == m_pRelationSet->end())
		return FALSE;
	INT nPlayerId = *m_itSet;
	m_itSet++;
	if (m_bAsMaster)
		m_pRelation->Init(m_nPlayerId, nPlayerId);
	else
		m_pRelation->Init(nPlayerId, m_nPlayerId);
	*ppRelation = m_pRelation;
	return TRUE;
}

INT KEnumUniRelation::GetCount()
{
	return static_cast<INT>(m_pRelationSet->size());
}
