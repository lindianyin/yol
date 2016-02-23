

#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#include "kescortmodule.h"
#include "kescortctrl.h"
#include "kescortprotocol.h"
#include "kescortrelayclient.h"

KEscortModule g_cEscortModule;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
template<>
IKModuleInterface* GetModuleInterFace<emMODULE_ESCORT>()
{
	return &g_cEscortModule;
}

KEscortModule::KEscortModule() : KModuleBase(emMODULE_ESCORT)
{}

BOOL KEscortModule::OnInit()
{
	return TRUE;
}

BOOL KEscortModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(KC2S_REFRESH_ESCORTS_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_START_ESCORT_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_TAKE_ESCORT_AWARD_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_ROB_ESCORT_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_DEL_ROB_ESCORT_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_TAKE_ROB_AWARD_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_ROB_FIGHT_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_RESCUE_FIGHT_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_RESCUE_PLAYER_REQUEST);

	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_ESCORT_MEMBERS);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_ESCORT_MEMBERS_END);
	KD_PTC_FIXED_SIZE_S2C(KS2C_ESCORT_ADD_NPC);
	KD_PTC_FIXED_SIZE_S2C(KS2C_ESCORT_REMOVE_NPC);
	KD_PTC_FIXED_SIZE_S2C(KS2C_ESCORT_ADD_SELF_NPC);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_ROB_MEMBERS);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_ROB_MEMBERS_END);
	KD_PTC_FIXED_SIZE_S2C(KS2C_UPDATE_ESCORT_STATE);

	return TRUE;
}

