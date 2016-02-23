#include "stdafx.h"
#include "kbladectrl.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "kbladeprotocol.h"
#include "kblademodule.h"
#include "kbladesetting.h"
#include "../onlinegamemodule/pet/kpetctrl.h"
#include "../onlinegamemodule/pet/kpet.h"
#include "../onlinegamemodule/item2/kplayeritem.h"

KBladeCtrl::KBladeCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KBladeCtrl::~KBladeCtrl()
{

}

BOOL KBladeCtrl::Init()
{
	memset(&m_sBladeAttr, 0, sizeof(m_sBladeAttr));

	memset(&m_sCurBladeAttr, 0, sizeof(m_sCurBladeAttr));

	return TRUE;
}

BOOL KBladeCtrl::OnLogin()
{
	return TRUE;
}

BOOL KBladeCtrl::Active()
{
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)//每秒执行检测一次
	{
		goto EXIT0;
	}

	if(m_rcPlayer.GetAngerLeftTime() > 0)
	{
		m_rcPlayer.SetAngerLeftTime(m_rcPlayer.GetAngerLeftTime() - 1);

		if(m_rcPlayer.GetAngerLeftTime() <= 0)
		{
			ResetBladeState();
		}
	}

EXIT0:
	return TRUE;
}

VOID KBladeCtrl::ResetBladeState()
{
	// 恢复形象
	ResetPendants();

	RemoveBladeAttr();

	m_rcPlayer.SetSpeed(m_rcPlayer.GetSpeed() - BLADE_SPEED_ADD);
}

VOID KBladeCtrl::CheckRemoveBladeState()
{
	if(m_rcPlayer.GetAngerLeftTime() > 0)
	{
		ResetBladeState();

		m_rcPlayer.SetAngerLeftTime(0);
	}
}

