/* -------------------------------------------------------------------------
//	文件名		：	playerfaction.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-25 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "onlinegamemodule/playerfaction.h"
#include "faction/factionmanager.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegamemodule/characterskill.h"
#include "fightskill/fightskillsetting.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "onlinegameworld/kplayermgr.h"
#include "fightskill/fightskillprotocolprocessor.h"
#include "faction/factionprotocolprocessor.h"
#include "award/kawardsetting.h"
#include "factionmodule.h"

PlayerFaction::PlayerFaction(KCharacter& rcPlayer)
: KCharacterCtrlBase(rcPlayer)
{
	//m_mapSkillLevelData.clear();
	//memset(m_mapSkillLevelData, '\0', sizeof(m_mapSkillLevelData));
}

PlayerFaction::~PlayerFaction()
{
	//m_mapSkillLevelData.clear();
	//memset(m_mapSkillLevelData, '\0', sizeof(m_mapSkillLevelData));
}

BOOL PlayerFaction::Init()
{
	m_bDbLoaded = FALSE;

	m_cSetting.bLowestQualityLock = FALSE;
	m_cSetting.bHideOtherPlayer = FALSE;
	m_cSetting.bHideParticleEffect = FALSE;
	m_cSetting.bLowestVolumeLock = FALSE;
	m_cSetting.nBackgroundMusicVolume = 70;
	m_cSetting.nGameSoundVolume = 100;
	m_cSetting.nRepresentQuality = 1;
	m_cSetting.nAutoAddHp = 80;
	m_cSetting.nAutoAddMp = 80;

	return TRUE;
}

BOOL PlayerFaction::UnInit()
{
	m_bDbLoaded = FALSE;

	/*for(FACTIONSKILLMAP::iterator iter = m_mapSkillLevelData.begin(); iter != m_mapSkillLevelData.end(); ++iter)
	{
		SAFE_DELETE(iter->first);
	}*/
	//m_mapSkillLevelData.clear();
	//memset(m_mapSkillLevelData, '\0', sizeof(m_mapSkillLevelData));
	return TRUE;
}

BOOL PlayerFaction::InitAttrs()
{
	INT nLevel = m_rcCharacter.GetLevel();
	QString strLevel("LevelUp");

	CONST QNumericalAward* pAward = g_cAwardSetting.RequestNumericalAward(strLevel, nLevel, nLevel);
	QCONFIRM_RET_FALSE(pAward);

	m_rcCharacter.ApplyNumericalAward(pAward);

	if(m_rcCharacter.IsFirstTimeLogin())
	{
		m_rcCharacter.SetCurrentLife(m_rcCharacter.GetMaxLife());
		m_rcCharacter.SetCurrentMana(m_rcCharacter.GetMaxMana());
	}

	// 调整伤害加成
	INT nModifyDmgP = m_rcCharacter.GetModifyDmgP() + 100;
	m_rcCharacter.SetModifyDmgP(nModifyDmgP);

	//职业系数
	m_rcCharacter.SetOccupationCoefficient(1);

	return TRUE;
}

