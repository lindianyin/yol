

#include "stdafx.h"
#include "kluaai.h"
#include "onlinegamemodule/ai/kaictrl.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegamemodule/ai/kaivm.h"
#include "onlinegameworld/knpc.h"
#include "../onlinegameworld/scene/kscenemgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

INT KLuaAI::Register2Character(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaCharacter* pLuaCharacter = (KLuaCharacter*)pLunaObj;
	QCONFIRM_RET_FALSE(pLuaCharacter);

	KCharacter* pCharacter = pLuaCharacter->GetHim();
	QCONFIRM_RET_FALSE(pCharacter);

	KAICtrl* pAICtrl = pCharacter->GetCtrl<KAICtrl>();
	QCONFIRM_RET_FALSE(pAICtrl);
	QLunaBase* pLuaAi = pAICtrl->GetScriptInterface();
	QCONFIRM_RET_FALSE(pLuaAi);
	sc.PushObj(pLuaAi);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: SetAIType
// @Description		: 设置Npc的AI
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nAIType AI类型
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaAI::LuaSetAIType(QLuaScript& sc)
{
	INT nRetCode    = 0;
	INT nAIType     = 0;

	nAIType = sc.GetInt(1);
	m_pAICtrl->nAIType = nAIType;

	m_rAIVM.Setup(&m_pAICtrl->m_rcCharacter, nAIType);

	return 0;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: GetAIType
// @Description		: 获取AI类型
// @ReturnCode		: nAIType AI类型
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaAI::LuaGetAIType(QLuaScript& sc)
{
	INT nRetCode    = 0;
	INT nAIType     = 0;

	nAIType = m_rAIVM.GetAIType();
	LOG_PROCESS_ERROR(m_pAICtrl->nAIType == nAIType);

	sc.PushNumber(nAIType);

	nRetCode = TRUE;
EXIT0:
	return nRetCode;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: FireAIEvent
// @Description		: 触发AI事件
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: nEventID 事件类型
// @ArgumentComment	: dwEventSrc 触发事件者ID
// @ArgumentComment	: dwEventParam
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaAI::LuaFireAIEvent(QLuaScript& sc)
{
	INT     nParamCount = 0;
	INT     nEventID    = 0;
	INT     nEventParam = 0;
	DWORD   dwEventSrc  = ERROR_ID;

	nParamCount = sc.GetTopIndex();
	LOG_PROCESS_ERROR(nParamCount == 3);

	nEventID = sc.GetInt(1);
	LOG_PROCESS_ERROR(nEventID >= KAI_USER_EVENT);

	dwEventSrc = sc.GetInt(2);

	nEventParam = sc.GetInt(3);

	m_rAIVM.FireEvent(nEventID, dwEventSrc, nEventParam);		

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: GetAIOriginPos
// @Description		: 获取Npc记录的原点
// @ReturnCode		: 
// @ArgumentFlag	: dddd
// @ArgumentComment	: posX
// @ArgumentComment	: posY
// @ArgumentComment	: posZ
// @ArgumentComment	: faceDirection
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaAI::LuaGetAIOriginPos(QLuaScript& sc)
{
	LOG_PROCESS_ERROR(m_pAICtrl->m_rcCharacter.GetScene());

	sc.PushNumber(m_pAICtrl->nOriginPosX);
	sc.PushNumber(m_pAICtrl->nOriginPosY);
	sc.PushNumber(m_pAICtrl->nOriginPosZ);
	sc.PushNumber(m_pAICtrl->nOriginDirection);

	return 4;
EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: SetAIOriginPos
// @Description		: 记录Npc原点
// @ReturnCode		: 
// @ArgumentFlag	: dddd
// @ArgumentComment	: posX
// @ArgumentComment	: posY
// @ArgumentComment	: posZ
// @ArgumentComment	: faceDirection
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaAI::LuaSetAIOriginPos(QLuaScript& sc)
{
	LOG_PROCESS_ERROR(m_pAICtrl->m_rcCharacter.GetScene());

	m_pAICtrl->nOriginPosX        = m_pAICtrl->m_rcCharacter.m_nX;
	m_pAICtrl->nOriginPosY        = m_pAICtrl->m_rcCharacter.m_nY;
	m_pAICtrl->nOriginPosZ        = m_pAICtrl->m_rcCharacter.m_nZ;
	m_pAICtrl->nOriginDirection   = m_pAICtrl->m_rcCharacter.m_nFaceDirection;

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: GetAISelectSkill
// @Description		: 获取释放技能Id
// @ReturnCode		: dwSkillID,dwSkillLevel
// @ArgumentFlag	: 
// @ArgumentComment	:
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaAI::LuaGetAISelectSkill(QLuaScript& sc)
{
	DWORD       dwSkillID       = 0;
	DWORD       dwSkillLevel    = 0;
	const KNPC_TEMPLATE* pTemplate = NULL;

	LOG_PROCESS_ERROR(IS_NPC(m_pAICtrl->m_rcCharacter.m_dwId));

	pTemplate = ((KNpc&)m_pAICtrl->m_rcCharacter).m_pTemplate;
	LOG_PROCESS_ERROR(pTemplate);

	PROCESS_ERROR(m_pAICtrl->m_nSkillSelectIndex >= 0);
	PROCESS_ERROR(m_pAICtrl->m_nSkillSelectIndex < MAX_NPC_AI_SKILL);

	dwSkillID = pTemplate->dwSkillIDList[m_pAICtrl->m_nSkillSelectIndex];
	dwSkillLevel = pTemplate->dwSkillLevelList[m_pAICtrl->m_nSkillSelectIndex];

EXIT0:
	sc.PushNumber(dwSkillID);
	sc.PushNumber(dwSkillLevel);
	return 2;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: GetAIEventSrcId
// @Description		: 获取事件触发者ID
// @ReturnCode		: nEventSrcId
// @ArgumentFlag	: 
// @ArgumentComment	:
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaAI::LuaGetAIEventSrcId(QLuaScript& sc)
{
	sc.PushNumber(m_pAICtrl->dwAIEventSrc);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: EnableDebug
// @Description		: 启动AI调试输出信息
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nEnable 1 开启 0 关闭
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaAI::LuaEnableDebug(QLuaScript &sc)
{
	m_rAIVM.m_nDebugCount = sc.GetInt(1);
	m_rAIVM.DebugAICurrentStateInfo();
	return 0;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: SetPatrolPath
// @Description		: 设置巡逻路径
// @ReturnCode		: 
// @ArgumentFlag	: dd[d]
// @ArgumentComment	: nPatrolPath
// @ArgumentComment	: nNextPointIndex
// @ArgumentComment	: nOrderIndex
// @LuaMarkEnd
// -------------------------------------------------------------------------
int KLuaAI::LuaSetPatrolPath(QLuaScript &sc)
{
	BOOL    bResult         = false;
	BOOL    bRetCode        = false;
	int     nPatrolPath     = 0;
	int     nNextPointIndex = 0;
	int     nTopIndex       = sc.GetTopIndex(); 
	int     nOrderIndex     = 0;

	LOG_PROCESS_ERROR(m_rCharacter.m_pScene);
	LOG_PROCESS_ERROR(nTopIndex == 2 || nTopIndex == 3);

	nPatrolPath = sc.GetInt(1);
	nNextPointIndex = sc.GetInt(2);
	if (nTopIndex == 3)
	{
		nOrderIndex = sc.GetInt(3);
		LOG_PROCESS_ERROR(nOrderIndex > 0);
	}

	if (m_pAICtrl->nPatrolPathID)
	{
		m_rCharacter.m_pScene->m_AIPatrolGroupManager.LeaveGroup(m_pAICtrl->nPatrolPathID, m_rCharacter.GetId());
	}

	m_pAICtrl->nPatrolPathID = 0;
	m_pAICtrl->pPatrolPath = NULL;
	m_pAICtrl->nPatrolOrderIndex = 0;

	if (nPatrolPath)
	{
		KPatrolPath* pPatrolPath = g_SceneMgr.m_PatrolPathList.GetPatrolPath(m_rCharacter.m_pScene->m_dwTemplateId, nPatrolPath);
		LOG_PROCESS_ERROR(pPatrolPath);

		if (nOrderIndex == 0)
			nOrderIndex = 1;

		m_pAICtrl->nPatrolOrderIndex = nOrderIndex;
		m_pAICtrl->pPatrolPath = pPatrolPath;
		// hack: 不填nOrderIndex的npc单独做一个巡逻组（任务用）
		if (nOrderIndex)
			m_pAICtrl->nPatrolPathID = nPatrolPath;
		else
			m_pAICtrl->nPatrolPathID = m_rCharacter.m_dwId;

		bRetCode = m_rCharacter.m_pScene->m_AIPatrolGroupManager.SetGroupPatrolPathIndex(nPatrolPath, nNextPointIndex);
		LOG_PROCESS_ERROR(bRetCode);

		m_rCharacter.m_pScene->m_AIPatrolGroupManager.JoinGroup(m_pAICtrl->nPatrolPathID, m_rCharacter.GetId());

		if (m_rCharacter.m_pRegion)
			m_rCharacter.m_pRegion->SetActive();
	}

	bResult = true;
EXIT0:
	sc.PushNumber(bResult);
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaAI
// @LuaApiName		: SetBattleZone
// @Description		: 设置Npc战斗区域，离开区域会触发AI事件
// @ReturnCode		: 
// @ArgumentFlag	: ddddd
// @ArgumentComment	: x
// @ArgumentComment	: y
// @ArgumentComment	: z
// @ArgumentComment	: radius
// @ArgumentComment	: height
// @LuaMarkEnd
// -------------------------------------------------------------------------
int KLuaAI::LuaSetBattleZone(QLuaScript &sc)
{
	int nX          = 0;
	int nY          = 0;
	int nZ          = 0;
	int nRadius     = 0;
	int nHeight     = 0;

	nX = sc.GetInt(1);
	LOG_PROCESS_ERROR(nX >= 0);

	nY = sc.GetInt(2);
	LOG_PROCESS_ERROR(nY >= 0);

	nZ = sc.GetInt(3);
	LOG_PROCESS_ERROR(nZ >= 0);

	nRadius = sc.GetInt(4);
	LOG_PROCESS_ERROR(nRadius >= 0);

	nHeight = sc.GetInt(5);
	LOG_PROCESS_ERROR(nHeight >= 0);

	m_rCharacter.m_SimpThreatList.SetBattleZone(nX, nY, nZ, nRadius, nHeight);

EXIT0:
	return 0;
}

#include "kluaai_table.hpp"
