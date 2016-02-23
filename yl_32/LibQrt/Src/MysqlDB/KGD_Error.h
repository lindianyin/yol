
#pragma once

#include "Misc/IDatabase.h"

////////////////////////////////////////////////////////////////////////////////
// class member function use it check result. 
// it will output error message and set m_LastErrorCode when error.
// !!note: so do not use it check a member function's return,
//         that will cause twice output, and lost last m_LastErrorCode set.
//         use PROCESS_ERROR to check member function's return.
#define QDB_PROCESS_ERROR(Condition, KGDErrorCode)                          \
do                                                                          \
{                                                                           \
    if (!(Condition))                                                       \
    {                                                                       \
        m_LastErrorCode = KGDErrorCode;                                     \
        QLogPrintf(												        \
            LOG_DEBUG,                                                    \
            "[KGD] QDB_PROCESS_ERROR(%s, "#KGDErrorCode"), "                \
            "at line %d in (0x%p)->%s()",						            \
             #Condition, __LINE__, this, __FUNCTION__                        \
        );                                                                  \
        goto EXIT0;                                                         \
    }                                                                       \
} while (false)
////////////////////////////////////////////////////////////////////////////////
// class member function use it check result. 
// it will output error message and set m_LastErrorCode when error.
// !!note: so do not use it check a member function's return,
//         that will cause twice output, and lost last m_LastErrorCode set.
#define KGD_MYSQL_ENGINE_PROCESS_ERROR(Condition, pMySQL, pszMsg)           \
do                                                                          \
{                                                                           \
    if (!(Condition))                                                       \
    {                                                                       \
        m_LastErrorCode = KGD_ERROR_DATABASE_ENGINE_ERROR;                  \
        QLogPrintf(                                                        \
            LOG_DEBUG,                                                    \
            "[KGD] KGD_ERROR_DATABASE_ENGINE_ERROR at line %d "             \
            "in (0x%p)->%s(), \"%s\", \"%s\"",								\
            __LINE__, this, __FUNCTION__, mysql_error(pMySQL), (pszMsg)      \
        );                                                                  \
        goto EXIT0;                                                         \
    }                                                                       \
} while (false)

////////////////////////////////////////////////////////////////////////////////
// @brief : Dump memory to buffer as null terminal string
//           format "(0x001745D,0x04)={4d 33 6f 77}"
// @param uBufSize     : [in] Buffer size, above sizeof("(0x00000000, 0x0000)={...}" 
// @param pszWritePos  : [in] Buffer to store dump string. must valid
// @param uDumpSize    : [in] bytes of memory want to dump
// @param pcvBeDumpMem : [in] Point to memory want to dump, can be NULL
// @return : The number of bytes stored in buffer, 
//           not counting the terminating null character
int KGD_DumpMemory(
    unsigned uBufSize, 
    char *pszWritePos,  
    unsigned uDumpSize, 
    const void *pcvBeDumpMem
);

////////////////////////////////////////////////////////////////////////////////
// @brief : Dump KGD_FIELD struct to buffer as null terminal string
// @param uBufSize : 
// @param pszBuf   : [in] must valid
// @param uCount   : [in] element count in array aField
// @param aField   : [in] array to dump, can be NULL
// @param pField1  : [in] field1 to be dump, can be NULL
// @param pField2  : [in] field2 to be dump, can be NULL
// @return : The number of bytes stored in buffer, 
//           not counting the terminating null character
int KGD_DumpKGDFields(
    unsigned uBufSize, 
    char *pszBuf,
    unsigned uCount,
    const KGD_FIELD aField[],
    const KGD_FIELD *pField1,
    const KGD_FIELD *pField2
);
