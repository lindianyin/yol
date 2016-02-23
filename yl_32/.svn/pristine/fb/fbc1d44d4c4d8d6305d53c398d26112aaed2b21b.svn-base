// TestRuntimeMemLeakDetect.cpp : Defines the entry poINT for the console application.
//
#if defined WIN32 && defined _DEBUG

#include "stdafx.h"
#include <windows.h>
#include <crtdbg.h>
#include <assert.h>
#include "onlinegamebase/kjxexunify.h"

#define PROCESS_ERROR(Condition) \
    do  \
    {   \
    if (!(Condition))   \
    goto EXIT0;     \
    } while (false)

//#define ASSERT assert

// 注意,这个结构定义是根据VS2005版本的C库出来的,如果是VS2003,也许需要核对一下是不是也是这样
#define nNoMansLandSize 4
typedef struct _CrtMemBlockHeader
{
    struct _CrtMemBlockHeader * pBlockHeaderNext;
    struct _CrtMemBlockHeader * pBlockHeaderPrev;
    CHAR *                      szFileName;
    INT                         nLine;
#ifdef _WIN64
    /* These items are reversed on Win64 to eliminate gaps in the struct
    * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
    * maINTained in the debug heap.
    */
    INT                         nBlockUse;
    size_t                      nDataSize;
#else  /* _WIN64 */
    size_t                      nDataSize;
    INT                         nBlockUse;
#endif  /* _WIN64 */
    LONG                        lRequest;
    unsigned char               gap[nNoMansLandSize];
    /* followed by:
    *  unsigned CHAR           data[nDataSize];
    *  unsigned CHAR           anotherGap[nNoMansLandSize];
    */
} _CrtMemBlockHeader;

#define         KALLOC_RECORD_COUNT     (1024 * 16)
static LONG     g_lAllocCounter[KALLOC_RECORD_COUNT];
static LONG     g_lAllocCounterBackBuffer[KALLOC_RECORD_COUNT];
static DWORD    g_dwLastMemoryCheckTickCount = 0;
static LONG     g_lTotalAlloc = 0;

static int MyAllocHook(int nAllocType, void* pvData, size_t nSize, int nBlockUse, long lRequest, const unsigned char * szFileName, int nLine)
{
    size_t              uAllocSize      = 0;
    size_t              uFreeSize       = 0;
    _CrtMemBlockHeader* pHeader         = NULL;
    INT                 nRecordIndex    = 0;

    switch (nAllocType)
    {
    case _HOOK_ALLOC:

        uAllocSize = sizeof(_CrtMemBlockHeader) + nSize + 4;
        InterlockedExchangeAdd(&g_lTotalAlloc, (LONG)uAllocSize);
        nRecordIndex = (INT)nSize;
        PROCESS_ERROR(nRecordIndex < KALLOC_RECORD_COUNT);
        InterlockedIncrement(g_lAllocCounter + nRecordIndex);

        break;

    case _HOOK_REALLOC:

        if (pvData)
        {
            pHeader = (_CrtMemBlockHeader*)pvData - 1;

            if (pHeader->nDataSize > 0)
            {
                uFreeSize = sizeof(_CrtMemBlockHeader) + pHeader->nDataSize + 4;
                InterlockedExchangeAdd(&g_lTotalAlloc, -(LONG)uFreeSize);
            }

            nRecordIndex = (INT)pHeader->nDataSize;
            if (nRecordIndex < KALLOC_RECORD_COUNT)
            {
                InterlockedDecrement(g_lAllocCounter + nRecordIndex);
            }
        }

        if (nSize > 0)
        {
            uAllocSize = sizeof(_CrtMemBlockHeader) + nSize + 4;
            InterlockedExchangeAdd(&g_lTotalAlloc, (LONG)uAllocSize);
        }

        nRecordIndex = (INT)nSize;
        if (nRecordIndex < KALLOC_RECORD_COUNT)
        {
            InterlockedIncrement(g_lAllocCounter + nRecordIndex);
        }

        break;

    case _HOOK_FREE:

        PROCESS_ERROR(pvData);
        pHeader = (_CrtMemBlockHeader*)pvData - 1;
        uFreeSize = sizeof(_CrtMemBlockHeader) + pHeader->nDataSize + 4;
        InterlockedExchangeAdd(&g_lTotalAlloc, -(LONG)uFreeSize);
        nRecordIndex = (INT)pHeader->nDataSize;
        PROCESS_ERROR(nRecordIndex < KALLOC_RECORD_COUNT);
        InterlockedDecrement(g_lAllocCounter + nRecordIndex);

        break;

    default:
        ASSERT(!"Unexpected memory operate !");
    }

EXIT0:
    return TRUE;
}

