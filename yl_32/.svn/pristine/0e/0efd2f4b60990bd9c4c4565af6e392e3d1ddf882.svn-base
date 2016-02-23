
#pragma once

#include <iostream>
#include <sstream>
#include <stdio.h>


#ifdef _WIN32
	class QConsoleHelper
	{
	public:
		static inline VOID DoInfoColor()
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_GREEN/* | FOREGROUND_INTENSITY*/);
		}
		static inline VOID DoErrorColor()
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				BACKGROUND_RED |
				FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		// 偷懒直接设置黑底白字
		static inline VOID RestoreColor()
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		}

		static inline BOOL CreateConsole()
		{
			if ( ! AllocConsole()) // 创建控制台
				return FALSE;
			SetConsoleTitle(TEXT("[Console] YOL控制台"));
			freopen("CONOUT$", "w", stdout); // 重定向标准输出
			freopen("CONOUT$", "w", stderr);
			freopen("CONIN$", "r", stdin);
			std::ios::sync_with_stdio(); // STL同步
			return TRUE;
		}
	};

#else // linux或其他符合ECMA-48的系统的控制台
	class QConsoleHelper
	{
	public:
		static inline VOID DoInfoColor()
		{
			fputs("\033[49;32m", stdout);
		}
		static inline VOID DoErrorColor()
		{
			fputs("\033[41;37m", stdout);
		}
		static inline VOID RestoreColor()
		{
			fputs("\033[0m", stdout);
		}
	};

#endif //#ifdef _WIN32


// -------------------------------------------------------------------------
#define Q_Error2(stream)	do { \
	std::stringstream _s_stmConsoleBuffer; \
	QConsoleHelper::DoErrorColor(); \
	_s_stmConsoleBuffer << __FUNCTION__ << "@" << __LINE__ << ": " << stream<<": "<<__FILE__ << std::endl; \
	QLogPrintf(LOG_ERR, "%s", _s_stmConsoleBuffer.str().c_str()); \
	QConsoleHelper::RestoreColor(); \
	} while (0)

#define Q_Printl(stream)	do { \
	std::stringstream _s_stmConsoleBuffer; \
	_s_stmConsoleBuffer << stream << std::endl; \
	QLogPrintf(LOG_INFO, "%s %s", _s_stmConsoleBuffer.str().c_str(),__FILE__); \
	} while (0)

#define Q_Error Q_Error2

#define Q_Error3(msg)	do { \
	QConsoleHelper::DoErrorColor(); \
	QLogPrintf(LOG_ERR, "%s@%d: %s %s", __FUNCTION__, __LINE__, msg,__FILE__); \
	QConsoleHelper::RestoreColor(); \
	} while (0)

#define QCONFIRM(condition) if (!(condition)) { Q_Error3(#condition); }
#define QCONFIRM_RET(condition, ret) if (!(condition)) { Q_Error3(#condition); return ret; }
#define QCONFIRM_RET_FALSE(condition) if (!(condition)) { Q_Error3(#condition); return FALSE; }
#define QCONFIRM_RET_NULL(condition) if (!(condition)) { Q_Error3(#condition); return 0; }
#define QCONFIRM_BREAK(condition) if (!(condition)) { Q_Error3(#condition); break; }
#define QCONFIRM_CONTINUE(condition) if (!(condition)) { Q_Error3(#condition); continue; }

// -------------------------------------------------------------------------

#undef LOG_PROCESS_ERROR
#undef LOG_COM_PROCESS_ERROR
#undef LOG_CHECK_ERROR
#undef LOG_COM_CHECK_ERROR

#ifdef WIN32

#define LOG_PROCESS_ERROR(Condition) \
	do {   \
	if (!(Condition))       \
	{                       \
	QConsoleHelper::DoErrorColor(); \
	QLogPrintf(        \
	LOG_ERR,    \
	"LOG_PROCESS_ERROR(%s) %s line %d", #Condition, __FUNCTION__, __LINE__  \
	);                  \
	QConsoleHelper::RestoreColor(); \
	goto EXIT0;         \
	}                       \
	} while (false)

#define LOG_COM_PROCESS_ERROR(Condition) \
	do  \
	{   \
	if (FAILED(Condition))  \
	{                       \
	QConsoleHelper::DoErrorColor(); \
	QLogPrintf(        \
	LOG_ERR,    \
	"LOG_COM_PROCESS_ERROR(0x%X) %s line %d", (Condition), __FUNCTION__, __LINE__  \
	);                  \
	QConsoleHelper::RestoreColor(); \
	goto EXIT0;         \
	}                       \
	} while (false)

#define LOG_CHECK_ERROR(Condition) \
	do  \
	{   \
	if (!(Condition))       \
	{                       \
	QConsoleHelper::DoErrorColor(); \
	QLogPrintf(        \
	LOG_ERR,    \
	"LOG_CHECK_ERROR(%s) %s line %d", #Condition, __FUNCTION__, __LINE__  \
	);                  \
	QConsoleHelper::RestoreColor(); \
	}                       \
	} while (false)

#define LOG_COM_CHECK_ERROR(Condition) \
	do  \
	{   \
	if (FAILED(Condition))       \
	{                       \
	QConsoleHelper::DoErrorColor(); \
	QLogPrintf(        \
	LOG_ERR,    \
	"LOG_COM_CHECK_ERROR(0x%X) %s line %d", (Condition), __FUNCTION__, __LINE__  \
	);                  \
	QConsoleHelper::RestoreColor(); \
	}                       \
	} while (false)

#else //WIN32

#define LOG_PROCESS_ERROR(Condition) \
	do {   \
	if (!(Condition))       \
	{                       \
	QLogPrintf(        \
	LOG_ERR,    \
	"\033[41;37mLOG_PROCESS_ERROR(%s) %s line %d file %s\033[0m", #Condition, __FUNCTION__, __LINE__ ,__FILE__ \
	);                  \
	goto EXIT0;         \
	}                       \
	} while (false)

#define LOG_COM_PROCESS_ERROR(Condition) \
	do  \
	{   \
	if (FAILED(Condition))  \
	{                       \
	QLogPrintf(        \
	LOG_ERR,    \
	"\033[41;37mLOG_COM_PROCESS_ERROR(0x%X) %s line %d file %s\033[0m", (Condition), __FUNCTION__, __LINE__ ,__FILE__\
	);                  \
	goto EXIT0;         \
	}                       \
	} while (false)

#define LOG_CHECK_ERROR(Condition) \
	do  \
	{   \
	if (!(Condition))       \
	{                       \
	QLogPrintf(        \
	LOG_ERR,    \
	"\033[41;37mLOG_CHECK_ERROR(%s) %s line %d file %s\033[0m", #Condition, __FUNCTION__, __LINE__,__FILE__ \
	);                  \
	}                       \
	} while (false)

#define LOG_COM_CHECK_ERROR(Condition) \
	do  \
	{   \
	if (FAILED(Condition))       \
	{                       \
	QLogPrintf(        \
	LOG_ERR,    \
	"\033[41;37mLOG_COM_CHECK_ERROR(0x%X) %s line %d\033[0m", (Condition), __FUNCTION__, __LINE__  \
	);                  \
	}                       \
	} while (false)

#endif // WIN32


