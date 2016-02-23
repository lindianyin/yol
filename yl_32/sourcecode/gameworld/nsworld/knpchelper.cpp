
#include "stdafx.h"
#include "config/kconfigfile.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kcharactermgr.h"
#include "knpchelper.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/koverloadextension.h"

#ifdef GAME_SERVER
#include "onlinegamemodule/ai/kaictrl.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LERP(L, R, V) (FLOAT)(V - L) / (FLOAT)(R - L);

KNpcHelper g_cNpcHelper;

BOOL DoNpcInit(KNpc& rcNpc)
{
	return g_cNpcHelper.LoadNpcTemplate(rcNpc);
}
// -------------------------------------------------------------------------
BOOL KNpcHelper::Init()
{
	KNSBASE_EXT::DoNpcInit = DoNpcInit;

	return ReadNpcTemplate();
}

BOOL KNpcHelper::ReadNpcTemplate()
{
	BOOL bResult = FALSE;
	QTabFile cTabFile;

	bResult = cTabFile.Load(KDF_NPC_LIST);
	QCONFIRM_RET_FALSE(bResult);

	INT nLines = cTabFile.GetHeight();

	// 第一行是中英文字段头描述
	for (INT n = 2; n <= nLines; ++n)
	{
		INT nTemplateId = 0;
		KNS_NPC_TEMPLATE sNpcInfo;

		bResult = cTabFile.GetInteger(n, "TemplateId", KD_BAD_ID, &nTemplateId);
		QCONFIRM_RET_FALSE(bResult);

		bResult = cTabFile.GetInteger(n, "AIType", 0, &sNpcInfo.nAIType);

		bResult = cTabFile.GetInteger(n, "Camp", 0, &sNpcInfo.nCamp);

		bResult = cTabFile.GetInteger(n, "PropId", 0, &sNpcInfo.nPropId);

		QCONFIRM_RET_FALSE(m_mapNpcTemplate.insert(std::make_pair(nTemplateId, sNpcInfo)).second);
	}

	bResult = ReadNpcIntensityTable();

	return bResult;
}

INT KNpcHelper::GetMatchLevel(DWORD dwNpcPropId, INT nLevel)
{
	INT nCount = 0;
	const NPC_INTENSITY* pIntensity = NULL;
	std::map<DWORD, std::map<DWORD, NPC_INTENSITY> >::iterator it;
	std::map<DWORD, NPC_INTENSITY>::iterator itIntensity;
	std::map<DWORD, NPC_INTENSITY>* pMapIntensity = NULL;

	COM_PROCESS_ERROR(nLevel > 0);
	it = m_mapNpcIntensityTable.find(dwNpcPropId);
	if (it == m_mapNpcIntensityTable.end())
	{
		QLogPrintf(LOG_ERR, "[%s] Can't find NpcPropId %d", __FUNCTION__, dwNpcPropId);
		goto EXIT0;
	}

	pMapIntensity = &it->second;
	COM_PROCESS_ERROR(pMapIntensity->size() >= 1);

	// 根据人物等级随机选取上下2级的Npc
	//nLevel = nLevel - 2 + KSysService::Rand(4);

	for (itIntensity = pMapIntensity->begin(); itIntensity != pMapIntensity->end(); ++itIntensity)
	{
		++nCount;
		pIntensity = &itIntensity->second;
		/*if(nCount <= 1 && nLevel < pIntensity->nLevel)
			return pIntensity->nLevel;*/
		if(nLevel <= pIntensity->nLevel)
			return pIntensity->nLevel;
		if(nCount == pMapIntensity->size())
			return pIntensity->nLevel;
	}

EXIT0:
	return 0;
}

