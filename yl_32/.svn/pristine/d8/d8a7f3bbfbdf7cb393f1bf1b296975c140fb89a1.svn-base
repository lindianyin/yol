#include "stdafx.h"
#include "GameCenter.h"
#include "DBTools.h"
#include "Misc/qconsolereader.h"
#include "onlinegamebase/kjxexunify.h"
#include "dbmanager.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegamebase/ktimermgr.h"
#include "onlinegamebase/ikbadwordfilter.h"
#include "config/kconfigfile.h"
#include "globaldata/ScheduleTaskCenter.h"
#include "battlezone/battlezonemaster.h"
#include "battlezone/battlezoneslave.h"
#include "overallqueue/kqueuemaster.h"
#include "overallqueue/kqueueslave.h"
#include "globalservermaster.h"
#include "globalserverslave.h"
#include "QRandomName/QRandomName.h"

#ifdef WIN32
    #include <conio.h>
	#include <windows.h>
	#include <time.h>
#else
	#include <sys/time.h>
#endif	//WIN32

static QConsoleReader s_ConsoleReader;

QGameCenter::QGameCenter(void)
{
    m_nWorkLoop         = 0;
    m_nIdleLoop         = 0;
    m_piSequence 	    = NULL;
    //m_piTextFilter      = NULL;
    m_nZoneChargeFlag   = 0;
    gDay				= 1;
	m_bDevMode			= FALSE;
}

