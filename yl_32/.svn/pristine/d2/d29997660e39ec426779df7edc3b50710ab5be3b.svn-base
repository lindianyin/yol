#include "stdafx.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluaplayer.h"
#include "luaplayertarget.h"
#include "playertarget.h"
#include "targetmanager.h"

INT LuaPlayerTarget::Register2Player(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	PlayerTarget* pPlayerTarget= pPlayer->GetCtrl<PlayerTarget>();
	QCONFIRM_RET_FALSE(pPlayerTarget);
	sc.PushObj(pPlayerTarget->GetScriptInterface());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerTarget
// @LuaApiName		: DoTarget
// @Description		: 添加目标
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 第几天
// @ArgumentComment	: id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerTarget::LuaDoTarget(QLuaScript& sc)
{
	INT nDay = sc.GetInt(1);
	INT nId = sc.GetInt(2);
	BOOL bOk = g_cTargetManager.DoTarget(m_rcCharacter.GetId(), nDay, nId);
	sc.PushNumber(bOk);
	return 1;
}

#include "luaplayertarget_table.hpp"