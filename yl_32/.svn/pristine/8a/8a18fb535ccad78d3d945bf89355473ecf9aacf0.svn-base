#pragma once
#include "onlinegameworld/kmodulebase.h"
#include "awardlobbyprotocol.h"

class KPlayer;

class AwardLobbyModule : public KModuleBase
{
public:
	AwardLobbyModule();
	virtual ~AwardLobbyModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

private:
	BOOL c2sOnSignedInRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sOnSignedInSyncRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);
};

extern AwardLobbyModule g_cAwardLobbyModule;