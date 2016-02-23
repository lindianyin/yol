/* -------------------------------------------------------------------------
//	文件名		：	kmailmgr_gs.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/23 17:46:28
//	功能描述	：	邮件GS模块
//
// -----------------------------------------------------------------------*/

#ifdef GAME_SERVER
#include "stdafx.h"
#include "onlinegamemodule/mail/kmailmgr_gs.h"
#include "onlinegamemodule/mail/kmailmodule.h"
#include "gcmodule/mail/kmailprotocol.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegamemodule/mail/kmailctrl.h"
#include "onlinegameworld/kplayermgr.h"
//#include "onlinegamemodule/item/kplayeritemop.h"
//#include "onlinegamemodule/item/kitem.h"
#include "onlinegamebase/kguidhelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KMailManager_GS KMailManager_GS::ms_inst;

KMailManager_GS::KMailManager_GS()
{
}

KMailManager_GS::~KMailManager_GS()
{
}

BOOL KMailManager_GS::ProcessGcProtocol(LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pbData);
	
	BOOL bResult = FALSE;
	KPTC_HEADER_BASE* pMsg = (KPTC_HEADER_BASE*)pbData;
	switch (pMsg->byProtocol)
	{
	case emKMAIL_G2S_PROTO_LOADOK:
		bResult = GameCenter_LoadOK(pbData, uDataLen);
		break;
	case emKMAIL_G2S_PROTO_FETCHINFO:
		bResult = GameCenter_FetchInfo(pbData, uDataLen);
		break;
	case emKMAIL_G2S_PROTO_FETCHMAIL:
		bResult = GameCenter_FetchMail(pbData, uDataLen);
		break;
	case emKMAIL_G2S_PROTO_FETCHITEM:
		bResult = GameCenter_FetchItem(pbData, uDataLen);
		break;
	case emKMAIL_G2S_PROTO_ADDMAIL:
		bResult = GameCenter_AddMail(pbData, uDataLen);
		break;
	case emKMAIL_G2S_PROTO_DELMAIL:
		bResult = GameCenter_DeleteMail(pbData, uDataLen);
		break;
	case emKMAIL_G2S_PROTO_CHECKRECEIVER:
		bResult = GameCenter_CheckReceiver(pbData, uDataLen);
		break;
	default:
		bResult = FALSE;
		break;
	}
	
	return bResult;
}

BOOL KMailManager_GS::GameCenter_LoadOK(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);
	
	KMAIL_G2S_PROTO_LOADOK *pMsg = (KMAIL_G2S_PROTO_LOADOK*)pbData;
	if (uSize !=  sizeof(KMAIL_G2S_PROTO_LOADOK)||
		pMsg->dwPlayerId == 0)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_G2S_PROTO_LOADOK size error!");
		return FALSE;
	}

	KPlayer* pPlayer = g_cPlayerMgr.GetById(pMsg->dwPlayerId);
	QCONFIRM_RET_FALSE(pPlayer);
	KMailCtrl* pMailCtrl = pPlayer->GetCtrl<KMailCtrl>();
	QCONFIRM_RET_FALSE(pMailCtrl);
	pMailCtrl->SetMailBoxLoaded(TRUE);
	
	//KMAIL_S2C_MAILLOADOK sNotify;
	//sNotify.byShowMsg = pMsg->byShowNotify;
	//return g_cMailModule.SendClientMsg(pMsg->dwPlayerId, sNotify);
	return TRUE;
}

