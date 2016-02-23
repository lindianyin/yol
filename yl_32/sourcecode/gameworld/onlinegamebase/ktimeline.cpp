
#include "stdafx.h"
#include "onlinegamebase/ktimeline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

#ifndef GAME_CENTER
KTimeline g_cTimeline;
#endif

VOID KTimeline::Init()
{
	//m_uToDoTimer	= KD_TIMER_INTMAX;
	m_uCurrentTime	= 0;
	m_dwIDCounter	= 0;
	//return TRUE;
}

VOID KTimeline::Uninit()
{
}

VOID KTimeline::Activate()
{
	// 不用考虑溢出，每次服务器启动是从0开始，每秒16帧，需要连续8年以上
	m_uCurrentTime++;

	//if (m_uToDoTimer > m_uCurrentTime)
	//{
	//	return;
	//}

	//顺序处理时间轴(处理横轴)
	INT nActionCount = 0;
	KTIMELINE::iterator it = m_mapTimeline.begin();
	while (it != m_mapTimeline.end() && it->first < m_uCurrentTime)
	{
		UINT uCurrFrame		= it->first;
		KTIMERS& cTimers	= it->second; 
		++it; // 后续操作可能会导致it失效，先步进一下

		// 处理挂在某时间点上的所有定时器(处理纵轴)
		UINT uCount = (UINT)cTimers.size();
		KTIMERS::iterator itTimer = cTimers.begin();
		while(uCount--)
		{
			DWORD dwTimerId	= itTimer->first;
			KTIMERDATA cTD	= itTimer->second;
			++itTimer;

			Unregister(dwTimerId);	// 此处一定会导致纵轴迭代器失效，可能会导致横轴迭代器失效 TODO 可进一步优化，批量删除触发过的定时器，需记录当前处理到哪个定时器，可能一轮未走完
			if (cTD.pCallBack)
			{
				INT nRet = cTD.pCallBack->OnTimer(cTD.pParam);
				if (nRet == E_TIMER_CONTINUE)
				{
					nRet = cTD.nInterval;
				}
				if (nRet > E_TIMER_DONE)
				{
					_Register(dwTimerId, cTD.nInterval, cTD.pCallBack, cTD.pParam);
				}
			}

			if (++nActionCount > KD_TIMER_MAX)
			{
				QLogPrintf(LOG_INFO, 
					"[Timer]定时器单帧注册数超过%d,超出部分顺延至下帧继续,时间轴:%d.", KD_TIMER_MAX, uCurrFrame);
				return;
			}
		}

		//m_uToDoTimer = uCurrFrame;
	}
}

DWORD KTimeline::Register(UINT nInterval, CONST PKTIMECALLBACK pTimeCallBack, LPVOID pParam)
{
	//// 如果插入过大的值，会导致新定时器可能正好插入到超时区域，导致无法分辨是"超时"还是"新加入"
	//if (nInterval > (KD_TIMER_INTMAX - KD_TIMEOUT))
	//{
	//	QLogPrintf(LOG_ERR, "[Timer]定时器时间范围超出限制，注册定时器失败.");
	//	return 0;
	//}

	DWORD dwTimerId = _NewId();
	if (dwTimerId <= 0)	
	{
		QLogPrintf(LOG_ERR, "[Timer]定时器ID池已经耗尽，注册定时器失败.");
		return dwTimerId;
	}

	return _Register(dwTimerId, nInterval, pTimeCallBack, pParam);
}

VOID KTimeline::Unregister(DWORD dwTimerId)
{
	KTIMERINDEX::iterator itIdx = m_mapTimerIndex.find(dwTimerId);
	if (itIdx == m_mapTimerIndex.end())
	{
		QLogPrintf(LOG_WARNING, "[Timer]不存在的定时器ID %d.", dwTimerId);
		return;
	}

	KTIMELINE::iterator itTimers = m_mapTimeline.find(itIdx->second);
	if (itTimers == m_mapTimeline.end())
	{
		QLogPrintf(LOG_ERR, "[Timer]定时器数据错误. ID %d, nInterval %d", dwTimerId, itIdx->second);
		return;
	}

	KTIMERS& cTimers = itTimers->second;
	KTIMERS::iterator itT = cTimers.find(dwTimerId);
	if (itT == cTimers.end())
	{
		QLogPrintf(LOG_ERR, "[Timer]定时器数据错误. ID %d", dwTimerId);
		return;
	}

	m_mapTimerIndex.erase(itIdx);
	cTimers.erase(itT);
	if (cTimers.size() <= 0)
	{
		m_mapTimeline.erase(itTimers);
	}
}

DWORD KTimeline::_NewId()
{
	INT nLoopCount = 50;
	KTIMERINDEX::iterator pos;
	do
	{
		pos = m_mapTimerIndex.find(++m_dwIDCounter);

		// 连续50次都找不到一个有效ID，就失败返回了
		nLoopCount--;
		if (nLoopCount <= 0)
		{
			return 0;
		}

	}while((!m_dwIDCounter) || pos != m_mapTimerIndex.end());

	return m_dwIDCounter;
}

DWORD KTimeline::_Register(DWORD dwTimerId, UINT nInterval, CONST PKTIMECALLBACK pTimeCallBack, LPVOID pParam)
{
	KTIMERDATA cTimerData;
	cTimerData.pCallBack = pTimeCallBack;
	cTimerData.pParam = pParam;
	cTimerData.nInterval = nInterval;

	// 将定时器挂到相应的时间轴下
	UINT uActiveFame = m_uCurrentTime + nInterval;
	KTIMELINE::iterator itTimers = m_mapTimeline.find(uActiveFame);
	if (itTimers == m_mapTimeline.end())
	{
		KTIMERS cTimers;
		cTimers[dwTimerId] = cTimerData;
		m_mapTimeline[uActiveFame] = cTimers;
	}
	else
	{
		KTIMERS& cTimers = itTimers->second;
		cTimers[dwTimerId] = cTimerData;
	}

	// 增加索引
	m_mapTimerIndex[dwTimerId] = uActiveFame;

	//m_uToDoTimer = m_mapTimeline.begin()->first;

	return dwTimerId;
}


// -------------------------------------------------------------------------
