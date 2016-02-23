/* -------------------------------------------------------------------------
//	文件名		：	factionmanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-25 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "factionmanager.h"
#include "onlinegamemodule/playerfaction.h"
#include "onlinegamemodule/characterskill.h"
#include "onlinegameworld/kcharactermgr.h"
#include "onlinegameworld/kcharacter.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "onlinegameworld/kplayermgr.h"
#include "award/kawardsetting.h"
#include "../arena/playerarena.h"

FactionManager g_cFactionManager;

#define KD_FACTION_SKILL_LIST_TABLE  "\\setting\\faction\\skilllist.txt"
#define KD_FACTION_ABILITY_TABLE     "\\setting\\faction\\ability.txt"
#define MODULE_NAME "diagrams_level"

FactionManager::FactionManager()
{

}

FactionManager::~FactionManager()
{

}
BOOL FactionManager::Init()
{
	INT nRowNum = 0;
	// 读取门派技能表
    ITabFile* pTabFile = NULL;//g_OpenTabFile(KD_FACTION_SKILL_LIST_TABLE);
	//LOG_PROCESS_ERROR(pTabFile);

	// 读取门派成长数值表
	pTabFile = g_OpenTabFile(KD_FACTION_ABILITY_TABLE);
	LOG_PROCESS_ERROR(pTabFile);

	nRowNum = pTabFile->GetHeight();
	for (INT i = 3; i < nRowNum; i++)
	{
		INT  nFactionId = 0, nRouteId = 0;
		pTabFile->GetInteger(i, "FactionId", 0, &nFactionId);
		pTabFile->GetInteger(i, "RouteId", 0, &nRouteId);

		INT nEnumId = (nFactionId << 16) | nRouteId; // 联合id
		if (m_cFactionData.mapFactionGrowthData.find(nEnumId) == m_cFactionData.mapFactionGrowthData.end())
		{
			m_cFactionData.mapFactionGrowthData.insert(
				std::pair<INT, std::vector<CHARACTER_BASE_ATTRIBUTE*> >(nEnumId, std::vector<CHARACTER_BASE_ATTRIBUTE*>())
			);
			// m_cFactionData.mapFactionGrowthData[nEnumId] = std::vector<CHARACTER_BASE_ATTRIBUTE*>();
		}

		std::vector<CHARACTER_BASE_ATTRIBUTE*>& arrFactionAbility = m_cFactionData.mapFactionGrowthData[nEnumId];

		CHARACTER_BASE_ATTRIBUTE* pBaseAttribute = new CHARACTER_BASE_ATTRIBUTE;
		LOG_PROCESS_ERROR(pBaseAttribute);

		memset(pBaseAttribute, 0, sizeof(CHARACTER_BASE_ATTRIBUTE));

		pTabFile->GetInteger(i, "Level", 0, &pBaseAttribute->nLevel);
		pTabFile->GetInteger(i, "Con", 0, &pBaseAttribute->nConstitution);
		pTabFile->GetInteger(i, "Str", 0, &pBaseAttribute->nStrength);
		pTabFile->GetInteger(i, "Agi", 0, &pBaseAttribute->nAgile);
		pTabFile->GetInteger(i, "Int", 0, &pBaseAttribute->nIntelligence);
		pTabFile->GetInteger(i, "Spi", 0, &pBaseAttribute->nSpirit);

		arrFactionAbility.push_back(pBaseAttribute);
	}

    return TRUE;
EXIT0:
	return FALSE;
}

BOOL FactionManager::Breath()
{
	return TRUE;
}

CHARACTER_BASE_ATTRIBUTE* FactionManager::GetCharacterAttribute(INT nFaction, INT nRoute, INT nLevel)
{
	INT nEnumId = (nFaction << 16) | nRoute;
	std::map<INT, std::vector<CHARACTER_BASE_ATTRIBUTE*> >::iterator it = m_cFactionData.mapFactionGrowthData.find(nEnumId);
	if (it != m_cFactionData.mapFactionGrowthData.end())
	{
		std::vector<CHARACTER_BASE_ATTRIBUTE*>& arrFactionAbility = it->second;

		LOG_PROCESS_ERROR(nLevel <= (INT)m_cFactionData.mapFactionGrowthData[nEnumId].size() && nLevel > 0);

		return arrFactionAbility[nLevel - 1]; // 索引从0开始，而等级从1开始
	}

EXIT0:
	return NULL;
}

// 得到基础生命上限
INT FactionManager::GetBaseMaxLife(KCharacter& rcCharacter)
{
	INT nBaseMaxLife = 0;
	INT nFaction = rcCharacter.GetPrimaryFaction();
	INT nRoute = rcCharacter.GetCurrentRoute();
	INT nLevel = rcCharacter.GetLevel();
	CHARACTER_BASE_ATTRIBUTE* pBaseAttribute = NULL;

	pBaseAttribute = GetCharacterAttribute(nFaction, nRoute, nLevel);
	LOG_PROCESS_ERROR(pBaseAttribute);

	// 基础生命上限 = 体质值（读表）* 5 + 490
	nBaseMaxLife = pBaseAttribute->nConstitution * 5 + 490;

	return nBaseMaxLife;

EXIT0:
	return 0;
}

// 得到基础真气上限
INT FactionManager::GetBaseMaxMana(KCharacter& rcCharacter)
{
	INT nBaseMaxMana = 0;
	INT nFaction = rcCharacter.GetPrimaryFaction();
	INT nRoute = rcCharacter.GetCurrentRoute();
	INT nLevel = rcCharacter.GetLevel();
	CHARACTER_BASE_ATTRIBUTE* pBaseAttribute = NULL;

	pBaseAttribute = GetCharacterAttribute(nFaction, nRoute, nLevel);
	LOG_PROCESS_ERROR(pBaseAttribute);

	// 基础真气上限 = 元气值（读表）* 5 + 90
	nBaseMaxMana = pBaseAttribute->nIntelligence * 5 + 90;

	return nBaseMaxMana;

EXIT0:
	return 0;
}

// 得到基础集中值上限
INT FactionManager::GetBaseMaxPower(KCharacter& rcCharacter)
{
	// 固定100点
	return 100;
}

// 最大血量
BOOL FactionManager::SetMaxLife( KCharacter& rcCharacter, DWORD nOldValue, DWORD nNewValue )
{
	INT nDelta = nNewValue - nOldValue;

	PROCESS_ERROR(nDelta != 0);

	if(IS_PLAYER(rcCharacter.GetId()))
	{
		KPlayer* pPlayer = (KPlayer*)&rcCharacter;
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunction("BufferMagicEffect", "Add_MaxLifeEx", 2, "od", pPlayer->GetScriptInterface(), nDelta);
	}

EXIT0:
	return TRUE;
}

BOOL FactionManager::SetLevel( KCharacter& rcCharacter, WORD wOldValue, WORD wNewValue )
{
	//QLogPrintf(LOG_INFO,"..FactionManager::SetLevel..");
	BOOL bRetCode = FALSE;
	PlayerFaction* playerFaction = NULL;
	PROCESS_ERROR(rcCharacter.GetLevel() && wOldValue != wNewValue);

	playerFaction = rcCharacter.GetCtrl<PlayerFaction>();
	LOG_PROCESS_ERROR(playerFaction);
	playerFaction->OnFactionRouteChange(rcCharacter.GetLevel(), rcCharacter.GetPrimaryFaction(), rcCharacter.GetCurrentRoute());

	playerFaction->OnLevelChange(wOldValue, wNewValue);

	if (wOldValue != 0 && wOldValue != wNewValue && wNewValue == ARENA_OPEN_LEVEL)
	{
		// 按战斗力加入竞技场排行榜
		PlayerArena* pPlayerArena = rcCharacter.GetCtrl<PlayerArena>();
		LOG_PROCESS_ERROR(pPlayerArena);
		pPlayerArena->AddArenaRank();
	}

	g_cCharacterHelper.CalcFightScore(rcCharacter);		// 这句话和上面的AddArenaRank顺序不能变，涉及到排名顺序问题

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL FactionManager::SetRoute( KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue )
{
	BOOL bRetCode = FALSE;
	if (byOldValue != byNewValue 
		&& rcCharacter.GetLevel() 
		&& rcCharacter.GetPrimaryFaction()) // 等级为0说明还未初始化
	{
		// 清空角色技能
		CharacterSkill* characterSkill = rcCharacter.GetCtrl<CharacterSkill>();
		LOG_PROCESS_ERROR(characterSkill);

		//characterSkill->ClearSkill();

		PlayerFaction* playerFaction = rcCharacter.GetCtrl<PlayerFaction>();
		LOG_PROCESS_ERROR(playerFaction);

		playerFaction->OnFactionRouteChange(rcCharacter.GetLevel(), rcCharacter.GetPrimaryFaction(), byNewValue);
	}
	
	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}


BOOL FactionManager::SplitString(const std::string& src, const std::string &separator, std::vector<string> &dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator,start);
		if (index != string::npos)
		{    
			substring = str.substr(start,index-start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator,index);
			if (start == string::npos) return TRUE;
		}
	}while(index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);

	return TRUE;
}

// 乾卦：攻击
BOOL FactionManager::SetEightTrigramLevel1( KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel /*= 0*/ )
{
	INT nCurTaiChiLevel = rcCharacter.GetEightTrigramLevelTaiChi();
	INT nOldTaiChiLevel = (nCurTaiChiLevel > nDeltaAllLevel) ? (nCurTaiChiLevel - nDeltaAllLevel) : 0;

	// 加0.5取整是四舍五入，策划需求
	//INT nOld = 13 * byOldValue * (1 + (DOUBLE)nOldTaiChiLevel / 100) + 0.5;
	//INT nNew = 13 * byNewValue * (1 + (DOUBLE)nCurTaiChiLevel / 100) + 0.5;
	INT nOld = 0;
	INT nNew = 0;

	INT nOldLevel = (byOldValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_1;
	CONST QNumericalAward* pAssetAwardOld = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nOldLevel);
	if (pAssetAwardOld != NULL)
	{
		INT nTaiChiLevel = (nOldTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nAttack;
		nOld = pAssetAwardOld->nAttack + dTaiChi;
	}

	INT nNewLevel = (byNewValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_1;
	CONST QNumericalAward* pAssetAwardNew = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nNewLevel);
	if (pAssetAwardNew != NULL)
	{
		INT nTaiChiLevel = (nCurTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nAttack;
		nNew = pAssetAwardNew->nAttack + dTaiChi;
	}

	INT nDelta = nNew - nOld;

	if (nDelta != 0)
	{
		rcCharacter.SetAttack(rcCharacter.GetAttack() + nDelta);
	}

	if (nDeltaAllLevel == 0)
		g_cCharacterHelper.CalcFightScore(rcCharacter);

	return TRUE;
}

// 兑卦：防御
BOOL FactionManager::SetEightTrigramLevel2( KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel /*= 0*/ )
{
	INT nCurTaiChiLevel = rcCharacter.GetEightTrigramLevelTaiChi();
	INT nOldTaiChiLevel = (nCurTaiChiLevel > nDeltaAllLevel) ? (nCurTaiChiLevel - nDeltaAllLevel) : 0;

	//INT nOld = 2 * byOldValue * (1 + (DOUBLE)nOldTaiChiLevel / 100) + 0.5;
	//INT nNew = 2 * byNewValue * (1 + (DOUBLE)nCurTaiChiLevel / 100) + 0.5;
	INT nOld = 0;
	INT nNew = 0;
	INT nOldLevel = (byOldValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_2;
	INT nNewLevel = (byNewValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_2;
	CONST QNumericalAward* pAssetAwardOld = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nOldLevel);
	if (pAssetAwardOld != NULL)
	{
		INT nTaiChiLevel = (nOldTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nDefense;
		nOld = pAssetAwardOld->nDefense + dTaiChi;
	}

	CONST QNumericalAward* pAssetAwardNew = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nNewLevel);
	if (pAssetAwardNew != NULL)
	{
		INT nTaiChiLevel = (nCurTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nDefense;
		nNew = pAssetAwardNew->nDefense + dTaiChi;
	}
	INT nDelta = nNew - nOld;
	
	if (nDelta != 0)
	{
		// rcCharacter.SetModifyDmgP(rcCharacter.GetModifyDmgP() + nDelta);
		rcCharacter.SetDef(rcCharacter.GetDef() + nDelta);
	}

	if (nDeltaAllLevel == 0)
		g_cCharacterHelper.CalcFightScore(rcCharacter);

	return TRUE;
}

// 离卦：生命
BOOL FactionManager::SetEightTrigramLevel3( KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel /*= 0*/ )
{
	INT nCurTaiChiLevel = rcCharacter.GetEightTrigramLevelTaiChi();
	INT nOldTaiChiLevel = (nCurTaiChiLevel > nDeltaAllLevel) ? (nCurTaiChiLevel - nDeltaAllLevel) : 0;

	//INT nOld = 1 * byOldValue * (1 + (DOUBLE)nOldTaiChiLevel / 100) + 0.5;
	//INT nNew = 1 * byNewValue * (1 + (DOUBLE)nCurTaiChiLevel / 100) + 0.5;
	INT nOld = 0;
	INT nNew = 0;
	INT nOldLevel = (byOldValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_3;
	INT nNewLevel = (byNewValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_3;
	CONST QNumericalAward* pAssetAwardOld = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nOldLevel);
	if (pAssetAwardOld != NULL)
	{
		INT nTaiChiLevel = (nOldTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nLife;
		nOld = pAssetAwardOld->nLife + dTaiChi;
	}

	CONST QNumericalAward* pAssetAwardNew = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nNewLevel);
	if (pAssetAwardNew != NULL)
	{
		INT nTaiChiLevel = (nCurTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nLife;
		nNew = pAssetAwardNew->nLife + dTaiChi;
	}
	INT nDelta = nNew - nOld;

	if (nDelta != 0)
	{
		rcCharacter.SetMaxLife(rcCharacter.GetMaxLife() + nDelta);
		//rcCharacter.SetCriticalAttack(rcCharacter.GetCriticalAttack() + nDelta);
	}

	if (nDeltaAllLevel == 0)
		g_cCharacterHelper.CalcFightScore(rcCharacter);

	return TRUE;
}

// 震卦：暴击
BOOL FactionManager::SetEightTrigramLevel4( KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel /*= 0*/ )
{
	INT nCurTaiChiLevel = rcCharacter.GetEightTrigramLevelTaiChi();
	INT nOldTaiChiLevel = (nCurTaiChiLevel > nDeltaAllLevel) ? (nCurTaiChiLevel - nDeltaAllLevel) : 0;

	//INT nOld = 5 * byOldValue * (1 + (DOUBLE)nOldTaiChiLevel / 100) + 0.5;
	//INT nNew = 5 * byNewValue * (1 + (DOUBLE)nCurTaiChiLevel/ 100) + 0.5;
	INT nOld = 0;
	INT nNew = 0;
	INT nOldLevel = (byOldValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_4;
	INT nNewLevel = (byNewValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_4;
	CONST QNumericalAward* pAssetAwardOld = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nOldLevel);
	if (pAssetAwardOld != NULL)
	{
		INT nTaiChiLevel = (nOldTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nCrit;
		nOld = pAssetAwardOld->nCrit + dTaiChi;
	}

	CONST QNumericalAward* pAssetAwardNew = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nNewLevel);
	if (pAssetAwardNew != NULL)
	{
		INT nTaiChiLevel = (nCurTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nCrit;
		nNew = pAssetAwardNew->nCrit + dTaiChi;
	}
	INT nDelta = nNew - nOld;

	if (nDelta != 0)
	{
		rcCharacter.SetCriticalValue(rcCharacter.GetCriticalValue() + nDelta);
	}

	if (nDeltaAllLevel == 0)
		g_cCharacterHelper.CalcFightScore(rcCharacter);

	return TRUE;
}

// 坤卦：抗暴
BOOL FactionManager::SetEightTrigramLevel5( KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel /*= 0*/ )
{
	INT nCurTaiChiLevel = rcCharacter.GetEightTrigramLevelTaiChi();
	INT nOldTaiChiLevel = (nCurTaiChiLevel > nDeltaAllLevel) ? (nCurTaiChiLevel - nDeltaAllLevel) : 0;

	//INT nOld = 13 * byOldValue * (1 + (DOUBLE)nOldTaiChiLevel / 100) + 0.5;
	//INT nNew = 13 * byNewValue * (1 + (DOUBLE)nCurTaiChiLevel / 100) + 0.5;
	INT nOld = 0;
	INT nNew = 0;
	INT nOldLevel = (byOldValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_5;
	INT nNewLevel = (byNewValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_5;
	CONST QNumericalAward* pAssetAwardOld = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nOldLevel);
	if (pAssetAwardOld != NULL)
	{
		INT nTaiChiLevel = (nOldTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nResist;
		nOld = pAssetAwardOld->nResist + dTaiChi;
	}

	CONST QNumericalAward* pAssetAwardNew = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nNewLevel);
	if (pAssetAwardNew != NULL)
	{
		INT nTaiChiLevel = (nCurTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nResist;
		nNew = pAssetAwardNew->nResist + dTaiChi;
	}
	INT nDelta = nNew - nOld;

	if (nDelta != 0)
	{
		rcCharacter.SetResist(rcCharacter.GetResist() + nDelta);
	}

	if (nDeltaAllLevel == 0)
		g_cCharacterHelper.CalcFightScore(rcCharacter);

	return TRUE;
}

// 艮卦：命中
BOOL FactionManager::SetEightTrigramLevel6( KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel /*= 0*/ )
{
	INT nCurTaiChiLevel = rcCharacter.GetEightTrigramLevelTaiChi();
	INT nOldTaiChiLevel = (nCurTaiChiLevel > nDeltaAllLevel) ? (nCurTaiChiLevel - nDeltaAllLevel) : 0;

	//INT nOld = 24 * byOldValue * (1 + (DOUBLE)nOldTaiChiLevel / 100) + 0.5;
	//INT nNew = 24 * byNewValue * (1 + (DOUBLE)nCurTaiChiLevel / 100) + 0.5;
	INT nOld = 0;
	INT nNew = 0;
	INT nOldLevel = (byOldValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_6;
	INT nNewLevel = (byNewValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_6;
	CONST QNumericalAward* pAssetAwardOld = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nOldLevel);
	if (pAssetAwardOld != NULL)
	{
		INT nTaiChiLevel = (nOldTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nHit;
		nOld = pAssetAwardOld->nHit + dTaiChi;
	}

	CONST QNumericalAward* pAssetAwardNew = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nNewLevel);
	if (pAssetAwardNew != NULL)
	{
		INT nTaiChiLevel = (nCurTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nHit;
		nNew = pAssetAwardNew->nHit + dTaiChi;
	}
	INT nDelta = nNew - nOld;

	if (nDelta != 0)
	{
		rcCharacter.SetHitCoefficient(rcCharacter.GetHitCoefficient() + nDelta);
		//rcCharacter.SetDef(rcCharacter.GetDef() + nDelta);
	}

	if (nDeltaAllLevel == 0)
		g_cCharacterHelper.CalcFightScore(rcCharacter);

	return TRUE;
}

// 坎卦：闪避
BOOL FactionManager::SetEightTrigramLevel7( KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel /*= 0*/ )
{
	INT nCurTaiChiLevel = rcCharacter.GetEightTrigramLevelTaiChi();
	INT nOldTaiChiLevel = (nCurTaiChiLevel > nDeltaAllLevel) ? (nCurTaiChiLevel - nDeltaAllLevel) : 0;

	INT nOld = 0;//50 * byOldValue * (1 + (DOUBLE)nOldTaiChiLevel / 100) + 0.5;
	INT nNew = 0;//50 * byNewValue * (1 + (DOUBLE)nCurTaiChiLevel / 100) + 0.5;
	INT nOldLevel = (byOldValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_7;
	INT nNewLevel = (byNewValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_7;
	CONST QNumericalAward* pAssetAwardOld = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nOldLevel);
	if (pAssetAwardOld != NULL)
	{
		INT nTaiChiLevel = (nOldTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nDodge;
		nOld = pAssetAwardOld->nDodge + dTaiChi;
	}

	CONST QNumericalAward* pAssetAwardNew = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nNewLevel);
	if (pAssetAwardNew != NULL)
	{
		INT nTaiChiLevel = (nCurTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nDodge;
		nNew = pAssetAwardNew->nDodge + dTaiChi;
	}
	INT nDelta = nNew - nOld;

	if (nDelta != 0)
	{
		rcCharacter.SetDodge(rcCharacter.GetDodge() + nDelta);
		//rcCharacter.SetMaxLife(rcCharacter.GetMaxLife() + nDelta);
	}

	if (nDeltaAllLevel == 0)
		g_cCharacterHelper.CalcFightScore(rcCharacter);

	return TRUE;
}

// 巽卦：生命
BOOL FactionManager::SetEightTrigramLevel8( KCharacter& rcCharacter, BYTE byOldValue, BYTE byNewValue, INT nDeltaAllLevel /*= 0*/ )
{
	INT nCurTaiChiLevel = rcCharacter.GetEightTrigramLevelTaiChi();
	INT nOldTaiChiLevel = (nCurTaiChiLevel > nDeltaAllLevel) ? (nCurTaiChiLevel - nDeltaAllLevel) : 0;

	//INT nOld = 0.1 * byOldValue * (1 + (DOUBLE)nOldTaiChiLevel / 100) + 0.5;
	//INT nNew = 0.1 * byNewValue * (1 + (DOUBLE)nCurTaiChiLevel / 100) + 0.5;
	INT nOld = 0;
	INT nNew = 0;
	INT nOldLevel = (byOldValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_8;
	INT nNewLevel = (byNewValue - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_8;
	CONST QNumericalAward* pAssetAwardOld = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nOldLevel);
	if (pAssetAwardOld != NULL)
	{
		INT nTaiChiLevel = (nOldTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nLife;
		nOld = pAssetAwardOld->nLife + dTaiChi;
	}

	CONST QNumericalAward* pAssetAwardNew = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nNewLevel);
	if (pAssetAwardNew != NULL)
	{
		INT nTaiChiLevel = (nCurTaiChiLevel - 1) * EIGTH_TRIGRAM_TaiChi + EIGTH_TRIGRAM_TaiChi;
		CONST QNumericalAward* pAssetAwardTaiChi = g_cAwardSetting.RequestNumericalAward(MODULE_NAME, 1, nTaiChiLevel);
		INT dTaiChi = pAssetAwardTaiChi == NULL ? 0 : pAssetAwardTaiChi->nLife;
		nNew = pAssetAwardNew->nLife + dTaiChi;
	}
	INT nDelta = nNew - nOld;

	if (nDelta != 0)
	{
		rcCharacter.SetMaxLife(rcCharacter.GetMaxLife() + nDelta);
	}

	if (nDeltaAllLevel == 0)
		g_cCharacterHelper.CalcFightScore(rcCharacter);

	return TRUE;
}

// 天地二仪放大其他卦加成效果，每级放大1%
BOOL FactionManager::SetEightTrigramLevelTaiChi( KCharacter& rcCharacter, BYTE byOldValue, BYTE nNewValue )
{
	// 触发各个卦象重新计算
	INT nDelta = nNewValue - byOldValue;
	if (nDelta != 0)
	{
		SetEightTrigramLevel1(rcCharacter, rcCharacter.GetEightTrigramLevel1(), rcCharacter.GetEightTrigramLevel1(), nNewValue - byOldValue);
		SetEightTrigramLevel2(rcCharacter, rcCharacter.GetEightTrigramLevel2(), rcCharacter.GetEightTrigramLevel2(), nNewValue - byOldValue);
		SetEightTrigramLevel3(rcCharacter, rcCharacter.GetEightTrigramLevel3(), rcCharacter.GetEightTrigramLevel3(), nNewValue - byOldValue);
		SetEightTrigramLevel4(rcCharacter, rcCharacter.GetEightTrigramLevel4(), rcCharacter.GetEightTrigramLevel4(), nNewValue - byOldValue);
		SetEightTrigramLevel5(rcCharacter, rcCharacter.GetEightTrigramLevel5(), rcCharacter.GetEightTrigramLevel5(), nNewValue - byOldValue);
		SetEightTrigramLevel6(rcCharacter, rcCharacter.GetEightTrigramLevel6(), rcCharacter.GetEightTrigramLevel6(), nNewValue - byOldValue);
		SetEightTrigramLevel7(rcCharacter, rcCharacter.GetEightTrigramLevel7(), rcCharacter.GetEightTrigramLevel7(), nNewValue - byOldValue);
		//SetEightTrigramLevel8(rcCharacter, rcCharacter.GetEightTrigramLevel8(), rcCharacter.GetEightTrigramLevel8(), nNewValue - byOldValue);

		g_cCharacterHelper.CalcFightScore(rcCharacter);
	}
	return TRUE;
}
// 设置快捷药槽！0是红，1是蓝
BOOL FactionManager::SetMeidicineSlot(DWORD nPlayerIndex, INT nSlot, INT nRoom, INT nX, INT nY)
{
	PlayerFaction* playerFaction;
	KPlayer *pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	playerFaction = pPlayer->GetCtrl<PlayerFaction>();
	QCONFIRM_RET_FALSE(playerFaction);

	return playerFaction->SetMeidicineSlot(nSlot, nRoom, nX, nY);
}
BOOL FactionManager::OnDbLoaded( KCharacter& rcCharacter )
{
	SetEightTrigramLevel1(rcCharacter, 0, (BYTE)rcCharacter.GetEightTrigramLevel1());
	SetEightTrigramLevel2(rcCharacter, 0, (BYTE)rcCharacter.GetEightTrigramLevel2());
	SetEightTrigramLevel3(rcCharacter, 0, (BYTE)rcCharacter.GetEightTrigramLevel3());
	SetEightTrigramLevel4(rcCharacter, 0, (BYTE)rcCharacter.GetEightTrigramLevel4());
	SetEightTrigramLevel5(rcCharacter, 0, (BYTE)rcCharacter.GetEightTrigramLevel5());
	SetEightTrigramLevel6(rcCharacter, 0, (BYTE)rcCharacter.GetEightTrigramLevel6());
	SetEightTrigramLevel7(rcCharacter, 0, (BYTE)rcCharacter.GetEightTrigramLevel7());
	SetEightTrigramLevel8(rcCharacter, 0, (BYTE)rcCharacter.GetEightTrigramLevel8());

	return TRUE;
}
