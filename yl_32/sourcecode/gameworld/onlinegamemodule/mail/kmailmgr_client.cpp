/* -------------------------------------------------------------------------
//	文件名		：	kmailmgr_client.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/23 17:56:14
//	功能描述	：	邮件Client模块
//
// -----------------------------------------------------------------------*/

#ifdef GAME_CLIENT
#include "stdafx.h"
#include "onlinegamemodule/mail/kmailmgr_client.h"
#include "gcmodule/mail/kmailprotocol.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMailManager_Client KMailManager_Client::ms_inst;

KMailManager_Client::KMailManager_Client()
{
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
}

KMailManager_Client::~KMailManager_Client()
{
}

BOOL KMailManager_Client::ProcessGsProtocal(LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pbData);

	BOOL bResult = FALSE;
	KPTC_HEADER_BASE* pMsg = (KPTC_HEADER_BASE*)pbData;
	switch (pMsg->byProtocol)
	{
	case emKMAIL_S2C_PROTO_LOADOK:
		bResult = OnMailBoxLoadOK(pbData, uDataLen);
		break;
	case emKMAIL_S2C_PROTO_FETCHINFO:
		bResult = OnFetchMailInfo(pbData, uDataLen);
		break;
	case emKMAIL_S2C_PROTO_FETCHMAIL:
		bResult = OnFetchMail(pbData, uDataLen);
		break;
	case emKMAIL_S2C_PROTO_SENDRESULT:
		bResult = OnSendResult(pbData, uDataLen);
		break;
	case emKMAIL_S2C_PROTO_NEWMAIL:
		bResult = OnNewMail(pbData, uDataLen);
		break;
	case emKMAIL_S2C_PROTO_FETCHITEMOK:
		bResult = OnFetchItemOK(pbData, uDataLen);
		break;
	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

BOOL KMailManager_Client::OnMailBoxLoadOK(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	KMAIL_S2C_MAILLOADOK* pMsg = (KMAIL_S2C_MAILLOADOK*)pbData;
	if (uSize != sizeof(KMAIL_S2C_MAILLOADOK))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_S2C_MAILLOADOK size error!");
		return FALSE;
	}

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->GetGlobalF("Mail:OnMailBoxLoadOK");
	cSafeScript->PushNumber(pMsg->byShowMsg);
	cSafeScript->DoCall(0, 2);

	return TRUE;
}

BOOL KMailManager_Client::OnFetchMailInfo(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	KMAIL_S2C_PROTO_FETCHINFO* pMsg = (KMAIL_S2C_PROTO_FETCHINFO*)pbData;
	if (uSize <= sizeof(KMAIL_S2C_MAILLOADOK))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_S2C_PROTO_FETCHINFO size error!");
		return FALSE;
	}

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->GetGlobalF("Mail:OnFetchMailList");
	INT nCurrentTime = (INT)KSysService::Time(NULL);
	INT nIndex = 1;
	cSafeScript->PushTable();
	for (INT i = pMsg->wCount - 1; i >= 0; --i)
	{
		cSafeScript->PushTable();
		memset(m_szBuffer, 0, sizeof(m_szBuffer));
		sprintf(m_szBuffer, "%lld", pMsg->aryMails[i].nMailID);
		cSafeScript->PushString(m_szBuffer);
		cSafeScript->SetTableField("nMailId");
		cSafeScript->PushNumber(pMsg->aryMails[i].nStat);
		cSafeScript->SetTableField("nState");
		cSafeScript->PushNumber(pMsg->aryMails[i].nType);
		cSafeScript->SetTableField("nType");
		cSafeScript->PushNumber(pMsg->aryMails[i].nIconId);
		cSafeScript->SetTableField("nIconId");
		cSafeScript->PushString(pMsg->aryMails[i].szCaption);
		cSafeScript->SetTableField("szCaption");
		cSafeScript->PushString(pMsg->aryMails[i].szReceiver);
		cSafeScript->SetTableField("szReceiver");
		cSafeScript->PushString(pMsg->aryMails[i].szSender);
		cSafeScript->SetTableField("szSender");

		if (pMsg->aryMails[i].nSendTime > nCurrentTime)
			pMsg->aryMails[i].nSendTime = nCurrentTime;
		cSafeScript->PushNumber(pMsg->aryMails[i].nSendTime);
		cSafeScript->SetTableField("nTime");
		cSafeScript->SetTableIndex(nIndex++);
	}

	cSafeScript->DoCall(0, 2);
	return TRUE;
}

