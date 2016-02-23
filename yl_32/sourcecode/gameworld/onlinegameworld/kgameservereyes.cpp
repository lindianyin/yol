
#ifdef GAME_SERVER

#include "stdafx.h"
#include "kgameservereyes.h"
#include "onlinegameworld/korpgworld.h"
#include "scene/kscenemgr.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/ikgameserver.h"
#include "ksocketinterface.h"
#include "KRelayClient.h"
#include <algorithm>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KD_GSEYE_DEFAULT_INTERVAL (3 * GAME_FPS) // 发送频率
#define KD_GSEYE_DEFAULT_MAP_COUNT 64

KGameserverEyes g_cGameserverEyes;
extern KSceneMgr g_SceneMgr;


BOOL GetSelfName(char* pszBuffer, size_t uBufferSize)
{
	BOOL    bResult  = false;
	int     nRetCode = false;

#ifdef WIN32
	nRetCode = (int)GetModuleFileName(NULL, pszBuffer, (DWORD)uBufferSize);
	LOG_PROCESS_ERROR(nRetCode);
#else
	nRetCode = readlink("/proc/self/exe", pszBuffer, uBufferSize);
	LOG_PROCESS_ERROR(nRetCode > 0 && nRetCode < (int)uBufferSize);
	pszBuffer[nRetCode] = '\0';
#endif

	bResult = true;
EXIT0:
	return bResult;
}
// --------------------------------------------------------------------------------

KGameserverEyes::KGameserverEyes():
m_dwPingInterval(KD_GSEYE_DEFAULT_INTERVAL),
m_dwSendCustomInfoInterval(KD_GSEYE_DEFAULT_INTERVAL),
m_dwSendPlayerCountInterval(KD_GSEYE_DEFAULT_INTERVAL),
m_dwSendMapInfoInterval(KD_GSEYE_DEFAULT_INTERVAL),
m_dwNextPingFrame(0),
m_dwNextSendCustomInfoFrame(0),
m_dwNextSendMapInfoFrame(0),
m_dwNextSendPlayerCountFrame(0),
//m_pMapDesc(NULL),
m_nMapDesc(0),
m_nCurMapDesc(0)
{

}

KGameserverEyes::~KGameserverEyes()
{

}

int KGameserverEyes::Init(int nKey)
{
	int nResult = 0;
	int nRet = 0;

	nRet = KEyes::Init(nKey);
	PROCESS_ERROR(nRet);

	m_fLastPerformance          = 0.0f;
	m_fLastClientFlux           = 0.0f;

	nRet = GetSelfName(m_szSelfName, sizeof(m_szSelfName));
	LOG_CHECK_ERROR(nRet);

	//m_pMapDesc = (l2e_update_map_list::KMapDesc*)malloc(KD_GSEYE_DEFAULT_MAP_COUNT*sizeof(l2e_update_map_list::KMapDesc));
	//PROCESS_ERROR(m_pMapDesc);
	m_nMapDesc = KD_GSEYE_DEFAULT_MAP_COUNT;
	m_nCurMapDesc = 0;

	nResult = 1;
EXIT0:
	return nResult;
}

int KGameserverEyes::Uninit()
{
	//if (m_pMapDesc)
	{
		//free(m_pMapDesc);
		//m_pMapDesc = NULL;
		m_nMapDesc = 0;
		m_nCurMapDesc = 0;
	}
	return KEyes::Uninit();
}

int KGameserverEyes::Active()
{
	KEyes::Active();

	DoPing(g_cOrpgWorld.m_dwLogicFrames);
	DoSendCustomInfo(g_cOrpgWorld.m_dwLogicFrames);
	DoSendPlayerCount(g_cOrpgWorld.m_dwLogicFrames);
	//DoSendMapInfo(g_cOrpgWorld.m_dwLogicFrames);

	return 1;
}

int KGameserverEyes::OnProtocal(BYTE byProtocal, BYTE* pData, size_t uDataLen)
{
	switch (byProtocal)
	{
	case g2l_header_def:
		OnG2LCommonProtocol(pData, uDataLen);
		break;

	case e2l_header_def:
		OnE2LCommonProtocol(pData, uDataLen);
		break;

	case e2l_exit_def:
		OnE2LStopProtocol( pData, uDataLen );
		break;

	default:
		QLogPrintf(LOG_DEBUG, "Unknown protocol %u from guard", byProtocal);
		break;
	}
	return 1;
}

