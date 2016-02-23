////////////////////////////////////////////////////////////////////////////////
//  Copyright(c) Kingsoft
//
//	FileName    :   KG_MicroTimer.cpp
//	Creator     :   Liang Bo
//	Date        :   2008-7-31 15:16:07
//	Comment     :   
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG_MicroTimer.h"
#ifndef WIN32
#include <sys/time.h>
#endif

////////////////////////////////////////////////////////////////////////////////
int KG_MicroTimer::Start()
{
    int nResult  = false;
    int nRetCode = false;

    if (!m_nGetCPUFrequenceFlag)
    {
        nRetCode = _GetCPUFrequence();
        LOG_PROCESS_ERROR(nRetCode && "_GetCPUFrequence()");
        m_nGetCPUFrequenceFlag = true;
    }

#ifdef WIN32
    nRetCode = QueryPerformanceCounter(&m_ullStart);
    LOG_PROCESS_ERROR(nRetCode);
#else
    nRetCode = gettimeofday(&m_lStart, NULL);
    LOG_PROCESS_ERROR(nRetCode == 0);
#endif

    nResult = true;
EXIT0:
    return nResult;
}

int KG_MicroTimer::Stop()
{
    int nResult  = false;
    int nRetCode = false;

#ifdef WIN32
    nRetCode = QueryPerformanceCounter(&m_ullStop);
    LOG_PROCESS_ERROR(nRetCode);
#else
    nRetCode = gettimeofday(&m_lStop, NULL);
    LOG_PROCESS_ERROR(nRetCode == 0);
#endif

    nResult = true;
EXIT0:
    return nResult;
}

int KG_MicroTimer::TimerDiff(double *pfRetTimerDiff)
{
    int nResult  = false;
    int nRetCode = false;
    double fTimeConsume = 0;

    LOG_PROCESS_ERROR(pfRetTimerDiff);

    ASSERT(m_nGetCPUFrequenceFlag);

    *pfRetTimerDiff = 0;

#ifdef WIN32
    ASSERT(m_CPUFrequency.QuadPart != 0);
    fTimeConsume = (m_ullStop.QuadPart - m_ullStart.QuadPart) * 1.0 / m_CPUFrequency.QuadPart;
#else
    fTimeConsume = (double)((m_lStop.tv_sec - m_lStart.tv_sec) * 1000000 + m_lStop.tv_usec - m_lStart.tv_usec) / 1000000;
#endif

    *pfRetTimerDiff = fTimeConsume;

    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KG_MicroTimer::_GetCPUFrequence()
{
    int nResult  = false;
    int nRetCode = false;

#ifdef WIN32
    m_CPUFrequency.QuadPart = 0;
    nRetCode = QueryPerformanceFrequency(&m_CPUFrequency);
    LOG_PROCESS_ERROR(nRetCode && "Get CPU Frequency Failed");
#endif

    nResult = true;
EXIT0:
    return nResult;
}
