/* -------------------------------------------------------------------------
//	文件名		：	kluanpc.h
//	创建者		：	simon
//	创建时间	：	2009/12/3 17:09:27
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KLUANPC_H__
#define __KLUANPC_H__

#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/knpc.h"

// -------------------------------------------------------------------------

class KLuaNpc : public KLuaCharacter
{
public:
	static KNpc* GetHim(QLuaScript& sc, INT nIndex);

	explicit KLuaNpc(KNpc* pNpc) : KLuaCharacter(pNpc), m_pNpc(pNpc)
	{
	}
	virtual ~KLuaNpc()
	{
	}

	KNpc* GetHim()	{ return m_pNpc; };
	BOOL	ClearTempTable();

	// 脚本接口定义
	DECLARE_LUA_DRIVATE_CLASS(KLuaNpc, KLuaCharacter);

	DWORD	getId(VOID) CONST;
	DWORD	getTemplateId(VOID) CONST;
	LPCSTR		getNickName()	CONST;
	VOID			setNickName(LPCSTR pszNickName);

	LPCSTR		getDisplayName(VOID) CONST;
	LPCSTR		getClassName(VOID) CONST;

	INT			LuaGetCombatGroup(QLuaScript& sc);
	INT			LuaGetCombatState(QLuaScript& sc);

	INT			LuaGetDeleteState(QLuaScript& sc);

	INT			LuaSetPosition(QLuaScript& sc); // 临时调试用

	LPCSTR		getTitle(VOID) CONST;

	INT			LuaGetThreatList(QLuaScript& sc);
	INT			LuaClearAllThreat(QLuaScript& sc);
	INT			LuaCopyThreatList(QLuaScript& sc);

	INT         LuaCallClientScriptOfNearByPlayer(QLuaScript& sc);

#ifdef GAME_SERVER
	INT			LuaTeleportTo(QLuaScript& sc); // 瞬移
	INT			LuaSetTeamId(QLuaScript& sc); // Npc分组Id
#endif // GAME_SERVER

#ifdef GAME_CLIENT
	INT			LuaGetRepresentPosition(QLuaScript& sc);
#endif // GAME_CLIENT

protected:
	KNpc* m_pNpc;
};

// -------------------------------------------------------------------------

#endif /* __KLUANPC_H__ */
