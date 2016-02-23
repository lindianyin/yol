/* -------------------------------------------------------------------------
//	文件名		：	KRoleSvc.cpp
//	创建者		：	simon
//	创建时间	：	2010/5/22 22:24:53
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kroleserver.h"
#include "ksocketinterface.h"
#include "kgatewayprocess.h"
#include "kdbclusterworker.h"
#include "kdbclustermanager.h"
#include "roleserver_cfg.h"
#include "kaccountindexing.h"
#include "kroleservereyes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KRoleServer g_cRoleSvc;


// -------------------------------------------------------------------------

KRoleServer::KRoleServer(void)
{
	m_bRunning = FALSE;
}

KRoleServer::~KRoleServer(void)
{
}

INT KRoleServer::Start(void)
{
	BOOL bRet = FALSE;

	bRet = g_piSocketServer->SocketOpen();
	if (bRet)
	{
		QLogPrintf(LOG_INFO, "_____________Role Server Ready_____________");

		while (m_bRunning)
		{
			g_cRoleServerEyes.Activate();

			INT nPackageCount = g_piSocketServer->MessageLoop();
			if (nPackageCount == 0)
			{
				QThread_Sleep(1);
			}
		}
	}

	return bRet;
}
//
//UINT64 g_uMemStreamed = 0;
//INT KRoleServer::TestCase_ThroughputCapacity(void)
//{
//	BOOL bRet = FALSE;
//	INT nPackId = 0;
//
//	g_piSocketServer->SocketOpen();
//
//#pragma comment(lib, "winmm.lib")
//	DWORD dwStart = timeGetTime();
//	while (timeGetTime() - dwStart < 10000)
//	{
//		g_piSocketServer->MessageLoop();
//		g_cGatewayProcess.Activate();
//		for (int i = 0; i < 1/* && nPackId++ % 4*/; ++i)
//		{
//			KProcessingPackage pack1;
//			pack1.u64PackageId = nPackId++;
//			pack1.pData = QCreateMemBlock(500);
//			g_cDbClusterMgr.PushWork(i + 1, pack1);
//		}
//		//QThread_Sleep(1);
//	}
//	printf("Stream size: %I64u MB, %d Packages\n", g_uMemStreamed /1024/1024, nPackId);
//	system("pause");
//	return bRet;
//}
//
//INT KRoleServer::TestCase_Functionality(void)
//{
//	BOOL bRet = FALSE;
//	INT nPackId = 0;
//	INT i = 0;
//
//
//	g_piSocketServer->SocketOpen();
//
//	while (TRUE)
//	{
//		g_piSocketServer->MessageLoop();
//		g_cGatewayProcess.Activate();
//		for (; i < 1; ++i)
//		{
//			//{
//			//	IMemBlock* pBuffer = QCreateMemBlock(500000);
//			//	KRequest_QueryRoleList* pReq = (KRequest_QueryRoleList*)pBuffer->GetData();
//			//	pReq->nType = emKREQ_QUERY_ROLE_LIST;
//			//	strncpy(pReq->szAccount, "账1", sizeof(pReq->szAccount));
//
//			//	g_cGatewayProcess.ProcessPacket(0, pBuffer);
//			//}
//			//{
//			//	IMemBlock* pBuffer = QCreateMemBlock(500000);
//			//	KRequest_CreateRole* pReq = (KRequest_CreateRole*)pBuffer->GetData();
//			//	pReq->nType = emKREQ_CREATE_ROLE;
//			//	pReq->uRoleListSize = 0;
//			//	pReq->uAccountExtSize = 0;
//			//	pReq->uRoleDataSize = 0;
//			//	strncpy(pReq->szAccount, "账3", sizeof(pReq->szAccount));
//			//	strncpy(pReq->szRole, "角3-1", sizeof(pReq->szRole));
//
//			//	g_cGatewayProcess.ProcessPacket(0, pBuffer);
//			//}
//			//{
//			//	IMemBlock* pBuffer = QCreateMemBlock(500000);
//			//	KRequest_SaveRoleData* pReq = (KRequest_SaveRoleData*)pBuffer->GetData();
//			//	pReq->nType = emKREQ_SAVE_ROLE_DATA;
//			//	pReq->uRoleListSize = 0;
//			//	pReq->uRoleDataSize = 16;
//			//	strncpy(pReq->szAccount, "账3", sizeof(pReq->szAccount));
//			//	strncpy(pReq->szRole, "角3-1", sizeof(pReq->szRole));
//			//	strcpy((CHAR*)pReq->abyData, "存档数据测试哦");
//
//			//	g_cGatewayProcess.ProcessPacket(0, pBuffer);
//			//}
//			//{
//			//	IMemBlock* pBuffer = QCreateMemBlock(500000);
//			//	KRequest_GetRoleData* pReq = (KRequest_GetRoleData*)pBuffer->GetData();
//			//	pReq->nType = emKREQ_GET_ROLE_DATA;
//			//	strncpy(pReq->szAccount, "账3", sizeof(pReq->szAccount));
//			//	strncpy(pReq->szRole, "角3-1", sizeof(pReq->szRole));
//
//			//	g_cGatewayProcess.ProcessPacket(0, pBuffer);
//			//}
//		}
//		QThread_Sleep(1);
//	}
//	printf("Stream size: %I64u MB, %d Packages\n", g_uMemStreamed /1024/1024, nPackId);
//	system("pause");
//	return bRet;
//}

