#pragma once

#include "serverbase/protocol/RelayMasterProtocol.h"
#include "globalserverprotocolhandler.h"
#include "battlezonedef.h"

class KRole;

struct KSLAVECR_INFO 
{
	INT nConnIndex;
	ISocketStream* piSocket;
	INT nWorldIndex;
	INT nLastRecvPacketTime;
};

struct KRoleData
{
	// 不超过64K
	DWORD uRoleDataLen;
	BYTE RoleData[100000];
};

//#define MAX_SLAVE_CR_COUNT	6

class BattleZoneMaster : public GlobalServerMasterProtocolHandler<BattleZoneMaster>
{
private:
	typedef std::map<DWORD, KRoleData>  KROLE_DATA_TABLE;
	KROLE_DATA_TABLE m_mapRoleData;

	typedef std::map<DWORD, INT> KROLE_SLAVEIDX_TABLE;
	KROLE_SLAVEIDX_TABLE m_mapRoleSlave;

	//INT m_nLastConnIndex;

	//INT m_nConnectionTimeout;

	//QSocketAcceptor	m_SocketAcceptor;

	//typedef BOOL (BattleZoneMaster::*PROCESS_PROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	//PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[r2m_protocol_end];

	//协议大小数组
	//size_t m_uProtocolSize[r2m_protocol_end];

	//KSLAVECR_INFO m_SlaveCRList[MAX_SLAVE_CR_COUNT];

public:
	BattleZoneMaster(void);
	~BattleZoneMaster(void);

	BOOL Init();

	BOOL UnInit();

	//BOOL CloseConnection(int nConnIndex);

	VOID Activate();

	//VOID ProcessAccepter();

	//BOOL ProcessConnection(int nConnIndex, ISocketStream* piSocket);

	//BOOL RelayBZoneTransferRequest();

	//VOID OnReceivedGlobal(); // dispatch

	//BOOL DoSend(INT nConnIdx);

	BOOL Send(int nConnIndex, IMemBlock* piBuffer);

	BOOL GetRoleData(DWORD dwRoleID, IMemBlock** ppiRoleData);

	BOOL DoBZoneTransferRespond(DWORD dwRoleId, int nPlayerIndex, int nRespondCode, DWORD dwRoleSourceID, 
		DWORD dwQueueID, int nLoginIP, int nLoginPort, GUID Guid);

	BOOL DoBZoneReturnRequest(KRole* pRole, KRoleBaseInfo* pBaseInfo, INT nExValue, LPBYTE byExData, DWORD uExDataLen);

private:
	//KSLAVECR_INFO* GetSlaveConnectionByIdx(INT nConnIndex);

	INT GetRoleSlaveIdx(DWORD dwRoleId);

	//BOOL OnHandshakeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	//BOOL OnPingSignal(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnBZoneTransferRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnBZoneReturnRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	//BOOL DoHandshakeRespond(int nConnIndex);

	BOOL DoAddPlayerRequest(int nConnIndex, KRole* pRole, KRoleBaseInfo& RoleBaseInfo);

	BOOL LoadMultiMapPos();

	BOOL GetMultiPos(INT nMapId, KMULTI_MAP_POS& sPos);

private:
	std::map<INT, KMULTI_MAP_POS> m_mapMultiPos;

};

extern BattleZoneMaster g_BZoneMaster;
