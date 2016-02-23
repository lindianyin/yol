
#include "CorePrivate.h"
#include "Misc/objevent.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

EVENTWATCHMAP KEventCreator::m_sEventWatchMap;
BOOL KEventCreator::m_sbProcessEvent = FALSE;

BOOL KEventCreator::AddWatcher(UINT nEvent, KObjEventWatch* pWatch)
{
	QCONFIRM_RET_FALSE(pWatch);
	m_sEventWatchMap[nEvent][pWatch] = TRUE;

	return TRUE;
}

BOOL KEventCreator::RemoveWatcher(UINT nEvent, KObjEventWatch* pWatch)
{
	QCONFIRM_RET_FALSE(pWatch);
	EVENTWATCHMAP::iterator it = m_sEventWatchMap.find(nEvent);
	QCONFIRM_RET_FALSE(it != m_sEventWatchMap.end());

	WATCHSET &mapWatch = it->second;
	WATCHSET::iterator itWatch = mapWatch.find(pWatch);
	QCONFIRM_RET_FALSE(itWatch != mapWatch.end());

	if (m_sbProcessEvent)
		itWatch->second = FALSE;
	else
		mapWatch.erase(itWatch);

	return TRUE;
}

// -------------------------------------------------------------------------
KEventCreator::KEventCreator()
:m_bProcessEvent(FALSE)
{
	//Init(); // 禁止构造函数调用虚函数
}

// -------------------------------------------------------------------------
KEventCreator::~KEventCreator()
{
	Release();
}


