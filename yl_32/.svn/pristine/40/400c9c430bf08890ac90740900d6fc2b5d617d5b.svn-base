/* -------------------------------------------------------------------------
//	文件名		：	kplayerchatchannel_gs.cpp
//	创建者		：	zhangdeheng
//	创建时间	：	2009-11-30 15:34:54
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifdef KD_OP_GAME_SERVER

#include "onlinegamemodule/kplayerchatchannel_gs.h"
#include "gc_core/kgc_league.h"
#include "kplayerteammanager_gs.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KEnumPlayer KEnumPlayer::m_scInst;

KGS_PlayerChatChannel _PlayerChatChannel;

KEnumPlayer* KEnumPlayer::GetEnumPlayer()
{
	m_scInst.Clear();
	m_scInst.m_nCurPlayer = 0;
	return &m_scInst;
}

IKPlayerChatChannel* GetPlayerChatChannel()
{
	return &_PlayerChatChannel;
}

KGS_PlayerChatChannel::KGS_PlayerChatChannel()
{
	GetPlayer[emKCHANNEL_GM]		= NULL;
	GetPlayer[emKCHANNEL_NEARBY]	= NULL;
	GetPlayer[emKCHANNEL_TEAM]		= &KGS_PlayerChatChannel::GetTeamPlayer;
	GetPlayer[emKCHANNEL_FACTION]	= &KGS_PlayerChatChannel::GetFactionPlayer;
	GetPlayer[emKCHANNEL_KIN]		= &KGS_PlayerChatChannel::GetKinPlayer;
	GetPlayer[emKCHANNEL_TONG]		= &KGS_PlayerChatChannel::GetTongPlayer;
	GetPlayer[emKCHANNEL_CITY]		= &KGS_PlayerChatChannel::GetCityPlayer;
	GetPlayer[emKCHANNEL_WORLD]		= &KGS_PlayerChatChannel::GetWorldPlayer;
	GetPlayer[emKCHANNEL_FRIEND]	= &KGS_PlayerChatChannel::GetFriendPlayer;
	GetPlayer[emKCHANNEL_SERVER]	= &KGS_PlayerChatChannel::GetServerPlayer;

}

BOOL KGS_PlayerChatChannel::CheckCanUseChannel(INT nPlayerId, INT nChannelId)
{
/*
	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(nPlayerId);
	if (nPlayerIdx <= 0)
	{
		return TRUE;
	}

	KScriptFunctionCallHelper sfcHelper(*g_pMainScript);
	sfcHelper.SetMe(&Player[nPlayerIdx]);
	if (!g_pMainScript->CallTableFunction(
		"ChatChannel",
		"CheckPermission",
		1,
		"od",
		Player[nPlayerIdx].m_pLuna,
		nChannelId))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return g_pMainScript->GetInt(-1) > 0;*/
	return TRUE;

}

KEnumPlayer* KGS_PlayerChatChannel::GetEnumPlayer(INT nPlayerId, INT nChannelId)
{
	if (!GetPlayer[nChannelId])
	{
		return NULL;
	}

	return (this->*GetPlayer[nChannelId])(nPlayerId);
}

BOOL KGS_PlayerChatChannel::CheckChannelMsgInterval(INT nPlayerId, INT nChannelId, INT nIntervalTime)
{
	KPlayer* pSendPlayer = g_cPlayerMgr.GetByIndex(nPlayerId);
	KGC_CONFIRM_RET_FALSE(pSendPlayer);
	if(nChannelId <= 0 || nChannelId >= emKCHANNEL_ALL)
	{
		//动态频道
		return TRUE;
	}
/*
	time_t tmNow = KSysService::Time(NULL);
	if (pSendPlayer->m_aryLastChatTime[nChannelId] != 0 && 
		tmNow - pSendPlayer->m_aryLastChatTime[nChannelId] < nIntervalTime)
	{
		return FALSE;
	}
	
	pSendPlayer->m_aryLastChatTime[nChannelId] = tmNow;*/
	return TRUE;
}