VOID KGameserverEyes::ExecuteGmCmd(e2l_gmcmd* pCmd, LPCSTR pszCmd)
{
	ScriptSafe rScript = g_cScriptManager.GetSafeScript();
	rScript->LoadBuffer(pszCmd);
	BOOL bResult = rScript->DoCall(1,0);
	if (bResult)
	{
		if (pCmd->byAction == 0)
		{
			BYTE byResult = emKGMCMD_RESULT_FAILURE;
			QLuaScript::KE_DATA_TYPE nResType = rScript->GetType(-1);
			if (nResType != QLuaScript::KE_DATA_TYPE_NUMBER || rScript->GetInt(-1) != 0)
			{
				byResult = emKGMCMD_RESULT_SUCCESS;
			}
			// 操作
			PostGmResult(pCmd->nSession,
				pCmd->byAction,
				byResult,
				pCmd->byAsker,
				"");
		}
		else
		{
			// 获取数据
			LPCSTR pszResult = rScript->GetStr(-1);
			if (pszResult)
			{
				PostGmResult(pCmd->nSession,
					pCmd->byAction,
					static_cast<BYTE>(emKGMCMD_RESULT_SUCCESS),
					pCmd->byAsker,
					pszResult);
			}
			else
			{
				PostGmResult(pCmd->nSession,
					pCmd->byAction,
					static_cast<BYTE>(emKGMCMD_RESULT_FAILURE),
					pCmd->byAsker,
					"");
			}
		}
	}
	else
	{
		PostGmResult(pCmd->nSession,
			static_cast<BYTE>(emKGMCMD_RESULT_WRONGCMD),
			pCmd->byAction,
			pCmd->byAsker,
			"");
	}
}

BOOL KGameserverEyes::ExecuteMappedGMCmd(e2l_gmmapcmd* pCmd)
{
	QCONFIRM_RET_FALSE(pCmd);
	INT nSession = pCmd->nSession;
	BYTE byAsker = pCmd->byAsker;
	LPCSTR pszKey = pCmd->szCmdKey;
	LPCSTR pszData = LPCSTR(pCmd + 1);

	ScriptSafe rScript = g_cScriptManager.GetSafeScript();

	rScript->CallTableFunction("GM", "DoMappedCmdKingEye", 2, "sdds", pszKey, nSession, byAsker, pszData);

	INT nRet = rScript->GetInt(-2);
	LPCSTR pszResult = rScript->GetStr(-1);
	if (nRet != 1)
	{
		PostGmResult(pCmd->nSession,
			static_cast<BYTE>(emKGMCMD_RESULT_WRONGCMD),
			1,
			pCmd->byAsker,
			pszResult ? pszResult : "");
		return FALSE;
	}
	return TRUE;
}

int KGameserverEyes::OnE2LCommonProtocol(BYTE* pData, size_t nLen)
{
	KSubProtocolHead* pHead = (KSubProtocolHead*)pData;
	//printf("E2LCommand: %d\n", pHead->wSubProtocol);
	switch (pHead->wSubProtocol)
	{
	case e2l_gmcmd_def:
		{
			e2l_gmcmd* pCmd = (e2l_gmcmd*)pData;
			if (nLen < sizeof(e2l_gmcmd) || nLen != sizeof(e2l_gmcmd) + pCmd->nCmdLen ||
				pCmd->nCmdLen <= 0)
			{
				ASSERT(FALSE);
				break;
			}
			pCmd->szName[sizeof(pCmd->szName) - 1] = '\0';
			LPSTR pszCmd = (LPSTR)(pCmd + 1);
			pszCmd[pCmd->nCmdLen - 1] = '\0';
			ExecuteGmCmd(pCmd, pszCmd);
			QLogPrintf(LOG_INFO, "[GM] from KingEye '%s'", pszCmd);
		}
		break;
	case e2l_queryrolelist_def:	// 查询角色列表
		{
			if (nLen != sizeof(e2l_queryrolelist))
			{
				ASSERT(FALSE);
				break;
			}
			e2l_queryrolelist* pCmd = (e2l_queryrolelist*)pData;
			//QueryRoleList(pCmd->nSession, pCmd->szAccount);
			printf("E2LCommand: e2l_queryrolelist_def '%s'\n", pCmd->szAccount);
		}
		break;
	case e2l_mappedgmcmd_def:
		{
			e2l_gmmapcmd* pCmd = (e2l_gmmapcmd*)pData;
			if (nLen < sizeof(e2l_gmmapcmd) || 
				nLen != sizeof(e2l_gmmapcmd) + pCmd->nParamLength)
			{
				ASSERT(FALSE);
				break;
			}
			ExecuteMappedGMCmd(pCmd);
			QLogPrintf(LOG_INFO, "[GM] from KingEye mapped '%s' '%.*s'", pCmd->szCmdKey, pCmd->nParamLength, pCmd + 1);
		}
		break;
	default:
		Q_Error("E2L subprotocol unrecognized: " << (INT)pHead->wSubProtocol);
	}
	return 1;
}

