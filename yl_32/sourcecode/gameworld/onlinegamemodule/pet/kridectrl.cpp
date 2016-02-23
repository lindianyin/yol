/* -------------------------------------------------------------------------
//	文件名		：	kridectrl.cpp
//	创建者		：	zhaoyu
//	创建时间	：	2013-3-15
//	功能描述	：	坐骑管理
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kridectrl.h"
#include "kpetmanager.h"
#include "kpetprotocol.h"
#include "kpetmodule.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "item2/kmagicattribmanager.h"
#include "onlinegameworld/synchelper.h"
#include "onlinegameworld/ikscenemgr.h"
#include "../onlinegameworld/scene/kscene.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "../onlinegamemodule/bladestate/kbladectrl.h"

KRideCtrl::KRideCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

KRideCtrl::~KRideCtrl()
{
	
}

BOOL KRideCtrl::Init()
{
	m_pCurrentRide = NULL;
	return TRUE;
}

BOOL KRideCtrl::UnInit()
{
	m_pCurrentRide = NULL;
	return TRUE;
}

BOOL KRideCtrl::OnLogout()
{
	if(m_pCurrentRide)
	{
		QCONFIRM_RET_FALSE(g_cPetMgr.RemoveRide(m_pCurrentRide->GetId()));
	}

	return TRUE;
}

BOOL KRideCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	if(m_pCurrentRide)
	{
		RideSaveData* pRideData = (RideSaveData*)pBuffer;
		SaveRideData(pRideData, m_pCurrentRide);
		pBuffer += sizeof(RideSaveData);
		uUsedSize += sizeof(RideSaveData);
	}

	return TRUE;
}

BOOL KRideCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bRet = FALSE;
	SIZE_T uLoadRideSize = 0;

	if(uLoadRideSize < uSize)
	{
		if (uLoadRideSize + sizeof(RideSaveData) > uSize)
		{
			QLogPrintf(LOG_ERR, "[KRideCtrl::OnDbLoad]	The Size is more than uSize");
			return FALSE;
		}

		RideSaveData* pSaveData = (RideSaveData*)pData;

		INT nIndex = g_cPetMgr.AddRide();
		QCONFIRM_RET_FALSE(nIndex >= 0);

		KRide* pRide = g_cPetMgr.GetRideByIndex(nIndex);
		QCONFIRM_RET_FALSE(pRide);

		bRet = MakeRide(pRide, pSaveData);
		QCONFIRM_RET_FALSE(bRet);

		bRet = Add(pRide, TRUE);
		QCONFIRM_RET_FALSE(bRet);

		bRet = g_cPetMgr.ContactRideWithOwner(pRide, m_rcPlayer.GetId());
		QCONFIRM_RET_FALSE(bRet);

		pData += sizeof(RideSaveData);
		uLoadRideSize += sizeof(RideSaveData);
	}

	return TRUE;
}

BOOL KRideCtrl::Active()
{
    return TRUE;
}

BOOL KRideCtrl::AddHungry(KRide* pRide, INT nHungry)
{
	BOOL bRet = FALSE;
	QCONFIRM_RET_FALSE(pRide);

	bRet = RemoveRideAttrib(pRide);
	QCONFIRM_RET_FALSE(bRet);

	pRide->m_nHungry += nHungry;
	MAKE_IN_RANGE(pRide->m_nHungry, 0, KD_RIDE_HUNGRY_MAX);

	bRet = ApplyRideAttrib(pRide);
	QCONFIRM_RET_FALSE(bRet);

	SyncRideInfo(pRide);

	// 饱食度为0强制下马
	if(pRide->GetHungry() <= 0)
	{
		ChangeRideState(pRide, emRIDE_STATE_REST);
	}

	return TRUE;
}

BOOL KRideCtrl::SyncRideInfo(KRide* pRide)
{
	KS2C_SYNC_RIDE_INFO sSync;

	sSync.byProtocol = (BYTE)s2c_sync_ride_info;
	// 生成同步宠物基本数据
	sSync.dwId = pRide->m_dwId;
	sSync.dwOwnerId = pRide->m_dwOwnerId;
	sSync.byGenre = (BYTE)pRide->m_nGenre;
	sSync.byDetail = (BYTE)pRide->m_nDetail;
	sSync.wParticular = (WORD)pRide->m_nParticular;
	sSync.byLevel = (BYTE)pRide->m_nLevel;
	sSync.byHungry = (BYTE)pRide->m_nHungry;
	sSync.byState = (BYTE)pRide->m_byState;
	sSync.wMagicRate = (WORD)pRide->m_nMagicRate;
	sSync.wRepresentId = (WORD)pRide->m_nRepresentId;
	sSync.dwFightScore[0] = pRide->GetRideFightScore();
	sSync.dwFightScore[1] = pRide->GetNextLevelFightScore();

	return g_cPetModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KRideCtrl::ModifyRideBaseAttrib(KRide* pRide, BOOL bIsEndProcess)
{
	BOOL bRet = FALSE;
	KMagicAttrib sMagicAttib;
	INT nRankId = pRide->GetRankId();

	KRideRankTemplate* pRankTemplate = g_cPetSetting.GetRideRankTemplate(nRankId);
	QCONFIRM_RET_FALSE(pRankTemplate);

	// 坐骑基础属性
	for(INT i = 0; i < emRIDE_COUNT_BASE; ++i)
	{
		if(!pRankTemplate->m_aryRankAttr[i].nAttrib)
			continue;

		INT nValue = pRankTemplate->m_aryRankAttr[i].nValue;

		sMagicAttib.nAttribType = pRankTemplate->m_aryRankAttr[i].nAttrib;
		sMagicAttib.nValue[0] = nValue;
		sMagicAttib.nValue[1] = nValue;

		g_cMagicAttribMgr.ModifyAttrib(&m_rcPlayer, &m_rcPlayer, 0, sMagicAttib, bIsEndProcess);
	}

	return TRUE;
}

BOOL KRideCtrl::ModifyRideAttrib(KRide* pRide, BOOL bIsEndProcess)
{
	INT nValue = 0;
	INT nMinValue = 0;
	KMagicAttrib sMagicAttib;
	KRideRankTemplate* pRankTemplate = NULL;

	PROCESS_ERROR(pRide->GetState() == emRIDE_STATE_UP);

	pRankTemplate = g_cPetSetting.GetRideRankTemplate(pRide->GetRankId());
	LOG_PROCESS_ERROR(pRankTemplate);

	// 坐骑乘骑属性
	for(INT i = 0; i < emRIDE_COUNT_RIDE; ++i)
	{
		if(!pRankTemplate->m_aryRideAttr[i].nAttrib)
			continue;

		nValue = pRankTemplate->m_aryRideAttr[i].nValue;
		// 最小发挥20%乘骑属性
		nMinValue = nValue / 5;
		nValue = nValue * pRide->GetHungry() / 100;

		if(nValue < nMinValue)
			nValue = nMinValue;

		sMagicAttib.nAttribType = pRankTemplate->m_aryRideAttr[i].nAttrib;
		sMagicAttib.nValue[0] = nValue;
		sMagicAttib.nValue[1] = nValue;

		g_cMagicAttribMgr.ModifyAttrib(&m_rcPlayer, &m_rcPlayer, 0, sMagicAttib, bIsEndProcess);
	}

EXIT0:
	return TRUE;
}


BOOL KRideCtrl::ApplyRideAttrib(KRide* pRide)
{
	return pRide->ApplyRideAddAttr();
}

BOOL KRideCtrl::RemoveRideAttrib(KRide* pRide)
{
	return pRide->RemoveRideAddAttr();
}

BOOL KRideCtrl::Add(KRide* pRide, BOOL bSync /* = FALSE */)
{
	QCONFIRM_RET_FALSE(pRide);
	QCONFIRM_RET_FALSE(m_pCurrentRide == NULL);

	pRide->m_dwOwnerId = m_rcPlayer.GetId();
	m_pCurrentRide = pRide;
    
	memset(&pRide->m_cCurrAddAttr, 0, sizeof(KRide::QRIDE_ADD_ATTR));
	pRide->SetMagicRate(pRide->m_nMagicRate);

	if(pRide->GetState() == emRIDE_STATE_UP)
	{
		m_rcPlayer.SetRideRepresentId(pRide->m_nRepresentId);
	}

	pRide->CalcRideFightScore();

	if(bSync)
	{
		SyncRideInfo(pRide);
	}

	return TRUE;
}

