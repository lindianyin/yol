#pragma once

#include "serverbase/protocol/RelayMasterProtocol.h"
#include "globalserverprotocolhandler.h"
#include "battlezonedef.h"

class KRole;
struct KRoleBaseInfo;

class BattleZoneSlave : public GlobalServerSlaveProtocolHandler<BattleZoneSlave>
{

#if 0
	ISocketStream* m_piSocketStream;
	INT m_nPingCycle;

	BOOL m_bSendErrorFlag;
	time_t m_nLastSendPacketTime;

	INT m_nWorldIndex;

	typedef BOOL (BattleZoneSlave::*PROCESS_PROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen);
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[m2r_protocol_end];

	//协议大小数组
	size_t m_nProtocolSize[m2r_protocol_end];
#endif

public:
	BattleZoneSlave(void);
	~BattleZoneSlave(void);

	BOOL Init();

	BOOL UnInit();

	VOID Activate();

	//INT  GetWorldIndex() { return m_nWorldIndex; }

	void OnBZoneTransferRequest();

	void OnConfirmLoginRespond();
	void OnConfirmLoginRequest();
	void OnRoleDataLoaded();

	void OnSaveRole();
	BOOL DoKickAccount();
	void OnKickAccount();

	BOOL DoBZoneReturnHomeRequest();
	void OnBZoneReturnHomeRequest();

	BOOL DoBZoneReturnHomeRespond();
	void OnBZoneReturnHomeRespond();

	void OnReceivedProtocol(); // dispatch

	BOOL SendGlobal(); // 

	BOOL DoBZoneTransferRequest(KRole* pRole, INT nPlayerIdx, KRoleBaseInfo* pBaseInfo, BYTE *byRoleData, DWORD uRoleDataLen);

	BOOL DoBZoneReturnRespond(DWORD dwRoleId, int nPlayerIndex, int nRespondCode, DWORD dwRoleSourceID, 
		DWORD dwQueueID, int nLoginIP, int nLoginPort, GUID Guid);

private:
	BOOL Send(IMemBlock* piBuffer);

#if 0
	BOOL DoHandshakeRequest();

	BOOL DoPingSignal();

	BOOL ProcessPackage();

	BOOL OnHandshakeRespond(BYTE* pbyData, size_t uDataLen);
#endif

	BOOL OnBZoneTransferRespond(BYTE* pbyData, size_t uDataLen);

	BOOL OnBZoneReturnRequest(BYTE* pbyData, size_t uDataLen);

private:
	std::map<DWORD, KMULTI_FINISH_INFO> m_mapFinishInfos;

public:
	VOID SetMultiFinishInfo(DWORD dwRoleId, INT nMapId, INT nStar);

	VOID RemoveMultiFinishInfo(DWORD dwRoleId);

	VOID OnGetMultiAwardRequest(DWORD dwRoleId);

	VOID OnGetCrossSvExDataRequest(DWORD dwRoleId);

	BOOL GetCrossSvData(DWORD dwRoleId, IMemBlock** ppiRoleData);

private:
	std::map<DWORD, KCrossSvData> m_mapCrossSvDatas;

};

extern BattleZoneSlave g_BZoneSlave;
