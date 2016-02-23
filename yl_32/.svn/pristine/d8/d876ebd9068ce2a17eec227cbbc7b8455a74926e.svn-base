/* -------------------------------------------------------------------------
//	文件名		：	relationmanagers_gc.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "relationmanager.h"
#include "playerrelation_i.h"
#include "relationfactorylist.h"
#include "relationrulemanager.h"
#include "trainingfactory.h"
#include "trainingsetting.h"
#include <algorithm>
#include "datasaveloader.h"

class RoleTraverseSaver
{
	RelationSaveLoader& m_saveLoader;

public:
	RoleTraverseSaver(RelationSaveLoader& saveLoader)
		: m_saveLoader(saveLoader)
	{

	}

	BOOL operator()(DWORD dwPlayerId, KRole kRole)
	{
		ASSERT(dwPlayerId);
		m_saveLoader.Reset();
		m_saveLoader.AddHead(dwPlayerId);
		for (INT nType = 0; nType < emKPLAYERRELATION_TYPE_COUNT; nType++)
		{
			IKRelationFactory *pFactory = NULL;
			if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory))
			{
				return FALSE;
			}
			pFactory->SaveAll(dwPlayerId, &m_saveLoader);
		}
		g_pGameCenter->m_MiscDB.DoSaveRelationData(dwPlayerId, m_saveLoader.GetSaveBuf(), m_saveLoader.GetSize());
		return TRUE;
	}
};

KRelationManager_GC::KTRAINING_EVENT KRelationManager_GC::ms_aTrainingEvent[] = {		// 师徒检查
	{&KRelationManager_GC::TrainingCheck_Student, &KRelationManager_GC::TrainingEvent_Student},
	{&KRelationManager_GC::TrainingCheck_Teacher, &KRelationManager_GC::TrainingEvent_Teacher},
	// {&KRelationManager_GC::TrainingCheck_TimeOut, &KRelationManager_GC::TrainingEvent_TimeOut},
};

// extern BOOL g_bGlobalServer;

KRelationManager_GC::KRelationManager_GC() :
	m_nMaxFavorEveryDayWithIB(500),
	m_nMaxFavorEveryDayWithoutIB(120),
	m_nMinFavor(1),
	m_nMaxFavor(1000000),
	m_nMaxRelationCount(100),
	m_tLastSearchStudent(0),
	m_tLastSearchTeacher(0),
	m_nLastStudentId(0),
	m_nLastTeacherId(0)
{
}

KRelationManager_GC::~KRelationManager_GC()
{
	m_mpCloseFriend.clear();
	m_vecStudentId.clear();
	m_vecTeacherId.clear();
	m_vecFriendAwardLevel.clear();
}

BOOL KRelationManager_GC::Init()
{
// 	if (!KGC_RelationManager::Init())
// 		return FALSE;
// 	KIniFile ini;
// 	if (ini.Load("\\setting\\relation\\relation.ini"))
// 	{
// 		LPCSTR pszSection = "system";
// 		ini.GetInteger(pszSection, "MaxFavorEveryDayWithIB", 500, &m_nMaxFavorEveryDayWithIB);
// 		ini.GetInteger(pszSection, "MaxFavorEveryDayWithoutIB",120, &m_nMaxFavorEveryDayWithoutIB);
// 		ini.GetInteger(pszSection, "MinFavor", 1, &m_nMinFavor);
// 		ini.GetInteger(pszSection, "MaxFavor", 1000000, &m_nMaxFavor);
// 		ini.GetInteger(pszSection, "MaxRelationCount", 100, &m_nMaxRelationCount);
// 		LPCSTR pszSectionAward = "friendaward";
// 		INT nCount =0;
// 		CHAR szReadData[256];
// 		INT nMaxCount = 0;
// 		INT nAward	= 0;
// 		ini.GetInteger(pszSectionAward, "Count", 0, &nCount);
// 		m_vecFriendAwardLevel.clear();
// 		KFriendAward cFriendAward;
// 
// 		for (INT i = 0; i < nCount; ++i)
// 		{
// 			sprintf(szReadData, "MaxLevel%d", i);
// 			ini.GetInteger(pszSectionAward, szReadData, 0, &nMaxCount);
// 			sprintf(szReadData, "Award%d", i);
// 			ini.GetInteger(pszSectionAward, szReadData, 0, &nAward);
// 			cFriendAward.nAward = nAward;
// 			cFriendAward.nMaxNum = nMaxCount;
// 			m_vecFriendAwardLevel.push_back(cFriendAward);
// 		}
// 	}
// 	m_mpCloseFriend.clear();
// 	// 师徒关系配置文件表初始化
// 	g_cTrainingSetting.Init();
	return TRUE;
}


BOOL KRelationManager_GC::UnInit()
{
	return SaveAll();
}


BOOL KRelationManager_GC::Activate()
{
	if (g_pGameCenter->m_nWorkLoop % (GAME_FPS * 10) == 0)
	{
		for (SET_DIRTY_PLAYER::iterator it = m_setDirtyPlayerId.begin();
			it != m_setDirtyPlayerId.end(); ++it)
		{
			SavePlayerData(*it);
		}
		if (!m_setDirtyPlayerId.empty()) 
		{ 
			m_setDirtyPlayerId.clear();
		}
	}
	return TRUE;
}

BOOL KRelationManager_GC::SaveAll()
{
// 	RelationSaveLoader cSaveLoader;
// 	RoleTraverseSaver t(cSaveLoader);
// 	g_pGameCenter->m_RoleManager.Traverse(t);

	for (SET_DIRTY_PLAYER::iterator it = m_setDirtyPlayerId.begin();
		it != m_setDirtyPlayerId.end(); ++it)
	{
		SavePlayerData(*it);
	}
	m_setDirtyPlayerId.clear();

	return TRUE;
}


BOOL KRelationManager_GC::OnLoadPlayerData( INT nPlayerId, BYTE* pbyData, size_t uDataLen )
{
	BOOL bRetCode			= FALSE;

	if (pbyData != NULL && uDataLen > 0)
	{
		bRetCode = RelationSaveLoader::OnLoadRelayData(nPlayerId, pbyData, uDataLen);
		LOG_PROCESS_ERROR(bRetCode);

		NotifyRelationListLoaded(nPlayerId);
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL KRelationManager_GC::SavePlayerData( INT nPlayerId )
{
	ASSERT(nPlayerId);
	RelationSaveLoader saveLoader;
	saveLoader.Reset();
	saveLoader.AddHead(nPlayerId);
	for (INT nType = 0; nType < emKPLAYERRELATION_TYPE_COUNT; nType++)
	{
		IKRelationFactory *pFactory = NULL;
		if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory))
		{
			return FALSE;
		}
		pFactory->SaveAll(nPlayerId, &saveLoader);
	}
	g_pGameCenter->m_MiscDB.DoSaveRelationData(nPlayerId, saveLoader.GetSaveBuf(), saveLoader.GetSize());
	return TRUE;
}

void KRelationManager_GC::OnPlayerLeave( INT nPlayerId )
{
	NotifyOnline(nPlayerId, FALSE);
	SET_DIRTY_PLAYER::iterator it = m_setDirtyPlayerId.find(nPlayerId);
	if (it != m_setDirtyPlayerId.end())
	{
		SavePlayerData(nPlayerId);
		m_setDirtyPlayerId.erase(nPlayerId);
	}
	Unload(nPlayerId);
}

void KRelationManager_GC::AddDirty( DWORD dwPlayerId )
{
	SET_DIRTY_PLAYER::iterator it = m_setDirtyPlayerId.find(dwPlayerId);
	if (it == m_setDirtyPlayerId.end())
	{
		m_setDirtyPlayerId.insert(dwPlayerId);
	}
}

// 同步好友列表
BOOL KRelationManager_GC::SyncFriendListToMyself(INT nPlayerId )
{
	BOOL								bRetCode		= FALSE;
	IKRelationFactory*					pFriendFactory	= NULL;
	IKEnumRelation*						pEnumMaster		= NULL;
	IKEnumRelation*						pEnumTarget		= NULL;
	IKEnumRelation*						pEnum			= NULL;
	INT									nRelationCount	= 0;
	IMemBlock*							pMemBlock		= NULL;
	IMemBlock_Resizable*				pMemResize		= NULL;
	R2S_RELATION_FRIEND_INFO_RESPOND*		pSync			= NULL;
	INT									nCount			= 0;
	KRole*								pRole			= NULL;

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFriendFactory);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = pFriendFactory->GetAllRelations(true, nPlayerId, &pEnumMaster) || 
		pFriendFactory->GetAllRelations(false, nPlayerId, &pEnumTarget);
	PROCESS_ERROR(bRetCode);

	if (pEnumMaster)
	{
		pEnumMaster->Reset();
		nRelationCount += pEnumMaster->GetCount();

	}
	if (pEnumTarget)
	{
		pEnumTarget->Reset();
		nRelationCount += pEnumTarget->GetCount();
	}
	LOG_PROCESS_ERROR(nRelationCount <= 1024);

	pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO) * 1024);
	LOG_PROCESS_ERROR(pMemBlock);

	pSync = (R2S_RELATION_FRIEND_INFO_RESPOND*)pMemBlock->GetData();

	if (pEnumMaster)
		GetFriendListToMyselfInfo(pEnumMaster, nPlayerId, nRelationCount, pSync->aryInfo, nCount);
	if (pEnumTarget)
		GetFriendListToMyselfInfo(pEnumTarget, nPlayerId, nRelationCount, pSync->aryInfo, nCount);

	if (nCount > 0)
	{
		pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
		LOG_PROCESS_ERROR(pRole);

		pMemBlock->QueryInterface(IID_IMemBlock_Resizable, (void**)&pMemResize);
		LOG_PROCESS_ERROR(pMemResize);

		bRetCode = pMemResize->SetSmallerSize(sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO) * nCount);
		LOG_PROCESS_ERROR(bRetCode);
		pSync->wProtocolID = r2s_relation_friend_info_respond;
		pSync->nCount = nCount;
		pSync->nPlayerID = nPlayerId;
		bRetCode = g_pGameCenter->m_GameServer.Send(pRole->m_nConnIndex, pMemBlock);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(pMemBlock);
	SAFE_RELEASE(pMemResize);
	return bRetCode;

// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFriendFactory))
// 	{
// 		// assert(0);
// 		assert(0);
// 		return FALSE;
// 	}
// 	if (!pFriendFactory->GetAllRelations(true, nPlayerId, &pEnumMaster) && 
// 		!pFriendFactory->GetAllRelations(false, nPlayerId, &pEnumTarget))
// 		return FALSE;
// 
// 	INT nRelationCount = 0;	
// 	
// 	if (pEnumMaster)
// 	{
// 		pEnumMaster->Reset();
// 		nRelationCount += pEnumMaster->GetCount();
// 		
// 	}
// 	if (pEnumTarget)
// 	{
// 		pEnumTarget->Reset();
// 		nRelationCount += pEnumTarget->GetCount();
// 	}
// 	assert(nRelationCount <= 1024);
// 	CHAR szBuf[sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO) * 1024];
// 	R2S_RELATION_FRIEND_INFO_RESPOND *pSync = (R2S_RELATION_FRIEND_INFO_RESPOND*)szBuf;
// 
// 	INT nCount = 0;
// 	if (pEnumMaster)
// 		GetFriendListToMyselfInfo(pEnumMaster, nPlayerId, nRelationCount, pSync->aryInfo, nCount);
// 	if (pEnumTarget)
// 		GetFriendListToMyselfInfo(pEnumTarget, nPlayerId, nRelationCount, pSync->aryInfo, nCount);
// 	
// 	if (nCount > 0)
// 	{
// 		INT nPackLen = sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO) * nCount;
// 		pSync->wProtocolID = r2s_relation_friend_info_respond;
// 		pSync->nCount = nCount;
// 		pSync->nPlayerID = nPlayerId;
// 		PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(nPackLen);
// 		memcpy(sBuf.GetBuf(), szBuf, nPackLen);
// 		m_pProtocolProcessor->SendPacket(sBuf, nConnectID);
// 	}
// 	return TRUE;
}

BOOL KRelationManager_GC::GetFriendListToMyselfInfo(IKEnumRelation *pEnum, 
													INT nPlayerSelf,
													INT nMaxCount,
													KRELATION_PROTO_FRIENDINFO aryInfo[], 
													INT& nCount)
										  
{
	BOOL						bRetCode			= FALSE;
	IKRelation*					pRelation			= NULL;
	IKBidFriendRelation*		pFriend				= NULL;
	KRole*						pMaster				= NULL;
	INT							nRelationID			= 0;
	KRole*						playerRelation		= NULL;
	KRELATION_PROTO_FRIENDINFO*	pInfo				= NULL;

	LOG_PROCESS_ERROR(nPlayerSelf);
	pMaster = g_pGameCenter->m_RoleManager.GetRole(nPlayerSelf);
	LOG_PROCESS_ERROR(pMaster);

	while (pEnum->Next(&pRelation) && nCount < nMaxCount)
	{
		LOG_PROCESS_ERROR(pRelation);

		bRetCode = pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend);
		LOG_PROCESS_ERROR(bRetCode);

		nRelationID = pRelation->GetOtherPlayerId(nPlayerSelf);

		if (nRelationID <= 0)
		{
			// TODO: wangbin: assert(0);
			ASSERT(0);
			continue;
		}
		playerRelation = g_pGameCenter->m_RoleManager.GetRole(nRelationID);
		if (!playerRelation)
		{
			// TODO: wangbin: assert(0);
			ASSERT(0);
			continue;
		}
		pInfo = &aryInfo[nCount++];
		InitRelationInfo(pMaster, playerRelation, pFriend->GetFavor(), pFriend->GetLastTalkTime(), pInfo);
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;

// 	IKRelation *pRelation = NULL;
// 	IKBidFriendRelation *pFriend = NULL;
// 	KRole* pMaster = g_pGameCenter->m_RoleManager.GetRole(nPlayerSelf);
// 
// 	if (!pMaster) 
// 		return FALSE;
// 	while (pEnum->Next(&pRelation) && nCount < nMaxCount)
// 	{
// 		// KGC_CONFIRM_CONTINUE(pRelation);
// 		if (!pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend))
// 		{
// 			// assert(0);
// 			assert(0);
// 			return FALSE;
// 		}
// 		INT nRelationID = pRelation->GetOtherPlayerId(nPlayerSelf);
// 
// 		if (nRelationID <= 0)
// 		{
// 			// TODO: wangbin: assert(0);
// 			continue;
// 		}
// 		KRole* playerRelation = g_pGameCenter->m_RoleManager.GetRole(nRelationID);
// 		if (!playerRelation)
// 		{
// 			// TODO: wangbin: assert(0);
// 			continue;
// 		}
// 		KRELATION_PROTO_FRIENDINFO *pInfo = &aryInfo[nCount++];
// 		InitRelationInfo(pMaster, playerRelation, pFriend->GetFavor(), pInfo);
// 	}
// 	return TRUE;
}
// 把自己信息同步给所有在线好友
BOOL KRelationManager_GC::SyncMyselfToFriendList(INT nPlayerID, KRole* pPlayer)
{
	ASSERT(nPlayerID > 0 && pPlayer);
	
	SyncMyselfToFriendList(pPlayer, nPlayerID, TRUE);
	SyncMyselfToFriendList(pPlayer, nPlayerID, FALSE);
	return TRUE;
}

BOOL KRelationManager_GC::SyncMyselfToFriendList(KRole* pPlayer, INT nPlayerID, BOOL bMaster)
{
	BOOL							bRetCode			= FALSE;
	IKRelationFactory*				pFriendFactory		= NULL;
	IKEnumRelation*					pEnum				= NULL;
	IKRelation*						pRelation			= NULL;
	IKBidFriendRelation*			pFriend				= NULL;
	INT								nCount				= 0;
	IMemBlock*						pMemBlock			= NULL;
	R2S_RELATION_FRIEND_INFO_RESPOND* pSync				= NULL;
	KRELATION_PROTO_FRIENDINFO*		pInfo				= NULL;
	INT								nRelationID			= 0;
	KRole*							pRole				= NULL;
	KRole*							pMaster				= NULL;

	pMaster = g_pGameCenter->m_RoleManager.GetRole(nPlayerID);
	LOG_PROCESS_ERROR(pMaster);

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFriendFactory);
	PROCESS_ERROR(bRetCode);

	bRetCode = pFriendFactory->GetAllRelations(bMaster, nPlayerID, &pEnum);
	PROCESS_ERROR(bRetCode);

	pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO));
	LOG_PROCESS_ERROR(pMemBlock);
	while (pEnum->Next(&pRelation))
	{
		LOG_PROCESS_ERROR(pRelation);

		bRetCode = pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend);
		LOG_PROCESS_ERROR(bRetCode);

		nRelationID = pRelation->GetOtherPlayerId(nPlayerID);
		LOG_PROCESS_ERROR(nRelationID > 0);

		pRole = g_pGameCenter->m_RoleManager.GetRole(nRelationID);
		if (!pRole)
		{
			continue;
		}

		if (pRole->m_nConnIndex > 0)
		{
			pSync = (R2S_RELATION_FRIEND_INFO_RESPOND*)pMemBlock->GetData();
			InitRelationInfo(pRole, pMaster, pFriend->GetFavor(), pFriend->GetLastTalkTime(), pSync->aryInfo);
			pSync->wProtocolID		= r2s_relation_friend_info_respond;
			pSync->nCount			= 1;
			pSync->nPlayerID		= nRelationID;
			g_pGameCenter->m_GameServer.Send(pRole->m_nConnIndex, pMemBlock);
		}
	}

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(pMemBlock);
	return bRetCode;

// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFriendFactory))
// 	{
// 		// assert(0);
// 		assert(0);
// 		return FALSE;
// 	}
// 	if (!pFriendFactory->GetAllRelations(bMaster, nPlayerID, &pEnum))
// 		return FALSE;
// 
// 	IKRelation *pRelation = NULL;
// 	IKBidFriendRelation *pFriend = NULL;
// 	INT nCount = 0;
// 	INT nPackLen = sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO);
// 
// 	while (pEnum->Next(&pRelation))
// 	{
// 		// KGC_CONFIRM_RET_FALSE(pRelation);
// 		if (!pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend))
// 		{
// 			// assert(0);
// 			assert(0);
// 			return FALSE;
// 		}
// 		INT nRelationID = pRelation->GetOtherPlayerId(nPlayerID);
// 		if (nRelationID <= 0)
// 		{
// 			continue;
// 		}
// 		KRole* pPlayerRelation = g_pPlayerManager->GetPlayer(nRelationID);
// 		if (!pPlayerRelation)
// 		{
// 			continue;
// 		}
// 		INT nServerID = pPlayerRelation->GetOnlineServer();
// 		if (nServerID > 0)
// 		{
// 			PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(nPackLen);
// 			R2S_RELATION_FRIEND_INFO_RESPOND *pSync = (R2S_RELATION_FRIEND_INFO_RESPOND*)sBuf.GetBuf();
// 			InitRelationInfo(pPlayer, pFriend->GetFavor(), pSync->aryInfo);
// 			pSync->ProtocolType = r2s_relation_friend_info_respond;
// 			pSync->nCount = 1;
// 			pSync->nPlayerID = nRelationID;
// 			m_pProtocolProcessor->SendPacket(sBuf, nServerID);
// 		}
// 	}	
// 	return TRUE;
}

BOOL KRelationManager_GC::CheckIsLoadEnd(KRole* pPlayerApp, INT nAppId, BOOL bSendMsg /*= TRUE*/)
{
	return pPlayerApp->IsOnline();
// 	BOOL bRet	= TRUE;
// 	if (!pPlayerApp->GetRelayLoaded())
// 	{
// 		// 系统提示
// 		INT nMasterSvrID = pPlayerApp->GetOnlineServer();
// 		if (nMasterSvrID > 0 && bSendMsg == TRUE)
// 		{
// 			g_ChatChannelManager.PostSystemMessage(nAppId, KD_STR_SYSTEM, KD_STR_NOTSYNCEND, nMasterSvrID);
// 		}
// 		bRet	= FALSE;
// 	}
// 	return bRet;
}

