

#include "stdafx.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegamemodule/chat/kluaplayerchat.h"
#include "onlinegamemodule/chat/kplayerchat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
INT KLuaPlayerChat::Register2Player(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	KPlayerChat* pKPlayerChat = pPlayer->GetCtrl<KPlayerChat>();
	QCONFIRM_RET_FALSE(pKPlayerChat);
	sc.PushObj(&pKPlayerChat->GetScriptInterface());

	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerChat
// @LuaApiName		: SendMsg
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: dds
// @ArgumentComment	: 频道ID
// @ArgumentComment	: 目标ID
// @ArgumentComment	: 消息内容
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerChat::LuaSendMsg(QLuaScript& sc)
{
	BOOL bRet = m_rcPlayerChat.SendMsg(sc.GetInt(1), sc.GetInt(2), sc.GetStr(3));
	sc.PushNumber(bRet);

	return 1;
}

#ifdef GAME_CLIENT
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaPlayerChat
// @LuaApiName		: SendMsgToPlayer
// @Description		: 给玩家发消息
// @ReturnCode		: 
// @ArgumentFlag	: dss
// @ArgumentComment	: 频道id
// @ArgumentComment	: 玩家名
// @ArgumentComment	: 消息内容
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerChat::LuaSendMsgToPlayer(QLuaScript& sc)
{
	BOOL bRet = m_rcPlayerChat.SendMsgToPlayer(sc.GetInt(1), sc.GetStr(2), sc.GetStr(3));
	sc.PushNumber(bRet);
	return 1;
}
#endif // #ifdef GAME_CLIENT

// -------------------------------------------------------------------------
#include "kluaplayerchat_table.hpp"
