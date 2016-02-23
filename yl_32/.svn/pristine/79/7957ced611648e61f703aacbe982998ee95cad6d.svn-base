
#include "CorePrivate.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#include "QTime.h"


#ifdef WIN32


//unsigned long KG_GetTickCount()
//{
//	return GetTickCount();
//}
#else 

#include <sys/time.h>

#include <unistd.h>


unsigned long GetTickCount()
{
#ifdef CLOCK_MONOTONIC
	struct timespec tmsTime = {0,0};
	/*int err = */clock_gettime(CLOCK_MONOTONIC, &tmsTime);

	return ULONG(tmsTime.tv_sec * 1000ULL + tmsTime.tv_nsec / 1000000);
#else
	struct timeval tmval;
	/*int err = */gettimeofday(&tmval, NULL);
	return ULONG(tmval.tv_sec * 1000ULL + tmval.tv_usec / 1000);
#endif
}
#endif // WIN32

//
//int KG_stime(time_t *ptmTime)
//{
//    int nResult  = false;
//    int nRetCode = false;
//
//#ifdef WIN32
//    tm  tmServerTime;
//    tm *pTime = NULL;
//    SYSTEMTIME	tmSystemTime;
//
//    pTime = localtime(ptmTime);
//    if (!pTime)
//         goto EXIT0;
//    tmServerTime               = *pTime;
//    tmSystemTime.wYear         = tmServerTime.tm_year + 1900;
//    tmSystemTime.wMonth        = tmServerTime.tm_mon + 1;
//    tmSystemTime.wDay          = tmServerTime.tm_mday;
//    tmSystemTime.wHour         = tmServerTime.tm_hour;
//    tmSystemTime.wMinute       = tmServerTime.tm_min;
//    tmSystemTime.wSecond       = tmServerTime.tm_sec;
//    tmSystemTime.wDayOfWeek    = tmServerTime.tm_wday;
//    tmSystemTime.wMilliseconds = 0;
//    nRetCode = SetLocalTime(&tmSystemTime);
//    PROCESS_ERROR(nRetCode);
//#else
//    PROCESS_ERROR(ptmTime);
//    
//    struct timeval tmval;
//    tmval.tv_sec  = *ptmTime;
//    tmval.tv_usec = 0;
//    
//    nRetCode = settimeofday(&tmval, NULL);
//    PROCESS_ERROR(nRetCode == 0);
//#endif
//
//    nResult = true;
//EXIT0:
//    return nResult;
//}
