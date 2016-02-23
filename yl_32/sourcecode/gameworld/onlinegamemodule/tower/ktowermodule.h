
#ifndef __KTOWERMODULE_H__
#define __KTOWERMODULE_H__

#include "onlinegameworld/kmodulebase.h"
#include "ktowerprotocol.h"

class KTowerModule : public KModuleBase
{
public:
	KTowerModule();
	~KTowerModule() {}

public:
	BOOL OnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

private:
	BOOL c2s_StartMopTower(KPlayer* pPlayer);

	BOOL c2s_StopMopTower(KPlayer* pPlayer);

	BOOL c2s_TakeAward(KPlayer* pPlayer);

	BOOL c2s_SpeedUp(KPlayer* pPlayer);

	BOOL c2s_FinishMopNow(KPlayer* pPlayer);

};

extern KTowerModule g_cTowerModule;

#endif
