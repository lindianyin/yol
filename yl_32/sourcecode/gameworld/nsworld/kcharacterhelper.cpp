
#include "stdafx.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/knpc.h"
#include "kcharacterhelper.h"
#include "../onlinegamemodule/fightskill/fightskillmodule.h"
#include "onlinegamemodule/ai/kaivm.h"
#include "onlinegamemodule/ai/kaictrl.h"
#include "../onlinegamemodule/ai/ThreatList.h"
#include "../onlinegameworld/scene/kscene.h"
#include "onlinegameworld/knpcmgr.h"
#include "onlinegamemodule/characterskill.h"
#include "../onlinegamemodule/fightskill/fightskillsetting.h"
#include "../onlinegamemodule/fightskill/fightskill.h"
#include "../onlinegamemodule/buffer/bufsetting.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "../onlinegamemodule/arena/arenadef.h"
#include "../onlinegamemodule/rank/RankRelayClient.h"
#include "../onlinegamemodule/award/kaward.h"
#include "onlinegamemodule/trade/kplayerpurse.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------
KCharacterHelper g_cCharacterHelper;

//-----------------------------------------------------------------------

BOOL KCharacterHelper::Init()
{
	INT nRowNum = 0;
	// 读取门派战斗力表
	ITabFile* pTabFile = g_OpenTabFile(KD_FACTION_FIGHT_SCORE_TABLE);
	LOG_PROCESS_ERROR(pTabFile);

	nRowNum = pTabFile->GetHeight();

	for (INT i = 3; i <= nRowNum; i++)
	{
		INT nRouteId = 0;
		pTabFile->GetInteger(i, "FactionId", 0, &nRouteId);

		CHARACTER_ATTRIB_SCORE* pAttribScore = new CHARACTER_ATTRIB_SCORE;

		pTabFile->GetInteger(i, "Life", 0, &pAttribScore->nLife);
		pTabFile->GetInteger(i, "Mana", 0, &pAttribScore->nMana);
		pTabFile->GetInteger(i, "Attack", 0, &pAttribScore->nAttack);
		pTabFile->GetInteger(i, "Defend", 0, &pAttribScore->nDefend);
		pTabFile->GetInteger(i, "Critical", 0, &pAttribScore->nCritical);
		pTabFile->GetInteger(i, "CriticalResist", 0, &pAttribScore->nCriticalResist);
		pTabFile->GetInteger(i, "Hit", 0, &pAttribScore->nHit);
		pTabFile->GetInteger(i, "Miss", 0, &pAttribScore->nMiss);
		pTabFile->GetInteger(i, "Skill", 0, &pAttribScore->nSkill);
		pTabFile->GetInteger(i, "PassiveSkill", 0, &pAttribScore->nPassiveSkill);
		pTabFile->GetInteger(i, "IgnoreDef", 0, &pAttribScore->nIgnoreDef);
		pTabFile->GetInteger(i, "IgnoreHarm", 0, &pAttribScore->nIgnoreHarm);

		m_mapFactionScore[nRouteId] = pAttribScore;
	}

EXIT0:
	return TRUE;
}

BOOL KCharacterHelper::Kill( KCharacter& rcDeathCharacter, DWORD dwKillerCharacterId )
{
	rcDeathCharacter.SetMoveState(cmsOnDeath);
	rcDeathCharacter.SetCurrentLife(0);
	//rcDeathCharacter.SetCurrentMana(0);  -- 死亡时不清0蓝量
	rcDeathCharacter.FireEvent(emKOBJEVENTTYPE_CHARACTER_KILL, rcDeathCharacter.GetId(), dwKillerCharacterId, 0);
	rcDeathCharacter.FireEvent(emKOBJEVENTTYPE_CHARACTER_DEATH, rcDeathCharacter.GetId(), dwKillerCharacterId, 0);

#ifdef GAME_SERVER
	KS2C_CHARACTER_KILLED sData;
	sData.dwDeathCharacterId = rcDeathCharacter.GetId();
	sData.dwKillerCharacterId = dwKillerCharacterId;
#endif
	return TRUE;
}


