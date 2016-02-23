
#pragma once

#ifdef GAME_SERVER
#include "onlinegameworld/kcharacterserver.h"
// -------------------------------------------------------------------------
class KNpc;
class KPlayer;
struct KCHARACTER_VAR_SYNC_OTHER;

class KNpcServer : public KCharacterServer
{
public:
	KNpcServer(KNpc& rNpc);
	BOOL SyncChangeRegion();
	BOOL SyncToNeighborPlayers(std::vector<KPlayer*>& vPlayers);
	BOOL SyncToOldNeighborPlayers(std::vector<KPlayer*>& vPlayers);

	BOOL FillSyncVar( KCHARACTER_VAR_SYNC_OTHER& rsVarSync, SIZE_T uInLen, SIZE_T& ruOutLen );
	BOOL TeleportTo( DWORD dwSceneId, INT nX, INT nY, INT nZ );	
protected:
	KNpc& m_rcNpc;
};

// -------------------------------------------------------------------------
#endif // #ifdef GAME_SERVER
