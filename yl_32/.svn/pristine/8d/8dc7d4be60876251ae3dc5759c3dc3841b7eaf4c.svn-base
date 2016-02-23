/* -------------------------------------------------------------------------
//	文件名		：	roledb_def.h
//	创建者		：	simon
//	创建时间	：	2010/5/20 15:45:14
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __ROLEDB_DEF_H__
#define __ROLEDB_DEF_H__

#include "kg_database.h"

#define		RDB_TABLE_ACCOUNT			"Account"
#define		RDB_FIELD_ACCOUNT				"AccountName"
#define		RDB_FIELD_ACCOUNT_EXT			"ExtData"
#define		RDB_FIELD_ACCOUNT_INIT_DB		"InitialDb"

#define		RDB_TABLE_ROLE					"Role"
#define		RDB_FIELD_ROLE_ID				"RoleId"
#define		RDB_FIELD_ROLE					"RoleName"
//#define		RDB_FIELD_ROLE_ACCOUNT			"AccountName"
#define		RDB_FIELD_ROLE_PREVIEW_INFO			"PreviewInfo"
#define		RDB_FIELD_ROLE_DATA						"RoleData"
#define		RDB_FIELD_ROLE_LAST_SAVE_TIME		"LastSaveTime"

#define		RDB_TABLE_ALL_TONGS		"AllTongs"

#define		RDB_TABLE_ALL_ROLES		"AllRoles"
//#define		RDB_FIELD_ALL_ROLES_ROLE		"RoleName"
//#define		RDB_FIELD_ALL_ROLES_ACCOUNT		"AccountName"
#define		RDB_FIELD_ALL_ROLES_ZONE				"Zone"
#define		RDB_FIELD_ALL_ROLES_CREATE_TIME		"CreateTime"
#define		RDB_FIELD_ALL_ROLES_COMMENT		"Comment"


const static CHAR ROLESVC_SQL_INIT_TABLE_ALL_ROLES[] =
	"CREATE TABLE IF NOT EXISTS "RDB_TABLE_ALL_ROLES
	" ( "
		RDB_FIELD_ROLE_ID" BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY, "
		RDB_FIELD_ROLE" VARCHAR(32) NOT NULL UNIQUE, "
		RDB_FIELD_ACCOUNT" VARCHAR(32) NOT NULL, "
		RDB_FIELD_ALL_ROLES_ZONE" VARCHAR(32), "
		RDB_FIELD_ALL_ROLES_CREATE_TIME" VARCHAR(20) NOT NULL, "
		RDB_FIELD_ALL_ROLES_COMMENT" VARCHAR(255), "
		" INDEX ("RDB_FIELD_ACCOUNT"), INDEX ("RDB_FIELD_ROLE")"
	" ) ENGINE=MyISAM "; // InnoDB有个像Bug一样的功能，即自增长ID只驻留内存不存盘，某些情况重启服务器会有惊喜 : (

const KGD_FIELD g_caRoleTableFields[] =
{
	{ RDB_FIELD_ROLE_ID,	KGD_TYPE_UNSIGNED_INT64,	KGD_UNSIGNED_INT64_SIZE,	KGD_FIELD_PRIMARY_KEY},
	{ RDB_FIELD_ROLE,	KGD_TYPE_VARCHAR,	KG_NAME_MAX_SIZE,	KGD_FIELD_UNIQUE_INDEX},
	{ RDB_FIELD_ACCOUNT,	KGD_TYPE_VARCHAR,	KG_NAME_MAX_SIZE,	KGD_FIELD_INDEX},
	{ RDB_FIELD_ROLE_PREVIEW_INFO,   KGD_TYPE_BLOB,    KGD_MEDIUM_FIELD_MAX_DATA_SIZE, KGD_FIELD_NORMAL},
	{ RDB_FIELD_ROLE_DATA,   KGD_TYPE_BLOB,    KGD_MEDIUM_FIELD_MAX_DATA_SIZE, KGD_FIELD_NORMAL},
	{ RDB_FIELD_ROLE_LAST_SAVE_TIME, KGD_TYPE_VARCHAR, KG_TIME_STRING_SIZE,	KGD_FIELD_NORMAL}
};
const KGD_FIELD g_caAccountTableFields[] =
{
	{ RDB_FIELD_ACCOUNT,	KGD_TYPE_VARCHAR,	KG_NAME_MAX_SIZE,	KGD_FIELD_PRIMARY_KEY},
	{ RDB_FIELD_ACCOUNT_EXT,	KGD_TYPE_BLOB,	KGD_MEDIUM_FIELD_MAX_DATA_SIZE,	KGD_FIELD_NORMAL},
	{ RDB_FIELD_ACCOUNT_INIT_DB,   KGD_TYPE_INT32,    KGD_INT32_SIZE, KGD_FIELD_NORMAL},
};

//////////////////////////////////////////////////////////////////////////
// nameserver ProcessingPackage定义
struct KRequest_CreateRoleName
{
	CHAR szDbName[KG_NAME_MAX_SIZE];
	CHAR szGateway[KG_NAME_MAX_SIZE];
	INT nDbIdentity;
	BOOL bNewAccount;
};
struct KResponse_CreateRoleName
{
	INT nErrorCode;
	UINT64 u64RoleId;
	INT nDbIdentity; // 传回GatewayProcess
	BOOL bNewAccount; // 传回GatewayProcess
};
//////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------------------------

#endif /* __ROLEDB_DEF_H__ */
