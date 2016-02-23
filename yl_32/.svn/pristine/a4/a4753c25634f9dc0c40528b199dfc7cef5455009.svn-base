
#ifndef __KSTAGEMODULE_H__
#define __KSTAGEMODULE_H__

#include "onlinegameworld/kmodulebase.h"
#include "kstageprotocol.h"

class KStageModule : public KModuleBase
{
public:
	KStageModule();
	~KStageModule() {}

public:
	BOOL OnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

private:
	BOOL c2s_StartMopStage(KPlayer* pPlayer, LPCBYTE pProtocol, UINT nDataLen);
	BOOL c2s_TakeStageAward(KPlayer* pPlayer);
	BOOL c2s_SpeedUp(KPlayer* pPlayer);

};

extern KStageModule g_cStageModule;

#endif
