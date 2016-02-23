/* -------------------------------------------------------------------------
//	文件名		：	ksocketserver.h
//	创建者		：	luobaohang
//	创建时间	：	2010/6/4 9:39:13
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KSOCKETSERVER_H__
#define __KSOCKETSERVER_H__
#include "serverbase/kg_censerver.h"
struct IKProcessServer;
// -------------------------------------------------------------------------
class KSocketServer : public KG_CenServer
{
public:
	static KSocketServer* Inst();
	BOOL Init(LPCSTR szListenIp, INT nPort);
	VOID UnInit();
	VOID Breath();
	BOOL SendPacket(INT nConnectId, IMemBlock* piBuffer);
	BOOL Send2RandomPacket(IMemBlock* piBuffer);
	BOOL RegisterGsProcessor(BYTE byModuleId, IKProcessServer* piProcessor);
	VOID OnConnectCreated(INT nID, KCenServerConnect *pConnect);
	BOOL ReAssignServerId(INT nConnectId, INT nServerId); // this trick replaces ConnectionId with real ServerId
private:
	KSocketServer();
	INT ProcessPacket(LPBYTE pData, UINT uSize, INT nConnectId) ;
private:
	IKProcessServer* m_apiProcessor[255];
	INT m_nLoopCount;
};

// -------------------------------------------------------------------------

#endif /* __KSOCKETSERVER_H__ */
