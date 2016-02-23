#ifndef __KBIDUNEQUALRELATIONFACTORY_T_H__
#define __KBIDUNEQUALRELATIONFACTORY_T_H__

#include "playerrelation_i.h"
#include "relationcounter.h"
#include "datarelation_t.h"
#include "relationrulemanager.h"
#include <list>
#include "onlinegamebase/kscriptmanager.h"
#include "KRole.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "serverbase/RelationDef.h"
#include "GameCenter.h"

template <class KData_T, class KRelation_T, CONST INT nInfLen = KSIZE_T<KData_T>::size, class KBaseT = IKRelationFactory>
class KBidUnEqualRelationFactory_T : public KRelationFactoryBaseT<KBaseT>
{
public:
	typedef KRelationFactoryBaseT<KBaseT> SuperT;
	typedef std::list<KData_T>									RELATION_LIST;
	typedef std::pair<bool, typename RELATION_LIST::iterator>	P_RELATIONLIST;
	typedef std::map<INT, P_RELATIONLIST>						RELATION_INNER_MAP;	
	typedef std::map<INT, RELATION_INNER_MAP>					RELATION_MAP;
	using SuperT::m_pHelper;
	using SuperT::m_pSender;
	class KEnumRelation : public IKEnumRelation
	{
	public:
		KEnumRelation(INT nType, IKRelationFactory *pFactory) :
			m_pMap(NULL),
			m_nType(nType),
			m_pFactory(pFactory),
			m_sRelation(nType, pFactory),
			m_bAsMaster(FALSE)
		{
			ASSERT(pFactory);
			m_nPlayerId = 0;
		}
		virtual ~KEnumRelation()
		{
		}
		BOOL Init(INT nPlayerId, BOOL bAsMaster, RELATION_INNER_MAP *pMap)
		{
			ASSERT(nPlayerId && pMap);
			m_nPlayerId = nPlayerId;
			m_pMap = pMap;
			m_iter = pMap->begin();
			m_bAsMaster = bAsMaster;
			return TRUE;
		}
		virtual VOID Reset()
		{
			m_iter = m_pMap->begin();
		}

