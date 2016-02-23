/* -------------------------------------------------------------------------
//	文件名		：	kpetmodule.cpp
//	创建者		：	NiuNiu
//	创建时间	：	2012-11-28 18:13:00
//	功能描述	：  
// -------------------------------------------------------------------------*/


#include "stdafx.h"
#include "onlinegameworld/kplayer.h"

#include "onlinegameworld/kplayermgr.h"
#include "kpetmodule.h"
#include "kpetprotocol.h"
#include "kpetctrl.h"
#include "kridectrl.h"
#include "kpetmanager.h"
#include "item2/kitemmgr.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "vip/vipmanager.h"
#include "vip/vipdef.h"

//-----------------------------------------------------------------------

KPetModule g_cPetModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_PET>()
{
	return &g_cPetModule;
}

KPetModule::KPetModule() : KModuleBase(emMODULE_PET)
{

}

BOOL KPetModule::OnInit()
{
	BOOL bOK = FALSE;
	g_cPetSetting.Init();
	g_cPetMgr.Init();

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	bOK = cSafeScript->CallTableFunction("PetIsland", "OnInit", 1, "");

	return bOK;
}

BOOL KPetModule::OnUnInit()
{
	KModuleBase::OnUnInit();

	g_cPetMgr.UnInit();

	return TRUE;
}

BOOL KPetModule::InitProtocol()
{
	// s2c
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_PET_INFO);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_PET_STATE);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_PET_EQUIP);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_FREE_RESULT);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_PET_EXP);
	KD_PTC_FIXED_SIZE_S2C(KS2C_PET_CAST_SKILL);
	KD_PTC_FIXED_SIZE_S2C(QS2C_PET_ISLAND_REFRESH_RATE);
	KD_PTC_FIXED_SIZE_S2C(QS2C_PET_CARDS);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_RIDE_INFO);
	KD_PTC_FIXED_SIZE_S2C(KS2C_TAKEOFF_RIDE);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_RIDE_STATE);
	KD_PTC_FIXED_SIZE_S2C(KS2C_SYNC_RIDE_LEVEL);
	

	// c2s
	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_PETINFO_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_CHANGE_PETSTATE_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_PET_FREE_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_RESIZE_PET_BAG_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_CHANGE_CHARACTER_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_FOSTER_PET_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_FEED_PET_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_CHANGE_NAME_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_UP_RANK_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_PUTON_EQUIP_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_TAKEOFF_EQUIP_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_TRANS_EXP_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_CHANGE_PET_TACTIC_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(QC2S_COMPOSE_PET_CARD);
	KD_PTC_FIXED_SIZE_C2S(KC2S_PET_SKILL_STATE);

	KD_PTC_FIXED_SIZE_C2S(KC2S_GET_RIDEINFO_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_TAKEOFF_RIDE_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_RIDE_UPRANK_REQUEST);
	KD_PTC_FIXED_SIZE_C2S(KC2S_CHANGE_RIDE_STATE);
	KD_PTC_FIXED_SIZE_C2S(KC2S_FEED_RIDE);
	KD_PTC_FIXED_SIZE_C2S(KC2S_ADD_RIDE_MAGICRATE);
	KD_PTC_FIXED_SIZE_C2S(KC2S_CHANGE_RIDE_REPRESENT);
	
	return TRUE;
}

BOOL KPetModule::c2sSyncPetInfoRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_GET_PETINFO_REQUEST* pSync = (KC2S_GET_PETINFO_REQUEST*)pProtocol;

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	KPet* pPet = g_cPetMgr.GetPetByOwnerId(pSync->dwCharacterId);
	if(!pPet)
		return FALSE;

	return pPetCtrl->SyncPetInfo(pPet);
}

BOOL KPetModule::c2sChangePetStateRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_CHANGE_PETSTATE_REQUEST* pSync = (KC2S_CHANGE_PETSTATE_REQUEST*)pProtocol;

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	KPet* pPet = g_cPetMgr.GetPetById(pSync->dwId);
	QCONFIRM_RET_FALSE(pPet);

	return pPetCtrl->OnChangeStateRequest(pPet, pSync->byState);
}

BOOL KPetModule::c2sPetFreeRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_PET_FREE_REQUEST* pSync = (KC2S_PET_FREE_REQUEST*)pProtocol;

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	return pPetCtrl->FreePet(pSync->dwId);
}

