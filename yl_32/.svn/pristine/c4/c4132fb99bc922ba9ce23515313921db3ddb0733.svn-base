#include "stdafx.h"
#include "krankmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/korpgworld.h"
#include "rank/RankRelayClient.h"
#include "../onlinegameworld/KRelayClient.h"
#include "../kin/KinRelayClient.h"
#include "rankdef.h"

KRankManager g_cRankManager;

KRankManager::KRankManager()
{

}

KRankManager::~KRankManager()
{

}

BOOL KRankManager::Init()
{
	InitRanks();
	InitOpenServerRanks();
	return TRUE;
}

VOID KRankManager::InitRanks()
{
	PROCESS_ERROR(g_cOrpgWorld.m_nServerMode != emRUNMODE_MASTER);

	InitRoleRank();
	InitPetLvRank();
	InitPetFSRank();
	InitRideRank();
	InitMpjjRank();
	InitLGRank();
	InitSmzcRank();

	InitTongFSRank();
	InitTongSrRank();

	//InitPlayerLevel();
	//InitTower();

	InitOpenServerRank();
EXIT0:
	return;
}

BOOL KRankManager::UnInit()
{
	return TRUE;
}

//刷新 个人排行->战力榜
BOOL KRankManager::InitRoleRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_ROLE_FIGHTSCORE] = tNow;

	m_nRoleRanksNum = 0;
	memset(&m_arrRoleRanks, 0, sizeof(m_arrRoleRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapRoleOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapRoleRanks.begin(); it != m_mapRoleRanks.end(); ++it)
	{
		m_mapRoleOldRanks[it->first] = it->second;
	}

	m_mapRoleFsModifyTime.clear();

	for(INT i = 0; i < emRANK_MAX_COUNT; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_ROLE_FIGHTSCORE, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

//刷新 个人排行->战力榜 操作返回
BOOL KRankManager::OnGetRoleRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime /* = 0 */)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapRoleRanks[dwRoleId] = nRankIdx;
	m_mapRoleFsModifyTime[dwRoleId] = dwLastModifyTime;
	++m_nRoleRanksNum;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_ROLE_FIGHTSCORE, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadRoleRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId)
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRoleRankInfo* pRankInfo = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	std::map<INT, INT>::iterator it = m_mapRoleRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapRoleRanks.end());
	
	nRankIdx = it->second;
	pRankInfo = &m_arrRoleRanks[nRankIdx];

	pRankInfo->byLevel = pDelayData->sBaseRoleData.byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;

	
	strncpy(pRankInfo->szRoleName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sBaseRoleData.dwFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwExValue = 0;
	pRankInfo->dwLastModifyTime = m_mapRoleFsModifyTime[dwRoleId];

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;
	//pRankInfo->sex = pDelayData->sBaseRoleData.bySex;

	it = m_mapRoleOldRanks.find(dwRoleId);
	if(it != m_mapRoleOldRanks.end() && it->second < nRankIdx)
	{
		pRankInfo->byUporDown = emRANK_COMP_DOWN;
		pRankInfo->byUporDownCount = nRankIdx - it->second;
	}
	else if(it == m_mapRoleOldRanks.end() || (it != m_mapRoleOldRanks.end() && it->second > nRankIdx))
	{
		pRankInfo->byUporDown = emRANK_COMP_UP;
		if(it != m_mapRoleOldRanks.end())
		{
			pRankInfo->byUporDownCount = it->second - nRankIdx;
		}
	}

	if(m_mapRoleOldRanks.size() <= 0)
	{
		pRankInfo->byUporDown = emRANK_COMP_NONE;
	}

	cSafeScript->CallTableFunction("Rank", "GiveRoleRankAward", 0, "dd", dwRoleId, nRankIdx);

	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId)
{
	BOOL bResult = FALSE;
	KDelayRoleData sDelayData;

	memset(&sDelayData, 0, sizeof(KDelayRoleData));
	memcpy(&sDelayData, piBuffer->GetData(), piBuffer->GetSize());
	switch(dwSrcId)
	{
	case emRANK_ROLE_FIGHTSCORE:
		OnDelayLoadRoleRankRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_PET_LEVEL:
		OnDelayLoadPetLvRankRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_PET_FIGHTSCORE:
		OnDelayLoadPetFSRankRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_RIDE_FIGHTSCORE:
		OnDelayLoadRideRankRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_MPJJ:
		OnDelayLoadMpjjRankRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_LG:
		OnDelayLoadLGRankRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_SMZC:
		OnDelayLoadSmzcRankRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_ROLE_LEVEL_OPEN_SERVER:
		OnDelayLoadPlayerLevelRankRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_RIDE_LV:
		OnDelayLoadRideLVRanksSaveRanksRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_AREAN_OS:
		OnDelayLoadAreanSaveRanksRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_PET_LV_OS:
		OnDelayLoadPetLVSaveRanksRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_LG_OS:
		OnDelayLoadLGSaveRanksRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_TOWER:
		OnDelayLoadTowerRankRespond(&sDelayData, dwRoleId);
		break;
	case emRANK_ROLE_FIGSHTSCORE_OS:
		OnDelayLoadRoleFightSaveRanksRespond(&sDelayData, dwRoleId);
		break;
	default:
		break;
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::InitPetLvRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_PET_LEVEL] = tNow;

	m_nPetLvRanksNum = 0;
	memset(&m_arrPetLvRanks, 0, sizeof(m_arrPetLvRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapPetLvOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapPetLvRanks.begin(); it != m_mapPetLvRanks.end(); ++it)
	{
		m_mapPetLvOldRanks[it->first] = it->second;
	}

	m_mapPetLvModifyTime.clear();

	for(INT i = 0; i < emRANK_MAX_COUNT; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_PET_LEVEL, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnGetPetLvRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime /* = 0 */)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapPetLvRanks[dwRoleId] = nRankIdx;
	m_mapPetLvModifyTime[dwRoleId] = dwLastModifyTime;

	++m_nPetLvRanksNum;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_PET_LEVEL, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadPetLvRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId)
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KPetRankInfo* pRankInfo = NULL;
	INT nPetIdx = 0;
	INT nMaxPetLevel = 0;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	std::map<INT, INT>::iterator it = m_mapPetLvRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapPetLvRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrPetLvRanks[nRankIdx];

	// 从离线数据里找到等级最大的神仙
	for(INT i = 0; i < 7; ++i)
	{
		if(pDelayData->aryPets[i].byLevel > nMaxPetLevel)
		{
			nMaxPetLevel = pDelayData->aryPets[i].byLevel;
			nPetIdx = i;
		}
	}
	pRankInfo->nQuality = pDelayData->aryPets[nPetIdx].nQuality;
	pRankInfo->byLevel = pDelayData->aryPets[nPetIdx].byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;
	
	strncpy(pRankInfo->szOwnerName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	strncpy(pRankInfo->szPetName, pDelayData->aryPets[nPetIdx].szPetName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->aryPets[nPetIdx].nFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwLastModifyTime = m_mapPetLvModifyTime[dwRoleId];

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;


	it = m_mapPetLvOldRanks.find(dwRoleId);
	if(it != m_mapPetLvOldRanks.end() && it->second < nRankIdx)
	{
		pRankInfo->byUporDown = emRANK_COMP_DOWN;
		pRankInfo->byUporDownCount = nRankIdx - it->second;
	}
	else if(it == m_mapPetLvOldRanks.end() || (it != m_mapPetLvOldRanks.end() && it->second > nRankIdx))
	{
		pRankInfo->byUporDown = emRANK_COMP_UP;
		if(it != m_mapPetLvOldRanks.end())
		{
			pRankInfo->byUporDownCount = it->second - nRankIdx;
		}
	}

	if(m_mapPetLvOldRanks.size() <= 0)
	{
		pRankInfo->byUporDown = emRANK_COMP_NONE;
	}

	cSafeScript->CallTableFunction("Rank", "GivePetLvRankAward", 0, "dd", dwRoleId, nRankIdx);

	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::InitPetFSRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_PET_FIGHTSCORE] = tNow;

	m_nPetFSRanksNum = 0;
	memset(&m_arrPetFSRanks, 0, sizeof(m_arrPetFSRanks));

	m_mapPetFSOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapPetFSRanks.begin(); it != m_mapPetFSRanks.end(); ++it)
	{
		m_mapPetFSOldRanks[it->first] = it->second;
	}

	m_mapPetFSModifyTime.clear();
	for(INT i = 0; i < emRANK_MAX_COUNT; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_PET_FIGHTSCORE, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnGetPetFSRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime /* = 0 */)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapPetFSRanks[dwRoleId] = nRankIdx;
	m_mapPetFSModifyTime[dwRoleId] = dwLastModifyTime;

	++m_nPetFSRanksNum;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_PET_FIGHTSCORE, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadPetFSRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId)
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KPetRankInfo* pRankInfo = NULL;
	INT nPetIdx = 0;
	INT nMaxFightScore = 0;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	std::map<INT, INT>::iterator it = m_mapPetFSRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapPetFSRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrPetFSRanks[nRankIdx];

	// 从离线数据里找到战斗力最大的神仙
	for(INT i = 0; i < 7; ++i)
	{
		if(pDelayData->aryPets[i].nFightScore > nMaxFightScore)
		{
			nMaxFightScore = pDelayData->aryPets[i].nFightScore;
			nPetIdx = i;
		}
	}

	pRankInfo->byLevel = pDelayData->aryPets[nPetIdx].byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;

	strncpy(pRankInfo->szOwnerName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	strncpy(pRankInfo->szPetName, pDelayData->aryPets[nPetIdx].szPetName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->aryPets[nPetIdx].nFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwLastModifyTime = m_mapPetFSModifyTime[dwRoleId];

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;

	it = m_mapPetFSOldRanks.find(dwRoleId);
	if(it != m_mapPetFSOldRanks.end() && it->second < nRankIdx)
	{
		pRankInfo->byUporDown = emRANK_COMP_DOWN;
		pRankInfo->byUporDownCount = nRankIdx - it->second;
	}
	else if(it == m_mapPetFSOldRanks.end() || (it != m_mapPetFSOldRanks.end() && it->second > nRankIdx))
	{
		pRankInfo->byUporDown = emRANK_COMP_UP;
		if(it != m_mapPetFSOldRanks.end())
		{
			pRankInfo->byUporDownCount = it->second - nRankIdx;
		}
	}

	if(m_mapPetFSOldRanks.size() <= 0)
	{
		pRankInfo->byUporDown = emRANK_COMP_NONE;
	}

	cSafeScript->CallTableFunction("Rank", "GivePetFSRankAward", 0, "dd", dwRoleId, nRankIdx);

	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::InitRideRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_RIDE_FIGHTSCORE] = tNow;

	m_nRideRanksNum = 0;
	memset(&m_arrRideRanks, 0, sizeof(m_arrRideRanks));

	m_mapRideOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapRideRanks.begin(); it != m_mapRideRanks.end(); ++it)
	{
		m_mapRideOldRanks[it->first] = it->second;
	}

	m_mapRideFSModifyTime.clear();
	for(INT i = 0; i < emRANK_MAX_COUNT; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_RIDE_FIGHTSCORE, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnGetRideRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime /* = 0 */)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapRideRanks[dwRoleId] = nRankIdx;
	m_mapRideFSModifyTime[dwRoleId] = dwLastModifyTime;

	++m_nRideRanksNum;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_RIDE_FIGHTSCORE, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadRideRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId)
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRideRankInfo* pRankInfo = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	std::map<INT, INT>::iterator it = m_mapRideRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapRideRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrRideRanks[nRankIdx];

	pRankInfo->byLevel = pDelayData->sRideData.byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;
	
	strncpy(pRankInfo->szOwnerName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sRideData.nFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwLastModifyTime = m_mapRideFSModifyTime[dwRoleId];

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;

	//pRankInfo->sex = pDelayData->sBaseRoleData.bySex;
	it = m_mapRideOldRanks.find(dwRoleId);
	if(it != m_mapRideOldRanks.end() && it->second < nRankIdx)
	{
		pRankInfo->byUporDown = emRANK_COMP_DOWN;
		pRankInfo->byUporDownCount = nRankIdx - it->second;
	}
	else if(it == m_mapRideOldRanks.end() || (it != m_mapRideOldRanks.end() && it->second > nRankIdx))
	{
		pRankInfo->byUporDown = emRANK_COMP_UP;
		if(it != m_mapRideOldRanks.end())
		{
			pRankInfo->byUporDownCount = it->second - nRankIdx;
		}
	}

	if(m_mapRideOldRanks.size() <= 0)
	{
		pRankInfo->byUporDown = emRANK_COMP_NONE;
	}

	cSafeScript->CallTableFunction("Rank", "GiveRideRankAward", 0, "dd", dwRoleId, nRankIdx);

	bResult = TRUE;

EXIT0:
	return bResult;
}


