#pragma once

#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)
enum TITLE_S2C_PROTOCOL
{
	emTITLE_PTC_S2C_SYNC_TITLE,
	emTITLE_PTC_S2C_ADD_TILLE,

	emTITLE_PTC_S2C_COUNT,
};

struct TITLE_INFO
{
	INT nTitleId;
	BYTE byUse;
};

struct TITLE_S2C_SYNC_TITLE : KLENPTC_HEADER<emTITLE_PTC_S2C_SYNC_TITLE>
{
	INT nCount;
	TITLE_INFO arrInfo[0];
};

struct TITLE_S2C_ADD_TITLE : KPTC_HEADER<emTITLE_PTC_S2C_ADD_TILLE>
{
	INT nTitleId;
};

#pragma pack(pop)