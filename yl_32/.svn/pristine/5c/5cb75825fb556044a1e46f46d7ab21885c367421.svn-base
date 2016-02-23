

#include "stdafx.h"
#include "onlinegamebase/ktimermgr.h"
#include "onlinegamebase/kscriptmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KTimerMgr g_cTimerMgr;

// -------------------------------------------------------------------------

KTimerMgr::KTimerMgr(void) : m_uNextScriptTime(0), m_dwCurrentTime(0)
{
}

KTimerMgr::~KTimerMgr(void)
{
}

BOOL KTimerMgr::Init()
{
	return TRUE;
}

BOOL KTimerMgr::UnInit()
{
	return TRUE;
}

UINT KTimerMgr::AddScriptTimePoint(UINT uFrame)
{
	UINT uActiveFame	= m_dwCurrentTime + uFrame;
	m_setScriptTimer.insert(uActiveFame);
	//Q_Printl("..m_setScriptTimer.size()="<<m_setScriptTimer.size());
	return uActiveFame;
}

BOOL KTimerMgr::Breath()
{
	m_dwCurrentTime++;
	//脚本定时器处理
	if (m_dwCurrentTime >= m_uNextScriptTime)
	{
		KScriptTimerSet::iterator it = m_setScriptTimer.begin();
		while (it != m_setScriptTimer.end())
		{
			if (*it > m_dwCurrentTime)
				break;
#ifdef GAME_SERVER
			_ASSERT(m_uNextScriptTime == *it);	// GameServer不会出现丢桢现象
#endif

			// 调用脚本系统初始化
			ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
			int nTopIdx = cSafeScript->GetTopIndex();
			cSafeScript->SafeCallBegin(nTopIdx);
			BOOL bRetCode = cSafeScript->CallTableFunction("Timer", "OnActive", 0, "d", *it);
			cSafeScript->SafeCallEnd(nTopIdx);
			if (!bRetCode)
			{
				ASSERT(FALSE);
				return FALSE;
			}			
			m_setScriptTimer.erase(it);
			it = m_setScriptTimer.begin();
		}

		if (it == m_setScriptTimer.end())
			m_uNextScriptTime	= (UINT)-1;
		else
			m_uNextScriptTime	= *it;
	}
	return TRUE;
}
