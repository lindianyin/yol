/* -------------------------------------------------------------------------
//	文件名		：	Buffer.cpp
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-7 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "buffer/buffer.h"
#include "buffer/buftriggereffect.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kluacharacter.h"
#include "buffer/bufsetting.h"
#include "onlinegameworld/kplayer.h"
#include "../fightskill/fightskillsetting.h"
#include "onlinegamemodule/ai/kaivm.h"
#include "../onlinegamemodule/ai/ThreatList.h"
#include "onlinegamemodule/ai/kaictrl.h"
#include "onlinegameworld/korpgworld.h"
#include "../onlinegameworld/scene/kscene.h"
#include "../onlinegameworld/scene/kscenemgr.h"
#include "buffermanager.h"

Buffer::Buffer() : m_dwId(KD_BAD_ID)
{
	Reset();
}

Buffer::~Buffer()
{
	Reset();
}

INT Buffer::GetBuffCategory(INT index)
{
	switch(index)
	{
	case 1:
		return m_nStackCategory;
	case 2:
		return m_nStackCategory1;
	case 3:
		return m_nStackCategory2;
	}
	return 0;
}

BOOL Buffer::Init(BufferTemplate* buffTemplate, INT nPersist /* = 0 */)
{
	LOG_PROCESS_ERROR(buffTemplate);
	m_nBuffTemplateID = buffTemplate->m_nBuffTemplateID;
	if(nPersist > 0)
	{
		m_nBuffPersist = nPersist;
	}
	else
	{
		m_nBuffPersist = buffTemplate->m_nBuffPersist;
	}
	m_nCurrentPersist = m_nBuffPersist;
	m_eBuffType = buffTemplate->m_eBuffType;
	m_nBuffLevel = buffTemplate->m_nBuffLevel;
	m_BuffPosition = buffTemplate->m_BuffPosition;
	m_nBuffType = buffTemplate->m_nBuffType;
	m_nK = buffTemplate->m_nK;
	m_nIfUseSkillLv = buffTemplate->m_nIfUseSkillLv;
	strncpy(m_szEffectMapType, buffTemplate->m_szEffectMapType, MAX_NAME_LEN);
	m_nStackCategory = buffTemplate->m_nStackCategory;
	m_nStackCategory1 = buffTemplate->m_nStackCategory1;
	m_nStackCategory2 = buffTemplate->m_nStackCategory2;
	m_nMagnification	= buffTemplate->m_nMagnification;			//系数
	m_nGrow				= buffTemplate->m_nGrow;					//成长
	m_nStackCount = buffTemplate->m_nStackCount;
	m_bSync = buffTemplate->m_bSync;
	m_bSave = buffTemplate->m_bSave;
	m_bDeath = buffTemplate->m_bDeath;
	m_bPlus = buffTemplate->m_bPlus;
	m_nDestX = 0;
	m_nDestY = 0;
	m_nDestZ = 0;
	m_bAffectOnce = FALSE;
	m_bStackAffectOnce = FALSE;
	m_bNeedAgree = buffTemplate->m_bNeedAgree;
	m_fBuffRadius = buffTemplate->m_fBuffRadius;
	m_nBuffEffectNum = buffTemplate->m_nBuffEffectNum;
	m_nBuffDelayFrame = buffTemplate->m_nBuffDelayFrame;
	m_nCurrentStackCount = 1;	//当前叠加层数默认为1
	m_nSkillTemplateId = 0;
	for(std::vector<INT>::iterator it_timer = buffTemplate->m_vecBufTimerEffectID.begin(); it_timer != buffTemplate->m_vecBufTimerEffectID.end(); ++it_timer)
	{
		BufTimerEffect* effect = g_cBufferSetting.GetTimerEffect(*it_timer);
		if(effect)
		{
			m_vecBufTimerEffect.push_back(effect);
			m_vecBufTimerInterval.push_back(0);
		}
	}
	for(std::vector<INT>::iterator it_trigger = buffTemplate->m_vecBufTriggerEffectID.begin(); it_trigger != buffTemplate->m_vecBufTriggerEffectID.end(); ++it_trigger)
	{
		TriggerData sEffect = g_cBufferSetting.GetTriggerEffect(*it_trigger);
		if(sEffect.psEffect)
		{
			m_vecBufTriggerEffect.push_back(sEffect);
		}
	}

	// 当前Buff的脚本加成数据
	m_vecAppendData.resize(0);

	return TRUE;
EXIT0:
	return FALSE;	
}


