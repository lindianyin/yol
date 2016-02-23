
#include "stdafx.h"
#include "onlinegameworld/klogwritter.h"
#include "ksocketinterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KLogWritter KLogWritter::ms_logWritter;
BOOL KLogWritter::bIsLogin = FALSE;

KLogWritter::KLogWritter() : m_nIdentity(1)
{

}

KLogWritter::~KLogWritter()
{

}

VOID KLogWritter::DoLogin()
{
	if (KLogWritter::bIsLogin == FALSE)
	{
		LogSever_Login("gate0101");
		KLogWritter::bIsLogin = TRUE;
	}
}

VOID KLogWritter::LogSever_PlayerLog(LPCSTR pszAccount, LPCSTR pszRole, KE_PLAYERLOG_TYPE nType, LPCSTR pszTarget, LPCSTR pszComment /* = NULL */)
{
	DoLogin();
	QCONFIRM(pszAccount && pszRole && nType >= 0);
	if (nType < 0 || nType >= emKPLAYERLOG_TYPE_COUNT)
	{
		QCONFIRM(FALSE);
		return;
	}

	KGPlayerActionLog* pLog = (KGPlayerActionLog*)m_szBuff;
	pLog->ProtocolType = LOGC2S_LOG_PLAYER_ACTION;
	pLog->ulIdentity = m_nIdentity++;
	pLog->uActionLevel = 1;
	pLog->eActionType = static_cast<PLAYER_ACTION_TYPE>(nType);

	QCONFIRM(pszTarget);
	QStrCpyLen(pLog->szActionTarget, pszTarget, sizeof(pLog->szActionTarget));
	QCONFIRM(pszAccount);
	QStrCpyLen(pLog->szAccountName, pszAccount, sizeof(pLog->szAccountName));
	QCONFIRM(pszRole);
	QStrCpyLen(pLog->szRoleName, pszRole, sizeof(pLog->szRoleName));

	INT nLen = 0;
	if (pszComment != NULL && pszComment[0] != '\0')
	{
		nLen = strlen(pszComment);
		if (nLen > emKCOMMENT_MAXLEN)
			nLen = emKCOMMENT_MAXLEN;
		memcpy(pLog->pbyData, pszComment, nLen);
	}
	pLog->pbyData[nLen] = '\0';
	pLog->uCommentSize = nLen + 1;

	//g_piOtherConnect->Send(emKOTHER_CONNECT_LOGSERVER, 0, pLog, sizeof(KGPlayerActionLog) + pLog->uCommentSize - 1);
}

VOID KLogWritter::LogSever_Login(LPCSTR pszGatewayName)
{
	LOGSERVER_LOGIN sData;
	sData.cProtocol = LOGC2S_LOG_LOGIN;
	sData.ulIdentity = m_nIdentity++;
	QStrCpyLen(sData.szGatewayName, pszGatewayName, sizeof(sData.szGatewayName));
	//g_piOtherConnect->Send(emKOTHER_CONNECT_LOGSERVER, 0, &sData, sizeof(sData));
}

VOID KLogWritter::LogSever_DataAnalysisLog(INT32 nLogType, LPCSTR pszComment /* = NULL */)
{
	//QCONFIRM(nLogType < DATA_ANALYSIS_LOG_TYPE_COUNT && nLogType >= 0);

	//DoLogin();

	//KGDataAnalysisLog* pLog = (KGDataAnalysisLog*)m_szBuff;
	//pLog->ProtocolType = LOGC2S_LOG_DATA_ANALYSIS;
	//pLog->ulIdentity = m_nIdentity++;
	//pLog->uLogLevel = 1;
	//pLog->eLogType = static_cast<DATA_ANALYSIS_LOG_TYPE>(nLogType);

	//INT nLen = 0;
	//if (pszComment != NULL && pszComment[0] != '\0')
	//{
	//	nLen = strlen(pszComment);
	//	if (nLen > emKCOMMENT_MAXLEN)
	//		nLen = emKCOMMENT_MAXLEN;
	//	memcpy(pLog->pbyData, pszComment, nLen);
	//}
	//pLog->pbyData[nLen] = '\0';
	//pLog->uCommentSize = nLen + 1;

	//g_piOtherConnect->Send(emKOTHER_CONNECT_LOGSERVER, 0, pLog, sizeof(KGDataAnalysisLog) + pLog->uCommentSize - 1);
}

VOID KLogWritter::LogSever_DataAnalysisLogV(INT32 nLogType, ...)
{
	//QCONFIRM(nLogType < DATA_ANALYSIS_LOG_TYPE_COUNT && nLogType >= 0);

	//DoLogin();

	//KGDataAnalysisLog* pLog = (KGDataAnalysisLog*)m_szBuff;
	//pLog->ProtocolType = LOGC2S_LOG_DATA_ANALYSIS;
	//pLog->ulIdentity = m_nIdentity++;
	//pLog->uLogLevel = 1;
	//pLog->eLogType = static_cast<DATA_ANALYSIS_LOG_TYPE>(nLogType);
	//
	//va_list vl;
	//va_start(vl, nLogType);
	//CHAR szLog[1024 * 256];
	//::memset(szLog, 0, sizeof(szLog));
	//StrConcat(szLog, vl);
	//va_end(vl);

	//INT nLen = strlen(szLog);
	//if (nLen > 0 && szLog[0] != '\0')
	//{
	//	if (nLen > emKCOMMENT_MAXLEN)
	//		nLen = emKCOMMENT_MAXLEN;
	//	memcpy(pLog->pbyData, szLog, nLen);
	//}

	//pLog->pbyData[nLen] = '\0';
	//pLog->uCommentSize = nLen + 1;

	//g_piOtherConnect->Send(emKOTHER_CONNECT_LOGSERVER, 0, pLog, sizeof(KGDataAnalysisLog) + pLog->uCommentSize - 1);
}

BOOL KLogWritter::StrConcat(LPSTR pszLog, va_list vl)
{
	CHAR buf[1024 * 256];
	::memset(buf, 0, sizeof(buf));
	LPSTR pszArg = va_arg(vl, LPSTR);
	while (pszArg)
	{
		strcat(buf, pszArg);
		strcat(buf, "\t");
		pszArg = va_arg(vl, LPSTR);
	}

	INT nLen = (INT)strlen(buf);
	if (nLen >= 1)
		buf[nLen - 1] = '\0';
	strcat(pszLog, buf);
	return TRUE;
}
// -------------------------------------------------------------------------
