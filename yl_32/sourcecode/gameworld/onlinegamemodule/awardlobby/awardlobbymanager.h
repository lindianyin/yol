#pragma once
#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "../../LibQrt/Include/Misc/objevent.h"

class AwardLobbyManager
{
public:
	AwardLobbyManager();
	~AwardLobbyManager();

	BOOL Init();
	BOOL UnInit();
	BOOL Breath();
};

extern AwardLobbyManager g_cAwardLobbyManager;