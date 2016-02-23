#include "stdafx.h"
#include "kgamecenter.h"
#include "kloader.h"
#include "kmodulesetting.h"
#include "Misc/qconsolereader.h"
#include "config/gamedef.h"
#include "onlinegamebase/kscriptmanager.h"
#include "gclogicbase/kgclogicextinterface.h"
#include "kscriptenv.h"
#include "kgc_coordinatorclient.h"
#include "kgamecentereyes.h"

KGameCenter g_cGameCenter;
static QConsoleReader s_ConsoleReader;

KGameCenter::KGameCenter(void)
{
}

KGameCenter::~KGameCenter(void)
{
}

INT KGameCenter::Init()
{
	INT nRet = FALSE;
	CHAR szIP[MAX_PATH] = "";
	INT nPort = 0;
	INT nLoaderId = 0;

	IIniFile* piIniFile = g_OpenIniFile(KDF_GC_CONFIG);
	LOG_PROCESS_ERROR(piIniFile);

	nRet = piIniFile->GetString("Coordinator", "IP", "", szIP, sizeof(szIP));
	LOG_PROCESS_ERROR(nRet);
	nRet = piIniFile->GetInteger("Coordinator", "Port", 0, &nPort);
	LOG_PROCESS_ERROR(nRet);
	nRet = piIniFile->GetInteger("GameCenter", "LoaderId", 0, &nLoaderId);
	LOG_PROCESS_ERROR(nRet);

	nRet = KModuleSetting::Inst()->Init();
	LOG_PROCESS_ERROR(nRet);

	nRet = KGC_CoordinatorClient::Inst().Init(szIP, nPort);
	LOG_PROCESS_ERROR(nRet);

	nRet = KGC_CoordinatorClient::Inst().WaitForModuleInfo();
	LOG_PROCESS_ERROR(nRet);

	s_ConsoleReader.Init();

	nRet = KLoader::Inst()->Init(nLoaderId);
	LOG_PROCESS_ERROR(nRet);

	{
		KGcScriptEnv::Inst()->GetScriptManager().LoadScript("\\script\\globaldata\\globaldatadef.lua");
		KGcScriptEnv::Inst()->GetScriptManager().LoadScript("\\script\\misc\\gcevent.lua");
		QLuaScript &rcScript = KGcScriptEnv::Inst()->GetScriptManager().GetMainScript();
		rcScript.CallTableFunction("GCEvent", "OnGCInited", 0, "");
	}

	nRet = KGameCenterEyes::Inst().Init(nLoaderId);
	LOG_PROCESS_ERROR(nRet && "KGameCenterEyes Init");

	m_bRunning = TRUE;
	nRet = TRUE;
EXIT0:
	SAFE_RELEASE(piIniFile);
	return nRet;
}

INT KGameCenter::Shutdown()
{
	// mark exit flag
	m_bRunning = FALSE;
	return TRUE;
}

INT KGameCenter::Uninit()
{
	KGameCenterEyes::Inst().Uninit();
	s_ConsoleReader.Uninit();
	KLoader::Inst()->Uninit();
	KGC_CoordinatorClient::Inst().Uninit();
	return TRUE;
}

INT KGameCenter::Run()
{
	while (m_bRunning)
	{
		PCSTR pszConsoleInput = s_ConsoleReader.ReadInput();
		if (pszConsoleInput)
		{
			PCSTR pszLuaCommand = pszConsoleInput;
			if (strstr(pszConsoleInput, GM_CMD_PREFIX_GAMECENTER))
				pszLuaCommand = pszConsoleInput + sizeof(GM_CMD_PREFIX_GAMECENTER) - 1;

			ScriptSafe cSafeScript = g_cScriptManager.GetMainScript();
			if (pszLuaCommand)
				cSafeScript->DoBuffer(pszLuaCommand);
		}

		KLoader::Inst()->GameLoop();
		KGC_CoordinatorClient::Inst().Activate();
		KGameCenterEyes::Inst().Active();
	}

	QLogPrintf(LOG_INFO, "%s\n", "Shutting down...");
	return TRUE;
}