int KGameserverEyes::OnG2LCommonProtocol(BYTE* pData, size_t uDataLen)
{
	int nResult = 0;
	KSubProtocolHead* pHead = (KSubProtocolHead*)pData;

	PROCESS_ERROR(uDataLen == sizeof(KSubProtocolHead));

	/* 请求进程id */
	if (pHead->wSubProtocol == emKPROTOCOL_G2L_PIDREQ)
	{
		KPROTOCOL_L2G_PIDRET sRet;
		sRet.wProtocol = l2g_header_def;
		sRet.wSubProtocol = emKPROTOCOL_L2G_PIDRET;
		sRet.nPid = getpid();
		sRet.wServer = 0;
		m_Stream.PutPack(&sRet, sizeof(sRet));
	}

	nResult = 1;
EXIT0:
	return nResult;
}

int KGameserverEyes::OnE2LStopProtocol( BYTE* pData, size_t uDataLen )
{
	g_GetGameServerInterface()->ShutDown();
	return 1;
}

int KGameserverEyes::SendPlayerCount(int nOnline)
{
	int nResult = 0;
	l2e_PlayerCount* pPlayerCnt = NULL;
	size_t uPackLen = 0;
	int* pnCount = NULL;
	int i = 0;

	uPackLen = sizeof(l2e_PlayerCount) + sizeof(int) * emKGUARD_PLAYERCOUNT_COUNT;
	if ( uPackLen > m_uBufferLen )
	{
		if ( m_pszBuffer )
			free( m_pszBuffer );
		m_uBufferLen = 0;
		m_pszBuffer = NULL;
		m_pszBuffer = (char*)malloc(uPackLen);
		PROCESS_ERROR(m_pszBuffer);
		m_uBufferLen = uPackLen;
	}
	pPlayerCnt = (l2e_PlayerCount*)m_pszBuffer;
	pPlayerCnt->wProtocol = l2e_header_def;
	pPlayerCnt->wSubProtocol = l2e_PlayerCount_def;
	pPlayerCnt->uCount = emKGUARD_PLAYERCOUNT_COUNT;
	pPlayerCnt->wServer = 0;

	pnCount = (int*)(pPlayerCnt + 1);
	for ( i = 0; i < emKGUARD_PLAYERCOUNT_COUNT; i++ )
	{
		pnCount[i] = 0;
	}
	pnCount[emKGUARD_PLAYERCOUNT_TOTOAL] = nOnline;

	m_Stream.PutPack(pPlayerCnt, uPackLen);

	nResult = 1;
EXIT0:
	return nResult;
}
//
//int KGameserverEyes::SendMapInfo( l2e_update_map_list::KMapDesc* pMapInfo, int nCount )
//{
//	int nResult = 0;
//	l2e_update_map_list* pMap = NULL;
//	size_t uPackLen = 0;
//
//	uPackLen = sizeof(l2e_update_map_list) + sizeof(l2e_update_map_list::KMapDesc) * nCount;
//	if ( uPackLen > m_uBufferLen )
//	{
//		if ( m_pszBuffer )
//			free( m_pszBuffer );
//		m_pszBuffer = NULL;
//		m_uBufferLen = 0;
//		m_pszBuffer = (char*)malloc(uPackLen);
//		PROCESS_ERROR(m_pszBuffer);
//		m_uBufferLen = uPackLen;
//	}
//	pMap = (l2e_update_map_list*)m_pszBuffer;
//	pMap->wProtocol = l2e_header_def;
//	pMap->wSubProtocol = l2e_update_map_list_def;
//	pMap->wServer = 0;
//	pMap->nCount = nCount;
//	memcpy( pMap->MapDesc, pMapInfo, sizeof(l2e_update_map_list::KMapDesc) * nCount );
//
//	m_Stream.PutPack(pMap, uPackLen);
//
//	nResult = 1;
//EXIT0:
//	return nResult;
//}

