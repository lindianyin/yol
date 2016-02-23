/* -------------------------------------------------------------------------
//	文件名		：	fightskillmodule.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-21
//	实现者		：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "fightskillmodule.h"
#include "fightskillprotocol.h"
#include "fightskillmanager.h"
#include "fightskillprotocolprocessor.h"
#include "fightskillsetting.h"

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_FIGHT_SKILL>()
{
	return GetFightSkillModule();
}

FightSkillModule::FightSkillModule() : KModuleBase(emMODULE_FIGHT_SKILL)
{

}

FightSkillModule::~FightSkillModule()
{

}

BOOL FightSkillModule::OnInit()
{
	KModuleBase::OnInit();
	g_cFightSkillSetting.Init();
	g_FightSkillManager.Init();
	return TRUE;
}

BOOL FightSkillModule::OnUnInit()
{
	KModuleBase::OnUnInit();

	return TRUE;
}

BOOL FightSkillModule::Breath()
{
	KModuleBase::Breath();
	return TRUE;
}

BOOL FightSkillModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	FightSkillProtocolProcessor::ProtocolProcess(nPlayerIndex, pbData, uDataLen);
	return TRUE;
}

BOOL FightSkillModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_S2C(PTC_FIGHT_SKILL_STARTSKILL);
	KD_PTC_FIXED_SIZE_S2C(PTC_FIGHT_SKILL_S2C_CANCEL_CHANNEL_SKILL);
	KD_PTC_FIXED_SIZE_S2C(PTC_FIGHT_SKILL_S2C_CHANGE_CHANNEL_STATE);
	//KD_PTC_FIXED_SIZE_S2C(PTC_FIGHT_SKILL_ENDSKILL);
	KD_PTC_FIXED_SIZE_S2C(PTC_FIGHT_SKILL_ADDSKILL);
	KD_PTC_FIXED_SIZE_S2C(PTC_FIGHT_SKILL_REMOVESKILL);
	KD_PTC_FIXED_SIZE_S2C(PTC_FIGHT_SKILL_CLEAR_CD);
	KD_PTC_FIXED_SIZE_S2C(KS2C_CHARACTER_REVIVE);	//临时放在这里
	KD_PTC_FIXED_SIZE_S2C(KS2C_CHARACTER_KILLED);
	//KD_PTC_FIXED_SIZE_S2C(PTC_FIGHT_SKILL_S2C_SYNC_RUNE);

	KD_PTC_FIXED_SIZE_C2S(PTC_FIGHT_SKILL_REQUESTSTARTSKILL);
	KD_PTC_FIXED_SIZE_C2S(PTC_FIGHT_SKILL_C2S_CANCEL_CHANNEL_SKILL);
	KD_PTC_FIXED_SIZE_C2S(PTC_FIGHT_SKILL_C2S_CHANGE_FACTION_ROUTE);
	KD_PTC_FIXED_SIZE_C2S(PTC_FIGHT_SKILL_C2S_UPGRADE_SKILL);

	KD_PTC_LENGTNED_SIZE_S2C(PTC_FIGHT_S2C_SKILL_SYNCSKILL);
	return TRUE;
}