/* -------------------------------------------------------------------------
//	文件名		：	luacharacterskill.h
//	创建者		：	zhangzhixiong
//	实现者		：	
//	创建时间	：	20012-7-6 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "onlinegamemodule/luacharacterskill.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegamemodule/characterskill.h"
#include "onlinegameworld/kluacharacter.h"
#include "fightskill/fightskill.h"
#include "fightskill/fightskillsetting.h"
#include "fightskill/fightskillprotocolprocessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
INT LuaCharacterSkill::Register2Character(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaCharacter* pLunaCharacter = dynamic_cast<KLuaCharacter*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaCharacter);
	KCharacter* pCharacter = pLunaCharacter->GetHim();
	QCONFIRM_RET_FALSE(pCharacter);

	CharacterSkill* pCharacterSkill = pCharacter->GetCtrl<CharacterSkill>();
	QCONFIRM_RET_FALSE(pCharacterSkill);
	sc.PushObj(pCharacterSkill->GetScriptInterface());

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: CastSkill
// @Description		: 释放技能
// @ReturnCode		: 
// @ArgumentFlag	: dd[ddd]
// @ArgumentComment	: 技能ID
// @ArgumentComment	: 接受者ID
// @ArgumentComment	: 导弹技的目标点X
// @ArgumentComment	: 导弹技的目标点Y
// @ArgumentComment	: 导弹技的目标点Z
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaCastSkill(QLuaScript& sc)
{
	INT nSkillID   = sc.GetInt(1);
	INT nReceiverID = sc.GetInt(2);
	INT nX = 0;
	INT nY = 0;
	INT nZ = 0;
	if(sc.IsNumber(3))
	{
		nX         = sc.GetInt(3);
		nY         = sc.GetInt(4);
		nZ         = sc.GetInt(5);
	}
	
	BOOL bOK = TRUE;
	CharacterSkill* pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	bOK = pCharacterSkill->CastSkill((WORD)nSkillID, (DWORD)nReceiverID, nX, nY, nZ, FALSE);
	sc.PushNumber(bOK);
	return 1;
EXIT0:
	bOK = FALSE;
	sc.PushNumber(bOK);
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: GetResistSkillLevel
// @Description		: 获取指定抗性技能等级
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 抗性技能类型
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaGetResistSkillLevel(QLuaScript& sc)
{
	INT nResistType   = sc.GetInt(1);
	INT nLevel = 0;
	CharacterSkill* pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	nLevel = pCharacterSkill->GetResistSkillLevel(nResistType);

EXIT0:
	sc.PushNumber(nLevel);
	return 1;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: CanCastSkill
// @Description		: 是否能释放技能
// @ReturnCode		: 
// @ArgumentFlag	: ddddd
// @ArgumentComment	: 技能ID
// @ArgumentComment	: 接受者ID
// @ArgumentComment	: 导弹技的目标点X
// @ArgumentComment	: 导弹技的目标点Y
// @ArgumentComment	: 导弹技的目标点Z
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaCanCastSkill(QLuaScript& sc)
{
	INT nSkillID   = sc.GetInt(1);
	INT nReceiverD = sc.GetInt(2);
	INT nX         = sc.GetInt(3);
	INT nY         = sc.GetInt(4);
	INT nZ         = sc.GetInt(5);

	BOOL bOK = TRUE;
	CharacterSkill* pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);  

	bOK = pCharacterSkill->CanCastSkill((WORD)nSkillID, (DWORD)nReceiverD, nX, nY, nZ);
	sc.PushNumber(bOK);
	return 1;
EXIT0:
	bOK = FALSE;
	sc.PushNumber(bOK);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: GetSkillCoefficient
// @Description		: 技能的計算信息(是否物理攻擊、成長值)
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 技能ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaGetSkillCoefficient(QLuaScript& sc)
{
	INT nSkillId = sc.GetInt(1);
	FightSkillTemplate* pTemplate = g_cFightSkillSetting.GetFightSkillTemplate(nSkillId);
	QCONFIRM_RET_NULL(pTemplate);

	sc.PushNumber(pTemplate->nCoefficientMin);
	sc.PushNumber(pTemplate->nCoefficientMax);

	return 2;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: GetSkillFightInfo
// @Description		: 技能的計算信息(是否物理攻擊、成長值)
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 技能ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaGetSkillFightInfo(QLuaScript& sc)
{
	INT		nSkillIsPhysical		= 0;
	INT		nSkillReqLevel			= 1;
	INT		nSkillCriticalPercent	= 0;
	INT nSkillID = sc.GetInt(1);
	CharacterSkill* pCharacterSkill = 0;
	CharacterSkill::SKILLDATAMAP::const_iterator it_skill;
	UINT uCriticalP = 0;
	
	{
		pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
		LOG_PROCESS_ERROR(pCharacterSkill);

		CONST CharacterSkill::SKILLDATAMAP& mapSkills = pCharacterSkill->GetSkillMap();
		it_skill = mapSkills.find(nSkillID);
		if(it_skill == mapSkills.end())
		{
			printf("Can't find SkillId:%d!!!!!\n", nSkillID);
		}
		PROCESS_ERROR(it_skill != mapSkills.end());

		uCriticalP += pCharacterSkill->GetSkillCritical(1, it_skill->second->pSkill->m_pFightSkillTemplate->nCategory);
		nSkillIsPhysical		= it_skill->second->pSkill->m_pFightSkillTemplate->bIsPhysical;
		nSkillReqLevel			= it_skill->second->pSkill->m_pFightSkillTemplate->nReqLevel;
		nSkillCriticalPercent	= uCriticalP;
	}


EXIT0:
	sc.PushNumber(nSkillIsPhysical);
	sc.PushNumber(nSkillReqLevel);
	sc.PushNumber(nSkillCriticalPercent);
	return 3;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: GetSkillCate
// @Description		: 当前Buff所属技能的Cate信息
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 技能ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaGetSkillCate(QLuaScript& sc)
{
	INT nSkillID = sc.GetInt(1);
	CharacterSkill::SKILLDATAMAP::const_iterator it_skill;

	CharacterSkill* pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	{
		CONST CharacterSkill::SKILLDATAMAP& mapSkills = pCharacterSkill->GetSkillMap();
		it_skill = mapSkills.find(nSkillID);
		if (it_skill == mapSkills.end())
		{
			QLogPrintf(LOG_ERR, "[LuaCharacterSkill::LuaGetSkillManification]Wrong skill id '%d'", nSkillID);
			goto EXIT0;
		}

		CONST FightSkillTemplate* pSkillTemplate = it_skill->second->pSkill->m_pFightSkillTemplate;
		LOG_PROCESS_ERROR(pSkillTemplate);

		{
			sc.PushTable();
			INT cate1 = pSkillTemplate->nCategory;
			sc.PushNumber(cate1);
			sc.SetTableField("cate1");
			INT cate2 = 0;					//pSkillTemplate->nCategory;
			sc.PushNumber(cate2);
			sc.SetTableField("cate2");
			INT cate3 = 0;					//pSkillTemplate->nCategory;
			sc.PushNumber(cate3);
			sc.SetTableField("cate3");
		}
	}

	return 1;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: GetSkillAttackType
// @Description		: 技能的攻击类型(NeedTarget, IsImmediately)
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 技能ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaGetSkillAttackType(QLuaScript& sc)
{
	INT nSkillID = sc.GetInt(1);
	CharacterSkill::SKILLDATAMAP::const_iterator it_skill;

	CharacterSkill* pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	{
		CONST CharacterSkill::SKILLDATAMAP& mapSkills = pCharacterSkill->GetSkillMap();
		it_skill = mapSkills.find(nSkillID);
		if (it_skill == mapSkills.end())
		{
			QLogPrintf(LOG_ERR, "[LuaCharacterSkill::LuaGetSkillManification]Wrong skill id '%d'", nSkillID);
			goto EXIT0;
		}

		{
			INT		bNeedTarget		= it_skill->second->pSkill->m_pFightSkillTemplate->bNeedTarget;
			BOOL	bIsImmediately	= it_skill->second->pSkill->m_pFightSkillTemplate->bIsImmediately;
			sc.PushNumber(bNeedTarget);
			sc.PushNumber(bIsImmediately);
		}

	}

	return 2;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: ReduceSkillCost
// @Description		: 修改技能的消耗
// @ReturnCode		: 
// @ArgumentFlag	: dddd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @ArgumentComment	: 是值还是百分比
// @ArgumentComment	: 修改值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaReduceSkillCost(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);
	BOOL	bIsValue		= sc.GetInt(3);
	DOUBLE  dReducePercent = 0;
	INT		nReduce = 0;
	if(bIsValue)
		nReduce = sc.GetInt(4);
	else
		dReducePercent = sc.GetNum(4);

	FightSkill* pSkill = NULL;
	FightSkillTemplate* pSkillTemplate = NULL;

	CharacterSkill::SKILLDATAMAP mapSkill;
	CharacterSkill::SKILLDATAMAP::iterator it_skill;
	CharacterSkill* pCharacterSkill = 0;

	pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	// 目前只有一种CateType
	ASSERT(nCateType == 1);

	mapSkill = pCharacterSkill->GetSkillMap();

	for (it_skill = mapSkill.begin(); it_skill != mapSkill.end(); ++it_skill)
	{
		pSkill = it_skill->second->pSkill;
		LOG_PROCESS_ERROR(pSkill);

		pSkillTemplate = pSkill->m_pFightSkillTemplate;
		LOG_PROCESS_ERROR(pSkillTemplate);

		if(nCateId == -1 || pSkillTemplate->nCategory == nCateId)
		{
			if(bIsValue)
			{
				pSkill->m_nReduceCost = nReduce;
			}
			else
			{
				pSkill->m_fReduceCostPercent = (FLOAT)dReducePercent;
			}
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: ResetSkillCost
// @Description		: 重置技能的消耗
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaResetSkillCost(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);

	FightSkill* pSkill = NULL;
	FightSkillTemplate* pSkillTemplate = NULL;

	CharacterSkill::SKILLDATAMAP mapSkill;
	CharacterSkill::SKILLDATAMAP::iterator it_skill;
	CharacterSkill* pCharacterSkill = 0;

	pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	// 目前只有一种CateType
	ASSERT(nCateType == 1);

	mapSkill = pCharacterSkill->GetSkillMap();

	for (it_skill = mapSkill.begin(); it_skill != mapSkill.end(); ++it_skill)
	{
		pSkill = it_skill->second->pSkill;
		LOG_PROCESS_ERROR(pSkill);

		pSkillTemplate = pSkill->m_pFightSkillTemplate;
		LOG_PROCESS_ERROR(pSkillTemplate);

		if(nCateId == -1 || pSkillTemplate->nCategory == nCateId)
		{
			pSkill->m_nReduceCost = 0;
			pSkill->m_fReduceCostPercent = 0;
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: ResetSkillMagnification
// @Description		: 修改技能的攻击力系数
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @ArgumentComment	: 修改值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaResetSkillMagnification(QLuaScript& sc)
{
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCancelChannelSkill
// @LuaApiName		: CancelChannelSkill
// @Description		: 打断当前正在读条的技能
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaCancelChannelSkill(QLuaScript& sc)
{
	INT nSkillId = 0;
	CharacterSkill* pCharacterSkill = NULL;

	pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);
	nSkillId = pCharacterSkill->GetCastingSkill();
	if(!nSkillId)
		return 0;

	pCharacterSkill->CancelChannelSkill(nSkillId);

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: ResetSkillChannelTime
// @Description		: 修改技能的施法时间
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @ArgumentComment	: 修改值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaResetSkillChannelTime(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);
	INT		nValue			= sc.GetInt(3);

	CharacterSkill::SKILLDATAMAP::iterator it_skill;
	CharacterSkill* pCharacterSkill = 0;

	pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	for (it_skill = pCharacterSkill->GetSkillMap().begin(); it_skill != pCharacterSkill->GetSkillMap().end(); ++it_skill)
	{
		switch(nCateType)
		{
		case 1:
			{
				if(it_skill->second->pSkill->m_pFightSkillTemplate->nCategory == nCateId)
				{
					it_skill->second->pSkill->m_nCurrentChannelTime += nValue;
				}
			}
			break;
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: AddSkillCriticalPercent
// @Description		: 修改技能的暴击率
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @ArgumentComment	: 修改值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaAddSkillCriticalPercent(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);
	INT		nPercent		= sc.GetInt(3);

	CharacterSkill::SKILLDATAMAP::iterator it_skill;
	CharacterSkill* pCharacterSkill = 0;

	pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	{
		if(nPercent > 0)
		{
			pCharacterSkill->AddSkillCritical(nCateType, nCateId, nPercent);
		}
		else
		{
			pCharacterSkill->DelSkillCritical(nCateType, nCateId, -nPercent);
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: ClearSkillCD
// @Description		: 修改技能CD
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaClearSkillCD(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);

	CharacterSkill::SKILLDATAMAP::iterator it_skill;
	CharacterSkill* pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);
	{
		CharacterSkill::SKILLDATAMAP& skillmap_ref = pCharacterSkill->GetSkillMap();

		for (it_skill = skillmap_ref.begin(); it_skill != skillmap_ref.end(); ++it_skill)
		{
			switch(nCateType)
			{
			case 1:
				{
					if(it_skill->second->pSkill->m_pFightSkillTemplate->nCategory == nCateId)
					{
						pCharacterSkill->ClearSkillCD( it_skill->second->pSkill->m_pFightSkillTemplate->nSkillID );
					}
				}
				break;
			case 2:
				break;
			case 3:
				break;
			}
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: ResetSkillChannelType
// @Description		: 修改技能引导类型
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @ArgumentComment	: 设置技能释放的类型(0、1、2、-1:设回默认)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaResetSkillChannelType(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);
	INT		nChannelType	= sc.GetInt(3);
	FightSkillTemplate* pSkillTemplate = NULL;
	CharacterSkill::SKILLDATAMAP::iterator it_skill;
	CharacterSkill* pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();

	LOG_PROCESS_ERROR(pCharacterSkill);
	LOG_PROCESS_ERROR(nCateType == 1);
	{
		CharacterSkill::SKILLDATAMAP& skillmap_ref = pCharacterSkill->GetSkillMap();

		for (it_skill = skillmap_ref.begin(); it_skill != skillmap_ref.end(); ++it_skill)
		{
			pSkillTemplate = it_skill->second->pSkill->m_pFightSkillTemplate;
			if(pSkillTemplate->nCategory == nCateId)
			{
				if(nChannelType == 0 || nChannelType == 1 || nChannelType == 2)
				{
					it_skill->second->pSkill->m_nFightSkillChannel = nChannelType;
				}
				else if (nChannelType == -1)
				{
					it_skill->second->pSkill->m_nFightSkillChannel =
						pSkillTemplate->nFightSkillChannel;
				}
				else
				{
					LOG_PROCESS_ERROR(!"nChannelType 非法");
				}
				if(nChannelType == -1)
					nChannelType = pSkillTemplate->nFightSkillChannel;
				FightSkillProtocolProcessor::BroadcastChangeChannelState(m_rcCharacter, pSkillTemplate->nSkillID, FIGHT_SKILL_STATE_CHANNEL, nChannelType);
			}
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCharacterSkill
// @LuaApiName		: AddBuffToSkill
// @Description		: 修改技能引导类型
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 技能nCateType
// @ArgumentComment	: 技能nCateId
// @ArgumentComment	: Buff ID(+:加Buff，-:减Buff)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaAddBuffToSkill(QLuaScript& sc)
{
	INT		nCateType		= sc.GetInt(1);
	INT		nCateId			= sc.GetInt(2);
	INT		nBuffId			= sc.GetInt(3);

	CharacterSkill::SKILLDATAMAP::iterator it_skill;
	CharacterSkill* pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);
	{
		CharacterSkill::SKILLDATAMAP& skillmap_ref = pCharacterSkill->GetSkillMap();

		for (it_skill = skillmap_ref.begin(); it_skill != skillmap_ref.end(); ++it_skill)
		{
			switch(nCateType)
			{
			case 1:
				{
					if(it_skill->second->pSkill->m_pFightSkillTemplate->nCategory == nCateId)
					{
						if(nBuffId < 0)
						{
							nBuffId = -nBuffId;
							for(size_t i = 1; i <= 4; ++i)
							{
								INT buff_id = it_skill->second->pSkill->GetCurrentIncroBuffer(i);
								if (buff_id == nBuffId)
								{
									it_skill->second->pSkill->SetCurrentIncroBuffer(i, 0);
									return 0;
								}
							}
						}
						else if(nBuffId > 0)
						{
							for(size_t i = 1; i <= 4; ++i)
							{
								INT buff_id = it_skill->second->pSkill->GetCurrentIncroBuffer(i);
								if (buff_id == 0)
								{
									it_skill->second->pSkill->SetCurrentIncroBuffer(i, nBuffId);
									return 0;
								}
							}
						}
						else
						{
							LOG_PROCESS_ERROR(!"nChannelType 非法");
						}
					}
				}
				break;
			case 2:
				break;
			case 3:
				break;
			}
		}
	}

	return 0;
EXIT0:
	sc.PushNumber(0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaCheckObstacle
// @LuaApiName		: CheckObstacle
// @Description		: 判断目标或目标点障碍
// @ReturnCode		: 
// @ArgumentFlag	: dddddd
// @ArgumentComment	: 起始点X
// @ArgumentComment	: 起始点Y
// @ArgumentComment	: 起始点Z
// @ArgumentComment	: 结束点X
// @ArgumentComment	: 结束点Y
// @ArgumentComment	: 结束点Z
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCharacterSkill::LuaCheckObstacle(QLuaScript& sc)
{
	INT		nStartX		= sc.GetInt(1);
	INT		nStartY		= sc.GetInt(2);
	INT		nStartZ		= sc.GetInt(3);
	INT		nEndX		= sc.GetInt(4);
	INT		nEndY		= sc.GetInt(5);
	INT		nEndZ		= sc.GetInt(6);

	INT nLastOkX = 0, nLastOkY = 0;  // 用于返回障碍前的一个可用位置(函数的引用)
	CharacterSkill* pCharacterSkill = m_rcCharacter.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	if (pCharacterSkill->CheckObstacle(nStartX, nStartY, nStartZ, nEndX, nEndY, nEndZ, m_rcCharacter.m_nTouchRange, nLastOkX, nLastOkY))
	{
		sc.PushNumber(1);  // success 无障碍
		sc.PushNumber(0);
		sc.PushNumber(0);
		return 3;
	}
	else
	{
		sc.PushNumber(0);  // 有障碍，返回障碍前的可用点
		sc.PushNumber(nLastOkX);
		sc.PushNumber(nLastOkY);
		return 3;
	}
EXIT0:
	sc.PushNumber(1);
	sc.PushNumber(0);
	sc.PushNumber(0);
	return 3;
}

#include "luacharacterskill_table.hpp"