BOOL KRelationManager_GC::CheckIsLoadEnd(INT nAppId)
{
	KRole* pAppPlayer = g_pGameCenter->m_RoleManager.GetRole(nAppId);
	if (pAppPlayer)
		return CheckIsLoadEnd(pAppPlayer, nAppId, FALSE);
	return FALSE;
}

BOOL KRelationManager_GC::CheckCreateDepRep(INT nType, INT nRole, INT nAppId, INT nDstId)
{
	BOOL						bRetCode		= FALSE;
	KRelationRuleManager*		pRuleManager	= NULL;

	LOG_PROCESS_ERROR(nAppId > 0 && nDstId > 0 && nAppId != nDstId);
	if (0 == nRole)
		std::swap(nAppId, nDstId);

	pRuleManager = KRelationRuleManager::Instance();
	if (!pRuleManager->CheckCreateDependency(nType, nAppId, nDstId) ||
		!pRuleManager->CheckCreateRepugnance(nType, nAppId, nDstId))
	{
		bRetCode = FALSE;
		goto EXIT0;
	}
	bRetCode = TRUE;

EXIT0:
	return bRetCode;
}

BOOL KRelationManager_GC::CheckDelDepRep(INT nType, INT nRole, INT nAppId, INT nDstId)
{
	BOOL					bRetCode		= FALSE;

	LOG_PROCESS_ERROR(nAppId > 0 && nDstId > 0 && nAppId != nDstId);
	if (0 == nRole)
		std::swap(nAppId, nDstId);

	bRetCode = FALSE;
	if (KRelationRuleManager::Instance()->CheckDeleteDependency(nType, nAppId, nDstId))
		bRetCode = TRUE;

EXIT0:
	return bRetCode;
}

BOOL KRelationManager_GC::ResetLimtWhenCrossDay(INT nAppId, INT nDstId)
{
	// KGC_CONFIRM_RET_FALSE(nAppId != nDstId);
// 	KRole* pAppPlayer = g_pPlayerManager->GetPlayer(nAppId);
// 	KRole* pDstPlayer = g_pPlayerManager->GetPlayer(nDstId);
// 	// KGC_CONFIRM_RET_FALSE(pAppPlayer && pDstPlayer);
// 
// 	// 检查加亲密度日期
// 	time_t tt = KSysService::Time(NULL);
// 	tm now = *KSysService::LocalTime(&tt);
// 	if (now.tm_yday != pAppPlayer->GetLastAddRelationDay())
// 	{
// 		pAppPlayer->SetLastAddRelationDay(now.tm_yday);
// 		KBidFriendRelation::ClearFavorLimit(nAppId);
// 	}
// 	if (now.tm_yday != pDstPlayer->GetLastAddRelationDay())
// 	{
// 		pDstPlayer->SetLastAddRelationDay(now.tm_yday);
// 		KBidFriendRelation::ClearFavorLimit(nDstId);
// 	}

	return TRUE;
}

// 同步关系列表
BOOL KRelationManager_GC::SyncRelationList(INT nPlayerId)
{
	// KGC_CONFIRM_RET_FALSE(nPlayerId);
	// !!!注意，必须先从高类型值开始同步，因为在GameServer端要用包含
	// 普通好友关系的关系类型（如结拜/师徒/夫妻）去覆盖普通好友关系

	BOOL									bRetCode			= FALSE;
	KRelationRuleManager*					pRuleManager		= NULL;
	INT										nType				= 0;
	IKRelationFactory*						pFactory			= NULL;
	CONST KRelationRuleManager::KPOLICY*	pPolicy				= NULL;

	LOG_PROCESS_ERROR(nPlayerId);

	pRuleManager = KRelationRuleManager::Instance();
	for (nType = emKPLAYERRELATION_TYPE_COUNT - 1; nType >= 0; nType--)
	{
		KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory);
		if (!pFactory) 
			continue;
		
		LOG_PROCESS_ERROR(pFactory);
		
		pPolicy = pRuleManager->GetPolicy(nType);
		LOG_PROCESS_ERROR(pPolicy);
		if (pPolicy->bSyncToServerAsMaster)
		{
			SyncRelationList(
				nPlayerId,
				nType,
				pFactory,
				true,
				pPolicy->bSyncToClientAsMaster);
		}
		if (pPolicy->bSyncToServerAsTarget)
		{
			SyncRelationList(
				nPlayerId,
				nType,
				pFactory,
				false,
				pPolicy->bSyncToClientAsTarget);
		}
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;

// 	KRelationRuleManager* pRuleManager = KRelationRuleManager::Instance();
// 	for (INT nType = emKPLAYERRELATION_TYPE_COUNT - 1; nType >= 0; nType--)
// 	{
// 		IKRelationFactory *pFactory = NULL;
// 		if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory))
// 		{
// 			assert(0);
// 			continue;
// 		}
// 		// KGC_CONFIRM_RET_FALSE(pFactory);
// 		CONST KRelationRuleManager::KPOLICY *pPolicy = pRuleManager->GetPolicy(nType);
// 		if (pPolicy->bSyncToServerAsMaster)
// 		{
// 			SyncRelationList(
// 				nPlayerId,
// 				nType,
// 				pFactory,
// 				true,
// 				nConnectID,
// 				pPolicy->bSyncToClientAsMaster);
// 		}
// 		if (pPolicy->bSyncToServerAsTarget)
// 		{
// 			SyncRelationList(
// 				nPlayerId,
// 				nType,
// 				pFactory,
// 				false,
// 				nConnectID,
// 				pPolicy->bSyncToClientAsTarget);
// 		}
// 	}
// 	return TRUE;
}

// 通知好友自己上线
BOOL KRelationManager_GC::NotifyOnline(INT nPlayerId, BOOL bOnline)
{
	BOOL									bRetCode			= FALSE;
	IKRelationFactory*						pFactory			= NULL;
	KRelationRuleManager*					pRuleManager		= NULL;
	SET_PLAYERID							setAlreadyNotify;
	CONST KRelationRuleManager::KPOLICY*	pPolicy	= NULL;

	pRuleManager = KRelationRuleManager::Instance();
	for (INT nType = 0; nType < emKPLAYERRELATION_TYPE_COUNT; nType++)
	{
		if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory))
		{
			// assert(0);
			ASSERT(0);
			continue;
		}
		pPolicy = pRuleManager->GetPolicy(nType);
		if (!pPolicy)
		{
			ASSERT(0);
			continue;
		}
		if (pPolicy->bNotifyOnlineAsMaster)
		{
			NotifyOnline(nType, TRUE, nPlayerId, pFactory, bOnline, setAlreadyNotify);

		}
		if (pPolicy->bNotifyOnlineAsTarget)
		{
			NotifyOnline(nType, FALSE, nPlayerId, pFactory, bOnline, setAlreadyNotify);
		}
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;

	// KGC_CONFIRM_RET_FALSE(nPlayerId);
// 	IKRelationFactory *pFactory = NULL;
// 	KRelationRuleManager* pRuleManager = KRelationRuleManager::Instance();
// 	SET_PLAYERID setAlreadyNotify;
// 	for (INT nType = 0; nType < emKPLAYERRELATION_TYPE_COUNT; nType++)
// 	{
// 		if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory))
// 		{
// 			// assert(0);
// 			assert(0);
// 			continue;
// 		}
// 		CONST KRelationRuleManager::KPOLICY *pPolicy = pRuleManager->GetPolicy(nType);
// 		if (pPolicy->bNotifyOnlineAsMaster)
// 		{
// 			NotifyOnline(nType, TRUE, nPlayerId, pFactory, bOnline, setAlreadyNotify);
// 
// 		}
// 		if (pPolicy->bNotifyOnlineAsTarget)
// 		{
// 			NotifyOnline(nType, FALSE, nPlayerId, pFactory, bOnline, setAlreadyNotify);
// 		}
// 	}
// 	return TRUE;
}

BOOL KRelationManager_GC::NotifyOnline(INT					nRelationType,
									   BOOL					bAsMaster,
									   INT					nPlayerId,
									   IKRelationFactory*	pFactory,
									   BOOL					bOnline,
									   SET_PLAYERID& rsetAlreadNotify)
{
	BOOL						bRetCode			= FALSE;
	IKEnumRelation*				pEnum				= NULL;
	INT							nRelationCount		= 0;
	KRole*						pRole				= NULL;
	IMemBlock*					pMemBlock			= NULL;
	IMemBlock_Resizable*        piReSize            = NULL;
	R2S_RELATION_ONLINE_NOTIFY* pNotify		= NULL;
	INT							nCount				= 0;
	IKRelation*					pRelation			= NULL;
	INT							nRelationID			= 0;


	LOG_PROCESS_ERROR(nPlayerId && pFactory);

	bRetCode = pFactory->GetAllRelations(bAsMaster, nPlayerId, &pEnum);
	PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pEnum);

	nRelationCount = pEnum->GetCount();
	if (nRelationCount == 0 || nRelationCount > emKRELATION_MAX_COUNT)
	{
		bRetCode = FALSE;
		goto EXIT0;
	}

	pEnum->Reset();

	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	// todo 隐身判断

	pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_ONLINE_NOTIFY) + emKRELATION_MAX_COUNT * sizeof(INT));
	LOG_PROCESS_ERROR(pMemBlock);

	pNotify = (R2S_RELATION_ONLINE_NOTIFY*)pMemBlock->GetData();
	memcpy(pNotify->szName, pRole->m_szName, sizeof(pRole->m_szName));

	while (pEnum->Next(&pRelation) && nCount < nRelationCount)
	{
		LOG_PROCESS_ERROR(pRelation);

		nRelationID = pRelation->GetOtherPlayerId(nPlayerId);
		if (nRelationID > 0)
		{ 
			if (!rsetAlreadNotify.insert(nRelationID).second)
				continue;

			pRole = g_pGameCenter->m_RoleManager.GetRole(nRelationID);
			if (pRole->IsOnline())
			{
				pNotify->aryRelationID[nCount++] = nRelationID;
			}
		}
	}

	if (nCount > 0)
	{
		pNotify->wProtocolID = r2s_relation_online_notify;
		pNotify->nType = static_cast<BYTE>(nRelationType);
		pNotify->nRole = bAsMaster ? 1 : 0;
		pNotify->nCount = static_cast<BYTE>(nCount);
		pNotify->nOnline = bOnline ? 1 : 0;	// 登录还是退出

		pMemBlock->QueryInterface(IID_IMemBlock_Resizable, (void**)&piReSize);
		LOG_PROCESS_ERROR(piReSize);

		bRetCode = piReSize->SetSmallerSize(sizeof(R2S_RELATION_ONLINE_NOTIFY) + nCount * sizeof(INT));
		LOG_PROCESS_ERROR(bRetCode);

		g_pGameCenter->m_GameServer.Broadcast(pMemBlock);
	}

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(pMemBlock);
	SAFE_RELEASE(piReSize);
	return bRetCode;

	// KGC_CONFIRM_RET_FALSE(pEnum);
// 	INT nRelationCount = pEnum->GetCount();
// 	if (nRelationCount == 0 || nRelationCount > emKRELATION_MAX_COUNT)
// 		return FALSE;
// 	// KGC_CONFIRM_RET_FALSE(nRelationCount > 0);
// 	pEnum->Reset();
// 	KOnlinePlayer* pOnlinePlayer	= KOnlinePlayerSet::Instance()->FindPlayer(nPlayerId);
// 	// KGC_CONFIRM_RET_FALSE(pOnlinePlayer);
// 	// 如果是隐身的不通知上下线
// 	if (pOnlinePlayer->IsHidden())
// 		return TRUE;
// 	
// 	CHAR szBuf[sizeof(R2S_RELATION_ONLINE_NOTIFY) + sizeof(INT) * emKRELATION_MAX_COUNT];
// 	R2S_RELATION_ONLINE_NOTIFY *pNotify = (R2S_RELATION_ONLINE_NOTIFY*)szBuf;
// 	KRole* pGcPlayerMe = g_pPlayerManager->GetPlayer(nPlayerId);
// 	// KGC_CONFIRM_RET_FALSE(pGcPlayerMe);
// 	strncpy(pNotify->szName, pGcPlayerMe->GetPlayerName(), sizeof(pNotify->szName) - 1);
// 	INT nCount = 0;
// 	IKRelation *pRelation = NULL;
// 	while (pEnum->Next(&pRelation) && nCount < nRelationCount)
// 	{
// 		// 亲密度达到一定数值才通知上下线
// 		if (nRelationType == emKPLAYERRELATION_TYPE_BINDFRIEND)
// 		{
// 			IKBidFriendRelation *pFriend = NULL;
// 			if (!pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend) ||
// 				pFriend->GetFavor() < emKRELATION_MINFAVOR_NOTIFYONLINE)
// 			{
// 				continue;
// 			}
// 		}
// 		KGC_CONFIRM_CONTINUE(pRelation);
// 		INT nRelationID = pRelation->GetOtherPlayerId(nPlayerId);
// 		if (nRelationID > 0)
// 		{ 
// 			if (!rsetAlreadNotify.insert(nRelationID).second)
// 				continue;
// 			KRole* pPlayer = g_pPlayerManager->GetPlayer(nRelationID);
// 			if (pPlayer && pPlayer->GetOnlineServer() > 0)
// 			{
// 				pNotify->aryRelationID[nCount++] = nRelationID;
// 			}
// 		}
// 	}
// 
// 	if (nCount > 0)
// 	{
// 		pNotify->ProtocolType = r2s_relation_online_notify;
// 		pNotify->nType = static_cast<BYTE>(nRelationType);
// 		pNotify->nRole = bAsMaster ? 1 : 0;
// 		pNotify->nCount = static_cast<BYTE>(nCount);
// 		pNotify->nOnline = bOnline ? 1 : 0;	// 登录还是退出
// 		INT nPackLen = sizeof(R2S_RELATION_ONLINE_NOTIFY) + sizeof(INT) * nCount;
// 		PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(nPackLen);
// 		memcpy(sBuf.GetBuf(), pNotify, nPackLen);
// 		m_pProtocolProcessor->SendPacket(sBuf);
// 	}
// 	return TRUE;
}
/*
BOOL KRelationManager_GC::FindPlayerGuidByName(LPCSTR pszName, GUID *pGuid)
{
	// KGC_CONFIRM_RET_FALSE(pszName && pGuid);
	INT nPlayerID = g_pPlayerManager->GetPlayerIDByName(pszName);
	// KGC_CONFIRM_RET_FALSE(nPlayerID > 0);
	
	KRole* pPlayer = g_pPlayerManager->GetPlayer(nPlayerID);
	// KGC_CONFIRM_RET_FALSE(pPlayer);
	*pGuid = *pPlayer->GetGUID();
	return TRUE;
}*/

// BOOL KRelationManager_GC::SendData(LPVOID pData, INT nSize, INT nConnectID)
// {
// 	// KGC_CONFIRM_RET_FALSE(pData && nSize > 0);
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(nSize);
// 	memcpy(sBuf.GetBuf(), pData, nSize);
// 	return m_pProtocolProcessor->SendPacket(sBuf, nConnectID);
// 	return TRUE;
// }

// BOOL KRelationManager_GC::ProcessProtocol(BYTE* pbyData, size_t uDataLen, int nConnIndex)
// {
// 	switch ((BYTE)(*pData))
// 	{
// 	case emKRELATION_S2G_PROTO_ADDFAVOR:
// 		// 增加好感度
// 		Process_AddFavor(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_SEARCHTEACHER:
// 		// 搜索可拜为师父的在线玩家
// 		Process_SearchTeacher(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_SEARCHSTUDENT:
// 		// 搜索可收为弟子的在线玩家
// 		Process_SearchStudent(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_SETTRAININGOPTION:
// 		// 设置允许收徒/拜师选项
// 		Process_SetTrainingOption(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_GETTRAININGOPTION:
// 		// 获取允许收徒/拜师选项
// 		Process_GetTrainingOption(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_REQUESTTEACHER:
// 		// 拜师申请
// 		Process_RequestTeacher(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_REQUESTSTUDENT:
// 		// 收徒申请
// 		Process_RequestStudent(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_TRAININGGRADUATE:
// 		// 出师
// 		Process_TrainingGraduate(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_TRAININGVIEWINFO:
// 		// 查看申请拜师/收徒者的信息
// 		Process_TrainingViewInfo(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_TRAININGPLAYERINFO:
// 		// 指定玩家的信息
// 		Process_TrainingPlayerInfo(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_TRAININGMAKEFRIEND:
// 		// 通过师徒系统成为好友
// 		Process_TrainingMakeFriend(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_TRAININGAPPLY:
// 		// 拜师
// 		Process_TrainingApply(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_ADDBINDCOIN:
// 		// 密友ib积分
// 		Process_AddCloseFriendBindCoin(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_APPLYGAINCOIN:
// 		// 申请领取绑定金币
// 		Process_ApplyGainIbBackCoin(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_GAINIBCOIN:
// 		// 领取绑定金币
// 		Process_GainIbBackCoin(pData, nSize, nConnectID);
// 		break;
// 	case emKRELATION_S2G_PROTO_CANCELGAINCOIN:
// 		Process_CancelGainCoin(pData, nSize, nConnectID);
// 		break;
// 	default:
// 		// assert(0);
// 		assert(0);
// 		return FALSE;
// 	}
//
//	return TRUE;
//}

BOOL KRelationManager_GC::DelRelationByHand(INT nAppId, INT nType, BOOL bRole, LPCSTR pszRelation)
{
// 	INT nDstID = g_pPlayerManager->GetPlayerIDByName(pszRelation);
// 	if (nDstID <= 0)
// 	{
// 		// assert(0);
// 		assert(0);
// 		return FALSE;
// 	}
// 	KRole* pPlayerApp = g_pPlayerManager->GetPlayer(nAppId);
// 	if (!pPlayerApp || strcmp(pPlayerApp->GetPlayerName(), pszRelation) == 0)
// 	{
// 		// assert(0);
// 		assert(0);
// 		return FALSE;
// 	}
// 	INT nMasterID = nAppId;
// 	INT nTargetID = nDstID;
// 	if (bRole == FALSE)
// 		std::swap(nMasterID, nTargetID);
// 	// 判断是否加载完成
// 	if (CheckIsLoadEnd(pPlayerApp, nMasterID, FALSE) == FALSE)
// 		return FALSE;
// 	// 检查关系依赖
// 	if (!KRelationRuleManager::Instance()->CheckDeleteDependency(nType, nMasterID, nTargetID))
// 	{
// 		return FALSE;
// 	}
// 	IKRelationFactory *pFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory) ||
// 		!pFactory->DeleteRelation(nMasterID, nTargetID))
// 	{
// 		// assert(0);
// 		assert(0);
// 		return FALSE;
// 	}
	return TRUE;
};

