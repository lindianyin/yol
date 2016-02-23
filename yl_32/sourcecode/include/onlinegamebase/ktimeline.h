
#pragma once
// -------------------------------------------------------------------------
// 和原有Timer相比 有 1、粒度以秒为单位(能满足粒度支持不同的需求么) 2、要能在C++中使用

#define E_TIMER_DONE		0
#define E_TIMER_CONTINUE	-1
//#define KD_TIMEOUT			(3 * 16)	// 定时器延时最长不得超出3秒（当前机制是每帧处理最多5W，一秒可处理80W，出现超时的概率很低）
#define KD_TIMER_MAX		50000
#define KD_TIMER_INTMAX		0xFFFFFFFF

struct IKTimerCallBack
{
	virtual INT OnTimer(LPVOID pParam) = 0;
};
typedef IKTimerCallBack* PKTIMECALLBACK;

class KTimeline
{
	struct KTIMERDATA
	{
		PKTIMECALLBACK pCallBack;
		LPVOID pParam;
		UINT nInterval;
	};

private:
	//UINT	m_uToDoTimer;					// 下次触发脚本时间
	UINT	m_uCurrentTime;
	UINT	m_dwIDCounter;

	typedef std::map<DWORD, KTIMERDATA>	KTIMERS;	// 纵向的定时器数组，存储注册在某个时间点上的所有定时器数据
	typedef std::map<UINT, KTIMERS> KTIMELINE;		// 横向时间轴
	typedef std::map<DWORD, UINT> KTIMERINDEX;		// 提供ID->Interval的索引，方便迅速找到具体的定时器

	KTIMELINE m_mapTimeline;
	KTIMERINDEX m_mapTimerIndex;

public:
	KTimeline(){}
	virtual ~KTimeline(){}

public:

	VOID Init();
	VOID Uninit();
	VOID Activate();
	DWORD Register(UINT nInterval, CONST PKTIMECALLBACK pTimeCallBack, LPVOID pParam = NULL);
	VOID Unregister(DWORD dwTimerId);

protected:
	DWORD _NewId();
	DWORD _Register(DWORD dwTimerId, UINT nInterval, CONST PKTIMECALLBACK pTimeCallBack, LPVOID pParam);
};

#ifndef GAME_CENTER
extern KTimeline g_cTimeline;
#endif

// -------------------------------------------------------------------------