KEnumPlayer* KGS_PlayerChatChannel::GetGMPlayer(INT nPlayerId)
{
	KEnumPlayer* psEnumPlayer = KEnumPlayer::GetEnumPlayer();
	return psEnumPlayer;
}

KEnumPlayer* KGS_PlayerChatChannel::GetTeamPlayer(INT nPlayerId)
{

#ifdef KD_OP_USING_SINGLE_SERVER_TEAM
	return NULL;
#else //#ifdef KD_OP_USING_SINGLE_SERVER_TEAM

	KEnumPlayer* psEnumPlayer = KEnumPlayer::GetEnumPlayer();

	KPlayer* pPlayer = g_cPlayerMgr.GetById(nPlayerId);
	INT nTeamId = pPlayer->GetTeamID();
	IKTeam sTeam = g_PlayerTeamManager.GetTeam(nTeamId);
	if (!sTeam)
	{
		return psEnumPlayer;
	}

	KITTableIntBranch itMemberSet = sTeam.GetMemberSet().GetTableItor();
	INT nMemberID = itMemberSet.GetCurItemKey();

	while(nMemberID > 0)
	{
		KPlayer* pPlayer = g_cPlayerMgr.GetById(nMemberID);
		INT nPlayerIndex = pPlayer->GetPlayerIndex();
		if (nPlayerIndex > 0)
		{
			psEnumPlayer->Push(nPlayerIndex);
		}
		nMemberID = itMemberSet.NextItemKey();
	}

	return psEnumPlayer;

#endif //#ifdef KD_OP_USING_SINGLE_SERVER_TEAM

}

KEnumPlayer* KGS_PlayerChatChannel::GetFactionPlayer(INT nPlayerId)
{

	KEnumPlayer* psEnumPlayer = KEnumPlayer::GetEnumPlayer();
/*
	KGC_Player* pPlayer = g_PlayerManager.GetPlayer(nPlayerId);
	INT nPlayerIndex = PlayerSet.GetFirstPlayer();

	while (nPlayerIndex > 0)
	{	
		if (pPlayer->GetFaction() == Player[nPlayerIndex].m_cPlayerFaction.GetFactionId())
		{
			psEnumPlayer->Push(nPlayerIndex);
		}
		nPlayerIndex = PlayerSet.GetNextPlayer();
	}*/

	return psEnumPlayer;

}

KEnumPlayer* KGS_PlayerChatChannel::GetKinPlayer(INT nPlayerId)
{

	KEnumPlayer* psEnumPlayer = KEnumPlayer::GetEnumPlayer();
/*
	KGC_Player* pPlayer = g_PlayerManager.GetPlayer(nPlayerId);
	DWORD dwKinId = pPlayer->GetKinId();
	CONST KGC_KinManager_GS::SET_PLAYER* pSetPlayer = g_KinManager.GetLocalKinMemberSet(dwKinId);
	if (!pSetPlayer)
	{
		return psEnumPlayer;
	}

	KGC_KinManager_GS::SET_PLAYER::const_iterator it = pSetPlayer->begin();
	for (; it != pSetPlayer->end(); ++it)
	{
		psEnumPlayer->Push(*it);
	}*/



	return psEnumPlayer;
}

KEnumPlayer* KGS_PlayerChatChannel::GetTongPlayer(INT nPlayerId)
{
	KEnumPlayer* psEnumPlayer = KEnumPlayer::GetEnumPlayer();
/*
	KGC_Player* pPlayer = g_PlayerManager.GetPlayer(nPlayerId);
	DWORD dwKinId = pPlayer->GetKinId();
	IKKin cKin = g_KinManager.GetKin(dwKinId);
	if (!cKin)
	{
		return psEnumPlayer;
	}

	DWORD dwTongId = (DWORD)cKin.GetBelongTong();
	IKTong cTong = g_TongManager.GetTong(dwTongId);
	if (!cTong)
	{
		return NULL;
	}

	KITTableIntBuf itMember = cTong.GetKinItor();
	dwKinId = (DWORD)itMember.GetCurItemKey();

	for (; dwKinId != 0; dwKinId = (DWORD)itMember.NextItemKey())
	{
		CONST KGC_KinManager_GS::SET_PLAYER* pSetPlayer = g_KinManager.GetLocalKinMemberSet(dwKinId);
		if (!pSetPlayer)
		{
			continue;
		}
		KGC_KinManager_GS::SET_PLAYER::const_iterator it = pSetPlayer->begin();
		for (; it != pSetPlayer->end(); ++it)
		{
			psEnumPlayer->Push(*it);
		}
	}*/
	return psEnumPlayer;
}
#define	CHAT_MAX_SYNC_RANGE	23


