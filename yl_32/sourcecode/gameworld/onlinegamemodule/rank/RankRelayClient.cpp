
#include "stdafx.h"
#include "RankRelayClient.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "../onlinegameworld/KRelayClient.h"
#include "arena/arenamodule.h"
#include "rank/krankmanager.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RankRelayClient::RankRelayClient(void)
{
}

RankRelayClient::~RankRelayClient(void)
{
}

VOID RankRelayClient::OnGetRankIdxRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_GET_RANK_IDX_RESPOND* pRespond = NULL;
	pRespond = (R2S_GET_RANK_IDX_RESPOND*)pbyData;
	time_t tNow = KSysService::Time(NULL);
	LOG_PROCESS_ERROR(pRespond);
	switch(pRespond->nRankId)
	{
	case emRANK_ARENA:
		g_ArenaModule.OnGetRankIdxRespond(pRespond->dwSrcId, pRespond->dwId, pRespond->nRankIdx);
		break;
	case emRANK_AREAN_OS:
		RankRelayClient::DoModifyRankValueRequest(emRANK_AREAN_OS, pRespond->dwId, pRespond->nRankIdx, tNow, FALSE);
		break;
	default:
		ASSERT(FALSE);
	}

EXIT0:
	return;
}

VOID RankRelayClient::OnGetRankIdRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_GET_RANK_ID_RESPOND* pRespond = NULL;

	pRespond = (R2S_GET_RANK_ID_RESPOND*)pbyData;

	switch(pRespond->nRankId)
	{
	case emRANK_ARENA:
		g_ArenaModule.OnGetRankIdRespond(pRespond->dwSrcId, pRespond->dwId, pRespond->nRankIdx);
		break;
	case emRANK_ROLE_FIGHTSCORE:
		g_cRankManager.OnGetRoleRankIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwLastModifyTime);
		break;
	case emRANK_PET_LEVEL:
		g_cRankManager.OnGetPetLvRankIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwLastModifyTime);
		break;
	case emRANK_PET_FIGHTSCORE:
		g_cRankManager.OnGetPetFSRankIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwLastModifyTime);
		break;
	case emRANK_RIDE_FIGHTSCORE:
		g_cRankManager.OnGetRideRankIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwLastModifyTime);
		break;
	case emRANK_MPJJ:
		g_cRankManager.OnGetMpjjRankIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_LG:
		g_cRankManager.OnGetLGRankIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_SMZC:
		g_cRankManager.OnGetSmzcRankIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_TONG_FIGHTSCORE:
		g_cRankManager.OnGetTongFSIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_TONG_SALARY:
		g_cRankManager.OnGetTongSrIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_ROLE_LEVEL_OPEN_SERVER:
		g_cRankManager.OnGetPlayerLevelIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwLastModifyTime);
		break;
	//开服活动保存的排行榜
	case emRANK_RIDE_LV:
		g_cRankManager.OnetGetRideLVRanksSaveIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_AREAN_OS:
		g_cRankManager.OnGetAreanRanksSaveIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_PET_LV_OS:
		g_cRankManager.OnGetGetPetLVSaveIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_LG_OS:
		g_cRankManager.OnGetLGSaveIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_TOWER:
		g_cRankManager.OnGetTowerIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwRankValue);
		break;
	case emRANK_ROLE_FIGSHTSCORE_OS:
		g_cRankManager.OnGetRoleFightScoreSaveIdRespond(pRespond->dwId, pRespond->nRankIdx, pRespond->dwLastModifyTime);
		break;
	default:
		ASSERT(FALSE);
	}

EXIT0:
	return;
}

VOID RankRelayClient::OnSwapRankRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_SWAP_RANK_RESPOND* pRespond = NULL;

	pRespond = (R2S_SWAP_RANK_RESPOND*)pbyData;

	switch(pRespond->nRankId)
	{
	case emRANK_ARENA:
		g_ArenaModule.OnSwapRankRespond(pRespond->szSrcPlayerName, pRespond->nSrcRank, pRespond->dwDestId, pRespond->nDestRank, pRespond->bSucceed);
		break;
	case emRANK_AREAN_OS:
		break;
	default:
		ASSERT(FALSE);
	}

EXIT0:
	return;
}

BOOL RankRelayClient::DoGetRankIdxRequest(INT nRankId, DWORD dwSrcId, DWORD dwId)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_GET_RANK_IDX_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_GET_RANK_IDX_REQUEST));
	pRequest = (S2R_GET_RANK_IDX_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_get_rank_idx_request;
	pRequest->dwSrcId = dwSrcId;
	pRequest->byRankId = (BYTE)nRankId;
	pRequest->dwId = dwId;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL RankRelayClient::DoGetRankIdRequest(INT nRankId, DWORD dwSrcId, INT nRankIdx)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_GET_RANK_ID_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_GET_RANK_ID_REQUEST));
	pRequest = (S2R_GET_RANK_ID_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_get_rank_id_request;
	pRequest->byRankId = (BYTE)nRankId;
	pRequest->dwSrcId = dwSrcId;
	pRequest->nRankIdx = nRankIdx;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

// 交换排名
BOOL RankRelayClient::DoSwapRankRequest(INT nRankId, DWORD dwSrcId, DWORD dwDstId)
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_SWAP_RANK_REQUEST*		pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_SWAP_RANK_REQUEST));
	pRequest = (S2R_SWAP_RANK_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_swap_rank_request;
	pRequest->byRankId = (BYTE)nRankId;
	pRequest->dwSrcId = dwSrcId;
	pRequest->dwDstId = dwDstId;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL RankRelayClient::DoModifyRankValueRequest(INT nRankId, DWORD dwId, INT nValue, DWORD dwModifyTime /* = 0 */, BOOL bIfRoll /* = TRUE */)
{
	BOOL								bResult = FALSE;
	BOOL								bRetCode = FALSE;
	S2R_MODIFY_RANK_VALUE_REQUEST*		pRequest = NULL;
	IMemBlock*							piPackage = NULL;

	//QLogPrintf(LOG_INFO,"..RankRelayClient::DoModifyRankValueRequest.. nRankId=%d nValue=%d",nRankId,nValue);
	piPackage = QCreateMemBlock(sizeof(S2R_MODIFY_RANK_VALUE_REQUEST));
	pRequest = (S2R_MODIFY_RANK_VALUE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_modify_rank_value_request;
	pRequest->byRankId = (BYTE)nRankId;
	pRequest->dwId = dwId;
	pRequest->nValue = nValue;
	pRequest->dwModifyTime = dwModifyTime;
	pRequest->bIfRoll = bIfRoll;
	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);
	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL RankRelayClient::Send( IMemBlock* piBuffer )
{
	return g_RelayClient.Send(piBuffer);
}