BOOL KRelationManager_GC::AddRelationByHand(INT nAppId, INT nType, BOOL bRole, LPCSTR pszRelation)
{
// 	INT nDstId = g_pPlayerManager->GetPlayerIDByName(pszRelation);
// 	if (nDstId <= 0 || nAppId == nDstId)
// 	{
// 		// DEBUG
// 		// KGC_Error("[Relation]AddRelationByHand fail! relation:" << pszRelation << ", AppId:" << nAppId << ", DstId:" << nDstId);
// 		assert(0);
// 		return FALSE;
// 	}
// 	KRole* pPlayerApp = g_pPlayerManager->GetPlayer(nAppId);
// 	KRole* pPlayerDst = g_pPlayerManager->GetPlayer(nDstId);
// 	// KGC_CONFIRM_RET_FALSE (pPlayerApp && pPlayerDst);
// 
// 	INT nMasterID = nAppId;
// 	INT nTargetID = nDstId;
// 	if (bRole == 0)
// 		std::swap(nMasterID, nTargetID);
// 
// 	if (!CheckIsLoadEnd(pPlayerApp, nMasterID, FALSE) && !CheckIsLoadEnd(pPlayerDst, nTargetID, FALSE))
// 		return FALSE; // 数据还没加载完
// 
// 	// 检查关系依赖和冲突
// 	KRelationRuleManager* pRuleManager = KRelationRuleManager::Instance();
// 	if (!pRuleManager->CheckCreateDependency(nType, nMasterID, nTargetID) ||
// 		!pRuleManager->CheckCreateRepugnance(nType, nMasterID, nTargetID))
// 	{
// 		return FALSE; // 
// 	}
// 
// 	IKRelationFactory *pFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory) || 
// 		!pFactory->CanCreateRelation(nMasterID, nTargetID) ||
// 		!pFactory->CreateRelation(nMasterID, nTargetID, TRUE))
// 	{
// 		return FALSE;
// 	}
 	return TRUE;
}

// 检查是否可以建立指定的人际关系
BOOL KRelationManager_GC::CanCreateRelation(INT nType, INT nRole, INT nMasterId, INT nTargetId)
{
	BOOL							bRetCode	= FALSE;
	KRole*							pMaster		= NULL;
	KRole*							pTarget		= NULL;
	IKRelationFactory*				pFactory	= NULL;

	LOG_PROCESS_ERROR(nType >= emKPLAYERRELATION_TYPE_TMPFRIEND && nType < emKPLAYERRELATION_TYPE_COUNT);
	LOG_PROCESS_ERROR(nMasterId > 0 && nTargetId > 0 && nMasterId != nTargetId);

	if (0 == nRole)
		std::swap(nMasterId, nTargetId);

	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMasterId);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nTargetId);
	LOG_PROCESS_ERROR(pMaster && pTarget);

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pFactory);

	bRetCode = pFactory->CanCreateRelation(nMasterId, nTargetId);
EXIT0:
	return bRetCode;

// 	KGC_CONFIRM_RET_FALSE(nType >= emKPLAYERRELATION_TYPE_TMPFRIEND && nType < emKPLAYERRELATION_TYPE_COUNT);
// 	KGC_CONFIRM_RET_FALSE(nMasterId > 0 && nTargetId > 0 && nMasterId != nTargetId);
// 
// 	if (0 == nRole)
// 		std::swap(nMasterId, nTargetId);
// 
// 	KRole* pPlayerMaster = g_pPlayerManager->GetPlayer(nMasterId);
// 	KRole* pPlayerTarget = g_pPlayerManager->GetPlayer(nTargetId);
// 	// KGC_CONFIRM_RET_FALSE (pPlayerMaster && pPlayerTarget);
// 
// 	IKRelationFactory *pFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory) || 
// 		!pFactory->CanCreateRelation(nMasterId, nTargetId))
// 	{
// 		return FALSE;
// 	}
// 
// 	return TRUE;
}

// 不做检查直接建立指定的人际关系
BOOL KRelationManager_GC::CreateRelation(INT nType, INT nMasterId, INT nTargetId, BOOL bIsMaster)
{
	BOOL				bRetCode = FALSE;
	INT					nMasterID = nMasterId;
	INT					nTargetID = nTargetId;
	IKRelationFactory*	pFactory = NULL;

	LOG_PROCESS_ERROR(nType >= emKPLAYERRELATION_TYPE_TMPFRIEND && nType < emKPLAYERRELATION_TYPE_COUNT);
	LOG_PROCESS_ERROR(nMasterId > 0 && nTargetId > 0 && nMasterId != nTargetId);

	if (bIsMaster == 0)
		std::swap(nMasterID, nTargetID);

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pFactory);

	bRetCode = pFactory->CreateRelation(nMasterID, nTargetID, TRUE);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

// 检查是否可以删除指定的人际关系
BOOL KRelationManager_GC::CanDelRelation(INT nType, INT nRole, INT nMasterId, LPCSTR pszTargetName)
{
	BOOL	bRetCode	= FALSE;
	KRole*	pRole		= NULL;
	KRole*	pMaster		= NULL;
	INT		nTargetId	= 0;

	LOG_PROCESS_ERROR(nMasterId > 0 && pszTargetName);
	pRole = g_pGameCenter->m_RoleManager.GetRole(pszTargetName);
	LOG_PROCESS_ERROR(pRole);
	nTargetId = pRole->m_dwPlayerID;

	pMaster = g_pGameCenter->m_RoleManager.GetRole(nMasterId);
	LOG_PROCESS_ERROR(pMaster);

	if (nRole == 0)
		std::swap(nMasterId, nTargetId);

	bRetCode = CheckIsLoadEnd(pMaster, nMasterId, FALSE);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = KRelationRuleManager::Instance()->CheckDeleteDependency(nType, nMasterId, nTargetId);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;

// 	KRole* pPlayerApp = g_pPlayerManager->GetPlayer(nMasterId);
// 	if (!pPlayerApp)
// 	{
// 		// assert(0);
// 		assert(0);
// 		return FALSE;
// 	}
// 	if (nRole == 0)
// 		std::swap(nMasterId, nTargetId);
// 	// 判断是否加载完成
// 	if (CheckIsLoadEnd(pPlayerApp, nMasterId, FALSE) == FALSE)
// 		return FALSE;
// 	// 检查关系依赖
// 	if (!KRelationRuleManager::Instance()->CheckDeleteDependency(nType, nMasterId, nTargetId))
// 	{
// 		return FALSE;
// 	}
// 	return TRUE;
}

// 不做检查，直接删除指定的人际关系
BOOL KRelationManager_GC::DelRelation(INT nType, INT nMasterId, INT nTargetId, BOOL bIsMaster)
{
	BOOL				bRetCode = FALSE;
	IKRelationFactory*	pFactory = NULL;

	LOG_PROCESS_ERROR(nMasterId > 0 && nTargetId > 0 && nMasterId != nTargetId);
	if (bIsMaster == FALSE)
		std::swap(nMasterId, nTargetId);

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pFactory);

	bRetCode = pFactory->DeleteRelation(nMasterId, nTargetId);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	return bRetCode;

// 	if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory) ||
// 		!pFactory->DeleteRelation(nMasterId, nTargetId))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	return TRUE;
}

// 密友、师徒、介绍人关系一年到期，删除密友关系，并且发邮件通知双方
BOOL KRelationManager_GC::DelOverTimeRelation(INT nType, INT nMasterId, INT nTargetId, BOOL bIsMaster)
{
	BOOL bRet = FALSE;
// 	do 
// 	{
// 		// 三种不同类型的密友关系，需要分别使用三种不同的信件内容和log内容
// 		LPCSTR pszMailFormat = NULL;
// 		LPCSTR pszLogFormat = NULL;
// 		if (nType == emKPLAYERRELATION_TYPE_BUDDY)
// 		{
// 			pszMailFormat = KD_STR_DELRELATION_TIMEOVER_BUDDY;
// 			pszLogFormat = KD_STR_CLOSEFRIEND_OVERTIME_BUDDY;
// 		}
// 		else if (nType == emKPLAYERRELATION_TYPE_INTRODUCTION)
// 		{
// 			pszMailFormat = KD_STR_DELRELATION_TIMEOVER_INTRODUCTION;
// 			pszLogFormat = KD_STR_CLOSEFRIEND_OVERTIME_INTRODUCTION;
// 		}
// 		else if (nType == emKPLAYERRELATION_TYPE_TRAINED)
// 		{
// 			pszMailFormat = KD_STR_DELRELATION_TIMEOVER_TRAINED;
// 			pszLogFormat = KD_STR_CLOSEFRIEND_OVERTIME_TRAINED;
// 		}
// 		else
// 		{
// 			KGC_CONFIRM_BREAK(FALSE);
// 		}
// 
// 		KGC_CONFIRM_BREAK(DelRelation(nType, nMasterId, nTargetId, bIsMaster));
// 		KRole* pGCAppPlayer = g_pPlayerManager->GetPlayer(nMasterId);
// 		KRole* pGCDstPlayer = g_pPlayerManager->GetPlayer(nTargetId);
// 		KGC_CONFIRM_BREAK(pGCAppPlayer);
// 		KGC_CONFIRM_BREAK(pGCDstPlayer);
// 		CHAR szMsg[256];
// 
// 		::sprintf(szMsg, pszMailFormat, pGCDstPlayer->GetPlayerName());
// 		bRet = KMailManager_GC::Instance()->PostSysMail(nMasterId, KD_STR_DELOVERTIMERELATION_TITLE, szMsg);
// 		KGC_CONFIRM_BREAK(bRet);
// 		::sprintf(szMsg, pszLogFormat, pGCDstPlayer->GetPlayerName());
// 		KLogService::GetInstance()->Goddess_PlayerLog(pGCAppPlayer, emKPLAYERLOG_TYPE_REALTION, szMsg);
// 
// 		::sprintf(szMsg, pszMailFormat, pGCAppPlayer->GetPlayerName());
// 		bRet = KMailManager_GC::Instance()->PostSysMail(nTargetId, KD_STR_DELOVERTIMERELATION_TITLE, szMsg);
// 		KGC_CONFIRM_BREAK(bRet);
// 		::sprintf(szMsg, pszLogFormat, pGCAppPlayer->GetPlayerName());
// 		KLogService::GetInstance()->Goddess_PlayerLog(pGCDstPlayer, emKPLAYERLOG_TYPE_REALTION, szMsg);
// 	} while (0);
	return bRet;
}

BOOL KRelationManager_GC::SetFriendTalkTime(INT nAppId, INT nDstId, DWORD dwTime)
{
	KRole* pAppPlayer = g_pGameCenter->m_RoleManager.GetRole(nAppId);
	KRole* pDstPlayer = g_pGameCenter->m_RoleManager.GetRole(nDstId);
	QCONFIRM_RET_FALSE(pAppPlayer && pDstPlayer);

	IKRelationFactory *pFactory = NULL;
	IKRelation *pRelation = NULL;
	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFactory) ||
		!pFactory->FindRelation(nAppId, nDstId, &pRelation))
	{
		assert(0);
		return FALSE;
	}
	QCONFIRM_RET_FALSE(pRelation);

	IKBidFriendRelation *pFriend = NULL;

	if (!pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend) ||
		!pFriend->SetLastTalkTime(dwTime))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL KRelationManager_GC::AddFriendFavor(INT nAppId, INT nDstId, INT nFavor, INT nMethod, BOOL bByHand)
{
	// KGC_CONFIRM_RET_FALSE(nAppId > 0 && nDstId > 0 && nAppId != nDstId && nFavor > 0);
// 	KRole* pAppPlayer = g_pPlayerManager->GetPlayer(nAppId);
// 	KRole* pDstPlayer = g_pPlayerManager->GetPlayer(nDstId);
// 	// KGC_CONFIRM_RET_FALSE(pAppPlayer && pDstPlayer);
// 
// 	IKRelationFactory *pFactory = NULL;
// 	IKRelation *pRelation = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFactory) ||
// 		!pFactory->FindRelation(nAppId, nDstId, &pRelation))
// 	{
// 		// assert(0);
// 		assert(0);
// 		return FALSE;
// 	}
// 	// KGC_CONFIRM_RET_FALSE(pRelation);
// 	IKBidFriendRelation *pFriend = NULL;
// 	INT nAward = 0;
// 	if (!pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend) ||
// 		!pFriend->AddFavor(nFavor, nMethod, nAward, bByHand))
// 	{
// 		return FALSE;
// 	}
// 	if (nAward > 0)
// 	{
// 		DoFrindFavorAward(pAppPlayer, pDstPlayer, nAppId, nDstId, nAward);
// 		OnFavorLevelUp(nAppId, nDstId, nAward);
// 	}

	return TRUE;
}

BOOL KRelationManager_GC::SyncFriendInfo(INT nAppId, INT nDstId)
{
	BOOL									bRetCode				= FALSE;
	KRole*									pAppRole				= NULL;
	KRole*									pDstRole				= NULL;
	IKRelationFactory*						pFactory				= NULL;
	IKRelation*								pRelation				= NULL;
	IKBidFriendRelation*					pFriend					= NULL;
	IMemBlock*								piMemBlock				= NULL;
	R2S_RELATION_FRIEND_INFO_RESPOND*			pRespond				= NULL;
	KRELATION_PROTO_FRIENDINFO*				pInfo					= NULL;

	LOG_PROCESS_ERROR(nAppId && nDstId);
	pAppRole = g_pGameCenter->m_RoleManager.GetRole(nAppId);
	pDstRole = g_pGameCenter->m_RoleManager.GetRole(nDstId);
	LOG_PROCESS_ERROR(pAppRole && pDstRole);

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFactory);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pFactory);

	bRetCode = pFactory->FindRelation(nAppId, nDstId, &pRelation);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pRelation);

	bRetCode = pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pFriend);

	piMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO));
	LOG_PROCESS_ERROR(piMemBlock);

	pRespond = (R2S_RELATION_FRIEND_INFO_RESPOND*)piMemBlock->GetData();
	pRespond->wProtocolID	= r2s_relation_friend_info_respond;
	pRespond->nCount		= 1;

	pInfo = pRespond->aryInfo;

	if (pAppRole->m_nConnIndex > 0)
	{
		pRespond->nPlayerID = nAppId;
		InitRelationInfo(pAppRole, pDstRole, pFriend->GetFavor(), pFriend->GetLastTalkTime(), pInfo);
		g_pGameCenter->m_GameServer.Send(pAppRole->m_nConnIndex, piMemBlock);
	}

	if (pDstRole->m_nConnIndex > 0)
	{
		pRespond->nPlayerID = nDstId;
		InitRelationInfo(pDstRole, pAppRole, pFriend->GetFavor(), pFriend->GetLastTalkTime(), pInfo);
		g_pGameCenter->m_GameServer.Send(pDstRole->m_nConnIndex, piMemBlock);
	}

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(piMemBlock);
	return bRetCode;

	// KGC_CONFIRM_RET_FALSE(nAppId > 0 && nDstId > 0 && nAppId != nDstId);
// 	KRole* pAppPlayer = g_pPlayerManager->GetPlayer(nAppId);
// 	KRole* pDstPlayer = g_pPlayerManager->GetPlayer(nDstId);
// 	// KGC_CONFIRM_RET_FALSE(pAppPlayer && pDstPlayer);
// 
// 	IKRelationFactory *pFactory = NULL;
// 	IKRelation *pRelation = NULL;
// 	IKBidFriendRelation *pFriend = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFactory) ||
// 		!pFactory->FindRelation(nAppId, nDstId, &pRelation) ||
// 		!pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend))
// 	{
// 		return FALSE;
// 	}
// 
// 	// 同步好友信息
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO));
// 	R2S_RELATION_FRIEND_INFO_RESPOND *pCmd = (R2S_RELATION_FRIEND_INFO_RESPOND*)sBuf.GetBuf();
// 	pCmd->ProtocolType = r2s_relation_friend_info_respond;
// 	pCmd->nCount = 1;
// 	pCmd->nPlayerID = nAppId;
// 	KRELATION_PROTO_FRIENDINFO *pInfo = pCmd->aryInfo;
// 	InitRelationInfo(pDstPlayer, pFriend->GetFavor(), pInfo);
// 	INT nAppSvrID = pAppPlayer->GetOnlineServer();
// 	if (nAppSvrID > 0)
// 		m_pProtocolProcessor->SendPacket(sBuf, nAppSvrID);
// 
// 	INT nDstSvrID = pDstPlayer->GetOnlineServer();
// 	if (nDstSvrID > 0)
// 	{
// 		pCmd->nPlayerID = nDstId;
// 		InitRelationInfo(pAppPlayer, pFriend->GetFavor(), pInfo);
// 		m_pProtocolProcessor->SendPacket(sBuf, nDstSvrID);
// 	}
// 
// 	return TRUE;
}

BOOL KRelationManager_GC::GetMyChannelFriend(INT nPlayerId, VECPLAYERID& vecPlayerId)
{
// 	KRole* pGcPlayer = g_pPlayerManager->GetPlayer(nPlayerId);
// 	// KGC_CONFIRM_RET_FALSE(pGcPlayer);
// 	if (!pGcPlayer->GetRelayLoaded())
// 		return FALSE;
// 
// 	IKRelationFactory *pFriendFactory = NULL;
// 
// 	// 普通好友关系
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFriendFactory))
// 	{
// 		// assert(0);
// 		assert(0);
// 		return FALSE;
// 	}
// 	IKEnumRelation* pEnum = NULL;
// 	
// 	// 以自己为主位的好友关系
// 	if (pFriendFactory->GetAllRelations(TRUE, nPlayerId, &pEnum) && pEnum->GetCount() > 0)
// 		AddFriendIdToVec(nPlayerId, pEnum, vecPlayerId);
	// 以自己为次位的好友关系
//	pEnum = NULL;
//	if (pFriendFactory->GetAllRelations(FALSE, nPlayerId, &pEnum) && pEnum->GetCount() > 0)
//		AddFriendIdToVec(nPlayerId, pEnum, vecPlayerId);

	return TRUE;
}

BOOL KRelationManager_GC::AddFriendIdToVec(INT nSendPlayerId, IKEnumRelation* pEnum, VECPLAYERID& vecPlayerId)
{
// 	IKRelation* pRelation = NULL;
// 	// KGC_CONFIRM_RET_FALSE(pEnum);
// 	while (pEnum->Next(&pRelation))
// 	{
// 		KGC_CONFIRM_CONTINUE(pRelation);
// 		INT nPlayerId = pRelation->GetOtherPlayerId(nSendPlayerId);
// 		KGC_CONFIRM_CONTINUE(nPlayerId > 0);
// 		KRole* pGcPlayer = g_pPlayerManager->GetPlayer(nPlayerId);
// 		KGC_CONFIRM_CONTINUE(pGcPlayer);
// 		INT nSvrId = pGcPlayer->GetOnlineServer();
// 		if (nSvrId > 0)
// 		{
// 			vecPlayerId.push_back(nPlayerId);
// 		}
// 	}
	return TRUE;
}

void KRelationManager_GC::Process_AddFavor(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData);
	// 增加好感度
