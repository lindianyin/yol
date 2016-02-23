/* -------------------------------------------------------------------------
//	文件名		：	kgcserver.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/6/1 16:51:48
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "ksocketserver.h"
#include "kgcserver.h"
#include "kloader.h"
#include "onlinegameworld/kprotocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KGcServer::KGcServer( BYTE byModuleId )
{
	m_byModuleId = byModuleId;
}

BOOL KGcServer::Send( INT nConnectId, LPVOID pvData, SIZE_T uDataSize )
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pvData;
	IMemBlock* piBuffer = QCreateMemBlock((UINT)uDataSize);
	pHeader->byProtocolFamily = m_byModuleId;
	memcpy(((LPBYTE)piBuffer->GetData()), pvData, uDataSize);
	BOOL bRet = KSocketServer::Inst()->SendPacket(nConnectId, piBuffer);
	SAFE_RELEASE(piBuffer);
	return bRet;
}

BOOL KGcServer::Send2Random(LPVOID pvData, UINT uDataSize)
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pvData;
	IMemBlock* piBuffer = QCreateMemBlock(uDataSize);
	pHeader->byProtocolFamily = m_byModuleId;
	memcpy(((LPBYTE)piBuffer->GetData()), pvData, uDataSize);
	BOOL bRet = KSocketServer::Inst()->Send2RandomPacket(piBuffer);
	SAFE_RELEASE(piBuffer);
	return bRet;
}

BOOL KGcServer::RegisterGsProcessor( IKProcessServer* piProcessor )
{
	return KSocketServer::Inst()->RegisterGsProcessor(m_byModuleId, piProcessor);
}

IKInterfaceAcceptor* KGcServer::RegisterGcAcceptor(IKProcessAcceptor* pIServer)
{
	return KLoader::Inst()->RegisterAcceptor(m_byModuleId, pIServer);
}

IKInterfaceConnector* KGcServer::RegisterGcConnector( BYTE byModuleId, IKProcessConnector* pIClient, BOOL bIsEssencial )
{
	return KLoader::Inst()->RegisterConnector(m_byModuleId, byModuleId, pIClient, bIsEssencial);
}
// -------------------------------------------------------------------------
