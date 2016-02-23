#include "stdafx.h"
#include "bankmodule.h"
#include "bankmanager.h"
#include "bankprotocol.h"
#include "playerbank.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "banksetting.h"

BankModule g_cBankModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_BANK>()
{
	return &g_cBankModule;
}

BankModule::BankModule() : KModuleBase(emMODULE_BANK)
{

}

BankModule::~BankModule()
{

}

BOOL BankModule::Breath()
{
	return TRUE;
}

BOOL BankModule::InitProtocol()
{
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_ALL_INVEST_INFO);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_MY_INVEST_INFO);

	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_INCOME_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_INVEST_REQUEST);
	return TRUE;
}

BOOL BankModule::OnInit()
{
	g_cBankManager.Init();
	g_cBankSetting.Init();
	return TRUE;
}

BOOL BankModule::OnUnInit()
{
	g_cBankManager.UnInit();
	g_cBankSetting.UnInit();
	return TRUE;
}

BOOL BankModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pbData;

	PlayerBank* pPlayerBank = NULL;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	pPlayerBank = pPlayer->GetCtrl<PlayerBank>();
	QCONFIRM_RET_FALSE(NULL != pPlayerBank);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_get_income_request:
		{
			KC2S_GET_INCOME_REQUEST* ptcRequest = (KC2S_GET_INCOME_REQUEST*)pData;
			pPlayerBank->DoGetIncomeRequest(ptcRequest->nId);
		}
		break;
	case c2s_invest_request:
		{
			KC2S_INVEST_REQUEST* ptcRequest = (KC2S_INVEST_REQUEST*)pData;
			pPlayerBank->DoInvestRequest(ptcRequest->nCoins, ptcRequest->nId);
		}
		break;
	default:
		break;
	}

	return TRUE;
}


BOOL BankModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cBankModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL BankModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cBankModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}