/* -------------------------------------------------------------------------
//	文件名		：	kdoodadmgr.cpp
//	创建者		：	simon
//	创建时间	：	2010/3/11 11:16:35
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegameworld/krequireinterface.h"
#include "config/kconfigfile.h"
#include "onlinegameworld/kdoodadmgr.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/korpgworld.h"
#include "kplayerbaseprocess.h"
#include "onlinegamebase/kscriptmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KDoodadMgr g_cDoodadMgr;


// -------------------------------------------------------------------------

KDoodadMgr::KDoodadMgr(void)
{
}

KDoodadMgr::~KDoodadMgr(void)
{
}

BOOL KDoodadMgr::Init( )
{
	const KENV_CONFIG* pConfig = g_piNsBaseRequire->GetEnvConfig();
	QCONFIRM_RET_FALSE(pConfig);

	m_cObjPool.Init(pConfig->nMaxDoodad);

	/*
	BOOL bRet = ReadDoodadTemplate();
	*/

	BOOL bRet = LoadDoodadTemplate();
	QCONFIRM_RET_FALSE(bRet && "LoadDoodadTemplate Failed!");

	return TRUE;
}

BOOL KDoodadMgr::AddToScene(KDoodad* pstDoodad, DWORD dwSceneId, INT nX, INT nY, INT nZ)
{
	BOOL bResult = FALSE;
	IKScene* pScene = NULL;
	INT nIdx = 0;

	PROCESS_ERROR(pstDoodad);
	PROCESS_ERROR(pScene = g_pSceneMgr->GetScene(dwSceneId));

	PROCESS_ERROR(pScene->AddDoodad(pstDoodad, nX, nY, nZ));

	g_cScriptManager.GetSafeScript()->CallTableFunction("Doodad", "OnEnterScene", 1, "os",
		pstDoodad->GetScriptInterface(), pstDoodad->m_strClass.c_str());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KDoodadMgr::CheckAutoPick(KDoodad* pDoodad)
{
	BOOL bResult = FALSE;
	KPlayer* pPlayer = NULL;

	pPlayer = g_cPlayerMgr.GetById(pDoodad->m_dwOwnerId);
	PROCESS_ERROR(pPlayer);

	// 经验直接给
	if(pDoodad->m_nGenre == 18 && pDoodad->m_nDetail == 1 && pDoodad->m_nParticular == 222)
	{
		g_cPlayerBaseProcess.DoPickUpDoodad(pPlayer, pDoodad->GetId(), FALSE);

		bResult = TRUE;
	}

EXIT0:
	return bResult;
}

BOOL KDoodadMgr::RemoveFromScene( DWORD dwId )
{
	BOOL bResult = FALSE;
	KDoodad* pstDoodad = NULL;
	IKScene* pScene = NULL;

	PROCESS_ERROR(pstDoodad = m_cObjPool.GetById(dwId));
	PROCESS_ERROR(pScene = pstDoodad->GetScene());
	PROCESS_ERROR(pScene->RemoveDoodad(pstDoodad));

	g_cScriptManager.GetSafeScript()->CallTableFunction("Doodad", "OnLeaveScene", 1, "os",
		pstDoodad->GetScriptInterface(), pstDoodad->m_strClass.c_str());

	bResult = TRUE;
EXIT0:
	return bResult;
}

KDoodad* KDoodadMgr::CreateDoodad(DWORD dwTemplateId, DWORD dwId /*= KD_BAD_ID*/ )
{
	BOOL bResult = FALSE;
	INT nIndex = 0;
	KDoodad* pDoodad = NULL;

	pDoodad = m_cObjPool.GetById(dwId);
	if (pDoodad)
	{
		nIndex = pDoodad->m_nIndex;
		IKScene* pScene = pDoodad->GetScene();
		if (pScene)
			pScene->RemoveDoodad(pDoodad);
	}
	else
	{
		KDoodadTemplateMapIt stIt = m_mapDoodadTemplate.find(dwTemplateId);
		LOG_PROCESS_ERROR(stIt != m_mapDoodadTemplate.end());

		nIndex = m_cObjPool.Add(dwId);
		LOG_PROCESS_ERROR(pDoodad = m_cObjPool.GetByIndex(nIndex));
		LOG_PROCESS_ERROR(pDoodad->Init(nIndex, &stIt->second));
	}

	bResult = TRUE;
EXIT0:
	if (FALSE == bResult)
	{
		if (pDoodad)
		{
			DestroyDoodad(pDoodad->m_dwId);
			pDoodad = NULL;
		}
	}
	return pDoodad;
}

BOOL KDoodadMgr::DestroyDoodad( DWORD dwId )
{
	BOOL bResult = FALSE;
	KDoodad* pstDoodad = NULL;

	PROCESS_ERROR(pstDoodad = m_cObjPool.GetById(dwId));

	pstDoodad->Uninit();
	m_cObjPool.Remove(dwId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KDoodadMgr::NotifyRemoveFromScene( DWORD dwId )
{
	BOOL bResult = FALSE;
	KDoodad* pstDoodad = NULL;

#ifdef GAME_SERVER
	PROCESS_ERROR(pstDoodad = GetById(dwId));
	if (pstDoodad->m_eState == emKDOODADSTATE_OPING)
	{
		pstDoodad->CancelDialog(pstDoodad->m_pCurrPlayer);
	}
	// 同步需要取场景，必须放在Uninit之前
	g_cPlayerBaseProcess.DoSyncLeave(*pstDoodad, TRUE);
#endif

	bResult = TRUE;
#ifdef GAME_SERVER
EXIT0:
#endif
	return bResult;
}

BOOL KDoodadMgr::RemoveDoodad( DWORD dwId )
{
	BOOL bRet;
	bRet = g_cDoodadMgr.NotifyRemoveFromScene(dwId);
	QCONFIRM_RET_FALSE(bRet);
	bRet = g_cDoodadMgr.RemoveFromScene(dwId);
	QCONFIRM_RET_FALSE(bRet);
	bRet = g_cDoodadMgr.DestroyDoodad(dwId);
	QCONFIRM_RET_FALSE(bRet);
	return TRUE;
}

BOOL KDoodadMgr::RemoveDoodadNoScene( DWORD dwId )
{
	BOOL bRet;
	bRet = g_cDoodadMgr.DestroyDoodad(dwId);
	QCONFIRM_RET_FALSE(bRet);
	return TRUE;
}

BOOL KDoodadMgr::Uninit()
{
	BOOL bResult = TRUE;

	m_mapDoodadTemplate.clear();

	// 所有Doodad应该都在场景里，如果有例外需要在这里显式调用Remove函数
	m_cObjPool.Uninit();
	return TRUE;
}

BOOL KDoodadMgr::Activate()
{
	BOOL bResult = FALSE;

	bResult = TRUE;
	return bResult;
}

KDoodad* KDoodadMgr::GetById( DWORD dwId )
{
	return m_cObjPool.GetById(dwId);
}

KDoodad* KDoodadMgr::GetByIndex( INT nIndex )
{
	return m_cObjPool.GetByIndex(nIndex);
}

BOOL KDoodadMgr::LoadDoodadTemplate()
{
	BOOL bResult = FALSE;

	ITabFile* pFile = g_OpenTabFile(KDF_DOODAD_LIST);
	LOG_PROCESS_ERROR(pFile);

	// 第一行行是中英文字段头描述
	for (INT n = 2; n <= pFile->GetHeight(); ++n)
	{
		INT nTmp = 0;
		KDOODAD_TEMPLATE sDoodadInfo;
		CHAR szBuf[MAX_NAME_LEN] = "";

		bResult = pFile->GetInteger(n, "TemplateId", KD_BAD_ID, &nTmp);
		sDoodadInfo.dwTemplateId = (DWORD)nTmp;

		bResult = pFile->GetString(n, "DisplayName", "", szBuf, countof(szBuf));
		sDoodadInfo.strDisplayName = szBuf;

		bResult = pFile->GetString(n, "Class", "", szBuf, countof(szBuf));
		sDoodadInfo.strClassName = szBuf;

		bResult = pFile->GetInteger(n, "Visible", KD_BAD_ID, &nTmp);
		sDoodadInfo.nVisibleMode = (DWORD)nTmp;

		bResult = pFile->GetInteger(n, "RepresentId", KD_BAD_ID, &nTmp);
		sDoodadInfo.dwRepresentId = (DWORD)nTmp;

		bResult = pFile->GetInteger(n, "ActiveTime", 0, &nTmp);
		sDoodadInfo.dwActiveTime = (DWORD)nTmp;

		QCONFIRM_RET_FALSE(m_mapDoodadTemplate.insert(std::make_pair(sDoodadInfo.dwTemplateId, sDoodadInfo)).second);
	}

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(pFile);
	return bResult;
}

CONST KDOODAD_TEMPLATE* KDoodadMgr::GetTemplateInfo( DWORD dwTemplateID )
{
	KDoodadTemplateMapIt it = m_mapDoodadTemplate.find(dwTemplateID);
	if (it != m_mapDoodadTemplate.end())
	{
		return &it->second;
	}
	return NULL;
}