BOOL KMailManager_Client::OnFetchMail(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);
	
	KMAIL_S2C_PROTO_FETCHMAIL* pMsg = (KMAIL_S2C_PROTO_FETCHMAIL*)pbData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	
	cSafeScript->GetGlobalF("Mail:OnFetchMail");
	cSafeScript->PushTable();
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	sprintf(m_szBuffer, "%lld", pMsg->nMailID);
	cSafeScript->PushString(m_szBuffer);
	cSafeScript->SetTableField("nMailId");
	cSafeScript->PushNumber(pMsg->nIconId);
	cSafeScript->SetTableField("nIconId");
	cSafeScript->PushNumber(pMsg->nType);
	cSafeScript->SetTableField("nType");
	cSafeScript->PushString((LPCSTR)&pMsg->aryItem[pMsg->nItemCount]);
	cSafeScript->SetTableField("szContent");
	
	if (pMsg->nItemCount > 0)
	{
		KMAIL_ITEM_DATA* pItem = (KMAIL_ITEM_DATA*)&pMsg->aryItem[0];
		cSafeScript->PushTable();
		for (INT i = 0; i < pMsg->nItemCount; ++i)
		{
			cSafeScript->PushTable();
			cSafeScript->PushNumber(pItem->nItemIndex);
			cSafeScript->SetTableField("nItemIndex");
			cSafeScript->PushNumber(pItem->sItem.dwClassID);
			cSafeScript->SetTableField("nClassId");
			cSafeScript->PushNumber(pItem->sItem.wStackCount);
			cSafeScript->SetTableField("nCount");
			cSafeScript->PushNumber(pItem->sItem.dwBuyPrice);
			cSafeScript->SetTableField("nPrice");
			cSafeScript->PushNumber(pItem->sItem.byStateFlags);
			cSafeScript->SetTableField("nStateFlags");
			cSafeScript->PushNumber(pItem->sItem.byTimeoutType);
			cSafeScript->SetTableField("nTimeoutType");
			cSafeScript->PushNumber(pItem->sItem.dwTimeout);
			cSafeScript->SetTableField("nTimeout");
			cSafeScript->SetTableIndex(i + 1);

			pItem++;
		}
		cSafeScript->SetTableField("tbItem");
	}

	cSafeScript->DoCall(0, 2);
	return TRUE;
}

BOOL KMailManager_Client::OnSendResult(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	KMAIL_S2C_PROTO_SENDRESULT* pMsg = (KMAIL_S2C_PROTO_SENDRESULT*)pbData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->GetGlobalF("Mail:OnSendResult");
	cSafeScript->PushNumber(pMsg->nResult);
	cSafeScript->PushNumber(pMsg->nReasonType);
	if (pMsg->nFailReasonLen > 0)
	{
		INT nFailReasonLen = pMsg->nFailReasonLen;
		CHAR* pszFailReason = new CHAR[nFailReasonLen + 1];
		memset(pszFailReason, 0, nFailReasonLen + 1);
		QStrCpyLen(pszFailReason, (CHAR*)(pMsg + 1), nFailReasonLen + 1);

		cSafeScript->PushString(pszFailReason);
		cSafeScript->DoCall(0, 4);

		SAFE_DELETE(pszFailReason);
	}
	else
	{
		cSafeScript->DoCall(0, 3);
	}

	return TRUE;
}

BOOL KMailManager_Client::OnNewMail(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->GetGlobalF("Mail:OnNewMail");
	cSafeScript->DoCall(0, 1);

	return TRUE;
}

BOOL KMailManager_Client::OnFetchItemOK(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	KMAIL_S2C_PROTO_FETCHITEMOK* pMsg = (KMAIL_S2C_PROTO_FETCHITEMOK*)pbData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->GetGlobalF("Mail:OnFetchItemOK");
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	sprintf(m_szBuffer, "%lld", pMsg->nMailID);
	cSafeScript->PushString(m_szBuffer);
	cSafeScript->PushNumber(pMsg->byItemIndex);
	cSafeScript->DoCall(0, 3);

	return TRUE;
}

// -------------------------------------------------------------------------
#endif /* GAME_CLIENT */
