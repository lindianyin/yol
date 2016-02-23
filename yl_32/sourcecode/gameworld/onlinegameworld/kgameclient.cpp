
#ifdef GAME_CLIENT

#include "stdafx.h"
#include "onlinegameworld/ikgameclient.h"
#include "ksocketinterface.h"
#include "knetconnectplayer_client.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/krequireinterface.h"
#include "onlinegamemodule/kscripthelper.h"
#include "onlinegamebase/ksysservice.h"
#include "kmodulemanager.h"
#include "onlinegameworld/kprotocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IKNsBaseRequire* g_piNsBaseRequire = NULL;

LPCSTR _GetTimeString()
{
	int nRetCode   = 0;
	tm LocalTime   = {0};
	time_t curtime = 0;
	static CHAR szTimeString[MAX_PATH] = {0};

	time(&curtime);  // get current time
	localtime_r(&curtime, &LocalTime);
	nRetCode = strftime(szTimeString, sizeof(szTimeString), "%Y-%m-%d %H:%M:%S", &LocalTime);
	ASSERT(nRetCode);

	return szTimeString;
}

// -------------------------------------------------------------------------
class KGameClient : public IKGameClient
{
public:
	virtual BOOL	Init(IKNsBaseRequire* piNsBaseRequire);
	virtual BOOL	UnInit();
	virtual BOOL	Connect(DWORD dwIpAddress, USHORT uPort, const GUID* pGuid, BOOL bHidden);
	virtual BOOL	DisConnect();
	virtual BOOL	Loop();
	virtual BOOL	IsReady() { return TRUE; }
	virtual BOOL	ProcessCommand(PCSTR pszCommand);
};

KGameClient _gGameClient;

extern IKSocketClientInterface* g_piSocketClient;

IKGameClient* g_GetGameClientInterface()
{
	return &_gGameClient;
}

BOOL KGameClient::Connect( DWORD dwIpAddress, USHORT uPort, const GUID* pGuid, BOOL bHidden )
{
	return g_piSocketClient->Connect(dwIpAddress, uPort, pGuid, bHidden);
}

BOOL KGameClient::DisConnect()
{
	return g_piSocketClient->ShutDown();
}

BOOL KGameClient::Loop()
{
	g_UnifyLoop();
	QCONFIRM_RET_FALSE(g_piSocketClient);
	g_piSocketClient->Activate();

	if (!g_cOrpgWorld.m_dwClientSceneId)
		return TRUE;

    ULONGLONG nServerLoop = g_cOrpgWorld.m_nGSGameLoop;
    DWORD dwTimeNow = timeGetTime();

    ULONGLONG nLogicFrameTime = (ULONGLONG)(g_cOrpgWorld.m_dwLogicFrames - g_cOrpgWorld.m_nStartLoop) * 1000;
    ULONGLONG nLogicFlowTime = (ULONGLONG)(dwTimeNow - g_cOrpgWorld.m_dwStartTime) * GAME_FPS;

    while (
        (g_cOrpgWorld.m_dwLogicFrames <= nServerLoop - GAME_FPS / 4 || nLogicFrameTime < nLogicFlowTime) &&
        (g_cOrpgWorld.m_dwLogicFrames <= nServerLoop + GAME_FPS / 2))
	{
		g_cOrpgWorld.Activate();
		g_cModuleManager.Breath();

		nLogicFrameTime = (ULONGLONG)(g_cOrpgWorld.m_dwLogicFrames - g_cOrpgWorld.m_nStartLoop) * 1000;
		nLogicFlowTime = (ULONGLONG)(timeGetTime() - g_cOrpgWorld.m_dwStartTime) * GAME_FPS;
	}

	return TRUE;
}

BOOL KGameClient::Init(IKNsBaseRequire* piNsBaseRequire)
{
    BOOL bRetCode = FALSE;
    BOOL bResult = FALSE;

	bRetCode = g_UnifyInit();
    LOG_PROCESS_ERROR(bRetCode);

	g_piNsBaseRequire = piNsBaseRequire;

	// 输出服务器版本等
	Q_Printl("Client Startup." <<
		"\n_______________________________________________________________________________" <<
		"\nBuilt on " << __TIME__ << __DATE__ << ". Protocol [" << KD_PROTOCOL_VERSION <<
		"~" << KD_PROTOCOL_VERSION_LOWER_LIMIT << "]. Local Time " << _GetTimeString() <<
		std::endl);

	bRetCode = g_cModuleManager.PreInit();
    LOG_PROCESS_ERROR(bRetCode);

	Q_Printl("9-Sky World Initializing...");

	bRetCode = g_cOrpgWorld.Init();
    LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_cModuleManager.Init();
    LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_piSocketClient->Init(&g_cNetConnectPlayer);
    LOG_PROCESS_ERROR(bRetCode);

	Q_Printl("\n_______________________________________________________________________________" <<
		"\nLocal Time " << _GetTimeString() <<
		"\nClient Ready." <<
		"\n_______________________________________________________________________________");

    bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KGameClient::UnInit()
{
    BOOL bRetCode = FALSE;
    BOOL bResult = FALSE;

	bRetCode = g_cModuleManager.UnInit();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = g_piSocketClient->ShutDown();
    LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_cOrpgWorld.UnInit();
	LOG_PROCESS_ERROR(bRetCode);

    bRetCode = g_UnifyUnInit();
    LOG_PROCESS_ERROR(bRetCode);

    bResult = TRUE;
EXIT0:
    return bResult;
}

BOOL KGameClient::ProcessCommand( PCSTR pszCommand )
{
	QCONFIRM_RET_FALSE(pszCommand);
	PCSTR pszLuaCommand = NULL;
	// 默认执行自己(client)的脚本指令
	if (*pszCommand != '?')
		pszLuaCommand = pszCommand;
	else if (strstr(pszCommand, GM_CMD_PREFIX_CLIENT))
		pszLuaCommand = pszCommand + sizeof(GM_CMD_PREFIX_CLIENT) - 1;

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	if (pszLuaCommand)
		return cSafeScript->DoBuffer(pszLuaCommand);
	else
		return cSafeScript->CallTableFunction("ClientEvent", "OnConsoleCommand", 0, "s", pszCommand);
}

// -------------------------------------------------------------------------
#endif
