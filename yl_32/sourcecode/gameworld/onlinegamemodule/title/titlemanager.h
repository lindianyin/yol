#pragma once
#include "stdafx.h"
#include "../../LibQrt/Include/Misc/objevent.h"
#include "onlinegameworld/kplayer.h"

class TitleManager : public KObjEventWatch
{
public:
	TitleManager();
	~TitleManager();

	BOOL Init();
	BOOL UnInit();
	BOOL Breath();

	virtual BOOL OnEvent(KEventCreator& rcSender, CONST KOBJEVENT& rcEvent);

	BOOL AddTitleToCharacter(DWORD dwId, INT nGetWay, INT nLevel, INT nType = 0, DWORD dwPassTime = 0);
	BOOL RemoveTitleToCharacter(DWORD dwId, INT nGetWay, INT nLevel, INT nType = 0);
};

extern TitleManager g_cTitleManager;