BOOL QGameCenter::Init(void)
{
    BOOL                bResult                             = false;
	BOOL                bRetCode                            = false;
    int                 nRetCode                            = 0;
    const char*         pszLocale                           = NULL;
    BOOL                bDBToolsInitFlag                    = false;
    BOOL                bGatewayInitFlag                    = false;
	BOOL                bPayInitFlag						= false;
    BOOL                bRoleManagerInit                    = false;
	BOOL				bFellowshipInitFlag                 = false;
    BOOL                bMiscDBInitFlag                     = false;
    BOOL                bGameServerInitFlag                 = false;
    // BOOL                bTeamsInitFlag                     = false;
    BOOL                bTeamCenterInitFlag                 = false;
    BOOL                bMapManagerInitFlag                 = false;
    BOOL                bMailManagerInitFlag                = false;
    BOOL                bAuctionManagerInitFlag             = false;
    BOOL                bRoleDBInitFlag                     = false;
    BOOL                bEyesInitFlag                       = false;
    BOOL                bScriptInitFlag                     = false;
    BOOL                bFilterInitFlag                     = false;
    unsigned long       ulMySQLClientVersionID              = 0;
    BOOL                bBattleFieldInitFlag                = false;
    BOOL                bCampInitFlag                       = false;
    BOOL                bPQCenterInitFlag                   = false;
    BOOL                bTongManagerInitFlag                = false;
	BOOL				bRankManagerInitFlag				= false;
	BOOL				bArenaManagerInitFlag				= false;
	BOOL				bQuizManagerInitFlag				= false;
	BOOL				bRecordManagerInitFlag				= false;
	BOOL                bRelationManagerInitFlag            = false;
    BOOL                bGlobalCustomDataManagerInitFlag    = false;
    BOOL                bStatDataManagerInitFlag            = false;
    BOOL                bRoadCenterInitFlag                 = false;
    BOOL                bApexProxyInitFlag                  = false;
    BOOL                bGameCardManagerInitFlag            = false;
	BOOL                bBattleZoneInitFlag					= false;
	BOOL				bQRandomNameInitFlag				= false;
	BOOL                bEscortManagerInitFlag                = false;
	BOOL				bGMSendAwardInitFlag				= false;
	BOOL				bOpenseSaveInitFlag				= false;
    KDBTools            DBTools;
 	
	s_ConsoleReader.Init();

    m_nBaseTime = time(NULL);
    m_nTimeNow  = m_nBaseTime;

    bRetCode = m_Settings.Load();
    LOG_PROCESS_ERROR(bRetCode);

    pszLocale = setlocale(LC_ALL, m_Settings.m_szLocale);
    LOG_PROCESS_ERROR(pszLocale);

    QLogPrintf(LOG_INFO, "Set locale: %s\n", pszLocale);

    //nRetCode = lzo_init();
    //LOG_PROCESS_ERROR(nRetCode == LZO_E_OK);

	bRetCode = g_cScriptManager.Init();
	LOG_PROCESS_ERROR(bRetCode && "Init Script Manager");

	bRetCode = g_UnifyInit();
	LOG_PROCESS_ERROR(bRetCode && "g_UnifyInit");

	bRetCode = g_cTimerMgr.Init();
	LOG_PROCESS_ERROR(bRetCode && "TimerMgr Init");

	bRetCode = KDbManager::Inst()->InitDBManager();
	LOG_PROCESS_ERROR(bRetCode && "InitDBManager");

	Q_Printl("[Loader] DbManager thread starting...");
	bRetCode = KDbManager::Inst()->StratUp();
	LOG_PROCESS_ERROR(bRetCode && "KDbManager StratUp");

	InitScriptEnv();

    bRetCode = DBTools.Init();
    LOG_PROCESS_ERROR(bRetCode);
    bDBToolsInitFlag = true;

    bRetCode = DBTools.CheckDatabase();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = m_Gateway.Init();
    LOG_PROCESS_ERROR(bRetCode);
    bGatewayInitFlag = true;

	//bRetCode = m_PayAgency.Init();
	//LOG_PROCESS_ERROR(bRetCode);
	bPayInitFlag = true;

    bRetCode = m_RoleManager.Init();
    LOG_PROCESS_ERROR(bRetCode);
    bRoleManagerInit = true;

    bRetCode = m_RoleDB.Init();
    LOG_PROCESS_ERROR(bRetCode);
    bRoleDBInitFlag = true;

	bRetCode = m_TongManager.Init();
	LOG_PROCESS_ERROR(bRetCode);
    bTongManagerInitFlag = true;

	bRetCode = m_RankManager.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bRankManagerInitFlag = true;

	bRetCode = m_ArenaManager.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bArenaManagerInitFlag = true;

	bRetCode = m_QuizManager.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bQuizManagerInitFlag = true;

	bRetCode = m_RecordManager.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bRecordManagerInitFlag = true;

	bResult = m_RelationManager.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bRelationManagerInitFlag = true;

    bRetCode = m_MiscDB.Init();
    LOG_PROCESS_ERROR(bRetCode);
    bMiscDBInitFlag = true;

	//bRetCode = m_FellowshipCenter.Init();
	//LOG_PROCESS_ERROR(bRetCode);
	//bFellowshipInitFlag = true;

    bRetCode = m_MapManager.Init();
	LOG_PROCESS_ERROR(bRetCode);
    bMapManagerInitFlag = true;

    bRetCode = m_MailManager.Init();
    LOG_PROCESS_ERROR(bRetCode);
    bMailManagerInitFlag = true;

    bRetCode = m_TeamCenter.Init();
    LOG_PROCESS_ERROR(bRetCode);
    bTeamCenterInitFlag = true;

    bRetCode = m_BornPlaceList.Load();
    LOG_PROCESS_ERROR(bRetCode);

	bRetCode = m_GameServer.Init();
	LOG_PROCESS_ERROR(bRetCode);
    bGameServerInitFlag = true;

    bRetCode = m_Monitor.Init();
    LOG_PROCESS_ERROR(bRetCode);
    bEyesInitFlag = true;

	// 随机起名
	bRetCode = m_QRandomName.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bQRandomNameInitFlag = true;

	bRetCode = m_GMSendAward.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bGMSendAwardInitFlag = true;

	bOpenseSaveInitFlag = m_OpenseSave.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bOpenseSaveInitFlag = true;
    //bRetCode = m_ScriptManager.Init();
    //LOG_PROCESS_ERROR(bRetCode);
    //bScriptInitFlag = true;
    //
    //bRetCode = m_CampManager.Init();
    //LOG_PROCESS_ERROR(bRetCode);
    //bCampInitFlag = true;

    //bRetCode = m_GlobalSystemValueManager.Load();
    //LOG_PROCESS_ERROR(bRetCode);

    //bRetCode = m_BattleFieldManager.Init();
    //LOG_PROCESS_ERROR(bRetCode);
    //bBattleFieldInitFlag = true;

    //bRetCode = m_PQCenter.Init();
    //LOG_PROCESS_ERROR(bRetCode);
    //bPQCenterInitFlag = true;

    //bRetCode = m_GlobalCustomDataManager.Init();
    //LOG_PROCESS_ERROR(bRetCode);
    //bGlobalCustomDataManagerInitFlag = true;

    bRetCode = m_StatDataManager.Init();
    LOG_PROCESS_ERROR(bRetCode);
    bStatDataManagerInitFlag = true;

	bRetCode = m_EscortManager.Init();
	LOG_PROCESS_ERROR(bRetCode);
	bEscortManagerInitFlag = true;

    //bRetCode = m_RoadCenter.Init();
    //LOG_PROCESS_ERROR(bRetCode);
    //bRoadCenterInitFlag = true;

    //bRetCode = m_ApexProxy.Init();
    //LOG_PROCESS_ERROR(bRetCode);
    //bApexProxyInitFlag = true;

    //bRetCode = m_GameCardManager.Init();
    //LOG_PROCESS_ERROR(bRetCode);
    //bGameCardManagerInitFlag = true;

    m_piSequence = CreateIDSequence(
        m_Settings.m_szDBIP, 
        m_Settings.m_szDBName, 
        m_Settings.m_szDBAcc, 
        m_Settings.m_szDBPsw
    );
    LOG_PROCESS_ERROR(m_piSequence);

    //m_piTextFilter = CreateTextFilterInterface();
    //LOG_PROCESS_ERROR(m_piTextFilter);

    //bRetCode = m_piTextFilter->LoadFilterFile("NameFilter.txt");
    //LOG_PROCESS_ERROR(bRetCode);

    //bRetCode = m_piTextFilter->LoadWhiteListFile("WhiteList.txt");
    //LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_ScheduleTask.Init();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_GlobalServerMaster.Init();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_GlobalServerSlave.Init();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_QueueMaster.Init();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_QueueSlave.Init();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_BZoneMaster.Init();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_BZoneSlave.Init();
	LOG_PROCESS_ERROR(bRetCode);

    // 注意,其他模块的Init请放到LoadGameData之前
    bRetCode = DBTools.LoadGameData();
    LOG_PROCESS_ERROR(bRetCode);

    m_LogServerAgency.Init();

	//bRetCode = g_pBadwordFilter->Init();
	//LOG_PROCESS_ERROR(bRetCode);
	//bRetCode = g_pBadwordFilter->LoadFilterFile(KDF_BAD_WORDS);
	//LOG_PROCESS_ERROR(bRetCode);

    //m_RemoteScript.Setup();

    //CallMainScript("Setup");

    //m_RoleManager.CalculateCampActivePlayerCount();

    m_bQuitFlag = false;

    bResult = true;
EXIT0:
    if (!bResult)
    {
        //SAFE_RELEASE(m_piTextFilter);
        SAFE_RELEASE(m_piSequence);

        m_LogServerAgency.UnInit();

        //if (bGameCardManagerInitFlag)
        //{
        //    m_GameCardManager.UnInit();
        //    bGameCardManagerInitFlag = false;
        //}

        //if (bApexProxyInitFlag)
        //{
        //    m_ApexProxy.UnInit();
        //    bApexProxyInitFlag = false;
        //}

        //if (bRoadCenterInitFlag)
        //{
        //    m_RoadCenter.UnInit();
        //    bRoadCenterInitFlag = false;
        //}

        if (bStatDataManagerInitFlag)
        {
            m_StatDataManager.UnInit();
            bStatDataManagerInitFlag = false;
        }

        //if (bGlobalCustomDataManagerInitFlag)
        //{
        //    m_GlobalCustomDataManager.UnInit();
        //    bGlobalCustomDataManagerInitFlag = false;
        //}

        //if (bPQCenterInitFlag)
        //{
        //    m_PQCenter.UnInit();
        //    bPQCenterInitFlag = false;
        //}

        //if (bBattleFieldInitFlag)
        //{
        //    m_BattleFieldManager.UnInit();
        //    bBattleFieldInitFlag = false;
        //}

        //if (bCampInitFlag)
        //{
        //    m_CampManager.UnInit();
        //    bCampInitFlag = false;
        //}

        //if (bScriptInitFlag)
        //{
        //    m_ScriptManager.UnInit();
        //    bScriptInitFlag = false;
        //}

        if (bEyesInitFlag)
        {
            m_Monitor.UnInit();
            bEyesInitFlag = false;
        }

        if (bGameServerInitFlag)
        {
            m_GameServer.UnInit();
            bGameServerInitFlag = false;
        }

        if(bMiscDBInitFlag)
        {
            m_MiscDB.UnInit();
            bMiscDBInitFlag = false;
        }
        
        if (bTeamCenterInitFlag)
        {
            m_TeamCenter.UnInit();
            bTeamCenterInitFlag = false;
        }

		if (bQRandomNameInitFlag)
		{
			m_QRandomName.UnInit();
			bQRandomNameInitFlag = false;
		}

        if (bMailManagerInitFlag)
        {
            m_MailManager.UnInit();
            bMailManagerInitFlag = false;
        }
        
        if (bMapManagerInitFlag)
        {
            m_MapManager.UnInit();
            bMapManagerInitFlag = false;
        }

		//if (bFellowshipInitFlag)
		//{
		//	m_FellowshipCenter.UnInit();
		//	bFellowshipInitFlag = false;
		//}

        if (bTongManagerInitFlag)
        {
            m_TongManager.UnInit();
            bTongManagerInitFlag = false;
        }

		if(bRankManagerInitFlag)
		{
			m_RankManager.UnInit();
			bRankManagerInitFlag = false;
		}

		if(bArenaManagerInitFlag)
		{
			m_ArenaManager.UnInit();
			bArenaManagerInitFlag = false;
		}

		if(bQuizManagerInitFlag)
		{
			m_QuizManager.UnInit();
			bQuizManagerInitFlag = false;
		}

		if(bRecordManagerInitFlag)
		{
			m_RecordManager.UnInit();
			bRecordManagerInitFlag = false;
		}

		if (bRelationManagerInitFlag)
		{
			m_RelationManager.UnInit();
			bRelationManagerInitFlag = false;
		}

		if(bEscortManagerInitFlag)
		{
			m_EscortManager.UnInit();
			bEscortManagerInitFlag = false;
		}

        if (bRoleDBInitFlag)
        {
            m_RoleDB.UnInit();
            bRoleDBInitFlag = false;
        }

        if (bRoleManagerInit)
        {
            m_RoleManager.UnInit();
            bRoleManagerInit = true;
        }

        if (bGatewayInitFlag)
        {
            m_Gateway.UnInit();
            bGatewayInitFlag = false;
        }

		//if (bPayInitFlag)
		//{
		//	m_PayAgency.UnInit();
		//	bPayInitFlag = false;
		//}
		if (bGMSendAwardInitFlag)
		{
			m_GMSendAward.UnInit();
			bGMSendAwardInitFlag = false;
		}
		
		if (bOpenseSaveInitFlag)
		{
			m_OpenseSave.UnInit();
			bOpenseSaveInitFlag = false;
		}

		if (bDBToolsInitFlag)
    	{
        	DBTools.UnInit();
        	bDBToolsInitFlag = false;
    	}
    }
	return bResult;
}

