/* -------------------------------------------------------------------------
//	文件名		：	kpet.cpp
//	创建者		：	zhaoyu
//	创建时间	：	2013-2-27
//	功能描述	：	宠物
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kpet.h"
#include "kluapet.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kcharactermgr.h"
#include "kpetctrl.h"
#include "onlinegameworld/korpgworld.h"
#include "award/awarddef.h"
#include "award/kawardsetting.h"
#include "buffer/buffermanager.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "fightskill/fightskillsetting.h"
#include "../onlinegameworld/scene/kscenemgr.h"
#include "../onlinegamemodule/bladestate/kbladectrl.h"
#include "../onlinegamemodule/rank/RankRelayClient.h"
#include "onlinegamemodule/trade/kplayerpurse.h"



KPet::KPet()
{
	m_pLuna = new KLuaPet(this);
	m_nCurSkillIndex = 0;
}

KPet::~KPet()
{
	SAFE_DELETE(m_pLuna);
}

BOOL KPet::Init(KPET_INDEX sIndex, UINT uRandSeed /* = 0 */)
{
	KPetTemplate* pPetTemplate = g_cPetSetting.GetPetTemplate(sIndex.nType);
	QCONFIRM_RET_FALSE(pPetTemplate);
	m_pPetTemplate = pPetTemplate;

	m_nType = sIndex.nType;
	m_nQuality = sIndex.nQuality;
	m_nCharacter = sIndex.nCharacter;
	strncpy(m_szPetName, pPetTemplate->m_szName, sizeof(m_szPetName));

	// TODO:根据宠物品质初始化品级和阶层

	m_nLevel = KD_PET_INIT_LEVEL;
	m_nRankLevel = KD_PET_INIT_RANKLEVEL;
	m_byHungry = KD_PET_HUNGRY_MAX;
	m_byState = emPET_STATE_REST;
	m_nLastState = emPET_STATE_REST;
	m_nEquipSlot = emPET_COUNT_EQUIP;
	m_dwExp = 0;
	m_nRename = 0;
	m_dwTargetId = 0;
	m_nX = 0;
	m_nY = 0;
	m_nTactic = 0;
	m_nCurrentSkillId = 0;
	m_nCurrentCastDelay = 0;
	m_nWaitAfterCast = 0;
	m_nCurSkillIndex = 0;
	m_nHungryConsume = pPetTemplate->m_nHungry;
	ZeroMemory(m_aryEquip,sizeof(m_aryEquip));

	m_mapSkillLevels.clear();
	m_mapSkill.clear();
	m_arrSkills.clear();

	// 初始化槽位锁
	for (INT i = 0; i < emPET_COUNT_EQUIP; i++)
	{
		if (i < PET_INIT_EQUIP_SLOT_NUM)
		{
            m_aryEquip[i].byLock = 0;
		}
		else
		{
            m_aryEquip[i].byLock = 1;
		}
	}
	/*
	if (!uRandSeed)
	{	// 打散随机种子
		g_Random(-1);
		uRandSeed = g_GetRandomSeed();
	}
	m_dwRandSeed = uRandSeed;
	m_cRandom.SetSeed(uRandSeed);	// 使用随机种子
	*/

	// 随机基础属性的上下浮动比例
	//m_byHungry = (BYTE)KSysService::Rand(257);
	//m_byHungry = KD_PET_HUNGRY_MAX;
	
	ZeroMemory(m_aryBaseAttr,sizeof(m_aryBaseAttr));



	Init_BaseAttrib();
	Init_TacticAttrib();
	Init_Skills();

	SetFightScore(GetBaseAttrScore());

	return TRUE;
}

DWORD KPet::GetNextLvExp(INT nNextLevel)
{
	ASSERT(nNextLevel > 0 && nNextLevel < 100);
	// 升级经验公式：500 * lv ^ 1.8
	//DWORD dwLvUpNeedExp = static_cast<int>(500 * pow(static_cast<double>(nNextLevel - 1), 1.8));
	CONST QNumericalAward* pAward = g_cAwardSetting.RequestNumericalAward(PET_LEVEL_MODULE_NAME, m_nQuality, nNextLevel);
	if(!pAward)
	{
		printf("Can't find pet, quality:%d, nextLevel:%d\n", m_nQuality, nNextLevel);
	}
	QCONFIRM_RET_FALSE(pAward);

	return pAward->nCost;
}

DWORD KPet::GetTotalExp(INT nLevel)
{
	DWORD dwExp = 0;

	INT nTargetLevel = m_nLevel + 1;
	if (nLevel > 0)
	{
		nTargetLevel = nLevel + 1;
	}
	else
	{
		dwExp += m_dwExp;
	}

	for(INT i = 2; i < nTargetLevel; ++i)
	{
		dwExp += GetNextLvExp(i);
	}

	return dwExp;
}

BOOL KPet::ClearSkillLevel()
{
	m_mapSkillLevels.clear();
	return TRUE;
}
//获得总消耗的技术能书
DWORD KPet::GetTotalSkillBookNum()
{
	INT dwBookNum = 0;
	CONST QNumericalAward* pAward = NULL;

	for (INT i = 0; i < emPET_COUNT_SKILL; i++)
	{
		INT nSkillLevel = GetSkillLevel(GetSkillIdByIndex(i));
		if (nSkillLevel > 1)
		{
            dwBookNum += g_cAwardSetting.RequestAwardSum("pet_upskill", m_nQuality, 1, nSkillLevel);
		}
	}
	LOG_PROCESS_ERROR(dwBookNum >= 0);
	pAward = g_cAwardSetting.RequestNumericalAward("pet_upskill", m_nQuality, 1);
	LOG_PROCESS_ERROR(pAward);
	LOG_PROCESS_ERROR((dwBookNum - pAward->nCost) >= 0 );
	dwBookNum -= pAward->nCost;		// 减去本阶所需要的升阶丸
EXIT0:
	return (DWORD)dwBookNum;
}

