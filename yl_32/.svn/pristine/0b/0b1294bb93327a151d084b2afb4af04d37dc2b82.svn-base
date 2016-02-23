#include "stdafx.h"

#include "onlinegameworld/scene_def.h"
#include "kscenemgr.h"
#include "kregion.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kobjeventsetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------
KSceneMgr	g_SceneMgr;
IKSceneMgr*	g_pSceneMgr = &g_SceneMgr;


struct KSceneActivator
{
	BOOL operator()(DWORD dwSceneId, KScene* pScene)
	{
		pScene->Activate();
		return TRUE;
	}
};


// --------------------------------------------------------------------
KSceneMgr::KSceneMgr(void)
{
}

KSceneMgr::~KSceneMgr(void)
{
}

BOOL KSceneMgr::Init()
{
	BOOL	bResult	= FALSE;
	
	bResult = _ReadSceneList();
	//QCONFIRM_RET_FALSE(bResult);

	bResult = m_PatrolPathList.Init();
	QCONFIRM_RET_FALSE(bResult);

	return bResult;
}

BOOL KSceneMgr::UnloadScene(INT nMapId, INT nMapCopyIndex/* = 0*/)
{
	BOOL	bRet = FALSE;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	KScene* pScene = (KScene*)GetScene(nMapId, nMapCopyIndex);
	LOG_PROCESS_ERROR(pScene);

	// added by liyun 这里需要通知lua脚本
	cSafeScript->CallTableFunction("Scene", "OnUnLoadScene", 0, "dd", pScene->GetTemplateId(), pScene->GetId());

	pScene->Uninit();
	m_cSceneSet.Unregister(pScene);
	SAFE_DELETE(pScene);

	bRet	= TRUE;
EXIT0:
	return bRet;
}

BOOL KSceneMgr::Uninit()
{
	UnloadAllScenes();

	m_cSceneCfgList.clear();

	m_PatrolPathList.UnInit();
	return TRUE;
}

KScene* KSceneMgr::GetScene(INT nMapId, INT nMapCopyIndex/* = 0*/)
{
	//QLogPrintf(LOG_ERR,"..KSceneMgr::GetScene(INT nMapId=%d, INT nMapCopyIndex=%d)",nMapId,nMapCopyIndex);
	KScene* pScene = NULL;
	KScene* pSceneOut = NULL;
	INT nCombinedMapId = (nMapId | (nMapCopyIndex << 16));
	INT nMapTemplateId = nMapId & 0xFFFF;

	pScene = m_cSceneSet.FindFirst();
	PROCESS_ERROR(pScene);

	while (pScene)
	{
		if ((nCombinedMapId & 0xFFFF0000) == 0 && pScene->GetTemplateId() == nMapTemplateId)
		{
			pSceneOut = pScene;
			break;
		}
		if (pScene->GetId() == nCombinedMapId)
		{
			pSceneOut = pScene;
			break;
		}

		pScene = m_cSceneSet.FindNext();
	}

EXIT0:
	return pSceneOut;
}

IKScene* KSceneMgr::FirstScene()
{
	return m_cSceneSet.FindFirst();
}

IKScene* KSceneMgr::NextScene()
{
	return m_cSceneSet.FindNext();
}

