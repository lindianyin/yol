/* -------------------------------------------------------------------------
//	文件名		：	fightskillprotocolprocessor.h
//	创建者		：	zhangzhixiong
//	创建时间	：	20012-5-21 
//	实现者		：	
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "fightskillprotocolprocessor.h"
#include "fightskillmanager.h"
#include "onlinegameworld/kprotocol.h"
#include "fightskillprotocol.h"
#include "fightskillmodule.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kcharactermgr.h"
#include "onlinegameworld/kplayerprotocolprocess.h"
#include "onlinegamemodule/playerfaction.h"
#include "onlinegamemodule/characterskill.h"
#include "fightskilldef.h"
#include "onlinegamebase/ksysservice.h"

BOOL FightSkillProtocolProcessor::ProtocolProcess(INT nPlayerIndex, LPCBYTE pProtocol, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pProtocol);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pProtocol;
	switch (pData->byProtocol)
	{
	case emFIGHT_SKILL_PTC_C2S_REQUESTSTARTSKILL:
		{
			ProcessC2S_RequestStartSkill(nPlayerIndex, pProtocol);
		}
		break;
	case emFIGHT_SKILL_PTC_C2S_CANCELCHANNELSKILL:
		{
		    ProcessC2S_CancelChannelSkill(nPlayerIndex, pProtocol);
		}
		break;
	case emFIGHT_SKILL_PTC_C2S_CHANGE_FACTION_ROUTE:
		{
			ProcessC2S_ChangeFactionRoute(nPlayerIndex, pProtocol);
		}
		break;
	case emFIGHT_SKILL_PTC_C2S_UPGRADE_SKILL:
		{
			ProcessC2S_UpgradeSkill(nPlayerIndex, pProtocol);
		}
		break;
	/*case emFIGHT_SKILL_PTC_C2S_CHANGE_RUNEID:
		{
			ProcessC2S_ChangeRune(nPlayerIndex, pProtocol);
		}
		break;*/
	default:
		{
			QLogPrintf(LOG_ERR, "[FightSkillProtocolProcessor::ProtocolProcess] Un recognize protocol %d",
				pData->byProtocol);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL FightSkillProtocolProcessor::BroadcastCancelChannelSkill(KCharacter& rcLauncher, WORD wSkillTemplateId)
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetFightSkillModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	PTC_FIGHT_SKILL_S2C_CANCEL_CHANNEL_SKILL cSendData;
	cSendData.dwCharacterId = rcLauncher.m_dwId;
	cSendData.wSkillId = wSkillTemplateId;
	return rcLauncher.m_cCharacterServer.BroadcastNearby(&cSendData, sizeof(cSendData), FALSE, pProtocolProcessor);
}

BOOL FightSkillProtocolProcessor::BroadcastChangeChannelState(KCharacter& rcLauncher, WORD wSkillTemplateId, WORD wType, DWORD dwValue)
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetFightSkillModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	PTC_FIGHT_SKILL_S2C_CHANGE_CHANNEL_STATE cSendData;
	cSendData.dwCharacterId = rcLauncher.m_dwId;
	cSendData.wSkillId = wSkillTemplateId;
	cSendData.wType = wType;
	cSendData.dwValue = dwValue;
	return rcLauncher.m_cCharacterServer.BroadcastNearby(&cSendData, sizeof(cSendData), TRUE, pProtocolProcessor);
}

BOOL FightSkillProtocolProcessor::BroadcastStartSkill(KCharacter& rcLauncher, DWORD dwTargetId, WORD wSkillTemplateId,INT nX,INT nY,INT nZ, BOOL bSyncSelf)
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetFightSkillModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	PTC_FIGHT_SKILL_STARTSKILL cSendData;
	cSendData.dwLauncherId = rcLauncher.m_dwId;
	cSendData.dwTargetId = dwTargetId;
	cSendData.wSkillTemplateId = wSkillTemplateId;
	cSendData.nX = nX;
	cSendData.nY = nY;
	cSendData.nZ = nZ;
	//自己在客户端已经释放，不需要广播给自己
	return rcLauncher.m_cCharacterServer.BroadcastNearby(&cSendData, sizeof(cSendData), bSyncSelf, pProtocolProcessor);
}