BOOL KRoleServer::LoadConfig()
{
	BOOL bRet = FALSE;
	QLogPrintf(LOG_INFO, "[KRoleServer] Reading configuration...");

	bRet = g_RoleSvcReadCfg(g_sRoleSvcCfg);
	QCONFIRM_RET_FALSE(bRet);

	QCONFIRM_RET_FALSE(g_sRoleSvcCfg.nDatabaseNodeCount > 0);

	KROLESVC_DB_PARAM* pDbParam = new KROLESVC_DB_PARAM[g_sRoleSvcCfg.nDatabaseNodeCount];
	QCONFIRM_RET_FALSE(pDbParam);

	g_sRoleSvcCfg.pDbNodeParams = pDbParam;

	bRet = g_RoleSvcReadClusterCfg(g_sRoleSvcCfg);
	QCONFIRM_RET_FALSE(bRet);
	return bRet;
}

BOOL KRoleServer::Init()
{
	BOOL bRet = FALSE;
	QLogPrintf(LOG_INFO, "[KRoleServer] Init...");

	bRet = LoadConfig();
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cDbClusterMgr.Init(g_sRoleSvcCfg.pDbNodeParams,
		g_sRoleSvcCfg.nDatabaseNodeCount,
		emKWORKER_CLASS_DB_NODE
		);
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cAccountIndexing.Init();
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cNameServerMgr.Init(&g_sRoleSvcCfg.sNameServerParam,
		1,
		emKWORKER_CLASS_NAMESERVER
		);
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_piSocketServer->Init(&g_cGatewayProcess);
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cRoleServerEyes.Init();
	QCONFIRM_RET_FALSE(bRet);

	m_bRunning = TRUE;
	return bRet;
}

BOOL KRoleServer::Uninit()
{
	BOOL bRet = FALSE;
	QLogPrintf(LOG_INFO, "[KRoleServer] Uninit...");
	m_bRunning = FALSE;

	g_cRoleServerEyes.UnInit();

	bRet = g_piSocketServer->SocketClose();
	QCONFIRM(bRet);

	bRet = g_cNameServerMgr.Uninit();
	bRet = g_cDbClusterMgr.Uninit();

	bRet = g_cAccountIndexing.Uninit();
	g_cGatewayProcess.Uninit();

	SAFE_DELETE_ARRAY(g_sRoleSvcCfg.pDbNodeParams);
	return bRet;
}
