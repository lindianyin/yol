#include "stdafx.h"
#include "gemmodule.h"
#include "gemmanager.h"
#include "gemprotocol.h"
#include "gemsetting.h"
#include "playergem.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"

GemModule g_cGemModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_GEM>()
{
	return &g_cGemModule;
}

GemModule::GemModule() : KModuleBase(emMODULE_GEM)
{

}

GemModule::~GemModule()
{

}

BOOL GemModule::Breath()
{
	return TRUE;
}

BOOL GemModule::InitProtocol()
{
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_GEM_INFO);
	return TRUE;
}

BOOL GemModule::OnInit()
{
	g_cGemManager.Init();
	g_cGemSetting.Init();
	return TRUE;
}

BOOL GemModule::OnUnInit()
{
	g_cGemManager.UnInit();
	g_cGemSetting.UnInit();
	return TRUE;
}

BOOL GemModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{/*
	case c2s_signed_in_request:
		c2sOnSignedInRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_signed_in_sync_request:
		c2sOnSignedInSyncRequest(pPlayer, pbData, uDataLen);
		break;*/
	default:
		break;
	}

	return TRUE;
}


BOOL GemModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cGemModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL GemModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cGemModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}