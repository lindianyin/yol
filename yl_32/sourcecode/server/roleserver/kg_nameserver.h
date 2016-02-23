/* -------------------------------------------------------------------------
//	文件名		：	KG_NameServer.h
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:18:56
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KG_NAMESERVER_H__
#define __KG_NAMESERVER_H__

// -------------------------------------------------------------------------


#include "kg_database.h"
#include "roleserver_cfg.h"
#include "ikdbworker.h"

static const unsigned ESCAPE_STRING_SIZE = 128;

class KG_NameServer : public IKDbWorker
{
public:
	static KG_NameServer* CreateInstance();
	INT AddRef() { m_nRef++; return TRUE; }
	INT Release();

	BOOL Init(const KROLESVC_DB_PARAM& rcDatabaseCfg, const std::string& strName);
	BOOL Uninit();

	BOOL TestWork(const KProcessingPackage& rPacks) { return TRUE; }
	BOOL PushWork(const KProcessingPackage& rPack);
	BOOL GetResult(KProcessingPackageList& rPacks);

	LPCSTR GetName() { return m_strWorkerName.c_str(); }

	BOOL GetStatics(INT& rnJobTotal, INT& rnJobPending, INT& rnJobOk,
		INT& rnBytesRecv, INT& rnBytesSend)
	{
		m_cJobListLock.Lock();
		rnJobPending = m_cJobList.size();
		m_cJobListLock.Unlock();
		rnJobTotal = m_uTotalJobs;
		rnJobOk = m_uTotalJobsOk; // aligned 32bit read on x86 is atomic, so no need to lock
		rnBytesSend = m_uBytesSend;
		rnBytesRecv = 0;
		return TRUE;
	}
private:
	KG_NameServer();
	virtual ~KG_NameServer();

	INT ProcessJobQueue();
	INT ProcessSingleJob(KProcessingPackage& rcRequest, KProcessingPackage& rcResult);

	INT _ThreadFunction();
	static VOID _WorkThreadFunction(VOID *pvParam); 

    INT _InitNameServerConnect(const KROLESVC_DB_PARAM &crParam);
    INT _UnInitNameServerConnect();

    INT _CreateNameServerDatabase(const KROLESVC_DB_PARAM &crParam);

	INT _InsertRoleName(LPCSTR pszAccoutName, LPCSTR pszRoleName, LPCSTR pszDatabaseName, LPCSTR pszGateway, UINT64& ru64RoleId, INT& rnRetCode);

	INT DeleteRoleName(const char cszRoleName[], unsigned uRoleNameSize);
	BOOL CreateRoleName(KProcessingPackage& rcRequest, KProcessingPackage& rcResult);

private:
	MYSQL* m_pMysqlConn;
	INT m_nRef;
	std::string m_strWorkerName;
	QThread m_WorkThread;
	QLock m_cJobListLock;
	QLock m_cResultListLock;
	KProcessingPackageList m_cJobList;
	KProcessingPackageList m_cResultList;
	BOOL m_bRunning;
	UINT m_uTotalJobs;
	UINT m_uTotalJobsOk;
	size_t m_uBytesSend;
};

// -------------------------------------------------------------------------

#endif /* __KG_NAMESERVER_H__ */
