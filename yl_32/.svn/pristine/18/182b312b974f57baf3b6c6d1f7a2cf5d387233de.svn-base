/********************************************************************
	created:	2012/05/28 9:17
	filename: 	characterskill.cpp
	author:		zhangjunjie
*********************************************************************/

#include "stdafx.h"
#include "findpath/kplayerfindpath.h"
#include "onlinegamemodule/characterskill.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kcharactermgr.h"
#include "fightskill/fightskillsetting.h"
#include "fightskill/fightskill.h"
#include "fightskill/fightskillprotocol.h"
#include "fightskill/fightskillprotocolprocessor.h"
#include "item2/kplayeritem.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "buffer/buffermanager.h"
#include "../onlinegameworld/scene/kscene.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include <algorithm>
#include "fightskillmanager.h"
#include "config/kplayerbasedbdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CharacterSkill::CharacterSkill(KCharacter& rcCharacter)
:KCharacterCtrlBase(rcCharacter),
m_pLunaObj(NULL)
{
	m_mapSkillData.clear();
	m_mapSkillLevelData.clear();
	memset(m_arrPassiveSkill, '\0', sizeof(m_arrPassiveSkill));
	m_LastChangeRouteTime = 0;
}
// -------------------------------------------------------------------------
CharacterSkill::~CharacterSkill()
{
	m_mapSkillData.clear();
	m_mapSkillLevelData.clear();
	memset(m_arrPassiveSkill, '\0', sizeof(m_arrPassiveSkill));

	SAFE_DELETE(m_pLunaObj);
}

BOOL CharacterSkill::Init()		
{
	m_pLunaObj = new LuaCharacterSkill(m_rcCharacter);
	m_bMapLoadEmpty = FALSE;

	m_mapSkillData.clear();
	m_mapSkillLevelData.clear();
	m_bDbLoaded = FALSE;
	memset(m_arrPassiveSkill, '\0', sizeof(m_arrPassiveSkill));
	
	return TRUE;
}

BOOL CharacterSkill::UnInit()
{
	for(SKILLDATAMAP::iterator iter = m_mapSkillData.begin(); iter != m_mapSkillData.end(); ++iter)
	{
		SAFE_DELETE(iter->second->pSkill);
		SAFE_DELETE(iter->second);
	}
	m_mapSkillData.clear();
	m_mapSkillLevelData.clear();
	m_bDbLoaded = FALSE;
	memset(m_arrPassiveSkill, '\0', sizeof(m_arrPassiveSkill));
	SAFE_DELETE(m_pLunaObj);
	return TRUE;
}


BOOL CharacterSkill::Active()
{
	for(SKILLDATAMAP::iterator iter = m_mapSkillData.begin(); iter != m_mapSkillData.end(); ++iter)
	{
		iter->second->pSkill->Breath();	
		if(iter->second->bStartCalcCD)
		{
			iter->second->nCurrentCDTime--;	//计算CD时间
			if (iter->second->nCurrentCDTime <= 0)
			{
				iter->second->nCurrentCDTime = iter->second->nCDTime;  // 完成冷却时间，恢复到需要冷却的时间
				iter->second->bStartCalcCD = FALSE;
			}
		}
	}

	return TRUE;
}

VOID CharacterSkill::ForwardFrame(INT nFrame)
{
    BOOL bNeedForwardFrame = FALSE;
    
	for(SKILLDATAMAP::iterator iter = m_mapSkillData.begin(); iter != m_mapSkillData.end(); ++iter)
	{
		if (IsSkillInCD(iter->first))
		{
		    bNeedForwardFrame = TRUE;
			break;;
		}
	}

	if (bNeedForwardFrame)
	{
		for (INT i = 1; i <= nFrame; i++)
		{
			Active();
		}
	}
}

BOOL CharacterSkill::CancelChannelSkill(WORD wSkillId)
{
	if (wSkillId <= 0)
	{
		//QLogPrintf(LOG_ERR, "技能ID必须大于0 nSkillId = %d", wSkillId);
		return FALSE;
	}
	//重新加某个技能
	if (m_mapSkillData.find(wSkillId) == m_mapSkillData.end())
	{
		//QLogPrintf(LOG_ERR, "重复技能 nSkillId = %d", wSkillId);
		return FALSE;
	}

	FightSkill* pFightSkill = m_mapSkillData[wSkillId]->pSkill;

	if (pFightSkill->m_pFightSkillTemplate->nFightSkillChannel != FIGHT_SKILL_CHANNEL_NO)
	{
		LOG_PROCESS_ERROR(pFightSkill);
		pFightSkill->CancelChannelSkill();

		FightSkillProtocolProcessor::BroadcastCancelChannelSkill(m_rcCharacter, wSkillId);
	}

EXIT0:
	return FALSE;
}

BOOL CharacterSkill::PreAddSkill(WORD wSkillId)
{
	// 技能是否已存在
	if (m_mapSkillData.find(wSkillId) != m_mapSkillData.end())
	{
		//QLogPrintf(LOG_ERR, "重复技能 nSkillId = %d", wSkillId);
		return FALSE;
	}

	SKILLDATAMAP tSkillData = m_mapSkillData;

	// 检查是否有同个Cate的技能
	FightSkillTemplate* pAimTemplate = g_cFightSkillSetting.GetFightSkillTemplate(wSkillId);
	LOG_PROCESS_ERROR(pAimTemplate);

	for (SKILLDATAMAP::iterator iter = tSkillData.begin();
		iter != tSkillData.end(); ++iter)
	{
		FightSkillTemplate* pBeHaveTemplate = (*iter).second->pSkill->m_pFightSkillTemplate;
		
		if (pBeHaveTemplate->nCategory == pAimTemplate->nCategory)
		{
			//if (pBeHaveTemplate->nSkillLevel < pAimTemplate->nSkillLevel)
			//{
			//	// 删除低等级Skill
			//	this->RemoveSkill(pBeHaveTemplate->nSkillID);
			//}
			//else if (pAimTemplate->nSkillLevel < pBeHaveTemplate->nSkillLevel)
			//{
			//	// 已经有高等级技能存在
			//	return FALSE;
			//}
			//else
			//{
			//	LOG_PROCESS_ERROR( !"相同Cate不同ID的Skill等级竟然一样" );
			//}
		}
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL CharacterSkill::HasSkill(WORD wSkillId)
{
	if(m_mapSkillData.find(wSkillId) == m_mapSkillData.end())
	{
		return FALSE;  // 没有
	}

	return TRUE;
}

VOID CharacterSkill::RawSetLock()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction("BufferMagicEffect", "SetAttrRawSet", 1, "d", 1);
}

VOID CharacterSkill::RawSetUnLock()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction("BufferMagicEffect", "SetAttrRawSet", 1, "d", 0);
}