		virtual BOOL Next(IKRelation **ppRelation)
		{
			ASSERT(m_pMap && ppRelation);
			if (m_iter == m_pMap->end())
				return FALSE;
			KData_T *pData = &(*m_iter->second.second);
			if (m_bAsMaster)
				m_sRelation.Init(m_nPlayerId, m_iter->first, pData);
			else
				m_sRelation.Init(m_iter->first, m_nPlayerId, pData);
			m_iter++;
			*ppRelation = &m_sRelation;
			return TRUE;
		}
		virtual INT GetCount()
		{
			return static_cast<INT>(m_pMap->size());
		}
	private:
		INT										m_nType;
		INT										m_nPlayerId;
		RELATION_INNER_MAP*						m_pMap;
		typename RELATION_INNER_MAP::iterator	m_iter;
		KRelation_T								m_sRelation;
		IKRelationFactory*						m_pFactory;
		BOOL									m_bAsMaster;	// m_sGuid是否作为主位玩家
	};
public:
	KBidUnEqualRelationFactory_T(
			CONST KEPLAYERRELATION_TYPE nType,
			BOOL bCountMaster = TRUE,			// 是否累加主位玩家的好友数目
			BOOL bCountTarget = TRUE,			// 是否累加次位玩家的好友数目
			BOOL bReverseOK = FALSE) :			// 是否允许反向关系存在
		m_pRelationImpl(NULL),
		m_pEnum(NULL),
		m_nType(nType),
		m_bCountMaster(bCountMaster),
		m_bCountTarget(bCountTarget),
		m_bReverseOK(bReverseOK)
	{
		m_pRelationImpl = new KRelation_T(nType, this);
		m_pEnum = new KEnumRelation(nType, this);
	}
	virtual ~KBidUnEqualRelationFactory_T()
	{
		//if (!m_manPlayerIdMasters.empty() || !m_manPlayerIdTargets.empty() || !m_listRelations.empty())
		//	KGC_Error(m_manPlayerIdMasters.size() << " " << m_manPlayerIdTargets.size() 
		//	<< " " << m_listRelations.size());
		ASSERT(m_pRelationImpl && m_pEnum);
		delete m_pRelationImpl;
		m_pRelationImpl = NULL;
		delete m_pEnum;
		m_pEnum = NULL;
	}
public:
	/***********************************************************
	 Function		: GetRelationInfoLen
	 Description	: 获取当前类型玩家关系的附带信息长度（对于某一种关系类型，附带信息长度是固定的）。
					  该附带数据可以用来表示好感度之类的信息，由每种玩家关系自行解释
	 Return			: INT
	 ***********************************************************/
	virtual INT GetRelationInfoLen()
	{
		return nInfLen;
	}
	/***********************************************************
	 Function		: CreateRelation
	 Description	: 创建玩家关系
	 Return			: BOOL
	 Param			: nPlayerIdMaster		主位玩家
	 Param			: nPlayerIdTarget		次位玩家
	 Param			: bIsMasterActive	主位玩家是否主动申请方
	 Param			: ppRelation		创建的玩家关系
	 ***********************************************************/
	virtual BOOL CreateRelation(
			INT nPlayerIdMaster,
			INT nPlayerIdTarget,
			BOOL			bIsMasterActive,
			IKRelation**	ppRelation)
	{
		BOOL									bRetCode	= FALSE;
		KRelationCounter*						pCounter	= NULL;
		IKRelation*								pRelation	= NULL;
		KRole*									pMaster		= NULL;
		KRole*									pTarget		= NULL;
		IMemBlock*								piMemBlock	= NULL;
		R2S_RELATION_SYNC_LIST_RESPOND*			pSync		= NULL;
		KRELATION_INFO*							pInfo		= NULL;
		const KRelationRuleManager::KPOLICY*	pPolicy		= NULL;


		LOG_PROCESS_ERROR(nPlayerIdMaster && nPlayerIdTarget);
		pCounter = KRelationCounter::Instance();

		// 检查反向关系是否存在
		if (!m_bReverseOK && FindRelation(nPlayerIdTarget, nPlayerIdMaster))
		{
			bRetCode = FALSE;
			goto EXIT0;
		}

		AddRelation(nPlayerIdMaster, nPlayerIdTarget, &pRelation);
		if (ppRelation)
			*ppRelation = pRelation;

		// 通知新建关系
		pMaster = g_pGameCenter->m_RoleManager.GetRole(nPlayerIdMaster);
		pTarget = g_pGameCenter->m_RoleManager.GetRole(nPlayerIdTarget);
		LOG_PROCESS_ERROR(pMaster);
		LOG_PROCESS_ERROR(pTarget);
		g_pGameCenter->m_RelationManager.AddDirty(nPlayerIdMaster);
		g_pGameCenter->m_RelationManager.AddDirty(nPlayerIdTarget);
		// m_pHelper->Dirty(nPlayerIdMaster, nPlayerIdTarget);

		piMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + sizeof(KRELATION_INFO));
		LOG_PROCESS_ERROR(piMemBlock);
		
		pSync = (R2S_RELATION_SYNC_LIST_RESPOND *)piMemBlock->GetData();
		pSync->wProtocolID	= r2s_relation_sync_list_respond;
		pSync->nCount		= 1;
		pSync->nType		= m_nType;
		pSync->byCreate		= TRUE;

