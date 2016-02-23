#include "stdafx.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluaplayer.h"
#include "luaplayeropense.h"
#include "playeropense.h"
#include "opensemanager.h"

INT LuaPlayerOpense::Register2Player(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	PlayerOpense* pPlayerOpense = pPlayer->GetCtrl<PlayerOpense>();
	QCONFIRM_RET_FALSE(pPlayerOpense);
	sc.PushObj(pPlayerOpense->GetScriptInterface());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerOpense
// @LuaApiName		: DoOpense
// @Description		: 添加目标
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 第几天
// @ArgumentComment	: id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerOpense::LuaDoOpense(QLuaScript& sc)
{
	INT nDay = sc.GetInt(1);
	INT nId = sc.GetInt(2);
	BOOL bOk = g_cOpenseManager.DoOpense(m_rcCharacter.GetId(), nDay, nId);
	sc.PushNumber(bOk);
	return 1;
}

#include "luaplayeropense_table.hpp"