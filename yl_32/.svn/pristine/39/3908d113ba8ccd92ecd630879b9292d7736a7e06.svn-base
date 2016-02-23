#ifndef KTASKFUNCS_H
#define KTASKFUNCS_H

#include <string>
#include <list>
#include <map>
#include <set>


#define TASKVALUE_ARREST_TIME 2046	// 被抓进桃源天牢的时间
#define TASKVALUE_JAIL_TERM   2047	// 在桃源天牢呆多久


class KPlayer;

enum KE_TIMERID
{
	KE_TIMERID_PROGRESS =  100000,
	KE_TIMERID_TASKTAG	=  100002,
	KE_TIMERID_GENERAL	=  100003,
};

#ifdef GAME_SERVER
// 任务定时器结构
struct TTaskTimer
{
	UINT	nTimerId;	// 定时器ID
	struct
	{
		INT nTime : 24;			// 使用次数
		INT nMode : 4;			// 模式：0、玩家下线后失效；1、玩家下线后仍然存在，计时以真实时间为准；2、玩家下线后仍然存在，计时以玩家在线时间为准
		INT nSuspending : 4;	// 是否暂停运行 
	}		cOption;
	INT		nInterval;	// 定时器间隔时间，1/18秒为单位的整数
	INT		nLastTime;	// 上次计时时间
	INT		nLastQuit;	// 上次退出时间
	DWORD	dwEvent;
	LPVOID	pData;
	DWORD	dwParam;
	INT		nParam;
};

// 任务定时器
class KTaskTimer
{
public:
	enum Mode
	{
		MODE_LOSTTIME,		// 玩家下线后失效
		MODE_REALTIME,		// 玩家下线后仍然存在，计时以真实时间为准
		MODE_GAMETIME,		// 玩家下线后仍然存在，计时以玩家在线时间为准
		//MODE_NOMOVE,		// 玩家在过程中不能够移动
		//MODE_NOATTACKED,	// 玩家在过程中不能被打到
		//MODE_NOMOVE_NOATTACKED, //不能移动也不能被打到
		MODE_COUNT
	};

	enum KE_CLOSE
	{
		KE_CLOSE_TIMEREND,
		KE_CLOSE_EVENT,
		KE_CLOSE_FORCE,

	};

	enum
	{
		VALUE_COUNT = 5 
	};

public:

	KTaskTimer();

	BOOL IsFree() {return m_cTimer.nTimerId == 0;}
	VOID Start(CONST TTaskTimer* pTimer);
	VOID Close(KPlayer* pOwner, KE_CLOSE emuCloseType);
	// 返回TRUE表示定时器继续存在，返回FALSE表示定时器需要关闭
	BOOL Activate(KPlayer* pOwner, INT nCurrent);
	BOOL Load(CONST INT nTaskValues[VALUE_COUNT], INT nCurrent);
	VOID Save(INT nTaskValues[VALUE_COUNT], INT nCurrent);
	INT GetRestTime(INT nCurrent);
	INT GetRestCount();
	INT GetId() {return m_cTimer.nTimerId;}
	BOOL Suspend();
	BOOL Resume();
	BOOL IsSuspending();

private:

	BOOL Load(TTaskTimer* pTimer, INT nCurrent);
	VOID Save(TTaskTimer* pTimer, INT nCurrent);
	static VOID AdjustTime(INT nLostTime, INT nCurrent, TTaskTimer *pTimer);
	VOID OnTime(KPlayer* pOwner);
	TTaskTimer m_cTimer;
};

// 任务定时器队列
class KTaskTimerList
{

public:

	enum
	{
		TIMER_COUNT = 10,
		TAKSVAL_IDX = 1611,						// 使用的任务变量1611-1660
		VALUE_COUNT = KTaskTimer::VALUE_COUNT,	// 每个定时器使用的任务变量的数目
	};
	/*
	2.生活技能：移动，使用技能，打坐，上下马，使用道具，更换装备，发送邮件，交易，切换战斗状态，退出游戏
	3.下线延迟：移动，使用技能，打坐，上下马，使用道具，移动物品栏中的道具，丢弃物品，更换装备，发送邮件，交易，切换战斗状态
	*/
	// 以下事件，会在读进度条的时候打断它
	enum
	{
		EVENT_MOVE,					// 移动
		EVENT_ATTACK,				// 主动攻击(使用部分技能)
		EVENT_SITE,					// 打坐
		EVENT_RIDE,					// 上下马
		EVENT_USEITEM,				// 使用道具
		EVENT_ARRANGEITEM,			// 移动物品栏中的道具
		EVENT_DROPITEM,				// 丢弃物品
		EVENT_CHANGEEQUIP,			// 更换装备
		EVENT_SENDMAIL,				// 发送电子邮件
		EVENT_TRADE,				// 交易
		EVENT_CHANGEFIGHTSTATE,		// 改变战斗状态
		EVENT_ATTACKED,				// 被攻击
		EVENT_DEATH,				// 死亡
		EVENT_LOGOUT,				// 下线
		EVENT_REVIVE,				// 重生打断
		EVENT_CLIENTCOMMAND,		// 客户端命令，强制打断
		EVENT_BUYITEM,				// 在Npc处买东西		
		EVENT_SELLITEM,				// 在Npc处卖东西
	
		EVENT_TASK_TAG,				// 任务目标中
		EVENT_GENERAL_PROCESS,		// 通用进度条
		EVENT_COUNT,
	};

private:

	typedef std::list<INT>		KTimerList;
	typedef std::set<INT>		KTimerSet;
	typedef std::map<INT, INT>	KTimerMap;

	struct TEvent
	{
		BYTE bEvent[TIMER_COUNT];
		INT	nCount;
	};

public:

	KTaskTimerList()
	{
		memset(&m_cEvent, 0, sizeof(m_cEvent));

		for (INT i = 0; i < TIMER_COUNT; i++)
		{
			m_setFreeTimers.insert(i);
		}
	}

	static VOID LoadTimer(CONST INT nTaskValues[VALUE_COUNT], TTaskTimer* pTimer);
	static VOID SaveTimer(INT nTaskValues[VALUE_COUNT], CONST TTaskTimer* pTimer);

	VOID Load(KPlayer* pOwner, INT nCurrent);
	VOID Save(KPlayer* pOwner, INT nCurrent);
	VOID Activate(KPlayer* pOwner, INT nCurrent, DWORD dwEvent);
	BOOL StartTimer(INT nTimerId, INT nInterval, INT nTime, INT nMode, DWORD dwParam = 0, INT nParam = 0,  LPVOID pData = NULL, DWORD dwEvent = 0);
	VOID CloseTimer(KPlayer* pOwner, INT nTimerId, KTaskTimer::KE_CLOSE eCloseType = KTaskTimer::KE_CLOSE_FORCE);
	INT  GetRestTime(INT nTimerId, INT nCurrent);
	INT	 GetRestCount(INT nTimerId);
	INT  GetTimerCount();
	BOOL SuspendTimer(INT nTimerId);		// 暂时中止定时器
	BOOL ResumeTimer(INT nTimerId);			// 恢复定时器
	BOOL IsTimerSuspending(INT nTimerId);	// 定时器是否被暂停
	BOOL HasTimer(INT nTimerId);

private:

	KTaskTimer		m_cTaskTimers[TIMER_COUNT];
	TEvent			m_cEvent[EVENT_COUNT];
	KTimerSet		m_setFreeTimers;	// 空闲定时器队列
	KTimerMap		m_mapUsingTimers;	// 正在使用的定时器队列

	VOID GetTimerList(KTimerList& listTimers);
	INT  CreateTimer(CONST TTaskTimer* pTimer);
	BOOL RegisterTimerEvent(CONST TTaskTimer* pTimer, INT nIndex);

};

#endif //GAME_SERVER

#endif