//获得总消耗的升阶丸
DWORD KPet::GetTotalUpRankBallNum()
{
	INT dwUpRankBallNum = 0;
	CONST QNumericalAward* pAward = NULL;

	if (m_nRankLevel > 1)
	{
        dwUpRankBallNum += g_cAwardSetting.RequestAwardSum("pet_uprank", m_nQuality, 1, m_nRankLevel);
	}
	LOG_PROCESS_ERROR(dwUpRankBallNum >= 0);
	pAward = g_cAwardSetting.RequestNumericalAward("pet_uprank", m_nQuality, 1);
	LOG_PROCESS_ERROR(pAward);
	LOG_PROCESS_ERROR((dwUpRankBallNum - pAward->nCost) >= 0);
	dwUpRankBallNum -= pAward->nCost;
EXIT0:
	return (DWORD)dwUpRankBallNum;
}

BOOL KPet::AddExp(DWORD dwExp)
{
	DWORD dwCurrentExp;
	DWORD dwNewExp = m_dwExp + dwExp;
	SetExp(dwCurrentExp, dwNewExp);
	m_dwExp = dwCurrentExp;

	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
	QCONFIRM_RET_FALSE(pPlayer);

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);
	
	pPetCtrl->SyncPetExp(this);

	return TRUE;
}

BOOL KPet::SetExp(DWORD& rValue, DWORD dwNewValue)
{
	BOOL bLevelChanged = FALSE;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
	QCONFIRM_RET_FALSE(pPlayer);
	time_t tNow = KSysService::Time(NULL);
	INT nLevel = m_nLevel;
	KPlayerPurse *pPlayerPurse = NULL;
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	char szStr[1024];
	
	INT nNextLevel = nLevel + 1;
	DWORD nLvUpNeedExp = GetNextLvExp(nNextLevel);  // 下一级

	rValue = dwNewValue;
	while(rValue >= nLvUpNeedExp)
	{
		// 宠物等级不能超过主人等级
		if(nLevel >= pPlayer->GetLevel())
		{
			rValue = nLvUpNeedExp;
			break;
		}

		// 宠物升级
		nLevel = nNextLevel;
		rValue -= nLvUpNeedExp;
		nNextLevel = nLevel + 1;  // 判断再下一级能否满足升级
		nLvUpNeedExp = GetNextLvExp(nNextLevel);
		bLevelChanged = TRUE;
	}

	if(bLevelChanged)
	{
		// 神仙技能等级随神仙等级提升而提升
		/*std::map<INT, INT>::iterator it;
		for (it = m_mapSkillLevels.begin(); it != m_mapSkillLevels.end(); ++it)
		{
			it->second = nLevel;
		}*/

		//
		QLogPrintf(LOG_INFO,"pet pet up level and modify the rank of pet");
		RankRelayClient::DoModifyRankValueRequest(emRANK_PET_LEVEL, pPlayer->GetId(), nLevel, tNow);
		RankRelayClient::DoModifyRankValueRequest(emRANK_PET_LV_OS, pPlayer->GetId(), nLevel, tNow);
		return Regenerate(m_nType, 
			m_nQuality, 
			m_nCharacter,
			nLevel, 
			m_nRankLevel, 
			m_dwRandSeed);
		
		//统计点 ldy
		pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
		QCONFIRM_RET_FALSE(pPlayerPurse);
		ZeroMemory(szStr,sizeof(szStr));
		sprintf(szStr,"nType=%d,nQuality=%d,nCharacter=%d,nLevel=%d,m_nRankLevel=%d",m_nType,m_nQuality,m_nCharacter,nLevel,m_nRankLevel);
		pPlayerPurse->LogMoney(EM_MONEY_COIN,0,61,&nResult,1,szStr);
		
	}
	return TRUE;
}

BOOL KPet::Init_UprankAttrib()
{
	CONST QNumericalAward* pBaseNumAward = g_cAwardSetting.RequestNumericalAward(PET_UPRANK_MODULE_NAME, m_nQuality, m_nRankLevel);
	QCONFIRM_RET_FALSE(pBaseNumAward);

	// 这里的nLife实际上是缩放比例
	m_aryBaseAttr[emPET_ATTR_LIFE].nValue = (pBaseNumAward->nLife * m_aryBaseAttr[emPET_ATTR_LIFE].nValue) / 100;

	m_aryBaseAttr[emPET_ATTR_ATTACK].nValue = (pBaseNumAward->nLife * m_aryBaseAttr[emPET_ATTR_ATTACK].nValue) / 100;

	m_aryBaseAttr[emPET_ATTR_HIT].nValue = (pBaseNumAward->nLife * m_aryBaseAttr[emPET_ATTR_HIT].nValue) / 100;

	m_aryBaseAttr[emPET_ATTR_CRIT].nValue = (pBaseNumAward->nLife * m_aryBaseAttr[emPET_ATTR_CRIT].nValue) / 100;

	m_aryBaseAttr[emPET_ATTR_RESIST].nValue = (pBaseNumAward->nLife * m_aryBaseAttr[emPET_ATTR_RESIST].nValue) / 100;

	m_aryBaseAttr[emPET_ATTR_DEFENSE].nValue = (pBaseNumAward->nLife * m_aryBaseAttr[emPET_ATTR_DEFENSE].nValue) / 100;

	m_aryBaseAttr[emPET_ATTR_DODGE].nValue = (pBaseNumAward->nLife * m_aryBaseAttr[emPET_ATTR_DODGE].nValue) / 100;

	return TRUE;
}