static BOOL WriteLog(LPCSTR szOutput, UINT uSize)
{
#if (defined(_MSC_VER) || defined(__ICL))
#define _KGLOG_OPENFILEFLAG_   (O_CREAT | O_APPEND | O_WRONLY)
#define _KGLOG_OPENFILEMODE_   (S_IREAD | S_IWRITE)
#else
#define _KGLOG_OPENFILEFLAG_   (O_CREAT | O_APPEND | O_WRONLY)
#define _KGLOG_OPENFILEMODE_   (S_IREAD | S_IWRITE |  S_IRGRP | S_IROTH)
#endif
	static INT s_nFileHandle = -1;
	if (s_nFileHandle == -1)
	{
		s_nFileHandle = open("memleakdected_log.txt", _KGLOG_OPENFILEFLAG_, _KGLOG_OPENFILEMODE_);
	}
	if (szOutput && s_nFileHandle >= 0)
	{
		write(s_nFileHandle, szOutput, uSize);
		return TRUE;
	}
	return FALSE;
}

// 在主循环中,以适当的频率调用下面的检测函数:
// 一两秒调一次差不多	
// 检测函数可以根据需要改改
VOID g_RunTimeMemLeakDetect(UINT uInterval)
{
    DWORD dwTickCount = GetTickCount();
    if (dwTickCount > g_dwLastMemoryCheckTickCount + uInterval)
    {        
        LONG lAllocCounterBackBuffer[KALLOC_RECORD_COUNT];

        for (INT i = 0; i < KALLOC_RECORD_COUNT; ++i)
            lAllocCounterBackBuffer[i] = g_lAllocCounter[i] - g_lAllocCounterBackBuffer[i];

        memcpy(g_lAllocCounterBackBuffer, g_lAllocCounter, sizeof(g_lAllocCounterBackBuffer));
		
		static CHAR szBuf[1024 * 1024];
		LPSTR pszWritePos = szBuf;
        pszWritePos += sprintf(pszWritePos, "------------------ Total %d M ------------------------\n", g_lTotalAlloc / 1024 / 1024);
		//WriteLog(szBuf);

        for (INT i = 0; i < KALLOC_RECORD_COUNT; i++)
        {
            INT nDeltaBlock = lAllocCounterBackBuffer[i];

            if (nDeltaBlock > 4)
            {
				pszWritePos += sprintf(pszWritePos, "size(%d) --> Inc:%d\tTotal:%d\n", i, nDeltaBlock, g_lAllocCounter[i]);
				//WriteLog(szBuf);
            }
        }

        *pszWritePos++ = '\n';
		*pszWritePos = '\0';
		WriteLog(szBuf, pszWritePos - szBuf);

        g_dwLastMemoryCheckTickCount = dwTickCount;
    }
}

INT g_RunTimeMemLeakDetectInit()
{
    // 在初始化的时候设置Hook:
    memset(g_lAllocCounter, 0, sizeof(g_lAllocCounter));
    memset(g_lAllocCounterBackBuffer, 0, sizeof(g_lAllocCounterBackBuffer));
	g_dwLastMemoryCheckTickCount = GetTickCount();
    _CrtSetAllocHook(MyAllocHook);

	return 0;
}

#endif
