
#include "stdafx.h"
#include "kescortrelayclient.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "../onlinegameworld/KRelayClient.h"
#include "onlinegameworld/kplayermgr.h"
#include "kescortctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KEscortRelayClient::KEscortRelayClient(void)
{
}

KEscortRelayClient::~KEscortRelayClient(void)
{
}

BOOL KEscortRelayClient::DoStartEscortRequest(INT nPlayerId, INT nProtectRoleId, INT nEscortLevel)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_START_ESCORT_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_START_ESCORT_REQUEST));
	pRequest = (S2R_START_ESCORT_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_start_escort_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->dwProtectRoleId = nProtectRoleId;
	pRequest->nEscortLevel = nEscortLevel;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortRelayClient::DoRefreshPlayerEscorts(INT nPlayerId)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_REFRESH_ESCORTS_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_REFRESH_ESCORTS_REQUEST));
	pRequest = (S2R_REFRESH_ESCORTS_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_refresh_escorts_request;
	pRequest->nPlayerId = nPlayerId;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortRelayClient::DoTakeEscortAwardRequest(INT nPlayerId)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_TAKE_ESCORT_AWARD_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_TAKE_ESCORT_AWARD_REQUEST));
	pRequest = (S2R_TAKE_ESCORT_AWARD_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_take_escort_award_request;
	pRequest->nPlayerId = nPlayerId;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortRelayClient::DoRobPlayerEscortRequest(INT nPlayerId, INT nRobPlayerId)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_ROB_PLAYER_ESCORT_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_ROB_PLAYER_ESCORT_REQUEST));
	pRequest = (S2R_ROB_PLAYER_ESCORT_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_rob_player_escort_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->nRobPlayerId = nRobPlayerId;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortRelayClient::DoDelRobEscortRequest(INT nPlayerId, INT nRobPlayerId)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_DEL_ROB_ESCORT_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_DEL_ROB_ESCORT_REQUEST));
	pRequest = (S2R_DEL_ROB_ESCORT_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_del_rob_escort_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->nRobPlayerId = nRobPlayerId;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortRelayClient::DoTakeRobAwardRequest(INT nPlayerId, INT nRobPlayerId, BYTE byEscortLevel)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_TAKE_ROB_AWARD_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_TAKE_ROB_AWARD_REQUEST));
	pRequest = (S2R_TAKE_ROB_AWARD_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_take_rob_award_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->nRobPlayerId = nRobPlayerId;
	pRequest->byEscortLevel = byEscortLevel;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortRelayClient::DoRemoveEscortWatchRequest(INT nPlayerId)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_REMOVE_ESCORT_WATCH_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_REMOVE_ESCORT_WATCH_REQUEST));
	pRequest = (S2R_REMOVE_ESCORT_WATCH_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_escort_remove_watch_request;
	pRequest->nPlayerId = nPlayerId;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortRelayClient::DoSpeedEscortRequest(INT nPlayerId, BOOL bSpeedUp)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_SPEED_UP_ESCORT*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_SPEED_UP_ESCORT));
	pRequest = (S2R_SPEED_UP_ESCORT*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_speedup_escort_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->bySpeed = (BYTE)bSpeedUp;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortRelayClient::DoRescuePlayerRequest(INT nPlayerId, INT nRescuePlayerId)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_RESCUE_PLAYER_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_RESCUE_PLAYER_REQUEST));
	pRequest = (S2R_RESCUE_PLAYER_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_rescue_player_request;
	pRequest->nPlayerId = nPlayerId;
	pRequest->nRescuePlayerId = nRescuePlayerId;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

VOID KEscortRelayClient::OnSyncEscortMembers(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	KEscortCtrl* pEscortCtrl = NULL;
	R2S_SYNC_ESCORT_MEMBERS* pRespond = NULL;

	pRespond = (R2S_SYNC_ESCORT_MEMBERS*)pbyData;
	LOG_PROCESS_ERROR(pRespond);

	pPlayer = g_cPlayerMgr.GetById(pRespond->nPlayerId);
	LOG_PROCESS_ERROR(pPlayer);

	pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
	LOG_PROCESS_ERROR(pEscortCtrl);

	pEscortCtrl->OnSyncEscortPlayers(pRespond->nMembersNum, pRespond->aryEscorts);

EXIT0:
	return;
}

VOID KEscortRelayClient::OnUpdateEscortState(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	KEscortCtrl* pEscortCtrl = NULL;
	R2S_UPDATE_ESCORT_STATE* pRespond = NULL;

	pRespond = (R2S_UPDATE_ESCORT_STATE*)pbyData;
	LOG_PROCESS_ERROR(pRespond);

	pPlayer = g_cPlayerMgr.GetById(pRespond->nSrcId);
	LOG_PROCESS_ERROR(pPlayer);

	pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
	LOG_PROCESS_ERROR(pEscortCtrl);

	pEscortCtrl->OnUpdatePlayerEscortState(pRespond->nPlayerId, pRespond->byEscortState, pRespond->nLeftTime, pRespond->dwRoberId, pRespond->dwRoberFightscore, pRespond->szRoberName);

EXIT0:
	return;
}

VOID KEscortRelayClient::OnSyncRobEscortMembers(BYTE* pbyData, size_t uDataLen)
{
	
	KPlayer* pPlayer = NULL;
	KEscortCtrl* pEscortCtrl = NULL;
	R2S_SYNC_PLAYER_ROB_ESCORTS* pRespond = NULL;

	pRespond = (R2S_SYNC_PLAYER_ROB_ESCORTS*)pbyData;
	LOG_PROCESS_ERROR(pRespond);

	pPlayer = g_cPlayerMgr.GetById(pRespond->nPlayerId);
	LOG_PROCESS_ERROR(pPlayer);

	pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
	LOG_PROCESS_ERROR(pEscortCtrl);

	pEscortCtrl->OnSyncRobEscortPlayers(pRespond->nRobNum, pRespond->aryRobEscorts);

EXIT0:
	return;
}

VOID KEscortRelayClient::OnSyncEscortState(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	R2S_SYNC_ESCORT_STATE* pSync = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	pSync = (R2S_SYNC_ESCORT_STATE*)pbyData;
	LOG_PROCESS_ERROR(pSync);

	pPlayer = g_cPlayerMgr.GetById(pSync->nPlayerId);
	LOG_PROCESS_ERROR(pPlayer);

	pPlayer->SetEscortState(pSync->byState);

	if(pSync->byState == emESCORT_STATE_COMPLETE_BEHIT || pSync->byState == emESCORT_STATE_COMPLETE_NORMAL)
	{
		cSafeScript->CallTableFunction("Escort", "OnFinishEscort", 0, "dd", pSync->nProtecterId, pSync->byEscortLevel);
	}

EXIT0:
	return;
}

VOID KEscortRelayClient::OnTakeEscortAwardRespond(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	R2S_TAKE_ESCORT_AWARD_RESPOND* pSync = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	pSync = (R2S_TAKE_ESCORT_AWARD_RESPOND*)pbyData;
	LOG_PROCESS_ERROR(pSync);

	pPlayer = g_cPlayerMgr.GetById(pSync->nPlayerId);
	LOG_PROCESS_ERROR(pPlayer);

	PROCESS_ERROR(pSync->bySuccess);

	//cSafeScript->CallTableFunction("Escort", "OnTakeEscortAward", 0, "odd", pPlayer->GetScriptInterface(), pSync->byEscortState, pSync->byEscortLevel);

EXIT0:
	return;
}

VOID KEscortRelayClient::OnTakeRobAwardRespond(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	R2S_TAKE_ROBESCORT_AWARD_RESPOND* pSync = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	pSync = (R2S_TAKE_ROBESCORT_AWARD_RESPOND*)pbyData;
	LOG_PROCESS_ERROR(pSync);

	pPlayer = g_cPlayerMgr.GetById(pSync->nPlayerId);
	LOG_PROCESS_ERROR(pPlayer);

	PROCESS_ERROR(pSync->bySuccess);

	cSafeScript->CallTableFunction("Escort", "OnTakeRobAward", 0, "od", pPlayer->GetScriptInterface(), pSync->byEscortLevel);

EXIT0:
	return;
}

VOID KEscortRelayClient::OnRescuePlayerRespond(BYTE* pbyData, size_t uDataLen)
{
	KPlayer* pPlayer = NULL;
	R2S_RESCUE_PLAYER_RESPOND* pSync = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	pSync = (R2S_RESCUE_PLAYER_RESPOND*)pbyData;
	LOG_PROCESS_ERROR(pSync);

	pPlayer = g_cPlayerMgr.GetById(pSync->nRescuePlayerId);
	LOG_PROCESS_ERROR(pPlayer);

	cSafeScript->CallTableFunction("Escort", "OnRescuePlayerRespond", 0, "odsd", pPlayer->GetScriptInterface(), pSync->nPlayerId, pSync->szPlayerName, pSync->bCanResue);

EXIT0:
	return;
}

BOOL KEscortRelayClient::Send( IMemBlock* piBuffer )
{
	return g_RelayClient.Send(piBuffer);
}