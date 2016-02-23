#include "stdafx.h"
#include "krecordserverhandler.h"
#include "GameCenter.h"
#include "config/kplayerbasedbdef.h"

VOID KRecordHandler::OnGetServerRecordRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	INT									nRankIdx	= 0;
	S2R_GET_SERVER_RECORD_REQUEST*		pRequest	= (S2R_GET_SERVER_RECORD_REQUEST*)pbyData;

	g_pGameCenter->m_RecordManager.OnGetServerRecordRequest(nConnIndex, pRequest->nRecordId);

EXIT0:
	return;
}

VOID KRecordHandler::OnAddKinBattleRecordRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	INT									nRankIdx	= 0;
	S2R_ADD_KIN_BATTLE_RECORD_REQUEST*	pRequest	= (S2R_ADD_KIN_BATTLE_RECORD_REQUEST*)pbyData;

	g_pGameCenter->m_RecordManager.AddKinBattleRecord(pRequest->dwRoleId);

EXIT0:
	return;
}

VOID KRecordHandler::OnSetKinBattleWinnerIdRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL									bRetCode	= FALSE;
	INT										nRankIdx	= 0;
	S2R_SET_KIN_BATTLE_WINNERID_REQUEST*	pRequest	= (S2R_SET_KIN_BATTLE_WINNERID_REQUEST*)pbyData;

	g_pGameCenter->m_RecordManager.SetKinBattleWinnerId(pRequest->dwWinnerId);

EXIT0:
	return;
}

VOID KRecordHandler::OnSetLastWinKinRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL									bRetCode	= FALSE;
	INT										nRankIdx	= 0;
	S2R_SET_LAST_WIN_KIN_REQUEST*			pRequest	= (S2R_SET_LAST_WIN_KIN_REQUEST*)pbyData;

	g_pGameCenter->m_RecordManager.SetLastWinKinInfo(pRequest->dwWinnerId);

EXIT0:
	return;
}

VOID KRecordHandler::OnSetBankInvestRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_SET_BANK_INVEST_REQUEST*			pRequest	= (S2R_SET_BANK_INVEST_REQUEST*)pbyData;

	g_pGameCenter->m_RecordManager.SetBankInvestRecord(pRequest->nCoins, pRequest->dwRoleId);

	g_pGameCenter->m_RecordManager.OnGetBankInvestRecordBroadcast();

EXIT0:
	return;
}

VOID KRecordHandler::OnSetBuyAllPurcaseRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_BUY_ALL_PURCHASE_REQUEST*			pRequest	= (S2R_BUY_ALL_PURCHASE_REQUEST*)pbyData;
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(pRequest);
	bResult = g_pGameCenter->m_RecordManager.SetBuyAllPurchaseRequest(*pRequest);
	LOG_PROCESS_ERROR(bResult);
	g_pGameCenter->m_RecordManager.OnAllPurchaseRespond(nConnIndex, pRequest->nShopId, pRequest->nCategoryId, pRequest->nGoodsId, pRequest->nCount, pRequest->dwRoleId);
	g_pGameCenter->m_RecordManager.OnBuyAllPurchaseRecordBroadcast();

EXIT0:
	return;
}

BOOL KRecordHandler::DoGetRecordRespond(INT nConnIndex, INT nRecordId, BYTE* pRecordData, size_t uRecordDataLen)
{
	BOOL							bRetCode = FALSE;
	BOOL							bResult	 = FALSE;
	R2S_GET_SERVER_RECORD_RESPOND*	pRespond = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_GET_SERVER_RECORD_RESPOND) + uRecordDataLen);
	pRespond = (R2S_GET_SERVER_RECORD_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_get_server_record_respond;
	pRespond->nRecordId = nRecordId;
	pRespond->uDataLen = uRecordDataLen;
	memcpy(pRespond->byData, pRecordData, uRecordDataLen);

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRecordHandler::DoGetRecordBroadcast(INT nRecordId, BYTE* pRecordData, size_t uRecordDataLen)
{
	BOOL							bRetCode = FALSE;
	BOOL							bResult	 = FALSE;
	R2S_GET_SERVER_RECORD_RESPOND*	pRespond = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_GET_SERVER_RECORD_RESPOND) + uRecordDataLen);
	pRespond = (R2S_GET_SERVER_RECORD_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_get_server_record_respond;
	pRespond->nRecordId = nRecordId;
	pRespond->uDataLen = uRecordDataLen;
	memcpy(pRespond->byData, pRecordData, uRecordDataLen);

	bRetCode = Broadcast(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRecordHandler::DoGetAllPurchaseRespond(INT nConnIndex, INT nShopId, INT nCategoryId, INT nGoodsId, INT nCount, INT nBuycount, DWORD dwRoleId)
{
	BOOL							bRetCode = FALSE;
	BOOL							bResult	 = FALSE;
	R2S_BUY_ALL_PURCHASE_REQUEST*		pRespond = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_BUY_ALL_PURCHASE_REQUEST));
	pRespond = (R2S_BUY_ALL_PURCHASE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_get_all_purchase_buy_respond;
	pRespond->nShopId = nShopId;
	pRespond->nCategoryId = nCategoryId;
	pRespond->nGoodsId = nGoodsId;
	pRespond->nCount = nCount;
	pRespond->dwRoleId = dwRoleId;
	pRespond->nBuyCount = nBuycount;
	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRecordHandler::Send( int nConnIndex, IMemBlock* piBuffer )
{
	return g_pGameCenter->m_GameServer.Send(nConnIndex, piBuffer);
}

BOOL KRecordHandler::Broadcast( IMemBlock* piBuffer )
{
	g_pGameCenter->m_GameServer.Broadcast(piBuffer);
	return TRUE;
}