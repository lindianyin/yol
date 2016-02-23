
#pragma once

#include <map>
#include <vector>

#define OBJ_EVENT_ANY 0xffffffff

struct KOBJEVENT
{
	KOBJEVENT(UINT nEvent, INT nParam1 = 0, INT nParam2 = 0, INT nParam3 = 0)
		:nEvent(nEvent)
	{
		anEventParam[0] = nParam1;
		anEventParam[1] = nParam2;
		anEventParam[2] = nParam3;
	}

	UINT nEvent;
	INT anEventParam[3];
};


class KObjEventWatch;

typedef std::map<KObjEventWatch*, BOOL> WATCHSET;		// <观察者对象, 是否有效>
typedef std::map<INT, WATCHSET> EVENTWATCHMAP;			// <事件类型, 观察者列表>

// 事件的发起者，需要派发事件的Class必须从此派生
class KEventCreator
{
	friend class KObjEventWatch;
	
public:
	static BOOL AddWatcher(UINT nEvent, KObjEventWatch* pWatch);
	static BOOL RemoveWatcher(UINT nEvent, KObjEventWatch* pWatch);

public:
	KEventCreator();
	virtual ~KEventCreator();

	virtual BOOL Init();
	BOOL Release();

public:
	template<typename T1, typename T2, typename T3>
	BOOL FireEvent(UINT nEvent, T1 Param1, T2 nParam2, T3 nParam3);

private:
	BOOL FireEvent(CONST KOBJEVENT& rcEvent);

protected:
	virtual BOOL DispatchEvent(LPCSTR szFormat, ...);

private:
	// 添加一个对nEvent感兴趣的pWatch
	BOOL __AddWatcher(UINT nEvent, KObjEventWatch* pWatch);

	// 删除一个对nEvent感兴趣的pWatch
	BOOL __RemoveWatcher(UINT nEvent, KObjEventWatch* pWatch);

	// 删除一个Watch
	BOOL __RemoveWatch(KObjEventWatch* pWatch);

	// 获得一个Watch对此KEventCreator感兴趣的Event数目
	INT __GetCareEventCount(KObjEventWatch* pWatch);

private:
	static EVENTWATCHMAP m_sEventWatchMap;	// 全局事件观察者，不管事件发生者是谁
	static BOOL  m_sbProcessEvent;
private:
	EVENTWATCHMAP m_mapObjEventWatch;		// 观察者Map
	BOOL m_bProcessEvent;					// 是否正在发送事件
};


template<>
inline BOOL KEventCreator::FireEvent(UINT nEvent, INT nParam1, INT nParam2, INT nParam3)
{
	FireEvent(KOBJEVENT(nEvent, nParam1, nParam2, nParam3));
	DispatchEvent("uddd", nEvent, nParam1, nParam2, nParam3);
	return TRUE;
}

template<>
inline BOOL KEventCreator::FireEvent(UINT nEvent, INT nParam1, INT nParam2, DWORD nParam3)
{
	FireEvent(KOBJEVENT(nEvent, nParam1, nParam2, nParam3));
	DispatchEvent("uddu", nEvent, nParam1, nParam2, nParam3);
	return TRUE;
}

template<>
inline BOOL KEventCreator::FireEvent(UINT nEvent, INT nParam1, DWORD nParam2, INT nParam3)
{
	FireEvent(KOBJEVENT(nEvent, nParam1, nParam2, nParam3));
	DispatchEvent("udud", nEvent, nParam1, nParam2, nParam3);
	return TRUE;
}

template<>
inline BOOL KEventCreator::FireEvent(UINT nEvent, INT nParam1, DWORD nParam2, DWORD nParam3)
{
	FireEvent(KOBJEVENT(nEvent, nParam1, nParam2, nParam3));
	DispatchEvent("uduu", nEvent, nParam1, nParam2, nParam3);
	return TRUE;
}

template<>
inline BOOL KEventCreator::FireEvent(UINT nEvent, DWORD nParam1, INT nParam2, INT nParam3)
{
	FireEvent(KOBJEVENT(nEvent, nParam1, nParam2, nParam3));
	DispatchEvent("uudd", nEvent, nParam1, nParam2, nParam3);
	return TRUE;
}

template<>
inline BOOL KEventCreator::FireEvent(UINT nEvent, DWORD nParam1, INT nParam2, DWORD nParam3)
{
	FireEvent(KOBJEVENT(nEvent, nParam1, nParam2, nParam3));
	DispatchEvent("uudu", nEvent, nParam1, nParam2, nParam3);
	return TRUE;
}

template<>
inline BOOL KEventCreator::FireEvent(UINT nEvent, DWORD nParam1, DWORD nParam2, INT nParam3)
{
	FireEvent(KOBJEVENT(nEvent, nParam1, nParam2, nParam3));
	DispatchEvent("uuud", nEvent, nParam1, nParam2, nParam3);
	return TRUE;
}

template<>
inline BOOL KEventCreator::FireEvent(UINT nEvent, DWORD nParam1, DWORD nParam2, DWORD nParam3)
{
	FireEvent(KOBJEVENT(nEvent, nParam1, nParam2, nParam3));
	DispatchEvent("uuuu", nEvent, nParam1, nParam2, nParam3);
	return TRUE;
}


// 事件的观察者，需要订阅事件的类从此派生
class KObjEventWatch
{
	friend class KEventCreator;
	typedef std::map<KEventCreator*, BOOL> EVENTCREATORMAP;			// <被观察者, 是否正在观察(0表示暂停)>
public:
	virtual ~KObjEventWatch();

	BOOL Release();
public:
	// 观察指定KEventCreator的指定事件，如果没有指定pEventCreator则是观察所有的pEventCreator
	BOOL Watch(UINT nEvent, KEventCreator* pEventCreator = NULL);

	// 取消观察
	BOOL CancelWatch(UINT nEvent, KEventCreator* pEventCreator = NULL);

	// 暂停对一个KEventCreator的观察
	BOOL Suspend(KEventCreator* pEventCreator);
	
	// 恢复对一个KEventCreator的观察
	BOOL Resume(KEventCreator* pEventCreator);

	// 是否对一个KEventCreator暂停观察
	BOOL IsSuspend(KEventCreator* pEventCreator);

public:
	// 响应事件
	virtual BOOL OnEvent(KEventCreator& rcSender, CONST KOBJEVENT& rcEvent) = 0;
	
private:
	EVENTCREATORMAP m_mapEventCreator;	// 保存[被观察者]列表，当观察者Release的时候必须通知所有[被观察者]
};

