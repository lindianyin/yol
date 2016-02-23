/* -------------------------------------------------------------------------
//	文件名		：	kinterfacemgr.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/7/14 10:40:59
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "serverbase/kgcmoduleinterface.h"
#include "kdbmanager.h"
#include "kinterfacemgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
BOOL KInterfaceMgr::Init()
{
	// 添加接口
	m_mpInterface[IID_IKG_GCDatabase] = KDbManager::Inst();
	return TRUE;
}

BOOL KInterfaceMgr::RegisterInterface( REFIID riid, LPVOID pvObject )
{
	return m_mpInterface.insert(MP_INTERFACE::value_type(riid, pvObject)).second;
}

LPVOID KInterfaceMgr::GetInterface( REFIID riid )
{
	MP_INTERFACE::iterator it = m_mpInterface.find(riid);
	if (it == m_mpInterface.end())
		return NULL;
	return it->second;
}

KInterfaceMgr* KInterfaceMgr::Inst()
{
	static KInterfaceMgr s_Object;
	return &s_Object;
}
// -------------------------------------------------------------------------
