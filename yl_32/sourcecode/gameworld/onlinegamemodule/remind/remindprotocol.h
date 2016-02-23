#pragma once
#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)

enum REMIND_S2C_PROTOCOL
{
	emS2C_ADD_REMIND,
	emS2C_REMOVE_REMIND,
	emS2C_PROTOCOL_COUNT,
};

struct PTC_S2C_ADD_REMIND : KPTC_HEADER<emS2C_ADD_REMIND>
{
	INT nRemindId;
	INT nCountDown;
	UINT nStartTime;
};

struct PTC_S2C_REMOVE_REMIND : KPTC_HEADER<emS2C_REMOVE_REMIND>
{
	INT nRemindId;
};


#pragma pack(pop)