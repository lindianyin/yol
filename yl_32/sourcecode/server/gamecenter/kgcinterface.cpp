/* -------------------------------------------------------------------------
//	文件名		：	kgcinterface.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/6/9 15:02:24
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kconnectmgr.h"
#include "kgcinterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KInterfaceAcceptor::KInterfaceAcceptor() : m_byOwnerModuleId(0)
{
}

KInterfaceAcceptor::KInterfaceAcceptor( BYTE byOwnerModuleId ) :
	m_byOwnerModuleId(byOwnerModuleId)
{
}

BYTE KInterfaceAcceptor::GetOwnerModuleId()
{
	return m_byOwnerModuleId;
}

BOOL KInterfaceAcceptor::Send( BYTE byConnectorModuleId, LPVOID pvData, UINT uDataSize )
{
	KConnectBase* pConnect = KConnectMgr::Inst()->GetConnect(byConnectorModuleId);
	QCONFIRM_RET_FALSE(pConnect);
	return pConnect->Send(m_byOwnerModuleId, byConnectorModuleId, pvData, uDataSize);
}

BOOL KInterfaceAcceptor::IsConnectValid( BYTE byConnectorModuleId )
{
	return TRUE;
}
// -------------------------------------------------------------------------
KInterfaceConnector::KInterfaceConnector() : m_byAcceptorModuleId(0), m_byOwnerModuleId(0)
{
	m_bEssencial = FALSE;
}

KInterfaceConnector::KInterfaceConnector( BYTE byAcceptorModuleId, BYTE byOwnerModuleId, BOOL bEssencial ) :
	m_byAcceptorModuleId(byAcceptorModuleId), m_byOwnerModuleId(byOwnerModuleId), m_bEssencial(bEssencial)
{
}

BYTE KInterfaceConnector::GetAcceptorModuleId()
{
	return m_byAcceptorModuleId;
}

BYTE KInterfaceConnector::GetOwnerModuleId()
{
	return m_byOwnerModuleId;
}

BOOL KInterfaceConnector::Send( LPVOID pvData, UINT uDataSize )
{
	KConnectBase* pConnect = KConnectMgr::Inst()->GetConnect(m_byOwnerModuleId);
	// 如果连接无效，且不是必要连接则不发送，必要连接仍然可以发送，数据会放到缓冲区等待连接重新生效
	if (!pConnect || (!pConnect->IsValid() && !m_bEssencial))
		return FALSE;
	// Connector的connectid最高位置1，以为Acceptor分开
	return pConnect->Send(m_byAcceptorModuleId, m_byOwnerModuleId | 0x80, pvData, uDataSize);
}

BOOL KInterfaceConnector::IsConnectValid()
{
	KConnectBase* pConnect = KConnectMgr::Inst()->GetConnect(m_byAcceptorModuleId);
	return pConnect && pConnect->IsValid();
}

BOOL KInterfaceConnector::IsEssencial()
{
	return m_bEssencial;
}
// -------------------------------------------------------------------------
