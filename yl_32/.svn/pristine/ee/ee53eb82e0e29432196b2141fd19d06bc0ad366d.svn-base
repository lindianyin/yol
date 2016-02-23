

#include "stdafx.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#include "krankmodule.h"
#include "krankprotocol.h"
#include "krankctrl.h"
#include "krankmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KRankModule g_cRankModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_RANK>()
{
	return &g_cRankModule;
}

KRankModule::KRankModule() : KModuleBase(emMODULE_RANK)
{}

BOOL KRankModule::OnInit()
{
	g_cRankManager.Init();
	return TRUE;
}

BOOL KRankModule::InitProtocol()
{
	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_ROLE_RANK);
	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_LASTUPDATETIME_REQUEST);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_LASTUPDATETIME);
	
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_ROLE_RANK);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_PET_LV_RANK);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_PET_FS_RANK);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_RIDE_RANK);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_MPJJ_RANK);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_LG_RANK);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_SMZC_RANK);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_TONG_FS_RANK);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_TONG_SR_RANK);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_TOWER_OPEN_SERVICE);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_PLAYER_LEVEL_OPEN_SERVICE);

	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_RIDE_LV_OS);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_AREAN_OS);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_PET_LV_OS);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_LG_OS);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_ROLE_FIGHTSCORE_OS);
	KD_PTC_LENGTNED_SIZE_S2C(KS2C_SYNC_END);
	return TRUE;
}

BOOL KRankModule::c2sOnSyncUpdateTimeRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL bResult = FALSE;
	KC2S_GET_LASTUPDATETIME_REQUEST* pSync = (KC2S_GET_LASTUPDATETIME_REQUEST*)pProtocol;
	KRankCtrl* pRankCtrl = NULL;
	//QLogPrintf(LOG_INFO,"..c2sOnSyncUpdateTimeRequest..");
	Q_Printl("..c2sOnSyncUpdateTimeRequest.. pSync->nRankId=" << pSync->nRankId);
	pRankCtrl = pPlayer->GetCtrl<KRankCtrl>();
	LOG_PROCESS_ERROR(pRankCtrl);
	LOG_PROCESS_ERROR(pRankCtrl->SyncLastUpdateTime(pSync->nRankId));
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankModule::c2sOnSyncRankRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	BOOL bResult = FALSE;
	KC2S_GET_ROLE_RANK*	pSync = (KC2S_GET_ROLE_RANK*)pProtocol;
	KRankCtrl* pRankCtrl = NULL;
	
	pRankCtrl = pPlayer->GetCtrl<KRankCtrl>();
	LOG_PROCESS_ERROR(pRankCtrl);
	QLogPrintf(LOG_INFO,"..c2sOnSyncRankRequest.. pSync->nRankId = %d",pSync->nRankId);
	
	switch(pSync->nRankId)
	{
	case emRANK_ROLE_FIGHTSCORE:
		pRankCtrl->SyncRoleRanks();
		break;
	case emRANK_PET_LEVEL:
		pRankCtrl->SyncPetLvRanks();
		break;
	case emRANK_PET_FIGHTSCORE:
		pRankCtrl->SyncPetFSRanks();
		break;
	case emRANK_RIDE_FIGHTSCORE:
		pRankCtrl->SyncRideRanks();
		break;
	case emRANK_MPJJ:
		pRankCtrl->SyncMpjjRanks();
		break;
	case emRANK_LG:
		pRankCtrl->SyncLgRanks();
		break;
	case emRANK_SMZC:
		pRankCtrl->SyncSmzcRanks();
		break;
	case emRANK_TONG_FIGHTSCORE:
		pRankCtrl->SyncTongFSRanks();
		break;
	case emRANK_TONG_SALARY:
		pRankCtrl->SyncTongSrRanks();
		break;
	case emRANK_ROLE_LEVEL_OPEN_SERVER:
		pRankCtrl->SyncRankRoleLvOfOpenServer();
		break;
	case emRANK_RIDE_LV:
		pRankCtrl->SyncRankRideLV();
		break;
	case emRANK_AREAN_OS:
		pRankCtrl->SyncRankArean();
		break;
	case emRANK_PET_LV_OS:
		pRankCtrl->SyncPetLV();
		break;
	case emRANK_LG_OS:
		pRankCtrl->SyncLGRank_OS();
		break;
	case emRANK_TOWER:
		pRankCtrl->SyncRankTower();
		break;
	case emRANK_ROLE_FIGSHTSCORE_OS:
		pRankCtrl->SyncRoleFightScore_OS();
		break;
	default:
		break;
	}
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	ASSERT(pbData && uDataLen);

	switch (*(pbData + 1))
	{
	case c2s_get_lastupdatetime_request:
		c2sOnSyncUpdateTimeRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_get_rank_request:
		c2sOnSyncRankRequest(pPlayer, pbData, uDataLen);
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL KRankModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData)
{
	return g_cRankModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KRankModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cRankModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}
