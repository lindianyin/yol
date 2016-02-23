#pragma once
#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "../../LibQrt/Include/Misc/objevent.h"

class GemManager
{
public:
	GemManager();
	~GemManager();

	BOOL Init();
	BOOL UnInit();
	BOOL Breath();
};

extern GemManager g_cGemManager;