BOOL KMailManager_GS::GameCenter_FetchInfo(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	KMAIL_G2S_PROTO_FETCHINFO *pMsg = (KMAIL_G2S_PROTO_FETCHINFO*)pbData;
	if (uSize < sizeof(KMAIL_G2S_PROTO_FETCHINFO) ||
		uSize != sizeof(KMAIL_G2S_PROTO_FETCHINFO) + pMsg->wCount * sizeof(KMAIL_PROTO_INFO))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_G2S_PROTO_FETCHINFO size error!");
		return FALSE;
	}

	INT nBufLen = sizeof(KMAIL_S2C_PROTO_FETCHINFO) + pMsg->wCount * sizeof(KMAIL_PROTO_INFO);
	if (nBufLen > sizeof(m_szBuffer))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_G2S_PROTO_FETCHINFO size too large!");
		return FALSE;
	}

	KMAIL_S2C_PROTO_FETCHINFO *pCmd = (KMAIL_S2C_PROTO_FETCHINFO*)m_szBuffer;
	*pCmd = KMAIL_S2C_PROTO_FETCHINFO();
	pCmd->wCount	   = pMsg->wCount;
	pCmd->wDataLen	   = nBufLen - 1;
	pCmd->nCurrentTime = pMsg->nCurrentTime;
	for (INT i = 0; i < pMsg->wCount; i++)
	{
		KMAIL_PROTO_INFO *pInfo = &pMsg->aryMails[i];
		pCmd->aryMails[i] = *pInfo;
	}
	return g_cMailModule.SendVarLenClientMsg(pMsg->dwPlayerId, *pCmd, nBufLen);
}

BOOL KMailManager_GS::GameCenter_FetchMail(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	KMAIL_G2S_PROTO_FETCHMAIL *pMsg = (KMAIL_G2S_PROTO_FETCHMAIL*)pbData;
	if (uSize <= sizeof(KMAIL_G2S_PROTO_FETCHMAIL) ||
		uSize != sizeof(KMAIL_G2S_PROTO_FETCHMAIL) + pMsg->wContentLen + pMsg->nItemCount * sizeof(KMAIL_ITEM_DATA))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_G2S_PROTO_FETCHMAIL size error!");
		return FALSE;
	}

	INT nBufLen = sizeof(KMAIL_S2C_PROTO_FETCHMAIL) + pMsg->wContentLen + pMsg->nItemCount * sizeof(KMAIL_ITEM_DATA);
	if (nBufLen > sizeof(m_szBuffer))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_G2S_PROTO_FETCHMAIL size too large!");
		return FALSE;
	}

	KMAIL_S2C_PROTO_FETCHMAIL *pCmd = (KMAIL_S2C_PROTO_FETCHMAIL*)m_szBuffer;
	*pCmd = KMAIL_S2C_PROTO_FETCHMAIL();
	pCmd->wContentLen = pMsg->wContentLen;
	pCmd->nMailID = pMsg->nMailID;
	pCmd->nIconId = pMsg->nIconId;
	pCmd->nType = pMsg->nType;
	pCmd->wDataLen = nBufLen - 1;
	pCmd->nItemCount = pMsg->nItemCount;
	for (INT i = 0; i < pMsg->nItemCount; i++)
	{
		KMAIL_ITEM_DATA *pItem = &pMsg->aryItem[i];
		pCmd->aryItem[i] = *pItem;
	}
	LPSTR pSrcContent = (LPSTR)&pMsg->aryItem[pMsg->nItemCount];
	LPSTR pDstContent = (LPSTR)&pCmd->aryItem[pMsg->nItemCount];
	QStrCpyLen(pDstContent, pSrcContent, pMsg->wContentLen);

	return g_cMailModule.SendVarLenClientMsg(pMsg->dwPlayerId, *pCmd, nBufLen);
}

