

#include "stdafx.h"
#include "kdatetimecontrol.h"
#include "Core/QTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KDateTimeControl::KDateTimeControl()
{
	m_uCurElapse = m_uCurElapseHigh = 0;
	m_uStartElapse = 0;
	m_tTimeAdjust = 0;
}

BOOL KDateTimeControl::Loop()
{
#if 0
	UINT uElapse = m_cTimer.GetElapse();
#else
	UINT uElapse = GetTickCount();
#endif
	// Òç³ö
	if (uElapse < m_uCurElapse)
	{
		m_uCurElapseHigh++;
	}
	// bug£ºÀÛ»ýÎó²î [8/18/2011 simon]
	//m_tCurGameTime += uElapse / 1000 - m_uCurElapse / 1000;
	m_tCurGameTime = time(NULL) + (INT)m_tTimeAdjust;
	m_uCurElapse = uElapse;
	return TRUE;
}

BOOL KDateTimeControl::Init()
{
	m_uStartElapse = GetTickCount();
	m_uCurElapse = m_uStartElapse;
	m_tCurGameTime = time(NULL);
	m_cRandGentor.RandSeed(m_tCurGameTime);
	return TRUE;
}

UINT KDateTimeControl::GetGameElapse()
{
	return m_uCurElapse - m_uStartElapse;
}

INT64 KDateTimeControl::GetRealGameElapse()
{
	return (((INT64)m_uCurElapseHigh << 32) | m_uCurElapse) - m_uStartElapse;
}

time_t KDateTimeControl::GetGameTime( time_t* pTime)
{
	if (pTime)
		*pTime = m_tCurGameTime;
	return m_tCurGameTime;
}

time_t KDateTimeControl::GetSysTime( time_t* pTime)
{
	return time(pTime);
}

BOOL KDateTimeControl::SetGameTime( const time_t& tTime )
{
	m_tTimeAdjust = tTime - time(NULL);
	return TRUE;
}

DWORD KDateTimeControl::Rand()
{
	return m_cRandGentor.Rand();
}
// -------------------------------------------------------------------------
