
#pragma once

#ifdef GAME_CLIENT

#include "onlinegameworld/kidobjpool.h"
#include "Runtime/qstring.h"
#include "onlinegameworld/kplayer.h"

// -------------------------------------------------------------------------
class KPlayerMgr
{
public:
	BOOL Init();
	BOOL Uninit();
	BOOL Activate();

	INT Add(LPCSTR szName, DWORD dwPlayerId);
	BOOL RemoveById(DWORD dwId);

	KPlayer* GetByIndex(INT nPlayerIndex);
	KPlayer* GetById(DWORD dwId);
	KPlayer* GetByName(const QString& strName);
	INT GetAllPlayers(std::vector<KPlayer*>& rvPlayers);
	// 获取玩家数
	INT GetPlayerCount() { return m_cObjMgr.GetUsedCount(); }
private:
	TKIdObjPool<KPlayer, KD_PLAYER_ID_MASK> m_cObjMgr; // 玩家对象池
	std::map<QString, KPlayer*>		m_mapNameToPlayer; // 名字到Player对象映射
};

extern KPlayerMgr g_cPlayerMgr;
// -------------------------------------------------------------------------
#endif // GAME_CLIENT

