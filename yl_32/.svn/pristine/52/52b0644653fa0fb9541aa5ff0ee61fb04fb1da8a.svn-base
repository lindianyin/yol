#include "stdafx.h"
#include "ArenaServerHandler.h"
#include "GameCenter.h"
#include "config/kplayerbasedbdef.h"

VOID KArenaHandler::OnLoadArenaRecordRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	INT									nRankIdx	= 0;
	S2R_LOAD_ARENA_RECORD_REQUEST*		pRequest	= (S2R_LOAD_ARENA_RECORD_REQUEST*)pbyData;

	//TODO：首先在arenamanager里找record，找不到再从数据库里load

	bRetCode = g_pGameCenter->m_ArenaManager.LoadRecord(nConnIndex, pRequest->dwRoleId);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

VOID KArenaHandler::OnAddArenaRecordRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	INT									nRankIdx	= 0;
	S2R_ADD_ARENA_RECORD_REQUEST*		pRequest	= (S2R_ADD_ARENA_RECORD_REQUEST*)pbyData;

	bRetCode = g_pGameCenter->m_ArenaManager.AddRecord(nConnIndex, 
		pRequest->dwRoleId,
		pRequest->bAttack,
		pRequest->bResult,
		pRequest->szTargetName,
		pRequest->nTargetIdx);

	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

BOOL KArenaHandler::DoLoadRecordRespond(INT nConnIndex, DWORD dwRoleId, BYTE* pRecordData, size_t uRecordDataLen)
{
	BOOL							bRetCode = FALSE;
	BOOL							bResult	 = FALSE;
	R2S_LOAD_ARENA_RECORD_RESPOND*	pRespond = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_LOAD_ARENA_RECORD_RESPOND) + uRecordDataLen);
	pRespond = (R2S_LOAD_ARENA_RECORD_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_load_arena_record_respond;
	pRespond->dwRoleId = dwRoleId;
	pRespond->uDataLen = uRecordDataLen;
	LOG_PROCESS_ERROR(piPackage->GetSize() >= uRecordDataLen);
	memcpy(pRespond->byData, pRecordData, uRecordDataLen);

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KArenaHandler::Send( int nConnIndex, IMemBlock* piBuffer )
{
	return g_pGameCenter->m_GameServer.Send(nConnIndex, piBuffer);
}