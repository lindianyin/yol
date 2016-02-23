#include "ArenaHelper.h"
#include "config/kplayerbasedbdef.h"
#include "onlinegameworld/knpc.h"
#include "onlinegamebase/ksysservice.h"
#include "../pet/kpet.h"
#include "../pet/kpetctrl.h"
#include "../award/kawardsetting.h"
#include "onlinegamemodule/characterskill.h"
#include "../onlinegamemodule/buffer/bufsetting.h"
#include "../fightskill/fightskillsetting.h"
#include "../onlinegameworld/KRelayClient.h"
#include "onlinegameworld/kplayermgr.h"
#include "arenaprotocol.h"
#include "arenamodule.h"
#include "Runtime/TypeDef.h"

ArenaHelper g_cArenaHelper;

ArenaHelper::ArenaHelper(void)
{
	m_CurrentFightId = 0;
}

ArenaHelper::~ArenaHelper(void)
{
}

BOOL ArenaHelper::SetNpcWithDelayData(KNpc* pNpc, KDelayRoleData* pData, INT nRankIdx)
{
	BOOL bResult = FALSE;
	KDelayBaseData* pBaseData = &(pData->sBaseRoleData);
	LOG_PROCESS_ERROR(pBaseData);

	pNpc->SetSex(pBaseData->bySex);

	pNpc->SetMaxLife(pBaseData->nMaxLife);
	pNpc->SetCurrentLife(pBaseData->nMaxLife);
	//外观
	pNpc->SetHatRepresentId(pBaseData->wHatRepresentId);
	pNpc->SetBodyRepresentId(pBaseData->wBodyRepresentId);
	pNpc->SetLeftWeaponRepresentId(pBaseData->wLeftWeaponRepresentId);
	pNpc->SetLeftShoulderRepresentId(pBaseData->wLeftShoulderRepresentId);
	pNpc->SetRightWeaponRepresentId(pBaseData->wRightWeaponRepresentId);
	pNpc->SetRightShoulerRepresentId(pBaseData->wRightShoulerRepresentId);
	pNpc->SetRideRepresentId(pBaseData->wRideRepresentId);
	pNpc->SetHeadPendantRepresentId(pBaseData->wHeadPendantRepresentId);
	pNpc->SetBottomPendantRepresentId(pBaseData->wBottomPendantRepresentId);
	pNpc->SetBackPendantRepresentId(pBaseData->wBackPendantRepresentId);
	pNpc->SetPetRepresentId(pBaseData->wPetRepresentId);

	pNpc->SetFightScore(pBaseData->dwFightScore);

	CHAR szNewName[64];
	sprintf(szNewName, LANG_010, pBaseData->m_szRoleName, nRankIdx + 1);
	pNpc->SetStrName(szNewName);

	bResult = TRUE;
EXIT0:
	return bResult;
}

INT ArenaHelper::GetSkillLevel(INT nSkillId, QDelaySkillData& rSkillData)
{
    for (int i = 0; i < sizeof(rSkillData.arrSkillIdAndLevel) / sizeof(INT); i++)
	{
		int nEnumValue = rSkillData.arrSkillIdAndLevel[i];
		int nCurSkillId = (nEnumValue >> 16) & 0x0000ffff;
		if (nCurSkillId == nSkillId)
		{
			return (nEnumValue & 0xffff);
		}
	}
	return 0;
}

