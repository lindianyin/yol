////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGD_MySQL_Database.cpp
//  Version     : 1.0
//  Creater     : Wu Caizhong
//  Create Date : 2005-01-24 16:14:57
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KGD_Error.h"
#include "KGD_MySQL_Database.h"
#include "errmsg.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
struct KGD_ENUM_MAP_ELEMENT
{
    char *pcszStr;      // point to string
    unsigned uSize;     // size of string
};

// type name map array, use value of KGD_FIELD_TYPE as index. 
static const KGD_ENUM_MAP_ELEMENT gs_caTypeName[] =
{
    {"INT",             sizeof("INT")            },// KGD_TYPE_INT32        =0x00
    {"INT UNSIGNED",    sizeof("INT UNSIGNED")   },//KGD_TYPE_UNSIGNED_INT32=0x01
    {"BIGINT",          sizeof("BIGINT")         },// KGD_TYPE_INT64        =0x02
    {"BIGINT UNSIGNED", sizeof("BIGINT UNSIGNED")},//KGD_TYPE_UNSIGNED_INT64=0x03
    {"CHAR",            sizeof("CHAR")           },// KGD_TYPE_CHAR         =0x04
    {"VARCHAR",         sizeof("VARCHAR")        },// KGD_TYPE_VARCHAR      =0x05
    {"TEXT",            sizeof("TEXT")           },// KGD_TYPE_TEXT         =0x06
    {"BINARY",          sizeof("BINARY")         },// KGD_TYPE_BINARY       =0x07
    {"VARBINARY",       sizeof("VARBINARY")      },// KGD_TYPE_VARBINARY    =0x08
    {"BLOB",            sizeof("BLOB")           },// KGD_TYPE_BLOB         =0x09
    {"DATETIME",        sizeof("DATETIME")       } // KGD_TYPE_DATETIME     =0x0A
};
static const unsigned TYPE_NAME_ARRAY_SIZE = 
    sizeof(gs_caTypeName) / sizeof(gs_caTypeName[0]);
// Field Description example:
//     "ID INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY" 
static const unsigned FIELD_DEFINIENS_BUFFER_MAX_SIZE = 
    KGD_NAME_BUFFER_MAX_SIZE + 64;

////////////////////////////////////////////////////////////////////////////////
// operate map array, use value of KGD_OPERATOR as index
static const KGD_ENUM_MAP_ELEMENT gs_caOperator[] =
{
    {"",        sizeof("")  },          // KGD_OPERATOR_NONE        = 0x00,
    {"<",       sizeof("<") },          // KGD_OPERATOR_LESS        = 0x01,
    {"<=",      sizeof("<=")},          // KGD_OPERATOR_LESS_EQUAL  = 0x02,
    {"=",       sizeof("=") },          // KGD_OPERATOR_EQUAL       = 0x03,
    {"!=",      sizeof("!=")},          // KGD_OPERATOR_NOT_EQUAL   = 0x04,
    {">",       sizeof(">") },          // KGD_OPERATOR_ABOVE       = 0x05,
    {">=",      sizeof(">=")}           // KGD_OPERATOR_ABOVE_EQUAL = 0x06,
};
static const unsigned OPERATOR_ARRAY_SIZE = 
    sizeof(gs_caOperator) / sizeof(gs_caOperator[0]);

static const unsigned long MYSQL5_VERSION_ID = 50000; // MySQL 5 Version ID