int KGameserverEyes::SetPingInterval( unsigned int uIntervalSec )
{
	m_dwPingInterval = uIntervalSec * GAME_FPS;
	return 1;
}

int KGameserverEyes::SetSendCustomInfoInterval( unsigned int uIntervalSec )
{
	m_dwSendCustomInfoInterval = uIntervalSec * GAME_FPS;
	return 1;
}

int KGameserverEyes::SetSendPlayerCountInterval( unsigned int uIntervalSec )
{
	m_dwSendPlayerCountInterval = uIntervalSec * GAME_FPS;
	return 1;
}

int KGameserverEyes::SetSendMapInfoInterval( unsigned int uIntervalSec )
{
	m_dwSendMapInfoInterval = uIntervalSec * GAME_FPS;
	return 1;
}

int KGameserverEyes::DoSendMapInfo( DWORD dwFrames )
{
	if ( dwFrames > m_dwNextSendMapInfoFrame )
	{
		GetMapDescInfoInC();
		//SendMapInfo( m_pMapDesc, m_nCurMapDesc );
		m_dwNextSendMapInfoFrame = dwFrames + m_dwSendMapInfoInterval;
	}
	return 1;
}

int KGameserverEyes::DoSendCustomInfo(DWORD dwFrames)
{
	if ( dwFrames > m_dwNextSendCustomInfoFrame )
	{
		std::string strCustomInfo;
		GetCustomInfoInScript(strCustomInfo);
		GetIntranetStatus(strCustomInfo);
		SendCustomInfo(strCustomInfo.c_str());
		m_dwNextSendCustomInfoFrame = dwFrames + m_dwSendCustomInfoInterval;
	}
	return 1;
}

int KGameserverEyes::DoSendPlayerCount(DWORD dwFrames)
{
	if ( dwFrames > m_dwNextSendPlayerCountFrame )
	{
		INT nOnline = g_cPlayerMgr.GetPlayerCount();
		SendPlayerCount(nOnline);
		m_dwNextSendPlayerCountFrame = dwFrames + m_dwSendPlayerCountInterval;
	}
	return 1;
}

int KGameserverEyes::DoPing(DWORD dwFrames)
{
	if ( dwFrames > m_dwNextPingFrame )
	{
		SendPing();
		m_dwNextPingFrame = dwFrames + m_dwPingInterval;
	}
	return 1;
}

