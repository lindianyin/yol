
#include "stdafx.h"
#include "kitem.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayermgr.h"
#include "kitemgenerator.h"
#include "kplayeritem.h"
#include "kmagicattrib.h"
#include <time.h>
#include <map>
#include "onlinegamebase/ksysservice.h"

#include "Core/QGUID.h"
#include "Core.h"


//---------------------------------------------------------------------------

BOOL KItemGenerator::Init(INT nVersion)
{
    m_nVersion = nVersion;
	return	m_BPTLib.Init(nVersion);
}

BOOL KItemGenerator::Gen_Item(KItem*					pItem,
							  INT						nItemGenre,
							  INT						nDetailType,
							  INT						nParticular,
							  INT						nLevel,
							  INT						nEnhTimes,
							  CONST KItem::KSTONE_INFO* pStoneInfo,
							  CONST KMagicAttrib*		pAppendAttr,
							  CONST KItem::KREFINE_LOCK* pRefineLock,
							  UINT						uRandSeed,
							  UINT						uRandSeedEx,
							  BOOL						bNew,
							  BOOL						bTemporary,		// = FALSE
							  CONST GUID*				pGuid			// = NULL
							  )
{
	BYTE pBuf[emITEM_COUNT_RANDOM] = {0};
	if (!pItem)
		return	FALSE;
	if (nItemGenre <= ITEM_INVALID_TYPE)
		return	FALSE;
	if (nDetailType <= ITEM_INVALID_TYPE)
		return	FALSE;
	if (nParticular <= ITEM_INVALID_TYPE)
		return	FALSE;

	BOOL bRet = FALSE;

#ifdef GAME_SERVER
	if (!uRandSeed)
	{	// 打散随机种子
		g_Random(-1);
		uRandSeed = g_GetRandomSeed();
	}
	if (!uRandSeedEx)
	{	// 打散随机种子
		g_Random(-1);
		uRandSeedEx = g_GetRandomSeed();
	}
	// 设置物品唯一ID
	if (pGuid)
	{
		pItem->SetGuid(pGuid);
	}
	else
	{
		GUID sGuid;
		QGenerateGUID(&sGuid);
		pItem->SetGuid(&sGuid);
	}

#endif

	if(bNew)
	{
		memset(pItem->m_aryStoneInfo, emSTONE_LOCKED_ID, sizeof(pItem->m_aryStoneInfo));
	}

	m_cRandom.SetSeed(uRandSeed);	// 使用随机种子
	m_cRandomEx.SetSeed(uRandSeedEx);

	pItem->SetRandSeed(uRandSeed);	// 设置随机种子
	pItem->SetRandSeedEx(uRandSeedEx);

	g_cScriptManager.SetIt((QLunaBase*)pItem->m_pLuna);

	switch (nItemGenre)
	{
	case item_equip_general:		// 普通装备类
		if(nDetailType <= equip_foot)
		{
			bRet = Gen_GeneralEquip(pItem, nDetailType, nParticular, nLevel, nEnhTimes, pStoneInfo, pAppendAttr, pRefineLock, bNew);
		}
		else
		{
			// 套装
			bRet = Gen_GeneralSuit(pItem, nDetailType, nParticular, nLevel, bNew);
		}
		
		break;
	case item_medicine:				// 药品类
		bRet = Gen_Medicine(pItem, nDetailType, nParticular);
		break;
	case item_script:				// 脚本道具类
		switch(nDetailType)
		{
		case item_script_stone:
			bRet = Gen_Stone(pItem, nDetailType, nParticular, nLevel);
			break;
		case item_script_ride:
			bRet = Gen_Ride(pItem, nDetailType, nParticular, nLevel);
			break;
		default:
			bRet = Gen_ScriptItem(pItem, nDetailType, nParticular);
			break;
		}
		break;
	case item_pet:					// 宠物道具类
		bRet = Gen_PetItem(pItem, nDetailType, nParticular);
		break;
	case item_skill:				// 技能道具类
		bRet = Gen_SkillItem(pItem, nDetailType, nParticular);
		break;
	case item_quest:				// 任务道具类
		bRet = Gen_TaskQuest(pItem, nDetailType, nParticular);
		break;
	case item_extbag:				// 扩展背包类
		bRet = Gen_ExtendBag(pItem, nDetailType, nParticular);
		break;
	case item_stuff:
		bRet = Gen_StuffItem(pItem, nDetailType, nParticular);
		break;
	case item_plan:
		bRet = Gen_PlanItem(pItem, nDetailType, nParticular);
		break;
	default:
		bRet = FALSE;
	}

	if (bRet)
	{
#ifdef GAME_SERVER
		if (bNew)
			pItem->SetGenTime(KSysService::Time(NULL));
#endif
		/*
		if (nStrengthen > 0)
		{
			pItem->SetStrengthen(nStrengthen); // 设置改造属性
		}
		*/

		pItem->SetTemporary(bTemporary);
		//VERIFY(pItem->EndGenerate());
	}

	return	bRet;
}