BOOL KPetModule::c2sChangeCharacterRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_CHANGE_CHARACTER_REQUEST* pSync = (KC2S_CHANGE_CHARACTER_REQUEST*)pProtocol;

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	KPet* pPet = g_cPetMgr.GetPetById(pSync->dwId);
	QCONFIRM_RET_FALSE(pPet);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Pet", "RerandPetCharacter", 1, "od", pPet->m_pLuna, pSync->byCharacter);
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KPetModule::c2sResizePetBagRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_RESIZE_PET_BAG_REQUEST* pSync = (KC2S_RESIZE_PET_BAG_REQUEST*)pProtocol;
	QCONFIRM_RET_FALSE(pSync);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Pet", "ResizePetBag", 0, "");
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KPetModule::c2sChangeNameRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_CHANGE_NAME_REQUEST* pSync = (KC2S_CHANGE_NAME_REQUEST*)pProtocol;

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	KPet* pPet = g_cPetMgr.GetPetById(pSync->dwId);
	QCONFIRM_RET_FALSE(pPet);

	return pPetCtrl->ChangeName(pPet, pSync->szName);
}

BOOL KPetModule::c2sUpRankRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_UP_RANK_REQUEST* pSync = (KC2S_UP_RANK_REQUEST*)pProtocol;

	KPet* pPet = g_cPetMgr.GetPetById(pSync->dwId);
	QCONFIRM_RET_FALSE(pPet);

	QCONFIRM_RET_FALSE(pPet->GetRankLevel() < pPet->GetMaxRank());

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Pet", "UpRank", 1, "o", pPet->m_pLuna);
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KPetModule::c2sFosterRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_FOSTER_PET_REQUEST* pSync = (KC2S_FOSTER_PET_REQUEST*)pProtocol;

	KPet* pPet = g_cPetMgr.GetPetById(pSync->dwId);
	QCONFIRM_RET_FALSE(pPet);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Pet", "Forster", 1, "od", pPet->m_pLuna, pSync->byAmount);
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KPetModule::c2sFeedRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_FEED_PET_REQUEST* pSync = (KC2S_FEED_PET_REQUEST*)pProtocol;

	KPet* pPet = g_cPetMgr.GetPetById(pSync->dwPetId);
	QCONFIRM_RET_FALSE(pPet);

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KItem* pItem = pPlayerItem->GetItemById(pSync->dwItemId);
	QCONFIRM_RET_FALSE(pItem);

	if(!(pItem->GetGenre() == item_pet && pItem->GetDetailType() == item_pet_food)) 
		return FALSE;

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Pet", "Feed", 1, "oo", pPet->m_pLuna, pItem->m_pLuna);
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KPetModule::c2sPutOnEquipRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_PUTON_EQUIP_REQUEST* pSync = (KC2S_PUTON_EQUIP_REQUEST*)pProtocol;

	KPet* pPet = g_cPetMgr.GetPetById(pSync->dwPetId);
	QCONFIRM_RET_FALSE(pPet);

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	return pPetCtrl->OnPutOnEquipRequest(pPet, pSync->dwItemId, pSync->byPos);
}

BOOL KPetModule::c2sTakeOffEquipRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_TAKEOFF_EQUIP_REQUEST* pSync = (KC2S_TAKEOFF_EQUIP_REQUEST*)pProtocol;

	KPet* pPet = g_cPetMgr.GetPetById(pSync->dwPetId);
	QCONFIRM_RET_FALSE(pPet);

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	return pPetCtrl->OnTakeOffEquipRequest(pPet, pSync->byPos);
}