// 得到角色本次攻击伤害值
INT ArenaHelper::GetHurtValue(KPlayer* pPlayer, 
							  KDelayRoleData* pData,
							  BOOL bAttack,
							  INT nSkillId, 
							  INT& nIsCrit, 
							  INT& nIsMiss)
{
	KDelayBaseData* pBaseData = &(pData->sBaseRoleData);
	// TODO zhaoyu:根据skillid计算出技能系数和技能攻击力
	INT nSkillParam = 85;
	INT nExtraHurtValue = 0;
	INT nK = 4;
	INT nHurt = 0;
	INT nRand = 0;
	INT nDodge = 0;
	INT nHit = 0;
	INT nAttack = 0;
	INT nDef = 0;
	INT nMaxLife = 0;
	INT nCrit = 0;
	INT nResist = 0;
	INT nCritRate = 0;
	float fDefRate = 0.0f;
	nIsCrit = 0;
	nIsMiss = 0;

	if(bAttack)
	{
		CharacterSkill* pCharSkill = pPlayer->GetCtrl<CharacterSkill>();
        INT nSkillLevel = pCharSkill->GetSkillLevel(nSkillId);
		nSkillLevel = nSkillLevel == 0 ? 1 : nSkillLevel;  //等于0直接使用1
		InactiveSkillNumber* pSkillNum = g_cFightSkillSetting.GetInactiveSkillNumber(nSkillId, nSkillLevel);
		LOG_PROCESS_ERROR(pSkillNum);

		nSkillParam = KSysService::Rand(pSkillNum->nSkillCoefficient - 8, pSkillNum->nSkillCoefficient);
		nExtraHurtValue = pSkillNum->nHarmValue;

		nHit = pPlayer->GetHitCoefficient();
		nDodge = pBaseData->nDodge;
		nAttack = pPlayer->GetAttack();
		nDef = pBaseData->nDef * 9;
		nCrit = pPlayer->GetCriticalValue();
		nResist = pBaseData->nResist;
		nMaxLife = pBaseData->nMaxLife;
	}
	else
	{
		INT nSkillLevel = GetSkillLevel(nSkillId, pData->sSkillData);
		nSkillLevel = nSkillLevel == 0 ? 1 : nSkillLevel;  //等于0直接使用1
		InactiveSkillNumber* pSkillNum = g_cFightSkillSetting.GetInactiveSkillNumber(nSkillId, nSkillLevel);
		LOG_PROCESS_ERROR(pSkillNum);

		nSkillParam = KSysService::Rand(pSkillNum->nSkillCoefficient - 8, pSkillNum->nSkillCoefficient);
		nExtraHurtValue = pSkillNum->nHarmValue;

		nHit = pBaseData->nHit;
		nDodge = pPlayer->GetDodge();
		nAttack = pBaseData->nAttack;
		nDef = pPlayer->GetDef() * 9;
		nCrit = pBaseData->nCritical;
		nResist = pPlayer->GetResist();
		nMaxLife = pPlayer->GetMaxLife();
	}

	nHit = nHit * 100 / nDodge;
	if(nHit < 70)
		nHit = 70;

	nRand = KSysService::Rand(100) + 1;
	if(nRand >= nHit)
	{
		// 闪避
		nIsMiss = 1;
		goto EXIT0;
	}

	fDefRate = (FLOAT)nMaxLife / (nMaxLife + nDef);
	MAKE_IN_RANGE(fDefRate, 0.0f, 1.0f);

	nHurt = (INT)((nAttack * nSkillParam * fDefRate) / 1000) + nExtraHurtValue;

	nCrit = nCrit / 5;
	nCritRate = nCrit * 100 / nResist;
	MAKE_IN_RANGE(nCritRate, 0, 30);

	nRand = KSysService::Rand(100) + 1;
	if(nRand < nCritRate)
	{
		// 暴击
		nIsCrit = 1;
		nHurt = nHurt * 2;
	}

EXIT0:
	return nHurt;
}