BOOL KMailManager_GS::GameCenter_FetchItem(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	//KMAIL_G2S_PROTO_FETCHITEM *pMsg = (KMAIL_G2S_PROTO_FETCHITEM*)pbData;
	//if (uSize != sizeof(KMAIL_G2S_PROTO_FETCHITEM) ||
	//	pMsg->dwPlayerId == 0)
	//{
	//	QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_G2S_PROTO_FETCHMAIL size error!");
	//	return FALSE;
	//}

	//KPlayer* pPlayer = g_cPlayerMgr.GetById(pMsg->dwPlayerId);
	//if (!pPlayer)
	//{
	//	QLogPrintf(LOG_ERR, "[mail] KMailManager_GS::GameCenter_FetchItem playerid:%d player no exist!", pMsg->dwPlayerId);
	//	return FALSE;
	//}
	//KPlayerItemOp *pItemPlayer = pPlayer->GetCtrl<KPlayerItemOp>();
	//if (!pItemPlayer)
	//{
	//	QLogPrintf(LOG_ERR, "[mail] %s", "KMailManager_GS::GameCenter_FetchItem Get KPlayerItemOp failed!");
	//	return FALSE;
	//}

	//// 创建道具
	//SIZE_T uUsedDataLen = 0;
	//KItem* pItem = NULL;
	//INT nItemIdx = 0;
	//BYTE abyItemData[sizeof(KItemData_Common) + sizeof(DWORD)];
	//memset(abyItemData, 0, sizeof(abyItemData));
	//KItemData_Common* pItemData = (KItemData_Common*)abyItemData;
	//*pItemData = pMsg->sItem;
	//LPCBYTE pData = (LPCBYTE)abyItemData;
	//if (!pItemPlayer->CreateItemFromData(pData, sizeof(abyItemData), uUsedDataLen, &pItem, &nItemIdx))
	//{
	//	// 创建道具失败
	//	QLogPrintf(LOG_ERR, "[mail] %s", "KMailManager_GS::GameCenter_FetchItem CreateItemFromData failed!");
	//	return FALSE;
	//}

	//QLuaScript& sc = g_cScriptManager.GetMainScript();
	//KScriptSafeCall sfc(sc);
	//
	//// 判断背包是否还有空格
	//BOOL bResult = sc.CallTableFunction(
	//	"Item",
	//	"CanAddItemIntoBag",
	//	1,
	//	"odd",
	//	pPlayer->GetScriptInterface(),
	//	pItem->dwClassID,
	//	pItem->wStackCount);
	//if (!bResult)
	//{
	//	// 执行脚本一定成功
	//	ASSERT(FALSE);
	//	QLogPrintf(LOG_ERR, "[mail] %s", "KMailManager_GS::GameCenter_FetchItem do Item:CanAddItemIntoBag failed!");
	//	return FALSE;
	//}
	//bResult = (BOOL)sc.GetInt(-1);
	//if (!bResult)
	//{
	//	QLogPrintf(LOG_ERR, "[mail] %s", "KMailManager_GS::GameCenter_FetchItem Can't AddItemIntoBag!");
	//	return FALSE;
	//}

	//// 获取加入背包类型
	//bResult = sc.CallTableFunction(
	//	"Item",
	//	"GetBagRoomType",
	//	1,
	//	"o",
	//	pItem->GetScriptInterface());
	//if (!bResult)
	//{
	//	// 执行脚本一定成功
	//	ASSERT(FALSE);
	//	QLogPrintf(LOG_ERR, "[mail] %s", "KMailManager_GS::GameCenter_FetchItem do Item:GetBagRoomType failed!");
	//	return FALSE;
	//}
	//BYTE eRoomType = (BYTE)sc.GetInt(-1);

	//// 加入背包
	//KE_ITEM_RESULT nItemResult = emKITEM_S_SUCCESS;
	//BOOL bAddRet = pItemPlayer->AddItemToBagRoom(
	//	nItemIdx, 
	//	eRoomType, 
	//	TRUE, 
	//	&nItemResult);
	//if (!bResult)
	//{
	//	// 一定加入成功
	//	ASSERT(FALSE);
	//	QLogPrintf(LOG_ERR, "[mail] %s", "KMailManager_GS::GameCenter_FetchItem AddItemToBagRoom failed!");
	//	return FALSE;
	//}
	//
	//// 向GameCenter请求删除附件中的物品
	//KMAIL_S2G_PROTO_DELETEITEM sReq;
	//sReq.dwPlayerId = pMsg->dwPlayerId;
	//sReq.nMailID = pMsg->nMailID;
	//sReq.nItemIdx = pMsg->nItemIdx;
	//g_cMailModule.SendGcMsg(&sReq, sizeof(sReq));

	//// 通知客户端获取物品成功
	//KMAIL_S2C_PROTO_FETCHITEMOK sNotify;
	//sNotify.nMailID = pMsg->nMailID;
	//sNotify.byItemIndex = pMsg->nItemIdx;
	//g_cMailModule.SendClientMsg(pMsg->dwPlayerId, sNotify);

	return TRUE;
}