BOOL KRankManager::InitMpjjRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_MPJJ] = tNow;

	m_mapMpjjValues.clear();

	m_nMpjjRanksNum = 0;
	memset(&m_arrMpjjRanks, 0, sizeof(m_arrMpjjRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapMpjjOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapMpjjRanks.begin(); it != m_mapMpjjRanks.end(); ++it)
	{
		m_mapMpjjOldRanks[it->first] = it->second;
	}

	for(INT i = 0; i < emRANK_MAX_COUNT; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_MPJJ, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::InitLGRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_LG] = tNow;

	m_mapLGRanks.clear();

	m_nLGRanksNum = 0;
	memset(&m_arrLGRanks, 0, sizeof(m_arrLGRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapLGOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapLGRanks.begin(); it != m_mapLGRanks.end(); ++it)
	{
		m_mapLGOldRanks[it->first] = it->second;
	}

	for(INT i = 0; i < emRANK_MAX_COUNT; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_LG, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::InitSmzcRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_SMZC] = tNow;

	m_mapSmzcValues.clear();

	m_nSmzcRanksNum = 0;
	memset(&m_arrSmzcRanks, 0, sizeof(m_arrSmzcRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapSmzcOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapSmzcRanks.begin(); it != m_mapSmzcRanks.end(); ++it)
	{
		m_mapSmzcOldRanks[it->first] = it->second;
	}

	for(INT i = 0; i < emRANK_MAX_COUNT; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_SMZC, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

//  排名返回 玩家 ID, ongetrankid返回
BOOL KRankManager::OnGetMpjjRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwRankValue)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapMpjjRanks[dwRoleId] = nRankIdx;

	++m_nMpjjRanksNum;

	// 记录积分
	m_mapMpjjValues[dwRoleId] = dwRankValue;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_MPJJ, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadMpjjRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId)
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRoleRankInfo* pRankInfo = NULL;
	std::map<INT, DWORD>::iterator it_value;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	std::map<INT, INT>::iterator it = m_mapMpjjRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapMpjjRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrMpjjRanks[nRankIdx];
	pRankInfo->byLevel = pDelayData->sBaseRoleData.byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;

	
	strncpy(pRankInfo->szRoleName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sBaseRoleData.dwFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);

	it_value = m_mapMpjjValues.find(dwRoleId);
	LOG_PROCESS_ERROR(it_value != m_mapMpjjValues.end());

	pRankInfo->dwExValue = it_value->second;

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;

	it = m_mapMpjjOldRanks.find(dwRoleId);
	if(it != m_mapMpjjOldRanks.end() && it->second < nRankIdx)
	{
		pRankInfo->byUporDown = emRANK_COMP_DOWN;
		pRankInfo->byUporDownCount = nRankIdx - it->second;
	}
	else if(it == m_mapMpjjOldRanks.end() || (it != m_mapMpjjOldRanks.end() && it->second > nRankIdx))
	{
		pRankInfo->byUporDown = emRANK_COMP_UP;
		if(it != m_mapMpjjOldRanks.end())
		{
			pRankInfo->byUporDownCount = it->second - nRankIdx;
		}
	}

	if(m_mapMpjjOldRanks.size() <= 0)
	{
		pRankInfo->byUporDown = emRANK_COMP_NONE;
	}

	cSafeScript->CallTableFunction("Rank", "GiveMpjjRankAward", 0, "dd", dwRoleId, nRankIdx);

	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::OnGetLGRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwRankValue)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapLGRanks[dwRoleId] = nRankIdx;

	++m_nLGRanksNum;

	// 记录积分
	m_mapLGValues[dwRoleId] = dwRankValue;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_LG, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadLGRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId)
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRoleRankInfo* pRankInfo = NULL;
	std::map<INT, DWORD>::iterator it_value;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	std::map<INT, INT>::iterator it = m_mapLGRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapLGRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrLGRanks[nRankIdx];

	pRankInfo->byLevel = pDelayData->sBaseRoleData.byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;
	
	strncpy(pRankInfo->szRoleName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sBaseRoleData.dwFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);

	it_value = m_mapLGValues.find(dwRoleId);
	LOG_PROCESS_ERROR(it_value != m_mapLGValues.end());

	pRankInfo->dwExValue = it_value->second;

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;

	it = m_mapLGOldRanks.find(dwRoleId);
	if(it != m_mapLGOldRanks.end() && it->second < nRankIdx)
	{
		pRankInfo->byUporDown = emRANK_COMP_DOWN;
		pRankInfo->byUporDownCount = nRankIdx - it->second;
	}
	else if(it == m_mapLGOldRanks.end() || (it != m_mapLGOldRanks.end() && it->second > nRankIdx))
	{
		pRankInfo->byUporDown = emRANK_COMP_UP;
		if(it != m_mapLGOldRanks.end())
		{
			pRankInfo->byUporDownCount = it->second - nRankIdx;
		}
	}

	if(m_mapLGOldRanks.size() <= 0)
	{
		pRankInfo->byUporDown = emRANK_COMP_NONE;
	}

	cSafeScript->CallTableFunction("Rank", "GiveLGRankAward", 0, "dd", dwRoleId, nRankIdx);

	bResult = TRUE;

