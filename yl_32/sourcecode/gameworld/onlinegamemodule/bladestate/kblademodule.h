
#ifndef __KBLADEMODULE_H__
#define __KBLADEMODULE_H__

#include "onlinegameworld/kmodulebase.h"
#include "kbladeprotocol.h"

class KBladeModule : public KModuleBase
{
public:
	KBladeModule();
	~KBladeModule() {}

public:
	BOOL OnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

private:
	BOOL c2sOnStartCrazyRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

};

extern KBladeModule g_cBladeModule;

#endif
