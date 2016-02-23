////////////////////////////////////////////////////////////////////////////////
//  Copyright(c) Kingsoft
//
//	FileName    :   KG_MicroTimer.h
//	Creator     :   Liang Bo
//	Date        :   2008-7-31 15:00:46
//	Comment     :   time for microseconds
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __KG_MicroTimer_h__
#define __KG_MicroTimer_h__

class KG_MicroTimer
{
public:
    KG_MicroTimer()  { m_nGetCPUFrequenceFlag = false; };
    ~KG_MicroTimer() { m_nGetCPUFrequenceFlag = false; };

    int Start();
    int Stop();
    
    int TimerDiff(double *pfRetTimerDiff);

private:
    int _GetCPUFrequence();

private:
    int m_nGetCPUFrequenceFlag;

#ifdef WIN32
    LARGE_INTEGER  m_ullStart;
    LARGE_INTEGER  m_ullStop;
    LARGE_INTEGER  m_CPUFrequency;
#else
    struct timeval m_lStart;
    struct timeval m_lStop;
#endif
};

#endif // __KG_MicroTimer_h__
