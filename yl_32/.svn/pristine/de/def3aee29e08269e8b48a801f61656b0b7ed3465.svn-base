
#include "stdafx.h"
#include "kfindpathmodule.h"
#include "onlinegamemodule/qfindpathprotocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
#ifdef _DEBUG
	#define KD_DLLNAME_NAV				"navlib_d.dll"
#else
	#define KD_DLLNAME_NAV				"navlib.dll"
#endif

#define KD_FUNNAME_LOADNAVMESH		"LoadNavMesh"
#define KD_FUNNAME_FINDPATH			"FindStraightPath"
#define KD_FUNNAME_GETPATH			"GetStraightPath"

template<>
IKModuleInterface* GetModuleInterFace<emKMODULE_FINDPATH>()
{
	KFindPathModule& rFindPathModule = GetFindPathModule();
	return &rFindPathModule;
}

KFindPathModule::KFindPathModule() : KModuleBase(emKMODULE_FINDPATH)

{
#ifdef GAME_CLIENT
	m_hNavModule     = 0;
	m_pFNLoadNavMesh = NULL;
	m_pFNFindPath    = NULL;
	m_pFNGetNavPath  = NULL;
#endif
}

KFindPathModule::~KFindPathModule()
{
}

BOOL KFindPathModule::OnInit()
{
#ifdef GAME_CLIENT
	m_hNavModule = LoadLibrary(KD_DLLNAME_NAV);
	LOG_PROCESS_ERROR(m_hNavModule);

	m_pFNLoadNavMesh = (FNLOADNAVMESH)::GetProcAddress(m_hNavModule, KD_FUNNAME_LOADNAVMESH);
	LOG_PROCESS_ERROR(m_pFNLoadNavMesh);
	
	m_pFNFindPath = (FNFINDPATH)::GetProcAddress(m_hNavModule, KD_FUNNAME_FINDPATH);
	LOG_PROCESS_ERROR(m_pFNFindPath);

	m_pFNGetNavPath = (FNGETNAVPATH)::GetProcAddress(m_hNavModule, KD_FUNNAME_GETPATH);
	LOG_PROCESS_ERROR(m_pFNGetNavPath);	

	return TRUE;

EXIT0:
	return FALSE;
#else
	return TRUE;
#endif
}

BOOL KFindPathModule::OnUnInit()
{
#ifdef GAME_CLIENT
	LOG_PROCESS_ERROR(m_hNavModule);
	FreeLibrary(m_hNavModule);
	m_hNavModule = 0;
	return TRUE;

EXIT0:
	return FALSE;
#else
	return TRUE;
#endif
}

BOOL KFindPathModule::InitProtocol()
{
    KD_PTC_FIXED_SIZE_S2C(QPTC_FINDPATH_S2C_AUTOGOTO);
	return TRUE;
}
