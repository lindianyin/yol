

#include "stdafx.h"
#include "errmsg.h"
#include "Misc/utilities.h"
#include "onlinegamebase/mysqldatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

#define  _KD_COPY_CSTR_ARRAY(cstrArray) {memcpy(m_pCurWritePos, cstrArray, sizeof(cstrArray));m_pCurWritePos += sizeof(cstrArray) - 1;}

static const unsigned long MYSQL5_VERSION_ID = 50000; // MySQL 5 Version ID

// type name map array, use value of KE_DBFIELD_TYPE as index. 
static LPCSTR gs_caTypeName[] =
{
	"INT",			// emK_FIELDTYPE_INT32        =0x00
	"INT UNSIGNED",	//emK_FIELDTYPE_UNSIGNED_INT32=0x01
	"BIGINT",		// emK_FIELDTYPE_INT64        =0x02
	"BIGINT UNSIGNED",//emK_FIELDTYPE_UNSIGNED_INT64=0x03
	"CHAR",			// emK_FIELDTYPE_CHAR         =0x04
	"VARCHAR",		// emK_FIELDTYPE_VARCHAR      =0x05
	"TEXT",			// emK_FIELDTYPE_TEXT         =0x06
	"BINARY",		// emK_FIELDTYPE_BINARY       =0x07
	"VARBINARY",	// emK_FIELDTYPE_VARBINARY    =0x08
	"BLOB",			// emK_FIELDTYPE_BLOB         =0x09
	"TINYTEXT",		// emK_FIELDTYPE_TINYTEXT      =0x10
	"TINYBLOB",		// emK_FIELDTYPE_TINYBLOB	=0x11
	"DATETIME",		// emK_FIELDTYPE_DATETIME	=0x12
	"TIMESTAMP",	// emK_FIELDTYPE_TIMESTAMP	=0x13
	"INT AUTO_INCREMENT", //emK_FIELDTYPE_AUTOINCRE
	"MEDIUMBLOB",	// emK_FIELDTYPE_MEDIUMBLOB
	"SMALLINT",		// emK_FIELDTYPE_SMALLINT
	"TINYINT",		// emK_FIELDTYPE_TINYINT
};
// -------------------------------------------------------------------------
KMySqlDatabase::KMySqlDatabase() : m_pMySQL(0) 
{
	 m_szCharacterSet[0] = '\0';
}

// -------------------------------------------------------------------------

BOOL KMySqlDatabase::DBConnect( CONST CHAR cszServerName[],
								  USHORT usPort,  
								  CONST CHAR cszUserName[],
								  CONST CHAR cszPassword[],
								  LPCSTR pcszCurrentDatabase,
								  ULONG ulClientFlag /* = CLIENT_FOUND_ROWS*/) 
{
	INT nResult  = false;
	INT nRetCode = false;
	static CONST CHAR s_cszCharacterSetServer[]   = "character_set_server";
	static CONST CHAR s_cszLowerCaseTableNames[]  = "lower_case_table_names";

	MYSQL *pConnectRet = NULL;
	CHAR szLowerCaseTableNames[4];
	ULONG ulMySqlClientVersion = 0;

	QCONFIRM_RET_FALSE(!m_pMySQL);
	QCONFIRM_RET_FALSE(cszServerName);
	QCONFIRM_RET_FALSE(cszUserName);
	QCONFIRM_RET_FALSE(cszPassword);

	// ----------------- connect to database  ----------------- 
	m_pMySQL = mysql_init(NULL); 
	QCONFIRM_RET_FALSE(m_pMySQL);

	// 允许自动重连
	ulMySqlClientVersion = mysql_get_client_version();
	if (ulMySqlClientVersion >= MYSQL5_VERSION_ID)
	{
		my_bool bReconnectFalg = true;
		nRetCode = mysql_options(m_pMySQL, MYSQL_OPT_RECONNECT, &bReconnectFalg);
		QCONFIRM(nRetCode == 0);
	}

	pConnectRet = mysql_real_connect(
		m_pMySQL, cszServerName, cszUserName, cszPassword, 
		pcszCurrentDatabase, usPort, NULL, 
		ulClientFlag  // Return the number of found (matched) rows, 
		// not the number of affected rows. 
		);
	if (pConnectRet != m_pMySQL)
	{
		Q_Error(mysql_error(m_pMySQL));
		return FALSE;
	}
	// because pConnectRet just for judge mysql_real_connect success or failed
	// now is unused
	pConnectRet = NULL;

	// --------- check MySQL Identifier Case Sensitivity  setting -------------
	nRetCode = _GetServerVariable(
		s_cszLowerCaseTableNames, 
		sizeof(szLowerCaseTableNames), 
		szLowerCaseTableNames
		);
	QCONFIRM_RET_FALSE(nRetCode);

	nRetCode = strcmp(szLowerCaseTableNames, "1");
	if (nRetCode != 0)
	{
		Q_Error("MySQL Server is not config to case-insensitive");
		Q_Error("Please set MySQL variables: lower_case_table_names=1");
		return FALSE;
	}
	// ---------- check database character set and server character set -------- 
	nRetCode = _GetServerVariable(
		s_cszCharacterSetServer, sizeof(m_szCharacterSet), m_szCharacterSet
		);
	QCONFIRM_RET_FALSE(nRetCode);

	nRetCode = _CheckDatabaseCharacterSet();
	QCONFIRM_RET_FALSE(nRetCode);

	// ----------------------- set client character set  ---------------------
	nRetCode = mysql_set_character_set(m_pMySQL, m_szCharacterSet);
	if (nRetCode != 0)
	{
		Q_Error(mysql_error(m_pMySQL));
		return FALSE;
	}
	return TRUE;
}

