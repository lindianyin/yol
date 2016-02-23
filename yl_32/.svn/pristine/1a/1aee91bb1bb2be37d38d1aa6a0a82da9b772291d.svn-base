/* -------------------------------------------------------------------------
//	文件名		：	kgc_scheduletask.h
//	创建者		：	luobaohang
//	创建时间	：	2007-3-12 14:32:58
//	功能描述	：	时间任务系统
//
// -----------------------------------------------------------------------*/
#ifndef __KGC_SCHEDULETASK_H__
#define __KGC_SCHEDULETASK_H__

#include <vector>

#define KDF_GC_SCHEDULETASKINI	"\\setting\\scheduletask\\tasklist.ini"
#define MAX_TASK_SIZE 100
#define MAX_TIME_NUM 100
#define KD_MAX_DYN_TASK_NUM 32	// 动态注册任务的最大数量
#define KD_MAX_TASKNAME_SIZE	64
#define KD_MAX_GROUPNAME_SIZE	128
#define KD_MAX_SCRIPTFUN_NAME_SIZE	128
#define KD_MAX_SPAN_MIN	10	// 最大跨度分钟数

// -------------------------------------------------------------------------
class KGC_ScheduleTask
{
	struct KTaskDef
	{
		CHAR szTaskName[KD_MAX_TASKNAME_SIZE];
		CHAR szGroupName[KD_MAX_GROUPNAME_SIZE];	// 脚本的组名
		CHAR szScriptFun[KD_MAX_SCRIPTFUN_NAME_SIZE];	// 脚本函数名
	};
	struct KTaskInfo 
	{
		INT nTime;	// 执行时间
		INT nSeqNum; // 执行序号
		KTaskDef *pTaskDef;	// 任务
		BOOL operator < (CONST KTaskInfo &r)CONST { return nTime < r.nTime; }
	};
public:
	KGC_ScheduleTask()
	{ m_nLoop = m_nCurPos = m_nLastHourMin = -1; m_nDynTaskNum = 0; }
	// 动态添加一个任务，返回动态任务ID（从1开始），失败返回0
	INT AddDynTask(LPCSTR szTaskName, LPCSTR szGroupName, LPCSTR szScriptFun);
	// 动态注册一个时间点任务，nDynTask必须为已动态添加的任务Id
	BOOL ResterTimeTask(INT nDynTask, INT nHourMin, INT nSeqNum);

	BOOL	ExecTask(CONST KTaskDef &Task, INT nSeqNum);
	virtual BOOL Breath();
	virtual BOOL Init();
protected:
	std::vector<KTaskInfo> m_vecTask;
	KTaskDef m_Task[MAX_TASK_SIZE];
	KTaskDef m_DynTask[KD_MAX_DYN_TASK_NUM];
	INT m_nDynTaskNum;	// 当前动态注册任务数量
	INT m_nCurPos;		// 下一个将要执行的任务下标
	INT m_nLastHourMin;	// 上一次Breath的时分
	INT m_nLoop;
private:
};
extern KGC_ScheduleTask g_ScheduleTask;
// -------------------------------------------------------------------------

#endif /* __KGC_SCHEDULETASK_H__ */
