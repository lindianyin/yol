#pragma once
#include "stdafx.h"
#include "onlinegameworld/kplayer.h"

class OpenseManager
{
public:
	OpenseManager();
	~OpenseManager();

	BOOL Init();
	BOOL UnInit();
	BOOL Breath();
	BOOL DoOpense(INT playerId, INT nDay, INT nId);
};

extern OpenseManager g_cOpenseManager;