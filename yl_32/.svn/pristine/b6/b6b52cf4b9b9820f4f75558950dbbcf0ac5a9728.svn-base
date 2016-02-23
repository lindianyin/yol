#pragma once
#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "../../LibQrt/Include/Misc/objevent.h"

class VipManager : public KObjEventWatch
{
public:
	VipManager();
	~VipManager();

	BOOL Init();
	BOOL UnInit();
	BOOL Breath();

	virtual BOOL OnEvent(KEventCreator& rcSender, CONST KOBJEVENT& rcEvent);

	BOOL IsOpen(DWORD dwPlayerId, INT nId, BOOL bSendMsg = 1);

	INT GetTotalBuyCount(DWORD dwPlayerId, INT nId);

	INT GetNowBuyCount(DWORD dwPlayerId, INT nId);

	INT GetRate(DWORD dwPlayerId, INT nId);

	BOOL RefreshBuyCount(DWORD dwPlayerId);
};

extern VipManager g_cVipManager;