#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kcharactermgr.h"
#include "kescortctrl.h"
#include "kescortrelayclient.h"
#include "../onlinegamemodule/arena/ArenaHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KEscortScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshEscortPlayers
	// @Description		: 刷新修仙场景里的玩家
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nPlayerId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshEscortPlayers(QLuaScript &sc)
	{
		INT nPlayerId = sc.GetInt(1);

		KEscortRelayClient::DoRefreshPlayerEscorts(nPlayerId);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: ClearEscortData
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nPlayerId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaClearEscortData(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		INT nPlayerId = sc.GetInt(1);
		QCONFIRM_RET_FALSE(IS_PLAYER(nPlayerId));
		pPlayer = (KPlayer*)g_cCharacterMgr.GetById(nPlayerId);

		QCONFIRM_RET_FALSE(pPlayer);

		KEscortCtrl* pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
		pEscortCtrl->ClearEscortData();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RemoveEscortWatch
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nPlayerId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRemoveEscortWatch(QLuaScript &sc)
	{
		INT nPlayerId = sc.GetInt(1);

		KEscortRelayClient::DoRemoveEscortWatchRequest(nPlayerId);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SpeedUpEscort
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: dd
	// @ArgumentComment	: nPlayerId
	// @ArgumentComment	: bSpeedUp
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSpeedUpEscort(QLuaScript &sc)
	{
		INT nPlayerId = sc.GetInt(1);
		BOOL bSpeed = sc.GetInt(2);

		KEscortRelayClient::DoSpeedEscortRequest(nPlayerId, bSpeed);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetLastRefreshTime
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: ?d
	// @ArgumentComment	: nPlayerId or pPlayer
	// @ArgumentComment	: tTime
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetLastRefreshTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		if (sc.IsNumber(1))
		{
			INT nPlayerId = sc.GetInt(1);
			QCONFIRM_RET_FALSE(IS_PLAYER(nPlayerId));
			pPlayer = (KPlayer*)g_cCharacterMgr.GetById(nPlayerId);
		}
		else
		{
			KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
			if (pLuaPlayer)
				pPlayer = pLuaPlayer->GetHim();
		}
		QCONFIRM_RET_FALSE(pPlayer);

		KEscortCtrl* pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
		QCONFIRM_RET_FALSE(pEscortCtrl);

		DWORD dwTime = sc.GetInt(2);

		pEscortCtrl->SetLastRefreshTime(dwTime);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetLastRefreshTime
	// @Description		: 得到上次刷新坐骑时间
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId or pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetLastRefreshTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		if (sc.IsNumber(1))
		{
			INT nPlayerId = sc.GetInt(1);
			QCONFIRM_RET_FALSE(IS_PLAYER(nPlayerId));
			pPlayer = (KPlayer*)g_cCharacterMgr.GetById(nPlayerId);
		}
		else
		{
			KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
			if (pLuaPlayer)
				pPlayer = pLuaPlayer->GetHim();
		}
		QCONFIRM_RET_FALSE(pPlayer);

		KEscortCtrl* pEscortCtrl = pPlayer->GetCtrl<KEscortCtrl>();
		QCONFIRM_RET_FALSE(pEscortCtrl);

		DWORD dwTime = pEscortCtrl->GetLastRefreshTime();
		sc.PushNumber(dwTime);

		return 1;
	}

	#include "kescortscript_table.hpp"
}

BOOL g_RegisterEscortScript()
{
	return g_cScriptManager.RegisterTableFuncs("KEscort", KEscortScript::arFunction, countof(KEscortScript::arFunction));
}