BOOL Buffer::Reset()
{
	m_pCharacter = NULL;
	m_pSender = NULL;
	m_dwSenderId = 0;
	m_nBuffPersist = 0;
	m_nCurrentPersist = 0;
	m_vecBufTimerEffect.clear();
	m_vecBufTimerInterval.clear();
	m_vecBufTriggerEffect.clear();
	return TRUE;
}

BOOL Buffer::BindCharacter(KCharacter* pCharacter)
{
	LOG_PROCESS_ERROR(pCharacter);
	m_pCharacter = pCharacter;
	return TRUE;
EXIT0:
	return FALSE;
};

BOOL Buffer::BindSender(KCharacter* pCharacter)
{
	LOG_PROCESS_ERROR(pCharacter);
	m_pSender = pCharacter;
	m_dwSenderId = pCharacter->GetId();

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL Buffer::BindFightSkill(INT nFightSkillTemplateId)
{
	m_nSkillTemplateId = nFightSkillTemplateId;
	return TRUE;
}

BOOL Buffer::SetSkillLevel(INT nSkillLevel)  // 设置buffer等级
{
	m_nSkillLevel = nSkillLevel;
	return TRUE;
}

BOOL Buffer::CheckSceneBuffEffect()
{
	if (!m_pCharacter || !m_pCharacter->GetScene())
	{
		return FALSE;
	}

	IKScene* pScene = m_pCharacter->GetScene();
	const KSCENE_CONFIG* pSceneSetting = g_SceneMgr.GetSceneInfo(pScene->GetTemplateId());
	if(strlen(m_szEffectMapType) && strcmp(pSceneSetting->szSceneType, m_szEffectMapType) != 0)
	{
		return FALSE;
	}

	return TRUE;
}

//检测自身的一些属性
BOOL Buffer::CheckSelfCondition(BOOL& bSync)
{
	bSync = FALSE;

	if (!m_pCharacter || !m_pCharacter->GetScene())
	{
		return FALSE;
	}

	// npc死亡后buff效果还在
	if(IS_PLAYER(m_dwSenderId))
	{
		if (!m_pSender || !m_pSender->GetScene())
		{
			return FALSE;
		}

		if(m_pSender->GetSceneId() != m_pCharacter->GetSceneId())
		{
			return FALSE;
		}
	}
	
	if (m_nBuffPersist >= 0 && m_nCurrentPersist <= 0)
	{
		return FALSE;
	}

	//如果是死亡保留，并且玩家死亡，则不进行breath
	if (!m_bDeath && m_pCharacter->GetMoveState() == cmsOnDeath)
	{
		bSync = TRUE;

		return FALSE;
	}

	return TRUE;
}

BOOL Buffer::CheckRelation(KCharacter* pCaster, KCharacter* pTarget)
{
	BOOL bIsTeamMate = FALSE;
	if(pCaster != pTarget && IS_PLAYER(pCaster->GetId()) && IS_PLAYER(pTarget->GetId()))
	{
		INT nTeamId1 = ((KPlayer*)pCaster)->GetTeamId();
		INT nTeamId2 = ((KPlayer*)pTarget)->GetTeamId();
		if (nTeamId1 != 0 && nTeamId1 == nTeamId2)
			bIsTeamMate = TRUE;
	}
	INT nRelation = g_GetRelation(pCaster, pTarget);
	BufferTemplate* pTemplate = g_cBufferSetting.GetBufferTemplate(m_nBuffTemplateID);
	QCONFIRM_RET_FALSE(pTemplate);
	if (
		(!pTarget->IsIgnoreDebuff(pTemplate->m_nBuffType))													&&	// 判断角色是否免疫固定类型的Buff
		(((nRelation == sortSelf) && pTemplate->m_abRelation[emRELATION_SELF-1])																	||	// 对自己有效(顺序不可变)
		((pTarget != pCaster) && bIsTeamMate && pTemplate->m_abRelation[emRELATION_TEAMMATE-1])	||	// 对队友有效
		(nRelation == sortAlly && pTemplate->m_abRelation[emRELATION_FRIEND-1])	||	// 对友方有效
		(nRelation == sortEnemy && pTemplate->m_abRelation[emRELATION_ENEMY-1]))								// 对敌人有效
		)
	{
		return TRUE;
	}
	QLogPrintf(LOG_ERR, "[CheckRelation]Buffer Relation error: %d", m_nBuffTemplateID);
	return FALSE;
}

BOOL Buffer::ModifyThreat(KCharacter* pCaster, KCharacter* pTarget)
{
	BOOL bRet = FALSE;
	if (IS_DEBUFF(m_nBuffType))
	{
		if (pTarget != pCaster)
		{
			if (!pTarget->IsSkillHidden(pCaster)) // 如对发射者隐身，则不触发受伤害事件
			{
				pTarget->m_SimpThreatList.UpdateKeepThreatField();

				pTarget->m_AIData.ChoiceTarget[aittAttackTarget].SetTarget(pCaster);

				pTarget->m_AIVM.FireEvent(aevOnAttacked, pCaster->GetId(), 0);

				bRet = pTarget->m_SimpThreatList.ModifyThreat(thtSpecialThreat, pCaster, 0);
				LOG_CHECK_ERROR(bRet);
			}
		}
	}
	else
	{
		bRet = pTarget->m_SimpThreatList.ModifyThreaten(thtSpecialThreat, pCaster, 0);
		LOG_CHECK_ERROR(bRet);
	}

	return TRUE;
}

BOOL Buffer::ProcessTimer()
{
	QLunaBase* pOldMe = NULL;
	QLunaBase* pOldHim = NULL;
	if(m_vecBufTimerEffect.size() <= 0 || !m_pCharacter)
	{
		return FALSE;
	}
	INT  nBeginIndex = 0;

	if(IS_PLAYER(m_dwSenderId) && m_pSender->GetMoveState() == cmsOnDeath)
		return FALSE;

	if(IS_PLAYER(m_dwSenderId) && !CheckRelation(m_pSender, m_pCharacter))
		return FALSE;

	ModifyThreat(m_pSender, m_pCharacter);

	// 处理Buff
	for(int index = 0;index < m_vecBufTimerEffect.size();++index)
	{	
		// 设置Buff接受对象、施法对象
		pOldMe = g_cScriptManager.SetMe((QLunaBase*)m_pCharacter->GetScriptInterface());
		pOldHim = g_cScriptManager.SetHim((QLunaBase*)m_pSender->GetScriptInterface());

		if(CheckTimerCondition(index))
		{
			// 检测Buff前置条件
			if(m_vecBufTimerEffect[index]->GetCondition().Excecute())
			{
				switch (m_vecBufTimerEffect[index]->GetLanuchType())
				{
				case UpperLaunch:
					m_vecBufTimerEffect[index]->GetUpperEffect().Excecute(GetTemplateID(), m_nSkillLevel);
					break;
				case LowerLaunch:
					m_vecBufTimerEffect[index]->GetLowerEffect().Excecute(GetTemplateID(), m_nSkillLevel);
					break;
				case BothLaunch:
					if (m_nCurrentPersist == m_nBuffPersist - 1 )	//用上沿效果
					{
						m_vecBufTimerEffect[index]->GetUpperEffect().Excecute(GetTemplateID(), m_nSkillLevel);
					}
					else if (m_nCurrentPersist == 0)	//用下沿效果
					{
						m_vecBufTimerEffect[index]->GetLowerEffect().Excecute(GetTemplateID(), m_nSkillLevel);
					}
					break;
				case IntervalLaunch:
					m_vecBufTimerEffect[index]->GetUpperEffect().Excecute(GetTemplateID(), m_nSkillLevel);
					break;
				default:
					break;
				}
			}
		}
	}

	// 清空Buff接受对象、施法对象
	g_cScriptManager.SetMe(pOldMe);
	g_cScriptManager.SetHim(pOldHim);

	return TRUE;
}

BOOL Buffer::CheckTimerCondition(INT nIndex)
{
	if(m_vecBufTimerInterval.size() <= 0 || !m_vecBufTimerEffect[nIndex])
	{
		return FALSE;
	}

	// 被动技能只生效1次
	if(m_nBuffPersist == -1 && !m_bAffectOnce)
	{
		if(nIndex >= m_vecBufTimerEffect.size() - 1)
		{
			m_bAffectOnce = TRUE;
		}
		return TRUE;
	}

	// 达到叠加上限后不会再生效
	if(m_nCurrentStackCount >= m_nStackCount && m_bStackAffectOnce)
	{
		return FALSE;
	}

	//更新帧数并保留更新前状态
	INT nTimerInterval = m_vecBufTimerInterval[nIndex]++;

	switch (m_vecBufTimerEffect[nIndex]->GetLanuchType())
	{
	case UpperLaunch:
		{
			if (m_nCurrentPersist == m_nBuffPersist - 1)	//breath开始就会减1
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case LowerLaunch:
		{
			if (m_nCurrentPersist == 0)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case BothLaunch:
		{
			if (m_nCurrentPersist == m_nBuffPersist - 1 || m_nCurrentPersist == 0)	//breath开始就会减1
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case IntervalLaunch:
		{
			//帧数满足条件执行操作
			if (nTimerInterval % m_vecBufTimerEffect[nIndex]->GetInterval())
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		break;
	default:
		return FALSE;
		break;
	}
	return FALSE;
}

BOOL Buffer::OnRemoveBuff()
{
	QLunaBase* pOldMe = NULL;
	QLunaBase* pOldHim = NULL;

	pOldMe = g_cScriptManager.SetMe((QLunaBase*)m_pCharacter->GetScriptInterface());
	if (m_pSender)
	{
		pOldHim = g_cScriptManager.SetHim((QLunaBase*)m_pSender->GetScriptInterface());
	}

	for (int i = 0; i < m_vecBufTimerEffect.size(); ++i)
	{
		switch (m_vecBufTimerEffect[i]->GetLanuchType())
		{
		case UpperLaunch:
		case IntervalLaunch:
			m_vecBufTimerEffect[i]->GetUpperEffect().ExcecuteReverse(GetTemplateID());
			break;
		case LowerLaunch:
			m_vecBufTimerEffect[i]->GetLowerEffect().ExcecuteReverse(GetTemplateID());
			break;
		case BothLaunch:
			m_vecBufTimerEffect[i]->GetUpperEffect().ExcecuteReverse(GetTemplateID());
			m_vecBufTimerEffect[i]->GetLowerEffect().ExcecuteReverse(GetTemplateID());
			break;
		default:
			ASSERT(FALSE);
		}
	}

	for (int i = 0; i != m_vecBufTriggerEffect.size(); ++i)
	{
		m_vecBufTriggerEffect[i].psEffect->GetMagicEffect().ExcecuteReverse(GetTemplateID());
	}

	g_cScriptManager.SetMe(pOldMe);
	g_cScriptManager.SetHim(pOldHim);

	return TRUE;
}


BOOL Buffer::CheckTriggerCondition(LPCSTR szEvent, KCharacter* pEventReceive, KCharacter* pEventSender)
{
	QLunaBase* pOldMe = NULL;
	QLunaBase* pOldHim = NULL;
	if (!m_pCharacter || !strlen(szEvent) || !m_vecBufTriggerEffect.size())
	{
		return FALSE;
	}

	// 设置脚本对象

	/*
	if(m_pCharacter == pEventSender)
	{
		pOldMe = g_cScriptManager.SetMe((QLunaBase*)m_pCharacter->GetScriptInterface());
	}
	else
	{
		pOldMe = g_cScriptManager.SetMe((QLunaBase*)pEventReceive->GetScriptInterface());
	}
	*/

	pOldMe = g_cScriptManager.SetMe((QLunaBase*)pEventReceive->GetScriptInterface());

	if (pEventSender)
	{
		pOldHim = g_cScriptManager.SetHim((QLunaBase*)pEventSender->GetScriptInterface());
	}

	for (int index = 0; index < m_vecBufTriggerEffect.size(); ++index)
	{
		TriggerData& rsTrigger = m_vecBufTriggerEffect[index];
		LOG_PROCESS_ERROR( rsTrigger.psEffect );

		if((rsTrigger.psEffect->GetEvent() == szEvent) &&
			(!rsTrigger.BeTriggered() || !rsTrigger.psEffect->BeHaveCD() || (rsTrigger.psEffect->BeHaveCD() && !rsTrigger.GetCDTime()))) //判断是否有资格进行触发检测
		{
			if (!rsTrigger.BeTriggered())
			{
				rsTrigger.SetTriggered();
			}

			if (rsTrigger.psEffect->BeHaveCD())
			{
				rsTrigger.ResetCDTime();
			}

			INT nMax = 100;	//要进行随机的数
			INT nRand = KSysService::Rand(nMax) + 1;
			if (nRand <= rsTrigger.psEffect->GetProbPercent())
			{
				TriggerEffect(index);
			}
		}
	}

	// 清空脚本对象
	g_cScriptManager.SetMe(pOldMe);
	g_cScriptManager.SetHim(pOldHim);

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL Buffer::ProcessTrigger()
{
	QLunaBase* pOldMe = NULL;
	QLunaBase* pOldHim = NULL;

	QCONFIRM_RET_FALSE(m_pSender);

	// 设置脚本对象
	pOldMe = g_cScriptManager.SetMe((QLunaBase*)m_pCharacter->GetScriptInterface());
	pOldHim = g_cScriptManager.SetHim((QLunaBase*)m_pSender->GetScriptInterface());

	if(IS_PLAYER(m_dwSenderId) && !CheckRelation(m_pSender, m_pCharacter))
		return FALSE;

	ModifyThreat(m_pSender, m_pCharacter);

	for (int index = 0; index < m_vecBufTriggerEffect.size(); ++index)
	{
		TriggerData& rsTrigger = m_vecBufTriggerEffect[index];
		LOG_PROCESS_ERROR( rsTrigger.psEffect );

		INT nInterval = rsTrigger.psEffect->BeHaveCD();

		// 第一次触发或者按照间隔来触发
		if((m_nBuffPersist == -1 && m_nCurrentPersist == m_nBuffPersist) || 
			m_nCurrentPersist == m_nBuffPersist - 1 || 
			(nInterval && g_cOrpgWorld.m_dwLogicFrames % nInterval == 0))
		{

			if(m_nBuffPersist == -1 && m_nCurrentPersist == m_nBuffPersist)
			{
				m_nCurrentPersist = 0;
			}

			BufConditionCommand& condition = rsTrigger.psEffect->GetCondition();

			if (condition.Excecute()) // 条件满足，触发效果
			{
				if (!rsTrigger.BeTriggered())
				{
					rsTrigger.SetTriggered();
				}

				INT nMax = 100;	//要进行随机的数
				INT nRand = KSysService::Rand(nMax) + 1;
				if (nRand <= rsTrigger.psEffect->GetProbPercent())
				{
					TriggerEffect(index);
				}
			}
		}
	}

	{// 清空脚本对象
		g_cScriptManager.SetMe(pOldMe);
		g_cScriptManager.SetHim(pOldHim);
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL Buffer::TriggerEffect(INT nIndex)
{
	if( !(m_vecBufTriggerEffect[nIndex].psEffect) )
	{
		return FALSE;
	}

	{//出发前操作
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

		cSafeScript->CallTableFunction("BufferCondition", "PreTriggerEffect", 0, "d",
			this->GetID());
	}

	m_vecBufTriggerEffect[nIndex].psEffect->GetMagicEffect().Excecute(GetTemplateID(), m_nSkillLevel);
	
	return TRUE;
}


VOID Buffer::Breath()
{
	BOOL bSync = FALSE;
	if(!CheckSelfCondition(bSync))
	{
		return;
	}

	// BUff效果延迟
	if (m_nBuffDelayFrame)
	{
		--m_nBuffDelayFrame;
		return;
	}

	if (m_nBuffPersist != -1) // 如果是-1，说明是永久buffer
	{
	    m_nCurrentPersist--;	//先进行时间计算，即使不触发，也算时间   
	}

	ProcessTimer();
	ProcessTrigger();
}

VOID Buffer::UpdateBufferStack()
{
	m_nCurrentPersist = m_nBuffPersist;

	if(m_nCurrentStackCount == m_nStackCount)
	{
		m_bStackAffectOnce = TRUE;
	}

	if (m_nCurrentStackCount < m_nStackCount)
	{
		m_nCurrentStackCount++;
	}
}

VOID Buffer::UpdateInterval(BOOL bStack)	//设置时间
{
	m_bStackAffectOnce = TRUE;

	if (bStack)
	{
		m_nCurrentPersist += m_nBuffPersist;
	}
	else
	{
		m_nCurrentPersist = m_nBuffPersist;
	}

}

VOID Buffer::RefreshBuffPersist()
{
	m_nCurrentPersist = m_nBuffPersist;
}

VOID Buffer::SetAppendDataFromDB(const VOID* pAppendData, size_t append_size)
{
	m_vecAppendData.resize(append_size / sizeof(BuffData));
	if (append_size)
	{
		memcpy(&m_vecAppendData[0], pAppendData, append_size);
	}
}

VOID Buffer::AppendData(BYTE type, INT value)
{
	for(size_t i = 0; i < m_vecAppendData.size(); ++i)
	{
		if(type == m_vecAppendData[i].data_type)
		{//同一类型的附加数据会被重置
			m_vecAppendData[i].data_value += value;
			return;
		}
	}

	BuffData data = {type, value};
	m_vecAppendData.push_back( data );
}

INT Buffer::GetAppendData(BYTE type)
{
	for(size_t i = 0; i < m_vecAppendData.size(); ++i)
	{
		if(type == m_vecAppendData[i].data_type)
		{
			return m_vecAppendData[i].data_value;
		}
	}

	return 0;
}

CONST FightSkillTemplate* Buffer::GetBelongSkillTemplate() CONST
{
	CONST FightSkillTemplate* pTemplate =
		g_cFightSkillSetting.GetFightSkillTemplate(m_nSkillTemplateId);
	LOG_PROCESS_ERROR(pTemplate);

	return pTemplate;
EXIT0:
	return 0;
}

VOID Buffer::SetDestPosition(INT nX, INT nY, INT nZ)
{
	m_nDestX = nX;
	m_nDestY = nY;
	m_nDestZ = nZ;
}