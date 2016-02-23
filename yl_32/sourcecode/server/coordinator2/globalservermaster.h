#pragma once

#include "globalserverprotocolhandler.h"

#define MAX_SLAVE_CR_COUNT	32

class GlobalServerMasterBaseProtocolHandler : public GlobalServerMasterProtocolHandler<GlobalServerMasterBaseProtocolHandler>
{
public:
    GlobalServerMasterBaseProtocolHandler();
	~GlobalServerMasterBaseProtocolHandler();

private:
	BOOL OnHandshakeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL OnPingSignal(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	BOOL DoHandshakeRespond(int nConnIndex);
};

class GlobalServerMaster
{
public:
	struct QSLAVECR_INFO 
	{
		INT nConnIndex;
		ISocketStream* piSocket;
		INT nWorldIndex;
		INT nLastRecvPacketTime;
	};

	typedef std::vector<GlobalServerMasterProtocolHandlerBase*> HANDLER_ARRAY;

    GlobalServerMaster();
	~GlobalServerMaster();	

	BOOL Init();
	BOOL UnInit();	

    VOID Activate();

	BOOL Send(int nConnIndex, IMemBlock* piBuffer);

	BOOL SendGlobal(IMemBlock* piBuffer);		// È«¾Ö·¢ËÍ

    BOOL CloseConnection(int nConnIndex);

	BOOL RegisterProtocolHandler(GlobalServerMasterProtocolHandlerBase* handler);

private:

	GlobalServerMasterProtocolHandlerBase* FindHandler(WORD protocol);

	QSLAVECR_INFO* GetSlaveConnectionByIdx(INT nConnIndex);

	BOOL ProcessConnection(int nConnIndex, ISocketStream* piSocket);

	VOID ProcessAccepter();

private:
	GlobalServerMasterBaseProtocolHandler m_BaseHandler;
	INT m_nLastConnIndex;
	INT m_nConnectionTimeout;
	QSLAVECR_INFO m_SlaveCRList[MAX_SLAVE_CR_COUNT];
	HANDLER_ARRAY m_arrHandlers;
	QSocketAcceptor	m_SocketAcceptor;
};

extern GlobalServerMaster g_GlobalServerMaster;