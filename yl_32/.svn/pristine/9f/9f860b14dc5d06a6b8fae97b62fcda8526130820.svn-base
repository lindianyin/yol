#include "stdafx.h"
#include "kescorthandler.h"
#include "GameCenter.h"

VOID KEscortHandler::OnStartEscortRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	S2R_START_ESCORT_REQUEST*	pRequest	= (S2R_START_ESCORT_REQUEST*)pbyData;

	g_pGameCenter->m_EscortManager.OnStartEscortRequest(pRequest->nPlayerId, pRequest->dwProtectRoleId, pRequest->nEscortLevel);

EXIT0:
	return;
}

VOID KEscortHandler::OnRefreshEscortsRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	S2R_REFRESH_ESCORTS_REQUEST*	pRequest	= (S2R_REFRESH_ESCORTS_REQUEST*)pbyData;

	g_pGameCenter->m_EscortManager.AddPlayerToWatchTable(pRequest->nPlayerId);

	bRetCode = DoSyncEscortMembers(nConnIndex, pRequest->nPlayerId);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = DoSyncRobEscortMembers(nConnIndex, pRequest->nPlayerId);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

VOID KEscortHandler::OnRemovePlayerFromWatchTable(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	S2R_REMOVE_ESCORT_WATCH_REQUEST*	pRequest	= (S2R_REMOVE_ESCORT_WATCH_REQUEST*)pbyData;

	g_pGameCenter->m_EscortManager.RemovePlayerFromWatchTable(pRequest->nPlayerId);

EXIT0:
	return;
}

VOID KEscortHandler::OnTakeEscortAwardRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	S2R_TAKE_ESCORT_AWARD_REQUEST*	pRequest	= (S2R_TAKE_ESCORT_AWARD_REQUEST*)pbyData;

	g_pGameCenter->m_EscortManager.OnTakeEscortAwardRequest(pRequest->nPlayerId);

EXIT0:
	return;
}

VOID KEscortHandler::OnRobPlayerEscortRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	S2R_ROB_PLAYER_ESCORT_REQUEST*	pRequest	= (S2R_ROB_PLAYER_ESCORT_REQUEST*)pbyData;

	g_pGameCenter->m_EscortManager.OnRobEscortRequest(pRequest->nPlayerId, pRequest->nRobPlayerId);

EXIT0:
	return;
}

VOID KEscortHandler::OnDelRobEscortRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	S2R_DEL_ROB_ESCORT_REQUEST*	pRequest	= (S2R_DEL_ROB_ESCORT_REQUEST*)pbyData;

	g_pGameCenter->m_EscortManager.OnDelRobEscortRequest(pRequest->nPlayerId, pRequest->nRobPlayerId);

EXIT0:
	return;
}

VOID KEscortHandler::OnTakeRobAwardRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	S2R_TAKE_ROB_AWARD_REQUEST*	pRequest	= (S2R_TAKE_ROB_AWARD_REQUEST*)pbyData;

	g_pGameCenter->m_EscortManager.OnTakeRobAwardRequest(pRequest->nPlayerId, pRequest->nRobPlayerId, pRequest->byEscortLevel);

EXIT0:
	return;
}

VOID KEscortHandler::OnSpeedUpRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	S2R_SPEED_UP_ESCORT*	pRequest	= (S2R_SPEED_UP_ESCORT*)pbyData;

	g_pGameCenter->m_EscortManager.OnSpeedUpEscortRequest(pRequest->nPlayerId, pRequest->bySpeed);

EXIT0:
	return;
}