BOOL CharacterSkill::AddSkill(WORD wSkillId, BOOL bSync, INT nSkillLevel/* = 1*/)
{
	FightSkillTemplate* fightSkillTemplate = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	if (wSkillId <= 0)
	{
		//QLogPrintf(LOG_ERR, "技能ID必须大于0 nSkillId = %d", wSkillId);
		return FALSE;
	}

	// 添加新技能
	SKILLDATA* pData = new SKILLDATA(wSkillId);
	pData->pSkill = new FightSkill();
	LOG_PROCESS_ERROR(pData->pSkill->Init(&m_rcCharacter,wSkillId, nSkillLevel));
	
	pData->nSlotId = pData->pSkill->m_pFightSkillTemplate->nSlot;
	//pData->nRundId = pData->pSkill->m_pFightSkillTemplate->nRune;
	pData->nSkillLevel = nSkillLevel; // 技能等级
	pData->nCDTime = pData->pSkill->m_pFightSkillTemplate->nTimePerCast;
	pData->nCurrentCDTime = pData->nCDTime;

	fightSkillTemplate = g_cFightSkillSetting.GetFightSkillTemplate(wSkillId);
	LOG_PROCESS_ERROR(fightSkillTemplate);

	if (fightSkillTemplate->nFightSkillType == FIGHT_SKILL_PASSIVE) // 如是被动技能，直接给玩家添加永久buffer
	{
		this->SetPassiveSkill(fightSkillTemplate->nPassiveSkillType, nSkillLevel, TRUE, TRUE);  // Add
	}
	
	m_mapSkillData[wSkillId] = pData;

	if (IS_PLAYER(m_rcCharacter.m_dwId) && bSync)
	{
		KPlayer* pPlayer = g_cPlayerMgr.GetById(m_rcCharacter.m_dwId);
		if (!pPlayer)
		{
			return FALSE;
		}
		//同步添加技能
		FightSkillProtocolProcessor::SyncS2C_AddSkill(pPlayer->GetPlayerIndex(),m_rcCharacter.GetId(), wSkillId);
	}
	
	//QLogPrintf(LOG_INFO, "[CharacrerSkill]Character[%d] Add skill[%d]", m_rcCharacter.GetId(), wSkillId);


	return TRUE;
EXIT0:
	return FALSE;
}

