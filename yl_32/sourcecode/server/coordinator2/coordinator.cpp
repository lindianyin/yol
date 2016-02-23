
#include "stdafx.h"
#include <signal.h>
#include "GameCenter.h"
#include "Misc/qconsolereader.h"
#include "serverbase/RankDef.h"

#ifdef unix
#include <sys/time.h>
#include <sys/resource.h>
#endif // unix


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

const static UINT g_nElapsePerFrame = 1000 / GAME_FPS;
static QConsoleReader s_ConsoleReader;

QGameCenter* g_pGameCenter = NULL;

// -------------------------------------------------------------------------

static void _SignalHandler(int nSignal)
{
	// async-signal-safe function [ISO/IEC 9945:2008]
	QLogPrintf(LOG_INFO, "Received signal %d! Shutdown pending...", nSignal);

	if (g_pGameCenter)
	{
		g_pGameCenter->Quit();
	}
}

#ifdef WIN32
static BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{
	_SignalHandler(dwCtrlType);
	return FALSE;
}
#endif





int main(int argc, char* argv[])
{
	INT				nRet	= 0;
	INT				nLogInitFlag = FALSE;
	HMODULE			hDumper = NULL;
	QIniFile tiIniFile;

	g_SetRootPath(argv[0]);

	{
		QLOG_PARAM sLogParam;
		LPSTR       pszRetCWD    = NULL;
		CHAR        szCWD[MAX_PATH];

		pszRetCWD = getcwd(szCWD, sizeof(szCWD));

		memset(&sLogParam, 0, sizeof(sLogParam)); 
		snprintf(sLogParam.szPath, sizeof(sLogParam.szPath), "%s/logs", szCWD);
		QStrCpyLen(sLogParam.szIdent, "coordinator", sizeof(sLogParam.szIdent));
		sLogParam.nMaxLineEachFile	= USHRT_MAX;
		sLogParam.Options = (QLOG_OPTIONS)(LOG_OPTION_FILE | LOG_OPTION_CONSOLE);
		g_ExtractFileName(sLogParam.szProcName, argv[0]);
		QLogInit(sLogParam, NULL);
		nLogInitFlag = TRUE;
	}
	
	QLogPrintf(LOG_INFO, "%s\n", "YOL Coordinator build " __TIME__ " " __DATE__);

#ifdef unix
	rlimit sLimit;
	sLimit.rlim_cur = -1;
	sLimit.rlim_max = -1;
	::setrlimit(RLIMIT_CORE, &sLimit);

	umask(022);

	//signals only affect Linux/Unix, not Windows
	signal(SIGTERM, _SignalHandler);  // when terminal by kill, call OnQuitSignal
	signal(SIGINT,  _SignalHandler);  // when Ctrl+C, call OnQuitSignal
	signal(SIGQUIT, _SignalHandler);
#else
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SetConsoleCtrlHandler(ConsoleHandlerRoutine, TRUE);

	DWORD dwConsoleMode = 0;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &dwConsoleMode);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ~ENABLE_MOUSE_INPUT & dwConsoleMode);

#ifdef _DEBUG
	hDumper = LoadLibraryA("dumperd.dll");
#else
	hDumper = LoadLibraryA("dumper.dll");
#endif // _DEBUG

#endif // unix

	g_pGameCenter = new QGameCenter();
	LOG_PROCESS_ERROR(g_pGameCenter);

	if (argc >= 2 && strcmp(argv[1], "devmode") == 0)
	{
		QConsoleHelper::DoInfoColor();
		QLogPrintf(LOG_INFO, "Detected DevMode Flag");
		QConsoleHelper::RestoreColor();

		g_pGameCenter->m_bDevMode = TRUE;
	}

	nRet = g_pGameCenter->Init();
	LOG_PROCESS_ERROR(nRet);
	g_pGameCenter->Run();
	nRet = TRUE;
EXIT0:
	g_pGameCenter->UnInit();

	QLogPrintf(LOG_INFO, "Coordinator Exiting.");

	if (nLogInitFlag)
	{
		QLogUnInit(NULL);
	}

	return nRet ? 0 : 1;
}

