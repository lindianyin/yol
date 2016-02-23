/* -------------------------------------------------------------------------
//	文件名		：	kchatapply_gc.cpp
//	创建者		：	liuchang
//	创建时间	：	2010-6-3 14:28:40
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifdef GAME_CENTER
#include "stdafx.h"
#include "kchatmodule_gc.h"
#include "gcmodule/chat/kchatchannelptc.h"
#include "kchatapply_gc.h"
#include "gclogicbase/kgcplayercommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KGCChatProcess_Global g_cGCChatGlobal;
KGCChatProcess_Private g_cGCChatPrivate;
KGCChatProcess_Team g_cGCChatTeam;
KGCChatProcess_Kin g_cGCChatKin;
KGCChatProcess_Group g_cGCChatGroup;
KGCChatProcess_GM g_cGCChatGM;
KProcessorFactory_GC g_cChatChannelProcessorFactory_GC;

static BYTE _sbySyncPackBuffer[KD_CHANNEL_PACKET_MAX_LEN]; // chat发送缓冲区

// -------------------------------------------------------------------------
// KGCChatProcess_Global
BOOL KGCChatProcess_Global::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	BOOL bFinish = TRUE;
	bFinish &= g_ChatChannelManager.ProcessRespond(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage, bSysMsg);

	return bFinish;
}

BOOL KGCChatProcess_Global::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	//memset(_sbySyncPackBuffer, 0, sizeof(_sbySyncPackBuffer));
	KCHATPROTO_G2S_MSG& cSendData = *(KCHATPROTO_G2S_MSG* )_sbySyncPackBuffer;
	cSendData = KCHATPROTO_G2S_MSG();
	cSendData.nChannelId = nChannelId;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;
	cSendData.nIsSysMsg = bSysMsg;
	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen < sizeof(_sbySyncPackBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, (INT)uMsgLen);

	// 向所有GS发送数据
	g_pGcServer->Send(-1, &cSendData, sizeof(cSendData) + uMsgLen);

	return TRUE;
}


// -------------------------------------------------------------------------
// KChatProcess_Private
BOOL KGCChatProcess_Private::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	BOOL bFinish = TRUE;
	bFinish &= g_ChatChannelManager.ProcessRespond(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage, bSysMsg);

	return bFinish;
}

BOOL KGCChatProcess_Private::ProcessSendMsgToPlayer(INT nChannelId, INT nSenderId, const char* pszReceiver, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	BOOL bResult = FALSE;
	KGcPlayerCommon* pReceiver = g_pOnlinePlayerMgr->GetPlayerByName(pszReceiver);
	if (pReceiver && pReceiver->GetOnlineServer())
	{
		return g_ChatChannelManager.ProcessRespond(nChannelId, nSenderId, pReceiver->GetId(), pszSenderName, pszMessage, bSysMsg);
	}
	else
	{
		//TODO, 发送离线消息, 暂时不做
		g_ChatChannelManager.ProcessRespond(nChannelId, 0, nSenderId, "[系统]", "指定的玩家不在线", 1);
		return TRUE;
	}
}


BOOL KGCChatProcess_Private::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	//memset(m_bySyncPackBuffer, 0, sizeof(m_bySyncPackBuffer));
	KCHATPROTO_G2S_MSG& cSendData = *(KCHATPROTO_G2S_MSG* )_sbySyncPackBuffer;
	cSendData = KCHATPROTO_G2S_MSG();
	cSendData.nChannelId = nChannelId;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;
	cSendData.nIsSysMsg = bSysMsg;
	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen < sizeof(_sbySyncPackBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, (INT)uMsgLen);

	// 查询玩家nReceiver所在的服务器
	KGcPlayerCommon* pPlayer = g_pOnlinePlayerMgr->GetPlayerById(nReceiverId);
	QCONFIRM_RET_FALSE(NULL != pPlayer);
	INT nConnectId = pPlayer->GetOnlineServer();
	g_pGcServer->Send(nConnectId, &cSendData, sizeof(cSendData) + uMsgLen);

	return TRUE;
}


// -------------------------------------------------------------------------
// KGCChatProcess_Team
BOOL KGCChatProcess_Team::TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
/*	static BYTE bySyncPackBuffer[10240];
	memset(bySyncPackBuffer, 0, sizeof(bySyncPackBuffer));
	KCHATPROTO_G2G_TEAM_MSG& cSendData = *(KCHATPROTO_G2G_TEAM_MSG* )bySyncPackBuffer;
	cSendData = KCHATPROTO_G2G_TEAM_MSG();

//	cSendData.byProtocolFamily = emKINTERFACE_TID_TEAM_POST;
	cSendData.nChannelId = nChannelId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	memcpy(cSendData.szSenderName, pszSenderName, strlen(pszSenderName) + 1);
	cSendData.nTeamId = nReceiverId;
	cSendData.nIsSysMsg = bSysMsg;
	QCONFIRM_RET_FALSE(strlen(pszMessage) < KD_CHANNEL_MSG_MAX_LEN);
	memcpy(cSendData.szMsg, pszMessage, strlen(pszMessage));
	cSendData.nMsgLen = (WORD)strlen(pszMessage);

	CONST KChannelInfo* pChannel = g_ChatChannelManager.GetChannelInfo(nChannelId);
	QCONFIRM_RET_FALSE(pChannel);

	// 转发给其它的GC模块
	IKInterfaceConnector* pConnector = pChannel->pInterface;
	if (pConnector)
	{
		pConnector->Send(&cSendData, sizeof(cSendData) + cSendData.nMsgLen);	
	}
*/
	return TRUE;
}

