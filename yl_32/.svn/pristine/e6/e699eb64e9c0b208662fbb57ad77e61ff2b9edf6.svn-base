
#ifdef GAME_SERVER
#include "stdafx.h"
#include "onlinegamemodule/ktaskctrl.h"
#include "ktaskmodule.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "ktasksetting.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct DB_TASK_DATA
{
	DWORD	dwTaskID;
	INT		nValue;
};

// -------------------------------------------------------------------------

BOOL KTaskCtrl::Active()
{
	m_listTaskTimer.Activate((KPlayer*)&m_rcCharacter, g_cOrpgWorld.m_dwLogicFrames, m_dwEvent);

	m_dwEvent = 0; // todo [7/20/2012 simon]
	return TRUE;
}

BOOL KTaskCtrl::OnLogin()
{
	return TRUE;
}

BOOL KTaskCtrl::OnLogout()
{
	return TRUE;
}

BOOL KTaskCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	if (!pData || !uSize)
		return TRUE;

	Release();

	INT nTaskId = 0;
	INT nTaskValue = 0;
	INT nTaskCount = uSize / sizeof(DB_TASK_DATA);
	ASSERT((uSize % sizeof(DB_TASK_DATA)) == 0);

	if (!nTaskCount)
		return TRUE;

	extern BOOL g_bDirectSetTaskValue;

	g_bDirectSetTaskValue = TRUE;

	CONST DB_TASK_DATA* pTaskData = (DB_TASK_DATA*)pData;
	for (INT i = 0; i < nTaskCount; ++i , ++pTaskData)
	{
		nTaskId = pTaskData->dwTaskID;
		nTaskValue = pTaskData->nValue;
		TASK_PROP sTaskProp = g_PlayerTaskSetting.GetTaskProp(nTaskId);
		if(sTaskProp.m_dwDateSec == 0 || (DWORD)KSysService::Time(NULL) < sTaskProp.m_dwDateSec)
		{
			SetTaskValue(nTaskId, nTaskValue, FALSE);
			if (sTaskProp.m_Flag & FLAG_SYNC)
			{
				SyncTask(nTaskId);
			}
		}
	}
	g_bDirectSetTaskValue = FALSE;

	//Dump("Task");

	OnDbLoaded();

	return TRUE;
}

BOOL KTaskCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize) 
{
	m_listTaskTimer.Save((KPlayer*)&m_rcCharacter, g_cOrpgWorld.m_dwLogicFrames);

	//Dump("Saving Task");

	INT nTaskCount = 0;
	DB_TASK_DATA *pTaskData = (DB_TASK_DATA*)pBuffer;
	//所有任务
	TTaskMap::const_iterator it = mapSave.begin();
	for (; it != mapSave.end(); ++it)
	{
		if ((LPBYTE)(pTaskData + 1) - pBuffer > (INT)uBuffSize)
			QCONFIRM_RET_FALSE(!"task save failed! buffer too small");

		pTaskData->dwTaskID	= it->first;
		pTaskData->nValue	= it->second;
		nTaskCount++;
		pTaskData++;
	}

	uUsedSize = sizeof(DB_TASK_DATA) * nTaskCount;

	return TRUE;
}