EXIT0:
	return bResult;
}

//  排名返回 玩家 ID, ongetrankid返回
BOOL KRankManager::OnGetSmzcRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwRankValue)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapSmzcRanks[dwRoleId] = nRankIdx;

	++m_nSmzcRanksNum;

	// 记录积分
	m_mapSmzcValues[dwRoleId] = dwRankValue;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_SMZC, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadSmzcRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId)
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRoleRankInfo* pRankInfo = NULL;
	std::map<INT, DWORD>::iterator it_value;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	std::map<INT, INT>::iterator it = m_mapSmzcRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapSmzcRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrSmzcRanks[nRankIdx];

	pRankInfo->byLevel = pDelayData->sBaseRoleData.byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;
	strncpy(pRankInfo->szRoleName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sBaseRoleData.dwFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);

	it_value = m_mapSmzcValues.find(dwRoleId);
	LOG_PROCESS_ERROR(it_value != m_mapSmzcValues.end());

	pRankInfo->dwExValue = it_value->second;

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;

	it = m_mapSmzcOldRanks.find(dwRoleId);
	if(it != m_mapSmzcOldRanks.end() && it->second < nRankIdx)
	{
		pRankInfo->byUporDown = emRANK_COMP_DOWN;
		pRankInfo->byUporDownCount = nRankIdx - it->second;
	}
	else if(it == m_mapSmzcOldRanks.end() || (it != m_mapSmzcOldRanks.end() && it->second > nRankIdx))
	{
		pRankInfo->byUporDown = emRANK_COMP_UP;
		if(it != m_mapSmzcOldRanks.end())
		{
			pRankInfo->byUporDownCount = it->second - nRankIdx;
		}
	}

	if(m_mapSmzcOldRanks.size() <= 0)
	{
		pRankInfo->byUporDown = emRANK_COMP_NONE;
	}

	cSafeScript->CallTableFunction("Rank", "GiveSmzcRankAward", 0, "dd", dwRoleId, nRankIdx);

	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::GetPlayerRank(INT nRankId, DWORD dwRoleId)
{
	INT nRank = -1;
	std::map<INT, INT>::iterator it;
	KPlayer* pPlayer = NULL;
	LOG_PROCESS_ERROR(nRankId >= emRANK_ARENA && nRankId <= emRANK_ROLE_FIGSHTSCORE_OS);

	switch(nRankId)
	{
	case emRANK_ARENA:
		pPlayer = g_cPlayerMgr.GetById(dwRoleId);
		LOG_PROCESS_ERROR(pPlayer);
		nRank = pPlayer->GetArenaRank() - 1;
		break;
	case emRANK_ROLE_FIGHTSCORE:
		it = m_mapRoleRanks.find(dwRoleId);
		if(it != m_mapRoleRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_PET_LEVEL:
		it = m_mapPetLvRanks.find(dwRoleId);
		if(it != m_mapPetLvRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_PET_FIGHTSCORE:
		it = m_mapPetFSRanks.find(dwRoleId);
		if(it != m_mapPetFSRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_RIDE_FIGHTSCORE:
		it = m_mapRideRanks.find(dwRoleId);
		if(it != m_mapRideRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_MPJJ:
		it = m_mapMpjjRanks.find(dwRoleId);
		if(it != m_mapMpjjRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_LG:
		it = m_mapLGRanks.find(dwRoleId);
		if(it != m_mapLGRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_SMZC:
		it = m_mapSmzcRanks.find(dwRoleId);
		if(it != m_mapSmzcRanks.end())
		{
			nRank = it->second;
		}
		break;

	case emRANK_ROLE_LEVEL_OPEN_SERVER:
		it = m_mapRoleLevelRanks.find(dwRoleId);
		if(it != m_mapRoleLevelRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_RIDE_LV:
		it = m_mapRideLvRanks.find(dwRoleId);
		if(it != m_mapRideLvRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_AREAN_OS:
		it = m_mapAreanSaveRanks.find(dwRoleId);
		if(it != m_mapAreanSaveRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_PET_LV_OS:
		it = m_mapPetLVSaveRanks.find(dwRoleId);
		if(it != m_mapPetLVSaveRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_LG_OS:
		it = m_mapLGSaveRanks.find(dwRoleId);
		if(it != m_mapLGSaveRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_TOWER:
		it = m_mapTowerRanks.find(dwRoleId);
		if(it != m_mapTowerRanks.end())
		{
			nRank = it->second;
		}
		break;	
	case emRANK_ROLE_FIGSHTSCORE_OS:
		it = m_mapRoleFightSaveRanks.find(dwRoleId);
		if(it != m_mapRoleFightSaveRanks.end())
		{
			nRank = it->second;
		}
		break;
	default:
		break;
	}

EXIT0:
	return nRank;
}

BOOL KRankManager::GetTongRank(INT nRankId, DWORD dwTongId)
{
	INT nRank = -1;
	std::map<INT, INT>::iterator it;
	LOG_PROCESS_ERROR(nRankId >= emRANK_TONG_FIGHTSCORE && nRankId <= emRANK_TONG_SALARY);

	switch(nRankId)
	{
	case emRANK_TONG_FIGHTSCORE:
		it = m_mapTongFSRanks.find(dwTongId);
		if(it != m_mapTongFSRanks.end())
		{
			nRank = it->second;
		}
		break;
	case emRANK_TONG_SALARY:
		it = m_mapTongSrRanks.find(dwTongId);
		if(it != m_mapTongSrRanks.end())
		{
			nRank = it->second;
		}
		break;
	default:
		break;
	}

EXIT0:
	return nRank;
}

DWORD KRankManager::GetLastUpdateTime(INT nRankId)
{
	DWORD dwTime = 0;
	std::map<INT, DWORD>::iterator it;

	it = m_mapLastUpdateTime.find(nRankId);
	PROCESS_ERROR(it != m_mapLastUpdateTime.end());

	dwTime = it->second;
EXIT0:
	return dwTime;
}


BOOL KRankManager::InitTongFSRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_TONG_FIGHTSCORE] = tNow;

	m_mapTongFSValues.clear();

	m_nTongFSRanksNum = 0;
	memset(&m_arrTongFSRanks, 0, sizeof(m_arrTongFSRanks));

	m_mapTongFSOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapTongFSRanks.begin(); it != m_mapTongFSRanks.end(); ++it)
	{
		m_mapTongFSOldRanks[it->first] = it->second;
	}

	for(INT i = 0; i < emRANK_MAX_COUNT; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_TONG_FIGHTSCORE, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnGetTongFSIdRespond(DWORD dwTongId, INT nRankIdx, DWORD dwRankValue)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwTongId > 0);

	// 记录排名
	m_mapTongFSRanks[dwTongId] = nRankIdx;
	m_mapTongFSValues[dwTongId] = dwRankValue;

	++m_nTongFSRanksNum;

	bRetCode = KinRelayClient::DoGetTongRankInfoRequest(dwTongId, emRANK_TONG_FIGHTSCORE);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnGetTongFsRankInfoRespond(DWORD dwTongId, INT nTongMemberNum, LPCTSTR szTongName, LPCTSTR szMasterName)
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KTongRankInfo* pRankInfo = NULL;
	std::map<INT, DWORD>::iterator it_value;

	std::map<INT, INT>::iterator it = m_mapTongFSRanks.find(dwTongId);
	LOG_PROCESS_ERROR(it != m_mapTongFSRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrTongFSRanks[nRankIdx];

	pRankInfo->nMemberNum = nTongMemberNum;
	strncpy(pRankInfo->szTongName, szTongName, MAX_NAME_LEN);
	strncpy(pRankInfo->szMasterName, szMasterName, MAX_NAME_LEN);

	it_value = m_mapTongFSValues.find(dwTongId);
	LOG_PROCESS_ERROR(it_value != m_mapTongFSValues.end());

	pRankInfo->dwFightScore = it_value->second;

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;


	it = m_mapTongFSOldRanks.find(dwTongId);
	if(it != m_mapTongFSOldRanks.end() && it->second < nRankIdx)
	{
		pRankInfo->byUporDown = emRANK_COMP_DOWN;
		pRankInfo->byUporDownCount = nRankIdx - it->second;
	}
	else if(it == m_mapTongFSOldRanks.end() || (it != m_mapTongFSOldRanks.end() && it->second > nRankIdx))
	{
		pRankInfo->byUporDown = emRANK_COMP_UP;
		if(it != m_mapTongFSOldRanks.end())
		{
			pRankInfo->byUporDownCount = it->second - nRankIdx;
		}
	}

	if(m_mapTongFSOldRanks.size() <= 0)
	{
		pRankInfo->byUporDown = emRANK_COMP_NONE;
	}

	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::InitTongSrRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_TONG_SALARY] = tNow;

	m_mapTongSrValues.clear();

	m_nTongSrRanksNum = 0;
	memset(&m_arrTongSrRanks, 0, sizeof(m_arrTongSrRanks));

	m_mapTongSrOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapTongSrRanks.begin(); it != m_mapTongSrRanks.end(); ++it)
	{
		m_mapTongSrOldRanks[it->first] = it->second;
	}

	for(INT i = 0; i < emRANK_MAX_COUNT; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_TONG_SALARY, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnGetTongSrIdRespond(DWORD dwTongId, INT nRankIdx, DWORD dwRankValue)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwTongId > 0);

	// 记录排名
	m_mapTongSrRanks[dwTongId] = nRankIdx;

	++m_nTongSrRanksNum;

	m_mapTongSrValues[dwTongId] = dwRankValue;

	bRetCode = KinRelayClient::DoGetTongRankInfoRequest(dwTongId, emRANK_TONG_SALARY);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnGetTongSrRankInfoRespond(DWORD dwTongId, INT nTongMemberNum, LPCTSTR szTongName, LPCTSTR szMasterName)
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KTongRankInfo* pRankInfo = NULL;
	std::map<INT, DWORD>::iterator it_value;

	std::map<INT, INT>::iterator it = m_mapTongSrRanks.find(dwTongId);
	LOG_PROCESS_ERROR(it != m_mapTongSrRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrTongSrRanks[nRankIdx];

	pRankInfo->nMemberNum = nTongMemberNum;
	strncpy(pRankInfo->szTongName, szTongName, MAX_NAME_LEN);
	strncpy(pRankInfo->szMasterName, szMasterName, MAX_NAME_LEN);

	it_value = m_mapTongSrValues.find(dwTongId);
	LOG_PROCESS_ERROR(it_value != m_mapTongSrValues.end());

	pRankInfo->dwSalary = it_value->second;

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;


	
	it = m_mapTongSrOldRanks.find(dwTongId);
	if(it != m_mapTongSrOldRanks.end() && it->second < nRankIdx)
	{
		pRankInfo->byUporDown = emRANK_COMP_DOWN;
		pRankInfo->byUporDownCount = nRankIdx - it->second;
	}
	else if(it == m_mapTongSrOldRanks.end() || (it != m_mapTongSrOldRanks.end() && it->second > nRankIdx))
	{
		pRankInfo->byUporDown = emRANK_COMP_UP;
		if(it != m_mapTongSrOldRanks.end())
		{
			pRankInfo->byUporDownCount = it->second - nRankIdx;
		}
	}

	if(m_mapTongSrOldRanks.size() <= 0)
	{
		pRankInfo->byUporDown = emRANK_COMP_NONE;
	}

	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::InitPlayerLevel()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_ROLE_LEVEL_OPEN_SERVER] = tNow;

	m_nRoleLevelRanksNum = 0;
	memset(&m_arrRoleLevelRanks, 0, sizeof(m_arrRoleLevelRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapRoleLevelOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapRoleLevelRanks.begin(); it != m_mapRoleLevelRanks.end(); ++it)
	{
		m_mapRoleLevelOldRanks[it->first] = it->second;
	}

	m_mapRoleLevelModifyTime.clear();

	for(INT i = 0; i < emRANK_MAX_COUNT_OPEN_SERVICE; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_ROLE_LEVEL_OPEN_SERVER, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}




BOOL KRankManager::InitRideLvRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_RIDE_LV] = tNow;

	m_nRideLvRanksNum= 0;
	memset(&m_arrRideLvRanks, 0, sizeof(m_arrRideLvRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapRideLvOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapRideLvOldRanks.begin(); it != m_mapRideLvOldRanks.end(); ++it)
	{
		m_mapRideLvOldRanks[it->first] = it->second;
	}

	m_mapRideLvRanksModifyTime.clear();

	for(INT i = 0; i < emRANK_MAX_COUNT_OPEN_SERVICE; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_RIDE_LV, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}
	
	bResult = TRUE;
EXIT0:
	return bResult;

}

BOOL KRankManager::InitArenaRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_AREAN_OS] = tNow;

	m_nAreanRanksNum= 0;
	memset(&m_arrAreanSaveRanks, 0, sizeof(m_arrAreanSaveRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapAreanSaveOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapAreanSaveOldRanks.begin(); it != m_mapAreanSaveOldRanks.end(); ++it)
	{
		m_mapAreanSaveOldRanks[it->first] = it->second;
	}

	m_mapAreanSaveModifyTime.clear();

	for(INT i = 0; i < emRANK_MAX_COUNT_OPEN_SERVICE; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_AREAN_OS, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}
	
	bResult = TRUE;
EXIT0:
	return bResult;

}


BOOL KRankManager::InitPetLvSaveRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_PET_LV_OS] = tNow;

	m_nPetLvNum= 0;
	memset(&m_arrPetLVSaveRanks, 0, sizeof(m_arrPetLVSaveRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapPetLVSaveOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapPetLVSaveOldRanks.begin(); it != m_mapPetLVSaveOldRanks.end(); ++it)
	{
		m_mapPetLVSaveOldRanks[it->first] = it->second;
	}

	m_mapPetLVSaveModifyTime.clear();

	for(INT i = 0; i < emRANK_MAX_COUNT_OPEN_SERVICE; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_PET_LV_OS, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}
	
	bResult = TRUE;
EXIT0:
	return bResult;

}



BOOL KRankManager::InitLGLvRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_LG_OS] = tNow;

	m_nLGNum= 0;
	memset(&m_arrLGSaveRanks, 0, sizeof(m_arrLGSaveRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapLGSaveOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapLGSaveOldRanks.begin(); it != m_mapLGSaveOldRanks.end(); ++it)
	{
		m_mapLGSaveOldRanks[it->first] = it->second;
	}

	m_mapLGSaveModifyTime.clear();

	for(INT i = 0; i < emRANK_MAX_COUNT_OPEN_SERVICE; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_LG_OS, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}
	
	bResult = TRUE;
EXIT0:
	return bResult;

}


BOOL KRankManager::InitTowerRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_TOWER] = tNow;

	m_nTowerRanksNum= 0;
	memset(&m_arrTowerRanks, 0, sizeof(m_arrTowerRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapTowerOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapTowerOldRanks.begin(); it != m_mapTowerOldRanks.end(); ++it)
	{
		m_mapTowerOldRanks[it->first] = it->second;
	}

	m_mapTowerModifyTime.clear();

	for(INT i = 0; i < emRANK_MAX_COUNT_OPEN_SERVICE; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_TOWER, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}
	
	bResult = TRUE;
EXIT0:
	return bResult;

}


BOOL KRankManager::InitRoleFsRank()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;

	time_t tNow = KSysService::Time(NULL);
	m_mapLastUpdateTime[emRANK_ROLE_FIGSHTSCORE_OS] = tNow;

	m_nRoleFightScoreNum= 0;
	memset(&m_arrRoleFightSaveRanks, 0, sizeof(m_arrRoleFightSaveRanks));

	// 先把上次排名记录移到OldRanks里面
	m_mapRoleFightSaveOldRanks.clear();
	for(std::map<INT, INT>::iterator it = m_mapRoleFightSaveOldRanks.begin(); it != m_mapRoleFightSaveOldRanks.end(); ++it)
	{
		m_mapRoleFightSaveOldRanks[it->first] = it->second;
	}

	m_mapRoleFightSaveModifyTime.clear();

	for(INT i = 0; i < emRANK_MAX_COUNT_OPEN_SERVICE; ++i)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_ROLE_FIGSHTSCORE_OS, 0, i);
		LOG_PROCESS_ERROR(bRetCode);
	}
	
	bResult = TRUE;
EXIT0:
	return bResult;
}









BOOL KRankManager::OnGetPlayerLevelIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapRoleLevelRanks[dwRoleId] = nRankIdx;
	m_mapRoleLevelModifyTime[dwRoleId] = dwLastModifyTime;
	++m_nRoleLevelRanksNum;
	//printf("**************RoleId:%d,Size:%d\n",dwRoleId,m_mapRoleLevelRanks.size());
	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_ROLE_LEVEL_OPEN_SERVER, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;

}

