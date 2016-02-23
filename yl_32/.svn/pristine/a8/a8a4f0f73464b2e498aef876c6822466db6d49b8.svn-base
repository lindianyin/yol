#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kcharactermgr.h"
#include "kinstancectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KInstanceScript
{

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetMultiStartTime
	// @Description		: 得到开始多人副本时间
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMultiStartTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		sc.PushNumber(pInstanceCtrl->sMultiInfo.dwStartTime);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetMultiStartTime
	// @Description		: 设置开始多人副本时间
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 开始挑战时间
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetMultiStartTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		DWORD dwTime = sc.GetInt(2);

		pInstanceCtrl->sMultiInfo.dwStartTime = dwTime;

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetMultiState
	// @Description		: 得到当前多人副本状态
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMultiState(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		sc.PushNumber(pInstanceCtrl->sMultiInfo.nState);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetMultiState
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 闯关
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetMultiState(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		INT nState = sc.GetInt(2);

		pInstanceCtrl->sMultiInfo.nState = nState;

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetMultiRushStar
	// @Description		: 得到多人副本评星
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMultiRushStar(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		sc.PushNumber(pInstanceCtrl->sMultiInfo.nRushStar);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetMultiRushStar
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 评星
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetMultiRushStar(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		INT nStar = sc.GetInt(2);

		pInstanceCtrl->sMultiInfo.nRushStar = nStar;

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetMultiRushMapId
	// @Description		: 得到多人副本地图id
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMultiRushMapId(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		sc.PushNumber(pInstanceCtrl->sMultiInfo.nRushMapId);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetMultiRushMapId
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 评星
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetMultiRushMapId(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		INT nMapId = sc.GetInt(2);

		pInstanceCtrl->sMultiInfo.nRushMapId = nMapId;

		pInstanceCtrl->SyncMultiInsInfo();

		return 0;
	}


	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: IsMultiMapPassed
	// @Description		: 是否通关了mapId的多人副本
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: mapId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaIsMultiMapPassed(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		INT nMapId = sc.GetInt(2);

		sc.PushNumber(pInstanceCtrl->IsMultiMapPassed(nMapId));

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetMultiMapPassed
	// @Description		: 设置mapId的多人副本已通关状态
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: mapId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetMultiMapPassed(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KInstanceCtrl* pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		QCONFIRM_RET_FALSE(pInstanceCtrl);

		INT nMapId = sc.GetInt(2);

		pInstanceCtrl->SetMultiMapPassed(nMapId);

		return 0;
	}


	#include "kinstancescript_table.hpp"
}

BOOL g_RegisterInstanceScript()
{
	return g_cScriptManager.RegisterTableFuncs("KInstance", KInstanceScript::arFunction, countof(KInstanceScript::arFunction));
}