BOOL KSceneMgr::_ReadSceneList()
{
	BOOL	bResult	= FALSE;

	QTabFile cTabMapList;
	INT nTabFileHeight;
	INT nTemplateId = 0;
	KSCENE_CONFIG kSceneCfg;
	INT nFightMap = 0;
	INT nCanTeam = 0;
	INT nSetFightMode = 0;

	LOG_PROCESS_ERROR(cTabMapList.Load(SCENE_LIST_FILE));

	nTabFileHeight = cTabMapList.GetHeight();
	for (int i = 2; i <= nTabFileHeight; ++i)
	{
		cTabMapList.GetInteger(	i,	"Id",	0, &nTemplateId);
		if (nTemplateId == 0)
			continue;

		cTabMapList.GetString(	i,	"Class",	"", kSceneCfg.szClass,	sizeof(kSceneCfg.szClass));
		cTabMapList.GetString(	i,	"SceneType", "", kSceneCfg.szSceneType,	sizeof(kSceneCfg.szSceneType));
		cTabMapList.GetString(	i,	"Path",	"", kSceneCfg.szPath,	sizeof(kSceneCfg.szPath));
		cTabMapList.GetString(	i,	"Name",	"", kSceneCfg.szName,	sizeof(kSceneCfg.szName));
		cTabMapList.GetInteger(	i,	"BroadcastRange",	1, &kSceneCfg.nBroadcastRange);
		cTabMapList.GetInteger( i,  "CanPK",   0, &kSceneCfg.bCanPK);
		cTabMapList.GetInteger( i,  "Type",   0, &kSceneCfg.nType);
		
		cTabMapList.GetInteger( i, "IsFightMap", 0, &nFightMap);
		cTabMapList.GetInteger( i, "CanTeam", 0, &nCanTeam);
		cTabMapList.GetInteger(i,"IsSetFightMode",0, &nSetFightMode);

		kSceneCfg.bFightMap = (BYTE)nFightMap;
		kSceneCfg.dwTemplateId = (DWORD)nTemplateId;
		kSceneCfg.bCanTeam = (BYTE)nCanTeam;
		kSceneCfg.bSetFightMode = (BYTE)nSetFightMode;

		if (m_cSceneCfgList.find(kSceneCfg.dwTemplateId) != m_cSceneCfgList.end())
		{
			//Q_Error("Duplicate Scene TemplateId[" << kSceneCfg.dwTemplateId << "]!");		// 模板ID重复了
			continue;
		}
		m_cSceneCfgList[kSceneCfg.dwTemplateId] = kSceneCfg;
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}


CONST KSCENE_CONFIG* KSceneMgr::GetSceneInfo( DWORD dwMapTemplateId )
{
	KSCENE_CONFIG_LIST::iterator it = m_cSceneCfgList.find(dwMapTemplateId);

	if (it != m_cSceneCfgList.end())
		return &it->second;
	else
		return NULL;
}

BOOL KSceneMgr::IsFightMap(DWORD dwMapTemplateId)
{
	CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(dwMapTemplateId);
	QCONFIRM_RET_NULL(pCfg);

	return (BOOL)pCfg->bFightMap;
}

INT KSceneMgr::GetSceneType(DWORD dwMapTemplateId)
{
	CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(dwMapTemplateId);
	QCONFIRM_RET_NULL(pCfg);

	return pCfg->nType;
}

BOOL KSceneMgr::CanTeam(DWORD dwMapTemplateId)
{
	CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(dwMapTemplateId);
	QCONFIRM_RET_NULL(pCfg);

	return (BOOL)pCfg->bCanTeam;
}

BOOL KSceneMgr::IsSetFightMode(DWORD dwMapTemplateId)
{
	CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(dwMapTemplateId);
	QCONFIRM_RET_NULL(pCfg);

	return (BOOL)pCfg->bSetFightMode;
}

BOOL KSceneMgr::Activate()
{
	KScene* pScene = NULL;
	KTerrainScene* pTerrainScene = NULL;
	KSceneActivator cSceneTraverse;

	// 激活加载的每个场景
	m_cSceneSet.Traverse(cSceneTraverse);

	///////////////////////////////////检查并释放共享地形///////////////////////////////////////
	pTerrainScene = m_cTerrainCache.FindFirst();

	while (pTerrainScene)
	{
		KTerrainScene* pNextTerrainScene = m_cTerrainCache.FindNext();

		if (pTerrainScene->CanUnloadNow())
		{
			Q_Printl("Unloading Shared Terrain Scene [" << pTerrainScene->m_dwId << "]...");

			m_cTerrainCache.Unregister(pTerrainScene);
			SAFE_RELEASE(pTerrainScene);
		}
		pTerrainScene = pNextTerrainScene;
	}
EXIT0:
	return TRUE;
}

INT KSceneMgr::GetCount()
{
	return m_cSceneSet.GetObjCount();
}

BOOL KSceneMgr::UnloadAllScenes()
{
	///////////////////////////////////释放所有场景///////////////////////////////////////
	KScene* pScene = m_cSceneSet.FindFirst();

	while (pScene)
	{
		m_cSceneSet.Unregister(pScene);
		SAFE_DELETE(pScene);
		pScene = m_cSceneSet.FindNext();
	}

	///////////////////////////////////释放所有共享地形///////////////////////////////////////
	KTerrainScene* pTerrainScene = m_cTerrainCache.FindFirst();

	while (pTerrainScene)
	{
		KTerrainScene* pNextTerrainScene = m_cTerrainCache.FindNext();

		Q_Printl("Unloading Shared Terrain Scene [" << pTerrainScene->m_dwId << "]...");

		m_cTerrainCache.Unregister(pTerrainScene);
		SAFE_RELEASE(pTerrainScene);

		pTerrainScene = pNextTerrainScene;
	}

	return TRUE;
}

KScene* KSceneMgr::LoadScene(INT nMapId, INT nMapCopyIndex)
{
	BOOL	bRet = FALSE;
	KScene* 	pScene 				= NULL;
	KTerrainScene* pTerrain 		= NULL;
	CONST KSCENE_CONFIG* pSceneCfg 	= NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	//LOG_PROCESS_ERROR(nMapId > 0 && nMapCopyIndex > 0);
	
	pSceneCfg = GetSceneInfo(nMapId);
	if (pSceneCfg)
	{
		QLogPrintf(LOG_DEBUG, "Loading scene [%d][%s] template[%d]", nMapCopyIndex, pSceneCfg->szName, nMapId);
	}
	else
	{
		QLogPrintf(LOG_DEBUG, "Can't load scene [%d] template[%d] not found", nMapCopyIndex, nMapId);
	}
	LOG_PROCESS_ERROR(pSceneCfg);

	pScene = m_cSceneSet.GetObj(nMapId | nMapCopyIndex << 16);
	LOG_PROCESS_ERROR(pScene == NULL);

	pScene = new KScene;
	LOG_PROCESS_ERROR(pScene);

	bRet = pScene->Init(*pSceneCfg);
	LOG_PROCESS_ERROR(bRet);

	// Find or Create TerrainScene for it 
	pTerrain = m_cTerrainCache.GetObj(nMapId);
	if (! pTerrain)
	{
		pTerrain = KTerrainScene::CreateInstance();
		LOG_PROCESS_ERROR(pTerrain);

		bRet = m_cTerrainCache.Register(pTerrain, nMapId);
		LOG_PROCESS_ERROR(bRet);
	}

	pTerrain->AddRef();
	pScene->m_pTerrainScene = pTerrain;
	pScene->m_nCopyIndex = nMapCopyIndex;

	bRet = m_cSceneSet.Register(pScene, nMapId | (nMapCopyIndex << 16));
	LOG_PROCESS_ERROR(bRet);

	// Initiating Terrain
	bRet = pScene->Load();
	LOG_PROCESS_ERROR(bRet);

#ifdef GAME_SERVER
	cSafeScript->CallTableFunction("Scene", "OnLoadScene", 0, "dds",
		nMapId, pScene->GetId(), pSceneCfg->szPath);
#endif // GAME_SERVER

	Q_Printl("Scene Loaded.\t[" << pScene->m_vNpcCfg.size()
		<< "] Npc(s)\t[" << pScene->m_vTrapCfg.size() << "] Trap(s)");

	bRet = TRUE;
EXIT0:
	if (!bRet)
	{
		if (pTerrain)
			m_cTerrainCache.Unregister(pTerrain);
		if (pScene)
			m_cSceneSet.Unregister(pScene);

		SAFE_RELEASE(pTerrain);
		SAFE_DELETE(pScene);
	}
	return pScene;
}
