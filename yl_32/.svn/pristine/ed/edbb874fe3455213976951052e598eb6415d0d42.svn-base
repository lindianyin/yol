

#include "stdafx.h"
#include <string>
#include <signal.h>
#ifdef __GNUC__
#include <sys/time.h>
#include <sys/resource.h>
#endif
#include "LogServer.h"
#include "PassGen.h"

#include "fseye/kipcstream.h"
#include "fseye/fseye_protocol.h"

KIpcStream      _gGuardIpcStream;

////////////////////////////////////////////////////////////////////////////////
LogServer *g_pLogServer = NULL;

const char KG_CONFIG_FILE_NAME[] = "logserver.ini";


////////////////////////////////////////////////////////////////////////////////
void OnQuitSignal(int)
{
    QLogPrintf(LOG_INFO, "[LogServer] Get quit signal!");
    if (g_pLogServer)
    {
        g_pLogServer->SetExitFlag(true);
    }
}

//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32

BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{
/*
CTRL_C_EVENT        CTRL+C signal was received
CTRL_BREAK_EVENT    CTRL+BREAK signal was received
CTRL_CLOSE_EVENT    the user closes the console (either by clicking Close on the console window's window menu, or by clicking the End Task button command from Task Manager). 
*/
    OnQuitSignal(0);
    
    return true;
}

static int _SetWin32ConsoleProperty()
{
    int nResult  = false;
    int nRetCode = false;
    HANDLE hStdin   = INVALID_HANDLE_VALUE;
    HANDLE hStdount = INVALID_HANDLE_VALUE;
    DWORD dwMode  = 0;
    COORD       BufferSize  = {120, 300};
    SMALL_RECT  WinRect     = {0, 0, BufferSize.X - 1, 27};
   
    hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
    LOG_PROCESS_ERROR(hStdin != INVALID_HANDLE_VALUE);

    hStdount = ::GetStdHandle(STD_OUTPUT_HANDLE);
    LOG_PROCESS_ERROR(hStdount != INVALID_HANDLE_VALUE);

    // disable quick edit mode of console. 
    // because quick edit maybe lock stdout
    nRetCode = ::GetConsoleMode(hStdin, &dwMode);
    LOG_PROCESS_ERROR(nRetCode);

    dwMode &= ~ENABLE_QUICK_EDIT_MODE;  //disable QUICK_EDIT MODE

    nRetCode = ::SetConsoleMode(hStdin, dwMode);
    LOG_PROCESS_ERROR(nRetCode);
    
    // set window size
    nRetCode = ::SetConsoleScreenBufferSize(hStdount, BufferSize);
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = ::SetConsoleWindowInfo(hStdount, true, &WinRect);
    LOG_PROCESS_ERROR(nRetCode);

    // set text color
    nRetCode = ::SetConsoleTextAttribute(hStdount, FOREGROUND_GREEN);
    LOG_PROCESS_ERROR(nRetCode);

    // get window close notify
    nRetCode = ::SetConsoleCtrlHandler(&ConsoleHandlerRoutine, TRUE);
    LOG_PROCESS_ERROR(nRetCode);

//EXIT1:
    nResult = true;
EXIT0: 
    return nResult;
}

#endif