KEnumPlayer* KGS_PlayerChatChannel::GetCityPlayer(INT nPlayerId)
{
	KEnumPlayer* psEnumPlayer = KEnumPlayer::GetEnumPlayer();
	return psEnumPlayer;
}

KEnumPlayer* KGS_PlayerChatChannel::GetWorldPlayer(INT nPlayerId)
{
	KEnumPlayer* psEnumPlayer = KEnumPlayer::GetEnumPlayer();
	return psEnumPlayer;
}

KEnumPlayer* KGS_PlayerChatChannel::GetFriendPlayer(INT nPlayerId)
{
	KPlayer* pPlayer = g_cPlayerMgr.GetById(nPlayerId);


	if (!pPlayer)
	{
		return NULL;
	}

	KEnumPlayer* psEnumPlayer = KEnumPlayer::GetEnumPlayer();
/*
	std::list<std::string> listRelation;
	KSyncRelationList::Instance()->GetRelationList(listRelation, pPlayer->GetPlayerName(), emKPLAYERRELATION_TYPE_BINDFRIEND);
	std::list<std::string>::const_iterator it = listRelation.begin();

	for (; it != listRelation.end(); ++it)
	{
		INT nPlayerId	= g_PlayerManager.GetPlayerIDByName(it->c_str());
		INT nPlayerIdx	= g_PlayerManager.FindLocalPlayer(nPlayerId);

		if (nPlayerIdx > 0)
		{
			psEnumPlayer->Push(nPlayerIdx);
		}
	}
	// 将自己加入
	INT nPlayerIdx = g_PlayerManager.FindLocalPlayer(nPlayerId);
	if (nPlayerIdx > 0)
	{
		psEnumPlayer->Push(nPlayerIdx);
	}*/


	return psEnumPlayer;
}

KEnumPlayer* KGS_PlayerChatChannel::GetServerPlayer(INT nPlayerId)
{
/*
	KGC_Player* pPlayer = g_PlayerManager.GetPlayer(nPlayerId);
	if (!nPlayerId)
	{
		return FALSE;
	}

	ILeagueS LeagueS = g_LeagueManager.GetLeagueSet(emKLEAGUETYPE_WLDH_CHANNEL, FALSE);
	if (!LeagueS || !pPlayer)
		return NULL;

	LPCSTR pszLeague	= LeagueS.GetMemberLeague(pPlayer->GetPlayerName());
	if (!pszLeague)
	{
		return NULL;
	}

	ILeague cLG = g_LeagueManager.GetLeague(emKLEAGUETYPE_WLDH_CHANNEL, pszLeague);
	if (!cLG)
	{
		return NULL;
	}
*/

	KEnumPlayer* psEnumPlayer = KEnumPlayer::GetEnumPlayer();
/*
	KITTableStrBranch TBch = cLG.GetMemberTableItor();
	LPCSTR szMemberName = TBch.GetCurItemKey();

	for (; szMemberName; szMemberName = TBch.NextItemKey())
	{
		INT nIndex	= PlayerSet.FindRole(szMemberName);
		if (nIndex > 0)
		{
			psEnumPlayer->Push(nIndex);
		}
	}*/

	return psEnumPlayer;
}



// -------------------------------------------------------------------------

#endif /* _SERVER */
// -------------------------------------------------------------------------