BOOL KMySqlDatabase::_GetServerVariable( CONST CHAR cszVariablesName[],  unsigned uRetBufferSize, CHAR *pszRetBuffer ) 
{
	INT nResult  = FALSE;
	INT nRetCode = FALSE;
	CHAR szQuery[256];
	MYSQL_RES *pResultSet = NULL;
	MYSQL_ROW row = NULL;
	unsigned uFieldCount = 0;

	ASSERT(cszVariablesName);
	ASSERT(uRetBufferSize > 1);
	ASSERT(pszRetBuffer);

	nRetCode = snprintf(
		szQuery, sizeof(szQuery) - 1, "SHOW VARIABLES LIKE \'%s\'", cszVariablesName
		);
	QCONFIRM_RET_FALSE(nRetCode > 0);
	szQuery[sizeof(szQuery) - 1] = '\0';

	nRetCode = _DoQuery(szQuery, nRetCode);
	QCONFIRM_RET_FALSE(nRetCode);

	pResultSet = mysql_store_result(m_pMySQL);
	if (!pResultSet)
	{
		Q_Error(mysql_error(m_pMySQL));
		return FALSE;
	}
	do 
	{
		uFieldCount = mysql_num_fields(pResultSet);    
		QCONFIRM_BREAK(uFieldCount == 2);

		row = mysql_fetch_row(pResultSet);

		if (!row)
		{
			Q_Error(mysql_error(m_pMySQL));
			break;
		}

		QCONFIRM_BREAK(row[1]);

		strncpy(pszRetBuffer, row[1], uRetBufferSize - 1);
		pszRetBuffer[uRetBufferSize - 1]  = '\0';

	} while(FALSE);
	mysql_free_result(pResultSet);
	return TRUE;
}

BOOL KMySqlDatabase::_DoQuery( CONST CHAR cszQuery[], DWORD ulLength ) 
{
	INT nResult  = 0;
	INT nRetCode = 0;
	INT nReconnectTimes = 0;
	unsigned uMySQLErrorCode = 0;

	QCONFIRM_RET_FALSE(cszQuery);

	while (true) 
	{
		nRetCode = mysql_real_query(m_pMySQL, cszQuery, ulLength);
		if (nRetCode == 0)
			break; // success
		uMySQLErrorCode = mysql_errno(m_pMySQL);
		if (
			(uMySQLErrorCode != CR_SERVER_LOST) &&
			(uMySQLErrorCode != CR_CONN_HOST_ERROR) &&
			(uMySQLErrorCode != CR_CONN_HOST_ERROR) &&
			(uMySQLErrorCode != CR_SERVER_GONE_ERROR)
			) 
			break; // false

		Q_Error("Connection lost, try reconnect.");
		// Checks whether the connection to the server is working. 
		// If the connection has gone down, an automatic reconnection is attempted
		while (nReconnectTimes <= 3)
		{ 
			nRetCode = mysql_ping(m_pMySQL);
			if (nRetCode)
				break;
			++nReconnectTimes;
		}
		QCONFIRM_RET_FALSE(nRetCode == 0);
	}
	if (nRetCode != 0)
	{
		CHAR szBuf[1024];
		UINT uLen = MIN((UINT)sizeof(szBuf) - 1, (UINT)ulLength);
		memcpy(szBuf, cszQuery, uLen);
		szBuf[uLen] = 0;
		Q_Printl("Failed MySQL Query: " << szBuf);
		Q_Error(mysql_error(m_pMySQL));
		return FALSE;
	}
	return TRUE;
}

BOOL KMySqlDatabase::_CheckDatabaseCharacterSet() 
{
	static CONST CHAR s_cszCharacterSetDatabase[] = "character_set_database";
	INT nResult  = false;
	INT nRetCode = false;
	CHAR szDatabaseCharacterSet[32];

	nRetCode = _GetServerVariable(
		s_cszCharacterSetDatabase, 
		sizeof(szDatabaseCharacterSet), 
		szDatabaseCharacterSet
		);
	if (!nRetCode)
	{
		Q_Error(mysql_error(m_pMySQL));
		return FALSE;
	}

	nRetCode = strcmp(szDatabaseCharacterSet, m_szCharacterSet);
	if (nRetCode != 0)
	{
		Q_Error("MySQL Server variables: character_set_database = "
			<< szDatabaseCharacterSet << "character_set_server = " << m_szCharacterSet);
		return FALSE;
	}
	return TRUE;
}