////////////////////////////////////////////////////////////////////////////////
// @brief :Set runtime environment 
// @return : true if successful, false if failed.
static int _SetRuntimeEnvironment()
{
    int nResult  = false;
    int nRetCode = false;

#ifdef __GNUC__
    //rlimit rlimno = {5000, 5000};
    rlimit sLimit = {-1, -1};

    //nRetCode = setrlimit(RLIMIT_NOFILE, &rlimno);
    //LOG_PROCESS_ERROR(nRetCode == 0);

    // enable create coredump file
    nRetCode = setrlimit(RLIMIT_CORE, &sLimit);
    LOG_PROCESS_ERROR(nRetCode == 0);
#endif

    setlocale(LC_ALL, "");

#ifdef _WIN32
    _set_error_mode(_OUT_TO_MSGBOX);

    nRetCode = _SetWin32ConsoleProperty();
    LOG_PROCESS_ERROR(nRetCode);
#endif

//EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    int nResult  = false;
    int nRetCode = false;
    int nKGLogInitFlag    = false;
    int nKGMemoryInitFlag = false;
    int nSequenceInitFlag = false; 
    QLOG_PARAM LogParam  = {
        "logs",
        "LogServer", 
		"",
        (QLOG_OPTIONS)(LOG_OPTION_FILE | LOG_OPTION_CONSOLE), 
        10000 * 10
    };
    //KG_ServerController Controller;
    int nRunAsServerFlag = false;
	g_ExtractFileName(LogParam.szProcName, argv[0]);
    nRetCode = QLogInit(LogParam, NULL);
    PROCESS_ERROR(nRetCode);
    nKGLogInitFlag = true;

    nRetCode = QMemoryInit(NULL);
    LOG_PROCESS_ERROR(nRetCode);
    nKGMemoryInitFlag = true;

    // -------------------- initialize  -------------------------- 
    g_SetRootPath(argv[0]);
    nRetCode = _SetRuntimeEnvironment();
    PROCESS_ERROR(nRetCode);

    // the signal only has effect on Linux, windows just define it, do noting
    signal(SIGTERM, ::OnQuitSignal);  // when terminal by kill, call OnQuitSignal
    signal(SIGINT,  ::OnQuitSignal);  // when Ctrl+C, call OnQuitSignal

    //-----------------------------------------------------------
    g_pLogServer = new LogServer();
    PROCESS_ERROR(g_pLogServer);

    // -------------------- starting server  -------------------- 
    nRetCode = g_pLogServer->Init(argc, argv, NULL);
    LOG_PROCESS_ERROR(nRetCode);

    g_pLogServer->Run();

    // -------------------- ending server  ---------------------- 
    g_pLogServer->UnInit(NULL);

EXIT1:
    nResult = true;
EXIT0:
    if (nKGMemoryInitFlag)
    {
        QMemoryUnInit(NULL);
        nKGMemoryInitFlag = false;
    }

    if (nKGLogInitFlag)
    {
        QLogUnInit(NULL);
        nKGLogInitFlag = false;
    }
    
    SAFE_DELETE(g_pLogServer);
    
    return nResult ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
LogServer::LogServer() : m_nExitFlag(false)
{
    //ASSERT(m_pController);
}

LogServer::~LogServer(void)
{
    ASSERT(m_RequestQueue.Queue.empty());
    ASSERT(m_ResponseQueue.Queue.empty());
}

////////////////////////////////////////////////////////////////////////////////
int LogServer::Init(int argc, char *argv[], void *pvContext)
{
    int nResult  = false;
    int nRetCode = false;
    KG_SOCKET_THREAD_PARAM  SocketThreadParam;
    KG_LOG_THREAD_PARAM     LogThreadParam;
    int nLogThreadInitFlag = 0;
    int nSocketThreadInitFlag = false;
	int nAngelLoadedFlag = false;

    USE_ARGUMENT(pvContext);

    QLogPrintf(LOG_INFO, "[LogServer] starting ...\n");
    //m_pController->Printf("[LogServer] starting ...\n");

    nRetCode = _ReadConfigFile(argc, argv, &SocketThreadParam, &LogThreadParam);
    LOG_PROCESS_ERROR(nRetCode);

    QLogPrintf(LOG_INFO, "[LogServer] Init Socket thread ...\n");
    //m_pController->Printf("[LogServer] Init Socket thread ...\n");
    nRetCode = m_SocketThread.Init(SocketThreadParam, &m_RequestQueue, &m_ResponseQueue);
    LOG_PROCESS_ERROR(nRetCode);
    nSocketThreadInitFlag = true;

    QLogPrintf(LOG_INFO, "[LogServer] Init Log thread ...\n");
    //m_pController->Printf("[LogServer] Init Log thread ...\n");
    nRetCode = m_LogThread.Init(LogThreadParam, &m_RequestQueue, &m_ResponseQueue);
    LOG_PROCESS_ERROR(nRetCode);
    nLogThreadInitFlag = true;

	// ÁÙÊ±Ìí¼Ó¼à¿Ø [8/3/2011 simon]
	nRetCode = _gGuardIpcStream.Initialize(emKGUARDSERVER_FIRSTID + emKGUARDSERVER_LOGSERVER, 200 * 1024);
	LOG_PROCESS_ERROR(nRetCode);

	nResult = true;
EXIT0:
    if (!nResult)
    {
        if (nLogThreadInitFlag)
        {
            m_LogThread.UnInit();
            nLogThreadInitFlag = false;
        }

        if (nSocketThreadInitFlag)
        {
            m_SocketThread.UnInit();
            nSocketThreadInitFlag = false;
        }
	}

    QLogPrintf(LOG_INFO,"[LogServer] start %s\n", nResult ? "success" : "fail");
    //m_pController->Printf("[LogServer] start %s\n", nResult ? "success" : "fail");
    
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int LogServer::UnInit(void *pvContext)
{
    int nResult  = false;
    int nRetCode = false;
    unsigned i = 0;
    size_t uQueueSize = 0;
    unsigned uConnectionCount = 0;

    USE_ARGUMENT(pvContext);

    QLogPrintf(LOG_INFO, "[LogServer] stopping ...\n");
    //m_pController->Printf("[LogServer] stopping ...\n");

    m_SocketThread.EnableReceive(false);

    // --------------- wait request in the queue be process ---------------- 
    while (true)
    {
        uQueueSize = m_RequestQueue.Queue.size();
        if (uQueueSize == 0)
            break;
        
        nRetCode = m_LogThread.IsActive();
        if (!nRetCode)
            break;

        QLogPrintf(
            LOG_INFO, 
            "[LogServer] process received request, remainder %d ...\n", 
            uQueueSize
        );
        //m_pController->Printf(
        //    "[LogServer] process received request, remainder %d ...\n", 
        //    uQueueSize
        //);
        
        QThread_Sleep(1000);
    }

    // -----------------------  stop work threads -------------------------- 
    m_LogThread.UnInit();

    // ----------------  wait response in the queue be send ---------------- 
    while (true)
    {
        uQueueSize = m_ResponseQueue.Queue.size();
        if (uQueueSize == 0)
            break;

        uConnectionCount = m_SocketThread.GetConnectionCount();
        if (uConnectionCount == 0)
            break;

        QLogPrintf(
            LOG_INFO, "[LogServer] send response, remainder %d ...\n", 
            uQueueSize
        );
        //m_pController->Printf(
        //    "[LogServer] send response, remainder %d ...\n", 
        //    uQueueSize
        //);

        QThread_Sleep(1000);
    }

	_gGuardIpcStream.UnInitialize();
	//SAFE_RELEASE(m_piRoleProcessor); //  [11/11/2010 simon]

	//KG_AngelLib::UnInit(NULL); //  [11/11/2010 simon]

    // ---------------------------  close network -------------------------- 
    m_SocketThread.UnInit();

    nResult = true;
//EXIT0:
    QLogPrintf(LOG_INFO,"[LogServer] Stopped\n");
    //m_pController->Printf("[LogServer] Stopped\n");

    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int LogServer::Run()
{
    int nResult  = false;
    int nRetCode = false;
    int argc = 0;
    char **argv = NULL;
    
    while (!m_nExitFlag)
    {
        //nRetCode = m_pController->GetCommand(&argc, &argv);
        if (!nRetCode || (argc <= 1))
        {
            QThread_Sleep(100);

			ProcessKingEye();
            continue;
        }

        //ASSERT(argv);
        //ASSERT(argv[1]); // argv[0] always module name

        //if ((argc == 2) && (strcmp(argv[1], "start") == 0))
        //{
        //    m_pController->Printf("Server already running\n");
        //}
        //else if ((argc == 2) && (strcmp(argv[1], "stop") == 0))
        //{
        //    QLogPrintf(LOG_INFO, "[LogServer] Service is terminated by administrator !");
        //    SetExitFlag(true);
        //}
        //else if ((argc == 2) && (strcmp(argv[1], "status") == 0))
        //{
        //    nRetCode = _OnCmd_Status();
        //    //PROCESS_ERROR(nRetCode);
        //}
        //else if (
        //    (argc == 3) &&
        //    (strcmp(argv[1], "set") == 0) &&
        //    (strcmp(argv[2], "--log-level=debug") == 0)
        //)
        //{
        //    nRetCode = _OnCmd_SetLogLevel(KGLOG_DEBUG);
        //    //PROCESS_ERROR(nRetCode);
        //}
        //else if (
        //    (argc == 3) &&
        //    (strcmp(argv[1], "set") == 0) &&
        //    (strcmp(argv[2], "--log-level=info") == 0)
        //)
        //{
        //    nRetCode = _OnCmd_SetLogLevel(LOG_INFO);
        //    //PROCESS_ERROR(nRetCode);
        //}
        //else if (
        //    (argc == 3) &&
        //    (strcmp(argv[1], "set") == 0) &&
        //    (strcmp(argv[2], "--log-level=warning") == 0)
        //)
        //{
        //    nRetCode = _OnCmd_SetLogLevel(LOG_WARNING);            
        //    //PROCESS_ERROR(nRetCode);
        //}
        //else if (
        //    (argc == 3) &&
        //    (strcmp(argv[1], "set") == 0) &&
        //    (strcmp(argv[2], "--log-level=error") == 0)
        //)
        //{
        //    nRetCode = _OnCmd_SetLogLevel(LOG_ERR);
        //    //PROCESS_ERROR(nRetCode);
        //}
        //else
        //{
        //    nRetCode = _OnCmd_ShowUsage();
        //    //PROCESS_ERROR(nRetCode);
        //}
    }

    nResult = true;
//EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int LogServer::_ReadConfigFile(
    int argc, char *argv[], 
    KG_SOCKET_THREAD_PARAM *pRetSocketThreadParam,
    KG_LOG_THREAD_PARAM    *pRetLogThreadParam
)
{
    int nResult  = false;
    int nRetCode = false;
    int nValue   = 0;
    char szPassword[PG_MAXBUFFER];
    IIniFile *piIniFile = NULL;

    LOG_PROCESS_ERROR(pRetSocketThreadParam);
    LOG_PROCESS_ERROR(pRetLogThreadParam);

    QLogPrintf(LOG_INFO, "[LogServer] Loading %s", KG_CONFIG_FILE_NAME);

    piIniFile = g_LoadStartArgument(argc, argv, KG_CONFIG_FILE_NAME);
    LOG_PROCESS_ERROR(piIniFile);
    
    // -------------------------- Server Manager Param -------------------------
    nRetCode = piIniFile->GetString(
        "LogServer", 
        "ListenIP", 
        "0.0.0.0", 
        pRetSocketThreadParam->szListenIP,
        sizeof(pRetSocketThreadParam->szListenIP)
    );
    LOG_PROCESS_ERROR(nRetCode && "Unable load [LogServer]:ListenIP");
    
    nRetCode = piIniFile->GetInteger(
        "LogServer", "ListenPort", 5100, &pRetSocketThreadParam->nListenPort
    );
    LOG_PROCESS_ERROR(nRetCode && "Unable load [LogServer]:ListenPort");
    
    nRetCode = piIniFile->GetInteger(
        "LogServer", "AutoDisconnectTime", 120, &nValue
    );
    LOG_PROCESS_ERROR(nRetCode && "Unable load [LogServer]:AutoDisconnectTime");
    LOG_PROCESS_ERROR((nValue > 0) && "AutoDisconnectTime must > 0");
    pRetSocketThreadParam->nAutoDisconnectTime = nValue;

    // --------------------------- Log thread param ---------------------------
    nRetCode = piIniFile->GetInteger(
        "LogServer",
        "MinItemValue",
        0,
        &nValue
    );
    LOG_PROCESS_ERROR(nRetCode && "Unable load [LogServer]:MinItemValue");
    LOG_PROCESS_ERROR((nValue >= 0) && "MinItemValue must >= 0");
    pRetLogThreadParam->uMinItemValue = (unsigned)nValue;

    nRetCode = piIniFile->GetInteger(
        "LogServer",
        "MinPlayerActionLevel",
        0,
        &nValue
    );
    pRetLogThreadParam->uMinPlayerActionLevel = (unsigned)nValue;

	nRetCode = piIniFile->GetInteger(
		"LogServer",
		"MinDataAnalysisLevel",
		0,
		&nValue
		);
	pRetLogThreadParam->uMinDataAnalysisLevel = (unsigned)nValue;

    nRetCode = piIniFile->GetInteger(
        "LogServer",
        "ExpiredDay",
        0,
        &nValue
    );
    LOG_PROCESS_ERROR(nRetCode && "Unable load [LogServer]:ExpiredDay");
    LOG_PROCESS_ERROR((nValue > 0) && "ExpiredDay must > 0");
    pRetLogThreadParam->nExpiredDay = nValue;

    nRetCode = piIniFile->GetString(
        "LogDatabase", 
        "server", 
        "127.0.0.1", 
        pRetLogThreadParam->DatabaseParam.szServer,
        sizeof(pRetLogThreadParam->DatabaseParam.szServer)
    );
    LOG_PROCESS_ERROR(nRetCode && "Unable load [LogDatabase]:server");
    
    nRetCode = piIniFile->GetString(
        "LogDatabase", 
        "username", 
        "logserver", 
        pRetLogThreadParam->DatabaseParam.szUsername,
        sizeof(pRetLogThreadParam->DatabaseParam.szUsername)
    );
    LOG_PROCESS_ERROR(nRetCode && "Unable load [LogDatabase]:username");  

    nRetCode = piIniFile->GetString(
        "LogDatabase", 
        "password", 
        "", 
        szPassword,
        sizeof(szPassword)
    );
    LOG_PROCESS_ERROR(nRetCode && "Unable load [LogDatabase]:password");

    nRetCode = (int)strlen(szPassword);
    //LOG_PROCESS_ERROR((nRetCode == PG_RESULTLENSTD) && "Password error");
	memcpy(pRetLogThreadParam->DatabaseParam.szPassword,szPassword,sizeof(szPassword));
    //nRetCode = SimplyDecryptPassword(pRetLogThreadParam->DatabaseParam.szPassword, szPassword);
    LOG_PROCESS_ERROR(nRetCode && "Password error");
    
    nRetCode = piIniFile->GetString(
        "LogDatabase", 
        "database", 
        "cslog", 
        pRetLogThreadParam->DatabaseParam.szDatabase,
        sizeof(pRetLogThreadParam->DatabaseParam.szDatabase)
    );
    LOG_PROCESS_ERROR(nRetCode && "Unable load [LogDatabase]:database");  

    nResult = true;
EXIT0:
    SAFE_RELEASE(piIniFile);
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int LogServer::_OnCmd_SetLogLevel(QLOG_LEVEL eLogLevel)
{
    int nResult  = false;
    int nRetCode = false;
    const char *pszResultMsg = "[LogServer] Set log level to unsupported type";

    switch (eLogLevel) 
    {
    case LOG_DEBUG:
        pszResultMsg = "[LogServer] Set log level to DEBUG";
    	break;
    case LOG_INFO:
        pszResultMsg = "[LogServer] Set log level to INFO";
        break;
    case LOG_WARNING:
        pszResultMsg = "[LogServer] Set log level to WARNING";
        break;
    case LOG_ERR:
        pszResultMsg = "[LogServer] Set log level to ERROR";
        break;
    default:
        PROCESS_ERROR(false);
        break;
    }

    nRetCode = QLogSetPriorityMask(QLOG_UPTO(eLogLevel));
    LOG_PROCESS_ERROR(nRetCode);

//EXIT1:
    nResult = true;
EXIT0:
    if (nResult)
    {
        QLogPrintf(LOG_INFO, "%s, success\n", pszResultMsg);
        //m_pController->Printf("%s, success\n", pszResultMsg);
    }
    else
    {
        QLogPrintf(LOG_INFO, "%s, failed\n", pszResultMsg);
        //m_pController->Printf("%s, failed\n", pszResultMsg);
    }

    return nResult;
}

//////////////////////////////////////////////////////////////////////////
int LogServer::_OnCmd_Status()
{
    int nResult  = false;
    int nRetCode = false;
    std::string sStatus;
    
    nRetCode = m_SocketThread.GetStatus(&sStatus);
    LOG_PROCESS_ERROR(nRetCode);
    //m_pController->Printf("%s", sStatus.c_str());

    nRetCode = m_LogThread.GetStatus(&sStatus);
    LOG_PROCESS_ERROR(nRetCode);
    //m_pController->Printf("%s", sStatus.c_str());
    
//EXIT1:
    nResult = true;
EXIT0:
    if (!nResult)
    {
        //m_pController->Printf("Get status failed\n");
    }
    return nResult;
}

//////////////////////////////////////////////////////////////////////////
//int LogServer::_OnCmd_ShowUsage()
//{
//    return m_pController->Printf("%s", USAGE_MESSAGE);
//}

void LogServer::ProcessKingEye()
{
	size_t nLen = 0;
	CHAR* pData = _gGuardIpcStream.GetPack(nLen);
	while (pData)
	{
		ASSERT(nLen > 0);
		BYTE byProtocol = *(BYTE*)pData;
		if (byProtocol == e2l_getpid_def)
		{
			l2e_getpid sMsg;
			sMsg.wProtocol = l2e_getpid_def;
			sMsg.wServer   = 0;
			sMsg.nPid	   = getpid();
			_gGuardIpcStream.PutPack(&sMsg, sizeof(sMsg));
		}
		else if (byProtocol == g2l_header_def)
		{
			BYTE bySubProto = *(BYTE*)pData;
			KSubProtocolHead* pHead = (KSubProtocolHead*)pData;
			if (nLen == sizeof(KSubProtocolHead) &&
				pHead->wSubProtocol == emKPROTOCOL_G2L_PIDREQ)
			{
				KPROTOCOL_L2G_PIDRET sRet;
				sRet.wProtocol		= l2g_header_def;
				sRet.wSubProtocol	= emKPROTOCOL_L2G_PIDRET;
				sRet.nPid			= getpid();
				sRet.wServer		= 0;
				_gGuardIpcStream.PutPack(&sRet, sizeof(sRet));
			}
		}
		else
		{
			//ProcessMessage(pData, nLen);
		}
		_gGuardIpcStream.PopPack();
		pData = _gGuardIpcStream.GetPack(nLen);
	}
	time_t tNow = time(NULL);
	static time_t tLastPing = 0;
	if (tNow - tLastPing > 20)
	{
		KSubProtocolHead sPing;
		sPing.wProtocol		= l2e_header_def;
		sPing.wSubProtocol	= l2e_ping_def;
		sPing.wServer		= 0;
		_gGuardIpcStream.PutPack(&sPing, sizeof(sPing));

		tLastPing = tNow;
	}
	static time_t tLastSendCustomInfo = 0;
	if (tNow - tLastSendCustomInfo > 10)
	{
		SendCustomInfo2KingEye();
		tLastSendCustomInfo = tNow;
	}
}

#define _MAX_CUSTOM_INFO_BUFFER 64000
int LogServer::SendCustomInfo2KingEye()
{
	int nResult = 0;
	l2e_update_custom_info* pInfo = NULL;
	size_t uPackLen = 0;
	CHAR szBuffer[_MAX_CUSTOM_INFO_BUFFER] = "";

	std::string sSockStatus;
	std::string sLogStatus;
	std::string strContent;

	nResult = m_SocketThread.GetStatus(&sSockStatus);
	nResult = m_LogThread.GetStatus(&sLogStatus);

	strContent += "Network ---------------------------\n";
	strContent += sSockStatus;
	strContent += "LogThread -------------------------\n";
	strContent += sLogStatus;

	uPackLen = sizeof(l2e_update_custom_info) - 1 + strContent.size();
	LOG_PROCESS_ERROR(uPackLen < sizeof(szBuffer));

	pInfo = (l2e_update_custom_info*)szBuffer;
	pInfo->wProtocol = l2e_header_def;
	pInfo->wSubProtocol = l2e_update_custom_info_def;
	pInfo->wServer = 0;
	pInfo->uDataLen = strContent.size() + 1;
	QStrCpy((char*)pInfo->byData, strContent.c_str());

	_gGuardIpcStream.PutPack(pInfo, uPackLen);

	nResult = 1;
EXIT0:
	return nResult;
}
