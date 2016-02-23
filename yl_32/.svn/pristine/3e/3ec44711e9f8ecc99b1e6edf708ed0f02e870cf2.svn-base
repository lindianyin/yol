#include "stdafx.h"
#include "awardlobbymodule.h"
#include "awardlobbymanager.h"
#include "awardlobbyprotocol.h"
#include "awardlobbysetting.h"
#include "playerawardlobby.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"

AwardLobbyModule g_cAwardLobbyModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_AWARD_LOBBY>()
{
	return &g_cAwardLobbyModule;
}

AwardLobbyModule::AwardLobbyModule() : KModuleBase(emMODULE_AWARD_LOBBY)
{

}

AwardLobbyModule::~AwardLobbyModule()
{

}

BOOL AwardLobbyModule::Breath()
{
	return TRUE;
}

BOOL AwardLobbyModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(KC2S_SIGNED_IN_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_SIGNED_IN_SYNC_REQUEST);

	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_SIGNEDIN_INFO);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_SEVENDAY_INFO);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_ACTIVITY_INFO);

	return TRUE;
}

BOOL AwardLobbyModule::OnInit()
{
	g_cAwardLobbyManager.Init();
	g_cAwardLobbySetting.Init();
	return TRUE;
}

BOOL AwardLobbyModule::OnUnInit()
{
	g_cAwardLobbyManager.UnInit();
	g_cAwardLobbySetting.UnInit();
	return TRUE;
}

BOOL AwardLobbyModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_signed_in_request:
		c2sOnSignedInRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_signed_in_sync_request:
		c2sOnSignedInSyncRequest(pPlayer, pbData, uDataLen);
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL AwardLobbyModule::c2sOnSignedInRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	PlayerAwardLobby*		pAwardLobby = NULL;
	KC2S_SIGNED_IN_REQUEST*	pRequest			= (KC2S_SIGNED_IN_REQUEST*)pProtocol;

	pAwardLobby = pPlayer->GetCtrl<PlayerAwardLobby>();
	LOG_PROCESS_ERROR(pAwardLobby);

	LOG_PROCESS_ERROR(pAwardLobby->PlayerSignedIn(pRequest->nDay));

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL AwardLobbyModule::c2sOnSignedInSyncRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL						bResult						= FALSE;
	PlayerAwardLobby*		pAwardLobby = NULL;

	pAwardLobby = pPlayer->GetCtrl<PlayerAwardLobby>();
	LOG_PROCESS_ERROR(pAwardLobby);

	pAwardLobby->SyncSignedInInfo();

	pAwardLobby->SyncSevenDays();

	pAwardLobby->SyncActivity();

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL AwardLobbyModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cAwardLobbyModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL AwardLobbyModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cAwardLobbyModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}