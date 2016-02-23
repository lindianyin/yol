
#pragma once

#ifdef GAME_SERVER

#include <map>
#include "Misc/qidpool.h"
#include "Runtime/qstring.h"
#include "onlinegameworld/kidobjpool.h"
#include "onlinegameworld/kplayer.h"

// -------------------------------------------------------------------------
class KPlayerMgr
{
public:
	KPlayerMgr();
	// 初始化，nMaxPlayer最大玩家数
	BOOL Init();
	BOOL Uninit();
	BOOL Activate();

	INT Add(LPCSTR pszName, DWORD dwId = KD_BAD_ID);
	KPlayer* Add2(LPCSTR pszName, DWORD dwId = KD_BAD_ID);
	BOOL RemoveById(DWORD dwId);
	BOOL RemoveByIndex(INT nIndex);

	// 获取玩家对象指针，失败返回NULL
	KPlayer* GetByIndex(INT nPlayerIndex);
	KPlayer* GetById(DWORD dwId);
	KPlayer* GetByName(const QString& strName);
	// 获取最大玩家数
	INT GetMaxPlayer() { return m_cObjMgr.GetMaxCount(); }
	// 获取当前（在线）玩家数
	INT GetPlayerCount() { return m_cObjMgr.GetUsedCount(); }

	//获取当前所有在线玩家id
	INT GetAllPlayerId(std::vector<DWORD>* pvecPlayerId);
	// 获得在线玩家列表快照（不要保存pPlayer指针，因为Player下线会重用，需要保存的存Id）
	INT GetAllPlayers(std::vector<KPlayer*>& rvPlayers);

private:
	BOOL OnRegularProcess(KPlayer* pPlayer);
protected:
	TKIdObjPool<KPlayer, KD_PLAYER_ID_MASK> m_cObjMgr;
	std::map<QString, KPlayer*>		m_mapNameToPlayer;
private:
	TIME_T m_timeActivate;
};

extern KPlayerMgr g_cPlayerMgr;
// -------------------------------------------------------------------------
#endif // #ifdef GAME_SERVER

