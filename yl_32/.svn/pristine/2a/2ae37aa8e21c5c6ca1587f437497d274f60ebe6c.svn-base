/* -------------------------------------------------------------------------
//	文件名		：	KDbClusterManager.cpp
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:26:27
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kdbclustermanager.h"
#include "roleserver_cfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KDbClusterManager g_cDbClusterMgr;
KDbClusterManager g_cNameServerMgr;


// -------------------------------------------------------------------------

KDbClusterManager::KDbClusterManager(void)
{
}

KDbClusterManager::~KDbClusterManager(void)
{
}

BOOL KDbClusterManager::PushWork( INT nWorkerIdentity, const KProcessingPackage& rPack )
{
	// 找到指定的工作组
	QCONFIRM_RET_FALSE(nWorkerIdentity && nWorkerIdentity <= (INT)m_vWorkgroups.size());
	KWorkerGroup& rcWorkGroup = m_vWorkgroups[nWorkerIdentity - 1];

	SIZE_T uWorkerCount = rcWorkGroup.vWorkerList.size();
	QCONFIRM_RET_FALSE(uWorkerCount > 0);

	// 在工作组内找一条工作线程（按dwWorkingTurn轮流）
	DWORD dwWorkerForThisTurn = rcWorkGroup.dwWorkingTurn++ % uWorkerCount;
	// 注意：如果多次请求对同一角色进行操作，可能被分配到不同线程而产生竞态
	// 把轮流分配法改为hash分配能解决这种问题，但是会造成线程压力不均匀
	// 因为gateway、gameserver会等待结果，一般不会同时对同一角色发出多次操作请求

	IKDbWorker* pDbWorker = rcWorkGroup.vWorkerList[dwWorkerForThisTurn];
	QCONFIRM_RET_FALSE(pDbWorker);

	if (rPack.pData)
		rPack.pData->AddRef();
	if (rPack.pAdditional)
		rPack.pAdditional->AddRef();

	BOOL bRet = pDbWorker->PushWork(rPack);
	QCONFIRM_RET_FALSE(bRet);

	return bRet;
}

BOOL KDbClusterManager::GetResult( KProcessingPackageList& rPacks )
{
	for (KWorkerGroupList::iterator it = m_vWorkgroups.begin(); it != m_vWorkgroups.end(); ++it)
	{
		KWorkerList& rcWorkerList = it->vWorkerList;

		for (KWorkerList::iterator itWorker = rcWorkerList.begin();
			itWorker != rcWorkerList.end();
			++itWorker)
		{
			IKDbWorker* pDbWorker = *itWorker;
			QCONFIRM_RET_FALSE(pDbWorker);

			BOOL bRet = pDbWorker->GetResult(rPacks);
			QCONFIRM_RET_FALSE(bRet);
		}
	}

	return TRUE;
}

BOOL KDbClusterManager::Init( const KROLESVC_DB_PARAM aDatabaseCfgs[],
							 INT nDbCfgCount, 
							 KEWORKER_CLASS eWorkerClass )
{
	QCONFIRM_RET_FALSE(nDbCfgCount > 0);
	BOOL bRet = FALSE;
	CHAR szWorkerName[ROLESVC_CFG_STRING_MAX_LEN] = "";

	QLogPrintf(LOG_INFO, "DbClusterManager Type[%d] Init...", eWorkerClass);

	for (INT nDbIdentity = 1; nDbIdentity <= nDbCfgCount; ++nDbIdentity)
	{
		KWorkerGroup sWorkGroup;
		sWorkGroup.dwWorkingTurn = 0;
		sWorkGroup.strGroupName = aDatabaseCfgs[nDbIdentity - 1].szDatabase;

		INT nWorkerCount = aDatabaseCfgs[nDbIdentity - 1].nThread;
		QCONFIRM_RET_FALSE(nWorkerCount > 0);

		for (INT nThread = 1; nThread <= nWorkerCount; ++nThread)
		{
			IKDbWorker* pWorker = g_CreateWorker(eWorkerClass);
			QCONFIRM_RET_FALSE(pWorker);

			snprintf(szWorkerName, sizeof(szWorkerName), "%s(%d, Thread%d)",
				aDatabaseCfgs[nDbIdentity - 1].szDatabase, nDbIdentity, nThread);

			bRet = pWorker->Init(aDatabaseCfgs[nDbIdentity - 1], szWorkerName);
			QCONFIRM_RET_FALSE(bRet);

			sWorkGroup.vWorkerList.insert(sWorkGroup.vWorkerList.end(), pWorker);
		}
		m_vWorkgroups.insert(m_vWorkgroups.end(), sWorkGroup);
	}

	return bRet;
}

BOOL KDbClusterManager::Uninit()
{
	QLogPrintf(LOG_INFO, "DbClusterManager Uninit...");
	for (KWorkerGroupList::iterator itGroup = m_vWorkgroups.begin(); itGroup != m_vWorkgroups.end(); ++itGroup)
	{
		for (KWorkerList::iterator itWorker = itGroup->vWorkerList.begin();
			itWorker != itGroup->vWorkerList.end();
			++itWorker)
		{
			(*itWorker)->Uninit();
		}
	}
	m_vWorkgroups.clear();

	return TRUE;
}

BOOL KDbClusterManager::GetWorkStat( std::string& strStatics )
{
	for (KWorkerGroupList::iterator it = m_vWorkgroups.begin(); it != m_vWorkgroups.end(); ++it)
	{
		KWorkerList& rcWorkerList = it->vWorkerList;

		for (KWorkerList::iterator itWorker = rcWorkerList.begin();
			itWorker != rcWorkerList.end();
			++itWorker)
		{
			IKDbWorker* pDbWorker = *itWorker;
			QCONFIRM_RET_FALSE(pDbWorker);

			CHAR szBuffer[1024];
			INT nJobTotal = 0, nJobPending = 0, nJobOk = 0, nBytesRecv = 0, nBytesSend = 0;
			pDbWorker->GetStatics(nJobTotal, nJobPending, nJobOk, nBytesRecv, nBytesSend);
			snprintf(szBuffer, sizeof(szBuffer),
				"%s :\n    jobs total %d, pending %d, done %d. bytes rx %d, tx %d\n",
				pDbWorker->GetName(),
				nJobTotal, nJobPending, nJobOk, nBytesRecv, nBytesSend
				);
			strStatics += szBuffer;
		}
	}

	return TRUE;
}
