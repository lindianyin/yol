
#pragma once
#ifdef GAME_CLIENT
// -------------------------------------------------------------------------
#include "ksocketinterface.h" 

// -------------------------------------------------------------------------
class KNetConnectPlayer : public IKSocketEventProcessor_Client
{
public:
	virtual BOOL ProcessEvent(INT nEventId, LPCVOID pData, INT nData);
	virtual BOOL ProcessPacket(LPCBYTE pData, UINT uDataLen);
};

extern KNetConnectPlayer g_cNetConnectPlayer;
// -------------------------------------------------------------------------
#endif // #ifdef GAME_CLIENT