/*
case emPET_CHARACTER_ATTACK:
nAttribIndex = emPET_ATTR_ATTACK;
Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("modifydmg_v");
break;
case emPET_CHARACTER_DEF:
nAttribIndex = emPET_ATTR_DEFENSE;
Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("adddef_v");
break;
case emPET_CHARACTER_HIT:
nAttribIndex = emPET_ATTR_HIT;
Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("addhitrate_v");
break;
case emPET_CHARACTER_DODGE:
nAttribIndex = emPET_ATTR_DODGE;
Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("adddodge_v");
break;
case emPET_CHARACTER_CRIT:
nAttribIndex = emPET_ATTR_CRIT;
Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("addcriticalrate_v");
break;
case emPET_CHARACTER_RESIST:
nAttribIndex = emPET_ATTR_RESIST;
Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("addresist_v");
break;
case emPET_CHARACTER_LIFE:
nAttribIndex = emPET_ATTR_LIFE;
Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("addmaxlife_v");
*/

BOOL KPet::Init_EquipAttrib()
{
    for (INT i = 0; i < emPET_COUNT_EQUIP; i++)
	{
		if (!m_aryEquip[i].byLock && m_aryEquip[i].nParticular > 0)
		{
            CONST QNumericalAward* pEquipNumAward = g_cAwardSetting.RequestNumericalAward("PetEquip", 1, (INT)m_aryEquip[i].wNumberIndex);
			LOG_PROCESS_ERROR(pEquipNumAward);

			m_aryBaseAttr[emPET_ATTR_LIFE].nValue += pEquipNumAward->nLife;
			//QLogPrintf(LOG_INFO,"..Init_EquipAttrib m_aryBaseAttr[emPET_ATTR_LIFE].nValue=%d",m_aryBaseAttr[emPET_ATTR_LIFE].nValue);

			m_aryBaseAttr[emPET_ATTR_ATTACK].nValue += pEquipNumAward->nAttack;

			m_aryBaseAttr[emPET_ATTR_HIT].nValue += pEquipNumAward->nHit;

			m_aryBaseAttr[emPET_ATTR_CRIT].nValue += pEquipNumAward->nCrit;

			m_aryBaseAttr[emPET_ATTR_RESIST].nValue += pEquipNumAward->nResist;

			m_aryBaseAttr[emPET_ATTR_DEFENSE].nValue += pEquipNumAward->nDefense;

			m_aryBaseAttr[emPET_ATTR_DODGE].nValue += pEquipNumAward->nDodge;
		}
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL KPet::Init_BaseAttrib()
{
	// 读取各项属性的缩放比例
	CONST QNumericalAward* pBaseNumAward = g_cAwardSetting.RequestNumericalAward(PET_BASE_MODULE_NAME, 1, m_nCharacter);
	QCONFIRM_RET_FALSE(pBaseNumAward);

	/*m_aryBaseAttr[emPET_ATTR_LIFE].nAttrib = g_cMagicAttribMgr.GetIdFormString("addmaxlife_v");
	m_aryBaseAttr[emPET_ATTR_LIFE].nValue = pBaseNumAward->nLife * nBaseAttrPercent / 1000;

	m_aryBaseAttr[emPET_ATTR_ATTACK].nAttrib = g_cMagicAttribMgr.GetIdFormString("modifydmg_v");
	m_aryBaseAttr[emPET_ATTR_ATTACK].nValue = pBaseNumAward->nAttack * nBaseAttrPercent / 1000;

	m_aryBaseAttr[emPET_ATTR_HIT].nAttrib = g_cMagicAttribMgr.GetIdFormString("addhitrate_v");
	m_aryBaseAttr[emPET_ATTR_HIT].nValue = pBaseNumAward->nHit * nBaseAttrPercent / 1000;

	m_aryBaseAttr[emPET_ATTR_CRIT].nAttrib = g_cMagicAttribMgr.GetIdFormString("addcriticalrate_v");
	m_aryBaseAttr[emPET_ATTR_CRIT].nValue = pBaseNumAward->nCrit * nBaseAttrPercent / 1000;

	m_aryBaseAttr[emPET_ATTR_RESIST].nAttrib = g_cMagicAttribMgr.GetIdFormString("addresist_v");
	m_aryBaseAttr[emPET_ATTR_RESIST].nValue = pBaseNumAward->nResist * nBaseAttrPercent / 1000;

	m_aryBaseAttr[emPET_ATTR_DEFENSE].nAttrib = g_cMagicAttribMgr.GetIdFormString("adddef_v");
	m_aryBaseAttr[emPET_ATTR_DEFENSE].nValue = pBaseNumAward->nDefense * nBaseAttrPercent / 1000;

	m_aryBaseAttr[emPET_ATTR_DODGE].nAttrib = g_cMagicAttribMgr.GetIdFormString("adddodge_v");
	m_aryBaseAttr[emPET_ATTR_DODGE].nValue = pBaseNumAward->nDodge * nBaseAttrPercent / 1000;*/

	CONST QNumericalAward* pLevelNumAward = g_cAwardSetting.RequestNumericalAward(PET_LEVEL_MODULE_NAME, m_nQuality, m_nLevel);
	QCONFIRM_RET_FALSE(pLevelNumAward);

	m_aryBaseAttr[emPET_ATTR_LIFE].nAttrib = g_cMagicAttribMgr.GetIdFormString("addmaxlife_v");
	m_aryBaseAttr[emPET_ATTR_LIFE].nValue = 1LL * pLevelNumAward->nLife * pBaseNumAward->nLife / 1000LL;
	//QLogPrintf(LOG_INFO,"..m_aryBaseAttr[emPET_ATTR_LIFE].nValue=%d",m_aryBaseAttr[emPET_ATTR_LIFE].nValue);

	m_aryBaseAttr[emPET_ATTR_ATTACK].nAttrib = g_cMagicAttribMgr.GetIdFormString("modifydmg_v");
	m_aryBaseAttr[emPET_ATTR_ATTACK].nValue = 1LL *  pLevelNumAward->nAttack * pBaseNumAward->nAttack / 1000LL;

	m_aryBaseAttr[emPET_ATTR_HIT].nAttrib = g_cMagicAttribMgr.GetIdFormString("addhitrate_v");
	m_aryBaseAttr[emPET_ATTR_HIT].nValue = 1LL *  pLevelNumAward->nHit * pBaseNumAward->nHit / 1000LL;

	m_aryBaseAttr[emPET_ATTR_CRIT].nAttrib = g_cMagicAttribMgr.GetIdFormString("addcriticalrate_v");
	m_aryBaseAttr[emPET_ATTR_CRIT].nValue = 1LL *  pLevelNumAward->nCrit * pBaseNumAward->nCrit / 1000LL;

	m_aryBaseAttr[emPET_ATTR_RESIST].nAttrib = g_cMagicAttribMgr.GetIdFormString("addresist_v");
	m_aryBaseAttr[emPET_ATTR_RESIST].nValue = 1LL * pLevelNumAward->nResist * pBaseNumAward->nResist / 1000LL;

	m_aryBaseAttr[emPET_ATTR_DEFENSE].nAttrib = g_cMagicAttribMgr.GetIdFormString("adddef_v");
	m_aryBaseAttr[emPET_ATTR_DEFENSE].nValue = 1LL * pLevelNumAward->nDefense * pBaseNumAward->nDefense / 1000LL;

	m_aryBaseAttr[emPET_ATTR_DODGE].nAttrib = g_cMagicAttribMgr.GetIdFormString("adddodge_v");
	m_aryBaseAttr[emPET_ATTR_DODGE].nValue = 1LL * pLevelNumAward->nDodge * pBaseNumAward->nDodge / 1000LL;

	return TRUE;
}

BOOL KPet::Get_TacticAttrib(INT nQuality, INT nRankLevel, KBASEATTR& Attr, BOOL bIsBlade /* = FALSE */)
{
	BOOL 				   	bResult 		= FALSE;
	CONST QNumericalAward* 	pNumAward 		= NULL;
	CONST QNumericalAward* 	pBaseNumAward 	= NULL;
	INT 					nAttribIndex 	= 0;
   
	// 放入阵法后，把神仙本身的属性按照一定比例加成给主角
	switch((KE_PET_CHARACTER)m_nCharacter)
	{
	case emPET_CHARACTER_ATTACK:
		nAttribIndex = emPET_ATTR_ATTACK;
		Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("modifydmg_v");
		break;
	case emPET_CHARACTER_DEF:
		nAttribIndex = emPET_ATTR_DEFENSE;
		Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("adddef_v");
		break;
	case emPET_CHARACTER_HIT:
		nAttribIndex = emPET_ATTR_HIT;
		Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("addhitrate_v");
		break;
	case emPET_CHARACTER_DODGE:
		nAttribIndex = emPET_ATTR_DODGE;
		Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("adddodge_v");
		break;
	case emPET_CHARACTER_CRIT:
		nAttribIndex = emPET_ATTR_CRIT;
		Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("addcriticalrate_v");
		break;
	case emPET_CHARACTER_RESIST:
		nAttribIndex = emPET_ATTR_RESIST;
		Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("addresist_v");
		break;
	case emPET_CHARACTER_LIFE:
		nAttribIndex = emPET_ATTR_LIFE;
		Attr.nAttrib = g_cMagicAttribMgr.GetIdFormString("addmaxlife_v");
		break;
	default:
		ASSERT(FALSE);
	}

	if(!bIsBlade)
	{
		pNumAward = g_cAwardSetting.RequestNumericalAward(PET_TACTIC_MODULE_NAME, 1, nAttribIndex);
	}
	else
	{
		pNumAward = g_cAwardSetting.RequestNumericalAward(PET_BLADE_MODULE_NAME, 1, nAttribIndex);
	}
	LOG_PROCESS_ERROR(pNumAward);
	pBaseNumAward = g_cAwardSetting.RequestNumericalAward(PET_UPRANK_MODULE_NAME, m_nQuality, m_nRankLevel);
	LOG_PROCESS_ERROR(pBaseNumAward);
	LOG_PROCESS_ERROR(GetBaseAttrValue(nAttribIndex) > 0);
	LOG_PROCESS_ERROR(pNumAward->nLife > 0);
	LOG_PROCESS_ERROR(pBaseNumAward->nLife > 0);
	LOG_PROCESS_ERROR((1LL *  GetBaseAttrValue(nAttribIndex) * pNumAward->nLife * pBaseNumAward->nLife / 10000LL)  > 0);
	Attr.nValue = (1LL *  GetBaseAttrValue(nAttribIndex) * pNumAward->nLife * pBaseNumAward->nLife / 10000LL);
	//Q_Printl("..Attr.nValue="<< Attr.nValue);
	bResult = TRUE;
EXIT0:
	return bResult;
}

// 根据宠物性格得到阵法属性
BOOL KPet::Init_TacticAttrib()
{
	Get_TacticAttrib(m_nQuality, m_nRankLevel, m_TacticAttr);

	ASSERT(m_TacticAttr.nAttrib && m_TacticAttr.nValue);

	return TRUE;
}

VOID KPet::SetTactic(INT nCharacter)
{
	ASSERT(nCharacter >= 0 && nCharacter <= emPET_CHARACTER_DODGE);
	m_nTactic = nCharacter;
}

INT KPet::GetEquip(BYTE byPos)
{
	QCONFIRM_RET_NULL(byPos >= 0 && byPos < m_nEquipSlot);

	return m_aryEquip[byPos].nParticular;
}

WORD KPet::GetEquipNumberIndex(BYTE byPos)
{
    QCONFIRM_RET_NULL(byPos >= 0 && byPos < m_nEquipSlot);

	return m_aryEquip[byPos].wNumberIndex;
}

BOOL KPet::HasEquip(BYTE byPos)
{
	QCONFIRM_RET_FALSE(byPos >= 0 && byPos < m_nEquipSlot);

	return m_aryEquip[byPos].nParticular > 0;
}

BOOL KPet::UnLockEquipPos(INT nPos)
{
	QCONFIRM_RET_FALSE(nPos >= 0 && nPos < m_nEquipSlot);
    m_aryEquip[nPos].byLock = 0;
	Regenerate(m_nType, m_nQuality, m_nCharacter, m_nLevel, m_nRankLevel, m_dwRandSeed);
	return TRUE;
}

BOOL KPet::IsEquipLocked(BYTE byPos)
{
	QCONFIRM_RET_FALSE(byPos >= 0 && byPos < m_nEquipSlot);

	return m_aryEquip[byPos].byLock;
}

BOOL KPet::PutOnEquip(BYTE byPos, INT nParticular, INT nNumberIndex)
{
	QCONFIRM_RET_FALSE(byPos >= 0 && byPos < m_nEquipSlot);
	QCONFIRM_RET_FALSE(!HasEquip(byPos));

	m_aryEquip[byPos].nParticular = nParticular;
	m_aryEquip[byPos].wNumberIndex = (WORD)nNumberIndex;

	Regenerate(m_nType, m_nQuality, m_nCharacter, m_nLevel, m_nRankLevel, m_dwRandSeed);

	return TRUE;
}

BOOL KPet::TakeOffEquip(BYTE byPos)
{
	QCONFIRM_RET_FALSE(byPos >= 0 && byPos < m_nEquipSlot);
	QCONFIRM_RET_FALSE(HasEquip(byPos));

	m_aryEquip[byPos].nParticular = 0;
	m_aryEquip[byPos].wNumberIndex = 0;

	Regenerate(m_nType, m_nQuality, m_nCharacter, m_nLevel, m_nRankLevel, m_dwRandSeed);
	
	return TRUE;
}

BOOL KPet::CopyPetData(CONST KPet* pPet)
{
	if (!pPet)
		return	FALSE;

	KLuaPet* pLuna = m_pLuna;
	*this = *pPet;			// 往该类里增加成员要注意根据情况重载=号
	m_pLuna	= pLuna;
	m_dwOwnerId = 0;
	return TRUE;
}

// 根据品质得到最大阶数
INT KPet::GetMaxRank()
{
	ASSERT(m_nQuality >= 1 && m_nQuality <= KD_PET_MAX_QUALITY);

	INT nMaxRank = g_cAwardSetting.GetLevelNumOfStage(PET_UPRANK_MODULE_NAME, m_nQuality);
	return nMaxRank;
}

BOOL KPet::Gen_Pet(KPET_INDEX sIndex,
					INT nLevel,
					INT nRankLevel,
					UINT uRandSeed)
{
	KPetTemplate* pPetTemplate = g_cPetSetting.GetPetTemplate(sIndex.nType);
	QCONFIRM_RET_FALSE(pPetTemplate);
	m_pPetTemplate = pPetTemplate;

	m_nType = sIndex.nType;
	m_nQuality = sIndex.nQuality;
	m_nCharacter = sIndex.nCharacter;
	m_nLevel = nLevel;
	m_nRankLevel = nRankLevel;
	m_dwTargetId = 0;
	m_nCurrentSkillId = 0;
	m_nCurrentCastDelay = 0;
	//m_byHungry = (uRandSeed <= 0) ? 1 : uRandSeed;
	m_byHungry = KD_PET_HUNGRY_MAX;
	m_nHungryConsume = pPetTemplate->m_nHungry;

	ASSERT(nRankLevel <= GetMaxRank());

	// 暂时不需要随机，先屏蔽
	/*
	if (!uRandSeed)
	{	// 打散随机种子
		g_Random(-1);
		uRandSeed = g_GetRandomSeed();
	}
	m_dwRandSeed = uRandSeed;
	m_cRandom.SetSeed(uRandSeed);	// 使用随机种子
	*/

	QCONFIRM_RET_FALSE(Init_BaseAttrib());

	QCONFIRM_RET_FALSE(Init_EquipAttrib());

	QCONFIRM_RET_FALSE(Init_Skills());

	QCONFIRM_RET_FALSE(Init_UprankAttrib());

	QCONFIRM_RET_FALSE(Init_TacticAttrib());

	return TRUE;
}

BOOL KPet::Regenerate(INT nType,
				INT nQuality,
				INT nCharacter,
				INT nLevel, 
				INT nRankLevel,
				UINT uRandSeed)
{

	KPet cNewPet;
	if (!cNewPet.CopyPetData(this))
		return	FALSE;

	KPET_INDEX sIndex = { nType, nQuality, nCharacter };

	if(!cNewPet.Gen_Pet(sIndex, nLevel, nRankLevel, m_byHungry))
		return FALSE;

	INT nPos = -1;
	KPlayer* pOwner = m_dwOwnerId ? g_cPlayerMgr.GetById(m_dwOwnerId) : NULL;
	KPetCtrl* pPetCtrl = pOwner ? pOwner->GetCtrl<KPetCtrl>() : NULL;

	if (pPetCtrl)
	{
		if((nPos = pPetCtrl->GetPetPos(m_dwId)) < 0)
			return FALSE;
		if (!pPetCtrl->RemoveOnly(nPos))		// 先将原宠物从人身上临时删除
			return FALSE;
	}

	KPet cOldPet;
	cOldPet.CopyPetData(this);	// 备份原宠物
	CopyPetData(&cNewPet);		// 使用新宠物

	if (pPetCtrl && !pPetCtrl->Insert(this, nPos))		// 添加失败，恢复原宠物
	{
		CopyPetData(&cOldPet);
		if(!pPetCtrl->Insert(this, nPos))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	if(GetPetState() == emPET_STATE_FIGHT)
	{
		g_cCharacterHelper.CalcFightScore(*pOwner);
	}

	CalcFightScore();

	if (m_nTactic > 0)
	{
		KBladeCtrl* pBladeCtrl = pOwner->GetCtrl<KBladeCtrl>();
		QCONFIRM_RET_FALSE(pBladeCtrl);

		if(pOwner->GetAngerLeftTime() > 0)
		{
			pBladeCtrl->RemoveBladeAttr();
		}

		pPetCtrl->RemoveTacticAttr(&cOldPet);
		pPetCtrl->ApplyTacticAttr(this);

		if(pOwner->GetAngerLeftTime() > 0)
		{
			pBladeCtrl->ApplyBladeAttr();
		}
	}

	pPetCtrl->SyncPetInfo(this);

	return TRUE;
}

BOOL KPet::ClearExp()
{
	m_dwExp = 0;
	return TRUE;
}

BOOL KPet::SetHungry(INT nHungry)
{
	QCONFIRM_RET_FALSE(nHungry >= 0 && nHungry <= KD_PET_HUNGRY_MAX);
	//m_byHungry = nHungry;
	m_byHungry = KD_PET_HUNGRY_MAX;//去掉饥饿度
	return TRUE;
}

INT KPet::GetBaseAttrValue(INT nIdx)
{
	QCONFIRM_RET_FALSE(nIdx >= 0 && nIdx < emPET_COUNT_BASE);
	return m_aryBaseAttr[nIdx].nValue;
}

INT KPet::GetPetSkillScore()
{ 
	INT nScore = 0;
	SKILL_DATA_MAP::iterator it;
	for (it = m_mapSkill.begin(); it != m_mapSkill.end(); ++it)
	{
		KSKILL_DATA& rSkillData = (*it).second;

		BufferNumber* pStartBufferNumber = g_cBufferSetting.GetBufferNumber(rSkillData.nBuffId, 1, GetMagicEffectStrBySkillType(rSkillData.nSkillType));
		LOG_PROCESS_ERROR(pStartBufferNumber);

		BufferNumber* pCurrBufferNumber = g_cBufferSetting.GetBufferNumber(rSkillData.nBuffId, rSkillData.nSkillLevel, GetMagicEffectStrBySkillType(rSkillData.nSkillType));
		LOG_PROCESS_ERROR(pCurrBufferNumber);

		INT cofficientDif = (pCurrBufferNumber->m_nSkillCoefficientMin - pStartBufferNumber->m_nSkillCoefficientMin) * GetBaseAttrValue(emPET_ATTR_ATTACK) / 1000;
		INT nHarmDif = abs(pCurrBufferNumber->m_nValue - pStartBufferNumber->m_nValue);

		nScore += (nHarmDif + cofficientDif) * g_cPetSetting.GetPetAttribScore(m_aryBaseAttr[emPET_ATTR_ATTACK].nAttrib); //以攻击的分数评分计算
	}

EXIT0:
	return nScore;
}

INT KPet::GetBaseAttrScore()
{
	INT nAttrib = 0;
	INT nScore = 0;
	INT nValue = 0;
	INT nFightScore = 0;

	for(INT i = 0; i < emPET_COUNT_BASE; ++i)
	{
		nAttrib = m_aryBaseAttr[i].nAttrib;
		nScore = g_cPetSetting.GetPetAttribScore(nAttrib);
		nValue = GetBaseAttrValue(i);
		nFightScore += nScore * nValue;
	}

	nFightScore = nFightScore / 100;

	return nFightScore;
}

BOOL KPet::CalcFightScore()
{
	INT nScore = 0;

	nScore += GetBaseAttrScore();
	nScore += GetPetSkillScore();

	SetFightScore(nScore);

	return TRUE;
}

// 根据宠物等级得到当前技能等级
INT KPet::GetSkillLevel(INT nSkillId)
{
    if (m_mapSkillLevels.find(nSkillId) == m_mapSkillLevels.end())
	{
		m_mapSkillLevels[nSkillId] = 1;
	}

	return m_mapSkillLevels[nSkillId];
}

BOOL KPet::SetSkillValues(INT nSkillId, KSKILL_DATA& sSkillData)
{
	memset((VOID*)sSkillData.nSkillValues, 0, sizeof(sSkillData.nSkillValues));
	BufferNumber* pBufferNumber = g_cBufferSetting.GetBufferNumber(sSkillData.nBuffId, m_mapSkillLevels[nSkillId], GetMagicEffectStrBySkillType(sSkillData.nSkillType));
	QCONFIRM_RET_FALSE(pBufferNumber);
	sSkillData.nSkillValues[0] = pBufferNumber->m_nSkillCoefficientMin;
	sSkillData.nSkillValues[1] = pBufferNumber->m_nSkillCoefficientMax;
	sSkillData.nSkillValues[2] = pBufferNumber->m_nValue;

	BufferNumber* pNextBufferNumber = g_cBufferSetting.GetBufferNumber(sSkillData.nBuffId, m_mapSkillLevels[nSkillId] + 1, GetMagicEffectStrBySkillType(sSkillData.nSkillType));
	if (pNextBufferNumber)
	{
		sSkillData.nSkillValues[3] = pNextBufferNumber->m_nSkillCoefficientMin;
		sSkillData.nSkillValues[4] = pNextBufferNumber->m_nSkillCoefficientMax;
		sSkillData.nSkillValues[5] = pNextBufferNumber->m_nValue;
	}

	return FALSE;
}

CONST CHAR* KPet::GetMagicEffectStrBySkillType(INT nSkillType)
{
	switch (nSkillType)
	{
	case emPET_NORMAL_ATTACK:
		return "me_pet_damagelife_v";
	case emPET_BUFF_ADD_BLOOD:
		return "me_pet_addselflife";
	case emPET_BUFF_SUB_BLOOD:
		return "me_pet_subtargetlife";
	case emPET_BUFF_ADD_ATTACK:
		return "me_pet_addselfattack";
	case emPET_BUFF_SUB_DEFENCE:
		return "me_pet_subtargetdefence";
	}
	return "";
}

BOOL KPet::Init_Skills()
{
	KSKILL_DATA sData;
	INT nSkillNum = emPET_COUNT_SKILL;

	m_mapSkill.clear();
	m_arrSkills.clear();
	INT arrPetSkills[] = {m_pPetTemplate->m_PetSkill1, m_pPetTemplate->m_PetSkill2};

	for(INT i = 0; i < nSkillNum; ++i)
	{
		INT nTemplateId = arrPetSkills[i];//(INT)g_aePetSkill[i]		

		KPetSkillTemplate* pSkillTemplate = g_cPetSetting.GetPetSkillTemplate(nTemplateId);
		QCONFIRM_RET_FALSE(pSkillTemplate);

		INT nSkillId = pSkillTemplate->m_arySkillId[0];
		INT nSkillLevel = GetSkillLevel(nSkillId);
		ASSERT(nSkillLevel > 0 && nSkillLevel <= GetLevel());

        FightSkillTemplate* pFightSkillTemplate = g_cFightSkillSetting.GetFightSkillTemplate((WORD)nSkillId);
		QCONFIRM_RET_FALSE(pFightSkillTemplate);

		sData.nBuffId = pSkillTemplate->m_aryBuffId[0];
		sData.nSkillType = pSkillTemplate->m_nSkillType;
		sData.bToSelf = pSkillTemplate->m_bToSelf;
		sData.nCDTime = pSkillTemplate->m_nSkillCD;
		sData.nCurrentCDTime = pSkillTemplate->m_nSkillCD;
		sData.nSkillLevel = nSkillLevel;
		sData.bStartCalcCD = FALSE;

		SetSkillValues(nSkillId, sData);

		m_mapSkill[nSkillId] = sData;

		m_arrSkills.push_back(nSkillId);
	}

	return TRUE;
}

INT KPet::GetSkillIdByIndex(INT nIndex)
{
    _ASSERT(nIndex < (INT)m_arrSkills.size());
	return m_arrSkills[nIndex];
}

INT KPet::GetSkillValue(INT nSkillId, INT nValueIndex)
{
	ASSERT(nValueIndex < emPET_COUNT_VALUE * 2);

	if (m_mapSkill.find(nSkillId) != m_mapSkill.end())
	{
		KSKILL_DATA data = m_mapSkill[nSkillId];
		return data.nSkillValues[nValueIndex];
	}

	return 0;
}

BOOL KPet::IsSkillInCD(INT nSkillId)
{
	SKILL_DATA_MAP::iterator it = m_mapSkill.find(nSkillId);
	QCONFIRM_RET_FALSE(it != m_mapSkill.end());

	if (it->second.nCDTime != it->second.nCurrentCDTime)
	{
		return TRUE;
	}

	return FALSE;
}

VOID KPet::SetPosition(INT nX, INT nY)
{
	m_nX = nX;
	m_nY = nY;
}

// 判断宠物与目标间的距离
BOOL KPet::CheckCastDistance()
{
	BOOL bRet = FALSE;
	KCharacter* pTarget = NULL;
	KPlayer* pPlayer = NULL;
	INT nX, nY, nZ;
	INT nTX, nTY, nTZ;
	INT nDistance = 0;

	PROCESS_ERROR(m_dwTargetId);

	pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
	LOG_PROCESS_ERROR(pPlayer);

	pTarget = g_cCharacterMgr.GetById(m_dwTargetId);
	PROCESS_ERROR(pTarget);

	PROCESS_ERROR(pTarget->GetSceneId() == pPlayer->GetSceneId());

	pPlayer->GetPosition(nX, nY, nZ);

	pTarget->GetPosition(nTX, nTY, nTZ);

	nDistance = g_GetDistance2(nX, nY, nTX, nTY);

	PROCESS_ERROR(nDistance <= KD_PET_CAST_DISTANCE * KD_PET_CAST_DISTANCE);

	bRet = TRUE;
EXIT0:
	return bRet;
}

BOOL KPet::GetCastSkillId()
{
	INT nSkillId = 0;
	INT nCastSkillId = 0;

	if(m_nCurSkillIndex >= (INT)m_arrSkills.size())
		m_nCurSkillIndex = 0;

	nCastSkillId = m_arrSkills[0];

	for (INT i = (INT)m_arrSkills.size() - 1; i >= 0; i--)
	{
        nSkillId = m_arrSkills[i];
		if (!IsSkillInCD(nSkillId))
		{
            nCastSkillId = nSkillId;
			break;
		}
	}

EXIT0:
	return nCastSkillId;
}

BOOL KPet::UpgradeSkill(INT nSkillId)
{
	if (m_mapSkillLevels.find(nSkillId) != m_mapSkillLevels.end())
	{
		m_mapSkillLevels[nSkillId]++;
		return Regenerate(m_nType, m_nQuality, m_nCharacter, m_nLevel, m_nRankLevel, m_dwRandSeed);
	}

	return FALSE;
}

INT KPet::GetBuffIdBySkillId(INT nSkillId)
{
	SKILL_DATA_MAP::iterator it;

	it = m_mapSkill.find(m_nCurrentSkillId);
	LOG_PROCESS_ERROR(it != m_mapSkill.end());

	return it->second.nBuffId;
EXIT0:
	return 0;
}

BOOL KPet::CastSkill()
{
	BOOL bResult = FALSE;
	INT nBuffIdx = 0;
	KPlayer* pPlayer = NULL;
	KCharacter* pTarget = NULL;
	SKILL_DATA_MAP::iterator it;

	LOG_PROCESS_ERROR(GetPetState() == emPET_STATE_FIGHT);

	pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
	LOG_PROCESS_ERROR(pPlayer);

	it = m_mapSkill.find(m_nCurrentSkillId);
	LOG_PROCESS_ERROR(it != m_mapSkill.end());

	if(it->second.bToSelf)
	{
		int nPersist = it->second.nSkillValues[0];
		if (nPersist > 0)
		{
			nBuffIdx = g_cBufferMgr.AddBufferToCharacter(pPlayer->GetId(), m_nCurrentSkillId, pPlayer->GetId(), it->second.nBuffId, 
				it->second.nSkillValues[0], TRUE, 0, 0, 0, GetSkillLevel(m_nCurrentSkillId));
		}
		
	}
	else
	{
		PROCESS_ERROR(m_dwTargetId);

		pTarget = g_cCharacterMgr.GetById(m_dwTargetId);
		PROCESS_ERROR(pTarget);

		//PROCESS_ERROR(CheckCastDistance());

		nBuffIdx = g_cBufferMgr.AddBufferToCharacter(pPlayer->GetId(), m_nCurrentSkillId, m_dwTargetId, it->second.nBuffId,
			0, TRUE, 0, 0, 0, GetSkillLevel(m_nCurrentSkillId));
	}	

	bResult = TRUE;
EXIT0:
	it->second.bStartCalcCD = TRUE;
	m_nCurrentCastDelay = 0;
	m_dwTargetId = 0; // 清空目标
	return bResult;
}

VOID KPet::GetSkillIds(std::vector<INT>& vecSkillIds)
{
	vecSkillIds.clear();
	for (INT i = 0; i < (INT)m_arrSkills.size(); i++)
	{
	    vecSkillIds.push_back(m_arrSkills[i]);
	}

	ASSERT(vecSkillIds.size() <= emPET_COUNT_SKILL);
}

BOOL KPet::Breath()
{
	KPlayer* pPlayer = NULL;
	KPetCtrl* pPetCtrl = NULL;
	IKScene* pScene = NULL;

	pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
	LOG_PROCESS_ERROR(pPlayer);

	pScene = pPlayer->GetScene();
	LOG_PROCESS_ERROR(pScene);

	// 战斗地图才能释放技能
	PROCESS_ERROR(g_SceneMgr.IsFightMap(pScene->GetTemplateId()));

	PROCESS_ERROR(GetPetState() == emPET_STATE_FIGHT);

	for(SKILL_DATA_MAP::iterator iter = m_mapSkill.begin(); iter != m_mapSkill.end(); ++iter)
	{
		if(iter->second.bStartCalcCD)
		{
			iter->second.nCurrentCDTime--;	//计算CD时间
			if (iter->second.nCurrentCDTime <= 0)
			{
				iter->second.nCurrentCDTime = iter->second.nCDTime;  // 完成冷却时间，恢复到需要冷却的时间
				iter->second.bStartCalcCD = FALSE;
			}
		}
	}

	if(g_cOrpgWorld.m_dwLogicFrames % PET_CAST_SKILL_FRAME == 0 && m_dwTargetId != 0)
	{
		KCharacter* pTarget = g_cCharacterMgr.GetById(m_dwTargetId);
		if(pTarget == NULL || pTarget->GetMoveState() == cmsOnDeath)
		{
			m_dwTargetId = 0;
			m_nCurrentCastDelay = 0;
			goto EXIT0;
		}

		pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		LOG_PROCESS_ERROR(pPetCtrl);
		DWORD dwMissileTime = 0;

		if(!m_nCurrentSkillId)
		{
			FightSkillTemplate* pTemplate = NULL;
			INT nTargetX = 0, nTargetY = 0, nTargetZ = 0;
			INT nStartDistance2 = 0;

			m_nCurrentSkillId = GetCastSkillId();
			PROCESS_ERROR(m_nCurrentSkillId);

			pTemplate = g_cFightSkillSetting.GetFightSkillTemplate(m_nCurrentSkillId);
			LOG_PROCESS_ERROR(pTemplate);

			m_nCurrentCastDelay = (INT)(pTemplate->nTimeBeforeCast * GAME_FPS) + 1;

			if (pTemplate->nFightSkillType == FIGHT_SKILL_MISSILE)
			{
            	pTarget->GetPosition(nTargetX, nTargetY, nTargetZ);

				// 神仙在角色身后4米
				nStartDistance2 = g_GetDistance2(m_nX, m_nY, nTargetX, nTargetY);
				DWORD dwSpeed = (DWORD)(pTemplate->fMissileSpeed * CELL_LENGTH / (DWORD)GAME_FPS); 
				dwMissileTime = (DWORD)(sqrtf((FLOAT)nStartDistance2) / dwSpeed); 
				
				m_nCurrentCastDelay += dwMissileTime;
				m_nWaitAfterCast = 0;
			}
			else
			{
				SKILL_DATA_MAP::iterator it = m_mapSkill.find(m_nCurrentSkillId);
				LOG_PROCESS_ERROR(it != m_mapSkill.end());

				if (!it->second.bToSelf)
				{
					KCharacter* pTarget = g_cCharacterMgr.GetById(m_dwTargetId);
					if(pTarget == NULL || pTarget->GetMoveState() == cmsOnDeath)
					{
						m_dwTargetId = 0;
						m_nCurrentCastDelay = 0;
						goto EXIT0;
					}
				}

				m_nWaitAfterCast = 16; // 加完buff后等待一点时间，免得客户端表现过于急促
			}

			pPetCtrl->BroadcastCastSkill(this, m_nCurrentSkillId, (INT)m_dwTargetId, (INT)dwMissileTime);
		}
	}

	if (m_nCurrentSkillId != 0)
	{
		if (m_nCurrentCastDelay > 0)
		{
			m_nCurrentCastDelay--;
			if(m_nCurrentCastDelay <= 0)
			{
				CastSkill();
				goto EXIT0;
			}
		}
		else
		{
			m_nWaitAfterCast--;
			if (m_nWaitAfterCast <= 0)
			{
				m_nCurrentSkillId = 0;
				m_nCurrentCastDelay = 0;
				m_nWaitAfterCast = 0;
			}
		}
	}

EXIT0:
	return TRUE;
}