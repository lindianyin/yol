
#pragma once

#include "KG_SocketThread.h"
#include "KG_LogThread.h"
#include "KG_Queue.h"

////////////////////////////////////////////////////////////////////////////////
class LogServer
{
public:
    LogServer();
    ~LogServer(void);

    int Init(int argc, char *argv[], void *pvContext);
    int UnInit(void *pvContext);

    int Run();

	void ProcessKingEye();
	int SendCustomInfo2KingEye();
	int SetExitFlag(int nFlag)  { m_nExitFlag = nFlag; return true; };

private:
    int _ReadConfigFile(
        int argc, char *argv[], 
        KG_SOCKET_THREAD_PARAM *pRetSocketThreadParam,
        KG_LOG_THREAD_PARAM    *pRetLogThreadParam
    );
    int _OnCmd_SetLogLevel(QLOG_LEVEL eLogLevel);
    int _OnCmd_Status();
    int _OnCmd_ShowUsage();

private:
    int                       m_nExitFlag;
    //KG_ServerController      *m_pController;
    KG_SocketThread           m_SocketThread;

    // use only one thread, to keep the log's order which from same server 
    KG_LogThread              m_LogThread;

    // even only on log thread, we need use queue to adapt performance variation
    KG_PACKAGE_LOCK_QUEUE     m_RequestQueue;
    KG_PACKAGE_LOCK_QUEUE     m_ResponseQueue;

};


