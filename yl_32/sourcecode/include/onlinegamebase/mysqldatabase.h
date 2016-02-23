
#pragma once

#include "mysql.h"

#define KD_DBNAME_MAX_SIZE	64	//表名和字段名的最大长度

CONST static unsigned KD_TINY_FIELD_MAX_DATA_SIZE     = 255;

// max size of type KGD_TYPE_TEXT, KGD_TYPE_BLOB
CONST static unsigned KD_MEDIUM_FIELD_MAX_DATA_SIZE   = 65535; 

CONST static unsigned KD_BLOB_OR_TEXT_INDEX_PREFIX_MAX_NUM = 255;

CONST static unsigned KD_MEDIUM_BLOB_MAX_SIZE = 16777215;

enum KE_DBCREATE_FLAG
{
	emKDBCREATE_IF_NOT_EXIST = 1,
};
enum KE_TABLE_CREATE_FLAG
{
	emKTABLE_CREATE_IF_NOT_EXIST = 1,
};
enum KE_GETRESULT_METHOD
{
	emKGET_RESULT_STORE = 1,
};
/***************************** Use for getting query result *******************************/
class KMySqlRow
{
public:
	KMySqlRow(MYSQL_RES *pMySQLRes)
	{
		//m_nFieldCount = mysql_num_fields(pMySQLRes);
		aFieldData = mysql_fetch_row(pMySQLRes);
		aFieldLenth = mysql_fetch_lengths(pMySQLRes);
	}
	BOOL IsValid() { return aFieldData != NULL; }
	MYSQL_ROW	aFieldData;
	//注意string的Lenth不包括'\0'
	ULONG * aFieldLenth;
	//INT m_nFieldCount;
};

class KMySqlResult
{
public:
	KMySqlResult() { m_pMySQLRes = NULL; }
	KMySqlResult(MYSQL_RES *pMySQLRes) : m_pMySQLRes(pMySQLRes) {}
	INT GetRowCount() { return (INT)mysql_num_rows(m_pMySQLRes); }
	INT GetFieldCount() { return mysql_num_fields(m_pMySQLRes); }
	// KMYSQL_FIELD GetFieldsDesc() { return mysql_fetch_fields(m_pMySQLRes); }
	KMySqlRow GetNextRow() { return KMySqlRow(m_pMySQLRes); }
	operator MYSQL_RES *() { return m_pMySQLRes; }
	BOOL IsValid() { return m_pMySQLRes != NULL; }
	VOID Release() { if (m_pMySQLRes) mysql_free_result(m_pMySQLRes); m_pMySQLRes = NULL; }
protected:
	MYSQL_RES *m_pMySQLRes;
};

// 自动释放
class KMyqlResultProtected : public KMySqlResult
{
public:
	KMyqlResultProtected(const KMySqlResult& rs) : KMySqlResult(rs) {}
	~KMyqlResultProtected()
	{
		Release();
	}
};

