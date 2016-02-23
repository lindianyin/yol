////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGD_Error.cpp
//  Version     : 1.0
//  Creater     : Wu Caizhong
//  Create Date : 2005-3-24 21:10:15
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KGD_Error.h"

////////////////////////////////////////////////////////////////////////////////
int KGD_DumpMemory(
    unsigned uBufSize, 
    char *pszWritePos,  
    unsigned uDumpSize, 
    const void *pcvBeDumpMem
)
{
    int nRetCode = 0;
    unsigned i = 0;
    unsigned uFreeBufSize = uBufSize;
    char *pszStartPos = pszWritePos;
    unsigned uRealDumpSize = 0;
    unsigned uCanDumpSize  = 0;
    const unsigned char *pcbyData = NULL;

    ASSERT(pszWritePos);
    PROCESS_ERROR(uFreeBufSize > sizeof("(0x00000000, 0x0000)={...}"));
    
    // store address and size
    if (!(pcvBeDumpMem) || (uDumpSize == 0))
    {
        nRetCode = sprintf(pszWritePos, "(0x%p,0x%u)={}", pcvBeDumpMem, uDumpSize);
        ASSERT(nRetCode != -1);
        pszWritePos += nRetCode;
        PROCESS_SUCCESS(true);
    }

    nRetCode = sprintf(pszWritePos, "(0x%p,0x%.2u)={", pcvBeDumpMem, uDumpSize);
    ASSERT(nRetCode != -1);
    pszWritePos += nRetCode;
    uFreeBufSize -= nRetCode;

    // store data, assert (uFreeBufSize >= sizeof("...}")
    uCanDumpSize = (uFreeBufSize - 2) / 3;   // assert uCanDumpSizen > 0
    uRealDumpSize = (uCanDumpSize < uDumpSize) ? (uCanDumpSize - 1) : uDumpSize;
    pcbyData = (const unsigned char *)pcvBeDumpMem;
    for (i = 0; i < uRealDumpSize; ++i)
    {
        nRetCode = sprintf(pszWritePos, "%.2x ", (*pcbyData));
        ASSERT(nRetCode != -1);
        pszWritePos += nRetCode;
        
        ++pcbyData;
    }

    if (uRealDumpSize < uDumpSize)
        nRetCode = sprintf(pszWritePos, "...}");
    else
        nRetCode = sprintf(pszWritePos, "}");
    ASSERT(nRetCode != -1);
    pszWritePos += nRetCode;

EXIT1: 
EXIT0:
    if (uBufSize > 0)
        *pszWritePos = '\0';
    return (int)(pszWritePos - pszStartPos);
}

////////////////////////////////////////////////////////////////////////////////
int KGD_DumpKGDFields(
    unsigned uBufSize, 
    char *pszBuf,
    unsigned uCount,
    const KGD_FIELD aField[],
    const KGD_FIELD *pField1,
    const KGD_FIELD *pField2
)
{
    int nResult  = false;
    int nRetCode = false;
    char *pszWritePos = pszBuf;

    ASSERT(pszWritePos);
    if (aField)
    {
        nRetCode = KGD_DumpMemory(
            uBufSize, pszWritePos, sizeof(KGD_FIELD) * uCount, aField
        );
        PROCESS_ERROR(nRetCode > 0);

        pszWritePos += nRetCode;
        uBufSize -= nRetCode;
    }

    if (pField1)
    {
        nRetCode = KGD_DumpMemory(
            uBufSize, pszWritePos, sizeof(KGD_FIELD), pField1
        );
        PROCESS_ERROR(nRetCode > 0);
        pszWritePos += nRetCode;
        uBufSize -= nRetCode;
    }

    if (pField2)
    {
        nRetCode = KGD_DumpMemory(
            uBufSize, pszWritePos, sizeof(KGD_FIELD), pField2
        );
        PROCESS_ERROR(nRetCode > 0);
        pszWritePos += nRetCode;
        uBufSize -= nRetCode;
    }
//EXIT1:
    nResult = true;
EXIT0:
    if (uBufSize > 0)
        *pszWritePos = '\0';
    return (int)(pszWritePos - pszBuf);
}