// 	KRELATION_PROTO_S2G_ADDFAVOR *pMsg = (KRELATION_PROTO_S2G_ADDFAVOR*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_S2G_ADDFAVOR))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	KRole* pPlayerApp = g_pPlayerManager->GetPlayer(pMsg->nPlayerID);
// 	// KGC_CONFIRM_RET_FALSE(pPlayerApp);
// 	INT nDstID = g_pPlayerManager->GetPlayerIDByName(pMsg->szRelation);
// 	// KGC_CONFIRM_RET_FALSE(nDstID > 0);
// 	
// 	// 判断是否加载完成
// 	if (CheckIsLoadEnd(pPlayerApp, pMsg->nPlayerID) == FALSE)
// 		return FALSE;
// 
// 	KRole* pPlayerDst = g_pPlayerManager->GetPlayer(nDstID);
// 	// KGC_CONFIRM_RET_FALSE(pPlayerDst);
// 
// 	// 检查加亲密度日期
// 	time_t tt = KSysService::Time(NULL);
// 	tm now = *KSysService::LocalTime(&tt);
// 	if (now.tm_yday != pPlayerApp->GetLastAddRelationDay())
// 	{
// 		pPlayerApp->SetLastAddRelationDay(now.tm_yday);
// 		KBidFriendRelation::ClearFavorLimit(pMsg->nPlayerID);
// 	}
// 	if (now.tm_yday != pPlayerDst->GetLastAddRelationDay())
// 	{
// 		pPlayerDst->SetLastAddRelationDay(now.tm_yday);
// 		KBidFriendRelation::ClearFavorLimit(nDstID);
// 	}
// 
// 	IKRelationFactory *pFactory = NULL;
// 	IKRelation *pRelation = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFactory) ||
// 		!pFactory->FindRelation(pMsg->nPlayerID, nDstID, &pRelation))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	// KGC_CONFIRM_RET_FALSE(pRelation);
// 	IKBidFriendRelation *pFriend = NULL;
// 	INT nAward = 0;
// 	if (!pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend) ||
// 		!pFriend->AddFavor(pMsg->nFavor, pMsg->nMethod, nAward))
// 	{
// 		return FALSE;
// 	}
// 	if (nAward > 0)
// 	{
// 		DoFrindFavorAward(pPlayerApp, pPlayerDst, pMsg->nPlayerID, nDstID, nAward);
// 		OnFavorLevelUp(pMsg->nPlayerID, nDstID, nAward);
// 	}
// 
// 	// 同步好友信息
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(R2S_RELATION_FRIEND_INFO_RESPOND) + sizeof(KRELATION_PROTO_FRIENDINFO));
// 	R2S_RELATION_FRIEND_INFO_RESPOND *pCmd = (R2S_RELATION_FRIEND_INFO_RESPOND*)sBuf.GetBuf();
// 	pCmd->ProtocolType = r2s_relation_friend_info_respond;
// 	pCmd->nCount = 1;
// 	pCmd->nPlayerID = pMsg->nPlayerID;
// 	KRELATION_PROTO_FRIENDINFO *pInfo = pCmd->aryInfo;
// 	InitRelationInfo(pPlayerDst, pFriend->GetFavor(), pInfo);
// 	m_pProtocolProcessor->SendPacket(sBuf, nConnectID);
// 	
// 	INT nDstSvrID = pPlayerDst->GetOnlineServer();
// 	if (nDstSvrID > 0)
// 	{
// 		pCmd->nPlayerID = nDstID;
// 		InitRelationInfo(pPlayerApp, pFriend->GetFavor(), pInfo);
// 		m_pProtocolProcessor->SendPacket(sBuf, nDstSvrID);
// 	}
}

BOOL KRelationManager_GC::IbBackOverMailSend(INT nSendId, INT nReceiveId, BOOL bBothSend)
{
 	BOOL bRet	= FALSE;
// 	KRole* pGCPlayer	= g_pPlayerManager->GetPlayer(nSendId);
// 	KRole* pGCReceivePlayer = g_pPlayerManager->GetPlayer(nReceiveId);
// 	// KGC_CONFIRM_RET_FALSE(pGCPlayer && pGCReceivePlayer);
// 	CHAR szMsg[256];
// 	sprintf(szMsg,KD_STR_IBBACLMAIL_TIMEOVER, pGCPlayer->GetPlayerName());
// 	bRet = KMailManager_GC::Instance()->PostSysMail(nReceiveId, KD_STR_IBBACKMAIL_TITLE, szMsg);
// 	sprintf(szMsg,KD_STR_IBBACK_TIMEOVER_RECEIVER, pGCPlayer->GetPlayerName());
// 	KLogService::GetInstance()->Goddess_PlayerLog(pGCReceivePlayer, emKPLAYERLOG_TYPE_REALTION, szMsg);
// 	if (bBothSend)
// 	{
// 		sprintf(szMsg,KD_STR_IBBACLMAIL_TIMEOVER, pGCReceivePlayer->GetPlayerName());
// 		bRet = bRet && KMailManager_GC::Instance()->PostSysMail(nSendId, KD_STR_IBBACKMAIL_TITLE, szMsg);
// 
// 		sprintf(szMsg,KD_STR_IBBACK_TIMEOVER_RECEIVER, pGCReceivePlayer->GetPlayerName());
// 		KLogService::GetInstance()->Goddess_PlayerLog(pGCPlayer, emKPLAYERLOG_TYPE_REALTION, szMsg);
// 	}
// 	else
// 	{
// 		sprintf(szMsg,KD_STR_IBBACK_TIMEOVER_SENDER, pGCReceivePlayer->GetPlayerName());
// 		KLogService::GetInstance()->Goddess_PlayerLog(pGCPlayer, emKPLAYERLOG_TYPE_REALTION, szMsg);
// 	}

	return bRet;
}

BOOL KRelationManager_GC::IbBackMailSend(INT nPlayerId)
{
 	BOOL bRet	= FALSE;
// 	KRole* pGCPlayer	= g_pPlayerManager->GetPlayer(nPlayerId);
// 	// KGC_CONFIRM_RET_FALSE(pGCPlayer);
// 	INT	nIbMail	= pGCPlayer->GetIbBackMail();
// 	time_t	tTime	= KSysService::Time(NULL);
// 	tm* pTm	= KSysService::LocalTime(&tTime);
// 	INT nNowWeek = pTm->tm_mon * 4 + pTm->tm_mday / 7 + 1;
// 
// 	if (nNowWeek != nIbMail)
// 	{
// 		pGCPlayer->SetIbBackMail(nNowWeek);
// 		
// 		KMailManager_GC::Instance()->PostSysMail(nPlayerId, KD_STR_IBBACKMAIL_TITLE, KD_STR_IBBACKMAIL_CONTENT);
// 		bRet	= TRUE;
// 	}
	return bRet;
}

void KRelationManager_GC::Process_AddCloseFriendBindCoin(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData);
	// KGC_CONFIRM_RET_FALSE(nSize	== sizeof(KRELATION_PROTO_S2G_ADDBINDCOIN));
	// 增加密友ib积分，返回绑定金币
// 	KRELATION_PROTO_S2G_ADDBINDCOIN* pMsg	= (KRELATION_PROTO_S2G_ADDBINDCOIN*)pData;
// 	INT nSendId	= pMsg->nPlayerId;
// 	KRole* pPlayer	= g_pPlayerManager->GetPlayer(pMsg->nPlayerId);
// 	// KGC_CONFIRM_RET_FALSE(pPlayer);
// 	INT nCoin	= pMsg->nCoin; 
// 	if (nCoin <= 0)
// 		return TRUE;
//  
// 	if (!CheckIsLoadEnd(pPlayer, pMsg->nPlayerId, FALSE))
// 		return FALSE;
// 	IKRelationFactory *pFactory = NULL;
// 	
// 	// 指定密友
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BUDDY, &pFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	IKEnumRelation* pEnum = NULL;
// 	IKRelation* pRelation = NULL; 
// 	KCloseFriendRelation*	pCloseRelation = NULL;
// 	if (pFactory->GetAllRelations(true, pMsg->nPlayerId, &pEnum) && pEnum->GetCount() > 0)
// 	{
// 		while (pEnum->Next(&pRelation))
// 		{
// 			KGC_CONFIRM_CONTINUE(pRelation);
// 			BOOL bRet	= pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BUDDY, (LPVOID *)&pCloseRelation);
// 			
// 			KGC_CONFIRM_CONTINUE(bRet);
// 	
// 			INT nPlayerId = pCloseRelation->GetOtherPlayerId(pMsg->nPlayerId);
// 			KGC_CONFIRM_CONTINUE(nPlayerId > 0);
// 			// 超过一年后，提示密友不在返回ib积分
// 			if (pCloseRelation->CheckOverTime())
// 			{
// 				DelOverTimeRelation(emKPLAYERRELATION_TYPE_BUDDY, nPlayerId, 
// 					pCloseRelation->GetOtherPlayerId(nPlayerId), TRUE);
// 			}
// 			else
// 			{	
// 				BOOL bMaster = FALSE;
// 				if (pCloseRelation->IsMaster(nPlayerId))
// 					bMaster	= TRUE;
// 				pCloseRelation->AddBindCoin(nCoin, bMaster);
// 
// 				if (pCloseRelation->GetBindCoin(bMaster) >= 1)
// 					IbBackMailSend(nPlayerId);
// 			}
// 		}
// 	}
// 	// 介绍人
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_INTRODUCTION, &pFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	pRelation	= NULL;
// 	pEnum		= NULL;
// 	if (pFactory->GetAllRelations(false, pMsg->nPlayerId, &pEnum) && pEnum->GetCount() > 0)
// 	{
// 		while (pEnum->Next(&pRelation))
// 		{
// 			KGC_CONFIRM_CONTINUE(pRelation);
// 			BOOL bRet	= pRelation->QueryRelation(emKPLAYERRELATION_TYPE_INTRODUCTION, (LPVOID *)&pCloseRelation);
// 			KGC_CONFIRM_CONTINUE(bRet);
// 			INT nPlayerId	= pRelation->GetOtherPlayerId(pMsg->nPlayerId);
// 			KGC_CONFIRM_CONTINUE(nPlayerId > 0);
// 			// 超过一年后，提示密友不在返回ib积分
// 			if (pCloseRelation->CheckOverTime())
// 			{
// 				DelOverTimeRelation(emKPLAYERRELATION_TYPE_INTRODUCTION, nPlayerId,
// 					pCloseRelation->GetOtherPlayerId(nPlayerId), FALSE);
// 			}
// 			else
// 			{	
// 				pCloseRelation->AddBindCoin(nCoin, TRUE);
// 				if (pCloseRelation->GetBindCoin(TRUE) >= 1)
// 					IbBackMailSend(nPlayerId);
// 			}
// 		}
// 	}
// 	// 师徒 出师弟子
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	pRelation	= NULL;
// 	pEnum		= NULL;
// 	if (pFactory->GetAllRelations(false, pMsg->nPlayerId, &pEnum) && pEnum->GetCount() > 0)
// 	{
// 		while (pEnum->Next(&pRelation))
// 		{
// 			KGC_CONFIRM_CONTINUE(pRelation);
// 			BOOL bRet	= pRelation->QueryRelation(emKPLAYERRELATION_TYPE_TRAINED, (LPVOID *)&pCloseRelation);
// 			KGC_CONFIRM_CONTINUE(bRet);
// 			INT nPlayerId	= pRelation->GetRole(TRUE);
// 			KGC_CONFIRM_CONTINUE(nPlayerId > 0);
// 			// 超过一年后，提示密友不在返回ib积分
// 			if (pCloseRelation->CheckOverTime())
// 			{
// 				DelOverTimeRelation(emKPLAYERRELATION_TYPE_TRAINED, nPlayerId,
// 					pCloseRelation->GetOtherPlayerId(nPlayerId), FALSE);
// 			}
// 			else
// 			{
// 				pCloseRelation->AddBindCoin(nCoin, TRUE);
// 				if (pCloseRelation->GetBindCoin(TRUE) >= 1)
// 					IbBackMailSend(nPlayerId);
// 			}
// 		}
// 	}
}

BOOL KRelationManager_GC::AddRelationMap(INT nPlayerId, BYTE btType, KCloseFriendRelation* pRelation, INT nCoin)
{
// 	KTPRELATION	sCloseFriend;
// 	
// 	INT nTargetId	= pRelation->GetOtherPlayerId(nPlayerId);
// 	KRole*	pPlayer	= g_pPlayerManager->GetPlayer(nTargetId);
// 	// KGC_CONFIRM_RET_FALSE(pPlayer);
// 	strncpy(sCloseFriend.szTarget,pPlayer->GetPlayerName(), _NAME_LEN - 1);
// 	sCloseFriend.szTarget[_NAME_LEN - 1] = '\0';
// 	sCloseFriend.nPlayerId	= nTargetId;
// 	
// 	sCloseFriend.nBindCoin	= nCoin;
// 	sCloseFriend.btType		= btType;
// 	BOOL bMaster	= pRelation->IsMaster(nPlayerId);
// 	sCloseFriend.bMaster	= bMaster;
// 	// 如果已经在缓存中了，就不再添加
// 	MAPRELATION::iterator it = m_mpCloseFriend.lower_bound(nPlayerId);
// 	MAPRELATION::iterator itEnd	  = m_mpCloseFriend.upper_bound(nPlayerId);
// 	for (; it != itEnd; ++it)
// 	{
// 		KTPRELATION& rRelation = it->second;
// 		if (nTargetId == rRelation.nPlayerId && 
// 			rRelation.btType == btType && 
// 			nCoin == rRelation.nBindCoin &&
// 			bMaster == rRelation.bMaster)
// 			return FALSE;
// 	}
// 	
// 	m_mpCloseFriend.insert(MAPRELATION::value_type(nPlayerId, sCloseFriend));
	return TRUE;
}

BOOL KRelationManager_GC::FeedBackApplyIbCoin(INT nPlayerId, BYTE byFirst)
{
	
// 	INT nLenBuf	= sizeof(KRELATION_PROTO_G2S_RESULTAPPLYIBCOIN);
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(nLenBuf);
// 	KRELATION_PROTO_G2S_RESULTAPPLYIBCOIN* pCmd = (KRELATION_PROTO_G2S_RESULTAPPLYIBCOIN*)sBuf.GetBuf();
// 	pCmd->ProtocolType = emKRELATION_G2S_PROTO_RESULTAPPLYIBCOIN;
// 	pCmd->wCount	= 0;
// 	pCmd->nPlayerId	= nPlayerId;
// 	pCmd->byFirst	= byFirst;
// 	MAPRELATION::iterator it = m_mpCloseFriend.lower_bound(nPlayerId);
// 	if (it != m_mpCloseFriend.end() && it->first == nPlayerId)
// 	{
// 		KTPRELATION& rRelation	= it->second;
// 		INT nCoin	= rRelation.nBindCoin;
// 		if (nCoin > 0)
// 		{
// 			pCmd->wCount	= 1;
// 			pCmd->aCloseRelation[0].nCoin	= nCoin;
// 			strncpy(pCmd->aCloseRelation[0].szTargeter, rRelation.szTarget, _NAME_LEN - 1);
// 			pCmd->aCloseRelation[0].szTargeter[_NAME_LEN - 1] = '\0';
// 		}
// 		else
// 		{
// 			// KGC_CONFIRM_RET_FALSE(FALSE);
// 		}
// 	}
// 	KRole* pPlayer	= g_pPlayerManager->GetPlayer(nPlayerId);
// 	// KGC_CONFIRM_RET_FALSE(pPlayer);
// 	INT nSeverId	= pPlayer->GetOnlineServer();
// 	if (nSeverId <= 0)
// 		return FALSE;
// 	return m_pProtocolProcessor->SendPacket(sBuf, nSeverId);
	return TRUE;
}


void KRelationManager_GC::Process_ApplyGainIbBackCoin(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData);
	// 申请领取ib密友返回的绑定金币
// 	KRELATION_PROTO_S2G_APPLYGAINCOIN* pMsg	= (KRELATION_PROTO_S2G_APPLYGAINCOIN*)pData;
// 	INT nPlayerId	= pMsg->nPlayerId;
// 	KRole* pPlayer	= g_pPlayerManager->GetPlayer(nPlayerId);
// 	// KGC_CONFIRM_RET_FALSE(pPlayer);
// 
// 	if (!CheckIsLoadEnd(pPlayer, nPlayerId, FALSE))
// 		return FALSE;
// 	
// 	IKRelationFactory *pFactory = NULL;
// 	// 指定密友
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BUDDY, &pFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	IKEnumRelation* pEnum = NULL;
// 	IKRelation* pRelation = NULL; 
// 	KCloseFriendRelation*	pCloseRelation = NULL;
// 	INT nCoin	= 0;
// 	if (pFactory->GetAllRelations(true, nPlayerId, &pEnum) && pEnum->GetCount() > 0)
// 	{
// 		while (pEnum->Next(&pRelation))
// 		{
// 			KGC_CONFIRM_CONTINUE(pRelation);
// 			BOOL bRet	= pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BUDDY, (LPVOID *)&pCloseRelation);
// 			if (bRet)
// 			{
// 				BOOL bMaster = pCloseRelation->IsMaster(nPlayerId);
// 				nCoin	= pCloseRelation->GetBindCoin(bMaster);
// 				if (nCoin > 0)
// 				{
// 					AddRelationMap(nPlayerId, emKPLAYERRELATION_TYPE_BUDDY, pCloseRelation, nCoin);
// 				}
// 			}
// 		}
// 	}
// 	// 介绍人
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_INTRODUCTION, &pFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	pRelation	= NULL;
// 	pEnum		= NULL;
// 	if (pFactory->GetAllRelations(true, nPlayerId, &pEnum) && pEnum->GetCount() > 0)
// 	{
// 		while (pEnum->Next(&pRelation))
// 		{
// 			KGC_CONFIRM_CONTINUE(pRelation);
// 			BOOL bRet	= pRelation->QueryRelation(emKPLAYERRELATION_TYPE_INTRODUCTION, (LPVOID *)&pCloseRelation);
// 			if (bRet)
// 			{
// 				nCoin	= pCloseRelation->GetBindCoin(TRUE);
// 				if (nCoin > 0)
// 				{
// 					AddRelationMap(nPlayerId,emKPLAYERRELATION_TYPE_INTRODUCTION, pCloseRelation, nCoin);
// 				}
// 			}
// 		}
// 	}
// 	// 出师师徒
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	pRelation	= NULL;
// 	pEnum		= NULL;
// 	if (pFactory->GetAllRelations(true, nPlayerId, &pEnum) && pEnum->GetCount() > 0)
// 	{
// 		while (pEnum->Next(&pRelation))
// 		{
// 			KGC_CONFIRM_CONTINUE(pRelation);
// 			BOOL bRet	= pRelation->QueryRelation(emKPLAYERRELATION_TYPE_TRAINED, (LPVOID *)&pCloseRelation);
// 			if (bRet)
// 			{
// 				nCoin	= pCloseRelation->GetBindCoin(TRUE);
// 				if (nCoin > 0)
// 				{
// 					AddRelationMap(nPlayerId, emKPLAYERRELATION_TYPE_TRAINED, pCloseRelation, nCoin);
// 				}
// 			}
// 		}
// 	}	
// 	return FeedBackApplyIbCoin(nPlayerId, 1);
}

KCloseFriendRelation* KRelationManager_GC::GetCloseFriendRelation(INT nPlayerId, BYTE btType, INT nPlayerId2)
{
	IKRelationFactory *pFactory = NULL;
	IKEnumRelation* pEnum = NULL;
	IKRelation* pRelation = NULL;
	KCloseFriendRelation* pCloseRelation = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(btType, &pFactory))
// 	{
// 		assert(0);
// 		return NULL;
// 	}
// 	pRelation	= NULL;
// 	pEnum		= NULL;
// 	KRole* pPlayer	= g_pPlayerManager->GetPlayer(nPlayerId);
// 	KGC_CONFIRM_RET_NULL(pPlayer);
// 
// 	pFactory->FindRelation(nPlayerId, nPlayerId2, &pRelation);
// 	// KGC_CONFIRM_RET_FALSE(pRelation);
// 	BOOL bRet	= pRelation->QueryRelation(btType, (LPVOID *)&pCloseRelation);
	return pCloseRelation;
}