// 吃药技能
BOOL CharacterSkill::ApplyMedicineSkill(INT nSkillSlot, INT nSkillLevel)
{
	KITEM_INDEX  medicineIndex;
	medicineIndex.nGenre = emMEDICINE_ITEM_INDEX_GENRE;
	medicineIndex.nDetailType = emMEDICINE_ITEM_INDEX_DETAIL;
	medicineIndex.nParticular = emMEDICINE_ITEM_INDEX_PARTICULAR;
	medicineIndex.nLevel = KD_ITEM_NO_LEVEL_LIMIT;

	INT nMedicineType = 0;

	LOG_PROCESS_ERROR(nSkillSlot == emRED_SKILL || nSkillSlot == emBLUE_SKILL);

	if (nSkillSlot == emRED_SKILL) 
	{
		MedicineSkillNumber* pTemplate = g_cFightSkillSetting.GetMedicineSkillNumber(emMEDICINE_SKILL_RED, nSkillLevel);
		QCONFIRM_RET_FALSE(pTemplate);

		KPlayerItem* pPlayerItem = m_rcCharacter.GetCtrl<KPlayerItem>();
		if (pPlayerItem->GetItemCountInBags(medicineIndex) >= pTemplate->nNeedMedicineCount)
		{
			pPlayerItem->ConsumeItemInBags(pTemplate->nNeedMedicineCount, medicineIndex);

			INT nNewLife = m_rcCharacter.GetCurrentLife() + pTemplate->nValue;
			INT nMaxLife = m_rcCharacter.GetMaxLife();
			nNewLife = nNewLife > nMaxLife ? nMaxLife : nNewLife;

			m_rcCharacter.SetCurrentLife(nNewLife);
		}
	}
	else if (nSkillSlot == emBLUE_SKILL)
	{
		MedicineSkillNumber* pTemplate = g_cFightSkillSetting.GetMedicineSkillNumber(emMEDICINE_SKILL_BLUE, nSkillLevel);
		QCONFIRM_RET_FALSE(pTemplate);

		KPlayerItem* pPlayerItem = m_rcCharacter.GetCtrl<KPlayerItem>();
		if (pPlayerItem->GetItemCountInBags(medicineIndex) >= pTemplate->nNeedMedicineCount)
		{
			pPlayerItem->ConsumeItemInBags(pTemplate->nNeedMedicineCount, medicineIndex);

			INT nNewMana = m_rcCharacter.GetCurrentMana() + pTemplate->nValue;
			INT nMaxMana = m_rcCharacter.GetMaxMana();
			nNewMana = nNewMana > nMaxMana ? nMaxMana : nNewMana;

			m_rcCharacter.SetCurrentMana(nNewMana);
		}
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL CharacterSkill::AddPassiveSkillValue(INT nPassiveType, INT nSkillLevel, BOOL bTakeBackNumber, BOOL bSync)
{
	PassiveSkillNumber* pPassiveSkillNumber = g_cFightSkillSetting.GetPassiveSkillNumber(nPassiveType, nSkillLevel);
	if (!pPassiveSkillNumber)
	{
		//QLogPrintf(LOG_WARNING, "找不到被动技能数值，类型：%d, 等级：%d", nPassiveType, nSkillLevel);
		return FALSE;
	}

	INT nValue = (pPassiveSkillNumber) ? pPassiveSkillNumber->nValue : 0;  // 没填值，填0

	switch(nPassiveType)
	{
	case emSKILL_ATTR_LIFE:  //加血被动
		if (bSync)
			m_rcCharacter.AddMaxLife(bTakeBackNumber ? -nValue : nValue);
		else 
			m_rcCharacter.RawAddMaxLife(bTakeBackNumber ? -nValue : nValue);
		break;
	case emSKILL_ATTR_ATTACK:
		if (bSync) 
			m_rcCharacter.AddAttack(bTakeBackNumber ? -nValue : nValue);
		else 
			m_rcCharacter.RawAddAttack(bTakeBackNumber ? -nValue : nValue);
		break;
	case emSKILL_ATTR_HIT:
		if (bSync) 
			m_rcCharacter.AddHitCoefficient(bTakeBackNumber ? -nValue : nValue);
		else 
			m_rcCharacter.RawAddHitCoefficient(bTakeBackNumber ? -nValue : nValue);
		break;
	case emSKILL_ATTR_CRIT: // 暴击
		if (bSync) 
			m_rcCharacter.AddCriticalValue(bTakeBackNumber ? -nValue : nValue);
		else 
			m_rcCharacter.RawAddCriticalValue(bTakeBackNumber ? -nValue : nValue);
		break;
	case emSKILL_ATTR_RESIST:  // 抗暴
		if (bSync) 
			m_rcCharacter.AddResist(bTakeBackNumber ? -nValue : nValue);
		else 
			m_rcCharacter.RawAddResist(bTakeBackNumber ? -nValue : nValue);
		break;
	case emSKILL_ATTR_DEFENSE:
		if (bSync) 
			m_rcCharacter.AddDef(bTakeBackNumber ? -nValue : nValue);
		else 
			m_rcCharacter.RawAddDef(bTakeBackNumber ? -nValue : nValue);
		break;
	case emSKILL_ATTR_DODGE:
		if (bSync) 
			m_rcCharacter.AddDodge(bTakeBackNumber ? -nValue : nValue);
		else 
			m_rcCharacter.RawAddDodge(bTakeBackNumber ? -nValue : nValue);
		break;
	}

	return TRUE;
}

// 被动技能类型, 添加或移除
BOOL CharacterSkill::SetPassiveSkill(INT nPassiveType, INT nSkillLevel, BOOL bAddOrRemove, BOOL bSync)
{
	QCONFIRM_RET_FALSE(nPassiveType <= emSKILL_ATTR_COUNT);  // 边界检查

	if((bAddOrRemove && !m_arrPassiveSkill[nPassiveType - 1]) ||  // 将要添加，保证不存在
		(!bAddOrRemove && m_arrPassiveSkill[nPassiveType - 1]))  // 将要移除，保证已经存在)
	{
		m_arrPassiveSkill[nPassiveType - 1] = bAddOrRemove;  // 标记被动属性状态

		AddPassiveSkillValue(nPassiveType, nSkillLevel, !bAddOrRemove, bSync);
	}

	return TRUE;
}

BOOL CharacterSkill::SyncSkillsToClient(BOOL resetCD)
{
	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_rcCharacter.m_dwId);
	if (!pPlayer)
	{
		//QLogPrintf(LOG_ERR, "[SyncSkillsToClient]无法找到Player");
		return FALSE;
	}

	// 创建所有技能id组成的数组
	WORD allSkills[KD_SYNC_SKILLS_MAX_COUNT];  // 技能ID~
	WORD allSkillsLevels[KD_SYNC_SKILLS_MAX_COUNT];  // 技能等级
	SKILLDATAMAP::iterator map_it = m_mapSkillData.begin();
	INT count = 0;
	while(map_it != m_mapSkillData.end())
	{
		if (resetCD)  // 如果要清空CD~~（客户端刚进入时）
		{
			if(map_it->second->nCDTime != map_it->second->nCurrentCDTime)
			{
				map_it->second->bStartCalcCD = TRUE;	//客户端加载完成后，开始CD时间计算
			}
		}
		allSkills[count] = (WORD)map_it->first;
		allSkillsLevels[count] = (WORD)map_it->second->nSkillLevel;
		++map_it;
		++count;
	}

	return FightSkillProtocolProcessor::SyncS2C_SyncSkills(pPlayer->GetPlayerIndex(),m_rcCharacter.GetId(), count, allSkills, allSkillsLevels);
}

BOOL CharacterSkill::IsSkillInCD(WORD wSkillId)
{
	if(m_mapSkillData.find(wSkillId) == m_mapSkillData.end())
		return FALSE;

	if (m_mapSkillData[wSkillId]->nCDTime != m_mapSkillData[wSkillId]->nCurrentCDTime || m_mapSkillData[wSkillId]->pSkill->IsActive())
	{
		return TRUE;
	}
	return FALSE;
}

// 判断技能距离
BOOL CharacterSkill::CheckSkillCastDistance(WORD wSkillId, DWORD dwTargetId, INT nX, INT nY, INT nZ)
{
	if(m_mapSkillData.find(wSkillId) == m_mapSkillData.end())
	{
		//QLogPrintf(LOG_INFO, "检查技能距离时找不到技能[%d]！", wSkillId);
		return FALSE;
	}

	if (m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->nFightSkillType != FIGHT_SKILL_PASSIVE)
	{
		// Target获取，判断是否需要
		KCharacter* pTarget = g_cCharacterMgr.GetById(dwTargetId);
		BOOL nIsNeedTarget = m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->bNeedTarget;
		if ( nIsNeedTarget && !pTarget)	//需要目标
		{
			//QLogPrintf(LOG_INFO, "检查该技能释放距离需要目标 SkillId:[%d]！", wSkillId);
			return FALSE;
		}

		// 技能释放距离判断
		INT nCharX, nCharY, nCharZ;
		m_rcCharacter.GetPosition(nCharX, nCharY, nCharZ);
		// 转换逻辑单位
		INT nAttackRadius = (INT)(m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->fAttackRadius * m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->fAttackRadius * 4 * CELL_LENGTH * CELL_LENGTH);

		if (nIsNeedTarget)
		{
			INT nTargetX, nTargetY, nTargetZ;
			pTarget->GetPosition(nTargetX, nTargetY, nTargetZ);
			INT nDistance = g_GetDistance2(nCharX, nCharY, nTargetX, nTargetY);
			//QLogPrintf(LOG_INFO, "距离 Distance :%d", nDistance);
			if (nDistance > nAttackRadius)
			{
				//QLogPrintf(LOG_INFO, "技能释放范围不对~人物所处位置大于技能释放范围 SkillId:[%d] Distance:[%d]！", wSkillId, nDistance);
				return FALSE;
			}
		}
		else
		{
			INT nDistance = g_GetDistance2(nCharX, nCharY, nX, nY);
			if (nDistance > nAttackRadius)
			{
				//QLogPrintf(LOG_INFO, "技能释放范围不对~人物所处位置大于技能释放范围 SkillId:[%d]！", wSkillId);
				return FALSE;
			}
		}
	}
	
	return TRUE;
}

BOOL CharacterSkill::CheckObstacle(INT nStartX, INT nStartY, INT nStartZ, INT nEndX, INT nEndY, INT nEndZ, INT nTouchRange)
{
	INT nLastOkX = 0, nLastOkY = 0;
	return this->CheckObstacle(nStartX, nStartY, nStartZ, nEndX, nEndY, nEndZ,nTouchRange, nLastOkX, nLastOkY);
}

BOOL CharacterSkill::CheckObstacle(INT nStartX, INT nStartY, INT nStartZ, INT nEndX, INT nEndY, INT nEndZ, INT nTouchRange, INT& nLastOkX, INT& nLastOkY)
{
	INT nDistance = 0;
	INT nLastDistance = 0;
	INT nObStacleCount = 0;
	INT nDirection = 0;
	INT nStepLength = CELL_LENGTH / 2;
	INT nDeltaX = 0;
	INT nDeltaY = 0;
	INT nDestX = 0;
	INT nDestY = 0;
	INT             nDestCellX              = 0;
	INT             nDestCellY              = 0;
	INT             nDestRegionX            = 0;
	INT             nDestRegionY            = 0;
	KRegion*        pDestRegion             = NULL;
	KCell*          pDestCell               = NULL;
	INT             nRelativeZ              = 0;

	// 令上次正常值，等于起始点
	nLastOkX = nStartX;
	nLastOkY = nStartY;

	KScene* pScene = (KScene*)m_rcCharacter.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	nDirection = g_GetDirection(nStartX, nStartY, nEndX, nEndY);

	nDistance = g_GetDistance2(nStartX, nStartY, nEndX, nEndY);
	
	nTouchRange = 3;  // Force强制

	while(nDistance > CELL_LENGTH * CELL_LENGTH / 4 * nTouchRange)
	{
		nLastDistance = nDistance;

		nDeltaX = nStepLength * g_Cos(nDirection) / SIN_COS_NUMBER;
		nDeltaY = nStepLength * g_Sin(nDirection) / SIN_COS_NUMBER;

		nDestX = nStartX + nDeltaX;
		nDestY = nStartY + nDeltaY;

		nDestCellX      = nDestX / CELL_LENGTH % REGION_GRID_WIDTH;
		nDestCellY      = nDestY / CELL_LENGTH % REGION_GRID_HEIGHT;

		nDestRegionX    = nDestX / CELL_LENGTH / REGION_GRID_WIDTH;
		nDestRegionY    = nDestY / CELL_LENGTH / REGION_GRID_HEIGHT;

		QCONFIRM_RET_FALSE(nDestRegionX < REGION_GRID_WIDTH);
		QCONFIRM_RET_FALSE(nDestRegionY < REGION_GRID_HEIGHT);
		pDestRegion = pScene->GetRegion(nDestRegionX, nDestRegionY);
		QCONFIRM_RET_FALSE(pDestRegion);

		pDestCell = pDestRegion->GetProximalCellForObj(nDestCellX, nDestCellY, nStartZ, m_rcCharacter.m_nHeight, TRUE, FALSE, &nRelativeZ);
		/*pDestCell = pDestRegion->GetAdaptedCellForObj(
			nDestCellX, nDestCellY, nStartZ, 0, 
			m_rcCharacter.m_nHeight, TRUE,
			&nRelativeZ, NULL
			);*/

		if (nRelativeZ > 0 && nRelativeZ < CRITICAL_JUMP_HEIGHT)
		{
			//if (m_eMoveState == cmsOnStand || m_eMoveState == cmsOnWalk || m_eMoveState == cmsOnRun)
			if (m_rcCharacter.GetMoveState() == cmsOnRun ||
				m_rcCharacter.GetMoveState() == cmsOnWalk ||
				m_rcCharacter.GetMoveState() == cmsOnStand)
			{
				// 注意,这个赋0操作会影响到打滑效果
				// 由于悬空是不打滑的,如果这里不赋0,则会造成斜坡上移动时打滑状态不连续
				nRelativeZ = 0;
			}
		}

		if(!pDestCell)
		{
			return FALSE;  // 有障碍
		}
		else
		{
			nLastOkX = nStartX;  // 这次循环没障碍，保存起来，如果为0，那么就是start点就是障碍 (这里上上次正常坐标
			nLastOkY = nStartY;

			nStartX = nDestX;
			nStartY = nDestY;
			nStartZ = pDestCell->m_wHighLayer * POINT_PER_ALTITUDE + nRelativeZ;
		}

		nDistance = g_GetDistance2(nStartX, nStartY, nEndX, nEndY);

		if(nDistance > nLastDistance)
			break;
	}

	return TRUE;
}

BOOL CharacterSkill::CheckObstacle(DWORD dwTargetId)
{
	if(!dwTargetId)
		return TRUE;

	INT nStartX, nStartY, nStartZ;
	m_rcCharacter.GetPosition(nStartX, nStartY, nStartZ);

	KCharacter* pTarget = g_cCharacterMgr.GetById(dwTargetId);
	QCONFIRM_RET_FALSE(pTarget);

	INT nEndX, nEndY, nEndZ;
	pTarget->GetPosition(nEndX, nEndY, nEndZ);

	return this->CheckObstacle(nStartX, nStartY, nStartZ, nEndX, nEndY, nEndZ, pTarget->m_nTouchRange);
}

BOOL CharacterSkill::CanCastSkill(WORD wSkillId,DWORD dwTargetId, INT nX, INT nY, INT nZ)
{
	if(m_mapSkillData.find(wSkillId) == m_mapSkillData.end())
	{
		//QLogPrintf(LOG_INFO, "找不到技能[%d]！", wSkillId);
		return FALSE;
	}
	
	//if (m_mapSkillData[wSkillId]->nCDTime != m_mapSkillData[wSkillId]->nCurrentCDTime || m_mapSkillData[wSkillId]->pSkill->IsActive())
	if (this->IsSkillInCD(wSkillId))
	{
		//QLogPrintf(LOG_INFO, "技能貌似进行中或未冷却完 SkillId:[%d]！ Active: [%d]", wSkillId, m_mapSkillData[wSkillId]->pSkill->IsActive());
		return FALSE;
	}

	if (IS_PLAYER(m_rcCharacter.m_dwId))
	{
		//门派限制
		BOOL* nFactions = m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->nFactionLimit;

		INT nCharFaction = m_rcCharacter.GetPrimaryFaction();
		if (!nFactions[nCharFaction])
		{
			//QLogPrintf(LOG_INFO, "技能门派限制 SkillId:[%d]！", wSkillId);
			return FALSE;
		}
		INT nRoute = m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->nRouteLimit;
		INT nCharRoute = m_rcCharacter.GetCurrentRoute();
		if ( nRoute > 0 && nRoute != nCharRoute)
		{
			//QLogPrintf(LOG_INFO, "技能路线限制 SkillId:[%d]！", wSkillId);
			return FALSE;
		}
	}
	
	if (m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->nFightSkillType != FIGHT_SKILL_PASSIVE)
	{
		KCharacter* pTarget = g_cCharacterMgr.GetById(dwTargetId);
		BOOL nIsNeedTarget = m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->bNeedTarget;
		if ( nIsNeedTarget && !pTarget)	//需要目标
		{
			//QLogPrintf(LOG_INFO, "技能需要目标 SkillId:[%d]！", wSkillId);
			return FALSE;
		}
	}

	// 为了保持技能的流畅性，玩家释放非控制技能不检查障碍 
	if (m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->bNeedTarget)	//对角色技能, 检查对角色障碍
	{
		if (IS_NPC(m_rcCharacter.GetId()))
		{
			if(!CheckObstacle(dwTargetId))
			{
				INT nStartX, nStartY, nStartZ;
				m_rcCharacter.GetPosition(nStartX, nStartY, nStartZ);
				//QLogPrintf(LOG_INFO, "[对人技能]释放者与目标人有障碍！释放者位置:%d,%d,%d  目标者位置:%d,%d,%d", nStartX, nStartY, nStartZ,nX, nY, nZ);
				return FALSE;
			}
		}
	}
	else // 对地技能，检查目标地障碍
	{
		INT nStartX, nStartY, nStartZ;
		m_rcCharacter.GetPosition(nStartX, nStartY, nStartZ);

		if(!CheckObstacle(nStartX, nStartY, nStartZ, nX, nY, nZ, m_rcCharacter.m_nTouchRange))
		{
			//QLogPrintf(LOG_INFO, "[对地技能]释放者与目标地方有障碍！");
			return FALSE;
		}
	}

	return TRUE;
}

// 得到当前正在释放的技能
INT CharacterSkill::GetCastingSkill()
{
	SKILLDATAMAP::iterator it;
	for (it = m_mapSkillData.begin(); it != m_mapSkillData.end(); ++ it)
	{
		if (it->second->pSkill->IsActive())
			return it->second->pSkill->m_pFightSkillTemplate->nSkillID;
	}
	return 0;
}

INT CharacterSkill::GetSkillType(INT nSkillId)
{
	FightSkillTemplate* pTemplate = g_cFightSkillSetting.GetFightSkillTemplate(nSkillId);
	QCONFIRM_RET_NULL(pTemplate);

	return pTemplate->nFightSkillKind;
}

BOOL CharacterSkill::ClearSkill()
{
	INT nSkillId = 0;
	BOOL bOK = TRUE;
	std::vector<WORD> arrSkillIds;
	SKILLDATAMAP::iterator it;
	for (it = m_mapSkillData.begin(); it != m_mapSkillData.end(); ++ it)
	{
		nSkillId = (*it).first;

		if(GetSkillType(nSkillId) == FIGHT_SKILL_KIND_PET)
			continue;

		arrSkillIds.push_back(nSkillId);
	}

	std::vector<WORD>::iterator it_idx;
	for (it_idx = arrSkillIds.begin(); it_idx != arrSkillIds.end(); ++ it_idx)
	{
		if (!RemoveSkill((*it_idx), FALSE))
		{
			bOK = FALSE;
			//QLogPrintf(LOG_ERR, "[CharacterSkill::ClearSkill]Remove skill '%d' failed!", (*it_idx));
		}
	}

	return bOK;
}

BOOL CharacterSkill::ClearSkillCD(WORD wSkillId)
{
	SKILLDATAMAP::const_iterator const_iter = m_mapSkillData.find(wSkillId);
	PROCESS_ERROR(const_iter != m_mapSkillData.end());

	{
		const_iter->second->nCurrentCDTime = 0;
		KPlayer* pPlayer = g_cPlayerMgr.GetById(m_rcCharacter.m_dwId);
		PROCESS_ERROR(pPlayer);
		FightSkillProtocolProcessor::SyncS2C_ClearCD(pPlayer->GetPlayerIndex(), m_rcCharacter.m_dwId, wSkillId);
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL CharacterSkill::RemoveSkill(WORD wSkillId, BOOL bSync)
{
	CONST BufferTemplate* pTemplate = NULL;
	SKILLDATA* pSkillData = NULL;
	PROCESS_ERROR(m_mapSkillData.find(wSkillId) != m_mapSkillData.end());
	
	pSkillData = m_mapSkillData[wSkillId];

	if (pSkillData->pSkill->m_pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_PASSIVE)
	{
		this->SetPassiveSkill(pSkillData->pSkill->m_pFightSkillTemplate->nPassiveSkillType, pSkillData->nSkillLevel, FALSE, FALSE); // no bSync

		//bSync = TRUE; // 被动技能移除也需要同步给客户端 by KK
	}

	m_mapSkillData.erase(wSkillId);
	SAFE_DELETE(pSkillData->pSkill);
	SAFE_DELETE(pSkillData);

	if (IS_PLAYER(m_rcCharacter.m_dwId) && bSync)
	{
		KPlayer* pPlayer = g_cPlayerMgr.GetById(m_rcCharacter.m_dwId);
		PROCESS_ERROR(pPlayer);
		//同步移除技能
		FightSkillProtocolProcessor::SyncS2C_RemoveSkill(pPlayer->GetPlayerIndex(),m_rcCharacter.GetId(),wSkillId);
	}

	//QLogPrintf(LOG_INFO, "[CharacrerSkill]Character[%d] Remove skill[%d]", m_rcCharacter.GetId(), wSkillId);

	return TRUE;
EXIT0:
	return FALSE;
}

// 释放技能前调整朝向
VOID CharacterSkill::TurnToBeforeCast(KCharacter* pCaster, KCharacter* pTarget)
{
	INT nHimX = 0;
	INT nHimY = 0;
	INT nHimZ = 0;
	INT nMeX = 0;
	INT nMeY = 0;
	INT nMeZ = 0;
	pCaster->GetPosition(nHimX, nHimY, nHimZ);
	pTarget->GetPosition(nMeX, nMeY, nMeZ);
	INT nDirection = g_GetDirection(nMeX, nMeY, nHimX, nHimY);
	pCaster->ChangeFaceDirection(nDirection, FALSE); // 不同步
}

BOOL CharacterSkill::CastSkill(WORD wSkillId,DWORD dwTargetId,INT nX,INT nY,INT nZ)
{
	return this->CastSkill(wSkillId, dwTargetId, nX, nY, nZ, FALSE);  // 默认不广播给自己
}

BOOL CharacterSkill::CastSkill(WORD wSkillId,DWORD dwTargetId,INT nX,INT nY,INT nZ, BOOL syncSelf, BOOL bSpecialTimeScale)
{
	FightSkill* pSkill = NULL;

	if (IS_NPC(m_rcCharacter.m_dwId))
	{
		// Npc 临时绕过需要学技能的限制, NPC技能不需要等级哦
		if (m_mapSkillData.find(wSkillId) == m_mapSkillData.end())
		{
			LOG_PROCESS_ERROR(AddSkill(wSkillId, FALSE));
		}
	}
	LOG_PROCESS_ERROR(m_mapSkillData.find(wSkillId) != m_mapSkillData.end());

	pSkill = m_mapSkillData[wSkillId]->pSkill;
	LOG_PROCESS_ERROR(pSkill);
	
	// 设置技能是否时间缩放
	pSkill->m_bSpecialTimeScale = FALSE;
	// 暴走状态
	if(m_rcCharacter.GetAngerLeftTime() > 0)
	{
		pSkill->m_bSpecialTimeScale = TRUE;
	}
	//pSkill->m_bSpecialTimeScale = bSpecialTimeScale;

	// 吃药技能判断
	if (pSkill->m_pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_MEDICINE)  // 吃药技能
	{
		this->CooldownSkill(wSkillId);
		ApplyMedicineSkill(pSkill->m_pFightSkillTemplate->nSlot, pSkill->m_nSkillLevel);
		return TRUE;
	}

	if (!this->CheckSkillCastDistance(wSkillId, dwTargetId, nX, nY, nZ))
	{
		//QLogPrintf(LOG_INFO, "距离太远技能无法释放");
		return FALSE;
	}

	{
		if(IS_PLAYER(m_rcCharacter.GetId()))
		{
			BOOL bCheckConsume = pSkill->CheckCastConsume();
			PROCESS_ERROR(bCheckConsume);
		}
	}
	
	this->CooldownSkill(wSkillId);

	// 目标判定开始
	if (pSkill->m_pFightSkillTemplate->bNeedTarget)	//对角色技能
	{
		if (pSkill->m_pFightSkillTemplate->bIsImmediately) // 立即释放是对自身
		{
			KCharacter* pTarget = g_cCharacterMgr.GetById(m_rcCharacter.GetId());
			LOG_PROCESS_ERROR(pTarget);
			pSkill->BindTarget(pTarget);
		}
		else
		{
			KCharacter* pTarget = g_cCharacterMgr.GetById(dwTargetId);
			LOG_PROCESS_ERROR(pTarget);
			pSkill->BindTarget(pTarget);
			TurnToBeforeCast(&m_rcCharacter, pTarget);
		}

		{
			BOOL bCheckRelation = pSkill->CheckCastRelation();
			PROCESS_ERROR(bCheckRelation);
		}
	}
	else	//对地技能
	{
		if (pSkill->m_pFightSkillTemplate->bIsImmediately) // 立即释放是对自身所在点
		{
            INT nDestX, nDestY, nDestZ;
			m_rcCharacter.GetPosition(nDestX, nDestY, nDestZ);
			pSkill->SetDestPostion(nDestX, nDestY, nDestZ);
		}
		else
		{
            pSkill->SetDestPostion(nX,nY,nZ);
		}
	}

	if (IS_PLAYER(m_rcCharacter.m_dwId) && m_rcCharacter.GetMoveState() != cmsOnStand)
	{
		//QLogPrintf(LOG_ERR, "[CastSkill]玩家发技能时不在站立状态！");
		return FALSE;
	}
	else if (m_rcCharacter.GetMoveState() == cmsOnRun)
	{
		// 非玩家，正在跑动，停止跑步
		m_rcCharacter.StopMoving(); // 释放技能时停止移动, 不适用stopmove, 强制停下
	}

	//pSkill->SetActive(TRUE);
	pSkill->StartSkill();

	{//事件触发
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

		cSafeScript->CallTableFunction("BufferCondition", "SkillOut", 0, "ddd",
			m_rcCharacter.GetId(), dwTargetId, wSkillId);
	}

	FightSkillProtocolProcessor::BroadcastStartSkill(m_rcCharacter, dwTargetId, wSkillId,nX,nY,nZ, syncSelf);

	return TRUE;
EXIT0:
	return FALSE;
}


// 冷却技能
BOOL CharacterSkill::CooldownSkill(WORD wSkillId)
{
	// 冷却时间，  公共CD时间冷却时间
	m_mapSkillData[wSkillId]->nCurrentCDTime = m_mapSkillData[wSkillId]->nCDTime; // 设置CD时间
	m_mapSkillData[wSkillId]->bStartCalcCD = TRUE;	//释放完之后开始计算CD时间

	if(IS_PLAYER(m_rcCharacter.GetId()) && m_rcCharacter.GetAngerLeftTime() > 0)
	{
		INT nSpecialCDTime = m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->nSpecialCdTime;
		if(nSpecialCDTime > 0)
		{
			m_mapSkillData[wSkillId]->nCurrentCDTime = m_mapSkillData[wSkillId]->nCDTime * nSpecialCDTime / 100;
		}
	}

	// 所有发射者的其它技能加上公共CD时间冷却时间 by KK

	if (m_mapSkillData[wSkillId]->pSkill->m_pFightSkillTemplate->nFightSkillType != FIGHT_SKILL_MEDICINE) // 回血回蓝技能不触发公共cd
	{
		for (SKILLDATAMAP::iterator it = m_mapSkillData.begin(); it != m_mapSkillData.end(); ++it)
		{
			if ((WORD)it->first == wSkillId) continue; // 跳过正在发射的技能

			SKILLDATA* skillData = it->second;

			// 如果已经冷却中
			if (skillData->bStartCalcCD)
			{
				// 判断当前冷却时间是否小于公共冷却时间，如果是，增加公共冷却时间
				if (skillData->nCurrentCDTime < COMMON_SKILL_COOLDOWN)
				{
					// 1.0s
					skillData->nCurrentCDTime = COMMON_SKILL_COOLDOWN;
				}
				// 当前冷却时间大于公共冷却时间，不做事
			}
			else  // 新冷却的
			{
				skillData->nCurrentCDTime = COMMON_SKILL_COOLDOWN;
				skillData->bStartCalcCD = TRUE;
			}
		}
	}

	return TRUE;
}

CONST CharacterSkill::SKILLDATAMAP& CharacterSkill::GetSkillMap() CONST
{
	return m_mapSkillData;
}

CharacterSkill::SKILLDATAMAP& CharacterSkill::GetSkillMap()
{
	return m_mapSkillData;
}

BOOL CharacterSkill::OnClientReady()
{
	if( m_bMapLoadEmpty)
	{
		return TRUE;
	}
	
	this->SyncSkillsToClient(TRUE);  //同步技能

	return TRUE;
}

BOOL CharacterSkill::SaveDelaySkillData(QDelaySkillData* pDelaySkillData)
{
    _ASSERT(pDelaySkillData);

	INT nSkillIndex = 0;
	SKILLDATAMAP::iterator it_skill;
	for (it_skill = m_mapSkillData.begin(); it_skill != m_mapSkillData.end(); ++it_skill)
	{
		INT nEnumValue = MAKELONG(it_skill->second->nSkillLevel, it_skill->second->nSkillId);  // Id + Level 组合
        pDelaySkillData->arrSkillIdAndLevel[nSkillIndex++] = nEnumValue;
	}

	pDelaySkillData->nFactionRoute = m_rcCharacter.GetCurrentRoute();

	return TRUE;
}

BOOL CharacterSkill::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	m_bDbLoaded = TRUE;

	if (uSize == 0)
		return TRUE;

	if (pData == NULL || uSize < (sizeof(SkillSaveData)))
	{
		//QLogPrintf(LOG_ERR, "[CharacterSkill::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(SkillSaveData + SkillLevelDbSaveData");
		return FALSE;
	}

	SIZE_T uLoadBuffSize = 0;
	while (uLoadBuffSize < uSize)
	{
		if (uLoadBuffSize + sizeof(SkillSaveData) > uSize)
		{
			//QLogPrintf(LOG_ERR, "[CharacterSkill::OnDbLoad]	The Size is more than uSize");
			return FALSE;
		}

		SkillSaveData* pSkillSaveData = (SkillSaveData*)pData;
		AddSkill(pSkillSaveData->nSkillId,FALSE, pSkillSaveData->nSkillLevel);
		if (m_mapSkillData.find(pSkillSaveData->nSkillId) != m_mapSkillData.end())
		{
			SKILLDATA* pSkillData = m_mapSkillData[pSkillSaveData->nSkillId];
			pSkillData->bStartCalcCD = FALSE;
			//pSkillData->nCDTime = pSkillSaveData->nCDTime;
			pSkillData->nCurrentCDTime = pSkillSaveData->nCurrentCDTime;
		}

		// 技能等级保存
		m_mapSkillLevelData[pSkillSaveData->nSkillId] = pSkillSaveData->nSkillLevel;

		pData += sizeof(SkillSaveData);
		uLoadBuffSize += sizeof(SkillSaveData);
	}
	if ( m_mapSkillData.size() <= 0 )
	{
		m_bMapLoadEmpty = TRUE;
	}


	// 读取技能等级
	//SIZE_T uLoadBuffSize = 0;
	/*while (uLoadBuffSize < uSize)
	{
		if (uLoadBuffSize + sizeof(SkillLevelDbSaveData) > uSize)
		{
			QLogPrintf(LOG_ERR, "[PlayerFaction::OnDbLoad]	The Size is more than uSize");
			return FALSE;
		}

		SkillLevelDbSaveData* pSkillLevelData = (SkillLevelDbSaveData*)pData;
		m_mapSkillLevelData[pSkillLevelData->nSkillTemplateId] = pSkillLevelData->nGrade;

		pData += sizeof(SkillLevelDbSaveData);
		uLoadBuffSize += sizeof(SkillLevelDbSaveData);
	}*/
	return TRUE;
}

BOOL CharacterSkill::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	if (m_mapSkillData.size() * sizeof(SkillSaveData) > uBuffSize)
	{
		//QLogPrintf(LOG_ERR, "[CharacterSkill::OnDbSave]	The real size is more than uBuffSzie");
		return FALSE;
	}

	// 开始保存
	for (SKILLDATAMAP::iterator it = m_mapSkillData.begin(); it != m_mapSkillData.end(); ++it)
	{
		*(SkillSaveData*)pBuffer = SkillSaveData(it->first,it->second->nCDTime,it->second->nCurrentCDTime, it->second->nSkillLevel);	
		pBuffer += sizeof(SkillSaveData);
		uUsedSize += sizeof(SkillSaveData);
	}

	// 保存技能等级
	//for (FACTIONSKILLMAP::iterator it = m_mapSkillLevelData.begin(); it != m_mapSkillLevelData.end(); ++it)
	//{
	//	*(SkillLevelDbSaveData*)pBuffer = SkillLevelDbSaveData(it->first, it->second);  // skillid -> lv
	//	pBuffer += sizeof(SkillLevelDbSaveData);
	//	uUsedSize += sizeof(SkillLevelDbSaveData);
	//}

	return TRUE;
}

BOOL CharacterSkill::OnLogin()
{
	INT nLevel = m_rcCharacter.GetLevel();

	{
		if (m_rcCharacter.IsFirstTimeLogin())
		{
			this->ReloadPlayerSkills();  // 第一次登陆, 通过这里传递

			m_bDbLoaded = TRUE;  // 第一不用读取，所以假定已读取
		}

	}

	return TRUE;
EXIT0:
	return FALSE;
}
LuaCharacterSkill* CharacterSkill::GetScriptInterface() CONST
{
	return m_pLunaObj;
}

VOID CharacterSkill::AddSkillCritical(WORD wCateType, WORD wCateId, UINT uPercent)
{
	DWORD dwKey = MAKELONG(wCateType, wCateId);

	LOG_PROCESS_ERROR( !(m_tSkillCriticalP[dwKey] < 0) );	// 会心百分比必须!<0

	m_tSkillCriticalP[dwKey] += uPercent;

EXIT0:
	return;
}

VOID CharacterSkill::DelSkillCritical(WORD wCateType, WORD wCateId, UINT uPercent)
{
	DWORD dwKey = MAKELONG(wCateType, wCateId);

	LOG_PROCESS_ERROR( !(m_tSkillCriticalP[dwKey] - uPercent < 0) );	// 会心百分比必须!<0

	m_tSkillCriticalP[dwKey] -= uPercent;

EXIT0:
	return;
}

INT CharacterSkill::GetSkillCritical(WORD wCateType, WORD wCateId)
{
	DWORD dwKey = MAKELONG(wCateType, wCateId);

	LOG_PROCESS_ERROR( !(m_tSkillCriticalP[dwKey] < 0) );	// 会心百分比必须!<0

	return m_tSkillCriticalP[dwKey];

EXIT0:
	return 0;
}



// 获取技能等级
INT CharacterSkill::GetSkillLevel(INT nSkillTemplateId)
{
	if (m_mapSkillLevelData.find(nSkillTemplateId) == m_mapSkillLevelData.end())
		return 0;
	return m_mapSkillLevelData[nSkillTemplateId];
}

// 技能重新传递
// 添加技能  (用于引用)
BOOL CharacterSkill::ReloadPlayerSkills()
{
	// 获取技能  (用于引用)
	std::vector<FightSkillTemplate*> vFactionSkills;

	INT nLevel = m_rcCharacter.GetLevel();
	INT nFaction = m_rcCharacter.GetPrimaryFaction();
	//INT nRoute = m_rcCharacter.GetCurrentRoute();

	if (g_FightSkillManager.GetCharacterAvailableSkills(nFaction, nLevel, vFactionSkills)) 
	{
		CharacterSkill* pCharacterSkill = 0;
		std::vector<FightSkillTemplate*>::iterator iter;

		pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
		LOG_PROCESS_ERROR(pCharacterSkill);

		pCharacterSkill->ClearSkill();  // 清除所有当前技能, 后reload

		for (iter = vFactionSkills.begin(); iter != vFactionSkills.end(); ++iter)  
		{
			FightSkillTemplate* skillTem = *iter;

			// 没有改技能的等级信息，默认1
			if (m_mapSkillLevelData.find(skillTem->nSkillID) == m_mapSkillLevelData.end())
			{
				m_mapSkillLevelData[skillTem->nSkillID] = 1;
			}

			INT nSkillLevel = m_mapSkillLevelData[skillTem->nSkillID];
			pCharacterSkill->AddSkill(skillTem->nSkillID, FALSE, nSkillLevel);  // 添加
		}

		pCharacterSkill->SyncSkillsToClient();   // TODO: 直接传送技能, 未用m_arrSkillLevelData
	}

EXIT1:
	return TRUE;
EXIT0:
	return FALSE;
}

// 获取抗性技能的等级
INT CharacterSkill::GetResistSkillLevel(INT nResistType)
{
	for (SKILLDATAMAP::iterator it = m_mapSkillData.begin(); it != m_mapSkillData.end(); ++it)
	{
		INT nSkillKind = (*it).second->pSkill->m_pFightSkillTemplate->nFightSkillKind;
		INT nSkillResistType = (*it).second->pSkill->m_pFightSkillTemplate->nPassiveSkillType;

		if (nSkillKind == emSKILL_TYPE_RESIST && nResistType == nSkillResistType)  // 指定抗性的抗性技能
		{
			return (*it).second->nSkillLevel;
		}
	}

	return 0;  // 玩家未拥有该技能
}

BOOL CharacterSkill::CheckSkillUpgradeCost(DWORD dwSkillTemplateId)
{
    FightSkillTemplate* pFightSkillTemplate =  g_cFightSkillSetting.GetFightSkillTemplate((WORD)dwSkillTemplateId);
	QCONFIRM_RET_FALSE(pFightSkillTemplate);

	INT& rnCurrentSkillLevel = m_mapSkillLevelData[dwSkillTemplateId]; // 引用

	QString moduleName;
	if (pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_MEDICINE || pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_PASSIVE)
	{
	    moduleName = "passiveskill";
	}
	else if (pFightSkillTemplate->nFightSkillType == emSKILL_TYPE_RESIST)
	{
	    moduleName = "resistskill";   
	}
	else
	{
		moduleName = "initiativeskill";
	}

	CONST QNumericalAward* pNumericalAward = g_cAwardSetting.RequestNumericalAward(moduleName, 1, rnCurrentSkillLevel + 1);
	QCONFIRM_RET_FALSE(pNumericalAward);

	KPlayerPurse* pPlayerPurse = m_rcCharacter.GetCtrl<KPlayerPurse>();
	QCONFIRM_RET_FALSE(pPlayerPurse);

	INT nEnergyHave = pPlayerPurse->GetMoney(EM_MONEY_YLENERGY);
	if (nEnergyHave >= pNumericalAward->nCost)
	{
		KE_PURSE_RESULT eResult;
	    pPlayerPurse->SetMoney(EM_MONEY_YLENERGY, nEnergyHave - pNumericalAward->nCost, &eResult);
	}
	else
	{
	    return FALSE;
	}

	return TRUE;
}

BOOL CharacterSkill::UpgradeSkill(DWORD nSkillTemplateId)
{
	QLogPrintf(LOG_INFO,"CharacterSkill::UpgradeSkill(DWORD nSkillTemplateId = %d)",nSkillTemplateId);
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript(); // 成功升级后，执行新手指引任务变量
	INT nPlayerLevel = m_rcCharacter.GetLevel();
	SKILLDATA* pSkillData = NULL;
	KPlayerPurse* pPlayerPurse = NULL;
	char szStr[256];
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	if (m_mapSkillLevelData.find(nSkillTemplateId) == m_mapSkillLevelData.end())
	{
		//QLogPrintf(LOG_ERR, "没有这个可升级技能: %d", nSkillTemplateId);
		return FALSE; // 无该技能
	}

	// 升级技能，不能大于主角等级 
	INT& rnCurrentSkillLevel = m_mapSkillLevelData[nSkillTemplateId]; // 引用
	if (nPlayerLevel <= rnCurrentSkillLevel)  // 玩家角色等级小于等于技能等级，无效
	{
		//QLogPrintf(LOG_ERR, "技能等级大于等于玩家等级，技能%d", nSkillTemplateId);
		return FALSE; // 无该技能
	}

	if (!CheckSkillUpgradeCost(nSkillTemplateId))
	{
		//QLogPrintf(LOG_ERR, "真气不足，技能%d", nSkillTemplateId);
		return FALSE; // 真气不足
	}

	rnCurrentSkillLevel++; // 升级

	FightSkillTemplate* pFightSkillTemplate = g_cFightSkillSetting.GetFightSkillTemplate(nSkillTemplateId);
	LOG_PROCESS_ERROR(pFightSkillTemplate);

	LOG_PROCESS_ERROR(m_mapSkillData.find(nSkillTemplateId) != m_mapSkillData.end());
	pSkillData = m_mapSkillData[nSkillTemplateId];
	LOG_PROCESS_ERROR(pSkillData);
	pSkillData->nSkillLevel = rnCurrentSkillLevel;
	pSkillData->pSkill->m_nSkillLevel = rnCurrentSkillLevel;

	if (pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_PASSIVE) // 如是被动技能，直接给玩家添加永久buffer
	{
		if ((rnCurrentSkillLevel - 1) > 0)
		{
		    this->AddPassiveSkillValue(pFightSkillTemplate->nPassiveSkillType, rnCurrentSkillLevel - 1, TRUE, FALSE);  // sub
		}
		this->AddPassiveSkillValue(pFightSkillTemplate->nPassiveSkillType, rnCurrentSkillLevel, FALSE, TRUE);  // Add
	}

	// 技能重新传递
	//this->ReloadPlayerSkills();

	g_cCharacterHelper.CalcFightScore(m_rcCharacter);
	pPlayerPurse = m_rcCharacter.GetCtrl<KPlayerPurse>();
	QCONFIRM_RET_FALSE(pPlayerPurse);
	ZeroMemory(szStr,sizeof(szStr));
	sprintf(szStr,"nSkillTemplateId=%d,rnCurrentSkillLevel=%d",nSkillTemplateId,rnCurrentSkillLevel);
	//统计点  by ldy
	pPlayerPurse->LogMoney(EM_MONEY_COIN,0,52,&nResult,1,szStr);
	return TRUE;

EXIT0:
	return FALSE;
}
