#include "stdafx.h"
#include "kescortctrl.h"
#include "onlinegameworld/kplayer.h"
#include "kescortprotocol.h"
#include "kescortmodule.h"
#include "../onlinegameworld/KRelayClient.h"
#include "onlinegameworld/korpgworld.h"
#include "kescorthelper.h"
#include "../onlinegamemodule/arena/ArenaHelper.h"
#include "pet/kpetsetting.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/knpcmgr.h"

KEscortCtrl::KEscortCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KEscortCtrl::~KEscortCtrl()
{

}

BOOL KEscortCtrl::Init()
{
	m_nCurEscortPage = 1;
	m_nLastEscortPage = 1;

	memset(m_arrRoleIdxs, 0, sizeof(m_arrRoleIdxs));

	return TRUE;
}

BOOL KEscortCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	return TRUE;
}

BOOL KEscortCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bResult = FALSE;

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::Active()
{
EXIT0:
	return TRUE;
}

VOID KEscortCtrl::ClearEscortData(BOOL bClearSelf /* = TRUE */)
{
	m_mapRole2Npcs.clear();
	m_mapRole2PetNpcs.clear();
	memset(m_arrRoleIdxs, 0, sizeof(m_arrRoleIdxs));
	m_mapRole2Idxs.clear();

	if(bClearSelf)
	{
		m_dwCurSelfNpcId = 0;
		m_dwCurSelfPetNpcId = 0;
	}
}

