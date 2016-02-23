#ifdef GAME_SERVER

#include "stdafx.h"
#include "config/kconfigfile.h"
#include "onlinegamebase/ksysservice.h"

#include "onlinegameworld/ikgameserver.h"
#include "ksocketinterface.h"

#include "onlinegameworld/krequireinterface.h"
#include "onlinegameworld/korpgworld.h"
#include "knetconnectplayer.h"
#include "kgamecenterprocess.h"
#include "kplayerbaseprocess.h"
#include "kplayerdbmanager.h"
#include "kmodulemanager.h"
#include "kgameservereyes.h"
#include "Misc/qconsolereader.h"
#include <signal.h>
#include "KRelayClient.h"
#include "onlinegamebase/performancestat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IKNsBaseRequire* g_piNsBaseRequire = NULL;
// -------------------------------------------------------------------------
class KGameServer : public IKGameServer
{
public:
	BOOL Init(IKNsBaseRequire* piNsBaseRequire);
	BOOL StartUp();
	BOOL ShutDown() { m_bRunning = FALSE; return TRUE; }
	BOOL Loop();
	BOOL UnInit();

	void DumpStatics();

	BOOL DoCommand(LPCSTR szCommand);

private:
	BOOL	_InitEnv();
	BOOL	_LoadMiniServerScene();

private:
	CHAR		m_szGameCenterIp[KD_IPADDRESS_LEN];
	INT			m_nGameCenterPort;
	volatile sig_atomic_t m_bRunning;
	QConsoleReader m_cConsoleReader;
};


IKGameServer* g_GetGameServerInterface()
{
	static KGameServer s_gGameServer;
	return &s_gGameServer;
}

// -------------------------------------------------------------------------
BOOL KGameServer::Init(IKNsBaseRequire* piNsBaseRequire)
{
	BOOL bRet = FALSE;
	m_bRunning = FALSE;
	QCONFIRM_RET_FALSE(g_UnifyInit());

	g_piNsBaseRequire = piNsBaseRequire;

	bRet = _InitEnv();
	QCONFIRM_RET_FALSE(bRet && "KGameServer::_InitEnv");

	m_cConsoleReader.Init();

	bRet = g_RelayClient.Init();
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cGameCenterProcess.Init(0);
	QCONFIRM_RET_FALSE(bRet);

	// 初始化Socket服务器
	bRet = g_piSocketServer->Init(&g_cNetConnectPlayer);
	QCONFIRM_RET_FALSE(bRet);
	
	bRet = g_cModuleManager.PreInit();
	QCONFIRM_RET_FALSE(bRet);

	Q_Printl("Game World Initializing...");

	bRet = g_cOrpgWorld.Init();
	QCONFIRM_RET_FALSE(bRet);

	//_LoadMiniServerScene(); // 世界初始化完毕后才可以

	bRet = g_cModuleManager.Init();
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cPlayerDbMgr.Init(); // 玩家存档管理器
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cGameserverEyes.Init(emKGUARDSERVER_FIRSTID + emKGUARDSERVER_GAMESERVER01 +
		g_cOrpgWorld.m_nServerId - 1);
	QCONFIRM_RET_FALSE(bRet);

	m_bRunning = bRet;
	return bRet;
}

BOOL KGameServer::StartUp()
{
	BOOL bRet = FALSE;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	bRet = g_piSocketServer->SocketOpen();
	QCONFIRM_RET_FALSE(bRet);

	bRet = cSafeScript->CallTableFunction("ServerEvent", "OnServerReady", 0, "");
	QCONFIRM_RET_FALSE(bRet);
	return TRUE;
}

BOOL KGameServer::Loop()
{
	if (!m_bRunning)
	{
		QLogPrintf(LOG_INFO, "Shutting down...");
		return FALSE;
	}

	//PCSTR pszConsoleInput = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript(); // 自动处理栈泄露

	BEGIN_STAT("KGameServer.Loop.Begin()");
	//pszConsoleInput = m_cConsoleReader.ReadInput();
	//if (pszConsoleInput)
	//	DoCommand(pszConsoleInput);
	END_STAT();

	BEGIN_STAT("KGameServer.Loop.g_UnifyLoop()");
	g_UnifyLoop();
	END_STAT();

	BEGIN_STAT("KGameServer.Loop.ProcessPackage()");
	g_RelayClient.ProcessPackage();
	END_STAT();

	BEGIN_STAT("KGameServer.Loop.MessageLoop()");
	g_piSocketServer->MessageLoop();
	END_STAT();

	static INT nIdleFrame = 0;
	static UINT uTickStart = KSysService::GetTickCount();
	UINT uTickNow = KSysService::GetTickCount();
	DWORD dwFrames = (DWORD)((ULONGLONG)(uTickNow - uTickStart) * GAME_FPS / 1000);
	DWORD dwFramesDiff = dwFrames - g_cOrpgWorld.m_dwLogicFrames;

	if (dwFramesDiff)
	{
		if (dwFramesDiff > GAME_FPS / 4)
			Q_Printl("Warning: MilliSecond Between Two Frames: " << dwFramesDiff * 1000 / GAME_FPS);

		BEGIN_STAT("KGameServer.Loop.g_RelayClient.Activate()");
		g_RelayClient.Activate();
		END_STAT();

		BEGIN_STAT("KGameServer.Loop.g_cGameserverEyes.Active()");
		g_cGameserverEyes.Active();
		END_STAT();

		g_cOrpgWorld.m_nIdleFrame = nIdleFrame;
		nIdleFrame = 0;
	}
	else
	{
		nIdleFrame++;
		BEGIN_STAT("KGameServer.Loop.QThread_Sleep()");
		QThread_Sleep(1);
		//QThread_Sleep(10);
		END_STAT();
	}

	/*这里会导致堵死消息*/
    while (dwFrames > g_cOrpgWorld.m_dwLogicFrames)
	{
		BEGIN_STAT("KGameServer.Loop.g_cOrpgWorld.Activate()");
		g_cOrpgWorld.Activate();
		END_STAT();

		BEGIN_STAT("KGameServer.Loop.g_cModuleManager.Breath()");
		g_cModuleManager.Breath();
		END_STAT();
	}
	
	INT nTopIndexDiff = cSafeScript->GetTopIndex() - cSafeScript.GetBeginTopIndex();
	BEGIN_STAT("KGameServer.Loop.End()");
	if (nTopIndexDiff > 0)
	{
		QLogPrintf(LOG_ERR, "Detected Lua Stack Leak! %d - %d",
			cSafeScript.GetBeginTopIndex(), cSafeScript->GetTopIndex());
	}
	END_STAT();

	return TRUE;
}

