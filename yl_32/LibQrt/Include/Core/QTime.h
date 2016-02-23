
#pragma once

#ifdef WIN32

//CORE_API unsigned long KG_GetTickCount();

#else

CORE_API unsigned long GetTickCount();

#endif


//CORE_API int KG_stime(time_t *ptmTime);


inline unsigned long long RDTSC()		// ReaD Time Stamp Counter
{
#ifdef WIN32
	__asm RDTSC
#else
    unsigned long long ullCPUTick = 0;
    unsigned uCPUTickLow  = 0;
    unsigned uCPUTickHigh = 0;

    __asm__ __volatile__("rdtsc" : "=a" (uCPUTickLow), "=d" (uCPUTickHigh): );

    ullCPUTick = (((unsigned long long)uCPUTickHigh) << 32) + uCPUTickLow;
    return ullCPUTick;
#endif
}

