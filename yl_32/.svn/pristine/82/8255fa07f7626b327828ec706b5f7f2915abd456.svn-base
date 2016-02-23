
#pragma once

#include "onlinegameworld/kprotocol.h"
#include "gcmodule/chat/kchanneldef.h"

// -------------------------------------------------------------------------
#pragma pack(push, 1)

// CLIENT->GS
enum KE_CHAT_C2S_PROTOCOL
{
	emKCHAT_PTC_C2S_NONE,
	emKCHAT_PTC_C2S_MSG,
	emKCHAT_PTC_C2S_PLAYER_MSG,
};

struct KCHATPROTO_C2S_MSG : KLENPTC_HEADER<emKCHAT_PTC_C2S_MSG>
{
	BYTE	nChannelId;
	INT		nReceiverId;
	//WORD	nMsgLen;
	CHAR	szMsg[];
};

struct KCHATPROTO_C2S_PLAYER_MSG : KLENPTC_HEADER<emKCHAT_PTC_C2S_PLAYER_MSG>
{
	BYTE	byChannelId;
	CHAR	szReceiverName[KD_CHANNEL_NAME_MAX_LEN];
	//WORD	nMsgLen;
	CHAR	szMsg[0];
};

// -------------------------------------------------------------------------
// GS->CLIENT
enum KE_CHAT_S2C_PROTOCOL
{
	emKCHAT_PTC_S2C_NONE,
	emKCHAT_PTC_S2C_MSG,
	emKCHAT_PTC_S2C_DISABLE_INFO,		// ·¢ËÍ½ûÑÔÐÅÏ¢
};

struct KCHATPROTO_S2C_MSG : KLENPTC_HEADER<emKCHAT_PTC_S2C_MSG>
{
	BYTE	nChannelId;
	BYTE    byShownType;
	INT		nSenderId;
	CHAR	szSenderName[KD_CHANNEL_NAME_MAX_LEN];
	INT		nReceiverId;

	//WORD	nMsgLen;
	CHAR	szMsg[];
};

struct KCHATPROTO_S2C_DISABLE_INFO : KPTC_HEADER<emKCHAT_PTC_S2C_DISABLE_INFO>
{
	BOOL bDisableAuth;
	INT nDisableChannel;
	DWORD dwDisableEndTime;
};

#pragma pack(pop)
// -------------------------------------------------------------------------

