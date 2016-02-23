

#include "stdafx.h"
#include "onlinegameworld/kplayer.h"

#ifndef GAME_SERVER
#include "onlinegameworld/kplayermanager_client.h"
#else
#include "onlinegameworld/kplayermgr.h"
#endif
#include "onlinegameworld/korpgworld.h"

#include "onlinegamemodule/trade/kplayerpurse.h"
#include "onlinegamemodule/trade/kpurseprotocol.h"
#include "onlinegamemodule/trade/kpurseprotocolhandler.h"
#include "kpursemodule.h"
#include "qpursesetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
template<>
IKModuleInterface* GetModuleInterFace<emKMODULE_PURSE>()
{
	KPurseModule& _m = GetPurseModule();
	return &_m;
}

KPurseModule::KPurseModule() : KModuleBase(emKMODULE_PURSE), m_nPlayerMaxCarryCurrency(0)
{}

BOOL KPurseModule::OnInit()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	BOOL bRetCode = cSafeScript->CallTableFunction(
		"Purse",
		"GetPlayerMaxCarryCurrency",
		1,
		"");
	QCONFIRM_RET_FALSE(bRetCode);
	m_nPlayerMaxCarryCurrency = cSafeScript->GetInt(-1);

	g_cPursesetting.Init();

	return GetProtocolHandler().OnInit();
}

BOOL KPurseModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_S2C(S2C_PURSE_SYNC_CURRENCY);		
	KD_PTC_FIXED_SIZE_S2C(S2C_PURSE_SYNC_REGIST_CURRENCYTYPE);	
	KD_PTC_FIXED_SIZE_S2C(S2C_PURSE_SYNC_MONEY);
	KD_PTC_FIXED_SIZE_S2C(S2C_PURSE_SYNC_REGIST_MONEYTYPE);
	KD_PTC_FIXED_SIZE_S2C(S2C_PURSE_SYNC_AL_CHEMY);	
	KD_PTC_FIXED_SIZE_S2C(S2C_PURSE_SYNC_COIN);	

	KD_PTC_FIXED_SIZE_C2S(C2S_PURSE_SYNC_AL_CHEMY);	
	return TRUE;
}


#ifdef GAME_SERVER

BOOL KPurseModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	QCONFIRM_RET_FALSE(NULL != pPlayerPurse);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pbData;

	switch (pData->byProtocol)
	{
	case emPURSE_PTC_C2S_SYNC_AL_CHEMY:
		KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
		LOG_PROCESS_ERROR(pPlayer);
		KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
		LOG_PROCESS_ERROR(pPlayerPurse);

		// 做出答题选择
		C2S_PURSE_SYNC_AL_CHEMY* ptcAward = (C2S_PURSE_SYNC_AL_CHEMY*)pData;
		pPlayerPurse->SyncBowlData(ptcAward->nRefresh);

		break;

	}
EXIT0:
	return TRUE;
}

#endif

#ifdef GAME_CLIENT

BOOL KPurseModule::ProcessProtocol(LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);
	KPlayer* pPlayer = g_cPlayerMgr.GetById(g_cOrpgWorld.m_dwClientPlayerId);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	QCONFIRM_RET_FALSE(NULL != pPlayerPurse);
	return GetProtocolHandler().ProtocolProcess(pPlayerPurse, pbData, uDataLen);
}

#endif


// -------------------------------------------------------------------------