void QGameCenter::UnInit()
{
    //SAFE_RELEASE(m_piTextFilter);
    SAFE_RELEASE(m_piSequence);

	
    m_LogServerAgency.UnInit();
    //m_GameCardManager.UnInit();
    //m_ApexProxy.UnInit();
    //m_RoadCenter.UnInit();
    m_StatDataManager.UnInit();
    //m_GlobalCustomDataManager.UnInit();
    //m_PQCenter.UnInit();
    //m_BattleFieldManager.UnInit();
    //m_CampManager.UnInit();
    //m_ScriptManager.UnInit();
    m_Monitor.UnInit();
    m_GameServer.UnInit();
    m_TeamCenter.UnInit();
    //m_FellowshipCenter.UnInit();
	m_QRandomName.UnInit(); // 随机起名
    m_MailManager.UnInit();
    m_MapManager.UnInit();
    m_TongManager.UnInit();
	m_RankManager.UnInit();
	m_ArenaManager.UnInit();
	m_RecordManager.UnInit();
	m_RelationManager.UnInit();
    m_MiscDB.UnInit();
    m_RoleManager.UnInit();
    m_RoleDB.UnInit();
    m_Gateway.UnInit();
	//m_PayAgency.UnInit();
    //m_AntiFarmerManager.Clear();
    //m_MentorCenter.Clear();

	KDbManager::Inst()->UnInit();
	g_cScriptManager.Uninit();
	g_UnifyUnInit();
	g_cTimerMgr.UnInit();
	s_ConsoleReader.Uninit();

	g_GlobalServerMaster.UnInit();
	g_GlobalServerSlave.UnInit();

	g_QueueMaster.UnInit();
	g_QueueSlave.UnInit();

	g_BZoneSlave.UnInit();
	g_BZoneMaster.UnInit();

	QLogPrintf(LOG_INFO, "Coordinator UnInited.");
}

