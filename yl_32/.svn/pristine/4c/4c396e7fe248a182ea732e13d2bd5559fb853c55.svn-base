
#ifndef __QESCORTMODULE_H__
#define __QESCORTMODULE_H__

#include "onlinegameworld/kmodulebase.h"
#include "kescortprotocol.h"

class KEscortModule : public KModuleBase
{
public:
	KEscortModule();
	~KEscortModule() {}

public:
	BOOL OnInit();
	BOOL InitProtocol();
	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

public:
	BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData );

	BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize );

public:
	BOOL OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId);

private:
	BOOL c2sRefreshEscorts(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sStartEscort(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sTakeEscortAward(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sRobEscortRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sDelRobEscortRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sTakeRobAwardRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sRobFightRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sRescueFightRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sRescuePlayerRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

};

extern KEscortModule g_cEscortModule;

#endif
