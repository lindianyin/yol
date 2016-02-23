

#ifdef GAME_SERVER

#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "../onlinegamebase/kdatetimecontrol.h"
#include "onlinegameworld/kplayermgr.h"
#include "scene/kscenemgr.h"
#include "ksocketinterface.h"
#include "kgameservereyes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern KDateTimeControl g_cDataTimeControl;

namespace KGameserverEyesNamespace
{
	INT LuaGetRealSecond(QLuaScript& sc)
	{
		INT nResult = 0;
		INT nRet = 0;

		double dRealElapse = (double)g_cDataTimeControl.GetRealGameElapse();
		dRealElapse = dRealElapse / 1000.f;
		nRet = sc.PushNumber( dRealElapse );
		LOG_PROCESS_ERROR(nRet && "[KGameserverEyes]: LuaGetRealSecond 返回值失败");

		nResult = 1;
		EXIT0:
		return nResult;
	}

	INT LuaGetGameserverSocketInfo(QLuaScript& sc)
	{
		INT nResult = 0;
		INT nRet = 0;
		KSOCKET_KINGEYES_INFO cSocketInfo = { 0 };

		g_piSocketServer->GetSocketInfo(&cSocketInfo);
		nRet = sc.PushTable();
		PROCESS_ERROR(nRet);
		
		nRet = sc.PushNumber((double)cSocketInfo.dwSendPackages);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwSendPackages");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber((double)cSocketInfo.dwRecvPackages);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwRecvPackages");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber((double)cSocketInfo.dwTotalConnection);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwTotalConnection");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber((double)cSocketInfo.dwTotalCloseConnection);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwTotalCloseConnection");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber((double)cSocketInfo.dwTotalLostConnection);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwTotalLostConnection");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber((double)cSocketInfo.dwProcessedConnection);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwProcessedConnection");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber(cSocketInfo.dwCurrentConnection);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwCurrentConnection");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber((double)cSocketInfo.dwRecvBytes);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwRecvBytes");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber((double)cSocketInfo.dwSendBytes);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwSendBytes");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber(cSocketInfo.dwRecvBytesPerSecond);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwRecvBytesPerSecond");
		PROCESS_ERROR(nRet);

		nRet = sc.PushNumber(cSocketInfo.dwSendBytesPerSecond);
		PROCESS_ERROR(nRet);
		nRet = sc.SetTableField("dwSendBytesPerSecond");
		PROCESS_ERROR(nRet);

		nResult = 1;
		EXIT0:
		return nResult;
	}

	INT LuaPostGmResult(QLuaScript& sc)
	{
		INT nSession  = sc.GetInt(1);
		BYTE byAction = sc.GetInt(2);
		INT nResult	  = sc.GetInt(3) ? emKGMCMD_RESULT_SUCCESS : emKGMCMD_RESULT_FAILURE;
		BYTE byAsker  = sc.GetInt(4);
		LPCSTR pszResult = sc.GetStr(5);
		g_cGameserverEyes.PostGmResult(nSession, byAction, nResult, byAsker, pszResult);
		return 0;
	}

	CONST SCRIPT_FUNCTION arFunction[]	=  
	{
		{"...", NULL, "", "", "", ""},
#ifdef GAME_SERVER
		{"GetRealSecond", LuaGetRealSecond, "获取服务器启动时间", "", "", ""},
		{"GetGameserverSocketInfo", LuaGetGameserverSocketInfo, "获取服务器网络相关信息", "", "", ""},
		{"PostGmResult", LuaPostGmResult, "向Monitor返回Gm指令结果", "dddds", "", ""},
#endif
	};
}

BOOL g_RegisterGameserverEyes()
{
	return g_cScriptManager.RegisterTableFuncs("KGameserverEyes", KGameserverEyesNamespace::arFunction, countof(KGameserverEyesNamespace::arFunction));
}

#endif