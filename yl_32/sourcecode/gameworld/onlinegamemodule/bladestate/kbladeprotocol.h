
#ifndef	__KBLADEPROTOCOL_H__
#define __KBLADEPROTOCOL_H__

#include "onlinegameworld/kprotocol.h"

#pragma pack(1)

enum KC2S_BLADE_PROTOCOL
{
	c2s_blade_protocol_start = 0,
	c2s_start_crazy_request,
};

enum KS2C_BLADE_PROTOCOL
{
	
};

// c2s
struct KC2S_START_CRAZY_REQUEST : KPTC_HEADER<c2s_start_crazy_request>
{

};

#pragma pack()

#endif
