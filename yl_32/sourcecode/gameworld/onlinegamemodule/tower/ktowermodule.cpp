

#include "stdafx.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#include "ktowermodule.h"
#include "ktowerprotocol.h"
#include "ktowerctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KTowerModule g_cTowerModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_TOWER>()
{
	return &g_cTowerModule;
}

KTowerModule::KTowerModule() : KModuleBase(emMODULE_TOWER)
{}

BOOL KTowerModule::OnInit()
{
	return TRUE;
}

BOOL KTowerModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_S2C(KS2C_START_MOP_RESULT);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_MOP_INFO);

	KD_PTC_FIXED_SIZE_C2S(QC2S_START_MOP);
	KD_PTC_FIXED_SIZE_C2S(QC2S_STOP_MOP);
	KD_PTC_FIXED_SIZE_C2S(QC2S_TAKE_MOP_AWARD);
	KD_PTC_FIXED_SIZE_C2S(QC2S_MOP_SPEED_UP);
	KD_PTC_FIXED_SIZE_C2S(QC2S_MOP_FINISH_NOW);

	return TRUE;
}

BOOL KTowerModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_start_mop_tower:
		c2s_StartMopTower(pPlayer);
		break;
	case c2s_stop_mop_tower:
		c2s_StopMopTower(pPlayer);
		break;
	case c2s_take_mop_award:
		c2s_TakeAward(pPlayer);
		break;
	case c2s_mop_speedup:
		c2s_SpeedUp(pPlayer);
		break;
	case c2s_finish_now:
		c2s_FinishMopNow(pPlayer);
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL KTowerModule::c2s_StartMopTower(KPlayer* pPlayer)
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Tower", "StartMop", 1, "o", pPlayer->GetScriptInterface());
	INT bRet = cSafeScript->GetInt(-1);

	KS2C_START_MOP_RESULT sSync;
	sSync.bResult = (BYTE)bRet;

	return SendData(pPlayer->GetPlayerIndex(), sSync);
}

BOOL KTowerModule::c2s_StopMopTower(KPlayer* pPlayer)
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Tower", "StopMop", 1, "o", pPlayer->GetScriptInterface());
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KTowerModule::c2s_TakeAward(KPlayer* pPlayer)
{
	KTowerCtrl* pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
	QCONFIRM_RET_FALSE(pTowerCtrl);

	QCONFIRM_RET_FALSE(pTowerCtrl->sMopInfo.nState == emMopStateDone);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Tower", "TakeAward", 0, "o", pPlayer->GetScriptInterface());

	QCONFIRM_RET_FALSE(pTowerCtrl->OnTakeAward());

	return TRUE;
}

BOOL KTowerModule::c2s_SpeedUp(KPlayer* pPlayer)
{
	KTowerCtrl* pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
	QCONFIRM_RET_FALSE(pTowerCtrl);

	QCONFIRM_RET_FALSE(pTowerCtrl->sMopInfo.nState == emMopStateDoing);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Tower", "SpeedUp", 0, "o", pPlayer->GetScriptInterface());

	return TRUE;
}

BOOL KTowerModule::c2s_FinishMopNow(KPlayer* pPlayer)
{
	KTowerCtrl* pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
	QCONFIRM_RET_FALSE(pTowerCtrl);

	QCONFIRM_RET_FALSE(pTowerCtrl->sMopInfo.nState == emMopStateDoing);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction( "Tower", "FinishMopNow", 1, "o", pPlayer->GetScriptInterface());
	
	return TRUE;
}

BOOL KTowerModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cTowerModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KTowerModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cTowerModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}