// 得到神仙本次攻击伤害值
INT ArenaHelper::GetPetHurtValue(KPlayer* pPlayer, KDelayRoleData* pData, BOOL bAttack, INT nSkillId)
{
	KPetCtrl* pPetCtrl = NULL;
	KPet* pPet = NULL;
	// TODO zhaoyu:根据skillid计算出技能系数和技能攻击力
	INT nSkillParam = 85;
	INT nAttack = 0;
	INT nDef = 0;
	INT nHurt = 0;
	INT nMaxLife = 0;
	INT nPetSkillId = 0;
	INT nPetSkillLevel = 0;
	FightSkillTemplate* pFightSkillTemplate = NULL;
	BufferNumber* pNumber = NULL;
	INT nBuffId = 0;
	FLOAT fDefRate = 0.0f;
	KDelayBaseData* pBaseData = &(pData->sBaseRoleData);

	if(bAttack)
	{
		pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		LOG_PROCESS_ERROR(pPetCtrl);

		pPet = pPetCtrl->GetCurrentFightPet();
		PROCESS_ERROR(pPet);

		nAttack = pPet->GetBaseAttrValue(emPET_ATTR_ATTACK);
		nDef = pBaseData->nDef * 9;
		nMaxLife = pBaseData->nMaxLife;

		nPetSkillId = pPet->GetSkillIdByIndex(0); // 固定取第一个宠物技能
		nPetSkillLevel = pPet->GetSkillLevel(nPetSkillId);
	}
	else
	{
		KDelayPetData* pPetData = GetFightPetData(pData);
		PROCESS_ERROR(pPetData);
		nAttack = pPetData->nAttack;
		nDef = pPlayer->GetDef() * 9;
		nMaxLife = pPlayer->GetMaxLife();

		nPetSkillId = pPetData->arySkillIds[0];  // 固定取第一个宠物技能
		nPetSkillLevel = pPetData->arwSkillLevels[0];
	}
	
	nBuffId = g_cPetSetting.GetBuffIdBySkillId(nPetSkillId);
	LOG_PROCESS_ERROR(nBuffId);
	pNumber = g_cBufferSetting.GetBufferNumber(nBuffId, nPetSkillLevel, "me_pet_damagelife_v");
	LOG_PROCESS_ERROR(pNumber);

	fDefRate = (FLOAT)nMaxLife / (nMaxLife + nDef);
	MAKE_IN_RANGE(fDefRate, 0.0f, 1.0f);

	nSkillParam = KSysService::Rand(pNumber->m_nSkillCoefficientMin, pNumber->m_nSkillCoefficientMax);

	nHurt = (INT)((nAttack * nSkillParam * fDefRate) / 1000) + pNumber->m_nValue;

EXIT0:
	return nHurt;
}

KDelayPetData* ArenaHelper::GetFightPetData(KDelayRoleData* pDelayData)
{
	KDelayPetData* pData = NULL;
	for(int i = 0; i < 7; ++i)
	{
		KDelayPetData* pPetData = &(pDelayData->aryPets[i]);
		if(!pPetData)
			break;

		if(pPetData->byState == emPET_STATE_FIGHT)
		{
			pData = pPetData;
			break;
		}
	}
	return pData;
}

BOOL ArenaHelper::IsPlayerInFight(DWORD dwPlayerId)
{
	std::map<INT, QOFFLINE_FIGHT_STATE>::iterator it;
	for (it = m_mapFights.begin(); it != m_mapFights.end(); ++it)
	{
		if (it->second.dwStartRoleMapNpcId == dwPlayerId)
		{
			return TRUE;
		}
		if (it->second.dwOfflineRoleMapNpcId == dwPlayerId)
		{
			return TRUE;
		}
	}
	return 0;
}

INT ArenaHelper::StartOfflineFight(DWORD dwStartPlayer, DWORD dwOfflinePlayer, DWORD dwStartRoleMapNpcId, DWORD dwStartPetMapNpcId, DWORD dwOfflineRoleMapNpcId, DWORD dwOfflinePetMapNpcId)
{
	if (IsPlayerInFight(dwStartPlayer) || IsPlayerInFight(dwOfflinePlayer))
		return -1;

    QOFFLINE_FIGHT_STATE fight;
	fight.dwStartRoleId = dwStartPlayer;
	fight.dwOfflineRoleId = dwOfflinePlayer;
	fight.dwStartRoleMapNpcId = dwStartRoleMapNpcId;
	fight.dwOfflineRoleMapNpcId = dwOfflineRoleMapNpcId;
	fight.dwStartPetMapNpcId = dwStartPetMapNpcId;
	fight.dwOfflinePetMapNpcId = dwOfflinePetMapNpcId;
	fight.nFightID = m_CurrentFightId++;
	fight.eState = emOffline_Fighting;

	m_mapFights[fight.nFightID] = fight;

	g_RelayClient.DoLoadDelayRoleDataRequest(MODULE_OFFLINE_FIGHT, dwStartPlayer, dwOfflinePlayer);

	return fight.nFightID;
}

BOOL ArenaHelper::OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId)
{
	BOOL bResult = FALSE;
	KDelayRoleData sDelayData;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwSrcId);
	PROCESS_ERROR(pPlayer);

	memset(&sDelayData, 0, sizeof(KDelayRoleData));
	LOG_PROCESS_ERROR(sizeof(KDelayRoleData) == piBuffer->GetSize());
	memcpy(&sDelayData, piBuffer->GetData(), piBuffer->GetSize());

    DoFightProcess(*pPlayer, &sDelayData);

	bResult = TRUE;
EXIT0:
	return bResult;
}