BOOL KTaskCtrl::ProcessProtocol(LPCBYTE pbData, UINT uDataLen)
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pbData;
	switch(pHeader->byProtocol)
	{
	case emPROT_SYNC_TASK_EVENT:
		{
			return OnTaskEvent((TASK_SYNC_EVENT*)pbData);
		}
		break;
	case emPROT_SYNC_TASK:
		{
			TASK_SYNC_TASK* pProtocol = (TASK_SYNC_TASK*)pbData;

			TASK_PROP sTaskProp = g_PlayerTaskSetting.GetTaskProp(pProtocol->nTaskID);

			// 只允许客户端任务变量可以被客户端要求设置
			if (sTaskProp.m_Flag & FLAG_CLIENT)
				SetTaskValue(pProtocol->nTaskID, pProtocol->nValue, FALSE);
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;
}

BOOL KTaskCtrl::OnTaskEvent(TASK_SYNC_EVENT* pTaskEvent)
{
	BOOL bRet = FALSE;
	switch (pTaskEvent->byType)
	{
	case emKTASK_ACCEPT:
		{
			bRet = CallScript("OnAccept", "ddd", pTaskEvent->nTaskID, pTaskEvent->nSubTaskIdx, pTaskEvent->nParam);
		}
		break;
	case emKTASK_AWARD:
		{
			bRet = CallScript("OnAward", "ddd", pTaskEvent->nTaskID, pTaskEvent->nSubTaskIdx, pTaskEvent->nParam);
		}
		break;
	case emKTASK_GIVEUP:
		{
			bRet = CallScript("OnGiveUp", "dd", pTaskEvent->nTaskID, pTaskEvent->nSubTaskIdx);
		}
		break;
	case emKTASK_SHARE:
		{
			bRet = CallScript("OnShare", "dd", pTaskEvent->nTaskID, pTaskEvent->nSubTaskIdx);
		}
		break;
	case emKTASK_GENERAL_AWARD:
		{
			KPlayer* pPlayer = (KPlayer*)&m_rcCharacter;
			QLunaBase* pOldMe = g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
			ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

			bRet = cSafeScript->CallTableFunction("GeneralAward", "OnAward", 0, "d", pTaskEvent->nParam);
			g_cScriptManager.SetMe(pOldMe);
		}
		break;
	case emKTASK_USE_SHORTCUT:
		{
			g_cScriptManager.SetMe((QLunaBase*)((KPlayer&)m_rcCharacter).GetScriptInterface());
			ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

			bRet = cSafeScript->CallTableFunction("PlayerEvent", "OnUseShortcut", 0, "d", pTaskEvent->nParam);
		}
		break;
	case emKTASK_SETENV_CALLBACK:
		{
			g_cScriptManager.SetMe((QLunaBase*)((KPlayer&)m_rcCharacter).GetScriptInterface());
			ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

			bRet = cSafeScript->CallTableFunction("BlackSky", "WhenEvnChange", 0, "d", pTaskEvent->nParam);
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return bRet;
}

BOOL KTaskCtrl::SyncTaskGroup(WORD wGroupID)
{
	CONST TTaskMap* mapTask = &mapSave;

	S2C_SYNC_TASKGROUP dataSend;
	dataSend.wTaskGroupID	= wGroupID;
	dataSend.byFirst		= 1;

	DWORD dwMin	= ((DWORD)wGroupID) << 16;
	DWORD dwMax	= dwMin | 0xffff;

	TTaskMap::const_iterator it = mapTask->lower_bound(dwMin);
	INT nCount;

	do
	{
		nCount	= 0;
		while (nCount < S2C_SYNC_TASKGROUP::KE_MAX_COUNT &&
			it != mapTask->end() && (DWORD)it->first <= dwMax)
		{
			dataSend.atpPairs[nCount].wTaskID	= (WORD)it->first; //注意，这里通过WORD转化，去掉了高位的GroupID
			dataSend.atpPairs[nCount].nValue	= it->second;
			it++;
			nCount++;
		}
		dataSend.wTaskCount	= (WORD)nCount;

		KPlayerProtocolProcessor* Processor = g_sTaskModule.GetProtocolProcessor();

		Processor->SendLenData(((KPlayer&)m_rcCharacter).GetPlayerIndex(), dataSend, dataSend.GetSize());
		dataSend.byFirst	= 0;
	} while (it != mapTask->end() && (DWORD)it->first <= dwMax);

	return TRUE;
}

BOOL KTaskCtrl::SyncAll()
{
	std::vector<UINT> vecGet;
	g_PlayerTaskSetting.GetAllSyncTask(vecGet);

	std::vector<UINT>::const_iterator it;
	for (it = vecGet.begin(); it != vecGet.end(); ++it)
		SyncTask((INT)*it);

	g_PlayerTaskSetting.GetAllSyncGroup(vecGet);

	for (it = vecGet.begin(); it != vecGet.end(); ++it)
		SyncTaskGroup((WORD)*it);
	return TRUE;
}

void KTaskCtrl::OnDbLoaded()
{
	m_listTaskTimer.Load((KPlayer*)&m_rcCharacter, g_cOrpgWorld.m_dwLogicFrames);

	//SyncAll();
}

#endif	// #ifdef GAME_SERVER

// -------------------------------------------------------------------------
