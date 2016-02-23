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
#include "KG_Database.h"

////////////////////////////////////////////////////////////////////////////////
int DoMysqlQuery(MYSQL *pConn, const char cszQuery[], unsigned long ulQueryLen)
{
    int nResult  = false;
    int nRetCode = false;
    int nReconnectTimes = 0;
    unsigned uMySQLErrorCode = 0;

    LOG_PROCESS_ERROR(pConn);
    LOG_PROCESS_ERROR(cszQuery);
    LOG_PROCESS_ERROR(ulQueryLen > 0);

    while (true) 
    {
        nRetCode = mysql_real_query(pConn, cszQuery, ulQueryLen);
        if (nRetCode == 0)
            break; // success
        uMySQLErrorCode = mysql_errno(pConn);
        if (
            (uMySQLErrorCode != CR_SERVER_LOST)          &&
            (uMySQLErrorCode != CR_CONN_HOST_ERROR)      &&
            (uMySQLErrorCode != CR_COMMANDS_OUT_OF_SYNC) &&
            (uMySQLErrorCode != CR_SERVER_GONE_ERROR)
        )
        {
            break; // false
        }

        QLogPrintf(LOG_INFO, "%s : Connection lost, try reconnect.", __FUNCTION__);
        // Checks whether the connection to the server is working. 
        // If the connection has gone down, an automatic reconnection is attempted
        while (nReconnectTimes <= 3)
        { 
            nRetCode = mysql_ping(pConn);
            if (nRetCode == 0)
                break;
            ++nReconnectTimes;
        }
        MYSQL_PROCESS_ERROR((nRetCode == 0), pConn);
    }
    MYSQL_PROCESS_ERROR((nRetCode == 0), pConn);

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
    char szSql[256];
    unsigned long ulSqlLen = 0;
    MYSQL_RES *pResultSet = NULL;
    MYSQL_ROW row = NULL;
    unsigned uFieldCount = 0;

    LOG_PROCESS_ERROR(pConn);
    LOG_PROCESS_ERROR(cszVariablesName);
    LOG_PROCESS_ERROR(uRetBufferSize > 1);
    LOG_PROCESS_ERROR(pszRetBuffer);

    nRetCode = snprintf(
        szSql, sizeof(szSql) - 1, "SHOW VARIABLES LIKE \'%s\'", cszVariablesName
    );
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szSql) - 1));
    szSql[sizeof(szSql) - 1] = '\0';
    ulSqlLen = (unsigned long)nRetCode;

    nRetCode = DoMysqlQuery(pConn, szSql, ulSqlLen);
    PROCESS_ERROR(nRetCode);

    pResultSet = mysql_store_result(pConn);
    MYSQL_PROCESS_ERROR(pResultSet, pConn);

    uFieldCount = mysql_num_fields(pResultSet);
    MYSQL_PROCESS_ERROR((uFieldCount == 2), pConn);

    row = mysql_fetch_row(pResultSet);
    MYSQL_PROCESS_ERROR(row, pConn);

    ASSERT(row[1]);

    strncpy(pszRetBuffer, row[1], uRetBufferSize - 1);
    pszRetBuffer[uRetBufferSize - 1]  = '\0';

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
            LOG_ERR, 
            "[Database] MySQL Server variables: character_set_database = %s, "
            "character_set_server = %s",
            szCharacterSetDatabase, szCharacterSetServer
        );
        PROCESS_ERROR(false);
    }

    // ----------------------- set client character set  ---------------------
    nRetCode = mysql_set_character_set(pConn, szCharacterSetServer);
    MYSQL_PROCESS_ERROR(nRetCode == 0, pConn);

    QLogPrintf(
        LOG_INFO, 
        "[Database] character set switch to %s",
        szCharacterSetServer
    );

    nResult = true;
EXIT0:
    return nResult;
}
