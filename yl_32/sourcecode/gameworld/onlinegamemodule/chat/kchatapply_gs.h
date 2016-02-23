
#pragma once

// -------------------------------------------------------------------------

#ifdef GAME_SERVER
#include "gcmodule/chat/kchatchannel.h"

// -------------------------------------------------------------------------
class KGSChatProcess_Default : public IKChatChannelProcessor
{
public:
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
	virtual BOOL TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);

private:
};

class KGSChatProcess_Nearby : public IKChatChannelProcessor
{
public:
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};

class KGSChatProcess_Private : public KGSChatProcess_Default
{
public:
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
	virtual BOOL ProcessSendMsgToPlayer(INT nChannelId, INT nSenderId, LPCSTR pszReceiver, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};

class KGSChatProcess_World : public KGSChatProcess_Default
{
public:
	//virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage);
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};

class KGSChatProcess_Global : public KGSChatProcess_Default
{
public:
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);

	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};
// 本场景 add by NN
class KGSChatProcess_Scene : public KGSChatProcess_Default
{
public:
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);

	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};

class KGSChatProcess_Team : public KGSChatProcess_Default
{
public:
	//virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage);
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};

// 门派频道  add by NN
class KGSChatProcess_Faction : public KGSChatProcess_Default
{
public:
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};

class KGSChatProcess_Kin : public KGSChatProcess_Default
{
public:
	//virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage);
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};

class KGSChatProcess_Group : public KGSChatProcess_Default
{
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};

class KGSChatProcess_GM : public KGSChatProcess_Default
{
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType);
};

// -------------------------------------------------------------------------
extern KGSChatProcess_Default	g_cGSChatDefault;
extern KGSChatProcess_Nearby	g_cGSChatNearby;
extern KGSChatProcess_Private	g_cGSChatPrivate;
extern KGSChatProcess_World		g_cGSChatWorld;
extern KGSChatProcess_Scene		g_cGSChatScene;
extern KGSChatProcess_Team		g_cGSChatTeam;
extern KGSChatProcess_Faction	g_cGsChatFaction;
extern KGSChatProcess_Kin		g_cGSChatKin;
extern KGSChatProcess_Group		g_cGSChatGroup;
extern KGSChatProcess_GM		g_cGSChatGM;
extern KGSChatProcess_Global	g_cGSChatGlobal;

class KProcessorFactory_GS : public IKChatChannelProcessorFactory
{
	IKChatChannelProcessor* GetChannelProcessor(CHAT_CHANNEL_TYPE emChannelType)
	{
		QCONFIRM_RET_FALSE(emChannelType > trInvalid && emChannelType < trTotal);

		switch (emChannelType)
		{
		case trNearby:
			return &g_cGSChatNearby;
		case trWhisper:
		case trLocalSys:
			return &g_cGSChatPrivate;
		case trGlobalSys:
			return &g_cGSChatGlobal;
		case trSence:
			return &g_cGSChatScene;
		case trWorld:
			return &g_cGSChatWorld;
		case trCustomCh1:
			return &g_cGSChatGroup;
		case trGmMessage:
			return &g_cGSChatGM;
		case trTeam:
			return &g_cGSChatTeam;
		case trFaction:
			return &g_cGsChatFaction;
		case trTong:
			return &g_cGSChatKin;
		default:
			return &g_cGSChatDefault;
		}
	}
};

extern KProcessorFactory_GS g_cChatChannelProcessorFactory_GS;
#endif
// -------------------------------------------------------------------------
