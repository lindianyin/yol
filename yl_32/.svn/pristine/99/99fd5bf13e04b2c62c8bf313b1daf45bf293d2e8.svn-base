
#pragma once

#ifdef GAME_SERVER

#include "config/kplayerbasedbdef.h"
#include "config/kplayerctrldbdef.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/ikscene.h"

// -------------------------------------------------------------------------

class KPlayerDbManager
{
public:
	BOOL Init();
	BOOL Uninit();

	BOOL CheckRoleData(const KROLE_DATA* pRoleData, SIZE_T uDataSize);
	BOOL OnPlayerDbLoad(KPlayer& rPlayer, const KROLE_DATA& rRoleData, SIZE_T uSize);
	BOOL OnPlayerDbSave(KPlayer& rPlayer, BOOL bLeaving);

	BOOL LoadBaseInfo2(KPlayer& rPlayer, const DB_PLAYER_BASE_DATA* pBaseInfo, SIZE_T uSize);

	BOOL SaveRole(KPlayer& rPlayer, KROLE_DATA& rRoleData, SIZE_T uSize, SIZE_T& ruUsedSize);
	BOOL LoadRole(KPlayer& rPlayer, const KROLE_DATA& rRoleData, SIZE_T uSize);

protected:
private:
	const KPLAYER_SAVE_DEF* GetPlayerSaveCtrl(KPlayer& rPlayer, KE_DB_ROLE_BLOCK_TYPE eBlockType);

	SIZE_T SaveBaseInfo(KPlayer& rPlayer, DB_PLAYER_BASE_DATA& rBaseInfo, SIZE_T uSize);
};

extern	KPlayerDbManager	g_cPlayerDbMgr;
// -------------------------------------------------------------------------

#endif // GAME_SERVER
