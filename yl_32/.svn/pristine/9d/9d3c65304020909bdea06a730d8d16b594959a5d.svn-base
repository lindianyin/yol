/* -------------------------------------------------------------------------
//	文件名		：	kmailmodule.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/23 15:06:45
//	功能描述	：	邮件模块
//
// -----------------------------------------------------------------------*/
#ifndef __KMAILMODULE_H__
#define __KMAILMODULE_H__

// -------------------------------------------------------------------------
#include "onlinegameworld/kgcmodulebase.h"

class KMailModule : public KGcModuleBase
{
public:
	KMailModule();
	virtual ~KMailModule();

public:
	BOOL OnInit();
	BOOL OnUnInit();
	BOOL InitProtocol();

#ifdef GAME_SERVER
	//处理GC发过来协议
	BOOL ProcessGcProtocol(LPCBYTE pbData, UINT uDataLen);
	// 处理玩家客户端协议
	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL SendGcMsg(LPCVOID pData, UINT uSize);
	BOOL SendClientMsg(DWORD dwPlayerId, KPTC_HEADER_BASE& rPtcData);
	BOOL SendVarLenClientMsg(DWORD dwPlayerId, KLENPTC_HEADER_BASE& rPtcData, UINT uDataLen);
#endif

#ifdef GAME_CLIENT
	// 处理GS发过来协议
	BOOL ProcessProtocol(LPCBYTE pbData, UINT uDataLen);

	BOOL SendGsMsg(KPTC_HEADER_BASE& rPtcData);
	BOOL SendVarLenGsMsg(KLENPTC_HEADER_BASE& rPtcData, UINT uDataLen);
#endif
};

extern KMailModule g_cMailModule;

// -------------------------------------------------------------------------

#endif /* __KMAILMODULE_H__ */
