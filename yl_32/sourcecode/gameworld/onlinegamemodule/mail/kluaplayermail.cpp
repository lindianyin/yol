

#include "stdafx.h"
#include "onlinegamemodule/mail/kluaplayermail.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegamemodule/mail/kmailctrl.h"
#include "onlinegamemodule/mail/kmailmodule.h"
#include "onlinegamemodule/mail/kmailmgr_client.h"
#include "gcmodule/mail/kmailprotocol.h"
#include "gcmodule/mail/kmail_datadef.h"
#include "onlinegamebase/kguidhelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 提前分配好的栈
static CHAR g_szBuffer[128 * 1024];
static CHAR g_achReceiver[KD_MAIL_MAX_RECEIVER_COUNT * _NAME_LEN];
static DWORD g_adwReceiver[KD_MAIL_MAX_RECEIVER_COUNT];
//static KItemData_Common g_asItems[KD_MAIL_MAX_ITEM_COUNT];

// -------------------------------------------------------------------------

INT KLuaPlayerMail::RegisterToPlayer(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	KMailCtrl* pMailCtrl = pPlayer->GetCtrl<KMailCtrl>();
	QCONFIRM_RET_FALSE(pMailCtrl);
	sc.PushObj(&pMailCtrl->GetScriptInterface());

	return 1;
}