BOOL KMySqlDatabase::CreateDatabase( CONST CHAR cszDatabaseName[KD_DBNAME_MAX_SIZE],  INT nCreateFlag ) 
{
	INT nResult  = 0;
	INT nRetCode = 0;
	CHAR szQuery[sizeof("CREATE DATABASE IF NOT EXISTS ") + KD_DBNAME_MAX_SIZE];

	QCONFIRM_RET_FALSE(m_pMySQL);
	QCONFIRM_RET_FALSE(cszDatabaseName);
	nRetCode = (INT)strlen(cszDatabaseName); 
	QLogPrintf(LOG_DEBUG,"cszDatabaseName = %s",cszDatabaseName);
	QCONFIRM_RET_FALSE(nRetCode > 0);
	QCONFIRM_RET_FALSE(nRetCode < KD_DBNAME_MAX_SIZE);
	// create database
	if (nCreateFlag & emKDBCREATE_IF_NOT_EXIST) 
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
	QCONFIRM_RET_FALSE(nRetCode != -1);

	nRetCode = _DoQuery(szQuery, nRetCode);
	QCONFIRM_RET_FALSE(nRetCode);

	return TRUE;
}

BOOL KMySqlDatabase::RemoveDatabase( CONST CHAR cszDatabaseName[KD_DBNAME_MAX_SIZE] ) 
{
	INT nResult  = 0;
	INT nRetCode = 0;
	CHAR szQuery[sizeof("DROP DATABASE IF EXISTS ") + KD_DBNAME_MAX_SIZE];
	QCONFIRM_RET_FALSE(m_pMySQL);
	QCONFIRM_RET_FALSE(cszDatabaseName);
	nRetCode = (INT)strlen(cszDatabaseName);
	QCONFIRM_RET_FALSE(nRetCode > 0);
	QCONFIRM_RET_FALSE(nRetCode < KD_DBNAME_MAX_SIZE);
	// remove database
	nRetCode = sprintf(szQuery, "DROP DATABASE IF EXISTS %s", cszDatabaseName);
	QCONFIRM_RET_FALSE(nRetCode != -1);
	nRetCode = _DoQuery(szQuery, nRetCode);
	QCONFIRM_RET_FALSE(nRetCode);
	return TRUE;
}

BOOL KMySqlDatabase::SetCurrentDatabase( CONST CHAR cszDatabaseName[KD_DBNAME_MAX_SIZE] ) 
{
	INT nResult  = 0;
	INT nRetCode = 0;
	QCONFIRM_RET_FALSE(m_pMySQL);
	QCONFIRM_RET_FALSE(cszDatabaseName);
	nRetCode = (INT)strlen(cszDatabaseName);
	QCONFIRM_RET_FALSE(nRetCode > 0);
	QCONFIRM_RET_FALSE(nRetCode < KD_DBNAME_MAX_SIZE);
	// set current database
	nRetCode = mysql_select_db(m_pMySQL, cszDatabaseName);
	if (nRetCode != 0)
	{
		Q_Error(mysql_error(m_pMySQL));
		return FALSE;
	}
	nRetCode = _CheckDatabaseCharacterSet();
	QCONFIRM_RET_FALSE(nRetCode);
	return TRUE;
}

KMySqlResult KMySqlDatabase::GetQueryResult( KE_GETRESULT_METHOD eGetMethod /*= emKGET_RESULT_STORE*/ ) 
{
	MYSQL_RES *pRes;
	if (eGetMethod == emKGET_RESULT_STORE)
		pRes =  mysql_store_result(m_pMySQL);
	else
		pRes = mysql_use_result(m_pMySQL);
	if (!pRes)
		Q_Error(mysql_error(m_pMySQL));
	return pRes;
}

DWORD KMySqlDatabase::EncodeString(LPSTR pszDst, LPCSTR pszSrc, INT nLen)
{
	QCONFIRM_RET_NULL(pszDst);
	pszDst[0] = '\0';
	QCONFIRM_RET_NULL(pszSrc && nLen > 0);
	return mysql_real_escape_string(m_pMySQL, pszDst, pszSrc, nLen);
}

KMySqlDatabase::~KMySqlDatabase() 
{
	if (m_pMySQL)
	{		
		mysql_close(m_pMySQL);
	}
}

