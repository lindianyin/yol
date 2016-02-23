#pragma once

#include "onlinegameworld/kprotocol.h"
#include "awardlobbydef.h"

#pragma pack(push, 1)

enum KC2S_INSTANCE_PROTOCOL
{
	c2s_awardlobby_protocol_start = 0,
	c2s_signed_in_request,
	c2s_signed_in_sync_request,
};

enum KS2C_INSTANCE_PROTOCOL
{
	s2c_sync_signedin_info,
	s2c_sync_sevenday_info,
	s2c_sync_activity_info,
};


// c2s
struct KC2S_SIGNED_IN_REQUEST : KPTC_HEADER<c2s_signed_in_request>
{
	INT nDay;
};

struct KC2S_SIGNED_IN_SYNC_REQUEST : KPTC_HEADER<c2s_signed_in_sync_request>
{
	INT nSync;
};


// s2c
struct KS2C_SYNC_SIGNEDIN_INFO : KPTC_HEADER<s2c_sync_signedin_info>
{
	INT nSignedNum;
	DWORD dwCreateTime;
};

struct KS2C_SYNC_SEVENDAY_INFO : KPTC_HEADER<s2c_sync_sevenday_info>
{
	INT nLoginDays;
	INT nMaxLogindays;
};

struct KLobbyActivityInfo
{
	INT nId;
	INT nAward;
};

struct KS2C_SYNC_ACTIVITY_INFO : KLENPTC_HEADER<s2c_sync_activity_info>
{
	INT nCount;
	KLobbyActivityInfo arrInfo[0];
};

#pragma pack(pop)