/***************************** Use for make query string *******************************/
enum KE_FIELD_FLAG	//枚举值必须可组合
{
	emK_FIELD_FLAG_NORMAL    = 0,  // normal field
	emK_FIELD_FLAG_NOTNULL   = 1,  // not null
	// = 2
	// = 4
	// = 8
};
//若是数值型，Query时返回的数据会被自动转为字符串！！！
//建议不要使用什么数值型，用TINYBLOB好了
enum KE_DBFIELD_TYPE
{
	emK_FIELDTYPE_INT32          = 0,     // 32 bit integer,  size = 4
	emK_FIELDTYPE_UNSIGNED_INT32 = 1,    // 32 bit unsigned integer, size = 4
	emK_FIELDTYPE_INT64          = 2,    // 64 bit integer,  size = 8
	emK_FIELDTYPE_UNSIGNED_INT64 = 3,    // 64 bit unsigned integer, size = 8
	emK_FIELDTYPE_CHAR           = 4,    // max size <= KD_TINY_FIELD_MAX_DATA_SIZE
	emK_FIELDTYPE_VARCHAR        = 5,    // max size <= KD_TINY_FIELD_MAX_DATA_SIZE
	emK_FIELDTYPE_TEXT           = 6,    // max size <= KD_MEDIUM_FIELD_MAX_DATA_SIZE
	//KGD_FIELDTYPE_BINARY         = 7,    // max size <= KD_TINY_FIELD_MAX_DATA_SIZE
	//KGD_FIELDTYPE_VARBINARY      = 8,    // max size <= KD_TINY_FIELD_MAX_DATA_SIZE
	emK_FIELDTYPE_BLOB           = 9,    // max size <= KD_MEDIUM_FIELD_MAX_DATA_SIZE
	emK_FIELDTYPE_TINYTEXT		= 10,    // max size <= KD_TINY_FIELD_MAX_DATA_SIZE
	emK_FIELDTYPE_TINYBLOB		= 11,    // max size <= KD_TINY_FIELD_MAX_DATA_SIZE
	emK_FIELDTYPE_DATETIME		= 12, // 数据可为time_t，也可为字符串，字符串要填写长度不包含结尾符
	emK_FIELDTYPE_TIMESTAMP		= 13, // 时间截
	emK_FIELDTYPE_AUTOINCRE		= 14, // 自增字段
	emK_FIELDTYPE_MEDIUMBLOB	= 15, // max size <= KD_MEDIUM_BLOB_MAX_SIZE
	emK_FIELDTYPE_SMALLINT		= 16, // max size <= 2
	emK_FIELDTYPE_TINYINT		= 17, // max size <= 1
};
enum KE_DBSQL_CONDITION
{
	emK_DBSQL_WHERE	= 0,	//WHERE
	emK_DBSQL_AND = 1,		//AND
	emK_DBSQL_OR = 2,		//OR
};
enum KE_DBSQL_OPERATOR
{
	emKOPERATOR_NONE        = 0,   // none operator
	emKOPERATOR_LESS        = 1,   //  .. <  .. 
	emKOPERATOR_LESS_EQUAL  = 2,   //  .. <= .. 
	emKOPERATOR_EQUAL       = 3,   //  .. =  .. 
	emKOPERATOR_NOT_EQUAL   = 4,   //  .. != .. 
	emKOPERATOR_ABOVE       = 5,   //  .. >  .. 
	emKOPERATOR_ABOVE_EQUAL = 6,    //  .. >= .. 
};
enum KE_DBSQL_MAIN_STAMENT
{
	emKSQL_STATEMENT_NONE,
	emKSQL_STATEMENT_CREATETABLE,
	emKSQL_STATEMENT_DROPTABLE,
	emKSQL_STATEMENT_SELECT,
	emKSQL_STATEMENT_INSERT,
	emKSQL_STATEMENT_REPLACE,
	emKSQL_STATEMENT_DELETE,
	emKSQL_STATEMENT_UPDATE,
};
enum KE_FIELD_INDEX_TYPE
{
	emKFIELD_INDEX_COMMON,	//INDEX
	emKFIELD_INDEX_UNIQUE,	//UNIQUE
	emKFIELD_INDEX_PRIMARY,	//PRIMARY KEY
};
#define KD_INT64_SIZE	64
#define KD_MAX_INDEX_SIZE	255	//对索引列最多索引前面多少个字节

struct KDBFieldDesc
{
	LPCSTR szFieldName;
	KE_DBFIELD_TYPE eType;
	INT nFlag;	//Combination of KE_FIELD_FLAG
	UINT uMaxSize;	//字符串类型长度或数值类型上限，为0表示不指定
	LPVOID pvData;			//若数据为字符串未尾需包含0结尾符
	UINT uDataSize;	//如是字符串类型长度包含结尾符
};
struct KDBTableDesc
{
	LPCSTR szTableName;
	INT nFieldNum;
	KDBFieldDesc *aFieldDesc;
};
struct KDBIndexDesc
{
	LPCSTR szFieldName;	//Index FieldName
	INT nIndexSize;		//Index Size, Less then KD_MAX_INDEX_SIZE, if not specify size，make it Zero
	KE_FIELD_INDEX_TYPE eType;	//Index Type, if Multi Index, Ingore
};
//该类为一些常用的sql语句提供帮助
class KGC_MySqlQueryStream
{
public:
	//Confirm that pWriteBuf Enough to Write
	KGC_MySqlQueryStream(CHAR *pWriteBuf, MYSQL *pMySQL) : 
	  m_pBuffer(pWriteBuf), m_pMySQL(pMySQL) { Reset();  }
	//加入CREATE TABLE ...语句
	//nCreteFlag : Combination of KE_TABLE_CREATE_FLAG
	BOOL AddCreateTable(INT nCreteFlag, KDBTableDesc &desc);

	//加入DROP TABLE ...语句
	BOOL AddRemoveTable(LPCSTR szTableName);

	//加入单列索引语句（须加在CREATE TABLE语句后）	INDEX(fieldname[(indexsize)])
	//nDescCount:单列索引数量
	//aIndexDesc:单列索引数组
	BOOL AddCreateTableSingleFieldIndex(INT nDescCount, KDBIndexDesc aIndexDesc[]);

