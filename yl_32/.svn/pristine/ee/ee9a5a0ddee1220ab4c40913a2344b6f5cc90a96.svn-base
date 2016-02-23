
#include "stdafx.h"
#include "onlinegamemodule/ktaskctrl.h"
#include "ktaskmodule.h"
#include "kluatask.h"
#include "ktasksetting.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/korpgworld.h"

#ifdef GAME_SERVER
#include "onlinegameworld/kplayermgr.h"
#else
#include "onlinegameworld/kplayermanager_client.h"
#include "onlinegameworld/korpgworld.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

BOOL KTaskCtrl::Init()
{
	m_pLuna = new KLuaTask((KPlayer&)m_rcCharacter);
	Release();

	return (NULL != m_pLuna);
}

BOOL KTaskCtrl::UnInit()
{
	Release();
	SAFE_DELETE(m_pLuna);
	return TRUE;
}

BOOL KTaskCtrl::CallScript(LPCSTR pszFunName, LPCSTR pszFormat, ...)
{
	BOOL bOK = FALSE;

#ifdef GAME_SERVER
	g_cScriptManager.SetMe((QLunaBase*)((KPlayer&)m_rcCharacter).GetScriptInterface());
#endif

	va_list vlist;
	va_start(vlist, pszFormat);
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	bOK = cSafeScript->CallTableFunctionV("Task", pszFunName, 0, pszFormat, vlist);
	va_end(vlist);

#ifdef GAME_SERVER
	g_cScriptManager.SetMe(NULL);
#endif
	return bOK;
}

//调试函数
VOID KTaskCtrl::Dump(LPCTSTR szMsg)
{
#ifdef _DEBUG
	printf("--------------- %s -------------------\n", szMsg);
	printf("GroupID\t\tTaskID\t\tValue\n");
	TTaskMap::const_iterator it = mapSave.begin();
	for (; it != mapSave.end(); ++it)
	{
		DWORD dwID = it->first;
		WORD wGroupID = WORD(dwID >> 16);
		WORD wTaskID = WORD(dwID & 0x0000FFFF);
		printf("%d\t\t%d\t\t%d\n", wGroupID, wTaskID, it->second);
	}
#endif
}

BOOL KTaskCtrl::SendTaskCommon(BYTE byType, INT nTaskID, INT nSubTaskIdx, INT nParam)
{
	TASK_SYNC_EVENT TaskEvent;
	TaskEvent.byType = byType;
	TaskEvent.nTaskID = nTaskID;
	TaskEvent.nSubTaskIdx = nSubTaskIdx;
	TaskEvent.nParam = nParam;

	KPlayerProtocolProcessor* pProtocolHandler = g_sTaskModule.GetProtocolProcessor();
#ifdef GAME_SERVER
	KPlayer* pPlayer = (KPlayer*)&m_rcCharacter;
	return pProtocolHandler->SendData(pPlayer->GetPlayerIndex(), TaskEvent);
#else
	return pProtocolHandler->SendData(TaskEvent);
#endif
}

BOOL KTaskCtrl::SendAccept(INT nTaskID, INT nSubTaskIdx, BOOL bAccept)
{
	TASK_SYNC_EVENT TaskEventEx;
	TaskEventEx.byType = emKTASK_ACCEPT;
	TaskEventEx.nTaskID = nTaskID;
	TaskEventEx.nSubTaskIdx = nSubTaskIdx;
	TaskEventEx.nParam = bAccept;

	KPlayerProtocolProcessor* pProtocolHandler = g_sTaskModule.GetProtocolProcessor();
#ifdef GAME_SERVER
	KPlayer* pPlayer = (KPlayer*)&m_rcCharacter;
	return pProtocolHandler->SendData(pPlayer->GetPlayerIndex(), TaskEventEx);
#else
	return pProtocolHandler->SendData(TaskEventEx);
#endif
}


//同步指定任务变量到客户/服务器端
BOOL KTaskCtrl::SyncTask( INT nTaskID )
{
	TASK_SYNC_TASK Task;
	Task.nTaskID = nTaskID;
	Task.nValue = GetTaskValue(nTaskID);

	KPlayerProtocolProcessor* pProtocolHandler = g_sTaskModule.GetProtocolProcessor();
#ifdef GAME_SERVER
	KPlayer* pPlayer = (KPlayer*)&m_rcCharacter;
	return pProtocolHandler->SendData(pPlayer->GetPlayerIndex(), Task);
#else
	return pProtocolHandler->SendData(Task);
#endif
}