#ifdef GAME_CLIENT

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: RequestMailList
// @Description		: 请求邮件列表
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 请求类型
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaRequestMailList(QLuaScript& sc)
{
	INT nMailType = sc.GetInt(1);
	BOOL bRet = FALSE;
	if (nMailType == 1)
	{
		KMAIL_C2S_PROTO_FETCHINFO sNotify;
		bRet = g_cMailModule.SendGsMsg(sNotify);
	}
	else
	{
		KMAIL_C2S_PROTO_FETCHCONDINFO sNotify;
		bRet = g_cMailModule.SendGsMsg(sNotify);
	}

	sc.PushNumber(bRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: RequestMail
// @Description		: 请求邮件具体内容
// @ReturnCode		: 
// @ArgumentFlag	: s
// @ArgumentComment	: 邮件Id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaRequestMail(QLuaScript& sc)
{
	LPCSTR pszMailId = sc.GetStr(1);
	KMAIL_C2S_PROTO_FETCHMAIL sNotify;
	sscanf(pszMailId, "%lld", &(sNotify.nMailID));
	BOOL bRet = g_cMailModule.SendGsMsg(sNotify);
	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: FetchItem
// @Description		: 请求获取附件
// @ReturnCode		: 
// @ArgumentFlag	: sd
// @ArgumentComment	: 邮件Id
// @ArgumentComment	: 附件索引
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaFetchItem(QLuaScript& sc)
{
	LPCSTR pszMailId = sc.GetStr(1);
	INT nItemIndex = sc.GetInt(2);
	KMAIL_C2S_PROTO_FETCHITEM sNotify;
	sscanf(pszMailId, "%lld", &(sNotify.nMailID));
	sNotify.nItemIndex = nItemIndex;
	BOOL bRet = g_cMailModule.SendGsMsg(sNotify);
	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: DeleteMail
// @Description		: 请求删除邮件
// @ReturnCode		: 
// @ArgumentFlag	: s
// @ArgumentComment	: 邮件Id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaDeleteMail(QLuaScript& sc)
{
	LPCSTR pszMailId = sc.GetStr(1);
	KMAIL_C2S_PROTO_DELMAIL sNotify;
	sscanf(pszMailId, "%lld", &(sNotify.nMailID));
	BOOL bRet = g_cMailModule.SendGsMsg(sNotify);
	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: SendMail
// @Description		: 发送邮件
// @ReturnCode		: 
// @ArgumentFlag	: dd?ss[?]
// @ArgumentComment	: 邮件图标Id
// @ArgumentComment	: 邮件类型
// @ArgumentComment	: 接收者名字
// @ArgumentComment	: 邮件标题
// @ArgumentComment	: 邮件内容
// @ArgumentComment	: 邮件附件列表
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaSendMail(QLuaScript& sc)
{
	memset(g_achReceiver, 0, sizeof(g_achReceiver));
	memset(g_asItems, 0, sizeof(KItemData_Common) * KD_MAIL_MAX_ITEM_COUNT);
	memset(g_szBuffer, 0, sizeof(g_szBuffer));

	INT nIconId = sc.GetInt(1);
	INT nType = sc.GetInt(2);
	
	// receiver
	INT nReceiverCount = 0;
	{
		KScriptSafeCall cSafeCall(sc);

		sc.GetTableField(3, "nCount");
		nReceiverCount = sc.GetInt(-1);
		if (nReceiverCount <= 0 || nReceiverCount > KD_MAIL_MAX_RECEIVER_COUNT)
		{
			CHAR szFailReason[MAX_PATH];
			memset(szFailReason, 0, sizeof(szFailReason));
			if (nReceiverCount <= 0)
				QStrCpy(szFailReason, "Receiver Count is Zero!");
			else
				QStrCpy(szFailReason, "Over Receiver Max Count!");
			INT nFailReasonLen = QStrLen(szFailReason);

			KMAIL_S2C_PROTO_SENDRESULT* pResult = (KMAIL_S2C_PROTO_SENDRESULT*)g_szBuffer;
			*pResult = KMAIL_S2C_PROTO_SENDRESULT();
			pResult->nResult = 0;
			pResult->nReasonType = emKMAIL_ERR_RECEIVER;
			pResult->nFailReasonLen = nFailReasonLen;

			CHAR* pszReason = (CHAR*)(pResult + 1);
			QStrCpy(pszReason, szFailReason);

			BOOL bRet = KMailManager_Client::Instance()->OnSendResult((LPCBYTE)pResult, sizeof(KMAIL_S2C_PROTO_SENDRESULT) + nFailReasonLen);
			sc.PushNumber(bRet);
			return 1;
		}
		else
		{
			LPSTR pReceiver = g_achReceiver;
			for (INT i = 0; i < nReceiverCount; ++i)
			{
				sc.GetTableIndex(3, i+1);
				LPCSTR pszReceiver = sc.GetStr(-1);
				QStrCpyLen(pReceiver, pszReceiver, _NAME_LEN);
				pReceiver += _NAME_LEN;
			}
		}
	}

	LPCSTR pszCaption = sc.GetStr(4);
	LPCSTR pszContent = sc.GetStr(5);

	// Item
	INT nItemCount = 0;
	if (sc.GetTopIndex() >= 6)
	{
		sc.GetTableField(6, "nItemCount");
		nItemCount = sc.GetInt(-1);
		for (INT i = 0; i < nItemCount; ++i)
		{
			KScriptSafeCall cSafeCall(sc);
			sc.GetTableIndex(6, i + 1);

			INT nTBIndex = sc.GetTopIndex();
			sc.GetTableField(nTBIndex, "nClassId");
			sc.GetTableField(nTBIndex, "nCount");
			sc.GetTableField(nTBIndex, "nPrice");
			sc.GetTableField(nTBIndex, "nTimeoutType");
			sc.GetTableField(nTBIndex, "nTimeout");
			sc.GetTableField(nTBIndex, "nStateFlags");
			sc.GetTableField(nTBIndex, "szGuid");

			g_asItems[i].dwClassID = (DWORD)sc.GetNum(nTBIndex + 1);
			g_asItems[i].wStackCount = (WORD)sc.GetNum(nTBIndex + 2);
			g_asItems[i].dwBuyPrice = (DWORD)sc.GetNum(nTBIndex + 3);
			g_asItems[i].byTimeoutType = (BYTE)sc.GetNum(nTBIndex + 4);
			g_asItems[i].dwTimeout = (DWORD)sc.GetNum(nTBIndex + 5);
			g_asItems[i].byStateFlags = (BYTE)sc.GetNum(nTBIndex + 6);
			LPCSTR lpszGuid = sc.GetStr(nTBIndex + 7);
			if (lpszGuid && QStrLen(lpszGuid) > 0)
			{
				KGuidHelper::StringToGuid((LPSTR)lpszGuid, &g_asItems[i].sGuid);
			}
		}
	}

	KMAIL_C2S_PROTO_SENDMAIL* pMailHead = (KMAIL_C2S_PROTO_SENDMAIL*)g_szBuffer;
	*pMailHead = KMAIL_C2S_PROTO_SENDMAIL();
	pMailHead->wReceiverCount = nReceiverCount;
	pMailHead->sMail.nIconId = nIconId;
	pMailHead->sMail.nType = nType;
	QStrCpyLen(pMailHead->sMail.szCaption, pszCaption, sizeof(pMailHead->sMail.szCaption));
	pMailHead->sMail.szCaption[sizeof(pMailHead->sMail.szCaption) - 1] = '\0';
	pMailHead->sMail.wContentLen = QStrLen(pszContent) + 1;
	pMailHead->sMail.nItemCount = 0;

	// receiver
	LPBYTE pMailBody = (LPBYTE)(pMailHead + 1);
	for (INT i = 0; i < nReceiverCount; ++i)
	{
		QStrCpyLen((LPSTR)pMailBody, &g_achReceiver[i*_NAME_LEN], _NAME_LEN);
		pMailBody[_NAME_LEN - 1] = 0;
		pMailBody += _NAME_LEN;
	}

	// Content
	QStrCpyLen((LPSTR)pMailBody, pszContent, pMailHead->sMail.wContentLen);
	pMailBody += pMailHead->sMail.wContentLen;
	
	// Item
	KItemData_Common* pItem = (KItemData_Common*)pMailBody;
	if (nItemCount > 0)
	{
		for (INT i = 0; i < nItemCount; ++i)
		{
			pItem->dwClassID = g_asItems[i].dwClassID;
			pItem->dwBuyPrice = g_asItems[i].dwBuyPrice;
			pItem->wStackCount = g_asItems[i].wStackCount;
			pItem->byTimeoutType = g_asItems[i].byTimeoutType;
			pItem->dwTimeout = g_asItems[i].dwTimeout;
			pItem->byStateFlags = g_asItems[i].byStateFlags;
			pItem->sGuid = g_asItems[i].sGuid;

			pItem++;
			pMailBody += sizeof(KItemData_Common);
		}
	}
	pMailHead->wDataLen = (WORD)((DWORD)pMailBody - (DWORD)pMailHead - 1);
	
	BOOL bRet = g_cMailModule.SendVarLenGsMsg(*pMailHead, (UINT)(pMailHead->wDataLen + 1));
	sc.PushNumber(bRet);
	return 1;
}

#endif /* GAME_CLIENT */

#ifdef GAME_SERVER

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: OverSendLimit
// @Description		: 检测是否超过发送限制
// @ReturnCode		: 超过发送限制-返回1，否则-返回0
// @ArgumentFlag	: d
// @ArgumentComment	: 玩家一天发送邮件的限制次数
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaOverSendLimit(QLuaScript& sc)
{	
	INT nSendLimit = sc.GetInt(1);
	if (m_rcMailCtrl.CanSendMail(nSendLimit))
	{
		sc.PushNumber(0);
	}
	else
	{
		sc.PushNumber(1);
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: AddSendTime
// @Description		: 增加一次邮件发送次数
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaAddSendTime(QLuaScript& sc)
{	
	m_rcMailCtrl.AddMailSendTime();
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: CheckReceiver
// @Description		: 检测收件人是否合法
// @ReturnCode		: 
// @ArgumentFlag	: dd?
// @ArgumentComment	: 待发邮件索引
// @ArgumentComment	: 待发邮件类型
// @ArgumentComment	: 收件人列表
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaCheckReceiver(QLuaScript& sc)
{	
	memset(g_szBuffer, 0, sizeof(g_szBuffer));
	INT nMailIndex = sc.GetInt(1);
	INT nType = sc.GetInt(2);
	
	INT nReceiverCount = 0;
	sc.GetTableField(3, "nCount");
	nReceiverCount = sc.GetInt(-1);
	if (nReceiverCount <= 0 || nReceiverCount > KD_MAIL_MAX_RECEIVER_COUNT)
	{
		// 直接记log，这种应该是外挂传了错误的协议
		QLogPrintf(LOG_ERR, "[mail] 检测收件人，收件人个数为0或超上限！");
		sc.PushNumber(0);
		return 1;
	}
	else
	{
		INT nDataLen = sizeof(KMAIL_S2G_PROTO_CHECKRECEIVER);
		KMAIL_S2G_PROTO_CHECKRECEIVER* pMsg = (KMAIL_S2G_PROTO_CHECKRECEIVER*)g_szBuffer;
		*pMsg = KMAIL_S2G_PROTO_CHECKRECEIVER();
		pMsg->dwPlayerId = m_rcMailCtrl.GetPlayerId();
		pMsg->nMailIndex = nMailIndex;
		pMsg->nType = nType;
		pMsg->wReceiverCount = (WORD)nReceiverCount;

		LPSTR pReceiver = (LPSTR)(pMsg + 1);
		for (INT i = 0; i < nReceiverCount; ++i)
		{
			sc.GetTableIndex(3, i+1);
			LPCSTR pszReceiver = sc.GetStr(-1);
			QStrCpyLen(pReceiver, pszReceiver, _NAME_LEN);
			pReceiver += _NAME_LEN;
			nDataLen += _NAME_LEN;
		}

		pMsg->wDataLen = nDataLen - 1;
		
		BOOL bRet = g_cMailModule.SendGcMsg(pMsg, nDataLen);
		sc.PushNumber(bRet);
		return 1;
	}
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: SendClientResult
// @Description		: 发送邮件结果
// @ReturnCode		: 
// @ArgumentFlag	: d[ds]
// @ArgumentComment	: 发送结果
// @ArgumentComment	: 失败类型
// @ArgumentComment	: 具体失败原因
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaSendClientResult(QLuaScript& sc)
{	
	INT nResult = sc.GetInt(1);
	INT nReasonType = 0;
	INT nFailReasonLen = 0;
	LPCSTR pszFailReason = NULL;

	if (sc.GetTopIndex() >= 2)
		nReasonType = sc.GetInt(2);
	if (sc.GetTopIndex() >= 3)
	{
		pszFailReason = sc.GetStr(3);
		nFailReasonLen = QStrLen(pszFailReason);
	}

	memset(g_szBuffer, 0, sizeof(g_szBuffer));
	KMAIL_S2C_PROTO_SENDRESULT* pResult = (KMAIL_S2C_PROTO_SENDRESULT*)g_szBuffer;
	*pResult = KMAIL_S2C_PROTO_SENDRESULT();
	pResult->nResult = nResult;
	pResult->nReasonType = nReasonType;
	pResult->nFailReasonLen = nFailReasonLen;
	if (pszFailReason)
	{
		CHAR* pszReason = (CHAR*)(pResult + 1);
		QStrCpy(pszReason, pszFailReason);
	}

	BOOL bRet = g_cMailModule.SendVarLenClientMsg(m_rcMailCtrl.GetPlayerId(), *pResult, sizeof(KMAIL_S2C_PROTO_SENDRESULT) + nFailReasonLen);
	sc.PushNumber(bRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerMail
// @LuaApiName		: SendMail2GC
// @Description		: 发送邮件
// @ReturnCode		: 
// @ArgumentFlag	: dd?ss[?]
// @ArgumentComment	: 邮件图标Id
// @ArgumentComment	: 邮件类型
// @ArgumentComment	: 接收者名字
// @ArgumentComment	: 邮件标题
// @ArgumentComment	: 邮件内容
// @ArgumentComment	: 邮件附件列表
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerMail::LuaSendMail2GC(QLuaScript& sc)
{
	//memset(g_adwReceiver, 0, sizeof(DWORD) * KD_MAIL_MAX_RECEIVER_COUNT);
	//memset(g_asItems, 0, sizeof(KItemData_Common) * KD_MAIL_MAX_ITEM_COUNT);
	//memset(g_szBuffer, 0, sizeof(g_szBuffer));

	//INT nIconId = sc.GetInt(1);
	//INT nType = sc.GetInt(2);

	//// receiver
	//INT nReceiverCount = 0;
	//{
	//	KScriptSafeCall cSafeCall(sc);

	//	sc.GetTableField(3, "nCount");
	//	nReceiverCount = sc.GetInt(-1);
	//	if (nReceiverCount <= 0 || nReceiverCount > KD_MAIL_MAX_RECEIVER_COUNT)
	//	{
	//		sc.PushNumber(0);
	//		return 1;
	//	}
	//	else
	//	{
	//		for (INT i = 0; i < nReceiverCount; ++i)
	//		{
	//			sc.GetTableIndex(3, i+1);
	//			DWORD dwReceiverId = (DWORD)sc.GetNum(-1);
	//			g_adwReceiver[i] = dwReceiverId;
	//		}
	//	}
	//}

	//LPCSTR pszCaption = sc.GetStr(4);
	//LPCSTR pszContent = sc.GetStr(5);

	//// Item
	//INT nItemCount = 0;
	//if (sc.GetTopIndex() >= 6)
	//{
	//	sc.GetTableField(6, "nItemCount");
	//	nItemCount = sc.GetInt(-1);
	//	for (INT i = 0; i < nItemCount; ++i)
	//	{
	//		KScriptSafeCall cSafeCall(sc);
	//		sc.GetTableIndex(6, i + 1);

	//		INT nTBIndex = sc.GetTopIndex();
	//		sc.GetTableField(nTBIndex, "nClassId");
	//		sc.GetTableField(nTBIndex, "nCount");
	//		sc.GetTableField(nTBIndex, "nPrice");
	//		sc.GetTableField(nTBIndex, "nTimeoutType");
	//		sc.GetTableField(nTBIndex, "nTimeout");
	//		sc.GetTableField(nTBIndex, "nStateFlags");
	//		sc.GetTableField(nTBIndex, "szGuid");

	//		g_asItems[i].dwClassID = (DWORD)sc.GetNum(nTBIndex + 1);
	//		g_asItems[i].wStackCount = (WORD)sc.GetNum(nTBIndex + 2);
	//		g_asItems[i].dwBuyPrice = (DWORD)sc.GetNum(nTBIndex + 3);
	//		g_asItems[i].byTimeoutType = (BYTE)sc.GetNum(nTBIndex + 4);
	//		g_asItems[i].dwTimeout = (DWORD)sc.GetNum(nTBIndex + 5);
	//		g_asItems[i].byStateFlags = (BYTE)sc.GetNum(nTBIndex + 6);
	//		LPCSTR lpszGuid = sc.GetStr(nTBIndex + 7);
	//		if (lpszGuid && QStrLen(lpszGuid) > 0)
	//		{
	//			KGuidHelper::StringToGuid((LPSTR)lpszGuid, &g_asItems[i].sGuid);
	//		}
	//	}
	//}

	//KMAIL_S2G_PROTO_SENDMAIL* pMailHead = (KMAIL_S2G_PROTO_SENDMAIL*)g_szBuffer;
	//*pMailHead = KMAIL_S2G_PROTO_SENDMAIL();
	//pMailHead->dwPlayerId = m_rcMailCtrl.GetPlayerId();
	//pMailHead->wReceiverCount = nReceiverCount;
	//pMailHead->nIconId = nIconId;
	//pMailHead->nType = nType;
	//pMailHead->wCaptionLen = QStrLen(pszCaption) + 1;
	//pMailHead->wContentLen = QStrLen(pszContent) + 1;
	//pMailHead->nItemCount = nItemCount;
	//pMailHead->byCondLen = 0;		// 个人发的邮件没有接收条件

	//// receiver
	//LPBYTE pMailBody = (LPBYTE)(pMailHead + 1);
	//memcpy(pMailBody, g_adwReceiver, sizeof(DWORD) * nReceiverCount);
	//pMailBody += sizeof(DWORD) * nReceiverCount;

	//// Caption
	//QStrCpyLen((LPSTR)pMailBody, pszCaption, pMailHead->wCaptionLen);
	//pMailBody += pMailHead->wCaptionLen;

	//// Content
	//QStrCpyLen((LPSTR)pMailBody, pszContent, pMailHead->wContentLen);
	//pMailBody += pMailHead->wContentLen;

	//// Item
	//KItemData_Common* pItem = (KItemData_Common*)pMailBody;
	//if (nItemCount > 0)
	//{
	//	for (INT i = 0; i < nItemCount; ++i)
	//	{
	//		pItem->dwClassID = g_asItems[i].dwClassID;
	//		pItem->dwBuyPrice = g_asItems[i].dwBuyPrice;
	//		pItem->wStackCount = g_asItems[i].wStackCount;
	//		pItem->byTimeoutType = g_asItems[i].byTimeoutType;
	//		pItem->dwTimeout = g_asItems[i].dwTimeout;
	//		pItem->byStateFlags = g_asItems[i].byStateFlags;
	//		pItem->sGuid = g_asItems[i].sGuid;

	//		pItem++;
	//		pMailBody += sizeof(KItemData_Common);
	//	}
	//}
	//pMailHead->wDataLen = (WORD)((DWORD)pMailBody - (DWORD)pMailHead - 1);

	//BOOL bRet = g_cMailModule.SendGcMsg(pMailHead, (UINT)(pMailHead->wDataLen + 1));
	//sc.PushNumber(bRet);
	return 1;
}

#endif /* GAME_SERVER */

#include "kluaplayermail_table.hpp"
// -------------------------------------------------------------------------
