#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kcharactermgr.h"
#include "kcrosssvctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KCrossSvScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: ClearDropItems
	// @Description		: 清空记录掉落道具
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaClearDropItems(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KCrossSvCtrl* pCrossSvCtrl = NULL;

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

		pCrossSvCtrl = pPlayer->GetCtrl<KCrossSvCtrl>();
		QCONFIRM_RET_FALSE(pCrossSvCtrl);

		pCrossSvCtrl->ClearDropItems();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RecordDropItems
	// @Description		: 记录掉落道具
	// @ReturnCode		: 
	// @ArgumentFlag	: ?ddddd
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: g
	// @ArgumentComment	: d
	// @ArgumentComment	: p
	// @ArgumentComment	: l
	// @ArgumentComment	: amount
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRecordDropItems(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KCrossSvCtrl* pCrossSvCtrl = NULL;

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

		pCrossSvCtrl = pPlayer->GetCtrl<KCrossSvCtrl>();
		QCONFIRM_RET_FALSE(pCrossSvCtrl);

		INT nGenre = sc.GetInt(2);
		INT nDetail = sc.GetInt(3);
		INT nParticular = sc.GetInt(4);
		INT nLevel = sc.GetInt(5);
		INT nAmount = sc.GetInt(6);

		pCrossSvCtrl->RecordDropItems(nGenre, nDetail, nParticular, nLevel, nAmount);

		return 0;
	}

	#include "kcrosssvscript_table.hpp"
}

BOOL g_RegisterCrossSvScript()
{
	return g_cScriptManager.RegisterTableFuncs("KCrossSv", KCrossSvScript::arFunction, countof(KCrossSvScript::arFunction));
}