BOOL KMySqlDatabase::ShutDown() 
{
	if (m_pMySQL)
		mysql_shutdown(m_pMySQL, SHUTDOWN_WAIT_CONNECTIONS);
	return TRUE;
}
/***************************** KGC_MySqlQueryStream *******************************/
BOOL KGC_MySqlQueryStream::AddCreateTable( INT nCreteFlag, KDBTableDesc &desc ) 
{
	if (m_eCurStament != emKSQL_STATEMENT_NONE)
	{
		Q_Error("Statement Not NULL");
		Reset();
	}
	if (nCreteFlag & emKTABLE_CREATE_IF_NOT_EXIST)
	{
		_KD_COPY_CSTR_ARRAY("CREATE TABLE IF NOT EXISTS ");
	}
	else
	{
		_KD_COPY_CSTR_ARRAY("CREATE TABLE ");
	}
	INT nRetCode = sprintf(m_pCurWritePos, "%s(", desc.szTableName);
	m_pCurWritePos += nRetCode;
	for (INT i = 0; i < desc.nFieldNum; i++)
	{
		nRetCode = sprintf(m_pCurWritePos, "%s %s", desc.aFieldDesc[i].szFieldName, gs_caTypeName[desc.aFieldDesc[i].eType]);
		m_pCurWritePos += nRetCode;
		switch(desc.aFieldDesc[i].eType)
		{
		case emK_FIELDTYPE_INT32:
		case emK_FIELDTYPE_UNSIGNED_INT32:
		case emK_FIELDTYPE_INT64:
		case emK_FIELDTYPE_UNSIGNED_INT64:
		case emK_FIELDTYPE_DATETIME:
		case emK_FIELDTYPE_AUTOINCRE:
		case emK_FIELDTYPE_SMALLINT:
		case emK_FIELDTYPE_TINYINT:
			break;
		case emK_FIELDTYPE_CHAR:
		case emK_FIELDTYPE_VARCHAR:
		case emK_FIELDTYPE_TIMESTAMP:
			//case emK_FIELDTYPE_BINARY:
			//case emK_FIELDTYPE_VARBINARY:			
			if (desc.aFieldDesc[i].uMaxSize > 0)
			{
				QCONFIRM_BREAK(desc.aFieldDesc[i].uMaxSize <= KD_TINY_FIELD_MAX_DATA_SIZE);
				nRetCode = sprintf(m_pCurWritePos, "(%u)", desc.aFieldDesc[i].uMaxSize);
				m_pCurWritePos += nRetCode;
			}
			break;
		case emK_FIELDTYPE_TEXT:
		case emK_FIELDTYPE_BLOB:
			QCONFIRM_RET_FALSE(desc.aFieldDesc[i].uMaxSize <= KD_MEDIUM_FIELD_MAX_DATA_SIZE);
			break;
		case emK_FIELDTYPE_TINYTEXT:
		case emK_FIELDTYPE_TINYBLOB:
			QCONFIRM_RET_FALSE(desc.aFieldDesc[i].uMaxSize <= KD_TINY_FIELD_MAX_DATA_SIZE);
			break;
		case emK_FIELDTYPE_MEDIUMBLOB:
			QCONFIRM_RET_FALSE(desc.aFieldDesc[i].uMaxSize <= KD_MEDIUM_BLOB_MAX_SIZE);
			break;
		default:
			QCONFIRM_RET_FALSE(FALSE);
		}
		if (desc.aFieldDesc[i].nFlag & emK_FIELD_FLAG_NOTNULL)
		{
			_KD_COPY_CSTR_ARRAY(" NOT NULL");
		}
		*m_pCurWritePos++ =  ',';
	}
	if (desc.nFieldNum > 0)
		--m_pCurWritePos;  // will cover last ','
	*m_pCurWritePos++ =  ')';
	*m_pCurWritePos = '\0';
	m_eCurStament = emKSQL_STATEMENT_CREATETABLE;
	return TRUE;
}

BOOL KGC_MySqlQueryStream::AddRemoveTable(LPCSTR szTableName)
{
	if (m_eCurStament != emKSQL_STATEMENT_NONE)
	{
		Q_Error("Statement Not NULL");
		Reset();
	}
	INT nRetCode = sprintf(m_pCurWritePos, "DROP TABLE IF EXISTS %s", szTableName);
	QCONFIRM_RET_FALSE(nRetCode > 0);
	m_pCurWritePos += nRetCode;
	m_eCurStament = emKSQL_STATEMENT_DROPTABLE;
	return TRUE;
}

