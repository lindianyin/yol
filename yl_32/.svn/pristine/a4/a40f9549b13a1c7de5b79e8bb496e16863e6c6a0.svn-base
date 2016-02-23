#pragma once

#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)

enum QFINDPATH_PROTOCOL
{
	emFINDPATH_S2C_AUTO_GOTO = 0
};

struct QPTC_FINDPATH_S2C_AUTOGOTO : KPTC_HEADER<emFINDPATH_S2C_AUTO_GOTO>
{
    DWORD dwMapID;
	INT nX, nY, nZ;
};