BOOL KMailManager_GS::GameCenter_AddMail(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	KMAIL_G2S_PROTO_ADDMAIL* pMsg = (KMAIL_G2S_PROTO_ADDMAIL*)pbData;
	if (uSize != sizeof(KMAIL_G2S_PROTO_ADDMAIL) ||
		pMsg->dwPlayerId == 0 ||
		pMsg->nMailID < 0)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_G2S_PROTO_ADDMAIL size error!");
		return FALSE;
	}

	KMAIL_S2C_PROTO_NEWMAIL sNotify;
	return g_cMailModule.SendClientMsg(pMsg->dwPlayerId, sNotify);
}

BOOL KMailManager_GS::GameCenter_DeleteMail(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	KMAIL_G2S_PROTO_DELMAIL* pMsg = (KMAIL_G2S_PROTO_DELMAIL*)pbData;
	if (uSize != sizeof(KMAIL_G2S_PROTO_DELMAIL) ||
		pMsg->dwPlayerId == 0)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_G2S_PROTO_DELMAIL size error!");
		return FALSE;
	}

	return TRUE;
}

BOOL KMailManager_GS::GameCenter_CheckReceiver(LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pbData);

	KMAIL_G2S_PROTO_CHECKRECEIVER *pMsg = (KMAIL_G2S_PROTO_CHECKRECEIVER*)pbData;
	if ((pMsg->nResult == 0 && uSize != sizeof(KMAIL_G2S_PROTO_CHECKRECEIVER) + pMsg->nFailReasonLen) ||
		(pMsg->nResult == 1 && uSize != sizeof(KMAIL_G2S_PROTO_CHECKRECEIVER) + pMsg->wReceiverCount * sizeof(DWORD)))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_G2S_PROTO_CHECKRECEIVER size error!");
		return FALSE;
	}

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->GetGlobalF("Mail:OnCheckReceiver");
	cSafeScript->PushNumber(pMsg->nMailIndex);
	cSafeScript->PushNumber(pMsg->nResult);
	if (pMsg->nResult == 1)
	{
		DWORD* pIds = (DWORD*)(pMsg + 1);
		cSafeScript->PushTable();
		for (INT i = 1; i <= pMsg->wReceiverCount; ++i)
		{
			cSafeScript->PushNumber(pIds[i - 1]);
			cSafeScript->SetTableIndex(i);
		}

		cSafeScript->DoCall(0, 4);
	}
	else
	{
		INT nFailReasonLen = pMsg->nFailReasonLen;
		CHAR* pszFailReason = new CHAR[nFailReasonLen + 1];
		memset(pszFailReason, 0, nFailReasonLen + 1);
		QStrCpyLen(pszFailReason, (CHAR*)(pMsg + 1), nFailReasonLen + 1);
		
		cSafeScript->PushString(pszFailReason);
		cSafeScript->DoCall(0, 4);

		SAFE_DELETE_ARRAY(pszFailReason);
	}

	return TRUE;
}

BOOL KMailManager_GS::ProcessClientProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0 && pbData);

	// 如果邮箱还没有加载完成，对邮件的所有操作都不行
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);
	KMailCtrl* pMailCtrl = pPlayer->GetCtrl<KMailCtrl>();
	QCONFIRM_RET_FALSE(pMailCtrl);
	if (!pMailCtrl->IsMailBoxLoaded())
	{
		QLogPrintf(LOG_ERR, "[mail] '%s' MailBox not Loaded!!!!!!!!!", pPlayer->GetName());
		return FALSE;
	}

	BOOL bResult = FALSE;
	KPTC_HEADER_BASE* pMsg = (KPTC_HEADER_BASE*)pbData;
	switch (pMsg->byProtocol)
	{
	case emKMAIL_C2S_PROTO_FETCHINFO:
		bResult = GameClient_FetchInfo(nPlayerIndex, pbData, uDataLen);
		break;
	case emKMAIL_C2S_PROTO_FETCHCONDINFO:
		bResult = GameClient_FetchCondInfo(nPlayerIndex, pbData, uDataLen);
		break;
	case emKMAIL_C2S_PROTO_FETCHMAIL:
		bResult = GameClient_FetchMail(nPlayerIndex, pbData, uDataLen);
		break;
	case emKMAIL_C2S_PROTO_FETCHITEM:
		bResult = GameClient_FetchItem(nPlayerIndex, pbData, uDataLen);
		break;
	case emKMAIL_C2S_PROTO_SENDMAIL:
		bResult = GameClient_SendMail(nPlayerIndex, pbData, uDataLen);
		break;
	case emKMAIL_C2S_PROTO_DELMAIL:
		bResult = GameClient_DeleteMail(nPlayerIndex, pbData, uDataLen);
		break;
	default:
		bResult = FALSE;
		QLogPrintf(LOG_ERR, "[mail] %s", "Not valid C2S Protocol!!!!!!!!!!!!!!!!!");
		break;
	}

	return bResult;
}