BOOL KEscortModule::OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId)
{
	BOOL bResult = FALSE;
	KDelayRoleData sDelayData;
	KEscortCtrl* pEscortCtrl = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwSrcId);
	PROCESS_ERROR(pPlayer);

	memset(&sDelayData, 0, sizeof(KDelayRoleData));
	memcpy(&sDelayData, piBuffer->GetData(), piBuffer->GetSize());

	pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
	pEscortCtrl->OnLoadDelayRoleDataRespond(dwSrcId, dwRoleId, &sDelayData);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);
	
	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_refresh_escorts_request:
		QLogPrintf(LOG_INFO,"..c2s_refresh_escorts_request..");
		c2sRefreshEscorts(pPlayer, pbData, uDataLen);
		break;
	case c2s_start_escort_request:
		QLogPrintf(LOG_INFO,"..c2s_start_escort_request..");
		c2sStartEscort(pPlayer, pbData, uDataLen);
		break;
	case c2s_take_escort_award_request:
		QLogPrintf(LOG_INFO,"..c2s_take_escort_award_request..");
		c2sTakeEscortAward(pPlayer, pbData, uDataLen);
		break;
	case c2s_rob_escort_request:
		QLogPrintf(LOG_INFO,"..c2s_rob_escort_request..");
		c2sRobEscortRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_del_rob_escort_request:
		QLogPrintf(LOG_INFO,"..c2s_del_rob_escort_request..");
		c2sDelRobEscortRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_take_rob_award_request:
		QLogPrintf(LOG_INFO,"..c2s_take_rob_award_request..");
		c2sTakeRobAwardRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_rob_fight_request:
		QLogPrintf(LOG_INFO,"..c2s_rob_fight_request..");
		c2sRobFightRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_rescue_fight_request:
		QLogPrintf(LOG_INFO,"..c2s_rescue_fight_request..");
		c2sRescueFightRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_rescue_player_request:
		QLogPrintf(LOG_INFO,"..c2s_rescue_player_request..");
		c2sRescuePlayerRequest(pPlayer, pbData, uDataLen);
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL KEscortModule::c2sRefreshEscorts(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_REFRESH_ESCORTS_REQUEST*	pSync					= (KC2S_REFRESH_ESCORTS_REQUEST*)pProtocol;
	KEscortCtrl* pEscortCtrl = NULL;

	pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
	LOG_PROCESS_ERROR(pEscortCtrl);

	pEscortCtrl->SetCurEscortPage(pSync->byPage);
	
	if(pSync->byRefresh)
	{
		pEscortCtrl->ClearNpcs();
		pEscortCtrl->ClearEscortData(FALSE);
	}

	KEscortRelayClient::DoRefreshPlayerEscorts(pPlayer->GetId());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortModule::c2sTakeEscortAward(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL bResult	= FALSE;
	KC2S_TAKE_ESCORT_AWARD_REQUEST*	pSync = (KC2S_TAKE_ESCORT_AWARD_REQUEST*)pProtocol;

	KEscortRelayClient::DoTakeEscortAwardRequest(pPlayer->GetId());

	bResult = TRUE;
EXIT0:
	return bResult;
}


BOOL KEscortModule::c2sStartEscort(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_START_ESCORT_REQUEST*	pSync					= (KC2S_START_ESCORT_REQUEST*)pProtocol;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	INT nRet = 0;

	// 次数检查
	cSafeScript->CallTableFunction("Escort", "OnStartEscort", 1, "o", pPlayer->GetScriptInterface());
	nRet = cSafeScript->GetInt(-1);

	LOG_PROCESS_ERROR(nRet);

	KEscortRelayClient::DoStartEscortRequest(pPlayer->GetId(), pSync->nProtectRoleId, pPlayer->GetEscortType());

	pPlayer->SetEscortType(1);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortModule::c2sRobEscortRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_ROB_ESCORT_REQUEST*	pSync					= (KC2S_ROB_ESCORT_REQUEST*)pProtocol;

	KEscortRelayClient::DoRobPlayerEscortRequest(pPlayer->GetId(), pSync->nRobPlayerId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortModule::c2sDelRobEscortRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_DEL_ROB_ESCORT_REQUEST*	pSync					= (KC2S_DEL_ROB_ESCORT_REQUEST*)pProtocol;

	KEscortRelayClient::DoDelRobEscortRequest(pSync->nPlayerId, pSync->nRobPlayerId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortModule::c2sTakeRobAwardRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_TAKE_ROB_AWARD_REQUEST*	pSync					= (KC2S_TAKE_ROB_AWARD_REQUEST*)pProtocol;

	KEscortRelayClient::DoTakeRobAwardRequest(pPlayer->GetId(), pSync->nRobPlayerId, pSync->byRobEscortLevel);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortModule::c2sRobFightRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_ROB_FIGHT_REQUEST*	pSync					= (KC2S_ROB_FIGHT_REQUEST*)pProtocol;
	KEscortCtrl* pEscortCtrl = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	INT nRet = 0;

	// 次数检查
	cSafeScript->CallTableFunction("Escort", "OnStartRobFight", 1, "o", pPlayer->GetScriptInterface());
	nRet = cSafeScript->GetInt(-1);

	LOG_PROCESS_ERROR(nRet);

	pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
	LOG_PROCESS_ERROR(pEscortCtrl);

	pEscortCtrl->OnRobFightRequest(pSync->nRobPlayerId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortModule::c2sRescueFightRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_RESCUE_FIGHT_REQUEST*	pSync					= (KC2S_RESCUE_FIGHT_REQUEST*)pProtocol;
	KEscortCtrl* pEscortCtrl = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	INT nRet = 0;

	// 次数检查
	cSafeScript->CallTableFunction("Escort", "OnStartRescueFight", 1, "o", pPlayer->GetScriptInterface());
	nRet = cSafeScript->GetInt(-1);

	LOG_PROCESS_ERROR(nRet);

	pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
	LOG_PROCESS_ERROR(pEscortCtrl);

	pEscortCtrl->OnRescueFightRequest(pSync->nRobPlayerId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortModule::c2sRescuePlayerRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KC2S_RESCUE_PLAYER_REQUEST*	pSync					= (KC2S_RESCUE_PLAYER_REQUEST*)pProtocol;

	KEscortRelayClient::DoRescuePlayerRequest(pSync->nPlayerId, pPlayer->GetId());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KEscortModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData )
{
	return g_cEscortModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KEscortModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cEscortModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}