BOOL KItemGenerator::Gen_Medicine(KItem*	pItem,
								  INT		nDetailType,
								  INT		nParticular)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_medicine, nDetailType, nParticular };
	CONST KBASICPROP_MEDICINE* pMed = static_cast<CONST KBASICPROP_MEDICINE*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pMed)
		return	FALSE;

	pItem->Init_Medicine(pMed);

	return TRUE;
}

BOOL KItemGenerator::Gen_Stone(KItem*	pItem,
								INT		nDetailType,
								INT		nParticular,
								INT		nLevel)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_script, item_script_stone, nParticular, nLevel };
	CONST KBASICPROP_STONE* pSt = static_cast<CONST KBASICPROP_STONE*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pSt)
		return	FALSE;

	// 默认给1级宝石
	if(nLevel == 0) nLevel = 1;
	pItem->Init_Stone(pSt);

	// 计算宝石战斗力
	{
		KPlayerItem::CalcStoneFightScore(pItem);
	}

	return TRUE;
}

BOOL KItemGenerator::Gen_ScriptItem(KItem*					pItem,
									INT 					nDetailType,
									INT 					nParticular)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_script, nDetailType, nParticular };
	CONST KBASICPROP_SCRIPT* pScript = static_cast<CONST KBASICPROP_SCRIPT*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pScript)
		return	FALSE;

	pItem->Init_Script(pScript);

	return TRUE;
}

BOOL KItemGenerator::Gen_Ride(KItem*					pItem,
									INT 					nDetailType,
									INT 					nParticular,
									INT						nLevel)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_script, nDetailType, nParticular, nLevel};
	CONST KBASICPROP_SCRIPT* pScript = static_cast<CONST KBASICPROP_SCRIPT*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pScript)
		return	FALSE;

	pItem->Init_Script(pScript);

	return TRUE;
}


BOOL KItemGenerator::Gen_PetItem(KItem*					pItem,
									INT 					nDetailType,
									INT 					nParticular)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_pet, nDetailType, nParticular };
	CONST KBASICPROP_SCRIPT* pScript = static_cast<CONST KBASICPROP_SCRIPT*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pScript)
		return	FALSE;

	pItem->Init_Script(pScript);

	return TRUE;
}

BOOL KItemGenerator::Gen_SkillItem(KItem*					pItem,
								   INT 						nDetailType,
								   INT 						nParticular)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_skill, nDetailType, nParticular };
	CONST KBASICPROP_SKILL* pSKill = static_cast<CONST KBASICPROP_SKILL*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pSKill)
		return	FALSE;

	pItem->Init_Skill(pSKill);

	return TRUE;
}