BOOL KMailManager_GS::GameClient_FetchInfo(INT nPlayerIndex, LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0 && pbData);

	if (uSize != sizeof(KMAIL_C2S_PROTO_FETCHINFO))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_C2S_PROTO_FETCHINFO size error!");
		return FALSE;
	}

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	KMAIL_S2G_PROTO_FETCHINFO sNotify;
	sNotify.dwPlayerId = pPlayer->GetId();
	return g_cMailModule.SendGcMsg(&sNotify, sizeof(sNotify));
}

BOOL KMailManager_GS::GameClient_FetchCondInfo(INT nPlayerIndex, LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0 && pbData);

	if (uSize != sizeof(KMAIL_C2S_PROTO_FETCHCONDINFO))
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_C2S_PROTO_FETCHCONDINFO size error!");
		return FALSE;
	}

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	KMAIL_S2G_PROTO_FETCHCONDINFO sNotify;
	sNotify.dwPlayerId = pPlayer->GetId();
	return g_cMailModule.SendGcMsg(&sNotify, sizeof(sNotify));
}

BOOL KMailManager_GS::GameClient_FetchMail(INT nPlayerIndex, LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0 && pbData);

	KMAIL_C2S_PROTO_FETCHMAIL* pMsg = (KMAIL_C2S_PROTO_FETCHMAIL*)pbData;
	if (uSize != sizeof(KMAIL_C2S_PROTO_FETCHMAIL) ||
		pMsg->nMailID <= 0)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_C2S_PROTO_FETCHMAIL size error!");
		return FALSE;
	}

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	KMAIL_S2G_PROTO_FETCHMAIL sNotify;
	sNotify.nMailID = pMsg->nMailID;
	sNotify.dwPlayerId = pPlayer->GetId();
	return g_cMailModule.SendGcMsg(&sNotify, sizeof(sNotify));
}

BOOL KMailManager_GS::GameClient_FetchItem(INT nPlayerIndex, LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0 && pbData);

	KMAIL_C2S_PROTO_FETCHITEM* pMsg = (KMAIL_C2S_PROTO_FETCHITEM*)pbData;
	if (uSize != sizeof(KMAIL_C2S_PROTO_FETCHITEM) ||
		pMsg->nMailID <= 0 ||
		pMsg->nItemIndex < 0)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_C2S_PROTO_FETCHITEM size error!");
		return FALSE;
	}

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	KMAIL_S2G_PROTO_FETCHITEM sNotify;
	sNotify.nMailID = pMsg->nMailID;
	sNotify.dwPlayerId = pPlayer->GetId();
	sNotify.nItemIdx = pMsg->nItemIndex;
	return g_cMailModule.SendGcMsg(&sNotify, sizeof(sNotify));
}