BOOL KPetModule::c2sTransExpRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	Q_Printl("..KPetModule::c2sTransExpRequest");
	BOOL 					bResult  = FALSE;
	KC2S_TRANS_EXP_REQUEST* pSync	 = (KC2S_TRANS_EXP_REQUEST*)pProtocol;
	ScriptSafe 				cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	KPet* 					pSrcPet	 = NULL;
	KPet* 					pDstPet  = NULL;
	KPetCtrl* 				pPetCtrl = NULL;
	INT 					bRet	 = 0;
	DWORD 					dwSkillBookNum = 0;
	DWORD 					dwUpRankBallNum = 0;


	pSrcPet = g_cPetMgr.GetPetById(pSync->dwSrcPetId);
	LOG_PROCESS_ERROR(pSrcPet);

	pDstPet = g_cPetMgr.GetPetById(pSync->dwDstPetId);
	LOG_PROCESS_ERROR(pDstPet);

	dwSkillBookNum = pSrcPet->GetTotalSkillBookNum();
	dwUpRankBallNum = pSrcPet->GetTotalUpRankBallNum();

	cSafeScript->CallTableFunction( "Pet", "CheckCanTransExp", 1, "ooddd", pSrcPet->GetScriptInterface(), pDstPet->GetScriptInterface(), pSync->byUseGold,dwSkillBookNum,dwUpRankBallNum);
	bRet = cSafeScript->GetInt(-1);
	LOG_PROCESS_ERROR(bRet);
	
	pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	LOG_PROCESS_ERROR(pPetCtrl);

	if (pSync->byUseGold)
	{
        bResult = pPetCtrl->TransExpToPet(pSrcPet, pDstPet, 100);
		LOG_PROCESS_ERROR(bResult);
	}
	else
	{
       	bResult = pPetCtrl->TransExpToPet(pSrcPet, pDstPet, 90);
		LOG_PROCESS_ERROR(bResult);
	}
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPetModule::c2sChangePetSkillState(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_PET_SKILL_STATE* pSync = (KC2S_PET_SKILL_STATE*)pProtocol;

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	QCONFIRM_RET_FALSE(pPlayer->GetId() != pSync->dwCharacterId);

	pPetCtrl->BindPetTarget(pSync->dwPetId, pSync->dwCharacterId, pSync->nPetPosX, pSync->nPetPosY);

	// TODO: 宠物技能状态
	return TRUE;
}

BOOL KPetModule::c2sComposePetCard(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	QC2S_COMPOSE_PET_CARD* pSync = (QC2S_COMPOSE_PET_CARD*)pProtocol;

	// 在脚本里处理
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->GetGlobalF("PetIsland:ComposePetCard");

	cSafeScript->PushObj(pPlayer->GetScriptInterface());
	cSafeScript->PushTable();
	for (INT i = 0; i < 12; i++)
	{
		cSafeScript->PushNumber((double)pSync->dwPetIds[i]);
		cSafeScript->SetTableIndex(i + 1);
	}

	return cSafeScript->DoCall(0, 3);
}

BOOL KPetModule::c2sChangeTacticRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_CHANGE_PET_TACTIC_REQUEST* pSync = (KC2S_CHANGE_PET_TACTIC_REQUEST*)pProtocol;

	KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
	QCONFIRM_RET_FALSE(pPetCtrl);

	if(pSync->dwPetId)
	{
		KPet* pPet = g_cPetMgr.GetPetById(pSync->dwPetId);
		QCONFIRM_RET_FALSE(pPet);

		QCONFIRM_RET_FALSE(pPetCtrl->OnChangeTactic(pSync->byCharacter, pPet));
	}
	else
	{
		QCONFIRM_RET_FALSE(pPetCtrl->RemoveFromTactic(pSync->byCharacter));
	}
	
	return TRUE;
}

BOOL KPetModule::c2sSyncRideInfoRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_GET_RIDEINFO_REQUEST* pSync = (KC2S_GET_RIDEINFO_REQUEST*)pProtocol;

	KRideCtrl* pRideCtrl = pPlayer->GetCtrl<KRideCtrl>();
	QCONFIRM_RET_FALSE(pRideCtrl);

	KRide* pRide = g_cPetMgr.GetRideByOwnerId(pSync->dwCharacterId);
	if(!pRide)
		return FALSE;

	return pRideCtrl->SyncRideInfo(pRide);
}

BOOL KPetModule::c2sTakeOffRideRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_TAKEOFF_RIDE_REQUEST* pSync = (KC2S_TAKEOFF_RIDE_REQUEST*)pProtocol;

	KRide* pRide = g_cPetMgr.GetRideById(pSync->dwRideId);
	QCONFIRM_RET_FALSE(pRide);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Pet", "TakeOffRide", 1, "oo", pPlayer->GetScriptInterface(), pRide->m_pLuna);
	BOOL bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KPetModule::c2sRideUpRankRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_RIDE_UPRANK_REQUEST* pSync = (KC2S_RIDE_UPRANK_REQUEST*)pProtocol;

	KRide* pRide = g_cPetMgr.GetRideById(pSync->dwRideId);
	QCONFIRM_RET_FALSE(pRide);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Pet", "RideUpRank", 1, "o", pRide->m_pLuna);
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