		pInfo = pSync->aryInfo;
		pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
		LOG_PROCESS_ERROR(pPolicy);
		if (pPolicy->bSyncToServerAsMaster && pMaster->m_nConnIndex > 0)
		{
			pSync->nPlayerID	  = nPlayerIdMaster;
			pSync->byNotifyClient = pPolicy->bSyncToClientAsMaster;
			pInfo->nOnline = pTarget->IsOnline();
			pSync->nRole = 0;
			pInfo->nSex				= pTarget->GetSex();
			pInfo->nLevel			= pTarget->GetLevel();
			pInfo->nPlayerId		= pTarget->GetId();
			memcpy(pInfo->szRelation, pTarget->m_szName, sizeof(pTarget->m_szName));
			g_pGameCenter->m_GameServer.Send(pMaster->m_nConnIndex, piMemBlock);
		}
		if (pPolicy->bSyncToServerAsTarget && pTarget->m_nConnIndex)
		{
			pSync->nPlayerID	  = nPlayerIdTarget;
			pSync->byNotifyClient = pPolicy->bSyncToClientAsTarget;
			pSync->nRole = 1;
			pInfo->nOnline = pMaster->IsOnline();
			pInfo->nSex				= pTarget->GetSex();
			pInfo->nLevel			= pTarget->GetLevel();
			pInfo->nPlayerId		= pTarget->GetId();
			memcpy(pInfo->szRelation, pMaster->m_szName, sizeof(pMaster->m_szName));
			g_pGameCenter->m_GameServer.Send(pTarget->m_nConnIndex, piMemBlock);
		}
		AfterCreateRelation(nPlayerIdMaster, nPlayerIdTarget, m_nType);

