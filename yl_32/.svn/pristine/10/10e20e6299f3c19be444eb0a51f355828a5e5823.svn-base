#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "remind/remindmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
namespace KRemindScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddRemindToPlayer
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: 索引
	// @ArgumentComment	: 提醒id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddRemindToPlayer(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));

		INT nRemindId = sc.GetInt(2);
		INT nCountDown = sc.GetInt(3);
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		QCONFIRM_RET_FALSE(pPlayer);
		g_cRemindManger.AddRemindToPlayer(pPlayer->GetPlayerIndex(), nRemindId, nCountDown);
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RemoveRemindToPlayer
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: 索引
	// @ArgumentComment	: 提醒id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRemoveRemindToPlayer(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));

		INT nRemindId = sc.GetInt(2);
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		QCONFIRM_RET_FALSE(pPlayer);
		g_cRemindManger.RemoveRemindToPlayer(pPlayer->GetPlayerIndex(), nRemindId);
		return 0;
	}


#include "remindscript_table.hpp"
}

BOOL g_RegisterRemindScript()
{
	return g_cScriptManager.RegisterTableFuncs("KRemind", KRemindScript::arFunction, countof(KRemindScript::arFunction));
}