// -------------------------------------------------------------------------
BOOL KEventCreator::Init()
{
	// FireEvent的时候不能被初始化
	if (m_bProcessEvent)
	{
		_ASSERT(FALSE);
		return FALSE;
	}
	m_mapObjEventWatch.clear();

	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KEventCreator::Release()
{
	_ASSERT(!m_bProcessEvent);

	// 遍历所有的观察者，取消它们对此EventCreator的观察
	for (EVENTWATCHMAP::iterator it = m_mapObjEventWatch.begin(); it != m_mapObjEventWatch.end(); ++it)
	{
		WATCHSET &rcWatchMap = it->second;
		for (WATCHSET::iterator itWatch = rcWatchMap.begin(); itWatch != rcWatchMap.end(); ++itWatch)
		{
			if (itWatch->second)
			{
				KObjEventWatch* pWatch = itWatch->first;
				pWatch->m_mapEventCreator.erase(this);
				itWatch->second = FALSE;
			}
		}
	}

	if (!m_bProcessEvent)
		m_mapObjEventWatch.clear();

	return TRUE;
}

static BOOL batch_trigger_event(WATCHSET& rcWatchMap, KEventCreator& rcEventCreator, CONST KOBJEVENT& rcEvent, BOOL bCheckSuspend = TRUE)
{
	for (WATCHSET::iterator itWatch = rcWatchMap.begin(); itWatch != rcWatchMap.end();)
	{
		// Warring:liuchang 执行FireEvent的时候不能对列表有修改，itWatch->second是用来标记失效
		if (itWatch->second)
		{
			KObjEventWatch* pWatch = itWatch->first;
			if (pWatch && (!bCheckSuspend || !pWatch->IsSuspend(&rcEventCreator)))
			{
				pWatch->OnEvent(rcEventCreator, rcEvent);
			}

			++itWatch;
		}
		else
		{
			rcWatchMap.erase(itWatch++);
		}
	}

	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KEventCreator::FireEvent(CONST KOBJEVENT& rcEvent)
{
	BOOL bRet = FALSE;
	m_bProcessEvent = TRUE;
	m_sbProcessEvent = TRUE;

	EVENTWATCHMAP::iterator it  = m_mapObjEventWatch.find(rcEvent.nEvent);
	if (it != m_mapObjEventWatch.end())
	{
		WATCHSET &mapWatch = it->second;	// 对rcEvent.nEvent感兴趣的Watch集合
		batch_trigger_event(mapWatch, *this, rcEvent);
	}

	it  = m_mapObjEventWatch.find(OBJ_EVENT_ANY);
	if (it != m_mapObjEventWatch.end())
	{
		WATCHSET &mapWatch = it->second;	// 对rcEvent.nEvent感兴趣的Watch集合
		batch_trigger_event(mapWatch, *this, rcEvent);
	}


	it  = m_sEventWatchMap.find(rcEvent.nEvent);
	if (it != m_sEventWatchMap.end())
	{
		WATCHSET &mapWatch = it->second;	// 对rcEvent.nEvent感兴趣的Watch集合
		batch_trigger_event(mapWatch, *this, rcEvent, FALSE);
	}

	it  = m_sEventWatchMap.find(OBJ_EVENT_ANY);
	if (it != m_sEventWatchMap.end())
	{
		WATCHSET &mapWatch = it->second;	// 对rcEvent.nEvent感兴趣的Watch集合
		batch_trigger_event(mapWatch, *this, rcEvent, FALSE);
	}

	bRet = TRUE;

	m_bProcessEvent = FALSE;
	m_sbProcessEvent = FALSE;

	return bRet;
}


// -------------------------------------------------------------------------
BOOL KEventCreator::DispatchEvent(LPCSTR szFormat, ...)
{
	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KEventCreator::__AddWatcher(UINT nEvent, KObjEventWatch* pWatch)
{
	QCONFIRM_RET_FALSE(pWatch);
	m_mapObjEventWatch[nEvent][pWatch] = TRUE;
	
	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KEventCreator::__RemoveWatcher(UINT nEvent, KObjEventWatch* pWatch)
{
	QCONFIRM_RET_FALSE(pWatch);
	EVENTWATCHMAP::iterator it = m_mapObjEventWatch.find(nEvent);
	if (it == m_mapObjEventWatch.end())
		return FALSE;


	WATCHSET &mapWatch = it->second;
	WATCHSET::iterator itWatch = mapWatch.find(pWatch);
	if (itWatch == mapWatch.end())
		return FALSE;
	
	if (m_bProcessEvent)
		itWatch->second = FALSE;
	else
		mapWatch.erase(itWatch);

	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KEventCreator::__RemoveWatch(KObjEventWatch* pWatch)
{
	QCONFIRM_RET_FALSE(pWatch);

	for (EVENTWATCHMAP::iterator it = m_mapObjEventWatch.begin(); it != m_mapObjEventWatch.end(); ++it)
	{
		WATCHSET &mapWatch = it->second;

		WATCHSET::iterator itWatch = mapWatch.find(pWatch);
		if (itWatch == mapWatch.end())
			continue;

		if (m_bProcessEvent)
			itWatch->second = FALSE;
		else
			mapWatch.erase(itWatch);
	}

	return TRUE;
}

// -------------------------------------------------------------------------
INT KEventCreator::__GetCareEventCount(KObjEventWatch* pWatch)
{
	if (!pWatch)
	{
		_ASSERT(FALSE);
		return 0;
	}

	INT nCount = 0;
	for (EVENTWATCHMAP::iterator it = m_mapObjEventWatch.begin(); it != m_mapObjEventWatch.end(); ++it)
	{
		WATCHSET &mapWatch = it->second;
		if (mapWatch.find(pWatch) != mapWatch.end())
			++nCount;
	}

	return nCount;
}


// -------------------------------------------------------------------------
// KObjEventWatch
KObjEventWatch::~KObjEventWatch()
{
	Release();
}

// -------------------------------------------------------------------------
BOOL KObjEventWatch::Release()
{
	// 通知各个KEventCreator取消自己所订阅的所有事件，这样Creator产生事件的时候不会遍历到这个Watcher了
	for (EVENTCREATORMAP::iterator it = m_mapEventCreator.begin(); it != m_mapEventCreator.end(); ++it)
	{
		KEventCreator* pEventCreator = it->first;
		pEventCreator->__RemoveWatch(this);
	}

	m_mapEventCreator.clear();

	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KObjEventWatch::Watch(UINT nEvent, KEventCreator* pEventCreator /* = NULL */)
{
	if (!pEventCreator)
	{
		KEventCreator::AddWatcher(nEvent, this);
	}
	else
	{
		pEventCreator->__AddWatcher(nEvent, this);
		m_mapEventCreator[pEventCreator] = TRUE;
	}

	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KObjEventWatch::CancelWatch(UINT nEvent, KEventCreator* pEventCreator /* = NULL */)
{
	if (!pEventCreator)
	{
		KEventCreator::RemoveWatcher(nEvent, this);
	}
	else
	{
		pEventCreator->__RemoveWatcher(nEvent, this);
		if (!pEventCreator->__GetCareEventCount(this))
			m_mapEventCreator.erase(pEventCreator);
	}

	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KObjEventWatch::Suspend(KEventCreator* pEventCreator)
{
	if (!pEventCreator)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	if (m_mapEventCreator.find(pEventCreator) == m_mapEventCreator.end())
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	m_mapEventCreator[pEventCreator] = FALSE;

	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KObjEventWatch::Resume(KEventCreator* pEventCreator)
{
	if (!pEventCreator)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	if (m_mapEventCreator.find(pEventCreator) == m_mapEventCreator.end())
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	m_mapEventCreator[pEventCreator] = TRUE;
	return TRUE;
}

// -------------------------------------------------------------------------
BOOL KObjEventWatch::IsSuspend(KEventCreator* pEventCreator)
{
	if (!pEventCreator)
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	if (m_mapEventCreator.find(pEventCreator) == m_mapEventCreator.end())
	{
		_ASSERT(FALSE);
		return FALSE;
	}

	return !m_mapEventCreator[pEventCreator];
}

// -------------------------------------------------------------------------
