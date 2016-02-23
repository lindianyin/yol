#include "stdafx.h"
#include "vipmanager.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "playervip.h"
#include "vipdef.h"

VipManager g_cVipManager;
VipManager::VipManager()
{

}

VipManager::~VipManager()
{

}

BOOL VipManager::Init()
{
	Watch(emKOBJEVENTTYPE_CHARACTER_VIP_LEVEL);
	return TRUE;
}

BOOL VipManager::UnInit()
{
	CancelWatch(emKOBJEVENTTYPE_CHARACTER_VIP_LEVEL);
	return TRUE;
}

BOOL VipManager::Breath()
{
	return TRUE;
}

BOOL VipManager::IsOpen(DWORD dwPlayerId, INT nId, BOOL bSendMsg)
{
	PlayerVip* pPlayerVipt = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwPlayerId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerVipt = pPlayer->GetCtrl<PlayerVip>();
	LOG_PROCESS_ERROR(pPlayerVipt);
	return pPlayerVipt->IsOpen(nId, bSendMsg);

EXIT0:
	return FALSE;
}

INT VipManager::GetTotalBuyCount(DWORD dwPlayerId, INT nId)
{
	PlayerVip* pPlayerVip = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwPlayerId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerVip = pPlayer->GetCtrl<PlayerVip>();
	LOG_PROCESS_ERROR(pPlayerVip);
	return pPlayerVip->GetTotalBuyCount(nId);

EXIT0:
	return 0;
}

INT VipManager::GetNowBuyCount(DWORD dwPlayerId, INT nId)
{
	PlayerVip* pPlayerVip = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwPlayerId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerVip = pPlayer->GetCtrl<PlayerVip>();
	LOG_PROCESS_ERROR(pPlayerVip);
	return pPlayerVip->GetNowBuyCount(nId);

EXIT0:
	return 0;
}

INT VipManager::GetRate(DWORD dwPlayerId, INT nId)
{
	PlayerVip* pPlayerVip = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwPlayerId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerVip = pPlayer->GetCtrl<PlayerVip>();
	LOG_PROCESS_ERROR(pPlayerVip);
	return pPlayerVip->GetRate(nId);

EXIT0:
	return 0;
}

BOOL VipManager::RefreshBuyCount(DWORD dwPlayerId)
{
	PlayerVip* pPlayerVip = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwPlayerId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerVip = pPlayer->GetCtrl<PlayerVip>();
	LOG_PROCESS_ERROR(pPlayerVip);
	return pPlayerVip->RefreshSaveData();

EXIT0:
	return FALSE;
}


BOOL VipManager::OnEvent(KEventCreator& rcSender, CONST KOBJEVENT& rcEvent)
{

	INT nId = rcEvent.anEventParam[0];
	INT nOld = rcEvent.anEventParam[1];
	INT nNew = rcEvent.anEventParam[2];
	KPlayer* pPlayer = NULL;
	PlayerVip* pPlayerVip = NULL;
	BOOL bResult = FALSE;

	PROCESS_ERROR(rcEvent.nEvent == emKOBJEVENTTYPE_CHARACTER_VIP_LEVEL);

	pPlayer = g_cPlayerMgr.GetById(nId);
	LOG_PROCESS_ERROR(pPlayer);

	pPlayerVip = pPlayer->GetCtrl<PlayerVip>();
	LOG_PROCESS_ERROR(pPlayerVip);

	// 不是体验状态
	LOG_PROCESS_ERROR(!pPlayerVip->IsFreeExpState(nNew, nOld));

	if(nNew >= VIP_FREE_EXP_LEVEL)
	{
		pPlayerVip->ResetFreeState();
	}
	pPlayerVip->SetRecFreeVipLevel(nNew);

	for (INT i = nOld+1; i <= nNew; i++)
	{
		pPlayerVip->DoRecevie(i);
	}
	if (nNew > nOld)
	{
		pPlayerVip->DoAddDegree(nOld, nNew);
	}

	bResult = TRUE;

EXIT0:
	return bResult;
}