BOOL KCharacterHelper::SetCurrentLife(KCharacter& rcCharacter, INT nValue)
{
	if (nValue <= 0)
		nValue = 1;

	if (nValue > rcCharacter.GetMaxLife())
		nValue = rcCharacter.GetMaxLife();

	rcCharacter.SetCurrentLife(nValue);

	return TRUE;
}

BOOL KCharacterHelper::ChangeCurrentLife(KCharacter& rcCharacter, INT nValue /* = 0 */, INT nPermillage /* = 0 */)
{
	INT nMaxLife = rcCharacter.GetMaxLife();
	INT nAddValue = nValue + nMaxLife*nPermillage/1000;
	INT nCurrentLife = rcCharacter.GetCurrentLife() + nAddValue;
	
	// 掉血会打断睡眠
	if (nAddValue < 0)
	{
		//KCharacterBuffer* pKCharacterBuffer = rcCharacter.GetCtrl<KCharacterBuffer>();
		//pKCharacterBuffer->RemoveBuffer((INT64)emKBUFFER_LOGICTYPE_SLEEP);
	}

	if (nCurrentLife <= 0)
	{
		rcCharacter.SetCurrentLife(0);
	}
	else if (nCurrentLife > rcCharacter.GetMaxLife())
	{
		rcCharacter.SetCurrentLife(nMaxLife);
	}
	else
	{
		rcCharacter.SetCurrentLife(nCurrentLife);
	}

	return TRUE;
}

BOOL KCharacterHelper::SetCurrentMana(KCharacter& rcCharacter, INT nValue)
{
	if (nValue < 0)
		nValue = 0;

	if (nValue > rcCharacter.GetMaxMana())
		nValue = rcCharacter.GetMaxMana();

	rcCharacter.SetCurrentMana(nValue);

	return TRUE;
}

BOOL KCharacterHelper::ChangeCurrentMana(KCharacter& rcCharacter, INT nValue /* = 0 */, INT nPermillage /* = 0 */)
{
	INT nMaxMana = rcCharacter.GetMaxMana();
	rcCharacter.AddCurrentMana(nValue + nMaxMana*nPermillage/1000);

	INT nCurrentMana = rcCharacter.GetCurrentMana();
	if (nCurrentMana > nMaxMana)
		nCurrentMana = nMaxMana;
	if (nCurrentMana < 0)
		nCurrentMana = 0;

	rcCharacter.SetCurrentMana(nCurrentMana);

	return TRUE;
}

BOOL KCharacterHelper::SetCombatCamp(KCharacter& rcCharacter, INT nCamp)
{
	if (nCamp < emKCHARACTER_CAMP_BLACK || nCamp > emKCHARACTER_CAMP_WHITE)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	//rcCharacter.SetCombatCamp(nCamp);
	//rcCharacter.SetCurrentCombatCamp(nCamp);

	return TRUE;
}

BOOL KCharacterHelper::SetCurrentCombatCamp(KCharacter& rcCharacter, INT nCamp)
{
	if (nCamp < emKCHARACTER_CAMP_BLACK || nCamp > emKCHARACTER_CAMP_WHITE)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	//rcCharacter.SetCurrentCombatCamp(nCamp);

	return TRUE;
}

