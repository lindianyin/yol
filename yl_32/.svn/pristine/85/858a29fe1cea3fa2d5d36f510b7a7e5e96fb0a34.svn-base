/* -------------------------------------------------------------------------
//	文件名		：	kchatprotocolprocess_gc.cpp
//	创建者		：	liuchang
//	创建时间	：	2010-6-24 11:15:06
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kchatprotocolprocess_gc.h"
#include "gcmodule/chat/kchatchannelptc.h"
#include "kchatapply_gc.h"
#include "gclogicbase/kgcplayercommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
// 处理GS的协议的接收器
KChatClientMsgProcessor g_ChatProcessAcceptor;
BOOL KChatClientMsgProcessor::OnConnectClose(INT nConnectId)
{
	return TRUE;
}

BOOL KChatClientMsgProcessor::OnConnected(INT nConnectId)
{
	return TRUE;
}

BOOL KChatClientMsgProcessor::ProcessData(INT nConnectId, LPBYTE pData, UINT uSize)
{
	QCONFIRM_RET_FALSE(pData && uSize > 0);
	KPTC_HEADER_BASE* pMsg = (KPTC_HEADER_BASE*)pData;
	switch (pMsg->byProtocol)
	{
	case emKCHAT_PTC_S2G_MSG:
		{
			ProcessReceiveMsg(nConnectId, pData, uSize);
		}
		break;
	case emKCHAT_PTC_S2G_PLAYER_MSG:
		{
			ProcessSendMsgToPlayer(nConnectId, pData, uSize);
		}
		break;
	default:
		QCONFIRM_RET_FALSE(FALSE);
	}

	return TRUE;
}

extern KGCChatProcess_Private g_cGCChatPrivate;

BOOL KChatClientMsgProcessor::ProcessSendMsgToPlayer(INT nConnectId, LPBYTE pData, UINT uSize)
{
	KCHATPROTO_S2G_PLAYER_MSG* pMsg = (KCHATPROTO_S2G_PLAYER_MSG*)pData;
	//QCONFIRM_RET_FALSE(uSize == sizeof(KCHATPROTO_S2G_PLAYER_MSG) + pMsg->nMsgLen);

	pMsg->szSenderName[KD_CHANNEL_NAME_MAX_LEN - 1] = '\0';

	return g_cGCChatPrivate.ProcessSendMsgToPlayer(pMsg->nChannelId,
											pMsg->nSenderId,
											pMsg->szReceiverName,
											pMsg->szSenderName,
											pMsg->szMsg, 
											pMsg->nIsSysMsg);
}

BOOL KChatClientMsgProcessor::ProcessReceiveMsg(INT nConnectId, LPBYTE pData, UINT uSize)
{
	KCHATPROTO_S2G_MSG* pMsg = (KCHATPROTO_S2G_MSG*)pData;
	//QCONFIRM_RET_FALSE(uSize == sizeof(KCHATPROTO_S2G_MSG) + pMsg->nMsgLen);

	pMsg->szSenderName[KD_CHANNEL_NAME_MAX_LEN-1] = '\0';
	g_ChatChannelManager.RequestSendMsg(pMsg->nChannelId, pMsg->nSenderId, pMsg->nReceiverId,
		pMsg->szSenderName, pMsg->szMsg, pMsg->nIsSysMsg);

	return TRUE;
}
// -------------------------------------------------------------------------
// 处理GC的Team模块消息
KChatMsgDispatcher_ForTeam g_cTeamChatProcess;
BOOL KChatMsgDispatcher_ForTeam::OnConnected()
{
	return TRUE;
}

BOOL KChatMsgDispatcher_ForTeam::OnConnectClose()
{
	return TRUE;
}

BOOL KChatMsgDispatcher_ForTeam::ProcessData(LPBYTE pData, UINT uSize)
{
	// TODO:liuchang 处理来自其它GC模块发来的消息
	return TRUE;
}

// -------------------------------------------------------------------------
// 处理GC的Kin模块消息
KChatMsgDispatcher_ForKin g_cKinChatProcess;
BOOL KChatMsgDispatcher_ForKin::OnConnected()
{
	return TRUE;
}

BOOL KChatMsgDispatcher_ForKin::OnConnectClose()
{
	//TODO:
	return TRUE;
}

BOOL KChatMsgDispatcher_ForKin::ProcessData(LPBYTE pData, UINT uSize)
{
	// TODO:liuchang 处理来自其它GC模块发来的消息
	return TRUE;
}


// -------------------------------------------------------------------------
// 处理GC的Group模块消息
KChatMsgDispatcher_ForGroup g_cGroupChatProcess;
BOOL KChatMsgDispatcher_ForGroup::OnConnected()
{
	return TRUE;
}

BOOL KChatMsgDispatcher_ForGroup::OnConnectClose()
{
	return TRUE;
}

BOOL KChatMsgDispatcher_ForGroup::ProcessData(LPBYTE pData, UINT uSize)
{
	// TODO:liuchang 处理来自其它GC模块发来的消息
	return TRUE;
}


// -------------------------------------------------------------------------
KChatModuleConnectorFactory g_cChatModuleConnectorFactory;
IKProcessConnector* KChatModuleConnectorFactory::GetModuleChatProcess(KE_GC_MODULE_ID emModuleId)
{
	switch (emModuleId)
	{
		// 	case emKINTERFACE_TID_TEAM_POST:
		// 		{
		// 			return &g_cTeamChatProcess;
		// 		}
		// 		break;
		// 	case emKINTERFACE_TID_KIN_POST:
		// 		{
		// 			return &g_cKinChatProcess;
		// 		}
		// 		break;
	case emKGCMODULE_GROUP:
		{
			return &g_cGroupChatProcess;
		}
		break;
	default:
		break;
	}

	return NULL;
}

// -------------------------------------------------------------------------