BOOL KGC_MySqlQueryStream::AddCreateTableSingleFieldIndex( INT nDescCount, KDBIndexDesc aIndexDesc[] ) 
{
	QCONFIRM_RET_FALSE(m_eCurStament == emKSQL_STATEMENT_CREATETABLE);
	QCONFIRM_RET_FALSE(nDescCount > 0);
	QCONFIRM_RET_FALSE(*(m_pCurWritePos - 1) == ')');
	--m_pCurWritePos; // will cover last ')'
	*m_pCurWritePos++ = ',';
	INT nRetCode;
	for (INT i = 0; i < nDescCount; i++)
	{
		switch(aIndexDesc[i].eType)
		{
		case emKFIELD_INDEX_COMMON:
			_KD_COPY_CSTR_ARRAY(" INDEX(");
			break;
		case emKFIELD_INDEX_UNIQUE:
			_KD_COPY_CSTR_ARRAY(" UNIQUE(");
			break;
		case emKFIELD_INDEX_PRIMARY:
			_KD_COPY_CSTR_ARRAY(" PRIMARY KEY(");
			break;
		default:
			ASSERT(FALSE);
			_KD_COPY_CSTR_ARRAY(" INDEX(");
			break;
		}
		size_t nSize = strlen(aIndexDesc[i].szFieldName);
		memcpy(m_pCurWritePos, aIndexDesc[i].szFieldName, nSize);
		m_pCurWritePos += nSize;
		if (aIndexDesc[i].nIndexSize > 0 && aIndexDesc[i].nIndexSize <= KD_MAX_INDEX_SIZE)
		{
			nRetCode = sprintf(m_pCurWritePos, "(%d)", aIndexDesc[i].nIndexSize);
			m_pCurWritePos += nRetCode;
		}
		*m_pCurWritePos++ = ')';
		*m_pCurWritePos++ = ',';
	}
	if (nDescCount > 0)
		--m_pCurWritePos;  // will cover last ','
	*m_pCurWritePos++ =  ')';
	*m_pCurWritePos = '\0';
	return TRUE;
}

BOOL KGC_MySqlQueryStream::AddCreateTableMultiFieldIndex( KE_FIELD_INDEX_TYPE eType, INT nFieldCount, KDBIndexDesc aIndexDesc[] ) 
{
	QCONFIRM_RET_FALSE(m_eCurStament == emKSQL_STATEMENT_CREATETABLE);
	QCONFIRM_RET_FALSE(nFieldCount > 1);
	QCONFIRM_RET_FALSE(*(m_pCurWritePos - 1) == ')');
	--m_pCurWritePos; // will cover last ')'
	*m_pCurWritePos++ = ',';
	INT nRetCode;
	switch(eType)
	{
	case emKFIELD_INDEX_COMMON:
		_KD_COPY_CSTR_ARRAY(" INDEX(");
		break;
	case emKFIELD_INDEX_UNIQUE:
		_KD_COPY_CSTR_ARRAY(" UNIQUE(");
		break;
	case emKFIELD_INDEX_PRIMARY:
		_KD_COPY_CSTR_ARRAY(" PRIMARY KEY(");
		break;
	default:
		ASSERT(FALSE);
		_KD_COPY_CSTR_ARRAY(" INDEX(");
	}	
	for (INT i = 0; i < nFieldCount; i++)
	{
		size_t nSize = strlen(aIndexDesc[i].szFieldName);
		memcpy(m_pCurWritePos, aIndexDesc[i].szFieldName, nSize);
		m_pCurWritePos += nSize;
		if (aIndexDesc[i].nIndexSize > 0 && aIndexDesc[i].nIndexSize <= KD_MAX_INDEX_SIZE)
		{
			nRetCode = sprintf(m_pCurWritePos, "(%d)", aIndexDesc[i].nIndexSize);
			m_pCurWritePos += nRetCode;
		}
		*m_pCurWritePos++ = ',';
	}
	if (nFieldCount > 0)
		--m_pCurWritePos;  // will cover last ','
	*m_pCurWritePos++ = ')';//Index Closure
	*m_pCurWritePos++ = ')';//Create Table Closure
	*m_pCurWritePos = '\0';
	return TRUE;
}

BOOL KGC_MySqlQueryStream::AddSelectQuery( INT nFieldCount, KDBFieldDesc aFieldDesc[], LPCSTR szTableName ) 
{
	//if (m_eCurStament != emKSQL_STATEMENT_NONE)
	//{
	//	Q_Error("Statement Not NULL");
	//	Reset();
	//}
	QCONFIRM_RET_FALSE(nFieldCount > 0);
	_KD_COPY_CSTR_ARRAY("SELECT ");
	for (INT i = 0; i < nFieldCount; i++)
	{
		size_t nSize = strlen(aFieldDesc[i].szFieldName);
		memcpy(m_pCurWritePos, aFieldDesc[i].szFieldName, nSize);
		m_pCurWritePos += nSize;
		*m_pCurWritePos++ = ',';
	}
	--m_pCurWritePos;  // will cover last ','
	_KD_COPY_CSTR_ARRAY(" FROM ");
	size_t nSize = strlen(szTableName);
	memcpy(m_pCurWritePos, szTableName, nSize + 1);	//must + 1 to store '\0'
	m_pCurWritePos += nSize;
	m_eCurStament = emKSQL_STATEMENT_SELECT;
	return TRUE;
}

