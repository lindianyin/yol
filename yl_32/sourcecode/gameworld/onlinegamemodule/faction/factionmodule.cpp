/* -------------------------------------------------------------------------
//	文件名		：	factionmanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-25 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "factionmodule.h"
#include "factionmanager.h"
#include "factionprotocol.h"
#include "factionsetting.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamemodule/playerfaction.h"

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_FACTION>()
{
	return GetFactionModule();
}

FactionModule::FactionModule() : KModuleBase(emMODULE_FACTION)
{

}

FactionModule::~FactionModule()
{

}

BOOL FactionModule::OnInit()
{
	KModuleBase::OnInit();

	g_cFactionManager.Init();

	g_cFactionSetting.Init();

	KD_PTC_FIXED_SIZE_S2C(FACTION_S2C_SYNC_CLIENT_SETTING);
	KD_PTC_FIXED_SIZE_C2S(FACTION_C2S_SYNC_CLIENT_SETTING);

	return TRUE;
}

BOOL FactionModule::OnUnInit()
{
	KModuleBase::OnUnInit();

	g_cFactionSetting.UnInit();

	return TRUE;
}

BOOL FactionModule::Breath()
{
	KModuleBase::Breath();

	g_cFactionManager.Breath();

	return TRUE;
}

BOOL FactionModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pbData);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pbData;
	switch (pData->byProtocol)
	{
	case emC2S_SYNC_CLIENT_SETTING:
		ApplyPlayerSetting(pbData, uDataLen, pPlayer);
		break;
	}
	
	return TRUE;
}

BOOL FactionModule::ApplyPlayerSetting(LPCBYTE pData, INT nDataLen, KPlayer* pPlayer)
{
	FACTION_C2S_SYNC_CLIENT_SETTING* syncSetting = (FACTION_C2S_SYNC_CLIENT_SETTING*)pData;
	QCONFIRM_RET_FALSE(syncSetting);

	FACTION_PLAYER_SETTING cSetting;
	cSetting.bHideOtherPlayer = syncSetting->byHideOtherPlayers > 0;
	cSetting.bHideParticleEffect = syncSetting->byHideParticleEffect > 0;
	cSetting.bLowestQualityLock = syncSetting->byQualityLock > 0;
	cSetting.bLowestVolumeLock = syncSetting->byVolumeLock > 0;
	cSetting.nRepresentQuality = syncSetting->byRepresentQuality;
	cSetting.nBackgroundMusicVolume = syncSetting->byBackgroundMusicVolume;
	cSetting.nGameSoundVolume = syncSetting->byGameSoundVolume;
	cSetting.nAutoAddHp = syncSetting->byAtuoAddHp;
	cSetting.nAutoAddMp = syncSetting->byAtuoAddMp;

	PlayerFaction* pPlayerFaction = pPlayer->GetCtrl<PlayerFaction>();
	QCONFIRM_RET_FALSE(pPlayerFaction);

	pPlayerFaction->SetFactionSetting(cSetting);

	return TRUE;
}

BOOL FactionModule::DoSyncPlayerSetting(KPlayer& rPlayer, CONST FACTION_PLAYER_SETTING& cSetting)
{
    FACTION_S2C_SYNC_CLIENT_SETTING cSyncSetting;
	cSyncSetting.byBackgroundMusicVolume = (BYTE)cSetting.nBackgroundMusicVolume;
	cSyncSetting.byGameSoundVolume = (BYTE)cSetting.nGameSoundVolume;
	cSyncSetting.byRepresentQuality = (BYTE)cSetting.nRepresentQuality;
	cSyncSetting.byHideOtherPlayers = (BYTE)cSetting.bHideOtherPlayer;
	cSyncSetting.byHideParticleEffect = (BYTE)cSetting.bHideParticleEffect;
	cSyncSetting.byVolumeLock = (BYTE)cSetting.bLowestVolumeLock;
	cSyncSetting.byQualityLock = (BYTE)cSetting.bLowestQualityLock;
	cSyncSetting.byAtuoAddHp = (BYTE)cSetting.nAutoAddHp;
	cSyncSetting.byAtuoAddMp = (BYTE)cSetting.nAutoAddMp;

	m_cProtocolProcessor.SendData(rPlayer.GetPlayerIndex(), cSyncSetting);

	return TRUE;
}

BOOL FactionModule::InitProtocol()
{
	//KD_PTC_FIXED_SIZE_S2C(PTC_FACTION_CHANGE_MEDICINE_SLOT);
	//KD_PTC_FIXED_SIZE_C2S(PTC_FACTION_CHANGE_MEDICINE_SLOT);
	return TRUE;
}