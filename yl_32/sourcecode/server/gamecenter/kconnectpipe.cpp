/* -------------------------------------------------------------------------
//	文件名		：	kconnectpipe.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/7/28 9:59:28
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kmempipesys.h"
#include "kconnectpipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KConnectPipe::KConnectPipe(BYTE byLoaderId) : KConnectBase(emKGC_CONNECT_PIPE)
{
	m_byLoaderId = byLoaderId;
	m_bIsValid = FALSE;
}

BOOL KConnectPipe::Send( BYTE byAcceptorModuleId, BYTE byConnectId, PVOID pvData, UINT uDataSize )
{
	return KMemPipeSys::Inst()->WriteInterfaceData((INT)m_byLoaderId,
		byConnectId, byAcceptorModuleId, pvData, uDataSize);
}
// -------------------------------------------------------------------------

KConnectPipeMgr* KConnectPipeMgr::Inst()
{
	static KConnectPipeMgr _sInst;
	return &_sInst;
}

KConnectPipe* KConnectPipeMgr::NewConnect(BYTE byLoaderId)
{
	m_lstConnect.push_back(KConnectPipe(byLoaderId));
	return &m_lstConnect.back();
}
// -------------------------------------------------------------------------
