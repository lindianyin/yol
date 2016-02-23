
#include "stdafx.h"
#include "qteamgchandler.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamemodule/kteamprotocol.h"
#include "onlinegamemodule/qplayerteam.h"
#include "qteamserver.h"
#include "onlinegameworld/kgcmodulebase.h"
#include "../onlinegameworld/KRelayClient.h"


//#define REGISTER_INTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
//{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
//	m_uProtocolSize[ProtocolID] = ProtocolSize;}


//QTeamGCHandler::PROCESS_PROTOCOL_FUNC	QTeamGCHandler::m_ProcessProtocolFuns[r2s_team_protocol_total] = {0};
//size_t QTeamGCHandler::m_uProtocolSize[r2s_team_protocol_total] = {0};

void QTeamGCHandler::OnInvitePlayerJoinTeamRequest(BYTE* pbyData, size_t uDataLen)
{
	R2S_INVITE_PLAYER_JOIN_TEAM_REQUEST* pRequest = (R2S_INVITE_PLAYER_JOIN_TEAM_REQUEST*)pbyData;
	KPlayer* pDstPlayer = NULL;

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnInvitePlayerJoinTeamRequest dwInviteSrc=%d szInviteSrc=%s dwInviteDst=%d, bySrcCamp=%d\n",
		pRequest->dwInviteSrc, pRequest->szInviteSrc, pRequest->dwInviteDst, (int)pRequest->bySrcCamp);

	pDstPlayer = g_cPlayerMgr.GetById(pRequest->dwInviteDst);
	LOG_PROCESS_ERROR(pDstPlayer);

	if (pDstPlayer->GetGameState() != gsPlaying)
	{
		DoInviteJoinTeamRespond(
			pRequest->szInviteSrc, pRequest->dwInviteDst, false
			);
		goto EXIT0;
	}

	pRequest->szInviteSrc[sizeof(pRequest->szInviteSrc) - 1] = '\0';

	QPlayerTeam::DoInvitePlayerJoinTeamRequest(
		pRequest->dwInviteDst, pRequest->szInviteSrc
		);

EXIT0:
	return;
}


