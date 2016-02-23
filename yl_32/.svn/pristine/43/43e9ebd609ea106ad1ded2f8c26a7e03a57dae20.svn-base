/* -------------------------------------------------------------------------
//	文件名		：	kconnectmgr.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/9/10 15:29:47
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kconnectmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KConnectMgr::KConnectMgr(void)
{
	ZeroStruct(m_aConnect);
}

KConnectMgr* KConnectMgr::Inst()
{
	static KConnectMgr _sInst;
	return &_sInst;
}

BOOL KConnectMgr::SetConnect( BYTE byConnectorModuleId, KConnectBase* pConnect )
{
	m_aConnect[byConnectorModuleId] = pConnect;
	return TRUE;
}

KConnectBase* KConnectMgr::GetConnect( BYTE byConnectorModuleId )
{
	return m_aConnect[byConnectorModuleId];
}
// -------------------------------------------------------------------------
