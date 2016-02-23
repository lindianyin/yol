// gamecenter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gameserver.h"
#include "onlinegameworld/ikgameserver.h"

#include "Misc/utilities.h"
#include "onlinegamemodule/kmoduledef.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kprotocol.h"
#include "config/knsbaserequire.h"
#include "onlinegamemodule/kscripthelper.h"
#include "config/kconfigfile.h"
#include "onlinegamebase/performancestat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LPCSTR _GetTimeString()
{
	int nRetCode   = 0;
	tm LocalTime   = {0};
	time_t curtime = 0;
	static CHAR szTimeString[MAX_PATH] = {0};

	time(&curtime);  // get current time
	localtime_r(&curtime, &LocalTime);
	nRetCode = (int)strftime(szTimeString, sizeof(szTimeString), "%Y-%m-%d %H:%M:%S", &LocalTime);
	
	ASSERT(nRetCode);

	return szTimeString;
}
// -------------------------------------------------------------------------

BOOL KGameServerApp::Init()
{
	// 输出服务器版本等
	QLogPrintf(LOG_DEBUG,
		"YOL GameServer Built on %s %s"
		"\n_______________________________________________________________________________",
		__TIME__, __DATE__);

	QLogPrintf(LOG_DEBUG, "Protocol %d - %d", KD_PROTOCOL_VERSION, KD_PROTOCOL_VERSION_LOWER_LIMIT);

	{
		static QLogFile s_ScriptLogFile; // 有些网络模块要在逻辑初始化之前写脚本日志，放在最外面先初始化
		BOOL bRet = s_ScriptLogFile.InitialLogFile(KDF_LOG_DIR_ROOT, KDF_SCRIPT_LOG_DIR);
		QCONFIRM_RET_FALSE(bRet && "Init ScriptLog Failed");
		extern QLogFile* g_pScriptLogFile;
		g_pScriptLogFile = &s_ScriptLogFile;
	}

	g_SetPackageVersion(1);			// 使用新的pak格式
	//// 注意顺序是倒着优先，越后面的越优先
	//if (!KGPack_LoadPackageFiles(KDF_PACKAGE_FILE, KGPACK_FILE_PRIORITY_EXTERNAL_FILE_FIRST))
	//{
	//	QLogPrintf(LOG_ERR, "LoadPackageFile '%s' failed!", KDF_PACKAGE_FILE);
	//}

	IKGameServer* piGsInterface = g_GetGameServerInterface();
	QCONFIRM_RET_FALSE(piGsInterface);

	Q_Printl("Script Helper Initializing...");
	g_cScriptHelper.InitScript();

	return piGsInterface->Init(g_InitNsBaseRequire());
}

BOOL KGameServerApp::Uninit()
{
	IKGameServer* piGsInterface = g_GetGameServerInterface();
	QCONFIRM_RET_FALSE(piGsInterface);
	Q_Printl("GameServer UnInit");
	BOOL bResult = piGsInterface->UnInit();
	g_cScriptHelper.Uninit();
	//KGPack_ClearPackageFiles();			// 使用新的pak格式
	return bResult;
}

BOOL KGameServerApp::Run()
{
	IKGameServer* piGsInterface = g_GetGameServerInterface();
	QCONFIRM_RET_FALSE(piGsInterface);

	Q_Printl("Starting Network...");
	Q_Printl("Starting Network...");
	BOOL bRet = piGsInterface->StartUp();
	QCONFIRM_RET_FALSE(bRet);

	Q_Printl("\n_______________________________________________________________________________" <<
		"\nLocal Time " << _GetTimeString() << ": GameServer Ready." <<
		"\n_______________________________________________________________________________");

	g_pStat = new QPerformanceStat();
	g_pStat->SetName("runtime_cost", "game_server");

	while(TRUE)
	{
		BEGIN_STAT("KGameServerApp::Run()");
		if (!piGsInterface->Loop())
			break;
		END_STAT();
		// 移到piGsInterface->Loop里方便控制
		//QThread_Sleep(1);
	}

	g_pStat->Output(true);
	delete g_pStat;
	g_pStat = NULL;

	return TRUE;
}