BOOL KRideCtrl::SaveRideData(RideSaveData* pBuffer, KRide* pRide)
{
	QCONFIRM_RET_FALSE(pBuffer);
	QCONFIRM_RET_FALSE(pRide);

	pBuffer->byGenre = pRide->m_nGenre;
	pBuffer->byDetail = pRide->m_nDetail;
	pBuffer->wParticular = pRide->m_nParticular;
	pBuffer->byLevel = pRide->m_nLevel;
	pBuffer->byHungry = pRide->m_nHungry;
	pBuffer->byState = pRide->m_byState;
	pBuffer->wMagicRate = (WORD)pRide->m_nMagicRate;
	pBuffer->wRepresentId = (WORD)pRide->m_nRepresentId;

	return TRUE;
}

BOOL KRideCtrl::MakeRide(KRide* pRide, RideSaveData* pSaveData)
{
	QCONFIRM_RET_FALSE(pSaveData);
	QCONFIRM_RET_FALSE(pRide);

	pRide->m_nGenre = pSaveData->byGenre;
	pRide->m_nDetail = pSaveData->byDetail;
	pRide->m_nParticular = pSaveData->wParticular;
	pRide->m_nLevel = pSaveData->byLevel;
	pRide->m_nHungry = pSaveData->byHungry;
	pRide->m_byState = pSaveData->byState;
	pRide->m_nMagicRate = pSaveData->wMagicRate;
	pRide->m_nRepresentId = (INT)pSaveData->wRepresentId;

	KITEM_INDEX sIndex = {pRide->m_nGenre, pRide->m_nDetail, pRide->m_nParticular, pRide->m_nLevel};
	KRideTemplate* pTemplate = g_cPetSetting.GetRideTemplate(sIndex);
	QCONFIRM_RET_FALSE(pTemplate);

	pRide->m_nQuality = pTemplate->m_nQuality;
	pRide->m_nRankId = pTemplate->m_nRankId;

	return TRUE;
}

