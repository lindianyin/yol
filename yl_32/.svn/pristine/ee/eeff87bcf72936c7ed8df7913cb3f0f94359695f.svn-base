/* -------------------------------------------------------------------------
//	文件名		：	rolesvc_cfg.h
//	创建者		：	simon
//	创建时间	：	2010/5/19 15:15:03
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __ROLESVC_CFG_H__
#define __ROLESVC_CFG_H__

// -------------------------------------------------------------------------

const static INT ROLESVC_DEFAULT_MAX_ROLE_LIST_SIZE = 524000; // 默认每账户的角色列表字段之和小于512KB
const static INT ROLESVC_DEFAULT_MAX_ACCOUNTS_PER_DB = 1000000; // 默认每数据库容纳账户数

const static INT ROLESVC_CFG_STRING_MAX_LEN = 256;
const static INT ROLESVC_CFG_DEFAULT_DB_PORT = 3306;

const static CHAR ROLESVC_CFG_FILE_NAME[] = "roleserver_cfg.ini";

const static CHAR ROLESVC_CFG_SECTION_NETWORK[] = "Network";
const static CHAR ROLESVC_CFG_SECTION_LIMIT[] = "Limit";
const static CHAR ROLESVC_CFG_SECTION_NAMESERVER[] = "NameServer";
const static CHAR ROLESVC_CFG_SECTION_DB_CLUSTER[] = "DatabaseCluster";
const static CHAR ROLESVC_CFG_SECTION_DB_NODE[] = "DatabaseNode";

struct KROLESVC_DB_PARAM 
{
	CHAR szServer[ROLESVC_CFG_STRING_MAX_LEN];
	INT nPort;
	CHAR szUserName[ROLESVC_CFG_STRING_MAX_LEN];
	CHAR szPassword[ROLESVC_CFG_STRING_MAX_LEN];
	CHAR szDatabase[ROLESVC_CFG_STRING_MAX_LEN];
	INT nThread;
};

struct KROLESVC_NETWORK_PARAM
{
	INT  nSendRecvTimeout;    // 毫秒
	INT  nPingTimeOut; // ping超时

	INT nInAdapterIdx;
	INT nOutAdapterIdx;
	CHAR szIntranetIp[ROLESVC_CFG_STRING_MAX_LEN]; // 内网Ip
	CHAR szInternetIp[ROLESVC_CFG_STRING_MAX_LEN]; // 外网Ip
	INT nListenPort;
};

struct KROLESVC_PARAM
{
	KROLESVC_NETWORK_PARAM sNetwork;
	INT  nMaxRolePerAccount; // 每账户角色数上限
	INT nMaxRoleListBuffer; // 每账户最大角色列表缓冲区大小
	INT nMaxAccountPerDB; // 每数据库容纳的账户上限

	KROLESVC_DB_PARAM sNameServerParam;

	INT nDatabaseNodeCount; // 角色数据库节点数
	CHAR szDbClusterUser[ROLESVC_CFG_STRING_MAX_LEN];
	CHAR szDbClusterPassword[ROLESVC_CFG_STRING_MAX_LEN];
	KROLESVC_DB_PARAM* pDbNodeParams; // 运行时动态载入
};

extern KROLESVC_PARAM g_sRoleSvcCfg;

BOOL g_RoleSvcReadCfg(KROLESVC_PARAM& rcParam);
BOOL g_RoleSvcReadClusterCfg(KROLESVC_PARAM& rcParam);

INT g_GetLocalTimeString(CHAR szTime[], SIZE_T uBufferSize);
// -------------------------------------------------------------------------

#endif /* __ROLESVC_CFG_H__ */