// 喂坐骑幻化果
BOOL KPetModule::c2sAddRideMagicRate(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_ADD_RIDE_MAGICRATE* pSync = (KC2S_ADD_RIDE_MAGICRATE*)pProtocol;

	KRide* pRide = g_cPetMgr.GetRideById(pSync->dwRideId);
	QCONFIRM_RET_FALSE(pRide);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Pet", "AddRideMagicRate", 1, "od", pRide->m_pLuna, pSync->nCostNum);
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KPetModule::c2sFeedRide(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_FEED_RIDE* pSync = (KC2S_FEED_RIDE*)pProtocol;

	KRide* pRide = g_cPetMgr.GetRideById(pSync->dwRideId);
	QCONFIRM_RET_FALSE(pRide);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Pet", "FeedRide", 1, "od", pRide->m_pLuna, pSync->byCostNum);
	INT bRet = cSafeScript->GetInt(-1);

	return bRet;
}

BOOL KPetModule::c2sChangeRideRepresent(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_CHANGE_RIDE_REPRESENT* pSync = (KC2S_CHANGE_RIDE_REPRESENT*)pProtocol;

	KRideCtrl* pRideCtl = pPlayer->GetCtrl<KRideCtrl>();
	QCONFIRM_RET_FALSE(pRideCtl);

	return pRideCtl->ChangeRideRepresentId(pSync->dwRideId, pSync->nRepresentId);
}

BOOL KPetModule::c2sChangeRideState(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen)
{
	KC2S_CHANGE_RIDE_STATE* pSync = (KC2S_CHANGE_RIDE_STATE*)pProtocol;

	KRide* pRide = g_cPetMgr.GetRideById(pSync->dwRideId);
	QCONFIRM_RET_FALSE(pRide);

	KRideCtrl* pRideCtrl = pPlayer->GetCtrl<KRideCtrl>();
	QCONFIRM_RET_FALSE(pRideCtrl);

	return pRideCtrl->ChangeRideState(pRide, pSync->byState);
}

BOOL KPetModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0);
	QCONFIRM_RET_FALSE(pbData != NULL && uDataLen > 0);

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(NULL != pPlayer);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pbData;
	QCONFIRM_RET_FALSE(pData);

	switch (pData->byProtocol)
	{
	case c2s_get_petinfo_request:
		c2sSyncPetInfoRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_change_petstate_request:
		c2sChangePetStateRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_pet_free_request:
		c2sPetFreeRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_resize_pet_bag_request:
		c2sResizePetBagRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_feed_request:
		c2sFeedRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_change_character_request:
		c2sChangeCharacterRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_change_name_request:
		c2sChangeNameRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_up_rank_request:
		c2sUpRankRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_foster_request:
		c2sFosterRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_puton_equip_request:
		c2sPutOnEquipRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_takeoff_equip_request:
		c2sTakeOffEquipRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_trans_exp_request:
		c2sTransExpRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_change_pet_tactic_request:
		c2sChangeTacticRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_compose_pet_card:
		c2sComposePetCard(pPlayer, pbData, uDataLen);
		break;
	case c2s_pet_skill_state:
		c2sChangePetSkillState(pPlayer, pbData, uDataLen);
		break;
	case c2s_get_rideinfo_request:
		c2sSyncRideInfoRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_takeoff_ride_request:
		c2sTakeOffRideRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_ride_uprank_request:
		c2sRideUpRankRequest(pPlayer, pbData, uDataLen);
		break;
	case c2s_change_ride_state:
		c2sChangeRideState(pPlayer, pbData, uDataLen);
		break;
	case c2s_add_ride_magicrate:
		c2sAddRideMagicRate(pPlayer, pbData, uDataLen);
		break;
	case c2s_feed_ride:
		c2sFeedRide(pPlayer, pbData, uDataLen);
		break;
	case c2s_change_ride_represent:
		c2sChangeRideRepresent(pPlayer, pbData, uDataLen);
	default:
		break;
	}
	return TRUE;
}


BOOL KPetModule::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData )
{
	return g_cPetModule.GetProtocolProcessor()->SendData(nPlayerIndex, rPtcData);
}

BOOL KPetModule::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	return g_cPetModule.GetProtocolProcessor()->SendLenData(nPlayerIndex, rPtcData, uDataSize);
}

BOOL KPetModule::SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize )
{
	return g_cPetModule.GetProtocolProcessor()->SendUnknownData(nPlayerIndex, pData, uDataSize);
}
