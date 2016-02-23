/* -------------------------------------------------------------------------
//	文件名		：	kgcgateway.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/4/27 10:27:00
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "common/kinstance.h"
#include "devbase/ksysservice.h"
#include "kgcgateway.h"
#include "kloader.h"
#include "kgcmonitorprocess.h"
#include "ksharememoryprovider.h"
#include "kmempipesys.h"
#include "kgcconfig.h"
#include "kkeysharemem.h"
#include "kloadersharemem.h"
#include "kloadermanager.h"
#include "kloadergatewayprotocol.h"

#ifdef __unix
#include <sys/time.h>
#include <sys/resource.h>
#endif // __unix

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
LPCSTR g_pstrCurProcName = NULL;

#define KD_GC_PACKAGE_FILE "package.txt"
#define GUID_GCGATEWAY	"KINGSOFT_XXJ_GCGATEWAY"

/*IKSysService* KSysService::m_pService = NULL;*/
// refact this
extern IKSocketClientInterface* g_piGcMonitorClient;
#define _KD_GCGW_CFG_FILE "gcgateway_cfg.ini"

INT main(INT argc, LPSTR argv[])
{
#ifndef WIN32
	// 允许产生coredump文件
	rlimit sLimit;
	sLimit.rlim_cur = -1;
	sLimit.rlim_max = -1;
	::setrlimit(RLIMIT_CORE, &sLimit);

	// 设置创建的新文件权限为(rw.r.r)
	umask(022);
#endif
	g_pstrCurProcName = argv[0];

	srand((INT)time(NULL));
	g_RandomSeed((INT)time(NULL));
	g_SetRootPath(NULL);
	g_SetFilePath("\\");

	// 初始化Log
	{
		CHAR szCWD[MAX_PATH] = ".";
		getcwd(szCWD, sizeof(szCWD));

		KGLOG_PARAM LogParam;
		memset(&LogParam, 0, sizeof(LogParam));
		snprintf(LogParam.szPath, sizeof(LogParam.szPath) - 1, "%s/log", szCWD);
		g_StrCpyLen(LogParam.szIdent, "gcgateway", sizeof(LogParam.szIdent));
		LogParam.nMaxLineEachFile = 65536 * 2;
		LogParam.Options = (KGLOG_OPTIONS)(KGLOG_OPTION_FILE | KGLOG_OPTION_CONSOLE);

		KGLogInit(LogParam, NULL);
	}

	// 是否Loader
	if (argc > 1 && argv[1][0] == '_')
	{
		INT nLoaderId = atoi(argv[1] + 1);
		KConsoleHelper::DoInfoColor();
		KG_Printl("Running as GcLoader[" << nLoaderId << "]...");
		KConsoleHelper::RestoreColor();

		g_SetPackageVersion(1);			// 使用新的pak格式
		// 注意顺序是倒着优先，越后面的越优先
		if (!KGPack_LoadPackageFiles(KD_GC_PACKAGE_FILE, KGPACK_FILE_PRIORITY_EXTERNAL_FILE_FIRST))
		{
			KGLogPrintf(KGLOG_ERR, "LoadPackageFile '%s' failed!", KD_GC_PACKAGE_FILE);
		}

		KLoader::Inst()->StartupLoader(nLoaderId);
	}
	else
	{
		KConsoleHelper::DoInfoColor();
		KG_Printl("Running as GcGateway...");
		KConsoleHelper::RestoreColor();

		KInstance _instance(GUID_GCGATEWAY);
		if (!_instance.Lock())
		{
			KG_Printl("GcGateway is already running!");
			system("pause");
			return -1;
		}
		KGcGateway cGame;
		if (cGame.Init())
			cGame.Run();
	}
	return 0;
}

BOOL KGcGateway::Init()
{
	BOOL bRet = FALSE;

	bRet = KGcConfig::Inst()->LoadConfig(); // 这个是GcGateway自己用的
	KGC_CONFIRM_RET_FALSE(bRet);

	bRet = KKeyShareMem::Inst()->Init();
	KGC_CONFIRM_RET_FALSE(bRet);
	bRet = KShareMemProvider::Inst()->Init();
	KGC_CONFIRM_RET_FALSE(bRet);

	bRet = g_piGcMonitorClient->Init(&g_cGcMonitorProcess);
	KGC_CONFIRM_RET_FALSE(bRet);

	m_bRunStatus = TRUE;
	return bRet;
}

BOOL KGcGateway::Run()
{
	BOOL bRet = FALSE;
	CHAR buf[128];
	KG_Printl("\n********************** GcGateway Startup ************************");
	time_t t;
	KSysService::Time(&t);
	strftime(buf, sizeof(buf), "OS Time: %x %X", localtime(&t));
	KG_Printl(buf);

	KGCGW_PARAM& cParam = KGcConfig::Inst()->GetGcParam();
	bRet = g_piGcMonitorClient->Connect(cParam.szMonitorIp, (USHORT)cParam.nMonitorPort);
	KGC_CONFIRM_RET_FALSE(bRet);

	while(m_bRunStatus)
	{
		GameLoop();
		KGThread_Sleep(1);
	}
	return bRet;
}


BOOL KGcGateway::GameLoop()
{
	g_piGcMonitorClient->Activate();
	KLoaderManager::Inst()->Activate();
	// TODO
	//KMemPipeSys::Inst()->Process(-1, &PipeProcessFun(this, &KGcGateway::ProcessPipeData));
	KMemPipeSys::Inst()->Flush();
	return TRUE;
}

//static BOOL L2GwProcess(KL2GW_INTERFACE_INFO* pInfo, UINT uDataSize)
//{
//	pInfo->byLoaderId;
//}

BOOL KGcGateway::ProcessPipeData( LPVOID pvData, UINT uDataSize )
{
	//switch (*(LPBYTE)pvData)
	//{
	//case emKLOAD2GW_INTERFACE_INFO:
	//	return L2GwProcess((KL2GW_INTERFACE_INFO* )pvData, uDataSize);
	//	break;
	//default:
	//	KGC_CONFIRM_RET_FALSE(FALSE);
	//}
	return TRUE;
}

BOOL KGcGateway::Uninit()
{

	return TRUE;
}

// -------------------------------------------------------------------------
