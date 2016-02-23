
#ifdef GAME_CLIENT
#include "stdafx.h"
#include "onlinegameworld/kplayermanager_client.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/krequireinterface.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/ikscene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KPlayerMgr g_cPlayerMgr;
// -------------------------------------------------------------------------
INT KPlayerMgr::Add(LPCSTR szName, DWORD dwPlayerId )
{
	KPlayer* pPlayer = m_cObjMgr.GetById(dwPlayerId);
	//ASSERT(NULL == pPlayer);

	INT nPlayerIndex = 0;

	if (pPlayer)
	{
		nPlayerIndex = pPlayer->GetPlayerIndex();

		printf("Duplicate Player[%d]!\n", dwPlayerId);
		IKScene* pScene = pPlayer->GetScene();
		if (pScene)
			pScene->RemovePlayer(pPlayer);
	}
	else
	{
		nPlayerIndex = m_cObjMgr.Add(dwPlayerId);

		pPlayer = m_cObjMgr.GetById(dwPlayerId);
		QCONFIRM_RET_FALSE(pPlayer);

		pPlayer->Init(nPlayerIndex, szName);

		// 名字到对象映射表
		m_mapNameToPlayer.insert(std::map<QString, KPlayer*>::value_type(szName, pPlayer));

		g_cOrpgWorld.m_cEventCreator.FireEvent(emKOBJEVENTTYPE_GAMEWORLD_ADD_PLAYER, dwPlayerId, 0, 0);
		Q_Printl("Player[" << pPlayer->m_dwId << "] " << szName << " added.");
	}
	return nPlayerIndex;
}

BOOL KPlayerMgr::Init()
{
	const KENV_CONFIG* pConfig = g_piNsBaseRequire->GetEnvConfig();
	m_cObjMgr.Init(pConfig->nMaxPlayer);
	m_mapNameToPlayer.clear();

	return TRUE;
}

KPlayer* KPlayerMgr::GetByIndex( INT nPlayerIndex )
{
	return m_cObjMgr.GetByIndex(nPlayerIndex);
}

BOOL KPlayerMgr::RemoveById( DWORD dwId )
{
	KPlayer* pPlayer = m_cObjMgr.GetById(dwId);
	QCONFIRM_RET_FALSE(pPlayer);

	QLogPrintf(LOG_DEBUG, "Player[%d] %s removing", pPlayer->m_dwId, pPlayer->GetName());

	g_cOrpgWorld.m_cEventCreator.FireEvent(emKOBJEVENTTYPE_GAMEWORLD_REMOVE_PLAYER, dwId, 0, 0);
	pPlayer->FireEvent(emKOBJEVENTTYPE_CHARACTER_LEAVE_SCENE, dwId, FALSE, 0);
	// 擦除映射表
	m_mapNameToPlayer.erase(pPlayer->GetName());
	pPlayer->Uninit();

	return m_cObjMgr.Remove(dwId);
}

KPlayer* KPlayerMgr::GetById( DWORD dwId )
{
	return m_cObjMgr.GetById(dwId);
}

BOOL KPlayerMgr::Uninit()
{
	m_mapNameToPlayer.clear();
    m_cObjMgr.Uninit();
	return TRUE;
}

BOOL KPlayerMgr::Activate()
{
	return TRUE;
}

KPlayer* KPlayerMgr::GetByName(const QString& strName )
{
	KPlayer* pPlayer = NULL;

	std::map<QString, KPlayer*>::const_iterator it = m_mapNameToPlayer.find(strName);
	if (it != m_mapNameToPlayer.end())
		pPlayer = it->second;

	return pPlayer;
}

INT KPlayerMgr::GetAllPlayers(std::vector<KPlayer*>& rvPlayers)
{
	INT nResult = 0;
	INT nPlayerIndex = 0;

	while ((nPlayerIndex = m_cObjMgr.NextUsedIndex(nPlayerIndex)) > 0)
	{
		KPlayer* pPlayer = m_cObjMgr.GetByIndex(nPlayerIndex);
		LOG_PROCESS_ERROR(pPlayer);
		rvPlayers.push_back(pPlayer);
	}

	nResult = 1;
EXIT0:
	return nResult;
}
// -------------------------------------------------------------------------
#endif
