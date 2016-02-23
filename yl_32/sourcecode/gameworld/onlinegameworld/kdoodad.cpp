	/* -------------------------------------------------------------------------
//	文件名		：	kdoodad.cpp
//	创建者		：	simon
//	创建时间	：	2010/3/12 10:53:19
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegameworld/kdoodad.h"
#include "onlinegameworld/kluadoodad.h"
#include "onlinegameworld/kdoodadmgr.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "kplayerbaseprocess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
#pragma warning(disable : 4355)
#ifdef GAME_SERVER
KDoodad::KDoodad(void) : m_cDoodadServer(*this)
#else
KDoodad::KDoodad(void)
#endif // GAME_SERVER
{
	m_pLuna = new KLuaDoodad(this);
}
#pragma warning(default : 4355)

KDoodad::~KDoodad(void)
{
    SAFE_DELETE(m_pLuna);
}

BOOL KDoodad::Init(INT nIndex, KDOODAD_TEMPLATE* pDoodadTemplate)
{
	QCONFIRM_RET_FALSE(KSceneObject::Init());
	m_nIndex = nIndex;
	m_eState = emKDOODADSTATE_NORMAL;

	m_nVisibleMode = pDoodadTemplate->nVisibleMode;

#ifdef GAME_SERVER
	if(!pDoodadTemplate->dwActiveTime)
	{
		m_dwActiveTimeOut = emKDOODADCOUNT_LAST_TIME;
	}
	else
	{
		m_dwActiveTimeOut = pDoodadTemplate->dwActiveTime;
	}

	//m_dwRespawnCounter	= 0;
	m_pCurrPlayer		= NULL;
#endif // GAME_SERVER

	m_dwTemplateId = pDoodadTemplate->dwTemplateId;
	//m_dwRepresentId = pDoodadTemplate->dwRepresentId;
	//m_dwRespawnInterval = pDoodadTemplate->dwRespawnInterval;
	//m_bRemoveAfterDialog = pDoodadTemplate->bRemoveAfterDialog;
	m_strClass = pDoodadTemplate->strClassName;
	m_dwOwnerId = KD_BAD_ID;

	return TRUE;
}

BOOL KDoodad::Uninit()
{
	m_pCurrPlayer = NULL;
	m_eState = emKDOODADSTATE_NONE;

	KSceneObject::Uninit();
	return TRUE;
}

BOOL KDoodad::Activate()
{
	if ( ! KSceneObject::Activate())
		return FALSE;  // 可能这帧已经激活过

	// 所有Doodad不计算运动(悬浮)
	//KSceneObject::ProcessMove(); // 注意激活顺序

#ifdef GAME_SERVER
	m_cDoodadServer.SyncChangeRegion();

	if (m_dwActiveTimeOut != 0)
	{
		if (--m_dwActiveTimeOut <= 0)
		{
			IKScene* pScene = GetScene();
			if(pScene)
			{
				/*
				KPlayer* pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
				PROCESS_ERROR(pPlayer);
				g_cPlayerBaseProcess.DoPickUpDoodad(pPlayer, GetId());
				*/
				g_cDoodadMgr.RemoveDoodad(GetId());
			}
		}
	}

#endif // GAME_SERVER

EXIT0:
	return TRUE;
}

BOOL KDoodad::OnRemove()
{
	g_cDoodadMgr.NotifyRemoveFromScene(m_dwId);
	g_cDoodadMgr.RemoveFromScene(m_dwId);
	g_cDoodadMgr.DestroyDoodad(m_dwId);
	//return g_cDoodadMgr.RemoveFromScene(m_dwId);
	return TRUE;
}