void QTeamGCHandler::OnApplyJoinTeamRequest(BYTE* pbyData, size_t uDataLen)
{
	R2S_APPLY_JOIN_TEAM_REQUEST* pRequest = (R2S_APPLY_JOIN_TEAM_REQUEST*)pbyData;
	KPlayer*                     pPlayer        = NULL;

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnApplyJoinTeamRequest dwApplyDst=%d dwApplySrc=%d szApplySrc=%s, bySrcLevel=%d bySrcCamp=%d bySrcForceID=%d dwSrcTongID=%d dwMapID=%d\n",
		pRequest->dwApplyDst, pRequest->dwApplySrc, pRequest->szApplySrc, (int)pRequest->bySrcLevel, (int)pRequest->bySrcCamp, (int)pRequest->bySrcForceID, pRequest->dwSrcTongID, pRequest->dwMapID);

	pRequest->szApplySrc[sizeof(pRequest->szApplySrc) - 1] = '\0';

	pPlayer = g_cPlayerMgr.GetById(pRequest->dwApplyDst);
	LOG_PROCESS_ERROR(pPlayer);

	QPlayerTeam::DoApplyJoinTeamRequest(
		pRequest->dwApplyDst, pRequest->szApplySrc
		);

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamCreateNotify(BYTE* pbyData, size_t uDataLen)
{
	int                      nRetCode       = false;
	R2S_TEAM_CREATE_NOTIFY*  pNotify        = (R2S_TEAM_CREATE_NOTIFY*)pbyData;
	QTeam*                   pTeam          = NULL;

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnTeamCreateNotify dwTeamID=%d bSystem=%d \n", pNotify->dwTeamID, (int)pNotify->bSystem);

	nRetCode = g_cTeamServer.Create(pNotify->dwTeamID);
	LOG_PROCESS_ERROR(nRetCode);

	pTeam = g_cTeamServer.GetTeam(pNotify->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	pTeam->bSystem      = pNotify->bSystem;
	pTeam->nGroupNum    = pNotify->nGroupNum;
	pTeam->nLootMode    = pNotify->nLootMode;
	pTeam->nRollQuality = pNotify->nRollQuality;

	memcpy(pTeam->dwAuthority, pNotify->dwAuthority, sizeof(pTeam->dwAuthority));
	memcpy(pTeam->dwMark, pNotify->dwTeamMark, sizeof(pTeam->dwMark));

	for (int i = 0; i < pNotify->byMemberCount; i++)
	{
		QTEAM_MEMBER_INFO* pMemberInfo  = NULL;

		nRetCode = g_cTeamServer.AddMember(
			pNotify->dwTeamID, pNotify->Member[i].nGroupIndex, pNotify->Member[i].dwMemberID
			);
		LOG_PROCESS_ERROR(nRetCode);

		pMemberInfo = g_cTeamServer.GetMember(pNotify->dwTeamID, pNotify->Member[i].dwMemberID);
		LOG_PROCESS_ERROR(pMemberInfo);

		pMemberInfo->bOnlineFlag = (BOOL)pNotify->Member[i].byOnLineFlag;
		pMemberInfo->nLevel      = (int)pNotify->Member[i].byLevel;

		strncpy(pMemberInfo->szRoleName, pNotify->Member[i].szMemberName, sizeof(pMemberInfo->szRoleName));
		pMemberInfo->szRoleName[sizeof(pMemberInfo->szRoleName) - 1] = '\0';
	}

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamAddMemberNotify(BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bRetCode                = false;
	R2S_TEAM_ADD_MEMBER_NOTIFY* pNotify   = (R2S_TEAM_ADD_MEMBER_NOTIFY*)pbyData;
	QTeam*                      pTeam                   = NULL;
	QTEAM_MEMBER_INFO*          pMemberInfo             = NULL;
	QTEAM_MEMBER_INFO*          pLeaderInfo             = NULL;
	KPlayer*                    pTeamMember             = NULL;
	KPlayer*					pTeamLeader				= NULL;

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnTeamAddMemberNotify dwTeamID=%d nGroupIndex=%d dwMemberID=%d bySystem=%d szMemberName=%s\n",
		pNotify->dwTeamID, pNotify->nGroupIndex, pNotify->dwMemberID, (int)pNotify->bySystem, pNotify->szMemberName);

	pTeam = g_cTeamServer.GetTeam(pNotify->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	bRetCode = g_cTeamServer.AddMember(
		pNotify->dwTeamID,
		pNotify->nGroupIndex,
		pNotify->dwMemberID,
		pNotify->bySystem
		);  
	LOG_PROCESS_ERROR(bRetCode);

	pLeaderInfo = g_cTeamServer.GetMember(pNotify->dwTeamID, pTeam->dwAuthority[tatLeader]);

	if(pLeaderInfo)
	{
		pTeamLeader = g_cPlayerMgr.GetById(pLeaderInfo->dwMemberID);
	}

	pMemberInfo = g_cTeamServer.GetMember(pNotify->dwTeamID, pNotify->dwMemberID);
	LOG_PROCESS_ERROR(pMemberInfo);

	strncpy(pMemberInfo->szRoleName, pNotify->szMemberName, sizeof(pMemberInfo->szRoleName));
	pMemberInfo->szRoleName[sizeof(pMemberInfo->szRoleName) - 1] = '\0';

	g_cTeamServer.SendTeamInfoToAll(pNotify->dwTeamID);
	g_cTeamServer.TeamMessageNotifyToAll(pNotify->dwTeamID, epncTeamJoined, pMemberInfo->szRoleName, sizeof(pMemberInfo->szRoleName));

	pTeamMember = g_cPlayerMgr.GetById(pNotify->dwMemberID);
	if(pTeamMember)
	{
		pTeamMember->FireEvent(emKOBJEVENTTYPE_JOIN_TEAM, pTeamMember->GetId(), 0, 0);
		pTeamMember->GetCtrl<QPlayerTeam>()->ResetTeamLastSyncParamRecord();
	}

	/*
	pTeamMember = g_cPlayerMgr.GetById(pNotify->dwMemberID);
	if (pLeaderInfo && pTeamMember) // if Team had leader
	{
		pTeamMember->m_cPlayerServer.DoMessageNotify(emMODULE_TEAM, epncTeamJoined, pLeaderInfo->szRoleName, sizeof(pLeaderInfo->szRoleName));

		if (pTeam->bSystem || pTeamMember->m_dwSystemTeamId == KD_BAD_ID)
			QPlayerTeam::DoSyncTeamBaseInfo(pNotify->dwTeamID, pTeamMember, true);
		// 给队长再发一次，应对A邀请B直接组队的情况
		if(pTeamLeader)
			QPlayerTeam::DoSyncTeamBaseInfo(pNotify->dwTeamID, pTeamLeader, true);
	}
	else if (pTeamMember) // New Team, You are Leader
	{
		pTeamMember->m_cPlayerServer.DoMessageNotify(emMODULE_TEAM, epncTeamCreated, 0, 0);

		if (pTeam->bSystem || pTeamMember->m_dwSystemTeamId == KD_BAD_ID)
			QPlayerTeam::DoSyncTeamBaseInfo(pNotify->dwTeamID, pTeamMember, true);
	}
	//else: player not in this server !

	if (pTeamMember)
	{
		//g_LogClient.LogPlayerJoinTeam(pTeamMember, pTeam);
	}
	
	*/

	QPlayerTeam::DoTeamAddMemberNotify(
		pNotify->dwTeamID, pNotify->nGroupIndex, pNotify->dwMemberID
		);

EXIT0:
	return;
}


void QTeamGCHandler::OnTeamDelMemberNotify(BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bRetCode                = false;
	R2S_TEAM_DEL_MEMBER_NOTIFY* pNotify   = (R2S_TEAM_DEL_MEMBER_NOTIFY*)pbyData;
	KPlayer*                    pPlayer                 = NULL;
	DWORD                       dwTeamID                = KD_BAD_ID;
	CHAR	szBuf[32];

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnTeamDelMemberNotify dwTeamID=%d nGroupIndex=%d dwMemberID=%d szMemberName=%s\n",
		pNotify->dwTeamID, pNotify->nGroupIndex, pNotify->dwMemberID, pNotify->szMemberName);

	bRetCode = g_cTeamServer.DelMember(
		pNotify->dwTeamID, pNotify->nGroupIndex, pNotify->dwMemberID
		);
	LOG_PROCESS_ERROR(bRetCode);

	QPlayerTeam::DoTeamDelMemberNotify(
		pNotify->dwTeamID, pNotify->nGroupIndex, pNotify->dwMemberID
		);

	pPlayer = g_cPlayerMgr.GetById(pNotify->dwMemberID);
	if (pPlayer)
	{
		pPlayer->FireEvent(emKOBJEVENTTYPE_LEAVE_TEAM, pPlayer->GetId(), pNotify->dwTeamID, 0);
		/*
		dwTeamID = pPlayer->GetTeamId();
		if (dwTeamID != KD_BAD_ID)
		{
			QPlayerTeam::DoSyncTeamBaseInfo(dwTeamID, pPlayer);
		}
		*/
		//g_LogClient.LogPlayerLeaveTeam(pPlayer);
	}

	snprintf(szBuf, sizeof(szBuf), "%s", pNotify->szMemberName);
	g_cTeamServer.TeamMessageNotifyToAll(pNotify->dwTeamID, epncTeamLeaved, szBuf, sizeof(szBuf));

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamDisbandNotify(BYTE* pbyData, size_t uDataLen)
{
	int                         nRetCode            = false;
	R2S_TEAM_DISBAND_NOTIFY*    pNotify = (R2S_TEAM_DISBAND_NOTIFY*)pbyData;
	QTeam*                      pTeam               = NULL;
	QTeam                       TempTeam;

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnTeamDisbandNotify dwTeamID=%d \n", pNotify->dwTeamID);

	pTeam = g_cTeamServer.GetTeam(pNotify->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	TempTeam = *pTeam;

	QPlayerTeam::DoTeamDisbandNotify(pNotify->dwTeamID);

	g_cTeamServer.Destroy(pNotify->dwTeamID);

	for (int i = 0; i < TempTeam.nGroupNum; i++)
	{
		for (
			QTEAM_MEMBER_LIST::iterator it = TempTeam.MemberGroup[i].MemberList.begin();
			it != TempTeam.MemberGroup[i].MemberList.end(); ++it
			)
		{
			KPlayer* pPlayer  = g_cPlayerMgr.GetById(it->dwMemberID);
			DWORD    dwTeamID = KD_BAD_ID;

			if (!pPlayer)
				continue;

			//g_LogClient.LogPlayerLeaveTeam(pPlayer);

			dwTeamID = pPlayer->GetTeamId();

			QPlayerTeam::DoSyncTeamBaseInfo(dwTeamID, pPlayer);
			pPlayer->FireEvent(emKOBJEVENTTYPE_DISBAND_TEAM, pPlayer->GetId(), 0, 0);
		}
	}

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamChangeAuthorityNotify(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                bRetCode                    = false;
	R2S_TEAM_CHANGE_AUTHORITY_NOTIFY*   pNotify    = (R2S_TEAM_CHANGE_AUTHORITY_NOTIFY*)pbyData;
	QTeam*                      pTeam               = NULL;

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnTeamChangeAuthorityNotify dwTeamID=%d byType=%d dwNewID=%d \n", 
		pNotify->dwTeamID, (int)pNotify->byType, pNotify->dwNewID);

	// 设置队长
	bRetCode = g_cTeamServer.SetAuthority(
		pNotify->dwTeamID, (TEAM_AUTHORITY_TYPE)pNotify->byType, pNotify->dwNewID
		);
	LOG_PROCESS_ERROR(bRetCode);

	pTeam = g_cTeamServer.GetTeam(pNotify->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		for (
			QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
			it != pTeam->MemberGroup[i].MemberList.end(); ++it
			)
		{
			KPlayer* pPlayer  = g_cPlayerMgr.GetById(it->dwMemberID);

			if (!pPlayer)
				continue;

			pPlayer->FireEvent(emKOBJEVENTTYPE_CAPTAIN_CHANGE, pPlayer->GetId(), 0, 0);
		}
	}

	// 通知队伍成员
	QPlayerTeam::DoTeamChangeAuthorityNotify(
		pNotify->dwTeamID, (TEAM_AUTHORITY_TYPE)pNotify->byType, pNotify->dwNewID
		);

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamSetLootModeNotify(BYTE* pbyData, size_t uDataLen)
{
	int                             nRetCode    = false;
	R2S_TEAM_SET_LOOT_MODE_NOTIFY*  pNotify     = (R2S_TEAM_SET_LOOT_MODE_NOTIFY*)pbyData;
	QTeam*                          pTeam       = NULL;

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnTeamSetLootModeNotify dwTeamID=%d nLootMode=%d nRollQuality=%d \n", 
		pNotify->dwTeamID, pNotify->nLootMode, pNotify->nRollQuality);

	pTeam = g_cTeamServer.GetTeam(pNotify->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	pTeam->nLootMode    = pNotify->nLootMode;
	pTeam->nRollQuality = pNotify->nRollQuality;

	QPlayerTeam::DoTeamSetLootModeNotify(pNotify->dwTeamID, pNotify->nLootMode, pNotify->nRollQuality);

EXIT0:
	return;
}

void QTeamGCHandler::OnSyncTeamMemberOnlineFlag(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                bRetCode    = false;
	R2S_SYNC_TEAM_MEMBER_ONLINE_FLAG*   pPak        = (R2S_SYNC_TEAM_MEMBER_ONLINE_FLAG*)pbyData;
	size_t                              uPakSize    = 0;
	QTEAM_MEMBER_INFO*                  pMemberInfo = NULL;

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnSyncTeamMemberOnlineFlag dwTeamID=%d dwMemberID=%d bOnlineFlag=%d\n",
		pPak->dwTeamID, pPak->dwMemberID, (int)pPak->bOnlineFlag);

	pMemberInfo = g_cTeamServer.GetMember(pPak->dwTeamID, pPak->dwMemberID);
	LOG_PROCESS_ERROR(pMemberInfo);

	pMemberInfo->bOnlineFlag = pPak->bOnlineFlag;

	QPlayerTeam::DoSyncTeamMemberOnlineFlag(pPak->dwTeamID, pPak->dwMemberID, pPak->bOnlineFlag);

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamMessageNotify(BYTE* pbyData, size_t uDataLen)
{
	R2S_PARTY_MESSAGE_NOTIFY* pNotify      = (R2S_PARTY_MESSAGE_NOTIFY*)pbyData;
	KPlayer*                  pPlayer   = NULL;

	QLogPrintf(LOG_INFO, "QTeamGCHandler::OnTeamMessageNotify dwPlayerID=%d nCode=%d byData=%s\n",
		pNotify->dwPlayerID, pNotify->nCode, pNotify->byData);

	pPlayer = g_cPlayerMgr.GetById(pNotify->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	pPlayer->m_cPlayerServer.DoMessageNotify(emMODULE_TEAM, pNotify->nCode, 
		pNotify->byData, uDataLen - sizeof(R2S_PARTY_MESSAGE_NOTIFY)
		);

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamSyncMemberMaxLMR(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bRetCode    = false;
	R2S_SYNC_TEAM_MEMBER_MAX_LMR*   pPak        = (R2S_SYNC_TEAM_MEMBER_MAX_LMR*)pbyData;
	size_t                          uPakSize    = 0;
	QTEAM_MEMBER_INFO*              pMemberInfo = NULL;

	//QLogPrintf(LOG_INFO, "QTeamGCHandler::OnTeamSyncMemberMaxLMR dwTeamID=%d dwMemberID=%d\n", pPak->dwTeamID, pPak->dwMemberID);

	pMemberInfo = g_cTeamServer.GetMember(pPak->dwTeamID, pPak->dwMemberID);
	PROCESS_ERROR(pMemberInfo);

	pMemberInfo->nMaxLife         = pPak->nMaxLife;
	pMemberInfo->nMaxMana         = pPak->nMaxMana;
	pMemberInfo->nMaxRage         = pPak->nMaxRage;

	QPlayerTeam::DoSyncTeamMemberMaxLMR(pPak->dwTeamID, pMemberInfo);

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamSyncMemberCurrentLMR(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                bRetCode        = false;
	R2S_SYNC_TEAM_MEMBER_CURRENT_LMR*   pPak            = (R2S_SYNC_TEAM_MEMBER_CURRENT_LMR*)pbyData;
	QTEAM_MEMBER_INFO*                  pMemberInfo     = NULL;

	pMemberInfo = g_cTeamServer.GetMember(pPak->dwTeamID, pPak->dwMemberID);
	PROCESS_ERROR(pMemberInfo);

	pMemberInfo->nLifePercent     = pPak->byLifePercent;
	pMemberInfo->nManaPercent     = pPak->byManaPercent;
	pMemberInfo->nRagePercent     = pPak->byRagePercent;

	QPlayerTeam::DoSyncTeamMemberCurrentLMRGlobal(pPak->dwTeamID, pMemberInfo);

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamSyncMemberMisc(BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bRetCode    = false;
	R2S_SYNC_TEAM_MEMBER_MISC*  pPak        = (R2S_SYNC_TEAM_MEMBER_MISC*)pbyData;
	size_t                      uPakSize    = 0;
	QTEAM_MEMBER_INFO*          pMemberInfo = NULL;

	pMemberInfo = g_cTeamServer.GetMember(pPak->dwTeamID, pPak->dwMemberID);
	PROCESS_ERROR(pMemberInfo);

	//pMemberInfo->PortraitInfo   = pPak->PortraitInfo;
	pMemberInfo->dwForceID      = pPak->dwForceID;
	//pMemberInfo->eCamp          = (KCAMP)pPak->byCamp;
	pMemberInfo->nLevel         = pPak->byLevel;
	pMemberInfo->nRoleType      = pPak->byRoleType;
	pMemberInfo->bDeathFlag     = pPak->byDeathFlag;

	QPlayerTeam::DoSyncTeamMemberMisc(pPak->dwTeamID, pMemberInfo);

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamSyncMemberPosition(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bRetCode        = false;
	R2S_SYNC_TEAM_MEMBER_POSITION*  pPak            = (R2S_SYNC_TEAM_MEMBER_POSITION*)pbyData;
	QTEAM_MEMBER_INFO*              pMemberInfo     = NULL;
	size_t                          uPakSize        = 0;

	pMemberInfo = g_cTeamServer.GetMember(pPak->dwTeamID, pPak->dwMemberID);
	PROCESS_ERROR(pMemberInfo);

	pMemberInfo->dwMapID         = pPak->dwMapID;
	pMemberInfo->nMapCopyIndex   = pPak->nMapCopyIndex;
	pMemberInfo->nPosX           = pPak->nPosX;
	pMemberInfo->nPosY           = pPak->nPosY;

	QPlayerTeam::DoSyncTeamMemberPositionGlobal(pPak->dwTeamID, pMemberInfo);

EXIT0:
	return;
}

void QTeamGCHandler::OnTeamLevelUpRaidNotify(BYTE* pbyData, size_t uDataLen)
{
	R2S_TEAM_LEVEL_UP_RAID_NOTIFY* pNotify  = (R2S_TEAM_LEVEL_UP_RAID_NOTIFY*)pbyData;
	QTeam*                         pTeam    = NULL;

	pTeam = g_cTeamServer.GetTeam(pNotify->dwTeamID);
	LOG_PROCESS_ERROR(pTeam);

	pTeam->nGroupNum = pNotify->nGroupNum;

	QPlayerTeam::DoTeamLevelUpRaidNotify(pNotify->dwTeamID, pNotify->nGroupNum);
EXIT0:
	return;
}

// 转发
BOOL QTeamGCHandler::DoInviteJoinTeamRequest(DWORD dwInviteSrc, const char cszInviteDst[])
{
	BOOL                                    bResult                 = false;
	BOOL                                    bRetCode                = false;
	IMemBlock*							    piSendBuffer			= NULL;
	S2R_INVITE_PLAYER_JOIN_TEAM_REQUEST*    pInvitePlayerJoinTeam   = NULL;
	QLogPrintf(LOG_INFO,"..QTeamGCHandler::DoInviteJoinTeamRequest(DWORD dwInviteSrc=%d, const char cszInviteDst[]=%s)"
							,dwInviteSrc,cszInviteDst);
	
	piSendBuffer = QCreateMemBlock(sizeof(S2R_INVITE_PLAYER_JOIN_TEAM_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pInvitePlayerJoinTeam = (S2R_INVITE_PLAYER_JOIN_TEAM_REQUEST *)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pInvitePlayerJoinTeam);

	pInvitePlayerJoinTeam->wProtocolID = s2r_invite_player_join_team_request;
	pInvitePlayerJoinTeam->dwInviteSrc		 = dwInviteSrc;

	strncpy(pInvitePlayerJoinTeam->szInviteRole, cszInviteDst, sizeof(pInvitePlayerJoinTeam->szInviteRole));
	pInvitePlayerJoinTeam->szInviteRole[sizeof(pInvitePlayerJoinTeam->szInviteRole) - 1] = '\0';

	bResult = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bResult);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoInviteJoinTeamRespond(const char cszInviteSrc[], DWORD dwInviteDst, BOOL bAgreeFlag)
{
	BOOL                                    bResult                 = false;
	BOOL                                    bRetCode                = false;
	IMemBlock*							    piSendBuffer			= NULL;
	S2R_INVITE_PLAYER_JOIN_TEAM_RESPOND*    pInviteRespond          = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_INVITE_PLAYER_JOIN_TEAM_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pInviteRespond = (S2R_INVITE_PLAYER_JOIN_TEAM_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pInviteRespond);

	pInviteRespond->wProtocolID = s2r_invite_player_join_team_respond;
	pInviteRespond->dwInviteDst		 = dwInviteDst;
	pInviteRespond->bAgreeFlag       = bAgreeFlag;

	strncpy(pInviteRespond->szInviteSrc, cszInviteSrc, sizeof(pInviteRespond->szInviteSrc));
	pInviteRespond->szInviteSrc[sizeof(pInviteRespond->szInviteSrc) - 1] = '\0';

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoApplyJoinTeamRequest(DWORD dwPlayerID, const char cszApplyDst[])
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	S2R_APPLY_JOIN_TEAM_REQUEST*    pApplyJoinTeam  = NULL;
	IMemBlock*                     piSendBuffer    = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_APPLY_JOIN_TEAM_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pApplyJoinTeam = (S2R_APPLY_JOIN_TEAM_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pApplyJoinTeam);

	pApplyJoinTeam->wProtocolID = s2r_apply_join_team_request;
	pApplyJoinTeam->dwApplySrc         = dwPlayerID;

	strncpy(pApplyJoinTeam->szApplyDst, cszApplyDst, sizeof(pApplyJoinTeam->szApplyDst));
	pApplyJoinTeam->szApplyDst[sizeof(pApplyJoinTeam->szApplyDst) - 1] = '\0';

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoCreateTeamRequest(DWORD dwPlayerID, DWORD dwCreateSrc, BOOL bSystem /* = 0 */)
{
	BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	S2R_CREATE_TEAM_REQUEST*		pCreateTeam		= NULL;
	IMemBlock*						piSendBuffer    = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_CREATE_TEAM_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pCreateTeam = (S2R_CREATE_TEAM_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pCreateTeam);

	pCreateTeam->wProtocolID = s2r_create_team_request;
	pCreateTeam->dwCreateSrc = dwCreateSrc;
	pCreateTeam->bySystem = (BYTE)bSystem;

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}


BOOL QTeamGCHandler::DoApplyJoinTeamRespond(DWORD dwLeaderID, const char cszApplySrc[], BOOL bAgreeFlag)
{
	BOOL                            bResult                 = false;
	BOOL                            bRetCode                = false;
	IMemBlock*						piSendBuffer			= NULL;
	S2R_APPLY_JOIN_TEAM_RESPOND*    pApplyRespond           = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_APPLY_JOIN_TEAM_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pApplyRespond = (S2R_APPLY_JOIN_TEAM_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pApplyRespond);

	pApplyRespond->wProtocolID = s2r_apply_join_team_respond;
	pApplyRespond->dwApplyDst		 = dwLeaderID;
	pApplyRespond->bAgreeFlag        = bAgreeFlag;

	strncpy(pApplyRespond->szApplySrc, cszApplySrc, sizeof(pApplyRespond->szApplySrc));
	pApplyRespond->szApplySrc[sizeof(pApplyRespond->szApplySrc) - 1] = '\0';

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamDisbandRequest(DWORD dwLeaderID, DWORD dwTeamID)
{
	BOOL                       bResult              = false;
	BOOL                       bRetCode             = false;
	S2R_TEAM_DISBAND_REQUEST*  pTeamDisbandRequest = NULL;
	IMemBlock*                piSendBuffer         = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_TEAM_DISBAND_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pTeamDisbandRequest = (S2R_TEAM_DISBAND_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pTeamDisbandRequest);

	pTeamDisbandRequest->wProtocolID = s2r_team_disband_request;
	pTeamDisbandRequest->dwLeaderID       = dwLeaderID;
	pTeamDisbandRequest->dwTeamID        = dwTeamID;

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamDelMemberRequest(DWORD dwTeamID, DWORD dwRequestSrc, DWORD dwPlayerID)
{
	BOOL                            bResult             = false;
	BOOL                            bRetCode            = false;
	S2R_TEAM_DEL_MEMBER_REQUEST*    pDelMemberRequest   = NULL;
	IMemBlock*                     piSendBuffer        = NULL;

	// TODO: 向Relay发请求: 将玩家从队伍删除
	piSendBuffer = QCreateMemBlock(sizeof(S2R_TEAM_DEL_MEMBER_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pDelMemberRequest = (S2R_TEAM_DEL_MEMBER_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pDelMemberRequest);

	pDelMemberRequest->wProtocolID	= s2r_team_del_member_request;
	pDelMemberRequest->dwTeamID        = dwTeamID;
	pDelMemberRequest->dwRequestSrc     = dwRequestSrc;
	pDelMemberRequest->dwMemberID       = dwPlayerID;

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamChangeAuthorityRequest(DWORD dwTeamID, DWORD dwRequestSrc, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID)
{
	BOOL                                bResult                 = false;
	BOOL                                bRetCode                = false;
	S2R_TEAM_CHANGE_AUTHORITY_REQUEST*  pChangeLeaderRequest    = NULL;
	IMemBlock*                         piSendBuffer            = NULL;

	// 向Relay发请求: 设置队长
	piSendBuffer = QCreateMemBlock(sizeof(S2R_TEAM_CHANGE_AUTHORITY_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pChangeLeaderRequest = (S2R_TEAM_CHANGE_AUTHORITY_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pChangeLeaderRequest);

	pChangeLeaderRequest->wProtocolID = s2r_team_change_authority_request;
	pChangeLeaderRequest->dwTeamID        = dwTeamID;
	pChangeLeaderRequest->dwRequestSrc     = dwRequestSrc;
	pChangeLeaderRequest->byType           = (BYTE)eType;
	pChangeLeaderRequest->dwTargetID       = dwTargetID;

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamSetLootModeRequest(DWORD dwTeamID, DWORD dwRequestSrc, int nLootMode)
{
	BOOL                            bResult                 = false;
	BOOL                            bRetCode                = false;
	S2R_TEAM_SET_LOOT_MODE_REQUEST* pRequest                = NULL;
	IMemBlock*                     piSendBuffer            = NULL;

	// 向Relay发请求: 设置队长
	piSendBuffer = QCreateMemBlock(sizeof(S2R_TEAM_SET_LOOT_MODE_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRequest = (S2R_TEAM_SET_LOOT_MODE_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID	= s2r_team_set_loot_mode_request;
	pRequest->dwTeamID     = dwTeamID;
	pRequest->dwRequestSrc  = dwRequestSrc;
	pRequest->nLootMode     = nLootMode;

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamSetRollQualityRequest(DWORD dwTeamID, DWORD dwRequestSrc, int nRollQuality)
{
	BOOL                                bResult         = false;
	BOOL                                bRetCode        = false;
	S2R_TEAM_SET_ROLL_QUALITY_REQUEST*  pRequest        = NULL;
	IMemBlock*                         piSendBuffer    = NULL;

	// 向Relay发请求: 设置队伍分配的ROLL品质等级
	piSendBuffer = QCreateMemBlock(sizeof(S2R_TEAM_SET_ROLL_QUALITY_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRequest = (S2R_TEAM_SET_ROLL_QUALITY_REQUEST *)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID     = s2r_team_set_roll_quality_request;
	pRequest->dwTeamID        = dwTeamID;
	pRequest->dwRequestSrc     = dwRequestSrc;
	pRequest->nRollQuality	   = nRollQuality;

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamSetFormationLeader(DWORD dwTeamID, DWORD dwRequestSrc, DWORD dwNewFormationLeaderID, int nGroupIndex)
{
	BOOL                                    bResult         = false;
	BOOL                                    bRetCode        = false;
	S2R_TEAM_SET_FORMATION_LEADER_REQUEST*  pRequest        = NULL;
	IMemBlock*                             piSendBuffer    = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_TEAM_SET_FORMATION_LEADER_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRequest = (S2R_TEAM_SET_FORMATION_LEADER_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID			= s2r_team_set_formation_leader_request;
	pRequest->dwTeamID             = dwTeamID;
	pRequest->dwRequestSrc          = dwRequestSrc;
	pRequest->dwNewFormationLeader	= dwNewFormationLeaderID;
	pRequest->nGroupIndex           = nGroupIndex;

	bRetCode = SendData(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamSyncMemberMaxLMR(KPlayer* pPlayer)
{
	BOOL                            bResult             = false;
	BOOL                            bRetCode            = false;
	QTeam*                          pTeam               = NULL;
	QTEAM_MEMBER_INFO*              pMemberInfo         = NULL;
	QTEAM_MEMBER_INFO*              pSystemMemberInfo   = NULL;
	BOOL                            bValueChanged       = false;
	BOOL                            bValueSystemChanged = false;
	IMemBlock*                     piSendBuffer        = NULL;
	S2R_SYNC_TEAM_MEMBER_MAX_LMR*   pPak                = NULL;

	pMemberInfo = g_cTeamServer.GetMember(pPlayer->m_dwTeamId, pPlayer->m_dwId);
	if (pMemberInfo)
	{
		bValueChanged |= (pMemberInfo->nMaxLife != pPlayer->GetMaxLife());
		bValueChanged |= (pMemberInfo->nMaxMana != pPlayer->GetMaxMana());
		bValueChanged |= (pMemberInfo->nMaxRage != pPlayer->GetMaxPower());
	}

	pSystemMemberInfo = g_cTeamServer.GetMember(pPlayer->m_dwSystemTeamId, pPlayer->m_dwId);
	if (pSystemMemberInfo)
	{
		bValueSystemChanged |= (pSystemMemberInfo->nMaxLife != pPlayer->GetMaxLife());
		bValueSystemChanged |= (pSystemMemberInfo->nMaxMana != pPlayer->GetMaxMana());
		bValueSystemChanged |= (pSystemMemberInfo->nMaxRage != pPlayer->GetMaxPower());
	}

	piSendBuffer = QCreateMemBlock(sizeof(S2R_SYNC_TEAM_MEMBER_MAX_LMR));
	LOG_PROCESS_ERROR(piSendBuffer);

	pPak = (S2R_SYNC_TEAM_MEMBER_MAX_LMR*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pPak);

	pPak->wProtocolID	   = s2r_sync_team_member_max_lmr;
	pPak->dwMemberID       = pPlayer->GetId();
	pPak->nMaxLife         = pPlayer->GetMaxLife();
	pPak->nMaxMana         = pPlayer->GetMaxMana();
	pPak->nMaxRage         = pPlayer->GetMaxPower();

	if (bValueChanged)
	{
		pPak->dwTeamID    = pPlayer->m_dwTeamId;

		assert(pMemberInfo);

		pMemberInfo->nMaxLife = pPlayer->GetMaxLife();
		pMemberInfo->nMaxMana = pPlayer->GetMaxMana();
		pMemberInfo->nMaxRage = pPlayer->GetMaxPower();

		bRetCode = g_RelayClient.Send(piSendBuffer);
		LOG_PROCESS_ERROR(bRetCode);
	}

	if (bValueSystemChanged)
	{
		pPak->dwTeamID    = pPlayer->m_dwSystemTeamId;

		assert(pSystemMemberInfo);

		pSystemMemberInfo->nMaxLife = pPlayer->GetMaxLife();
		pSystemMemberInfo->nMaxMana = pPlayer->GetMaxMana();
		pSystemMemberInfo->nMaxRage = pPlayer->GetMaxPower();

		bRetCode = g_RelayClient.Send(piSendBuffer);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamSyncMemberCurrentLMR(KPlayer* pPlayer)
{
	BOOL                                bResult         = false;
	BOOL                                bRetCode        = false;
	int                                 nNewLifePercent = 0;
	int                                 nNewManaPercent = 0;
	int                                 nNewRagePercent = 0;
	BOOL                                bValueChanged   = false;
	IMemBlock*                         piSendBuffer    = NULL;
	S2R_SYNC_TEAM_MEMBER_CURRENT_LMR*   pPak            = NULL;
	QPlayerTeam* pPlayerTeam								= NULL;

	if (pPlayer->GetMaxLife() > 0)
		nNewLifePercent = pPlayer->GetCurrentLife() * 100 / pPlayer->GetMaxLife();

	if (pPlayer->GetMaxMana() > 0)
		nNewManaPercent = pPlayer->GetCurrentMana() * 100 / pPlayer->GetMaxMana();

	if (pPlayer->GetMaxPower() > 0)
		nNewRagePercent = pPlayer->GetCurrentPower() * 100 / pPlayer->GetMaxPower();

	pPlayerTeam = pPlayer->GetCtrl<QPlayerTeam>();
	LOG_PROCESS_ERROR(pPlayerTeam);

	bValueChanged |= (nNewLifePercent != pPlayerTeam->m_nLastSyncGlobalTeamLifePercent);
	bValueChanged |= (nNewManaPercent != pPlayerTeam->m_nLastSyncGlobalTeamManaPercent);
	bValueChanged |= (nNewRagePercent != pPlayerTeam->m_nLastSyncGlobalTeamRagePercent);

	PROCESS_ERROR(bValueChanged);

	pPlayerTeam->m_nLastSyncGlobalTeamLifePercent = nNewLifePercent;
	pPlayerTeam->m_nLastSyncGlobalTeamManaPercent = nNewManaPercent;
	pPlayerTeam->m_nLastSyncGlobalTeamRagePercent = nNewRagePercent;

	// 全局同步覆盖局部同步数值
	pPlayerTeam->m_nLastSyncLocalTeamLifePercent = nNewLifePercent;
	pPlayerTeam->m_nLastSyncLocalTeamManaPercent = nNewManaPercent;
	pPlayerTeam->m_nLastSyncLocalTeamRagePercent = nNewRagePercent;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_SYNC_TEAM_MEMBER_CURRENT_LMR));
	LOG_PROCESS_ERROR(piSendBuffer);

	pPak = (S2R_SYNC_TEAM_MEMBER_CURRENT_LMR*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pPak);

	pPak->wProtocolID = s2r_sync_team_member_current_lmr;
	pPak->dwMemberID       = pPlayer->GetId();
	pPak->byLifePercent    = (BYTE)nNewLifePercent;
	pPak->byManaPercent    = (BYTE)nNewManaPercent;
	pPak->byRagePercent    = (BYTE)nNewRagePercent;

	if (pPlayer->m_dwSystemTeamId != KD_BAD_ID)
	{
		pPak->dwTeamID    = pPlayer->m_dwSystemTeamId;

		bRetCode = g_RelayClient.Send(piSendBuffer);
		LOG_PROCESS_ERROR(bRetCode);
	}

	if (pPlayer->m_dwTeamId != KD_BAD_ID)
	{
		pPak->dwTeamID     = pPlayer->m_dwTeamId;

		bRetCode = g_RelayClient.Send(piSendBuffer);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamSyncMemberMisc(KPlayer* pPlayer)
{
	BOOL                        bResult             = false;
	BOOL                        bRetCode            = false;
	QTEAM_MEMBER_INFO*          pMemberInfo         = NULL;
	QTEAM_MEMBER_INFO*          pSystemMemberInfo   = NULL;
	BOOL                        bValueChanged       = false;
	BOOL                        bValueSystemChanged = false;
	IMemBlock*                 piSendBuffer        = NULL;
	S2R_SYNC_TEAM_MEMBER_MISC*  pPak                = NULL;

	pMemberInfo = g_cTeamServer.GetMember(pPlayer->m_dwTeamId, pPlayer->m_dwId);
	if (pMemberInfo)
	{
		bValueChanged |= (pMemberInfo->nLevel != pPlayer->GetLevel());
		bValueChanged |= (pMemberInfo->bDeathFlag != (pPlayer->GetMoveState() == cmsOnDeath));
	}

	pSystemMemberInfo = g_cTeamServer.GetMember(pPlayer->m_dwSystemTeamId, pPlayer->m_dwId);
	if (pSystemMemberInfo)
	{
		bValueSystemChanged |= (pSystemMemberInfo->nLevel != pPlayer->GetLevel());
		bValueSystemChanged |= (pSystemMemberInfo->bDeathFlag != (pPlayer->GetMoveState() == cmsOnDeath)); 
	}

	piSendBuffer = QCreateMemBlock(sizeof(S2R_SYNC_TEAM_MEMBER_MISC));
	LOG_PROCESS_ERROR(piSendBuffer);

	pPak = (S2R_SYNC_TEAM_MEMBER_MISC*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pPak);

	pPak->wProtocolID = s2r_sync_team_member_misc;
	pPak->dwMemberID       = pPlayer->GetId();
	pPak->byCamp           = (BYTE)pPlayer->GetCurrentCamp();
	pPak->byLevel          = (BYTE)pPlayer->GetLevel();
	pPak->byRoleType       = (BYTE)pPlayer->GetSex();
	pPak->byDeathFlag       = (pPlayer->GetMoveState() == cmsOnDeath);

	if (bValueChanged)
	{
		pPak->dwTeamID    = pPlayer->m_dwTeamId;

	//	pMemberInfo->PortraitInfo   = PortraitInfo;
	//	pMemberInfo->dwForceID      = pPlayer->m_dwForceID;
	//	pMemberInfo->nLevel         = pPlayer->m_nLevel;
	//	pMemberInfo->eCamp          = pPlayer->m_eCamp;
	//	pMemberInfo->nRoleType      = pPlayer->m_eRoleType;
	//	pMemberInfo->bDeathFlag     = (pPlayer->m_eMoveState == cmsOnDeath);

		bRetCode = g_RelayClient.Send(piSendBuffer);
		LOG_PROCESS_ERROR(bRetCode);
	}

	if (bValueSystemChanged)
	{
		pPak->dwTeamID    = pPlayer->m_dwSystemTeamId;

	//	pSystemMemberInfo->PortraitInfo   = PortraitInfo;
	//	pSystemMemberInfo->dwForceID      = pPlayer->m_dwForceID;
	//	pSystemMemberInfo->nLevel         = pPlayer->m_nLevel;
	//	pSystemMemberInfo->eCamp          = pPlayer->m_eCamp;
	//	pSystemMemberInfo->nRoleType      = pPlayer->m_eRoleType;
	//	pSystemMemberInfo->bDeathFlag     = (pPlayer->m_eMoveState == cmsOnDeath);

		bRetCode = g_RelayClient.Send(piSendBuffer);
		LOG_PROCESS_ERROR(bRetCode);
	}

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoSyncTeamMemberPosition(KPlayer* pPlayer)
{
	BOOL                             bResult        = false;
	BOOL                             bRetCode       = false;
	BOOL                             bValueChanged  = false;
	IMemBlock*                      piSendBuffer   = NULL;
	S2R_SYNC_TEAM_MEMBER_POSITION*   pPak           = NULL;

	//PROCESS_ERROR(pPlayer->GetScene());

	//bValueChanged |= (pPlayer->m_pScene->m_dwMapID != pPlayer->m_dwLastSyncGlobalTeamMemberMapID);
	//bValueChanged |= (pPlayer->m_pScene->m_nCopyIndex != pPlayer->m_nLastSyncGlobalTeamMemberMapCopyIndex);
	//bValueChanged |= (pPlayer->m_nX != pPlayer->m_nLastSyncGlobalTeamMemberPosX);
	//bValueChanged |= (pPlayer->m_nY != pPlayer->m_nLastSyncGlobalTeamMemberPosY);

	//PROCESS_ERROR(bValueChanged);

	//pPlayer->m_dwLastSyncGlobalTeamMemberMapID         = pPlayer->m_pScene->m_dwMapID;
	//pPlayer->m_nLastSyncGlobalTeamMemberMapCopyIndex   = pPlayer->m_pScene->m_nCopyIndex;
	//pPlayer->m_nLastSyncGlobalTeamMemberPosX           = pPlayer->m_nX;
	//pPlayer->m_nLastSyncGlobalTeamMemberPosY           = pPlayer->m_nY;

	//// 全局同步时覆盖局部同步数值
	//pPlayer->m_dwLastSyncLocalTeamMemberMapID         = pPlayer->m_pScene->m_dwMapID;
	//pPlayer->m_nLastSyncLocalTeamMemberMapCopyIndex   = pPlayer->m_pScene->m_nCopyIndex;
	//pPlayer->m_nLastSyncLocalTeamMemberPosX           = pPlayer->m_nX;
	//pPlayer->m_nLastSyncLocalTeamMemberPosY           = pPlayer->m_nY;

	//piSendBuffer = QCreateMemBlock(sizeof(S2R_SYNC_TEAM_MEMBER_POSITION));
	//LOG_PROCESS_ERROR(piSendBuffer);

	//pPak = (S2R_SYNC_TEAM_MEMBER_POSITION*)piSendBuffer->GetData();
	//LOG_PROCESS_ERROR(pPak);

	//pPak->wProtocolID		   = s2r_sync_team_member_position;
	//pPak->dwMemberID         = pPlayer->m_dwId;
	//pPak->dwMapID            = pPlayer->m_pScene->m_dwMapID;
	//pPak->nMapCopyIndex      = pPlayer->m_pScene->m_nCopyIndex;
	//pPak->nPosX              = pPlayer->m_nX;
	//pPak->nPosY              = pPlayer->m_nY;

	//if (pPlayer->m_dwSystemTeamID != KD_BAD_ID)
	//{
	//	pPak->dwTeamID		 = pPlayer->m_dwSystemTeamID;

	//	bRetCode = Send(piSendBuffer);
	//	LOG_PROCESS_ERROR(bRetCode);
	//}

	//if (pPlayer->m_dwTeamID != KD_BAD_ID)
	//{
	//	pPak->dwTeamID		 = pPlayer->m_dwTeamID;

	//	bRetCode = Send(piSendBuffer);
	//	LOG_PROCESS_ERROR(bRetCode);
	//}

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamSetMarkRequest(DWORD dwTeamID, DWORD dwPlayerID, int nMarkType, DWORD dwTargetID)
{
	BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	IMemBlock*                 piSendBuff      = NULL;
	S2R_TEAM_SET_MARK_REQUEST*  pTeamSetMark   = NULL;

	piSendBuff = QCreateMemBlock(sizeof(S2R_TEAM_SET_MARK_REQUEST));
	LOG_PROCESS_ERROR(piSendBuff);

	pTeamSetMark = (S2R_TEAM_SET_MARK_REQUEST*)piSendBuff->GetData();
	LOG_PROCESS_ERROR(pTeamSetMark);

	pTeamSetMark->wProtocolID = s2r_team_set_mark_request;
	pTeamSetMark->dwTeamID   = dwTeamID;
	pTeamSetMark->dwMemberID  = dwPlayerID;
	pTeamSetMark->nMarkType   = nMarkType;
	pTeamSetMark->dwTargetID  = dwTargetID;

	bRetCode = SendData(piSendBuff);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuff);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamLevelUpRequest(DWORD dwTeamID, DWORD dwRequestSrc)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piSendBuff  = NULL;
	S2R_TEAM_LEVEL_UP_RAID_REQUEST* pRequest    = NULL;

	piSendBuff = QCreateMemBlock((unsigned)sizeof(S2R_TEAM_LEVEL_UP_RAID_REQUEST));
	LOG_PROCESS_ERROR(piSendBuff);

	pRequest = (S2R_TEAM_LEVEL_UP_RAID_REQUEST*)piSendBuff->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID  = s2r_team_level_up_raid_request;
	pRequest->dwTeamID     = dwTeamID;
	pRequest->dwRequestSrc = dwRequestSrc;

	bRetCode = SendData(piSendBuff);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuff);
	return bResult;
}

BOOL QTeamGCHandler::DoTeamChangeMemberGroupRequest(
	DWORD dwTeamID, DWORD dwMemberID, int nDstGroupIndex, DWORD dwDstMemberID, DWORD dwRequestSrc
	)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	IMemBlock*                             piSendBuff  = NULL;
	S2R_TEAM_CHANGE_MEMBER_GROUP_REQUEST*   pRequest    = NULL;

	piSendBuff = QCreateMemBlock((unsigned)sizeof(S2R_TEAM_CHANGE_MEMBER_GROUP_REQUEST));
	LOG_PROCESS_ERROR(piSendBuff);

	pRequest = (S2R_TEAM_CHANGE_MEMBER_GROUP_REQUEST*)piSendBuff->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID		= s2r_team_change_member_group_request;
	pRequest->dwTeamID          = dwTeamID;
	pRequest->dwMemberID        = dwMemberID;
	pRequest->nDstGroupIndex    = nDstGroupIndex;
	pRequest->dwDstMemberID     = dwDstMemberID;
	pRequest->dwRequestSrc      = dwRequestSrc;

	bRetCode = SendData(piSendBuff);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuff);
	return bResult;
}

BOOL QTeamGCHandler::SendData(IMemBlock* piSendBuff)
{
	return g_RelayClient.Send(piSendBuff);
}

BOOL QTeamGCHandler::InitProtocol()
{
	//memset(m_ProcessProtocolFuns, 0, sizeof(m_ProcessProtocolFuns));
	//memset(m_uProtocolSize, 0, sizeof(m_uProtocolSize));

	//REGISTER_INTERNAL_FUNC(r2s_invite_player_join_team_request, &QTeamGCHandler::OnInvitePlayerJoinTeamRequest, sizeof(R2S_INVITE_PLAYER_JOIN_TEAM_REQUEST));
	//REGISTER_INTERNAL_FUNC(r2s_apply_join_team_request, &QTeamGCHandler::OnApplyJoinTeamRequest, sizeof(R2S_APPLY_JOIN_TEAM_REQUEST));
	//REGISTER_INTERNAL_FUNC(r2s_team_create_notify, &QTeamGCHandler::OnTeamCreateNotify, sizeof(R2S_TEAM_CREATE_NOTIFY));
	//REGISTER_INTERNAL_FUNC(r2s_team_add_member_notify, &QTeamGCHandler::OnTeamAddMemberNotify, sizeof(R2S_TEAM_ADD_MEMBER_NOTIFY));
	//REGISTER_INTERNAL_FUNC(r2s_team_del_member_notify, &QTeamGCHandler::OnTeamDelMemberNotify, sizeof(R2S_TEAM_DEL_MEMBER_NOTIFY));
	//REGISTER_INTERNAL_FUNC(r2s_team_disband_notify, &QTeamGCHandler::OnTeamDisbandNotify, sizeof(R2S_TEAM_DISBAND_NOTIFY));
	//REGISTER_INTERNAL_FUNC(r2s_team_change_authority_notify, &QTeamGCHandler::OnTeamChangeAuthorityNotify, sizeof(R2S_TEAM_CHANGE_AUTHORITY_NOTIFY));
	//REGISTER_INTERNAL_FUNC(r2s_team_set_loot_mode_notify, &QTeamGCHandler::OnTeamSetLootModeNotify, sizeof(R2S_TEAM_SET_LOOT_MODE_NOTIFY));
	//REGISTER_INTERNAL_FUNC(r2s_party_message_notify, &QTeamGCHandler::OnTeamMessageNotify, sizeof(R2S_PARTY_MESSAGE_NOTIFY));
	////REGISTER_INTERNAL_FUNC(r2s_team_set_formation_leader_notify, &QTeamGCHandler::OnSetFormationLeaderNotify, sizeof(R2S_TEAM_SET_FORMATION_LEADER_NOTIFY));

	//REGISTER_INTERNAL_FUNC(r2s_sync_team_member_online_flag, &QTeamGCHandler::OnSyncTeamMemberOnlineFlag, sizeof(R2S_SYNC_TEAM_MEMBER_ONLINE_FLAG));
	//REGISTER_INTERNAL_FUNC(r2s_sync_team_member_max_lmr, &QTeamGCHandler::OnTeamSyncMemberMaxLMR, sizeof(R2S_SYNC_TEAM_MEMBER_MAX_LMR));
	//REGISTER_INTERNAL_FUNC(r2s_sync_team_member_current_lmr, &QTeamGCHandler::OnTeamSyncMemberCurrentLMR, sizeof(R2S_SYNC_TEAM_MEMBER_CURRENT_LMR));
	//REGISTER_INTERNAL_FUNC(r2s_sync_team_member_misc, &QTeamGCHandler::OnTeamSyncMemberMisc, sizeof(R2S_SYNC_TEAM_MEMBER_MISC));
	//REGISTER_INTERNAL_FUNC(r2s_sync_team_member_position, &QTeamGCHandler::OnTeamSyncMemberPosition, sizeof(R2S_SYNC_TEAM_MEMBER_POSITION));
	////REGISTER_INTERNAL_FUNC(r2s_team_set_mark_respond, &QTeamGCHandler::OnTeamSetMarkRespond, sizeof(R2S_TEAM_SET_MARK_RESPOND));
	////REGISTER_INTERNAL_FUNC(r2s_team_camp_change, &QTeamGCHandler::OnTeamCampChange, sizeof(R2S_TEAM_CAMP_CHANGE));
	//REGISTER_INTERNAL_FUNC(r2s_team_level_up_raid_notify, &QTeamGCHandler::OnTeamLevelUpRaidNotify, sizeof(R2S_TEAM_LEVEL_UP_RAID_NOTIFY));
	////REGISTER_INTERNAL_FUNC(r2s_team_change_member_group_notify, &QTeamGCHandler::OnTeamChangeMemberGroupNotify, sizeof(R2S_TEAM_CHANGE_MEMBER_GROUP_NOTIFY));

	return TRUE;
}

BOOL QTeamGCHandler::ProcessGcProtocol( LPCBYTE pbData, UINT uDataLen )
{
	BOOL bResult = FALSE;
//	PROCESS_PROTOCOL_FUNC       pFunc       = NULL;
//	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pbData;
//
//	LOG_PROCESS_ERROR(pbData);
//	LOG_PROCESS_ERROR(uDataLen);
//
//	pFunc = m_ProcessProtocolFuns[pHeader->byProtocol];
//	LOG_PROCESS_ERROR(pFunc);
//
//	(pFunc)((BYTE*)pbData, uDataLen);
//
//
//	bResult = TRUE;
//EXIT0:
	return bResult;
}

