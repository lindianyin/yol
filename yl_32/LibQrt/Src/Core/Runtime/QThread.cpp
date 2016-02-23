
#include "CorePrivate.h"
#include "QThread.h"

//---------------------------------------------------------------------------
// 函数:	~KThread
// 功能:	构造函数
// 参数:	void
// 返回:	void
//---------------------------------------------------------------------------
QThread::QThread()
{
	#ifdef WIN32
		m_ThreadHandle      = NULL;
	#else
		m_ThreadHandle      = 0;
	#endif
	m_pfnThread         = NULL;
    m_pvThreadArg       = NULL;
}

//---------------------------------------------------------------------------
// 函数:	~KThread
// 功能:	析购函数
// 参数:	void
// 返回:	void
//---------------------------------------------------------------------------
QThread::~QThread()
{
    Wait();
}
//---------------------------------------------------------------------------
// 函数:	MyThreadProc
// 功能:	线程过程
// 参数:	pvArg       参数
// 返回:	void
//---------------------------------------------------------------------------
#ifdef WIN32
static unsigned __stdcall MyThreadProc(void *pvArg)
#else
static void *MyThreadProc(void *pvArg)
#endif
{
	QThread *pThread = (QThread *)pvArg;
	
	pThread->ThreadFunction();
	
    return 0;
}

//---------------------------------------------------------------------------
// 函数:	ThreadFunction
// 功能:
// 参数:	void
// 返回:	void
//---------------------------------------------------------------------------
void QThread::ThreadFunction()
{
	m_pfnThread(m_pvThreadArg);
}

//---------------------------------------------------------------------------
// 函数:	Create
// 功能:	创建线程
// 参数:	pfnThread		线程函数
//			pvArg 		    线程参数
// 返回:	int
//---------------------------------------------------------------------------
int QThread::Create(QThreadFunction *pfnThread, void *pvArg)
{
	m_pfnThread     = pfnThread;
	m_pvThreadArg   = pvArg;
	
	if (m_ThreadHandle)
	    return false;

#ifdef WIN32
    unsigned uThreadID;
	m_ThreadHandle = (HANDLE)_beginthreadex(
		NULL,			        // SD
		0,				        // initial stack size
		MyThreadProc,		    // thread function
		(void *)this,	        // thread argument
		0,				        // creation option
		(unsigned *)&uThreadID   // thread identifier
    );	
	return (m_ThreadHandle != NULL);
#else
    int nRetCode = -1;
    pthread_attr_t   ThreadAttr; 
    nRetCode = pthread_attr_init(&ThreadAttr);
    if (nRetCode != 0)  // if fail
        return false;
    
    nRetCode = pthread_attr_setstacksize(&ThreadAttr, 256 * 1024);
    if (nRetCode != 0)  // if fail
        return false;

    nRetCode = pthread_create(
        &m_ThreadHandle, 
        &ThreadAttr, 
        MyThreadProc, 
        this
    );

    if (nRetCode != 0) // if fail
	{
		m_ThreadHandle      = 0;
	}
    
    pthread_attr_destroy(&ThreadAttr);    
	return (nRetCode == 0);
#endif
}

int QThread::Wait()
{
	int nResult = false;

	PROCESS_ERROR(m_ThreadHandle);


#ifdef WIN32
	if (m_ThreadHandle)
	{
		WaitForSingleObject(m_ThreadHandle, INFINITE);

		CloseHandle(m_ThreadHandle);
		m_ThreadHandle      = NULL;
	}
#else
	if (m_ThreadHandle)
	{
		pthread_join(m_ThreadHandle, NULL);
		m_ThreadHandle      = 0;
	}
#endif

	nResult = true;
EXIT0:
	return nResult;
}

//---------------------------------------------------------------------------
// 函数:	Terminate
// 功能:	终结线程
// 参数:	void
// 返回:	void
//---------------------------------------------------------------------------
int QThread::Terminate(DWORD dwExitCode /* = 0 */)
{
	int nResult = false;

	PROCESS_ERROR(m_ThreadHandle);

#ifdef WIN32
	if (m_ThreadHandle)
	{
		TerminateThread(m_ThreadHandle, dwExitCode);
		m_ThreadHandle      = NULL;
	}
#else
	if (m_ThreadHandle)
	{
		pthread_cancel(m_ThreadHandle);
		m_ThreadHandle      = 0;
	}
#endif

	nResult = true;
EXIT0:
	return nResult;
}

#ifdef WIN32
int QThread::IsAlive()
{
	INT nResult = 0;
	DWORD dwExitCode = 0;

	PROCESS_ERROR(m_ThreadHandle && m_ThreadHandle != INVALID_HANDLE_VALUE);

	nResult = GetExitCodeThread(m_ThreadHandle, &dwExitCode);
	LOG_PROCESS_ERROR(nResult);

	nResult = (dwExitCode == STILL_ACTIVE);
EXIT0:
	return nResult;
}
#else
int QThread::IsAlive()
{
	INT nResult = 0;
	INT nRetCode = 0;

	PROCESS_ERROR(m_ThreadHandle);

	nRetCode = pthread_kill(m_ThreadHandle, 0);

	nResult = (ESRCH != nRetCode);
EXIT0:
	return nResult;
}
#endif

int QThread_Sleep(unsigned uMilliseconds)
{
    #ifdef WIN32
    Sleep(uMilliseconds);
    return true;
    #else
    int nRetCode = false;
    nRetCode = usleep(uMilliseconds * 1000);
    return (nRetCode == 0);
    #endif
}