BOOL KRideCtrl::TakeOffRide(KRide* pRide)
{
	QCONFIRM_RET_FALSE(m_pCurrentRide && m_pCurrentRide == pRide);
	KS2C_TAKEOFF_RIDE sSync;
	sSync.dwRideId = m_pCurrentRide->GetId();
	RemoveRideAttrib(pRide);

	m_rcPlayer.SetRideRepresentId(0);

	QCONFIRM_RET_FALSE(g_cPetMgr.RemoveRide(pRide->GetId()));

	m_pCurrentRide = NULL;

	return g_cPetModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KRideCtrl::RemoveOnly()
{
	QCONFIRM_RET_FALSE(m_pCurrentRide);

	RemoveRideAttrib(m_pCurrentRide);
	m_pCurrentRide = NULL;

	return TRUE;
}

BOOL KRideCtrl::ChangeRideRepresentId(DWORD dwRideId, INT nRepresentId)
{
	KRide* pRide = g_cPetMgr.GetRideById(dwRideId);
	QCONFIRM_RET_FALSE(pRide);

	BOOL bCanUse = FALSE;
	KITEM_INDEX cIndex;
	for (INT i = 1; i <= pRide->GetLevel(); i++)
	{
		cIndex.nGenre = pRide->GetGenre();
		cIndex.nDetailType = pRide->GetDetail();
		cIndex.nParticular = pRide->GetParticular();
		cIndex.nLevel = i;
        KRideTemplate* pTemplate= g_cPetSetting.GetRideTemplate(cIndex);
		QCONFIRM_RET_FALSE(pTemplate);

		if (pTemplate->m_nRepresentId == nRepresentId)
		{
			bCanUse = TRUE;
			break;
		}
	}
	
	if (bCanUse)
	{
		pRide->m_nRepresentId = nRepresentId;
		m_rcPlayer.SetRideRepresentId(nRepresentId);

		SyncRideInfo(pRide);
	}	

	return TRUE;
}

BOOL KRideCtrl::ChangeRideState(KRide* pRide, BYTE byState)
{
	QCONFIRM_RET_FALSE(byState == emRIDE_STATE_UP || byState == emRIDE_STATE_REST);
	QCONFIRM_RET_FALSE(m_pCurrentRide == pRide);

	if(pRide->GetState() == byState)
	{
		return FALSE;
	}

	KScene* pScene = (KScene*)m_rcPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	INT nMapId = pScene->GetTemplateId();

	if(byState == emRIDE_STATE_UP && (g_pSceneMgr->GetSceneType(nMapId) == emtDungeon || nMapId == 97))
	{
		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, m_rcPlayer.GetId(), "DEBUG", "你正在活动地图中，不能骑马");
		return FALSE;
	}

	pRide->SetState(byState);

	if(byState == emRIDE_STATE_REST)
	{
		m_rcPlayer.SetSpeedDelaySet(KD_BASE_RUN_SPEED);
		m_rcPlayer.SetRideRepresentId(0);
	}
	else
	{
		INT nNewSpeed = KD_BASE_RUN_SPEED + (KD_BASE_RUN_SPEED * pRide->m_cCurrAddAttr.nSpeed / 100);
		m_rcPlayer.SetSpeedDelaySet(nNewSpeed);
		m_rcPlayer.SetRideRepresentId(pRide->GetRepresentId());
	}

	return BroadcastRideState(pRide);
}

