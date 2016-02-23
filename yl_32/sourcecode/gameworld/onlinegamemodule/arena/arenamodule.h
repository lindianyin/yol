#pragma once
#include "onlinegameworld/kmodulebase.h"

class KPlayer;

class ArenaModule : public KModuleBase
{

public:
	ArenaModule();
	virtual ~ArenaModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData );

	BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize );

	BOOL SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize );

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	// protocol
	BOOL c2sOnFightRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);
	BOOL c2sRefreshArenaPlayers(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);
	BOOL c2sOnOfflineFightRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	// r2s
	BOOL OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId);

	BOOL OnGetRankIdxRespond(DWORD dwSrcId, DWORD dwPlayerId,INT nRankIdx);

	BOOL OnGetRankIdRespond(DWORD dwSrcId, DWORD dwPlayerId,INT nRankIdx);

	BOOL OnSwapRankRespond(std::string szSrcPlayerName, INT nSrcRank, DWORD dwDestId, INT nDestRank, BOOL bSucceed);
};

extern ArenaModule g_ArenaModule;