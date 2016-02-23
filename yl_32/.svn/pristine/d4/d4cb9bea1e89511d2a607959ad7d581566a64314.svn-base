////////////////////////////////////////////////////////////////////////////////
//  Copyright(c) Kingsoft
//
//	FileName    :   KG_Database.cpp
//	Creator     :   Liang Bo
//	Date        :   2008-5-19
//	Comment     :   
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "kg_database.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
int DoMysqlQuery(MYSQL *pConn, const char cszQuery[])
{
    int nResult  = false;
    int nRetCode = false;
    int nReconnectTimes = 0;
    unsigned uMySQLErrorCode = 0;

    LOG_PROCESS_ERROR(pConn);
    LOG_PROCESS_ERROR(cszQuery);

    while (true) 
    {
        nRetCode = mysql_query(pConn, cszQuery);
        if (nRetCode == 0)
            break; // success
        uMySQLErrorCode = mysql_errno(pConn);
        if (
            (uMySQLErrorCode != CR_SERVER_LOST) &&
            (uMySQLErrorCode != CR_CONN_HOST_ERROR) &&
            (uMySQLErrorCode != CR_COMMANDS_OUT_OF_SYNC) &&
            (uMySQLErrorCode != CR_SERVER_GONE_ERROR)
        )
        {
            break; // false
        }

        QLogPrintf(
            LOG_INFO, 
            "%s : Connection lost, try reconnect.",
            __FUNCTION__
        );
        // Checks whether the connection to the server is working. 
        // If the connection has gone down, an automatic reconnection is attempted
        while (nReconnectTimes <= 3)
        { 
            nRetCode = mysql_ping(pConn);
            if (nRetCode == 0)
                break;
            ++nReconnectTimes;
        }
        KG_PROCESS_MYSQL_ERROR((nRetCode == 0), pConn);
    }
    KG_PROCESS_MYSQL_ERROR((nRetCode == 0), pConn);

    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int GetServerVariable(
    MYSQL *pConn, 
    const char cszVariablesName[], 
    unsigned uRetBufferSize, char *pszRetBuffer
)
{
    int nResult  = false;
    int nRetCode = false;
    char szQuery[256];
    MYSQL_RES *pResultSet = NULL;
    MYSQL_ROW row = NULL;
    unsigned uFieldCount = 0;

    LOG_PROCESS_ERROR(pConn);
    LOG_PROCESS_ERROR(cszVariablesName);
    LOG_PROCESS_ERROR(uRetBufferSize > 1);
    LOG_PROCESS_ERROR(pszRetBuffer);

    nRetCode = snprintf(
        szQuery, sizeof(szQuery) - 1, "SHOW VARIABLES LIKE \'%s\'", cszVariablesName
    );
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szQuery) - 1));
    szQuery[sizeof(szQuery) - 1] = '\0';

    nRetCode = DoMysqlQuery(pConn, szQuery);
    PROCESS_ERROR(nRetCode);

    pResultSet = mysql_store_result(pConn);
    KG_PROCESS_MYSQL_ERROR(pResultSet, pConn);

    uFieldCount = mysql_num_fields(pResultSet);
    KG_PROCESS_MYSQL_ERROR((uFieldCount == 2), pConn);

    row = mysql_fetch_row(pResultSet);
    KG_PROCESS_MYSQL_ERROR(row, pConn);

    ASSERT(row[1]);

    QStrCpyLen(pszRetBuffer, row[1], uRetBufferSize);

    nResult = true;
EXIT0:
    if (pResultSet)
    {
        mysql_free_result(pResultSet);
        pResultSet = NULL;
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int CheckDataBaseVariable(MYSQL *pConn)
{
    int nResult  = false;
    int nRetCode = false;
    static const char s_cszLowerCaseTableNames[]  = "lower_case_table_names";
    static const char s_cszCharacterSetServer[]   = "character_set_server";
    static const char s_cszCharacterSetDatabase[] = "character_set_database";
    char szCharacterSetServer[64];
    char szCharacterSetDatabase[64];
    char szLowerCaseTableNames[4];

    LOG_PROCESS_ERROR(pConn);

    // --------- check MySQL Identifier Case Sensitivity setting ---------------
    nRetCode = GetServerVariable(
        pConn,
        s_cszLowerCaseTableNames, 
        sizeof(szLowerCaseTableNames), szLowerCaseTableNames
    );
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = strcmp(szLowerCaseTableNames, "1");
    LOG_PROCESS_ERROR(
        (nRetCode == 0) && 
        "MySQL Server is not config to case-insensitive,"
        "Please set MySQL variables: lower_case_table_names=1"
    );

    // ---------- check database character set and server character set -------- 
    nRetCode = GetServerVariable(
        pConn,
        s_cszCharacterSetServer, 
        sizeof(szCharacterSetServer), szCharacterSetServer
    );
    PROCESS_ERROR(nRetCode);

    nRetCode = GetServerVariable(
        pConn,
        s_cszCharacterSetDatabase, 
        sizeof(szCharacterSetDatabase), szCharacterSetDatabase
    );
    PROCESS_ERROR(nRetCode);

    nRetCode = strcmp(szCharacterSetDatabase, szCharacterSetServer);
    if (nRetCode != 0)
    {
        QLogPrintf(
            LOG_DEBUG, 
            "[Database] MySQL Server variables: character_set_database = %s, "
            "character_set_server = %s",
            szCharacterSetDatabase, szCharacterSetServer
        );
        PROCESS_ERROR(false);
    }

    // ----------------------- set client character set  ---------------------
    nRetCode = mysql_set_character_set(pConn, szCharacterSetServer);
    KG_PROCESS_MYSQL_ERROR(nRetCode == 0, pConn);

    QLogPrintf(
        LOG_DEBUG, 
        "[Database] character set switch to %s",
        szCharacterSetServer
    );

    nResult = true;
EXIT0:
    return nResult;
}
