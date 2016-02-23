
#pragma once

#include <map>

// -------------------------------------------------------------------------
#define  KD_GCPLAYER_NAME_LEN 32

template<INT N_TASK_BYTE_MAX, INT N_TASK_WORD_MAX, INT N_TASK_DWORD_MAX>
class KGcPlayerBase
{
#define KD_GCPLAYER_TASKFUN_BYTE(_Name, _TaskId) \
	INT Get##_Name() { return KGcPlayerBase<N_TASK_BYTE_MAX, N_TASK_WORD_MAX, N_TASK_DWORD_MAX>::GetByteVar(_TaskId); } \
	VOID Set##_Name(INT n##_Name) { KGcPlayerBase<N_TASK_BYTE_MAX, N_TASK_WORD_MAX, N_TASK_DWORD_MAX>::SetByteVar(_TaskId, (BYTE)n##_Name); }
#define KD_GCPLAYER_TASKFUN_WORD(_Name, _TaskId) \
	INT Get##_Name() { return KGcPlayerBase<N_TASK_BYTE_MAX, N_TASK_WORD_MAX, N_TASK_DWORD_MAX>::GetWordVar(_TaskId); } \
	VOID Set##_Name(INT n##_Name) { KGcPlayerBase<N_TASK_BYTE_MAX, N_TASK_WORD_MAX, N_TASK_DWORD_MAX>::SetWordVar(_TaskId, (DWORD)n##_Name); }
#define KD_GCPLAYER_TASKFUN_DWORD(_Name, _TaskId) \
	INT Get##_Name() { return KGcPlayerBase<N_TASK_BYTE_MAX, N_TASK_WORD_MAX, N_TASK_DWORD_MAX>::GetDWordVar(_TaskId); } \
	VOID Set##_Name(INT n##_Name) { KGcPlayerBase<N_TASK_BYTE_MAX, N_TASK_WORD_MAX, N_TASK_DWORD_MAX>::SetDWordVar(_TaskId, (DWORD)n##_Name); }
public:
	KGcPlayerBase()
	{
		m_dwId = 0;
		ZeroStruct(m_aByTask);
		ZeroStruct(m_aWTask);
		ZeroStruct(m_aDwTask);
		m_szName[0] = '\0';
		m_bDirty = FALSE;
	}
	~KGcPlayerBase() {};
	inline DWORD GetId()
	{
		return m_dwId;
	}
	inline INT GetByteVar(UINT nVarId)
	{
		QCONFIRM_RET(nVarId < N_TASK_BYTE_MAX, 0)
		return m_aByTask[nVarId];
	}
	inline INT SetByteVar(UINT nVarId, INT nValue)
	{
		QCONFIRM_RET(nVarId < N_TASK_BYTE_MAX, 0)
		m_aByTask[nVarId] = nValue;
		m_bDirty = TRUE;
		return TRUE;
	}
	inline INT GetWordVar(UINT nVarId)
	{
		QCONFIRM_RET(nVarId < N_TASK_WORD_MAX, 0)
			return m_aWTask[nVarId];
	}
	inline INT SetWordVar(UINT nVarId, INT nValue)
	{
		QCONFIRM_RET(nVarId < N_TASK_WORD_MAX, 0)
		m_aWTask[nVarId] = nValue;
		m_bDirty = TRUE;
		return TRUE;
	}
	inline INT GetDWordVar(UINT nVarId)
	{
		QCONFIRM_RET(nVarId < N_TASK_DWORD_MAX, 0)
			return m_aDwTask[nVarId];
	}
	inline INT SetDWordVar(UINT nVarId, INT nValue)
	{
		QCONFIRM_RET(nVarId < N_TASK_DWORD_MAX, 0)
			m_aDwTask[nVarId] = nValue;
		m_bDirty = TRUE;
		return TRUE;
	}
	//INT GetTaskValue(INT nTaskId)
	//{
	//	if ((UINT)nTaskId  >= N_TASK_DWORD_MAX)
	//	{
	//		Q_Error("");
	//		return 0;
	//	}
	//	return m_aDwTask[(UINT)nTaskId];
	//}
	//VOID SetTaskValue(INT nTaskId, INT nValue)
	//{
	//	if ((UINT)nTaskId  >= N_TASK_DWORD_MAX)
	//	{
	//		Q_Error("");
	//		return;
	//	}
	//	m_aDwTask[(UINT)nTaskId] = nValue;
	//	m_bDirty = TRUE;
	//}
	// 获取模块专有变量
	INT GetModuleTask(BYTE byModuleId, BYTE byTaskId)
	{
		WORD wKey = (byModuleId << 8) | byTaskId;
		MP_MODULETASK::iterator it = m_mpModuleTask.find(wKey);
		if (it == m_mpModuleTask.end())
			return 0;
		return it->second;
	}
	// 设置模块专有变量
	VOID SetModuleTask(BYTE byModuleId, BYTE byTaskId, INT nValue)
	{
		WORD wKey = (byModuleId << 8) | byTaskId;
		m_mpModuleTask[wKey] = nValue;
		m_bDirty = TRUE;
	}
	inline LPCSTR GetPlayerName()
	{
		return m_szName;
	}
	inline VOID SetPlayerName(LPCSTR szName)
	{
		QStrCpyLen(m_szName, szName, sizeof(m_szName));
		m_bDirty = TRUE;
	}
	inline VOID SetId(DWORD dwId)
	{
		m_dwId = dwId;
	}
	virtual VOID Reset()
	{
		m_dwId = 0;
		ZeroStruct(m_aByTask);
		ZeroStruct(m_aWTask);
		ZeroStruct(m_aDwTask);
		m_szName[0] = '\0';
		m_mpModuleTask.clear();
		m_bDirty = FALSE;
	}
	BOOL m_bDirty; // 上次存盘后是否变化过
protected:
	typedef std::map<WORD, INT> MP_MODULETASK;
	DWORD m_dwId;
	BYTE m_aByTask[N_TASK_BYTE_MAX];
	WORD m_aWTask[N_TASK_WORD_MAX];
	DWORD m_aDwTask[N_TASK_DWORD_MAX];
	CHAR m_szName[KD_GCPLAYER_NAME_LEN];
	MP_MODULETASK m_mpModuleTask;
};

