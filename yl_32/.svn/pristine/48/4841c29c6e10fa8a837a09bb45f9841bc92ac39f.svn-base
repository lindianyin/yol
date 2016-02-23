
#pragma once

#include "onlinegameworld/kmodulebase.h"
// -------------------------------------------------------------------------
typedef BOOL (*FNLOADNAVMESH)(CONST CHAR* szFilePath);
typedef BOOL (*FNFINDPATH)(const float* aStartPos, const float* aEndPos, size_t* uNavPathLen);
typedef BOOL (*FNGETNAVPATH)(float* aNavPath);

class KFindPathModule : public KModuleBase
{
public:
	KFindPathModule();
	virtual ~KFindPathModule();

public:
	BOOL OnInit();
	BOOL OnUnInit();

	BOOL InitProtocol();

#ifdef GAME_CLIENT	
	FNLOADNAVMESH GetFNLoadNavMesh() { return m_pFNLoadNavMesh; }
	FNFINDPATH    GetFNFindPath()    { return m_pFNFindPath; }
	FNGETNAVPATH  GetFNGetNavPath()  { return m_pFNGetNavPath; }

private:
	FNLOADNAVMESH m_pFNLoadNavMesh;
	FNFINDPATH    m_pFNFindPath;
	FNGETNAVPATH  m_pFNGetNavPath;
	HMODULE		  m_hNavModule;

#endif //#ifdef GAME_CLIENT
};

inline KFindPathModule& GetFindPathModule()
{
	static KFindPathModule s_mFindPathModule;
	return s_mFindPathModule;
}

