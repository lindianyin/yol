
#pragma once

#include "Misc/IDatabase.h"
#include "mysql/mysql.h"
#include "mysql/errmsg.h"

////////////////////////////////////////////////////////////////////////////////
#define KG_PROCESS_MYSQL_ERROR(Condition, pMySQL)           \
do                                                          \
{                                                           \
    if (!(Condition))                                       \
    {                                                       \
        QLogPrintf(                                        \
            LOG_DEBUG,                                    \
            "[MySQL error] line %d, function %s(): \"%s\"",	\
            __LINE__, __FUNCTION__, mysql_error(pMySQL)      \
        );                                                  \
        goto EXIT0;                                         \
    }                                                       \
} while (false)

////////////////////////////////////////////////////////////////////////////////
const unsigned KG_DATABASE_STRING_MAX_SIZE = 64;

const unsigned KG_DATABASE_PORT            = 3306;

struct KROLE_DB_PARAM 
{
    char szServer[KG_DATABASE_STRING_MAX_SIZE];
    char szUserName[KG_DATABASE_STRING_MAX_SIZE];
    char szPassword[KG_DATABASE_STRING_MAX_SIZE];
    char szDatabase[KG_DATABASE_STRING_MAX_SIZE];
};

const unsigned KG_NAME_MAX_SIZE    = 32;
const unsigned KG_TIME_STRING_SIZE = sizeof("2005-01-01 12:00:00");
const unsigned MAX_QUERY_SIZE = 1024;

////////////////////////////////////////////////////////////////////////////////
int DoMysqlQuery(MYSQL *pConn, const char cszQuery[]);

int GetServerVariable(
    MYSQL *pConn, 
    const char cszVariablesName[], 
    unsigned uRetBufferSize, char *pszRetBuffer
);

int CheckDataBaseVariable(MYSQL *pConn);
