
#ifdef GAME_CLIENT
#include "stdafx.h"
#include "kchatapply_client.h"
#include "gcmodule/chat/kchatchannelptc.h"
#include "onlinegameworld/kplayerprotocolprocess.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kobjeventsetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
KClientChatProcess_Default g_cClientChatDefault;
KProcessorFactory_Client g_cChatChannelProcessorFactory_Client;
KClientChatProcess_GM g_cClientChatGM;
KClientChatProcess_Private g_cClientChatPrivate;

static BYTE _sbyChatPacketBuffer[KD_CHANNEL_PACKET_MAX_LEN]; // chat网络包缓冲区
static CHAR _pszTempText[KD_CHANNEL_MSG_MAX_LEN];		// chat消息临时字符串
// -------------------------------------------------------------------------
BOOL KClientChatProcess_Default::TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{	
	// NOTE:没有使用nSenderId和pszSenderName是为了防止客户端做假，而这些信息都可以在GS获得
	KCHATPROTO_C2S_MSG& cSendData = *(KCHATPROTO_C2S_MSG* )_sbyChatPacketBuffer;
	cSendData = KCHATPROTO_C2S_MSG();
	cSendData.nChannelId = nChannelId;
	cSendData.nReceiverId = nReceiverId;
	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= KD_CHANNEL_MSG_MAX_LEN);
	QStrCpyLen(cSendData.szMsg, pszMessage, KD_CHANNEL_PACKET_MAX_LEN - sizeof(KCHATPROTO_C2S_MSG));

	KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	pProtocolProcessor->SendLenData(cSendData, sizeof(cSendData) + uMsgLen);
	return TRUE;
}

BOOL KClientChatProcess_Private::TransferToServerByPlayerName(INT nChannelId, INT nSenderId, LPCSTR pszReceiverName, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	BOOL bResult = FALSE;
	KPlayerProtocolProcessor* pProtocolProcessor = NULL;
	
	KCHATPROTO_C2S_PLAYER_MSG* pSendData = (KCHATPROTO_C2S_PLAYER_MSG*)_sbyChatPacketBuffer;
	LOG_PROCESS_ERROR(NULL != pSendData);

	*pSendData = KCHATPROTO_C2S_PLAYER_MSG();

	pSendData->byChannelId = (BYTE)nChannelId;
	QStrCpyLen(pSendData->szReceiverName, pszReceiverName, KD_CHANNEL_NAME_MAX_LEN);

	size_t uMsgLen = strlen(pszMessage) + 1;
	QCONFIRM_RET_FALSE(uMsgLen <= KD_CHANNEL_MSG_MAX_LEN);
	QStrCpyLen(pSendData->szMsg, pszMessage, KD_CHANNEL_PACKET_MAX_LEN - sizeof(KCHATPROTO_C2S_PLAYER_MSG));

	pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);
	pProtocolProcessor->SendLenData(*pSendData, sizeof(KCHATPROTO_C2S_PLAYER_MSG) + uMsgLen);
	bResult = TRUE;

EXIT0:
	return bResult;
}


BOOL KClientChatProcess_Default::ProcessRespondMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	// 过滤非法字符
	CONST CHAR* pszMsg = pszMessage;
	if (!bSysMsg && nChannelId != emKCHATCHANNEL_GM)
	{
		g_pChatFilter->KillBadword(pszMessage, _pszTempText, KD_CHANNEL_MSG_MAX_LEN);
		pszMsg = _pszTempText;
	}

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->CallTableFunction("CallCenter", "OnEvent", 0, "uuuussd", emKOBJEVENTTYPE_PLAYER_MSG_ARRIVE,
		nChannelId, nSenderId, nReceiverId, pszSenderName, pszMsg, bSysMsg);

	return TRUE;
}

BOOL KClientChatProcess_GM::ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
{
	LPCSTR pszPrefix = GM_CMD_PREFIX_CLIENT;
	SIZE_T nPrefixLen = sizeof(GM_CMD_PREFIX_CLIENT) - 1;

	//QCONFIRM_RET(strlen(pszMessage) < KD_CHANNEL_MSG_MAX_LEN, TRUE); // gm指令不限制长度

	if (QStrCmpLen(pszMessage, pszPrefix, nPrefixLen))
	{
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

		cSafeScript->DoBuffer(pszMessage + nPrefixLen);
		return TRUE;
	}

	return FALSE;
}
// -------------------------------------------------------------------------
#endif
