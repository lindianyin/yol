#pragma once

#include "serverbase/protocol/RelayMasterProtocol.h"

class GlobalServerMasterProtocolHandlerBase
{
public:
	GlobalServerMasterProtocolHandlerBase(R2M_PROTOCOL eProtocolBegin, R2M_PROTOCOL eProtocolEnd) : m_eProtocolBegin(eProtocolBegin),m_eProtocolEnd(eProtocolEnd){}

	virtual BOOL ProcessData(VOID* pData, size_t uDataLen, int nConnIndex) = 0;

	R2M_PROTOCOL m_eProtocolBegin;
	R2M_PROTOCOL m_eProtocolEnd;
};

template <typename Handler>
class GlobalServerMasterProtocolHandler : public GlobalServerMasterProtocolHandlerBase
{
public:
	GlobalServerMasterProtocolHandler(R2M_PROTOCOL protocolBegin, R2M_PROTOCOL protocolEnd) : GlobalServerMasterProtocolHandlerBase(protocolBegin, protocolEnd)
	{
		memset(m_ProcessProtocolFuns, 0, sizeof(m_ProcessProtocolFuns));
		memset(m_uProtocolSize, 0, sizeof(m_uProtocolSize));
	}
	~GlobalServerMasterProtocolHandler()
	{
	}

	typedef BOOL (Handler::*PROCESS_PROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen, int nConnIndex);

public:
	BOOL ProcessData(VOID* pData, size_t uDataLen, int nConnIndex)
	{
		BOOL bSucess = FALSE;
		PROCESS_PROTOCOL_FUNC ProcessFunc = NULL;

		INTERNAL_PROTOCOL_HEADER* pHeader = (INTERNAL_PROTOCOL_HEADER*)pData;
		LOG_PROCESS_ERROR(pHeader);

		LOG_PROCESS_ERROR(uDataLen >= m_uProtocolSize[pHeader->wProtocolID]);

		ProcessFunc = m_ProcessProtocolFuns[pHeader->wProtocolID];
		LOG_PROCESS_ERROR(ProcessFunc);

		bSucess = (((Handler*)this)->*ProcessFunc)((BYTE*)(pHeader), uDataLen, nConnIndex);

EXIT0:
		return bSucess;
	}

protected:
	VOID RegisterProtocolProcessFunction(R2M_PROTOCOL protocol, PROCESS_PROTOCOL_FUNC pProcessFunction, size_t size)
	{
		ASSERT(protocol > m_eProtocolBegin);
		ASSERT(protocol < m_eProtocolEnd);
		m_ProcessProtocolFuns[protocol] = pProcessFunction;
		m_uProtocolSize[protocol] = size;
	}

private:
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[r2m_protocol_end];
	//协议大小数组
	size_t m_uProtocolSize[r2m_protocol_end];
};

class GlobalServerSlaveProtocolHandlerBase
{
public:
	GlobalServerSlaveProtocolHandlerBase(M2R_PROTOCOL eProtocolBegin, M2R_PROTOCOL eProtocolEnd) : m_eProtocolBegin(eProtocolBegin),m_eProtocolEnd(eProtocolEnd){}

	virtual BOOL ProcessData(VOID* pData, size_t uDataLen) = 0;

	M2R_PROTOCOL m_eProtocolBegin;
	M2R_PROTOCOL m_eProtocolEnd;
};

template <typename Handler>
class GlobalServerSlaveProtocolHandler : public GlobalServerSlaveProtocolHandlerBase
{
public:
	GlobalServerSlaveProtocolHandler(M2R_PROTOCOL protocolBegin, M2R_PROTOCOL protocolEnd) : GlobalServerSlaveProtocolHandlerBase(protocolBegin, protocolEnd)
	{
		memset(m_ProcessProtocolFuns, 0, sizeof(m_ProcessProtocolFuns));
		memset(m_uProtocolSize, 0, sizeof(m_uProtocolSize));
	}
	~GlobalServerSlaveProtocolHandler()
	{
	}

	typedef BOOL (Handler::*PROCESS_PROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen);

public:
	BOOL ProcessData(VOID* pData, size_t uDataLen)
	{
		BOOL bSucess = FALSE;
		PROCESS_PROTOCOL_FUNC ProcessFunc = NULL;

		INTERNAL_PROTOCOL_HEADER* pHeader = (INTERNAL_PROTOCOL_HEADER*)pData;
		LOG_PROCESS_ERROR(pHeader);

		LOG_PROCESS_ERROR(uDataLen >= m_uProtocolSize[pHeader->wProtocolID]);

		ProcessFunc = m_ProcessProtocolFuns[pHeader->wProtocolID];
		LOG_PROCESS_ERROR(ProcessFunc);

		bSucess = (((Handler*)this)->*ProcessFunc)((BYTE*)(pHeader), uDataLen);

EXIT0:
		return bSucess;
	}

protected:
	VOID RegisterProtocolProcessFunction(M2R_PROTOCOL protocol, PROCESS_PROTOCOL_FUNC pProcessFunction, size_t size)
	{
		ASSERT(protocol > m_eProtocolBegin);
		ASSERT(protocol < m_eProtocolEnd);
		m_ProcessProtocolFuns[protocol] = pProcessFunction;
		m_uProtocolSize[protocol] = size;
	}

private:
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[m2r_protocol_end];
	//协议大小数组
	size_t m_uProtocolSize[m2r_protocol_end];
};