BOOL KEscortCtrl::GetPlayerEscort(INT nPlayerId, KPlayerEscort& sEscort)
{
	BOOL bResult = FALSE;
	std::map<DWORD, KPlayerEscort>::iterator it_escort = m_mapPlayerEscorts.find(nPlayerId);
	PROCESS_ERROR(it_escort != m_mapPlayerEscorts.end());

	sEscort = it_escort->second;

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::DoRefreshEscortNpcs2()
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	INT nStartIdx = 0;
	INT nEndIdx = 0;

	bRetCode = ClearNpcs();
	LOG_PROCESS_ERROR(bRetCode);

	LOG_PROCESS_ERROR(m_nCurEscortPage > 0);

	nStartIdx = (m_nCurEscortPage - 1) * MAX_ESCORTPAGE_PLAYER_NUM;
	nEndIdx = m_nCurEscortPage * MAX_ESCORTPAGE_PLAYER_NUM;

	LOG_PROCESS_ERROR(nEndIdx < MAX_ESCORT_POS_NUM);

	for(INT nIdx = nStartIdx; nIdx < nEndIdx; ++nIdx)
	{
		DWORD dwPlayerId = m_arrRoleIdxs[nIdx];
		if(!dwPlayerId)
		{
			continue;
		}
		g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ESCORT, m_rcPlayer.GetId(), dwPlayerId);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::DoRefreshEscortNpcs3()
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;

	KNpc* pNpc = NULL;
	KNpc* pPetNpc = NULL;
	INT nStartIdx = 0;
	INT nEndIdx = 0;
	std::map<DWORD, DWORD>::iterator it_npc;
	std::map<DWORD, DWORD>::iterator it_petnpc;

	// 先删掉上一页的npc
	nStartIdx = (m_nLastEscortPage - 1) * MAX_ESCORTPAGE_PLAYER_NUM;
	nEndIdx = m_nLastEscortPage * MAX_ESCORTPAGE_PLAYER_NUM;

	LOG_PROCESS_ERROR(nEndIdx < MAX_ESCORT_POS_NUM);

	for(INT nIdx = nStartIdx; nIdx < nEndIdx; ++nIdx)
	{
		DWORD dwPlayerId = m_arrRoleIdxs[nIdx];
		if(!dwPlayerId)
		{
			continue;
		}
		it_npc = m_mapRole2Npcs.find(dwPlayerId);
		LOG_PROCESS_ERROR(it_npc != m_mapRole2Npcs.end());

		bRetCode = RemoveEscortNpc(it_npc->first, it_npc->second);
		LOG_PROCESS_ERROR(bRetCode);
	}

	m_mapRole2Npcs.clear();

	// 加载新页面的npc
	nStartIdx = (m_nCurEscortPage - 1) * MAX_ESCORTPAGE_PLAYER_NUM;
	nEndIdx = m_nCurEscortPage * MAX_ESCORTPAGE_PLAYER_NUM;

	LOG_PROCESS_ERROR(nEndIdx < MAX_ESCORT_POS_NUM);

	for(INT nIdx = nStartIdx; nIdx < nEndIdx; ++nIdx)
	{
		DWORD dwPlayerId = m_arrRoleIdxs[nIdx];
		if(!dwPlayerId)
		{
			continue;
		}

		g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ESCORT, m_rcPlayer.GetId(), dwPlayerId);
	}

	
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::RemoveEscortNpc(DWORD dwPlayerId, DWORD dwNpcId)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	KNpc* pNpc = NULL;
	KNpc* pPetNpc = NULL;
	std::map<DWORD, DWORD>::iterator it_petnpc;

	pNpc = g_cNpcMgr.GetById(dwNpcId);
	LOG_PROCESS_ERROR(pNpc);
	bRetCode = g_cOrpgWorld.RemoveNpc(dwNpcId);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = SyncRemoveNpc(dwNpcId);
	LOG_PROCESS_ERROR(bRetCode);

	// 顺便删掉神仙npc
	it_petnpc = m_mapRole2PetNpcs.find(dwPlayerId);
	if(it_petnpc != m_mapRole2PetNpcs.end())
	{
		DWORD dwPetNpcId = it_petnpc->second;

		pNpc = g_cNpcMgr.GetById(dwPetNpcId);
		LOG_PROCESS_ERROR(pNpc);
		bRetCode = g_cOrpgWorld.RemoveNpc(dwPetNpcId);
		LOG_PROCESS_ERROR(bRetCode);

		bRetCode = SyncRemoveNpc(dwPetNpcId, TRUE);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::DoRefreshEscortNpcs()
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	std::map<DWORD, KPlayerEscort>::iterator it_escort;
	std::map<DWORD, DWORD>::iterator it_npc;
	std::map<DWORD, DWORD>::iterator it_petnpc;
	std::map<DWORD, DWORD> mapSwapRole2Npcs;
	std::map<DWORD, DWORD> mapSwapRole2PetNpcs;
	KNpc* pNpc = NULL;
	KNpc* pPetNpc = NULL;
	INT nStartIdx = 0;
	INT nEndIdx = 0;

	for(it_npc = m_mapRole2Npcs.begin(); it_npc != m_mapRole2Npcs.end(); ++it_npc)
	{
		it_escort = m_mapPlayerEscorts.find(it_npc->first);

		// 没找到，证明这个npc已经不存在了，删掉
		if(it_escort == m_mapPlayerEscorts.end())
		{
			bRetCode = RemoveEscortNpc(it_npc->first, it_npc->second);
			LOG_PROCESS_ERROR(bRetCode);
		}
		else
		{
			mapSwapRole2Npcs[it_npc->first] = it_npc->second;

			it_petnpc = m_mapRole2PetNpcs.find(it_npc->first);
			if(it_petnpc != m_mapRole2PetNpcs.end())
			{
				mapSwapRole2PetNpcs[it_petnpc->first] = it_petnpc->second;
			}
		}
	}

	nStartIdx = (m_nCurEscortPage - 1) * MAX_ESCORTPAGE_PLAYER_NUM;
	nEndIdx = m_nCurEscortPage * MAX_ESCORTPAGE_PLAYER_NUM;

	LOG_PROCESS_ERROR(nEndIdx < MAX_ESCORT_POS_NUM);

	for(INT nIdx = nStartIdx; nIdx < nEndIdx; ++nIdx)
	{
		DWORD dwPlayerId = m_arrRoleIdxs[nIdx];
		if(!dwPlayerId)
		{
			continue;
		}
		it_npc = m_mapRole2Npcs.find(dwPlayerId);

		// 没找到，说明要重新加载这个npc
		if(it_npc == m_mapRole2Npcs.end())
		{
			g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ESCORT, m_rcPlayer.GetId(), dwPlayerId);
		}
	}

	m_mapRole2Npcs.clear();

	for(std::map<DWORD, DWORD>::iterator it = mapSwapRole2Npcs.begin(); it != mapSwapRole2Npcs.end(); ++it)
	{
		m_mapRole2Npcs[it->first] = it->second;
	}

	m_mapRole2PetNpcs.clear();

	for(std::map<DWORD, DWORD>::iterator it = mapSwapRole2PetNpcs.begin(); it != mapSwapRole2PetNpcs.end(); ++it)
	{
		m_mapRole2PetNpcs[it->first] = it->second;
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

// 找到一个空的位置插入修仙玩家
BOOL KEscortCtrl::InsertEscortPlayer(INT nPlayerId)
{
	BOOL bResult = FALSE;
	INT nIdx = 0;

	for(; nIdx < MAX_ESCORT_POS_NUM; ++nIdx)
	{
		if(!m_arrRoleIdxs[nIdx])
		{
			m_arrRoleIdxs[nIdx] = nPlayerId;
			break;
		}
	}

	LOG_PROCESS_ERROR(nIdx < MAX_ESCORT_POS_NUM);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::OnSyncEscortPlayers(INT nMembersNum, KPlayerEscort* pEscort)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	std::map<DWORD, INT>::iterator it_idx;
	std::map<DWORD, KPlayerEscort>::iterator it_escort;

	m_mapPlayerEscorts.clear();

	for(INT i = 0; i < nMembersNum; ++i)
	{
		KPlayerEscort sEscort;
		memcpy(&sEscort, &(pEscort[i]), sizeof(KPlayerEscort));

		m_mapPlayerEscorts[sEscort.nPlayerId] = sEscort;
	}

	// 重新排位置
	for(it_idx = m_mapRole2Idxs.begin(); it_idx != m_mapRole2Idxs.end(); ++it_idx)
	{
		it_escort = m_mapPlayerEscorts.find(it_idx->first);
		if(it_escort == m_mapPlayerEscorts.end())
		{
			m_arrRoleIdxs[it_idx->second] = 0;
		}
	}

	for(it_escort = m_mapPlayerEscorts.begin(); it_escort != m_mapPlayerEscorts.end(); ++it_escort)
	{
		it_idx = m_mapRole2Idxs.find(it_escort->first);
		if(it_idx == m_mapRole2Idxs.end())
		{
			bRetCode = InsertEscortPlayer(it_escort->first);
			LOG_PROCESS_ERROR(bRetCode);
		}
	}

	m_mapRole2Idxs.clear();
	for(INT i = 0; i < MAX_ESCORT_POS_NUM; ++i)
	{
		if(m_arrRoleIdxs[i])
		{
			m_mapRole2Idxs[m_arrRoleIdxs[i]] = i;
		}
	}

	// 设置本人修仙状态
	{
		std::map<DWORD, KPlayerEscort>::iterator it = m_mapPlayerEscorts.find(m_rcPlayer.GetId());
		if(it != m_mapPlayerEscorts.end())
		{
			m_rcPlayer.SetEscortState(it->second.byEscortState);
		}
		else
		{
			m_rcPlayer.SetEscortState(emESCORT_STATE_NONE);
		}

		if(!m_dwCurSelfNpcId)
		{
			g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ESCORT, m_rcPlayer.GetId(), m_rcPlayer.GetId());
		}
	}

	bRetCode = DoSyncEscortMembers();
	LOG_PROCESS_ERROR(bRetCode);

	if(m_nCurEscortPage == m_nLastEscortPage)
	{
		bRetCode = DoRefreshEscortNpcs();
		LOG_PROCESS_ERROR(bRetCode);
	}
	else
	{
		bRetCode = DoRefreshEscortNpcs3();
		LOG_PROCESS_ERROR(bRetCode);

		m_nLastEscortPage = m_nCurEscortPage;
	}
	

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::CreateSelfNpc(KDelayRoleData* pDelayData)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	KNpc* pNpc = NULL;
	KNpc* pPetNpc = NULL;
	KDelayPetData* pPetData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	pNpc =  g_cOrpgWorld.AddNpc(PLAYER_NPC_TEMPLATE_ID);
	LOG_PROCESS_ERROR(pNpc);
	bRetCode = g_cEscortHelper.SetNpcWithDelayData(pNpc, pDelayData);
	LOG_PROCESS_ERROR(bRetCode);

	m_dwCurSelfNpcId = pNpc->GetId();

	pPetData = g_cArenaHelper.GetFightPetData(pDelayData);

	// 有宠物，添加宠物
	if (pPetData && pPetData->wType > 0)
	{
		DWORD dwNpcTemplateId = (DWORD)g_cPetSetting.GetPetNpcIdByType(pPetData->wType);
		pPetNpc = g_cOrpgWorld.AddNpc(dwNpcTemplateId);  // 根据NPC
		LOG_PROCESS_ERROR(pPetNpc);
		m_dwCurSelfPetNpcId = pPetNpc->GetId();
	}

	bRetCode = SyncAddSelfNpc(m_rcPlayer.GetId(), pNpc->GetId(), pPetNpc != NULL ? pPetNpc->GetId() : 0);  // 0时不能fight
	LOG_PROCESS_ERROR(bRetCode);

	cSafeScript->CallTableFunction("Escort", "OnLoadSelfPlayerComplete", 0, "ooo", m_rcPlayer.GetScriptInterface(), pNpc->GetScriptInterface(), 
		pPetNpc != NULL ? pPetNpc->GetScriptInterface() : NULL);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::OnLoadDelayRoleDataRespond(DWORD dwSrcId, DWORD dwRoleId, KDelayRoleData* pDelayData)
{
	if(m_rcPlayer.GetId() == dwRoleId && !m_dwCurSelfNpcId)
	{
		CreateSelfNpc(pDelayData);
	}
	else
	{
		CreateEscortNpc(dwRoleId, pDelayData);
	}
	
	return TRUE;
}

BOOL KEscortCtrl::CreateEscortNpc(DWORD dwRoleId, KDelayRoleData* pDelayData)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	KNpc* pNpc = NULL;
	KNpc* pPetNpc = NULL;
	KDelayPetData* pPetData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	std::map<DWORD, INT>::iterator it;
	INT nStandIdx = 0;

	pNpc =  g_cOrpgWorld.AddNpc(PLAYER_NPC_TEMPLATE_ID);
	LOG_PROCESS_ERROR(pNpc);
	bRetCode = g_cEscortHelper.SetNpcWithDelayData(pNpc, pDelayData);
	LOG_PROCESS_ERROR(bRetCode);

	m_mapRole2Npcs[dwRoleId] = pNpc->GetId();

	pPetData = g_cArenaHelper.GetFightPetData(pDelayData);

	// 有宠物，添加宠物
	if (pPetData && pPetData->wType > 0)
	{
		DWORD dwNpcTemplateId = (DWORD)g_cPetSetting.GetPetNpcIdByType(pPetData->wType);
		pPetNpc = g_cOrpgWorld.AddNpc(dwNpcTemplateId);  // 根据NPC
		LOG_PROCESS_ERROR(pPetNpc);

		m_mapRole2PetNpcs[dwRoleId] = pPetNpc->GetId();
	}

	it = m_mapRole2Idxs.find(dwRoleId);

	if(dwRoleId == m_dwCurFightPlayerId && it == m_mapRole2Idxs.end())
	{
		DWORD dwMeNpcId = GetNpcIdByRoleId(m_rcPlayer.GetId());
		DWORD dwMePetNpcId = GetPetNpcIdByRoleId(m_rcPlayer.GetId());

		cSafeScript->CallTableFunction("Escort", "OnLoadRobPlayerComplete", 0, "oooddd", m_rcPlayer.GetScriptInterface(), pNpc->GetScriptInterface(), 
			pPetNpc != NULL ? pPetNpc->GetScriptInterface() : NULL, m_dwCurFightPlayerId, dwMeNpcId, dwMePetNpcId);

		m_dwCurFightPlayerId = 0;

		goto EXIT0;
	}

	LOG_PROCESS_ERROR(it != m_mapRole2Idxs.end());
	nStandIdx = it->second % MAX_ESCORTPAGE_PLAYER_NUM + 1;

	cSafeScript->CallTableFunction("Escort", "OnLoadPlayerComplete", 0, "oood", m_rcPlayer.GetScriptInterface(), pNpc->GetScriptInterface(), 
		pPetNpc != NULL ? pPetNpc->GetScriptInterface() : NULL, nStandIdx);

	bRetCode = SyncAddNpc(dwRoleId, pNpc->GetId(), pPetNpc != NULL ? pPetNpc->GetId() : 0, it->second, dwRoleId != m_rcPlayer.GetId());  // 0时不能fight
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::OnSyncRobEscortPlayers(INT nMembersNum, KRobEscort* pRobEscort)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;

	m_mapRobEscorts.clear();

	for(INT i = 0; i < nMembersNum; ++i)
	{
		KRobEscort sRobEscort;
		memcpy(&sRobEscort, &(pRobEscort[i]), sizeof(KRobEscort));

		m_mapRobEscorts[sRobEscort.nRobPlayerId] = sRobEscort;
	}

	bRetCode = DoSyncRobEscortMembers();
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::OnRobFightRequest(INT nRobPlayerId)
{
	BOOL bResult = FALSE;
	std::map<DWORD, KPlayerEscort>::iterator it_escort;
	std::map<DWORD, INT>::iterator it_idx;
	DWORD dwMeNpcId = 0;
	DWORD dwMePetNpcId = 0;
	DWORD dwHimNpcId = 0;
	DWORD dwHimPetNpcId = 0;

	it_escort = m_mapPlayerEscorts.find(nRobPlayerId);
	LOG_PROCESS_ERROR(it_escort != m_mapPlayerEscorts.end());

	// 已经被人劫驾了，就跟劫驾的那个人打
	if(it_escort->second.dwRoberId != 0)
	{
		it_idx = m_mapRole2Idxs.find(it_escort->second.dwRoberId);
		if(it_idx == m_mapRole2Idxs.end())
		{
			m_dwCurFightPlayerId = it_escort->second.dwRoberId;
			g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ESCORT, m_rcPlayer.GetId(), it_escort->second.dwRoberId);
		}
		else
		{
			dwMeNpcId = m_dwCurSelfNpcId;
			dwMePetNpcId = m_dwCurSelfPetNpcId;
			dwHimNpcId = GetNpcIdByRoleId(it_escort->second.dwRoberId);
			dwHimPetNpcId = GetPetNpcIdByRoleId(it_escort->second.dwRoberId);
			g_cArenaHelper.StartOfflineFight(m_rcPlayer.GetId(), nRobPlayerId, dwMeNpcId, dwMePetNpcId, dwHimNpcId, dwHimPetNpcId);
		}
	}
	else
	{
		// 没有护驾的人
		if(it_escort->second.dwProtectRoleId == 0)
		{
			dwMeNpcId = m_dwCurSelfNpcId;
			dwMePetNpcId = m_dwCurSelfPetNpcId;
			dwHimNpcId = GetNpcIdByRoleId(nRobPlayerId);
			dwHimPetNpcId = GetPetNpcIdByRoleId(nRobPlayerId);

			g_cArenaHelper.StartOfflineFight(m_rcPlayer.GetId(), nRobPlayerId, dwMeNpcId, dwMePetNpcId, dwHimNpcId, dwHimPetNpcId);
		}
		else
		{
			it_idx = m_mapRole2Idxs.find(it_escort->second.dwProtectRoleId);
			if(it_idx == m_mapRole2Idxs.end())
			{
				m_dwCurFightPlayerId = it_escort->second.dwProtectRoleId;
				g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ESCORT, m_rcPlayer.GetId(), it_escort->second.dwProtectRoleId);
			}
			else
			{
				dwMeNpcId = m_dwCurSelfNpcId;
				dwMePetNpcId = m_dwCurSelfPetNpcId;
				dwHimNpcId = GetNpcIdByRoleId(it_escort->second.dwProtectRoleId);
				dwHimPetNpcId = GetPetNpcIdByRoleId(it_escort->second.dwProtectRoleId);
				g_cArenaHelper.StartOfflineFight(m_rcPlayer.GetId(), nRobPlayerId, dwMeNpcId, dwMePetNpcId, dwHimNpcId, dwHimPetNpcId);
			}
		}
	}

