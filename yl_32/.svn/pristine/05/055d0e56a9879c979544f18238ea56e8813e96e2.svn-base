
//#pragma once

#if (defined(_MSC_VER) || defined(__ICL))
#include <direct.h>
#include <io.h>
#else   // if linux
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if (defined(_MSC_VER) || defined(__ICL))
#define snprintf  _snprintf
#define vsnprintf _vsnprintf
#endif 

//#if (defined(_MSC_VER) || defined(__ICL))
//#define fileno _fileno
//#endif


#if (defined(_MSC_VER) || defined(__ICL))
#else   // if linux
inline int mkdir(const char cszDir[])
{
	char *ptr = (char*)cszDir;
	while (*ptr)
	{
		if (*ptr == '\\')
			*ptr = '/';
		ptr++;
	}
    return mkdir(cszDir, 0755);
}
#endif

#if (defined(_MSC_VER) || defined(__ICL))

inline struct tm *localtime_r(const time_t *timep, struct tm *result)
{
	struct tm *ptm = NULL;
#ifdef WIN32
	if (0 == localtime_s(result, timep))
		ptm = result;
#else
	ptm = localtime(timep);
	if (
		(result) &&
		(ptm)
		)
	{
		*result = *ptm;
	}
#endif
    return ptm;
};
#endif

#ifndef PATH_MAX
#define PATH_MAX    1024
#endif

