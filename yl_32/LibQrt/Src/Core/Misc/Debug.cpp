
#include "CorePrivate.h"
#include <stdarg.h>

//---------------------------------------------------------------------------
// 函数:	g_DebugLog
// 功能:	向调试窗口输出调试信息
// 参数:	Fmt		格式化字符串
//			...		输出的字符串
//---------------------------------------------------------------------------
void g_DebugLog(const char* Fmt, ...)
{
	#ifdef _DEBUG
		char buffer[1024];
		va_list va;
		va_start(va, Fmt);
		vsprintf(buffer, Fmt, va);
		va_end(va);
		strcat(buffer, "\n");
#ifdef WIN32
		OutputDebugStringA(buffer);
#else
		printf("%s", buffer);
#endif
	#endif
}
