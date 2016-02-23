#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kcharactermgr.h"
#include "kstagectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KStageScript
{

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: StartMopStage
	// @Description		: 开始扫荡关卡
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: nTotalTime
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaStartMopStage(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		INT nTotalTime = sc.GetInt(2);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		pStageCtrl->StartMopStage(nTotalTime);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetRushStartTime
	// @Description		: 得到开始挑战关卡时间
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetRushStartTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		sc.PushNumber(pStageCtrl->GetRushStartTime());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetRushStartTime
	// @Description		: 设置开始挑战关卡时间
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 开始挑战时间
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetRushStartTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);


		DWORD dwTime = sc.GetInt(2);

		pStageCtrl->SetRushStartTime(dwTime);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetRushLayer
	// @Description		: 得到当前挑战完的关卡数
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetRushLayer(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		sc.PushNumber(pStageCtrl->GetRushLayer());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetRushLayer
	// @Description		: 设置当前挑战完的关卡数
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 关卡数
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetRushLayer(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);


		INT nLayer = sc.GetInt(2);

		pStageCtrl->SetRushLayer(nLayer);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetRushDifficulty
	// @Description		: 得到当前挑战的关卡难度
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetRushDifficulty(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		sc.PushNumber(pStageCtrl->GetRushDifficulty());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetRushDifficulty
	// @Description		: 设置当前挑战关卡难度
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 关卡难度
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetRushDifficulty(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		INT nDifficulty = sc.GetInt(2);

		pStageCtrl->SetRushDifficulty(nDifficulty);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetStageStar
	// @Description		: 设置挑战星级
	// @ReturnCode		: 
	// @ArgumentFlag	: odd
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 难度
	// @ArgumentComment	: 星级
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetStageStar(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		INT nDifficulty = sc.GetInt(2); 
		INT nStar = sc.GetInt(3);

		pStageCtrl->SetStageStar(nDifficulty, nStar);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetStageStar
	// @Description		: 得到挑战星级
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 难度
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetStageStar(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		INT nDifficulty = sc.GetInt(2); 

		sc.PushNumber(pStageCtrl->GetStageStar(nDifficulty));

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetLeftMopTime
	// @Description		: 得到剩余扫荡时间
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetLeftMopTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		sc.PushNumber(pStageCtrl->GetMopLeftTime());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: StopMopStage
	// @Description		: 点击停止扫荡关卡
	// @ReturnCode		: 
	// @ArgumentFlag	: ?d
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: 是否有扫完一层
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaStopMopStage(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KStageCtrl* pStageCtrl = NULL;

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

		INT bFinish = sc.GetInt(2);

		pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		pStageCtrl->StopMopStage(bFinish);

		return 0;
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
		KStageCtrl* pStageCtrl = NULL;

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

		pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		pStageCtrl->OnFinishMop();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetMopStage
	// @Description		: 得到当前正在扫荡的关卡难度
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMopStage(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		sc.PushNumber(pStageCtrl->GetMopStage());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetMopState
	// @Description		: 得到当前扫荡状态
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMopState(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		sc.PushNumber(pStageCtrl->GetMopState());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetMopStage
	// @Description		: 设置当前正在扫荡的关卡难度
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 关卡难度
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetMopStage(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);


		INT nStage = sc.GetInt(2);

		pStageCtrl->SetMopStage(nStage);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetRushState
	// @Description		: 得到当前闯关状态
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetRushState(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		sc.PushNumber(pStageCtrl->GetRushState());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetRushState
	// @Description		: 
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 闯关
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetRushState(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		KStageCtrl* pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		INT nState = sc.GetInt(2);

		pStageCtrl->SetRushState(nState);

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
		KStageCtrl* pStageCtrl = NULL;

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

		pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		pStageCtrl->OnSpeedUp();

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
		KStageCtrl* pStageCtrl = NULL;

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

		pStageCtrl = pPlayer->GetCtrl<KStageCtrl>();
		QCONFIRM_RET_FALSE(pStageCtrl);

		sc.PushNumber(pStageCtrl->sMopInfo.bSpeedUp);

		return 1;
	}


	#include "kstagescript_table.hpp"
}

BOOL g_RegisterStageScript()
{
	return g_cScriptManager.RegisterTableFuncs("KStage", KStageScript::arFunction, countof(KStageScript::arFunction));
}