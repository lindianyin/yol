
#pragma once

// -------------------------------------------------------------------------
#include "onlinegamemodule/kmoduledef.h"
#include "serverbase/kgcmoduledef.h"
#include "gcmodule/chat/kchatchannel.h"
#include "onlinegameworld/kgcmodulebase.h"
#include "config/kconfigfile.h"
#include "onlinegamebase/ikbadwordfilter.h"

class KChatModule_new : public KChatChannel, public KGcModuleBase
{
public:
	KChatModule_new() : KGcModuleBase(emKMODULE_CHAT_NEW, emKGCMODULE_CHAT) {};

public:
	BOOL OnInit();
	BOOL InitProtocol();
	BOOL OnUnInit();

#ifdef GAME_SERVER

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);
	BOOL SyncMsg2Client(INT nPlayerIndex, INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);

#endif

#ifdef GAME_CLIENT
	BOOL ProcessProtocol(LPCBYTE pbData, UINT uDataLen);
#endif

private:
	BOOL InitFreeWorldSpeaker();

public:
	INT GetFreeWorldSpeaker();
	BOOL SetFreeWorldSpeaker(INT nCount);

private:
	INT m_nFreeWorldSpeakerCount;	// 每日线聊赠送个数
};

extern KChatModule_new g_ChatChannelManager;
extern IKBadwordFilter* g_pChatFilter;

