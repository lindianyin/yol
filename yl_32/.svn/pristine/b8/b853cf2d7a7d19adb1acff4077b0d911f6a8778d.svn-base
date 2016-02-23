#ifndef __KBIDEQUALRELATIONFACTORY_T_H__
#define __KBIDEQUALRELATIONFACTORY_T_H__

#include "playerrelation_i.h"
#include "relationcounter.h"
#include "datarelation_t.h"
#include "bidfriendrelation.h"
#include "relationrulemanager.h"
#include <list>
#include "KRole.h"
#include "GameCenter.h"
#include "onlinegamebase/kscriptmanager.h"

template <class KData_T, class KRelation_T, CONST INT nInfLen = KSIZE_T<KData_T>::size, class KBaseT = IKRelationFactory>
class KBidEqualRelationFactory_T : public KRelationFactoryBaseT<KBaseT>
{
public:
	typedef KRelationFactoryBaseT<KBaseT> SuperT;
	typedef std::list<KData_T>											RELATION_LIST;
	typedef std::pair<bool, typename RELATION_LIST::iterator>			P_RELATIONLIST;
	typedef std::map<INT, P_RELATIONLIST>								RELATION_INNER_MAP;
	typedef std::map<INT, RELATION_INNER_MAP>							RELATION_MAP;
	using SuperT::m_pHelper;
	using SuperT::m_pSender;
	class KEnumRelation : public IKEnumRelation
	{
	public:
		KEnumRelation(INT nType, IKRelationFactory *pFactory) :
			m_pMap(NULL),
			m_pFactory(pFactory),
			m_nType(nType),
			m_sRelation(nType, pFactory)
		{
			m_nPlayerId = 0;
		}
		virtual ~KEnumRelation()
		{
		}
		BOOL Init(INT nPlayerId, RELATION_INNER_MAP *pMap)
		{
			if (!pMap)
			{
				ASSERT(0);
				return FALSE;
			}
			m_nPlayerId = nPlayerId;
			m_pMap = pMap;
			m_iter = pMap->begin();
			return TRUE;
		}
		virtual VOID Reset()
		{
			m_iter = m_pMap->begin();
		}
		virtual BOOL Next(IKRelation **ppRelation)
		{
			if (!(m_pMap && ppRelation))
			{
				ASSERT(0);
				return FALSE;
			}
			
			if (m_iter == m_pMap->end())
				return FALSE;
			KData_T *pData = &(*(m_iter->second.second));
			INT nMaster, nTarget;;
			if (m_iter->second.first == true)
			{
				nMaster = m_nPlayerId;
				nTarget = m_iter->first;
			}
			else
			{
				nMaster = m_iter->first;
				nTarget = m_nPlayerId;
			}
			m_sRelation.Init(nMaster, nTarget, pData);
			m_iter++;
			*ppRelation = &m_sRelation;
			return TRUE;
		}
		virtual INT GetCount()
		{
			return static_cast<INT>(m_pMap->size());
		}
	private:
		INT										m_nPlayerId;
		RELATION_INNER_MAP*						m_pMap;
		typename RELATION_INNER_MAP::iterator	m_iter;
		KRelation_T								m_sRelation;
		IKRelationFactory*						m_pFactory;
		INT										m_nType;
	};
public:
	KBidEqualRelationFactory_T(CONST KEPLAYERRELATION_TYPE nType) :
		m_pRelationImpl(NULL),
		m_pEnum(NULL),
		m_nType(nType)
	{
		m_pRelationImpl = new KRelation_T(nType, this);
		m_pEnum = new KEnumRelation(nType, this);
	}
	virtual ~KBidEqualRelationFactory_T()
	{
		//if (!m_mapRelations.empty() || !m_listRelations.empty())
		//	KGC_Error(m_mapRelations.size() << " " << m_listRelations.size());
		ASSERT(m_pRelationImpl && m_pEnum);
		SAFE_DELETE(m_pRelationImpl);
		SAFE_DELETE(m_pEnum);
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
	 Param			: nMaster			主位玩家
	 Param			: nTarget			次位玩家
	 Param			: bIsMasterActive	主位玩家是否主动申请方
	 Param			: ppRelation		创建的玩家关系
	 ***********************************************************/
	virtual BOOL CreateRelation(
			INT				nMaster,
			INT				nTarget,
			BOOL			bIsMasterActive,
			IKRelation**	ppRelation)
	{
		BOOL									bRetCode = FALSE;
		IKRelation*								pRelation = NULL;
		INT										nDataLen = 0;
		LPVOID									pData = NULL;
		KRole*									pMaster = NULL;
		KRole*									pTarget = NULL;
		IMemBlock*								pMemBlock = NULL;
		R2S_RELATION_SYNC_LIST_RESPOND*			pRespond = NULL;
		KRELATION_INFO*							pInfo = NULL;
		const KRelationRuleManager::KPOLICY*	pPolicy = NULL;

		LOG_PROCESS_ERROR(nMaster && nTarget);

		AddRelation(nMaster, nTarget, &pRelation);
		// 增加好友计数
		// KRelationCounter::Instance()->AddRelationCount(nMaster);
		// KRelationCounter::Instance()->AddRelationCount(nTarget);

		// 保存数据
		nDataLen = 0;
		pData = pRelation->GetInfo(nDataLen);
		//if (m_pDatabase)
		//	m_pDatabase->AddIntoDatabase(m_nType, nMaster, nTarget, pData, nDataLen);
		if (ppRelation)
			*ppRelation = pRelation;

		pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
		pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
		LOG_PROCESS_ERROR(pMaster);
		LOG_PROCESS_ERROR(pTarget);
		g_pGameCenter->m_RelationManager.AddDirty(nMaster);
		g_pGameCenter->m_RelationManager.AddDirty(nTarget);

		pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + sizeof(KRELATION_INFO));
		LOG_PROCESS_ERROR(pMemBlock);
		pRespond = (R2S_RELATION_SYNC_LIST_RESPOND*)pMemBlock->GetData();
		pRespond->wProtocolID = r2s_relation_sync_list_respond;
		pRespond->nType = m_nType;
		pRespond->nRole = 0;
		pRespond->nCount = 1;
		pRespond->byCreate = TRUE;
		pInfo = pRespond->aryInfo;
		pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
		if (pPolicy->bSyncToServerAsMaster && pMaster->m_nConnIndex)
		{
			pRespond->nPlayerID			= nMaster;
			pRespond->byNotifyClient	= pPolicy->bSyncToClientAsMaster;
			pInfo->nOnline				= pTarget->IsOnline();
			pInfo->nSex				= pTarget->GetSex();
			pInfo->nLevel			= pTarget->GetLevel();
			pInfo->nPlayerId		= pTarget->GetId();
			memcpy(pInfo->szRelation, pTarget->m_szName, sizeof(pTarget->m_szName));
			g_pGameCenter->m_GameServer.Send(pMaster->m_nConnIndex, pMemBlock);
		}
		if (pPolicy->bSyncToServerAsTarget && pTarget->m_nConnIndex)
		{
			pRespond->nPlayerID			= nTarget;
			pRespond->byNotifyClient	= pPolicy->bSyncToClientAsTarget;
			pInfo->nOnline				= pMaster->IsOnline();
			pInfo->nSex				= pTarget->GetSex();
			pInfo->nLevel			= pTarget->GetLevel();
			memcpy(pInfo->szRelation, pMaster->m_szName, sizeof(pMaster->m_szName));
			g_pGameCenter->m_GameServer.Send(pTarget->m_nConnIndex, pMemBlock);
		}

		AfterCreateRelation(nMaster, nTarget, m_nType);
		bRetCode = TRUE;
EXIT0:
		SAFE_RELEASE(pMemBlock);
		return bRetCode;

		// 通知新建关系
// 		KGC_Player* pMaster = g_pPlayerManager->GetPlayer(nMaster);
// 		KGC_Player* pTarget = g_pPlayerManager->GetPlayer(nTarget);
// 		KGC_CONFIRM_RET(pMaster && pTarget, TRUE);
// 		m_pHelper->Dirty(nMaster, nTarget);
// 		
// 		INT nMasterSvrID = pMaster->GetOnlineServer();
// 		INT nTargetSvrID = pTarget->GetOnlineServer();
// 		CHAR szBuf[sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + sizeof(KRELATION_INFO)];
// 		R2S_RELATION_SYNC_LIST_RESPOND *pSync = (R2S_RELATION_SYNC_LIST_RESPOND*)szBuf;
// 		pSync->nCount = 1;
// 		pSync->nType = m_nType;
// 		pSync->nRole = 0;
// 		pSync->nPlayerID = nMaster;
// 		pSync->ProtocolType = r2s_relation_sync_list_respond;
// 		KRELATION_INFO *pInfo = pSync->aryInfo;
// 		CONST KRelationRuleManager::KPOLICY* pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
// 		if (pPolicy->bSyncToServerAsMaster && nMasterSvrID > 0)
// 		{
// 			pSync->nPlayerID	  = nMaster;
// 			pSync->byNotifyClient = pPolicy->bSyncToClientAsMaster;
// 			pInfo->nOnline		  = nTargetSvrID > 0 ? 1 : 0;
// 			strncpy(pInfo->szRelation, pTarget->GetPlayerName(), sizeof(pInfo->szRelation) - 1);
// 			pInfo->szRelation[sizeof(pInfo->szRelation) - 1] = 0;
// 			m_pSender->SendData(szBuf, sizeof(szBuf), nMasterSvrID);
// 		}
// 		if (pPolicy->bSyncToServerAsTarget && nTargetSvrID > 0)
// 		{
// 			pSync->nPlayerID	  = nTarget;
// 			pSync->byNotifyClient = pPolicy->bSyncToClientAsTarget;
// 			pInfo->nOnline		  = nMasterSvrID > 0 ? 1 : 0;
// 			strncpy(pInfo->szRelation, pMaster->GetPlayerName(), sizeof(pInfo->szRelation) - 1);
// 			pInfo->szRelation[sizeof(pInfo->szRelation) - 1] = 0;
// 			m_pSender->SendData(szBuf, sizeof(szBuf), nTargetSvrID);
// 		}
// 
// 		AfterCreateRelation(nMaster, nTarget, m_nType);
// 
// 		return TRUE;
	}
	/***********************************************************
	 Function		: CanCreateRelation
	 Description	: 检查是否可以创建玩家关系
	 Return			: BOOL
	 Param			: nMaster		主位玩家
	 Param			: nTarget		次位玩家
	 ***********************************************************/
	virtual BOOL CanCreateRelation(
			INT			nMaster,
			INT			nTarget)
	{
		KRelationCounter *pCounter = KRelationCounter::Instance();
		if (!pCounter->CheckReachMaxCount(m_nType, nMaster) ||
			!pCounter->CheckReachMaxCount(m_nType, nTarget))
		{
			return FALSE;
		}
		if (nMaster == nTarget || FindRelation(nMaster, nTarget))
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
	 Param			: nPlayerId		玩家
	 Param			: ppEnum		返回的玩家关系枚举器
	 ***********************************************************/
	virtual BOOL GetAllRelations(
			BOOL			bAsMaster,
			INT				nPlayerId,
			IKEnumRelation**	ppEnum)
	{
		if (!ppEnum)
		{
			ASSERT(0);
			return FALSE;
		}
//		if (!bAsMaster)
//			return FALSE;
		typename RELATION_MAP::iterator it = m_mapRelations.find(nPlayerId);
		if (it == m_mapRelations.end())
			return FALSE;
		m_pEnum->Init(nPlayerId, &it->second);
		m_pEnum->Reset();
		*ppEnum = m_pEnum;
		return TRUE;
	}
	/***********************************************************
	 Function		: FindRelation
	 Description	: 查找指定主次位玩家的玩家关系。
	 Return			: BOOL
	 Param			: nMaster		主位玩家
	 Param			: nTarget		次位玩家
	 Param			: ppRelation	返回的玩家关系
	 ***********************************************************/
	virtual BOOL FindRelation(
			INT			nMaster,
			INT			nTarget,
			IKRelation**		ppRelation)
	{
		KData_T *pData = NULL;
		if (!FindRelation(nMaster, nTarget, &pData))
			return FALSE;
		ASSERT(pData);
		if (ppRelation)
		{
			m_pRelationImpl->Init(nMaster, nTarget, pData);
			*ppRelation = m_pRelationImpl;
		}
		return TRUE;
	}
	/***********************************************************
	 Function		: DeleteRelation
	 Description	: 删除指定主次位玩家的玩家关系。
	 Return			: BOOL
	 Param			: nMaster		主位玩家
	 Param			: nTarget		次位玩家
	 ***********************************************************/
	virtual BOOL DeleteRelation(INT nMaster, INT nTarget)
	{
		BOOL										bRetCode = FALSE;
		typename RELATION_MAP::iterator				it1;
		RELATION_INNER_MAP*							pMap = NULL;
		typename RELATION_INNER_MAP::iterator		it2;
		bool										bMainPos = false;
		KRole*										pMaster = NULL;
		KRole*										pTarget = NULL;
		IMemBlock*									pMemBlock = NULL;
		R2S_RELATION_DEL_RELATION_RESPOND*			pRespond = NULL;
		const KRelationRuleManager::KPOLICY*		pPolicy = NULL;

		LOG_PROCESS_ERROR(nMaster && nTarget);
		it1 = m_mapRelations.find(nMaster);
		LOG_PROCESS_ERROR(it1 != m_mapRelations.end());
		pMap = &(it1->second);
		it2 = pMap->find(nTarget);
		LOG_PROCESS_ERROR(it2 != pMap->end());
		bMainPos = it2->second.first;
		m_listRelations.erase(it2->second.second);
		pMap->erase(it2);
		if (pMap->empty())
		{
			m_mapRelations.erase(it1);
		}
		it1 = m_mapRelations.find(nTarget);
		// KGC_CONFIRM_RET_FALSE (it1 != m_mapRelations.end());
		
		it1->second.erase(nMaster);
		//if (m_pDatabase)
		//{
		//	if (bMainPos) // 如果nMaster为主位
		//		m_pDatabase->DelFromDatabase(m_nType, nMaster, nTarget);
		//	else
		//		m_pDatabase->DelFromDatabase(m_nType, nTarget, nMaster);
		//}

		// 通知删除关系
		pMaster = g_pGameCenter->m_RoleManager.GetRole(nMaster);
		pTarget = g_pGameCenter->m_RoleManager.GetRole(nTarget);
		LOG_PROCESS_ERROR(pMaster);
		LOG_PROCESS_ERROR(pTarget);
		g_pGameCenter->m_RelationManager.AddDirty(nMaster);
		g_pGameCenter->m_RelationManager.AddDirty(nTarget);

		pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_DEL_RELATION_RESPOND));
		LOG_PROCESS_ERROR(pMemBlock);

