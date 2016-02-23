/* -------------------------------------------------------------------------
//	文件名		：	kawardmodule.h
//	创建者		：	zhaoyu
//	创建时间	：	2013-08-14
//	功能描述	：  
// -------------------------------------------------------------------------*/

#ifndef __KAWARDMODULE_H__
#define __KAWARDMODULE_H__

#include "onlinegameworld/kmodulebase.h"
#include "onlinegameworld/kplayer.h"

class KAwardModule : public KModuleBase
{
public:
	KAwardModule();
	~KAwardModule() { }

public:
	BOOL OnInit();

	BOOL OnUnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData );

	BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize );

	BOOL SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize );

private:

	BOOL c2sTakeAwardRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL InitProtocol();
};

extern KAwardModule g_cAwardModule;

#endif // __KAWARDMODULE_H__