void KRelationManager_GC::Process_GainIbBackCoin(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData);
// 	KRELATION_PROTO_S2G_GAINIBCOIN* pMsg	= (KRELATION_PROTO_S2G_GAINIBCOIN*)pData;
// 	
// 	INT nBindCoin	= 0;
// 	MAPRELATION::iterator it	= m_mpCloseFriend.lower_bound(pMsg->nPlayerId);
// 
// 	while (it != m_mpCloseFriend.end())
// 	{
// 		if (it->first != pMsg->nPlayerId)
// 			break;
// 		KTPRELATION& rCloseFriend	= it->second;
// 		if (strcmp(rCloseFriend.szTarget, pMsg->szTarget) == 0)
// 		{
// 			INT nMaster = pMsg->nPlayerId;
// 			INT nTarget = rCloseFriend.nPlayerId;
// 			if (rCloseFriend.bMaster == FALSE)
// 			{
// 				nMaster	= rCloseFriend.nPlayerId;
// 				nTarget = pMsg->nPlayerId;
// 			}
// 			KCloseFriendRelation* pRelaltion = GetCloseFriendRelation(nMaster, rCloseFriend.btType, nTarget);
// 			// KGC_CONFIRM_RET_FALSE(pRelaltion);
// 			BOOL bMaster = pRelaltion->IsMaster(pMsg->nPlayerId);
// 			if (pRelaltion != NULL)
// 			{
// 				nBindCoin	= pRelaltion->GetBindCoin(bMaster);
// 				pRelaltion->GainBindCoin(bMaster);
// 			}
// 			break;
// 		}
// 		it++;
// 	}
// 	
// 	if (it != m_mpCloseFriend.end())
// 		m_mpCloseFriend.erase(it);
// 
// 	// 返回gs，把获得的绑定金币加上
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(KRELATION_PROTO_S2G_ADDBINDCOIN));
// 	KRELATION_PROTO_S2G_ADDBINDCOIN *pCmd = (KRELATION_PROTO_S2G_ADDBINDCOIN*)sBuf.GetBuf();
// 	
// 	pCmd->nCoin	= nBindCoin;
// 	pCmd->nPlayerId	= pMsg->nPlayerId;
// 	pCmd->ProtocolType	= emKRELATION_G2S_PROTO_RESULTGAINIBCOIN;
// 	strncpy(pCmd->szCloseFriend, pMsg->szTarget, _NAME_LEN - 1);
// 	pCmd->szCloseFriend[_NAME_LEN - 1] = '\0';
// 	m_pProtocolProcessor->SendPacket(sBuf);
// 
// 	// 对下一个操作
// 	return  FeedBackApplyIbCoin(pMsg->nPlayerId, 0);
}


void KRelationManager_GC::Process_CancelGainCoin(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
// 	KRELATION_PROTO_S2G_APPLYGAINCOIN* pMsg	= (KRELATION_PROTO_S2G_APPLYGAINCOIN*)pbyData;
// 	INT nPlayerId	= pMsg->nPlayerId;
// 	MAPRELATION::iterator itBegin = m_mpCloseFriend.lower_bound(nPlayerId);
// 	MAPRELATION::iterator itEnd	  = m_mpCloseFriend.upper_bound(nPlayerId);
// 	for (MAPRELATION::iterator it = itBegin; it != itEnd;)
// 	{
// 		MAPRELATION::iterator itCur	= it++;
// 		if (itCur->first == nPlayerId)
// 			m_mpCloseFriend.erase(itCur);
// 	}
}

void KRelationManager_GC::Process_SearchTeacher(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
// 	// KGC_CONFIRM_RET_FALSE(pData && nSize > 0);
// 	KRELATION_PROTO_S2G_SEARCHPLAYER* pMsg = (KRELATION_PROTO_S2G_SEARCHPLAYER*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_S2G_SEARCHPLAYER) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	KRole* pGCPlayer = g_pPlayerManager->GetPlayer(pMsg->nPlayerID);
// 	if (!pGCPlayer || pGCPlayer->GetOnlineServer() <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	// 判断是否加载完成
// 	if (CheckIsLoadEnd(pGCPlayer, pMsg->nPlayerID) == FALSE)
// 		return FALSE;
// 
// 	IKRelationFactory* pFactory = NULL;
// 	IKTrainingFactory* pTrainingFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINING, &pFactory) ||
// 		!pFactory->QueryFactory(emKPLAERRELATION_FACTORY_SUBTYPE_TRAINING, (LPVOID *)&pTrainingFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	
// 	// 检查弟子资格
// 	if (!pTrainingFactory->CheckStudentQualification(pMsg->nPlayerID))
// 		return FALSE;
// 	return SearchTeacherList(pTrainingFactory, pGCPlayer, pMsg->nPlayerID);
}

void KRelationManager_GC::Process_SearchStudent(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData && nSize > 0);
// 	KRELATION_PROTO_S2G_SEARCHPLAYER* pMsg = (KRELATION_PROTO_S2G_SEARCHPLAYER*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_S2G_SEARCHPLAYER) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	KRole* pGCPlayer = g_pPlayerManager->GetPlayer(pMsg->nPlayerID);
// 	if (!pGCPlayer || pGCPlayer->GetOnlineServer() <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	// 判断是否加载完成
// 	if (CheckIsLoadEnd(pGCPlayer, pMsg->nPlayerID) == FALSE)
// 		return FALSE;
// 
// 	IKRelationFactory* pFactory = NULL;
// 	IKTrainingFactory* pTrainingFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINING, &pFactory) ||
// 		!pFactory->QueryFactory(emKPLAERRELATION_FACTORY_SUBTYPE_TRAINING, (LPVOID *)&pTrainingFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	
// 	// 检查师父资格
// 	if (!pTrainingFactory->CheckTeacherQualification(pMsg->nPlayerID))
// 		return FALSE;
// 	
// 	return SearchStudentList(pTrainingFactory, pGCPlayer, pMsg->nPlayerID);
}

void KRelationManager_GC::Process_SetTrainingOption(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData && nSize > 0);
// 	KRELATION_PROTO_S2G_SETTRAININGOPTION* pMsg = (KRELATION_PROTO_S2G_SETTRAININGOPTION*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_S2G_SETTRAININGOPTION) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	INT nPlayerSvrId = 0;
// 	KRole* pGCPlayer = g_pPlayerManager->GetPlayer(pMsg->nPlayerID);
// 	// KGC_CONFIRM_RET_FALSE(pGCPlayer);
// 	BOOL bIsStudent = (pMsg->byTeacherOrStudent != 0 ? TRUE : FALSE);
// 
// 	// 拜师条件不符合
// 	PACKET_BUF sNotify = m_pProtocolProcessor->GetPacketBuf(sizeof(KRELATION_PROTO_G2S_TRAININGERROR));
// 	KRELATION_PROTO_G2S_TRAININGERROR* pNotify = (KRELATION_PROTO_G2S_TRAININGERROR*)sNotify.GetBuf();
// 	pNotify->ProtocolType = emKRELATION_G2S_PROTO_TRAININGERROR;
// 	pNotify->nPlayerID	  = pMsg->nPlayerID;
// 	pNotify->byError	  = 0;
// 	INT nMaxLevel = g_cTrainingSetting.GetTeacherMinLevel();
// 	if (bIsStudent == TRUE)
// 		nMaxLevel = g_cTrainingSetting.GetStudentMinLevel();
// 	if (bIsStudent && pGCPlayer->GetLevel() >= g_cTrainingSetting.GetStudentMaxLevel())
// 	{
// 		pNotify->byError = emKRELATION_TRAINING_ERR21;
// 		nPlayerSvrId = pGCPlayer->GetOnlineServer(); 
// 		if (nPlayerSvrId <= 0)
// 			return FALSE;
// 	}
// 	if (pGCPlayer->GetLevel() < nMaxLevel)
// 	{
// 		nPlayerSvrId = pGCPlayer->GetOnlineServer(); 
// 		if (nPlayerSvrId <= 0)
// 			return FALSE;
// 		if (bIsStudent == TRUE)
// 			pNotify->byError = emKRELATION_TRAINING_ERR20;
// 		else
// 			pNotify->byError = emKRELATION_TRAINING_ERR19;
// 	}
// 	if (pNotify->byError != 0 && nPlayerSvrId > 0)
// 	{
// 		m_pProtocolProcessor->SendPacket(sNotify, nPlayerSvrId);
// 		return FALSE;
// 	}
// 	INT nValue = pGCPlayer->GetTrainingOption();
// 	INT nMask = pMsg->byTeacherOrStudent != 0 ? 0x1 : 0x2;
// 	if (pMsg->byOption != 0)
// 		nValue |= nMask;
// 	else
// 		nValue &= ~nMask;
// 	pGCPlayer->SetTrainingOption(nValue);
}

void KRelationManager_GC::Process_GetTrainingOption(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData);
// 	KRELATION_PROTO_S2G_PLAYERID* pMsg = (KRELATION_PROTO_S2G_PLAYERID*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_S2G_PLAYERID) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	KRole* pGCPlayer = g_pPlayerManager->GetPlayer(pMsg->nPlayerID);
// 	if (!pGCPlayer)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(KRELATION_PROTO_G2S_TRAININGOPTION));
// 	KRELATION_PROTO_G2S_TRAININGOPTION* pOption = (KRELATION_PROTO_G2S_TRAININGOPTION*)sBuf.GetBuf();
// 	pOption->ProtocolType = emKRELATION_G2S_PROTO_TRAININGOPTION;
// 	pOption->nOption	  = pGCPlayer->GetTrainingOption();
// 	pOption->nPlayerID	  = pMsg->nPlayerID;
// 	return m_pProtocolProcessor->SendPacket(sBuf);
}

// 检查弟子和师父资格
BOOL KRelationManager_GC::CheckTeacherAndStudent(KRole* pTeacher,
												 KRole* pStudent,
												 INT		 nTeacherID,
												 INT		 nStudentID,
												 BOOL		 bIsTeacherActive)
{
	// KGC_CONFIRM_RET_FALSE(pTeacher && pStudent && nTeacherID > 0 && nStudentID > 0);
// 	INT nTeacherSvrID = pTeacher->GetOnlineServer();
// 	INT nStudentSvrID = pStudent->GetOnlineServer();
// 	
// 	
// 	IKRelationFactory* pRelationFactory = NULL;
// 	IKTrainingFactory* pTrainingFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINING, &pRelationFactory) ||
// 		!pRelationFactory->QueryFactory(emKPLAERRELATION_FACTORY_SUBTYPE_TRAINING, (LPVOID *)&pTrainingFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	// 检查拜师条件
// 	INT nTeacherErr = emKRELATION_TRAINING_ERR_NONE;
// 	INT nStudentErr = emKRELATION_TRAINING_ERR_NONE;
// 	if (pTrainingFactory->CheckTeacherQualification(nTeacherID, &nTeacherErr, &nStudentErr) &&
// 		pTrainingFactory->CheckStudentQualification(nStudentID, &nTeacherErr, &nStudentErr))
// 	{
// 		return TRUE;
// 	}
// 
// 	// 拜师条件不符合
// 	PACKET_BUF sNotify = m_pProtocolProcessor->GetPacketBuf(sizeof(KRELATION_PROTO_G2S_TRAININGERROR));
// 	KRELATION_PROTO_G2S_TRAININGERROR* pNotify = (KRELATION_PROTO_G2S_TRAININGERROR*)sNotify.GetBuf();
// 	pNotify->ProtocolType = emKRELATION_G2S_PROTO_TRAININGERROR;
// 	if (nTeacherErr != emKRELATION_TRAINING_ERR_NONE &&
// 		nTeacherSvrID > 0 &&
// 		bIsTeacherActive)
// 	{
// 		pNotify->nPlayerID = nTeacherID;
// 		pNotify->byError = nTeacherErr;
// 		m_pProtocolProcessor->SendPacket(sNotify, nTeacherSvrID);
// 	}
// 	if (nStudentErr != emKRELATION_TRAINING_ERR_NONE &&
// 		nStudentSvrID > 0 &&
// 		!bIsTeacherActive)
// 	{
// 		pNotify->nPlayerID = nStudentID;
// 		pNotify->byError = nStudentErr;
// 		m_pProtocolProcessor->SendPacket(sNotify, nStudentSvrID);
// 	}
	return FALSE;
}

void KRelationManager_GC::Process_RequestTeacher(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData && nSize > 0);
// 	KRELATION_PROTO_S2G_TRAININGREQUEST* pMsg = (KRELATION_PROTO_S2G_TRAININGREQUEST*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_S2G_TRAININGREQUEST) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	pMsg->szTarget[_NAME_LEN - 1] = 0;
// 	INT nTeacherID = g_pPlayerManager->GetPlayerIDByName(pMsg->szTarget);
// 	// KGC_CONFIRM_RET_FALSE(nTeacherID > 0);
// 	
// 	KRole* pStudent = g_pPlayerManager->GetPlayer(pMsg->nPlayerID);
// 	KRole* pTeacher = g_pPlayerManager->GetPlayer(nTeacherID);
// 	// KGC_CONFIRM_RET_FALSE(pStudent && pTeacher);
// 	INT nTeacherSvrID = pTeacher->GetOnlineServer();
// 	if (nTeacherSvrID <= 0)
// 		return FALSE;
// 
// 	// 判断是否加载完成
// 	if (CheckIsLoadEnd(pTeacher, nTeacherID) == FALSE)
// 		return FALSE;
// 	// 判断是否加载完成
// 	if (CheckIsLoadEnd(pStudent, pMsg->nPlayerID) == FALSE)
// 		return FALSE;
// 
// 	// 检查拜师条件
// 	if (!CheckTeacherAndStudent(pTeacher, pStudent, nTeacherID, pMsg->nPlayerID, FALSE))
// 		return FALSE;
// 
// 	// 在学生对象上记录拜师对象的ID，防止师父强行收徒
// 	KOnlinePlayer* pOLStudent = KOnlinePlayerSet::Instance()->FindPlayer(pMsg->nPlayerID);
// 	// KGC_CONFIRM_RET_FALSE(pStudent);
// 	
// 	pOLStudent->SetApplyingTeacher(nTeacherID);
// 	// 通知目标玩家
// 
// 	g_ChatChannelManager.PrivateMsg(pMsg->nPlayerID, nTeacherID, KD_STR_TRAINING_APPLYINGTEACHER);
// 
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(KRELATION_PROTO_G2S_TRAININGAPPLY));
// 	KRELATION_PROTO_G2S_TRAININGAPPLY* pNotify = (KRELATION_PROTO_G2S_TRAININGAPPLY*)sBuf.GetBuf();
// 	pNotify->ProtocolType = emKRELATION_G2S_PROTO_APPLYINGTEACHER;
// 	pNotify->nPlayerID = nTeacherID;
// 	strncpy(pNotify->szApplicant, pStudent->GetPlayerName(), sizeof(pNotify->szApplicant) - 1);
// 	pNotify->szApplicant[sizeof(pNotify->szApplicant) - 1] = 0;
// 	return m_pProtocolProcessor->SendPacket(sBuf, nTeacherSvrID);
}

void KRelationManager_GC::Process_RequestStudent(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData && nSize > 0);
// 	KRELATION_PROTO_S2G_TRAININGREQUEST* pMsg = (KRELATION_PROTO_S2G_TRAININGREQUEST*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_S2G_TRAININGREQUEST) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	pMsg->szTarget[_NAME_LEN - 1] = 0;
// 	INT nStudentID = g_pPlayerManager->GetPlayerIDByName(pMsg->szTarget);
// 	// KGC_CONFIRM_RET_FALSE(nStudentID > 0);
// 	
// 	KRole* pStudent = g_pPlayerManager->GetPlayer(nStudentID);
// 	KRole* pTeacher = g_pPlayerManager->GetPlayer(pMsg->nPlayerID);
// 	// KGC_CONFIRM_RET_FALSE(pStudent && pTeacher);
// 	INT nStudentSvrID = pStudent->GetOnlineServer();
// 	if (nStudentSvrID <= 0)
// 		return FALSE;
// 
// 	// 判断是否加载完成
// 	if (CheckIsLoadEnd(pTeacher, pMsg->nPlayerID) == FALSE)
// 		return FALSE;
// 	// 判断是否加载完成
// 	if (CheckIsLoadEnd(pStudent,nStudentID) == FALSE)
// 		return FALSE;
// 
// 	// 检查拜师条件
// 	if (!CheckTeacherAndStudent(pTeacher, pStudent, pMsg->nPlayerID, nStudentID, TRUE))
// 		return FALSE;
// 	// 在师父对象上记录收徒对象的ID，防止弟子强行拜师
// 	KOnlinePlayer* pOLTeacher = KOnlinePlayerSet::Instance()->FindPlayer(pMsg->nPlayerID);
// 	// KGC_CONFIRM_RET_FALSE(pTeacher);
// 	
// 	pOLTeacher->SetApplyingStudent(nStudentID);
// 	// 通知目标玩家
// 	g_ChatChannelManager.PrivateMsg(pMsg->nPlayerID, nStudentID, KD_STR_TRAINING_APPLYINGSTUDENT);
// 
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(KRELATION_PROTO_G2S_TRAININGAPPLY));
// 	KRELATION_PROTO_G2S_TRAININGAPPLY* pNotify = (KRELATION_PROTO_G2S_TRAININGAPPLY*)sBuf.GetBuf();
// 	pNotify->ProtocolType = emKRELATION_G2S_PROTO_APPLYINGSTUDENT;
// 	pNotify->nPlayerID = nStudentID;
// 	strncpy(pNotify->szApplicant, pTeacher->GetPlayerName(), sizeof(pNotify->szApplicant) - 1);
// 	pNotify->szApplicant[sizeof(pNotify->szApplicant) - 1] = 0;
// 	return m_pProtocolProcessor->SendPacket(sBuf, nStudentSvrID);
}

void KRelationManager_GC::Process_TrainingGraduate(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData);
// 	KRELATION_PROTO_S2G_TRAININGGRADUATE* pMsg = (KRELATION_PROTO_S2G_TRAININGGRADUATE*)pData;
// 	// KGC_CONFIRM_RET_FALSE (nSize == sizeof(KRELATION_PROTO_S2G_TRAININGGRADUATE));
// 	
// 	pMsg->szStudent[_NAME_LEN - 1] = 0;
// 	pMsg->szTeacher[_NAME_LEN - 1] = 0;
// 	INT nStudentID = g_pPlayerManager->GetPlayerIDByName(pMsg->szStudent);
// 	INT nTeacherID = g_pPlayerManager->GetPlayerIDByName(pMsg->szTeacher);
// 	// KGC_CONFIRM_RET_FALSE(nTeacherID > 0 && nStudentID > 0);
// 	
// 	KRole* pStudent = g_pPlayerManager->GetPlayer(nStudentID);
// 	KRole* pTeacher = g_pPlayerManager->GetPlayer(nTeacherID);
// 	// KGC_CONFIRM_RET_FALSE (pStudent && pTeacher);
// 	
// 	IKRelationFactory* pFactory = NULL;
// 	// 删除未出师师徒关系
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINING, &pFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	if (!pFactory->FindRelation(nTeacherID, nStudentID))
// 		return FALSE;
// 	if (!pFactory->DeleteRelation(nTeacherID, nStudentID))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	// 添加已出师师徒关系
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pFactory) ||
// 		!pFactory->CreateRelation(nTeacherID, nStudentID, FALSE))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	// 提示信息
// 
// 	char szMsg[1024];
// 	INT nStudentSvrID = pStudent->GetOnlineServer();
// 	
// 	if (nStudentSvrID > 0)
// 	{
// 		g_ChatChannelManager.PostSystemMessage(nStudentID, KD_STR_SYSTEM,
// 			KD_STR_TRAINING_GRADUATE_TOSTUDENT, nStudentSvrID);
// 		// 密友提示
// 		sprintf(szMsg, KD_STR_TRAINING_BUDDY1, pTeacher->GetPlayerName());
// 
// 		g_ChatChannelManager.PostSystemMessage(nStudentID, KD_STR_SYSTEM, szMsg, nStudentSvrID);
// 	}
// 	INT nTeacherSvrID = pTeacher->GetOnlineServer();
// 	if (nTeacherSvrID > 0)
// 	{
// 		CHAR szMsg[MAX_PATH];
// 		sprintf(szMsg, KD_STR_TRAINING_GRADUATE_TOTEACHER, pStudent->GetPlayerName());
// 		g_ChatChannelManager.PostSystemMessage(
// 			nTeacherID,
// 			KD_STR_SYSTEM,
// 			szMsg,
// 			nTeacherSvrID);
// 		// 密友提示
// 		sprintf(szMsg, KD_STR_TRAINING_BUDDY2, pStudent->GetPlayerName());
// 		g_ChatChannelManager.PostSystemMessage(nTeacherID, KD_STR_SYSTEM, szMsg, nTeacherSvrID);
// 	}
}