BOOL KTaskCtrl::SetStringTaskValue(INT nTaskId, LPCSTR lpszTask)
{
	if (nTaskId >= MAX_STRINGTASK_COUNT || nTaskId < 0)
		return FALSE;
	if (!lpszTask)
	{
		szTask[nTaskId][0] = 0;
		return FALSE;
	}

	if (strlen(lpszTask) > MAX_STRINGTASK_LEN - 1)
		return FALSE;
	strcpy(szTask[nTaskId], lpszTask);
	return TRUE;
};

VOID KTaskCtrl::Release()
{
	mapSave.clear();
	memset(nClear, 0, sizeof(nClear));
	memset(szTask, 0, sizeof(szTask));
}

extern BOOL g_bDirectSetTaskValue;
INT KTaskCtrl::GetTaskValue(INT nTaskId) CONST
{
	//QLogPrintf(LOG_ERR,"..KTaskCtrl::GetTaskValue nTaskId=%d",nTaskId);
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	TASK_PROP sTaskProp = g_PlayerTaskSetting.GetTaskProp(nTaskId);

	if ((DWORD)KSysService::Time(NULL) > sTaskProp.m_dwDateSec && sTaskProp.m_dwDateSec != 0 && !g_bDirectSetTaskValue)
	{
		cSafeScript->OutPutErrMsgF("GetTaskDateOut : GroupId:%d, TaskId:%d", HIWORD(nTaskId), LOWORD(nTaskId));
		//assert(false);
		QLogPrintf(LOG_ERR,"..KTaskCtrl::GetTaskValue GetTaskDateOut : GroupId:%d, TaskId:%d", HIWORD(nTaskId), LOWORD(nTaskId));
	}
	if(sTaskProp.m_nDefFlag == 0 && !g_bDirectSetTaskValue)
	{
		cSafeScript->OutPutErrMsgF("GetTaskNoDefine : GroupId:%d, TaskId:%d", HIWORD(nTaskId), LOWORD(nTaskId));
		QLogPrintf(LOG_ERR,"..KTaskCtrl::GetTaskValue GetTaskNoDefine : GroupId:%d, TaskId:%d", HIWORD(nTaskId), LOWORD(nTaskId));
		//assert(false);
	}
	TTaskMap::const_iterator it	= mapSave.find(nTaskId);
	if (it == mapSave.end())
		return 0;

	return it->second;
}

VOID KTaskCtrl::SetTaskValue(INT nTaskId, INT nTaskValue, BOOL bSync /*=TRUE */)
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	TASK_PROP sTaskProp = g_PlayerTaskSetting.GetTaskProp(nTaskId);	

	if ((DWORD)KSysService::Time(NULL) > sTaskProp.m_dwDateSec && sTaskProp.m_dwDateSec != 0 && !g_bDirectSetTaskValue)
		cSafeScript->OutPutErrMsgF("SetTaskDateOut : GroupId:%d, TaskId:%d", HIWORD(nTaskId), LOWORD(nTaskId));

	if(sTaskProp.m_nDefFlag == 0 && !g_bDirectSetTaskValue)
		cSafeScript->OutPutErrMsgF("SetTaskNoDefine : GroupId:%d, TaskId:%d", HIWORD(nTaskId), LOWORD(nTaskId));

	if (GetTaskValue(nTaskId) == nTaskValue)
		return;
	INT nOldValue	= GetTaskValue(nTaskId);
	if (nTaskValue)
		mapSave[nTaskId] = nTaskValue;
	else
		mapSave.erase(nTaskId);