	//加入多列索引语句（须加在CREATE TABLE语句后）	INDEX(fieldname1[(indexsize)], fieldname2[(indexsize)], ...)
	//nFieldCount:多列索引字段数
	//aIndexDesc:索引描述数组
	BOOL AddCreateTableMultiFieldIndex(KE_FIELD_INDEX_TYPE eType, INT nFieldCount, KDBIndexDesc aIndexDesc[]);

	//加入SELECT FIELD1, FIELD2, ... FROM TABLE语句
	//nFieldCount:How Much Fields to Query
	//aFieldDesc:FieldDesc, must set: szFieldName
	//szTableName:Which Table to Select
	BOOL AddSelectQuery(INT nFieldCount, KDBFieldDesc aFieldDesc[], LPCSTR szTableName);

	//加入INSERT INTO TABLE(FIELD1, FIELD2, ...)VALUES(VALUE1,VALUE2, ...)语句
	//nFieldCount:How Much Fields to Select
	//aFieldDesc:FieldDesc, must set: szFieldName, KE_DBFIELD_TYPE, pvData, uDataSize(TEXT,BLOB,TINYBLOB需要)
	//szTableName:Which Table to Insert
	BOOL AddInsertRecord(INT nFieldCount, KDBFieldDesc aFieldDesc[], LPCSTR szTableName, BOOL bInsertIgnore = FALSE);

	//加入REPLACE INTO TABLE(FIELD1, FIELD2, ...)VALUES(VALUE1,VALUE2, ...)语句
	//nFieldCount:How Much Fields to Select
	//aFieldDesc:FieldDesc, must set: szFieldName, KE_DBFIELD_TYPE, pvData, uDataSize(TEXT,BLOB,TINYBLOB需要)
	//szTableName:Which Table to Replace
	BOOL AddReplaceRecord(INT nFieldCount, KDBFieldDesc aFieldDesc[], LPCSTR szTableName);

	//加入DELETE FROM TABLE语句
	BOOL AddDeleteRecord(LPCSTR szTableName);

	//加入UPDATE TABLE SET FIELD1 = VALUE1, FIELD2 = VALUE2, ...语句
	//nFieldCount:How Much Fields to Update
	//aFieldDesc:FieldDesc, must set: szFieldName, KE_DBFIELD_TYPE, pvData, uDataSize(变长类型需要)
	//szTableName:Which Table to Update
	BOOL AddUpdateRecord(INT nFieldCount, KDBFieldDesc aFieldDesc[], LPCSTR szTableName);

	//加入查询定位语句 WHERE FIELD opt VALUE, AND FIELD opt VALUE 等...
	//FieldDesc: must set szFieldName, KE_DBFIELD_TYPE, pvData, uDataSize(变长类型需要)
	BOOL AddFieldCondition(KE_DBSQL_CONDITION condition, KE_DBSQL_OPERATOR opt, KDBFieldDesc &FieldDesc);

	//加入ON DUPLICATE KEY UPDATE FIELD1 = VALUE1, FIELD2 = VALUE2, ...
	//FieldDesc: must set szFieldName, KE_DBFIELD_TYPE, pvData, uDataSize(变长类型需要)
	BOOL AddDupliUpdate(INT nFieldCount, KDBFieldDesc aFieldDesc[]);
	LPCSTR GetStr();
	UINT GetStrSize() { return (UINT)(m_pCurWritePos - m_pBuffer); }
	CHAR *GetCurWritePos() { return m_pCurWritePos; }
	CHAR *AddStr(LPCSTR pszAdd);
	VOID Reset() { m_pCurWritePos = m_pBuffer; m_eCurStament = emKSQL_STATEMENT_NONE; }
private:
	BOOL _StoreFieldData(KDBFieldDesc &FieldDesc);
protected:
	CHAR *CONST m_pBuffer;	//Write Buffer, Provided by Extern Use
	CHAR *m_pCurWritePos;
	MYSQL *m_pMySQL;
	KE_DBSQL_MAIN_STAMENT m_eCurStament;
};