void KRelationManager_GC::Process_TrainingViewInfo(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData);
// 	KRELATION_PROTO_S2G_TRAININGVIEWINFO* pMsg = (KRELATION_PROTO_S2G_TRAININGVIEWINFO*)pData;
// 	if (nSize != sizeof(KRELATION_PROTO_S2G_TRAININGVIEWINFO) ||
// 		pMsg->nPlayerID <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	pMsg->szApplicant[sizeof(pMsg->szApplicant) - 1] = 0;
// 	INT nApplicantID = g_pPlayerManager->GetPlayerIDByName(pMsg->szApplicant);
// 	// KGC_CONFIRM_RET_FALSE(nApplicantID > 0);
// 	
// 	KRole* pGCPlayerApplicant = g_pPlayerManager->GetPlayer(nApplicantID);
// 	// KGC_CONFIRM_RET_FALSE(pGCPlayerApplicant);
// 	
// 	INT nApplicantSvrID = pGCPlayerApplicant->GetOnlineServer();
// 	if (nApplicantSvrID <= 0)
// 	{
// 		// TODO: 要查看的对象玩家不在线
// 		return FALSE;
// 	}
// 	KOnlinePlayer* pApplicant = KOnlinePlayerSet::Instance()->FindPlayer(nApplicantID);
// 	// KGC_CONFIRM_RET_FALSE(pApplicant);
// 	
// 	/* 申请多个人
// 	if (pApplicant->GetApplyingTeacher() != pMsg->nPlayerID &&
// 		pApplicant->GetApplyingStudent() != pMsg->nPlayerID)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	*/
// 
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(KRELATION_PROTO_G2S_TRAININGVIEWINFO));
// 	KRELATION_PROTO_G2S_TRAININGVIEWINFO* pCmd = (KRELATION_PROTO_G2S_TRAININGVIEWINFO*)sBuf.GetBuf();
// 	pCmd->ProtocolType = emKRELATION_G2S_PROTO_TRAININGVIEWINFO;
// 	pCmd->nTargetID = nApplicantID;
// 	pCmd->nViewerID = pMsg->nPlayerID;
// 	m_pProtocolProcessor->SendPacket(sBuf, nApplicantSvrID);
}

void KRelationManager_GC::Process_TrainingPlayerInfo(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData);
// 	KRELATION_PROTO_S2G_PLAYERINFO* pMsg = (KRELATION_PROTO_S2G_PLAYERINFO*)pData;
// 	if (nSize != pMsg->Length || pMsg->nViewerID <= 0)
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	KRole* pPlayerViewer = g_pPlayerManager->GetPlayer(pMsg->nViewerID);
// 	// KGC_CONFIRM_RET_FALSE(pPlayerViewer);
// 	
// 	INT nServerID = pPlayerViewer->GetOnlineServer();
// 	if (nServerID > 0)
// 	{
// 		CHAR szBuff[sizeof(KRELATION_PROTO_G2S_PLAYERINFO) + sizeof(KVIEWITEMINFO) * (emEQUIPPOS_NUM + emPARTNER_NUM)];
// 		KRELATION_PROTO_G2S_PLAYERINFO* pInfo = (KRELATION_PROTO_G2S_PLAYERINFO*)szBuff;
// 		memcpy(pInfo, pMsg, nSize);
// 		pInfo->ProtocolType = emKRELATION_G2S_PROTO_TRAININGPLAYERINFO;
// 		pInfo->nViewerID = pMsg->nViewerID;
// 		pInfo->Length = pMsg->Length;		// 这个与GS与客户端的协议不同，不需要减去协议头的1个长度
// 
// 		PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(pInfo->Length, 0);
// 		memcpy(sBuf.GetBuf(), szBuff, pInfo->Length);
// 		m_pProtocolProcessor->SendPacket(sBuf, nServerID);
// 	}
}

void KRelationManager_GC::Process_TrainingMakeFriend(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	// KGC_CONFIRM_RET_FALSE(pData);
// 	do
// 	{
// 		KRELATION_PROTO_S2G_TRAININGMAKEFRIEND* pMsg = (KRELATION_PROTO_S2G_TRAININGMAKEFRIEND*)pData;
// 		if (nSize != sizeof(KRELATION_PROTO_S2G_TRAININGMAKEFRIEND) ||
// 			pMsg->nPlayerID <= 0)
// 		{
// 			assert(0);
// 			break;
// 		}
// 		pMsg->szRelation[sizeof(pMsg->szRelation) - 1] = '\0';
// 		INT nRelation = g_pPlayerManager->GetPlayerIDByName(pMsg->szRelation);
// 		// KGC_CONFIRM_RET_FALSE(nRelation > 0);
// 		
// 		KRole* pPlayer1 = g_pPlayerManager->GetPlayer(pMsg->nPlayerID);
// 		KRole* pPlayer2 = g_pPlayerManager->GetPlayer(nRelation);
// 		KOnlinePlayer* pOnlineApplicant = KOnlinePlayerSet::Instance()->FindPlayer(nRelation);
// 		if (!pPlayer1 || !pPlayer2 || !pOnlineApplicant)
// 		{
// 			assert(0);
// 			break;
// 		}
// 		
// 		IKRelationFactory* pRelationFactory = NULL;
// 		if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pRelationFactory))
// 		{
// 			assert(0);
// 			break;
// 		}
// 		INT nPlayerId1 = pMsg->nPlayerID;
// 		INT nPlayerId2 = nRelation;
// 		INT nServerID1 = pPlayer1->GetOnlineServer();
// 		INT nServerID2 = pPlayer2->GetOnlineServer();
// 		if (!pRelationFactory->FindRelation(nPlayerId1, nPlayerId2))
// 		{
// 			if (!pRelationFactory->CreateRelation(nPlayerId1, nPlayerId2))
// 			{
// 				assert(0);
// 				break;
// 			}
// 			CHAR szMsg[1024];
// 			if (nServerID1 > 0)
// 			{
// 				sprintf(szMsg, KD_STR_MAKEFRIEND, pPlayer2->GetPlayerName());
// 
// 				g_ChatChannelManager.PostSystemMessage(
// 					pMsg->nPlayerID,
// 					KD_STR_SYSTEM,
// 					szMsg,
// 					nServerID1);
// 			}
// 			if (nServerID2 > 0)
// 			{
// 				sprintf(szMsg, KD_STR_MAKEFRIEND, pPlayer1->GetPlayerName());
// 				g_ChatChannelManager.PostSystemMessage(
// 					nRelation,
// 					KD_STR_SYSTEM,
// 					szMsg,
// 					nServerID2);
// 			}
// 		}
// 		else if (nServerID1 > 0)
// 		{
// 			CHAR szMsg[1024];
// 			sprintf(szMsg, KD_STR_FIRENDALREADY, pPlayer2->GetPlayerName());
// 			g_ChatChannelManager.PostSystemMessage(
// 				pMsg->nPlayerID,
// 				KD_STR_SYSTEM,
// 				szMsg,
// 				nServerID1);
// 		}
// 	}
// 	while (0);
}

void KRelationManager_GC::Process_TrainingApply(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bRet = FALSE;
// 	do
// 	{
// 		KRELATION_PROTO_S2G_TRAININGAPPLY* pMsg = (KRELATION_PROTO_S2G_TRAININGAPPLY*)pData;
// 		if (nSize != sizeof(KRELATION_PROTO_S2G_TRAININGAPPLY) ||
// 			pMsg->nPlayerID <= 0)
// 		{
// 			assert(0);
// 			break;
// 		}
// 		pMsg->szTeacher[sizeof(pMsg->szTeacher) - 1] = '\0';
// 		INT nTeacherID = g_pPlayerManager->GetPlayerIDByName(pMsg->szTeacher);
// 		// KGC_CONFIRM_RET_FALSE(nTeacherID > 0);
// 		
// 		KRole* pTeacher = g_pPlayerManager->GetPlayer(nTeacherID);
// 		KRole* pStudent = g_pPlayerManager->GetPlayer(pMsg->nPlayerID);
// 		KOnlinePlayer* pOnlineStudent = KOnlinePlayerSet::Instance()->FindPlayer(pMsg->nPlayerID);
// 		if (!pTeacher || !pStudent || !pOnlineStudent)
// 		{
// 			assert(0);
// 			break;
// 		}
// 
// 		INT nTeacherSvrID = pTeacher->GetOnlineServer();
// 		if (nTeacherSvrID <= 0)
// 			break;
// 		if (!CheckTeacherAndStudent(pTeacher, pStudent, nTeacherID, pMsg->nPlayerID, FALSE))
// 			break;
// 		
// 		pOnlineStudent->SetApplyingTeacher(nTeacherID);
// 		PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(KRELATION_PROTO_G2S_TRAININGAPPLYFORTEACHER));
// 		KRELATION_PROTO_G2S_TRAININGAPPLYFORTEACHER* pRes = (KRELATION_PROTO_G2S_TRAININGAPPLYFORTEACHER*)sBuf.GetBuf();
// 		pRes->cProtocol = emKRELATION_G2S_PROTO_TRAININGAPPLY;
// 		pRes->nPlayerID = nTeacherID;
// 		strncpy(pRes->szStudent, pStudent->GetPlayerName(), sizeof(pRes->szStudent) - 1);
// 		pRes->szStudent[sizeof(pRes->szStudent) - 1] = '\0';
// 		m_pProtocolProcessor->SendPacket(sBuf, nTeacherSvrID);
// 		bRet = TRUE;
// 
// 	}
// 	while (0);
}

BOOL KRelationManager_GC::PreSearchPlayer(IKTrainingFactory* pFactory, BOOL bStudent)
{
	INT nCount		= 0;
	INT nPlayerId	= 0;
	INT nFirst = 0;
	INT nLastId	= 0;
// 	std::vector<KTPPLAYERSEX> vecTemp;
// 	if (bStudent == TRUE)
// 	{
// 		nFirst = m_nLastStudentId;
// 	}
// 	else
// 	{
// 		nFirst = m_nLastTeacherId;
// 	}
// 	KTPPLAYERSEX sPlayerSex;
// 	IKEnumPlayerID* pEnum = KOnlinePlayerSet::Instance()->EnumPlayer(nFirst);
// 
// 	while (pEnum->Next(nPlayerId) && nCount < emKRELATION_MAX_COUNT)
// 	{		
// 		KRole* pGCPlayer = g_pPlayerManager->GetPlayer(nPlayerId);
// 		if (!pGCPlayer || pGCPlayer->GetOnlineServer() <= 0)
// 			continue;
// 		INT nValue	= pGCPlayer->GetTrainingOption();
// 		if ((nValue & 0x2) && bStudent == FALSE)
// 		{
// 			if (!pFactory->CheckTeacherQualification(nPlayerId))
// 				continue;
// 			sPlayerSex.bySex = pGCPlayer->GetSex();
// 			sPlayerSex.nPlayerId = nPlayerId;
// 		}
// 		else if ((nValue & 0x1) && bStudent == TRUE)
// 		{
// 			if (!pFactory->CheckStudentQualification(nPlayerId))
// 				continue;
// 			sPlayerSex.bySex = pGCPlayer->GetSex();
// 			sPlayerSex.nPlayerId = nPlayerId;
// 		}
// 		else
// 			continue;
// 		nCount++;
// 		vecTemp.push_back(sPlayerSex);
// 		nLastId = nPlayerId;
// 	}
// 	if (bStudent)
// 	{
// 		if (nCount < emKRELATION_SEARCHRES_MAXCOUNT)
// 			m_nLastStudentId = 0;
// 		else
// 			m_nLastStudentId = nLastId;
// 		m_vecStudentId.clear();
// 		if (vecTemp.size() > 0)
// 			m_vecStudentId.assign(vecTemp.begin(), vecTemp.end());
// 		std::sort(m_vecStudentId.begin(), m_vecStudentId.end(), _KPlayerSexSort());
// 	}
// 	else
// 	{
// 		if (nCount < emKRELATION_SEARCHRES_MAXCOUNT)
// 			m_nLastTeacherId = 0;
// 		else
// 			m_nLastTeacherId = nLastId;
// 		m_vecTeacherId.clear();
// 		if (vecTemp.size() > 0)
// 			m_vecTeacherId.assign(vecTemp.begin(), vecTemp.end());
// 		std::sort(m_vecTeacherId.begin(), m_vecTeacherId.end(), _KPlayerSexSort());
// 	}
	return TRUE;
	
}

// 搜索符合条件的可拜为师父的在线玩家
BOOL KRelationManager_GC::SearchTeacherList(IKTrainingFactory*	pFactory,
											KRole*			pStudent,
											INT					nPlayerID)
{
	// KGC_CONFIRM_RET_FALSE(pFactory && pStudent && nPlayerID > 0);
// 	if (CheckIsLoadEnd(pStudent, nPlayerID) == FALSE)
// 		return FALSE;
// 
// 	CONST INT nMaxLen = sizeof(KRELATION_PROTO_G2S_SEARCHTEACHER) + emKRELATION_SEARCHRES_MAXCOUNT * sizeof(KRELATION_TEACHER);
// 	CHAR szBuf[nMaxLen];
// 	KRELATION_PROTO_G2S_SEARCHTEACHER* pResult = (KRELATION_PROTO_G2S_SEARCHTEACHER*)szBuf;
// 
// 	INT nTeacherID = 0;
// 	INT nLastID = 0;
// 	INT nCount = 0;
// 	INT nLevel = pStudent->GetLevel();
// 	IKRelationFactory* pTrainedFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pTrainedFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	// KGC_CONFIRM_RET_FALSE(pTrainedFactory);
// 	
// 	KRELATION_TEACHER* pRLTeacher = (KRELATION_TEACHER*)(pResult + 1);
// 	time_t	tNowTime	= KSysService::Time(NULL);
// 	if (tNowTime - m_tLastSearchTeacher >= emKRELATION_NEXTSEARCHSCRAP)
// 	{	
// 		m_tLastSearchTeacher = tNowTime;
// 		PreSearchPlayer(pFactory, FALSE);
// 	}
// 	INT nSize	= (INT)m_vecTeacherId.size();
// 	if (nSize >= emKRELATION_SHOWPLAYERNUMBER)
// 	{
// 		std::random_shuffle(m_vecTeacherId.begin(), m_vecTeacherId.end());
// 	}
// 	for (INT i = 0; i < nSize && nCount < emKRELATION_SHOWPLAYERNUMBER ; ++i)
// 	{
// 		nTeacherID	= m_vecTeacherId[i].nPlayerId;
// 		if (nPlayerID == nTeacherID)
// 			continue;
// 		KRole* pTeacher = g_pPlayerManager->GetPlayer(nTeacherID);
// 		KGC_CONFIRM_CONTINUE(pTeacher);
// 		if (pTeacher->GetOnlineServer() <= 0)
// 			continue;
// 		
// 		INT nCurStudentCount = 0;
// 		INT nHisStudentCount = 0;
// 		IKEnumRelation* pEnumHisStudent = NULL;
// 		IKEnumRelation* pEnumCurStudent = NULL;
// 		if (pTrainedFactory->GetAllRelations(true, nTeacherID, &pEnumHisStudent))
// 			nHisStudentCount = pEnumHisStudent->GetCount();
// 		if (pFactory->GetAllRelations(true, nTeacherID, &pEnumCurStudent))
// 			nCurStudentCount = pEnumCurStudent->GetCount();
// 	
// 		strncpy(pRLTeacher->szName, pTeacher->GetPlayerName(), _NAME_LEN - 1);
// 		pRLTeacher->szName[_NAME_LEN - 1] = 0;
// 		pRLTeacher->szTong[0]		 = 0;	// TODO: 帮会名
// 		pRLTeacher->bySex			 = pTeacher->GetSex();
// 		pRLTeacher->byFaction		 = pTeacher->GetFaction();
// 		pRLTeacher->nLevel			 = pTeacher->GetLevel();
// 		pRLTeacher->nPlayerID		 = nTeacherID;
// 		pRLTeacher->wCurStudentCount = nCurStudentCount;
// 		pRLTeacher->wHisStudentCount = nHisStudentCount;
// 		pRLTeacher->wAllStudentCount = pTeacher->GetTrainingAllStudentCount();
// 		pRLTeacher++;
// 		nCount++;
// 	}
// 	INT nPackLen = sizeof(KRELATION_PROTO_G2S_SEARCHTEACHER) + nCount * sizeof(KRELATION_TEACHER);
// 	pResult->ProtocolType = emKRELATION_G2S_PROTO_SEARCHTEACHERRES;
// 	pResult->wCount		  = nCount;
// 	pResult->nPlayerID	  = nPlayerID;
// 	
// 	return SendData(pResult, nPackLen, pStudent->GetOnlineServer());
	return TRUE;
}

// 搜索符合条件的可收为弟子的玩家
BOOL KRelationManager_GC::SearchStudentList(IKTrainingFactory*	pFactory,
											KRole*			pTeacher,
											INT					nPlayerID)
											
{

	// KGC_CONFIRM_RET_FALSE(pFactory && pTeacher && nPlayerID > 0);
// 	if (CheckIsLoadEnd(pTeacher, nPlayerID) == FALSE)
// 		return FALSE;
// 
// 	CONST INT nMaxLen = sizeof(KRELATION_PROTO_G2S_SEARCHSTUDENT) + emKRELATION_SEARCHRES_MAXCOUNT * sizeof(KRELATION_STUDENT);
// 	CHAR szBuf[nMaxLen];
// 	KRELATION_PROTO_G2S_SEARCHSTUDENT* pResult = (KRELATION_PROTO_G2S_SEARCHSTUDENT*)szBuf;
// 
// 	
// 	INT nStudentID = 0;
// 	INT nLastID = 0;
// 	INT nCount = 0;
// 	KRELATION_STUDENT* pRLStudent = (KRELATION_STUDENT*)(pResult + 1);
// 	time_t	tNowTime	= KSysService::Time(NULL);
// 	if (tNowTime - m_tLastSearchStudent >= emKRELATION_NEXTSEARCHSCRAP)
// 	{	
// 		m_tLastSearchStudent = tNowTime;
// 		
// 		PreSearchPlayer(pFactory, TRUE);
// 	}
// 
// 	INT nSize	= (INT)m_vecStudentId.size();
// 	if (nSize >= emKRELATION_SHOWPLAYERNUMBER)
// 	{
// 		std::random_shuffle(m_vecStudentId.begin(), m_vecStudentId.end());
// 	}
// 	for (INT i = 0; i < nSize && nCount < emKRELATION_SHOWPLAYERNUMBER; ++i)
// 	{
// 		nStudentID	= m_vecStudentId[i].nPlayerId;
// 		if (nPlayerID == nStudentID)
// 			continue;
// 		KRole* pStudent = g_pPlayerManager->GetPlayer(nStudentID);
// 		KGC_CONFIRM_CONTINUE(pStudent);
// 		if (pStudent->GetOnlineServer() <= 0)
// 			continue;
// 		strncpy(pRLStudent->szName, pStudent->GetPlayerName(), _NAME_LEN - 1);
// 		pRLStudent->szName[_NAME_LEN - 1] = 0;
// 		DWORD dwTongId = pStudent->GetPlayerTongId();
// 		IKTong cTong = g_pTongManager->GetTong(dwTongId);
// 		if (cTong)
// 		{
// 			::memcpy(pRLStudent->szTong, cTong.GetName(), sizeof(pRLStudent->szTong));
// 			pRLStudent->szTong[_NAME_LEN - 1] = 0;
// 		}
// 		else
// 		{
// 			pRLStudent->szTong[0] = 0;
// 		}
// 		pRLStudent->bySex		= pStudent->GetSex();
// 		pRLStudent->byFaction	= pStudent->GetFaction();
// 		pRLStudent->nLevel		= pStudent->GetLevel();
// 		pRLStudent->nPlayerID	= nStudentID;
// 		
// 		pRLStudent++;
// 		nCount++;
// 	}
// 
// 	INT nPackLen = sizeof(KRELATION_PROTO_G2S_SEARCHSTUDENT) + nCount * sizeof(KRELATION_STUDENT);
// 	pResult->ProtocolType = emKRELATION_G2S_PROTO_SEARCHSTUDENTRES;
// 	pResult->wCount		  = nCount;
// 	pResult->nPlayerID	  = nPlayerID;
// 	
// 	return SendData(pResult, nPackLen, pTeacher->GetOnlineServer());
	return FALSE;
}

