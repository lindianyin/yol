
#pragma once

#include "mysql.h"
#include "errmsg.h"

////////////////////////////////////////////////////////////////////////////////
#define MYSQL_PROCESS_ERROR(Condition, pMySQL)                           \
do                                                                          \
{                                                                           \
    if (!(Condition))                                                       \
    {                                                                       \
        QLogPrintf(                                                        \
            LOG_ERR,                                                      \
            "[MySQL error] line %d, function %s(): \"(%d) %s\"",	        \
            __LINE__, __FUNCTION__, mysql_errno(pMySQL), mysql_error(pMySQL) \
        );                                                                  \
        goto EXIT0;                                                         \
    }                                                                       \
} while (false)

////////////////////////////////////////////////////////////////////////////////
const unsigned KG_DATABASE_STRING_MAX_SIZE = 64;

const unsigned KG_DATABASE_PORT            = 3306;

struct KG_DATABASE_PARAM 
{
    char szServer[KG_DATABASE_STRING_MAX_SIZE];
    char szUsername[KG_DATABASE_STRING_MAX_SIZE];
    char szPassword[KG_DATABASE_STRING_MAX_SIZE];
    char szDatabase[KG_DATABASE_STRING_MAX_SIZE];
};

const unsigned KG_NAME_MAX_SIZE = 32;

////////////////////////////////////////////////////////////////////////////////
int DoMysqlQuery(MYSQL *pConn, const char cszQuery[], unsigned long ulQueryLen);

int GetServerVariable(
    MYSQL *pConn, 
    const char cszVariablesName[], 
    unsigned uRetBufferSize, char *pszRetBuffer
);

int CheckDataBaseVariable(MYSQL *pConn);

