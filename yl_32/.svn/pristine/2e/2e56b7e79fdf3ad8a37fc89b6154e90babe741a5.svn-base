

#include "stdafx.h"
#include "onlinegamemodule/trade/kpurseprotocol.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "onlinegamemodule/trade/kpurseprotocolhandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

BOOL KPurseProtocolHandler::OnInit()
{
#ifdef GAME_CLIENT
	m_aProtocolHandler[emPURSE_PTC_S2C_SYNC_CURRENCY]     = &KPurseProtocolHandler::HandleAddCurrency;		
	m_aProtocolHandler[emPURSE_PTC_S2C_SYNC_REGIST_CURRENCYTYPE] = &KPurseProtocolHandler::HandleRegistCurrencyType;	
	m_aProtocolHandler[emPURSE_PTC_S2C_SYNC_MONEY]		  = &KPurseProtocolHandler::HandleAddMoney;
	m_aProtocolHandler[emPURSE_PTC_S2C_SYNC_REGIST_MONEYTYPE] = &KPurseProtocolHandler::HandleRegistMoneyType;
	m_aProtocolHandler[emPURSE_PTC_S2C_SYNC_COIN]		  = &KPurseProtocolHandler::HandleAddCoin;
#endif
	return TRUE;
}

BOOL KPurseProtocolHandler::ProtocolProcess(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen)
{
	ASSERT(pPlayerPurse);
	QCONFIRM_RET_FALSE(NULL!= pData);
	QCONFIRM_RET_FALSE(0 != nDataLen);

	KPTC_HEADER_BASE* pProtocol = (KPTC_HEADER_BASE*)pData;

#ifdef GAME_CLIENT
	QCONFIRM_RET_FALSE(pProtocol->byProtocol >= 0 && pProtocol->byProtocol < emPURSE_PTC_S2C_COUNT);
	QCONFIRM_RET_FALSE(m_aProtocolHandler[pProtocol->byProtocol] != NULL);	
	return (this->*m_aProtocolHandler[pProtocol->byProtocol])(pPlayerPurse, pData, nDataLen);

#endif

#ifdef GAME_SERVER
	//ÔÝÊ±Ã»ÓÐ
	ASSERT(FALSE);
	return FALSE;
#endif
}

#ifdef GAME_CLIENT

BOOL KPurseProtocolHandler::HandleAddCurrency(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen)
{
	ASSERT(pPlayerPurse);
	QCONFIRM_RET_FALSE(NULL != pData && nDataLen > 0);
	QCONFIRM_RET_FALSE(nDataLen == sizeof(S2C_PURSE_SYNC_CURRENCY));

	S2C_PURSE_SYNC_CURRENCY* pSyncAddCurrency =  (S2C_PURSE_SYNC_CURRENCY*)pData;
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	return pPlayerPurse->SetCurrency(pSyncAddCurrency->byCurrencyType, pSyncAddCurrency->nCurrency, &nResult);
}

BOOL KPurseProtocolHandler::HandleRegistCurrencyType(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen)
{
	ASSERT(pPlayerPurse);
	QCONFIRM_RET_FALSE(NULL != pData && nDataLen > 0);
	QCONFIRM_RET_FALSE(nDataLen == sizeof(S2C_PURSE_SYNC_REGIST_CURRENCYTYPE));

	S2C_PURSE_SYNC_REGIST_CURRENCYTYPE* pSyncRegistCurrencyType = (S2C_PURSE_SYNC_REGIST_CURRENCYTYPE*)pData;
	return pPlayerPurse->RegistCurrencyType(pSyncRegistCurrencyType->byCurrencyType);
}


BOOL KPurseProtocolHandler::HandleAddCoin(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen)
{
	ASSERT(pPlayerPurse);
	QCONFIRM_RET_FALSE(NULL != pData && nDataLen > 0);
	QCONFIRM_RET_FALSE(nDataLen == sizeof(S2C_PURSE_SYNC_COIN));

	S2C_PURSE_SYNC_COIN* pSyncCoin = (S2C_PURSE_SYNC_COIN*)pData;
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	return pPlayerPurse->SetCoin(pSyncCoin->nJbCoin, &nResult);
}

BOOL KPurseProtocolHandler::HandleAddMoney(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen)
{
	ASSERT(pPlayerPurse);
	QCONFIRM_RET_FALSE(NULL != pData && nDataLen > 0);
	QCONFIRM_RET_FALSE(nDataLen == sizeof(S2C_PURSE_SYNC_MONEY));

	S2C_PURSE_SYNC_MONEY* pSyncMoney = (S2C_PURSE_SYNC_MONEY*)pData;
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	return pPlayerPurse->SetMoney(pSyncMoney->byMoneyType, pSyncMoney->nMoney, &nResult);
}

BOOL KPurseProtocolHandler::HandleRegistMoneyType(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen)
{
	ASSERT(pPlayerPurse);
	QCONFIRM_RET_FALSE(NULL != pData && nDataLen > 0);
	QCONFIRM_RET_FALSE(nDataLen == sizeof(S2C_PURSE_SYNC_REGIST_MONEYTYPE));

	S2C_PURSE_SYNC_REGIST_MONEYTYPE* pSyncRegistMoneyType = (S2C_PURSE_SYNC_REGIST_MONEYTYPE*)pData;	
	return pPlayerPurse->RegistMoneyType(pSyncRegistMoneyType->byMoneyType);
}

#endif //#ifdef GAME_CLIENT
