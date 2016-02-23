#include "playerarena.h"
#include "arenadef.h"
#include "ArenaHelper.h"
#include "ArenaRelayClient.h"
#include "arenamodule.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/korpgworld.h"
#include "../onlinegameworld/KRelayClient.h"
#include "rank/RankRelayClient.h"
#include "onlinegameworld/knpcmgr.h"
#include "onlinegameworld/kplayermgr.h"
#include "arenaprotocol.h"
#include "../award/kawardctrl.h"
#include "pet/kpetsetting.h"
#include "onlinegamebase/ksysservice.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "Runtime/TypeDef.h"

PlayerArena::PlayerArena(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	
}

PlayerArena::~PlayerArena(void)
{
}

BOOL PlayerArena::Init()
{
	m_pLunaObj = new LuaPlayerArena(m_rcPlayer);

	m_nWinCombo = 0;
	m_nFailCombo = 0;
	m_nCDTime = 0;
	m_nFightNum = 0;
	m_nCurrChallengeCDTime = 0;

	m_dwLastTakeAwardTime = 0;
	m_nTakeComboAward = 0;
	m_bCheckedAreaRecordFightResult = FALSE;

	return TRUE;
}

BOOL PlayerArena::UnInit()
{
	SAFE_DELETE(m_pLunaObj);
	return TRUE;
}

BOOL PlayerArena::ResetData()
{
	m_nWinCombo = 0;
	m_nFailCombo = 0;
	m_nCDTime = 0;
	m_nFightNum = 0;
	m_nCurrChallengeCDTime = 0;

	// 不重置领取奖励时间
	//m_dwLastTakeAwardTime = 0;
	m_nTakeComboAward = 0;

	SyncArenaInfo();

	return TRUE;
}

BOOL PlayerArena::AddArenaRank()
{
	// 加入排行榜
    g_RelayClient.DoSaveDelayRoleData(&m_rcPlayer);

	RankRelayClient::DoModifyRankValueRequest(emRANK_ARENA, m_rcPlayer.GetId(), m_rcPlayer.GetFightScore());
	//RankRelayClient::DoModifyRankValueRequest(emRANK_AREAN_OS, m_rcPlayer.GetId(),0);
	DoGetPlayerIdxRequest(m_rcPlayer.GetId(), m_rcPlayer.GetId());
	m_bFirstLogin = TRUE;

	return TRUE;
}

BOOL PlayerArena::OnLogin()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	m_bFirstLogin = FALSE;

	if (m_rcPlayer.IsFirstTimeLogin())
	{
		// 创建离线角色数据
		g_RelayClient.DoCreateDelayRoleData(&m_rcPlayer);
	}

	if (m_rcPlayer.GetLevel() >= ARENA_OPEN_LEVEL)
	{
		ArenaRelayClient::DoLoadRecordRequest(m_rcPlayer.GetId());

		cSafeScript->CallTableFunction("Arena", "CheckRankAward", 0, "od", m_rcPlayer.GetScriptInterface(), m_dwLastTakeAwardTime);

		SyncArenaInfo();
	}

	return TRUE;
}

BOOL PlayerArena::OnLogout()
{
	ClearNpcs();
	return TRUE;
}

BOOL PlayerArena::Active()
{
	if(m_nCDTime > 0)
	{
		--m_nCDTime;
	}
	return TRUE;
}

BOOL PlayerArena::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	KArenaSaveData* pSaveData = (KArenaSaveData*)pData;

	m_dwLastTakeAwardTime = pSaveData->dwLastTakeAwardTime;
	m_nTakeComboAward = pSaveData->byTakeComboAward;
	m_nCDTime = pSaveData->wCDTime;
	m_nCurrChallengeCDTime = pSaveData->wCurrentChallengeCDTime;
	m_nFightNum = pSaveData->wFightNum;
	m_nWinCombo = pSaveData->byWinCombo;

	return TRUE;
}

BOOL PlayerArena::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	KArenaSaveData sSaveData;

	sSaveData.dwLastTakeAwardTime = m_dwLastTakeAwardTime;
	sSaveData.wCDTime = (WORD)m_nCDTime;
	sSaveData.wCurrentChallengeCDTime = (WORD)m_nCurrChallengeCDTime;
	sSaveData.wFightNum = (WORD)m_nFightNum;
	sSaveData.byTakeComboAward = (BYTE)m_nTakeComboAward;
	sSaveData.byWinCombo = (BYTE)m_nWinCombo;

	memcpy(pBuffer, &sSaveData, sizeof(KArenaSaveData));
	pBuffer += sizeof(KArenaSaveData);
	uUsedSize += sizeof(KArenaSaveData);

	return TRUE;
}