DWORD ArenaHelper::GetPlayerPetMapNpcId(DWORD dwPlayerId)
{
	std::map<INT, QOFFLINE_FIGHT_STATE>::iterator it;
	for (it = m_mapFights.begin(); it != m_mapFights.end(); ++it)
	{
		if (it->second.dwStartRoleId == dwPlayerId)
		{
			return it->second.dwStartPetMapNpcId;
		}
		if (it->second.dwOfflineRoleId == dwPlayerId)
		{
			return it->second.dwOfflinePetMapNpcId;
		}
	}
	return 0;
}

DWORD ArenaHelper::GetPlayerMapNpcId(DWORD dwPlayerId)
{
	std::map<INT, QOFFLINE_FIGHT_STATE>::iterator it;
	for (it = m_mapFights.begin(); it != m_mapFights.end(); ++it)
	{
        if (it->second.dwStartRoleId == dwPlayerId)
		{
            return it->second.dwOfflineRoleMapNpcId;
		}
		if (it->second.dwOfflineRoleId == dwPlayerId)
		{
			return it->second.dwOfflineRoleMapNpcId;
		}
	}
	return 0;
}

INT ArenaHelper::GetSkillIdOfCurrRound(INT nRound, INT nRoute)
{
	static INT  arrSkillIds[2][2] = {1,4,5,4};         // 两条路线的单、必技能id

	MAKE_IN_RANGE(nRoute, 1, 2);
	// 固定按照第五回合放必杀技节奏
	if (nRound % 5 == 0)
	{
		return arrSkillIds[nRoute - 1][1];
	}
	else
	{
		return arrSkillIds[nRoute - 1][0];
	}
}

VOID ArenaHelper::RemoveFightData(DWORD dwStartRoleId, DWORD dwOfflineRoleId)
{
	std::map<INT, QOFFLINE_FIGHT_STATE>::iterator it;
	for (it = m_mapFights.begin(); it != m_mapFights.end(); ++it)
	{
		if (it->second.dwStartRoleId == dwStartRoleId && it->second.dwOfflineRoleId == dwOfflineRoleId)
		{
			m_mapFights.erase(it);
			break;
		}
	}
}

