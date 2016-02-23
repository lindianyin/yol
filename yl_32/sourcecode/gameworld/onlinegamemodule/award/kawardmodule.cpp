/* -------------------------------------------------------------------------
//	文件名		：	kawardmodule.cpp
//	创建者		：	zhaoyu
//	创建时间	：	2013-08-14
//	功能描述	：  
// -------------------------------------------------------------------------*/


#include "stdafx.h"
#include "onlinegameworld/kplayer.h"

#include "onlinegameworld/kplayermgr.h"
#include "kawardmodule.h"
#include "kawardprotocol.h"
#include "kawardctrl.h"
#include "kawardmanager.h"
#include "kawardsetting.h"

//-----------------------------------------------------------------------

KAwardModule g_cAwardModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_AWARD>()
{
	return &g_cAwardModule;
}

KAwardModule::KAwardModule() : KModuleBase(emMODULE_AWARD)
{

}

BOOL KAwardModule::OnInit()
{
	g_cAwardSetting.Init();

	g_cAwardMgr.Init();
	return TRUE;
}

BOOL KAwardModule::OnUnInit()
{
	KModuleBase::OnUnInit();

	g_cAwardSetting.UnInit();

	g_cAwardMgr.UnInit();

	return TRUE;
}

BOOL KAwardModule::InitProtocol()
{
	// s2c
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_AWARD);
	KD_PTC_FIXED_SIZE_S2C(KS2C_TAKE_AWARD_RESPOND);
	
	// c2s
	KD_PTC_FIXED_SIZE_C2S(KC2S_TAKE_AWARD);
	
	return TRUE;
}

BOOL KAwardModule::c2sTakeAwardRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_TAKE_AWARD* pSync = (KC2S_TAKE_AWARD*)pProtocol;
	KAwardCtrl* pAwardCtrl = pPlayer->GetCtrl<KAwardCtrl>();
	QCONFIRM_RET_FALSE(pAwardCtrl);
	QLogPrintf(LOG_INFO,"..KAwardModule::c2sTakeAwardRequest pSync->dwAwardId=%d pSync->szModuleName=%s" ,pSync->dwAwardId,pSync->szModuleName);
	BOOL bRet = pAwardCtrl->TakeAward(pSync->dwAwardId,pSync->szModuleName);
	QCONFIRM_RET_FALSE(bRet);
	
	return TRUE;
}

BOOL KAwardModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pbData;
	QCONFIRM_RET_FALSE(pData);

	switch (pData->byProtocol)
	{
	case c2s_take_award:
		c2sTakeAwardRequest(pPlayer, pbData, uDataLen);
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL KAwardModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData )
{
	return g_cAwardModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KAwardModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cAwardModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}

BOOL KAwardModule::SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize )
{
	return g_cAwardModule.GetProtocolProcessor()->SendUnknownData(nPlayerIndex, pData, uDataSize);
}
