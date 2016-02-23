
#pragma once

#include <vector>

// -------------------------------------------------------------------------
class KVarSetting
{
	struct TASK_PROP
	{
		DWORD m_Flag;
		DWORD m_dwReSetDate;	// 重置日期
	};
	struct VAR_KEY
	{
		BYTE byVarGroup;
		BYTE byVarId;
	};
	enum KE_VAR_FLAG
	{
		//按2进制最多32个：1,2,4,8,16.....
		emKVARFLAG_DEF = 1,	// 是否有定义，没定义的任务变量使用时警告
		emKVARFLAG_SYNC_SELF = 2,	// 是否同步给自身
		emKVARFLAG_SYNC_OTHER = 4, // 是否同步给他人（附近玩家）
		emKVARFLAG_CLIENT = 8,
	};
#define KD_W_KEY(wgroup, wid) (WORD)((wgroup << 8) | wid)
public:
	typedef std::vector<VAR_KEY> VEC_VARKEY;
	KVarSetting();
	~KVarSetting();
	BOOL LoadTaskDef(LPCSTR FileName);
	// 检查变量ID是否已在 KDF_CHARACTER_VAR_DEF 配置文件定义
	BOOL CheckDefine(WORD wVarGroup, WORD wVarId)
	{
		return m_pTaskProp[KD_W_KEY(wVarGroup, wVarId)].m_Flag != 0;
	}
	// 是否同步给自己
	BOOL CheckSyncSelf(WORD wVarGroup, WORD wVarId)
	{
		return (m_pTaskProp[KD_W_KEY(wVarGroup, wVarId)].m_Flag & emKVARFLAG_SYNC_SELF) != 0;
	}
	// 是否同步给周围玩家
	BOOL CheckSyncOther(WORD wVarGroup, WORD wVarId)
	{
		return (m_pTaskProp[KD_W_KEY(wVarGroup, wVarId)].m_Flag & emKVARFLAG_SYNC_OTHER) != 0;
	}
	// 是否可由客户端指定
	BOOL CheckClientSet(WORD wVarGroup, WORD wVarId)
	{
		return (m_pTaskProp[KD_W_KEY(wVarGroup, wVarId)].m_Flag & emKVARFLAG_CLIENT) != 0;
	}
	// 获取重置日期
	DWORD GetResetDate(WORD wVarGroup, WORD wVarId)
	{
		return m_pTaskProp[KD_W_KEY(wVarGroup, wVarId)].m_dwReSetDate;
	}
	const VEC_VARKEY& GetSyncSelfTask()
	{
		return m_vecSyncSelfTask;
	}
	const VEC_VARKEY& GetSyncOtherTask()
	{
		return m_vecSyncOtherTask;
	}

	BOOL SetTaskSyncSelf(BYTE nGroupId, BYTE nTaskId)
	{
		VAR_KEY vKey = {nGroupId, nTaskId};
		m_vecSyncSelfTask.push_back(vKey);
		return TRUE;
	}

	BOOL SetTaskSyncOther(BYTE nGroupId, BYTE nTaskId)
	{
		VAR_KEY vKey = {nGroupId, nTaskId};
		m_vecSyncOtherTask.push_back(vKey);
		return TRUE;
	}

//private:
	BOOL LoadBaseVardef();
protected:
	VEC_VARKEY m_vecSyncSelfTask;
	VEC_VARKEY m_vecSyncOtherTask;
	TASK_PROP* m_pTaskProp;
};
extern KVarSetting g_cCharacterVarSetting;
// -------------------------------------------------------------------------

