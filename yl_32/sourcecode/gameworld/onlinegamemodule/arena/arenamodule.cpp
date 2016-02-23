#include "stdafx.h"
#include "arenamodule.h"
#include "arenaprotocol.h"
#include "onlinegameworld/kplayermgr.h"
#include "playerarena.h"
#include "rank/RankRelayClient.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "arena/ArenaHelper.h"
#include "onlinegamebase/ksysservice.h"
#include "Runtime/TypeDef.h"

ArenaModule g_ArenaModule;

template<>IKModuleInterface* GetModuleInterFace<emMODULE_ARENA>()
{
	return &g_ArenaModule;
}


ArenaModule::ArenaModule() : KModuleBase(emMODULE_ARENA)
{

}

ArenaModule::~ArenaModule()
{
}


BOOL ArenaModule::OnInit()
{
	return TRUE;
}

BOOL ArenaModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_ARENA_INFO);
	KD_PTC_FIXED_SIZE_S2C(KS2C_ARENA_ADD_NPC);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_FIGHT_PROCESS);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_ARENA_RECORD);

	KD_PTC_FIXED_SIZE_C2S(KC2S_BEGIN_FIGHT_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(QC2S_REFRESH_ARENA_PLAYERS);
	KD_PTC_FIXED_SIZE_C2S(KC2S_START_OFFLINE_FIGHT_REQUEST);

	return TRUE;
}

BOOL ArenaModule::OnUnInit()
{
	return TRUE;
}

BOOL ArenaModule::Breath()
{
	return TRUE;
}

BOOL ArenaModule::c2sRefreshArenaPlayers(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	QC2S_REFRESH_ARENA_PLAYERS*	pSync						= (QC2S_REFRESH_ARENA_PLAYERS*)pProtocol;

	PlayerArena* pPlayerArena = pPlayer->GetCtrl<PlayerArena>();
	LOG_PROCESS_ERROR(pPlayerArena);

	pPlayerArena->RefreshArenaPlayers();
	pPlayerArena->SyncArenaInfo();
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL ArenaModule::c2sOnFightRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_BEGIN_FIGHT_REQUEST*	pSync						= (KC2S_BEGIN_FIGHT_REQUEST*)pProtocol;
	ScriptSafe					cSafeScript					= g_cScriptManager.GetSafeScript();

	cSafeScript->CallTableFunction("Arena", "OnChallenge", 0, "od", pPlayer->GetScriptInterface(), pSync->dwNpcId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL ArenaModule::c2sOnOfflineFightRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_START_OFFLINE_FIGHT_REQUEST*	pSync						= (KC2S_START_OFFLINE_FIGHT_REQUEST*)pProtocol;

	g_cArenaHelper.StartOfflineFight(pSync->dwStartPlayerId, pSync->dwOfflinePlayerId, pSync->dwStartRoleNpcId, pSync->dwStartPetNpcId, pSync->dwOfflineRoleNpcId, pSync->dwOfflinePetNpcId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL ArenaModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	KPTC_HEADER_BASE* pData = (KPTC_HEADER_BASE*)pbData;
	QCONFIRM_RET_FALSE(pData);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	switch (pData->byProtocol)
	{
	case c2s_begin_fight_request:
		c2sOnFightRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_refresh_arena_players:
		c2sRefreshArenaPlayers(pPlayer, pbData, uDataLen);
		break;
	case c2s_start_offline_fight_request:
		c2sOnOfflineFightRequest(pPlayer, pbData, uDataLen);
		break;
	default:
		ASSERT(FALSE);
	}

	return TRUE;
}

BOOL ArenaModule::OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId)
{
	BOOL bResult = FALSE;
	KDelayRoleData sDelayData;
	PlayerArena* pPlayerArena = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwSrcId);
	PROCESS_ERROR(pPlayer);

	memset(&sDelayData, 0, sizeof(KDelayRoleData));
	LOG_PROCESS_ERROR(sizeof(KDelayRoleData) == piBuffer->GetSize());
    memcpy(&sDelayData, piBuffer->GetData(), piBuffer->GetSize());

	pPlayerArena = pPlayer->GetCtrl<PlayerArena>();
	pPlayerArena->OnLoadDelayRoleDataRespond(dwSrcId, dwRoleId, &sDelayData);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL ArenaModule::OnGetRankIdxRespond(DWORD dwSrcId, DWORD dwPlayerId, INT nRankIdx)
{
	BOOL bResult = FALSE;
	PlayerArena* pPlayerArena = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwSrcId);
	PROCESS_ERROR(pPlayer);

	pPlayerArena = pPlayer->GetCtrl<PlayerArena>();
	pPlayerArena->OnGetPlayerIdxRespond(dwPlayerId, nRankIdx);
	
	//RankRelayClient::DoModifyRankValueRequest(emRANK_AREAN_OS, rcCharacter.GetId(), rcCharacter.GetFightScore(), tNow, FALSE);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL ArenaModule::OnGetRankIdRespond(DWORD dwSrcId, DWORD dwRoleId, INT nRankIdx)
{
	BOOL bResult = FALSE;
	PlayerArena* pPlayerArena = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwSrcId);
	PROCESS_ERROR(pPlayer);

	pPlayerArena = pPlayer->GetCtrl<PlayerArena>();
	pPlayerArena->OnGetPlayerIdRespond(dwRoleId, nRankIdx);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL ArenaModule::OnSwapRankRespond(std::string szSrcPlayerName, INT nSrcRank, DWORD dwDestId, INT nDestRank, BOOL bSucceed)
{
	BOOL bResult = FALSE;
	PlayerArena* pPlayerArena = NULL;
	IKScene* pInScene = NULL;
	DWORD dwTemplateId = 0;
	time_t tNow = KSysService::Time(NULL);

	// 设置下对方的排名信息
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwDestId);
	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR((nDestRank + 1) >= 0);
	pPlayer->SetArenaRank(nDestRank + 1);

	pInScene = pPlayer->GetScene();
	PROCESS_ERROR(pInScene);
    dwTemplateId = pInScene->GetTemplateId();
	if (dwTemplateId != ARENA_MAP_ID) // 不在竞技场时提示对方被击败
	{
		// 提示对方被击败
		CHAR szTempRemindStr[_MAX_PATH];
		sprintf(szTempRemindStr, LANG_030, szSrcPlayerName.c_str(), nDestRank - nSrcRank);
		g_ChatChannelManager.RequestSendMsg(trLocalSys, 0, dwDestId, "", szTempRemindStr, emQCHAT_PANEL);
	}

	// 连胜清0
	pPlayerArena = pPlayer->GetCtrl<PlayerArena>();
	LOG_PROCESS_ERROR(pPlayerArena);
	pPlayerArena->SetWinCombo(0);
	RankRelayClient::DoModifyRankValueRequest(emRANK_AREAN_OS, pPlayer->GetPlayerId(),nDestRank, tNow, FALSE);
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL ArenaModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData )
{
	return g_ArenaModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL ArenaModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_ArenaModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}

BOOL ArenaModule::SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize )
{
	return g_ArenaModule.GetProtocolProcessor()->SendUnknownData(nPlayerIndex, pData, uDataSize);
}