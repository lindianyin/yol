#include "stdafx.h"
#include "opensemodule.h"
#include "opensemanager.h"
#include "openseprotocol.h"
#include "opensesetting.h"
#include "playeropense.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"

OpenseModule g_OpenseModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_Opense>()
{
	return &g_OpenseModule;
}

OpenseModule::OpenseModule() : KModuleBase(emMODULE_Opense)
{

}

OpenseModule::~OpenseModule()
{

}

BOOL OpenseModule::Breath()
{
	return TRUE;
}

BOOL OpenseModule::InitProtocol()
{
	KD_PTC_LENGTNED_SIZE_S2C(OPENSE_S2C_SYNC);
	KD_PTC_FIXED_SIZE_S2C(OPENSE_S2C_ALL_FINISH);

	KD_PTC_FIXED_SIZE_C2S(OPENSE_C2S_SYNC);
	KD_PTC_FIXED_SIZE_C2S(OPENSE_C2S_GETAWARD);
	return TRUE;
}

BOOL OpenseModule::OnInit()
{
	g_cOpenseManager.Init();
	g_cOpenseSetting.Init();
	return TRUE;
}

BOOL OpenseModule::OnUnInit()
{
	g_cOpenseManager.UnInit();
	g_cOpenseSetting.UnInit();
	return TRUE;
}

BOOL OpenseModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pbData);
	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pbData;
	PlayerOpense* pPlayerOpense = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerOpense = pPlayer->GetCtrl<PlayerOpense>();
	LOG_PROCESS_ERROR(pPlayerOpense);
	switch (pData->byProtocol)
	{
	case emOPENSE_PTC_C2S_SYNC:
		pPlayerOpense->SyncOpense();
		break;
	case emOPENSE_PTC_C2S_GETAWARD:
		OPENSE_C2S_GETAWARD* ptcAward = (OPENSE_C2S_GETAWARD*)pData;
		pPlayerOpense->GetAward(ptcAward->nDay, ptcAward->nId);
		break;
	}
	return TRUE;

EXIT0:
	return FALSE;
}