BOOL KCharacterHelper::Revive(KCharacter& rcCharacter, INT nLifePermillage, INT nManaPermillage, int nChoice)
{
	rcCharacter.SetMoveState(cmsOnStand);

	rcCharacter.m_bSystemShield = FALSE;
	rcCharacter.m_AIData.ResetData();
	rcCharacter.m_SimpThreatList.ClearAllThreat();
	rcCharacter.m_SimpThreatList.ClearAllThreaten();

	rcCharacter.SelectTarget(ttNoTarget, 0);

#ifdef GAME_SERVER
	ChangeCurrentLife(rcCharacter, rcCharacter.GetMaxLife()*nLifePermillage/1000);
	ChangeCurrentMana(rcCharacter, rcCharacter.GetMaxMana()*nManaPermillage/1000);
	//rcCharacter.SetMoveSpeed(KD_BASE_RUN_SPEED);
#endif
	
	rcCharacter.FireEvent(emKOBJEVENTTYPE_CHARACTER_REVIVE, rcCharacter.GetId(), nChoice, 0);

#ifdef GAME_SERVER
	KS2C_CHARACTER_REVIVE sData;
	sData.dwReviveCharacterId = rcCharacter.GetId();
	rcCharacter.m_cCharacterServer.BroadcastNearby(&sData, sizeof(sData), TRUE, GetFightSkillModule()->GetProtocolProcessor());
#endif

	return TRUE;
}

// 目标受到伤害相关处理
BOOL KCharacterHelper::OnHurt(KCharacter& rcCharacter, KCharacter& rcCaster, INT nDamage)
{
	ScriptSafe cJxScript = g_cScriptManager.GetSafeScript();
	//告诉双方受伤程度
	if(IS_PLAYER(rcCaster.GetId()))
	{
	//	KPlayer* pPlayer = (KPlayer*)&rcCaster;
		//g_cPlayerBaseProcess.DoSyncHeadText(pPlayer, rcCharacter.GetId(), emHeadTextBlood, -nDamage);
		cJxScript->CallTableFunction("Activity", "OnDamage", 0, "ood",rcCaster.GetScriptInterface(), rcCharacter.GetScriptInterface(), nDamage);
	}
	if (IS_PLAYER(rcCharacter.GetId()))
	{
		rcCharacter.SetCurrentDamage(nDamage);
	}
	return TRUE;
}

// 目标受到伤害相关处理
BOOL KCharacterHelper::OnRecoverLife(KCharacter& rcCharacter, KCharacter& rcCaster, INT nRecover)
{
	//KPlayer* pPlayer = NULL;

	//if (IS_PLAYER(rcCharacter.GetId()))
	//{
	//	pPlayer = (KPlayer*)&rcCharacter;
		//g_cPlayerBaseProcess.DoSyncHeadText(pPlayer, rcCharacter.GetId(), emHeadTextBlood, nRecover);
	//}

	g_cPlayerBaseProcess.BroadcastHeadText(rcCharacter, rcCharacter.GetId(), emHeadTextAddBlood, nRecover);

	return TRUE;
}

// 目标死亡后相关处理
BOOL KCharacterHelper::OnDeath(KCharacter& rcCharacter, KCharacter& rcCaster)
{
	KS2C_CHARACTER_KILLED sData;
	ScriptSafe cJxScript = g_cScriptManager.GetSafeScript();

	rcCharacter.SetMoveState(cmsOnDeath);
	
	if (IS_NPC(rcCharacter.GetId()))
	{
		KNpc* pNpc = (KNpc*)&rcCharacter;
		// 延迟5帧删除
		pNpc->SetDisappearFrame(5);
		cJxScript->CallTableFunction("Npc", "OnDeath", 0, "uuus",
			rcCaster.GetId(), pNpc->GetId(), pNpc->GetTemplateId(), pNpc->GetClassName().CStr());
	}
	else
	{
		cJxScript->CallTableFunction("Player", "OnDeath", 0, "uu",
			rcCaster.GetId(), rcCharacter.GetId());
	}

	sData.dwDeathCharacterId = rcCharacter.GetId();
	sData.dwKillerCharacterId = rcCaster.GetId();
	rcCharacter.m_cCharacterServer.BroadcastNearby(&sData, sizeof(sData), TRUE, GetFightSkillModule()->GetProtocolProcessor());

	rcCharacter.FireEvent(emKOBJEVENTTYPE_CHARACTER_DEATH, rcCharacter.GetId(), rcCaster.GetId(), 0);

	rcCharacter.m_AIVM.FireEvent(aevOnSelfKilled, rcCharacter.GetId(), rcCaster.GetId());

	PROCESS_ERROR(rcCharacter.m_pScene);

	if (rcCharacter.m_AIData.nPatrolPathID)
		rcCharacter.m_pScene->m_AIPatrolGroupManager.LeaveGroup(rcCharacter.m_AIData.nPatrolPathID, rcCharacter.m_dwId);

EXIT0:
	return TRUE;
}

