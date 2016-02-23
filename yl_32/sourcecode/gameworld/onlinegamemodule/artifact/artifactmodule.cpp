#include "stdafx.h"
#include "artifactmodule.h"
#include "artifactmanager.h"
#include "artifactprotocol.h"
#include "artifactsetting.h"
#include "playerartifact.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"

ArtifactModule g_cArtifactModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_ARTIFACT>()
{
	return &g_cArtifactModule;
}

ArtifactModule::ArtifactModule() : KModuleBase(emMODULE_ARTIFACT)
{

}

ArtifactModule::~ArtifactModule()
{

}

BOOL ArtifactModule::Breath()
{
	return TRUE;
}

BOOL ArtifactModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(KC2S_SYNC_ARTIFACT_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_PIECE_USE_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_LEVEL_UP_REQUEST);
	/*
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_SIGNEDIN_INFO);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_SEVENDAY_INFO);*/
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_ARTIFACT_INFO);

	return TRUE;
}

BOOL ArtifactModule::OnInit()
{
	g_cArtifactManager.Init();
	g_cArtifactSetting.Init();
	return TRUE;
}

BOOL ArtifactModule::OnUnInit()
{
	g_cArtifactManager.UnInit();
	g_cArtifactSetting.UnInit();
	return TRUE;
}

BOOL ArtifactModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);


	KC2S_PIECE_USE_REQUEST*	pRequest = NULL;
	KC2S_LEVEL_UP_REQUEST*	pLevepUp = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	PlayerArtifact* pPlayerArtifact = NULL;
	QCONFIRM_RET_FALSE(NULL != pPlayer);
	pPlayerArtifact = pPlayer->GetCtrl<PlayerArtifact>();
	QCONFIRM_RET_FALSE(NULL != pPlayerArtifact);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_artifact_sync_request:
		pPlayerArtifact->SyncArtifact();
		break;
	case c2s_artifact_piece_use_request:
		pRequest = (KC2S_PIECE_USE_REQUEST*)pbData;
		pPlayerArtifact->UsePiece(pRequest->nId, pRequest->nCount);
		break;
	case c2s_artiface_level_up_request:
		pLevepUp = (KC2S_LEVEL_UP_REQUEST*)pbData;
		pPlayerArtifact->LevelUp(pLevepUp->nId);
		break;
	}

	return TRUE;
}

BOOL ArtifactModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cArtifactModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL ArtifactModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cArtifactModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}