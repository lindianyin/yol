#include "stdafx.h"
#include "targetmodule.h"
#include "targetmanager.h"
#include "targetprotocol.h"
#include "targetsetting.h"
#include "playertarget.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"

TargetModule g_TargetModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_TARGET>()
{
	return &g_TargetModule;
}

TargetModule::TargetModule() : KModuleBase(emMODULE_TARGET)
{

}

TargetModule::~TargetModule()
{

}

BOOL TargetModule::Breath()
{
	return TRUE;
}

BOOL TargetModule::InitProtocol()
{
	KD_PTC_LENGTNED_SIZE_S2C(TARGET_S2C_SYNC);
	KD_PTC_FIXED_SIZE_S2C(TARGET_S2C_ALL_FINISH);

	KD_PTC_FIXED_SIZE_C2S(TARGET_C2S_SYNC);
	KD_PTC_FIXED_SIZE_C2S(TARGET_C2S_GETAWARD);
	return TRUE;
}

BOOL TargetModule::OnInit()
{
	g_cTargetManager.Init();
	g_cTargetSetting.Init();
	return TRUE;
}

BOOL TargetModule::OnUnInit()
{
	g_cTargetManager.UnInit();
	g_cTargetSetting.UnInit();
	return TRUE;
}

BOOL TargetModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pbData);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pbData;
	PlayerTarget* pPlayerTarget = NULL;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerTarget = pPlayer->GetCtrl<PlayerTarget>();
	LOG_PROCESS_ERROR(pPlayerTarget);
	switch (pData->byProtocol)
	{
	case emTARGET_PTC_C2S_SYNC:
		pPlayerTarget->SyncTarget();
		break;
	case emTARGET_PTC_C2S_GETAWARD:
		TARGET_C2S_GETAWARD* ptcAward = (TARGET_C2S_GETAWARD*)pData;
		pPlayerTarget->GetAward(ptcAward->nDay, ptcAward->nId);
		break;
	}
	return TRUE;

EXIT0:
	return FALSE;
}