BOOL KGCChatProcess_Team::TransferToClient( INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg )
{
	//memset(m_bySyncPackBuffer, 0, sizeof(m_bySyncPackBuffer));
	KCHATPROTO_G2S_MSG& cSendData = *(KCHATPROTO_G2S_MSG* )_sbySyncPackBuffer;
	cSendData = KCHATPROTO_G2S_MSG();
	cSendData.nChannelId = nChannelId;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;
	cSendData.nIsSysMsg = bSysMsg;
	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen < sizeof(_sbySyncPackBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, uMsgLen);

	// TODO 向队伍查询所在server
	// 向所有GS发送数据
	g_pGcServer->Send(-1, &cSendData, sizeof(cSendData) + uMsgLen);

	return TRUE;
}

BOOL KGCChatProcess_Team::ProcessSendMsg( INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg )
{
	BOOL bFinish = TRUE;
	bFinish &= g_ChatChannelManager.ProcessRespond(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage, bSysMsg);

	return bFinish;
}


// -------------------------------------------------------------------------
// KGCChatProcess_Kin
//BOOL KGCChatProcess_Kin::TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
//{
//	static BYTE bySyncPackBuffer[10240];
//	memset(bySyncPackBuffer, 0, sizeof(bySyncPackBuffer));
//	KCHATPROTO_G2G_KIN_MSG& cSendData = *(KCHATPROTO_G2G_KIN_MSG* )bySyncPackBuffer;
//	cSendData = KCHATPROTO_G2G_KIN_MSG();
//
////	cSendData.byProtocolFamily = emKINTERFACE_TID_KIN_POST;
//	cSendData.nChannelId = nChannelId;
//	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
//	memcpy(cSendData.szSenderName, pszSenderName, strlen(pszSenderName) + 1);
//	cSendData.nKinId = nReceiverId;
//	cSendData.nIsSysMsg = bSysMsg;
//	QCONFIRM_RET_FALSE(strlen(pszMessage) < KD_CHANNEL_MSG_MAX_LEN);
//	memcpy(cSendData.szMsg, pszMessage, strlen(pszMessage));
//	cSendData.nMsgLen = (WORD)strlen(pszMessage);
//
//
//	CONST KChannelInfo* pChannel = g_ChatChannelManager.GetChannelInfo(nChannelId);
//	QCONFIRM_RET_FALSE(pChannel);
//
//	// 转发给其它的GC模块
//	IKInterfaceConnector* pConnector = pChannel->pInterface;
//	if (pConnector)
//	{
//		pConnector->Send(&cSendData, sizeof(cSendData) + cSendData.nMsgLen);	
//	}
//
//	return TRUE;
//}
BOOL KGCChatProcess_Kin::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	BOOL bFinish = TRUE;
	bFinish &= g_ChatChannelManager.ProcessRespond(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage, bSysMsg);

	return bFinish;
}
BOOL KGCChatProcess_Kin::TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	//memset(m_bySyncPackBuffer, 0, sizeof(m_bySyncPackBuffer));
	KCHATPROTO_G2S_MSG& cSendData = *(KCHATPROTO_G2S_MSG* )_sbySyncPackBuffer;
	cSendData = KCHATPROTO_G2S_MSG();
	cSendData.nChannelId = nChannelId;
	cSendData.nSenderId = nSenderId;
	QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	cSendData.nReceiverId = nReceiverId;
	cSendData.nIsSysMsg = bSysMsg;
	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen < sizeof(_sbySyncPackBuffer) - sizeof(cSendData));
	QStrCpyLen(cSendData.szMsg, pszMessage, (INT)uMsgLen);

	// 向所有GS发送数据
	g_pGcServer->Send(-1, &cSendData, sizeof(cSendData) + uMsgLen);

	return TRUE;
}