		pRespond = (R2S_RELATION_DEL_RELATION_RESPOND*)pMemBlock->GetData();
		pRespond->wProtocolID = r2s_relation_del_relation_respond;
		pRespond->nType = m_nType;
		pRespond->nRelationRole = 0;
		pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
		if (pPolicy->bNotifyDeleteToMaster && pMaster->m_nConnIndex > 0)
		{
			pRespond->nPlayerID = nMaster;
			memcpy(pRespond->szRelation, pTarget->m_szName, sizeof(pTarget->m_szName));
			g_pGameCenter->m_GameServer.Send(pMaster->m_nConnIndex, pMemBlock);
		}
		if (pPolicy->bNotifyDeleteToTarget && pTarget->m_nConnIndex > 0)
		{
			pRespond->nPlayerID = nTarget;
			memcpy(pRespond->szRelation, pMaster->m_szName, sizeof(pMaster->m_szName));
			g_pGameCenter->m_GameServer.Send(pTarget->m_nConnIndex, pMemBlock);
		}

		bRetCode = TRUE;
EXIT0:
		SAFE_RELEASE(pMemBlock);
		return bRetCode;

		// 通知删除关系		
// 		KGC_Player* pMaster = g_pPlayerManager->GetPlayer(nMaster);
// 		KGC_Player* pTarget = g_pPlayerManager->GetPlayer(nTarget);
// 		KGC_CONFIRM_RET(nMaster && nTarget, TRUE);
// 		m_pHelper->Dirty(nMaster, nTarget);
// 		
// 		INT nMasterSvrID = pMaster->GetOnlineServer();
// 		INT nTargetSvrID = pTarget->GetOnlineServer();
// 		R2S_RELATION_DEL_RELATION_RESPOND sNotify;
// 		sNotify.ProtocolType = r2s_relation_del_relation_respond;
// 		sNotify.nType = m_nType;
// 		sNotify.nRelationRole = 0;
// 		CONST KRelationRuleManager::KPOLICY* pPolicy = KRelationRuleManager::Instance()->GetPolicy(m_nType);
// 		if (pPolicy->bNotifyDeleteToMaster && nMasterSvrID > 0)
// 		{
// 			sNotify.nPlayerID = nMaster;
// 			strncpy(sNotify.szRelation, pTarget->GetPlayerName(), sizeof(sNotify.szRelation) - 1);
// 			sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 			m_pSender->SendData(&sNotify, sizeof(sNotify), nMasterSvrID);
// 		}
// 		if (pPolicy->bNotifyDeleteToTarget && nTargetSvrID > 0)
// 		{
// 			sNotify.nPlayerID = nTarget;
// 			strncpy(sNotify.szRelation, pMaster->GetPlayerName(), sizeof(sNotify.szRelation) - 1);
// 			sNotify.szRelation[sizeof(sNotify.szRelation) - 1] = 0;
// 			m_pSender->SendData(&sNotify, sizeof(sNotify), nTargetSvrID);
// 		}
//
//		return TRUE;
	}
	/***********************************************************
	 Function		: Unload
	 Description	: 把指定玩家从内存中删除
	 Return			: BOOL
	 Param			: nPlayerId		玩家角色ID
	 ***********************************************************/
	virtual BOOL Unload(INT nPlayerId)
	{
		if(!nPlayerId)
		{
			ASSERT(0);
			return FALSE;
		}

		typename RELATION_MAP::iterator it1 = m_mapRelations.find(nPlayerId);
		if (it1 == m_mapRelations.end())
			return FALSE;
		RELATION_INNER_MAP *pInnerMap = &it1->second;
		typename RELATION_INNER_MAP::iterator it2 = pInnerMap->begin();
		while (it2 != pInnerMap->end())
		{
			typename RELATION_INNER_MAP::iterator curr = it2++;
			INT nPlayerId2 = curr->first;
			ASSERT(nPlayerId != nPlayerId2);
			if (m_pHelper->CheckCanUnload(nPlayerId2))
			{
				RemoveRelation(nPlayerId2, nPlayerId);
				m_listRelations.erase(curr->second.second);
				pInnerMap->erase(curr);
			}
		}
		if (pInnerMap->empty())
		{
			m_mapRelations.erase(it1);
		}
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
		// KGC_CONFIRM_RET_FALSE(nPlayerId);
		typename RELATION_MAP::iterator it1 = m_mapRelations.find(nPlayerId);
		if (it1 == m_mapRelations.end())
			return FALSE;
		RELATION_INNER_MAP *pInnerMap = &it1->second;
		typename RELATION_INNER_MAP::iterator it2 = pInnerMap->begin();
		if (it2 == pInnerMap->end())
			return TRUE;
		pSaver->SaveBegin(m_nType, nInfLen, TRUE);
		do
		{
			typename RELATION_INNER_MAP::iterator curr = it2++;
			INT nPlayerId2 = curr->first;
			ASSERT(nPlayerId != nPlayerId2);
			LPVOID pData = NULL;
			if (nInfLen > 0 && curr->second.first == true)
			{
				pData = curr->second.second->GetData();
				pSaver->AddRecord(nPlayerId2, pData);
			}
		} while (it2 != pInnerMap->end());
		pSaver->SaveEnd(m_nType);

		it2 = pInnerMap->begin();
		if (it2 == pInnerMap->end())
			return TRUE;
			
		pSaver->SaveBegin(m_nType, nInfLen, FALSE);
		do
		{
			typename RELATION_INNER_MAP::iterator curr = it2++;	
			INT nPlayerId2 = curr->first;
			ASSERT(nPlayerId != nPlayerId2);
			LPVOID pData = NULL;
			if (nInfLen > 0 && curr->second.first == false)
			{
				pData = curr->second.second->GetData();
				pSaver->AddRecord(nPlayerId2, pData);
			}
		} while (it2 != pInnerMap->end());
		pSaver->SaveEnd(m_nType);
		return TRUE;
	}
	/***********************************************************
	 Function		: Load
	 Description	: 加载玩家关系列表
	 Return			: BOOL
	 Param			: nMaster		主位玩家角色ID
	 Param			: nTarget		次位玩家角色ID
	 Param			: pData			关系数据
	 Param			: nLen			数据长度
	 ***********************************************************/
	virtual BOOL Load(INT nMaster, INT nTarget, LPVOID pData, INT nLen)
	{
		if (nLen != nInfLen || (nLen > 0 && pData == NULL))
		{
			ASSERT(0);
			return FALSE;
		}
		if (!m_pHelper->CheckCanLoad(nTarget))
			return TRUE;
		// 如果关系已经存在，则直接返回，防止覆盖内存中的关系数据
		if (FindRelation(nMaster, nTarget))
			return TRUE;

		typename RELATION_LIST::iterator iter = pData != NULL ?
			m_listRelations.insert(m_listRelations.end(), KRelation_T::Load(pData, nLen)) :
			m_listRelations.insert(m_listRelations.end(), KRelation_T::GetDefaultValue());
		AddRelation(nMaster, nTarget, P_RELATIONLIST(true, iter));
		AddRelation(nTarget, nMaster, P_RELATIONLIST(false, iter));
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
		typename RELATION_MAP::iterator it1 = m_mapRelations.find(nPlayerId);
		if (it1 == m_mapRelations.end())
			return TRUE;
		RELATION_INNER_MAP *pInnerMap = &it1->second;
		typename RELATION_INNER_MAP::iterator it2 = pInnerMap->begin();
		for (; it2 != pInnerMap->end(); it2++)
		{
			ASSERT(nPlayerId != it2->first);
			RemoveRelation(it2->first, nPlayerId);
			m_listRelations.erase(it2->second.second);
		}
		m_mapRelations.erase(it1);
		//pDatabase->Remove(nPlayerId);
		return TRUE;
	}
	VOID AfterCreateRelation(INT nMasterId, INT nTargetId, INT nType)
	{
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunction("Relation", "AfterAddRelation_GC", 0, 
			"ddd", nMasterId, nTargetId, nType);
		// g_pGCScript->CallTableFunction("Relation", "AfterAddRelation_GC", 0, "ddd", nMasterId, nTargetId, nType);
	}
