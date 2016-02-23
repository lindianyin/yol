
#ifndef __KRANKMODULE_H__
#define __KRANKMODULE_H__

#include "onlinegameworld/kmodulebase.h"
#include "krankprotocol.h"

class KRankModule : public KModuleBase
{
public:
	KRankModule();
	~KRankModule() {}

public:
	BOOL OnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

private:
	BOOL c2sOnSyncUpdateTimeRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);
	BOOL c2sOnSyncRankRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

};

extern KRankModule g_cRankModule;

#endif
