#include "stdafx.h"
#include "awardlobbymanager.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "playerawardlobby.h"
#include "awardlobbydef.h"

AwardLobbyManager g_cAwardLobbyManager;
AwardLobbyManager::AwardLobbyManager()
{

}

AwardLobbyManager::~AwardLobbyManager()
{

}

BOOL AwardLobbyManager::Init()
{
	return TRUE;
}

BOOL AwardLobbyManager::UnInit()
{
	return TRUE;
}

BOOL AwardLobbyManager::Breath()
{
	return TRUE;
}
