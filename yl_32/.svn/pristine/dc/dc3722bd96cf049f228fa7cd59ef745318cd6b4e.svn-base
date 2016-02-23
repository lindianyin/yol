
#include "stdafx.h"
#include "kgcmonitorprocess.h"
#include "serverbase/kgcmoduledef.h"
#include "serverbase/protocol/gcm2gcgw_protocol.h"
#include "kloadermanager.h"
#include "kdatadef.h"
#include "kloadersharemem.h"
#include "engine/smartptr.h"
#include "kmempipesys.h"
#include "kmodulesetting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KGcMonitorProcess g_cGcMonitorProcess;
extern IKSocketClientInterface* g_piGcMonitorClient;
// -------------------------------------------------------------------------
BOOL KGcMonitorProcess::ProcessEvent( INT nEventId, LPCVOID pData, INT nData )
{
	switch (nEventId)
	{
	case emKSOCKET_EVENT_CREATE:
		{
			KGLogPrintf(KGLOG_DEBUG, "KGcMonitorProcess connected.");
		}
		break;
	case emKSOCKET_EVENT_CLOSE:
		{
			KGLogPrintf(KGLOG_DEBUG, "KGcMonitorProcess connection closed.");
		}
		break;
	case emKSOCKET_EVENT_LOST:
		{
			KGLogPrintf(KGLOG_DEBUG, "KGcMonitorProcess connection lost.");
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;
}


BOOL KGcMonitorProcess::ProcessPacket( LPCBYTE pData, UINT uDataLen )
{
	KGCM2GCGW_HEADER* pHeader = (KGCM2GCGW_HEADER*)pData;
	KGC_CONFIRM_RET_FALSE(pHeader);

	switch (pHeader->byProtocol)
	{
	case emKGCM2GCGW_LOAD_MODULE:
		OnLoadModule(pData, uDataLen);
		break;
	//case emKGCM2GCGW_MODULE_LOADED_NOTIFY:
	//	OnUpdateModuleInfo(pData, uDataLen);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return TRUE;
}

BOOL KGcMonitorProcess::OnLoadModule( LPCBYTE pData, UINT uDataLen )
{
	std::vector<std::pair<BYTE, std::string> > vecModules;
	KGCM2GCGW_LOAD_MODULE* pPak = (KGCM2GCGW_LOAD_MODULE*)pData;
	USHORT uPortGc = 0;
	USHORT uPortGs = 0;

	KG_Printl("Received LoadModule Cmd from monitor." <<
		"\nTotalLoaders: " << (INT)pPak->byTotalLoaderCount <<
		"\nInterCommPort: " << pPak->nGcInterPort <<
		"\nTotalModules: " << pPak->nModuleCount);

	KGC_CONFIRM_RET_FALSE(pPak->byTotalLoaderCount > 0 &&
		pPak->byTotalLoaderCount < emKGCMODULE_ALL);
	KGC_CONFIRM_RET_FALSE(pPak->byLocalLoaderCount > 0 &&
		pPak->byLocalLoaderCount <= pPak->byTotalLoaderCount);
	KLoaderShareMem::Inst()->Init(pPak->byTotalLoaderCount);

	KGCMODULE_INFO asModuleSettings[emKGCMODULE_ALL];
	ZeroStruct(asModuleSettings);
	KLOADER_INFO aInfo[emKGCMODULE_ALL];
	ZeroStruct(aInfo);

	for (INT n = 0; n < pPak->byTotalLoaderCount; ++n)
	{
		KLOADER_INFO& rInfo = aInfo[n];
		rInfo.nLoaderIndex = n;
		rInfo.byLoaderId = pPak->byLocalLoaderIdBegin + (BYTE)n;
		rInfo.uPortGs = pPak->nLoaderPortBase + (BYTE)n;
		KG_Printl("Loader " << (INT)rInfo.byLoaderId << " BasePort " << rInfo.uPortGs);
	}

	for (INT i = 0; i < pPak->nModuleCount; ++i)
	{
		KGCGW_MODULE_INFO& rModuleInfo = ((KGCGW_MODULE_INFO*)pPak->abyModules)[i];
		KGC_CONFIRM_RET_FALSE(rModuleInfo.byLoaderId > 0 && rModuleInfo.byLoaderId <= pPak->byTotalLoaderCount);
		KGC_CONFIRM_RET_FALSE(rModuleInfo.byModuleId > emKGCMODULE_LOGICBASE &&
			rModuleInfo.byModuleId < emKGCMODULE_ALL);

		// byModuleId 与kgcmoduledef.h的KE_GC_MODULE_ID枚举对应
		KGCMODULE_INFO& rInfoSet = asModuleSettings[i];
		rInfoSet.byLoaderId = rModuleInfo.byLoaderId;
		rInfoSet.byModuleId = rModuleInfo.byModuleId;

		g_StrCpyLen(rInfoSet.szModulePath,
			g_aszGcModuleFiles[rModuleInfo.byModuleId], sizeof(rInfoSet.szModulePath));
		g_StrCpyLen(rInfoSet.szModuleName,
			g_aszGcModuleNames[rModuleInfo.byModuleId], sizeof(rInfoSet.szModuleName));
		KG_Printl("Module " << (INT)rModuleInfo.byModuleId << " " << rInfoSet.szModuleName <<
			"(" << rInfoSet.szModulePath << ") --> Loader " << (INT)rModuleInfo.byLoaderId);
	}
	for (INT n = 0; n < pPak->byTotalLoaderCount; ++n)
	{
		KLoaderShareMem::Inst()->GwSetMem(n, emKLSMK_LOADER_INFO, &aInfo[n], sizeof(KLOADER_INFO));
	}

	KMemPipeSys::Inst()->GwInit(pPak->byTotalLoaderCount);

	KModuleSetting::Inst()->GwInit(pPak->nModuleCount, asModuleSettings);

	KG_Printl("Prepare to start " << (INT)pPak->byLocalLoaderCount << " Loader(s)...");
	// 开始启动Loader
	KLoaderManager::Inst()->StartLoaders(pPak->byLocalLoaderIdBegin, pPak->byLocalLoaderCount);
	return TRUE;
}

BOOL KGcMonitorProcess::OnUpdateModuleInfo( LPCBYTE pData, UINT uDataLen )
{
	//KGCM2GCGW_UPDATE_MODULE_INFO* pPak = (KGCM2GCGW_UPDATE_MODULE_INFO*)pData;
	// all Loaders UpdateInterconnectionInfo(pPak->byModuleId, pPak->dwGcGatewayIp, pPak->uGcGatewayPort);
	return TRUE;
}

BOOL KGcMonitorProcess::OnModuleLoaded( BYTE byModuleId, USHORT uPortGs )
{
	KGC_CONFIRM_RET_FALSE(byModuleId > emKGCMODULE_LOGICBASE && byModuleId < emKGCMODULE_ALL);
	KGC_CONFIRM_RET_FALSE(uPortGs);

	IKG_Buffer* piBuffer = KG_MemoryCreateBuffer(sizeof(KGCGW2GCM_LOAD_MODULE_RESULT));
	KGCGW2GCM_LOAD_MODULE_RESULT* pPak = (KGCGW2GCM_LOAD_MODULE_RESULT*)piBuffer->GetData();
	KGC_CONFIRM_RET_FALSE(piBuffer);

	pPak->byProtocol = emKGCGW2GCM_LOAD_MODULE_RESULT;
	pPak->byModuleId = byModuleId;
	pPak->byResult = TRUE;
	pPak->uPortGs = uPortGs;

	g_piGcMonitorClient->Send(piBuffer->GetData(), piBuffer->GetSize());
	return TRUE;
}
// -------------------------------------------------------------------------