BOOL KRideCtrl::SyncRideState(KRide* pRide)
{
	KS2C_SYNC_RIDE_STATE sSync;

	sSync.dwRideId = pRide->GetId();
	sSync.byState = pRide->GetState();

	return g_cPetModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KRideCtrl::BroadcastRideLevel(KRide* pRide)
{
	KS2C_SYNC_RIDE_LEVEL sSync;

	sSync.byProtocol = s2c_sync_ride_level;
	sSync.dwRideId = pRide->GetId();
	sSync.byLevel = pRide->GetLevel();

	// broadcast
	KPlayerProtocolProcessor* pProtocolProcessor = g_cPetModule.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = (LPCVOID)&sSync;
	cBroadcaster.m_uSize = sizeof(sSync);
	cBroadcaster.m_pProcessor = pProtocolProcessor;
	//cBroadcaster.m_nRange = 1; // by ldy 设置广播区域

	IKScene* pScene = m_rcPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	return pScene->TraverseNearbyPlayers(cBroadcaster, &m_rcPlayer); // 向自己和自己周围玩家广播
}

BOOL KRideCtrl::BroadcastRideState(KRide* pRide)
{
	KS2C_SYNC_RIDE_STATE sSync;

	sSync.byProtocol = s2c_sync_ride_state;
	sSync.dwRideId = pRide->GetId();
	sSync.byState = pRide->GetState();

	// broadcast
	KPlayerProtocolProcessor* pProtocolProcessor = g_cPetModule.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = (LPCVOID)&sSync;
	cBroadcaster.m_uSize = sizeof(sSync);
	cBroadcaster.m_pProcessor = pProtocolProcessor;
	//cBroadcaster.m_nRange = 1;  // by ldy 设置广播区域

	IKScene* pScene = m_rcPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	return pScene->TraverseNearbyPlayers(cBroadcaster, &m_rcPlayer); // 向自己和自己周围玩家广播
}

BOOL KRideCtrl::SaveRideDelayData(KDelayRideData* pData)
{
	KRide* pRide = GetCurrentRide();

	if(pRide)
	{
		pData->byLevel = (BYTE)pRide->GetLevel();
		pData->nFightScore = pRide->GetRideFightScore();
		pData->nMagicRate = pRide->GetMagicRate();
	}

	return TRUE;
}