EXIT0:
	return bResult;
}

// 救驾
BOOL KEscortCtrl::OnRescueFightRequest(INT nRobPlayerId)
{
	BOOL bResult = FALSE;
	std::map<DWORD, KPlayerEscort>::iterator it_escort;
	std::map<DWORD, DWORD>::iterator it;
	DWORD dwRoberId = 0;
	DWORD dwMeNpcId = 0;
	DWORD dwMePetNpcId = 0;
	DWORD dwHimNpcId = 0;
	DWORD dwHimPetNpcId = 0;

	it_escort = m_mapPlayerEscorts.find(nRobPlayerId);
	LOG_PROCESS_ERROR(it_escort != m_mapPlayerEscorts.end());

	dwRoberId = it_escort->second.dwRoberId;

	it = m_mapRole2Npcs.find(dwRoberId);

	// 劫驾人的npc已经在场景中
	if(it != m_mapRole2Npcs.end())
	{
		dwMeNpcId = m_dwCurSelfNpcId;
		dwMePetNpcId = m_dwCurSelfPetNpcId;
		dwHimNpcId = GetNpcIdByRoleId(dwRoberId);
		dwHimPetNpcId = GetPetNpcIdByRoleId(dwRoberId);

		g_cArenaHelper.StartOfflineFight(m_rcPlayer.GetId(), nRobPlayerId, dwMeNpcId, dwMePetNpcId, dwHimNpcId, dwHimPetNpcId);
	}
	else
	{
		m_dwCurFightPlayerId = dwRoberId;
		g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ESCORT, m_rcPlayer.GetId(), dwRoberId);
	}