BOOL PlayerArena::ClearNpcs()
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	std::map<DWORD, DWORD>::iterator it;
	KNpc* pNpc = NULL;
	KNpc* pPetNpc = NULL;

	for(it = m_mapNpc2Role.begin(); it != m_mapNpc2Role.end(); ++it)
	{
		// 宠物npc删除
		DWORD dwPetId = m_mapNpc2Pets[it->first];
		if (dwPetId > 0)
		{
			pPetNpc = g_cNpcMgr.GetById(dwPetId);
			LOG_PROCESS_ERROR(pPetNpc);
			bRetCode = g_cOrpgWorld.RemoveNpc(dwPetId);
			LOG_PROCESS_ERROR(bRetCode);
		}

		pNpc = g_cNpcMgr.GetById(it->first);

		if(pNpc)
		{
			bRetCode = g_cOrpgWorld.RemoveNpc(it->first);
			LOG_PROCESS_ERROR(bRetCode);
		}
	}

	m_mapNpc2Role.clear();

	m_mapRole2Npc.clear();

	m_mapDelayData.clear();

	m_mapRankIdx.clear();

	m_mapNpc2Pets.clear();

	bResult = TRUE;
EXIT0:
	return bResult;
}

// arena开始部分, 读取主角信息 by kk
BOOL PlayerArena::StartArena()
{
	BOOL bRetCode = TRUE;
	DWORD dwId = m_rcPlayer.GetId();

	//bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(dwId, dwId);  // 玩家自己
	//LOG_PROCESS_ERROR(bRetCode);
	//m_bFrontArenaNpcsMode = FALSE; // 默认非前5模式
	g_RelayClient.DoSaveDelayRoleData(&m_rcPlayer);

	DoGetPlayerIdxRequest(dwId, dwId);  // playerId , player role id , 里面创建npc

	bRetCode = TRUE;

EXIT0:
	return bRetCode;
}

// 刷新
BOOL PlayerArena::RefreshArenaPlayers()
{
	if (!m_bFrontArenaNpcsMode)
	{
		//this->DoLoadArenaPlayers();
		this->StartArena();  // 使用start，因为DoLoad是不会更新玩家排名的
	}
	else
	{
		this->DoLoadFrontArenaPlayers();
	}

	return TRUE;
}

