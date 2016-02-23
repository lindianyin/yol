/* -------------------------------------------------------------------------
//	文件名		：	kgc_scheduletask.cpp
//	创建者		：	luobaohang
//	创建时间	：	2007-3-12 17:17:50
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include <algorithm>
#include "onlinegamebase/ksysservice.h"
#include "kgc_scheduletask.h"
#include "onlinegamebase/kscriptmanager.h"
#include "gclogicbase/kgclogicextinterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KGC_ScheduleTask g_ScheduleTask;	//产生实例对象
extern IKGcScriptEnv* g_pScriptEnv;
// -------------------------------------------------------------------------

BOOL KGC_ScheduleTask::Breath() 
{
	if ((++m_nLoop & 15) != 0)	//15帧执行一次
		return TRUE;
	time_t	timeCurrent;
	KSysService::Time(&timeCurrent);
	BOOL bSucceed = FALSE;
	//tm tmStruct = *KSysService::LocalTime(&timeCurrent);
	tm tmStruct = *localtime(&timeCurrent);
	INT nCurHourMin = tmStruct.tm_hour * 100 + tmStruct.tm_min;
	if (m_nCurPos < 0)	//初次运行
	{
		KTaskInfo tmp;
		tmp.nTime = nCurHourMin;
		//定位下一个要执行的脚本
		std::vector<KTaskInfo>::iterator it = std::lower_bound(m_vecTask.begin(), m_vecTask.end(), tmp);
		m_nCurPos = (INT)(it - m_vecTask.begin());
		m_nLastHourMin = nCurHourMin;
	}
	if (nCurHourMin < m_nLastHourMin) //有可能是过新一天0点或调前了时间
	{
		if (nCurHourMin < KD_MAX_SPAN_MIN && m_nLastHourMin > 2359 - KD_MAX_SPAN_MIN && m_nCurPos > 0)
		{
			//表明是过了0点，并且与上一次Breath的时间跨度在10分钟内
			for (; m_nCurPos < (INT)m_vecTask.size(); m_nCurPos++)
			{
				if (m_vecTask[m_nCurPos].nTime > 2359)
					break;
				ExecTask(*m_vecTask[m_nCurPos].pTaskDef, m_vecTask[m_nCurPos].nSeqNum);
			}
			m_nCurPos = 0;	//重新从第1个任务开始执行
		}
		else	//往前调了时间，重新定位脚本
		{
			m_nCurPos = -1;
			goto KExit;
		}
	}
	else if (nCurHourMin - m_nLastHourMin > KD_MAX_SPAN_MIN)	//时间跨度大于10分钟
	{
		m_nCurPos = -1;	//往后调了超过10分钟时间，重新定位
		goto KExit;
	}
	//大于或达到当前时间，执行
	while (m_nCurPos < (INT)m_vecTask.size() && nCurHourMin >= m_vecTask[m_nCurPos].nTime)
	{
		//如上一个任务返回TRUE下一个任务跳帧执行
		if (bSucceed)
		{
			m_nLoop--;	//循环数减1使下一帧立即执行
			break;
		}
		bSucceed = ExecTask(*m_vecTask[m_nCurPos].pTaskDef, m_vecTask[m_nCurPos].nSeqNum);
		m_nCurPos++;
	}
KExit:
	m_nLastHourMin = nCurHourMin;
	return TRUE;
}

BOOL KGC_ScheduleTask::Init() 
{
	Q_Printl("Loading Schedule Task xxxxxxxxxxxxxxxxxxxx");
	QIniFile iniFile;
	if (!iniFile.Load(KDF_GC_SCHEDULETASKINI))
	{
		Q_Printl("Can Not Load " << KDF_GC_SCHEDULETASKINI << " , Schedule-Task Will Not Run!");
		return TRUE;
	}
	INT nTask = 0;
	for (; nTask < MAX_TASK_SIZE; nTask++)
	{
		CHAR szSection[128];
		sprintf(szSection, "Task_%02d", nTask + 1);
		if (!iniFile.IsSectionExist(szSection))
			break;
		if (!iniFile.GetString(szSection, "TaskName", "", 
			m_Task[nTask].szTaskName, KD_MAX_TASKNAME_SIZE - 1) ||
		!iniFile.GetString(szSection, "ScriptGroup", "", 
			m_Task[nTask].szGroupName, KD_MAX_TASKNAME_SIZE - 1) || 
		!iniFile.GetString(szSection, "ScriptFun", "", 
			m_Task[nTask].szScriptFun, KD_MAX_SCRIPTFUN_NAME_SIZE - 1))
		{
			Q_Error("");
			Q_Printl("Error Reading " << KDF_GC_SCHEDULETASKINI << " In Section " << szSection << " !");
			continue;
		}
		INT nCurSeqNum = 0;
		for (INT nSeqNum = 1; nSeqNum <= MAX_TASK_SIZE; nSeqNum++)
		{
			CHAR szTime[128];
			sprintf(szTime, "TaskTime_%02d", nSeqNum);
			INT nHourMin;
			iniFile.GetInteger(szSection, szTime, -2, &nHourMin);
			if (nHourMin < -1)
				break;
			if(nHourMin >= 0)
			{
				++nCurSeqNum;
				KGC_ScheduleTask::KTaskInfo task;
				task.nTime = nHourMin;
				task.nSeqNum = nCurSeqNum;
				task.pTaskDef = &m_Task[nTask];
				m_vecTask.push_back(task);
			}
		}
	}
	std::stable_sort(m_vecTask.begin(), m_vecTask.end());
	Q_Printl("Schedule Task Loaded, Task Count = " << nTask);
	m_nCurPos = -1;
	return TRUE;
}

BOOL KGC_ScheduleTask::ExecTask( CONST KTaskDef &Task, INT nSeqNum ) 
{
	Q_Printl("Execute Task: " << Task.szTaskName);
	ScriptSafe cSafeScript = g_pScriptEnv->GetScriptManager().GetSafeScript();

	if (!cSafeScript->CallTableFunction(Task.szGroupName, Task.szScriptFun, 1, "d", nSeqNum))
	{
		Q_Error("Error Called ScheduleTask Function " << Task.szGroupName);
		return FALSE;
	}
	//获取最后一个返回值，如果是1则return TRUE，否则FALSE
	if (cSafeScript->IsNumber(-1) && cSafeScript->GetInt(-1) == 1)
		return TRUE;
	return FALSE;
}

INT KGC_ScheduleTask::AddDynTask( LPCSTR szTaskName, LPCSTR szGroupName, LPCSTR szScriptFun ) 
{
	QCONFIRM_RET_NULL(m_nDynTaskNum < KD_MAX_DYN_TASK_NUM);
	strncpy(m_DynTask[m_nDynTaskNum].szTaskName, szTaskName, sizeof(m_DynTask[m_nDynTaskNum].szTaskName) - 1);
	strncpy(m_DynTask[m_nDynTaskNum].szGroupName, szGroupName, sizeof(m_DynTask[m_nDynTaskNum].szGroupName) - 1);
	strncpy(m_DynTask[m_nDynTaskNum].szScriptFun, szScriptFun, sizeof(m_DynTask[m_nDynTaskNum].szScriptFun) - 1);
	return ++m_nDynTaskNum;
}

BOOL KGC_ScheduleTask::ResterTimeTask( INT nDynTask, INT nHourMin, INT nSeqNum ) 
{
	QCONFIRM_RET_FALSE(m_nDynTaskNum > 0 && m_nDynTaskNum >= nDynTask);
	KTaskInfo task;
	task.nTime = nHourMin;
	task.nSeqNum = nSeqNum;
	task.pTaskDef = &m_DynTask[m_nDynTaskNum - 1];

	KTaskInfo tmp;
	tmp.nTime = nHourMin;
	std::vector<KTaskInfo>::iterator it = std::lower_bound(m_vecTask.begin(), m_vecTask.end(), tmp);
	// 插入任务
	if (it != m_vecTask.end())
		m_vecTask.insert(it, task);
	else
		m_vecTask.push_back(task);

	// 重新定位执行位置
	if (m_nCurPos >= 0 && m_nLastHourMin >= nHourMin)
		++m_nCurPos;
	return TRUE;
}
