/* -------------------------------------------------------------------------
//	文件名		：	fightskillmanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2011.5.21
//	实现者		：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "fightskillmanager.h"
#include "fightskillsetting.h"
#include "onlinegameworld/kcharactermgr.h"
#include "fightskillprotocolprocessor.h"
#include "onlinegamemodule/characterskill.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegamemodule/ai/kaivm.h"

FightSkillManager g_FightSkillManager;

#define KD_MAX_SKILL_NUM 10000

FightSkillManager::FightSkillManager()
{
    m_bInit = FALSE;
}

FightSkillManager::~FightSkillManager()
{

}

BOOL FightSkillManager::Init()
{
	m_bInit = TRUE;
	return TRUE;
}

BOOL FightSkillManager::UnInit()
{
	return TRUE;
}

BOOL FightSkillManager::Breath()
{
	return TRUE;
}

BOOL FightSkillManager::CancelChannelSkill(WORD wSkillTemplateId, DWORD dwLauncherId)
{
	CharacterSkill* characterSkill = NULL;
	KCharacter* pCharacter = g_cCharacterMgr.GetById(dwLauncherId);
	LOG_PROCESS_ERROR(pCharacter);
	characterSkill = pCharacter->GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(characterSkill);
	characterSkill->CancelChannelSkill(wSkillTemplateId);

	pCharacter->m_AIVM.FireEvent(aevOnBreakPerpare, pCharacter->GetId(), 0);

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL FightSkillManager::CanCastSkill(WORD wSkillTemplateId, DWORD dwLauncherId, DWORD dwTargetId, INT nX, INT nY, INT nZ)
{
	CharacterSkill* characterSkill = NULL;
	KCharacter* pCharacter = g_cCharacterMgr.GetById(dwLauncherId);
	LOG_PROCESS_ERROR(pCharacter);
	characterSkill = pCharacter->GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(characterSkill);
	return characterSkill->CanCastSkill(wSkillTemplateId,dwTargetId, nX, nY, nZ);
EXIT0:
	return FALSE;
}

BOOL FightSkillManager::RemoveSkill(DWORD dwLauncherId,WORD wSkillId)
{
	CharacterSkill* characterSkill = NULL;
	KCharacter* pCharacter = g_cCharacterMgr.GetById(dwLauncherId);
	LOG_PROCESS_ERROR(pCharacter);
	characterSkill = pCharacter->GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(characterSkill);
	return characterSkill->RemoveSkill(wSkillId);
EXIT0:
	return FALSE;
}

BOOL FightSkillManager::AddSkill(DWORD dwLauncherId,WORD wSkillId)
{
	CharacterSkill* characterSkill = NULL;
	KCharacter* pCharacter = g_cCharacterMgr.GetById(dwLauncherId);
	LOG_PROCESS_ERROR(pCharacter);
	characterSkill = pCharacter->GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(characterSkill);
	return characterSkill->AddSkill(wSkillId, FALSE);
EXIT0:
	return FALSE;
}

BOOL FightSkillManager::CastSkill(WORD wSkillTemplateId, DWORD dwLauncherId, DWORD dwTargetId,INT nX,INT nY,INT nZ, BYTE bTimeScale)
{
	INT nRetCode = 0;

	CharacterSkill* characterSkill = NULL;
	KCharacter* pCharacter = g_cCharacterMgr.GetById(dwLauncherId);
	QCONFIRM_RET_FALSE(pCharacter);
	characterSkill = pCharacter->GetCtrl<CharacterSkill>();
	QCONFIRM_RET_FALSE(characterSkill);

	// 受技能控制状态
	if(pCharacter->m_eMoveState >= cmsOnSkill)
		return FALSE;

	nRetCode = characterSkill->CastSkill(wSkillTemplateId,dwTargetId,nX,nY,nZ, FALSE, bTimeScale == 0x01 ? TRUE : FALSE);

	if (nRetCode)
		pCharacter->m_AIVM.FireEvent(aevOnCastSkill, pCharacter->GetId(), 0);   // success
	else
		pCharacter->m_AIVM.FireEvent(aevOnCastSkillFailed, pCharacter->GetId(), 0);
		

	return TRUE;
}

BOOL FightSkillManager::GetCharacterAvailableSkills(INT nFaction, INT nCharacterLevel, std::vector<FightSkillTemplate*>& outSkills)
{
	BOOL bOK = TRUE;
	bOK = _GetCharacterAvailableSkills(nFaction, nCharacterLevel, outSkills);
	return bOK;
}

BOOL FightSkillManager::_GetCharacterAvailableSkills(INT nFaction, INT nCharacterLevel, std::vector<FightSkillTemplate*>& outSkills)
{
	std::map<INT, FightSkillTemplate*>::iterator it;
	std::map<INT, FightSkillTemplate*>* mapSkillTemplates = NULL;

	outSkills.resize(0);
	INT nEnumId = MAKELONG(0, nFaction);//(nFaction << 16) | 0;

	LOG_PROCESS_ERROR(0 < nCharacterLevel);

	mapSkillTemplates = g_cFightSkillSetting.GetFightSkillSettings();

	for (it = mapSkillTemplates->begin(); it != mapSkillTemplates->end();it++)
	{
		if ((*it).second->nFactionLimit[nFaction] && (nCharacterLevel >= (*it).second->nReqLevel)
			&& (*it).second->nFightSkillKind == FIGHT_SKILL_KIND_FACTION)  // 门派符合, 技能等级符合
		{
			outSkills.push_back((*it).second);
		}
	}

	return TRUE;
EXIT0:
	return FALSE;
}