struct _CStrPair
{
	LPCSTR str;
	UINT uLen;
};

CONST static _CStrPair cpCondition[] = {
	{" WHERE ", sizeof(" WHERE ") -1},
	{" AND ", sizeof(" AND ") -1},
	{" OR ", sizeof(" OR ") -1},
};
CONST static _CStrPair cpOperator[] = {
	{" ", sizeof(" ") -1},
	{"<", sizeof("<") -1},
	{"<=", sizeof("<=") -1},
	{"=", sizeof("=") -1},
	{"!=", sizeof("!=") -1},
	{">", sizeof(">") -1},
	{">=", sizeof(">=") -1},
};

BOOL KGC_MySqlQueryStream::AddFieldCondition( KE_DBSQL_CONDITION condition,
											 KE_DBSQL_OPERATOR opt, KDBFieldDesc &FieldDesc) 
{
	QCONFIRM_RET_FALSE(m_eCurStament == emKSQL_STATEMENT_SELECT || m_eCurStament == emKSQL_STATEMENT_DELETE || 
		m_eCurStament == emKSQL_STATEMENT_UPDATE);
	memcpy(m_pCurWritePos, cpCondition[condition].str, cpCondition[condition].uLen);
	m_pCurWritePos += cpCondition[condition].uLen;
	size_t nSize = strlen(FieldDesc.szFieldName);
	memcpy(m_pCurWritePos, FieldDesc.szFieldName, nSize);
	m_pCurWritePos += nSize;
	*m_pCurWritePos++ = ' ';
	memcpy(m_pCurWritePos, cpOperator[opt].str, cpOperator[opt].uLen);
	m_pCurWritePos += cpOperator[opt].uLen;
	*m_pCurWritePos++ = ' ';
	_StoreFieldData(FieldDesc);
	*m_pCurWritePos = '\0';
	return TRUE;
}

BOOL KGC_MySqlQueryStream::_StoreFieldData( KDBFieldDesc &FieldDesc ) 
{
	QCONFIRM_RET_FALSE(FieldDesc.pvData || FieldDesc.uDataSize == 0);
	UINT uRealSize = FieldDesc.uDataSize;
	switch(FieldDesc.eType)
	{
	case emK_FIELDTYPE_INT32:
		{
			INT nSize = sprintf(m_pCurWritePos, "%d", *(LPINT)FieldDesc.pvData);
			m_pCurWritePos += nSize;
		}
		break;
	case emK_FIELDTYPE_UNSIGNED_INT32:
		{
			INT nSize = sprintf(m_pCurWritePos, "%u", *(LPINT)FieldDesc.pvData);
			m_pCurWritePos += nSize;
		}
		break;
	case emK_FIELDTYPE_SMALLINT:
		{
			INT nSize = sprintf(m_pCurWritePos, "%u", *(LPWORD)FieldDesc.pvData);
			m_pCurWritePos += nSize;
		}
		break;
	case emK_FIELDTYPE_TINYINT:
		{
			INT nSize = sprintf(m_pCurWritePos, "%u", *(LPBYTE)FieldDesc.pvData);
			m_pCurWritePos += nSize;
		}
		break;
	case emK_FIELDTYPE_INT64:
		{
#ifdef WIN32
			INT nSize = sprintf(m_pCurWritePos, "%I64d", *(LPINT64)FieldDesc.pvData);
#else
			INT nSize = sprintf(m_pCurWritePos, "%lld", *(LPINT64)FieldDesc.pvData);
#endif
			m_pCurWritePos += nSize;
		}
		break;
	case emK_FIELDTYPE_UNSIGNED_INT64:
		{
#ifdef WIN32
			INT nSize = sprintf(m_pCurWritePos, "%I64u", *(LPINT64)FieldDesc.pvData);
#else
			INT nSize = sprintf(m_pCurWritePos, "%llu", *(LPINT64)FieldDesc.pvData);
#endif
			m_pCurWritePos += nSize;

		}
		break;
	case emK_FIELDTYPE_TEXT:
	case emK_FIELDTYPE_BLOB:
		{
			*m_pCurWritePos++ = '\'';
			if (FieldDesc.uDataSize > 0)
			{
				if (FieldDesc.uDataSize > KD_MEDIUM_FIELD_MAX_DATA_SIZE)
				{
					ASSERT(FALSE);
					FieldDesc.uDataSize = KD_MEDIUM_FIELD_MAX_DATA_SIZE;
				}
				DWORD uRetCode = mysql_real_escape_string(m_pMySQL, 
				m_pCurWritePos, (LPCSTR)FieldDesc.pvData, FieldDesc.uDataSize);
				m_pCurWritePos += uRetCode;
			}
			*m_pCurWritePos++ = '\'';
		}
		break;
	case emK_FIELDTYPE_MEDIUMBLOB:
		{
			*m_pCurWritePos++ = '\'';
			if (FieldDesc.uDataSize > 0)
			{
				if (FieldDesc.uDataSize > KD_MEDIUM_BLOB_MAX_SIZE)
				{
					ASSERT(FALSE);
					FieldDesc.uDataSize = KD_MEDIUM_BLOB_MAX_SIZE;
				}
				DWORD uRetCode = mysql_real_escape_string(m_pMySQL, 
					m_pCurWritePos, (LPCSTR)FieldDesc.pvData, FieldDesc.uDataSize);
				m_pCurWritePos += uRetCode;
			}
			*m_pCurWritePos++ = '\'';
		}
		break;
	case emK_FIELDTYPE_CHAR:
	case emK_FIELDTYPE_VARCHAR:
	case emK_FIELDTYPE_TINYTEXT:
		uRealSize = (UINT)strlen((LPCSTR)FieldDesc.pvData);
		if (uRealSize > FieldDesc.uMaxSize)
			uRealSize = FieldDesc.uMaxSize;
		// no break
	case emK_FIELDTYPE_TINYBLOB:
		{
			*m_pCurWritePos++ = '\'';
			if (uRealSize > 0)
			{
				if (uRealSize > KD_TINY_FIELD_MAX_DATA_SIZE)
				{
					ASSERT(FALSE);
					uRealSize = KD_TINY_FIELD_MAX_DATA_SIZE;
				}
				DWORD uRetCode = mysql_real_escape_string(m_pMySQL, 
					m_pCurWritePos, (LPCSTR)FieldDesc.pvData, uRealSize);
				m_pCurWritePos += uRetCode;
			}
			*m_pCurWritePos++ = '\'';
		}
		break;
	case emK_FIELDTYPE_DATETIME:
		{
			*m_pCurWritePos++ = '\'';
			if (sizeof(time_t) == FieldDesc.uDataSize)
			{
				tm tmStruct = *localtime((time_t*)FieldDesc.pvData);
				UINT uSize = (UINT)strftime(m_pCurWritePos, 32, "%Y-%m-%d %H:%M:%S", &tmStruct);
				m_pCurWritePos += uSize;
			}
			else
			{
				const static char cszTime[] = "yyyy-mm-dd hh:mm:ss";
				ASSERT(FieldDesc.uDataSize > 0 && FieldDesc.uDataSize <= sizeof(cszTime) 
					&& ((LPCSTR)FieldDesc.pvData)[FieldDesc.uDataSize] == '\0');
				memcpy(m_pCurWritePos, FieldDesc.pvData, FieldDesc.uDataSize);
				m_pCurWritePos += FieldDesc.uDataSize;
			}
			*m_pCurWritePos++ = '\'';
		}
		break;
	default:
		Q_Error("Invalid Type!");
	}
	return TRUE;
}

