/* -------------------------------------------------------------------------
//	文件名		：	fightskill.h
//	创建者		：	zhangzhixiong
//	实现者		：	
//	创建时间	：	2011.5.22
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include <math.h>
#include "fightskill/fightskill.h"
#include "fightskill/fightskillsetting.h"
#include "buffer/buffermanager.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/synchelper.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegamemodule/characterskill.h"
#include <algorithm>
#include "onlinegamemodule/ai/kaivm.h"
#include "faction/factionmanager.h"
#include "../onlinegamemodule/bladestate/bladedef.h"


struct CharacterDistanceGreater
{
	INT nX;
	INT nY;
	INT nZ;

	bool operator ()(KCharacter* elem1, KCharacter* elem2) CONST
	{
		INT nX2, nY2, nZ2;
		elem1->GetPosition(nX2, nY2, nZ2);
		INT nDistance1 = g_GetDistance2(nX, nY, nX2, nY2);
		elem2->GetPosition(nX2, nY2, nZ2);
		INT nDistance2 = g_GetDistance2(nX, nY, nX2, nY2);
		return nDistance1 < nDistance2;
	}
};
//////////////////////////////////////////////////////////////////////////

FightSkill::FightSkill()
{

}

FightSkill::~FightSkill()
{
	m_pFightSkillTemplate = NULL;
	m_nCurrentChannelTime = 0;
	m_pCaster = NULL;
	m_pTarget = NULL;
}

BOOL FightSkill::Init(KCharacter* pCaster,WORD nTemplateId, INT nSkillLevel)
{
	FLOAT fTimeBeforeCast = 0;
	LOG_PROCESS_ERROR(pCaster);
	m_pFightSkillTemplate = g_cFightSkillSetting.GetFightSkillTemplate(nTemplateId);
	LOG_PROCESS_ERROR(m_pFightSkillTemplate);
	m_pCaster = pCaster;
	m_pTarget = NULL;
	m_bIsActive = FALSE;
	m_nSkillLevel = nSkillLevel;
	m_arrCurrentIncroBuffer[0] = m_pFightSkillTemplate->nIncorBuff1Id;
	m_arrCurrentIncroBuffer[1] = m_pFightSkillTemplate->nIncorBuff2Id;
	m_arrCurrentIncroBuffer[2] = m_pFightSkillTemplate->nIncorBuff3Id;
	m_nIncroBuffer1Id = 0;
	m_nIncroBuffer2Id = 0;
	m_nIncroBuffer3Id = 0;
	m_nIncroBuffer4 = 0;
	m_nDestX = 0;
	m_nDestY = 0;
	m_nDestZ = 0;
	m_bSpecialTimeScale = FALSE;
	
	m_nCurrentChainLightningTargetIndex = 0;  // 闪电链
	m_nChainFixedBuffAddonDelay = 2;
	m_bChainSelfBufferAdded = FALSE;

	if(IS_PLAYER(pCaster->GetId()) && pCaster->GetAngerLeftTime() > 0)
	{
		fTimeBeforeCast = m_pFightSkillTemplate->nSpecialTimeBeforeCast;
	}
	else
	{
		fTimeBeforeCast = m_pFightSkillTemplate->nTimeBeforeCast;
	}
	m_nCurrentCastDelay = (INT)(fTimeBeforeCast * GAME_FPS * (m_bSpecialTimeScale ? m_pFightSkillTemplate->fSpecialTimeScale : 1));  // 秒转帧
	m_nCurrentChannelTime = (INT)(m_pFightSkillTemplate->nChannelTime * (m_bSpecialTimeScale ? m_pFightSkillTemplate->fSpecialTimeScale : 1));
	m_nCostPercent = m_pFightSkillTemplate->nCostPercent;
	m_nFightSkillChannel = m_pFightSkillTemplate->nFightSkillChannel;
	m_nReduceCost = 0;
	m_fReduceCostPercent = 0;

	m_bIsMissileTimeInit = FALSE; // 子弹运行时间是否已计算

	return TRUE;
EXIT0:
	return FALSE;
}

VOID FightSkill::StartSkill() // 开始技能
{
	m_bIsActive = TRUE;
	FLOAT fTimeBeforeCast = 0;

	if (m_nFightSkillChannel != FIGHT_SKILL_CHANNEL_NO)
	{
		// 引导技能, 
		m_eSkillState = SKILL_CHANNEL_GOING;
		m_pCaster->SetMoveState(cmsOnChannelSkill);  // 设置成引导技能状态
	}
	else
	{
		m_eSkillState = SKILL_CASTING;
		// 角色发技能状态
		m_pCaster->SetMoveState(cmsOnSkill);   // End Skill 里有设置成stand
	}

	if(IS_PLAYER(m_pCaster->GetId()) && m_pCaster->GetAngerLeftTime() > 0)
	{
		fTimeBeforeCast = m_pFightSkillTemplate->nSpecialTimeBeforeCast;
	}
	else
	{
		fTimeBeforeCast = m_pFightSkillTemplate->nTimeBeforeCast;
	}

	if (m_pFightSkillTemplate->bNeedJumpTo) // 处理跳砸，客户端也会自己处理，不需同步
	{
		m_pCaster->SetPosition(m_nDestX, m_nDestY, 1048576);
	}

	// 技能引导时间
	m_nCurrentChannelTime = (INT)(m_pFightSkillTemplate->nChannelTime * (m_bSpecialTimeScale ? m_pFightSkillTemplate->fSpecialTimeScale : 1));
	// 发招延迟
	m_nCurrentCastDelay = (INT)(fTimeBeforeCast * GAME_FPS * (m_bSpecialTimeScale ? m_pFightSkillTemplate->fSpecialTimeScale : 1)); // 秒转帧
}

VOID FightSkill::EndSkill() // 结束技能时的通用处理
{
	FLOAT fTimeBeforeCast = 0;
	m_eSkillState = SKILL_STOP;
	if (m_pCaster->GetMoveState() == cmsOnSkill || m_pCaster->GetMoveState() == cmsOnChannelSkill) // 如果已经死亡状态了，就不改变的它的死亡状态了
	{
		m_pCaster->SetMoveState(cmsOnStand);  // 设置状态为站立完毕
	}

	if(IS_PLAYER(m_pCaster->GetId()) && m_pCaster->GetAngerLeftTime() > 0)
	{
		fTimeBeforeCast = m_pFightSkillTemplate->nSpecialTimeBeforeCast;
	}
	else
	{
		fTimeBeforeCast = m_pFightSkillTemplate->nTimeBeforeCast;
	}

	m_bIsActive = FALSE;
	m_pTarget = NULL;
	m_nCurrentCastDelay = (INT)(fTimeBeforeCast * GAME_FPS * (m_bSpecialTimeScale ? m_pFightSkillTemplate->fSpecialTimeScale : 1)); // 秒转帧
	m_nCurrentChannelTime = m_pFightSkillTemplate->nChannelTime * (m_bSpecialTimeScale ? m_pFightSkillTemplate->fSpecialTimeScale : 1);
	m_bIsMissileTimeInit = FALSE;  // 令下次导弹运行时重新计算

	// m_pCaster加相应的怒气值（玩家不在大神状态才加）
	if(IS_PLAYER(m_pCaster->GetId()) && m_pCaster->GetAngerLeftTime() <= 0 && m_pCaster->GetLevel() >= BLADE_OPEN_LEVEL)
	{
		INT nCurrentAnger = m_pCaster->GetAnger();
		InactiveSkillNumber* pSkillNumber = g_cFightSkillSetting.GetInactiveSkillNumber(ChangeGodSkillId(m_pFightSkillTemplate->nSkillID), m_nSkillLevel);
		LOG_PROCESS_ERROR(pSkillNumber);

		nCurrentAnger += pSkillNumber->nAddAnger;
		if(nCurrentAnger > 100)
		{
			nCurrentAnger = 100;
		}

		m_pCaster->SetAnger(nCurrentAnger);
	}

EXIT0:
	return;
}

BOOL FightSkill::BindTarget(KCharacter* pTarget)
{
	LOG_PROCESS_ERROR(pTarget);
	m_pTarget = pTarget;
	
	// 导弹技或者闪电链记录下起始位置以及速度
	if (m_pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_MISSILE || this->IsChainLightningSkill()) 
	{
		pTarget->GetPosition(m_nMissileX, m_nMissileY, m_nMissileHeight);
		m_nMissileHeight = m_nMissileHeight + m_pFightSkillTemplate->nMissileHeight;

		//m_nMissileHorizontalSpeed = m_pFightSkillTemplate->nMissileHorizontalSpeed;//RepresentSpeedToLogicSpeed(m_pFightSkillTemplate->nMissileHorizontalSpeed * _3D_CELL_CM_LENGTH);
		//m_nMissileVerticalSpeed = m_pFightSkillTemplate->nMissileVerticalSpeed;	
	}

	if (m_pFightSkillTemplate->nFightSkillStartPos == FIGHT_SKILL_STARTPOS_SELF)	//水平
	{
		INT castX, castY, castZ;
		m_pCaster->GetPosition(castX, castY, castZ);
		m_nMissileX = castX;
		m_nMissileY = castY;
	}
	else if (m_pFightSkillTemplate->nFightSkillStartPos == FIGHT_SKILL_STARTPOS_TARGET)	//垂直
	{
		INT targetX, targetY, targetZ;
		m_pTarget->GetPosition(targetX, targetY, targetZ);
		m_nMissileX = targetX;
		m_nMissileY = targetY;
	}
	return TRUE;
EXIT0:
	return FALSE;
}

VOID FightSkill::GenerateChainLightningTargets(KCharacter* pCenterTarget, FLOAT nRadius)
{
	BOOL bOk = TRUE;
	INT nX = 0, nY = 0, nZ = 0;
	INT nX1 = 0, nY1 = 0, nZ1 = 0;
	std::vector<KCharacter*> arrCharacters;
	TKTraverseHelper<KCharacter> cTravHelper;	//遍历角色需要的遍历器
	INT nMaxDistance = 0;
	KCharacter* pCharMaxDistance = NULL;
	std::vector<KCharacter*>::iterator it_find;

    IKScene* pScene = pCenterTarget->GetScene();
	LOG_PROCESS_ERROR(pScene);

    bOk = pScene->TraverseNearbyPlayers(cTravHelper, pCenterTarget);
	LOG_PROCESS_ERROR(bOk);
	bOk = pScene->TraverseNearbyNpcs(cTravHelper, pCenterTarget);
	LOG_PROCESS_ERROR(bOk);

	pCenterTarget->GetPosition(nX1, nY1, nZ1);

	if (m_pFightSkillTemplate->bDmgOrRvv == 2)  // 治疗链，先给自己加上一次治疗buffer
	{
		if (m_bChainSelfBufferAdded == FALSE)
		{
			m_pTarget = m_pCaster;
			ApplyBuffer();
			m_bChainSelfBufferAdded = TRUE;
		}
	}

	for (std::vector<KCharacter*>::iterator it = cTravHelper.m_vObjList.begin(); it != cTravHelper.m_vObjList.end(); ++it)
	{
		if ((*it) == m_pCaster) // 不用考虑发射者
			continue;
		
		it_find = std::find(m_arrChainLightningTargets.begin(), m_arrChainLightningTargets.end(), (*it));  // 判断是否已在目标列表
		if (it_find != m_arrChainLightningTargets.end()) // 已经加入目标列表的角色跳过
			continue;

		// 进行技能关系判断
		if (!CheckRelation(m_pCaster, (*it), FALSE))  // 关系不合适，跳过, 不输出提醒信息
			continue;

        (*it)->GetPosition(nX, nY, nZ);
		INT nDistance = g_GetDistance2(nX, nY, nX1, nY1);
		if (nDistance < (INT)((nRadius * CELL_LENGTH * 2) * (nRadius * CELL_LENGTH * 2)))
		{
			if (nDistance > nMaxDistance)
			{
				pCharMaxDistance = (*it);
				nMaxDistance = nDistance;
			}

			arrCharacters.push_back((*it));
		}
	}

	CharacterDistanceGreater cGreater;
	cGreater.nX = nX;
	cGreater.nY = nY;
	cGreater.nZ = nZ;
	std::sort(arrCharacters.begin(), arrCharacters.end(), cGreater);
	for (std::vector<KCharacter*>::iterator it = arrCharacters.begin(); it != arrCharacters.end(); ++it)
	{
        m_arrChainLightningTargets.push_back((*it));

		if ((INT)m_arrChainLightningTargets.size() >= m_pFightSkillTemplate->nChainLightningTargetNum)
		{
			goto EXIT0;
		}
	}

	if (pCharMaxDistance != NULL)
	{
		GenerateChainLightningTargets(pCharMaxDistance, nRadius);
	}

EXIT0:;
}

// 检查角色对象是否为空, 同客户端
BOOL FightSkill::CheckSkillCharacters()
{
	if (!m_pCaster)
	{
		//QLogPrintf(LOG_INFO, "[FightSkill:CheckSkillCharacters]技能释放过程中，Caster发射者不见了");
		return FALSE;
	}
	
	if (m_pFightSkillTemplate->bNeedTarget == TRUE)
	{
		// 闪电链技能不作目标角色丢失判定
		if (m_pFightSkillTemplate->nFightSkillType != FIGHT_SKILL_OTHER)
		{
			if (!this->IsChainLightningSkill())
			{
				// 目标判定
				if (!m_pTarget)
				{
					//QLogPrintf(LOG_INFO, "[FightSkill:CheckSkillCharacters]技能释放过程中，Target目标不见了");
					return FALSE;
				}
			}
		}
	}

	// 发射者死亡，技能结束
	if (m_pCaster->GetMoveState() == cmsOnDeath)
	{
		//QLogPrintf(LOG_INFO, "[FightSkill:CheckSkillCharacters]技能释放过程中，Caster死亡");
		return FALSE;
	}

	return TRUE;
}

BOOL FightSkill::OnChannelSkillState()  // 引导技能过程中
{
	if (m_nFightSkillChannel == FIGHT_SKILL_CHANNEL_REVERSE) // 反向引导技能
	{ 
		ProcessResult();  // 反向引导中不断添加效果
	}

	if (m_nCurrentChannelTime > 0)
	{
		m_nCurrentChannelTime--;
	}
	else
	{
		// 引导结束，正向引导进入下一阶段，反向引导技能结束
		if (m_nFightSkillChannel == FIGHT_SKILL_CHANNEL_FORWARD)
		{
			m_eSkillState = SKILL_CASTING;
		}
		else if (m_nFightSkillChannel == FIGHT_SKILL_CHANNEL_REVERSE)
		{
			this->EndSkill();
		}
	}

	return TRUE;
}

BOOL FightSkill::OnSkillState()  // 正常技能
{
	if (m_nCurrentCastDelay > 0) // 发招延迟
	{
		m_nCurrentCastDelay--;
		if (m_nCurrentCastDelay <= 0)
		{
			// 计算闪电链技能的目标列表
			if (this->IsChainLightningSkill())
			{
				m_arrChainLightningTargets.clear();
				m_arrChainLightningTargets.push_back(m_pTarget);
				m_nCurrentChainLightningTargetIndex = 0;
				GenerateChainLightningTargets(m_pTarget, m_pFightSkillTemplate->fChainLightningRange);
			}
		}

		goto EXIT0;
	}

	// 非导弹技能马上应用效果
	//if (m_pFightSkillTemplate->nFightSkillType != FIGHT_SKILL_MISSILE && m_pFightSkillTemplate->nFightSkillType != FIGHT_SKILL_CHAIN_LIGHTNING) 
	if (m_pFightSkillTemplate->nFightSkillType != FIGHT_SKILL_MISSILE) 
	{
		ProcessResult();
		goto EXIT0;
	}
	else
	{
		// 导弹技能计算时间 TODO:

		if (m_pFightSkillTemplate->bNeedTarget)
		{
			// 只根据时间
			if (TrackHorizontalOperation())	//如果到达了，就应用属性
			{
				ProcessResult();
			}
		}
		else
		{
			//if (TrackVerticalOperation())
			//{
			//Q_Printl("垂直导弹技能未实现！");
			//}
		}
	}

EXIT0:
	return TRUE;
}

VOID FightSkill::CancelChannelSkill()
{
	if (m_nFightSkillChannel != FIGHT_SKILL_CHANNEL_NO)  // 有引导技能
	{
		if (m_eSkillState == SKILL_CHANNEL_GOING)
		{
			// 取消引导, 打断引导
			this->EndSkill();
		}
		else 
		{
			//QLogPrintf(LOG_ERR, "[CancelChannelSkill]引导型技能,非引导状态执行取消引导操作！");
		}
	}
	else 
	{
		//QLogPrintf(LOG_ERR, "[CancelChannelSkill]非引导型技能执行取消引导操作！");
	}
}

VOID FightSkill::Breath()
{
	if (!m_bIsActive)
	{
		goto EXIT0;
	}

	// 检查技能角色是否出问题
	if (!CheckSkillCharacters())
	{
		//QLogPrintf(LOG_INFO, "技能发射中角色出现异常，直接结束技能");
		this->EndSkill();
		goto EXIT0;
	}

	if (m_eSkillState == SKILL_CASTING)
	{
		this->OnSkillState();
	}
	else if (m_eSkillState == SKILL_CHANNEL_GOING)
	{
		this->OnChannelSkillState();
	}
	else
	{
		//QLogPrintf(LOG_INFO, "非正常Fightskill技能运行状态");
	}

EXIT0:;
}

BOOL FightSkill::TrackHorizontalOperation()
{
	INT targetX, targetY, targetZ;
	m_pTarget->GetPosition(targetX, targetY, targetZ);
	INT casterX, casterY, casterZ;
	m_pCaster->GetPosition(casterX, casterY, casterZ);

	DWORD nowTimeFrame = g_cOrpgWorld.m_dwLogicFrames;

	if (!m_bIsMissileTimeInit) // 第一次导弹运行，计算结束时间
	{
		INT startDistance2 = g_GetDistance2(casterX, casterY, targetX, targetY);
		//INT attackRadius2 = (INT)(m_pFightSkillTemplate->fAttackRadius * 64 * m_pFightSkillTemplate->fAttackRadius * 64); // 表现端长度转成逻辑端，平方
		//// 开方
		//FLOAT timeRatio = sqrtf((FLOAT)startDistance2 / (FLOAT)attackRadius2); // 时间比率的开方		
		//DWORD dwMissileTime = (DWORD)((m_pFightSkillTemplate->fMissileSpeed * GAME_FPS) * timeRatio); // 导弹飞行时间计算,  运行时间 = 填表时间(秒, 转帧) * (当前距离 / 填表距离)
		//m_dwMissileEndTime = nowTimeFrame + dwMissileTime;
		DWORD dwSpeed = m_pFightSkillTemplate->fMissileSpeed * CELL_LENGTH / (DWORD)GAME_FPS; // 转逻辑速度,,  还要除以16啊!计算的是每逻辑帧速度
		DWORD dwMissileTime = sqrtf(startDistance2) / dwSpeed; // t= S / v

		m_dwMissileTimeRemaining = dwMissileTime;

		m_bIsMissileTimeInit = TRUE;
	}
	
	if (m_dwMissileTimeRemaining <= 0)
	{
		return TRUE;  // Over
	}
	
	m_dwMissileTimeRemaining--;

	return FALSE;
}

BOOL FightSkill::CheckRelation(KCharacter* pCaster, KCharacter* pTarget, BOOL outputWarning)
{
	int nRelation = g_GetRelation(pCaster, pTarget);
	if (
			((nRelation == sortSelf) &&   
			!m_pFightSkillTemplate->abRelation[emRELATION_SELF-1]) // 关系是自己，但技能没设置自己允许
			||
			((nRelation == sortAlly) && 
			!m_pFightSkillTemplate->abRelation[emRELATION_FRIEND-1])   // 友方
			||
			((nRelation == sortTeammate) && 
			!m_pFightSkillTemplate->abRelation[emRELATION_TEAMMATE-1])  // 队友
			||
			((nRelation == sortEnemy) && 
			!m_pFightSkillTemplate->abRelation[emRELATION_ENEMY-1]) // 敌人  
		)
	{
		if (outputWarning)
		{
			//QLogPrintf(LOG_WARNING, "[FightSkill::CheckRelation]技能释放的人物关系，不适合. 双方关系:%d", nRelation);
		}
		return FALSE;
	}
	return TRUE;
}

INT FightSkill::ChangeGodSkillId(int nGodSkillId)
{
	int mapSkillId = nGodSkillId;
    int nAngerLeftTime = m_pCaster->GetAngerLeftTime();
	int nFactionRoute = m_pCaster->GetCurrentRoute();
	if (nAngerLeftTime > 0)
	{		
		switch (nGodSkillId)
		{
		case emGOD_SINGLE_SKILL_ID:
			if (nFactionRoute == 1)
			{
                mapSkillId = emNEAR_SINGLE_SKILL_ID;
			}
			else
			{
                mapSkillId = emFAR_SINGLE_SKILL_ID;
			}
			break;
		case emGOD_GROUP_SKILL_ID:
			if (nFactionRoute == 1)
			{
                mapSkillId = emNEAR_GROUP_SKILL_ID;
			}
			else
			{
                mapSkillId = emFAR_GROUP_SKILL_ID;
			}
			break;
		case emGOD_DEATH_SKILL_ID:
			if (nFactionRoute == 1)
			{
                mapSkillId = emNEAR_DEATH_SKILL_ID;
			}
			else
			{
                mapSkillId = emFAR_DEATH_SKILL_ID;
			}
			break;
		}
	}

	return mapSkillId;
}

// 处理技能消耗
BOOL FightSkill::CheckCastConsume()
{
	INT nRealSkillId = ChangeGodSkillId(m_pFightSkillTemplate->nSkillID);
	if (m_pFightSkillTemplate->nSkillCostType == FIGHT_SKILL_COST_TYPE_MANA) // 真气
	{
		INT nCurrentMana = m_pCaster->GetCurrentMana();
		InactiveSkillNumber* pSkillNumber = g_cFightSkillSetting.GetInactiveSkillNumber(nRealSkillId, m_nSkillLevel);
		INT nCost = 0, nNewMana = 0;
		LOG_PROCESS_ERROR(pSkillNumber);
		
		nCost = pSkillNumber->nManaCost;
		nNewMana = nCurrentMana - nCost;
		nNewMana = nNewMana < 0 ? 0 : nNewMana;
		m_pCaster->SetCurrentMana(nNewMana);
	}
	else if (m_pFightSkillTemplate->nSkillCostType == FIGHT_SKILL_COST_TYPE_LIFE) // 生命
	{
		INT nCurrentLife = m_pCaster->GetCurrentLife();
		INT nBaseMaxLife = g_cFactionManager.GetBaseMaxLife(*m_pCaster);
		INT nCost = nBaseMaxLife * m_nCostPercent / 100;

		if(m_fReduceCostPercent != 0)
		{
			nCost = static_cast<INT>((FLOAT)nCost * (100.0f - m_fReduceCostPercent) / 100.0f);
		}

		nCost -= m_nReduceCost;
		ASSERT(nCost >= 0);

		if(nCurrentLife < nCost)
			return FALSE;
		INT nNewLife = nCurrentLife - nCost;
		m_pCaster->SetCurrentLife(nNewLife);
	}
	else
	{
		//QLogPrintf(LOG_ERR, "[Skill]Wrong skill cost type");
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL FightSkill::CheckCastRelation()
{
	if(m_pFightSkillTemplate->bNeedTarget && !CheckRelation(m_pCaster, m_pTarget))
		return FALSE;

	return TRUE;
}

BOOL FightSkill::ProcessResult()
{
    if (m_nFightSkillChannel == FIGHT_SKILL_CHANNEL_REVERSE)
	{
		if (m_nCurrentChannelTime % m_pFightSkillTemplate->nChannelFreq == 0) // 到达添加buffer时机, 不断定时添加
		{
			ApplyBuffer();
		}
	}
	else if (this->IsChainLightningSkill()) // 闪电链
	{
		if (m_nChainFixedBuffAddonDelay == 2) // 除第一个目标，其他目标固定2帧后添加buf
		{
			if (m_nCurrentChainLightningTargetIndex < (INT)m_arrChainLightningTargets.size())
			{
				m_pTarget = m_arrChainLightningTargets[m_nCurrentChainLightningTargetIndex++];
				ApplyBuffer();
			}
			else
			{
                 goto EXIT0;
			}

			m_nChainFixedBuffAddonDelay = 0;
		}
		m_nChainFixedBuffAddonDelay++;
	}
	else
	{
		ApplyBuffer();  // 一般技能
		goto EXIT0;
	}

	return TRUE;
EXIT0:
	this->EndSkill();
	return TRUE;
}

VOID FightSkill::AddBuffer(INT nBufId, INT& rnSelfBufIdVal)
{
	CONST BufferTemplate* pTemplate = NULL;

	if(!nBufId)
		goto EXIT0;

	pTemplate = g_cBufferSetting.GetBufferTemplate(nBufId);
	LOG_PROCESS_ERROR(pTemplate);

	PROCESS_ERROR(nBufId != 0);

	if (!pTemplate->m_bIsGroundBuffer)
	{
		DWORD dwTargetId = 0;
		if(m_pTarget)
		{
			dwTargetId = m_pTarget->GetId();
		}

		rnSelfBufIdVal = g_cBufferMgr.AddBufferToCharacter(m_pCaster->GetId(), m_pFightSkillTemplate->nSkillID, dwTargetId, nBufId, 0, TRUE, m_nDestX,m_nDestY,m_nDestZ, m_nSkillLevel);
		//LOG_PROCESS_ERROR(rnSelfBufIdVal);
		if (!(rnSelfBufIdVal))       
		{                       
			//QLogPrintf(LOG_DEBUG, "无法添加技能buffer(%d,%d) at line %d in %s\n", m_pFightSkillTemplate->nSkillID, nBufId, __LINE__, __FUNCTION__);                  
			goto EXIT0;    
		}

		if (g_cOrpgWorld.m_bDebugSelf)
		{
			//std::cout << "AddBuffer BuffId: " << nBufId << " Caster: " << m_pCaster->GetName() << " Target: " << 
			//	(m_pTarget ? m_pTarget->GetName() : m_pCaster->GetName()) << std::endl;
		}
	}
	else
	{
		if (m_pCaster && m_pCaster->GetScene())
		{
			DWORD dwSceneId = m_pCaster->GetScene()->GetId();
			rnSelfBufIdVal = g_cBufferMgr.AddGroundBuffer(nBufId, m_pCaster->GetId(), m_pFightSkillTemplate->nSkillID, dwSceneId,m_nDestX,m_nDestY,m_nDestZ, pTemplate->m_bSync, 0, m_nSkillLevel);
			LOG_PROCESS_ERROR(rnSelfBufIdVal);

			if (g_cOrpgWorld.m_bDebugSelf)
			{
			    //std::cout << "AddGroundBuffer BuffId: " << nBufId << " Caster: " << m_pCaster->GetName() << std::endl;
			}
		}
	}
EXIT0:;
}

BOOL FightSkill::ApplyBuffer()
{
	// 添加几率触发的Buffer
	if (m_pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_BUFFER || m_pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_MISSILE)
	{
	    if (m_pCaster && m_pTarget && IS_PLAYER(m_pCaster->GetId()))
		{
			ResistSkillNumber* pResistSkillNumber = NULL;
			InactiveSkillNumber* pInactiveSkillNumber = NULL;
			CharacterSkill* pCharSkill = NULL;
			INT nResistSkillLevel = 0;
			pInactiveSkillNumber = g_cFightSkillSetting.GetInactiveSkillNumber(ChangeGodSkillId(m_pFightSkillTemplate->nSkillID), m_nSkillLevel);
            LOG_PROCESS_ERROR(pInactiveSkillNumber);

			if (pInactiveSkillNumber->nResistSkillId != 0)
			{
                INT nResistIncroRate = 0;
				pCharSkill = m_pTarget->GetCtrl<CharacterSkill>();
				LOG_PROCESS_ERROR(pCharSkill);
				nResistSkillLevel = pCharSkill->GetSkillLevel(pInactiveSkillNumber->nResistSkillId);
				pResistSkillNumber = g_cFightSkillSetting.GetResistSkillNumber(m_pFightSkillTemplate->nSkillID, nResistSkillLevel);
				if (pResistSkillNumber != NULL)
				{
					nResistIncroRate = pResistSkillNumber->nValue;
				}

				INT nIncroRate = pInactiveSkillNumber->nBufferIncroRate - nResistIncroRate;
				nIncroRate = nIncroRate < 0 ? 0 : nIncroRate;

				//printf("此次主动技能实际buff触发概率: %d  主动技能触发概率：%d, 抗性技能减少触发概率：%d", nIncroRate, pInactiveSkillNumber->nBufferIncroRate, pResistSkillNumber->nValue);

				INT nRateValue = KSysService::Rand() % 1000;
				if (nRateValue < nIncroRate)
				{
					if (pInactiveSkillNumber->nAddBuffToSelf) // 是否给自己上buff，如加血
					{
						g_cBufferMgr.AddBufferToCharacter(m_pCaster->GetId(), m_pFightSkillTemplate->nSkillID , m_pCaster->GetId(), pInactiveSkillNumber->nBufferId, pInactiveSkillNumber->nBufferExistTime, TRUE, m_nDestX,m_nDestY,m_nDestZ, m_nSkillLevel);
					}
					else
					{
						g_cBufferMgr.AddBufferToCharacter(m_pCaster->GetId(), m_pFightSkillTemplate->nSkillID , m_pTarget->GetId(), pInactiveSkillNumber->nBufferId, pInactiveSkillNumber->nBufferExistTime, TRUE, m_nDestX,m_nDestY,m_nDestZ, m_nSkillLevel);
					}
				}
			}
		}
	}

	AddBuffer(m_arrCurrentIncroBuffer[0], m_nIncroBuffer1Id);  // 里面判断是加给人还是加给地
	AddBuffer(m_arrCurrentIncroBuffer[1], m_nIncroBuffer2Id);
	AddBuffer(m_arrCurrentIncroBuffer[2], m_nIncroBuffer3Id);
	AddBuffer(m_nIncroBuffer4, m_nIncroBuffer4Id);

	return TRUE;
EXIT0:
	return FALSE;
}


VOID FightSkill::SetDestPostion(INT nX,INT nY,INT nZ)
{
	m_nDestX = nX;
	m_nDestY = nY;
	m_nDestZ = nZ;
	if (m_pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_MISSILE) // 导弹技记录下起始位置以及速度
	{
		m_nMissileHeight = m_nDestZ + m_pFightSkillTemplate->nMissileHeight;
		//m_nMissileHorizontalSpeed = m_pFightSkillTemplate->nMissileHorizontalSpeed;//RepresentSpeedToLogicSpeed(m_pFightSkillTemplate->nMissileHorizontalSpeed * _3D_CELL_CM_LENGTH);
		//m_nMissileVerticalSpeed = m_pFightSkillTemplate->nMissileVerticalSpeed;
	}
	if (m_pFightSkillTemplate->nFightSkillStartPos == FIGHT_SKILL_STARTPOS_SELF)	//水平
	{
		INT castX, castY, castZ;
		m_pCaster->GetPosition(castX, castY, castZ);
		m_nMissileX = castX;
		m_nMissileY = castY;
	}
	else if (m_pFightSkillTemplate->nFightSkillStartPos == FIGHT_SKILL_STARTPOS_TARGET)	//垂直
	{
		m_nMissileX = m_nDestX;
		m_nMissileY = m_nDestY;
	}
}

VOID FightSkill::SetCurrentIncroBuffer(INT nIndex, INT nBuffId)
{
	if (nIndex < 4)
	{
	    m_arrCurrentIncroBuffer[nIndex - 1] = nBuffId;
	}
	else if (nIndex == 4)
	{
		m_nIncroBuffer4 = nBuffId;
	}
	else
	{
		//QLogPrintf(LOG_ERR, "[FightSkill]Wrong incro buffer index[%d]", nIndex);
	}
}

INT FightSkill::GetCurrentIncroBuffer(INT nIndex)
{
	if (nIndex < 4)
	{
	    return m_arrCurrentIncroBuffer[nIndex - 1];
	}
	else if (nIndex == 4)
	{
		return m_nIncroBuffer4;
	}
	else
	{
		//QLogPrintf(LOG_ERR, "[FightSkill]Wrong incro buffer index[%d]", nIndex);
		return -1;//参数错误
	}
}

BOOL FightSkill::IsChainLightningSkill()  // 判断是否闪电链技能
{
	if (this->m_pFightSkillTemplate->nFightSkillType == FIGHT_SKILL_OTHER)
	{
		//QString skillPlugins = this->m_pFightSkillTemplate->szPlugins;
		string skillPlugins(this->m_pFightSkillTemplate->szPlugins);
		if (skillPlugins.find("QRLSkillChainLightning") != string::npos)
		//if (skillPlugins.Find("QRLSkillChainLightning") )
		{
			return TRUE;
		}
	}

	return FALSE;
}