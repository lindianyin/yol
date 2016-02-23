/* -------------------------------------------------------------------------
//	文件名		：	kmailscript.cpp
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/27 9:57:11
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegamemodule/mail/kmailmodule.h"
#include "onlinegamebase/kscriptmanager.h"
#include "gcmodule/mail/kmailprotocol.h"
#include "gcmodule/mail/kmail_datadef.h"
#include "onlinegamebase/kguidhelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//// 提前分配好栈
//static DWORD g_adwReceiverId[KD_MAIL_MAX_RECEIVER_COUNT];
//static KItemData_Common g_asItems[KD_MAIL_MAX_ITEM_COUNT];
//static BYTE g_abyCond[KD_MAIL_MAX_CONTENT_LENGTH];
//static BYTE g_abyMailBuffer[sizeof(KMAIL_S2G_PROTO_SENDMAIL) + 
//						    _NAME_LEN * KD_MAIL_MAX_RECEIVER_COUNT + 
//						    KD_MAIL_MAX_CAPTION_LENGTH + 
//						    KD_MAIL_MAX_CONTENT_LENGTH + 
//						    KD_MAIL_MAX_ITEM_COUNT * sizeof(KItemData_Common) + 
//						    KD_MAIL_MAX_COND_LENGTH];

// -------------------------------------------------------------------------
namespace kmailscript
{
#ifdef GAME_SERVER

	VOID GetMailCond(QLuaScript& sc, INT nIndex, LPBYTE pCond, INT& nCondLen)
	{
		if (!pCond)
			return;

		ScriptSafe cSafeScript(sc);

		sc.GetTableField(nIndex, "nType");
		INT nType = sc.GetInt(-1);
		if (nType == emKMAILCOND_TYPE_SEXLEVEL)
		{
			// 性别/级别
			KMAILCOND_SEXLEVEL* pSexLevel = (KMAILCOND_SEXLEVEL*)pCond;
			pSexLevel->byType = (BYTE)nType;
			sc.GetTableField(nIndex, "nSex");
			pSexLevel->bySex = (BYTE)sc.GetNum(-1);
			sc.GetTableField(nIndex, "nLevel1");
			pSexLevel->nLevel1 = (INT)sc.GetNum(-1);
			sc.GetTableField(nIndex, "nLevel1Op");
			pSexLevel->byLevel1Op = (BYTE)sc.GetNum(-1);
			sc.GetTableField(nIndex, "nLevel2");
			pSexLevel->nLevel2 = (INT)sc.GetNum(-1);
			sc.GetTableField(nIndex, "nLevel2Op");
			pSexLevel->byLevel2Op = (BYTE)sc.GetNum(-1);

			nCondLen = sizeof(KMAILCOND_SEXLEVEL);
		}
		else if (nType == emKMAILCOND_TYPE_SENDTIME)
		{
			// 发送时间
			KMAILCOND_SENDTIME* pSendTime = (KMAILCOND_SENDTIME*)pCond;
			pSendTime->byType = (BYTE)nType;
			sc.GetTableField(nIndex, "nSendTime");
			pSendTime->nSendTime = (INT)sc.GetNum(-1);
			sc.GetTableField(nIndex, "nDeltaTime");
			pSendTime->nDeltaTime = (INT)sc.GetNum(-1);
			sc.GetTableField(nIndex, "nTimeOp");
			pSendTime->byTimeOp = (BYTE)sc.GetNum(-1);

			nCondLen = sizeof(KMAILCOND_SENDTIME);
		}
	}
	
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: 
	// @LuaApiName		: CheckReceiver
	// @Description		: 检测收件人是否合法
	// @ReturnCode		: 
	// @ArgumentFlag	: dd?
	// @ArgumentComment	: 待发邮件索引
	// @ArgumentComment	: 待发邮件类型
	// @ArgumentComment	: 收件人列表
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaCheckReceiver(QLuaScript& sc)
	{	
		static BYTE abyBuffer[sizeof(KMAIL_S2G_PROTO_CHECKRECEIVER) + KD_MAIL_MAX_RECEIVER_COUNT * _NAME_LEN];
		memset(abyBuffer, 0, sizeof(abyBuffer));

		INT nMailIndex = sc.GetInt(1);
		INT nType = sc.GetInt(2);

		INT nReceiverCount = 0;
		sc.GetTableField(3, "nCount");
		nReceiverCount = sc.GetInt(-1);
		if (nReceiverCount <= 0 || nReceiverCount > KD_MAIL_MAX_RECEIVER_COUNT)
		{
			sc.PushNumber(0);
			return 1;
		}
		else
		{
			INT nDataLen = sizeof(KMAIL_S2G_PROTO_CHECKRECEIVER);

			KMAIL_S2G_PROTO_CHECKRECEIVER* pMsg = (KMAIL_S2G_PROTO_CHECKRECEIVER*)abyBuffer;
			*pMsg = KMAIL_S2G_PROTO_CHECKRECEIVER();
			pMsg->dwPlayerId = 0;
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

			BOOL bRet = g_cMailModule.SendGcMsg(abyBuffer, nDataLen);
			sc.PushNumber(bRet);
			return 1;
		}
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: 
	// @LuaApiName		: SendMail
	// @Description		: 发送邮件
	// @ReturnCode		: 
	// @ArgumentFlag	: dd?ss[??]
	// @ArgumentComment	: 邮件图标Id
	// @ArgumentComment	: 邮件类型
	// @ArgumentComment	: 接收者名字列表
	// @ArgumentComment	: 邮件标题
	// @ArgumentComment	: 邮件内容
	// @ArgumentComment	: 邮件附件列表
	// @ArgumentComment	: 邮件接收条件
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSendMail(QLuaScript& sc)
	{
		//memset(g_adwReceiverId, 0, sizeof(DWORD) * KD_MAIL_MAX_RECEIVER_COUNT);
		//memset(g_asItems, 0, sizeof(KItemData_Common) * KD_MAIL_MAX_ITEM_COUNT);
		//memset(g_abyCond, 0, sizeof(BYTE) * KD_MAIL_MAX_CONTENT_LENGTH);
		//memset(g_abyMailBuffer, 0, sizeof(BYTE) * (sizeof(KMAIL_S2G_PROTO_SENDMAIL) + 
		//										 _NAME_LEN * KD_MAIL_MAX_RECEIVER_COUNT +
		//										 KD_MAIL_MAX_CAPTION_LENGTH + 
		//										 KD_MAIL_MAX_CONTENT_LENGTH + 
		//										 KD_MAIL_MAX_ITEM_COUNT * sizeof(KItemData_Common) + 
		//										 KD_MAIL_MAX_COND_LENGTH));

		//INT nIconId = sc.GetInt(1);
		//INT nType = sc.GetInt(2);

		//// receiver
		//INT nReceiverCount = 0;
		//{
		//	KScriptSafeCall cSafeCall(sc);

		//	sc.GetTableField(3, "nCount");
		//	nReceiverCount = sc.GetInt(-1);
		//	if (nReceiverCount > 0)
		//	{
		//		for (INT i = 0; i < nReceiverCount; ++i)
		//		{
		//			sc.GetTableIndex(3, i+1);
		//			DWORD dwReceiverId = (DWORD)sc.GetNum(-1);
		//			g_adwReceiverId[i] = dwReceiverId;
		//		}
		//	}
		//}

		//LPCSTR pszCaption = sc.GetStr(4);
		//LPCSTR pszContent = sc.GetStr(5);

		//// Item
		//INT nItemCount = 0;
		//if (sc.GetTopIndex() >= 6)
		//{
		//	KScriptSafeCall cSafeCall(sc);
		//	
		//	sc.GetTableField(6, "nItemCount");
		//	nItemCount = sc.GetInt(-1);
		//	for (INT i = 0; i < nItemCount; ++i)
		//	{
		//		KScriptSafeCall cSafeCall(sc);
		//		sc.GetTableIndex(6, i + 1);
		//		
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

		//// Condition
		//BOOL bHasCond = FALSE;
		//INT nCondLen = 0;
		//if (sc.GetTopIndex() >= 7)
		//{
		//	GetMailCond(sc, 7, (LPBYTE)g_abyCond, nCondLen);
		//}

		//if (nReceiverCount > 0)
		//{
		//	// 给指定玩家发送系统邮件
		//	KMAIL_S2G_PROTO_SENDMAIL* pMailHead = (KMAIL_S2G_PROTO_SENDMAIL*)g_abyMailBuffer;
		//	*pMailHead = KMAIL_S2G_PROTO_SENDMAIL();
		//	pMailHead->dwPlayerId = 0;
		//	pMailHead->wReceiverCount = nReceiverCount;
		//	pMailHead->nIconId = nIconId;
		//	pMailHead->nType = nType;
		//	pMailHead->wCaptionLen = QStrLen(pszCaption) + 1;
		//	pMailHead->wContentLen = QStrLen(pszContent) + 1;
		//	pMailHead->nItemCount = nItemCount;
		//	pMailHead->byCondLen = nCondLen;

		//	// receiver
		//	LPBYTE pMailBody = (LPBYTE)(pMailHead + 1);
		//	memcpy(pMailBody, g_adwReceiverId, sizeof(DWORD) * nReceiverCount);
		//	pMailBody += sizeof(DWORD) * nReceiverCount;

		//	// Caption
		//	QStrCpyLen((LPSTR)pMailBody, pszCaption, pMailHead->wCaptionLen);
		//	pMailBody += pMailHead->wCaptionLen;

		//	// Content
		//	QStrCpyLen((LPSTR)pMailBody, pszContent, pMailHead->wContentLen);
		//	pMailBody += pMailHead->wContentLen;

		//	// Item
		//	KItemData_Common* pItem = (KItemData_Common*)pMailBody;
		//	if (nItemCount > 0)
		//	{
		//		for (INT i = 0; i < nItemCount; ++i)
		//		{
		//			pItem->dwClassID = g_asItems[i].dwClassID;
		//			pItem->dwBuyPrice = g_asItems[i].dwBuyPrice;
		//			pItem->wStackCount = g_asItems[i].wStackCount;
		//			pItem->byTimeoutType = g_asItems[i].byTimeoutType;
		//			pItem->dwTimeout = g_asItems[i].dwTimeout;
		//			pItem->byStateFlags = g_asItems[i].byStateFlags;
		//			pItem->sGuid = g_asItems[i].sGuid;

		//			pItem++;
		//			pMailBody += sizeof(KItemData_Common);
		//		}
		//	}

		//	// Condition
		//	if (nCondLen > 0)
		//	{
		//		memcpy(pMailBody, g_abyCond, nCondLen);
		//		pMailBody += nCondLen;
		//	}
		//	pMailHead->wDataLen = (WORD)((DWORD)pMailBody - (DWORD)pMailHead - 1);

		//	BOOL bRet = g_cMailModule.SendGcMsg(pMailHead, (UINT)(pMailHead->wDataLen + 1));
		//	sc.PushNumber(bRet);
		//}
		//else
		//{
		//	// 给全区玩家发送系统邮件
		//	KMAIL_S2G_PROTO_SENDCONDMAIL* pMailHead = (KMAIL_S2G_PROTO_SENDCONDMAIL*)g_abyMailBuffer;
		//	*pMailHead = KMAIL_S2G_PROTO_SENDCONDMAIL();
		//	pMailHead->nIconId = nIconId;
		//	pMailHead->wCaptionLen = QStrLen(pszCaption) + 1;
		//	pMailHead->wContentLen = QStrLen(pszContent) + 1;
		//	pMailHead->nItemCount = nItemCount;
		//	pMailHead->byCondLen = nCondLen;

		//	// Condition
		//	LPBYTE pMailBody = (LPBYTE)(pMailHead + 1);
		//	if (nCondLen > 0)
		//	{
		//		memcpy(pMailBody, g_abyCond, nCondLen);
		//		pMailBody += nCondLen;
		//	}

		//	// Caption
		//	QStrCpyLen((LPSTR)pMailBody, pszCaption, pMailHead->wCaptionLen);
		//	pMailBody += pMailHead->wCaptionLen;

		//	// Content
		//	QStrCpyLen((LPSTR)pMailBody, pszContent, pMailHead->wContentLen);
		//	pMailBody += pMailHead->wContentLen;

		//	// Item
		//	KItemData_Common* pItem = (KItemData_Common*)pMailBody;
		//	if (nItemCount > 0)
		//	{
		//		for (INT i = 0; i < nItemCount; ++i)
		//		{
		//			pItem->dwClassID = g_asItems[i].dwClassID;
		//			pItem->dwBuyPrice = g_asItems[i].dwBuyPrice;
		//			pItem->wStackCount = g_asItems[i].wStackCount;
		//			pItem->byTimeoutType = g_asItems[i].byTimeoutType;
		//			pItem->dwTimeout = g_asItems[i].dwTimeout;
		//			pItem->byStateFlags = g_asItems[i].byStateFlags;

		//			pItem++;
		//			pMailBody += sizeof(KItemData_Common);
		//		}
		//	}
		//	pMailHead->wDataLen = (WORD)((DWORD)pMailBody - (DWORD)pMailHead - 1);

		//	BOOL bRet = g_cMailModule.SendGcMsg(pMailHead, (UINT)(pMailHead->wDataLen + 1));
		//	sc.PushNumber(bRet);
		//}

		return 1;
	}

#endif /*GAME_SERVER*/

#include "kmailscript_table.hpp"
}

BOOL g_RegisterMailScriptFuns()
{
	return g_cScriptManager.RegisterTableFuncs("KMail", kmailscript::arFunction, countof(kmailscript::arFunction));
}

// -------------------------------------------------------------------------
