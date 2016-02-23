/* -------------------------------------------------------------------------
//	文件名		：	kgclogicprovide.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/7/14 9:55:38
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "serverbase/kgcmoduledef.h"
#include "kgclogicprovide.h"
#include "ksocketserver.h"
#include "kinterfacemgr.h"
#include "gclogicbase/kgclogicbaseprotocol.h"
#include "kgc_coordinatorclient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KGcLogicProvide::KGcLogicProvide(void)
{
}

KGcLogicProvide::~KGcLogicProvide(void)
{
}

KGcLogicProvide* KGcLogicProvide::Inst()
{
	static KGcLogicProvide s_Object;
	return &s_Object;
}

BOOL KGcLogicProvide::RegisterExtInterface( REFIID rrid, LPVOID pvObject )
{
	return KInterfaceMgr::Inst()->RegisterInterface(rrid, pvObject);
}

BOOL KGcLogicProvide::Send( INT nConnectId, LPVOID pvData, UINT uDataSize )
{
	KGCLOGIC_BASE_PTC_HEADER* pHeader = (KGCLOGIC_BASE_PTC_HEADER*)pvData;
	pHeader->byFamiliy = emKGCMODULE_LOGICBASE;
	IMemBlock* piBuffer = QCreateMemBlock(uDataSize);
	memcpy(((LPBYTE)piBuffer->GetData()), pvData, uDataSize);
	BOOL bRet = KSocketServer::Inst()->SendPacket(nConnectId, piBuffer);
	SAFE_RELEASE(piBuffer);
	return bRet;
}

BOOL KGcLogicProvide::RegisterGsProcessor( IKProcessServer* pIServer )
{
	return KSocketServer::Inst()->RegisterGsProcessor(emKGCMODULE_LOGICBASE, pIServer);
}

BOOL KGcLogicProvide::Send2OtherGc( INT nModuleId, LPVOID pData, UINT uDataSize )
{
	IMemBlock* piBuffer = QCreateMemBlock(uDataSize);
	memcpy(((LPBYTE)piBuffer->GetData()), pData, uDataSize);
	BOOL bRet = KGC_CoordinatorClient::Inst().Send(piBuffer);
	SAFE_RELEASE(piBuffer);
	return bRet;
}

// -------------------------------------------------------------------------
