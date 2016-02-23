
#include "stdafx.h"
#include "klibofbpt.h"
#include "kitem.h"
#include "kplayeritem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

BOOL KLibOfBPT::Init(INT nVersion)
{
	// 初始化
	KBasicPropertyTable *aryBPT[] =
	{
		&m_cGE_Sword,
		&m_cGE_Spear,
		&m_cGE_Necklace,
		&m_cGE_Ring,
		&m_cGE_Pendant,
		&m_cGE_Head,
		&m_cGE_Armor,
		&m_cGE_Belt,
		&m_cGE_Cloak,
		&m_cGE_Foot,

		&m_cGE_Suit,
		&m_cGE_Bottom,
		&m_cGE_Back,

		&m_cStone,
		&m_cMedicine,
		&m_cScript,
		&m_cRide,
		&m_cPetItem,
		&m_cPetEquip,
		&m_cUpgradeMaterial,
		&m_cBox,
	};

	for (INT i = 0; i < countof(aryBPT); i++)
		VERIFY(aryBPT[i]->Load(nVersion));

	return TRUE;
}

BOOL KLibOfBPT::InitRandMAT(INT nVersion)
{
	// todo [7/10/2012 simon]
	//typedef KBPT_MagicAttrib::KINDEX_MAP KMAMap;
	//KMAMap &mapIndex = m_cMagicAttrib.GetIndexMap();

 //   for (KMAMap::const_iterator i = mapIndex.begin(); i != mapIndex.end(); i++)
 //   {
 //       KBASICPROP_MAGICATTRIB *pMAT = const_cast<KBASICPROP_MAGICATTRIB *>(m_cMagicAttrib.GetData(i->first));
 //       if (!pMAT)
 //       {
 //           _ASSERT(pMAT);
 //           continue;
 //       }

	//	INT nDark = pMAT->bDarkness ? 1 : 0;
 //       for (INT nType = 0; nType < emMATF_COUNT_EQUIPTYPE; ++nType)
	//	{
 //           if ((pMAT->aryDropRate[nType]) <= 0)
 //               continue;	// 如果没有概率出现这个类型就跳到下一个

	//		INT nSeriesMin = pMAT->nSeries;
	//		INT nSeriesMax = pMAT->nSeries;

 //           if (pMAT->nSeries <= KD_ITEM_NO_SERIES_LIMIT)
 //           {
 //               nSeriesMin = series_metal;
 //               nSeriesMax = series_earth;
 //           }
 //           else
	//		{
	//			_ASSERT((pMAT->nSeries >= 0) && (pMAT->nSeries < emMATF_COUNT_SERIES));
	//		}

 //           for (INT nSeries = nSeriesMin; nSeries <= nSeriesMax; ++nSeries)
	//		{
	//			KScriptSafeCall cSafeCall(*g_pMainScript);
	//			VERIFY(g_pMainScript->CallTableFunction("Item", "GetRandMATLevel", 2, "dd", i->first.nLevel, nVersion));
	//			INT nLevelMin = g_pMainScript->GetInt(-2);
	//			INT nLevelMax = g_pMainScript->GetInt(-1);
	//			if (nLevelMax < nLevelMin)
	//				std::swap(nLevelMin, nLevelMax);

	//			if (nLevelMin < KD_ITEM_MIN_LEVEL)
	//				nLevelMin = KD_ITEM_MIN_LEVEL;
	//			if (nLevelMax > emMATF_COUNT_LEVEL)
	//				nLevelMax = emMATF_COUNT_LEVEL;

 //               for (INT nLevel = nLevelMin; nLevel <= nLevelMax; ++nLevel)
 //                   m_RandMAT[nDark][nType][nSeries][nLevel - 1].push_back(i->first);
 //           }
 //       }
 //   }

    return TRUE;
}

CONST KLibOfBPT::KMagicIndexVec *KLibOfBPT::GetRandMAT(BOOL bDarkness, INT nType) CONST
{
    if (nType <= 0 || nType > emMATF_COUNT_EQUIPTYPE)
	{	// 类型从1开始
		_ASSERT(FALSE);
        return NULL;
	}
	return &m_RandMAT[bDarkness ? 1 : 0][nType - 1];
}

CONST KBASICPROP_BASEDATA *KLibOfBPT::GetBasicProp(CONST KITEM_INDEX &sIndex) CONST
{
	KITEM_INDEX sItemIdx = sIndex;
	CONST KBASICPROP_BASEDATA *pRet = NULL;

	/*
	// 先找级别是0的默认数据
	sItemIdx.nLevel = 0;
	pRet = GetBasicPropStrictly(sItemIdx);
	if (pRet)
		return	pRet;
	*/

	// 再找指定级别的数据
	return	GetBasicPropStrictly(sIndex);
}

CONST KSUITE_INFO* KLibOfBPT::GetGoldSuiteInfo(INT nSuiteId) CONST
{
	KSuiteInfoMap::const_iterator it = m_mapSIGold.find(nSuiteId);
	if (it == m_mapSIGold.end())
		return	NULL;

	return	&it->second;
}

CONST KSUITE_INFO* KLibOfBPT::GetGreenSuiteInfo(INT nSuiteId) CONST
{
	KSuiteInfoMap::const_iterator it = m_mapSIGreen.find(nSuiteId);
	if (it == m_mapSIGreen.end())
		return	NULL;

	return	&it->second;
}

CONST KBASICPROP_MAGICACTIVERULE* KLibOfBPT::GetActiveRuleInfo(INT nRuleId) CONST
{
	return m_cMagicActiveRule.GetData(nRuleId);
}

