/* -------------------------------------------------------------------------
//	文件名		：	kglobaldatadbprocess.h
//	创建者		：	liuchang
//	创建时间	：	2010-9-1 9:19:45
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGLOBALDATADBPROCESS_H__
#define __KGLOBALDATADBPROCESS_H__

// -------------------------------------------------------------------------
#include "serverbase/kdbcallback.h"

// -------------------------------------------------------------------------


// Warring: 此枚举与上面的数组索引一一对应
// -------------------------------------------------------------------------
// Common
static KDBFieldDesc s_TableFieldDesc[] = 
{
	// 群Id，群名，群主Id，成员列表
	{"GroupId", emK_FIELDTYPE_UNSIGNED_INT32, emK_FIELD_FLAG_NOTNULL, 0},
	{"GroupData", emK_FIELDTYPE_MEDIUMBLOB,	  emK_FIELD_FLAG_NOTNULL,	0},
};
static const INT s_nColumnNum = sizeof(s_TableFieldDesc) / sizeof(s_TableFieldDesc[0]);
enum KE_GlobalDATA_COLUMN_INDEX
{
	emKGLOBALDATA_GROUPID,
	emKGLOBALDATA_GROUPDATA,
};
// 创建主键
static KDBIndexDesc s_TableSingleColIndex[] = {
	{ s_TableFieldDesc[emKGLOBALDATA_GROUPID].szFieldName, 0, emKFIELD_INDEX_PRIMARY },
};

// -------------------------------------------------------------------------
// INT
static const char *szGlobalIntDataName = "tblGlobalIntData";
static KDBTableDesc s_GlobalIntDataTableDesc = {
	szGlobalIntDataName, s_nColumnNum, s_TableFieldDesc
};

// -------------------------------------------------------------------------
// BIN
static const char *szGlobalBinDataName = "tblGlobalBinData";
static KDBTableDesc s_GlobalBinDataTableDesc = {
	szGlobalBinDataName, s_nColumnNum, s_TableFieldDesc
};

class KDB_INT_DATA_ITEM;
class KDB_BIN_DATA_ITEM;

class KGlobalDataQuery_Save_INT : public KDBThreadCallback
{
public:
	KGlobalDataQuery_Save_INT()
	{
		m_nGroupId = 0;
		m_nDataSize = 0;
		m_pGroupData = NULL;
	}

	virtual VOID Release();
	virtual BOOL DBThreadCallback(INT nParam, LPVOID pParam);

public:
	INT m_nGroupId;
	INT m_nDataSize;
	KDB_INT_DATA_ITEM *m_pGroupData;
};

class KGlobalDataQueryRet_SaveIntGroup : public KMainThreadCallback
{
	virtual VOID Release();
	virtual BOOL MainThreadCallback(INT nParam, LPVOID pParam);
	
public:
	INT m_nGroupId;
};

class KGlobalDataQuery_SaveBinGroup : public KDBThreadCallback
{
public:
	KGlobalDataQuery_SaveBinGroup()
	{
		m_nGroupId = 0;
		m_nDataSize = 0;
		m_pGroupData = NULL;
	}

	virtual VOID Release();
	virtual BOOL DBThreadCallback(INT nParam, LPVOID pParam);

public:
	INT m_nGroupId;
	INT m_nDataSize;
	BYTE *m_pGroupData;
};


class KGlobalDataQueryRet_SaveBinGroup : public KMainThreadCallback
{
public:
	virtual VOID Release();
	virtual BOOL MainThreadCallback(INT nParam, LPVOID pParam);

public:
	INT m_nGroupId;
};

#endif /* __KGLOBALDATADBPROCESS_H__ */
