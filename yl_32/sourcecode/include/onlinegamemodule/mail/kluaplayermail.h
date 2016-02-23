/* -------------------------------------------------------------------------
//	文件名		：	kluaplayermail.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/24 9:31:26
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KLUAPLAYERMAIL_H__
#define __KLUAPLAYERMAIL_H__

// -------------------------------------------------------------------------


class KMailCtrl;

class KLuaPlayerMail : public QLunaBase
{
public:
	KLuaPlayerMail(KMailCtrl& rcMailCtrl) : m_rcMailCtrl(rcMailCtrl) {}
	
	static INT RegisterToPlayer(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(KLuaPlayerMail);

public:
#ifdef GAME_CLIENT
	INT LuaRequestMailList(QLuaScript& sc);
	INT LuaRequestMail(QLuaScript& sc);
	INT LuaFetchItem(QLuaScript& sc);
	INT LuaDeleteMail(QLuaScript& sc);
	INT LuaSendMail(QLuaScript& sc);
#endif /* GAME_CLIENT */

#ifdef GAME_SERVER
	INT LuaOverSendLimit(QLuaScript& sc);
	INT LuaAddSendTime(QLuaScript& sc);
	INT LuaCheckReceiver(QLuaScript& sc);
	INT LuaSendClientResult(QLuaScript& sc);
	INT LuaSendMail2GC(QLuaScript& sc);
#endif /* GAME_SERVER */

private:
	KMailCtrl& m_rcMailCtrl;
};

// -------------------------------------------------------------------------

#endif /* __KLUAPLAYERMAIL_H__ */
