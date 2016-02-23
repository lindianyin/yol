

#include "stdafx.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#include "kstagemodule.h"
#include "kstageprotocol.h"
#include "kstagectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KStageModule g_cStageModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_STAGE>()
{
	return &g_cStageModule;
}

KStageModule::KStageModule() : KModuleBase(emMODULE_STAGE)
{}

BOOL KStageModule::OnInit()
{
	return TRUE;
}

BOOL KStageModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(KC2S_MOP_STAGE_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_TAKE_MOP_AWARD);
	KD_PTC_FIXED_SIZE_C2S(KC2S_MOP_SPEED_UP);

	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_STAGE_INFO);
	KD_PTC_FIXED_SIZE_S2C(KS2C_START_MOP_STAGE_RESULT);

	return TRUE;
}

BOOL KStageModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_mop_stage_request:
		c2s_StartMopStage(pPlayer, pbData, uDataLen);
		break;
	case c2s_take_mop_award:
		c2s_TakeStageAward(pPlayer);
		break;
	case c2s_mop_stage_speedup:
		c2s_SpeedUp(pPlayer);
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL KStageModule::c2s_StartMopStage(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen)
{
	KC2S_MOP_STAGE_REQUEST* pSync;
	pSync = (KC2S_MOP_STAGE_REQUEST*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Stage", "StartMop", 1, "od", pPlayer->GetScriptInterface(), pSync->nDifficulty);
	INT bRet = cSafeScript->GetInt(-1);

	KS2C_START_MOP_STAGE_RESULT sSync;
	sSync.bResult = (BYTE)bRet;

	return SendData(pPlayer->GetPlayerIndex(), sSync);
}

BOOL KStageModule::c2s_TakeStageAward(KPlayer* pPlayer)
{
	KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
	QCONFIRM_RET_FALSE(pStageCtrl);

	QCONFIRM_RET_FALSE(pStageCtrl->sMopInfo.nState == emMopStateDone);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Stage", "TakeAward", 1, "o", pPlayer->GetScriptInterface());
	BOOL bRet = cSafeScript->GetInt(-1);

	if(bRet)
	{
		QCONFIRM_RET_FALSE(pStageCtrl->OnTakeAward());
	}

	return TRUE;
}

BOOL KStageModule::c2s_SpeedUp(KPlayer* pPlayer)
{
	KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
	QCONFIRM_RET_FALSE(pStageCtrl);

	QCONFIRM_RET_FALSE(pStageCtrl->sMopInfo.nState == emMopStateDoing);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Stage", "SpeedUp", 0, "o", pPlayer->GetScriptInterface());

	return TRUE;
}

BOOL KStageModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cStageModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KStageModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cStageModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}