const NPC_INTENSITY* KNpcHelper::GetNpcIntensity(DWORD dwNpcPropId, INT nLevel)
{
	INT nIntensitySize = 0;
	INT nTmp = 0;
	INT nSumRate = 0;
	DOUBLE* arrRate = NULL;

	DOUBLE dRandValue = 0;
	INT nIdx = 0;

	const NPC_INTENSITY* pIntensity = NULL;
	std::map<DWORD, std::map<DWORD, NPC_INTENSITY> >::iterator it;
	std::map<DWORD, NPC_INTENSITY>::iterator itIntensity;
	std::map<DWORD, NPC_INTENSITY>* pMapIntensity = NULL;

	it = m_mapNpcIntensityTable.find(dwNpcPropId);
    if (it == m_mapNpcIntensityTable.end())
	{
		QLogPrintf(LOG_ERR, "[%s] Can't find NpcPropId %d", __FUNCTION__, dwNpcPropId);
		goto EXIT0;
	}

	pMapIntensity = &it->second;
	COM_PROCESS_ERROR(pMapIntensity->size() >= 1);

	if (nLevel > 0)
	{
		itIntensity = pMapIntensity->find(nLevel);
		if(itIntensity == pMapIntensity->end())
		{
			QLogPrintf(LOG_ERR, "[%s] Can't find NpcLevel %d (NpcPropId:%d)", __FUNCTION__, nLevel, dwNpcPropId);
			goto EXIT0;
		}
		pIntensity = &itIntensity->second;
	}
	else
	{
		// 按照比例从propid相同的npcintensity中选一个level不同的
		nIntensitySize = pMapIntensity->size();
		arrRate = new DOUBLE[nIntensitySize];
		COM_PROCESS_ERROR(arrRate);
		for(INT i = 0; i < nIntensitySize; ++i)
		{
			arrRate[i] = 0;
		}
		for (itIntensity = pMapIntensity->begin(); itIntensity != pMapIntensity->end(); ++itIntensity, ++nTmp)
		{
			pIntensity = &itIntensity->second;
			nSumRate += pIntensity->nLevelRate;
		}
		nTmp = 0;
		for (itIntensity = pMapIntensity->begin(); itIntensity != pMapIntensity->end(); ++itIntensity, ++nTmp)
		{
			pIntensity = &itIntensity->second;
			if(!nTmp) 
				arrRate[nTmp] = DOUBLE(pIntensity->nLevelRate) / (DOUBLE)nSumRate;
			else 
				arrRate[nTmp] = arrRate[nTmp-1] + (DOUBLE)pIntensity->nLevelRate / (DOUBLE)nSumRate;
		}
		arrRate[nIntensitySize-1] = 1.0;

		dRandValue = KSysService::Random();
		nIdx = 0;
		for(; nIdx < nIntensitySize; ++nIdx)
		{
			if(dRandValue <= arrRate[nIdx]) break;
		}
		SAFE_DELETE_ARRAY(arrRate);
		nTmp = 0;
		for (itIntensity = pMapIntensity->begin(); itIntensity != pMapIntensity->end(); ++itIntensity, ++nTmp)
		{
			if(nTmp < nIdx) continue;
			pIntensity = &itIntensity->second;
			break;
		}
	}

EXIT0:
	return pIntensity;
}

INT KNpcHelper::GetNpcExp(DWORD dwNpcPropId, INT nLevel)
{
	const NPC_INTENSITY* pIntensity = GetNpcIntensity(dwNpcPropId, nLevel);
	QCONFIRM_RET_NULL(pIntensity);
	return pIntensity->nExp;
}

INT KNpcHelper::GetNpcExpType(DWORD dwNpcPropId, INT nLevel)
{
	const NPC_INTENSITY* pIntensity = GetNpcIntensity(dwNpcPropId, nLevel);
	QCONFIRM_RET_NULL(pIntensity);
	return pIntensity->nExpType;
}

BOOL KNpcHelper::ReadNpcIntensityTable()
{
	BOOL bResult = FALSE;
	ITabFile* pFile = g_OpenTabFile(DF_NPC_INTENSITY);
	QCONFIRM_RET_FALSE(pFile);

	DWORD dwNpcID;
	DWORD dwNpcLevel;
	NPC_INTENSITY cIntensity;
	INT nTmp = 0;
	for (INT n = 2; n <= pFile->GetHeight(); ++n)
	{
		pFile->GetInteger(n, "ID", 0, &nTmp);
		dwNpcID = (DWORD)nTmp;
		pFile->GetInteger(n, "Level", 0, &nTmp);
		dwNpcLevel = (DWORD)nTmp;
		cIntensity.nLevel = dwNpcLevel;
		pFile->GetInteger(n, "LevelRate", 0, &cIntensity.nLevelRate);
        pFile->GetInteger(n, "Life", 0, &cIntensity.nNpcHp);
		pFile->GetInteger(n, "LifeReplenish", 0, &cIntensity.nNpcHpReplenish);
		pFile->GetInteger(n, "Mana", 0, &cIntensity.nMana);
		pFile->GetInteger(n, "ManaReplenish", 0, &cIntensity.nManaReplenish);
		pFile->GetInteger(n, "Exp", 0, &cIntensity.nExp);
		pFile->GetInteger(n, "ExpType", 0, &cIntensity.nExpType);
		pFile->GetInteger(n, "Hit", 0, &cIntensity.nHit);
		pFile->GetInteger(n, "Duck", 0, &cIntensity.nDuck);
		pFile->GetInteger(n, "Armor", 0, &cIntensity.nArmor);
		pFile->GetInteger(n, "Attack", 0, &cIntensity.nAttack);
		pFile->GetInteger(n, "Critical", 0, &cIntensity.nCritical);
		pFile->GetInteger(n, "DeCritical", 0, &cIntensity.nDeCritical);
		pFile->GetInteger(n, "WalkSpeed", 0, &cIntensity.nWalkSpeed);
		pFile->GetInteger(n, "RunSpeed", 0, &cIntensity.nRunSpeed);
		pFile->GetInteger(n, "IgnoreDef", 0, &cIntensity.nIgnoreDef);

		if (m_mapNpcIntensityTable.find(dwNpcID) == m_mapNpcIntensityTable.end())
		{
			m_mapNpcIntensityTable[dwNpcID] = std::map<DWORD, NPC_INTENSITY>();
		}

		std::map<DWORD, NPC_INTENSITY>& mapNpcIntensity = m_mapNpcIntensityTable[dwNpcID];
		mapNpcIntensity[dwNpcLevel] = cIntensity;
	}

	SAFE_RELEASE(pFile);
    return TRUE;
}

