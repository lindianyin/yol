
#pragma once

#include "Core/QThread.h"

class QConsoleReader
{
public:
	QConsoleReader(){}
	virtual ~QConsoleReader() { Uninit(); }

	BOOL Init();
	BOOL Uninit();

	PSTR ReadInput();

protected:
	const static SIZE_T LINE_BUFFER_SIZE = 1024;
#ifdef _WIN32
	CHAR m_szBuffer[2][LINE_BUFFER_SIZE]; // 主缓冲区/后备缓冲区
	HANDLE m_hReceiveEvent;
	INT m_nBufferIndex;
	BOOL m_bLineReady;
	BOOL m_bRunFlag;
	static VOID _ThreadFunc(void* pArg);
	VOID ReceiveThreadWorker();
	QThread m_cThread;
#else
	CHAR m_szBuffer[LINE_BUFFER_SIZE];
#endif // _WIN32
};