BOOL KCharacterHelper::ChangeMaxMana(KCharacter& rcCharacter, INT nValue)
{
	rcCharacter.AddMaxMana(nValue);
	return TRUE;
}


BOOL KCharacterHelper::CalcFightScore(KCharacter& rcCharacter)
{
	//QLogPrintf(LOG_INFO,"..CalcFightScore..");
	INT nScore = 0;
	BOOL bResult = FALSE;
	INT nFactionId = rcCharacter.GetPrimaryFaction();
	CharacterSkill::SKILLDATAMAP mapSkill;
	CharacterSkill::SKILLDATAMAP::iterator it_skill;
	CharacterSkill* pCharacterSkill = 0;
	FightSkillTemplate* pSkillTemplate = NULL;
	INT nPhysicAttack = 0;
	time_t tNow = KSysService::Time(NULL);
	KPlayerPurse* pPlayerPurse = NULL;
	KE_PURSE_RESULT pResult = emKPURSE_S_SUCCESS;
	char szStr[1024];
	
	CHARACTER_ATTRIB_SCORE* pAttribScore = m_mapFactionScore[nFactionId];
	PROCESS_ERROR(pAttribScore);

	nPhysicAttack = rcCharacter.GetAttack();

	nScore += pAttribScore->nLife * rcCharacter.GetMaxLife();
	nScore += pAttribScore->nMana * rcCharacter.GetMaxMana();
	nScore += pAttribScore->nAttack * nPhysicAttack;
	nScore += pAttribScore->nCritical * rcCharacter.GetCriticalValue();
	
	nScore += pAttribScore->nCriticalResist * rcCharacter.GetResist();
	nScore += pAttribScore->nHit * rcCharacter.GetHitCoefficient();
	nScore += pAttribScore->nMiss * rcCharacter.GetDodge();
	nScore += pAttribScore->nDefend * rcCharacter.GetDef();

	nScore += pAttribScore->nIgnoreDef * rcCharacter.GetIgnoreDef();
	nScore += pAttribScore->nIgnoreHarm * rcCharacter.GetIgnorDmg();

	pCharacterSkill = rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	mapSkill = pCharacterSkill->GetSkillMap();

	
	pPlayerPurse = rcCharacter.GetCtrl<KPlayerPurse>();
	LOG_PROCESS_ERROR(pPlayerPurse);
	
	for (it_skill = mapSkill.begin(); it_skill != mapSkill.end(); ++it_skill)
	{
		FightSkill* pSkill = it_skill->second->pSkill;
		LOG_PROCESS_ERROR(pSkill);

		pSkillTemplate = pSkill->m_pFightSkillTemplate;
		LOG_PROCESS_ERROR(pSkillTemplate);

		if(pSkillTemplate->nFightSkillType == FIGHT_SKILL_MEDICINE)
		{
			MedicineSkillNumber* pMedicineSkillNumber = NULL;
			if (pSkillTemplate->nSlot == emRED_SKILL)
			{
			    pMedicineSkillNumber = g_cFightSkillSetting.GetMedicineSkillNumber(emMEDICINE_SKILL_RED, pSkill->m_nSkillLevel);
			}
			else
			{
                pMedicineSkillNumber = g_cFightSkillSetting.GetMedicineSkillNumber(emMEDICINE_SKILL_BLUE, pSkill->m_nSkillLevel);
			}
			
			if (pMedicineSkillNumber != NULL)
			{
                nScore += (pMedicineSkillNumber->nValue * pAttribScore->nPassiveSkill);
			}
		}
		else if (pSkillTemplate->nFightSkillType == emSKILL_TYPE_RESIST)
		{
			ResistSkillNumber* pSkillNumber = g_cFightSkillSetting.GetResistSkillNumberByResistSkillId(pSkillTemplate->nSkillID, pSkill->m_nSkillLevel);
            if (pSkillNumber != NULL)
			{
				nScore += (pSkillNumber->nValue * pAttribScore->nPassiveSkill);
			}
		}
		else if (pSkillTemplate->nFightSkillType != FIGHT_SKILL_PASSIVE)
		{
			InactiveSkillNumber* pSkillNumber = g_cFightSkillSetting.GetInactiveSkillNumber(pSkillTemplate->nSkillID, pSkill->m_nSkillLevel);
			if (pSkill->m_nSkillLevel > 1)
			{
				InactiveSkillNumber* pPreSkillNumber = g_cFightSkillSetting.GetInactiveSkillNumber(pSkillTemplate->nSkillID, pSkill->m_nSkillLevel - 1);
				if (pSkillNumber != NULL && pPreSkillNumber != NULL)
				{
					nScore += (pSkillNumber->nSkillCoefficient - pPreSkillNumber->nSkillCoefficient) * 2 + pSkillNumber->nHarmValue * pAttribScore->nSkill;
				}
			}
		}
	}
	
	//确保不会出现负值
	LOG_PROCESS_ERROR(nScore > 0);
	rcCharacter.SetFightScore(nScore / 100);

	// 修改 个人排行-> 战斗力榜
	RankRelayClient::DoModifyRankValueRequest(emRANK_ROLE_FIGHTSCORE, rcCharacter.GetId(), rcCharacter.GetFightScore(), tNow);

	// 修改排行榜排名
	RankRelayClient::DoModifyRankValueRequest(emRANK_ROLE_FIGSHTSCORE_OS, rcCharacter.GetId(), rcCharacter.GetFightScore(), tNow);

	// 只修改战斗力，不计算排名
	if(rcCharacter.GetLevel() >= ARENA_OPEN_LEVEL)
	{
		RankRelayClient::DoModifyRankValueRequest(emRANK_ARENA, rcCharacter.GetId(), rcCharacter.GetFightScore(), tNow, FALSE);
		//RankRelayClient::DoModifyRankValueRequest(emRANK_AREAN_OS, rcCharacter.GetId(), rcCharacter.GetFightScore(), tNow, FALSE);
	}

	//by ldy
	ZeroMemory(szStr,sizeof(szStr));
	//sprintf(szStr,"fightforce=%d",rcCharacter.GetFightScore());
	//记录战斗力
	pPlayerPurse->LogMoney(EM_MONEY_COIN,0,69,&pResult,TRUE,szStr);
	bResult = TRUE;
EXIT0:
	return bResult;
}

DWORD KCharacterHelper::GetNextLvExp(INT nNextLevel)
{
	ASSERT(nNextLevel > 0 && nNextLevel < 80);

	DWORD dwLvUpNeedExp = 0;
    CONST QAssetAward* pAssetAward = g_cAwardSetting.RequestAssetAward("levelexp", nNextLevel);
	if (pAssetAward != NULL)
	{
		dwLvUpNeedExp = pAssetAward->nExpAwardId;
	}
	/*// 升级经验公式：500 * (lv - 1) ^ 2 + (lv  /10 * 10) ^ 2 * 500
	INT nLv = nNextLevel / 10 * 10;
	DWORD nLvUpNeedExp = 500 * (nNextLevel - 1) * (nNextLevel - 1) + nLv * nLv * 500;

	if(nNextLevel > 50)
	{
		nLvUpNeedExp = nLvUpNeedExp * 4;	// 当前版本需求，50级以上经验 * 4
	}*/
	return dwLvUpNeedExp;
}

// -------------------------------------------------------------------------