BOOL KRankManager::OnGetTowerIdRespond( DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapTowerRanks[dwRoleId] = nRankIdx;
	m_mapTowerModifyTime[dwRoleId] = dwLastModifyTime;
	++m_nTowerRanksNum;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_TOWER, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;

}

BOOL KRankManager::OnDelayLoadPlayerLevelRankRespond( KDelayRoleData* pDelayData, DWORD dwRoleId )
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRoleRankInfo* pRankInfo = NULL;

	std::map<INT, INT>::iterator it = m_mapRoleLevelRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapRoleLevelRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrRoleLevelRanks[nRankIdx];
	pRankInfo->byLevel = pDelayData->sBaseRoleData.byLevel;
	strncpy(pRankInfo->szRoleName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sBaseRoleData.dwFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwExValue = 0;
	pRankInfo->dwLastModifyTime = m_mapRoleFsModifyTime[dwRoleId];

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;
	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadTowerRankRespond( KDelayRoleData* pDelayData, DWORD dwRoleId )
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRoleRankInfo* pRankInfo = NULL;

	std::map<INT, INT>::iterator it = m_mapTowerRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapTowerRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrTowerRanks[nRankIdx];

	pRankInfo->byLevel = pDelayData->sBaseRoleData.byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;
	
	strncpy(pRankInfo->szRoleName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sBaseRoleData.dwFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwExValue = m_mapTowerModifyTime[dwRoleId];
	pRankInfo->dwLastModifyTime = m_mapRoleFsModifyTime[dwRoleId];

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;

	bResult = TRUE;

EXIT0:
	return bResult;

}

BOOL KRankManager::InitOpenServerRank()
{
	BOOL bResult = FALSE;
	
	InitPlayerLevel();
	InitRideLvRank();
	InitArenaRank();	
	InitPetLvSaveRank();
	InitLGLvRank();
	InitTowerRank();
	InitRoleFsRank();
	
	bResult = TRUE;
EXIT0:
	return bResult;


}
/////////////////////////////////
BOOL KRankManager::OnetGetRideLVRanksSaveIdRespond( DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime /*= 0*/ )
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapRideLvRanks[dwRoleId] = nRankIdx;
	m_mapRideLvRanksModifyTime[dwRoleId] = dwLastModifyTime;

	++m_nRideLvRanksNum;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_RIDE_LV, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnGetAreanRanksSaveIdRespond( DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime /*= 0*/ )
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapAreanSaveRanks[dwRoleId] = nRankIdx;
	m_mapAreanSaveModifyTime[dwRoleId] = dwLastModifyTime;
	++m_nAreanRanksNum;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_AREAN_OS, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;

}

