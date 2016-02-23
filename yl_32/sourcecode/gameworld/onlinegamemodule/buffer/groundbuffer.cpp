/********************************************************************
	created:	2012/06/05 10:22
	filename: 	groundbuffer.cpp
	author:		zhangjunjie
*********************************************************************/

#include "stdafx.h"
#include "groundbuffer.h"
#include "buffer/bufsetting.h"
#include "../onlinegameworld/scene/kscenemgr.h"
#include "onlinegameworld/kplayerprotocolprocess.h"
#include "onlinegameworld/kplayer.h"
#include "buffer/bufferprotocolprocessor.h"
#include "buffer/bufferprotocol.h"
#include "buffer/buffermanager.h"
#include "onlinegamemodule/buffermodule.h"
#include "onlinegamemodule/characterbuffer.h"
#include <algorithm>
#include "onlinegamemodule/ai/kaivm.h"
#include "../onlinegamemodule/ai/ThreatList.h"
#include "onlinegamemodule/ai/kaictrl.h"
#include "onlinegameworld/korpgworld.h"


GroundBuffer::GroundBuffer() : m_dwId(KD_BAD_ID)
{
	Clear();
}

GroundBuffer::~GroundBuffer()
{
	Clear();
}

BOOL GroundBuffer::Init(INT nBufferTemplateId,DWORD dwSceneId,INT nX,INT nY,INT nZ)
{
	BufferTemplate* pTemplate = g_cBufferSetting.GetBufferTemplate(nBufferTemplateId);
	LOG_PROCESS_ERROR(pTemplate);
	m_pTemplate = pTemplate;
	m_nTemplateId = nBufferTemplateId;
	m_nPositionX = nX;
	m_nPositionY = nY;
	m_nPositionZ = nZ;
	m_dwSceneId = dwSceneId;
	m_pCaster = NULL;
	m_bIsActive = FALSE;
	m_bDoSelf	= FALSE;
	m_bAffectOnce = FALSE;
	m_nBuffPersist = pTemplate->m_nBuffPersist;
	m_bDeath = pTemplate->m_bDeath;

	m_nStackCategory = pTemplate->m_nStackCategory;

	m_vecBufTimerEffect.resize(0);
	m_vecBufTriggerEffect.resize(0);
	m_vecLastEffectCharacter.resize(0);
	m_vecGroundAppendData.resize(0);

	m_uLifeTime = 0;
	m_nBuffDelayFrame = pTemplate->m_nBuffDelayFrame;
	m_nMagnification = pTemplate->m_nMagnification;
	m_nGrow = pTemplate->m_nGrow;
	m_nIfUseSkillLv = pTemplate->m_nIfUseSkillLv;
	m_nK = pTemplate->m_nK;

	for(std::vector<INT>::iterator it_timer = m_pTemplate->m_vecBufTimerEffectID.begin();
		it_timer != m_pTemplate->m_vecBufTimerEffectID.end();
		++it_timer)
	{
		BufTimerEffect* effect = g_cBufferSetting.GetTimerEffect(*it_timer);
		if(effect)
		{
			m_vecBufTimerEffect.push_back(effect);
		}
	}
	for(std::vector<INT>::iterator it_trigger = m_pTemplate->m_vecBufTriggerEffectID.begin();
		it_trigger != m_pTemplate->m_vecBufTriggerEffectID.end();
		++it_trigger)
	{
		TriggerData sEffect = g_cBufferSetting.GetTriggerEffect(*it_trigger);
		if(sEffect.psEffect)
		{
			m_vecBufTriggerEffect.push_back(sEffect);
		}
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL GroundBuffer::ScanCharacter(std::vector<KCharacter*>& rtCharacter)
{
	using namespace std;
	assert(m_pTemplate);

	if(!m_pTemplate->m_bIsGroundBuffer && m_pCaster->m_pScene)
		m_dwSceneId = m_pCaster->GetSceneId();

	KScene* pScene = (KScene*)g_pSceneMgr->GetScene(m_dwSceneId);
	if(!pScene)
	{
		RemoveFromPool();
		return FALSE;
	}

	TKTraverseHelper<KCharacter> m_cTravHelper;
	pScene->TraverseNearbyPlayers(m_cTravHelper, m_nPositionX, m_nPositionY);
	pScene->TraverseNearbyNpcs(m_cTravHelper, m_nPositionX, m_nPositionY);

	// 区域内玩家
	const std::vector<KCharacter*>& rtRegionList = m_cTravHelper.m_vObjList;
	if ( !rtRegionList.size() )
	{
		return FALSE;
	}

	// Buff范围内可作用角色
	std::vector<KCharacter*> tInBuffList;
	const INT nRadius = (INT)(m_pTemplate->m_fBuffRadius * CELL_LENGTH * 2);	// Buff有效距离
	for (std::vector<KCharacter*>::const_iterator iter = rtRegionList.begin();
		iter != rtRegionList.end();
		++iter)
	{
		KCharacter* pTarget = *iter;
		QCONFIRM_RET_FALSE(pTarget);

		BOOL bIsTeamMate = FALSE;
		if(m_pCaster != pTarget && IS_PLAYER(m_pCaster->GetId()) && IS_PLAYER(pTarget->GetId()))
		{
			INT nTeamId1 = ((KPlayer*)m_pCaster)->GetTeamId();
			INT nTeamId2 = ((KPlayer*)pTarget)->GetTeamId();
			if (nTeamId1 != 0 && nTeamId1 == nTeamId2)
				bIsTeamMate = TRUE;
		}

		INT nRelation = g_GetRelation(m_pCaster, pTarget);
		if ((pTarget->GetMoveState() != cmsOnDeath && m_pCaster->GetMoveState() != cmsOnDeath)																					&&	// 死亡玩家从队列中删掉
			(!pTarget->IsIgnoreDebuff(m_pTemplate->m_nBuffType))																&&	// 玩家有免疫特定类型的buff
			((nRelation == sortSelf && m_pTemplate->m_abRelation[emRELATION_SELF-1])																	||	// 对自己有效(顺序不可变)
			(pTarget != m_pCaster && bIsTeamMate && m_pTemplate->m_abRelation[emRELATION_TEAMMATE-1])	||	// 对队友有效
			 ((nRelation == sortAlly) && m_pTemplate->m_abRelation[emRELATION_FRIEND-1])	||	// 对友方有效
			 ((nRelation == sortEnemy) && m_pTemplate->m_abRelation[emRELATION_ENEMY-1]))								// 对敌人有效
			 )
		{
			INT nX, nY, nZ;
			pTarget->GetPosition(nX,nY,nZ);
			INT nDistance2 = g_GetDistance2(nX,nY,m_nPositionX,m_nPositionY);

			// 检测Buff有效范围内的玩家
			if ( !(nRadius * nRadius < nDistance2) )
			{
				tInBuffList.push_back(pTarget);
			}
		}
	}

	// Buff实际作用角色
	rtCharacter.clear();
	const INT nMaxEffectNum = m_pTemplate->m_nBuffEffectNum;			// Buff有效作用数量
	if ( !(nMaxEffectNum < tInBuffList.size()) )
	{
		rtCharacter.swap(tInBuffList);
	}
	else
	{
		random_shuffle(tInBuffList.begin(), tInBuffList.end());
		tInBuffList.resize(nMaxEffectNum);
		rtCharacter.swap(tInBuffList);
	}

	return TRUE;
}

VOID GroundBuffer::Clear()
{
	m_pTemplate  = NULL;
	m_pCaster = NULL;
	m_bIsActive = FALSE;

	m_nPositionX = 0;
	m_nPositionY = 0;
	m_nPositionZ = 0;
	m_dwSceneId = 0;
	m_uLifeTime = 0;
	m_nBuffDelayFrame = 0;
	m_pBinder	= 0;
	
	m_vecBufTimerEffect.clear();
	m_vecBufTriggerEffect.clear();
	m_vecLastEffectCharacter.clear();
}

VOID GroundBuffer::UnInit()
{
	// 对作用玩家逆运算
	CheckTriggerCondition("LeaveRange()", m_vecLastEffectCharacter);

	if (m_vecLastEffectCharacter.size() > 0)
	{
		OnRemoveGroundBuffer(m_vecLastEffectCharacter);
	}

	m_bIsActive = FALSE;
	m_dwSceneId = 0;
}

VOID GroundBuffer::RemoveFromPool()
{
	//删除buffer
	g_cBufferMgr.RemoveGroundBuffer(m_dwId);
	if(m_vecLastEffectCharacter.size() > 0)
		OnRemoveGroundBuffer(m_vecLastEffectCharacter);
}

VOID GroundBuffer::Breath()
{
	using namespace std;

	INT nRetCode = 0;

	if (!m_bIsActive)
	{
		return;
	}

	//对地Buff必须有范围
	if(m_pTemplate->m_fBuffRadius == 0)
	{
		printf("fffffffffffffffffffffffffffffffffffff:%d\n", m_pTemplate->m_nBuffTemplateID);
		ASSERT(!"对地Buff必须有范围");
		return;
	}

	// BUff效果延迟
	if (m_nBuffDelayFrame)
	{
		--m_nBuffDelayFrame;
		return;
	}

	// 如果是绑定Buff要更新Buff位置
	if (m_pBinder && !m_pTemplate->m_bIsGroundBuffer)
	{
		m_pBinder->GetPosition(m_nPositionX, m_nPositionY, m_nPositionZ);
	}

	// 计算实际操作角色列表
	vector<KCharacter*> tEffectCharacter;
	BOOL  bEffect = ScanCharacter(tEffectCharacter);

	if(!CheckSelfCondition())
	{
		g_cBufferMgr.RemoveGroundBuffer(m_dwId, FALSE);  // 时间终结结束的buffer，不发到客户端
		return;
	}

	if (bEffect)
	{
		ModifyThreat(tEffectCharacter);
		// 对范围内角色操作
		ProcessTimer(tEffectCharacter);
		ProcessTrigger(tEffectCharacter);
	}

	// 排序，对下面过程用
	sort(m_vecLastEffectCharacter.begin(), m_vecLastEffectCharacter.end());
	sort(tEffectCharacter.begin(), tEffectCharacter.end());

	// 对新进入范围的玩家执行触发检测
	{
		vector<KCharacter*> tNewCharacter(tEffectCharacter.size() + m_vecLastEffectCharacter.size());	//离开区域的玩家
		vector<KCharacter*>::const_iterator iterNewEnd =
			set_difference(tEffectCharacter.begin(), tEffectCharacter.end(), m_vecLastEffectCharacter.begin(), m_vecLastEffectCharacter.end(),
			tNewCharacter.begin());

		tNewCharacter.resize(iterNewEnd - tNewCharacter.begin());
		CheckTriggerCondition("EnterRange()", tNewCharacter);
	}

	// 对离开作用范围的玩家执行触发逆运算
	{
		vector<KCharacter*> tOUtCharacter(tEffectCharacter.size() + m_vecLastEffectCharacter.size());	//离开区域的玩家
		vector<KCharacter*>::const_iterator iterOutEnd =
			set_difference(m_vecLastEffectCharacter.begin(), m_vecLastEffectCharacter.end(), tEffectCharacter.begin(), tEffectCharacter.end(),
			tOUtCharacter.begin());

		tOUtCharacter.resize(iterOutEnd - tOUtCharacter.begin());
		CheckTriggerCondition("LeaveRange()", tOUtCharacter);
	}

	// 更新最后一次作用效果的玩家列表
	m_vecLastEffectCharacter.swap(tEffectCharacter);

	++m_uLifeTime;	// 生命期
}

BOOL GroundBuffer::CheckTimmerCondition(const BufTimerEffect* pEffect)
{
	switch (pEffect->GetLanuchType())
	{
	case UpperLaunch:
		{
			return m_uLifeTime == 0;
		}
		break;
	case LowerLaunch:
		{
			return m_uLifeTime == m_nBuffPersist - 1;
		}
		break;
	case BothLaunch:
		{
			return (m_uLifeTime == 0) || (m_uLifeTime == m_nBuffPersist - 1);
		}
		break;
	case IntervalLaunch:
		{
			//帧数满足条件执行操作
			return !(m_uLifeTime % pEffect->GetInterval());
		}
		break;
	default:
		return FALSE;
	}
}

BOOL GroundBuffer::CheckTriggerCondition(LPCSTR szEvent, const std::vector<KCharacter*>& rtBeCastCha)
{
	if (!m_pCaster  || !strlen(szEvent) || !m_vecBufTriggerEffect.size())
	{
		return FALSE;
	}

	for (int index = 0; index < m_vecBufTriggerEffect.size(); ++index)
	{
		TriggerData& rsTrigger = m_vecBufTriggerEffect[index];
		LOG_PROCESS_ERROR( rsTrigger.psEffect );

		if((rsTrigger.psEffect->GetEvent() == szEvent) &&
			(/*!rsTrigger.BeTriggered() || */!rsTrigger.psEffect->BeHaveCD() || !rsTrigger.GetCDTime())) //判断是否有资格进行触发检测
		{
			/*if (!rsTrigger.BeTriggered())
			{
				rsTrigger.SetTriggered();
			}*/

			if (rsTrigger.psEffect->BeHaveCD())
			{
				rsTrigger.ResetCDTime();
			}

			INT nMax = 100;	//要进行随机的数
			INT nRand = KSysService::Rand(nMax) + 1;
			if (nRand <= rsTrigger.psEffect->GetProbPercent())
			{
				for (vector<KCharacter*>::const_iterator iter = rtBeCastCha.begin();
							 iter != rtBeCastCha.end(); ++iter)
				{
					g_cScriptManager.SetMe((QLunaBase*)(*iter)->GetScriptInterface());
					g_cScriptManager.SetHim((QLunaBase*)m_pCaster->GetScriptInterface());

					rsTrigger.psEffect->GetMagicEffect().Excecute(GetTemplateId(), m_nSkillLevel);
				}
			}
		}
	}

	g_cScriptManager.SetMe(NULL);
	g_cScriptManager.SetHim(NULL);

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL GroundBuffer::ModifyThreat(const std::vector<KCharacter*>& rtBeCastCha)
{
	BOOL bRetCode = FALSE;
	KCharacter* pTarget = NULL;

	for (std::vector<KCharacter*>::const_iterator iter = rtBeCastCha.begin();
		iter != rtBeCastCha.end();
		++iter)
	{
		pTarget = *iter;

		if(pTarget == m_pCaster)
			continue;

		if (IS_DEBUFF(m_pTemplate->m_nBuffType)) 
		{
			if (!pTarget->IsSkillHidden(m_pCaster))// 如对发射者隐身，则不触发受伤害事件
			{
				pTarget->m_SimpThreatList.UpdateKeepThreatField();

				pTarget->m_AIData.ChoiceTarget[aittAttackTarget].SetTarget(m_pCaster);

				pTarget->m_AIVM.FireEvent(aevOnAttacked, m_pCaster->GetId(), 0);

				bRetCode = pTarget->m_SimpThreatList.ModifyThreat(thtSpecialThreat, m_pCaster, 0);
				QCONFIRM_RET_FALSE(bRetCode);
			}
		}
		else
		{
			bRetCode = pTarget->m_SimpThreatList.ModifyThreaten(thtSpecialThreat, m_pCaster, 0);
			QCONFIRM_RET_FALSE(bRetCode);
		}
	}

	return TRUE;
}

VOID GroundBuffer::ProcessTimer(const std::vector<KCharacter*>& rtBeCastCha)
{
	if( !m_vecBufTimerEffect.size() || !m_pCaster)
	{
		return;
	}

	// 设置Buff接受对象、施法对象

	// 处理TimerEffect
	for(int index = 0;index < m_vecBufTimerEffect.size();++index)
	{
		BufTimerEffect* pEffect = m_vecBufTimerEffect[index];

		if ( CheckTimmerCondition(pEffect) )
		{

			for (std::vector<KCharacter*>::const_iterator iter = rtBeCastCha.begin();
				 iter != rtBeCastCha.end();
				 ++iter)
			{
				KCharacter* pTarget = *iter;
				QCONFIRM_RET(pTarget, );
				// 设置脚本对象
				g_cScriptManager.SetMe(pTarget->GetScriptInterface());
				g_cScriptManager.SetHim(m_pCaster->GetScriptInterface());

				if(m_pCaster->GetMoveState() == cmsOnDeath)
					continue;

				if(!pEffect->GetCondition().Excecute())
				{
					continue;
				}

				switch (pEffect->GetLanuchType())
				{
				case UpperLaunch:
					pEffect->GetUpperEffect().Excecute(GetTemplateId(), m_nSkillLevel);
					break;
				case LowerLaunch:
					pEffect->GetLowerEffect().Excecute(GetTemplateId(), m_nSkillLevel);
					break;
				case BothLaunch:
					if (m_uLifeTime == 0 )	//用上沿效果
					{
						pEffect->GetUpperEffect().Excecute(GetTemplateId(), m_nSkillLevel);
					}
					else if (m_uLifeTime == m_nBuffPersist - 1)	//用下沿效果
					{
						pEffect->GetLowerEffect().Excecute(GetTemplateId(), m_nSkillLevel);
					}
					break;
				case IntervalLaunch:
					{
						pEffect->GetUpperEffect().Excecute(GetTemplateId(), m_nSkillLevel);
					}
					break;
				default:
					break;
				}
			}
		}
	}

	// 清空脚本对象
	g_cScriptManager.SetMe(NULL);
	g_cScriptManager.SetHim(NULL);
}

VOID GroundBuffer::ProcessTrigger(const std::vector<KCharacter*>& rtBeCastCha)
{
	QLunaBase* pOldMe = NULL;
	QLunaBase* pOldHim = NULL;
	KCharacter* pTarget = NULL;

	if( !m_vecBufTriggerEffect.size() || !m_pCaster)
	{
		return;
	}

	// 设置Buff接受对象、施法对象

	for (int index = 0; index < m_vecBufTriggerEffect.size(); ++index)
	{
		TriggerData& rsTrigger = m_vecBufTriggerEffect[index];
		LOG_PROCESS_ERROR( rsTrigger.psEffect );

		INT nInterval = rsTrigger.psEffect->BeHaveCD();

		// 如果nInterval为0就只生效1次
		if((!nInterval && !m_bAffectOnce) || (nInterval && g_cOrpgWorld.m_dwLogicFrames % nInterval == 0))
		{
			m_bAffectOnce = TRUE;
			BOOL bTriggered = false;
			for (std::vector<KCharacter*>::const_iterator iter = rtBeCastCha.begin();
				 iter != rtBeCastCha.end();
				 ++iter)
			{
				// 设置脚本对象
				pOldMe = g_cScriptManager.SetMe((*iter)->GetScriptInterface());
				pOldHim = g_cScriptManager.SetHim(m_pCaster->GetScriptInterface());

				if ( !rsTrigger.psEffect->GetCondition().Excecute()) // 条件满足，触发效果
				{
					continue;
				}

				INT nMax = 100;	//要进行随机的数
				INT nRand = KSysService::Rand(nMax) + 1;
				if (nRand <= rsTrigger.psEffect->GetProbPercent())
				{
					bTriggered = true;
					break;
				}
			}

			if (!bTriggered)
				continue;

			for (std::vector<KCharacter*>::const_iterator iter = rtBeCastCha.begin();
				iter != rtBeCastCha.end();
				++iter)
			{
				pTarget = *iter;
				// 设置脚本对象
				pOldMe = g_cScriptManager.SetMe(pTarget->GetScriptInterface());
				pOldHim = g_cScriptManager.SetHim(m_pCaster->GetScriptInterface());

				{// 触发前操作
					ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

					cSafeScript->CallTableFunction("BufferCondition", "PreTriggerEffect", 0, "ddd",
						m_nPositionX, m_nPositionY, m_nPositionZ);
				}

				rsTrigger.psEffect->GetMagicEffect().Excecute(GetTemplateId(), m_nSkillLevel);
			}
		}
	}

	g_cScriptManager.SetMe(pOldMe);
	g_cScriptManager.SetHim(pOldHim);

EXIT0:
	return;
}

VOID GroundBuffer::OnRemoveGroundBuffer(const std::vector<KCharacter*>& rtBeCastCha)
{
	BufTimerEffect* pEffect = NULL;
	
	// timer逆运算
	for (int i = 0; i != m_vecBufTimerEffect.size(); ++i)
	{
		pEffect = m_vecBufTimerEffect[i];
		for (std::vector<KCharacter*>::const_iterator iter = rtBeCastCha.begin();
			iter != rtBeCastCha.end();
			++iter)
		{
			g_cScriptManager.SetMe((*iter)->GetScriptInterface());
			if(m_pCaster)
				g_cScriptManager.SetHim(m_pCaster->GetScriptInterface());
			if(!pEffect) continue;
			switch (pEffect->GetLanuchType())
			{
			case UpperLaunch:
			case IntervalLaunch:
				pEffect->GetUpperEffect().ExcecuteReverse(GetTemplateId());
				break;
			case LowerLaunch:
				pEffect->GetLowerEffect().ExcecuteReverse(GetTemplateId());
				break;
			case BothLaunch:
				pEffect->GetUpperEffect().ExcecuteReverse(GetTemplateId());
				pEffect->GetLowerEffect().ExcecuteReverse(GetTemplateId());
				break;
			default:
				break;
			}
		}
	}

	// trigger逆运算
	for (int index = 0; index < m_vecBufTriggerEffect.size(); ++index)
	{
		TriggerData& rsTrigger = m_vecBufTriggerEffect[index];
		for (std::vector<KCharacter*>::const_iterator iter = rtBeCastCha.begin();
			iter != rtBeCastCha.end();
			++iter)
		{
			g_cScriptManager.SetMe((*iter)->GetScriptInterface());
			if(m_pCaster)
				g_cScriptManager.SetHim(m_pCaster->GetScriptInterface());

			if(rsTrigger.psEffect)
				rsTrigger.psEffect->GetMagicEffect().ExcecuteReverse(GetTemplateId());
		}
	}
	g_cScriptManager.SetMe(NULL);
	g_cScriptManager.SetHim(NULL);
}

BOOL GroundBuffer::CheckSelfCondition()
{
	if (m_uLifeTime < m_nBuffPersist)
	{
		return TRUE;
	}
	return FALSE;
}

VOID GroundBuffer::BindCaster(KCharacter* pCaster)
{
	if (pCaster)
	{
		m_pCaster = pCaster;
	}
}

VOID GroundBuffer::BindFightSkill(INT nSkillTemplateId)
{
	m_nSkillTemplateId = nSkillTemplateId;
}

BOOL GroundBuffer::BroadCastGroundBuffer()
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetYLBufferModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	QCONFIRM_RET_FALSE(m_pTemplate);
	BUFFER_ADDBUFFER cAddBuffer;
	cAddBuffer.dwBufferID = m_dwId + BUFFER_POOL_MAX_COUNT;//在客户端buff和groundbuff不区分，所以加上buff的最大值，保证id唯一
	cAddBuffer.nBuffTemplateId = m_pTemplate->m_nBuffTemplateID;
	cAddBuffer.bGroundBuffer = TRUE;
	cAddBuffer.bStackChange = FALSE;
	cAddBuffer.bAddOnLogin = FALSE;
	cAddBuffer.nX = m_nPositionX;
	cAddBuffer.nY = m_nPositionY;
	cAddBuffer.nZ = m_nPositionZ;
	cAddBuffer.nBuffInterval = m_nBuffPersist;
	cAddBuffer.nStackCount = 0;
	cAddBuffer.nCharacterID = m_pCaster ? m_pCaster->GetId() : 0;
	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = (LPCVOID)&cAddBuffer;
	cBroadcaster.m_uSize = sizeof(cAddBuffer);
	cBroadcaster.m_pProcessor = pProtocolProcessor;
	//cBroadcaster.m_nRange = 1; //by ldy 限制广播范围

	KScene* pScene = (KScene*)g_pSceneMgr->GetScene(m_dwSceneId);
	if(!pScene)
	{
		RemoveFromPool();
		return FALSE;
	}

	TKTraverseHelper<KCharacter> m_cTravHelper;
	pScene->TraverseNearbyPlayers(cBroadcaster, m_nPositionX, m_nPositionY);

	//BufferProtocolProcessor::Broadcast_AddGroundBuffer(m_dwSceneId, m_dwId, m_pTemplate->m_nBuffTemplateID, m_nPositionX, m_nPositionY, m_nPositionZ);
	return TRUE;
}

BOOL GroundBuffer::BroadcastRemoveGroundBuffer()
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetYLBufferModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	QCONFIRM_RET_FALSE(m_pTemplate);
	BUFFER_REMOVEBUFFER cRemoveBuffer;
	cRemoveBuffer.dwBufferID = m_dwId + BUFFER_POOL_MAX_COUNT;//在客户端buff和groundbuff不区分，所以加上buff的最大值，保证id唯一
	cRemoveBuffer.nBuffTemplateId = m_pTemplate->m_nBuffTemplateID;
	cRemoveBuffer.bGroundBuffer = TRUE;
	//cRemoveBuffer.bStackChange = FALSE;
	//cRemoveBuffer.bAddOnLogin = FALSE;
	//cRemoveBuffer.nX = m_nPositionX;
	//cRemoveBuffer.nY = m_nPositionY;
	//cRemoveBuffer.nZ = m_nPositionZ;
	//cRemoveBuffer.nBuffInterval = m_pTemplate->m_nBuffPersist;
	//cRemoveBuffer.nStackCount = 0;
	cRemoveBuffer.dwCharacterId = m_pCaster ? m_pCaster->GetId() : 0;

	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = (LPCVOID)&cRemoveBuffer;
	cBroadcaster.m_uSize = sizeof(cRemoveBuffer);
	cBroadcaster.m_pProcessor = pProtocolProcessor;
	//cBroadcaster.m_nRange = 1; // by ldy 限制广播区域

	KScene* pScene = (KScene*)g_pSceneMgr->GetScene(m_dwSceneId);
	if(!pScene)
	{
		RemoveFromPool();
		return FALSE;
	}

	TKTraverseHelper<KCharacter> m_cTravHelper;
	pScene->TraverseNearbyPlayers(cBroadcaster, m_nPositionX, m_nPositionY);

	//BufferProtocolProcessor::Broadcast_RemoveGroundBuffer(m_dwId, m_pTemplate->m_nBuffTemplateID, m_nPositionX, m_nPositionY, m_nPositionZ);
	return TRUE;
}

VOID GroundBuffer::AppendData(BYTE type, INT value)
{
	for(size_t i = 0; i < m_vecGroundAppendData.size(); ++i)
	{
		if(type == m_vecGroundAppendData[i].data_type)
		{//同一类型的附加数据会被重置
			m_vecGroundAppendData[i].data_value += value;
			return;
		}
	}

	BuffData data = {type, value};
	m_vecGroundAppendData.push_back( data );
}

INT GroundBuffer::GetAppendData(BYTE type)
{
	for(size_t i = 0; i < m_vecGroundAppendData.size(); ++i)
	{
		if(type == m_vecGroundAppendData[i].data_type)
		{
			return m_vecGroundAppendData[i].data_value;
		}
	}

	return 0;
}