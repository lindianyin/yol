#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "kinstancectrl.h"
#include "kinstanceprotocol.h"
#include "kinstancemodule.h"
#include "../onlinegamemodule/overallteam/koverallmanager.h"
#include "../onlinegamemodule/overallteam/koverallrelayclient.h"

KInstanceCtrl::KInstanceCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KInstanceCtrl::~KInstanceCtrl()
{

}

BOOL KInstanceCtrl::Init()
{
	ResetMultiInfo();
	return TRUE;
}

BOOL KInstanceCtrl::OnLogout()
{
	if(m_rcPlayer.GetOverallTeamIdx() > 0)
	{
		DWORD dwLeaderCombinedIdx = m_rcPlayer.GetOverallTeamIdx();
		KOverallRelayClient::DoCheckOverallOnlineRequest(m_rcPlayer.GetId(), m_rcPlayer.GetMultiMapId(), m_dwOverallCombinedIdx, dwLeaderCombinedIdx);
	}
	
	g_OverallManager.RemovePlayerFromWatchTable(m_rcPlayer.GetId());

	return TRUE;
}

VOID KInstanceCtrl::DoLeaveOverallTeam()
{
	if(m_rcPlayer.GetOverallTeamIdx() > 0)
	{
		DWORD dwLeaderCombinedIdx = m_rcPlayer.GetOverallTeamIdx();
		KOverallRelayClient::DoLeaveOverallTeamRequest(m_dwOverallCombinedIdx, dwLeaderCombinedIdx, 0);
	}
}

BOOL KInstanceCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	memcpy(pBuffer, &sMultiInfo, sizeof(sMultiInfo));
	uUsedSize += sizeof(sMultiInfo);
	pBuffer += sizeof(sMultiInfo);

	for(std::map<INT, INT>::iterator it = m_mapPassMultiMapIds.begin(); it != m_mapPassMultiMapIds.end(); ++it)
	{
		memcpy(pBuffer, &(it->first), sizeof(INT));
		uUsedSize += sizeof(INT);
		pBuffer += sizeof(INT);
	}

	return TRUE;
}

BOOL KInstanceCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bResult = FALSE;
	LPCBYTE pBuffer = pData;
	INT nMapId = 0;
	ASSERT(uSize >= sizeof(sMultiInfo));

	memcpy(&sMultiInfo, pData, sizeof(sMultiInfo));

	uSize -= sizeof(sMultiInfo);
	pBuffer += sizeof(sMultiInfo);

	m_mapPassMultiMapIds.clear();
	while(uSize > 0)
	{
		memcpy(&nMapId, pBuffer, sizeof(INT));
		m_mapPassMultiMapIds[nMapId] = 1;
		pBuffer += sizeof(INT);
		uSize -= sizeof(INT);
	}

	SyncMultiInsInfo();

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KInstanceCtrl::Active()
{

EXIT0:
	return TRUE;
}

VOID KInstanceCtrl::ResetMultiInfo()
{
	memset(&sMultiInfo, 0, sizeof(sMultiInfo));
}

