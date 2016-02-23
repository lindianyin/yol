/* -------------------------------------------------------------------------
//	文件名		：	KDbClusterWorker.h
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:21:21
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KDBCLUSTERWORKER_H__
#define __KDBCLUSTERWORKER_H__

// -------------------------------------------------------------------------

#include <string>
#include <list>
#include "protocol/roleserver_protocol.h"
#include "ikdbworker.h"

class IKGD_Database;
struct KROLESVC_DB_PARAM;


const unsigned KG_GODDESS_STRING_MAX_SIZE = 64;
const unsigned KG_GODDESS_SERVER_INFORMATION_STRING_SIZE = 256;


class KDbClusterWorker : public IKDbWorker
{
public:
	static KDbClusterWorker* CreateInstance() { return new KDbClusterWorker; }
	INT AddRef() { m_nRef++; return TRUE; }
	INT Release() { if (--m_nRef <= 0) delete this; return TRUE;  }

	BOOL Init(const KROLESVC_DB_PARAM& rcDatabaseCfg, const std::string& strName);
	BOOL Uninit();

	BOOL TestWork(const KProcessingPackage& rPacks); // 预先测试可否接受此任务
	BOOL PushWork(const KProcessingPackage& rcPack);
	BOOL GetResult(KProcessingPackageList& rPacks);

	LPCSTR GetName() { return m_strWorkerName.c_str(); }
private:
	KDbClusterWorker();
	virtual ~KDbClusterWorker(void);

	INT ProcessSingleJob(KProcessingPackage& rcRequest, KProcessingPackage& rcResult);
    INT ProcessJobQueue();

    INT _ThreadFunction();
    static VOID _WorkThreadFunction(VOID *pvParam); 

    // Package processing
    INT _QueryRoleList(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult);
    INT _CreateRole(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult);
    INT _DeleteRole(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult);
    INT _SaveRoleData(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult);
    INT _GetRoleData(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult);

	BOOL InitDatabase( const KROLESVC_DB_PARAM& rcParam );
	BOOL GetStatics(INT& rnJobTotal, INT& rnJobPending, INT& rnJobOk,
		INT& rnBytesRecv, INT& rnBytesSend)
	{
		m_cJobListLock.Lock();
		rnJobPending = m_cJobList.size();
		m_cJobListLock.Unlock();
		rnJobTotal = m_uTotalJobs;
		rnJobOk = m_uTotalJobsOk; // aligned 32bit read on x86 is atomic, so no need to lock
		rnBytesSend = m_uBytesSend;
		rnBytesRecv = m_uBytesRecv;
		return TRUE;
	}
private:
    typedef INT (KDbClusterWorker::*PProtocolProcessFunction)
		(const KProcessingPackage& rcRequest, KProcessingPackage& rcResult); 
	static PProtocolProcessFunction ms_afnProtocolFunction[UCHAR_MAX + 1];

	std::string m_strWorkerName;
	QThread m_WorkThread;
	IKGD_Database* m_piDatabase;
	QLock m_cJobListLock;
	QLock m_cResultListLock;
	KProcessingPackageList m_cJobList;
	KProcessingPackageList m_cResultList;
	INT m_nRef;
	BOOL m_bRunning;
	UINT m_uTotalJobs;
	UINT m_uTotalJobsOk;
	size_t m_uBytesRecv;
	size_t m_uBytesSend;
};


#endif /* __KDBCLUSTERWORKER_H__ */