BOOL FightSkillProtocolProcessor::ProcessC2S_RequestStartSkill(INT nPlayerIndex, LPCBYTE pProtocol)
{
	PTC_FIGHT_SKILL_REQUESTSTARTSKILL* pRequestStartSkill = (PTC_FIGHT_SKILL_REQUESTSTARTSKILL*)pProtocol;
	
	if (pRequestStartSkill->dwClientFrame > 0)
	{
		if (IS_PLAYER(pRequestStartSkill->dwLauncherId))
		{
			KPlayer* pPlayer = (KPlayer *)g_cCharacterMgr.GetById(pRequestStartSkill->dwLauncherId);

			//if (pPlayer->GetMoveState() == cmsOnRun || pPlayer->GetMoveState() == cmsOnSkill)
			{
				//QLogPrintf(LOG_ERR, "[FightSkill]进入追帧[S%d C%d]", pPlayer->m_cPlayerServer.m_dwVirtualFrame, pRequestStartSkill->dwClientFrame);
				INT nX = 0, nY = 0, nZ = 0;
				pPlayer->GetPosition(nX, nY, nZ);
				KMOVE_CRITICAL_PARAM pParam;
				pParam.byMoveState = pPlayer->GetMoveState();
				pParam.nX = nX;
				pParam.nY = nY;
				pParam.nZ = nZ;
				pPlayer->m_cPlayerServer.CheckMoveAdjust(pRequestStartSkill->dwClientFrame, pParam);

				CharacterSkill* characterSkill = NULL;
				characterSkill = pPlayer->GetCtrl<CharacterSkill>();
				LOG_PROCESS_ERROR(characterSkill);
				if (characterSkill->IsSkillInCD(pRequestStartSkill->wSkillTemplateId))
				{
                    QLogPrintf(LOG_ERR, "[FightSkill]技能追完帧后仍处于发技能状态[S%d C%d]", pPlayer->m_cPlayerServer.m_dwVirtualFrame, pRequestStartSkill->dwClientFrame);
				}
			}
		}
	}

	if (g_FightSkillManager.CanCastSkill(pRequestStartSkill->wSkillTemplateId, pRequestStartSkill->dwLauncherId, pRequestStartSkill->dwTargetId, pRequestStartSkill->nX,pRequestStartSkill->nY,pRequestStartSkill->nZ))
	{
        g_FightSkillManager.CastSkill(pRequestStartSkill->wSkillTemplateId, pRequestStartSkill->dwLauncherId, pRequestStartSkill->dwTargetId,pRequestStartSkill->nX,pRequestStartSkill->nY,pRequestStartSkill->nZ, pRequestStartSkill->bTimeScale);
	}

	return TRUE;
EXIT0:
	QLogPrintf(LOG_INFO, "[ProcessC2S_RequestStartSkill]技能追帧异常！");
	return FALSE;
}

BOOL FightSkillProtocolProcessor::ProcessC2S_CancelChannelSkill(INT nPlayerIndex, LPCBYTE pProtocol)
{
    PTC_FIGHT_SKILL_C2S_CANCEL_CHANNEL_SKILL* pCancelChannelSkill = (PTC_FIGHT_SKILL_C2S_CANCEL_CHANNEL_SKILL*)pProtocol;
	g_FightSkillManager.CancelChannelSkill(pCancelChannelSkill->wSkillTemplateId, pCancelChannelSkill->dwCharacterId);
	return TRUE;
}