		bRetCode = TRUE;
EXIT0:
		SAFE_RELEASE(piMemBlock);
		return bRetCode;

// 		KGC_CONFIRM_RET_FALSE(nPlayerIdMaster && nPlayerIdTarget);
// 		KRelationCounter *pCounter = KRelationCounter::Instance();
// 		
// 		// 检查反向关系是否存在
// 		if (!m_bReverseOK && FindRelation(nPlayerIdTarget, nPlayerIdMaster))
// 		{
// 			KGC_Error("");
// 			return FALSE;
// 		}
// 
// 		IKRelation *pRelation = NULL;
// 		AddRelation(nPlayerIdMaster, nPlayerIdTarget, &pRelation);
// 	
// 		if (ppRelation)
// 			*ppRelation = pRelation;
// #ifdef KG_GC_SERVER
// 		// 通知新建关系
// 		KGC_CONFIRM_RET(nPlayerIdMaster > 0 && nPlayerIdTarget > 0, TRUE);
// 		
// 		KGC_Player* pMaster = g_pPlayerManager->GetPlayer(nPlayerIdMaster);
// 		KGC_Player* pTarget = g_pPlayerManager->GetPlayer(nPlayerIdTarget);
// 		KGC_CONFIRM_RET(nPlayerIdMaster && nPlayerIdTarget, TRUE);
// 		m_pHelper->Dirty(nPlayerIdMaster, nPlayerIdTarget);
// 		
// 		INT nMasterSvrID = pMaster->GetOnlineServer();
// 		INT nTargetSvrID = pTarget->GetOnlineServer();
// 		CHAR szBuf[sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + sizeof(KRELATION_INFO)];
// 		R2S_RELATION_SYNC_LIST_RESPOND *pSync = (R2S_RELATION_SYNC_LIST_RESPOND*)szBuf;
// 		pSync->nCount = 1;
// 		pSync->nType = m_nType;
// 		pSync->ProtocolType = r2s_relation_sync_list_respond;
// 		KRELATION_INFO *pInfo = pSync->aryInfo;
// 		CONST KRelationRuleManager::KPOLICY* pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
// 		if (pPolicy->bSyncToServerAsMaster && nMasterSvrID > 0)
// 		{
// 			pSync->nPlayerID	  = nPlayerIdMaster;
// 			pSync->byNotifyClient = pPolicy->bSyncToClientAsMaster;
// 			pInfo->nOnline = nTargetSvrID > 0 ? 1 : 0;
// 			pSync->nRole = 0;
// 			strncpy(pInfo->szRelation, pTarget->GetPlayerName(), sizeof(pInfo->szRelation) - 1);
// 			pInfo->szRelation[sizeof(pInfo->szRelation) - 1] = 0;
// 			m_pSender->SendData(szBuf, sizeof(szBuf), nMasterSvrID);
// 		}
// 		if (pPolicy->bSyncToServerAsTarget && nTargetSvrID > 0)
// 		{
// 			pSync->nPlayerID	  = nPlayerIdTarget;
// 			pSync->byNotifyClient = pPolicy->bSyncToClientAsTarget;
// 			pSync->nRole = 1;
// 			pInfo->nOnline = nMasterSvrID > 0 ? 1 : 0;
// 			strncpy(pInfo->szRelation, pMaster->GetPlayerName(), sizeof(pInfo->szRelation) - 1);
// 			pInfo->szRelation[sizeof(pInfo->szRelation) - 1] = 0;
// 			m_pSender->SendData(szBuf, sizeof(szBuf), nTargetSvrID);
// 		}
// 		AfterCreateRelation(nPlayerIdMaster, nPlayerIdTarget, m_nType);
// #endif //KG_GC_SERVER
// 		return TRUE;
	}
	/***********************************************************
	 Function		: CanCreateRelation
	 Description	: 检查是否可以创建玩家关系
	 Return			: BOOL
	 Param			: nPlayerIdMaster		主位玩家
	 Param			: nPlayerIdTarget		次位玩家
	 Param			: pData			附带数据
	 Param			: nLen			附带数据长度
	 ***********************************************************/
	virtual BOOL CanCreateRelation(
			INT nPlayerIdMaster,
			INT nPlayerIdTarget)
	{
		if (!KRelationCounter::Instance()->CheckReachMaxCount(m_nType, nPlayerIdMaster) ||
			!KRelationCounter::Instance()->CheckReachMaxCount(m_nType, nPlayerIdTarget))
			return FALSE;

		if (nPlayerIdMaster == nPlayerIdTarget ||
			FindRelation(nPlayerIdMaster, nPlayerIdTarget) ||
			FindRelation(nPlayerIdTarget, nPlayerIdMaster))
		{
			return FALSE;
		}
		return TRUE;
	}
	/***********************************************************
	 Function		: GetAllRelations
	 Description	: 获取指定玩家指定角色的所有玩家关系。
	 Return			: BOOL
	 Param			: bAsMaster		是否作为主位玩家
	 Param			: pGuigd		玩家
	 Param			: ppEnum		返回的玩家关系枚举器
	 ***********************************************************/
	virtual BOOL GetAllRelations(
			BOOL				bAsMaster,
			INT nPlayerId,
			IKEnumRelation**	ppEnum)
	{
// 		BOOL bRetCode			= FALSE;
// 		RELATION_MAP* pMap		= NULL;
// 		RELATION_MAP::iterator it;
// 
// 		LOG_PROCESS_ERROR(nPlayerId && ppEnum);
// 		pMap = bAsMaster ? &m_manPlayerIdMasters : &m_manPlayerIdTargets;
// 		it = pMap->find(nPlayerId);
// 		if (it == pMap->end())
// 		{
// 			bRetCode = FALSE;
// 			goto EXIT0;
// 		}
// 		m_pEnum->Init(nPlayerId, bAsMaster, &it->second);
// 		m_pEnum->Reset();
// 		*ppEnum = m_pEnum;
// 
// 		bRetCode = TRUE;
// EXIT0:
// 		return bRetCode;

		if (!(nPlayerId && ppEnum))
		{
			ASSERT(0);
			return FALSE;
		}

		RELATION_MAP *pMap = bAsMaster ? &m_manPlayerIdMasters : &m_manPlayerIdTargets;
		typename RELATION_MAP::iterator it = pMap->find(nPlayerId);
		if (it == pMap->end())
			return FALSE;
		m_pEnum->Init(nPlayerId, bAsMaster, &it->second);
		m_pEnum->Reset();
		*ppEnum = m_pEnum;
		return TRUE;
	}
	/***********************************************************
	 Function		: FindRelation
	 Description	: 查找指定主次位玩家的玩家关系。
	 Return			: BOOL
	 Param			: nPlayerIdMaster		主位玩家
	 Param			: nPlayerIdTarget		次位玩家
	 Param			: ppRelation	返回的玩家关系
	 ***********************************************************/
	virtual BOOL FindRelation(
			INT nPlayerIdMaster,
			INT nPlayerIdTarget,
			IKRelation**		ppRelation)
	{
		if (!(nPlayerIdMaster && nPlayerIdTarget))
		{
			ASSERT(0);
			return FALSE;
		}

		KData_T *pData = NULL;
		if (!FindRelation(nPlayerIdMaster, nPlayerIdTarget, &pData))
			return FALSE;
		ASSERT(pData);
		if (ppRelation)
		{
			m_pRelationImpl->Init(nPlayerIdMaster, nPlayerIdTarget, pData);
			*ppRelation = m_pRelationImpl;
		}
		return TRUE;
	}
	/***********************************************************
	 Function		: DeleteRelation
	 Description	: 删除指定玩家关系。
	 Return			: BOOL
	 Param			: nPlayerIdMaster		主位玩家
	 Param			: nPlayerIdTarget		次位玩家
	 ***********************************************************/
	virtual BOOL DeleteRelation(INT nPlayerIdMaster, INT nPlayerIdTarget)
	{
		BOOL										bRetCode	= FALSE;
		typename RELATION_INNER_MAP::iterator		iter;
		KRole*										pMaster		= NULL;
		KRole*										pTarget		= NULL;
		IMemBlock*									piMemBlock	= NULL;
		R2S_RELATION_DEL_RELATION_RESPOND*			pNotify		= NULL;
		const KRelationRuleManager::KPOLICY*		pPolicy	= NULL;

		LOG_PROCESS_ERROR(nPlayerIdMaster && nPlayerIdTarget);
		if (!FindRelation(nPlayerIdMaster, nPlayerIdTarget, iter))
		{
			std::swap(nPlayerIdMaster, nPlayerIdTarget);
			if (!FindRelation(nPlayerIdMaster, nPlayerIdTarget, iter))
			{
				bRetCode = FALSE;
				goto EXIT0;
			}
		}

		m_listRelations.erase(iter->second.second);
		DelRelation(nPlayerIdMaster, nPlayerIdTarget, &m_manPlayerIdMasters);
		DelRelation(nPlayerIdTarget, nPlayerIdMaster, &m_manPlayerIdTargets);

		// 通知删除关系
		LOG_PROCESS_ERROR(nPlayerIdMaster > 0 && nPlayerIdTarget > 0);		
		pMaster = g_pGameCenter->m_RoleManager.GetRole(nPlayerIdMaster);
		pTarget = g_pGameCenter->m_RoleManager.GetRole(nPlayerIdTarget);
		LOG_PROCESS_ERROR(pMaster);
		LOG_PROCESS_ERROR(pTarget);
		g_pGameCenter->m_RelationManager.AddDirty(nPlayerIdMaster);
		g_pGameCenter->m_RelationManager.AddDirty(nPlayerIdTarget);
		// m_pHelper->Dirty(nPlayerIdMaster, nPlayerIdTarget);

		piMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_DEL_RELATION_RESPOND));
		LOG_PROCESS_ERROR(piMemBlock);

		pNotify = (R2S_RELATION_DEL_RELATION_RESPOND*)piMemBlock->GetData();
		pNotify->wProtocolID = r2s_relation_del_relation_respond;
		pNotify->nType = m_nType;
		pNotify->nRelationRole = 0;
		pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
		LOG_PROCESS_ERROR(pPolicy);
		if (pPolicy->bNotifyDeleteToMaster && pMaster->m_nConnIndex > 0)
		{
			pNotify->nPlayerID = nPlayerIdMaster;
			memcpy(pNotify->szRelation, pTarget->m_szName, sizeof(pNotify->szRelation));
			g_pGameCenter->m_GameServer.Send(pMaster->m_nConnIndex, piMemBlock);
		}
		if (pPolicy->bNotifyDeleteToTarget && pTarget->m_nConnIndex > 0)
		{
			pNotify->nPlayerID = nPlayerIdTarget;
			memcpy(pNotify->szRelation, pMaster->m_szName, sizeof(pNotify->szRelation));
			g_pGameCenter->m_GameServer.Send(pTarget->m_nConnIndex, piMemBlock);
		}

		bRetCode = TRUE;
