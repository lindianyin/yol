#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kcharactermgr.h"
#include "krankctrl.h"
#include "krankmanager.h"
#include "onlinegamebase/ksysservice.h"
#include "../onlinegamemodule/rank/RankRelayClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KRankScript
{

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: ModifyRankValue
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: ddd
	// @ArgumentComment	: 排行榜id（排行唯一依据）
	// @ArgumentComment	: nRankId
	// @ArgumentComment	: nValue
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaModifyRankValue(QLuaScript &sc)
	{
		DWORD dwId = sc.GetInt(1);
		INT nRankId = sc.GetInt(2);
		INT nValue = sc.GetInt(3);
		time_t tNow = KSysService::Time(NULL);
		QLogPrintf(LOG_INFO,"..LuaModifyRankValue.. nRankId=%d nValue=%d",nRankId,nValue);
		RankRelayClient::DoModifyRankValueRequest(nRankId, dwId, nValue, tNow);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetRank
	// @Description		: 设置排名
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetRank(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KRankCtrl* pRankCtrl = NULL;

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

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshRoleRank
	// @Description		: 刷新战斗力排行
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshRoleRank(QLuaScript &sc)
	{
		g_cRankManager.InitRoleRank();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshPetLvRank
	// @Description		: 刷新宠物等级排行
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshPetLvRank(QLuaScript &sc)
	{
		g_cRankManager.InitPetLvRank();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshPetFSRank
	// @Description		: 刷新宠物战斗力排行
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshPetFSRank(QLuaScript &sc)
	{
		g_cRankManager.InitPetFSRank();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshRideRank
	// @Description		: 刷新坐骑排行
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshRideRank(QLuaScript &sc)
	{
		g_cRankManager.InitRideRank();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshMpjjRank
	// @Description		: 刷新门派竞技排行
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshMpjjRank(QLuaScript &sc)
	{
		g_cRankManager.InitMpjjRank();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshLGRank
	// @Description		: 刷新炼卦排行
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshLGRank(QLuaScript &sc)
	{
		g_cRankManager.InitLGRank();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshSmzcRank
	// @Description		: 刷新神魔战场排行
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshSmzcRank(QLuaScript &sc)
	{
		g_cRankManager.InitSmzcRank();

		return 0;
	}


	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshTongFSRank
	// @Description		: 刷新家族战斗力排行
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshTongFSRank(QLuaScript &sc)
	{
		g_cRankManager.InitTongFSRank();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshTongSrRank
	// @Description		: 刷新家族贡献度排行
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshTongSrRank(QLuaScript &sc)
	{
		g_cRankManager.InitTongSrRank();

		return 0;
	}



	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RefreshAllRank
	// @Description		: 刷新全部排行榜
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRefreshAllRank(QLuaScript &sc)
	{
		g_cRankManager.InitOpenServerRanks();
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetMyRank
	// @Description		: 得到我的排名
	// @ReturnCode		: 
	// @ArgumentFlag	: ?d
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: nRankId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMyRank(QLuaScript &sc)
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

		INT nRankId = sc.GetInt(2);

		sc.PushNumber(g_cRankManager.GetPlayerRank(nRankId, pPlayer->GetId()));

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetLastTakeAwardTime
	// @Description		: 得到上次领奖时间
	// @ReturnCode		: 
	// @ArgumentFlag	: ?d
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: nRankId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetLastTakeAwardTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KRankCtrl* pRankCtrl = NULL;

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

		INT nRankId = sc.GetInt(2);

		pRankCtrl = pPlayer->GetCtrl<KRankCtrl>();
		QCONFIRM_RET_FALSE(pRankCtrl);

		sc.PushNumber(pRankCtrl->GetLastTakeAwardTime(nRankId));

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetLastTakeAwardTime
	// @Description		: 设置上次领奖时间
	// @ReturnCode		: 
	// @ArgumentFlag	: ?dd
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: nRankId
	// @ArgumentComment	: time
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetLastTakeAwardTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KRankCtrl* pRankCtrl = NULL;

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

		INT nRankId = sc.GetInt(2);
		DWORD dwTime = sc.GetInt(3);

		pRankCtrl = pPlayer->GetCtrl<KRankCtrl>();
		QCONFIRM_RET_FALSE(pRankCtrl);

		pRankCtrl->SetLastTakeAwardTime(nRankId, dwTime);

		return 0;
	}

	#include "krankscript_table.hpp"
}

BOOL g_RegisterRankScript()
{
	return g_cScriptManager.RegisterTableFuncs("KRank", KRankScript::arFunction, countof(KRankScript::arFunction));
}