BOOL KGameServer::UnInit()
{
	m_bRunning = FALSE;
	g_piSocketServer->SocketClose(); // 先关闭客户端连接

	g_cGameserverEyes.Uninit();
	g_cPlayerDbMgr.Uninit();
	g_cModuleManager.UnInit();

	Q_Printl("World UnInitializing...");
	g_cOrpgWorld.UnInit();

#ifdef unix
	QThread_Sleep(2000); // 等待网络包发往其他服务器
#endif // unix

	g_cNetConnectPlayer.Uninit();

	g_RelayClient.UnInit();

	g_UnifyUnInit();
	m_cConsoleReader.Uninit();

	DumpStatics();
	return TRUE;
}

BOOL KGameServer::DoCommand( LPCSTR pszCommand )
{
	QCONFIRM_RET_FALSE(pszCommand);
	PCSTR pszLuaCommand = NULL;
	// 默认执行自己(gameserver)的脚本指令
	if (*pszCommand != '?')
		pszLuaCommand = pszCommand;
	else if (strstr(pszCommand, GM_CMD_PREFIX_GAMESERVER))
		pszLuaCommand = pszCommand + sizeof(GM_CMD_PREFIX_GAMESERVER) - 1;

	QLogPrintf(LOG_INFO, "[CMD]: %s", pszCommand);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	if (pszLuaCommand)
		return cSafeScript->DoBuffer(pszLuaCommand);
	else
		return cSafeScript->CallTableFunction("ServerEvent", "OnConsoleCommand", 0, "s", pszCommand);
	return FALSE;
}

BOOL KGameServer::_InitEnv()
{
	// Get GameCenter Ip/Port
	IIniFile* pIniFile = g_OpenIniFile(KDF_SERVER_CFG);
	QCONFIRM_RET_FALSE(pIniFile);

	//pIniFile->GetInteger("GameCenter", "Port", 5135, &m_nGameCenterPort);
	//pIniFile->GetString("GameCenter", "Ip", "", m_szGameCenterIp, countof(m_szGameCenterIp));

	pIniFile->GetInteger("GameServer", "ServerId", 0, &g_cOrpgWorld.m_nServerId);
	pIniFile->GetInteger("GameServer", "ServerMode", 0, &g_cOrpgWorld.m_nServerMode);
	return TRUE;
}

BOOL KGameServer::_LoadMiniServerScene()
{
	IIniFile* pIniFile = g_OpenIniFile(KDF_SERVER_CFG);
	QCONFIRM_RET_FALSE(pIniFile);

	INT nSceneCount = 0;
	pIniFile->GetInteger("PreloadScene", "Count", 0, &nSceneCount);

	if (nSceneCount > 0)
	{
		INT* pSceneTemplateIds = new INT[nSceneCount];
		INT nActualCount = pIniFile->GetMultiInteger("PreloadScene", "Scenes", pSceneTemplateIds, nSceneCount);
		QCONFIRM(nActualCount == nSceneCount);
		nSceneCount = nActualCount;

		for (INT j = 0; j < nSceneCount; j++)
		{
			INT nSceneId = pSceneTemplateIds[j];
			if (nSceneId > 0)
				g_pSceneMgr->LoadScene((DWORD)nSceneId, 0);
		}
	}

	return TRUE;
}

void KGameServer::DumpStatics()
{
	FILE* pFile = fopen(KDF_PROFILING_LOG, "a");
	// 输出协议流量等统计结果
	std::string strBuffer;
	LPCSTR pszStatics = g_cNetConnectPlayer.GetProtocolStat(strBuffer);
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
		fputs("Player Protocol Statics:\n", pFile);
		fputs(pszStatics, pFile);
		fputs("\n", pFile);
	}
	else
	{
		QLogPrintf(LOG_INFO, "Player Protocol Statics:\n%s", pszStatics);
	}

	std::string strBufferScriptStat;
	LPCSTR pszScriptStatics = g_cPlayerBaseProcess.GetScriptFunStat(strBufferScriptStat);
	if (pFile)
	{
		fputs("Script Call RxTimes/RxBytes Statics:\n", pFile);
		fputs(pszScriptStatics, pFile);
		fputs("\n", pFile);
	}
	else
	{
		QLogPrintf(LOG_INFO, "Script Call RxTimes/RxBytes Statics:\n%s", pszScriptStatics);
	}
	if (pFile)
		fclose(pFile);
}

// -------------------------------------------------------------------------
#endif
