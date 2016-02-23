
#include "stdafx.h"
#include <utility> 
#include "kescortmanager.h"
#include "kescorthandler.h"
#include "GameCenter.h"
#include "onlinegamebase/kscriptmanager.h"
#include "Runtime/TypeDef.h"

KEscortManager g_cEscortMgr;

KEscortManager::KEscortManager()
{

}

KEscortManager::~KEscortManager()
{

}

BOOL KEscortManager::Init()
{
EXIT0:
	return TRUE;
}

VOID KEscortManager::UnInit()
{
	
}

VOID KEscortManager::Activate()
{
	BOOL 	bResult = FALSE;
	BYTE    byBuffer[32];
	int   	nEnergy = 0;
	std::vector<KeyValue> vecEscortList;
	if(g_pGameCenter->m_nWorkLoop % GAME_FPS != 0)
	{
		goto EXIT0;
	}

	for(std::map<DWORD, KPlayerEscort>::iterator it = m_mapEscorts.begin(); it != m_mapEscorts.end(); ++it)
	{
		if(it->second.nLeftTime <= 0)
		{
			continue;
		}

		--it->second.nLeftTime;
		if(it->second.nLeftTime <= 0)
		{
			if(it->second.byEscortState == emESCORT_STATE_NORMAL)
			{
				it->second.byEscortState = emESCORT_STATE_COMPLETE_NORMAL;
			}
			else
			{
				it->second.byEscortState = emESCORT_STATE_COMPLETE_BEHIT;
			}
			
		
			// 同步状态
			KEscortHandler::DoSyncEscortState(it->second.nPlayerId);

			// 向所有玩家更新状态
			DoUpdatePlayerEscortState(it->second.nPlayerId);

			// 如果被劫驾就修改劫驾状态
			{
				std::map<DWORD, DWORD>::iterator it_rob = m_mapRobPlayers.find(it->second.nPlayerId);
				if(it_rob != m_mapRobPlayers.end())
				{
					LOG_PROCESS_ERROR(DoModifyRobState(it->second.nPlayerId, it_rob->second));
					m_mapRobPlayers.erase(it_rob);
				}
			}
			//2000*等级+(等级-1)*2000
			nEnergy = 2000 * (it->second.byEscortLevel) + (it->second.byEscortLevel - 1) * 2000;
			KeyValue kv;
			kv.nKey  = it->second.nPlayerId;
			kv.nValue = nEnergy;
			vecEscortList.push_back(kv);//有删除操作，迭代器会失效
		}
	}

	for(std::vector<KeyValue>::iterator it = vecEscortList.begin();it != vecEscortList.end();++it)
	{
			//领奖励
			bResult = OnTakeEscortAwardRequest(it->nKey);
			LOG_PROCESS_ERROR(bResult);
			
			byBuffer[0] = '\0';
	        g_pGameCenter->m_MailManager.PostRequestSendMail(
	            0, it->nKey, 0, eMailType_System, 
	            g_pGameCenter->m_Settings.m_szSystemMailSender, 
	           g_pGameCenter->m_Settings.m_szTongReturnMoneyMailTitle,
	            0,0,it->nValue,
	            (BYTE *)LANG_042, strlen(LANG_042)+1,
	            NULL, 0, 0
	        );
	}
	
EXIT0:
	return;
}