// 根据填表的npc能力，设置NPC
BOOL KNpcHelper::ResetNpcIntensity(KNpc& rcNpc, INT nPropId, INT nLevel /*=0*/)
{
	INT nDmg = 0;
	INT nMagicDmg = 0;
	CONST NPC_INTENSITY* pIntensity = NULL;

	pIntensity = GetNpcIntensity(nPropId, nLevel);
	LOG_PROCESS_ERROR(pIntensity);

	rcNpc.SetLevel(pIntensity->nLevel);
	rcNpc.SetMaxLife(pIntensity->nNpcHp);
	rcNpc.SetCurrentLife(pIntensity->nNpcHp);
	rcNpc.RawSetLifeReplenish(pIntensity->nNpcHpReplenish);
	rcNpc.SetMaxMana(pIntensity->nMana);
	rcNpc.SetCurrentMana(pIntensity->nMana);
	rcNpc.RawSetManaReplenish(pIntensity->nManaReplenish);

	rcNpc.SetAttack(pIntensity->nAttack);
	rcNpc.SetHitCoefficient(pIntensity->nHit);
	rcNpc.SetDodge(pIntensity->nDuck);
	rcNpc.SetDef(pIntensity->nArmor);
	rcNpc.SetCriticalAttack(pIntensity->nCritical);
	rcNpc.SetResist(pIntensity->nDeCritical);
	rcNpc.SetIgnoreDef(pIntensity->nIgnoreDef);

	rcNpc.SetModifyDmgP(100);

	rcNpc.SetMoveSpeed(pIntensity->nRunSpeed);

	return TRUE;
EXIT0:
	return FALSE;
}

INT KNpcHelper::GetNpcPropId(INT nTemplateId)
{
	KNS_NPC_TEMPLATE* pNpcTemplate = NULL;
	const NPC_INTENSITY* pIntensity = NULL;

	MP_NPC_TEMPLATE::iterator it = m_mapNpcTemplate.find(nTemplateId);

	if (it == m_mapNpcTemplate.end())
	{
		QLogPrintf(LOG_ERR, "KNpcHelper::LoadNpcTemplate TemplateId[%u] Is Invalid", nTemplateId);
		return 0;
	}

	pNpcTemplate = &it->second;
	return pNpcTemplate->nPropId;
}

BOOL KNpcHelper::LoadNpcTemplate( KNpc& rcNpc )
{
	INT nLevel = 0;
	KNS_NPC_TEMPLATE* pNpcTemplate = NULL;
	const NPC_INTENSITY* pIntensity = NULL;
	INT nDmg = 0;
	INT nMagicDmg = 0;
	KAICtrl* pAICtrl = NULL;

	MP_NPC_TEMPLATE::iterator it = m_mapNpcTemplate.find(rcNpc.GetTemplateId());

	if (it == m_mapNpcTemplate.end())
	{
		QLogPrintf(LOG_ERR, "KNpcHelper::LoadNpcTemplate TemplateId[%u] Is Invalid", rcNpc.GetTemplateId());
		goto EXIT0;
	}

	pNpcTemplate = &it->second;
	LOG_PROCESS_ERROR(pNpcTemplate);

	ResetNpcIntensity(rcNpc, pNpcTemplate->nPropId);

	rcNpc.SetRelation(it->second.nRelation);
    rcNpc.SetCurrentCamp(it->second.nCamp);

#ifdef GAME_SERVER
	if (pNpcTemplate->nAIType != 0)
	{
		pAICtrl = rcNpc.GetCtrl<KAICtrl>();
		pAICtrl->nAIType = it->second.nAIType;

		////所有的NPC先全部加载非战斗AI
		//rcNpc.m_AIVM.Setup(&rcNpc, pAICtrl->nAIType);
	}
#endif

	return TRUE;
EXIT0:
	return FALSE;
}

// -------------------------------------------------------------------------
