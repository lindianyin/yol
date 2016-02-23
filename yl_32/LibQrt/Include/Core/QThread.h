
#pragma once

#include "CoreDeclspec.h"

typedef void QThreadFunction(void *pvArg);

#ifdef WIN32
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif


//---------------------------------------------------------------------------
class CORE_API QThread
{
public:
    int             Create(QThreadFunction *pfnThread, void *pvArg);
    int             Wait(); // µÈ´ý½áÊø
	int				Terminate(DWORD dwExitCode = 0);
	int				IsAlive();
public:
	QThread();
	~QThread();

    void            ThreadFunction();

private:
#ifdef WIN32
    HANDLE          m_ThreadHandle;
#else
    pthread_t       m_ThreadHandle;  
#endif

	QThreadFunction *m_pfnThread;
    void            *m_pvThreadArg;
};

CORE_API int  QThread_Sleep(unsigned uMilliseconds);

#ifdef WIN32

inline void *QGetCurrentThreadId()
{
    return (void *)(INT_PTR)GetCurrentThreadId();
}

#else     //linux

inline void *QGetCurrentThreadId()
{
    return (void *)pthread_self();
}

#endif