BOOL ArenaHelper::SyncFightProcess(KPlayer& rStartPlayer, KDelayRoleData* pDelayData, BOOL bFightResult, std::list<KATTACK_RECORD>& lstRecords)
{
	INT									nIdx		= 0;
	IMemBlock*							piMem		= NULL;
	BOOL								bResult		= FALSE;
	BOOL								bRetCode	= FALSE;
	KS2C_SYNC_FIGHT_PROCESS*			pSync		= NULL;
	size_t								uPakSize	= 0;
	KDelayRoleData*						pHimDelayData	= pDelayData;
	KDelayPetData*						pHimPetData = NULL;
	KPetCtrl*                           pPetCtrl = NULL;
	KPet*                               pPet = NULL;
	INT                                 nPetSkillId = 0;
	std::list<KATTACK_RECORD>::iterator	it;

	LOG_PROCESS_ERROR(lstRecords.size() > 0);

	uPakSize = sizeof(KS2C_SYNC_FIGHT_PROCESS) + lstRecords.size() * sizeof(KATTACK_RECORD);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);
	pSync = (KS2C_SYNC_FIGHT_PROCESS*)piMem->GetData();

	pSync->byProtocol = s2c_sync_fight_process;
	pSync->bResult = bFightResult;
	pSync->dwFightNpcId = GetPlayerMapNpcId(rStartPlayer.GetId());

	pPetCtrl = rStartPlayer.GetCtrl<KPetCtrl>();
	LOG_PROCESS_ERROR(pPetCtrl);

	pPet = pPetCtrl->GetCurrentFightPet();
	if (pPet != NULL)
	{
		nPetSkillId = pPet->GetSkillIdByIndex(0); // 固定取第一个宠物技能
		pSync->nMePetSkillId = nPetSkillId;
	}

	pSync->nHimPetNpcId = GetPlayerPetMapNpcId(pHimDelayData->sBaseRoleData.dwRoleId);
	pSync->nHimPetSkillId = 0;
	pHimPetData = g_cArenaHelper.GetFightPetData(pHimDelayData);
	if(pHimPetData)
	{
		pSync->nHimPetSkillId = pHimPetData->arySkillIds[0];
	}

	pSync->nRecordCount = (INT)lstRecords.size();

	for(it = lstRecords.begin(); it != lstRecords.end(); ++it)
	{
		memcpy((VOID*)&(pSync->arrRecord[nIdx]), &(*it), sizeof(KATTACK_RECORD));
		++nIdx;
	}

	RemoveFightData(rStartPlayer.GetId(), pHimDelayData->sBaseRoleData.dwRoleId);

	bRetCode = g_ArenaModule.SendLenData(rStartPlayer.GetPlayerIndex(), *pSync, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	rStartPlayer.FireEvent(emKOBJEVENTTYPE_OFFLINE_FIGHT_EXIT, rStartPlayer.GetId(), pHimDelayData->sBaseRoleData.dwRoleId, bFightResult ? 1 : 0);

	bResult = TRUE;

EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL ArenaHelper::DoFightProcess(KPlayer& rStartPlayer, KDelayRoleData* pDelayData)
{
	BOOL			bResult						= FALSE;
	KATTACK_RECORD	sRecord;
	INT				nMeLife						= 0;
	INT				nHimLife					= 0;
	INT				nAttackValue				= 0;
	INT				nPetAttackValue				= 0;
	INT				nIsCrit						= 0;
	INT				nIsMiss						= 0;
	KDelayBaseData* pBaseData					= NULL;
	ScriptSafe cSafeScript						= g_cScriptManager.GetSafeScript();
	INT             nCurrSkillId = 0;
	INT             nRoundNum = 1;
	INT             nSelfRoute = 0;
	INT             nTargetRoute = 0;
	BOOL            bFightResult = FALSE;
	std::list<KATTACK_RECORD> vecRecordList;

	pBaseData = &(pDelayData->sBaseRoleData);

	nSelfRoute = rStartPlayer.GetCurrentRoute();
	nTargetRoute = pDelayData->sSkillData.nFactionRoute;

	nHimLife = pBaseData->nMaxLife;
	nMeLife	= rStartPlayer.GetMaxLife();

	while(TRUE)
	{
		nCurrSkillId = GetSkillIdOfCurrRound(nRoundNum, nSelfRoute);

		nAttackValue = g_cArenaHelper.GetHurtValue(&rStartPlayer, pDelayData, TRUE, nCurrSkillId, nIsCrit, nIsMiss);
		LOG_PROCESS_ERROR(nAttackValue > 0 || nIsMiss);

		nPetAttackValue = g_cArenaHelper.GetPetHurtValue(&rStartPlayer, pDelayData, TRUE, 7000);

		sRecord.bySkillId = (BYTE)nCurrSkillId;
		sRecord.bAttack = 1;
		sRecord.bIsCrit = (BYTE)nIsCrit;
		sRecord.bIsMiss = (BYTE)nIsMiss;
		sRecord.nHurtValue = nAttackValue;
		sRecord.nPetHurtValue = nPetAttackValue;
		vecRecordList.push_back(sRecord);

		nHimLife -= nAttackValue;
		nHimLife -= nPetAttackValue;
		if(nHimLife <= 0)
		{
			bFightResult = TRUE;

			break;
		}

		nCurrSkillId = GetSkillIdOfCurrRound(nRoundNum, nTargetRoute);

		nAttackValue = GetHurtValue(&rStartPlayer, pDelayData, FALSE, nCurrSkillId, nIsCrit, nIsMiss);
		LOG_PROCESS_ERROR(nAttackValue > 0 || nIsMiss);

		nPetAttackValue = g_cArenaHelper.GetPetHurtValue(&rStartPlayer, pDelayData, FALSE, 7000);

		sRecord.bySkillId = (BYTE)nCurrSkillId;
		sRecord.bAttack = 0;
		sRecord.bIsCrit = (BYTE)nIsCrit;
		sRecord.bIsMiss = (BYTE)nIsMiss;
		sRecord.nHurtValue = nAttackValue;
		sRecord.nPetHurtValue = nPetAttackValue;
		vecRecordList.push_back(sRecord);

		nMeLife -= nAttackValue;
		nMeLife -= nPetAttackValue;
		// 挑战失败
		if(nMeLife <= 0)
		{
			bFightResult = FALSE;

			break;
		}

		nRoundNum++;
	}

	SyncFightProcess(rStartPlayer, pDelayData, bFightResult, vecRecordList);

	bResult = TRUE;
EXIT0:
	return bResult;
}