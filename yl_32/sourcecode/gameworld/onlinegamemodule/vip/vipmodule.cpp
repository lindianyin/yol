#include "stdafx.h"
#include "vipmodule.h"
#include "vipmanager.h"
#include "vipprotocol.h"
#include "vipsetting.h"
#include "playervip.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"

VipModule g_VipModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_VIP>()
{
	return &g_VipModule;
}

VipModule::VipModule() : KModuleBase(emMODULE_VIP)
{

}

VipModule::~VipModule()
{

}

BOOL VipModule::Breath()
{
	return TRUE;
}

BOOL VipModule::InitProtocol()
{
	KD_PTC_LENGTNED_SIZE_S2C(VIP_S2C_BUY_COUNT);
	KD_PTC_FIXED_SIZE_S2C(VIP_S2C_EXP);
	KD_PTC_FIXED_SIZE_S2C(VIP_S2C_RECEVIE);
	KD_PTC_FIXED_SIZE_S2C(VIP_S2C_SYNC_EXP_TIME);
	KD_PTC_FIXED_SIZE_S2C(VIP_S2C_SYNC_MSG);
	
	KD_PTC_FIXED_SIZE_C2S(VIP_C2S_SYNC);
	KD_PTC_FIXED_SIZE_C2S(VIP_C2S_GETGIFTS);
	KD_PTC_FIXED_SIZE_C2S(VIP_C2S_BUY_COUNT);
	KD_PTC_FIXED_SIZE_C2S(VIP_C2S_GET_OR_SET_MSG);
	return TRUE;
}

BOOL VipModule::OnInit()
{
	g_cVipManager.Init();
	g_cVipSetting.Init();
	return TRUE;
}

BOOL VipModule::OnUnInit()
{
	g_cVipManager.UnInit();
	g_cVipSetting.UnInit();
	return TRUE;
}

BOOL VipModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pbData);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pbData;
	PlayerVip* pPlayerVip = NULL;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerVip = pPlayer->GetCtrl<PlayerVip>();
	LOG_PROCESS_ERROR(pPlayerVip);

	QLogPrintf(LOG_INFO,"..VipModule::ProcessProtocol(INT nPlayerIndex=%d pData->byProtocol=%d",nPlayerIndex,pData->byProtocol);
	
	switch (pData->byProtocol)
	{
	case emVIP_PTC_C2S_SYNC:
		{
			//VIP_C2S_SYNC* ptcSync = (VIP_C2S_SYNC*)pData;
			pPlayerVip->DoSync();
		}
		break;
	case emVIP_PTC_C2S_GETGIFTS:
		{
			VIP_C2S_GETGIFTS* ptcGetGifts = (VIP_C2S_GETGIFTS*)pData;
			pPlayerVip->DoRecevie(ptcGetGifts->nVipLevel);
		}
		break;
	case emVIP_PTC_C2S_BUY_COUNT:
		{
			VIP_C2S_BUY_COUNT* ptcBuyCount = (VIP_C2S_BUY_COUNT*)pData;
			pPlayerVip->DoBuyCount(ptcBuyCount->nId, ptcBuyCount->nNum);
		}
		break;
	case emVIP_PTC_C2S_GET_OR_SET_MSG:
		{
			//QLogPrintf(LOG_INFO,"..emVIP_PTC_C2S_GET_OR_SET_MSG..");
			VIP_C2S_GET_OR_SET_MSG* pGetOrSetMsg = (VIP_C2S_GET_OR_SET_MSG*)pData;
			pPlayerVip->GetOrSetMsg(pGetOrSetMsg->nType,pGetOrSetMsg->szMsg);
		}
		break;
		
	}
	return TRUE;

EXIT0:
	return FALSE;
}
