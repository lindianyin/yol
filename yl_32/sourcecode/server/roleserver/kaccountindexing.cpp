/* -------------------------------------------------------------------------
//	文件名		：	KAccountIndexing.cpp
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:27:35
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kaccountindexing.h"
#include "roleserver_cfg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KAccountIndexing g_cAccountIndexing;


// -------------------------------------------------------------------------

KAccountIndexing::KAccountIndexing(void)
{
}

KAccountIndexing::~KAccountIndexing(void)
{
}

BOOL KAccountIndexing::Init(void)
{
	QLogPrintf(LOG_INFO, "[KAccountIndexing] Init...");
	BOOL bRet = LoadAllDatabases(g_sRoleSvcCfg.pDbNodeParams, g_sRoleSvcCfg.nDatabaseNodeCount);

	return bRet;
}

BOOL KAccountIndexing::Uninit()
{
	QLogPrintf(LOG_INFO, "[AccountIndexStatics] total accounts %u", m_cAccIdxContainer.size());
	QLogPrintf(LOG_INFO, "[KAccountIndexing] Uninit...");
	m_cAccIdxContainer.clear();
	m_cDbOverloadMgr.clear();
	return TRUE;
}

const KAccountInfo* KAccountIndexing::GetAccountInfo( LPCSTR pszAccount )
{
	const KAccountInfo* pInfo = NULL;
	KAccIdxContainer::const_iterator it = m_cAccIdxContainer.find(pszAccount);

	if (it != m_cAccIdxContainer.end())
		pInfo = &it->second;

	return pInfo;
}

BOOL KAccountIndexing::LoadFromDatabase( const KROLESVC_DB_PARAM& rsDbCfg, INT nDbIdentity)
{
	const static CHAR szQuery[] =
		"SELECT "RDB_FIELD_ACCOUNT" FROM "RDB_TABLE_ROLE;
	const static CHAR szQueryAccount[] =
		"SELECT "RDB_FIELD_ACCOUNT" FROM "RDB_TABLE_ACCOUNT;

	MYSQL* pPreDbConn = mysql_init(NULL);
	QCONFIRM_RET_FALSE(pPreDbConn);

	MYSQL* pDbConn = mysql_real_connect(pPreDbConn,
		rsDbCfg.szServer, 
		rsDbCfg.szUserName, 
		rsDbCfg.szPassword, 
		rsDbCfg.szDatabase,
		rsDbCfg.nPort,
		NULL,
		CLIENT_FOUND_ROWS);
	if (! pDbConn)
	{
		QLogPrintf(LOG_ERR, "@%d %s(), '%s'", __LINE__, __FUNCTION__, mysql_error(pPreDbConn));
		return FALSE;
	}

	INT nQueryResult = CheckDataBaseVariable(pDbConn);
	QCONFIRM_RET_FALSE(nQueryResult && "Check Database Variable Failed");

	nQueryResult = mysql_query(pDbConn, szQuery);
	QCONFIRM_RET_FALSE(nQueryResult == 0);

	MYSQL_RES* pResult = mysql_store_result(pDbConn);
	QCONFIRM_RET_FALSE(pResult);

	INT nTotalAccountsWithRole = 0;
	INT nRoleCount = 0;
	while ( MYSQL_ROW row = mysql_fetch_row(pResult) )
	{
		KAccountInfo sInfo = { nDbIdentity, /*nLocker =*/ 0, /*RoleCount*/ 1 };
		LPCSTR pszAccount = row[0];
		QCONFIRM_RET_FALSE(pszAccount);

		std::pair<KAccIdxContainer::iterator, bool>
			cResultPair =	m_cAccIdxContainer.insert(std::make_pair(pszAccount, sInfo));
		if ( ! cResultPair.second)
		{
			cResultPair.first->second.nRoleCount++; // map中已有此账户，直接增加RoleCount
			INT nOriginalDBI = cResultPair.first->second.nDatabaseIdentity;
			if (nOriginalDBI != nDbIdentity)
			{
				KDbOverloadMgr::iterator it1 = m_cDbOverloadMgr.find(nOriginalDBI);
				KDbOverloadMgr::iterator it2 = m_cDbOverloadMgr.find(nDbIdentity);
				QLogPrintf(LOG_ERR,
					"账户[%s] 在数据库 %s 和 %s 的角色表中重复出现！！！ "
					"Account exists in another database, the later's considered invalid.\n",
					pszAccount,
					it1->second.strDbNameInfo.c_str(),
					it2->second.strDbNameInfo.c_str());
			}
		}
		else
		{
			++nTotalAccountsWithRole; // 成功插入新账户索引
		}
		nRoleCount++;
	}

	mysql_free_result(pResult);

	QLogPrintf(LOG_INFO,
		"在 数据库[%s] 表[%s] 中发现 %d 个账户, %d 个角色\n", 
		rsDbCfg.szDatabase, RDB_TABLE_ROLE,
		nTotalAccountsWithRole, nRoleCount);

	nQueryResult = mysql_query(pDbConn, szQueryAccount);
	QCONFIRM_RET_FALSE(nQueryResult == 0);

	pResult = mysql_store_result(pDbConn);
	QCONFIRM_RET_FALSE(pResult);

	INT nTotalAccountsWithoutRole = 0;
	INT nAccountsInAccount = 0;
	while ( MYSQL_ROW row = mysql_fetch_row(pResult) )
	{
		KAccountInfo sInfo = { nDbIdentity, /*nLocker =*/ 0, /*RoleCount*/ 0 };
		LPCSTR pszAccount = row[0];
		QCONFIRM_RET_FALSE(pszAccount);

		std::pair<KAccIdxContainer::iterator, bool>
			cResultPair =	m_cAccIdxContainer.insert(std::make_pair(pszAccount, sInfo));
		if (cResultPair.second)
		{
			++nTotalAccountsWithoutRole; // 没有角色的账户
		}
		else
		{
			INT nOriginalDBI = cResultPair.first->second.nDatabaseIdentity;
			if (nOriginalDBI != nDbIdentity)
			{
				KDbOverloadMgr::iterator it1 = m_cDbOverloadMgr.find(nOriginalDBI);
				KDbOverloadMgr::iterator it2 = m_cDbOverloadMgr.find(nDbIdentity);
				QLogPrintf(LOG_ERR,
					"账户[%s] 在数据库 %s 和 %s 的账户表中重复出现！！！ "
					"Account exists in another database, the later's considered invalid.\n",
					pszAccount, it1->second.strDbNameInfo.c_str(), it1->second.strDbNameInfo.c_str());
			}
		}
		nAccountsInAccount++;
	}

	mysql_free_result(pResult);

	QLogPrintf(LOG_INFO,
		"在 数据库[%s] 表[%s] 中发现 %d 个账户, 其中 %d 个无角色信息\n", 
		rsDbCfg.szDatabase, RDB_TABLE_ACCOUNT,
		nAccountsInAccount, nTotalAccountsWithoutRole);

	mysql_close(pDbConn);

	KDbOverload sDbOverload;
	sDbOverload.bReadOnly = FALSE; // 改为读配置
	sDbOverload.nNumAccounts = nTotalAccountsWithRole + nTotalAccountsWithoutRole;
	m_cDbOverloadMgr.insert(std::make_pair(nDbIdentity, sDbOverload));
	sDbOverload.strDbNameInfo = rsDbCfg.szServer;
	sDbOverload.strDbNameInfo += "{";
	sDbOverload.strDbNameInfo += rsDbCfg.szDatabase;
	sDbOverload.strDbNameInfo += "}";

	return TRUE;
}