// -------------------------------------------------------------------------
class KMySqlDatabase
{
public:
	KMySqlDatabase();
	~KMySqlDatabase();
	BOOL ShutDown();
	////////////////////////////////////////////////////////////////////////////
	// @brief : Connect to database server
	// @param cszServerName       : [in] specify database server. 
	// @param usPort              : [in] If port is not 0, the value will be 
	//                                   used as the port number for the TCP/IP 
	//                                   connection. Note that the pcszHost
	//                                   parameter determines the type of 
	//                                   the connection. 
	// @param cszUserName         : [in] User's login ID. If user is NULL or the
	//                                   empty string "", current user assumed. 
	// @param cszPassword         : [in] specify password for User.
	// @param pcszCurrentDatabase : [in] set current database after connected.
	//                                   Can be NULL, if set to NULL, you should
	//                                   call IKGD_Database::SetCurrentDatabase
	//                                   before do anything relate to a table.
	// @return : true if successful, false if failed.
	BOOL DBConnect(
		CONST CHAR cszServerName[], 
		USHORT usPort, 
		CONST CHAR cszUserName[], 
		CONST CHAR cszPassword[],
		LPCSTR pcszCurrentDatabase,
		ULONG ulClientFlag = CLIENT_FOUND_ROWS
		);
	////////////////////////////////////////////////////////////////////////////
	// @brief : Create new database.
	// @param cszDatabaseName : Database's Name
	// @param nCreateFlag : see KE_DBCREATE_FLAG
	BOOL CreateDatabase(
		CONST CHAR cszDatabaseName[KD_DBNAME_MAX_SIZE], 
		INT nCreateFlag
		);
	////////////////////////////////////////////////////////////////////////////
	// @brief : Remove an exist database.
	// @remark :
	BOOL RemoveDatabase(
		CONST CHAR cszDatabaseName[KD_DBNAME_MAX_SIZE]
	);   
	////////////////////////////////////////////////////////////////////////////
	// @brief : Set current database,
	// @remark :
	BOOL SetCurrentDatabase(
		CONST CHAR cszDatabaseName[KD_DBNAME_MAX_SIZE]
	);
	////////////////////////////////////////////////////////////////////////////
	// @brief : Create QueryStream to Write Query Language
	// @param pBuf : Buffer to Write
	// @return : QueryStream Object
	KGC_MySqlQueryStream CreateQueryStream(CHAR *pWriteBuf);
	////////////////////////////////////////////////////////////////////////////
	// @brief : direct send query to server, if connect lost, reconnect.
	// @return : true if successful, false if failed.
	BOOL DoQuery(CONST CHAR cszQuery[], DWORD ulLength);
	//强烈建议使用KGC_MySqlQueryStream构造SQL语句，更方便安全及易于维护
	BOOL DoQuery(KGC_MySqlQueryStream &sStream);
	////////////////////////////////////////////////////////////////////////////
	// @brief : Get Query Result for Last DoQuery.
	// @return : 
	// @remark :KGC_MySQLResult使用完毕必须Release
	KMySqlResult GetQueryResult(KE_GETRESULT_METHOD eGetMethod = emKGET_RESULT_STORE);
	////////////////////////////////////////////////////////////////////////////
	// @brief : create a legal SQL string that you can use in an SQL statement
	// @return : The length of the value placed into pszDst, not including the terminating null character
	DWORD EncodeString(LPSTR pszDst, LPCSTR pszSrc, INT nLen);
	MYSQL* GetMysqlHandle() { return m_pMySQL; }
	////////////////////////////////////////////////////////////////////////////
	// @brief : Returns the number of rows changed by the last UPDATE, deleted 
	// @return : An integer greater than zero indicates the number of rows affected or retrieved. 
	DWORD GetAffectedRows( );
protected:
	////////////////////////////////////////////////////////////////////////////
	// @brief : Get MySQL server variables of this connection
	// @param szVariablesName : [in] variables you want get
	// @param uRetBufferSize  : [in] buffer size to store result
	// @param pszRetBuffer    : [out] NULL terminal string contain value
	// @return : true if successful, false if failed.
	BOOL _GetServerVariable(
		CONST CHAR cszVariablesName[], 
		unsigned uRetBufferSize,
		CHAR *pszRetBuffer
		);
	////////////////////////////////////////////////////////////////////////////
	// @brief : Check database character set with server character set.
	// @return : true if equal, false if failed.
	BOOL _CheckDatabaseCharacterSet();
	////////////////////////////////////////////////////////////////////////////
	// @brief : send query to server, if connect lost, reconnect.
	// @return : true if successful, false if failed.
	BOOL _DoQuery(CONST CHAR cszQuery[], DWORD ulLength);
protected:
	MYSQL *m_pMySQL;
	CHAR m_szCharacterSet[64];
private:
};

inline BOOL KMySqlDatabase::DoQuery(CONST CHAR cszQuery[], DWORD ulLength)
{
	return _DoQuery(cszQuery, ulLength);
}
inline BOOL KMySqlDatabase::DoQuery(KGC_MySqlQueryStream &sStream)
{
	return _DoQuery(sStream.GetStr(), sStream.GetStrSize());
}
inline KGC_MySqlQueryStream KMySqlDatabase::CreateQueryStream(CHAR *pWriteBuf)
{
	return KGC_MySqlQueryStream(pWriteBuf, m_pMySQL);
}

inline DWORD KMySqlDatabase::GetAffectedRows( )
{
	return (DWORD)mysql_affected_rows( m_pMySQL );
}
// -------------------------------------------------------------------------