VOID KEscortHandler::OnRescuePlayerRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	QLogPrintf(LOG_INFO,"..KEscortHandler::OnRescuePlayerRequest..");
	BOOL						bRetCode	= FALSE;
	S2R_RESCUE_PLAYER_REQUEST*	pRequest	= (S2R_RESCUE_PLAYER_REQUEST*)pbyData;
	R2S_RESCUE_PLAYER_RESPOND*	pRespond = NULL;
	IMemBlock*					piPackage = NULL;
	KRole* pRole = NULL;
	KPlayerEscort sEscort;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	piPackage = QCreateMemBlock(sizeof(R2S_RESCUE_PLAYER_RESPOND));
	pRespond = (R2S_RESCUE_PLAYER_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_rescue_player_respond;
	pRespond->nPlayerId = pRequest->nPlayerId;
	strncpy(pRespond->szPlayerName, pRole->GetPlayerName(), _NAME_LEN);
	pRespond->nRescuePlayerId = pRequest->nRescuePlayerId;

	bRetCode = g_pGameCenter->m_EscortManager.GetPlayerEscortInfo(pRequest->nPlayerId, sEscort);
	if(!bRetCode)
	{
		pRespond->bCanResue = FALSE;
	}
	else
	{
		pRespond->bCanResue = (sEscort.byEscortState == emESCORT_STATE_BEHIT);
	}

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}