EXIT0:
		SAFE_RELEASE(piMemBlock);
		return bRetCode;


// 		assert(nPlayerIdMaster && nPlayerIdTarget);
// 		typename RELATION_INNER_MAP::iterator iter;
// 		if (!FindRelation(nPlayerIdMaster, nPlayerIdTarget, iter))
// 		{
// 			std::swap(nPlayerIdMaster, nPlayerIdTarget);
// 			if (!FindRelation(nPlayerIdMaster, nPlayerIdTarget, iter))
// 				return FALSE;
// 		}
// 		m_listRelations.erase(iter->second.second);
// 		DelRelation(nPlayerIdMaster, nPlayerIdTarget, &m_manPlayerIdMasters);
// 		DelRelation(nPlayerIdTarget, nPlayerIdMaster, &m_manPlayerIdTargets);
// 
// 		// 删除数据
// 		//if (m_pDatabase)
// 		//	m_pDatabase->DelFromDatabase(m_nType, nPlayerIdMaster, nPlayerIdTarget);
// #ifdef KG_GC_SERVER
// 		// 通知删除关系
// 		KGC_CONFIRM_RET(nPlayerIdMaster > 0 && nPlayerIdTarget > 0, TRUE);		
// 		KGC_Player* pMaster = g_pPlayerManager->GetPlayer(nPlayerIdMaster);
// 		KGC_Player* pTarget = g_pPlayerManager->GetPlayer(nPlayerIdTarget);
// 		KGC_CONFIRM_RET(nPlayerIdMaster && nPlayerIdTarget, TRUE);
// 		m_pHelper->Dirty(nPlayerIdMaster, nPlayerIdTarget);
// 		INT nMasterSvrID = pMaster->GetOnlineServer();
// 		INT nTargetSvrID = pTarget->GetOnlineServer();
// 		R2S_RELATION_DEL_RELATION_RESPOND sNotify;
// 		sNotify.ProtocolType = r2s_relation_del_relation_respond;
// 		sNotify.nType = m_nType;
// 		sNotify.nRelationRole = 0;
// 		CONST KRelationRuleManager::KPOLICY* pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
// 		if (pPolicy->bNotifyDeleteToMaster && nMasterSvrID > 0)
// 		{
// 			sNotify.nPlayerID = nPlayerIdMaster;
// 			strncpy(sNotify.szRelation, pTarget->GetPlayerName(), sizeof(sNotify.szRelation) - 1);
// 			sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 			m_pSender->SendData(&sNotify, sizeof(sNotify), nMasterSvrID);
// 		}
// 		if (pPolicy->bNotifyDeleteToTarget && nTargetSvrID > 0)
// 		{
// 			sNotify.nPlayerID = nPlayerIdTarget;
// 			strncpy(sNotify.szRelation, pMaster->GetPlayerName(), sizeof(sNotify.szRelation) - 1);
// 			sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 			m_pSender->SendData(&sNotify, sizeof(sNotify), nTargetSvrID);
// 		}
// #endif //KG_GC_SERVER
// 		return TRUE;
	}
	/***********************************************************
	 Function		: Unload
	 Description	: 把指定玩家从内存中删除
	 Return			: BOOL
	 Param			: nPlayerId		玩家角色ID
	 ***********************************************************/
	virtual BOOL Unload(INT nPlayerId)
	{
		ASSERT(nPlayerId);
		Unload(nPlayerId, &m_manPlayerIdMasters, &m_manPlayerIdTargets);
		Unload(nPlayerId, &m_manPlayerIdTargets, &m_manPlayerIdMasters);
		return TRUE;
	}
	/***********************************************************
	 Function		: SaveAll
	 Description	: 存盘
	 Return			: BOOL	
	 Param			: nPlayerId		玩家角色ID
	 ***********************************************************/
	virtual BOOL SaveAll(INT nPlayerId, IKRelationSaver* pSaver)
	{
		ASSERT(nPlayerId);
		SaveAll(nPlayerId, &m_manPlayerIdMasters, &m_manPlayerIdTargets, TRUE, pSaver);
		SaveAll(nPlayerId, &m_manPlayerIdTargets, &m_manPlayerIdMasters, FALSE, pSaver);
		return TRUE;
	}
	/***********************************************************
	 Function		: Load
	 Description	: 加载玩家关系列表
	 Return			: BOOL
	 Param			: nPlayerIdMaster		主位玩家角色ID
	 Param			: nPlayerIdMaster		次位玩家角色ID
	 Param			: pData			关系数据
	 Param			: nLen			数据长度
	 ***********************************************************/
	virtual BOOL Load(INT nPlayerIdMaster, INT nPlayerIdTarget, LPVOID pData, INT nLen)
	{
		ASSERT(nPlayerIdMaster && nPlayerIdTarget);
		if (nLen != nInfLen || (nLen > 0 && pData == NULL))
		{
			ASSERT(0);
			return FALSE;
		}
		if (!m_pHelper->CheckCanLoad(nPlayerIdTarget))
			return TRUE;
		// 如果关系已经存在，则直接返回，防止覆盖内存中的关系数据
		if (FindRelation(nPlayerIdMaster, nPlayerIdTarget))
			return TRUE;
		typename RELATION_LIST::iterator iter = pData != NULL ?
			m_listRelations.insert(m_listRelations.end(), KRelation_T::Load(pData, nLen)) :
			m_listRelations.insert(m_listRelations.end(), KRelation_T::GetDefaultValue());
		AddRelation(nPlayerIdMaster, nPlayerIdTarget, &m_manPlayerIdMasters,  P_RELATIONLIST(true, iter));
		AddRelation(nPlayerIdTarget, nPlayerIdMaster, &m_manPlayerIdTargets,  P_RELATIONLIST(false, iter));

		return TRUE;
	}
	/***********************************************************
	 Function		: Remove
	 Description	: 删除指定玩家的所有关系
	 Return			: BOOL
	 Param			: nPlayerId		玩家角色ID
	 ***********************************************************/
	virtual BOOL Remove(INT nPlayerId)
	{
		ASSERT(nPlayerId);
		m_manPlayerIdMasters.erase(nPlayerId);
		m_manPlayerIdTargets.erase(nPlayerId);
		//pDatabase->Remove(nPlayerId);
		return TRUE;
	}