BOOL KRankManager::OnGetGetPetLVSaveIdRespond( DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime /*= 0*/ )
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapPetLVSaveRanks[dwRoleId] = nRankIdx;
	m_mapPetLVSaveModifyTime[dwRoleId] = dwLastModifyTime;
	++m_nPetLvNum;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_PET_LV_OS, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;

}

BOOL KRankManager::OnGetLGSaveIdRespond( DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime /*= 0*/ )
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapLGSaveRanks[dwRoleId] = nRankIdx;
	//m_mapLGSaveModifyTime[dwRoleId] = dwLastModifyTime;
	++m_nLGNum;

	m_mapLGSaveModifyTime[dwRoleId] = dwLastModifyTime;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_LG_OS, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;

}

BOOL KRankManager::OnGetRoleFightScoreSaveIdRespond( DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime /*= 0*/ )
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	PROCESS_ERROR(dwRoleId > 0);

	// 记录排名
	m_mapRoleFightSaveRanks[dwRoleId] = nRankIdx;
	m_mapRoleFightSaveModifyTime[dwRoleId] = dwLastModifyTime;
	++m_nRoleFightScoreNum;

	bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_RANK, emRANK_ROLE_FIGSHTSCORE_OS, dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadRideLVRanksSaveRanksRespond( KDelayRoleData* pDelayData, DWORD dwRoleId )
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRideRankInfo* pRankInfo = NULL;

	std::map<INT, INT>::iterator it = m_mapRideLvRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapRideLvRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrRideLvRanks[nRankIdx];
	
	pRankInfo->byLevel = pDelayData->sRideData.byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;
	strncpy(pRankInfo->szOwnerName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sRideData.nFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwLastModifyTime = m_mapRideLvRanksModifyTime[dwRoleId]%100;
	
	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;
	//pRankInfo->sex = pDelayData->sBaseRoleData.bySex;

	bResult = TRUE;

EXIT0:
	return bResult;

}

