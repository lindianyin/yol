#include "stdafx.h"
#include "opensemanager.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "playeropense.h"
#include "opensedef.h"

OpenseManager g_cOpenseManager;
OpenseManager::OpenseManager()
{

}

OpenseManager::~OpenseManager()
{

}

BOOL OpenseManager::Init()
{
	return TRUE;
}

BOOL OpenseManager::UnInit()
{
	return TRUE;
}

BOOL OpenseManager::Breath()
{
	return TRUE;
}

BOOL OpenseManager::DoOpense(INT playerId, INT nDay, INT nId)
{
	BOOL bResult = FALSE;
	PlayerOpense* pPlayerOpense = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(playerId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerOpense = pPlayer->GetCtrl<PlayerOpense>();
	LOG_PROCESS_ERROR(pPlayerOpense);
	LOG_PROCESS_ERROR(pPlayerOpense->DoOpense(nDay, nId));
	bResult = TRUE;
EXIT0:
	return bResult;
}