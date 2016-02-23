
#pragma once

#include "stdafx.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "instance/instancedef.h"

class KOverallManager
{
public:
	KOverallManager();
	~KOverallManager();

public:
	BOOL Init();
	BOOL UnInit();

public:
	VOID AddPlayerToWatchTable(INT nPlayerId, INT nInstanceMapId);

	VOID RemovePlayerFromWatchTable(INT nPlayerId);

	VOID OnSyncOverallTeam(INT nInstanceMapId, INT nTeamsNum, KTEAM_INFO* pTeams);

	BOOL GetOverallTeamList(INT nInstanceMapId, LIST_OVERALL_TEAMS& listTeams);

private:
	std::map<INT, INT> m_mapWatchTable;		// 监听列表，<nPlayerId, nMapId>，打开界面就将一个玩家加入到监听列表，有改变就同步给这个玩家

	MAP_OVERALL_TEAMS m_mapOverallTeams;
};

extern KOverallManager g_OverallManager;