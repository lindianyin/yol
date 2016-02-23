

#include "stdafx.h"
#include "Misc/utilities.h"
#include "gameserver.h"
#include "onlinegameworld/ikgameserver.h"
#include <signal.h>
#ifdef __unix
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif // __unix

#include "onlinegamebase/kscriptmanager.h"
#include <config/kconfigfile.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// -------------------------------------------------------------------------
#ifdef WIN32
BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{
	// bugfix: 信号处理函数最好不调系统和库函数，只操作sig_atomic_t类型的数据
	// async-signal-safe function [ISO/IEC 9945:2008]
	// https://www.securecoding.cert.org/confluence/display/seccode/11.+Signals+%28SIG%29
	//QLogPrintf(LOG_INFO, "Received signal %d! Shutdown pending...", dwCtrlType);
	IKGameServer* piGameServer = g_GetGameServerInterface();
	piGameServer->ShutDown();
	return TRUE;
}
#endif

static void _SignalHandler(int nSignal)
{
	// bugfix: 信号处理函数最好不调系统和库函数，只操作sig_atomic_t类型的数据
	// async-signal-safe function [ISO/IEC 9945:2008]
	// https://www.securecoding.cert.org/confluence/display/seccode/11.+Signals+%28SIG%29
	// QLogPrintf(LOG_INFO, "Received signal %d! Shutdown pending...", nSignal);
	IKGameServer* piGameServer = g_GetGameServerInterface();
	piGameServer->ShutDown();
}

void StartLuaProfiler() 
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	lua_State* pLvm = cSafeScript->GetLuaState();
	// lua profiler
	extern int luaopen_profiler(lua_State *L);
	extern int prof_start(lua_State *L);
	QLogPrintf(LOG_DEBUG, "Starting Lua Profiler...");
	luaopen_profiler(pLvm);
	prof_start(pLvm);
}

void StopLuaProfiler() 
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	lua_State* pLvm = cSafeScript->GetLuaState();

	std::stringstream stmBuffer;
	// lua profiler
	extern int prof_stop(lua_State *L);
	extern int prof_dump_buffer(lua_State *L, std::stringstream& stmBuffer);

	prof_stop(pLvm);
	prof_dump_buffer(pLvm, stmBuffer);

	FILE* pFile = fopen(KDF_PROFILING_LOG, "a");
	if (pFile)
	{
		tm LocalTime   = {0};
		time_t curtime = 0;
		static CHAR szTimeString[MAX_PATH] = {0};

		time(&curtime);  // get current time
		localtime_r(&curtime, &LocalTime);
		strftime(szTimeString, sizeof(szTimeString), "%Y-%m-%d %H:%M:%S", &LocalTime);

		fputs(szTimeString, pFile);
		fputs("\n-------------------------------------------------\n", pFile);
		fputs("Lua Profiling:\n", pFile);
		fputs(stmBuffer.str().c_str(), pFile);
		fputs("\n", pFile);
		fclose(pFile);
	}
}

INT main(INT argc, LPSTR argv[])
{
	HMODULE		hDumper = NULL;

	g_SetRootPath(argv[0]);

	// 初始化Log
	QLOG_PARAM LogParam;
	CHAR        szCWD[MAX_PATH] = ".";

	getcwd(szCWD, sizeof(szCWD));
	memset(&LogParam, 0, sizeof(LogParam));
	snprintf(LogParam.szPath, sizeof(LogParam.szPath) - 1, "%s/logs", szCWD);
	QStrCpyLen(LogParam.szIdent, "gameserver", sizeof(LogParam.szIdent));
	LogParam.nMaxLineEachFile = 65536 * 2;
	LogParam.Options = (QLOG_OPTIONS)(LOG_OPTION_FILE | LOG_OPTION_CONSOLE);
	g_ExtractFileName(LogParam.szProcName, argv[0]);
	QLogInit(LogParam, NULL);
	
#ifndef WIN32
	// 允许产生coredump文件
	rlimit sLimit;
	sLimit.rlim_cur = -1;
	sLimit.rlim_max = -1;
	::setrlimit(RLIMIT_CORE, &sLimit);

	// 设置创建的新文件权限为(rw.r.r)
	umask(022);

#else
	#ifdef _DEBUG
		// 允许控制台右键菜单
		DWORD dwConsoleMode = 0;
		GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &dwConsoleMode);
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ~ENABLE_MOUSE_INPUT & dwConsoleMode);

		SetConsoleCtrlHandler(&ConsoleHandlerRoutine, true);

		hDumper = LoadLibraryA("dumperd.dll");
	#else
		hDumper = LoadLibraryA("dumper.dll");
	#endif
#endif

	setlocale(LC_ALL, "");
	srand((UINT)time(NULL));
	g_RandomSeed((INT)time(NULL));

	KGameServerApp cApp;
	BOOL bNormalExit = TRUE;

	if (!cApp.Init())
	{
		bNormalExit = FALSE;
	}
	else
	{
		// only works for linux
		signal(SIGTERM, _SignalHandler);  // when terminate signal received
		signal(SIGINT,  _SignalHandler);  // when Ctrl+C

		//StartLuaProfiler();

		if (!cApp.Run())
			bNormalExit = FALSE;

		//StopLuaProfiler();
	}
	if (!cApp.Uninit())
		bNormalExit = FALSE;	

	Q_Printl("\n_______________________________________________________________________________" <<
		"\nGameServer Closed.");

#ifdef WIN32
	if (hDumper)
	{
		FreeLibrary(hDumper);
		hDumper = NULL;
	}
#endif // WIN32

	return !bNormalExit;
}

// -------------------------------------------------------------------------