BOOL KDoodad::DoDialog( KPlayer* pDialogPlayer )
{
	BOOL bResult = FALSE;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	//m_pCurrPlayer = pDialogPlayer;

	g_cScriptManager.SetMe((QLunaBase*)pDialogPlayer->GetScriptInterface());
	g_cScriptManager.SetIt(m_pLuna);

	cSafeScript->CallTableFunction("Doodad", "OnDialog", 1, "s", m_strClass.c_str());

	g_cScriptManager.SetMe(NULL);
	g_cScriptManager.SetIt(NULL);

	bResult = cSafeScript->GetInt(-1);
	if (!bResult)
	{
		QLogPrintf(LOG_ERR, "Doodad:OnDialog [%u][%s] failed!", GetId(), m_strClass.c_str());
		goto EXIT0;
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KDoodad::CancelDialog( KPlayer* pDialogPlayer )
{
	if (pDialogPlayer != m_pCurrPlayer)
	{
		return ;
	}

	m_pCurrPlayer = NULL;
}

BOOL KDoodad::IsMemberVisible(KPlayer* pPlayer, KPlayer* pMember)
{
	BOOL bVisiable = TRUE;
	// 如果ownerId为0则大家都能看到
	PROCESS_ERROR(m_dwOwnerId != KD_BAD_ID);

	switch(m_nVisibleMode)
	{
	case emDoodadVisibleSelf:
		if(pMember->GetId() != pPlayer->GetId())
			bVisiable = FALSE;
		break;
	case emDoodadVisibleTeam:
		// 自己肯定能看到
		if(pMember->GetId() == pPlayer->GetId())
		{
			break;
		}

		// 别人teamId为0肯定看不到
		if(pMember->GetTeamId() == 0)
		{
			bVisiable = FALSE;
			break;
		}

		if(pMember->GetTeamId() != pPlayer->GetTeamId())
		{
			bVisiable = FALSE;
			break;
		}
		
		break;
	case emDoodadVisibleTong:
		// 自己肯定能看到
		if(pMember->GetId() == pPlayer->GetId())
		{
			break;
		}

		// 无家族不可见
		if(pMember->GetKinId() == 0)
		{
			bVisiable = FALSE;
			break;
		}

		if(pMember->GetKinId() != pPlayer->GetKinId())
		{
			bVisiable = FALSE;
			break;
		}
		break;
	case emDoodadVisibleCamp:
		if(pMember->GetCurrentCamp() != pPlayer->GetCurrentCamp())
			bVisiable = FALSE;
		break;
	case emDoodadVisibleAll:
		break;
	case emDoodadVisibleAuto:
		// 自己肯定能看到
		if(pMember->GetId() == pPlayer->GetId())
		{
			break;
		}

		// 无家族无队伍不可见
		if(pMember->GetKinId() == 0 && pMember->GetTeamId() == 0)
		{
			bVisiable = FALSE;
			break;
		}

        // 优先判断队伍
		if(pMember->GetTeamId() != pPlayer->GetTeamId())
		{
			bVisiable = FALSE;
			break;
		}
		// 判断家族
		if(pMember->GetKinId() != pPlayer->GetKinId())
		{
			bVisiable = FALSE;
			break;
		}
		break;
	}

EXIT0:
	return bVisiable;
}

BOOL KDoodad::InitVisibleList(std::vector<DWORD>& vecPlayers)
{
	BOOL bResult = FALSE;
	KPlayer* pPlayer = NULL;

	PROCESS_ERROR(m_dwOwnerId != KD_BAD_ID);

	pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
	LOG_PROCESS_ERROR(pPlayer);

	m_mapVisiblePlayers.clear();

	for(std::vector<DWORD>::iterator it = vecPlayers.begin(); it != vecPlayers.end(); ++it)
	{
		KPlayer* pMember = g_cPlayerMgr.GetById(*it);
		if(!pMember)
		{
			continue;
		}
		if(!IsMemberVisible(pPlayer, pMember))
		{
			continue;
		}
		
		m_mapVisiblePlayers[pMember->GetId()] = 1;
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

// 判断doodad对一个玩家是否可见
BOOL KDoodad::IsPlayerVisible(KPlayer* pPlayer)
{
	BOOL bVisible = FALSE;
	std::map<DWORD, INT>::iterator it;

	if(m_dwOwnerId == KD_BAD_ID)
	{
		bVisible = TRUE;
		goto EXIT0;
	}

	it = m_mapVisiblePlayers.find(pPlayer->GetId());
	if(it != m_mapVisiblePlayers.end())
	{
		bVisible = TRUE;
	}

EXIT0:
	return bVisible;
}