BOOL KItemGenerator::Gen_TaskQuest(KItem*					pItem,
								   INT 						nDetailType,
								   INT 						nParticular)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_quest, nDetailType, nParticular };
	CONST KBASICPROP_QUEST* pQuest = static_cast<CONST KBASICPROP_QUEST*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pQuest)
		return	FALSE;

	pItem->Init_Quest(pQuest);

	return TRUE;
}

BOOL KItemGenerator::Gen_ExtendBag(KItem*					pItem,
								   INT 						nDetailType,
								   INT 						nParticular)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_extbag, nDetailType, nParticular };
	CONST KBASICPROP_EXTBAG* pExtBag = static_cast<CONST KBASICPROP_EXTBAG*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pExtBag)
		return	FALSE;

	pItem->Init_ExtBag(pExtBag);

	return TRUE;
}

BOOL KItemGenerator::Gen_StuffItem(KItem*	pItem,
								   INT	 	nDetailType,
								   INT		nParticular)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_stuff, nDetailType, nParticular };
	CONST KBASICPROP_STUFF* pStuff = static_cast<CONST KBASICPROP_STUFF*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pStuff)
		return	FALSE;

	pItem->Init_Stuff(pStuff);

	return TRUE;
}

BOOL KItemGenerator::Gen_PlanItem(KItem*	pItem,
								  INT	 	nDetailType,
								  INT		nParticular)
{
	BOOL bRet = TRUE;

	KITEM_INDEX sIndex = { item_plan, nDetailType, nParticular };
	CONST KBASICPROP_PLAN* pPlan = static_cast<CONST KBASICPROP_PLAN*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pPlan)
		return	FALSE;

	pItem->Init_Plan(pPlan);

	return TRUE;
}

BOOL KItemGenerator::Gen_GeneralEquip(KItem*					pItem,
									  INT						nDetailType,
									  INT						nParticular,
									  INT						nLevel,
									  INT						nEnhTimes,
									  CONST KItem::KSTONE_INFO* pStoneInfo,
									  CONST KMagicAttrib*		pAppendAttr,
									  CONST KItem::KREFINE_LOCK* pRefineLock,
									  BOOL bNew)
{
	// 根据入口参数, 确定装备的基本数据
	KITEM_INDEX sIndex = { item_equip_general, nDetailType, nParticular, nLevel };
	CONST KBASICPROP_GENERALEQUIP* pEq = static_cast<CONST KBASICPROP_GENERALEQUIP*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pEq)
		return	FALSE;
	
	pItem->Init_Equip(pEq, m_cRandom, m_cRandomEx, bNew);

	if(pStoneInfo)
	{
		memcpy(pItem->m_aryStoneInfo, pStoneInfo, sizeof(pItem->m_aryStoneInfo));
	}

	// 强化属性
	pItem->SetEnhanceTimes(nEnhTimes);
	pItem->InitEnhAttrib();

	if(pAppendAttr)
	{
		memcpy(pItem->m_aryAppendAttrib, pAppendAttr, sizeof(pItem->m_aryAppendAttrib));
	}
	else
	{
		if(!pRefineLock)
		{
			pItem->InitAppendAttrib();
		}
		else
		{
			pItem->RerandAppendAttrib(pRefineLock);
		}
	}

	// 计算装备战斗力
	{
		KPlayerItem::CalcEquipFightScore(pItem);
	}

	return TRUE;
}

BOOL KItemGenerator::Gen_GeneralSuit(KItem*					pItem,
									  INT						nDetailType,
									  INT						nParticular,
									  INT						nLevel,
									  BOOL bNew)
{
	// 根据入口参数, 确定装备的基本数据
	KITEM_INDEX sIndex = { item_equip_general, nDetailType, nParticular, nLevel };
	CONST KBASICPROP_SUIT* pEq = static_cast<CONST KBASICPROP_SUIT*>(m_BPTLib.GetBasicProp(sIndex));
	if (!pEq)
		return	FALSE;

	pItem->Init_SuitEquip(pEq, m_cRandom, bNew);

	return TRUE;
}

// -------------------------------------------------------------------------
