#include "ArenaRelayClient.h"
#include "playerarena.h"
#include "../onlinegameworld/KRelayClient.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "onlinegameworld/kplayermgr.h"

ArenaRelayClient::ArenaRelayClient(void)
{

}	

ArenaRelayClient::~ArenaRelayClient(void)
{
	
}

BOOL ArenaRelayClient::Send( IMemBlock* piBuffer )
{
	return g_RelayClient.Send(piBuffer);
}

BOOL ArenaRelayClient::DoLoadRecordRequest(DWORD dwRoleId)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_LOAD_ARENA_RECORD_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_LOAD_ARENA_RECORD_REQUEST));
	pRequest = (S2R_LOAD_ARENA_RECORD_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_load_arena_record_request;
	pRequest->dwRoleId = dwRoleId;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

VOID ArenaRelayClient::OnLoadRecordRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_LOAD_ARENA_RECORD_RESPOND*	pRespond		= NULL;
	KPlayer*						pPlayer			= NULL;
	PlayerArena*					pPlayerArena	= NULL;

	pRespond = (R2S_LOAD_ARENA_RECORD_RESPOND*)pbyData;
	LOG_PROCESS_ERROR(pRespond);

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwRoleId);
	PROCESS_ERROR(pPlayer);

	pPlayerArena = pPlayer->GetCtrl<PlayerArena>();
	LOG_PROCESS_ERROR(pPlayerArena);

	pPlayerArena->OnLoadRecordRespond(pRespond->byData, pRespond->uDataLen);

EXIT0:
	return;
}

VOID ArenaRelayClient::DoAddRecordRequest(DWORD dwRoleId, BOOL bAttack, BOOL bFightResult, LPCSTR szName, INT nTargetIdx)
{
	BOOL							bRetCode = FALSE;
	S2R_ADD_ARENA_RECORD_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_ADD_ARENA_RECORD_REQUEST));
	pRequest = (S2R_ADD_ARENA_RECORD_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_add_arena_record_request;
	pRequest->dwRoleId = dwRoleId;
	pRequest->bAttack = bAttack;
	pRequest->bResult = bFightResult;
	::strncpy(pRequest->szTargetName, szName, _NAME_LEN);
	pRequest->nTargetIdx = nTargetIdx;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}