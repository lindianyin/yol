
#ifndef __KINSTANCEMODULE_H__
#define __KINSTANCEMODULE_H__

#include "onlinegameworld/kmodulebase.h"
#include "kinstanceprotocol.h"

class KInstanceModule : public KModuleBase
{
public:
	KInstanceModule();
	~KInstanceModule() {}

public:
	BOOL OnInit();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	BOOL InitProtocol();

	static BOOL SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);

	static BOOL SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);

private:
	BOOL c2sOnGetOverallTeamRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sOnCreateOverallTeamRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sOnJoinOverallTeamRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sOnLeaveOverallTeamRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sOnMemberPrepareRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sOnGetPlayerTeamInfoRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sOnKickOverallPlayerRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sAddPlayerToWatch(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sDelPlayerToWatch(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sBroadcastOverallTeamMsg(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sStartMission(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

	BOOL c2sMemberCountDownRequest(KPlayer* pPlayer, LPCBYTE pProtocol, UINT uDataLen);

};

extern KInstanceModule g_cInstanceModule;

#endif
