
#pragma once

#ifdef WIN32

class CORE_API QLock
{
public:
    QLock()        { InitializeCriticalSection(&m_CriticalSection);            }
    ~QLock()       { DeleteCriticalSection(&m_CriticalSection);                }
    int Lock()      { EnterCriticalSection(&m_CriticalSection); return true;    }
    int Unlock()    { LeaveCriticalSection(&m_CriticalSection); return true;    }

private:
    CRITICAL_SECTION m_CriticalSection;
};


#else   // POSIX

#include <pthread.h>

class QLock
{
public:
    QLock()        { pthread_mutex_init(&m_Mutex, NULL);                                             }
    ~QLock()       { pthread_mutex_destroy(&m_Mutex);                                          }
    int Lock()      { int nRetCode = pthread_mutex_lock(&m_Mutex);      return (nRetCode == 0); }
    int Unlock()    { int nRetCode = pthread_mutex_unlock(&m_Mutex);    return (nRetCode == 0); }

private:
    pthread_mutex_t m_Mutex;
};

#endif
