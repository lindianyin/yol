
#ifndef __KTASKPLAYERCTRL_H__
#define __KTASKPLAYERCTRL_H__

#include "onlinegameworld/kcharacter.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kcharacterctrlbase.h"
#include "ktaskprotocol.h"
#include "onlinegamemodule/ktasktimer.h"

#define		MAX_TASK		2560
#define		MAX_TEMP_TASK	256
#define		MAX_STRINGTASK_LEN 33
#define		MAX_STRINGTASK_COUNT 10


class KNpc;
class KPlayer;
class KLuaTask;

// -------------------------------------------------------------------------
class KTaskCtrl : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_TASK	};

	KTaskCtrl(KCharacter& rcCharacter) : KCharacterCtrlBase(rcCharacter),m_pLuna(NULL) 
	{
	}
	~KTaskCtrl(VOID)
	{
	}
	KLuaTask* GetScriptInterface() {	return m_pLuna;	}

	typedef std::map<INT, INT> TTaskMap;				// TODO: 暂时用个Map，效率没有验证过	by FanZai
	TTaskMap	mapSave;								// 用于记录任务是否完成，须保存到数据库
	INT			nClear[MAX_TEMP_TASK];					// 用于记录任务过程中的中间步骤的完成情况，不保存到数据库，玩家下线后次数据清空
	CHAR		szTask[MAX_STRINGTASK_COUNT][MAX_STRINGTASK_LEN];
	KLuaTask* m_pLuna;

public:
	virtual BOOL Init();
	virtual BOOL UnInit();
	virtual BOOL OnLogin();
	virtual BOOL OnLogout();

#ifdef GAME_SERVER
	virtual BOOL Active();
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);
	//virtual BOOL OnClientReady();
#endif

	INT		GetTaskValue(INT nTaskId) CONST;														// 得到任务完成情况
	VOID	SetTaskValue(INT nTaskId, INT nValue, BOOL bSync = TRUE /*bSync：是否允许同步 */);		// 设定任务完成情况
	VOID	ClearTaskValue(INT nTaskIdStart, INT nTaskIdEnd);										// 清除一段任务变量
	VOID	DirectSetTaskValue(INT nTaskId, INT nValue, BOOL bSync);								// 直接设置不调脚本

	INT		GetTmpTaskValue(INT nTaskId);															// 得到临时过程控制变量值
	VOID	SetTmpTaskValue(INT nTaskId, INT nTaskValue);											// 设定临时过程控制变量值

	LPCSTR GetStringTaskValue(INT nTaskId)
	{
		if (nTaskId >= MAX_STRINGTASK_COUNT || nTaskId < 0)
			return "";
		return szTask[nTaskId];
	}
	BOOL	SetStringTaskValue(INT nTaskId, LPCSTR lpszTask);
	INT GetTaskValueEx(WORD wGroupId, WORD wTaskId) CONST
	{
		return GetTaskValue(MAKELONG(wTaskId, wGroupId));
	}
	// nSyncMode: 同步方式	0、自动；1、同步；2、不同步
	BOOL SetTaskValueEx(WORD wGroupId, WORD wTaskId, INT nTaskValue, INT nSyncMode = 0);

public:
	BOOL SendAccept(INT nTaskID, INT nSubTaskIdx, BOOL bAccept);
	BOOL SendAward(INT nTaskID, INT nSubTaskIdx, INT nSelIdx)
	{
		return SendTaskCommon(emKTASK_AWARD, nTaskID, nSubTaskIdx, nSelIdx);
	}
	BOOL SendTaskCommon(BYTE byType, INT nTaskID, INT nSubTaskIdx, INT nParam);
	VOID Dump(LPCTSTR szMsg);
	BOOL SyncTask(INT nTaskID);

protected:
	void OnDbLoaded();
	VOID Release();
public:
	BOOL CallScript(LPCSTR pszFunName, LPCSTR pszFormat, ...);

#ifdef GAME_SERVER
public:
	BOOL SendRefresh(INT nTaskID, INT nSubTaskIdx, INT nTaskGroupID)
	{
		return SendTaskCommon(emKTASK_REFRESH, nTaskID, nSubTaskIdx, nTaskGroupID);
	}

	BOOL StepStart(INT nTaskID, INT nSubTaskIdx, INT nStepIdx)
	{
		return SendTaskCommon(emKTASK_STEP_START, nTaskID, nSubTaskIdx, nStepIdx);
	}

	BOOL ProcessProtocol(LPCBYTE pbData, UINT uDataLen);
	BOOL SyncTaskGroup(WORD wGroupID);
	BOOL SyncAll();

protected:
	BOOL OnTaskEvent(TASK_SYNC_EVENT* pTaskCommon); 

	KTaskTimerList	m_listTaskTimer;								// 定时器列表，最多有10个定时器
	DWORD	m_dwEvent;
#endif
};

#define m_PlayerTask GetCtrlRef<KTaskCtrl>()

// -------------------------------------------------------------------------

#endif /* __KTASKPLAYERCTRL_H__ */
