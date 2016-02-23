/* -------------------------------------------------------------------------
//	文件名		：	kroleservereyes.h
//	创建者		：	simon
//	创建时间	：	2011/8/2 20:52:42
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KROLESERVEREYES_H__
#define __KROLESERVEREYES_H__

// -------------------------------------------------------------------------

#include "fseye/kipcstream.h"
#include "fseye/fseye_protocol.h"

class KRoleServerEyes
{
public:
	KRoleServerEyes();
	~KRoleServerEyes();

	BOOL Init();
	void UnInit();

	BOOL Activate();

private:
	KIpcStream      m_Stream;
	time_t          m_nNextPingTime;
	time_t          m_nNextSendPlayerCountTime;
	time_t          m_nNextSendCustomInfoTime;

private:
	typedef void (KRoleServerEyes::*PROTOCOL_FUNCTION)(BYTE* pbyData, size_t uDataLen);
	PROTOCOL_FUNCTION m_ProtocolFunctions[fseye_protocol_maxcount + 1];

	void OnE2LStop(BYTE* pbyData, size_t uDataLen);
	void OnE2LGetPid(BYTE* pbyData, size_t uDataLen);
	void OnG2LProtocol(BYTE* pbyData, size_t uDataLen);

private:
	void Ping();
	void SendPlayerCount();
	void SendCustomInfo();
};
// -------------------------------------------------------------------------
extern KRoleServerEyes g_cRoleServerEyes;

#endif /* __KROLESERVEREYES_H__ */
