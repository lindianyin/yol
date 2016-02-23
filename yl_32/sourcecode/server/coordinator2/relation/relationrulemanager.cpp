/* -------------------------------------------------------------------------
//	文件名		：	relationrulemanager.cpp
//	创建者		：	
//	创建时间	：	
//	功能描述	：	关系规则管理器（管理关系依赖和冲突）
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "relationrulemanager.h"
#include "relationfactorylist.h"


KRelationRuleManager KRelationRuleManager::ms_inst;

KRelationRuleManager::KPOLICY KRelationRuleManager::ms_aryPolicy[emKPLAYERRELATION_TYPE_COUNT] =	// 人际关系策略
{
	// SyncToServerAsMaster SyncToServerAsTarget SyncToClientAsMaster SyncToClientAsTarget NotifyOnlineAsMaster NotifyOnlineAsTarget NotifyDeleteAsMaster NotifyDeleteAsTarget NotifyTargetCreation NotifyDeleteToMaster NotifyDeleteToTarget
	{TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	TRUE,	FALSE},	// 临时好友
	{TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE},	// 黑名单
	{TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	FALSE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE},	// 普通好友
 	{TRUE,	TRUE,	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE},	// 结拜（兄弟、姐妹）
 	{TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	TRUE,	FALSE},	// 仇人
 	{TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE},	// 当前师徒
 	{TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE},	// 出师师徒
 	{TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE},	// 夫妻
 	{TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	TRUE,	FALSE,	TRUE,	TRUE,	TRUE,	TRUE},	// 介绍人
 	{TRUE,	TRUE,	TRUE,	TRUE,	TRUE,	FALSE,	TRUE,	TRUE,	TRUE,	TRUE,	TRUE},	// 指定密友
};

KRelationRuleManager::KRelationRuleManager()
{
	for (INT i = 0; i < emKTYPE_COUNT; i++)
	{
		m_aDelPermission[i] = TRUE;
	}
	// 临时好友
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_TMPFRIEND].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BLACKLIST,	emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_TMPFRIEND].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BINDFRIEND,	emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_TMPFRIEND].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_ENEMEY,		emKMASK_POSSIDE));
	// 黑名单
//	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_BLACKLIST].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_TMPFRIEND,	emKMASK_POSSIDE));
//	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_BLACKLIST].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BINDFRIEND,	emKMASK_POSSIDE));
//	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_BLACKLIST].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_ENEMEY,		emKMASK_POSSIDE));

	// 黑名单的冲突关系
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_BLACKLIST].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_SIBLING,		emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_BLACKLIST].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_TRAINED,		emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_BLACKLIST].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_TRAINING,		emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_BLACKLIST].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_COUPLE,		emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_BLACKLIST].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_INTRODUCTION,	emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_BLACKLIST].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BUDDY,		emKMASK_POSSIDE));
	// 普通好友
	m_aryDeleteDepRules[emKPLAYERRELATION_TYPE_BINDFRIEND].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_SIBLING,		emKMASK_ALLSIDE));
	m_aryDeleteDepRules[emKPLAYERRELATION_TYPE_BINDFRIEND].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_TRAINING,	emKMASK_ALLSIDE));
	m_aryDeleteDepRules[emKPLAYERRELATION_TYPE_BINDFRIEND].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_TRAINED,		emKMASK_ALLSIDE));
	m_aryDeleteDepRules[emKPLAYERRELATION_TYPE_BINDFRIEND].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_COUPLE,		emKMASK_ALLSIDE));
	m_aryDeleteDepRules[emKPLAYERRELATION_TYPE_BINDFRIEND].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_INTRODUCTION,emKMASK_ALLSIDE));
	m_aryDeleteDepRules[emKPLAYERRELATION_TYPE_BINDFRIEND].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BUDDY,		emKMASK_ALLSIDE));
	// 结拜
	m_aryCreateDepRules[emKPLAYERRELATION_TYPE_SIBLING].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BINDFRIEND,		emKMASK_POSSIDE));
	// 仇人
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_ENEMEY].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_TMPFRIEND,		emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_ENEMEY].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BLACKLIST,		emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_ENEMEY].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BINDFRIEND,		emKMASK_POSSIDE));
	m_aryCreateRepRules[emKPLAYERRELATION_TYPE_ENEMEY].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_ENEMEY,			emKMASK_POSSIDE));
	// 师徒
	m_aryCreateDepRules[emKPLAYERRELATION_TYPE_TRAINING].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BINDFRIEND,		emKMASK_POSSIDE));
	m_aryCreateDepRules[emKPLAYERRELATION_TYPE_TRAINED].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BINDFRIEND,		emKMASK_POSSIDE));
	// 夫妻
	m_aryCreateDepRules[emKPLAYERRELATION_TYPE_COUPLE].push_back(KRELATION_RULE(emKPLAYERRELATION_TYPE_BINDFRIEND,		emKMASK_POSSIDE));
}

KRelationRuleManager::~KRelationRuleManager()
{
}

// 检查依赖关系，返回TRUE表示满足依赖关系
BOOL KRelationRuleManager::CheckCreateDependency(INT nType, INT nMaster, INT nTarget)
{
	BOOL					bRetCode	= FALSE;
	KRELATION_VEC*			pRuleList	= NULL;
	KRelationFactoryList*	pFactoryList = NULL;
	IKRelationFactory*		pFactory	= NULL;
	const KRELATION_RULE*	pRule		= NULL;

	LOG_PROCESS_ERROR(nType >= 0 && nType < emKTYPE_COUNT);
	LOG_PROCESS_ERROR(nMaster && nTarget);

	pRuleList = &m_aryCreateDepRules[nType];
	pFactoryList = KRelationFactoryList::Instance();
	for (KRELATION_VEC::const_iterator it = pRuleList->begin(); 
		it != pRuleList->end(); it++)
	{
		pRule = &(*it);
		LOG_PROCESS_ERROR(pRule->nMode != 0);

		bRetCode = pFactoryList->CreateFactory(pRule->nType, &pFactory);
		LOG_PROCESS_ERROR(bRetCode);

		if ((pRule->nMode & 0x1) != 0)
		{
			if (pFactory->FindRelation(nMaster, nTarget))
			{
				bRetCode = FALSE;
				goto EXIT0;
			}
		}
		if ((pRule->nMode & 0x2) != 0)
		{
			if (pFactory->FindRelation(nTarget, nMaster))
			{
				bRetCode = FALSE;
				goto EXIT0;
			}
		}
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

// 检查冲突关系，返回TRUE表示没有冲突
BOOL KRelationRuleManager::CheckCreateRepugnance(INT nType, INT nMaster, INT nTarget)
{
	BOOL					bRetCode = FALSE;
	KRELATION_VEC*			pRuleList = NULL;
	KRelationFactoryList*	pFactoryList = NULL;
	const KRELATION_RULE*	pRule = NULL;
	IKRelationFactory*		pFactory = NULL;

	LOG_PROCESS_ERROR(nType >= 0 && nType < emKTYPE_COUNT);
	LOG_PROCESS_ERROR(nMaster && nTarget);

	pRuleList = &m_aryCreateRepRules[nType];
	pFactoryList = KRelationFactoryList::Instance();
	for (KRELATION_VEC::const_iterator it = pRuleList->begin(); 
		it != pRuleList->end(); it++)
	{
		pRule = &(*it);

		LOG_PROCESS_ERROR(pRule->nMode != 0);
		
		bRetCode = pFactoryList->CreateFactory(pRule->nType, &pFactory);
		LOG_PROCESS_ERROR(bRetCode);

		if ((pRule->nMode & 0x1) != 0)
		{
			if (pFactory->FindRelation(nMaster, nTarget))
			{
				bRetCode = FALSE;
				goto EXIT0;
			}
		}
		if ((pRule->nMode & 0x2) != 0)
		{
			if (pFactory->FindRelation(nTarget, nMaster))
			{
				bRetCode = FALSE;
				goto EXIT0;
			}
		}
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

// 检查删除关系的依赖关系，返回TRUE表示满足依赖关系
BOOL KRelationRuleManager::CheckDeleteDependency(INT nType, INT nMaster, INT nTarget)
{
	BOOL					bRetCode = FALSE;
	KRELATION_VEC*			pRuleList = NULL;
	KRelationFactoryList*	pFactoryList = NULL;
	IKRelationFactory*		pFactory = NULL;
	const KRELATION_RULE*	pRule = NULL;

	LOG_PROCESS_ERROR(nType >= 0 && nType < emKTYPE_COUNT);
	LOG_PROCESS_ERROR(nMaster && nTarget);
	LOG_PROCESS_ERROR(m_aDelPermission[nType]);

	pRuleList = &m_aryDeleteDepRules[nType];
	pFactoryList = KRelationFactoryList::Instance();
	for (KRELATION_VEC::const_iterator it = pRuleList->begin(); 
		it != pRuleList->end(); it++)
	{
		pRule = &(*it);

		LOG_PROCESS_ERROR(pRule->nMode != 0);

		pFactory = NULL;
		bRetCode = pFactoryList->CreateFactory(pRule->nType, &pFactory);
		LOG_PROCESS_ERROR(bRetCode);

		if ((pRule->nMode & 0x1) != 0)
		{
			if (pFactory->FindRelation(nMaster, nTarget))
			{
				bRetCode = FALSE;
				goto EXIT0;
			}
		}
		if ((pRule->nMode & 0x2) != 0)
		{
			if (pFactory->FindRelation(nTarget, nMaster))
			{
				bRetCode = FALSE;
				goto EXIT0;
			}
		}
	}

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

CONST KRelationRuleManager::KPOLICY* KRelationRuleManager::GetPolicy(KEPLAYERRELATION_TYPE emType)
{
	if(emType >= 0 && emType < emKPLAYERRELATION_TYPE_COUNT)
	{
		return &ms_aryPolicy[emType];
	}
	return NULL;
}

BOOL KRelationRuleManager::SetPolicy(KEPLAYERRELATION_TYPE emType, CONST KPOLICY* pPolicy)
{
	if (emType >= 0 && emType < emKPLAYERRELATION_TYPE_COUNT && pPolicy)
	{
		ms_aryPolicy[emType] = *pPolicy;
		return TRUE;
	}
	return FALSE;
}