BOOL PlayerFaction::OnLogin()
{
	InitAttrs();

	if (m_rcCharacter.IsFirstTimeLogin())
	{
		m_bDbLoaded = TRUE;  // 第一不用读取，所以假定已读取
	}

	FactionModule* pFactionModule = GetFactionModule();
	LOG_PROCESS_ERROR(pFactionModule);

	pFactionModule->DoSyncPlayerSetting((KPlayer&)m_rcCharacter, m_cSetting);

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerFaction::OnLogout()
{
	return TRUE;
}

// 自动生命回复
VOID PlayerFaction::LifeAutoRecover()
{
	/*
	INT nRecoverLife = 0;
	INT nBaseMaxLife = g_cFactionManager.GetBaseMaxLife(m_rcCharacter);
	INT nBodyAbility = m_rcCharacter.GetBodyAblity();
	INT nCurrentLife = m_rcCharacter.GetCurrentLife();

	PROCESS_SUCCESS(nCurrentLife >= m_rcCharacter.GetMaxLife());

	// 生命回复速度 = 1% * 生命上限 * (体质值 * 2 / (体质值 + 1000)) + 10
	nRecoverLife = nBaseMaxLife * nBodyAbility * 2 / ((nBodyAbility + 1000) * 100) + 10;
	// 最小1点
	nRecoverLife = MAX(nRecoverLife, 1);
	nCurrentLife += nRecoverLife;

	if(nCurrentLife > m_rcCharacter.GetMaxLife())
		nCurrentLife = m_rcCharacter.GetMaxLife();

	m_rcCharacter.SetCurrentLife(nCurrentLife);
	*/
EXIT1:
	return;
}

// 自动真气回复
VOID PlayerFaction::ManaAutoRecover()
{
	/*
	INT nRecoverMana = 0;
	INT nFaction = m_rcCharacter.GetPrimaryFaction();
	INT nCurrentMana = m_rcCharacter.GetCurrentMana();
	INT nBaseMaxMana = g_cFactionManager.GetBaseMaxMana(m_rcCharacter);
	INT nAtomAbility = m_rcCharacter.GetAtomAblity();

	PROCESS_SUCCESS(nCurrentMana >= m_rcCharacter.GetMaxMana());
	// 森罗不回复真气
	PROCESS_SUCCESS(nFaction == FACTION_SENLUO);

	// 真气回复速度 = 1% * 真气上限 * （1 + 精神值 / 1000） + 10
	nRecoverMana = (1000 * nBaseMaxMana + nBaseMaxMana * nAtomAbility) / 100000 + 10;
	// 最小1点
	nRecoverMana = MAX(nRecoverMana, 1);
	nCurrentMana += nRecoverMana;

	if(nCurrentMana > m_rcCharacter.GetMaxMana())
		nCurrentMana = m_rcCharacter.GetMaxMana();

	m_rcCharacter.SetCurrentMana(nCurrentMana);
	*/
EXIT1:
	return;
}

BOOL PlayerFaction::Active()
{
	// 每秒处理一次自动回复生命、真气和集中值
	if (!(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS))
	{
		if(IS_PLAYER(m_rcCharacter.GetId()) && m_rcCharacter.GetMoveState() != cmsOnDeath)
		{
			LifeAutoRecover();
			ManaAutoRecover();
		}
	}
	return TRUE;
}

BOOL PlayerFaction::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	memcpy(pBuffer, (VOID*)(&m_cSetting), sizeof(FACTION_PLAYER_SETTING));
	uUsedSize += sizeof(FACTION_PLAYER_SETTING);
	
	return TRUE;
}

BOOL PlayerFaction::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bRet = FALSE;
	m_bDbLoaded = TRUE;

	m_cSetting = (*(FACTION_PLAYER_SETTING*)(pData));
	uSize -= sizeof(FACTION_PLAYER_SETTING);

	g_cFactionManager.OnDbLoaded(m_rcCharacter);
	
	return TRUE;
}
INT PlayerFaction::GetSkillSlotRuneKey(INT nSlot, INT nRune)  // 返回槽位符文组合键
{
	//return (nSlot << 16) | nRune;
	return MAKELONG(nRune, nSlot);
}


//BOOL PlayerFaction::ChangeRune(INT nSlot, INT nRune)
//{
//	return TRUE;
//	//INT nCurrentCombineKey;  // 组合key
//
//	//if (nSlot == 0 && nRune == 0)
//	//{
//	//	QLogPrintf(LOG_INFO, "[FightSkillProtocolProcessor::ProcessC2S_ChangeRune]不能升级槽0符文0技能！");
//	//}
//
//	//nCurrentCombineKey = this->GetSkillSlotRuneKey(nSlot, nRune);  // 槽位、符文组合
//	//if (m_arrSkillLevelData.find(nCurrentCombineKey) == m_arrSkillLevelData.end())
//	//{
//	//	QLogPrintf(LOG_WARNING, "找不到第%d槽%d符文存在的技能！", nSlot, nRune);
//	//	goto EXIT1;
//	//}
//
//	//m_aryRuneId[nSlot - 1] = nRune;
//
////EXIT1:
////	return TRUE;
////
////EXIT0:
////	return FALSE;
//}