BOOL KInstanceCtrl::SyncMultiInsInfo()
{
	KS2C_SYNC_MULTI_INSTANCE_INFO sSync;

	sSync.nState = sMultiInfo.nState;
	sSync.nRushMapId = sMultiInfo.nRushMapId;

	return g_cInstanceModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

VOID KInstanceCtrl::SetMultiMapPassed(INT nMapId)
{
	std::map<INT, INT>::iterator it = m_mapPassMultiMapIds.find(nMapId);
	PROCESS_ERROR(it == m_mapPassMultiMapIds.end());

	m_mapPassMultiMapIds[nMapId] = 1;

EXIT0:
	return;
}

BOOL KInstanceCtrl::IsMultiMapPassed(INT nMapId)
{
	std::map<INT, INT>::iterator it = m_mapPassMultiMapIds.find(nMapId);
	if(it == m_mapPassMultiMapIds.end())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL KInstanceCtrl::SyncOverallTeams(BYTE byTeamsNum, KTEAM_INFO* pTeams)
{
	BOOL					bRetCode		= FALSE;
	BOOL					bResult			= FALSE;
	IMemBlock*				piData		= NULL;
	size_t					uPakSize		= 0;
	KS2C_SYNC_OVERALL_TEAMS*		pSync			= NULL;

	uPakSize = sizeof(KS2C_SYNC_OVERALL_TEAMS) + byTeamsNum * sizeof(KTEAM_INFO);
	piData = QCreateMemBlock(uPakSize);

	pSync = (KS2C_SYNC_OVERALL_TEAMS*)piData->GetData();

	pSync->byProtocol = s2c_sync_overall_teams;
	pSync->byTeamsNum = byTeamsNum;
	for(INT i = 0; i < byTeamsNum; ++i)
	{
		memcpy(&(pSync->arrTeams[i]), &(pTeams[i]), sizeof(KTEAM_INFO));
	}

	bRetCode = g_cInstanceModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piData);
	return bResult;
}

BOOL KInstanceCtrl::SyncPlayerOverallTeam(DWORD dwLeaderCombinedIdx, INT nMemberNum, INT nRequireFightscore, KRS_MEMBER_INFO* arrMembers)
{
	BOOL					bRetCode		= FALSE;
	BOOL					bResult			= FALSE;
	IMemBlock*				piData		= NULL;
	size_t					uPakSize		= 0;
	KS2C_SYNC_PLAYER_OVERALLTEAM*		pSync			= NULL;

	uPakSize = sizeof(KS2C_SYNC_PLAYER_OVERALLTEAM) + nMemberNum * sizeof(KRS_MEMBER_INFO);
	piData = QCreateMemBlock(uPakSize);

	pSync = (KS2C_SYNC_PLAYER_OVERALLTEAM*)piData->GetData();

	pSync->byProtocol = s2c_sync_player_overallteam;
	pSync->byMemberNum = (BYTE)nMemberNum;
	pSync->nRequireFightscore = nRequireFightscore;
	memcpy(pSync->arrMembers, arrMembers, sizeof(KRS_MEMBER_INFO) * nMemberNum);

	bRetCode = g_cInstanceModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piData);
	return bResult;
}

BOOL KInstanceCtrl::SyncOverallTeamList(LIST_OVERALL_TEAMS& listTeams)
{
	BYTE byTeamsNum = listTeams.size();
	BOOL					bRetCode		= FALSE;
	BOOL					bResult			= FALSE;
	IMemBlock*				piData		= NULL;
	size_t					uPakSize		= 0;
	KS2C_SYNC_OVERALL_TEAMS*		pSync			= NULL;
	INT nIdx = 0;

	uPakSize = sizeof(KS2C_SYNC_OVERALL_TEAMS) + byTeamsNum * sizeof(KTEAM_INFO);
	piData = QCreateMemBlock(uPakSize);

	pSync = (KS2C_SYNC_OVERALL_TEAMS*)piData->GetData();

	pSync->byProtocol = s2c_sync_overall_teams;
	pSync->byTeamsNum = byTeamsNum;
	for(LIST_OVERALL_TEAMS::iterator it = listTeams.begin(); it != listTeams.end(); ++it)
	{
		pSync->arrTeams[nIdx].byCurrentMemberNum = it->byCurrentMemberNum;
		pSync->arrTeams[nIdx].dwLeaderCombinedIdx = it->dwLeaderCombinedIdx;
		pSync->arrTeams[nIdx].nRequireFightscore = it->nRequireFightscore;
		strncpy(pSync->arrTeams[nIdx].szCreaterName, it->szCreaterName, MAX_NAME_LEN);
		++nIdx;
	}

	bRetCode = g_cInstanceModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piData);
	return bResult;	
}

BOOL KInstanceCtrl::DoSyncOverallTeamopResult(BYTE byTeamsOp, BYTE byResult)
{
	KS2C_OVERALL_TEAMOP_RESULT sSync;

	sSync.byTeamsOp = byTeamsOp;
	sSync.byResult = byResult;

	return g_cInstanceModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KInstanceCtrl::DoMemberCountDownNotify()
{
	KS2C_MEMBER_COUNTDOWN_NOTIFY sSync;

	return g_cInstanceModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}