BOOL FightSkillProtocolProcessor::ProcessC2S_ChangeFactionRoute(INT nPlayerIndex, LPCBYTE pProtocol)
{
	PTC_FIGHT_SKILL_C2S_CHANGE_FACTION_ROUTE* pChangeFactionRoute = (PTC_FIGHT_SKILL_C2S_CHANGE_FACTION_ROUTE*)pProtocol;
	KCharacter* pCharacter;
	//PlayerFaction* pPlayerFaction;
	CharacterSkill* pCharacterSkill = NULL;
	INT nTimeNow = 0;

	pCharacter = g_cCharacterMgr.GetById(pChangeFactionRoute->dwCharacterId);
	LOG_PROCESS_ERROR(pCharacter);

	LOG_PROCESS_ERROR(pCharacter->GetLevel() >= 21);

	pCharacterSkill = pCharacter->GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);

	nTimeNow = KSysService::Time(NULL);
	PROCESS_ERROR((nTimeNow - pCharacterSkill->m_LastChangeRouteTime) > FIGHT_SKILL_CHANGE_ROUTE_CD_TIME);

	LOG_PROCESS_ERROR(pChangeFactionRoute->byFactionRoute <= 2); // 路线只有1,2,  by KK
	pCharacter->SetCurrentRoute((INT)pChangeFactionRoute->byFactionRoute);

	pCharacterSkill->m_LastChangeRouteTime = nTimeNow;

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL FightSkillProtocolProcessor::ProcessC2S_UpgradeSkill(INT nPlayerIndex, LPCBYTE pProtocol)
{
	QLogPrintf(LOG_INFO,"..FightSkillProtocolProcessor::ProcessC2S_UpgradeSkill");
	// 技能升级
	PTC_FIGHT_SKILL_C2S_UPGRADE_SKILL* pUpgradeSkill = (PTC_FIGHT_SKILL_C2S_UPGRADE_SKILL*)pProtocol;
	KCharacter* pCharacter;
	//PlayerFaction* pPlayerFaction;
	CharacterSkill* pCharacterSkill;

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	char szNum[256];
	ZeroMemory(szNum,sizeof(szNum));
	pCharacter = g_cCharacterMgr.GetById(pUpgradeSkill->dwCharacterId);
	LOG_PROCESS_ERROR(pCharacter);
	pCharacterSkill = pCharacter->GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharacterSkill);
	/*pPlayerFaction = pCharacter->GetCtrl<PlayerFaction>();
	LOG_PROCESS_ERROR(pPlayerFaction);*/

	pCharacterSkill->UpgradeSkill(pUpgradeSkill->dwSkillTemplateId);

	//if (pCharacter->GetIsFightMap() > 0)	// 战斗地图切换符文，冷却30秒
	//{
	//	pCharacterSkill->CooldownSkillOfChangeRune(pUpgradeSkill->nSlot, pUpgradeSkill->nRune);// 冷却符文
	//}
	sprintf(szNum,"%d",pUpgradeSkill->dwSkillTemplateId);
	
	cSafeScript->CallTableFunction( "DailyTasks", "commitTaskValue", 0, "osdd",pCharacter->GetScriptInterface(),szNum,6,1);
		
	return TRUE;
EXIT0:
	return FALSE;
}

//BOOL FightSkillProtocolProcessor::ProcessC2S_ChangeRune(INT nPlayerIndex, LPCBYTE pProtocol)
//{
//	// 技能升级
//	//PTC_FIGHT_SKILL_C2S_CHANGE_RUNE* pUpgradeSkill = (PTC_FIGHT_SKILL_C2S_CHANGE_RUNE*)pProtocol;
//	//KCharacter* pCharacter;
//	//PlayerFaction* pPlayerFaction;
//	//CharacterSkill* pCharacterSkill;
//
//	//pCharacter = g_cCharacterMgr.GetById(pUpgradeSkill->dwCharacterId);
//	//LOG_PROCESS_ERROR(pCharacter);
//	//pCharacterSkill = pCharacter->GetCtrl<CharacterSkill>();
//	//LOG_PROCESS_ERROR(pCharacterSkill);
//	//pPlayerFaction = pCharacter->GetCtrl<PlayerFaction>();
//	//LOG_PROCESS_ERROR(pPlayerFaction);
//
//	//pPlayerFaction->ChangeRune(pUpgradeSkill->nSlot, pUpgradeSkill->nRune);
//
//	//if (pCharacter->GetIsFightMap() > 0)	// 战斗地图切换符文，冷却30秒
//	//{
//	//	pCharacterSkill->CooldownSkillOfChangeRune(pUpgradeSkill->nSlot, pUpgradeSkill->nRune);// 冷却符文
//	//}
//	return TRUE;
//EXIT0:
//	return FALSE;
//}