EXIT0:
	return bResult;
}

INT KEscortCtrl::GetTotalEscortPage()
{
	INT nLastIdx = 0;
	INT nTotalPage = 1;
	for(INT i = 0; i < MAX_ESCORT_POS_NUM; ++i)
	{
		if(m_arrRoleIdxs[i])
		{
			nLastIdx = i;
		}
	}

	nTotalPage = nLastIdx / MAX_ESCORTPAGE_PLAYER_NUM + 1;

	return nTotalPage;
}

BOOL KEscortCtrl::DoSyncEscortMembers()
{
	INT										nIdx		= 0;
	IMemBlock*								piMem		= NULL;
	IMemBlock*								piMemEnd	= NULL;
	BOOL									bResult		= FALSE;
	BOOL									bRetCode	= FALSE;
	KS2C_SYNC_ESCORT_MEMBERS*				pSync		= NULL;
	KS2C_SYNC_ESCORT_MEMBERS_END*			pSyncEnd	= NULL;
	size_t									uPakSize	= 0;
	std::map<DWORD, KPlayerEscort>::iterator it;
	uPakSize = sizeof(KS2C_SYNC_ESCORT_MEMBERS) + sizeof(KPlayerEscort);
	QLogPrintf(LOG_INFO,"........m_mapPlayerEscorts.size()=%d,uPakSize=%d",m_mapPlayerEscorts.size(),uPakSize);

	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);
	
	piMemEnd = QCreateMemBlock(sizeof(KS2C_SYNC_ESCORT_MEMBERS_END));
	LOG_PROCESS_ERROR(piMemEnd);

	pSyncEnd = (KS2C_SYNC_ESCORT_MEMBERS_END*)piMemEnd->GetData();
	pSyncEnd->nTotalNumber = m_mapPlayerEscorts.size();
	pSyncEnd->byProtocol = s2c_sync_escort_members_end;
	
	pSync = (KS2C_SYNC_ESCORT_MEMBERS*)piMem->GetData();
	pSync->byProtocol = s2c_sync_escort_members;
	pSync->byCurEscortPage = (BYTE)m_nCurEscortPage;
	pSync->byTotalEscortPage = (BYTE)GetTotalEscortPage();
	pSync->nMembersNum = 1;

	
	for(it = m_mapPlayerEscorts.begin(); it != m_mapPlayerEscorts.end(); ++it)
	{
		KPlayerEscort* pEscort = (KPlayerEscort*)&it->second;
		memcpy((VOID*)&(pSync->aryEscorts[0]), pEscort, sizeof(KPlayerEscort));
		bRetCode = g_cEscortModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, uPakSize);
		LOG_PROCESS_ERROR(bRetCode);
	}
	bRetCode = g_cEscortModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, sizeof(KS2C_SYNC_ESCORT_MEMBERS_END));
	LOG_PROCESS_ERROR(bRetCode);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemEnd);
	return bResult;
}

