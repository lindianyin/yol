
#include "stdafx.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegamemodule/ktaskctrl.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/synchelper.h"
#include "ktaskmodule.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

namespace KTaskScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: SendAccept
	// @Description		: 向服务端/客户端发送接受任务请求
	// @ReturnCode		: 1、成功；nil、失败
	// @ArgumentFlag	: odd[d]
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: nTaskId:主任务ID
	// @ArgumentComment	: nReferId:引用子任务ID
	// @ArgumentComment	: bAccept:是否接受
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSendAccept(QLuaScript& sc)
	{
		KPlayer* pPlayer = KLuaPlayer::GetHim(sc, 1);
		QCONFIRM_RET_FALSE(pPlayer);

		INT nTaskID		= sc.GetInt(2);
		INT nSubTaskIdx	= sc.GetInt(3);
		BOOL bAccept	= (sc.GetTopIndex() >= 4) ? sc.GetInt(4) : FALSE;

		BOOL bRet = pPlayer->m_PlayerTask.SendAccept(nTaskID, nSubTaskIdx, bAccept);

		sc.PushNumber(bRet);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: Dump
	// @Description		: 输出任务变量信息
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaDump(QLuaScript& sc)
	{
		KPlayer* pPlayer = KLuaPlayer::GetHim(sc, 1);
		QCONFIRM_RET_FALSE(pPlayer);

		pPlayer->m_PlayerTask.Dump("Task Dump");
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: SendAward
	// @Description		: 向服务端/客户端发送任务奖励请求
	// @ReturnCode		: 1、成功；nil、失败
	// @ArgumentFlag	: odd[d]
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: nTaskId:主任务ID
	// @ArgumentComment	: nReferId:引用子任务ID
	// @ArgumentComment	: nSelIdx:所选项的索引
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSendAward(QLuaScript& sc)
	{
		KPlayer* pPlayer = KLuaPlayer::GetHim(sc, 1);
		QCONFIRM_RET_FALSE(pPlayer);

		INT nTaskID		= sc.GetInt(2);
		INT nSubTaskIdx	= sc.GetInt(3);
		INT nSelIdx		= sc.GetInt(4);

		BOOL bRet = pPlayer->m_PlayerTask.SendAward(nTaskID, nSubTaskIdx, nSelIdx);

		sc.PushNumber(bRet);
		return 1;
	}
	
#ifdef GAME_SERVER
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SendRefresh
	// @Description		: 向客户端发送刷新要求
	// @ReturnCode		: 1、成功；nil、失败
	// @ArgumentFlag	: odd[d]
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: nTaskId:主任务ID
	// @ArgumentComment	: nReferId:引用子任务ID
	// @ArgumentComment	: nTaskGroupID:该任务保存的TaskGroup
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSendRefresh(QLuaScript& sc)
	{
		KPlayer* pPlayer = KLuaPlayer::GetHim(sc, 1);
		QCONFIRM_RET_FALSE(pPlayer);

		INT nTaskID		= sc.GetInt(2);
		INT nSubTaskIdx	= sc.GetInt(3);
		INT nTaskGroupID= sc.GetInt(4);

		BOOL bRet = pPlayer->m_PlayerTask.SendRefresh(nTaskID, nSubTaskIdx, nTaskGroupID);

		sc.PushNumber(bRet);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: StartProgressTimer
	// @Description		: 开启一个计时器，并设置客户端的进度条
	// @ReturnCode		: 无
	// @ArgumentFlag	: ods
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 间隔时间
	// @ArgumentComment	: 描述文字
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaStartProgressTimer(QLuaScript& sc)
	{
		//KPlayer* pPlayer = KLuaPlayer::GetHim(sc, 1);
		//INT nIntervalTime = sc.GetInt(2);
		//LPCSTR szText = sc.GetStr(3);

		//pPlayer->RegisterProcessEvent(KTaskTimerList::EVENT_TASK_TAG);
		//// 若已经注册了合成配方的计时器则关闭这个计时器，然后通知界面。
		//if (pPlayer->m_listTaskTimer.HasTimer(KE_TIMERID_TASKTAG))
		//{
		//	pPlayer->m_listTaskTimer.CloseTimer(pPlayer, KE_TIMERID_TASKTAG);
		//}

		//if (pPlayer->m_listTaskTimer.StartTimer(
		//	KE_TIMERID_TASKTAG, nIntervalTime, 1, KTaskTimer::MODE_LOSTTIME, 0, 0 ,NULL, 
		//	1 << KTaskTimerList::EVENT_MOVE | 1 << KTaskTimerList::EVENT_ATTACK |
		//	1 << KTaskTimerList::EVENT_ATTACKED | 1 << KTaskTimerList::EVENT_DEATH |
		//	1 << KTaskTimerList::EVENT_TRADE | 1 << KTaskTimerList::EVENT_SITE |
		//	1 << KTaskTimerList::EVENT_RIDE  | 1 << KTaskTimerList::EVENT_GENERAL_PROCESS))
		//{
		//	pPlayer->m_dwEvent = 0;
		//	PLAYER_UISCRIPT_SYNC UiInfo;
		//	UiInfo.cProtocol	= s2c_playeruiscript;
		//	UiInfo.m_bUIId = UI_PROGRESSBAR;
		//	UiInfo.m_bOptionNum = 0;
		//	UiInfo.m_nBufferLen = sizeof(KPLAYER_UI_DATA::KPLAYER_PROGRESSBAR);
		//	UiInfo.m_bParam1 = 0;
		//	UiInfo.m_bParam2 = 1;

		//	KPLAYER_UI_DATA::KPLAYER_PROGRESSBAR * pData = (KPLAYER_UI_DATA::KPLAYER_PROGRESSBAR*) UiInfo.m_pContent;
		//	pData->dwTimerId = KE_TIMERID_TASKTAG;
		//	pData->dwInterVal = nIntervalTime;
		//	pData->szActionName[0] = 0;
		//	strcpy(pData->szActionName, szText);
		//	UiInfo.wProtocolSize = sizeof(UiInfo) - KD_UISCIRPT_BUFFER_MAX + UiInfo.m_nBufferLen - 1;
		//	g_pCoreServer->SendData(pPlayer->m_nNetConnectIdx, (LPBYTE)&UiInfo, UiInfo.wProtocolSize + 1);
		//}
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: StepStart
	// @Description		: 通知客户端目标开始
	// @ReturnCode		: 无
	// @ArgumentFlag	: oddd
	// @ArgumentComment	: 玩家对象
	// @ArgumentComment	: 任务Id
	// @ArgumentComment	: 子任务Id
	// @ArgumentComment	: 步骤数
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaStepStart(QLuaScript& sc)
	{
		KPlayer* pPlayer = KLuaPlayer::GetHim(sc, 1);
		QCONFIRM_RET_FALSE(pPlayer);

		INT nTaskID		= sc.GetInt(2);
		INT nSubTaskIdx	= sc.GetInt(3);
		INT nStepIdx	= sc.GetInt(4);

		BOOL bOK = pPlayer->m_PlayerTask.StepStart(nTaskID, nSubTaskIdx, nStepIdx);
		sc.PushNumber(bOK);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetBlackSky
	// @Description		: 通知客户端开启或结束黑屏模式
	// @ReturnCode		: 无
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 是否开启黑屏模式
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetBlackSky(QLuaScript& sc)
	{
		KPlayer* pPlayer = KLuaPlayer::GetHim(sc, 1);
		BOOL bDark	= sc.GetInt(2);

		QCONFIRM_RET_FALSE(pPlayer);

		pPlayer->m_PlayerTask.SendTaskCommon(emKTASK_SET_BLACKSKY, 0, 0, bDark);
		
		return 0;
	}
#else

#endif

#include "ktaskscript_table.hpp"
}

BOOL g_RegisterTaskScript()
{
	return g_cScriptManager.RegisterTableFuncs("KTask", KTaskScript::arFunction, countof(KTaskScript::arFunction));
}

