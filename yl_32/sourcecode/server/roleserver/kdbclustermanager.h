/* -------------------------------------------------------------------------
//	文件名		：	KDbClusterManager.h
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:26:12
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KDBCLUSTERMANAGER_H__
#define __KDBCLUSTERMANAGER_H__

// -------------------------------------------------------------------------

#include <list>
#include "kdbclusterworker.h"


struct KROLESVC_DB_PARAM;
struct KProcessingPackage;

struct KWorkerGroup;
typedef std::vector<IKDbWorker*> KWorkerList;
typedef std::vector<KWorkerGroup> KWorkerGroupList;

struct KWorkerGroup 
{
	std::string strGroupName;
	KWorkerList vWorkerList;
	DWORD dwWorkingTurn; // 轮流次序
};

class KDbClusterManager
{
public:
	KDbClusterManager(void);
	~KDbClusterManager(void);

	BOOL Init(const KROLESVC_DB_PARAM aDatabaseCfgs[], INT nDbCfgCount, KEWORKER_CLASS eWorkerClass);
	BOOL Uninit();

	BOOL PushWork(INT nWorkerIdentity, const KProcessingPackage& rPacks);
	BOOL GetResult(KProcessingPackageList& rPacks);

	BOOL GetWorkStat(std::string& strStatics);
private:
	KWorkerGroupList m_vWorkgroups;
};

extern KDbClusterManager g_cDbClusterMgr;
extern KDbClusterManager g_cNameServerMgr;

// -------------------------------------------------------------------------

#endif /* __KDBCLUSTERMANAGER_H__ */
