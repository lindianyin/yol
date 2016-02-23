

#include "stdafx.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#include "kactivitymodule.h"
#include "kactivityprotocol.h"
#include "kactivityctrl.h"
#include "kactivitymanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KActivityModule g_cActivityModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_ACTIVITY>()
{
	return &g_cActivityModule;
}

KActivityModule::KActivityModule() : KModuleBase(emMODULE_ACTIVITY)
{}

BOOL KActivityModule::OnInit()
{
	g_cActivityManager.Init();
	return TRUE;
}

BOOL KActivityModule::InitProtocol()
{
	// s2c
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_ACTIVITY_TIME);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_KINBATTLE_WINNERID);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_LAST_WINKIN_INFO);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_KINBATTLE_JOIN_NUM);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_KINBATTLE_RECORD);

	// c2s
	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_KINBATTLE_RECORD_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_KINBATTLE_WINNERID_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_KINBATTLE_JOINNUM_REQUEST);

	return TRUE;
}

BOOL KActivityModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_get_kinbattle_record_request:
		c2sOnGetKinBattleRecordRequest(pPlayer);
		break;
	case c2s_get_kinbattle_winnerid_request:
		c2sOnGetKinBattleWinnerIdRequest(pPlayer);
		break;
	case c2s_get_kinbattle_join_num_request:
		c2sOnGetKinBattleJoinNumRequest(pPlayer);
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL KActivityModule::c2sOnGetKinBattleRecordRequest(KPlayer* pPlayer)
{
	BOOL								bResult						= FALSE;

	KActivityCtrl* pActivityCtrl = pPlayer->GetCtrl<KActivityCtrl>();
	LOG_PROCESS_ERROR(pActivityCtrl);

	pActivityCtrl->SyncKinBattleRecord();

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityModule::c2sOnGetKinBattleWinnerIdRequest(KPlayer* pPlayer)
{
	BOOL								bResult						= FALSE;

	KActivityCtrl* pActivityCtrl = pPlayer->GetCtrl<KActivityCtrl>();
	LOG_PROCESS_ERROR(pActivityCtrl);

	pActivityCtrl->SyncKinBattleWinnerId();

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityModule::c2sOnGetKinBattleJoinNumRequest(KPlayer* pPlayer)
{
	BOOL								bResult						= FALSE;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();


	cSafeScript->CallTableFunction( "Activity", "OnGetKinBattleJoinNumRequest", 1, "o", pPlayer->GetScriptInterface());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cActivityModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KActivityModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cActivityModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}