

#include "stdafx.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#include "kblademodule.h"
#include "kbladeprotocol.h"
#include "kbladectrl.h"
#include "kbladesetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KBladeModule g_cBladeModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_BLADE>()
{
	return &g_cBladeModule;
}

KBladeModule::KBladeModule() : KModuleBase(emMODULE_BLADE)
{}

BOOL KBladeModule::OnInit()
{
	g_cBladeSetting.Init();

	return TRUE;
}

BOOL KBladeModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(KC2S_START_CRAZY_REQUEST);

	return TRUE;
}

BOOL KBladeModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_start_crazy_request:
		c2sOnStartCrazyRequest(pPlayer, pbData, uDataLen);
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL KBladeModule::c2sOnStartCrazyRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	KBladeCtrl* pBladeCtrl = pPlayer->GetCtrl<KBladeCtrl>();
	LOG_PROCESS_ERROR(pBladeCtrl);

	pBladeCtrl->OnStartCrazy();

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KBladeModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cBladeModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KBladeModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cBladeModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}