// 切换暴走状态
BOOL KBladeCtrl::OnStartCrazy()
{
	BOOL bResult = FALSE;

	LOG_PROCESS_ERROR(m_rcPlayer.GetAngerLeftTime() <= 0);
	LOG_PROCESS_ERROR(m_rcPlayer.GetAnger() >= 100);

	m_rcPlayer.SetAngerLeftTime(BLADE_STATE_TIME);

	m_rcPlayer.SetSpeed(m_rcPlayer.GetSpeed() + BLADE_SPEED_ADD);

	// 清空怒气
	m_rcPlayer.SetAnger(0);

	// 暴走形象
	ApplyBladePendants();

	// 换上暴走数值
	ApplyBladeAttr();

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KBladeCtrl::ApplyBladePendants()
{
	m_rcPlayer.SetHatRepresentId(0);
	if(m_rcPlayer.GetSex() == emROLETYPE_MALE)
	{
		m_rcPlayer.SetBodyRepresentId(BLADE_MALE_BODY_PENDANT);
		m_rcPlayer.SetHeadPendantRepresentId(BLADE_MALE_HEAD_PENDANT);
		m_rcPlayer.SetBackPendantRepresentId(BLADE_MALE_BACK_PENDANT);
		m_rcPlayer.SetRightWeaponRepresentId(BLADE_MALE_WEAPON_PENDANT);
	}
	else
	{
		m_rcPlayer.SetBodyRepresentId(BLADE_FEMALE_BODY_PENDANT);
		m_rcPlayer.SetHeadPendantRepresentId(BLADE_FEMALE_HEAD_PENDANT);
		m_rcPlayer.SetBackPendantRepresentId(BLADE_FEMALE_BACK_PENDANT);
		m_rcPlayer.SetRightWeaponRepresentId(BLADE_FEMALE_WEAPON_PENDANT);
	}
}

VOID KBladeCtrl::ResetPendants()
{
	KPlayerItem* pPlayerItem = m_rcPlayer.GetCtrl<KPlayerItem>();
	LOG_PROCESS_ERROR(pPlayerItem);

	pPlayerItem->ReApplyEquipRes();

EXIT0:
	return;
}


// 变身时的属性加成
BOOL KBladeCtrl::ApplyBladeAttr()
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	KPetCtrl* pPetCtrl = NULL;
	KPet* pPet = NULL;
	KBASEATTR sTacticAttr;
	KBLADE_ATTR_PERCENT* pPercent = &g_cBladeSetting.m_cBladeAttrPercent;

	ASSERT(m_sBladeAttr.nLife == 0);

	pPetCtrl = m_rcPlayer.GetCtrl<KPetCtrl>();
	LOG_PROCESS_ERROR(pPetCtrl);

	// 先减去原来阵法加的属性值
	for(INT nCharacter = emPET_CHARACTER_ATTACK; nCharacter <= emPET_CHARACTER_DODGE; ++nCharacter)
	{
		pPet = pPetCtrl->GetPetByTactic(nCharacter);
		if(!pPet)
		{
			continue;
		}

		pPetCtrl->RemoveTacticAttr(pPet);
	}

	m_sBladeAttr.nLife = m_rcPlayer.GetMaxLife() * pPercent->nLifeP / 100;
	m_sBladeAttr.nMana = m_rcPlayer.GetMaxMana() * pPercent->nManaP / 100;
	m_sBladeAttr.nAttack = m_rcPlayer.GetAttack() * pPercent->nAttackP / 100;
	m_sBladeAttr.nDef = m_rcPlayer.GetDef() * pPercent->nDefP / 100;
	m_sBladeAttr.nHit = m_rcPlayer.GetHitCoefficient() * pPercent->nHitP /100;
	m_sBladeAttr.nMiss = m_rcPlayer.GetDodge() * pPercent->nMissP / 100;
	m_sBladeAttr.nCrit = m_rcPlayer.GetCriticalValue() * pPercent->nCritP / 100;
	m_sBladeAttr.nResist = m_rcPlayer.GetResist() * pPercent->nResistP / 100;

	// 再加上变身神仙阵法属性值
	for(INT nCharacter = emPET_CHARACTER_LIFE; nCharacter <= emPET_CHARACTER_DODGE; ++nCharacter)
	{
		pPet = pPetCtrl->GetPetByTactic(nCharacter);
		if(!pPet)
		{
			continue;
		}

		pPet->Get_TacticAttrib(pPet->GetQuality(), pPet->GetRankLevel(), sTacticAttr, TRUE);

		switch(nCharacter)
		{
		case emPET_CHARACTER_LIFE:
			m_sBladeAttr.nLife += sTacticAttr.nValue;
			break;
		case emPET_CHARACTER_ATTACK:
			m_sBladeAttr.nAttack += sTacticAttr.nValue;
			break;
		case emPET_CHARACTER_HIT:
			m_sBladeAttr.nHit += sTacticAttr.nValue;
			break;
		case emPET_CHARACTER_CRIT:
			m_sBladeAttr.nCrit += sTacticAttr.nValue;
			break;
		case emPET_CHARACTER_RESIST:
			m_sBladeAttr.nResist += sTacticAttr.nValue;
			break;
		case emPET_CHARACTER_DEF:
			m_sBladeAttr.nDef += sTacticAttr.nValue;
			break;
		case emPET_CHARACTER_DODGE:
			m_sBladeAttr.nMiss += sTacticAttr.nValue;
			break;
		default:
			ASSERT(FALSE);
		}
	}

	m_sCurBladeAttr.nLife = m_rcPlayer.GetMaxLife() + m_sBladeAttr.nLife;
	m_sCurBladeAttr.nMana = m_rcPlayer.GetMaxMana() + m_sBladeAttr.nMana;
	m_sCurBladeAttr.nAttack = m_rcPlayer.GetAttack() + m_sBladeAttr.nAttack;
	m_sCurBladeAttr.nDef = m_rcPlayer.GetDef() + m_sBladeAttr.nDef;
	m_sCurBladeAttr.nHit = m_rcPlayer.GetHitCoefficient() + m_sBladeAttr.nHit;
	m_sCurBladeAttr.nMiss = m_rcPlayer.GetDodge() + m_sBladeAttr.nMiss;
	m_sCurBladeAttr.nCrit = m_rcPlayer.GetCriticalValue() + m_sBladeAttr.nCrit;
	m_sCurBladeAttr.nResist = m_rcPlayer.GetResist() + m_sBladeAttr.nResist;

	//m_rcPlayer.SetMaxLife(m_sCurBladeAttr.nLife);
	//m_rcPlayer.SetCurrentLife(m_rcPlayer.GetMaxLife());

	//m_rcPlayer.SetMaxMana(m_sCurBladeAttr.nMana);
	//m_rcPlayer.SetCurrentMana(m_rcPlayer.GetMaxMana());

	m_rcPlayer.SetAttack(m_sCurBladeAttr.nAttack);

	m_rcPlayer.SetDef(m_sCurBladeAttr.nDef);

	m_rcPlayer.SetHitCoefficient(m_sCurBladeAttr.nHit);

	m_rcPlayer.SetDodge(m_sCurBladeAttr.nMiss);

	m_rcPlayer.SetCriticalValue(m_sCurBladeAttr.nCrit);

	m_rcPlayer.SetResist(m_sCurBladeAttr.nResist);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KBladeCtrl::RemoveBladeAttr()
{
	BOOL bResult = FALSE;
	KPetCtrl* pPetCtrl = NULL;
	KPet* pPet = NULL;

	LOG_PROCESS_ERROR(m_sBladeAttr.nLife > 0);

	pPetCtrl = m_rcPlayer.GetCtrl<KPetCtrl>();
	LOG_PROCESS_ERROR(pPetCtrl);

	/*m_rcPlayer.SetMaxLife(m_rcPlayer.GetMaxLife() - m_sBladeAttr.nLife);
	if(m_rcPlayer.GetCurrentLife() > m_rcPlayer.GetMaxLife())
	{
		m_rcPlayer.SetCurrentLife(m_rcPlayer.GetMaxLife());
	}

	m_rcPlayer.SetMaxMana(m_rcPlayer.GetMaxMana() - m_sBladeAttr.nMana);
	if(m_rcPlayer.GetCurrentMana() > m_rcPlayer.GetMaxMana())
	{
		m_rcPlayer.SetCurrentMana(m_rcPlayer.GetMaxMana());
	}*/

	m_rcPlayer.SetAttack(m_rcPlayer.GetAttack() - m_sBladeAttr.nAttack);

	m_rcPlayer.SetDef(m_rcPlayer.GetDef() - m_sBladeAttr.nDef);

	m_rcPlayer.SetHitCoefficient(m_rcPlayer.GetHitCoefficient() - m_sBladeAttr.nHit);

	m_rcPlayer.SetDodge(m_rcPlayer.GetDodge() - m_sBladeAttr.nMiss);

	m_rcPlayer.SetCriticalValue(m_rcPlayer.GetCriticalValue() - m_sBladeAttr.nCrit);

	m_rcPlayer.SetResist(m_rcPlayer.GetResist() - m_sBladeAttr.nResist);

	memset(&m_sBladeAttr, 0, sizeof(m_sBladeAttr));

	// 加上原来阵法加的属性值
	for(INT nCharacter = emPET_CHARACTER_ATTACK; nCharacter <= emPET_CHARACTER_DODGE; ++nCharacter)
	{
		pPet = pPetCtrl->GetPetByTactic(nCharacter);
		if(!pPet)
		{
			continue;
		}

		pPetCtrl->ApplyTacticAttr(pPet);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

CONST KBLADE_ATTR& KBladeCtrl::GetBladeAttr()
{
	return m_sBladeAttr;
}