void QGameCenter::Run()
{
    BOOL        bRetCode   = false;
    UINT64    uBeginTime = GetTickCount();
    UINT64    uTimeNow   = 0;
    int         nIdleCount = 0;

    while (!m_bQuitFlag)
    {
        uTimeNow = GetTickCount();

        m_Gateway.Activate();
        m_GameServer.Activate();
		//m_PayAgency.Activate();

        if ((uTimeNow - uBeginTime) * GAME_FPS < (UINT64)m_nWorkLoop * 1000)
        {
            nIdleCount++;
            QThread_Sleep(10);
            continue;
        }

        bRetCode = CheckQuitComplete();
        if (bRetCode)
            break;

		if(m_nWorkLoop % KD_GC_SAVE_INTERNAL == 0)
		{
			OnRegularSaveProcess();//一分钟存盘一次，包括排行数据等
		}

        m_nIdleLoop = nIdleCount;
        nIdleCount  = 0;

        Activate();
    }
}

void QGameCenter::Activate()
{
    m_nTimeNow = time(NULL);
    m_nWorkLoop++;

//	PCSTR pszConsoleInput = s_ConsoleReader.ReadInput();
//	if (pszConsoleInput)
//	{
//		PCSTR pszLuaCommand = pszConsoleInput;
//		if (strstr(pszConsoleInput, GM_CMD_PREFIX_GAMECENTER))
//			pszLuaCommand = pszConsoleInput + sizeof(GM_CMD_PREFIX_GAMECENTER) - 1;

//		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
//		if (pszLuaCommand)
//			cSafeScript->DoBuffer(pszLuaCommand);
//	}

	g_UnifyLoop();

	g_cTimerMgr.Breath();

	KDbManager::Inst()->MainThreadActive();

    m_MapManager.Activate();

    //m_FellowshipCenter.Active();

    m_MailManager.Activate();

    m_RoleDB.Activate();

    m_RoleManager.Activate();

    m_MiscDB.Activate();

    m_Monitor.Activate();

    //m_ScriptManager.Activate();

    //m_BattleFieldManager.Activate();

    m_TeamCenter.Activate();

    m_TongManager.Activate();

	m_RankManager.Activate();

	m_RelationManager.Activate();

	m_EscortManager.Activate();

    //m_PQCenter.Activate();

    //m_CampManager.Activate();

    //m_GlobalCustomDataManager.Activate();

    //m_GlobalSystemValueManager.Activate();

    m_StatDataManager.Activate();

    //m_ActivityManager.Activate();

    //m_ApexProxy.Breathe();

    //m_GameCardManager.Activate();

    m_LogServerAgency.Activate();

	g_ScheduleTask.Breath();

	/*
	g_BZoneMaster.Activate();

	g_BZoneSlave.Activate();
	*/

	g_GlobalServerMaster.Activate();
	g_GlobalServerSlave.Activate();
    //m_AntiFarmerManager.Activate();

    //m_MentorCenter.Activate();


	if (m_nWorkLoop % (GAME_FPS * 5) == 0  && g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_MASTER)
	{
		m_GMSendAward.Activate();

		//m_RecordManager.ResetBuyPurchases();
		
	}

    if (m_nWorkLoop % GAME_FPS == 0)
    {
        CallMainScript("Activate");
    }

	m_OpenseSave.Activate();


	
}