protected:
	BOOL SaveAll(INT nPlayerId, RELATION_MAP* pMap1, RELATION_MAP* pMap2, BOOL bMaster, IKRelationSaver* pSaver)
	{
		typename RELATION_MAP::iterator it1 = pMap1->find(nPlayerId);
		if (it1 == pMap1->end())
			return FALSE;
		RELATION_INNER_MAP *pInnerMap = &it1->second;
		typename RELATION_INNER_MAP::iterator it2 = pInnerMap->begin();

		pSaver->SaveBegin(m_nType, nInfLen, bMaster);
		while (it2 != pInnerMap->end())
		{		
			typename RELATION_INNER_MAP::iterator curr = it2++;
			INT nPlayerId2 = curr->first;
			LPVOID pData = NULL;
			if (nInfLen > 0)
				pData = curr->second.second->GetData();
			pSaver->AddRecord(nPlayerId2, pData);
		}
		pSaver->SaveEnd(m_nType);
		return TRUE;
	}

private:
	BOOL Unload(INT nPlayerId, RELATION_MAP* pMap1, RELATION_MAP* pMap2)
	{
		typename RELATION_MAP::iterator it1 = pMap1->find(nPlayerId);
		if (it1 == pMap1->end())
			return FALSE;
		RELATION_INNER_MAP *pInnerMap = &it1->second;
		typename RELATION_INNER_MAP::iterator it2 = pInnerMap->begin();
		while (it2 != pInnerMap->end())
		{		
			typename RELATION_INNER_MAP::iterator curr = it2++;
			INT nPlayerId2 = curr->first;
			if (m_pHelper->CheckCanUnload((nPlayerId2)))
			{
				RemoveRelation(nPlayerId2, nPlayerId, pMap2);
				m_listRelations.erase(curr->second.second);
				pInnerMap->erase(curr);
			}			
		}
		if (pInnerMap->empty())
		{
			pMap1->erase(it1);
		}
		return TRUE;
	}
	


	BOOL RemoveRelation(INT nPlayerId1, INT nPlayerId2, RELATION_MAP *pMap)
	{
		typename RELATION_MAP::iterator it1 = pMap->find(nPlayerId1);
		ASSERT(it1 != pMap->end());
		
		RELATION_INNER_MAP *pInnerMap = &it1->second;
		typename RELATION_INNER_MAP::iterator it2 = pInnerMap->find(nPlayerId2);
		ASSERT(it2 != pInnerMap->end());
		
		pInnerMap->erase(it2);
		if (pInnerMap->empty())
		{
			pMap->erase(it1);
		}
		return TRUE;
	}
	BOOL DelRelation(INT nKey1, INT nKey2, RELATION_MAP *pMap)
	{
		typename RELATION_MAP::iterator it = pMap->find(nKey1);
		ASSERT(it != pMap->end());
		it->second.erase(nKey2);
		return TRUE;
	}
	VOID AddRelation(INT nPlayerIdMaster, INT nPlayerIdTarget, IKRelation **ppRelation)
	{
		typename RELATION_LIST::iterator iter = m_listRelations.insert(
			m_listRelations.end(),
			KRelation_T::GetDefaultValue());
		AddRelation(nPlayerIdMaster, nPlayerIdTarget, &m_manPlayerIdMasters, P_RELATIONLIST(true, iter));
		AddRelation(nPlayerIdTarget, nPlayerIdMaster, &m_manPlayerIdTargets, P_RELATIONLIST(false, iter));
		if (ppRelation)
		{
			m_pRelationImpl->Init(nPlayerIdMaster, nPlayerIdTarget, &(*iter));
			*ppRelation = m_pRelationImpl;
		}
	}

	VOID AddRelation(INT nKey1, INT nKey2, RELATION_MAP *pMap, P_RELATIONLIST pList)
	{
		typename RELATION_MAP::iterator it = pMap->find(nKey1);
		if (it == pMap->end())
			it = pMap->insert(make_pair(nKey1, RELATION_INNER_MAP())).first;
		RELATION_INNER_MAP &sMap = it->second;
		sMap[nKey2] = pList;
	}
	BOOL FindRelation(INT nPlayerIdMaster, INT nPlayerIdTarget, typename RELATION_INNER_MAP::iterator &iter)
	{
		typename RELATION_MAP::iterator it1 = m_manPlayerIdMasters.find(nPlayerIdMaster);
		if (it1 == m_manPlayerIdMasters.end())
			return FALSE;
		RELATION_INNER_MAP *pSubMap = &it1->second;
		typename RELATION_INNER_MAP::iterator it2 = pSubMap->find(nPlayerIdTarget);
		if (it2 == pSubMap->end())
			return FALSE;
		iter = it2;
		return TRUE;
	}

	BOOL FindRelation(INT nPlayerIdMaster, INT nPlayerIdTarget, KData_T **ppData = NULL)
	{
		typename RELATION_INNER_MAP::iterator iter;
		if (!FindRelation(nPlayerIdMaster, nPlayerIdTarget, iter))
			return FALSE;
		if (ppData)
			*ppData = &(*iter->second.second);
		return TRUE;
	}
	VOID AfterCreateRelation(INT nMasterId, INT nTargetId, INT nType)
	{
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunction("Relation", "AfterAddRelation_GC", 0, "ddd", nMasterId, nTargetId, nType);
	}
protected:
	RELATION_MAP			m_manPlayerIdMasters;
	RELATION_MAP			m_manPlayerIdTargets;

private:
	RELATION_LIST			m_listRelations;
	KRelation_T*			m_pRelationImpl;
	KEnumRelation*			m_pEnum;
	CONST BOOL				m_bCountMaster;		// 是否要把主位玩家计数（用于限制最大好友数目）
	CONST BOOL				m_bCountTarget;		// 是否要把次位玩家计数（用于限制最大好友数目）
	KEPLAYERRELATION_TYPE	m_nType;			// 玩家关系类型
	BOOL					m_bReverseOK;		// 是否允许反向关系
};

#endif //__KBIDUNEQUALRELATIONFACTORY_T_H__