BOOL KEscortCtrl::DoSyncRobEscortMembers()
{
	INT										nIdx		= 0;
	IMemBlock*								piMem		= NULL;
	IMemBlock*								piMemEnd	= NULL;
	BOOL									bResult		= FALSE;
	BOOL									bRetCode	= FALSE;
	KS2C_SYNC_ROB_MEMBERS*					pSync		= NULL;
	KS2C_SYNC_ROB_MEMBERS_END*              pSyncEnd    = NULL;
	size_t									uPakSize	= 0;
	std::map<DWORD, KRobEscort>::iterator it;

	uPakSize = sizeof(KS2C_SYNC_ROB_MEMBERS) +  sizeof(KRobEscort);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);
	pSync = (KS2C_SYNC_ROB_MEMBERS*)piMem->GetData();

	pSync->byProtocol = s2c_sync_rob_escort_members;
	pSync->nMembersNum = 1;

	piMemEnd= QCreateMemBlock(sizeof(KS2C_SYNC_ROB_MEMBERS_END));
	pSyncEnd= (KS2C_SYNC_ROB_MEMBERS_END*)piMemEnd->GetData();
	pSyncEnd->nTotalNumber = m_mapRobEscorts.size();
	pSyncEnd->byProtocol = s2c_sync_rob_escort_members_end;


	for(it = m_mapRobEscorts.begin(); it != m_mapRobEscorts.end(); ++it)
	{
		KRobEscort* pEscort = (KRobEscort*)&it->second;
		memcpy((VOID*)&(pSync->aryRobEscorts[0]), pEscort, sizeof(KRobEscort));
		bRetCode = g_cEscortModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, uPakSize);
		LOG_PROCESS_ERROR(bRetCode);		
	}
	
	bRetCode = g_cEscortModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSyncEnd, sizeof(KS2C_SYNC_ROB_MEMBERS_END));
	LOG_PROCESS_ERROR(bRetCode);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	SAFE_RELEASE(piMemEnd);
	return bResult;
}

