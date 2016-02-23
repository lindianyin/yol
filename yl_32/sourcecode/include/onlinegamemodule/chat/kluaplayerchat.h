
#pragma once


class KPlayerChat;
// -------------------------------------------------------------------------
class KLuaPlayerChat : public QLunaBase
{
public:
	KLuaPlayerChat(KPlayerChat& rcPlayerChat)
		:m_rcPlayerChat(rcPlayerChat){}

	static INT Register2Player(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(KLuaPlayerChat);

	// -------------------------------------------------------------------------
	INT LuaSendMsg(QLuaScript& sc);

#ifdef GAME_CLIENT
	INT LuaSendMsgToPlayer(QLuaScript& sc);
#endif

private:
	KPlayerChat& m_rcPlayerChat;
};

// -------------------------------------------------------------------------