#ifdef GAME_CLIENT
	//KE_NPC_FLYCHAR_TYPE emType	= emNPC_FLYCHAR_TYPE_NUM;
	//switch(nTaskId)
	//{
	//case TASKVALUE_BINDCOIN:
	//	::g_CoreEventNotify(emCOREEVENT_SYNC_BINDCOINANDMONEY);
	//	emType = emNPC_FLYCHAR_TYPE_BINDGOLD_INC;
	//	break;
	//case TASKVALUE_BINDMONEY:
	//	::g_CoreEventNotify(emCOREEVENT_SYNC_BINDCOINANDMONEY);
	//	emType = emNPC_FLYCHAR_TYPE_BINDMONEY_INC;
	//	break;
	//case TASKVALUE_MACHINECOIN:
	//	emType = emNPC_FLYCHAR_TYPE_MACHINERY_INC;
	//    break;
	//case TASKVALUE_BANKSTORECOIN:
	//	if (nOldValue != 0) // 防止登陆的同步任务变量出现提示
	//		::g_CoreEventNotify(emCOREEVENT_UPDATEBANKINFO, "d", nTaskValue - nOldValue);
	//	break;
	//default:
	//    break;
	//}
	//
	//if (emType != emNPC_FLYCHAR_TYPE_NUM)
	//{
	//	KNpc* pNpc	= m_rPlayer.GetNpcObject();
	//	INT nAddValue = nTaskValue - nOldValue;
	//	if (pNpc && nAddValue > 0 && nOldValue > 0)
	//		pNpc->AddFlyChar(emType, nAddValue);
	//}
#endif

#ifdef GAME_SERVER
	if ((sTaskProp.m_Flag & FLAG_SYNC) && bSync)
#else
	if ((sTaskProp.m_Flag & FLAG_CLIENT) && bSync)
#endif
	{
		SyncTask(nTaskId);
	}
}

//---------------------------------------------------------------------------
//	功能：直接设置任务变量及指定是否同步
//---------------------------------------------------------------------------
VOID KTaskCtrl::DirectSetTaskValue(INT nTaskId, INT nValue, BOOL bSync)
{
	QLogPrintf(LOG_INFO,"..KTaskCtrl::DirectSetTaskValue(INT nTaskId=%d, INT nValue=%d, BOOL bSync=%d)",nTaskId,nValue,(INT)bSync);
	if (nValue)
		mapSave[nTaskId] = nValue;
	else
		mapSave.erase(nTaskId);
#ifdef GAME_SERVER
	if (bSync)
		SyncTask(nTaskId);
#endif
}

//---------------------------------------------------------------------------
//	功能：清除一段任务变量
//---------------------------------------------------------------------------
VOID KTaskCtrl::ClearTaskValue(INT nTaskIdStart, INT nTaskIdEnd)
{
	TTaskMap::iterator it = mapSave.lower_bound(nTaskIdStart);
	while (it != mapSave.end() && it->first <= nTaskIdEnd)
	{
		TTaskMap::iterator curr = it++;
		mapSave.erase(curr);
	}
}

//---------------------------------------------------------------------------
//	功能：得到临时过程控制变量值
//---------------------------------------------------------------------------
INT KTaskCtrl::GetTmpTaskValue(INT nTaskId)
{
	if (nTaskId < 0 || nTaskId >= MAX_TEMP_TASK)
		return 0;
	return nClear[nTaskId];
}

//---------------------------------------------------------------------------
//	功能：设定临时过程控制变量值
//---------------------------------------------------------------------------
VOID KTaskCtrl::SetTmpTaskValue(INT nTaskId, INT nTaskValue)
{
	if (nTaskId < 0 || nTaskId >= MAX_TEMP_TASK)
		return;
	nClear[nTaskId] = nTaskValue;
}

BOOL KTaskCtrl::SetTaskValueEx(WORD wGroupId, WORD wTaskId, INT nTaskValue, INT nSyncMode)
{
	BOOL bSuccess	= FALSE;
	DWORD dwTaskId	= MAKELONG(wTaskId, wGroupId);
	LOG_PROCESS_ERROR(dwTaskId);

	switch(nSyncMode)
	{
	case 0:
		SetTaskValue(dwTaskId, nTaskValue, TRUE);
		break;
#ifdef GAME_SERVER
	case 1:
		SetTaskValue(dwTaskId, nTaskValue, FALSE);
		SyncTask(dwTaskId);
		break;
#endif
	case -1:
		SetTaskValue(dwTaskId, nTaskValue, FALSE);
		break;
	default:
		LOG_PROCESS_ERROR(FALSE);
		return 0;
	}

EXIT0:
	return 0;
}


