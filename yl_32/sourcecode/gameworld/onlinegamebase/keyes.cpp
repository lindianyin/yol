
#include "stdafx.h"
#include "onlinegamebase/keyes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


KEyes::KEyes():
m_bInited(0),
m_pszBuffer(NULL),
m_uBufferLen(0)
{

}

KEyes::~KEyes()
{

}

int KEyes::Init(int nKey)
{
	int nResult = 0;
	int nRet = 0;

	nRet = m_Stream.Initialize(nKey, 200 * 1024);
	PROCESS_ERROR(nRet);

	m_pszBuffer = (char*)malloc(KD_KEYES_INIT_BUFF_LEN);
	PROCESS_ERROR(m_pszBuffer);
	m_uBufferLen = KD_KEYES_INIT_BUFF_LEN;

	m_bInited = 1;
	nResult = 1;
EXIT0:
	if (0 == nResult)
	{
			m_Stream.UnInitialize();
			if (m_pszBuffer)
			{
				free(m_pszBuffer);
				m_uBufferLen = 0;
				m_pszBuffer = NULL;
			}
	}
	return nResult;
}

int KEyes::Uninit()
{
	m_Stream.UnInitialize();
	if (m_pszBuffer)
	{
		free(m_pszBuffer);
		m_uBufferLen = 0;
		m_pszBuffer = NULL;
	}
	m_bInited = 0;
	return 1;
}

int KEyes::Active()
{
	BYTE byProtocol = 0;
	char* pData = NULL;
	size_t uDataLen = 0;

	while(1)
	{
		pData = m_Stream.GetPack(uDataLen);
		if (NULL == pData) 
			break;
		assert(uDataLen > 0);
		
		byProtocol = *((BYTE*)pData);

		OnProtocal(byProtocol, (BYTE*)pData, uDataLen);

		m_Stream.PopPack();
	}
	return 1;
}

//int KEyes::SendWarnningInfo(const char* pszWarningInfo)
//{
//	int nResult = 0;
//	l2e_warning_info* pWarn = NULL;
//	size_t uInfoLen = 0;
//	size_t uPackLen = 0;
//
//	PROCESS_ERROR(pszWarningInfo);
//	uInfoLen = strlen(pszWarningInfo) + 1;
//	uPackLen = sizeof(l2e_warning_info) + uInfoLen;
//	if (uPackLen > m_uBufferLen)
//	{
//		if (m_pszBuffer)
//			free(m_pszBuffer);
//		m_uBufferLen = 0;
//		m_pszBuffer = (char*)malloc(uPackLen);
//		PROCESS_ERROR(m_pszBuffer);
//		m_uBufferLen = uPackLen;
//	}
//
//	pWarn = (l2e_warning_info*)m_pszBuffer;
//	pWarn->wProtocol = l2e_header_def;
//	pWarn->wSubProtocol = l2e_warning_info_def;
//	pWarn->wServer = 0;
//	pWarn->nLevel = 1;
//	pWarn->uDataLen = uInfoLen;
//	strcpy((char*)pWarn->byData, pszWarningInfo);
//
//	m_Stream.PutPack(pWarn, uPackLen);
//
//	nResult = 1;
//EXIT0:
//	return nResult;
//}

int KEyes::SendCustomInfo(const char* pszCustomInfo)
{
	int nResult = 0;
	l2e_update_custom_info* pInfo = NULL;
	size_t uPackLen = 0;
	size_t uInfoLen = 0;

	PROCESS_ERROR(pszCustomInfo);

	uInfoLen = strlen(pszCustomInfo) + 1;
	uPackLen = sizeof(l2e_update_custom_info) - 1 + uInfoLen;
	if (uPackLen > m_uBufferLen)
	{
		if (m_pszBuffer)
			free(m_pszBuffer);
		m_uBufferLen = 0;
		m_pszBuffer = (char*)malloc(uPackLen);
		PROCESS_ERROR(m_pszBuffer);
		m_uBufferLen = uPackLen;
	}
	pInfo = (l2e_update_custom_info*)m_pszBuffer;
	pInfo->wProtocol = l2e_header_def;
	pInfo->wSubProtocol = l2e_update_custom_info_def;
	pInfo->wServer = 0;
	pInfo->uDataLen = uInfoLen;
	strcpy((char*)pInfo->byData, pszCustomInfo);

	m_Stream.PutPack(pInfo, uPackLen);

	nResult = 1;
EXIT0:
	return nResult;
}

VOID KEyes::PostGmResult(INT nSession, BYTE byAction, BYTE byResult, BYTE byAsker, LPCSTR pszResult)
{
	ASSERT(pszResult);
	CONST INT MAX_MSGLEN = 64000;
	CHAR szBuf[sizeof(l2e_gmcmd) + MAX_MSGLEN];
	l2e_gmcmd* pRet = (l2e_gmcmd*)szBuf;
	LPSTR pszRet = (LPSTR)(pRet + 1);
	INT nLen = strlen(pszResult) + 1;
	if (nLen > MAX_MSGLEN)
		nLen = MAX_MSGLEN;
	QStrCpyLen(pszRet, pszResult, nLen);
	pszRet[nLen - 1] = '\0';
	pRet->nLen		= nLen;
	pRet->wProtocol	= l2e_gmcmd_def;
	pRet->wServer	= -1;
	pRet->byAction  = byAction;
	pRet->byResult  = byResult;
	pRet->nSession	= nSession;
	pRet->byAsker = byAsker;
	m_Stream.PutPack(pRet, sizeof(l2e_gmcmd) + pRet->nLen);
}

int KEyes::SendPing()
{
	KSubProtocolHead sPing;

	sPing.wProtocol = l2e_header_def;
	sPing.wSubProtocol = l2e_ping_def;
	sPing.wServer = 0;

	m_Stream.PutPack(&sPing, sizeof(sPing));
	return 1;
}

int KEyes::OnProtocal(BYTE byProtocal, BYTE* pData, size_t uDataLen)
{
	return 1;
}