BOOL KAccountIndexing::LoadAllDatabases( const KROLESVC_DB_PARAM aDbCfg[], INT nCount )
{
	QCONFIRM_RET_FALSE(nCount > 0);

	for (INT nDbIdentity = 1; nDbIdentity <= nCount; ++nDbIdentity)
	{
		BOOL bRet = LoadFromDatabase(aDbCfg[nDbIdentity - 1], nDbIdentity);
		QCONFIRM_RET_FALSE(bRet);
	}

	return TRUE;
}

BOOL KAccountIndexing::OnCreateRole( LPCSTR pszAccount, INT nDbIdentity )
{
	QCONFIRM_RET_FALSE(pszAccount);
	if (nDbIdentity)
	{
		KAccountInfo sInsertAccInfo = { nDbIdentity, /*nLocker*/ 0, /*nRoleCount*/ 1 };
		std::pair<KAccIdxContainer::iterator, bool>
			cResultPair =	m_cAccIdxContainer.insert(std::make_pair(pszAccount, sInsertAccInfo));

		KAccountInfo& rcAccInfo = cResultPair.first->second;

		// insert成功（m_cAccIdxContainer原先无此账户）
		if (cResultPair.second)
		{
			// 更新数据库容量
			KDbOverloadMgr::iterator it = m_cDbOverloadMgr.find(nDbIdentity);
			QCONFIRM_RET_FALSE(it != m_cDbOverloadMgr.end());
			it->second.nNumAccounts++;
		}
		else
		{
			rcAccInfo.nRoleCount++;
		}
	}
	else
	{
		KAccIdxContainer::iterator itAcc = m_cAccIdxContainer.find(pszAccount);
		QCONFIRM_RET_FALSE(itAcc != m_cAccIdxContainer.end());
		KDbOverloadMgr::iterator itDb = m_cDbOverloadMgr.find(itAcc->second.nDatabaseIdentity);
		QCONFIRM_RET_FALSE(itDb != m_cDbOverloadMgr.end());

		if ( -- itAcc->second.nRoleCount <= 0)
		{
			m_cAccIdxContainer.erase(itAcc);
			// 更新数据库容量
			ASSERT(itDb->second.nNumAccounts > 0);
			itDb->second.nNumAccounts--;
		}
	}
	return TRUE;
}

INT KAccountIndexing::GetSuggestedDb( LPCSTR pszAccount )
{
	// 按轮流顺序选一个没写满的数据库，如果需要，可以改为按随机或哈希
	// todo: 改为hash选择
	static INT nDbTurn = 0;
	INT nSelectedDb = 0;
	for (INT i = 0; i < (INT)m_cDbOverloadMgr.size(); ++i)
	{
		INT nTmp = nDbTurn++ % m_cDbOverloadMgr.size() + 1; // DbIdentity从1开始
		KDbOverloadMgr::iterator it = m_cDbOverloadMgr.find(nTmp);
		if (it != m_cDbOverloadMgr.end()
			&& !it->second.bReadOnly
			&& it->second.nNumAccounts < g_sRoleSvcCfg.nMaxAccountPerDB)
		{
			nSelectedDb = nTmp;
			break;
		}
	}

	return nSelectedDb;
}

BOOL KAccountIndexing::UpdateRoleCount( LPCSTR pszAccount, INT nRoleCount )
{
	KAccIdxContainer::iterator it = m_cAccIdxContainer.find(*(KAccIdxKey*)pszAccount);

	if (it != m_cAccIdxContainer.end())
	{
		KAccountInfo& rcAccInfo = it->second;
		rcAccInfo.nRoleCount = nRoleCount;
	}
	return TRUE;
}