////////////////////////////////////////////////////////////////////////////////
//                              Interface function
////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::CreateDatabase(
    const char cszDatabaseName[KGD_NAME_BUFFER_MAX_SIZE], 
    int nCreateFlag
)
{
    int nResult  = false;
    int nRetCode = false;
    char szQuery[sizeof("CREATE DATABASE IF NOT EXISTS ") + KGD_NAME_BUFFER_MAX_SIZE];

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);
    
    QDB_PROCESS_ERROR(cszDatabaseName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszDatabaseName); 
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);
    
    // create database
    if (nCreateFlag & KGD_CREATE_IF_NOT_EXIST) 
    {
        nRetCode = sprintf(
            szQuery, "CREATE DATABASE IF NOT EXISTS %s", cszDatabaseName
        );
    }
    else
    {
        nRetCode = sprintf(
            szQuery, "CREATE DATABASE %s", cszDatabaseName
        );
    }
    ASSERT(nRetCode != -1);

    nRetCode = _DoQuey(szQuery, nRetCode);
    PROCESS_ERROR(nRetCode);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult)
    {
        // log param 
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] (0x%p)->%s(\"%s\", %d)",
            this, __FUNCTION__, cszDatabaseName, nCreateFlag
        );
    }
    return  nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::RemoveDatabase(
    const char cszDatabaseName[KGD_NAME_BUFFER_MAX_SIZE]
)
{
    int nResult  = false;
    int nRetCode = false;
    char szQuery[sizeof("DROP DATABASE IF EXISTS ") + KGD_NAME_BUFFER_MAX_SIZE];

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);
  
    QDB_PROCESS_ERROR(cszDatabaseName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszDatabaseName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);
    
    // remove database
    nRetCode = sprintf(szQuery, "DROP DATABASE IF EXISTS %s", cszDatabaseName);
    ASSERT(nRetCode != -1);

    nRetCode = _DoQuey(szQuery, nRetCode);
    PROCESS_ERROR(nRetCode);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult)
    {
        // log param 
        QLogPrintf(
            LOG_DEBUG, "[KGD] (0x%p)->%s(\"%s\")", this, __FUNCTION__, cszDatabaseName
        );
    }
    return nResult;
}   

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::SetCurrentDatabase(
    const char cszDatabaseName[KGD_NAME_BUFFER_MAX_SIZE]
)
{
    int nResult  = false;
    int nRetCode = false;

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    QDB_PROCESS_ERROR(cszDatabaseName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszDatabaseName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    // set current database
    nRetCode = mysql_select_db(m_pMySQL, cszDatabaseName);
    KGD_MYSQL_ENGINE_PROCESS_ERROR(nRetCode == 0, m_pMySQL, "mysql_select_db()");

    nRetCode = _CheckDatabaseCharacterSet();
    PROCESS_ERROR(nRetCode);

    // get table information of current database
    nRetCode = _LoadAllTables();
    PROCESS_ERROR(nRetCode);    

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult)
    {
        // log param 
        QLogPrintf(
            LOG_DEBUG, "[KGD] (0x%p)->%s(\"%s\")", this, __FUNCTION__, cszDatabaseName
        );
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::CreateTable(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE],
    unsigned uFieldCount,
    const KGD_FIELD caFields[],
    int nCreateFlag
)
{
    int nResult  = false;
    int nRetCode = false;
    unsigned i   = 0;

    IMemBlock *piBuf = NULL;
    char *pszQuery    = NULL;
    char *pszWritePos = NULL;

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);
    
    QDB_PROCESS_ERROR(cszTableName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszTableName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    QDB_PROCESS_ERROR(uFieldCount > 0, KGD_ERROR_FIELD_COUNT_IS_ZERO);
    QDB_PROCESS_ERROR(uFieldCount <= KGD_MAX_FIELD_COUNT, KGD_ERROR_TOO_MANY_FIELD);
    QDB_PROCESS_ERROR(caFields, KGD_ERROR_NULL_POINTER);

    // create buffer
    piBuf = QCreateMemBlock(
        sizeof("CREATE TABLE IF NOT EXISTS  ()") + KGD_NAME_BUFFER_MAX_SIZE + // table name
        (FIELD_DEFINIENS_BUFFER_MAX_SIZE + sizeof(',')) * uFieldCount +       // fields define
        (KGD_NAME_BUFFER_MAX_SIZE + sizeof("UNIQUE INDEX ((255)),")) * uFieldCount   // index
		+ sizeof("PRIMARY KEY ((255))") + KGD_NAME_BUFFER_MAX_SIZE
    );
    QDB_PROCESS_ERROR(piBuf, KGD_ERROR_NOT_ENOUGH_MEMORY);
    pszQuery = (char *)piBuf->GetData();
    pszWritePos = pszQuery;
    
    // -------------------- make query string -------------------- 
    if (nCreateFlag & KGD_CREATE_IF_NOT_EXIST) 
    {   
        nRetCode = sprintf(
            pszWritePos, "CREATE TABLE IF NOT EXISTS %s (", cszTableName
        );
    }
    else
    {
        nRetCode = sprintf(pszWritePos, "CREATE TABLE %s (", cszTableName);
    }
    ASSERT(nRetCode != -1);
    pszWritePos += nRetCode;
    
    // fields define list(include set of name, type, primary key
    for (i = 0; i < uFieldCount; ++i)
    {
        nRetCode = _StoreFieldDefiniens(pszWritePos, caFields[i]);
        PROCESS_ERROR(nRetCode != -1);
        pszWritePos += nRetCode - 1;
        *pszWritePos++ = ','; 
    }
    // index list --- because index do not include in field define ----
    for (i = 0; i < uFieldCount; ++i)
    {
        if (caFields[i].nFlag & KGD_FIELD_INDEX)
        {
            // create index 
            QDB_PROCESS_ERROR(
                !(caFields[i].nFlag & (KGD_FIELD_UNIQUE_INDEX | KGD_FIELD_PRIMARY_KEY)), 
                 KGD_ERROR_INVALID_FLAG
             );

            // we checked filed name length at above loop.
			if ((caFields[i].Type == KGD_TYPE_BLOB) || (caFields[i].Type == KGD_TYPE_TEXT))
			{
				nRetCode = sprintf(
					pszWritePos, 
					"INDEX(%s(%d)),", 
					caFields[i].szName, KGD_BLOB_OR_TEXT_INDEX_PREFIX_MAX_NUM
				);
			}
			else
			{
				nRetCode = sprintf(pszWritePos, "INDEX(%s),", caFields[i].szName);
			}
            ASSERT(nRetCode != -1);
            pszWritePos += nRetCode;        
        }
        else if (caFields[i].nFlag & KGD_FIELD_UNIQUE_INDEX)
        {
             // create unique index 
            QDB_PROCESS_ERROR(
                !(caFields[i].nFlag & (KGD_FIELD_INDEX | KGD_FIELD_PRIMARY_KEY)),
                KGD_ERROR_INVALID_FLAG
            );

            // we checked filed name length at above loop.
			if ((caFields[i].Type == KGD_TYPE_BLOB) || (caFields[i].Type == KGD_TYPE_TEXT))
			{
				nRetCode = sprintf(
					pszWritePos, 
					"UNIQUE INDEX(%s(%d)),", 
					caFields[i].szName, KGD_BLOB_OR_TEXT_INDEX_PREFIX_MAX_NUM
				);
			}
			else
			{
				nRetCode = sprintf(pszWritePos, "UNIQUE INDEX(%s),", caFields[i].szName);
			}
            ASSERT(nRetCode != -1);
            pszWritePos += nRetCode;
        }

		// ------  flag ------  
		if (caFields[i].nFlag & KGD_FIELD_PRIMARY_KEY) 
		{
			QDB_PROCESS_ERROR(
				!(caFields[i].nFlag & (KGD_FIELD_INDEX | KGD_FIELD_UNIQUE_INDEX)),
				KGD_ERROR_INVALID_FLAG
			);

			if ((caFields[i].Type == KGD_TYPE_BLOB) || (caFields[i].Type == KGD_TYPE_TEXT))
			{
				nRetCode = sprintf(
					pszWritePos, 
					"PRIMARY KEY(%s(%d)),", 
					caFields[i].szName, KGD_BLOB_OR_TEXT_INDEX_PREFIX_MAX_NUM
				);
			}
			else
			{
				nRetCode = sprintf(pszWritePos, "PRIMARY KEY(%s),", caFields[i].szName);
			}
			ASSERT(nRetCode != -1);
			pszWritePos += nRetCode;
		}
    }

    --pszWritePos;         // will cover last ',' assert uFieldCount > 0
    *pszWritePos++ = ')';  
    *pszWritePos = '\0';
    
    // --------------------  create table --------------------  
    nRetCode = _DoQuey(pszQuery, (unsigned)(pszWritePos - pszQuery));
    PROCESS_ERROR(nRetCode);

    // load table information
    nRetCode = _LoadTable(cszTableName);
    PROCESS_ERROR(nRetCode);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    SAFE_RELEASE(piBuf);
    if (!nResult)
    {
        // log param 
        char szLogBuf[1024];
        nRetCode = snprintf(
            szLogBuf, sizeof(szLogBuf) - 1,
            "[KGD] (0x%p)->%s(\"%s\", %u, 0x%p, %d),",
            this, __FUNCTION__, cszTableName, uFieldCount, caFields, nCreateFlag
        );
        szLogBuf[sizeof(szLogBuf) - 1] = '\0';
        if (nRetCode > 0)
        {
            KGD_DumpKGDFields(
                (sizeof(szLogBuf) - nRetCode), (szLogBuf + nRetCode),
                uFieldCount, caFields, NULL, NULL
            );
        }
        QLogPrintf(LOG_DEBUG, "%s", szLogBuf);
    }    
    return  nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::RemoveTable(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE]
)
{
    int nResult  = false;
    int nRetCode = false;
    char szQuery[sizeof("DROP TABLE IF EXISTS ") + KGD_NAME_BUFFER_MAX_SIZE];
    
    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    QDB_PROCESS_ERROR(cszTableName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszTableName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);


    // remove table
    nRetCode = sprintf(szQuery, "DROP TABLE IF EXISTS %s", cszTableName);
    ASSERT(nRetCode != -1);

    nRetCode = _DoQuey(szQuery, nRetCode);
    PROCESS_ERROR(nRetCode);

    m_TableMap.erase(cszTableName);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:   
    if (!nResult)
    {
        // log param 
        QLogPrintf(
            LOG_DEBUG, "[KGD] (0x%p)->%s(\"%s\")", this, __FUNCTION__, cszTableName
        );
    }
    return  nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::AddField(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
    unsigned uFieldCount,
    const KGD_FIELD caFields[] 
)
{
    int nResult  = false;
    int nRetCode = false;
    unsigned i = 0;

    IMemBlock *piBuf = NULL;
    char *pszQuery    = NULL;
    char *pszWritePos = NULL;
    KGD_MySQL_TableMap::iterator itMySQLTable;

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    QDB_PROCESS_ERROR(cszTableName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszTableName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    itMySQLTable = m_TableMap.find(cszTableName);
    QDB_PROCESS_ERROR(itMySQLTable != m_TableMap.end(), KGD_ERROR_TABLE_NOT_EXIST);
   
    QDB_PROCESS_ERROR(uFieldCount > 0, KGD_ERROR_FIELD_COUNT_IS_ZERO);
    QDB_PROCESS_ERROR(
        uFieldCount <= (KGD_MAX_FIELD_COUNT - itMySQLTable->second.FieldMap.size()), 
        KGD_ERROR_TOO_MANY_FIELD
    );
    QDB_PROCESS_ERROR(caFields, KGD_ERROR_NULL_POINTER);
       
    // create buffer
    piBuf = QCreateMemBlock(
        sizeof("ALTER TABLE ADD()") + KGD_NAME_BUFFER_MAX_SIZE +         // table name
        (FIELD_DEFINIENS_BUFFER_MAX_SIZE + sizeof(','))* uFieldCount +   // field define
        (KGD_NAME_BUFFER_MAX_SIZE + sizeof(",ADD UNIQUE()")) * uFieldCount // index
    );
    QDB_PROCESS_ERROR(piBuf, KGD_ERROR_NOT_ENOUGH_MEMORY);
    pszQuery = (char *)piBuf->GetData();
    pszWritePos = pszQuery;

    // -------------------------- add field ----------------------------
    nRetCode = sprintf(pszWritePos, "ALTER TABLE %s ADD(", cszTableName);
    ASSERT(nRetCode != -1);
    pszWritePos += nRetCode;

    // fields define list(include set of name, type, primary key
    for (i = 0; i < uFieldCount; ++i)
    {
        nRetCode = _StoreFieldDefiniens(pszWritePos, caFields[i]);
        PROCESS_ERROR(nRetCode != -1);
        pszWritePos += nRetCode - 1;
        *pszWritePos++ = ','; 
    }
    --pszWritePos;         // will cover last ',' assert uFieldCount > 0
    *pszWritePos++ = ')'; 
    *pszWritePos   = '\0';

    // index list --- because index do not include in field define ----
    for (i = 0; i < uFieldCount; ++i)
    {
        if (caFields[i].nFlag & KGD_FIELD_INDEX)
        {
            // create index 
            QDB_PROCESS_ERROR(
                !(caFields[i].nFlag & (KGD_FIELD_UNIQUE_INDEX | KGD_FIELD_PRIMARY_KEY)),
                KGD_ERROR_INVALID_FLAG
            );

            // we checked filed name length at above loop.
			if ((caFields[i].Type == KGD_TYPE_BLOB) || (caFields[i].Type == KGD_TYPE_TEXT))
			{
				nRetCode = sprintf(
					pszWritePos, 
					",ADD INDEX(%s(%d))", 
					caFields[i].szName, KGD_BLOB_OR_TEXT_INDEX_PREFIX_MAX_NUM
				);
			}
			else
			{
				nRetCode = sprintf(pszWritePos, ",ADD INDEX(%s)", caFields[i].szName);
			}
			ASSERT(nRetCode != -1);
			pszWritePos += nRetCode;
        }
        else if (caFields[i].nFlag & KGD_FIELD_UNIQUE_INDEX)
        {
            // create unique index 
            QDB_PROCESS_ERROR(
                !(caFields[i].nFlag & (KGD_FIELD_INDEX | KGD_FIELD_PRIMARY_KEY)),
                KGD_ERROR_INVALID_FLAG
            );

            // we checked filed name length at above loop.
			if ((caFields[i].Type == KGD_TYPE_BLOB) || (caFields[i].Type == KGD_TYPE_TEXT))
			{
				nRetCode = sprintf(
					pszWritePos, 
					",ADD UNIQUE(%s(%d))", 
					caFields[i].szName, KGD_BLOB_OR_TEXT_INDEX_PREFIX_MAX_NUM
				);
			}
			else
			{
				nRetCode = sprintf(pszWritePos, ",ADD UNIQUE(%s)", caFields[i].szName);
			}
            ASSERT(nRetCode != -1);
            pszWritePos += nRetCode;
        }
        else if (caFields[i].nFlag & KGD_FIELD_PRIMARY_KEY) 
		{
			QDB_PROCESS_ERROR(
				!(caFields[i].nFlag & (KGD_FIELD_INDEX | KGD_FIELD_UNIQUE_INDEX)),
				KGD_ERROR_INVALID_FLAG
			);

			if ((caFields[i].Type == KGD_TYPE_BLOB) || (caFields[i].Type == KGD_TYPE_TEXT))
			{
				nRetCode = sprintf(
					pszWritePos, 
					",ADD PRIMARY KEY(%s(%d))", 
					caFields[i].szName, KGD_BLOB_OR_TEXT_INDEX_PREFIX_MAX_NUM
				);
			}
			else
			{
				nRetCode = sprintf(pszWritePos, ",ADD PRIMARY KEY(%s)", caFields[i].szName);
			}
			ASSERT(nRetCode != -1);
			pszWritePos += nRetCode;
		}
    }

    // send query to MySQL to add field.
    nRetCode = _DoQuey(pszQuery, (unsigned)(pszWritePos - pszQuery));
    PROCESS_ERROR(nRetCode);

    // reload information
    nRetCode = _LoadTable(cszTableName);
    PROCESS_ERROR(nRetCode);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    SAFE_RELEASE(piBuf);
    if (!nResult)
    {
        // log param 
        char szLogBuf[1024];
        nRetCode = snprintf(
            szLogBuf, sizeof(szLogBuf) - 1,
            "[KGD] (0x%p)->%s(\"%s\", %u, 0x%p), ",
            this, __FUNCTION__, cszTableName, uFieldCount, caFields
        );
        szLogBuf[sizeof(szLogBuf) - 1] = '\0';
        if (nRetCode > 0)
        {
            KGD_DumpKGDFields(
                (sizeof(szLogBuf) - nRetCode), (szLogBuf + nRetCode),
                uFieldCount, caFields, NULL, NULL
            );
        }
        QLogPrintf(LOG_DEBUG, "%s", szLogBuf);
    }    
    return  nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::RemoveField(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
    unsigned uFieldCount,
    const KGD_FIELD caFields[] 
)
{
    int nResult  = false;
    int nRetCode = false;
    unsigned i = 0;
    IMemBlock *piBuf = NULL;
    char *pszQuery    = NULL;
    char *pszWritePos = NULL;

    m_LastErrorCode = KGD_ERROR_UNKNOW;     
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    QDB_PROCESS_ERROR(cszTableName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszTableName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    QDB_PROCESS_ERROR(uFieldCount > 0, KGD_ERROR_FIELD_COUNT_IS_ZERO);
    QDB_PROCESS_ERROR(uFieldCount <= KGD_MAX_FIELD_COUNT, KGD_ERROR_TOO_MANY_FIELD);
    QDB_PROCESS_ERROR(caFields, KGD_ERROR_NULL_POINTER);
       
    // create buffer
    piBuf = QCreateMemBlock(
        sizeof("ALTER TABLE  ") + KGD_NAME_BUFFER_MAX_SIZE +        // table name
        (KGD_NAME_BUFFER_MAX_SIZE + sizeof("DROP ,")) * uFieldCount // fields name
    );
    QDB_PROCESS_ERROR(piBuf, KGD_ERROR_NOT_ENOUGH_MEMORY);
    pszQuery = (char *)piBuf->GetData();
    pszWritePos = pszQuery;

    // -------------------- remove field -------------------- 
    nRetCode = sprintf(pszWritePos, "ALTER TABLE %s ", cszTableName);
    ASSERT(nRetCode != -1);
    pszWritePos += nRetCode;

    for (i = 0; i < uFieldCount; ++i)
    {
        nRetCode = (int)strlen(caFields[i].szName);
        QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
        QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);
        
        nRetCode = sprintf(pszWritePos, "DROP %s,", caFields[i].szName);
        ASSERT(nRetCode != -1);
        pszWritePos += nRetCode;
    }
    --pszWritePos;         // will cover last ',' assert uFieldCount > 0
    *pszWritePos = '\0';

    // send query to MySQL to remove field.
    nRetCode = _DoQuey(pszQuery, (unsigned)(pszWritePos - pszQuery));
    PROCESS_ERROR(nRetCode);

    // reload information
    nRetCode = _LoadTable(cszTableName);
    PROCESS_ERROR(nRetCode);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    SAFE_RELEASE(piBuf);
    if (!nResult)
    {
        // log param 
        char szLogBuf[1024];
        nRetCode = snprintf(
            szLogBuf, sizeof(szLogBuf) - 1,
            "[KGD] (0x%p)->%s(\"%s\", %u, 0x%p), ",
            this, __FUNCTION__, cszTableName, uFieldCount, caFields
            );
        szLogBuf[sizeof(szLogBuf) - 1] = '\0';
        if (nRetCode > 0)
        {
            KGD_DumpKGDFields(
                (sizeof(szLogBuf) - nRetCode), (szLogBuf + nRetCode),
                uFieldCount, caFields, NULL, NULL
                );
        }
        QLogPrintf(LOG_DEBUG, "%s", szLogBuf);
    }  
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::Query(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
    unsigned uResultFieldCount,
    const KGD_FIELD caResultFields[], 
    const KGD_FIELD *pcMatchField1, 
    KGD_OPERATOR Operator1,
    const KGD_FIELD *pcMatchField2, 
    KGD_OPERATOR Operator2
)
{
    int nResult  = false;
    int nRetCode = false;
    KGD_MYSQL_FIELD_PAIR MatchFieldPair1;
    KGD_MYSQL_FIELD_PAIR MatchFieldPair2;
    IMemBlock *piBuf = NULL;
    char *pszWritePos = NULL;
    char *pszQuery    = NULL;
    unsigned i = 0;

    m_LastErrorCode = KGD_ERROR_UNKNOW;    
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    QDB_PROCESS_ERROR(cszTableName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszTableName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    QDB_PROCESS_ERROR(uResultFieldCount > 0, KGD_ERROR_FIELD_COUNT_IS_ZERO); 
    QDB_PROCESS_ERROR(uResultFieldCount <= KGD_MAX_FIELD_COUNT,KGD_ERROR_TOO_MANY_FIELD);
    QDB_PROCESS_ERROR(caResultFields, KGD_ERROR_NULL_POINTER);
    if (pcMatchField2)
    {   
        // is specify match field 2, must specify field1 too
        QDB_PROCESS_ERROR(pcMatchField1, KGD_ERROR_NULL_POINTER);
    }

    nRetCode = _MapParam(
        cszTableName, 
        0, NULL, NULL, 
        pcMatchField1, &MatchFieldPair1, pcMatchField2, &MatchFieldPair2
    );
    PROCESS_ERROR(nRetCode);
    piBuf = _CreateBuffer(
        NULL, 
        &MatchFieldPair1, Operator1, &MatchFieldPair2, Operator2,
        sizeof("SELECT  FROM  ") +  KGD_NAME_BUFFER_MAX_SIZE +        // table 
        (KGD_NAME_BUFFER_MAX_SIZE + sizeof(',')) * uResultFieldCount  // fields name
    );
    PROCESS_ERROR(piBuf);

    pszQuery = (char *)piBuf->GetData();
    pszWritePos = pszQuery;

    // -------------------------- make query string ----------------------------
    memcpy(pszWritePos, "SELECT ", sizeof("SELECT ") - 1);
    pszWritePos += sizeof("SELECT ") - 1;
    
    // result field list
    for (i = 0; i < uResultFieldCount; ++i)
    {
        nRetCode = (int)strlen(caResultFields[i].szName);
        QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
        QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

        memcpy(pszWritePos, caResultFields[i].szName, nRetCode);
        pszWritePos += nRetCode;
        
        *pszWritePos++ = ',';             
    }
    --pszWritePos;  // will cover last ','
    
    // table name
    nRetCode = sprintf(pszWritePos, " FROM %s", cszTableName);
    ASSERT(nRetCode != -1);
    pszWritePos += nRetCode;

    // condition
    if (pcMatchField1) 
    {
        nRetCode = _StoreWhereCondition(
            pszWritePos,
            MatchFieldPair1, Operator1, MatchFieldPair2, Operator2
        );
        PROCESS_ERROR(nRetCode != -1);
        pszWritePos += nRetCode - 1;  
    }

    // ----------------------- send query to server ------------------------
    nRetCode = _DoQuey(pszQuery, (unsigned)(pszWritePos - pszQuery));
    PROCESS_ERROR(nRetCode);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult  = true;
EXIT0:
    SAFE_RELEASE(piBuf);
    if (!nResult)
    {
        // log param 
        char szLogBuf[1024];
        nRetCode = snprintf(
            szLogBuf, sizeof(szLogBuf) - 1,
            "[KGD] (0x%p)->%s(\"%s\", %u, 0x%p, 0x%p, %d, 0x%p, %d),",
            this, __FUNCTION__, cszTableName, uResultFieldCount, caResultFields, 
            pcMatchField1, Operator1, pcMatchField2, Operator2 
        );
        szLogBuf[sizeof(szLogBuf) - 1] = '\0';
        if (nRetCode > 0)
        {
            KGD_DumpKGDFields(
                (sizeof(szLogBuf) - nRetCode), (szLogBuf + nRetCode),
                uResultFieldCount, caResultFields, pcMatchField1, pcMatchField2
            );
        }
        QLogPrintf(LOG_DEBUG, "%s", szLogBuf);
    }    
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
IKGD_Result *KGD_MySQL_Database::GetQueryResult(KGD_RESULT_FLAG Flag)
{
    int nResult  = false;
    int nRetCode = false;
    KGD_MySQL_Result *pQueryResult = NULL;

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    pQueryResult = new KGD_MySQL_Result;
    QDB_PROCESS_ERROR(pQueryResult, KGD_ERROR_NOT_ENOUGH_MEMORY);

    nRetCode = pQueryResult->StoreResult(Flag, m_pMySQL, this);
    PROCESS_ERROR(nRetCode);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult) 
    {
        SAFE_RELEASE(pQueryResult);
        // log param 
        QLogPrintf(LOG_DEBUG, "[KGD] (0x%p)->%s(%d)", this, __FUNCTION__, Flag);
    }

    return pQueryResult;
}

////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::Update(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
    unsigned uFieldCount,
    const KGD_FIELD caBeUpdateFields[],
    const KGD_FIELD *pcMatchField1, 
    KGD_OPERATOR Operator1,
    const KGD_FIELD *pcMatchField2, 
    KGD_OPERATOR Operator2
)
{
    int nResult  = false;
    int nRetCode = false;
    KGD_MYSQL_FIELD_PAIR MatchFieldPair1;
    KGD_MYSQL_FIELD_PAIR MatchFieldPair2;
    KGD_MySQL_FieldPairVector aFieldPair;
    IMemBlock *piBuf = NULL;
    char *pszWritePos = NULL;
    char *pszQuery = NULL;
    unsigned i = 0;

    m_LastErrorCode = KGD_ERROR_UNKNOW;    
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    QDB_PROCESS_ERROR(cszTableName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszTableName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    QDB_PROCESS_ERROR(uFieldCount > 0, KGD_ERROR_FIELD_COUNT_IS_ZERO);
    QDB_PROCESS_ERROR(uFieldCount <= KGD_MAX_FIELD_COUNT, KGD_ERROR_TOO_MANY_FIELD);
    QDB_PROCESS_ERROR(caBeUpdateFields, KGD_ERROR_NULL_POINTER);
    if (pcMatchField2)
    {   
        // is specify match field 2, must specify field1 too
        QDB_PROCESS_ERROR(pcMatchField1, KGD_ERROR_NULL_POINTER);
    }

    nRetCode = _MapParam(
        cszTableName,
        uFieldCount, caBeUpdateFields, &aFieldPair, 
        pcMatchField1, &MatchFieldPair1, pcMatchField2, &MatchFieldPair2
    );
    PROCESS_ERROR(nRetCode);
    piBuf = _CreateBuffer(
        &aFieldPair,
        &MatchFieldPair1, Operator1, &MatchFieldPair2, Operator2,
        sizeof("UPDATE  SET ") + KGD_NAME_BUFFER_MAX_SIZE
    );
    PROCESS_ERROR(piBuf);

    pszQuery = (char *)piBuf->GetData();
    pszWritePos = pszQuery;

    // -------------------------- make query string ----------------------------
    // use pTableInfo->szName, so we do not need cover cszTable to lower case.
    nRetCode = sprintf(pszWritePos, "UPDATE %s SET ", cszTableName);
    ASSERT(nRetCode != -1);
    pszWritePos += nRetCode;
    // Field list
    for (i = 0; i < uFieldCount; ++i)
    {
        // name
        ASSERT(aFieldPair[i].pcMySQLField);
        nRetCode = aFieldPair[i].pcMySQLField->StoreNameToBuffer(pszWritePos);
        QDB_PROCESS_ERROR(nRetCode != -1, KGD_ERROR_UNKNOW);
        pszWritePos += nRetCode - 1;
        *pszWritePos++ = '=';
       
        // value
        ASSERT(aFieldPair[i].pcKGDField);
        nRetCode = aFieldPair[i].pcMySQLField->StoreDataToBuffer(
            pszWritePos, *(aFieldPair[i].pcKGDField)
        );
        QDB_PROCESS_ERROR(nRetCode != -1, KGD_ERROR_UNKNOW);
        pszWritePos += nRetCode - 1;
        *pszWritePos++ = ',';
    }
    --pszWritePos;             // -- will cover last ','
    *pszWritePos = '\0';

    // update condition
    if (pcMatchField1) 
    {
        nRetCode = _StoreWhereCondition(
            pszWritePos, 
            MatchFieldPair1, Operator1, MatchFieldPair2, Operator2
        );
        PROCESS_ERROR(nRetCode != -1);
        pszWritePos += nRetCode - 1;        // not include '\0'
    }

    // ---------------------------- query to server ------------------------
    nRetCode = _DoQuey(pszQuery, (unsigned)(pszWritePos - pszQuery));
    PROCESS_ERROR(nRetCode);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    SAFE_RELEASE(piBuf);
    if (!nResult)
    {
        // log param 
        char szLogBuf[1024];
        nRetCode = snprintf(
            szLogBuf, sizeof(szLogBuf) - 1,
            "[KGD] (0x%p)->%s(\"%s\", %u, 0x%p, 0x%p, %d, 0x%p, %d),",
            this, __FUNCTION__, cszTableName, uFieldCount, caBeUpdateFields, 
            pcMatchField1, Operator1, pcMatchField2, Operator2 
        );
        szLogBuf[sizeof(szLogBuf) - 1] = '\0';
        if (nRetCode > 0)
        {
            KGD_DumpKGDFields(
                (sizeof(szLogBuf) - nRetCode), (szLogBuf + nRetCode),
                uFieldCount, caBeUpdateFields, pcMatchField1, pcMatchField2
            );
        }
        QLogPrintf(LOG_DEBUG, "%s", szLogBuf);
    }    
    return  nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::Delete(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
    const KGD_FIELD *pcMatchField1, 
    KGD_OPERATOR Operator1,
    const KGD_FIELD *pcMatchField2, 
    KGD_OPERATOR Operator2
)
{
    int nResult  = false;
    int nRetCode = false;
    KGD_MYSQL_FIELD_PAIR MatchFieldPair1;
    KGD_MYSQL_FIELD_PAIR MatchFieldPair2;
    IMemBlock *piBuf = NULL;
    char *pszWritePos = NULL;
    char *pszQuery = NULL;

    m_LastErrorCode = KGD_ERROR_UNKNOW;     
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    QDB_PROCESS_ERROR(cszTableName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszTableName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    if (pcMatchField2)
    {   
        // is specify match field 2, must specify field1 too
        QDB_PROCESS_ERROR(pcMatchField1, KGD_ERROR_NULL_POINTER);
    }

    nRetCode = _MapParam(
        cszTableName,
        0, NULL, NULL, 
        pcMatchField1, &MatchFieldPair1, pcMatchField2, &MatchFieldPair2
    );
    PROCESS_ERROR(nRetCode);

    piBuf = _CreateBuffer(
        NULL,
        &MatchFieldPair1, Operator1, &MatchFieldPair2, Operator2,
        sizeof("DELETE FROM ") + KGD_NAME_BUFFER_MAX_SIZE
    );
    PROCESS_ERROR(piBuf);
    pszQuery = (char *)piBuf->GetData();
    pszWritePos = pszQuery;

    // -------------------------- make query string ----------------------------
    // use pTableInfo->szName, so we do not need cover cszTable to lower case.
    nRetCode = sprintf(pszWritePos, "DELETE FROM %s", cszTableName);
    ASSERT(nRetCode != -1);
    pszWritePos += nRetCode;

    // condition
    if (pcMatchField1) 
    {
        nRetCode = _StoreWhereCondition(
            pszWritePos, MatchFieldPair1, Operator1, MatchFieldPair2, Operator2
        );
        PROCESS_ERROR(nRetCode != -1);
        pszWritePos += nRetCode - 1;    
    }

    // ---------------------------- query to server ------------------------
    nRetCode = _DoQuey(pszQuery, (unsigned)(pszWritePos - pszQuery));
    PROCESS_ERROR(nRetCode);

//EXIT1:                                           
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    SAFE_RELEASE(piBuf);
    if (!nResult)
    {
        // log param 
        char szLogBuf[256];
        nRetCode = snprintf(
            szLogBuf, sizeof(szLogBuf) - 1,
            "[KGD] (0x%p)->%s(\"%s\", 0x%p, %d, 0x%p, %d),",
            this, __FUNCTION__, cszTableName, 
            pcMatchField1, Operator1, pcMatchField2, Operator2 
        );
        szLogBuf[sizeof(szLogBuf) - 1] = '\0';
        if (nRetCode > 0)
        {
            KGD_DumpKGDFields(
                (sizeof(szLogBuf) - nRetCode), (szLogBuf + nRetCode),
                0, NULL, pcMatchField1, pcMatchField2
            );
        }
        QLogPrintf(LOG_DEBUG, "%s", szLogBuf);
    } 
    return  nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::Insert(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE], 
    unsigned uFieldCount, 
    const KGD_FIELD caFields[]
)
{
    int nResult  = false;
    int nRetCode = false;
    unsigned i = 0;
    KGD_MySQL_FieldPairVector aFieldPair;
    IMemBlock *piBuf = NULL;
    char *pszWritePos = NULL;
    char *pszQuery = NULL;

    m_LastErrorCode = KGD_ERROR_UNKNOW;    
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    QDB_PROCESS_ERROR(cszTableName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszTableName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    QDB_PROCESS_ERROR(uFieldCount > 0, KGD_ERROR_FIELD_COUNT_IS_ZERO); 
    QDB_PROCESS_ERROR(uFieldCount <= KGD_MAX_FIELD_COUNT, KGD_ERROR_TOO_MANY_FIELD);
    QDB_PROCESS_ERROR(caFields, KGD_ERROR_NULL_POINTER);

    nRetCode = _MapParam(
        cszTableName, 
        uFieldCount, caFields, &aFieldPair, 
        NULL, NULL, NULL, NULL
    );
    PROCESS_ERROR(nRetCode);

    piBuf = _CreateBuffer(
        &aFieldPair,
        NULL, KGD_OPERATOR_NONE, NULL, KGD_OPERATOR_NONE,
        sizeof("INSERT INTO ()VALUES()") + KGD_NAME_BUFFER_MAX_SIZE
    );
    PROCESS_ERROR(piBuf);

    pszQuery = (char*)piBuf->GetData();
    pszWritePos = pszQuery;

    // -------------------------- make query string ----------------------------
    nRetCode = sprintf(pszWritePos, "INSERT INTO %s(", cszTableName);
    ASSERT(nRetCode != -1);
    pszWritePos += nRetCode;
   
    // name list
    for (i = 0; i < uFieldCount; ++i)
    {
        ASSERT(aFieldPair[i].pcMySQLField);

        nRetCode = aFieldPair[i].pcMySQLField->StoreNameToBuffer(pszWritePos);
        QDB_PROCESS_ERROR(nRetCode, KGD_ERROR_UNKNOW);
        pszWritePos += nRetCode - 1;
        *pszWritePos++ = ',';
    }
    ASSERT(uFieldCount > 0);

    // value list
    --pszWritePos;           // cover last ','                           
    memcpy(pszWritePos, ")VALUES(", sizeof(")VALUES(") - 1);
    pszWritePos += sizeof(")VALUES(") - 1;


    for (i = 0; i < uFieldCount; ++i)
    {
        ASSERT(aFieldPair[i].pcMySQLField);
        ASSERT(aFieldPair[i].pcKGDField);

        nRetCode = aFieldPair[i].pcMySQLField->StoreDataToBuffer(
            pszWritePos, *(aFieldPair[i].pcKGDField)
        );
        QDB_PROCESS_ERROR(nRetCode != -1, KGD_ERROR_UNKNOW);
        pszWritePos += nRetCode - 1;
        *pszWritePos++ = ',';
    }
    --pszWritePos;        //  to cover last ','
    *pszWritePos++ = ')';
    *pszWritePos   = '\0';

    // ---------------------------- query to server ------------------------
    nRetCode = _DoQuey(pszQuery, (unsigned)(pszWritePos - pszQuery));
    PROCESS_ERROR(nRetCode);

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    SAFE_RELEASE(piBuf);
    if (!nResult)
    {
        // log param 
        char szLogBuf[1024];
        nRetCode = snprintf(
            szLogBuf, sizeof(szLogBuf) - 1,
            "[KGD] (0x%p)->%s(\"%s\", %u, 0x%p),",
            this, __FUNCTION__, cszTableName, uFieldCount, caFields
        );
        szLogBuf[sizeof(szLogBuf) - 1] = '\0';
        if (nRetCode > 0)
        {
            KGD_DumpKGDFields(
                (sizeof(szLogBuf) - nRetCode), (szLogBuf + nRetCode),
                uFieldCount, caFields, NULL, NULL
            );
        }
        QLogPrintf(LOG_DEBUG, "%s", szLogBuf);
    }    
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::GetAffectedRow()
{
    int nResult = false;
    my_ulonglong ullAffectedRow = (my_ulonglong)-1;

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    ullAffectedRow = mysql_affected_rows(m_pMySQL);
    KGD_MYSQL_ENGINE_PROCESS_ERROR(
        (my_ulonglong)-1 != ullAffectedRow, m_pMySQL, "mysql_affected_rows()"
    );

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (nResult == -1)
    {
        // log param 
        QLogPrintf(LOG_DEBUG, "[KGD] (0x%p)->%s()", this, __FUNCTION__);
    }
    return (int)ullAffectedRow;
}

////////////////////////////////////////////////////////////////////////////////
//                The following is for internal use                           //
////////////////////////////////////////////////////////////////////////////////
KGD_MySQL_Database::KGD_MySQL_Database() :
    m_LastErrorCode(KGD_ERROR_UNKNOW), 
    m_pMySQL(NULL),
    m_ulRefCount(1)
{
    m_szCharacterSet[0] = '\0';
}
////////////////////////////////////////////////////////////////////////////////
KGD_MySQL_Database::~KGD_MySQL_Database()
{
    ASSERT(!m_pMySQL);
    QDB_PROCESS_ERROR(!m_pMySQL, KGD_ERROR_RESOURCE_NOT_RELEASE);
EXIT0:
    if (m_pMySQL)
    {
        _Close();
        m_pMySQL = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::Connect(
    const char cszServerName[], 
    unsigned short usPort, 
    const char cszUserName[], 
    const char cszPassword[],
    const char *pcszCurrentDatabase
)
{
    static const char s_cszCharacterSetServer[]   = "character_set_server"; 
    static const char s_cszLowerCaseTableNames[]  = "lower_case_table_names"; 
    int nResult  = false;
    int nRetCode = false;
    const char *pcszHost = "localhost";
    MYSQL *pConnectRet = NULL;
    char szLowerCaseTableNames[4];
    my_bool bReconnectFlag = 0;
    unsigned long ulMySQLClientVersionID = 0;

    m_LastErrorCode = KGD_ERROR_UNKNOW;   
    QDB_PROCESS_ERROR(!m_pMySQL, KGD_ERROR_RESOURCE_NOT_RELEASE);

    QDB_PROCESS_ERROR(cszServerName, KGD_ERROR_NULL_POINTER);
    QDB_PROCESS_ERROR(cszUserName,  KGD_ERROR_NULL_POINTER);
    QDB_PROCESS_ERROR(cszPassword,  KGD_ERROR_NULL_POINTER);

    // ----------------- connect to database  ----------------- 
    m_pMySQL = mysql_init(NULL); 
    KGD_MYSQL_ENGINE_PROCESS_ERROR(m_pMySQL, NULL, "mysql_init()");

    // enable reconnect if MySQL client version > 5
    ulMySQLClientVersionID = mysql_get_client_version();
    if (ulMySQLClientVersionID >= MYSQL5_VERSION_ID)
    {
        bReconnectFlag = 1;
        nRetCode = mysql_options(m_pMySQL, MYSQL_OPT_RECONNECT, &bReconnectFlag);
        KGD_MYSQL_ENGINE_PROCESS_ERROR((nRetCode == 0), m_pMySQL, "mysql_options");
    }

    if (strcmp(cszServerName, "127.0.0.1") != 0)
        pcszHost = cszServerName;

    pConnectRet = mysql_real_connect(
        m_pMySQL, pcszHost, cszUserName, cszPassword, 
        pcszCurrentDatabase, usPort, NULL, 
        CLIENT_FOUND_ROWS  // Return the number of found (matched) rows, 
                           // not the number of affected rows. 
    );
    KGD_MYSQL_ENGINE_PROCESS_ERROR(pConnectRet, m_pMySQL, "mysql_real_connect()");
    ASSERT(pConnectRet == m_pMySQL);
    // because pConnectRet just for judge mysql_real_connect success or failed
    // now is unused
    pConnectRet = NULL;

    // --------- check MySQL Identifier Case Sensitivity  setting -------------
    nRetCode = _GetServerVariable(
        s_cszLowerCaseTableNames, 
        sizeof(szLowerCaseTableNames), 
        szLowerCaseTableNames
    );
    PROCESS_ERROR(nRetCode);

    nRetCode = strcmp(szLowerCaseTableNames, "1");
    KGD_MYSQL_ENGINE_PROCESS_ERROR(
        nRetCode == 0, 
        m_pMySQL, 
        "MySQL Server is not config to case-insensitive,"
        "Please set MySQL variables: lower_case_table_names=1\r\n"
    ); 

    // ---------- check database character set and server character set -------- 
    nRetCode = _GetServerVariable(
        s_cszCharacterSetServer, sizeof(m_szCharacterSet), m_szCharacterSet
    );
    PROCESS_ERROR(nRetCode);

    nRetCode = _CheckDatabaseCharacterSet();
    PROCESS_ERROR(nRetCode);

    // ----------------------- set client character set  ---------------------
    nRetCode = mysql_set_character_set(m_pMySQL, m_szCharacterSet);
    KGD_MYSQL_ENGINE_PROCESS_ERROR(nRetCode == 0, m_pMySQL, "mysql_set_character_set()");

    //  ----------- if current database set, load table information ---------- 
    m_TableMap.clear();

    if (
        (pcszCurrentDatabase) &&
        (pcszCurrentDatabase[0] != '\0')
    )
    {
        nRetCode = _LoadAllTables();
        PROCESS_ERROR(nRetCode);    
    }

    QLogPrintf(
        LOG_DEBUG, 
        "[KGD] Success connect to %s:%u, character set switch to %s",
        pcszHost, usPort, m_szCharacterSet
    );
//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult) 
    {
        if (m_pMySQL)
        {
            mysql_close(m_pMySQL);
            m_pMySQL = NULL;    
        }
        // log parameter
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] (0x%p)->%s(\"%s\", %d, \"%s\", 0x%p(\"***\"), \"%s\")",
            this,  __FUNCTION__, cszServerName, usPort, 
            cszUserName, cszPassword, pcszCurrentDatabase 
        ); 
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::_Close()
{
    int nResult = false;

    m_LastErrorCode = KGD_ERROR_UNKNOW;      
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    m_TableMap.clear();

    mysql_close(m_pMySQL);
    m_pMySQL = NULL;    

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::_LoadAllTables()
{
    int nResult  = false;
    int nRetCode = false;
    MYSQL_RES *pTableSet = NULL;
    MYSQL_ROW row = NULL;

    m_LastErrorCode = KGD_ERROR_UNKNOW;          
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    m_TableMap.clear();

    pTableSet = mysql_list_tables(m_pMySQL, NULL);
    KGD_MYSQL_ENGINE_PROCESS_ERROR(pTableSet, m_pMySQL, "mysql_list_tables()");
    
    // load information for each table
    while (true)
    {
        row = mysql_fetch_row(pTableSet);
        if (!row)
            break;

        ASSERT(row[0]);
        // ignore return error. because table maybe create by another way,
        // such as by MySQL.exe, so we just skip this table.
        nRetCode = _LoadTable(row[0]); 
    }

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (pTableSet) 
    {
        mysql_free_result(pTableSet);
        pTableSet = NULL;    
    }
    if (!nResult)
    {
        // log param 
        QLogPrintf(LOG_DEBUG, "[KGD] (0x%p)->%s()", __FUNCTION__, this);
    } 
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::_LoadTable(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE]
)
{
    int nResult  = false;
    int nRetCode = false;
    unsigned i = 0;
    MYSQL_RES *pFieldSet = NULL;
    const MYSQL_FIELD *pcFields = NULL;
    unsigned uFieldCount = 0;
    KGD_MySQL_Field MySQLField;
    KGD_MySQL_TableMap::iterator itMySQLTable = m_TableMap.end();
    pair<KGD_MySQL_TableMap::iterator, bool> pairMySQLTable;
    pair<KGD_MySQL_FieldMap::iterator, bool> pairMySQLField;

    m_LastErrorCode = KGD_ERROR_UNKNOW;   
    QDB_PROCESS_ERROR(m_pMySQL, KGD_ERROR_CAN_NOT_CALL_THIS_NOW);

    QDB_PROCESS_ERROR(cszTableName, KGD_ERROR_NULL_POINTER);
    nRetCode = (int)strlen(cszTableName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    // try remove loaded table to do reload
    m_TableMap.erase(cszTableName); 
    pairMySQLTable = m_TableMap.insert(make_pair(cszTableName, KGD_MYSQL_TABLE()));
    ASSERT(pairMySQLTable.second);
    ASSERT(pairMySQLTable.first != m_TableMap.end());
    itMySQLTable = pairMySQLTable.first;

    // name
    strncpy(itMySQLTable->second.szName, cszTableName, KGD_NAME_BUFFER_MAX_SIZE);
    itMySQLTable->second.szName[KGD_NAME_BUFFER_MAX_SIZE - 1] = '\0';
    // field list
    pFieldSet = mysql_list_fields(m_pMySQL, cszTableName, NULL);
    KGD_MYSQL_ENGINE_PROCESS_ERROR(pFieldSet, m_pMySQL, "mysql_list_fields()");

    uFieldCount = mysql_num_fields(pFieldSet);
    pcFields = mysql_fetch_fields(pFieldSet);

    for (i = 0; i < uFieldCount; ++i)
    {
        ASSERT(pcFields);
        
        nRetCode = MySQLField.SetFieldInfo(*pcFields, m_pMySQL);
        QDB_PROCESS_ERROR(nRetCode, KGD_ERROR_NOT_SUPPORT);
 
        pairMySQLField = itMySQLTable->second.FieldMap.insert(
            make_pair(pcFields->name, MySQLField)
        );
        ASSERT(pairMySQLField.second);

        ++pcFields;
    }

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (pFieldSet) 
    {
        mysql_free_result(pFieldSet);
        pFieldSet = NULL;    
    }
    if (!nResult)
    {
        if (itMySQLTable != m_TableMap.end())
        {
            m_TableMap.erase(itMySQLTable);
            itMySQLTable = m_TableMap.end();
        }
        // log param 
        QLogPrintf(
            LOG_DEBUG, "[KGD] (0x%p)->%s(\"%s\")", this, __FUNCTION__, cszTableName
        );
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::_MapParam(
    const char cszTableName[KGD_NAME_BUFFER_MAX_SIZE],

    unsigned uFieldCount, 
    const KGD_FIELD *pFields,
    KGD_MySQL_FieldPairVector *paFieldPair,

    const KGD_FIELD *pcMatchField1, 
    KGD_MYSQL_FIELD_PAIR *pMatchFieldPair1,

    const KGD_FIELD *pcMatchField2, 
    KGD_MYSQL_FIELD_PAIR *pMatchFieldPair2
)
{
    int nResult = false;
    int nRetCode = false;
    unsigned i = 0;
    KGD_MySQL_TableMap::const_iterator citTable;
    KGD_MySQL_FieldMap::const_iterator citField;
    KGD_MYSQL_FIELD_PAIR FieldPair;
    const KGD_MySQL_FieldMap *pcFieldMap = NULL;

    m_LastErrorCode = KGD_ERROR_UNKNOW;

    ASSERT(cszTableName);

    // --------------------------  search table --------------------------
    citTable = m_TableMap.find(cszTableName);
    QDB_PROCESS_ERROR(citTable != m_TableMap.end(), KGD_ERROR_TABLE_NOT_EXIST);
    pcFieldMap = &(citTable->second.FieldMap);
    // ---------------  get field list -------------- 
    if (uFieldCount > 0)
    {
        QDB_PROCESS_ERROR(uFieldCount <= KGD_MAX_FIELD_COUNT, KGD_ERROR_TOO_MANY_FIELD);
        QDB_PROCESS_ERROR(pFields, KGD_ERROR_NULL_POINTER);
        QDB_PROCESS_ERROR(paFieldPair, KGD_ERROR_NULL_POINTER);

        paFieldPair->clear();
        paFieldPair->reserve(uFieldCount);
        for (i = 0; i < uFieldCount; ++i)
        {
            // search field in table
            citField = pcFieldMap->find(pFields[i].szName);
            QDB_PROCESS_ERROR(
                citField != pcFieldMap->end(), KGD_ERROR_FIELD_NOT_EXIST
            );
            FieldPair.pcKGDField   = &pFields[i];
            FieldPair.pcMySQLField = &(citField->second);
            paFieldPair->push_back(FieldPair);
        }
    }
    // --------------- match field 1--------------------------------------
    if (pMatchFieldPair1) 
    {
        pMatchFieldPair1->pcKGDField   = pcMatchField1;
        if (pcMatchField1)
        {
            citField = pcFieldMap->find(pcMatchField1->szName);
            QDB_PROCESS_ERROR(
                citField != pcFieldMap->end(), KGD_ERROR_FIELD_NOT_EXIST
            );
            pMatchFieldPair1->pcMySQLField = &(citField->second);
        }
        else
        {
            pMatchFieldPair1->pcMySQLField = NULL;
        }
    }
    // --------------- match field 2--------------------------------------
    if (pMatchFieldPair2) 
    { 
        pMatchFieldPair2->pcKGDField   = pcMatchField2;
        if (pcMatchField2)
        {
            citField = pcFieldMap->find(pcMatchField2->szName);
            QDB_PROCESS_ERROR(
                citField != pcFieldMap->end(), KGD_ERROR_FIELD_NOT_EXIST
            );
            pMatchFieldPair2->pcMySQLField = &(citField->second);
        }
        else
        {
            pMatchFieldPair2->pcMySQLField = NULL;
        }
    } 
//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult) 
    {
        if (paFieldPair)
        {
            paFieldPair->clear();
        }
        // log param 
        char szLogBuf[1024];
        nRetCode = snprintf(
            szLogBuf, sizeof(szLogBuf) - 1,
            "[KGD] (0x%p)->%s(\"%s\", %u, 0x%p, 0x%p, 0x%p, 0x%p, 0x%p, 0x%p),",
            this, __FUNCTION__, cszTableName, uFieldCount, pFields, paFieldPair, 
            pcMatchField1, pMatchFieldPair1, pcMatchField2, pMatchFieldPair2 
        );
        szLogBuf[sizeof(szLogBuf) - 1] = '\0';
        if (nRetCode > 0)
        {
            KGD_DumpKGDFields(
                (sizeof(szLogBuf) - nRetCode), (szLogBuf + nRetCode),
                uFieldCount, pFields, pcMatchField1, pcMatchField2
            );
        }
        QLogPrintf(LOG_DEBUG, "%s", szLogBuf);
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
IMemBlock *KGD_MySQL_Database::_CreateBuffer(
    const KGD_MySQL_FieldPairVector *pcaFieldPair,
    const KGD_MYSQL_FIELD_PAIR *pcMatchFieldPair1,
    KGD_OPERATOR Operator1,
    const KGD_MYSQL_FIELD_PAIR *pcMatchFieldPair2,
    KGD_OPERATOR Operator2,
    unsigned uAppendSize)
{
    int nResult = false;
    IMemBlock *piBuffer = NULL;
    unsigned uBufSize = 0;
    unsigned uFieldBufNeed = 0;
    unsigned i = 0;

    m_LastErrorCode = KGD_ERROR_UNKNOW;

    // count field  
    if (pcaFieldPair)
    {
        for (i = 0; i < pcaFieldPair->size(); ++i)
        {
            uFieldBufNeed = (*pcaFieldPair)[i].pcMySQLField->GetStoreBufferSize(
                *((*pcaFieldPair)[i].pcKGDField)
            );
            QDB_PROCESS_ERROR(uFieldBufNeed, KGD_ERROR_DATA_TOO_LONG);

            // append ',' for separate field, append '=' for format of "name = value,"
            uBufSize += uFieldBufNeed + sizeof("=,");
        }
    }

    uFieldBufNeed += sizeof(" WHERE  BETWEEN  AND ") ;
    // count match condition
    if (
        (pcMatchFieldPair1) &&
        (pcMatchFieldPair1->pcKGDField)
    )
    {

        QDB_PROCESS_ERROR(Operator1 > KGD_OPERATOR_NONE, KGD_ERROR_INVALID_OPERATOR);
        QDB_PROCESS_ERROR(Operator1 < OPERATOR_ARRAY_SIZE, KGD_ERROR_INVALID_OPERATOR);
        uBufSize += gs_caOperator[Operator1].uSize + sizeof("  ");

        ASSERT(pcMatchFieldPair1->pcMySQLField);
        uFieldBufNeed = pcMatchFieldPair1->pcMySQLField->GetStoreBufferSize(
            *(pcMatchFieldPair1->pcKGDField)
        );
        QDB_PROCESS_ERROR(uFieldBufNeed, KGD_ERROR_DATA_TOO_LONG);

        uBufSize += uFieldBufNeed;    
    }

    if (
        (pcMatchFieldPair2) &&
        (pcMatchFieldPair2->pcKGDField)
    )
    {
        QDB_PROCESS_ERROR(Operator2 > KGD_OPERATOR_NONE, KGD_ERROR_INVALID_OPERATOR);
        QDB_PROCESS_ERROR(Operator2 < OPERATOR_ARRAY_SIZE, KGD_ERROR_INVALID_OPERATOR);
        uBufSize += gs_caOperator[Operator2].uSize + sizeof("  ");

        ASSERT(pcMatchFieldPair2->pcMySQLField);
        uFieldBufNeed = pcMatchFieldPair2->pcMySQLField->GetStoreBufferSize(
            *(pcMatchFieldPair2->pcKGDField)
        );
        QDB_PROCESS_ERROR(uFieldBufNeed, KGD_ERROR_DATA_TOO_LONG); 

        uBufSize += uFieldBufNeed;    
    }
    // append size 
    uBufSize += uAppendSize;

    piBuffer = QCreateMemBlock(uBufSize); 
    QDB_PROCESS_ERROR(piBuffer, KGD_ERROR_NOT_ENOUGH_MEMORY);
//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = true;
EXIT0:
    if (!nResult)
    {
        // log param 
        QLogPrintf(
            LOG_DEBUG,
            "[KGD] (0x%p)->%s(0x%p, 0x%p, %d, 0x%p, %d, %u),",
            this, __FUNCTION__, pcaFieldPair, 
            pcMatchFieldPair1, Operator1, pcMatchFieldPair2, Operator2, 
            uAppendSize 
        );
    }
    return piBuffer;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::_StoreWhereCondition(
    char *pszWritePos,
    const KGD_MYSQL_FIELD_PAIR &crMatchFieldPair1,
    KGD_OPERATOR Operator1,
    const KGD_MYSQL_FIELD_PAIR &crMatchFieldPair2,
    KGD_OPERATOR Operator2
)
{
    int nResult  = -1;
    int nRetCode = false;
    char *pszStartPos = pszWritePos;

    m_LastErrorCode = KGD_ERROR_UNKNOW;

    ASSERT(pszWritePos);
    if (crMatchFieldPair2.pcKGDField)
    {
        // if specify condition 2, must specify condition1 too.
        QDB_PROCESS_ERROR(crMatchFieldPair1.pcKGDField, KGD_ERROR_NULL_POINTER);
    };
    PROCESS_SUCCESS(!crMatchFieldPair1.pcKGDField);
    ASSERT(crMatchFieldPair1.pcMySQLField);

    // -------------------- match condition 1 --------------------
    memcpy(pszWritePos, " WHERE ", sizeof(" WHERE ") - 1);
    pszWritePos += sizeof(" WHERE ") - 1;

    // -------------------- if can , optimize to BETWEEN .. AND ..-------------------- 
    if (
        (crMatchFieldPair1.pcMySQLField == crMatchFieldPair2.pcMySQLField) &&
        (
            (
                (Operator1 + Operator2) == 
                (KGD_OPERATOR_LESS_EQUAL + KGD_OPERATOR_ABOVE_EQUAL) 
            )&& 
            (
                (Operator1 == KGD_OPERATOR_ABOVE_EQUAL) || 
                (Operator2 == KGD_OPERATOR_ABOVE_EQUAL)
            )
        )
    )
    {
        // name
        nRetCode = crMatchFieldPair1.pcMySQLField->StoreNameToBuffer(pszWritePos);
        QDB_PROCESS_ERROR(nRetCode != -1, KGD_ERROR_UNKNOW);
        pszWritePos += nRetCode - 1;

        memcpy(pszWritePos, " BETWEEN ", sizeof(" BETWEEN ") - 1);
        pszWritePos += sizeof(" BETWEEN ") - 1;
        // value 1
        nRetCode = crMatchFieldPair1.pcMySQLField->StoreDataToBuffer(
            pszWritePos, *(crMatchFieldPair1.pcKGDField)
        );
        QDB_PROCESS_ERROR(nRetCode != -1, KGD_ERROR_UNKNOW);
        pszWritePos += nRetCode - 1;

        memcpy(pszWritePos, " AND ", sizeof(" AND ") - 1);
        pszWritePos += sizeof(" AND ") - 1;

        // value 2
        ASSERT(crMatchFieldPair2.pcKGDField);
        nRetCode = crMatchFieldPair2.pcMySQLField->StoreDataToBuffer(
            pszWritePos, *(crMatchFieldPair2.pcKGDField)
        );
        QDB_PROCESS_ERROR(nRetCode != -1, KGD_ERROR_UNKNOW);
        pszWritePos += nRetCode - 1;
        PROCESS_SUCCESS(true);
    };

    // -------------------- can't optimize, normal condition -------------------- 
    // match condition 1
    nRetCode = _StoreSingleCondition(
        pszWritePos, crMatchFieldPair1, Operator1
    );
    PROCESS_ERROR(nRetCode != -1);
    pszWritePos += nRetCode - 1;

    //  match condition 2 
    if (crMatchFieldPair2.pcKGDField)
    {
        ASSERT(crMatchFieldPair2.pcMySQLField);

        memcpy(pszWritePos, " AND ", sizeof(" AND ") - 1);
        pszWritePos += sizeof(" AND ") - 1;

        nRetCode = _StoreSingleCondition(
            pszWritePos, crMatchFieldPair2, Operator2
        );
        PROCESS_ERROR(nRetCode != -1);
        pszWritePos += nRetCode - 1;
    }
EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = (int)(pszWritePos - pszStartPos + 1); // plus '\0'
EXIT0:
    if (nResult == -1)
    {
        // log param 
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] (0x%p)->%s(0x%p, {0x%p, 0x%p}, %d, {0x%p, 0x%p}, %d)",
            this, __FUNCTION__, pszWritePos, 
            crMatchFieldPair1.pcMySQLField, crMatchFieldPair1.pcKGDField, 
            Operator1,
            crMatchFieldPair2.pcMySQLField, crMatchFieldPair2.pcKGDField, 
            Operator2
        ); 
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::_StoreSingleCondition(
    char *pszWritePos,
    const KGD_MYSQL_FIELD_PAIR &crMatchFieldPair,
    KGD_OPERATOR Operator
)
{
    int nResult  = -1;
    int nRetCode = false;
    char *pszStartPos = pszWritePos;
    
    m_LastErrorCode = KGD_ERROR_UNKNOW;
    ASSERT(pszWritePos);

    // name
    ASSERT(crMatchFieldPair.pcMySQLField);
    nRetCode = crMatchFieldPair.pcMySQLField->StoreNameToBuffer(pszWritePos);
    QDB_PROCESS_ERROR(nRetCode != -1, KGD_ERROR_UNKNOW);
    pszWritePos += nRetCode - 1;
    *pszWritePos++ = ' ';
    
    // operator
    QDB_PROCESS_ERROR(Operator > KGD_OPERATOR_NONE, KGD_ERROR_INVALID_OPERATOR);
    QDB_PROCESS_ERROR(Operator < OPERATOR_ARRAY_SIZE, KGD_ERROR_INVALID_OPERATOR);
    memcpy(
        pszWritePos, 
        gs_caOperator[Operator].pcszStr, 
        gs_caOperator[Operator].uSize - 1
    );
    pszWritePos += gs_caOperator[Operator].uSize - 1;
    *pszWritePos++ = ' ';
    
    // value
    ASSERT(crMatchFieldPair.pcKGDField);
    nRetCode = crMatchFieldPair.pcMySQLField->StoreDataToBuffer(
        pszWritePos, *(crMatchFieldPair.pcKGDField)
    );
    QDB_PROCESS_ERROR(nRetCode != -1, KGD_ERROR_UNKNOW);
    pszWritePos += nRetCode - 1;

//EXIT1:
    nResult = (int)(pszWritePos - pszStartPos + 1); // plus '\0'
EXIT0:
    if (nResult == -1)
    {
        // log param 
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] (0x%p)->%s(0x%p, {0x%p, 0x%p}, %d)",
            this, __FUNCTION__, pszWritePos, 
            crMatchFieldPair.pcMySQLField, crMatchFieldPair.pcKGDField, 
            Operator
        ); 
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::_StoreFieldDefiniens(
    char *pszWritePos,
    const KGD_FIELD &crField 
)
{
    int nResult  = -1;
    int nRetCode = false;
    char *pszStartPos = pszWritePos;

    m_LastErrorCode = KGD_ERROR_UNKNOW;
    ASSERT(pszWritePos);

    // ------ field name ------ 
    nRetCode = (int)strlen(crField.szName);
    QDB_PROCESS_ERROR(nRetCode > 0, KGD_ERROR_EMPTY_STRING);
    QDB_PROCESS_ERROR(nRetCode < KGD_NAME_BUFFER_MAX_SIZE, KGD_ERROR_NAME_TOO_LONG);

    memcpy(pszWritePos, crField.szName, nRetCode + 1);
    pszWritePos += nRetCode;

    // ------  type ------ 
    QDB_PROCESS_ERROR(crField.Type >= KGD_TYPE_INT32, KGD_ERROR_INVALID_TYPE);
    QDB_PROCESS_ERROR(crField.Type < TYPE_NAME_ARRAY_SIZE, KGD_ERROR_INVALID_TYPE);
    *pszWritePos++ = ' ';
    memcpy(
        pszWritePos, 
        gs_caTypeName[crField.Type].pcszStr, 
        gs_caTypeName[crField.Type].uSize
    ); 
    pszWritePos += gs_caTypeName[crField.Type].uSize - 1;   //exclude '\0'
    
    // ------  size ------ 
    switch (crField.Type)
    {
    case KGD_TYPE_INT32:
        QDB_PROCESS_ERROR(
            crField.uMaxDataSize == KGD_INT32_SIZE,
            KGD_ERROR_MAX_DATA_TOO_LONG
        );
        break;
    case KGD_TYPE_UNSIGNED_INT32:
        QDB_PROCESS_ERROR(
            crField.uMaxDataSize == KGD_UNSIGNED_INT32_SIZE,
            KGD_ERROR_MAX_DATA_TOO_LONG
        );
        break;
    case KGD_TYPE_INT64:
        QDB_PROCESS_ERROR(
            crField.uMaxDataSize == KGD_INT64_SIZE,
            KGD_ERROR_MAX_DATA_TOO_LONG
        );
        break;
    case KGD_TYPE_UNSIGNED_INT64:
        QDB_PROCESS_ERROR(
            crField.uMaxDataSize == KGD_UNSIGNED_INT64_SIZE,
            KGD_ERROR_MAX_DATA_TOO_LONG
        );
        break;
    case KGD_TYPE_CHAR:
    case KGD_TYPE_VARCHAR:
    //case KGD_TYPE_BINARY:
    //case KGD_TYPE_VARBINARY:
        QDB_PROCESS_ERROR(
            crField.uMaxDataSize <= KGD_TINY_FIELD_MAX_DATA_SIZE, 
            KGD_ERROR_MAX_DATA_TOO_LONG
        );
        nRetCode = sprintf(pszWritePos, "(%u)", crField.uMaxDataSize);
        pszWritePos += nRetCode;
        break;
    case KGD_TYPE_TEXT:
    case KGD_TYPE_BLOB:
        QDB_PROCESS_ERROR(
            crField.uMaxDataSize <= KGD_MEDIUM_FIELD_MAX_DATA_SIZE, 
            KGD_ERROR_MAX_DATA_TOO_LONG
        );
        break;
    case KGD_TYPE_DATETIME:
        QDB_PROCESS_ERROR(
            crField.uMaxDataSize == KGD_DATETIME_SIZE,
            KGD_ERROR_MAX_DATA_TOO_LONG
        );
        break;
    default:
        ASSERT(false);
        PROCESS_ERROR(false);
    }

//EXIT1:
    m_LastErrorCode = KGD_ERROR_SUCCESS;
    nResult = (int)(pszWritePos - pszStartPos + 1); // plus '\0'
EXIT0:
    if (nResult == -1)
    {
        // log param 
        char szLogBuf[256];
        nRetCode = snprintf(
            szLogBuf, sizeof(szLogBuf) - 1,
            "[KGD] (0x%p)->%s(0x%p, *(0x%p), ",
            this, __FUNCTION__, pszWritePos, &crField
        );
        szLogBuf[sizeof(szLogBuf) - 1] = '\0';
        if (nRetCode > 0)
        {
            KGD_DumpKGDFields(
                (sizeof(szLogBuf) - nRetCode), (szLogBuf + nRetCode),
                0, NULL, &crField, NULL
            );
        }
        QLogPrintf(LOG_DEBUG, "%s", szLogBuf);
    }
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
// @brief : send query to server, if connect lost, reconnect.
// @return : true if successful, false if failed.
int KGD_MySQL_Database::_DoQuey(const char cszQuery[], unsigned long ulLength)
{
    int nResult  = false;
    int nRetCode = false;
    int nReconnectTimes = 0;
    unsigned uMySQLErrorCode = 0;

    ASSERT(cszQuery);

    while (true) 
    {
        nRetCode = mysql_real_query(m_pMySQL, cszQuery, ulLength);
        if (nRetCode == 0)
            break; // success
        uMySQLErrorCode = mysql_errno(m_pMySQL);
        if (
            (uMySQLErrorCode != CR_SERVER_LOST) &&
            (uMySQLErrorCode != CR_CONN_HOST_ERROR) &&
            (uMySQLErrorCode != CR_COMMANDS_OUT_OF_SYNC) &&
            (uMySQLErrorCode != CR_SERVER_GONE_ERROR)
        ) 
            break; // false
        
        QLogPrintf(
            LOG_INFO, 
            "[KGD] (0x%p)->%s : Connection lost, try reconnect.",
            this, __FUNCTION__
        );
        // Checks whether the connection to the server is working. 
        // If the connection has gone down, an automatic reconnection is attempted
        while (nReconnectTimes <= 3)
        { 
            nRetCode = mysql_ping(m_pMySQL);
            if (nRetCode == 0)
                break;
            ++nReconnectTimes;
        }
        QDB_PROCESS_ERROR(nRetCode == 0, KGD_ERROR_CONNECT_LOST);
    }
    KGD_MYSQL_ENGINE_PROCESS_ERROR(nRetCode == 0, m_pMySQL, cszQuery);

//EXIT1:
    nResult = true;
EXIT0:
    return nResult; 
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::_GetServerVariable(
    const char cszVariablesName[], unsigned uRetBufferSize, char *pszRetBuffer
)
{
    int nResult  = false;
    int nRetCode = false;
    char szQuery[256];
    MYSQL_RES *pResultSet = NULL;
    MYSQL_ROW row = NULL;
    unsigned uFieldCount = 0;

    ASSERT(cszVariablesName);
    ASSERT(uRetBufferSize > 1);
    ASSERT(pszRetBuffer);

    nRetCode = snprintf(
        szQuery, sizeof(szQuery) - 1, "SHOW VARIABLES LIKE \'%s\'", cszVariablesName
    );
    LOG_PROCESS_ERROR(nRetCode > 0);
    szQuery[sizeof(szQuery) - 1] = '\0';
   
    nRetCode = _DoQuey(szQuery, nRetCode);
    PROCESS_ERROR(nRetCode);

    pResultSet = mysql_store_result(m_pMySQL);
    KGD_MYSQL_ENGINE_PROCESS_ERROR(pResultSet, m_pMySQL, "mysql_store_result()");

    uFieldCount = mysql_num_fields(pResultSet);    
    KGD_MYSQL_ENGINE_PROCESS_ERROR(uFieldCount == 2, m_pMySQL, "mysql_num_fields()");

    row = mysql_fetch_row(pResultSet);
    KGD_MYSQL_ENGINE_PROCESS_ERROR(row, m_pMySQL, "mysql_fetch_row()");

    ASSERT(row[1]);
    
    strncpy(pszRetBuffer, row[1], uRetBufferSize - 1);
    pszRetBuffer[uRetBufferSize - 1]  = '\0';

//EXIT1:
    nResult = true;
EXIT0:
    if (pResultSet)
    {
        mysql_free_result(pResultSet);
        pResultSet = NULL;
    }
    if (!nResult)
    {
        // log param 
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] (0x%p)->%s(\"%s\", %u, 0x%p)", 
            this, __FUNCTION__, cszVariablesName, uRetBufferSize, pszRetBuffer
        );
    } 
    return nResult;
}

////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Database::_CheckDatabaseCharacterSet()
{
    static const char s_cszCharacterSetDatabase[] = "character_set_database";
    int nResult  = false;
    int nRetCode = false;
    char szDatabaseCharacterSet[32];

    nRetCode = _GetServerVariable(
        s_cszCharacterSetDatabase, 
        sizeof(szDatabaseCharacterSet), 
        szDatabaseCharacterSet
    );
    PROCESS_ERROR(nRetCode);
    
    nRetCode = strcmp(szDatabaseCharacterSet, m_szCharacterSet);
    if (nRetCode != 0)
    {
        QLogPrintf(
            LOG_DEBUG, 
            "[KGD] MySQL Server variables: character_set_database=%s, "
            "character_set_server = %s",
            szDatabaseCharacterSet, m_szCharacterSet
        );
    }
    KGD_MYSQL_ENGINE_PROCESS_ERROR(
        nRetCode == 0, 
        m_pMySQL, 
        "Database character set not equal server character set."
    ); 
//EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Init(void *pvContext)
{
    int nResult  = false;
    int nRetCode = false;

    USE_ARGUMENT(pvContext); 

    nRetCode = QMemoryInit(NULL);
    LOG_PROCESS_ERROR(nRetCode > 0);

//EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

////////////////////////////////////////////////////////////////////////////////
int KGD_MySQL_Uninit(void *pvContext)
{
    int nResult  = false;
    int nRetCode = false;

    USE_ARGUMENT(pvContext);

    nRetCode = QMemoryUnInit(NULL);
    LOG_PROCESS_ERROR(nRetCode >= 0);
//EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}