BOOL FightSkillProtocolProcessor::SyncS2C_SyncSkills(INT nPlayerIdx, DWORD dwCharacterId, WORD wSkillsCount, WORD awSkills[], WORD awSkillsLevels[])
{
	BOOL bRetCode = FALSE;
	KPlayerProtocolProcessor* pProtocolProcessor = GetFightSkillModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	IMemBlock* pMemBlock = NULL;
	PTC_FIGHT_S2C_SKILL_SYNCSKILL* pSendData = NULL;

	// 大小
	UINT nSize = sizeof(PTC_FIGHT_S2C_SKILL_SYNCSKILL) + sizeof(SKILL_ID_LEVEL) * wSkillsCount;
	pMemBlock = QCreateMemBlock(nSize);
	pSendData = (PTC_FIGHT_S2C_SKILL_SYNCSKILL*)pMemBlock->GetData();
	pSendData->byProtocol = emFIGHT_SKILL_PTC_S2C_SYNCSKILL;  // 变长需要复制protocol
	pSendData->dwCharacterId = dwCharacterId;
	pSendData->wSkillsCount = wSkillsCount;
	
	//SKILL_ID_LEVEL[] arrAllSkills = new SKILL_ID_LEVEL[wSkillsCount];  // 构造一个技能->等级结构
	SKILL_ID_LEVEL* paAllSkills = pSendData->awSkills;
	for (INT loop=0; loop < wSkillsCount;++loop)
	{
		SKILL_ID_LEVEL skill_id_level(awSkills[loop], awSkillsLevels[loop]);
		memcpy(paAllSkills + loop, (VOID*)&skill_id_level, sizeof(SKILL_ID_LEVEL)); // 拷贝技能id和等级
	}

	//UINT tmp = cSendData->GetSize();
	bRetCode = pProtocolProcessor->SendLenData(nPlayerIdx, *pSendData, nSize);

	SAFE_RELEASE(pMemBlock);

	return bRetCode;
}


// 这个基本不用了
BOOL FightSkillProtocolProcessor::SyncS2C_AddSkill(INT nPlayerIdx, DWORD dwCharacterId, INT nSkillId)
{
	KPlayerProtocolProcessor* pProtocolProcessor = GetFightSkillModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	PTC_FIGHT_SKILL_ADDSKILL cSendData;
	cSendData.nSkillId = nSkillId;
	cSendData.dwCharacterId = dwCharacterId;
	return pProtocolProcessor->SendData(nPlayerIdx, cSendData);
}

BOOL FightSkillProtocolProcessor::SyncS2C_RemoveSkill(INT nPlayerIdx, DWORD dwCharacterId, INT nSkillId)
{
	KPlayerProtocolProcessor* pProtocolProcessor =	GetFightSkillModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	PTC_FIGHT_SKILL_REMOVESKILL cSendData;
	cSendData.nSkillId = nSkillId;
	cSendData.dwCharacterId = dwCharacterId;
	return pProtocolProcessor->SendData(nPlayerIdx, cSendData);
}

BOOL FightSkillProtocolProcessor::SyncS2C_ClearCD(INT nPlayerIdx, DWORD dwCharacterId, INT nSkillId)
{
	KPlayerProtocolProcessor* pProtocolProcessor =	GetFightSkillModule()->GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	PTC_FIGHT_SKILL_CLEAR_CD cSendData;
	cSendData.nSkillId = nSkillId;
	cSendData.dwCharacterId = dwCharacterId;
	return pProtocolProcessor->SendData(nPlayerIdx, cSendData);
}

