
#include "stdafx.h"
#include "DBBase.h"
#include "errmsg.h"

static const unsigned long MYSQL5_VERSION_ID = 50000; // MySQL 5 Version ID

BOOL MySQL_CheckCaseInsensitive(MYSQL* pHandle)
{
    BOOL        bResult         = false;
    int         nRetCode        = 0;
    char        szSQL[]         = "show variables like \'lower_case_table_names\'";
    MYSQL_RES*  pResultSet      = NULL;
    MYSQL_ROW   Row             = NULL;
    unsigned    uFieldCount     = 0;
    
    assert(pHandle);

    nRetCode = MySQL_Query(pHandle, szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pResultSet = mysql_store_result(pHandle);
    LOG_PROCESS_ERROR(pResultSet);

    uFieldCount = mysql_num_fields(pResultSet);    
    LOG_PROCESS_ERROR(uFieldCount == 2);

    Row = mysql_fetch_row(pResultSet);
    LOG_PROCESS_ERROR(Row);

    nRetCode = strcmp(Row[1], "1");
    if (nRetCode)
    {
        QLogPrintf(LOG_ERR, "[DB] DB error, lower_case_table_names --> 1\n");
        goto EXIT0;
    }

    bResult = true;
EXIT0:
    if (pResultSet)
    {
        mysql_free_result(pResultSet);
        pResultSet = NULL;
    }
    return bResult;
}

BOOL MySQL_CheckCharacterSet(MYSQL* pHandle)
{
    BOOL        bResult         = false;
    int         nRetCode        = 0;
    char        szSQL[]         = "show variables like \'character_set_database\'";
    MYSQL_RES*  pResultSet      = NULL;
    MYSQL_ROW   Row             = NULL;
    unsigned    uFieldCount     = 0;

    assert(pHandle);
    
    // get database character set
    nRetCode = MySQL_Query(pHandle, szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    pResultSet = mysql_store_result(pHandle);
    LOG_PROCESS_ERROR(pResultSet);

    uFieldCount = mysql_num_fields(pResultSet);    
    LOG_PROCESS_ERROR(uFieldCount == 2);

    Row = mysql_fetch_row(pResultSet);
    LOG_PROCESS_ERROR(Row);

    LOG_PROCESS_ERROR(Row[1]);

    nRetCode = mysql_set_character_set(pHandle, Row[1]);    
    LOG_PROCESS_ERROR(nRetCode == 0);

    bResult = true;
EXIT0:
    if (pResultSet)
    {
        mysql_free_result(pResultSet);
        pResultSet = NULL;
    }
    return bResult;
}

MYSQL* MySQL_Connect(
    const char cszDBAddr[], const char cszDBName[], const char cszUserName[], const char cszPassword[]
)
{
    MYSQL*        pResult                   = NULL;
    int           nRetCode                  = false;
    MYSQL*        pDBHandle                 = NULL;
    MYSQL*        pTmpHandle                = NULL;
    my_bool       bReconnectFlag            = 0;
    unsigned long ulMySQLClientVersionID    = 0;
    int           nStrLen                   = 0;
    char          szSQL[1024];

    pDBHandle = mysql_init(NULL);
    LOG_PROCESS_ERROR(pDBHandle);
    
    // enable reconnect if MySQL client version > 5
    ulMySQLClientVersionID = mysql_get_client_version();
    if (ulMySQLClientVersionID >= MYSQL5_VERSION_ID)
    {
        bReconnectFlag = 1;
        nRetCode = mysql_options(pDBHandle, MYSQL_OPT_RECONNECT, &bReconnectFlag);
        LOG_PROCESS_ERROR(nRetCode == 0);
    }

    pTmpHandle = mysql_real_connect(
        pDBHandle, cszDBAddr, cszUserName, cszPassword, "", 3306, NULL, 0
    );
    if (pTmpHandle == NULL)
    {
        QLogPrintf(
            LOG_ERR, 
            "[DB] Can't open database \"%s\" at \"%s\", db error : \"%s\"\n", 
            cszDBName, cszDBAddr, mysql_error(pDBHandle)
        );
        goto EXIT0;
    }
    
    nStrLen = snprintf(
        szSQL, sizeof(szSQL), "create database if not exists %s", cszDBName
    );
    LOG_PROCESS_ERROR(nStrLen > 0 && nStrLen < (int)sizeof(szSQL));

    nRetCode = MySQL_Query(pDBHandle, szSQL);
	QLogPrintf(LOG_DEBUG,"%s",szSQL);
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = mysql_select_db(pDBHandle, cszDBName);
    LOG_PROCESS_ERROR(nRetCode == 0);

    nRetCode = MySQL_CheckCaseInsensitive(pDBHandle);
    LOG_PROCESS_ERROR(nRetCode);

    nRetCode = MySQL_CheckCharacterSet(pDBHandle);
    LOG_PROCESS_ERROR(nRetCode);

    pResult = pDBHandle;
EXIT0:
    if (!pResult)
    {
        if (pDBHandle)
        {
            QLogPrintf(LOG_ERR, "%d:%s\n", mysql_errno(pDBHandle), mysql_error(pDBHandle));

            mysql_close(pDBHandle);
            pDBHandle = NULL;
        }
    }
    return pResult;
}

void MySQL_Disconnect(MYSQL* pHandle)
{
    if (pHandle)
    {
    	ASSERT(false);
        mysql_close(pHandle);
        pHandle = NULL;
    }
}

BOOL MySQL_Query(MYSQL* pHandle, const char cszSQL[], unsigned* puRetMySQLErrorCode /*= NULL*/)
{
    BOOL        bResult             = false;
    int         nRetCode            = false;
    int         nReconnectTimes     = 0;
    unsigned    uMySQLErrorCode     = 0;
    size_t      uStrLen             = strlen(cszSQL);
	BOOL        bReconnect          = false;

    assert(pHandle);
    assert(cszSQL);
    
	//QLogPrintf(LOG_INFO, "[DB] mysql_real_query SQL statement: %s \n", cszSQL);

    while (true) 
    {
        nRetCode = mysql_real_query(pHandle, cszSQL, (unsigned long)uStrLen);
        if (nRetCode == 0)
            break;

        uMySQLErrorCode = mysql_errno(pHandle);
        PROCESS_ERROR(uMySQLErrorCode != ER_DUP_ENTRY);

		QLogPrintf(
			LOG_ERR,
			"[DB] mysql_real_query failed : %d:%s, line %d at %s\n",
			uMySQLErrorCode,
			mysql_error(pHandle),
			__LINE__, __FUNCTION__
			);

        if (
            (uMySQLErrorCode != CR_SERVER_LOST) &&
            (uMySQLErrorCode != CR_CONN_HOST_ERROR) &&
            (uMySQLErrorCode != CR_SERVER_GONE_ERROR)
        )
        {

            goto EXIT0;
        }
		//尝试重新连接
        while (nReconnectTimes < 50)
        { 

            nRetCode = mysql_ping(pHandle);
            if (nRetCode == 0)
            {
            	bReconnect = TRUE;
				break;
			}
			int error_code = mysql_errno(pHandle);
			QLogPrintf(
				LOG_ERR,
				"[DB] mysql_ping failed : %d:%s, line %d at %s\n",
				error_code,
				mysql_error(pHandle),
				__LINE__, __FUNCTION__
				);

            QThread_Sleep(500);
            ++nReconnectTimes;
        }
		
		if(!bReconnect)
		{
			assert(false);
		}
        //if (nReconnectTimes >= 5)
        //{
        //    QLogPrintf(                                                        
        //        LOG_ERR,                                                    
        //        "[DB] Call \"mysql_ping\" failed : %s\n",
        //        mysql_error(pHandle)
        //   );
        //    
        //    goto EXIT0;
        //}
    }
	
    bResult = true;
EXIT0:
    if (puRetMySQLErrorCode)
    {
        *puRetMySQLErrorCode = uMySQLErrorCode;
    }
    return bResult; 
}
