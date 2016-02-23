#include "stdafx.h"
#include "targetmanager.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "playertarget.h"
#include "targetdef.h"

TargetManager g_cTargetManager;
TargetManager::TargetManager()
{

}

TargetManager::~TargetManager()
{

}

BOOL TargetManager::Init()
{
	return TRUE;
}

BOOL TargetManager::UnInit()
{
	return TRUE;
}

BOOL TargetManager::Breath()
{
	return TRUE;
}

BOOL TargetManager::DoTarget(INT playerId, INT nDay, INT nId)
{
	PlayerTarget* pPlayerTarget = NULL;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(playerId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerTarget = pPlayer->GetCtrl<PlayerTarget>();
	LOG_PROCESS_ERROR(pPlayerTarget);
	pPlayerTarget->DoTarget(nDay, nId);

	return TRUE;
EXIT0:
	return FALSE;
}