BOOL KMailManager_GS::GameClient_SendMail(INT nPlayerIndex, LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0 && pbData);

	//KMAIL_C2S_PROTO_SENDMAIL* pMsg = (KMAIL_C2S_PROTO_SENDMAIL*)pbData;
	//if (uSize <= sizeof(KMAIL_C2S_PROTO_SENDMAIL))
	//{
	//	QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_C2S_PROTO_SENDMAIL size too small!");
	//	return FALSE;
	//}
	//INT nMsgLen = sizeof(KMAIL_C2S_PROTO_SENDMAIL) +
	//			  _NAME_LEN * pMsg->wReceiverCount +
	//			  pMsg->sMail.wContentLen +
	//			  pMsg->sMail.nItemCount * sizeof(KItemData_Common);
	//if (nMsgLen != uSize ||
	//	pMsg->wReceiverCount == 0)
	//{
	//	QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_C2S_PROTO_SENDMAIL size error!");
	//	return FALSE;
	//}

	//KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	//QCONFIRM_RET_FALSE(pPlayer);
	//
	//QLuaScript& sc = g_cScriptManager.GetMainScript();
	//KScriptSafeCall cSafeCall(sc);

	//sc.GetGlobalF("Mail:SendMailByPlayer");
	//sc.PushTable();
	//// playerid
	//sc.PushNumber(pPlayer->GetId());
	//sc.SetTableField("nPlayerId");
	//// iconid
	//sc.PushNumber(pMsg->sMail.nIconId);
	//sc.SetTableField("nIconId");
	//// nType
	//sc.PushNumber(pMsg->sMail.nType);
	//sc.SetTableField("nType");
	//// tbReceiver
	//sc.PushTable();
	//LPSTR pszReceiver = (LPSTR)(pMsg + 1);
	//for (INT i = 0; i < pMsg->wReceiverCount; ++i)
	//{
	//	LPSTR pszName = &pszReceiver[_NAME_LEN * i];
	//	sc.PushString(pszName);
	//	sc.SetTableIndex(i + 1);
	//}
	//sc.SetTableField("tbReceiver");
	//// Caption
	//sc.PushString(pMsg->sMail.szCaption);
	//sc.SetTableField("szCaption");
	//// Content
	//LPSTR pszContent = pszReceiver + _NAME_LEN * pMsg->wReceiverCount;
	//sc.PushString(pszContent);
	//sc.SetTableField("szContent");
	//// tbItem
	//if (pMsg->sMail.nItemCount > 0)
	//{
	//	sc.PushTable();
	//	KItemData_Common* pItem = (KItemData_Common*)&pszContent[pMsg->sMail.wContentLen];
	//	for (INT i = 0; i < pMsg->sMail.nItemCount; ++i)
	//	{
	//		sc.PushTable();
	//		sc.PushNumber(pItem->dwClassID);
	//		sc.SetTableField("nClassId");
	//		sc.PushNumber(pItem->wStackCount);
	//		sc.SetTableField("nCount");
	//		sc.PushNumber(pItem->dwBuyPrice);
	//		sc.SetTableField("nPrice");
	//		sc.PushNumber(pItem->byStateFlags);
	//		sc.SetTableField("nStateFlags");
	//		sc.PushNumber(pItem->byTimeoutType);
	//		sc.SetTableField("nTimeoutType");
	//		sc.PushNumber(pItem->dwTimeout);
	//		sc.SetTableField("nTimeout");
	//		LPCSTR lpszGuid = KGuidHelper::GuidToString(pItem->sGuid);
	//		sc.PushString(lpszGuid);
	//		sc.SetTableField("szGuid");

	//		sc.SetTableIndex(i + 1);
	//		pItem++;
	//	}
	//	sc.SetTableField("tbItem");
	//}

	//sc.DoCall(0, 2);
	return TRUE;
}

BOOL KMailManager_GS::GameClient_DeleteMail(INT nPlayerIndex, LPCBYTE pbData, UINT uSize)
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0 && pbData);

	KMAIL_C2S_PROTO_DELMAIL* pMsg = (KMAIL_C2S_PROTO_DELMAIL*)pbData;
	if (uSize != sizeof(KMAIL_C2S_PROTO_DELMAIL) ||
		pMsg->nMailID <= 0)
	{
		QLogPrintf(LOG_ERR, "[mail] %s", "KMAIL_C2S_PROTO_DELMAIL size error!");
		return FALSE;
	}

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	KMAIL_S2G_PROTO_DELMAIL sNotify;
	sNotify.nMailID = pMsg->nMailID;
	sNotify.dwPlayerId = pPlayer->GetId();
	return g_cMailModule.SendGcMsg(&sNotify, sizeof(sNotify));
}

// -------------------------------------------------------------------------
#endif /* GAME_SERVER */