VOID KLibOfBPT::InitExternSetting(INT nVersion)
{
	//KScriptSafeCall cSafeCall(*g_pMainScript);
	//CHAR szDirPath[MAX_PATH];
	//::sprintf(szDirPath, "%s\\%03d\\%s", KD_ITEM_SETTING_PATH, nVersion, "extern");
	//g_pMainScript->CallTableFunction("Item", "LoadExternSetting", 1, "sd", szDirPath, nVersion);
	//_ASSERT(g_pMainScript->GetInt(-1));
}

CONST KBASICPROP_BASEDATA *KLibOfBPT::GetBasicPropStrictly(CONST KITEM_INDEX &sIndex) CONST
{
	switch (sIndex.nGenre)
	{
	case item_equip_general:
		switch (sIndex.nDetailType)
		{
		case equip_sword:
			return m_cGE_Sword.GetData(sIndex);
		case equip_spear:
			return m_cGE_Spear.GetData(sIndex);
		case equip_necklace:
			return	m_cGE_Necklace.GetData(sIndex);
		case equip_ring:
			return	m_cGE_Ring.GetData(sIndex);
		case equip_pendant:
			return	m_cGE_Pendant.GetData(sIndex);
		case equip_head:
			return m_cGE_Head.GetData(sIndex);
		case equip_armor:
			return	m_cGE_Armor.GetData(sIndex);
		case equip_belt:
			return	m_cGE_Belt.GetData(sIndex);
		case equip_cloak:
			return m_cGE_Cloak.GetData(sIndex);
		case equip_foot:
			return	m_cGE_Foot.GetData(sIndex);
		case equip_suit:
			return m_cGE_Suit.GetData(sIndex);
		case equip_bottom:
			return	m_cGE_Bottom.GetData(sIndex);
		case equip_back:
			return	m_cGE_Back.GetData(sIndex);
		default:
			ASSERT(!"Invalid DetailType");
			break;
		}
		break;

	case item_medicine:
		return	m_cMedicine.GetData(sIndex);
	case item_pet:
		switch(sIndex.nDetailType)
		{
		case item_pet_equip:
			return m_cPetEquip.GetData(sIndex);
		default:
			return m_cPetItem.GetData(sIndex);
		}
		break;
	case item_script:
		switch(sIndex.nDetailType)
		{
		case item_script_other:
			return m_cScript.GetData(sIndex);
		case item_script_stone:
			return m_cStone.GetData(sIndex);
		case item_script_ride:
			return m_cRide.GetData(sIndex);
		case item_upgrade_material:
			return m_cUpgradeMaterial.GetData(sIndex);
		case item_script_box:
			return m_cBox.GetData(sIndex);
		default:
			ASSERT(!"Invalid DetailType");
			break;
		}
		break;
	//case item_skill:
	//	return	m_cSkill.GetData(sIndex);
	case item_quest:
		return	m_cQuest.GetData(sIndex);
	//case item_extbag:
	//	return	m_cExtBag.GetData(sIndex);
	//case item_stuff:
	//	return	m_cStuff.GetData(sIndex);
	//case item_plan:
	//	return	m_cPlan.GetData(sIndex);
	default:
		ASSERT(!"Invalid Genre");
		break;
	}

	return	NULL;
}

VOID KLibOfBPT::InitSuiteInfo(INT nVersion)
{
	//InitSuiteInfoT<KBPT_GoldEquip,  KBASICPROP_GOLDEQUIP,  KBPT_GoldInfo> (m_mapSIGold,  m_cGoldEquip,  m_cGoldInfo);
	//InitSuiteInfoT<KBPT_GreenEquip, KBASICPROP_GREENSUITE, KBPT_GreenInfo>(m_mapSIGreen, m_cGreenEquip, m_cGreenInfo);
}

template<typename KBPT_Equip, typename KBASICPROP_SUITE, typename KBPT_Info>
VOID KLibOfBPT::InitSuiteInfoT(KSuiteInfoMap &mapSuiteInfo, KBPT_Equip &cBPTEquip, KBPT_Info &cBPTInfo)
{
	mapSuiteInfo.clear();
	typename KBPT_Equip::KINDEX_MAP &mapIndex = cBPTEquip.GetIndexMap();
	typename KBPT_Info::KINDEX_MAP &mapInfo  = cBPTInfo.GetIndexMap();
	typename KBPT_Equip::KINDEX_MAP::const_iterator i = mapIndex.begin();

	for (; i != mapIndex.end(); i++)
	{
		CONST KBASICPROP_SUITE *pEq = mapIndex[i->first];
		if (!pEq)
		{
			_ASSERT(FALSE);
			continue;
		}

		if (pEq->nSuiteID <= KD_ITEM_INVALID_SUITEID)		// 不是套装/套装ID无效
			continue;

		INT nEqPlace = KPlayerItem::GetEquipPos(pEq->nDetailType);
		if (nEqPlace < 0)
		{
			_ASSERT(FALSE);
			continue;
		}

		KSUITE_INFO *pSuiteInfo = &mapSuiteInfo[pEq->nSuiteID];
		KITEM_INDEX sEquipIdx = { pEq->nItemGenre, pEq->nDetailType, pEq->nParticular, pEq->nLevel };

		pSuiteInfo->pInfo = cBPTInfo.GetData(pEq->nSuiteID);
		pSuiteInfo->aryPart[nEqPlace].push_back(sEquipIdx);
		if (pSuiteInfo->aryPart[nEqPlace].size() == 1)
			pSuiteInfo->nCount++;
	}
}

// -------------------------------------------------------------------------