BOOL KRankManager::OnDelayLoadAreanSaveRanksRespond( KDelayRoleData* pDelayData, DWORD dwRoleId )
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRoleRankInfo* pRankInfo = NULL;

	std::map<INT, INT>::iterator it = m_mapAreanSaveRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapAreanSaveRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrAreanSaveRanks[nRankIdx];

	pRankInfo->byLevel = pDelayData->sBaseRoleData.byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;
	strncpy(pRankInfo->szRoleName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sBaseRoleData.dwFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwExValue = m_mapAreanSaveModifyTime[dwRoleId];
	pRankInfo->dwLastModifyTime = m_mapAreanSaveModifyTime[dwRoleId];

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;
	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadPetLVSaveRanksRespond( KDelayRoleData* pDelayData, DWORD dwRoleId )
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KPetRankInfo* pRankInfo = NULL;
	INT nPetIdx = 0;
	INT nMaxPetLevel = 0;

	std::map<INT, INT>::iterator it = m_mapPetLVSaveRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapPetLVSaveRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrPetLVSaveRanks[nRankIdx];

	// 从离线数据里找到等级最大的神仙
	for(INT i = 0; i < 7; ++i)
	{
		if(pDelayData->aryPets[i].byLevel > nMaxPetLevel)
		{
			nMaxPetLevel = pDelayData->aryPets[i].byLevel;
			nPetIdx = i;
		}
	}
	
	pRankInfo->byLevel = pDelayData->aryPets[nPetIdx].byLevel;
	pRankInfo->nQuality = pDelayData->aryPets[nPetIdx].nQuality;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;
	
	strncpy(pRankInfo->szOwnerName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	strncpy(pRankInfo->szPetName, pDelayData->aryPets[nPetIdx].szPetName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->aryPets[nPetIdx].nFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwLastModifyTime = m_mapPetLVSaveModifyTime[dwRoleId]%10000000;

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;
	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL KRankManager::OnDelayLoadLGSaveRanksRespond( KDelayRoleData* pDelayData, DWORD dwRoleId )
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRoleRankInfo* pRankInfo = NULL;

	std::map<INT, INT>::iterator it = m_mapLGSaveRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapLGSaveRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrLGSaveRanks[nRankIdx];

	pRankInfo->byLevel = pDelayData->sBaseRoleData.byLevel;
	strncpy(pRankInfo->szRoleName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sBaseRoleData.dwFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	//pRankInfo->dwExValue = 0;
	pRankInfo->dwLastModifyTime = m_mapRoleFsModifyTime[dwRoleId];
	pRankInfo->dwExValue = m_mapLGSaveModifyTime[dwRoleId];
	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;
	bResult = TRUE;

EXIT0:
	return bResult;

}

BOOL KRankManager::OnDelayLoadRoleFightSaveRanksRespond( KDelayRoleData* pDelayData, DWORD dwRoleId )
{
	INT nRankIdx = 0;
	BOOL bResult = FALSE;
	KRoleRankInfo* pRankInfo = NULL;

	std::map<INT, INT>::iterator it = m_mapRoleFightSaveRanks.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapRoleFightSaveRanks.end());

	nRankIdx = it->second;
	pRankInfo = &m_arrRoleFightSaveRanks[nRankIdx];

	pRankInfo->byLevel = pDelayData->sBaseRoleData.byLevel;
	pRankInfo->bySex = pDelayData->sBaseRoleData.bySex;

	
	strncpy(pRankInfo->szRoleName, pDelayData->sBaseRoleData.m_szRoleName, MAX_NAME_LEN);
	pRankInfo->nFightScore = pDelayData->sBaseRoleData.dwFightScore;
	strncpy(pRankInfo->szTongName, pDelayData->sBaseRoleData.m_szTongName, MAX_NAME_LEN);
	pRankInfo->dwExValue = 0;
	//pRankInfo->dwLastModifyTime = m_mapRoleFsModifyTime[dwRoleId];

	pRankInfo->byUporDown = emRANK_COMP_FAIR;
	pRankInfo->byUporDownCount = 0;
	bResult = TRUE;
EXIT0:
	return bResult;
}

//刷新开服活动排行
VOID KRankManager::InitOpenServerRanks()
{
	//InitPlayerLevel();
	//InitTower();
	InitOpenServerRank();
EXIT0:
	return;
}
