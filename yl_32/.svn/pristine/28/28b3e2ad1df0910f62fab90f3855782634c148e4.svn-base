#pragma once

#include "globalserverprotocolhandler.h"

class GlobalServerSlaveBaseProtocolHandler : public GlobalServerSlaveProtocolHandler<GlobalServerSlaveBaseProtocolHandler>
{
public:
	GlobalServerSlaveBaseProtocolHandler();
	~GlobalServerSlaveBaseProtocolHandler();

private:
	BOOL OnHandshakeRespond(BYTE* pbyData, size_t uDataLen);
};

class GlobalServerSlave
{
public:
	GlobalServerSlave();
	~GlobalServerSlave();

	typedef std::vector<GlobalServerSlaveProtocolHandlerBase*> HANDLER_ARRAY;

	BOOL Init();
	BOOL UnInit();

	VOID Activate();

	BOOL RegisterProtocolHandler(GlobalServerSlaveProtocolHandlerBase* handler);

	BOOL Send(IMemBlock* piBuffer);

private:
	GlobalServerSlaveProtocolHandlerBase* FindHandler(WORD protocol);

	BOOL DoHandshakeRequest();

	BOOL DoPingSignal();

	BOOL ProcessPackage();

	BOOL OnHandshakeRespond(BYTE* pbyData, size_t uDataLen);

private:
	GlobalServerSlaveBaseProtocolHandler m_BaseHander;
	ISocketStream* m_piSocketStream;
	INT m_nPingCycle;

	BOOL m_bSendErrorFlag;
	time_t m_nLastSendPacketTime;

	INT m_nWorldIndex;

	HANDLER_ARRAY m_arrHandlers;
};

extern GlobalServerSlave g_GlobalServerSlave;