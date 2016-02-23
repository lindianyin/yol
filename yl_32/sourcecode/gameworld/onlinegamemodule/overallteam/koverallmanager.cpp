#include "stdafx.h"
#include "koverallmanager.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "../onlinegamemodule/instance/kinstancectrl.h"

KOverallManager g_OverallManager;

KOverallManager::KOverallManager()
{

}

KOverallManager::~KOverallManager()
{

}

BOOL KOverallManager::Init()
{
	return TRUE;
}

BOOL KOverallManager::UnInit()
{
	return TRUE;
}

VOID KOverallManager::AddPlayerToWatchTable(INT nPlayerId, INT nInstanceMapId)
{
	std::map<INT, INT>::iterator it = m_mapWatchTable.find(nPlayerId);
	if(it != m_mapWatchTable.end())
	{
		m_mapWatchTable.erase(it);
	}

	m_mapWatchTable[nPlayerId] = nInstanceMapId;
}

VOID KOverallManager::RemovePlayerFromWatchTable(INT nPlayerId)
{
	std::map<INT, INT>::iterator it = m_mapWatchTable.find(nPlayerId);
	if(it != m_mapWatchTable.end())
	{
		m_mapWatchTable.erase(it);
	}
}

BOOL KOverallManager::GetOverallTeamList(INT nInstanceMapId, LIST_OVERALL_TEAMS& listTeams)
{
	BOOL bResult = FALSE;

	MAP_OVERALL_TEAMS::iterator it = m_mapOverallTeams.find(nInstanceMapId);
	if(it == m_mapOverallTeams.end())
	{
		LIST_OVERALL_TEAMS listTeams;
		m_mapOverallTeams[nInstanceMapId] = listTeams;
		it = m_mapOverallTeams.find(nInstanceMapId);
	}

	listTeams = it->second;

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KOverallManager::OnSyncOverallTeam(INT nInstanceMapId, INT nTeamsNum, KTEAM_INFO* pTeams)
{
	KPlayer* pPlayer = NULL;
	KInstanceCtrl* pInstanceCtrl = NULL;
	MAP_OVERALL_TEAMS::iterator it = m_mapOverallTeams.find(nInstanceMapId);
	if(it == m_mapOverallTeams.end())
	{
		LIST_OVERALL_TEAMS listTeams;
		m_mapOverallTeams[nInstanceMapId] = listTeams;
		it = m_mapOverallTeams.find(nInstanceMapId);
	}

	{
		LIST_OVERALL_TEAMS& listOverallTeams = it->second;
		listOverallTeams.clear();
		for(INT i = 0; i < nTeamsNum; ++i)
		{
			KTEAM_INFO info;
			memcpy(&info, &(pTeams[i]), sizeof(KTEAM_INFO));
			listOverallTeams.push_back(info);
		}
	}

	// 向所有监听玩家即时同步队伍列表
	for(std::map<INT, INT>::iterator it = m_mapWatchTable.begin(); it != m_mapWatchTable.end(); ++it)
	{
		if(nInstanceMapId != it->second)
		{
			continue;
		}

		pPlayer = g_cPlayerMgr.GetById(it->first);
		LOG_PROCESS_ERROR(pPlayer);

		pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
		LOG_PROCESS_ERROR(pInstanceCtrl);

		LOG_PROCESS_ERROR(pInstanceCtrl->SyncOverallTeams((BYTE)nTeamsNum, pTeams));
	}

EXIT0:
	return;
}