BOOL KRelationManager_GC::SyncStartRelationList(INT nPlayerID)
{
	BOOL					bRetCode = FALSE;
	IMemBlock*				pMemBlock = NULL;
	R2S_RELATION_SYNC_LIST_RESPOND*	pSync = NULL;
	KRole*					pRole = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerID);
	LOG_PROCESS_ERROR(pRole);

	pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_SYNC_LIST_RESPOND));
	LOG_PROCESS_ERROR(pMemBlock);
	pSync = (R2S_RELATION_SYNC_LIST_RESPOND*)pMemBlock->GetData();
	pSync->wProtocolID		= r2s_relation_start_sync_list_respond;
	pSync->nPlayerID		= nPlayerID;
	pSync->byNotifyClient	= 1;
	pSync->nCount			= 0;
	pSync->byCreate			= FALSE;

	bRetCode = g_pGameCenter->m_GameServer.Send(pRole->m_nConnIndex, pMemBlock);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(pMemBlock);
	return bRetCode;

// 	CHAR szBuf[sizeof(R2S_RELATION_SYNC_LIST_RESPOND)];
// 	R2S_RELATION_SYNC_LIST_RESPOND* pSync = (R2S_RELATION_SYNC_LIST_RESPOND*)szBuf;
// 	// KGC_CONFIRM_RET_FALSE(pSync);
// 	pSync->ProtocolType     = r2s_relation_start_sync_list_respond;
// 	pSync->nPlayerID		= nPlayerID;
// 	pSync->byNotifyClient	= 1;
// 
// 	INT nPackLen = sizeof(R2S_RELATION_SYNC_LIST_RESPOND);
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(nPackLen);
// 	memcpy(sBuf.GetBuf(), szBuf, nPackLen);
// 	return m_pProtocolProcessor->SendPacket(sBuf, nConnectID);
// 	return TRUE;
}

BOOL KRelationManager_GC::SyncRelationList(INT					nPlayerId,
										   INT					nRelationType,
										   IKRelationFactory*	pFactory,
										   BOOL					bAsMaster,
										   BOOL					bNotifyClient)
{
	BOOL									bRetCode		= FALSE;
	IKEnumRelation*							pEnum			= NULL;
	INT										nCount			= 0;
	IMemBlock*								pMemBlock		= NULL;
	R2S_RELATION_SYNC_LIST_RESPOND*					pSync			= NULL;
	KRELATION_INFO*							pInfo			= NULL;
	IKRelation*								pRelation		= NULL;
	INT										nIndex			= 0;
	INT										nRelationID		= 0;
	KRole*									pRole			= NULL;

	LOG_PROCESS_ERROR(pFactory);
	bRetCode = pFactory->GetAllRelations(bAsMaster, nPlayerId, &pEnum);
	PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pEnum);

	nCount = pEnum->GetCount();
	LOG_PROCESS_ERROR(nCount <= emKRELATION_MAX_COUNT);

	pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + nCount * sizeof(KRELATION_INFO));
	LOG_PROCESS_ERROR(pMemBlock);

	pSync = (R2S_RELATION_SYNC_LIST_RESPOND *)pMemBlock->GetData();
	pSync->wProtocolID		= r2s_relation_sync_list_respond;
	pSync->nType			= nRelationType;
	pSync->nRole			= bAsMaster ? 0 : 1;
	pSync->nPlayerID		= nPlayerId;
	pSync->byNotifyClient	= bNotifyClient ? 1 : 0;
	pSync->byCreate			= FALSE;

	pInfo = pSync->aryInfo;

	while (pEnum->Next(&pRelation) && nIndex < nCount)
	{
		LOG_PROCESS_ERROR(pRelation);

		nRelationID  = pRelation->GetOtherPlayerId(nPlayerId);
		if (nRelationID <= 0)
		{
			// TODO: assert(0); wangbin
			ASSERT(0);
			continue;
		}

		pRole = g_pGameCenter->m_RoleManager.GetRole(nRelationID);
		if (!pRole)
		{
			// TODO: assert(0); wangbin
			ASSERT(0);
			continue;
		}

		memcpy(pInfo->szRelation, pRole->m_szName, sizeof(pRole->m_szName));
		pInfo->nOnline = pRole->IsOnline();
		pInfo->nSex				= (INT)pRole->m_byType;//pRole->GetSex();
		pInfo->nLevel			= (INT)pRole->m_byLevel;//pRole->GetLevel();
		pInfo->nPlayerId		= pRole->GetId();
		pInfo++;
		nIndex++;
	}

	if (nIndex > 0)
	{
		pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
		LOG_PROCESS_ERROR(pRole);

		pSync->nCount = nIndex;
		bRetCode = g_pGameCenter->m_GameServer.Send(pRole->m_nConnIndex, pMemBlock);
		LOG_PROCESS_ERROR(bRetCode);
	}


	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(pMemBlock);
	return bRetCode;

// 	if (!pFactory->GetAllRelations(bAsMaster, nPlayerId, &pEnum))
// 		return FALSE;
// 	pEnum->Reset();
// 	INT nCount = pEnum->GetCount();
// 	// KGC_CONFIRM_RET_FALSE(nCount <= emKRELATION_MAX_COUNT);
// 	
// 
// 	CHAR szBuf[sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + emKRELATION_MAX_COUNT * sizeof(KRELATION_INFO)];
// 	R2S_RELATION_SYNC_LIST_RESPOND *pSync = (R2S_RELATION_SYNC_LIST_RESPOND*)szBuf;
// 	// KGC_CONFIRM_RET_FALSE(pSync);
// 						  
// 	pSync->ProtocolType = r2s_relation_sync_list_respond;
// 	pSync->nType			= nRelationType;
// 	pSync->nRole			= bAsMaster ? 0 : 1;
// 	pSync->nPlayerID		= nPlayerId;
// 	pSync->byNotifyClient	= bNotifyClient ? 1 : 0;
// 	KRELATION_INFO *pInfo = pSync->aryInfo;
// 
// 	IKRelation *pRelation = NULL;
// 	INT nIndex = 0;
// 	while (pEnum->Next(&pRelation) && nIndex < nCount)
// 	{
// 		KGC_CONFIRM_CONTINUE(pRelation);
// 
// 		INT nRelationID  = pRelation->GetOtherPlayerId(nPlayerId);
// 		if (nRelationID <= 0)
// 		{
// 			// TODO: assert(0); wangbin
// 			continue;
// 		}
// 		KRole* pGCPlayer = g_pPlayerManager->GetPlayer(nRelationID);
// 		if (!pGCPlayer)
// 		{
// 			// TODO: assert(0); wangbin
// 			continue;
// 		}
// 		strncpy(pInfo->szRelation, pGCPlayer->GetPlayerName(), sizeof(pInfo->szRelation) - 1);
// 		pInfo->szRelation[sizeof(pInfo->szRelation) - 1] = 0;
// 		pInfo->nOnline = pGCPlayer->GetOnlineServer() > 0 ? 1 : 0;
// 		pInfo++;
// 		nIndex++;
// 	}
// 	if (nIndex > 0)
// 	{
// 		pSync->nCount = nIndex;
// 		INT nPackLen = sizeof(R2S_RELATION_SYNC_LIST_RESPOND) + nIndex * sizeof(KRELATION_INFO);
// 		PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(nPackLen);
// 		memcpy(sBuf.GetBuf(), szBuf, nPackLen);
// 		m_pProtocolProcessor->SendPacket(sBuf, nConnectID);
// 	}
// 	return TRUE;
}

// 保存关系列表并从内存中卸载
BOOL KRelationManager_GC::Unload(INT nPlayerId)
{
	KRelationRuleManager* pRuleManager = KRelationRuleManager::Instance();
	for (INT nType = 0; nType < emKPLAYERRELATION_TYPE_COUNT; nType++)
	{
		CONST KRelationRuleManager::KPOLICY* pPolicy = pRuleManager->GetPolicy(nType);
		IKRelationFactory *pFactory = NULL;
		if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory))
		{
			ASSERT(0);
			return FALSE;
		}
		ASSERT(pFactory);
		pFactory->Unload(nPlayerId);
	}
	return TRUE;
}

// 完全删除某个玩家的关系
BOOL KRelationManager_GC::Remove(INT nPlayerId, LPCSTR pszPlayerName)
{
	// KGC_CONFIRM_RET_FALSE(nPlayerId && pszPlayerName);
	//KRelationDatabase_T<IKRelationDatabase> sDatabase;
	KRelationRuleManager* pRuleManager = KRelationRuleManager::Instance();
	IKRelationFactory *pFactory = NULL;
	for (INT nType = 0; nType < emKPLAYERRELATION_TYPE_COUNT; nType++)
	{
		if (!KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory))
		{
			ASSERT(0);
			return FALSE;
		}
		// 删号时通知关系玩家
		CONST KRelationRuleManager::KPOLICY* pPolicy = pRuleManager->GetPolicy(nType);
		if (pPolicy->bNotifyDeleteAsMaster)
		{
			IKEnumRelation *pEnum = NULL;
			if (pFactory->GetAllRelations(true, nPlayerId, &pEnum))
			{
				NotifyDelete(nPlayerId, pszPlayerName, nType, true, pEnum);
			}
		}
		if (pPolicy->bNotifyDeleteAsTarget)
		{
			IKEnumRelation *pEnum = NULL;
			if (pFactory->GetAllRelations(false, nPlayerId, &pEnum))
			{
				NotifyDelete(nPlayerId, pszPlayerName, nType, false, pEnum);
			}
		}
		pFactory->Remove(nPlayerId);
	}
	return TRUE;
}

// 通知关系列表加载完毕
BOOL KRelationManager_GC::NotifyRelationListLoaded(INT nPlayerId)
{
	ASSERT(nPlayerId > 0);
	//告诉客户端开始同步数据
	SyncStartRelationList(nPlayerId);
	// 同步关系列表
	SyncRelationList(nPlayerId);
	// 好友列表单独同步
	SyncFriendListToMyself(nPlayerId);

	return TRUE;
}

// 通知好友自己删号
BOOL KRelationManager_GC::NotifyDelete(INT nSelfId, LPCSTR pszPlayerName, INT nType, BOOL bAsMaster, IKEnumRelation *pEnum)
{
	BOOL							bRetCode			= FALSE;
	IMemBlock*						pMemBlock			= NULL;
	R2S_RELATION_DEL_RELATION_RESPOND* pNotify			= NULL;
	IKRelation*						pRelation			= NULL;
	INT								nPlayerID			= 0;
	KRole*							pRole				= NULL;

	pMemBlock = QCreateMemBlock(sizeof(R2S_RELATION_DEL_RELATION_RESPOND));
	LOG_PROCESS_ERROR(pMemBlock);

	pNotify = (R2S_RELATION_DEL_RELATION_RESPOND*)pMemBlock->GetData();
	pNotify->wProtocolID			= r2s_relation_delete_player_respond;
	pNotify->nType					= nType;
	pNotify->nRelationRole			= bAsMaster ? 0 : 1;
	pNotify->nPlayerID				= 0;
	memset(pNotify->szRelation, 0, sizeof(pNotify->szRelation));
	memcpy(pNotify->szRelation, pszPlayerName, sizeof(pNotify->szRelation));

	while (pEnum->Next(&pRelation))
	{
		LOG_PROCESS_ERROR(pRelation);

		nPlayerID  = pRelation->GetOtherPlayerId(nSelfId);
		if (nPlayerID <= 0)
			continue;

		pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerID);
		LOG_PROCESS_ERROR(pRole);

		if (pRole->m_nConnIndex > 0)
		{
			pNotify->nPlayerID = nPlayerID;
			g_pGameCenter->m_GameServer.Send(pRole->m_nConnIndex, pMemBlock);
		}
	}

	bRetCode = TRUE;
EXIT0:
	SAFE_RELEASE(pMemBlock);
	return bRetCode;

// 	// KGC_CONFIRM_RET_FALSE(pszPlayerName && pEnum);
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(R2S_RELATION_DEL_RELATION_RESPOND));
// 	R2S_RELATION_DEL_RELATION_RESPOND *pNotify = (R2S_RELATION_DEL_RELATION_RESPOND*)sBuf.GetBuf();
// 	pNotify->ProtocolType = r2s_relation_delete_player_respond;
// 	pNotify->nType = nType;
// 	pNotify->nRelationRole = bAsMaster ? 0 : 1;
// 	pNotify->nPlayerID = 0;
// 	strncpy(pNotify->szRelation, pszPlayerName, sizeof(pNotify->szRelation) - 1);
// 	pNotify->szRelation[sizeof(pNotify->szRelation) - 1] = 0;
// 
// 	IKRelation *pRelation = NULL;
// 	while (pEnum->Next(&pRelation))
// 	{
// 		KGC_CONFIRM_CONTINUE(pRelation);
// 		INT nPlayerID  = pRelation->GetOtherPlayerId(nSelfId);
// 		if (nPlayerID <= 0)
// 			continue;
// 		KRole* pGCPlayer = g_pPlayerManager->GetPlayer(nPlayerID);
// 		KGC_CONFIRM_CONTINUE(pGCPlayer);
// 		
// 		INT nServerID = pGCPlayer->GetOnlineServer();
// 		if (nServerID > 0)
// 		{
// 			pNotify->nPlayerID = nPlayerID;
// 			m_pProtocolProcessor->SendPacket(sBuf, nServerID);
// 		}
// 	}
// 	return TRUE;
}

// 和师徒相关的检查
BOOL KRelationManager_GC::TrainingCheck(INT nPlayerId)
{
	// KGC_CONFIRM_RET_FALSE (nPlayerId > 0);
	
// 	KRole* pGCPlayer = g_pPlayerManager->GetPlayer(nPlayerId);
// 	// KGC_CONFIRM_RET_FALSE(pGCPlayer);
// 	if (CheckIsLoadEnd(pGCPlayer, nPlayerId) == FALSE)
// 		return FALSE;
// 	IKRelationFactory* pFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINING, &pFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	INT nCount = sizeof(ms_aTrainingEvent) / sizeof(ms_aTrainingEvent[0]);
// 	for (INT i = 0; i < nCount; ++i)
// 	{
// 		KTRAINING_EVENT* pEvent = &ms_aTrainingEvent[i];
// 		LPVOID pParam = NULL;
// 		if ((this->*pEvent->pfnCheck)(pGCPlayer, nPlayerId, pFactory, &pParam))
// 		{
// 			(this->*pEvent->pfnEvent)(pGCPlayer, nPlayerId, pFactory, pParam);
// 		}
// 	}
	return TRUE;
}

// 检查是否可以拜师
BOOL KRelationManager_GC::TrainingCheck_Student(KRole*			pPlayer,
												INT					nPlayerID,
												IKRelationFactory*	pFactory,
												LPVOID *				ppParam)
{
	// KGC_CONFIRM_RET_FALSE(ppParam);
	// 检查级别
// 	INT nLevel = pPlayer->GetLevel();
// 	if (nLevel < g_cTrainingSetting.GetStudentMinLevel() ||
// 		nLevel >= g_cTrainingSetting.GetStudentHintMaxLevel())
// 	{
// 		return FALSE;
// 	}
// 	// 检查当前是否徒弟
// 	IKEnumRelation* pEnum = NULL;
// 	if (pFactory->GetAllRelations(false, nPlayerID, &pEnum) &&
// 		pEnum->GetCount() > 0)
// 	{
// 		return FALSE;
// 	}
// 	IKRelationFactory* pHisFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pHisFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	// 检查是否出师徒弟
// 	IKEnumRelation* pHisEnum = NULL;
// 	if (pHisFactory->GetAllRelations(false, nPlayerID, &pHisEnum) && 
// 		pHisEnum->GetCount() > 0)
// 	{
// 		return FALSE;
// 	}
	return TRUE;
}


// 提醒玩家拜师
BOOL KRelationManager_GC::TrainingEvent_Student(KRole*			pPlayer,
												INT					nPlayerID,
												IKRelationFactory*	pFactory,
												LPVOID				pParam)
{
	// KGC_CONFIRM_RET_FALSE(pPlayer);
// 	INT nServerId = pPlayer->GetOnlineServer();
// 	// 系统提示
// 	if (nServerId)
// 	{
// 		g_ChatChannelManager.PostSystemMessage(nPlayerID,
// 		KD_STR_ANNOUNCE,
// 		KD_STR_TRAINING_STUDENTONLINEHINT,
// 		nServerId);
// 	}
// 	// 检查上次发送邮件时间
// 	INT nHint = pPlayer->GetTrainingStudentHintTime();
// 	if (nHint != 1)
// 	{
// 		// 发送提示拜师邮件
// 		KMailManager_GC::Instance()->PostSysMail(
// 			nPlayerID,
// 			KD_STR_TRAINING_MAILHEAD_STUDENTHINT,
// 			KD_STR_TRAINING_STUDENTONLINEMAIL);
// 		pPlayer->SetTrainingStudentHintTime(1);
// 	}
	return TRUE;
}

// 检查是否可以收徒
BOOL KRelationManager_GC::TrainingCheck_Teacher(KRole*			pPlayer,
												INT					nPlayerID,
												IKRelationFactory*	pFactory,
												LPVOID *				ppParam)
{
	// KGC_CONFIRM_RET_FALSE(ppParam);
	// 检查级别
// 	INT nLevel = pPlayer->GetLevel();
// 	if (nLevel < g_cTrainingSetting.GetTeacherMinLevel())
// 		return FALSE;
// 	// 当前弟子
// 	IKEnumRelation* pEnum = NULL;
// 	if (pFactory->GetAllRelations(true, nPlayerID, &pEnum) && pEnum->GetCount() > 0)
// 		return FALSE;
// 	// 出师弟子
// 	IKRelationFactory* pHisFactory = NULL;
// 	if (!KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_TRAINED, &pHisFactory))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	IKEnumRelation* pHisEnum = NULL;
// 	if (pHisFactory->GetAllRelations(true, nPlayerID, &pHisEnum) &&
// 		pHisEnum->GetCount() > 0)
// 	{
// 		return FALSE;
// 	}
	return TRUE;
}

// 提醒玩家收徒
BOOL KRelationManager_GC::TrainingEvent_Teacher(KRole*			pPlayer,
												INT					nPlayerID,
												IKRelationFactory*	pFactory,
												LPVOID				pParam)
{
	// KGC_CONFIRM_RET_FALSE(pPlayer);
// 	INT nServerId = pPlayer->GetOnlineServer();
// 	// 系统提示
// 	if (nServerId)
// 	{
// 		g_ChatChannelManager.PostSystemMessage(
// 			nPlayerID,
// 			KD_STR_ANNOUNCE,
// 			KD_STR_TRAINING_TEACHERONLINEHINT,
// 			nServerId);
// 	}
// 	// 检查上次发送邮件时间
// 	INT  nHint = pPlayer->GetTrainingTeacherHintTime();
// 	if (nHint != 1)
// 	{
// 		// 发送提示收徒邮件
// 		KMailManager_GC::Instance()->PostSysMail(
// 			nPlayerID,
// 			KD_STR_TRAINING_MAILHEAD_TEACHERHINT,
// 			KD_STR_TRAINING_TEACHERONLINEMAIL);
// 		pPlayer->SetTrainingTeacherHintTime(1);
// 	}
	return TRUE;
}

