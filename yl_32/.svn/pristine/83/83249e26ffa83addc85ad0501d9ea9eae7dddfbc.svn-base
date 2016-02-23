

#include "stdafx.h"
#include "kchatprotocolprocess.h"
#include "onlinegameworld/kprotocol.h"
#include "gcmodule/chat/kchatchannelptc.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/kplayermgr.h"
#include "kchatapply_gs.h"
#include "onlinegamemodule/chat/kplayerchat.h"
#include "Runtime/TypeDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef GAME_SERVER

// 处理Client的请求
BOOL KChatProtocolProcessor::ProtocolProcess(INT nPlayerIdx, LPCBYTE pProtocol, UINT nDataLen)
{
	QCONFIRM_RET_FALSE(pProtocol);
	
	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pProtocol;
	QLogPrintf(LOG_INFO,"..KChatProtocolProcessor::ProtocolProcess(INT nPlayerIdx=%d pData->byProtocol=%d"
										,nPlayerIdx, pData->byProtocol);
	switch (pData->byProtocol)
	{
	case emKCHAT_PTC_C2S_MSG:
		{
			ProcessRequstMsg(nPlayerIdx, pProtocol, nDataLen);
		}
		break;
	case emKCHAT_PTC_C2S_PLAYER_MSG:
		{
			ProcessSendMsgToPlayer(nPlayerIdx, pProtocol, nDataLen);
		}
		break;
	default:
		QCONFIRM_RET_FALSE(FALSE);
	}

	return TRUE;
}

BOOL KChatProtocolProcessor::ProcessSendMsgToPlayer(INT nPlayerIdx, LPCBYTE pProtocol, UINT nDataLen)
{
	KCHATPROTO_C2S_PLAYER_MSG* pMsg = (KCHATPROTO_C2S_PLAYER_MSG*)pProtocol;
	QCONFIRM_RET_FALSE(nDataLen > sizeof(KCHATPROTO_C2S_PLAYER_MSG));
	// 让每个频道自己限制长度

	KPlayer *pSender = g_cPlayerMgr.GetByIndex(nPlayerIdx);
	QCONFIRM_RET_FALSE(pSender);

	QLogPrintf(LOG_INFO," KChatProtocolProcessor::ProcessSendMsgToPlayer(INT nPlayerIdx=%d ",nPlayerIdx);
	
	return g_cGSChatPrivate.ProcessSendMsgToPlayer(pMsg->byChannelId,
											 pSender->GetPlayerId(),
											 pMsg->szReceiverName,
											 pSender->GetName(),
											 pMsg->szMsg, emQCHAT_PANEL
											 );
}

BOOL KChatProtocolProcessor::ProcessRequstMsg(INT nPlayerIdx, LPCBYTE pProtocol, UINT nDataLen)
{
	KCHATPROTO_C2S_MSG* pMsg = (KCHATPROTO_C2S_MSG*)pProtocol;

	QLogPrintf(LOG_INFO,"..KChatProtocolProcessor::ProcessRequstMsg(INT nPlayerIdx=%d pMsg->nChannelId=%d,pMsg->nReceiverId=%d,pMsg->szMsg=%s",nPlayerIdx,pMsg->nChannelId,pMsg->nReceiverId,pMsg->szMsg);
	
	// 聊天模块自己需要处理的
	QCONFIRM_RET_FALSE(nDataLen > sizeof(KCHATPROTO_C2S_MSG));
	// 让每个频道自己限制长度

	KPlayer *pSender = g_cPlayerMgr.GetByIndex(nPlayerIdx);
	QCONFIRM_RET_FALSE(pSender);

	KPlayerChat* pPlayerChat = pSender->GetCtrl<KPlayerChat>();
	QCONFIRM_RET_FALSE(pPlayerChat);

	if(!pPlayerChat->IfCanSendMsg(pMsg->nChannelId))
	{
		// 被禁言了
		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, pSender->GetId(), "DEBUG", LANG_040);
		return FALSE;
	}
	if (pMsg->nChannelId == trWorld && pSender->GetLevel() <= 10)
	{
		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, pSender->GetId(), "DEBUG", LANG_041);
		return FALSE;
	}

	g_ChatChannelManager.RequestSendMsg(pMsg->nChannelId, pSender->GetPlayerId(),
		pMsg->nReceiverId, pSender->GetName(), pMsg->szMsg, emQCHAT_PANEL);

	return TRUE;
}
#else
// -------------------------------------------------------------------------
// 处理GS应答
BOOL KChatProtocolProcessor::ProtocolProcess(LPCBYTE pProtocol, UINT nDataLen)
{
	QCONFIRM_RET_FALSE(pProtocol);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pProtocol;
	switch (pData->byProtocol)
	{
	case emKCHAT_PTC_S2C_MSG:
		{
			ProcessRespondMsg(pProtocol, nDataLen);
		}
		break;
	default:
		{
			_ASSERT(FALSE);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL KChatProtocolProcessor::ProcessRespondMsg(LPCBYTE pProtocol, UINT nDataLen)
{
	QCONFIRM_RET_FALSE(pProtocol);
	KCHATPROTO_S2C_MSG* pMsg = (KCHATPROTO_S2C_MSG*)pProtocol;

	// 允许上级服务器发来长消息
	pMsg->szSenderName[KD_CHANNEL_NAME_MAX_LEN-1] = '\0';

	g_ChatChannelManager.ProcessRespond(pMsg->nChannelId, pMsg->nSenderId, pMsg->nReceiverId,
		pMsg->szSenderName, pMsg->szMsg);

	return TRUE;
}
#endif
// -------------------------------------------------------------------------