BOOL KGC_MySqlQueryStream::AddInsertRecord( INT nFieldCount, KDBFieldDesc aFieldDesc[], 
										   LPCSTR szTableName, BOOL bInsertIgnore /*= FALSE*/ ) 
{
	if (m_eCurStament != emKSQL_STATEMENT_NONE)
	{
		Q_Error("Statement Not NULL");
		Reset();
	}
	if (!bInsertIgnore)
	{
		_KD_COPY_CSTR_ARRAY("INSERT INTO ");
	}
	else
	{
		_KD_COPY_CSTR_ARRAY("INSERT IGNORE INTO ");
	}
	size_t nSize = strlen(szTableName);
	memcpy(m_pCurWritePos, szTableName, nSize);
	m_pCurWritePos += nSize;
	*m_pCurWritePos++ = '(';
	for (INT i = 0; i < nFieldCount; i++)
	{
		size_t nSize = strlen(aFieldDesc[i].szFieldName);
		memcpy(m_pCurWritePos, aFieldDesc[i].szFieldName, nSize);
		m_pCurWritePos += nSize;
		*m_pCurWritePos++ = ',';
	}
	if (nFieldCount > 0)
		--m_pCurWritePos; // cover last ','
	_KD_COPY_CSTR_ARRAY(")VALUES(");
	for (INT i = 0; i < nFieldCount; i++)
	{
		_StoreFieldData(aFieldDesc[i]);
		*m_pCurWritePos++ = ',';
	}
	if (nFieldCount > 0)
		--m_pCurWritePos; // cover last ','
	*m_pCurWritePos++ = ')';
	*m_pCurWritePos = '\0';
	m_eCurStament = emKSQL_STATEMENT_INSERT;
	return TRUE;
}