BOOL KEscortManager::DoRefreshEscortPlayers()
{
	QLogPrintf(LOG_INFO,"..KEscortManager::DoRefreshEscortPlayers()");
	BOOL bResult = FALSE;

	// 向所有监听玩家即时同步修仙列表
	for(std::map<INT, INT>::iterator it = m_mapWatchTable.begin(); it != m_mapWatchTable.end(); ++it)
	{
		DWORD dwRoleId = it->first;
		KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleId);
		LOG_PROCESS_ERROR(pRole);

		if(pRole->IsOnline())
		{
			KEscortHandler::DoSyncEscortMembers(pRole->m_nConnIndex, dwRoleId);
		}
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

// 向所有玩家更新某个人的状态
BOOL KEscortManager::DoUpdatePlayerEscortState(INT nPlayerId)
{
	BOOL bResult = FALSE;

	for(std::map<INT, INT>::iterator it = m_mapWatchTable.begin(); it != m_mapWatchTable.end(); ++it)
	{
		DWORD dwRoleId = it->first;
		KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleId);
		LOG_PROCESS_ERROR(pRole);

		if(pRole->IsOnline())
		{
			KEscortHandler::DoUpdateEscortState(pRole->m_nConnIndex, dwRoleId, nPlayerId);
		}
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

// 将劫驾状态改为可领奖
BOOL KEscortManager::DoModifyRobState(INT nRobPlayerId, INT nRoberId)
{
	BOOL bResult = FALSE;
	KRole* pRole = NULL;

	std::map<DWORD, KMAP_ROB_ESCORT>::iterator it_rob = m_mapRobEscorts.find(nRoberId);
	LOG_PROCESS_ERROR(it_rob != m_mapRobEscorts.end());

	{
		KMAP_ROB_ESCORT& mapRobs = it_rob->second;
		KMAP_ROB_ESCORT::iterator it = mapRobs.find(nRobPlayerId);
		LOG_PROCESS_ERROR(it != mapRobs.end());

		it->second.byCanTakeAward = TRUE;
	}

	pRole = g_pGameCenter->m_RoleManager.GetRole(nRoberId);
	LOG_PROCESS_ERROR(pRole);

	if(pRole->IsOnline())
	{
		KEscortHandler::DoSyncRobEscortMembers(pRole->m_nConnIndex, nRoberId);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortManager::OnStartEscortRequest(INT nPlayerId, DWORD dwProtectRoleId, INT nEscortLevel)
{
	//ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	//ScriptSafe cSafeScript_MAX = g_cScriptManager.GetSafeScript();
	//INT EscortNum = 0;
	//INT MaxEscortNum = 0;
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	KPlayerEscort sEscort;
	INT nEscortProtectRoleId = 0;
	KRole* pRole = NULL;
	std::map<DWORD, KPlayerEscort>::iterator it;

	it = m_mapEscorts.find(nPlayerId);
	LOG_PROCESS_ERROR(it == m_mapEscorts.end());

	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	sEscort.nPlayerId = nPlayerId;
	sEscort.byPlayerLevel = (BYTE)pRole->GetLevel();
	strncpy(sEscort.szPlayerName, pRole->GetPlayerName(), _NAME_LEN);
	sEscort.dwFightScore = (DWORD)pRole->m_nFightScore;
	sEscort.byEscortLevel = (BYTE)nEscortLevel;
	sEscort.byEscortState = (BYTE)emESCORT_STATE_NORMAL;
	sEscort.nLeftTime = ESCORT_TIME;
	sEscort.byIfSpeedUp = FALSE;
	sEscort.dwProtectRoleId = 0;
	sEscort.dwRoberId = 0;

	
	if(dwProtectRoleId)
	{
		KRole* pProtectRole = g_pGameCenter->m_RoleManager.GetRole(dwProtectRoleId);
		LOG_PROCESS_ERROR(pProtectRole);

		// 战斗力不足才需要保护
		//if(pRole->m_nFightScore < pProtectRole->m_nFightScore)
		{
			sEscort.dwProtectRoleId = dwProtectRoleId;
			strncpy(sEscort.szProtecterName, pProtectRole->GetPlayerName(), _NAME_LEN);
		}
	}

	m_mapEscorts[nPlayerId] = sEscort;

	bRetCode = DoRefreshEscortPlayers();
	//bRetCode = DoUpdatePlayerEscortState(nPlayerId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortManager::SerializeMemberEscorts(LPBYTE byData)
{
	BOOL bResult = FALSE;

	for(std::map<DWORD, KPlayerEscort>::iterator it = m_mapEscorts.begin(); it != m_mapEscorts.end(); ++it)
	{
		KPlayerEscort* pEscort = &(it->second);
		memcpy(byData, pEscort, sizeof(KPlayerEscort));

		byData += sizeof(KPlayerEscort);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortManager::SerializeRobEscorts(INT nPlayerId, LPBYTE byData)
{
	BOOL bResult = FALSE;
	std::map<DWORD, KMAP_ROB_ESCORT>::iterator it_rob;
	std::map<DWORD, KPlayerEscort>::iterator it_escort;

	it_rob = m_mapRobEscorts.find(nPlayerId);
	if(it_rob == m_mapRobEscorts.end())
	{
		bResult = TRUE;
		goto EXIT0;
	}

	for(std::map<DWORD, KRobEscort>::iterator it = it_rob->second.begin(); it != it_rob->second.end(); ++it)
	{
		// 更新下时间
		{
			it_escort = m_mapEscorts.find(it->second.nRobPlayerId);
			if(it_escort == m_mapEscorts.end())
			{
				it->second.nLeftTime = 0;
				it->second.byCanTakeAward = 1;
			}
			else
			{
				if(!it->second.byCanTakeAward)
				{
					it->second.nLeftTime = it_escort->second.nLeftTime;
				}
			}
		}

		KRobEscort* pRobEscort = &(it->second);
		memcpy(byData, pRobEscort, sizeof(KRobEscort));

		byData += sizeof(KRobEscort);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

INT KEscortManager::GetRobEscortsMembersNum(INT nPlayerId)
{
	INT nMembersNum = 0;
	std::map<DWORD, KMAP_ROB_ESCORT>::iterator it_rob;

	it_rob = m_mapRobEscorts.find(nPlayerId);
	PROCESS_ERROR(it_rob != m_mapRobEscorts.end());

	nMembersNum = it_rob->second.size();

EXIT0:
	return nMembersNum;
}

INT KEscortManager::GetPlayerEscortState(INT nPlayerId)
{
	INT nState = emESCORT_STATE_NONE;
	std::map<DWORD, KPlayerEscort>::iterator it;

	it = m_mapEscorts.find(nPlayerId);
	PROCESS_ERROR(it != m_mapEscorts.end());

	nState = it->second.byEscortState;

EXIT0:
	return nState;
}

BOOL KEscortManager::GetPlayerEscortInfo(INT nPlayerId, KPlayerEscort& sEscort)
{
	BOOL bResult = FALSE;
	std::map<DWORD, KPlayerEscort>::iterator it;

	it = m_mapEscorts.find(nPlayerId);
	PROCESS_ERROR(it != m_mapEscorts.end());

	sEscort = it->second;

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KEscortManager::AddPlayerToWatchTable(INT nPlayerId)
{
	std::map<INT, INT>::iterator it = m_mapWatchTable.find(nPlayerId);
	if(it != m_mapWatchTable.end())
	{
		m_mapWatchTable.erase(it);
	}

	m_mapWatchTable[nPlayerId] = 1;
}

VOID KEscortManager::RemovePlayerFromWatchTable(INT nPlayerId)
{
	std::map<INT, INT>::iterator it = m_mapWatchTable.find(nPlayerId);
	if(it != m_mapWatchTable.end())
	{
		m_mapWatchTable.erase(it);
	}
}

BOOL KEscortManager::OnTakeEscortAwardRequest(INT nPlayerId)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	BYTE byEscortState = 0;
	BYTE byEscortLevel = 0;
	std::map<DWORD, KPlayerEscort>::iterator it;

	it = m_mapEscorts.find(nPlayerId);
	LOG_PROCESS_ERROR(it != m_mapEscorts.end());

	LOG_PROCESS_ERROR(it->second.byEscortState == emESCORT_STATE_COMPLETE_BEHIT || it->second.byEscortState == emESCORT_STATE_COMPLETE_NORMAL);

	byEscortState = it->second.byEscortState;
	byEscortLevel = it->second.byEscortLevel;

	m_mapEscorts.erase(it);

	bRetCode = DoRefreshEscortPlayers();
	//bRetCode = DoUpdatePlayerEscortState(nPlayerId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	KEscortHandler::DoTakeEscortAwardRespond(nPlayerId, byEscortState, byEscortLevel, bResult);

	return bResult;
}

//请求劫驾
BOOL KEscortManager::OnRobEscortRequest(INT nPlayerId, INT nRobPlayerId)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	KRole* pRobRole = NULL;
	KRole* pRole = NULL;
	KRobEscort sRobEscort;
	std::map<DWORD, KMAP_ROB_ESCORT>::iterator it_rob;
	std::map<DWORD, KPlayerEscort>::iterator it_escort;
	std::map<DWORD, DWORD>::iterator it_be_rob;

	it_escort = m_mapEscorts.find(nRobPlayerId);
	LOG_PROCESS_ERROR(it_escort != m_mapEscorts.end());

	pRobRole = g_pGameCenter->m_RoleManager.GetRole(nRobPlayerId);
	LOG_PROCESS_ERROR(pRobRole);

	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	it_be_rob = m_mapRobPlayers.find(nRobPlayerId);
	// 已经被其他人劫驾，先把原来的删掉
	if(it_be_rob != m_mapRobPlayers.end() && nPlayerId != it_be_rob->second)
	{
		DWORD dwRoberId = it_be_rob->second;
		it_rob = m_mapRobEscorts.find(dwRoberId);
		LOG_PROCESS_ERROR(it_rob != m_mapRobEscorts.end());

		{
			KMAP_ROB_ESCORT& mapRobs = it_rob->second;
			KMAP_ROB_ESCORT::iterator it = mapRobs.find(nRobPlayerId);
			LOG_PROCESS_ERROR(it != mapRobs.end());

			mapRobs.erase(it);
		}

		{
			KRole* pOtherRole = g_pGameCenter->m_RoleManager.GetRole(dwRoberId);
			LOG_PROCESS_ERROR(pOtherRole);

			if(pOtherRole->IsOnline())
			{
				bRetCode = KEscortHandler::DoSyncRobEscortMembers(pOtherRole->m_nConnIndex, pOtherRole->GetId());
				LOG_PROCESS_ERROR(bRetCode);
			}
		}
		
	}

	it_rob = m_mapRobEscorts.find(nPlayerId);
	if(it_rob == m_mapRobEscorts.end())
	{
		std::map<DWORD, KRobEscort> mapRob;
		m_mapRobEscorts[nPlayerId] = mapRob;
		it_rob = m_mapRobEscorts.find(nPlayerId);
	}

	LOG_PROCESS_ERROR(it_rob != m_mapRobEscorts.end());

	{
		KMAP_ROB_ESCORT& mapRobs = it_rob->second;
		KMAP_ROB_ESCORT::iterator it = mapRobs.find(nRobPlayerId);
		LOG_PROCESS_ERROR(it == mapRobs.end());

		sRobEscort.nRobPlayerId = nRobPlayerId;
		sRobEscort.nRoberId = nPlayerId;
		sRobEscort.nLeftTime = it_escort->second.nLeftTime;
		strncpy(sRobEscort.szPlayerName, pRobRole->GetPlayerName(), _NAME_LEN);
		sRobEscort.byPlayerLevel = (BYTE)pRobRole->GetLevel();
		sRobEscort.byEscortLevel = it_escort->second.byEscortLevel;
		sRobEscort.byCanTakeAward = FALSE;

		mapRobs[nRobPlayerId] = sRobEscort;
	}

	m_mapRobPlayers[nRobPlayerId] = nPlayerId;

	LOG_PROCESS_ERROR(it_escort->second.byEscortState == emESCORT_STATE_NORMAL || it_escort->second.byEscortState == emESCORT_STATE_BEHIT);
	it_escort->second.byEscortState = emESCORT_STATE_BEHIT;
	it_escort->second.dwRoberId = nPlayerId;
	it_escort->second.dwRoberFightscore = pRole->m_nFightScore;
	strncpy(it_escort->second.szRoberName, pRole->GetPlayerName(), _NAME_LEN);

	if(pRobRole->IsOnline())
	{
		// 同步状态
		bRetCode = KEscortHandler::DoSyncEscortState(nRobPlayerId);
		LOG_PROCESS_ERROR(bRetCode);
	}

	// 向抢劫玩家同步一次列表
	if(pRole->IsOnline())
	{
		bRetCode = KEscortHandler::DoSyncRobEscortMembers(pRole->m_nConnIndex, pRole->GetId());
		LOG_PROCESS_ERROR(bRetCode);
	}

	// 向所有玩家更新状态
	bRetCode = DoUpdatePlayerEscortState(nRobPlayerId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

// 救驾成功
BOOL KEscortManager::OnDelRobEscortRequest(INT nPlayerId, INT nRobPlayerId)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	INT nRoberId = 0;
	KRole* pRobRole = NULL;
	KRole* pRole = NULL;
	std::map<DWORD, KMAP_ROB_ESCORT>::iterator it_rob;
	std::map<DWORD, KPlayerEscort>::iterator it_escort;

	it_escort = m_mapEscorts.find(nRobPlayerId);
	LOG_PROCESS_ERROR(it_escort != m_mapEscorts.end());

	pRobRole = g_pGameCenter->m_RoleManager.GetRole(nRobPlayerId);
	LOG_PROCESS_ERROR(pRobRole);

	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	it_rob = m_mapRobEscorts.find(nPlayerId);
	LOG_PROCESS_ERROR(it_rob != m_mapRobEscorts.end());

	{
		KMAP_ROB_ESCORT& mapRobs = it_rob->second;
		KMAP_ROB_ESCORT::iterator it = mapRobs.find(nRobPlayerId);
		LOG_PROCESS_ERROR(it != mapRobs.end());

		mapRobs.erase(it);
	}

	{
		std::map<DWORD, DWORD>::iterator it = m_mapRobPlayers.find(nRobPlayerId);
		LOG_PROCESS_ERROR(it != m_mapRobPlayers.end());
		m_mapRobPlayers.erase(it);
	}

	LOG_PROCESS_ERROR(it_escort->second.byEscortState == emESCORT_STATE_BEHIT);
	it_escort->second.byEscortState = emESCORT_STATE_NORMAL;
	it_escort->second.dwRoberId = 0;

	if(pRobRole->IsOnline())
	{
		// 同步状态
		bRetCode = KEscortHandler::DoSyncEscortState(nRobPlayerId);
		LOG_PROCESS_ERROR(bRetCode);
	}

	// 像抢劫玩家同步一次列表
	if(pRole->IsOnline())
	{
		bRetCode = KEscortHandler::DoSyncRobEscortMembers(pRole->m_nConnIndex, pRole->GetId());
		LOG_PROCESS_ERROR(bRetCode);
	}

	// 向所有玩家更新状态
	bRetCode = DoUpdatePlayerEscortState(nRobPlayerId);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortManager::OnSpeedUpEscortRequest(INT nPlayerId, BOOL bSpeedUp)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	std::map<DWORD, KMAP_ROB_ESCORT>::iterator it_rob;
	std::map<DWORD, KPlayerEscort>::iterator it_escort;

	BYTE    byBuffer[32];
	int   	nEnergy = 0;
	std::vector<KeyValue> vecEscortList;
	KeyValue kv;


	
	it_escort = m_mapEscorts.find(nPlayerId);
	LOG_PROCESS_ERROR(it_escort != m_mapEscorts.end());

	LOG_PROCESS_ERROR(it_escort->second.nLeftTime > 0);

	PROCESS_ERROR(!it_escort->second.byIfSpeedUp);

	if(bSpeedUp)
	{
		it_escort->second.nLeftTime = it_escort->second.nLeftTime / 2;
	}
	else
	{
		it_escort->second.nLeftTime = 0;

		if(it_escort->second.byEscortState == emESCORT_STATE_NORMAL)
		{
			it_escort->second.byEscortState = emESCORT_STATE_COMPLETE_NORMAL;
		}
		else
		{
			it_escort->second.byEscortState = emESCORT_STATE_COMPLETE_BEHIT;
		}
	}

	it_escort->second.byIfSpeedUp = TRUE;

	// 同步状态
	KEscortHandler::DoSyncEscortState(nPlayerId);

	// 向所有玩家更新状态
	DoUpdatePlayerEscortState(nPlayerId);

	// 如果有人劫驾，修改劫驾时间
	it_rob = m_mapRobEscorts.find(it_escort->second.dwRoberId);
	if(it_rob != m_mapRobEscorts.end())
	{
		KMAP_ROB_ESCORT& mapRobs = it_rob->second;
		KMAP_ROB_ESCORT::iterator it = mapRobs.find(nPlayerId);
		LOG_PROCESS_ERROR(it != mapRobs.end());
		it->second.nLeftTime = it_escort->second.nLeftTime;
		{
			KRole* pRobRole = g_pGameCenter->m_RoleManager.GetRole(it_escort->second.dwRoberId);
			LOG_PROCESS_ERROR(pRobRole);
			if(pRobRole->IsOnline())
			{
				bRetCode = KEscortHandler::DoSyncRobEscortMembers(pRobRole->m_nConnIndex, pRobRole->GetId());
				LOG_PROCESS_ERROR(bRetCode);
			}
		}
	}

	//2000*等级+(等级-1)*2000
	nEnergy = 2000 * (it_escort->second.byEscortLevel) + (it_escort->second.byEscortLevel - 1) * 2000;
	
	kv.nKey  = it_escort->second.nPlayerId;
	kv.nValue = nEnergy;
	vecEscortList.push_back(kv);//有删除操作，迭代器会失效


	for(std::vector<KeyValue>::iterator it = vecEscortList.begin();it != vecEscortList.end();++it)
	{
			//领奖励
			bResult = OnTakeEscortAwardRequest(it->nKey);
			LOG_PROCESS_ERROR(bResult);
			
			byBuffer[0] = '\0';
	        g_pGameCenter->m_MailManager.PostRequestSendMail(
	            0, it->nKey, 0, eMailType_System, 
	            g_pGameCenter->m_Settings.m_szSystemMailSender, 
	           g_pGameCenter->m_Settings.m_szTongReturnMoneyMailTitle,
	            0,0,it->nValue,
	            (BYTE *)LANG_042, strlen(LANG_042)+1,
	            NULL, 0, 0
	        );
	}
	
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortManager::OnTakeRobAwardRequest(INT nPlayerId, INT nRobPlayerId, BYTE byEscortLevel)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	KRole* pRole = NULL;
	std::map<DWORD, KMAP_ROB_ESCORT>::iterator it_rob;

	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	it_rob = m_mapRobEscorts.find(nPlayerId);
	LOG_PROCESS_ERROR(it_rob != m_mapRobEscorts.end());

	{
		KMAP_ROB_ESCORT& mapRobs = it_rob->second;
		KMAP_ROB_ESCORT::iterator it = mapRobs.find(nRobPlayerId);
		PROCESS_ERROR(it != mapRobs.end());

		mapRobs.erase(it);
	}

	bResult = TRUE;
EXIT0:
	// 像抢劫玩家同步一次列表
	if(pRole->IsOnline())
	{
		KEscortHandler::DoSyncRobEscortMembers(pRole->m_nConnIndex, pRole->GetId());
	}

	KEscortHandler::DoTakeRobAwardRespond(nPlayerId, byEscortLevel, bResult);

	return bResult;
}