private:
	BOOL RemoveRelation(INT nPlayerId1, INT nPlayerId2)
	{
		typename RELATION_MAP::iterator it1 = m_mapRelations.find(nPlayerId1);
		ASSERT(it1 != m_mapRelations.end());
		RELATION_INNER_MAP *pInnerMap = &it1->second;
		typename RELATION_INNER_MAP::iterator it2 = pInnerMap->find(nPlayerId2);
		ASSERT(it2 != pInnerMap->end());
		
		pInnerMap->erase(it2);
		if (pInnerMap->empty())
		{
			m_mapRelations.erase(it1);
		}
		return TRUE;
	}
	VOID AddRelation(INT nMaster, INT nTarget, IKRelation **ppRelation)
	{
		typename RELATION_LIST::iterator iter = m_listRelations.insert(
			m_listRelations.end(),
			KRelation_T::GetDefaultValue());
		AddRelation(nMaster, nTarget, P_RELATIONLIST(true, iter));
		AddRelation(nTarget, nMaster, P_RELATIONLIST(false, iter));
		if (ppRelation)
		{
			KData_T *pData = &(*iter);
			m_pRelationImpl->Init(nMaster, nTarget, pData);
			*ppRelation = m_pRelationImpl;
		}
	}
	VOID AddRelation(INT nPlayerId1, INT nPlayerId2, P_RELATIONLIST pList)
	{
		typename RELATION_MAP::iterator it = m_mapRelations.find(nPlayerId1);
		if (it == m_mapRelations.end())
			it = m_mapRelations.insert(make_pair(nPlayerId1, RELATION_INNER_MAP())).first;
		RELATION_INNER_MAP &sMap = it->second;
		sMap[nPlayerId2] = pList;
	}
	BOOL FindRelation(INT nMaster, INT nTarget, KData_T **ppData = NULL)
	{
		typename RELATION_MAP::iterator it = m_mapRelations.find(nMaster);
		if (it == m_mapRelations.end())
			return FALSE;
		RELATION_INNER_MAP *pMap = &(it->second);
		typename RELATION_INNER_MAP::const_iterator iter = pMap->find(nTarget);
		if (iter == pMap->end())
			return FALSE;
		if (ppData)
			*ppData = &(*(iter->second.second));
		return TRUE;
	}

private:
	RELATION_MAP			m_mapRelations;
	RELATION_LIST			m_listRelations;
	KRelation_T*			m_pRelationImpl;
	KEnumRelation*			m_pEnum;
	KEPLAYERRELATION_TYPE	m_nType;
};

#endif //__KBIDEQUALRELATIONFACTORY_H__
