/* -------------------------------------------------------------------------
//	文件名		：	kgcquery.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/6/1 16:43:40
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kgcquery.h"
#include "kinterfacemgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KGcQuery::KGcQuery( BYTE byModuleId ) : m_cGcServer(byModuleId)
{
}

BOOL KGcQuery::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void **ppvObject )
{
	if (riid == IID_IKG_GCServer)
	{
		*ppvObject = &m_cGcServer;
	}
	else // 静态接口
	{
		*ppvObject = KInterfaceMgr::Inst()->GetInterface(riid);
		QCONFIRM_RET_FALSE(*ppvObject);
	}
	return TRUE;
}

// -------------------------------------------------------------------------
