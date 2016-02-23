/* -------------------------------------------------------------------------
//	文件名		：	gclogicbase/kgclogicbaseprotocol.h
//	创建者		：	luobaohang
//	创建时间	：	2010/7/12 17:40:10
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGCLOGICBASEPROTOCOL_H__
#define __KGCLOGICBASEPROTOCOL_H__

#include "config/gamedef.h"

#pragma pack(push, 1)
#pragma warning(push)
#pragma warning(disable : 4200)

struct KGCLOGIC_BASE_PTC_HEADER
{
	BYTE byFamiliy;
	BYTE byProtocol;
};

// -------------------------------------------------------------------------
enum KE_GC_BASE_PTC
{
	emKGCBASE_PTC_NONE,
	emKGCBASE_PTC_PING,

	emKGCBASE_PTC_PLAYER_LOGIN,		// 玩家上线
	emKGCBASE_PTC_PLAYER_LOGOUT,	// 玩家下线
	emKGCBASE_PTC_S2G_DO_SCRIPT,	// 让GC执行指定脚本

	emKGCBASE_PTC_PLAYER_VAR_CHANGE,		// 变量同步
	emKGCBASE_PTC_PLAYER_SAVE_TRIGGER,	// 同步存档

	emKGCBASE_PTC_SERVER_INFO, // gameserver信息

	emKGCBASE_PTC_COUNT,
};

// GC玩家基础信息
struct KGCPLAYER_BASE_INFO : KGCLOGIC_BASE_PTC_HEADER
{
	DWORD dwPlayerId;
	CHAR szName[MAX_NAME_LEN];
	CHAR szAccount[MAX_NAME_LEN];

	DWORD dwVarSize;
	BYTE abyVar[];
};

struct KGCPLAYER_VAR_CHANGE : KGCLOGIC_BASE_PTC_HEADER
{
	DWORD dwRoleId;
	WORD wGroupId;
	WORD wVarId;
	INT nVal;
};
struct KGCPLAYER_SAVE_TRIGGER : KGCLOGIC_BASE_PTC_HEADER
{
	DWORD dwRoleId;
};

struct KGC_SERVER_INFO : KGCLOGIC_BASE_PTC_HEADER
{
	INT nServerId;
};

// 玩家下线
struct KGCPLAYER_LOGOUT : KGCLOGIC_BASE_PTC_HEADER
{
	DWORD dwPlayerId;
};

struct KLOGICBASE_S2G_DO_GC_SCRIPT : KGCLOGIC_BASE_PTC_HEADER
{
	INT nLength;
	BYTE arBuffer[];
};

// -------------------------------------------------------------------------
enum KEGC2GS_BASE_PTC
{
	emKGC2GS_BASE_PTC_NONE,
	emKGC2GS_BASE_PTC_DO_SCRIPT,	// GC通知GS执行指定脚本

	emKGC2GS_BASE_PTC_COUNT,
};

struct KLOGICBASE_GC2GS_DO_SCRIPT : KGCLOGIC_BASE_PTC_HEADER
{
	UINT uLength;
	BYTE arBuffer[];
};

// -------------------------------------------------------------------------
#pragma warning(pop)
#pragma pack(pop)

#endif /* __KGCLOGICBASEPROTOCOL_H__ */