BOOL PlayerFaction::OnFactionRouteChange(INT nLevel, INT nFaction, INT nRoute)
{
	//职业系数
	m_rcCharacter.SetOccupationCoefficient(1);

	if (m_bDbLoaded)
	{
		//this->ReloadPlayerSkills();
	}

	return TRUE;
}

BOOL PlayerFaction::OnLevelChange(INT nOldLevel, INT nNewLevel)
{
	BOOL bResult = FALSE;
	CONST QNumericalAward* pOldAward = NULL;
	CONST QNumericalAward* pNewAward = NULL;
	QString strLevel("LevelUp");
	CharacterSkill* pCharacterSkill = NULL;

	PROCESS_ERROR(m_bDbLoaded);

	if(nOldLevel)
	{
		pOldAward = g_cAwardSetting.RequestNumericalAward(strLevel, nOldLevel, nOldLevel);
		LOG_PROCESS_ERROR(pOldAward);
		LOG_PROCESS_ERROR(m_rcCharacter.RemoveNumericalAward(pOldAward, TRUE));
	}

	pNewAward = g_cAwardSetting.RequestNumericalAward(strLevel, nNewLevel, nNewLevel);
	LOG_PROCESS_ERROR(pNewAward);
	LOG_PROCESS_ERROR(m_rcCharacter.ApplyNumericalAward(pNewAward));

	pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);
	pCharacterSkill->ReloadPlayerSkills();
	
	bResult = TRUE;
EXIT0:
	return bResult;
}

//BOOL PlayerFaction::SyncSkillRuneIds()
//{
//	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_rcCharacter.m_dwId);
//	QCONFIRM_RET_FALSE(pPlayer);
//
//	return FightSkillProtocolProcessor::SyncS2C_SyncRuneIds(pPlayer->GetPlayerIndex(), m_aryRuneId);
//}


// 同步药槽设置
//BOOL PlayerFaction::SyncMedicineSlots()
//{
//	//KPlayer* pPlayer = g_cPlayerMgr.GetById(m_rcCharacter.m_dwId);
//	//QCONFIRM_RET_FALSE(pPlayer);
//	//for(INT slot = 0; slot < QD_PLAYER_MEDICINE_SLOT_SIZE; ++slot)
//	//{
//	//	
//	//	PlayerMedicineSlot& medSlot = m_PlayerMedicineSlots[slot];
//	//	FactionProtocolProcessor::SyncS2C_MedicineSlot(pPlayer->GetPlayerIndex(), slot, medSlot.nRoom, medSlot.nX, medSlot.nY);
//	//	/*PTC_FACTION_CHANGE_MEDICINE_SLOT cSendData;
//	//	cSendData.nSlot = slot;
//	//	cSendData.nRoom = medSlot->nRoom;
//	//	cSendData.nX = medSlot->nX;
//	//	cSendData.nY = medSlot->nY;
//
//	//	pProtocolProcessor->SendData(m_rcCharacter.m_dwId, cSendData);*/
//	//}
//
//	return TRUE;
//
//}

BOOL PlayerFaction::SetMeidicineSlot(INT nSlot, INT nRoom, INT nX, INT nY)
{
	if (nSlot > 1 || nSlot < 0)
	{
		QLogPrintf(LOG_ERR, "[PlayerFaction::SetMeidicineSlot]nSlot不正常: %d", nSlot);
		return FALSE;
	}

	//PlayerMedicineSlot medSlot = {nRoom, nX, nY};
	//m_PlayerMedicineSlots[nSlot] = medSlot;

	return TRUE;
}