

#include "stdafx.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "gcmodule/chat/kchatchannelptc.h"
#include "kchatprotocolprocess.h"
#include "kchatapply_gs.h"
#include "kchatapply_client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// -------------------------------------------------------------------------
KChatModule_new g_ChatChannelManager;
IKBadwordFilter* g_pChatFilter;

static BYTE _sbyChatPacketBuffer[KD_CHANNEL_PACKET_MAX_LEN]; // chat网络包缓冲区

template<>
IKModuleInterface* GetModuleInterFace<emKMODULE_CHAT_NEW>()
{
	return &g_ChatChannelManager;
}

BOOL KChatModule_new::OnInit()
{
#ifdef GAME_CLIENT
	SetProcessorFactory(&g_cChatChannelProcessorFactory_Client);
#else
	SetProcessorFactory(&g_cChatChannelProcessorFactory_GS);
#endif
	QCONFIRM_RET_FALSE(KChatChannel::Init());
	QCONFIRM_RET_FALSE(InitFreeWorldSpeaker());

	//g_pChatFilter = CreateBadWordFilter();
	//g_pChatFilter->Init();
	//g_pChatFilter->LoadFilterFile(KDF_CHAT_FILTER_WORDS);

	return TRUE;
}

BOOL KChatModule_new::InitProtocol()
{
	KD_PTC_LENGTNED_SIZE_C2S(KCHATPROTO_C2S_MSG);
	KD_PTC_LENGTNED_SIZE_C2S(KCHATPROTO_C2S_PLAYER_MSG);

	KD_PTC_LENGTNED_SIZE_S2C(KCHATPROTO_S2C_MSG);
	KD_PTC_FIXED_SIZE_S2C(KCHATPROTO_S2C_DISABLE_INFO);
	return TRUE;
}

BOOL KChatModule_new::OnUnInit()
{
	SAFE_RELEASE(g_pChatFilter);
	return TRUE;
}

#ifdef GAME_SERVER

	BOOL KChatModule_new::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
	{
		return KChatProtocolProcessor::ProtocolProcess(nPlayerIndex, pbData, uDataLen);
	}

	BOOL KChatModule_new::SyncMsg2Client(INT nPlayerIndex, INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg)
	{
		QCONFIRM_RET_FALSE(nPlayerIndex > 0);

		KCHATPROTO_S2C_MSG& cSendData = *(KCHATPROTO_S2C_MSG* )_sbyChatPacketBuffer;
		cSendData = KCHATPROTO_S2C_MSG();
		cSendData.nChannelId = nChannelId;
		cSendData.nSenderId = nSenderId;
		QCONFIRM_RET_FALSE(strlen(pszSenderName) < KD_CHANNEL_NAME_MAX_LEN);
		QStrCpyLen(cSendData.szSenderName, pszSenderName, sizeof(cSendData.szSenderName));
		cSendData.nReceiverId = nReceiverId;

		size_t uMsgLen = strlen(pszMessage) + 1;
		//QCONFIRM_RET_FALSE(cSendData.nMsgLen <= KD_CHANNEL_MSG_MAX_LEN); // 允许发长消息
		QStrCpyLen(cSendData.szMsg, pszMessage, sizeof(_sbyChatPacketBuffer) - sizeof(cSendData));

		KPlayerProtocolProcessor* pProtocolProcessor = g_ChatChannelManager.GetProtocolProcessor();
		pProtocolProcessor->SendLenData(nPlayerIndex,cSendData, sizeof(cSendData) + uMsgLen);
		return TRUE;
	}

#else
	BOOL KChatModule_new::ProcessProtocol(LPCBYTE pbData, UINT uDataLen)
	{
		return KChatProtocolProcessor::ProtocolProcess(pbData, uDataLen);
	}
#endif

BOOL KChatModule_new::InitFreeWorldSpeaker()
{
	QIniFile iniChannelCfg;
	if (!iniChannelCfg.Load(KD_CHANNEL_SETTING_FILE))
	{
		Q_Printl("Failed To Open" << KD_CHANNEL_SETTING_FILE);
		return FALSE;
	}
	
	iniChannelCfg.GetInteger("WorldChannel", "nFreeWorldSpeakerCount", 10, &m_nFreeWorldSpeakerCount);
	return TRUE;
}

INT KChatModule_new::GetFreeWorldSpeaker()
{
	return m_nFreeWorldSpeakerCount;
}

BOOL KChatModule_new::SetFreeWorldSpeaker(INT nCount)
{
	m_nFreeWorldSpeakerCount = nCount;
	return TRUE;
}

// -------------------------------------------------------------------------
