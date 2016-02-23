/* -------------------------------------------------------------------------
//	文件名		：	kconsolereader.cpp
//	创建者		：	simon
//	创建时间	：	2011/4/22 1:47:48
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "CorePrivate.h"
#include "Misc/qconsolereader.h"

#ifdef unix
#include <unistd.h>
#include <fcntl.h>
#endif // unix

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

PSTR QConsoleReader::ReadInput()
{
	PSTR pString = NULL;
#ifdef _WIN32
	if (m_bLineReady)
	{
		m_bLineReady = FALSE;
		pString = m_szBuffer[m_nBufferIndex];
		m_nBufferIndex ^= 1; // 切换缓冲区
		SetEvent(m_hReceiveEvent); // 通知继续接受键盘输入
	}
#endif // _WIN32
#ifdef unix
		pString = fgets(m_szBuffer, sizeof(m_szBuffer), stdin);
#endif // unix
	return pString;
}

#ifdef _WIN32

VOID QConsoleReader::_ThreadFunc( void* pArg )
{
	QConsoleReader* pThis = (QConsoleReader*)pArg;
	pThis->ReceiveThreadWorker();
}

VOID QConsoleReader::ReceiveThreadWorker()
{
	HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
	while (m_bRunFlag)
	{
		DWORD dwWaitResult = WaitForSingleObject(m_hReceiveEvent, 200);
		if (dwWaitResult == WAIT_TIMEOUT)
			continue;
		QCONFIRM_CONTINUE(dwWaitResult == WAIT_OBJECT_0);

		DWORD dwNum = 0;
		ReadConsole(hConsole, m_szBuffer[m_nBufferIndex], sizeof(m_szBuffer[m_nBufferIndex]) - 1, &dwNum, NULL);
		if (dwNum)
		{
			m_szBuffer[m_nBufferIndex][dwNum] = '\0';
			m_bLineReady = TRUE;
		}
	}
}

#endif // _WIN32

BOOL QConsoleReader::Init()
{
	ZeroStruct(m_szBuffer);
#ifdef _WIN32
	m_hReceiveEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	QCONFIRM_RET_FALSE(m_hReceiveEvent != INVALID_HANDLE_VALUE);

	m_bRunFlag = TRUE;
	m_nBufferIndex = 0;
	m_bLineReady = FALSE;

	INT nRet = m_cThread.Create(_ThreadFunc, this);
	QCONFIRM_RET_FALSE(nRet && "Create ConsoleReader Thread");
#endif // _WIN32

#ifdef unix
	int fd = fileno(stdin);
	int fcflags = fcntl(fd, F_GETFL);
	QCONFIRM_RET_FALSE(fcflags >= 0);
	int nresult = fcntl(fd,F_SETFL,fcflags | O_NONBLOCK); // 非阻塞模式stdio，保持默认line-buffered
	QCONFIRM_RET_FALSE(nresult >= 0);
#endif // unix
	return TRUE;
}

BOOL QConsoleReader::Uninit()
{
#ifdef _WIN32
	m_bRunFlag = FALSE;
	HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
	if (hConsole && hConsole != INVALID_HANDLE_VALUE &&
		m_hReceiveEvent && m_hReceiveEvent != INVALID_HANDLE_VALUE)
	{
		SetEvent(m_hReceiveEvent);

		DWORD dwWritten = 0;
		INPUT_RECORD sInput;
		sInput.EventType = KEY_EVENT;
		sInput.Event.KeyEvent.bKeyDown = TRUE;
		sInput.Event.KeyEvent.dwControlKeyState = 0;
		sInput.Event.KeyEvent.uChar.AsciiChar = '\r';
		sInput.Event.KeyEvent.wRepeatCount = 1;
		sInput.Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
		sInput.Event.KeyEvent.wVirtualScanCode = 0;
		WriteConsoleInput(hConsole, &sInput, 1, &dwWritten); // make ReadConsole() return gracefully
		if (dwWritten)
			m_cThread.Wait();
		else
			m_cThread.Terminate(0);
		//CloseHandle(m_hReceiveEvent);
	}
#endif // _WIN32
	return TRUE;
}
