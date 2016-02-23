
#ifdef GAME_CENTER

#include "stdafx.h"
#include "kgamecentereyes.h"
#include "kgamecenter.h"
#include "onlinegamebase/kscriptmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KD_GSEYE_DEFAULT_INTERVAL 7000


KGameCenterEyes::KGameCenterEyes():
m_dwPingInterval(KD_GSEYE_DEFAULT_INTERVAL),
m_dwSendCustomInfoInterval(KD_GSEYE_DEFAULT_INTERVAL),
m_dwSendPlayerCountInterval(KD_GSEYE_DEFAULT_INTERVAL),
m_dwNextPingFrame(0),
m_dwNextSendCustomInfoFrame(0),
m_dwNextSendPlayerCountFrame(0)
{

}

INT KGameCenterEyes::Init(INT nServerId)
{
	int nResult = 0;
	int nRet = 0;

	LOG_PROCESS_ERROR(nServerId > 0);
	nRet = KEyes::Init(emKGUARDSERVER_FIRSTID + emKGUARDSERVER_GAMECENTER_START + nServerId - 1);
	PROCESS_ERROR(nRet);

	nResult = 1;
EXIT0:
	return nResult;
}

INT KGameCenterEyes::Uninit()
{
	return KEyes::Uninit();
}

INT KGameCenterEyes::Active()
{
	DWORD dwTickNow = GetTickCount();
	KEyes::Active();

	DoPing(dwTickNow);
	DoSendCustomInfo(dwTickNow);
	DoSendPlayerCount(dwTickNow);

	return 1;
}

INT KGameCenterEyes::OnProtocal(BYTE byProtocal, BYTE* pData, size_t uDataLen)
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

VOID KGameCenterEyes::ExecuteGmCmd(e2l_gmcmd* pCmd, LPCSTR pszCmd)
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

BOOL KGameCenterEyes::ExecuteMappedGMCmd(e2l_gmmapcmd* pCmd)
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

INT KGameCenterEyes::OnE2LCommonProtocol(BYTE* pData, size_t nLen)
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

INT KGameCenterEyes::OnG2LCommonProtocol(BYTE* pData, size_t uDataLen)
{
	INT nResult = 0;
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

INT KGameCenterEyes::OnE2LStopProtocol( BYTE* pData, size_t uDataLen )
{
	g_cGameCenter.Shutdown();
	return 1;
}

INT KGameCenterEyes::SendPlayerCount(INT nOnline)
{
	INT nResult = 0;
//	l2e_PlayerCount* pPlayerCnt = NULL;
//	size_t uPackLen = 0;
//	INT* pnCount = NULL;
//	INT i = 0;
//
//	uPackLen = sizeof(l2e_PlayerCount) + sizeof(int) * emKGUARD_PLAYERCOUNT_COUNT;
//	if ( uPackLen > m_uBufferLen )
//	{
//		if ( m_pszBuffer )
//			free( m_pszBuffer );
//		m_uBufferLen = 0;
//		m_pszBuffer = NULL;
//		m_pszBuffer = (char*)malloc(uPackLen);
//		PROCESS_ERROR(m_pszBuffer);
//		m_uBufferLen = uPackLen;
//	}
//	pPlayerCnt = (l2e_PlayerCount*)m_pszBuffer;
//	pPlayerCnt->wProtocol = l2e_header_def;
//	pPlayerCnt->wSubProtocol = l2e_PlayerCount_def;
//	pPlayerCnt->uCount = emKGUARD_PLAYERCOUNT_COUNT;
//	pPlayerCnt->wServer = 0;
//
//	pnCount = (INT*)(pPlayerCnt + 1);
//	for ( i = 0; i < emKGUARD_PLAYERCOUNT_COUNT; i++ )
//	{
//		pnCount[i] = 0;
//	}
//	pnCount[emKGUARD_PLAYERCOUNT_TOTOAL] = nOnline;
//
//	m_Stream.PutPack(pPlayerCnt, uPackLen);
//
//	nResult = 1;
//EXIT0:
	return nResult;
}

INT KGameCenterEyes::DoSendCustomInfo(DWORD dwFrames)
{
	if ( dwFrames > m_dwNextSendCustomInfoFrame )
	{
		std::string strCustomInfo;
		GetCustomInfoInScript(strCustomInfo);

		SendCustomInfo(strCustomInfo.c_str());
		m_dwNextSendCustomInfoFrame = dwFrames + m_dwSendCustomInfoInterval;
	}
	return 1;
}

INT KGameCenterEyes::DoSendPlayerCount(DWORD dwFrames)
{
	//if ( dwFrames > m_dwNextSendPlayerCountFrame )
	//{
	//	INT nOnline = g_cPlayerMgr.GetPlayerCount();
	//	SendPlayerCount(nOnline);
	//	m_dwNextSendPlayerCountFrame = dwFrames + m_dwSendPlayerCountInterval;
	//}
	return 1;
}

INT KGameCenterEyes::DoPing(DWORD dwFrames)
{
	if ( dwFrames > m_dwNextPingFrame )
	{
		SendPing();
		m_dwNextPingFrame = dwFrames + m_dwPingInterval;
	}
	return 1;
}

INT KGameCenterEyes::GetCustomInfoInScript(std::string& rStrInfo)
{
	INT nResult = 0;
	LPCSTR lpszCustomInfo = NULL;

	//ScriptSafe rSafeScript = g_cScriptManager.GetSafeScript();
	//rSafeScript->CallTableFunction("GM", "GetGameCenterEyesCustomInfo", 1, "");
	//lpszCustomInfo = rSafeScript->GetStr(-1);
	//if (lpszCustomInfo)
	//	rStrInfo += lpszCustomInfo;

	nResult = TRUE;
	return nResult;
}

#endif