BOOL KRelationManager_GC::TrainingCheck_TimeOut(KRole*			pPlayer,
												INT					nPlayerID,
												IKRelationFactory*	pFactory,
												LPVOID *				ppParam)
{
	// KGC_CONFIRM_RET_FALSE(ppParam);
	// 检查是否有当前师父
// 	IKEnumRelation* pEnum = NULL;
// 	if (!pFactory->GetAllRelations(false, nPlayerID, &pEnum) ||
// 		pEnum->GetCount() == 0)
// 	{
// 		return FALSE;
// 	}
// 	// 检查报到时间是否超时
// 	time_t ttCheckTime = (time_t)pPlayer->GetTrainingCheckInTime();
// 	if (ttCheckTime == 0)
// 		return FALSE;
// 	time_t ttNow = KSysService::Time(NULL);
// 	if (ttNow - ttCheckTime < g_cTrainingSetting.GetCheckInSecCount())
// 		return FALSE;
// 
// 	*ppParam = pEnum;
	return TRUE;
}

BOOL KRelationManager_GC::TrainingEvent_TimeOut(KRole*			pPlayer,
												INT					nPlayerID,
												IKRelationFactory*	pFactory,
												LPVOID				pParam)
{
	// KGC_CONFIRM_RET_FALSE(pParam);
// 	IKEnumRelation* pEnum = (IKEnumRelation*)pParam;
// 	// 删除师徒关系
// 	// KGC_CONFIRM_RET_FALSE(pEnum->GetCount() == 1);
// 	IKRelation* pRelation = NULL;
// 	if (!pEnum->Next(&pRelation))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	INT nTeacherID = pRelation->GetRole(TRUE);
// 	// KGC_CONFIRM_RET_FALSE(nTeacherID > 0);
// 	if (!pFactory->DeleteRelation(nTeacherID, nPlayerID))
// 	{
// 		assert(0);
// 		return FALSE;
// 	}
// 	// 发送通知邮件
// 	KRole* pTeacher = g_pPlayerManager->GetPlayer(nTeacherID);
// 	// KGC_CONFIRM_RET_FALSE(pTeacher);
// 	
// 	KMailManager_GC* pMailManager = KMailManager_GC::Instance();
// 	CHAR szMsg[1024];
// 	LPCSTR pszStudent = pPlayer->GetPlayerName();
// 	LPCSTR pszTeacher = pTeacher->GetPlayerName();
// 	sprintf(szMsg, KD_STR_TRAINING_BREAK_TEACHERMAIL,
// 		pszStudent,
// 		g_cTrainingSetting.GetCheckInDayCount(),
// 		pszStudent);
// 	pMailManager->PostSysMail(nTeacherID, KD_STR_TRAINING_BREAK_CAPTION, szMsg);
// 	sprintf(szMsg, KD_STR_TRAINING_BREAK_STUDENTMAIL,
// 		g_cTrainingSetting.GetCheckInDayCount(),
// 		pszTeacher,
// 		pszTeacher);
// 	return pMailManager->PostSysMail(nPlayerID, KD_STR_TRAINING_BREAK_CAPTION, szMsg);
	return FALSE;
}

// 设置好友信息
BOOL KRelationManager_GC::InitRelationInfo(KRole* pMaster, KRole* pTarget, INT nFavor, DWORD dwLastTalkTime, KRELATION_PROTO_FRIENDINFO* pInfo)
{
	ASSERT(nFavor >= 0);
	memcpy(pInfo->szRelation, pTarget->m_szName, sizeof(pTarget->m_szName));
	pInfo->szRelation[sizeof(pInfo->szRelation) - 1] = 0;
	pInfo->nFavor	 = nFavor;
	pInfo->nLevel	 = pTarget->GetLevel();
	pInfo->nSex		 = pTarget->GetSex();
	pInfo->nKinFriend = (pTarget->m_dwTongID != KD_BAD_ID) && (pMaster->m_dwTongID != KD_BAD_ID) && (pTarget->m_dwTongID == pMaster->m_dwTongID);
	pInfo->nPortrait = 0;
	pInfo->dwLastTalkTime = dwLastTalkTime;
	return TRUE;
}

VOID KRelationManager_GC::OnFavorLevelUp(INT nPlayerAppId, INT nPlayerDstId, INT nFavorLevel)
{
	if (nPlayerAppId <= 0 || nPlayerDstId <= 0 || nFavorLevel <= 0)
		return;

// 	KScriptSafeCall cSafeCall(*g_pGCScript);
// 	g_pGCScript->CallTableFunction("Relation", "OnFavorLevelUp", 0, "ddd", nPlayerAppId, nPlayerDstId, nFavorLevel);
}

// 处理亲密度等级升高时的奖励
BOOL KRelationManager_GC::DoFrindFavorAward(KRole* pPlayerApp, KRole* pPlayerDst, INT nAppId, INT nDstId, 
											INT nAwardLevel)
{
	
// 	// KGC_CONFIRM_RET_FALSE(nAwardLevel > 0);
// 	INT nAppSvrId = pPlayerApp->GetOnlineServer();
// 	INT nDstSvrId = pPlayerDst->GetOnlineServer();
// 	// KGC_CONFIRM_RET_FALSE(nAppSvrId > 0 && nDstSvrId > 0);
// 	INT nAppCount = 0;
// 	INT nDstCount = 0;
// 	BOOL bSend = FALSE;
// 	PACKET_BUF sBuf = m_pProtocolProcessor->GetPacketBuf(sizeof(KRELATION_PROTO_G2S_AWARDBYFRIENDFAVOR));
// 	KRELATION_PROTO_G2S_AWARDBYFRIENDFAVOR *pNotify = (KRELATION_PROTO_G2S_AWARDBYFRIENDFAVOR*)sBuf.GetBuf();
// 	pNotify->ProtocolType = emKRELATION_G2S_PROTP_AWARDCOINBYFRIENDFAVOR;
// 	pNotify->nLevel	 = nAwardLevel;
// 	
// 	nAwardLevel = nAwardLevel - 1;
// 	// 等级大于10级，奖励人数不再限制
// 	if ( nAwardLevel >= (INT)m_vecFriendAwardLevel.size())
// 		return FALSE;
// 
// 	pNotify->byAwardApp	 = 0;
// 	pNotify->byGiveCountApp = 0;
// 	pNotify->byLeftCountApp = 0;
// 
// 	INT nLeftCount = 0;
// 	CONST KFriendAward& rcFriendAward = m_vecFriendAwardLevel[nAwardLevel];
// 	pNotify->nAwardCoin = rcFriendAward.nAward;
// 	pNotify->byMaxNum	= rcFriendAward.nMaxNum;
// 	
// 	nAppCount = pPlayerApp->GetAwardNumByLevel(nAwardLevel);
// 	nDstCount = pPlayerDst->GetAwardNumByLevel(nAwardLevel);
// 	
// 	// 好友人数大于等于领奖人数
// 	INT nCount	= KRelationCounter::Instance()->GetOneTypeCount(nAppId, emKPLAYERRELATION_TYPE_BINDFRIEND);
// 	if (nCount >= nAppCount && nAppCount < rcFriendAward.nMaxNum)
// 	{
// 		pPlayerApp->AddAwardNumByLevel(nAwardLevel);
// 		nAppCount += 1;
// 		if (nAwardLevel == 0 && nAppCount < nCount)
// 		{
// 			nCount = nCount < rcFriendAward.nMaxNum ? nCount : rcFriendAward.nMaxNum;
// 			nCount = nCount - nAppCount ;
// 			nCount = nCount < 0 ? 0 : nCount;
// 			SetBeforeFrindFavor(pPlayerApp, nCount, 0);
// 			pNotify->byGiveCountApp = nCount;
// 		}
// 		pNotify->byLeftCountApp = (BYTE)(rcFriendAward.nMaxNum - pPlayerApp->GetAwardNumByLevel(nAwardLevel));
// 		pNotify->byAwardApp = 1;
// 
// 		pNotify->nPlayerAppId = nAppId;
// 		pNotify->nPlayerDstId = nDstId;
// 		m_pProtocolProcessor->SendPacket(sBuf, nAppSvrId);
// 	}
// 	nCount	= KRelationCounter::Instance()->GetOneTypeCount(nDstId, emKPLAYERRELATION_TYPE_BINDFRIEND);
// 	if (nCount >= nDstCount && nDstCount < rcFriendAward.nMaxNum)
// 	{
// 		pPlayerDst->AddAwardNumByLevel(nAwardLevel);
// 		nDstCount += 1;
// 		if (nAwardLevel == 0 && nDstCount < nCount)
// 		{
// 			nCount = nCount < rcFriendAward.nMaxNum ? nCount : rcFriendAward.nMaxNum;
// 			nCount = nCount - nDstCount;
// 			nCount = nCount < 0 ? 0 : nCount;
// 			SetBeforeFrindFavor(pPlayerDst, nCount, 0);
// 			pNotify->byGiveCountApp = nCount;
// 		}
// 		pNotify->byLeftCountApp = (BYTE)(rcFriendAward.nMaxNum - pPlayerDst->GetAwardNumByLevel(nAwardLevel));
// 		pNotify->byAwardApp = 1;
// 
// 		pNotify->nPlayerAppId = nDstId;
// 		pNotify->nPlayerDstId = nAppId;
// 		m_pProtocolProcessor->SendPacket(sBuf, nDstSvrId);
// 	}	
	return TRUE;	
}

BOOL KRelationManager_GC::GetCloseFriendTimeInfo(INT nAppPlayerId, std::vector<KTIMEINFO>& rvctTimeInfo)
{
	BOOL bRet = FALSE;
// 	do 
// 	{
// 		KRole* pAppPlayer = g_pPlayerManager->GetPlayer(nAppPlayerId);
// 		KGC_CONFIRM_BREAK(pAppPlayer);
// 		KGC_CONFIRM_BREAK(CheckIsLoadEnd(pAppPlayer, nAppPlayerId, FALSE));
// 
// 		// 需要进行密友期限检测的关系有：指定密友，介绍人关系，出师的师徒关系
// 		std::vector<INT> vctRelationNeedCheck;
// 		vctRelationNeedCheck.push_back(emKPLAYERRELATION_TYPE_BUDDY);
// 		vctRelationNeedCheck.push_back(emKPLAYERRELATION_TYPE_INTRODUCTION);
// 		vctRelationNeedCheck.push_back(emKPLAYERRELATION_TYPE_TRAINED);
// 
// 		// 每种密友关系都需要做主位和次位的检查
// 		std::vector<BOOL> vctAsMaster;
// 		vctAsMaster.push_back(TRUE);
// 		vctAsMaster.push_back(FALSE);
// 
// 		IKRelationFactory *pFactory = NULL;
// 		std::vector<INT>::iterator itRelation = vctRelationNeedCheck.begin();
// 		for (; itRelation != vctRelationNeedCheck.end(); ++itRelation)
// 		{
// 			INT nRelationType = *itRelation;
// 			KGC_CONFIRM_CONTINUE(KRelationFactoryList::Instance()->CreateFactory(nRelationType, &pFactory));
// 			IKEnumRelation* pEnum = NULL;
// 			IKRelation* pRelation = NULL; 
// 			KCloseFriendRelation*	pCloseRelation = NULL;
// 
// 			std::vector<BOOL>::iterator itAsMaster = vctAsMaster.begin();
// 			for (; itAsMaster != vctAsMaster.end(); ++itAsMaster)
// 			{
// 				if (!pFactory->GetAllRelations(*itAsMaster, nAppPlayerId, &pEnum) || pEnum->GetCount() <= 0)
// 					continue;
// 
// 				// 指定密友关系是双向对等的，主位和次位之中只需要检查一种就行了
// 				if (*itAsMaster == FALSE && nRelationType == emKPLAYERRELATION_TYPE_BUDDY)
// 					continue;
// 
// 				while (pEnum->Next(&pRelation))
// 				{
// 					KGC_CONFIRM_CONTINUE(pRelation);
// 					KGC_CONFIRM_CONTINUE(pRelation->QueryRelation(nRelationType, (LPVOID *)&pCloseRelation));
// 					INT nPlayerId = pCloseRelation->GetOtherPlayerId(nAppPlayerId);
// 					KGC_CONFIRM_CONTINUE(nPlayerId > 0);
// 					KTIMEINFO sTimeInfo;
// 					INT nRemanentTime = pCloseRelation->GetRemanentTime();
// 					sTimeInfo.nPlayerId = nPlayerId;
// 					sTimeInfo.nRemanentTime = nRemanentTime;
// 					sTimeInfo.nType = nRelationType;
// 					sTimeInfo.bAsMaster = *itAsMaster;
// 					rvctTimeInfo.push_back(sTimeInfo);
// 				}
// 			}
// 		}
// 
// 		bRet = TRUE;
// 	} while (0);
	return bRet;
}

INT KRelationManager_GC::GetOneRelationCount(INT nPlayerId, INT nType, BOOL bAsMaster /*= TRUE*/)
{
	INT						nCount				= 0;
	KRole*					pRole				= NULL;
	KRelationCounter*		pCounter			= NULL;
	KRole*					pPlayer				= NULL;
	
	LOG_PROCESS_ERROR(nPlayerId > 0 && nType >= emKPLAYERRELATION_TYPE_TMPFRIEND &&
		nType < emKPLAYERRELATION_TYPE_COUNT);
	pPlayer = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	LOG_PROCESS_ERROR(pPlayer);

	pCounter = KRelationCounter::Instance();
	nCount = pCounter->GetOneTypeCount(nPlayerId, nType, bAsMaster);
	
EXIT0:
	return nCount;
}

BOOL KRelationManager_GC::HasRelation(INT nAppId, INT nDstId, INT nType, BOOL bAsMaster /* = TRUE */)
{
	BOOL				bRetCode = FALSE;
	KRole*				pMaster  = NULL;
	KRole*				pTarget  = NULL;
	IKRelationFactory*	pFactory = NULL;
	IKRelation*			pRelation = NULL;

	LOG_PROCESS_ERROR(nAppId > 0 && nDstId > 0 && nType >= emKPLAYERRELATION_TYPE_TMPFRIEND &&
		nType < emKPLAYERRELATION_TYPE_COUNT);

	pMaster = g_pGameCenter->m_RoleManager.GetRole(nAppId);
	pTarget = g_pGameCenter->m_RoleManager.GetRole(nDstId);
	LOG_PROCESS_ERROR(pMaster && pTarget);

	bRetCode = KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(pFactory);

	bRetCode = pFactory->FindRelation(nAppId, nDstId, &pRelation);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return bRetCode;

// 	do 
// 	{
// 		KGC_CONFIRM_BREAK(nAppId > 0 && nDstId > 0 && nType >= emKPLAYERRELATION_TYPE_TMPFRIEND &&
// 			nType < emKPLAYERRELATION_TYPE_COUNT);
// 		KRole* pPlayerApp = g_pPlayerManager->GetPlayer(nAppId);
// 		KRole* pPlayerDst = g_pPlayerManager->GetPlayer(nDstId);
// 		KGC_CONFIRM_BREAK(pPlayerApp && pPlayerDst);
// 
// 		IKRelationFactory* pFactory = NULL;
// 		IKRelation* pRelation = NULL;
// 		KGC_CONFIRM_BREAK(KRelationFactoryList::Instance()->CreateFactory(nType, &pFactory))
// 
// 		bRet = pFactory->FindRelation(nAppId, nDstId, &pRelation);
// 	} while (0);
// 	return bRet;
}

INT KRelationManager_GC::GetFriendFavor(INT nPlayerId1, INT nPlayerId2)
{
	INT nFavor = 0;
// 	do 
// 	{
// 		KGC_CONFIRM_BREAK(nPlayerId1 > 0 && nPlayerId2 > 0 && nPlayerId1 != nPlayerId2);
// 		KRole* pPlayer1 = g_pPlayerManager->GetPlayer(nPlayerId1);
// 		KRole* pPlayer2 = g_pPlayerManager->GetPlayer(nPlayerId2);
// 		KGC_CONFIRM_BREAK(pPlayer1 && pPlayer2);
// 
// 		IKRelationFactory *pFactory = NULL;
// 		IKRelation *pRelation = NULL;
// 		KGC_CONFIRM_BREAK(KRelationFactoryList::Instance()->CreateFactory(emKPLAYERRELATION_TYPE_BINDFRIEND, &pFactory));
// 		KGC_CONFIRM_BREAK(pFactory->FindRelation(nPlayerId1, nPlayerId2, &pRelation));
// 		KGC_CONFIRM_BREAK(pRelation);
// 		IKBidFriendRelation *pFriend = NULL;
// 		KGC_CONFIRM_BREAK(pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend))
// 		nFavor = pFriend->GetFavor();
// 
// 	} while (0);
	return nFavor;
}

BOOL KRelationManager_GC::SetBeforeFrindFavor(KRole* pPlayer, INT nLeftCount, INT nAwardLevel)
{
// 	for (INT i = 0; i < nLeftCount; ++i)
// 		pPlayer->AddAwardNumByLevel(nAwardLevel);
	return TRUE;
}

#define RECODE_FAVOR 401	// 亲密度等级三级
BOOL KRelationManager_GC::NeedLogRecord(IKRelationFactory *pFactory, BYTE byRelationType, INT nMasterId, INT nTargetId)
{
	// KGC_CONFIRM_RET_FALSE(pFactory);
	IKRelation *pRelation = NULL;
	IKBidFriendRelation* pFriend = NULL;
	
	if (byRelationType == emKPLAYERRELATION_TYPE_BINDFRIEND)
	{
		 pFactory->FindRelation(nMasterId, nTargetId, &pRelation);
		if (pRelation && pRelation->QueryRelation(emKPLAYERRELATION_TYPE_BINDFRIEND, (LPVOID *)&pFriend)
			&& pFriend->GetFavor() >= RECODE_FAVOR)
			return TRUE;
	}
	else if (byRelationType == emKPLAYERRELATION_TYPE_TRAINING || byRelationType == emKPLAYERRELATION_TYPE_TRAINED || 
		byRelationType == emKPLAYERRELATION_TYPE_INTRODUCTION || byRelationType == emKPLAYERRELATION_TYPE_BUDDY)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL KRelationManager_GC::RecodeDeleteLog(KRole* pAppPlayer, KRole* pTargetPlayer, BYTE byType)
{
	// KGC_CONFIRM_RET_FALSE(pAppPlayer && pTargetPlayer);
// 	static CHAR szRelationType[256];
// 
// 	switch(byType)
// 	{
// 	case emKPLAYERRELATION_TYPE_BINDFRIEND:
// 		strcpy(szRelationType, KD_STR_BINDFRIEND_TYPE);
// 		break;
// 	case emKPLAYERRELATION_TYPE_TRAINING:
// 		strcpy(szRelationType, KD_STR_TRAINING_TYPE);
// 		break;
// 	case emKPLAYERRELATION_TYPE_TRAINED:
// 		strcpy(szRelationType, KD_STR_TRAINED_TYPE);
// 	    break;
// 	case emKPLAYERRELATION_TYPE_INTRODUCTION:
// 		strcpy(szRelationType, KD_STR_INTRODUCTION_TYPE);
// 	    break;
// 	case emKPLAYERRELATION_TYPE_BUDDY:
// 		strcpy(szRelationType, KD_STR_BUDDY_TYPE);
// 		break;
// 	default:
// 		KGC_Error("byType=" << (INT)byType);
// 	    return FALSE;
// 	}
// 
// 	KLogService::GetInstance()->Goddess_PlayerLogV(pAppPlayer, emKPLAYERLOG_TYPE_REALTION, KD_STR_PLAYERLOG_APPLYDELETE, 
// 		pTargetPlayer->GetPlayerName(), szRelationType);
// 
// 	KLogService::GetInstance()->Goddess_PlayerLogV(pTargetPlayer, emKPLAYERLOG_TYPE_REALTION, KD_STR_PLAYERLOG_TARGETDELETE,
// 		pAppPlayer->GetPlayerName(), szRelationType);
	return FALSE;
}
