
#pragma once

#include "Protocol/KG_CSLogServerProtocal.h"

#define MAX_LOG_BUFFER_SIZE 1024

class KRole;

#define PROTOCOL_START_SIZE (4)
#define P2R_PROTOCOL_BEGIN (0)
#define P2R_PROTOCOL_CHECK_NAME (1)
#define P2R_PROTOCOL_PAY (2)
#define P2R_PROTOCOL_TOTAL (3)

class KPayAgency
{
public:
    KPayAgency();

    BOOL Init();
    void UnInit();

    void Activate();
private:
	BOOL ProcessAcceptor();
	BOOL ProcessPackage();
	BOOL CheckTimeout();
    BOOL Send(IMemBlock* piBuffer);

	typedef BOOL (KPayAgency::*PROTOCOL_FUNCTION)(BYTE* pbyData, size_t uDataLen);

	PROTOCOL_FUNCTION m_ProtocolFunctions[P2R_PROTOCOL_TOTAL];
	size_t            m_uPakSize[P2R_PROTOCOL_TOTAL];

	BOOL OnCheckName(BYTE* pbyData, size_t uDataLen);

	BOOL OnUserPay(BYTE* pbyData, size_t uDataLen);

public:
    char              m_szIdentity[_NAME_LEN];

private:
	QSocketServerAcceptor	m_cSockServer;	// Server Linstener，用于监听GameClient
	QSOCKET_EVENT* m_pSockEvents;	// 监听事件的缓冲区指针
	QSocketAcceptor		m_Acceptor;
    ISocketStream*		m_piSocket;
    int					m_nPingCycle;
	time_t              m_nLastPingTime;
    time_t				m_nNextPingTime;
    char				m_szTempBuffer[MAX_LOG_BUFFER_SIZE];
};

