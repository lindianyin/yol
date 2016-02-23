#pragma once

#include "stdafx.h"
#include "arenadef.h"
#include "onlinegameworld/kplayer.h"

class ArenaHelper
{
public:
	ArenaHelper(void);
	~ArenaHelper(void);

	BOOL SetNpcWithDelayData(KNpc* pNpc, KDelayRoleData* pData, INT nRankIdx);

	INT GetHurtValue(KPlayer* pPlayer, 
		KDelayRoleData* pData,
		BOOL bAttack,
		INT nSkillId, 
		INT& nIsCrit, 
		INT& nIsMiss);

	INT GetPetHurtValue(KPlayer* pPlayer, KDelayRoleData* pData, BOOL bAttack, INT nSkillId);

	KDelayPetData* GetFightPetData(KDelayRoleData* pData);

	INT GetSkillIdOfCurrRound(INT nRound, INT nRoute);

    INT StartOfflineFight(DWORD dwStartPlayer, DWORD dwOfflinePlayer, DWORD dwStartRoleMapNpcId, DWORD dwStartPetMapNpcId, DWORD dwOfflineRoleMapNpcId, DWORD dwOfflinePetMapNpcId);

	BOOL IsPlayerInFight(DWORD dwPlayerId);

    BOOL OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId);

private:
	BOOL SyncFightProcess(KPlayer& rStartPlayer, KDelayRoleData* pDelayData, BOOL bFightResult, std::list<KATTACK_RECORD>& lstRecords);
	BOOL DoFightProcess(KPlayer& rStartPlayer, KDelayRoleData* pDelayData);

	DWORD GetPlayerMapNpcId(DWORD dwPlayerId);
	DWORD GetPlayerPetMapNpcId(DWORD dwPlayerId);

	VOID  RemoveFightData(DWORD dwStartRoleId, DWORD dwOfflineRoleId);

	INT GetSkillLevel(INT nSkillId, QDelaySkillData& pSkillData);
	INT m_CurrentFightId;
	std::map<INT, QOFFLINE_FIGHT_STATE> m_mapFights;
};


extern ArenaHelper g_cArenaHelper;