
#ifndef __KTASKSETTING_H__
#define __KTASKSETTING_H__

#include <set>

template <class TaskID_T>
class TTaskID
{
public:
	TTaskID(TaskID_T id)
	{
		m_idFirst = m_idLast = id;
	}
	TTaskID(TaskID_T idFirst, TaskID_T idLast)
	{
		ASSERT(idFirst <= idLast);
		m_idFirst = idFirst;
		m_idLast = idLast;
	}
	bool operator == (CONST TaskID_T &id)CONST
	{
		return id >= m_idFirst && id <= m_idLast;		
	}
	bool operator < (CONST TTaskID &id)CONST
	{
		return m_idFirst < id.m_idFirst;
	}
public:
	TaskID_T m_idFirst;
	TaskID_T m_idLast;
};

enum TASK_FLAG
{
	//按2进制最多32个：1,2,4,8,16.....
	FLAG_SYNC = 1,
	FLAG_CLIENT = 2,
};

struct TASK_PROP
{
	TASK_PROP() : m_Flag(0) , m_dwDateSec(0), m_nDefFlag(0) {}
	BOOL IsEmpty() { return m_Flag == 0 && m_dwDateSec == 0; }
	DWORD m_Flag;
	DWORD m_dwDateSec;
	INT m_nDefFlag;
};
// -------------------------------------------------------------------------
class KTaskSetting
{
public:
typedef std::set<TTaskID<UINT> >	TIDSET;
typedef std::set<UINT>	TGIDSET;
typedef std::map<UINT, TASK_PROP> TIDPROP;

	BOOL LoadTaskDef(LPCSTR FileName);
	BOOL CheckSyncFlag(UINT nTaskID)
	{
		return m_setTaskSync.find(nTaskID) != m_setTaskSync.end();
	}
	size_t GetAllSyncTask(std::vector<UINT> &vecGet)
	{
		TIDSET::iterator it;
		vecGet.clear();
		for (it = m_setTaskSync.begin(); it != m_setTaskSync.end(); ++it)
		{
			UINT i;
			for (i = it->m_idFirst; i <= it->m_idLast; i++)
			{
				vecGet.push_back(i);
			}
		}
		return vecGet.size();
	}
	size_t GetAllSyncGroup(std::vector<UINT> &vecGet) CONST
	{
		TGIDSET::const_iterator it;
		vecGet.clear();
		for (it = m_setTaskGroupSync.begin(); it != m_setTaskGroupSync.end(); ++it)
			vecGet.push_back(*it);
		return vecGet.size();
	}
	TASK_PROP GetTaskProp(UINT nTaskID)
	{
		BOOL bResult = FALSE;
		TASK_PROP sTskPrp;
		TIDPROP::const_iterator it =  m_mapIDProp.find(nTaskID);
		if (it != m_mapIDProp.end())
		{
			bResult = TRUE;
			sTskPrp	= it->second;
		}
		else
		{
			it =  m_mapGroupIDProp.find(HIWORD(nTaskID));
			if (it != m_mapGroupIDProp.end())
			{
				bResult = TRUE;
				sTskPrp	= it->second;
			}
		}
		//assert(bResult);
		if(!bResult)
		{
			QLogPrintf(LOG_ERR,"..GetTaskProp(UINT nTaskID=%d)",nTaskID);
		}
		return sTskPrp;
	}
protected:
	//需要同步的变量均放到这个set里
	TIDSET	m_setTaskSync;
	TGIDSET	m_setTaskGroupSync;
	TIDPROP m_mapIDProp;
	TIDPROP m_mapGroupIDProp;
};

extern KTaskSetting			g_PlayerTaskSetting;

// -------------------------------------------------------------------------

#endif /* __KTASKSETTING_H__ */
