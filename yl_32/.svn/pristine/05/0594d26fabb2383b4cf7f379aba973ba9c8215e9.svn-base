

#include "stdafx.h"
#include "kluatask.h"
#include "onlinegamemodule/ktaskctrl.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KLuaTask::KLuaTask( KPlayer& rPlayer ) : m_rcPlayer(rPlayer)
{
	m_pTaskCtrl = rPlayer.GetCtrl<KTaskCtrl>();
}

INT KLuaTask::Register2Character( QLuaScript& sc, QLunaBase* pLunaObj )
{
	KLuaCharacter* pLuaCharacter = (KLuaCharacter*)pLunaObj;
	QCONFIRM_RET_FALSE(pLuaCharacter);

	KCharacter* pCharacter = pLuaCharacter->GetHim();
	QCONFIRM_RET_FALSE(pCharacter);

	KTaskCtrl* pTaskCtrl = pCharacter->GetCtrl<KTaskCtrl>();
	QCONFIRM_RET_FALSE(pTaskCtrl);
	//QCONFIRM_RET_FALSE(pTaskCtrl->GetScriptInterface()); // TODO:角色下线报红by kk
	sc.PushObj(pTaskCtrl->GetScriptInterface());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: GetTask
// @Description		: 获取任务变量
// @ReturnCode		: 成功-返回变量值，失败-返回nil
// @ArgumentFlag	: uu
// @ArgumentComment	: wGroupID:任务变量组ID
// @ArgumentComment	: wTaskID:任务变量组内的子ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaGetTask(QLuaScript& sc)
{
	WORD wGroupID	= (WORD)sc.GetInt(1);
	WORD wTaskID	= (WORD)sc.GetInt(2);

	INT nValue	= m_pTaskCtrl->GetTaskValueEx(wGroupID, wTaskID);

	sc.PushNumber(nValue);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: SetTask
// @Description		: 设置任务变量
// @ReturnCode		: 无返回值
// @ArgumentFlag	: uud[d]
// @ArgumentComment	: wGroupID:任务变量组ID
// @ArgumentComment	: wTaskID:任务变量组内的子ID
// @ArgumentComment	: nTaskValue:变量值
// @ArgumentComment	: nSyncMode:同步模式(0:Auto, 1:Force, -1:No)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaSetTask(QLuaScript& sc)
{
	WORD wGroupID	= (WORD)sc.GetInt(1);
	WORD wTaskID	= (WORD)sc.GetInt(2);
	INT nTaskValue	= sc.GetInt(3);
	INT nSyncMode	= sc.GetInt(4);	// 0:Auto, 1:Force, -1:No

	m_pTaskCtrl->SetTaskValueEx(wGroupID, wTaskID, nTaskValue, nSyncMode);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: DirectSetTask
// @Description		: 直接设置任务变量
// @ReturnCode		: 无返回值
// @ArgumentFlag	: uud[d]
// @ArgumentComment	: wGroupID:任务变量组ID
// @ArgumentComment	: wTaskID:任务变量组内的子ID
// @ArgumentComment	: nTaskValue:变量值
// @ArgumentComment	: nSyncMode:同步模式(0:Auto, 1:Force, -1:No)
// @LuaMarkEnd
// -------------------------------------------------------------------------
BOOL g_bDirectSetTaskValue = FALSE;
INT KLuaTask::LuaDirectSetTask(QLuaScript& sc)
{
	WORD wGroupID	= (WORD)sc.GetInt(1);
	WORD wTaskID	= (WORD)sc.GetInt(2);
	INT nTaskValue	= sc.GetInt(3);
	INT nSyncMode	= sc.GetInt(4);	// 0:Auto, 1:Force, -1:No
	g_bDirectSetTaskValue = TRUE;
	m_pTaskCtrl->SetTaskValueEx(wGroupID, wTaskID, nTaskValue, nSyncMode);
	g_bDirectSetTaskValue = FALSE;
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: GetTaskBit
// @Description		: 将一组所有任务变量(65536个)的值按位排列(0-65536*32)，按位计算出TaskID(每32位为一个ID)，并获取对应变量值的位值
// @ReturnCode		: 成功-返回指定位的值，失败-返回nil
// @ArgumentFlag	: uu
// @ArgumentComment	: wGroupID:任务变量组ID
// @ArgumentComment	: dwBit:指定的位(范围：0-65536*32)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaGetTaskBit(QLuaScript& sc)
{
	WORD wGroupID	= (WORD)sc.GetInt(1);
	DWORD dwBit		= (DWORD)sc.GetInt(2);
	if (wGroupID && dwBit)
	{
		dwBit	+= 31;
		WORD wTaskID	= (WORD)(dwBit >> 5);
		DWORD dwValue	= m_pTaskCtrl->GetTaskValue(MAKELONG(wTaskID, wGroupID));
		sc.PushNumber((dwValue >> (dwBit & 31)) & 1);
	}
	else
	{
		sc.PushNull();
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: SetTaskBit
// @Description		: 将一组所有任务变量(65536个)的值按位排列(0-65536*32)，按位计算出TaskID(每32位为一个ID)，并设置对应变量值的位值
// @ReturnCode		: 无返回值
// @ArgumentFlag	: uud
// @ArgumentComment	: wGroupID:任务变量组ID
// @ArgumentComment	: dwBit:指定的位(范围：0-65536*32)
// @ArgumentComment	: bValue:位值(0 or 1)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaSetTaskBit(QLuaScript& sc)
{
	WORD wGroupID	= (WORD)sc.GetInt(1);
	DWORD dwBit		= (DWORD)sc.GetInt(2);
	BOOL bValue		= (BOOL)sc.GetInt(3);
	if (wGroupID && dwBit)
	{
		dwBit	+= 31;
		WORD wTaskID	= (WORD)(dwBit >> 5);
		DWORD dwTaskID	= ((DWORD)wGroupID << 16) | wTaskID;
		DWORD dwValue	= m_pTaskCtrl->GetTaskValue(dwTaskID);
		DWORD dwMask	= 1 << (dwBit & 31);
		if (bValue)
			dwValue	|= dwMask;
		else
			dwValue	= dwValue &~ dwMask;
		m_pTaskCtrl->SetTaskValue(dwTaskID, dwValue);
	}

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: ClearTaskGroup
// @Description		: 清除一组任务变量
// @ReturnCode		: 无返回值
// @ArgumentFlag	: u[d]
// @ArgumentComment	: wGroupID:任务变量组ID
// @ArgumentComment	: bSync:是否同步到客户端(服务端有效)(0 or 1)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaClearTaskGroup(QLuaScript& sc)
{
	WORD wGroupID	= (WORD)sc.GetInt(1);
	BOOL bSync		= (BOOL)sc.GetInt(2);
	if (wGroupID)
	{
		DWORD dwMin	= ((DWORD)wGroupID) << 16;
		m_pTaskCtrl->ClearTaskValue(dwMin, dwMin | 0xffff);
	}

#ifdef GAME_SERVER
	if (bSync)
	{
		m_pTaskCtrl->SyncTaskGroup(wGroupID);
	}
#endif

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: GetAllTask
// @Description		: 获取全部Task变量
// @ReturnCode		: {[nTaskId]=nValue,[nTaskId]=nValue,...}
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaGetAllTask(QLuaScript& sc)
{
	KTaskCtrl::TTaskMap& mapTask	= m_pTaskCtrl->mapSave;
	KTaskCtrl::TTaskMap::const_iterator it;

	sc.PushTable();
	for (it = mapTask.begin(); it != mapTask.end(); ++it)
	{
		sc.PushNumber(it->second);
		sc.SetTableIndex(it->first);
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: SendAccept
// @Description		: 向服务端/客户端发送接受任务请求
// @ReturnCode		: 1、成功；nil、失败
// @ArgumentFlag	: dd[d]
// @ArgumentComment	: nTaskId:主任务ID
// @ArgumentComment	: nReferId:引用子任务ID
// @ArgumentComment	: bAccept:是否接受
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaSendAccept(QLuaScript& sc)
{
	INT nTaskID		= sc.GetInt(1);
	INT nSubTaskIdx	= sc.GetInt(2);
	BOOL bAccept	= (sc.GetTopIndex() >= 3) ? sc.GetInt(3) : FALSE;

	BOOL bOK = m_pTaskCtrl->SendAccept(nTaskID, nSubTaskIdx, bAccept);
	sc.PushNumber(bOK);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: SendAward
// @Description		: 向服务端/客户端发送任务奖励请求
// @ReturnCode		: 1、成功；nil、失败
// @ArgumentFlag	: dd[d]
// @ArgumentComment	: nTaskId:主任务ID
// @ArgumentComment	: nReferId:引用子任务ID
// @ArgumentComment	: nSelIdx:所选项的索引
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaSendAward(QLuaScript& sc)
{
	INT nTaskID		= sc.GetInt(1);
	INT nSubTaskIdx	= sc.GetInt(2);
	INT nSelIdx		= sc.GetInt(3);

	BOOL bOK = m_pTaskCtrl->SendAward(nTaskID, nSubTaskIdx, nSelIdx);
	sc.PushNumber(bOK);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: GetTaskStr
// @Description		: 获取任务变量(每次使用4个任务变量,最长16个字符长度,角色名的长度)
// @ReturnCode		: 成功-返回变量值，失败-返回nil
// @ArgumentFlag	: uu
// @ArgumentComment	: wGroupID:任务变量组ID
// @ArgumentComment	: wTaskID:任务变量组内的子ID,需要4个子ID,此ID为起点ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaGetTaskStr(QLuaScript& sc)
{
	WORD wGroupID	= (WORD)sc.GetInt(1);
	WORD wTaskID	= (WORD)sc.GetInt(2);
	CHAR szTaskStr[64];
	LPINT pnTaskStr = (LPINT)szTaskStr;
	for (INT i = 0; i <= 7; i++)
	{
		pnTaskStr[i]	= m_pTaskCtrl->GetTaskValueEx(wGroupID, (wTaskID + i));
	}
	szTaskStr[32]	= '\0';
	sc.PushString(szTaskStr);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaTask
// @LuaApiName		: SetTaskStr
// @Description		: 设置任务变量(每次使用8个任务变量,最长32个字符长度)
// @ReturnCode		: 无返回值
// @ArgumentFlag	: uus[d]
// @ArgumentComment	: wGroupID:任务变量组ID
// @ArgumentComment	: wTaskID:任务变量组内的子ID,需要4个子ID,此ID为起点ID
// @ArgumentComment	: szTaskValue:变量值(32个字符,16个汉字,角色名长度8个汉字,预留8个汉字越南版);
// @ArgumentComment	: nSyncMode:同步模式(0:Auto, 1:Force, -1:No)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaSetTaskStr(QLuaScript& sc)
{
	INT nResult		= 0;
	WORD wGroupID	= (WORD)sc.GetInt(1);
	WORD wTaskID	= (WORD)sc.GetInt(2);
	LPCSTR pszTaskValue	= sc.GetStr(3);
	INT nSyncMode	= sc.GetInt(4);	// 0:Auto, 1:Force, -1:No
	LPINT pnTaskValue = (LPINT)pszTaskValue;
	LOG_PROCESS_ERROR(strlen(pszTaskValue) <= 32);
	for (INT i=0; i <= 7; i++)
	{
		m_pTaskCtrl->SetTaskValueEx(wGroupID, (wTaskID + i), pnTaskValue[i], nSyncMode);
	}
	nResult = 1;
EXIT0:
	sc.PushNumber(nResult);
	return 1;
}

#ifdef GAME_SERVER
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaTask
// @LuaApiName		: SendRefresh
// @Description		: 向客户端发送刷新要求
// @ReturnCode		: 1、成功；nil、失败
// @ArgumentFlag	: dd[d]
// @ArgumentComment	: nTaskId:主任务ID
// @ArgumentComment	: nReferId:引用子任务ID
// @ArgumentComment	: nTaskGroupID:该任务保存的TaskGroup
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaSendRefresh(QLuaScript& sc)
{
	INT nTaskID		= sc.GetInt(1);
	INT nSubTaskIdx	= sc.GetInt(2);
	INT nTaskGroupID= sc.GetInt(3);

	BOOL bOK = m_pTaskCtrl->SendRefresh(nTaskID, nSubTaskIdx, nTaskGroupID);
	sc.PushNumber(bOK);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaTask
// @LuaApiName		: StartProgressTimer
// @Description		: 开启一个计时器，并设置客户端的进度条
// @ReturnCode		: 无
// @ArgumentFlag	: ods
// @ArgumentComment	: pPlayer
// @ArgumentComment	: 间隔时间
// @ArgumentComment	: 描述文字
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaStartProgressTimer(QLuaScript& sc)
{
	//KPlayer* pPlayer	= KLuaTask::GetMe(sc, 1);
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
// @ClassName		: KLuaTask
// @LuaApiName		: StepStart
// @Description		: 通知客户端目标开始
// @ReturnCode		: 无
// @ArgumentFlag	: ddd
// @ArgumentComment	: 任务Id
// @ArgumentComment	: 子任务Id
// @ArgumentComment	: 步骤数
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaStepStart(QLuaScript& sc)
{
	INT nTaskID		= sc.GetInt(1);
	INT nSubTaskIdx	= sc.GetInt(2);
	INT nStepIdx	= sc.GetInt(3);
	BOOL bOK = m_pTaskCtrl->StepStart(nTaskID, nSubTaskIdx, nStepIdx);
	sc.PushNumber(bOK);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaTask
// @LuaApiName		: SyncTaskGroup
// @Description		: 同步一组任务变量
// @ReturnCode		: 成功：1, 失败：0
// @ArgumentFlag	: u
// @ArgumentComment	: 任务变量组ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaTask::LuaSyncTaskGroup(QLuaScript& sc)
{
	WORD wGroupID = (WORD)sc.GetInt(1);
	BOOL bOK = m_pTaskCtrl->SyncTaskGroup(wGroupID);
	sc.PushNumber(bOK);
	return 1;
}

#else

#endif

#include "kluatask_table.hpp"


// -------------------------------------------------------------------------
