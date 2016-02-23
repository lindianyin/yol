#include "stdafx.h"
#include "krankhandler.h"
#include "GameCenter.h"
#include "krankpool.h"
#include "serverbase/RankDef.h"

VOID KRankHandler::OnGetRankIdxRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	INT							nRankIdx	= 0;
	S2R_GET_RANK_IDX_REQUEST*	pRequest	= (S2R_GET_RANK_IDX_REQUEST*)pbyData;
	KRankPool*					pRankPool	= NULL;

	pRankPool = g_pGameCenter->m_RankManager.GetRankPool(pRequest->byRankId);
	LOG_PROCESS_ERROR(pRankPool);

	nRankIdx = pRankPool->GetIdxById(pRequest->dwId);

	bRetCode = DoGetRankIdxRespond(nConnIndex, pRequest->byRankId, pRequest->dwSrcId, pRequest->dwId, nRankIdx);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

VOID KRankHandler::OnGetRankIdRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	DWORD						dwId		= 0;
	DWORD						dwValue		= 0;
	DWORD						dwModifyTime = 0;
	S2R_GET_RANK_ID_REQUEST*	pRequest	= (S2R_GET_RANK_ID_REQUEST*)pbyData;
	KRankPool*					pRankPool	= NULL;

	pRankPool = g_pGameCenter->m_RankManager.GetRankPool(pRequest->byRankId);
	LOG_PROCESS_ERROR(pRankPool);
	dwId = pRankPool->GetIdByIdx(pRequest->nRankIdx);
	dwValue = pRankPool->GetValueByIdx(pRequest->nRankIdx);
	dwModifyTime = pRankPool->GetLastModifyTime(pRequest->nRankIdx);
	bRetCode = DoGetRankIdRespond(nConnIndex, pRequest->byRankId, pRequest->dwSrcId, dwId, pRequest->nRankIdx, dwValue, dwModifyTime);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

VOID KRankHandler::OnSwapRankRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL						bRetCode	= FALSE;
	BOOL						bSucceed	= FALSE;
	DWORD						dwId		= 0;
	S2R_SWAP_RANK_REQUEST*		pRequest	= (S2R_SWAP_RANK_REQUEST*)pbyData;
	KRankPool*					pRankPool	= NULL;
	KRole*						pRole		= NULL;

	pRankPool = g_pGameCenter->m_RankManager.GetRankPool(pRequest->byRankId);
	LOG_PROCESS_ERROR(pRankPool);

	bSucceed = pRankPool->SwapRank(pRequest->dwSrcId, pRequest->dwDstId);
	LOG_PROCESS_ERROR(bSucceed);

	
	// 交换双方都发下
	bRetCode = DoSwapRankRespond(nConnIndex, pRequest->byRankId, pRequest->dwSrcId, pRankPool->GetIdxById(pRequest->dwSrcId), pRequest->dwDstId, pRankPool->GetIdxById(pRequest->dwDstId), bSucceed);
	LOG_PROCESS_ERROR(bRetCode);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwDstId);
	if (pRole != NULL && pRole->m_nConnIndex != nConnIndex) // 在目标玩家和源玩家不在同一个gs的情况下，给目标玩家也发一次，以便更新目标玩家的竞技场信息
	{
		bRetCode = DoSwapRankRespond(pRole->m_nConnIndex, pRequest->byRankId, pRequest->dwSrcId, pRankPool->GetIdxById(pRequest->dwSrcId), pRequest->dwDstId, pRankPool->GetIdxById(pRequest->dwDstId), bSucceed);
		LOG_PROCESS_ERROR(bRetCode);
	}	

EXIT0:
	return;
}

VOID KRankHandler::OnModifyRankValueRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL							bRetCode	= FALSE;
	DWORD							dwId		= 0;
	S2R_MODIFY_RANK_VALUE_REQUEST*	pRequest	= (S2R_MODIFY_RANK_VALUE_REQUEST*)pbyData;
	KRankPool*						pRankPool	= NULL;

	pRankPool = g_pGameCenter->m_RankManager.GetRankPool(pRequest->byRankId);
	LOG_PROCESS_ERROR(pRankPool);

	pRankPool->ModifyRankValue(pRequest->dwId, pRequest->nValue, pRequest->dwModifyTime, pRequest->bIfRoll,pRequest->byRankId);

EXIT0:
	return;
}

BOOL KRankHandler::DoGetRankIdxRespond(INT nConnIndex, INT nRankId, DWORD dwSrcId, DWORD dwId, INT nRankIdx)
{
	BOOL						bRetCode = FALSE;
	BOOL						bResult	 = FALSE;
	R2S_GET_RANK_IDX_RESPOND*	pRespond = NULL;
	IMemBlock*					piPackage = NULL;


	piPackage = QCreateMemBlock(sizeof(R2S_GET_RANK_IDX_RESPOND));
	pRespond = (R2S_GET_RANK_IDX_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_get_rank_idx_respond;
	pRespond->nRankId = nRankId;
	pRespond->nRankIdx = nRankIdx;
	pRespond->dwSrcId = dwSrcId;
	pRespond->dwId = dwId;

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRankHandler::DoGetRankIdRespond(INT nConnIndex, INT nRankId, DWORD dwSrcId, DWORD dwId, INT nRankIdx, DWORD dwValue /* = 0 */, DWORD dwLastModifyTime /* = 0 */)
{
	BOOL						bRetCode = FALSE;
	BOOL						bResult	 = FALSE;
	R2S_GET_RANK_ID_RESPOND*	pRespond = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_GET_RANK_ID_RESPOND));
	pRespond = (R2S_GET_RANK_ID_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_get_rank_id_respond;
	pRespond->nRankId = nRankId;
	pRespond->dwSrcId = dwSrcId;
	pRespond->dwId = dwId;
	pRespond->nRankIdx = nRankIdx;
	pRespond->dwRankValue = dwValue;
	pRespond->dwLastModifyTime = dwLastModifyTime;

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRankHandler::DoSwapRankRespond(INT nConnIndex, INT nRankId, DWORD dwSrcId, INT nSrcRank, DWORD dwDestId, INT nDestRank, BOOL bSucceed)
{
	BOOL						bRetCode = FALSE;
	BOOL						bResult	 = FALSE;
	R2S_SWAP_RANK_RESPOND*		pRespond = NULL;
	IMemBlock*					piPackage = NULL;

	KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(dwSrcId);
	PROCESS_ERROR(pRole);

	piPackage = QCreateMemBlock(sizeof(R2S_SWAP_RANK_RESPOND));
	pRespond = (R2S_SWAP_RANK_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_swap_rank_respond;
	pRespond->nRankId = nRankId;
	strcpy(pRespond->szSrcPlayerName, pRole->getName());		;
	pRespond->nSrcRank = nSrcRank;
	pRespond->dwDestId = dwDestId;
	pRespond->nDestRank = nDestRank;
	pRespond->bSucceed = bSucceed;

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRankHandler::Send( int nConnIndex, IMemBlock* piBuffer )
{
	return g_pGameCenter->m_GameServer.Send(nConnIndex, piBuffer);
}