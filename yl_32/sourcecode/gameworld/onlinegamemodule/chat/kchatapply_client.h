
#pragma once

// -------------------------------------------------------------------------

#ifdef GAME_CLIENT
#include "gcmodule/chat/kchatchannel.h"

// -------------------------------------------------------------------------
class KClientChatProcess_Default : public IKChatChannelProcessor
{
public:
	virtual BOOL TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
	virtual BOOL ProcessRespondMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
};
extern KClientChatProcess_Default g_cClientChatDefault;

//к╫ад
class KClientChatProcess_Private : public KClientChatProcess_Default
{
public:
	virtual BOOL TransferToServerByPlayerName(INT nChannelId, INT nSenderId, LPCSTR pszReceiverName, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
};

extern KClientChatProcess_Private g_cClientChatPrivate;

class KClientChatProcess_GM : public KClientChatProcess_Default
{
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
};
extern KClientChatProcess_GM g_cClientChatGM;

class KProcessorFactory_Client : public IKChatChannelProcessorFactory
{
	IKChatChannelProcessor* GetChannelProcessor(CHAT_CHANNEL_TYPE emChannelType)
	{
		QCONFIRM_RET_FALSE(emChannelType > trInvalid && emChannelType < trTotal);

		switch (emChannelType)
		{
		case emKCHATCHANNEL_GM:
			return &g_cClientChatGM;
		default:
			return &g_cClientChatDefault;
		}
	}
};

extern KProcessorFactory_Client g_cChatChannelProcessorFactory_Client;
#endif
// -------------------------------------------------------------------------