// Npc加载部分, 这里调用前，需要更新玩家自身排名
BOOL PlayerArena::DoLoadArenaPlayers()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	DWORD dwId = m_rcPlayer.GetId();
	INT nCount = 0;
	m_bFrontArenaNpcsMode = FALSE;

	ClearNpcs();

	g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ARENA, m_rcPlayer.GetId(), m_rcPlayer.GetId());  // 玩家自己

	m_mapRankPos.clear();

	// 获取我的排名前5的人，如果我是非前5名
	if (m_nRankIdx >= ARENA_PRE_RANK_NUM)
	{
		for(INT nIdx = m_nRankIdx - 1; nIdx >= 0 && nCount < ARENA_PRE_RANK_NUM; --nIdx, ++nCount)  // nIdx排名大于0， nCount 5
		{
			bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_ARENA, m_rcPlayer.GetId(), nIdx);
			LOG_PROCESS_ERROR(bRetCode);
		}
	}
	else
	{
		//我在前5名内, 全部前5名加载
		for(INT nIdx = 0; nIdx < ARENA_PRE_RANK_NUM; ++nIdx)
		{
			bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_ARENA, m_rcPlayer.GetId(), nIdx);
			LOG_PROCESS_ERROR(bRetCode);
		}
	}


	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerArena::DoLoadFrontArenaPlayers()
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	DWORD dwId = m_rcPlayer.GetId();

	m_bFrontArenaNpcsMode = TRUE;

	ClearNpcs();
	g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ARENA, m_rcPlayer.GetId(), m_rcPlayer.GetId());  // 玩家自己

	m_mapRankPos.clear();

	for(INT nIdx = 0; nIdx < ARENA_PRE_RANK_NUM; ++nIdx)
	{
		bRetCode = RankRelayClient::DoGetRankIdRequest(emRANK_ARENA, m_rcPlayer.GetId(), nIdx);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

DWORD PlayerArena::GetRoleIdByNpcId(DWORD dwNpcId)
{
	DWORD dwRoleId = 0;
	std::map<DWORD, DWORD>::iterator it = m_mapNpc2Role.find(dwNpcId);
	if(it != m_mapNpc2Role.end())
	{
		dwRoleId = it->second;
	}

	return dwRoleId;
}

DWORD PlayerArena::GetNpcIdByRoleId(DWORD dwRoleId)
{
	DWORD dwNpcId = 0;
	std::map<DWORD, DWORD>::iterator it = m_mapRole2Npc.find(dwRoleId);
	if(it != m_mapRole2Npc.end())
	{
		dwNpcId = it->second;
	}

	return dwNpcId;
}

INT PlayerArena::GetRankIdxByRoleId(DWORD dwRoleId)
{
	INT nIdx = -1;
	std::map<DWORD, INT>::iterator it = m_mapRankIdx.find(dwRoleId);
	if(it != m_mapRankIdx.end())
	{
		nIdx = it->second;
	}

	return nIdx;
}

KDelayRoleData* PlayerArena::GetDelayDataById(DWORD dwRoleId)
{
	KDelayRoleData* pData = NULL;
	KMAP_DELAY_DATA::iterator it;

	it = m_mapDelayData.find(dwRoleId);
	PROCESS_ERROR(it != m_mapDelayData.end());

	pData = &(it->second);

EXIT0:
	return pData;
}

BOOL PlayerArena::DoGetPlayerIdxRequest(DWORD dwSrcPlayerId, DWORD dwRoleId)
{
	BOOL bRet = RankRelayClient::DoGetRankIdxRequest(emRANK_ARENA, dwSrcPlayerId, dwRoleId);
	return bRet;
}

// 玩家ID 返回排名
BOOL PlayerArena::OnGetPlayerIdxRespond(DWORD dwRoleId, INT nRankIdx)
{
	INT nCount = 0;
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	if(m_rcPlayer.GetId() == dwRoleId)
	{
		LOG_PROCESS_ERROR((nRankIdx + 1) >= 0);
		m_rcPlayer.SetArenaRank(nRankIdx + 1);

		if(!m_bFirstLogin)
		{
			m_nRankIdx = nRankIdx;
			DoLoadArenaPlayers();
			m_mapRankIdx[dwRoleId] = nRankIdx;  // 保存玩家自身排名

			if (nRankIdx < ARENA_PRE_RANK_NUM)
			{
			    SetRankPos(dwRoleId, nRankIdx);
			}
		}

		m_bFirstLogin = FALSE;
	}
	else  // 读非玩家，一般是开始战斗
	{
		INT m_nOldRankIdx = 0;
		if (!m_bFrontArenaNpcsMode)  // 非前5模式下的排名计算   
		{
			m_nOldRankIdx = m_nRankIdx < ARENA_PRE_RANK_NUM ? ARENA_PRE_RANK_NUM - m_mapRankPos[dwRoleId]: m_nRankIdx - m_mapRankPos[dwRoleId]; // 我排名 - （总数量 - 对方位置）
		}
		else
		{  // 前5名模式，
			m_nOldRankIdx = ARENA_PRE_RANK_NUM - m_mapRankPos[dwRoleId];
		}
		
		// 排名如果有变化，重新刷新一遍
		if(nRankIdx != m_nOldRankIdx)
		{
			ProcessRankChangedWhenChallenge();
		}
		else
		{
			m_dwCurrentFightRoleId = dwRoleId;
			DoFightProcess();
		}
	}
	
	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID PlayerArena::SetRankPos(DWORD dwRoleId, INT nRankIdx)
{
	time_t tNow = time(NULL);
	RankRelayClient::DoModifyRankValueRequest(emRANK_AREAN_OS, dwRoleId,nRankIdx,tNow, FALSE);
	
	// 记录角色id对应的竞技场位置(1 - 5)   排名从0开始，pos从1开始（0pos是玩家），+1
	if (!m_bFrontArenaNpcsMode) 
	{
		if (m_nRankIdx >= ARENA_PRE_RANK_NUM) // 非前五名,  位置是我的排名 - 对方排名    排名越高，Pos越小，显示是从左到右，从大到小
		{
			m_mapRankPos[dwRoleId] = m_nRankIdx - nRankIdx;
		}
		else  // 前5名，pos就是5 - 排名, 排名越大，pos越小
		{
			m_mapRankPos[dwRoleId] = ARENA_PRE_RANK_NUM - nRankIdx;
		}
	}
	else // 玩家在前五名， 其余的直接取排名+1为位置
	{
		m_mapRankPos[dwRoleId] = ARENA_PRE_RANK_NUM - nRankIdx;
	}
}

//  排名返回 玩家 ID, ongetrankid返回
BOOL PlayerArena::OnGetPlayerIdRespond(DWORD dwRoleId, INT nRankIdx)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	//ASSERT(nRankIdx < m_nRankIdx);
	PROCESS_ERROR(dwRoleId > 0);

	m_mapRankIdx[dwRoleId] = nRankIdx;
	//printf("\nOnGetPlayerIdRespond %d %d %d", m_rcPlayer.GetId(), dwRoleId, nRankIdx);
	
	SetRankPos(dwRoleId, nRankIdx);

	if(m_rcPlayer.GetId() != dwRoleId)
	{
		bRetCode = g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ARENA, m_rcPlayer.GetId(), dwRoleId);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerArena::OnSwapRankRespond(BOOL bSucceed)
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(bSucceed);

	// 刷新排名 交换排名 后不主动刷新
	//DoLoadArenaPlayers();

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerArena::CreateArenaNpc(DWORD dwRoleId, INT nPos, KDelayRoleData* pDelayData)
{
	BOOL bRetCode = FALSE;
	BOOL bResult = FALSE;
	KNpc* pNpc = NULL;
	KNpc* pPetNpc = NULL;
	KDelayRoleData sRoleData;
	KDelayPetData* pPetData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	INT nRankIdx = GetRankIdxByRoleId(dwRoleId);  // 排名
	BOOL bCanFight = TRUE;

	pNpc =  g_cOrpgWorld.AddNpc(PLAYER_NPC_TEMPLATE_ID);
	LOG_PROCESS_ERROR(pNpc);
	bRetCode = g_cArenaHelper.SetNpcWithDelayData(pNpc, pDelayData, nRankIdx);
	LOG_PROCESS_ERROR(bRetCode);


	memcpy((VOID*)&sRoleData, pDelayData, sizeof(KDelayRoleData));
	m_mapDelayData[dwRoleId] = sRoleData;

	pPetData = g_cArenaHelper.GetFightPetData(pDelayData);

	// 有宠物，添加宠物
	if (pPetData && pPetData->wType > 0)
	{
		DWORD dwNpcTemplateId = (DWORD)g_cPetSetting.GetPetNpcIdByType(pPetData->wType);
		pPetNpc = g_cOrpgWorld.AddNpc(dwNpcTemplateId);  // 根据NPC
		LOG_PROCESS_ERROR(pPetNpc);
	}

	m_mapNpc2Role[pNpc->GetId()] = dwRoleId;
	m_mapRole2Npc[dwRoleId] = pNpc->GetId();
	m_mapNpc2Pets[pNpc->GetId()] = pPetNpc != NULL ? pPetNpc->GetId() : 0;

	cSafeScript->CallTableFunction("Arena", "OnLoadPlayerComplete", 0, "ooodd", m_rcPlayer.GetScriptInterface(), pNpc->GetScriptInterface(), 
		pPetNpc != NULL ? pPetNpc->GetScriptInterface() : NULL, nPos, m_bFrontArenaNpcsMode ? 1 : 0);

	if (nPos > m_nRankIdx)  // 玩家排名=m_nRankdIdx+1,  NPC至少排名 = nPos(Pos是1开始)， 所以这里>  ；  另外可获取npc排名再对比也行
		bCanFight = FALSE;

	bRetCode = SyncAddNpc(dwRoleId, pNpc->GetId(), pPetNpc != NULL ? pPetNpc->GetId() : 0, nPos, nPos != 0);  // 0时不能fight
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerArena::OnLoadDelayRoleDataRespond(DWORD dwSrcId, DWORD dwRoleId, KDelayRoleData* pDelayData)
{
	INT nPos = 0;
	
	if(dwSrcId == dwRoleId)  // 玩家自己，从StartArena返回的
	{
		nPos = 0;
		m_dwPlayerRoleId = dwRoleId; // 主角NPC镜像的ID
		this->CreateArenaNpc(dwRoleId, nPos, pDelayData);  // 主角自己的NPC

		if (m_nRankIdx >= ARENA_PRE_RANK_NUM)  // 玩家非前5名，只同步0 pos
		{
			return TRUE;	
		} // else 前5名，继续执行，npc
	}

	nPos = m_mapRankPos[dwRoleId];
	this->CreateArenaNpc(dwRoleId, nPos, pDelayData);

	printf ("OnLoadDelayRoleDataRespond End %d %d", dwRoleId, nPos);

	return TRUE;
}

VOID PlayerArena::ProcessRankChangedWhenChallenge()
{
	if (!m_bFrontArenaNpcsMode)
		DoLoadArenaPlayers();// 提示排名变化
	else
		DoLoadFrontArenaPlayers(); 

	SyncArenaInfo();
	g_cPlayerBaseProcess.DoSyncBlackMsg(&m_rcPlayer, LANG_034);
}

// 战斗部分
BOOL PlayerArena::OnFightRequest(DWORD dwNpcId)
{
	BOOL bResult = FALSE;
	DWORD dwRoleId;

	dwRoleId = GetRoleIdByNpcId(dwNpcId);
	LOG_PROCESS_ERROR(dwRoleId > 0);

	// 客户端已作限制，如出现这种情况一般是客户端角色未刷新
	if(dwRoleId == m_rcPlayer.GetId())
	{
        ProcessRankChangedWhenChallenge();
		goto EXIT0;
	}

	DoGetPlayerIdxRequest(m_rcPlayer.GetId(), dwRoleId);  // 对方roleid

	bResult = TRUE;
EXIT0:
	return bResult;
}

INT PlayerArena::GetSkillIdOfCurrRound(INT nRound, INT nRoute)
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

DWORD PlayerArena::GetPetNpcIdByRoleNpcId(DWORD dwRoleNpcId)
{
	if (m_mapNpc2Pets.find(dwRoleNpcId) != m_mapNpc2Pets.end())
	{
		return m_mapNpc2Pets[dwRoleNpcId];
	}
	return 0;
}

BOOL PlayerArena::DoFightProcess()
{
	BOOL			bResult						= FALSE;
	KATTACK_RECORD	sRecord;
	INT				nMeLife						= 0;
	INT				nHimLife					= 0;
	INT				nAttackValue				= 0;
	INT				nPetAttackValue				= 0;
	INT				nIsCrit						= 0;
	INT				nIsMiss						= 0;
	KDelayRoleData*	pDelayData					= NULL;
	KDelayBaseData* pBaseData					= NULL;
	ScriptSafe cSafeScript						= g_cScriptManager.GetSafeScript();
	INT				nHimRankIdx					= GetRankIdxByRoleId(m_dwCurrentFightRoleId);
	INT				nMeRankIdx					= m_rcPlayer.GetArenaRank();
	//DWORD           dwMeNpcId = 0;
	//DWORD           dwHimNpcId = 0;
	//DWORD           dwMePetNpcId = 0;
	//DWORD           dwHimPetNpcId = 0;
	INT             nCurrSkillId = 0;
	INT             nRoundNum = 1;
	INT             nSelfRoute = 0;
	INT             nTargetRoute = 0;

	pDelayData = GetDelayDataById(m_dwCurrentFightRoleId);
	LOG_PROCESS_ERROR(pDelayData);
	pBaseData = &(pDelayData->sBaseRoleData);

	nSelfRoute = m_rcPlayer.GetCurrentRoute();
	nTargetRoute = pDelayData->sSkillData.nFactionRoute;

	m_RecordList.clear();

	nHimLife = pBaseData->nMaxLife;
	nMeLife	= m_rcPlayer.GetMaxLife();

    /*dwMeNpcId  = GetNpcIdByRoleId(m_rcPlayer.GetId());
	dwHimNpcId = GetNpcIdByRoleId(m_dwCurrentFightRoleId);
	dwMePetNpcId = GetPetNpcIdByRoleNpcId(dwMeNpcId);
	dwHimPetNpcId = GetPetNpcIdByRoleNpcId(dwHimNpcId);
	g_cArenaHelper.StartOfflineFight(m_rcPlayer.GetId(), m_dwCurrentFightRoleId, dwMeNpcId, dwMePetNpcId, dwHimNpcId, dwHimPetNpcId);*/

	while(TRUE)
	{
		nCurrSkillId = GetSkillIdOfCurrRound(nRoundNum, nSelfRoute);

		nAttackValue = g_cArenaHelper.GetHurtValue(&m_rcPlayer, pDelayData, TRUE, nCurrSkillId, nIsCrit, nIsMiss);
		LOG_PROCESS_ERROR(nAttackValue > 0 || nIsMiss);

		nPetAttackValue = g_cArenaHelper.GetPetHurtValue(&m_rcPlayer, pDelayData, TRUE, 7000);

		sRecord.bySkillId = (BYTE)nCurrSkillId;
		sRecord.bAttack = 1;
		sRecord.bIsCrit = (BYTE)nIsCrit;
		sRecord.bIsMiss = (BYTE)nIsMiss;
		sRecord.nHurtValue = nAttackValue;
		sRecord.nPetHurtValue = nPetAttackValue;
		m_RecordList.push_back(sRecord);

		nHimLife -= nAttackValue;
		nHimLife -= nPetAttackValue;
		if(nHimLife <= 0)
		{
			// 胜利后交换排名
			if (m_rcPlayer.GetArenaRank() != 1) // 第一名挑战别人，胜利后位置不变
			{
				LOG_PROCESS_ERROR((nHimRankIdx + 1) >= 0);
				m_rcPlayer.SetArenaRank(nHimRankIdx + 1);
				RankRelayClient::DoSwapRankRequest(emRANK_ARENA, m_rcPlayer.GetId(), m_dwCurrentFightRoleId);
				RankRelayClient::DoSwapRankRequest(emRANK_AREAN_OS, m_rcPlayer.GetId(), m_dwCurrentFightRoleId);
			}

			++m_nWinCombo;
			m_nFailCombo = 0;
			m_bFightResult = TRUE;
			
			break;
		}

		nCurrSkillId = GetSkillIdOfCurrRound(nRoundNum, nTargetRoute);

		nAttackValue = g_cArenaHelper.GetHurtValue(&m_rcPlayer, pDelayData, FALSE, nCurrSkillId, nIsCrit, nIsMiss);
		LOG_PROCESS_ERROR(nAttackValue > 0 || nIsMiss);

		nPetAttackValue = g_cArenaHelper.GetPetHurtValue(&m_rcPlayer, pDelayData, FALSE, 7000);

		sRecord.bySkillId = (BYTE)nCurrSkillId;
		sRecord.bAttack = 0;
		sRecord.bIsCrit = (BYTE)nIsCrit;
		sRecord.bIsMiss = (BYTE)nIsMiss;
		sRecord.nHurtValue = nAttackValue;
		sRecord.nPetHurtValue = nPetAttackValue;
		m_RecordList.push_back(sRecord);

		nMeLife -= nAttackValue;
		nMeLife -= nPetAttackValue;
		// 挑战失败
		if(nMeLife <= 0)
		{
			++m_nFailCombo;
			m_nWinCombo = 0;
			m_bFightResult = FALSE;

			break;
		}

		nRoundNum++;
	}

	cSafeScript->CallTableFunction("Arena", "OnChallengeEnd", 0, "odd", m_rcPlayer.GetScriptInterface(), m_bFightResult, m_nWinCombo);

	// 战斗记录
	{
		DWORD dwNpcId = GetNpcIdByRoleId(m_dwCurrentFightRoleId);
		KNpc* pNpc = g_cNpcMgr.GetById(dwNpcId);
		LOG_PROCESS_ERROR(pNpc);

		// 由于第一名可挑战排名比自己低的人，因此这里要做个判断
		INT nMaxIdx = nHimRankIdx > (nMeRankIdx - 1) ? nHimRankIdx : nMeRankIdx - 1;
		INT nMinIdx = nHimRankIdx > (nMeRankIdx - 1) ? nMeRankIdx - 1 : nHimRankIdx;
		
		// 记录自己战斗记录
		ArenaRelayClient::DoAddRecordRequest(m_rcPlayer.GetId(), TRUE, m_bFightResult, pNpc->GetName(), m_bFightResult ? nMinIdx: nMaxIdx);
		// 记录对方战斗记录
		ArenaRelayClient::DoAddRecordRequest(m_dwCurrentFightRoleId, FALSE, !m_bFightResult, m_rcPlayer.GetName(), m_bFightResult ? nMinIdx : nMaxIdx);
	}

	m_nFightNum++;

	m_nCurrChallengeCDTime = (m_nFightNum / 5) *  ARENA_CHALLENGE_CD_TIME;
	if (m_nFightNum > 0 && m_nFightNum % 5 == 0) // 每五次cd时间加5分钟
	{
		m_nCDTime = m_nCurrChallengeCDTime;
	}

	SyncFightProcess();

	SyncArenaInfo();
	
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerArena::SyncFightProcess()
{
	INT									nIdx		= 0;
	IMemBlock*							piMem		= NULL;
	BOOL								bResult		= FALSE;
	BOOL								bRetCode	= FALSE;
	KS2C_SYNC_FIGHT_PROCESS*			pSync		= NULL;
	size_t								uPakSize	= 0;
	KDelayRoleData*						pMeDelayData	= NULL;
	KDelayPetData*                      pMePetData      = NULL;
	KDelayRoleData*						pHimDelayData	= NULL;
	KDelayPetData*						pHimPetData = NULL;
	std::list<KATTACK_RECORD>::iterator	it;

	LOG_PROCESS_ERROR(m_RecordList.size() > 0);

	uPakSize = sizeof(KS2C_SYNC_FIGHT_PROCESS) + m_RecordList.size() * sizeof(KATTACK_RECORD);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);
	pSync = (KS2C_SYNC_FIGHT_PROCESS*)piMem->GetData();

	pHimDelayData = GetDelayDataById(m_dwCurrentFightRoleId);
	LOG_PROCESS_ERROR(pHimDelayData);

	pMeDelayData = GetDelayDataById(m_dwPlayerRoleId);
	LOG_PROCESS_ERROR(pMeDelayData);

	pSync->byProtocol = s2c_sync_fight_process;
	pSync->bResult = m_bFightResult;
	pSync->dwFightNpcId = GetNpcIdByRoleId(m_dwCurrentFightRoleId);

	pMePetData = g_cArenaHelper.GetFightPetData(pMeDelayData);
	if (pMePetData)
	{
		pSync->nMePetSkillId = pMePetData->arySkillIds[0];
	}

	if (m_mapNpc2Pets.find(pSync->dwFightNpcId) != m_mapNpc2Pets.end())
	{
        pSync->nHimPetNpcId = m_mapNpc2Pets[pSync->dwFightNpcId];
	}
	else
	{
		pSync->nHimPetNpcId = 0;
	}
	
	pSync->nHimPetSkillId = 0;
	pHimPetData = g_cArenaHelper.GetFightPetData(pHimDelayData);
	if(pHimPetData)
	{
		pSync->nHimPetSkillId = pHimPetData->arySkillIds[0];
	}
	
	pSync->nRecordCount = m_RecordList.size();

	for(it = m_RecordList.begin(); it != m_RecordList.end(); ++it)
	{
		memcpy((VOID*)&(pSync->arrRecord[nIdx]), &(*it), sizeof(KATTACK_RECORD));
		++nIdx;
	}

	bRetCode = g_ArenaModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;

EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}


BOOL PlayerArena::SyncArenaInfo()
{
	INT									nChallengeTimes		= 0;
	KS2C_SYNC_ARENA_INFO				sSync;
	BOOL								bResult				= FALSE;
	BOOL								bRetCode			= FALSE;
	std::vector<DWORD>					vecRankAwardIds;
	KAwardCtrl*							pAwardCtrl			= NULL;
	ScriptSafe							cSafeScript			= g_cScriptManager.GetSafeScript();

	cSafeScript->CallTableFunction("Arena", "GetChallengeTimes", 1, "o", m_rcPlayer.GetScriptInterface());
	nChallengeTimes = cSafeScript->GetInt(-1);

	// 如挑战次数等于默认，说明已过结算时间，战斗次数也必须随着清0
	if (nChallengeTimes == ARENA_CHALLENGE_DEFAULT_TIME)
	{
		m_nFightNum = 0;
		m_nCurrChallengeCDTime = 0;
	}

	pAwardCtrl = m_rcPlayer.GetCtrl<KAwardCtrl>();
	LOG_PROCESS_ERROR(pAwardCtrl);

	sSync.byProtocol = s2c_sync_arena_info;
	sSync.byChallengeTimes = (BYTE)nChallengeTimes;
	sSync.byWinCombo = (BYTE)m_nWinCombo;
	sSync.dwCDOverTime = (DWORD)(m_nCDTime / GAME_FPS) + KSysService::Time(NULL);   // CD结束时间戳 = 现在时间(unix timestamp) + CD秒数  , 需要转换秒数
	sSync.byComboAward = (BYTE)m_nTakeComboAward;
	sSync.dwCurRankAwardId = 0;
	sSync.dwCurWinComboAwardId = 0;

	bRetCode = pAwardCtrl->GetAwardListByModuleName(vecRankAwardIds, "ArenaRank");
	LOG_PROCESS_ERROR(bRetCode);
	if(vecRankAwardIds.size())
	{
		sSync.dwCurRankAwardId = vecRankAwardIds[0];
	}

	bRetCode = pAwardCtrl->GetAwardListByModuleName(vecRankAwardIds, "Arena");
	LOG_PROCESS_ERROR(bRetCode);
	if(vecRankAwardIds.size())
	{
		sSync.dwCurWinComboAwardId = vecRankAwardIds[(INT)vecRankAwardIds.size() - 1];
	}

	bRetCode = g_ArenaModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerArena::SyncAddNpc(DWORD dwRoleId, DWORD dwNpcId, DWORD dwPetNpcId, INT nPos, BOOL bCanFight)
{
	KS2C_ARENA_ADD_NPC					sSync;
	BOOL								bResult				= FALSE;
	BOOL								bRetCode			= FALSE;

	sSync.dwMappingRoleId = dwRoleId;
	sSync.dwNpcId = dwNpcId;
	sSync.dwPetNpcId = dwPetNpcId;
	sSync.byPos = (BYTE)nPos;
	sSync.byCanFight = (BYTE)bCanFight;

	bRetCode = g_ArenaModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	printf("\nSyncAddNpc %u %u %d", dwRoleId, dwNpcId, nPos);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerArena::OnLoadRecordRespond(BYTE* pRecordData, size_t uRecordDataLen)
{
	BOOL		bResult = TRUE;
	BYTE*		pBuffer = pRecordData;
	size_t		uPakSize = uRecordDataLen;

	m_vecArenaRecord.clear();

	while(uPakSize >= sizeof(KArenaRecord))
	{
		KArenaRecord sRecord;
		memcpy((VOID*)&sRecord, pBuffer, sizeof(KArenaRecord));
		m_vecArenaRecord.push_back(sRecord);
		pBuffer += sizeof(KArenaRecord);
		uPakSize -= sizeof(KArenaRecord);
	}

	LOG_PROCESS_ERROR(uPakSize == 0);

	if (!m_bCheckedAreaRecordFightResult)
	{
		if (!m_vecArenaRecord.empty())
		{
			INT nLastLogoutTime = m_rcPlayer.GetLastLogoutTime();

			// 在玩家下线后只要有一条记录不是胜利,清空连胜数
			for (INT i = 0; i < (INT)m_vecArenaRecord.size(); i++)
			{
				KArenaRecord sRecord = m_vecArenaRecord[i];
				if (!sRecord.bResult && sRecord.dwTimestamp >= (DWORD)nLastLogoutTime)
				{
					m_nWinCombo = 0;
					break;
				}
			}
		}

		m_bCheckedAreaRecordFightResult = TRUE;
	}

	SyncArenaRecord();

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerArena::SyncArenaRecord()
{
	INT										nIdx		= 0;
	IMemBlock*								piMem		= NULL;
	BOOL									bResult		= FALSE;
	BOOL									bRetCode	= FALSE;
	KS2C_SYNC_ARENA_RECORD*					pSync		= NULL;
	size_t									uPakSize	= 0;
	std::vector<KArenaRecord>::iterator		it;

	PROCESS_ERROR(m_vecArenaRecord.size() > 0);

	uPakSize = sizeof(KS2C_SYNC_ARENA_RECORD) + m_vecArenaRecord.size() * sizeof(KArenaRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);
	pSync = (KS2C_SYNC_ARENA_RECORD*)piMem->GetData();

	pSync->byProtocol = s2c_sync_arena_record;
	pSync->nRecordNums = (INT)m_vecArenaRecord.size();
	for(it = m_vecArenaRecord.begin(); it != m_vecArenaRecord.end(); ++it)
	{
		KArenaRecord* pRecord = &(*it);
		memcpy((VOID*)&(pSync->arrRecord[nIdx++]), pRecord, sizeof(KArenaRecord));
	}

	bRetCode = g_ArenaModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}