BOOL KGC_MySqlQueryStream::AddReplaceRecord( INT nFieldCount, KDBFieldDesc aFieldDesc[], LPCSTR szTableName) 
{
	if (m_eCurStament != emKSQL_STATEMENT_NONE)
	{
		Q_Error("Statement Not NULL");
		Reset();
	}
	_KD_COPY_CSTR_ARRAY("REPLACE INTO ");
	size_t nSize = strlen(szTableName);
	memcpy(m_pCurWritePos, szTableName, nSize);
	m_pCurWritePos += nSize;
	*m_pCurWritePos++ = '(';
	for (INT i = 0; i < nFieldCount; i++)
	{
		size_t nSize = strlen(aFieldDesc[i].szFieldName);
		memcpy(m_pCurWritePos, aFieldDesc[i].szFieldName, nSize);
		m_pCurWritePos += nSize;
		*m_pCurWritePos++ = ',';
	}
	if (nFieldCount > 0)
		--m_pCurWritePos; // cover last ','
	_KD_COPY_CSTR_ARRAY(")VALUES(");
	for (INT i = 0; i < nFieldCount; i++)
	{
		_StoreFieldData(aFieldDesc[i]);
		*m_pCurWritePos++ = ',';
	}
	if (nFieldCount > 0)
		--m_pCurWritePos; // cover last ','
	*m_pCurWritePos++ = ')';
	*m_pCurWritePos = '\0';
	m_eCurStament = emKSQL_STATEMENT_REPLACE;
	return TRUE;
}

BOOL KGC_MySqlQueryStream::AddDeleteRecord( LPCSTR szTableName ) 
{
	if (m_eCurStament != emKSQL_STATEMENT_NONE)
	{
		Q_Error("Statement Not NULL");
		Reset();
	}
	_KD_COPY_CSTR_ARRAY("DELETE FROM ");
	size_t nSize = strlen(szTableName);
	memcpy(m_pCurWritePos, szTableName, nSize + 1);	//+1 to stored '\0'
	m_pCurWritePos += nSize;
	m_eCurStament = emKSQL_STATEMENT_DELETE;
	return TRUE;
}

BOOL KGC_MySqlQueryStream::AddUpdateRecord( INT nFieldCount, KDBFieldDesc aFieldDesc[], LPCSTR szTableName ) 
{
	if (m_eCurStament != emKSQL_STATEMENT_NONE)
	{
		Q_Error("Statement Not NULL");
		Reset();
	}
	QCONFIRM_RET_FALSE(nFieldCount > 0);
	_KD_COPY_CSTR_ARRAY("UPDATE ");
	size_t nSize = strlen(szTableName);
	memcpy(m_pCurWritePos, szTableName, nSize + 1);	//+1 to stored '\0'
	m_pCurWritePos += nSize;
	*m_pCurWritePos++ = ' ';
	_KD_COPY_CSTR_ARRAY("SET ");
	for (INT i = 0; i < nFieldCount; i++)
	{
		size_t nSize = strlen(aFieldDesc[i].szFieldName);
		memcpy(m_pCurWritePos, aFieldDesc[i].szFieldName, nSize);
		m_pCurWritePos += nSize;
		*m_pCurWritePos++ = '=';
		_StoreFieldData(aFieldDesc[i]);
		*m_pCurWritePos++ = ',';
	}
	if (nFieldCount > 0)
		*(m_pCurWritePos - 1) = ' '; // cover last ','
	*m_pCurWritePos = '\0';
	m_eCurStament = emKSQL_STATEMENT_UPDATE;
	return TRUE;
}

LPCSTR KGC_MySqlQueryStream::GetStr() 
{
	if (*m_pCurWritePos != '\0')
	{
		Q_Error("Closure not NULL");
		*m_pCurWritePos = '\0';
	}
	return m_pBuffer;
}

BOOL KGC_MySqlQueryStream::AddDupliUpdate( INT nFieldCount, KDBFieldDesc aFieldDesc[] ) 
{
	QCONFIRM_RET_FALSE(m_eCurStament == emKSQL_STATEMENT_INSERT)
	QCONFIRM_RET_FALSE(nFieldCount > 0);
	_KD_COPY_CSTR_ARRAY("ON DUPLICATE KEY UPDATE ");
	for (INT i = 0; i < nFieldCount; i++)
	{
		size_t nSize = strlen(aFieldDesc[i].szFieldName);
		memcpy(m_pCurWritePos, aFieldDesc[i].szFieldName, nSize);
		m_pCurWritePos += nSize;
		*m_pCurWritePos++ = '=';
		_StoreFieldData(aFieldDesc[i]);
		*m_pCurWritePos++ = ',';
	}
	if (nFieldCount > 0)
		*(m_pCurWritePos - 1) = ' '; // cover last ','
	*m_pCurWritePos = '\0';
	return TRUE;
}

CHAR * KGC_MySqlQueryStream::AddStr( LPCSTR pszAdd ) 
{
	m_pCurWritePos += sprintf(m_pCurWritePos, "%s", pszAdd);
	return m_pCurWritePos;
}
