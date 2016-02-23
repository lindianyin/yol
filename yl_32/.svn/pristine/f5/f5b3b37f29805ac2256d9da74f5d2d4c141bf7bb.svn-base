////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGD_Connect.cpp
//  Version     : 1.0
//  Creater     : Wu Caizhong
//  Create Date : 2005-2-20 0:19:01
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KGD_Error.h"
#include "Misc/IDatabase.h"
#include "KGD_MySQL_Database.h"

static long gs_lInitRefCout = 0;  // reference count for init time

////////////////////////////////////////////////////////////////////////////////
int KGD_Init(void *pvContext)
{
    int nResult  = false;
    int nRetCode = false;
    int nKGDMySQLInitFlag = false;
    static QLock s_InitLock;

    nRetCode = s_InitLock.Lock();

    USE_ARGUMENT(pvContext); 

    if (gs_lInitRefCout == 0)
    {
        nRetCode = KGD_MySQL_Init(pvContext);
        LOG_PROCESS_ERROR(nRetCode);
        nKGDMySQLInitFlag = true;
    }

//EXIT1:
    ++gs_lInitRefCout;
    nResult = true;
EXIT0:
    nRetCode = s_InitLock.Unlock();
    if (!nResult)
    {
        if (nKGDMySQLInitFlag)
        {
            KGD_MySQL_Uninit(pvContext);
            nKGDMySQLInitFlag = false;
        }
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_Uninit(void *pvContext)
{
    int nResult  = false;
    long lNewCount = 0;

    USE_ARGUMENT(pvContext); 

    lNewCount = InterlockedDecrement(&gs_lInitRefCout);
   
    if (lNewCount < 0)
    {
        ASSERT(!"Unexpected call of KGD_Uninit()");
        QLogPrintf(LOG_DEBUG, "Unexpected call of KGD_Uninit()");
        PROCESS_ERROR(false);
    }
    else if (lNewCount == 0)
    {
        KGD_MySQL_Uninit(pvContext);
    }
    
//EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
// @brief : Connect to database server
// @remark : interface function, see more information in KGD_Interface.h
IKGD_Database *KGD_Connect(
    KGD_ENGINE_TYPE EngineType,
    const char cszServerName[], 
    unsigned short usPort, 
    const char cszUserName[], 
    const char cszPassword[],
    const char *pcszCurrentDatabase
)
{
    int nResult  = false;
    int nRetCode = false;
    IKGD_Database *piDatabase = NULL;

    LOG_PROCESS_ERROR(gs_lInitRefCout > 0);

    LOG_PROCESS_ERROR(cszServerName);
    LOG_PROCESS_ERROR(cszUserName);
    LOG_PROCESS_ERROR(cszPassword);

    switch (EngineType)
    {
    case KGD_ENGINE_DEFAULT:
        piDatabase = new KGD_MySQL_Database;
        LOG_PROCESS_ERROR(piDatabase);
        nRetCode = ((KGD_MySQL_Database *)piDatabase)->Connect(
             cszServerName, usPort, cszUserName, cszPassword, pcszCurrentDatabase
        );
        PROCESS_ERROR(nRetCode);
        break;

    default:
        ASSERT(!"Unknown database engine type.");
        LOG_PROCESS_ERROR(false);
    }
//EXIT1:
    nResult = true;
EXIT0:
    if (!nResult)
    {
        SAFE_RELEASE(piDatabase);
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] KGD_Connect(%u, \"%s\", %d, \"%s\", 0x%p(\"***\"), \"%s\")",
            EngineType, cszServerName, usPort, 
            cszUserName, cszPassword, pcszCurrentDatabase 
        );
    }
    return piDatabase;
}
