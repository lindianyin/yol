#include "stdafx.h"
#include <signal.h>
#include "Gateway.h"
#include <stdlib.h>
#ifdef __unix
#include <sys/time.h>
#include <sys/resource.h>
#endif // __unix

static QGateway* g_pGateway = NULL;

#ifdef WIN32
BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{
	if (g_pGateway)
	{
		// bugfix: async-signal-safe function [ISO/IEC 9945:2008]
		//QLogPrintf(LOG_INFO, "Receive quit signal !\n");
		g_pGateway->Quit();
	}
	return true;
}

void SetConsoleInfo()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	SetConsoleCtrlHandler(&ConsoleHandlerRoutine, true);
}
#else

void OnQuitSignal(int nSignal)
{
	if (g_pGateway)
	{
		// bugfix: async-signal-safe function [ISO/IEC 9945:2008]
		//QLogPrintf(LOG_INFO, "Receive quit signal !\n");
		g_pGateway->Quit();
	}
}

BOOL SetEnv()
{
	BOOL    bResult   = false;
	int     nRetCode  = 0;
	//int     nNullFile = -1;
	rlimit  ResLimit;

	//ResLimit.rlim_cur = USHRT_MAX;
	//ResLimit.rlim_max = USHRT_MAX;

	//nRetCode = setrlimit(RLIMIT_NOFILE, &ResLimit);
	//LOG_PROCESS_ERROR(nRetCode == 0 && "PLEASE RUN AS ROOT!");

	ResLimit.rlim_cur = -1;
	ResLimit.rlim_max = -1;

	nRetCode = setrlimit(RLIMIT_CORE, &ResLimit);
	LOG_PROCESS_ERROR(nRetCode == 0 && "PLEASE RUN AS ROOT!");

	signal(SIGINT, OnQuitSignal);
	signal(SIGQUIT, OnQuitSignal);
	signal(SIGTERM, OnQuitSignal); 

	//nNullFile = open("/dev/null", O_WRONLY);
	//LOG_PROCESS_ERROR(nNullFile != -1);

	//dup2(nNullFile, STDIN_FILENO);
	//dup2(nNullFile, STDOUT_FILENO);
	//dup2(nNullFile, STDERR_FILENO);

	bResult = true;
EXIT0:
	//if (nNullFile != -1)
	//{
	//	close(nNullFile);
	//	nNullFile = -1;
	//}
	return bResult;
}
#endif

int main(int argc, char* argv[])
{
	int             nRetCode            = false;
	BOOL            bLogInitFlag        = false;
	BOOL            bMemInitFlag        = false;
	BOOL            bGatewayInitFlag    = false;
	char*           pszRetCWD           = NULL;
	QLOG_PARAM     LogParam;
	char            szCWD[MAX_PATH];

	g_SetRootPath(argv[0]);

#ifdef WIN32
	SetConsoleInfo();
#endif

	setlocale(LC_ALL, "");
	srand((unsigned int)time(NULL));

	pszRetCWD = getcwd(szCWD, sizeof(szCWD));    
	LOG_PROCESS_ERROR(pszRetCWD);

	memset(&LogParam, 0, sizeof(LogParam));

	nRetCode = snprintf(LogParam.szPath, sizeof(LogParam.szPath), "%s/logs", szCWD);
	LOG_PROCESS_ERROR(nRetCode >= 0);
	LOG_PROCESS_ERROR(nRetCode < sizeof(LogParam.szPath));

	strncpy(LogParam.szIdent, "gateway", sizeof(LogParam.szIdent));
	LogParam.szIdent[sizeof(LogParam.szIdent) - 1] = '\0';

	LogParam.nMaxLineEachFile   = 65536;
	LogParam.Options = (QLOG_OPTIONS)(LOG_OPTION_FILE | LOG_OPTION_CONSOLE);
	g_ExtractFileName(LogParam.szProcName, argv[0]);
	nRetCode = QLogInit(LogParam, NULL);
	LOG_PROCESS_ERROR(nRetCode);
	bLogInitFlag = true;

	nRetCode = QMemoryInit(NULL);
	LOG_PROCESS_ERROR(nRetCode);
	bMemInitFlag = true;

	QLogPrintf(LOG_INFO, "%s\n", "YOL Gateway v2.1 build " __TIME__ " " __DATE__);

#ifdef unix
	nRetCode = SetEnv();
	LOG_PROCESS_ERROR(nRetCode);
#endif

	g_pGateway = new QGateway;
	LOG_PROCESS_ERROR(g_pGateway);

	nRetCode = g_pGateway->Init();
	LOG_PROCESS_ERROR(nRetCode);
	bGatewayInitFlag = true;

	QLogPrintf(LOG_INFO, "Gateway Startup Complete!\n");

	g_pGateway->Run();

EXIT0:
	if (bMemInitFlag)
	{
		QMemoryUnInit(NULL);
		bMemInitFlag = false;
	}

	if (bLogInitFlag)
	{
		QLogUnInit(NULL);
		bLogInitFlag = false;
	}

	if (bGatewayInitFlag)
	{
		g_pGateway->UnInit();
		bGatewayInitFlag = false;
	}

	SAFE_DELETE(g_pGateway);
	return 0;
}