void QGameCenter::Quit()
{
	KDbManager::Inst()->ReSetRuningState();
    m_GameServer.PrepareToQuit();
    m_Gateway.PrepareToQuit();
    m_bQuitFlag = true;
}

BOOL QGameCenter::CallMainScript(const char cszFunction[])
{
    BOOL    bResult             = false;
    BOOL    bRetCode            = false;
    int     nIndex              = 0;
    DWORD   dwScriptID          = 0;
//
//    dwScriptID = g_FileNameHash(MAIN_SCRIPT_FILE);
//
//    m_ScriptManager.SafeCallBegin(&nIndex);
//
//    bRetCode = m_ScriptManager.IsScriptExist(dwScriptID);
//    PROCESS_ERROR(bRetCode);
//
//    bRetCode = m_ScriptManager.IsFuncExist(dwScriptID, cszFunction);
//    PROCESS_ERROR(bRetCode);
//
//    m_ScriptManager.Call(dwScriptID, cszFunction, 0);
//
//    bResult = true;
//EXIT0:
//    m_ScriptManager.SafeCallEnd(nIndex);
    return bResult;
}

VOID QGameCenter::OnRegularSaveProcess()
{
	m_MailManager.SaveAll();

	//// 确保所有自定义数据存盘完毕
	//m_GlobalCustomDataManager.SaveAll();
	//m_GlobalSystemValueManager.SaveAll();

	// 确保所有帮会数据存盘完毕
	m_TongManager.SaveAll();

	// 确保排行榜数据存盘完毕
	m_RankManager.SaveAll();

	// 确保竞技场数据存盘完毕
	m_ArenaManager.SaveAll();

	//确保答题数据存盘完毕
	m_QuizManager.SaveAll();

	m_RecordManager.SaveAll();

	//// 确保所有好友数据存盘完毕
	//m_FellowshipCenter.SaveAllFellowshipData();
	m_RelationManager.SaveAll();

	//// 确保所有公共任务数据存盘完毕
	//m_PQCenter.SaveAll();

	//m_MentorCenter.SaveAll();

	// 角色数据库不用检查,因为其工作线程会保证
	//m_RoleDB.CheckSaveQueueComplete();

	m_RoleDB.ProcessSavePayRecord();
}

