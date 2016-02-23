
#pragma once

#include "ksocketinterface.h"

// -------------------------------------------------------------------------
class KPlayer;

class KGameCenterProcess : public IKSocketEventProcessor
{
public:
	virtual BOOL Init(INT nMaxConnectId);
	// 处理事件
	virtual BOOL ProcessEvent(INT nConnectId, INT nEventId, LPCVOID pData, INT nData);
	// 处理协议
	virtual BOOL ProcessPacket(INT nConnectId, LPCBYTE pData, UINT uDataLen);
	virtual BOOL Activate();

	BOOL OnPlayerVarChange(KPlayer& rPlayer, WORD wVarGroup, WORD wVarId, INT nValue);
	BOOL OnPlayerSave(KPlayer& rPlayer);
	BOOL DoSendServerInfo(INT nConnectId);

	BOOL DoResyncLoginPlayers(INT nConnectId);

	void SyncLoginPlayer(INT nConnectId, KPlayer* pPlayer);

private:
	BOOL DoPing(UINT uNow);
private:
	UINT m_uLastPing;

};

extern KGameCenterProcess g_cGameCenterProcess;
// -------------------------------------------------------------------------

