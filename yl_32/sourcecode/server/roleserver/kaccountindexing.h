/* -------------------------------------------------------------------------
//	文件名		：	KAccountIndexing.h
//	创建者		：	simon
//	创建时间	：	2010/5/21 21:43:27
//	功能描述	：	管理账户与其所在数据库的信息等
//
// -----------------------------------------------------------------------*/
#ifndef __KACCOUNTINDEXING_H__
#define __KACCOUNTINDEXING_H__

// -------------------------------------------------------------------------

#include <map>
#include "protocol/roleserver_protocol.h"

struct KROLESVC_DB_PARAM;

struct KAccountInfo 
{
	//KAccountInfo() { nDatabaseIdentity = 0; nLocker = 0; nRoleCount = 0; }

	SHORT nDatabaseIdentity;
	SHORT nLocker;
	SHORT nRoleCount;
};

class KAccountIndexing
{
public:
	KAccountIndexing(void);
	~KAccountIndexing(void);

	BOOL Init(void);
	BOOL Uninit(void);

	INT GetSuggestedDb(LPCSTR pszAccount); // 选择一个数据库（为新建账户）
	const KAccountInfo* GetAccountInfo(LPCSTR pszAccount);
	BOOL OnCreateRole(LPCSTR pszAccount, INT nDbIdentity);
	BOOL UpdateRoleCount(LPCSTR pszAccount, INT nRoleCount); // 调试 检测绕过rs删号的情况
	SIZE_T TotalAccounts() { return m_cAccIdxContainer.size(); }
private:
	BOOL LoadFromDatabase(const KROLESVC_DB_PARAM& rsDbCfg, INT nDbIdentity);
	BOOL LoadAllDatabases(const KROLESVC_DB_PARAM aDbCfg[], INT nCount);

private:
	struct KAccIdxKey
	{
		KAccIdxKey(LPCSTR pString) { QStrCpyLen(szAccount, pString, sizeof(szAccount)); }

		friend bool operator < (const KAccIdxKey& acc1, const KAccIdxKey& acc2)
		{ return strncmp(acc1.szAccount, acc2.szAccount, sizeof(acc1.szAccount)) < 0; }

		CHAR szAccount[KD_MAX_NAME_LEN];
	};
	struct KDbOverload
	{
		INT nNumAccounts;
		BOOL bReadOnly;
		std::string strDbNameInfo;
	};

	typedef std::map<KAccIdxKey, KAccountInfo> KAccIdxContainer;
	KAccIdxContainer m_cAccIdxContainer;

	typedef std::map<INT, KDbOverload> KDbOverloadMgr;
	KDbOverloadMgr m_cDbOverloadMgr;
};

extern KAccountIndexing g_cAccountIndexing;

// -------------------------------------------------------------------------

#endif /* __KACCOUNTINDEXING_H__ */