int KGameserverEyes::GetCustomInfoInScript(std::string& rStrInfo)
{
	LPCSTR lpszCustomInfo = NULL;
	size_t                  uStrLen             = 0;
	int                     nWorldIndex         = 0;
	size_t                  uMemory             = 0;
	int                     nPlayerCount        = 0;
	int                     nConnectionCount    = 0;
	float                   fClientFlux             = 0.0f;
	int                     nPID                = 0;
	float                   fUpTraffic          = 0.0f;
	float                   fDownTraffic        = 0.0f;
	CHAR					szTmp[1024]			= {0};
	ScriptSafe				rSafeScript			= g_cScriptManager.GetSafeScript();

	rSafeScript->CallTableFunction("QMonitor", "GetRunStat", 1, "");
	lpszCustomInfo = rSafeScript->GetStr(-1);
	if (lpszCustomInfo)
		rStrInfo += lpszCustomInfo;

	m_fLastPerformance  = m_fLastPerformance * 0.8f + g_cOrpgWorld.m_nIdleFrame * 0.2f;
	nWorldIndex         = g_RelayClient.GetWorldIndex();
	nPID                = (int)getpid();
	fUpTraffic          = g_RelayClient.GetUpTraffic() / 1024.0f;
	fDownTraffic        = g_RelayClient.GetDownTraffic() / 1024.0f;

	extern SIZE_T _GetUsedMemSize();
	uMemory             = _GetUsedMemSize();

	m_fLastClientFlux = m_fLastClientFlux * 0.9f + (float)fClientFlux * 0.1f;

	uStrLen = snprintf(
		szTmp, sizeof(szTmp),
		"------------------GS%.2d-----------------------\n"
		"Performance ... ... ... %.1f\n"
		"Memory ... .... ... ... %.3f MB\n"
		"Connection ... .... ... %d\n"
		"Center up traffic .. .. %.1f KB/s\n"
		"Center down traffic ... %.1f KB/s\n"
		"-----------------------------------------------\n"
		"IMG: %s[%d]\n",
		nWorldIndex,
		m_fLastPerformance,
		uMemory / 1024.0f / 1024.0f,
		nConnectionCount,
		fUpTraffic,
		fDownTraffic,
		m_szSelfName,
		nPID
		);
	LOG_PROCESS_ERROR(uStrLen > 0);

	rStrInfo += szTmp;
EXIT0:
	return TRUE;
}

int KGameserverEyes::GetMapDescInfoInC()
{
	int nResult = 0;
//	int nRet = 0;
//	int nIndex = 0;
//	INT nSceneCount = 0;
//	IKScene* piScene = NULL;
//	IKScene* piNextScene = NULL;
//
//	nSceneCount = g_SceneMgr.GetCount();
//	if ( nSceneCount > m_nMapDesc )
//	{
//		if (m_pMapDesc)
//			free(m_pMapDesc);
//		m_pMapDesc = (l2e_update_map_list::KMapDesc*)malloc( (nSceneCount + KD_GSEYE_DEFAULT_MAP_COUNT) * sizeof(l2e_update_map_list::KMapDesc) );
//		PROCESS_ERROR(m_pMapDesc);
//		m_nMapDesc = nSceneCount + KD_GSEYE_DEFAULT_MAP_COUNT;
//	}
//
//	nIndex = 0;
//	piScene = g_SceneMgr.FirstScene();
//	while (piScene)
//	{
//		piNextScene = g_SceneMgr.NextScene();
//
//		QStrCpyLen( m_pMapDesc[nIndex].szName, piScene->GetName(), 16 );
//		m_pMapDesc[nIndex].dwMapID = piScene->GetTemplateId();
//		m_pMapDesc[nIndex].nCopyIndex = piScene->GetId();
//		m_pMapDesc[nIndex].nNpcCount = piScene->GetNpcCount();
//		m_pMapDesc[nIndex].nPlayerCount = piScene->GetPlayerCount();
//		m_pMapDesc[nIndex].nTotalRegion = piScene->GetRegionCount();
//		m_pMapDesc[nIndex].nRegionActive = m_pMapDesc[nIndex].nTotalRegion;
//
//		piScene = piNextScene;
//		nIndex++;
//	}
//	m_nCurMapDesc = nSceneCount;
//
//	nResult = 1;
//EXIT0:
	return nResult;
}

int KGameserverEyes::GetIntranetStatus( std::string& strCustomInfo )
{
	strCustomInfo += "Intra-net Status --------------------------------\n";

	KSOCKET_KINGEYES_INFO sSockInfo;
	ZeroStruct(sSockInfo);
	//g_piGcConnect->GetSocketInfo(&sSockInfo);

	CHAR szTmp[1024] = "";
	//snprintf(szTmp, sizeof(szTmp),
	//	"GameCenter %u/%u, ",
	//	sSockInfo.dwCurrentConnection,
	//	sSockInfo.dwTotalConnection);

	if (sSockInfo.dwCurrentConnection != sSockInfo.dwTotalConnection)
	{
		strCustomInfo += "<font color=red>";
		strCustomInfo += szTmp;

		strCustomInfo += "</font>";
	}
	else
	{
		strCustomInfo += szTmp;
	}

	return TRUE;
}
#endif