// -------------------------------------------------------------------------
// KGCChatProcess_Group
BOOL KGCChatProcess_Group::TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	////memset(m_bySyncPackBuffer, 0, sizeof(m_bySyncPackBuffer));
	//KGROUPPROTO_INTERFACE_GROUP_MSG& cSendData = *(KGROUPPROTO_INTERFACE_GROUP_MSG* )_sbySyncPackBuffer;
	//cSendData = KGROUPPROTO_INTERFACE_GROUP_MSG();

	//cSendData.nChannelId = nChannelId;
	//QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
	//QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
	//cSendData.nGroupId = nReceiverId;
	//cSendData.nIsSysMsg = bSysMsg;
	////QCONFIRM_RET_FALSE(strlen(pszMessage) < KD_CHANNEL_MSG_MAX_LEN);
	//QStrCpyLen(cSendData.szMsg, pszMessage, sizeof(_sbySyncPackBuffer)-sizeof(KGROUPPROTO_INTERFACE_GROUP_MSG));
	//cSendData.nMsgLen = (WORD)strlen(pszMessage) + 1;


	//CONST KChannelInfo* pChannel = g_ChatChannelManager.GetChannelInfo(nChannelId);
	//QCONFIRM_RET_FALSE(pChannel);

	//// 转发给其它的GC模块
	//IKInterfaceConnector* pConnector = pChannel->pInterface;
	//if (pConnector)
	//{
	//	pConnector->Send(&cSendData, sizeof(cSendData) + cSendData.nMsgLen);	
	//}

	return TRUE;
}

// -------------------------------------------------------------------------
// KGCChatProcess_GM
BOOL KGCChatProcess_GM::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	LPCSTR pszPrefix = "?gc";
	SIZE_T nPrefixLen = strlen(pszPrefix);

	//QCONFIRM_RET_FALSE(strlen(pszMessage) > nPrefixLen && strlen(pszMessage) < KD_CHANNEL_MSG_MAX_LEN);

	if (QStrCmpLen(pszMessage, pszPrefix, (INT)nPrefixLen))
	{
		g_pScriptEnv->GetScriptManager().GetMainScript().DoBuffer(pszMessage+nPrefixLen);
		return TRUE;
	}

	return FALSE;
}
// -------------------------------------------------------------------------
#endif
