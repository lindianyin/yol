#include "stdafx.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluaplayer.h"
#include "luaplayergem.h"
#include "playergem.h"
#include "gemmanager.h"

INT LuaPlayerGem::Register2Player(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	PlayerGem* pPlayerGem= pPlayer->GetCtrl<PlayerGem>();
	QCONFIRM_RET_FALSE(pPlayerGem);
	sc.PushObj(pPlayerGem->GetScriptInterface());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerGem
// @LuaApiName		: OnUse
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: dddd
// @ArgumentComment	: g
// @ArgumentComment	: d
// @ArgumentComment	: p
// @ArgumentComment	: l
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerGem::LuaOnUse(QLuaScript& sc)
{
	INT	nGenre			= sc.GetInt(1);
	INT	nDetailType		= sc.GetInt(2);
	INT	nParticular		= sc.GetInt(3);
	INT	nLevel			= sc.GetInt(4);

	PlayerGem* pGem = m_rcCharacter.GetCtrl<PlayerGem>();
	BOOL bOk = pGem->OnUse(nGenre, nDetailType, nParticular, nLevel);
	sc.PushNumber(bOk);

EXIT0:
	return 1;
}

#include "luaplayergem_table.hpp"