/* -------------------------------------------------------------------------
//	文件名		：	relationfactorylist.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	玩家关系工厂集合
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "relationfactorylist.h"
#include "relationmanager.h"
#include "onlinegameworld/kplayermgr.h"

KRelationFactoryList KRelationFactoryList::ms_inst;

class KRelationHelper : public IKRelationHelper
{
	virtual BOOL CheckCanLoad(INT nPlayerId)
	{
		KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
		if (pRole == NULL)
			return FALSE;

		return TRUE;

// 		KGC_Player* pPlayer = g_pPlayerManager->GetPlayer(nPlayerId);
// 		if (!pPlayer)
// 			return TRUE;
// 		// 若对方关系已加载，则不需再加载该关系数据
//  		return TRUE;
	}

	virtual BOOL CheckCanUnload(INT nPlayerId)
	{
		KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
		if (!pRole)
		{
			return TRUE;
		}

		return pRole->IsOnline() ? FALSE : TRUE;

// 		KGC_Player* pPlayer = g_pPlayerManager->GetPlayer(nPlayerId);
// 		if (!pPlayer)
// 			return TRUE;
// 		return pPlayer->GetRelayLoaded() == 0;
	}

	virtual BOOL Dirty(INT nPlayerId1, INT nPlayerId2)
	{
// 		KGC_CONFIRM_RET_FALSE(nPlayerId1);
// 		g_cPlayerSaveLoadManager.AddRelayDirty(nPlayerId1);
// 		if (nPlayerId2 > 0)
// 			g_cPlayerSaveLoadManager.AddRelayDirty(nPlayerId2);
		return TRUE;
	}
};

KRelationFactoryList::KRelationFactoryList() :
	m_pHelper(NULL),
 	m_factoryBlackList(emKPLAYERRELATION_TYPE_BLACKLIST),
 	m_factorySibling(emKPLAYERRELATION_TYPE_SIBLING),
 	m_factoryTrained(emKPLAYERRELATION_TYPE_TRAINED)
{
	m_aryFactories[emKPLAYERRELATION_TYPE_TMPFRIEND		] = &m_factoryTmpFriend;		// 临时好友
	m_aryFactories[emKPLAYERRELATION_TYPE_BLACKLIST		] = &m_factoryBlackList;		// 黑名单
	m_aryFactories[emKPLAYERRELATION_TYPE_BINDFRIEND	] = &m_factoryBidFriend;		// 普通好友
	m_aryFactories[emKPLAYERRELATION_TYPE_SIBLING		] = &m_factorySibling;			// 结拜
	m_aryFactories[emKPLAYERRELATION_TYPE_TRAINING		] = &m_factoryTraining;			// 师徒（未出师）：单独处理
	m_aryFactories[emKPLAYERRELATION_TYPE_TRAINED		] = &m_factoryTrained;			// 师徒（已出师）
	m_aryFactories[emKPLAYERRELATION_TYPE_ENEMEY		] = &m_factoryEnemy;			// 仇敌
	m_aryFactories[emKPLAYERRELATION_TYPE_COUPLE		] = &m_factoryCouple;			// 夫妻
	m_aryFactories[emKPLAYERRELATION_TYPE_INTRODUCTION	] = &m_factoryIntroduction;		// 介绍人
	m_aryFactories[emKPLAYERRELATION_TYPE_BUDDY			] = &m_factoryBuddy;			// 指定密友

	m_pHelper = new KRelationHelper;
	// KRelationManager_GC* pRelationManager = KRelationManager_GC::Instance();
	m_factoryTmpFriend.Init(m_pHelper);
	m_factoryBlackList.Init(m_pHelper);
	m_factoryBidFriend.Init(m_pHelper);
	m_factorySibling.Init(m_pHelper);
	m_factoryTraining.Init(m_pHelper);
	m_factoryTrained.Init(m_pHelper);
	m_factoryEnemy.Init(m_pHelper);
	m_factoryCouple.Init(m_pHelper);
	m_factoryIntroduction.Init(m_pHelper);
	m_factoryBuddy.Init(m_pHelper);
}

KRelationFactoryList::~KRelationFactoryList()
{
	ASSERT(m_pHelper);
	delete m_pHelper;
	m_pHelper = NULL;
}

BOOL KRelationFactoryList::CreateFactory(INT nType, IKRelationFactory **ppFactory)
{
	BOOL bRetCode = FALSE;

	LOG_PROCESS_ERROR(ppFactory);

	if (nType >= 0 && nType < emKPLAYERRELATION_TYPE_COUNT)
	{
		*ppFactory = ms_inst.m_aryFactories[nType];
		LOG_PROCESS_ERROR(*ppFactory != NULL);
	}
	
	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}
