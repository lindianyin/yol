#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kcharactermgr.h"
#include "ktowerctrl.h"
#include "../onlinegamemodule/rank/RankRelayClient.h"
#include "serverbase/RankDef.h"
#include "onlinegamebase/ksysservice.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KTowerScript
{

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: StartMopTower
	// @Description		: 开始扫塔
	// @ReturnCode		: 
	// @ArgumentFlag	: ?d
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: 扫塔需要时间
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaStartMopTower(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KTowerCtrl* pTowerCtrl = NULL;

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

		INT nMopTime = sc.GetInt(2);

		pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
		QCONFIRM_RET_FALSE(pTowerCtrl);

		pTowerCtrl->StartMopTower(nMopTime);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: StopMopTower
	// @Description		: 点击停止扫塔
	// @ReturnCode		: 
	// @ArgumentFlag	: ?d
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: 是否有扫完一层
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaStopMopTower(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KTowerCtrl* pTowerCtrl = NULL;

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

		INT nCurLayer = sc.GetInt(2);

		pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
		QCONFIRM_RET_FALSE(pTowerCtrl);

		pTowerCtrl->StopMopTower(nCurLayer);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetSpeedUp
	// @Description		: 得到是否在加速
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetSpeedUp(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KTowerCtrl* pTowerCtrl = NULL;

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

		pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
		QCONFIRM_RET_FALSE(pTowerCtrl);

		sc.PushNumber(pTowerCtrl->sMopInfo.bSpeedUp);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetLeftMopTime
	// @Description		: 得到扫荡剩余时间
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetLeftMopTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KTowerCtrl* pTowerCtrl = NULL;

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

		pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
		QCONFIRM_RET_FALSE(pTowerCtrl);

		sc.PushNumber(pTowerCtrl->sMopInfo.nLeftTime);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetMopStartTime
	// @Description		: 得到扫荡开始时间
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMopStartTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KTowerCtrl* pTowerCtrl = NULL;

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

		pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
		QCONFIRM_RET_FALSE(pTowerCtrl);

		sc.PushNumber(pTowerCtrl->sMopInfo.dwStartMopTime);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: FinishMop
	// @Description		: 直接完成扫荡
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaFinishMop(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KTowerCtrl* pTowerCtrl = NULL;

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

		pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
		QCONFIRM_RET_FALSE(pTowerCtrl);

		pTowerCtrl->OnFinishMop();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SpeedUpMop
	// @Description		: 加速扫荡
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSpeedUpMop(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KTowerCtrl* pTowerCtrl = NULL;

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

		pTowerCtrl = pPlayer->GetCtrl<KTowerCtrl>();
		QCONFIRM_RET_FALSE(pTowerCtrl);

		pTowerCtrl->OnSpeedUp();

		return 0;
	}


	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SyncTowerRank
	// @Description		: 同步伏魔塔排行榜到
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: id  伏魔塔层数
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSyncTowerRank(QLuaScript &sc)
	{
		DWORD RoleID = sc.GetInt(1);
		int TowerLayer = sc.GetInt(2);
		time_t tNow = KSysService::Time(NULL);
		RankRelayClient::DoModifyRankValueRequest(emRANK_TOWER, RoleID, TowerLayer, tNow);
		return 0;
	}


	#include "ktowerscript_table.hpp"
}

BOOL g_RegisterTowerScript()
{
	return g_cScriptManager.RegisterTableFuncs("KTower", KTowerScript::arFunction, countof(KTowerScript::arFunction));
}