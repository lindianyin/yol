/* -------------------------------------------------------------------------
//	文件名		：	KGatewayProcess.h
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:25:24
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGATEWAYPROCESS_H__
#define __KGATEWAYPROCESS_H__

#include "ksocketinterface.h"
#include "ikdbworker.h"

// -------------------------------------------------------------------------

class KG_NameServer;
struct IMemBlock;

struct KRS_CLIENT_SESSION 
{
	KRS_CLIENT_SESSION() {nSessionId = 0; timeLastPing = 0;}
	INT nSessionId;
	time_t timeLastPing;
};

class KGatewayProcess : public IKSocketEventHandler
{
public:
	KGatewayProcess(void);
	virtual ~KGatewayProcess(void);

	BOOL Init(INT nMaxConnectId);
	BOOL Uninit();
	BOOL ProcessEvent(INT nConnectId, INT nEventId, LPCVOID pData, INT nData);
	BOOL ProcessPacket(INT nConnectId, IMemBlock* piPacketBuffer);

	INT OnRequestQueryRoleList(const KProcessingPackage& rcPack);
	INT OnRequestCreateRole(const KProcessingPackage& rcPack);
	INT OnRequestGetRoleData(const KProcessingPackage& rcPack);
	INT OnRequestSaveRoleData(const KProcessingPackage& rcPack);
	INT OnPing(const KProcessingPackage& rcPack);

	BOOL Activate();

	INT OnResultCreateRoleName(const KProcessingPackage& rcPack);
protected:
	INT ProcessRoleDbResults();
	INT ProcessNameServerResults();
	INT SendReplyPackage(UINT64 u64ClientId, IMemBlock* piBuffer);
	INT CheckSessionAlive();

private:
	INT m_nMaxConnId;
	UINT64 m_u64PackageId;
	UINT64 m_u64TotalSend;
	UINT64 m_u64TotalSendOk;
	size_t m_uBytesRecv;
	size_t m_uBytesSend;

	std::vector<KRS_CLIENT_SESSION> m_vConnId2Session;

	typedef INT (KGatewayProcess::*PProtocolProcessFunction)(const KProcessingPackage& rcPack); 
	PProtocolProcessFunction m_afnProtocolFunction[UCHAR_MAX + 1];

	//typedef INT (KGatewayProcess::*PResultProcessFunction)(const KProcessingPackage& rcPack); 
	//PResultProcessFunction m_afnResultFunction[UCHAR_MAX + 1];

};

extern KGatewayProcess g_cGatewayProcess;

// -------------------------------------------------------------------------

#endif /* __KGATEWAYPROCESS_H__ */
