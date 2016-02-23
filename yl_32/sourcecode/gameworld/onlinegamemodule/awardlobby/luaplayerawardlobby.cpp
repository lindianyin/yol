#include "stdafx.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluaplayer.h"
#include "luaplayerawardlobby.h"
#include "playerawardlobby.h"
#include "awardlobbymanager.h"
#include "bank/playerbank.h"

INT LuaPlayerAwardLobby::Register2Player(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	PlayerAwardLobby* pPlayerAwardLobby= pPlayer->GetCtrl<PlayerAwardLobby>();
	QCONFIRM_RET_FALSE(pPlayerAwardLobby);
	sc.PushObj(pPlayerAwardLobby->GetScriptInterface());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerAwardLobby
// @LuaApiName		: DoFinishActivity
// @Description		: 完成活动
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: id
// @ArgumentComment	: awardid
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerAwardLobby::LuaDoFinishActivity(QLuaScript& sc)
{
	INT nId = sc.GetInt(1);
	INT nAwardId = sc.GetInt(2);
	PlayerAwardLobby* pPlayerLobby = m_rcCharacter.GetCtrl<PlayerAwardLobby>();

	BOOL bOk = pPlayerLobby->DoFinishActivity(nId, nAwardId);
	sc.PushNumber(bOk);

EXIT0:
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerAwardLobby
// @LuaApiName		: DoFinishRankActivity
// @Description		: 完成冲榜活动
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: id
// @ArgumentComment	: awardid
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerAwardLobby::LuaDoFinishRankActivity(QLuaScript& sc)
{
	INT nId = sc.GetInt(1);
	INT nAwardId = sc.GetInt(2);
	PlayerAwardLobby* pPlayerLobby = m_rcCharacter.GetCtrl<PlayerAwardLobby>();

	BOOL bOk = pPlayerLobby->DoFinishRankActivity(nId, nAwardId);
	sc.PushNumber(bOk);

EXIT0:
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerAwardLobby
// @LuaApiName		: DoSignIn
// @Description		: 完成活动
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 日期天数
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerAwardLobby::LuaDoSignIn(QLuaScript& sc)
{
	INT nDay = sc.GetInt(1);
	PlayerAwardLobby* pPlayerLobby = m_rcCharacter.GetCtrl<PlayerAwardLobby>();

	BOOL bOk = pPlayerLobby->PlayerSignedIn(nDay);
	sc.PushNumber(bOk);

EXIT0:
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerAwardLobby
// @LuaApiName		: DoGetBankIncomeTest
// @Description		: 完成活动
// @ReturnCode		: 
// @ArgumentFlag	: ds
// @ArgumentComment	: id
// @ArgumentComment	: 日期
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerAwardLobby::LuaDoGetBankIncomeTest(QLuaScript& sc)
{
	INT nId = sc.GetInt(1);
	LPCSTR szForbitType = sc.GetStr(2);
	PlayerBank* pPlayerBank = m_rcCharacter.GetCtrl<PlayerBank>();

	BOOL bOk = pPlayerBank->DoGetIncomeTest(nId, (CHAR*)szForbitType);
	sc.PushNumber(bOk);

EXIT0:
	return 1;
}

#include "luaplayerawardlobby_table.hpp"