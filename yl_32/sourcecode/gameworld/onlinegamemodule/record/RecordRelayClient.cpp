#include "RecordRelayClient.h"
#include "activity/kactivityctrl.h"
#include "../onlinegameworld/KRelayClient.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "onlinegameworld/kplayermgr.h"
#include "serverbase/RecordDef.h"
#include "activity/kactivitymanager.h"
#include "bank/bankmanager.h"
#include "trade/kbuysellmgr.h"
#include "onlinegameworld/korpgworld.h"

RecordRelayClient::RecordRelayClient(void)
{

}	

RecordRelayClient::~RecordRelayClient(void)
{
	
}

BOOL RecordRelayClient::Send( IMemBlock* piBuffer )
{
	return g_RelayClient.Send(piBuffer);
}

BOOL RecordRelayClient::DoGetServerRecordRequest(INT nRecordId)
{
	BOOL							bResult = FALSE;
	BOOL							bRetCode = FALSE;
	S2R_GET_SERVER_RECORD_REQUEST*	pRequest = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_GET_SERVER_RECORD_REQUEST));
	pRequest = (S2R_GET_SERVER_RECORD_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_get_server_record_request;
	pRequest->nRecordId = nRecordId;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL RecordRelayClient::DoAddKinBattleRecordRequest(DWORD dwRoleId)
{
	BOOL								bResult = FALSE;
	BOOL								bRetCode = FALSE;
	S2R_ADD_KIN_BATTLE_RECORD_REQUEST*	pRequest = NULL;
	IMemBlock*							piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_ADD_KIN_BATTLE_RECORD_REQUEST));
	pRequest = (S2R_ADD_KIN_BATTLE_RECORD_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_add_kinbattle_record_request;
	pRequest->dwRoleId = dwRoleId;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL RecordRelayClient::DoSetKinBattleWinnerIdRequest(DWORD dwWinnerId)
{
	BOOL									bResult = FALSE;
	BOOL									bRetCode = FALSE;
	S2R_SET_KIN_BATTLE_WINNERID_REQUEST*	pRequest = NULL;
	IMemBlock*								piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_SET_KIN_BATTLE_WINNERID_REQUEST));
	pRequest = (S2R_SET_KIN_BATTLE_WINNERID_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_set_kinbattle_winnerId_request;
	pRequest->dwWinnerId = dwWinnerId;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL RecordRelayClient::DoSetLastWinKinRequest(DWORD dwWinnerId)
{
	BOOL									bResult = FALSE;
	BOOL									bRetCode = FALSE;
	S2R_SET_LAST_WIN_KIN_REQUEST*			pRequest = NULL;
	IMemBlock*								piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_SET_LAST_WIN_KIN_REQUEST));
	pRequest = (S2R_SET_LAST_WIN_KIN_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_set_last_winkin_request;
	pRequest->dwWinnerId = dwWinnerId;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL RecordRelayClient::DoSetBankInvestRequest(INT nCoins, DWORD dwRoleId)
{
	BOOL									bResult = FALSE;
	BOOL									bRetCode = FALSE;
	S2R_SET_BANK_INVEST_REQUEST*			pRequest = NULL;
	IMemBlock*								piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_SET_BANK_INVEST_REQUEST));
	pRequest = (S2R_SET_BANK_INVEST_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_set_bank_invest_request;
	pRequest->dwRoleId = dwRoleId;
	pRequest->nCoins = nCoins;

	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL RecordRelayClient::DoBuyAllPurchaseRequest( S2R_BUY_ALL_PURCHASE_REQUEST &sRequest)
{
	BOOL									bResult = FALSE;
	BOOL									bRetCode = FALSE;
	S2R_BUY_ALL_PURCHASE_REQUEST*			pRequest = NULL;
	IMemBlock*								piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_BUY_ALL_PURCHASE_REQUEST));
	pRequest = (S2R_BUY_ALL_PURCHASE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_buy_all_purchase_request;
	pRequest->dwRoleId = sRequest.dwRoleId;
	pRequest->nShopId = sRequest.nShopId;
	pRequest->nCategoryId = sRequest.nCategoryId;
	pRequest->nGoodsId = sRequest.nGoodsId;
	pRequest->nCount = sRequest.nCount;
	pRequest->dwLimitTime = sRequest.dwLimitTime;
	pRequest->dwStartTime = sRequest.dwStartTime;
	pRequest->nMaxCount = sRequest.nMaxCount;
	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

VOID RecordRelayClient::OnGetAllPurchaseRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_BUY_ALL_PURCHASE_REQUEST*	pRespond		= NULL;
	KPlayer*						pPlayer			= NULL;
	pRespond = (R2S_BUY_ALL_PURCHASE_REQUEST*)pbyData;
	LOG_PROCESS_ERROR(pRespond);
	pPlayer = g_cPlayerMgr.GetById(pRespond->dwRoleId);
	LOG_PROCESS_ERROR(pPlayer);
	g_cBuySellMgr.Buy_Purchase_Limit(pPlayer->GetPlayerIndex(), pRespond);
EXIT0:
	return;
}

//全服记录返回
VOID RecordRelayClient::OnGetServerRecordRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_GET_SERVER_RECORD_RESPOND*	pRespond		= NULL;
	KPlayer*						pPlayer			= NULL;
	KActivityCtrl*					pActivityCtrl	= NULL;

	pRespond = (R2S_GET_SERVER_RECORD_RESPOND*)pbyData;
	LOG_PROCESS_ERROR(pRespond);

	QLogPrintf(LOG_INFO,"RecordRelayClient::OnGetServerRecordRespond=%d",pRespond->nRecordId);
	
	switch(pRespond->nRecordId)
	{
	case emRecordKinBattle:			// 三界争霸历史记录
		g_cActivityManager.OnGetKinBattleRecordRespond(pRespond->byData, pRespond->uDataLen);
		break;
	case emRecordKinBattleWinnerId:		// 三界神皇
		g_cActivityManager.OnGetKinBattleWinnerIdRespond(pRespond->byData, pRespond->uDataLen);
		break;
	case emRecordLastWinKinInfo:
		g_cActivityManager.OnGetLastWinKinInfoRespond(pRespond->byData, pRespond->uDataLen);
		break;
	case emRecordBankInvest:
		g_cBankManager.OnGetBankInvestRecordRespond(pRespond->byData, pRespond->uDataLen);
		break;
	case emRecordBuyAllPurchase:
		g_cBuySellMgr.OnGetAllBuyPurchaseRecordRespond(pRespond->byData, pRespond->uDataLen);
		break;
	case emRecordServerStartTime:		// 开服时间
		g_cOrpgWorld.OnGetServerStartTimeRespond(pRespond->byData, pRespond->uDataLen);
		break;
	}

EXIT0:
	return;
}