DWORD KEscortCtrl::GetNpcIdByRoleId(DWORD dwRoleId)
{
	DWORD dwNpcId = 0;
	std::map<DWORD, DWORD>::iterator it = m_mapRole2Npcs.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapRole2Npcs.end());

	dwNpcId = it->second;

EXIT0:
	return dwNpcId;
}

DWORD KEscortCtrl::GetPetNpcIdByRoleId(DWORD dwRoleId)
{
	DWORD dwPetNpcId = 0;
	std::map<DWORD, DWORD>::iterator it = m_mapRole2PetNpcs.find(dwRoleId);
	PROCESS_ERROR(it != m_mapRole2PetNpcs.end());

	dwPetNpcId = it->second;

EXIT0:
	return dwPetNpcId;
}

BOOL KEscortCtrl::OnUpdatePlayerEscortState(INT nPlayerId, INT nEscortState, INT nLeftTime, DWORD dwRoberId, DWORD dwRoberFightscore, LPCTSTR szRoberName)
{
	QLogPrintf(LOG_INFO,"..KEscortCtrl::OnUpdatePlayerEscortState()");
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	std::map<DWORD, KPlayerEscort>::iterator it_escort;
	KS2C_UPDATE_ESCORT_STATE sSync;

	it_escort = m_mapPlayerEscorts.find(nPlayerId);
	LOG_PROCESS_ERROR(it_escort != m_mapPlayerEscorts.end());

	it_escort->second.byEscortState = (BYTE)nEscortState;
	it_escort->second.dwRoberId = dwRoberId;
	strncpy(it_escort->second.szRoberName, szRoberName, _NAME_LEN);

	sSync.nPlayerId = nPlayerId;
	sSync.byEscortState = (BYTE)nEscortState;
	sSync.nLeftTime = nLeftTime;
	sSync.dwRoberId = dwRoberId;
	sSync.dwRoberFightscore = dwRoberFightscore;
	strncpy(sSync.szRoberName, szRoberName, _NAME_LEN);

	bRetCode = g_cEscortModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::SyncAddNpc(DWORD dwRoleId, DWORD dwNpcId, DWORD dwPetNpcId, INT nPos, BOOL bCanFight)
{
	KS2C_ESCORT_ADD_NPC					sSync;
	BOOL								bResult				= FALSE;
	BOOL								bRetCode			= FALSE;

	sSync.dwMappingRoleId = dwRoleId;
	sSync.dwNpcId = dwNpcId;
	sSync.dwPetNpcId = dwPetNpcId;
	sSync.byPos = (BYTE)nPos;
	sSync.byCanFight = (BYTE)bCanFight;

	bRetCode = g_cEscortModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::SyncRemoveNpc(DWORD dwNpcId, BOOL bIsPet /* = FALSE */)
{
	KS2C_ESCORT_REMOVE_NPC					sSync;
	BOOL								bResult				= FALSE;
	BOOL								bRetCode			= FALSE;

	sSync.dwNpcId = dwNpcId;
	sSync.byIsPet = (BYTE)bIsPet;

	bRetCode = g_cEscortModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::SyncAddSelfNpc(DWORD dwRoleId, DWORD dwNpcId, DWORD dwPetNpcId)
{
	KS2C_ESCORT_ADD_SELF_NPC					sSync;
	BOOL								bResult				= FALSE;
	BOOL								bRetCode			= FALSE;

	sSync.dwMappingRoleId = dwRoleId;
	sSync.dwNpcId = dwNpcId;
	sSync.dwPetNpcId = dwPetNpcId;

	bRetCode = g_cEscortModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortCtrl::ClearNpcs()
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	std::map<DWORD, DWORD>::iterator it;
	KNpc* pNpc = NULL;

	for(it = m_mapRole2Npcs.begin(); it != m_mapRole2Npcs.end(); ++it)
	{
		DWORD dwNpcId = it->second;

		pNpc = g_cNpcMgr.GetById(dwNpcId);
		LOG_PROCESS_ERROR(pNpc);
		bRetCode = g_cOrpgWorld.RemoveNpc(dwNpcId);
		LOG_PROCESS_ERROR(bRetCode);

		bRetCode = SyncRemoveNpc(dwNpcId);
		LOG_PROCESS_ERROR(bRetCode);
	}

	for(it = m_mapRole2PetNpcs.begin(); it != m_mapRole2PetNpcs.end(); ++it)
	{
		DWORD dwPetNpcId = it->second;

		pNpc = g_cNpcMgr.GetById(dwPetNpcId);
		LOG_PROCESS_ERROR(pNpc);
		bRetCode = g_cOrpgWorld.RemoveNpc(dwPetNpcId);
		LOG_PROCESS_ERROR(bRetCode);

		bRetCode = SyncRemoveNpc(dwPetNpcId, TRUE);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;

EXIT0:
	m_mapRole2Npcs.clear();
	m_mapRole2PetNpcs.clear();

	return bResult;
}