BOOL KEscortHandler::DoSyncEscortMembers(INT nConnIndex, INT nPlayerId)
{
	QLogPrintf(LOG_INFO,"..KEscortHandler::DoSyncEscortMembers..");
	BOOL						bRetCode = FALSE;
	BOOL						bResult	 = FALSE;
	R2S_SYNC_ESCORT_MEMBERS*	pRespond = NULL;
	IMemBlock*					piPackage = NULL;
	INT nMembersNum = 0;

	nMembersNum = g_pGameCenter->m_EscortManager.GetEscortMembersNum();

	piPackage = QCreateMemBlock(sizeof(R2S_SYNC_ESCORT_MEMBERS) + nMembersNum * sizeof(KPlayerEscort));
	pRespond = (R2S_SYNC_ESCORT_MEMBERS*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_sync_escort_members;
	pRespond->nPlayerId = nPlayerId;
	pRespond->nMembersNum = nMembersNum;

	bRetCode = g_pGameCenter->m_EscortManager.SerializeMemberEscorts((LPBYTE)pRespond->aryEscorts);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortHandler::DoUpdateEscortState(INT nConnIndex, INT nSrcId, INT nUpdatePlayerId)
{
	QLogPrintf(LOG_INFO,"..KEscortHandler::DoUpdateEscortState..");
	BOOL						bRetCode = FALSE;
	BOOL						bResult	 = FALSE;
	R2S_UPDATE_ESCORT_STATE*	pRespond = NULL;
	IMemBlock*					piPackage = NULL;
	KPlayerEscort sEscort;

	piPackage = QCreateMemBlock(sizeof(R2S_UPDATE_ESCORT_STATE));
	pRespond = (R2S_UPDATE_ESCORT_STATE*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	bRetCode = g_pGameCenter->m_EscortManager.GetPlayerEscortInfo(nUpdatePlayerId, sEscort);
	LOG_PROCESS_ERROR(bRetCode);

	pRespond->wProtocolID = r2s_update_escort_state;
	pRespond->nSrcId = nSrcId;
	pRespond->nPlayerId = nUpdatePlayerId;
	pRespond->byEscortState = sEscort.byEscortState;
	pRespond->nLeftTime = sEscort.nLeftTime;
	pRespond->dwRoberId = sEscort.dwRoberId;
	pRespond->dwRoberFightscore = sEscort.dwRoberFightscore;
	strncpy(pRespond->szRoberName, sEscort.szRoberName, _NAME_LEN);

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortHandler::DoSyncRobEscortMembers(INT nConnIndex, INT nPlayerId)
{
	QLogPrintf(LOG_INFO,"..KEscortHandler::DoSyncRobEscortMembers..");
	BOOL						bRetCode = FALSE;
	BOOL						bResult	 = FALSE;
	R2S_SYNC_PLAYER_ROB_ESCORTS*	pRespond = NULL;
	IMemBlock*					piPackage = NULL;
	INT nMembersNum = 0;

	nMembersNum = g_pGameCenter->m_EscortManager.GetRobEscortsMembersNum(nPlayerId);

	piPackage = QCreateMemBlock(sizeof(R2S_SYNC_PLAYER_ROB_ESCORTS) + nMembersNum * sizeof(KRobEscort));
	pRespond = (R2S_SYNC_PLAYER_ROB_ESCORTS*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_sync_rob_escort_members;
	pRespond->nPlayerId = nPlayerId;
	pRespond->nRobNum = nMembersNum;

	bRetCode = g_pGameCenter->m_EscortManager.SerializeRobEscorts(nPlayerId, (LPBYTE)pRespond->aryRobEscorts);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

// 同步当前修仙状态
BOOL KEscortHandler::DoSyncEscortState(INT nPlayerId)
{
	QLogPrintf(LOG_INFO,"..KEscortHandler::DoSyncEscortState..");
	BOOL						bRetCode = FALSE;
	BOOL						bResult	 = FALSE;
	R2S_SYNC_ESCORT_STATE*	pSync = NULL;
	INT nEscortState = 0;
	IMemBlock*					piPackage = NULL;
	KRole* pRole = NULL;
	KPlayerEscort sEscort;

	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	piPackage = QCreateMemBlock(sizeof(R2S_SYNC_ESCORT_STATE));
	pSync = (R2S_SYNC_ESCORT_STATE*)piPackage->GetData();
	LOG_PROCESS_ERROR(pSync);

	bRetCode = g_pGameCenter->m_EscortManager.GetPlayerEscortInfo(nPlayerId, sEscort);
	LOG_PROCESS_ERROR(bRetCode);

	pSync->wProtocolID = r2s_sync_escort_state;
	pSync->nPlayerId = nPlayerId;
	pSync->nProtecterId = sEscort.dwProtectRoleId;
	pSync->byEscortLevel = sEscort.byEscortLevel;
	pSync->byState = sEscort.byEscortState;

	bRetCode = Send(pRole->m_nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortHandler::DoTakeEscortAwardRespond(INT nPlayerId, BYTE byEscortState, BYTE byEscortLevel, BYTE bySuccess)
{
	QLogPrintf(LOG_INFO,"..KEscortHandler::DoTakeEscortAwardRespond..");
	BOOL						bRetCode = FALSE;
	BOOL						bResult	 = FALSE;
	R2S_TAKE_ESCORT_AWARD_RESPOND*	pSync = NULL;
	IMemBlock*					piPackage = NULL;
	KRole* pRole = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	piPackage = QCreateMemBlock(sizeof(R2S_TAKE_ESCORT_AWARD_RESPOND));
	pSync = (R2S_TAKE_ESCORT_AWARD_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->wProtocolID = r2s_take_escort_award_respond;
	pSync->nPlayerId = nPlayerId;
	pSync->byEscortState = byEscortState;
	pSync->byEscortLevel = byEscortLevel;
	pSync->bySuccess = bySuccess;

	bRetCode = Send(pRole->m_nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortHandler::DoTakeRobAwardRespond(INT nPlayerId, BYTE byEscortLevel, BYTE bySuccess)
{
	QLogPrintf(LOG_INFO,"..KEscortHandler::DoTakeRobAwardRespond..");
	BOOL						bRetCode = FALSE;
	BOOL						bResult	 = FALSE;
	R2S_TAKE_ROBESCORT_AWARD_RESPOND*	pSync = NULL;
	IMemBlock*					piPackage = NULL;
	KRole* pRole = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	LOG_PROCESS_ERROR(pRole);

	piPackage = QCreateMemBlock(sizeof(R2S_TAKE_ROBESCORT_AWARD_RESPOND));
	pSync = (R2S_TAKE_ROBESCORT_AWARD_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->wProtocolID = r2s_take_robescort_award_respond;
	pSync->nPlayerId = nPlayerId;
	pSync->byEscortLevel = byEscortLevel;
	pSync->bySuccess = bySuccess;

	bRetCode = Send(pRole->m_nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KEscortHandler::Send( int nConnIndex, IMemBlock* piBuffer )
{
	return g_pGameCenter->m_GameServer.Send(nConnIndex, piBuffer);
}

VOID KEscortHandler::Broadcast(IMemBlock* piBuffer)
{
	g_pGameCenter->m_GameServer.Broadcast(piBuffer);
}