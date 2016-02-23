/* -------------------------------------------------------------------------
//	文件名		：	buffermanager.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-8
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __BUFFER_PROTOCOL_H__
#define __BUFFER_PROTOCOL_H__

// -------------------------------------------------------------------------
#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)
// -------------------------------------------------------------------------
enum BUFFER_C2S_PROTOCOL
{
	BUFFER_PTC_C2S_NONE = 0,
	BUFFER_PTC_C2S_BEGBUFFERLIST,
};

enum BUFFER_S2C_PROTOCOL
{
	BUFFER_PTC_S2C_NONE = 0,
	BUFFER_PTC_S2C_ADDBUFFER,
	BUFFER_PTC_S2C_REMOVEBUFFER,
	BUFFER_PTC_S2C_STACKBUFFER,
};

// -------------------------------------------------------------------------
//                server - client
// -------------------------------------------------------------------------
struct BUFFER_ADDBUFFER : KPTC_HEADER<BUFFER_PTC_S2C_ADDBUFFER>
{
	DWORD nCharacterID;
	DWORD dwCasterID;
	BYTE  bGroundBuffer;	//是否是对地
	BYTE  bStackChange;		//是否是叠加
	BYTE  bAddOnLogin;		//是否是登陆加载存盘buff
	INT   nX;
	INT   nY;
	INT   nZ;
	INT	  nBuffTemplateId;
	INT	  nBuffInterval;
	INT	  nStackCount;
	DWORD dwBufferID;
};

struct BUFFER_REMOVEBUFFER : KPTC_HEADER<BUFFER_PTC_S2C_REMOVEBUFFER>
{
	DWORD dwCharacterId;
	BOOL  bGroundBuffer;
	INT	  nBuffTemplateId;
	DWORD dwBufferID;
};

struct BUFFER_STACKBUFFER : KPTC_HEADER<BUFFER_PTC_S2C_STACKBUFFER>
{
	DWORD dwCharacterId;
	DWORD dwCasterID;
	DWORD dwBufferID;
	INT	  nBuffTemplateId;
	INT nBuffInterval;
	BYTE byStack;
};

#endif