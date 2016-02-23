
#ifndef __KACTIVITYMODULE_H__
#define __KACTIVITYMODULE_H__

#include "onlinegameworld/kmodulebase.h"
#include "kactivityprotocol.h"

class KActivityModule : public KModuleBase
{
public:
	KActivityModule();
	~KActivityModule() {}

public:
	BOOL OnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

private:
	BOOL c2sOnGetKinBattleRecordRequest(KPlayer* pPlayer);

	BOOL c2sOnGetKinBattleWinnerIdRequest(KPlayer* pPlayer);

	BOOL c2sOnGetKinBattleJoinNumRequest(KPlayer* pPlayer);

};

extern KActivityModule g_cActivityModule;

#endif