BOOL QGameCenter::CheckQuitComplete()
{
    BOOL bResult    = false;
    BOOL bRetCode   = false;

    // 只有哪些“变化才会存盘”的数据，才能在这里保存
    // 比如统计数据，就不能在这里保存，因为只要调用了save
    // 它就回生成一个数据库操作请求，导致数据库线程无法停止工作

    PROCESS_ERROR(m_bQuitFlag);

    // 确保所有GS都已经关闭
    bRetCode = m_GameServer.IsAnyGSConnected();
    PROCESS_ERROR(!bRetCode);

	// 存数据库
	OnRegularSaveProcess();

    // 确保MiscDB的请求与返回队列都全部被处理了
    // 这里必须保证请求队列和应答队列都被完全处理
    // 因为返回的应答可能带来新的数据变化
    bRetCode = m_MiscDB.IsAllPackageComplete();
    PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

void QGameCenter::InitScriptEnv()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->PushNumber(1);
	cSafeScript->SetGlobalName("MODULE_GAMECENTER");

	extern BOOL g_RegisterKLibScriptFun();
	extern BOOL g_RegisterKFileScriptFun();
	extern BOOL g_RegisterKUnifyScriptFun();
	extern BOOL g_RegisterGcLogicBaseScriptFun();
	extern BOOL g_RegisterKinScriptFun();
	extern BOOL g_RegisterTimerMgrScriptFun();
	extern BOOL g_RegisterRelationScriptFun();

	g_RegisterKLibScriptFun();
	g_RegisterKFileScriptFun();
	g_RegisterKUnifyScriptFun();
	g_RegisterGcLogicBaseScriptFun();
	g_RegisterKinScriptFun();
	g_RegisterTimerMgrScriptFun();
	g_RegisterRelationScriptFun();

	g_cScriptManager.LoadScript("\\script\\preload.lua");
	g_cScriptManager.LoadDir("\\script\\lib\\",  "\\script\\script.pak.txt");
	g_cScriptManager.LoadScript("\\script\\misc\\gcevent.lua");
	g_cScriptManager.LoadDir("\\script\\gcscript\\", "\\script\\script.pak.txt");

	cSafeScript->CallTableFunction("GCEvent", "OnStartUp", 0, "");
}
