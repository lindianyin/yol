
#include "stdafx.h"
#include "onlinegamemodule/qplayerteam.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamemodule/kteamprotocol.h"
#include "kteammodule.h"
#include "qteamserver.h"
#include "onlinegameworld/korpgworld.h"
#include "qteamgchandler.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "onlinegameworld/ikscenemgr.h"
#include "../onlinegameworld/scene/kscene.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/chat/kchatmodule.h"


BYTE	QPlayerTeam::m_byTempData[32*1024] = {0};

QPlayerTeam::QPlayerTeam( KCharacter& rcPlayer ) : KCharacterCtrlBase(rcPlayer), m_rPlayer((KPlayer&)rcPlayer)
{

}

QPlayerTeam::~QPlayerTeam()
{
	UnInit();
}


BOOL QPlayerTeam::Init()
{
	return TRUE;
}

BOOL QPlayerTeam::UnInit()
{
	return TRUE;
}

BOOL QPlayerTeam::Active()
{
	BOOL bRetCode = FALSE;
	KScene* pScene = NULL;
	CONST KSCENE_CONFIG* pCfg = NULL;
	INT nFrame = g_cOrpgWorld.m_dwLogicFrames;

	// 全局同步队友最大血量等
	if (nFrame % GAME_FPS == 0)
	{
		QTeamGCHandler::DoTeamSyncMemberMaxLMR(&m_rPlayer);
	}

	// 跨服同步队友当前血量等
	if (nFrame % GAME_FPS == 0)
	{
		QTeamGCHandler::DoTeamSyncMemberCurrentLMR(&m_rPlayer);
	}

	// 同步附近队友当前生命,注意,这个同步的频率会被跨服同步覆盖
	if (nFrame % 7 == 0)
	{
		DoSyncTeamMemberCurrentLMRLocal(&m_rPlayer);
	}

	// 跨服同步队友当前位置
	if (nFrame % (GAME_FPS * 2) == 0)
	{
		QTeamGCHandler::DoSyncTeamMemberPosition(&m_rPlayer);
	}

	// 本地同步队友当前位置,注意,这个同步的频率会被跨服同步覆盖
	if (nFrame % GAME_FPS == 0)
	{
		DoSyncTeamMemberPositionLocal(&m_rPlayer);
	}

	// 全局同步队友杂项参数
	if (nFrame % (2 * GAME_FPS) == 0)
	{
		QTeamGCHandler::DoTeamSyncMemberMisc(&m_rPlayer);
	}

	pScene = (KScene*)m_rPlayer.GetScene();
	PROCESS_ERROR(pScene);

	pCfg = g_pSceneMgr->GetSceneInfo(pScene->GetTemplateId());
	LOG_PROCESS_ERROR(pCfg);

	if ((!pScene->m_bSaved) && m_rPlayer.m_cPlayerServer.m_nBanishTime == 0 
		&& (nFrame - (int)m_rPlayer.GetId()) % (GAME_FPS * 5) == 0 
		&& strcmp(pCfg->szSceneType, "fb") == 0)
	{
		DWORD dwSceneOwner = pScene->m_dwOwnerID;

		if (dwSceneOwner != KD_BAD_ID && m_rPlayer.GetId() != dwSceneOwner)
		{
			int nBanishDelay = BANISH_PLAYER_WAIT_SECONDS;

			if (m_rPlayer.GetTeamId() == KD_BAD_ID)
			{
				m_rPlayer.m_cPlayerServer.m_nBanishTime = KSysService::GameTime(NULL) + BANISH_PLAYER_WAIT_SECONDS;
				g_cPlayerBaseProcess.DoMessageNotify(
					&m_rPlayer, ectBanishNotifyCode, bpcNotInMapOwnerTeam,
					&nBanishDelay, sizeof(nBanishDelay)
					);
				g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, m_rPlayer.GetId(),
					"DEBUG", "你退出了副本队伍，即将离开副本。",emQCHAT_PANEL); // DoMessageNotify起作用后删除这行
			}
			else
			{
				bRetCode = g_cTeamServer.IsPlayerInTeam(m_rPlayer.GetTeamId(), dwSceneOwner);
				if (!bRetCode)
				{
					m_rPlayer.m_cPlayerServer.m_nBanishTime = KSysService::GameTime(NULL) + BANISH_PLAYER_WAIT_SECONDS;
					g_cPlayerBaseProcess.DoMessageNotify(
						&m_rPlayer, ectBanishNotifyCode, bpcNotInMapOwnerTeam,
						&nBanishDelay, sizeof(nBanishDelay)
						);
					g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, m_rPlayer.GetId(),
						"DEBUG", "你加入其他队伍，即将离开副本。", emQCHAT_PANEL); // DoMessageNotify起作用后删除这行
				}
			}
		}
	}

	if (m_rPlayer.m_cPlayerServer.m_nBanishTime > 0 &&
		KSysService::GameTime(NULL) > m_rPlayer.m_cPlayerServer.m_nBanishTime)
	{
		m_rPlayer.m_cPlayerServer.ReturnToLastCity();

		m_rPlayer.m_cPlayerServer.m_nBanishTime = 0;
	}

	PROCESS_ERROR(m_rPlayer.GetScene());

	if (m_rPlayer.m_cPlayerServer.m_nBanishTime > 0 && (nFrame - (int)m_rPlayer.GetId()) % (GAME_FPS * 3) == 0)
	{
		DWORD dwSceneOwner  = pScene->m_dwOwnerID;
		BOOL  bInOwnerTeam = false;

		if (m_rPlayer.GetTeamId() != KD_BAD_ID)
		{
			bInOwnerTeam = g_cTeamServer.IsPlayerInTeam(m_rPlayer.GetTeamId(), dwSceneOwner);
		}

		if (m_rPlayer.GetId() == dwSceneOwner || bInOwnerTeam)
		{
			m_rPlayer.m_cPlayerServer.m_nBanishTime = 0;
			g_cPlayerBaseProcess.DoMessageNotify(&m_rPlayer, ectBanishNotifyCode, bpcCancelBanish);
			g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, m_rPlayer.GetId(),
				"DEBUG", "离开副本倒计时已停止。", emQCHAT_PANEL); // DoMessageNotify起作用后删除这行
		}
	}
EXIT0:
	return TRUE;
}

// 组队操作前的判断
BOOL QPlayerTeam::CanTeamProcess(KPlayer* pDstPlayer)
{
	BOOL bRet = FALSE;
	KScene* pScene = NULL;
	INT nMapId = 0;

	pScene = (KScene*)pDstPlayer->GetScene();
	LOG_PROCESS_ERROR(pScene);

	nMapId = pScene->GetTemplateId();

	if(g_pSceneMgr->GetSceneType(nMapId) == emtDungeon || nMapId == 97 /*准备场*/)
	{
		goto EXIT0;
	}

	bRet = TRUE;
EXIT0:
	return bRet;
}

void QPlayerTeam::ResetTeamLastSyncParamRecord()
{
	m_nLastSyncLocalTeamLifePercent            = -1;
	m_nLastSyncLocalTeamManaPercent            = -1;
	m_nLastSyncLocalTeamRagePercent            = -1;
	m_dwLastSyncLocalTeamMemberMapID           = 0;
	m_nLastSyncLocalTeamMemberMapCopyIndex     = 0;
	m_nLastSyncLocalTeamMemberPosX             = -1;
	m_nLastSyncLocalTeamMemberPosY             = -1;

	m_nLastSyncGlobalTeamLifePercent           = -1;
	m_nLastSyncGlobalTeamManaPercent           = -1;
	m_nLastSyncGlobalTeamRagePercent           = -1;
	m_dwLastSyncGlobalTeamMemberMapID          = 0;
	m_nLastSyncGlobalTeamMemberMapCopyIndex    = 0;
	m_nLastSyncGlobalTeamMemberPosX            = -1;
	m_nLastSyncGlobalTeamMemberPosY            = -1;
}

void QPlayerTeam::TeamBroadcast(DWORD dwTeamID, void* pvData, size_t uSize)
{
	BOOL           bRetCode = false;
	QTeamBroadcast Func;

	Func.m_dwTeamID = dwTeamID;
	Func.m_pvData   = pvData;
	Func.m_uSize    = uSize;

	bRetCode = g_cTeamServer.Traverse(Func);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

BOOL QPlayerTeam::DoLoginTeamSync(KPlayer* pPlayer)
{
	BOOL    bResult  = false;
	int     nRetCode = false;
	DWORD   dwTeamID = KD_BAD_ID;

	dwTeamID = pPlayer->GetTeamId();
	PROCESS_SUCCESS(dwTeamID == KD_BAD_ID);

	nRetCode = DoSyncTeamBaseInfo(dwTeamID, pPlayer);
	LOG_PROCESS_ERROR(nRetCode);

EXIT1:
	bResult = true;
EXIT0:
	return bResult;
}

BOOL QPlayerTeam::DoSyncTeamBaseInfo(DWORD dwTeamID, KPlayer* pPlayer, BOOL bAddTeamMemberFlag /* = false */)
{
	int                              nResult            = false;
	int                              nRetCode           = false;
	int                              nMemberIndex       = 0;
	size_t                           uLeftSize          = sizeof(m_byTempData);
	size_t                           uUsedSize          = 0;
	QTeam*                           pTeam              = NULL;
	S2C_SYNC_PLAYER_TEAM_BASE_INFO*  pSyncTeamBaseInfo = (S2C_SYNC_PLAYER_TEAM_BASE_INFO*)m_byTempData;

	pTeam = g_cTeamServer.GetTeam(dwTeamID);
	PROCESS_ERROR(pTeam);

	pPlayer->SetTeamId(dwTeamID);

	LOG_PROCESS_ERROR(sizeof(S2C_SYNC_PLAYER_TEAM_BASE_INFO) <= uLeftSize);
	uLeftSize -= sizeof(S2C_SYNC_PLAYER_TEAM_BASE_INFO);
	uUsedSize += sizeof(S2C_SYNC_PLAYER_TEAM_BASE_INFO);

	pSyncTeamBaseInfo->dwTeamID             = dwTeamID;
	pSyncTeamBaseInfo->bSystem               = pTeam->bSystem;
	pSyncTeamBaseInfo->dwLeaderID            = pTeam->dwAuthority[tatLeader];
	pSyncTeamBaseInfo->nGroupNum             = pTeam->nGroupNum;
	pSyncTeamBaseInfo->nLootMode             = pTeam->nLootMode;
	pSyncTeamBaseInfo->nRollQuality          = pTeam->nRollQuality;
	//pSyncTeamBaseInfo->byCamp                = (BYTE)pTeam->eCamp;
	pSyncTeamBaseInfo->dwDistributeMan       = pTeam->dwAuthority[tatDistribute];
	pSyncTeamBaseInfo->dwMarkMan             = pTeam->dwAuthority[tatMark];
	pSyncTeamBaseInfo->bAddTeamMemberFlag   = bAddTeamMemberFlag;

	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		pSyncTeamBaseInfo->dwFormationLeader[i] = pTeam->MemberGroup[i].dwFormationLeader;
	}

	memcpy(pSyncTeamBaseInfo->dwTeamMark, pTeam->dwMark, sizeof(pSyncTeamBaseInfo->dwTeamMark));

	for (int i = 0; i < pTeam->nGroupNum; i++)
	{
		for (
			QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
			it != pTeam->MemberGroup[i].MemberList.end(); ++it
			)
		{
			size_t   uSize   = 0;

			LOG_PROCESS_ERROR(sizeof(S2C_SYNC_PLAYER_TEAM_BASE_INFO::KMEMBER_INFO) <= uLeftSize);
			uLeftSize -= sizeof(S2C_SYNC_PLAYER_TEAM_BASE_INFO::KMEMBER_INFO);
			uUsedSize += sizeof(S2C_SYNC_PLAYER_TEAM_BASE_INFO::KMEMBER_INFO);

			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].dwMemberID       = it->dwMemberID;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].byGroupIndex     = (BYTE)i;
			//pSyncTeamBaseInfo->MemberInfo[nMemberIndex].PortraitInfo     = it->PortraitInfo;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].dwMapID          = it->dwMapID;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].byForceID        = (BYTE)it->dwForceID;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].byLevel          = (BYTE)it->nLevel;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].byOnLineFlag     = (BYTE)it->bOnlineFlag;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].nMaxLife         = it->nMaxLife;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].nMaxMana         = it->nMaxMana; 
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].nMaxRage         = it->nMaxRage;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].byLifePercent    = (BYTE)it->nLifePercent;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].byManaPercent    = (BYTE)it->nManaPercent;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].byRagePercent    = (BYTE)it->nRagePercent;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].dwMapID          = it->dwMapID;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].nMapCopyIndex    = it->nMapCopyIndex;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].nPosX            = it->nPosX;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].nPosY            = it->nPosY;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].byRoleType       = (BYTE)it->nRoleType;
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].byDeathFlag      = (BYTE)it->bDeathFlag;

			uSize = sizeof(pSyncTeamBaseInfo->MemberInfo[nMemberIndex].szMemberName);
			strncpy(
				pSyncTeamBaseInfo->MemberInfo[nMemberIndex].szMemberName, it->szRoleName, uSize
				);
			pSyncTeamBaseInfo->MemberInfo[nMemberIndex].szMemberName[uSize - 1] = '\0';

			nMemberIndex++;
		}
	}

	pSyncTeamBaseInfo->byMemberCount = (BYTE)nMemberIndex;

	LOG_PROCESS_ERROR(uUsedSize < USHRT_MAX);
	pSyncTeamBaseInfo->wDataLen = (WORD)uUsedSize;

	nRetCode = QTeamModule::SendLenData(pPlayer->GetPlayerIndex(), *pSyncTeamBaseInfo, uUsedSize);
	PROCESS_ERROR(nRetCode);

	nResult = true;
EXIT0:
	return nResult;
}

BOOL QPlayerTeam::DoInvitePlayerJoinTeamRequest(DWORD dwInviteDst, const char cszInviteSrc[])
{
	int                             nResult         = false;
	int                             nRetCode        = false;
	KPlayer*                        pInviteDst      = NULL;
	S2C_INVITE_JOIN_TEAM_REQUEST    TeamInvite;

	pInviteDst = g_cPlayerMgr.GetById(dwInviteDst);
	PROCESS_ERROR(pInviteDst);

	PROCESS_ERROR(CanTeamProcess(pInviteDst));

	strncpy(TeamInvite.szInviteSrc, cszInviteSrc, sizeof(TeamInvite.szInviteSrc));
	TeamInvite.szInviteSrc[sizeof(TeamInvite.szInviteSrc) - 1] = '\0';

	nRetCode = QTeamModule::SendData(pInviteDst->GetPlayerIndex(), TeamInvite);
	PROCESS_ERROR(nRetCode);

	nResult = true;
EXIT0:
	return nResult;
}

BOOL QPlayerTeam::DoApplyJoinTeamRequest(DWORD dwLeader, const char cszApplySrc[])
{
	int                             nResult         = false;
	int                             nRetCode        = false;
	KPlayer*                        pTeamLeader    = NULL;
	S2C_APPLY_JOIN_TEAM_REQUEST     TeamApply;

	pTeamLeader = g_cPlayerMgr.GetById(dwLeader);
	PROCESS_ERROR(pTeamLeader);

	PROCESS_ERROR(CanTeamProcess(pTeamLeader));

	strncpy(TeamApply.szApplySrc, cszApplySrc, sizeof(TeamApply.szApplySrc));
	TeamApply.szApplySrc[sizeof(TeamApply.szApplySrc) - 1] = '\0';

	nRetCode = QTeamModule::SendData(pTeamLeader->GetPlayerIndex(), TeamApply);
	PROCESS_ERROR(nRetCode);

	nResult = true;
EXIT0:
	return nResult;
}

BOOL QPlayerTeam::DoTeamAddMemberNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwMemberID)
{
	int                         nResult     = false;
	KPlayer*                    pPlayer     = NULL;
	QTEAM_MEMBER_INFO*          pMemberInfo = NULL;
	S2C_TEAM_ADD_MEMBER_NOTIFY  Notify;
	QTeamBroadcast              Func;

	pMemberInfo = g_cTeamServer.GetMember(dwTeamID, dwMemberID);
	LOG_PROCESS_ERROR(pMemberInfo);

	Notify.dwTeamID    = dwTeamID;
	Notify.nGroupIndex  = nGroupIndex;
	Notify.dwMemberID   = dwMemberID;

	strncpy(Notify.szMemberName, pMemberInfo->szRoleName, sizeof(Notify.szMemberName));
	Notify.szMemberName[sizeof(Notify.szMemberName) - 1] = '\0';

	Func.m_dwExceptID = dwMemberID;
	Func.m_dwTeamID   = dwTeamID;
	Func.m_pvData     = (void*)&Notify;
	Func.m_uSize      = sizeof(S2C_TEAM_ADD_MEMBER_NOTIFY);

	g_cTeamServer.Traverse(Func);

	nResult = true;
EXIT0:
	return nResult;
}

BOOL QPlayerTeam::DoTeamDelMemberNotify(DWORD dwTeamID, int nGroupIndex, DWORD dwMemberID)
{
	int                         nResult                 = false;
	DWORD                       dwPlayerID              = 0;
	KPlayer*                    pPlayer                 = NULL;
	S2C_TEAM_DEL_MEMBER_NOTIFY  TeamDelMemberNotify;
	QTeamBroadcast              Func;

	TeamDelMemberNotify.dwTeamID    = dwTeamID;
	TeamDelMemberNotify.nGroupIndex  = nGroupIndex;
	TeamDelMemberNotify.dwMemberID   = dwMemberID;

	pPlayer = g_cPlayerMgr.GetById(dwMemberID);

	if (pPlayer)
	{
		QTeamModule::SendData(pPlayer->GetPlayerIndex(), TeamDelMemberNotify);
	}
	//else: 玩家可能不在本服务器上

	// 为什么不把上面的这个Send纳入下面的循环? 因为这个玩家可能已经从列表中删除了,循环就遍历不到这个玩家.

	Func.m_dwTeamID     = dwTeamID;
	Func.m_dwExceptID   = dwMemberID;
	Func.m_pvData       = (void*)&TeamDelMemberNotify;
	Func.m_uSize        = sizeof(TeamDelMemberNotify);

	g_cTeamServer.Traverse(Func);

	nResult = true;
EXIT0:
	return nResult;
}

BOOL QPlayerTeam::DoTeamChangeAuthorityNotify(DWORD dwTeamID, TEAM_AUTHORITY_TYPE eType, DWORD dwTargetID)
{
	int                                 nResult     = false;
	S2C_TEAM_CHANGE_AUTHORITY_NOTIFY    Notify;
	QTeamBroadcast                      Func;

	Notify.dwTeamID      = dwTeamID;
	Notify.byType         = (BYTE)eType;
	Notify.dwNewID        = dwTargetID;

	// 逐个通知各个队员，注: 队员可能既不在新队长附近，也不在旧队长附近
	// 客户端的 "xxx成为队长"的消息是这里通知的，而上面主要是设置队长的标旗
	Func.m_dwTeamID = dwTeamID;
	Func.m_pvData   = (void*)&Notify;
	Func.m_uSize    = sizeof(Notify);

	g_cTeamServer.Traverse(Func);

	nResult = true;
EXIT0:
	return nResult;
}

BOOL QPlayerTeam::DoTeamSetLootModeNotify(DWORD dwTeamID, int nLootMode, int nRollQuality)
{
	int                             nResult     = false;
	S2C_TEAM_SET_LOOT_MODE_NOTIFY   Notify;

	Notify.dwTeamID         = dwTeamID;
	Notify.nLootMode		= nLootMode;
	Notify.nRollQuality		= nRollQuality;

	TeamBroadcast(dwTeamID, &Notify, sizeof(Notify));

	nResult = true;
EXIT0:
	return nResult;
}

BOOL QPlayerTeam::DoTeamDisbandNotify(DWORD dwTeamID)
{
	int                     nResult     = false;
	int                     nRetCode    = false;
	S2C_TEAM_DISBAND_NOTIFY Notify;

	Notify.dwTeamID    = dwTeamID;

	TeamBroadcast(dwTeamID, &Notify, sizeof(Notify));        

	nResult = true;
EXIT0:
	return nResult;
}

BOOL QPlayerTeam::DoSyncTeamMemberOnlineFlag(DWORD dwTeamID, DWORD dwMemberID, BOOL bOnlineFlag)
{
	int                                 nResult             = false;
	S2C_SYNC_TEAM_MEMBER_ONLINE_FLAG    MemberOnlineInfo;

	MemberOnlineInfo.dwTeamID    = dwTeamID;
	MemberOnlineInfo.dwMemberID   = dwMemberID;
	MemberOnlineInfo.bOnlineFlag  = bOnlineFlag;

	TeamBroadcast(dwTeamID, &MemberOnlineInfo, sizeof(MemberOnlineInfo));

	nResult = true;
EXIT0:
	return nResult;
}

BOOL QPlayerTeam::DoSyncTeamMemberMaxLMR(DWORD dwTeamID, QTEAM_MEMBER_INFO* pMember)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	S2C_SYNC_TEAM_MEMBER_MAX_LMR    Pak;

	Pak.dwMemberID   = pMember->dwMemberID;
	Pak.nMaxLife     = pMember->nMaxLife;
	Pak.nMaxMana     = pMember->nMaxMana;
	Pak.nMaxRage     = pMember->nMaxRage;

	TeamBroadcast(dwTeamID, &Pak, sizeof(Pak));

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QPlayerTeam::DoSyncTeamMemberCurrentLMRGlobal(DWORD dwTeamID, QTEAM_MEMBER_INFO* pMember)
{
	BOOL                                bResult         = false;
	BOOL                                bRetCode        = false;
	S2C_SYNC_TEAM_MEMBER_CURRENT_LMR    Pak;

	Pak.dwMemberID      = pMember->dwMemberID;
	Pak.byLifePercent   = (BYTE)pMember->nLifePercent;
	Pak.byManaPercent   = (BYTE)pMember->nManaPercent;
	Pak.byRagePercent   = (BYTE)pMember->nRagePercent;

	TeamBroadcast(dwTeamID, &Pak, sizeof(Pak));

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QPlayerTeam::DoSyncTeamMemberCurrentLMRLocal(KPlayer* pPlayer)
{
	BOOL                                bResult         = false;
	BOOL                                bRetCode        = false;
	QTeam*                              pTeam           = NULL;
	BOOL                                bValueChanged   = false;
	int                                 nLifePercent    = 0;
	int                                 nManaPercent    = 0;
	int                                 nRagePercent    = 0;
	DWORD                               dwTeamID        = KD_BAD_ID;
	S2C_SYNC_TEAM_MEMBER_CURRENT_LMR    Pak;

	dwTeamID = pPlayer->GetTeamId();
	PROCESS_ERROR(dwTeamID != KD_BAD_ID);

	//if (pPlayer->m_nMaxLife > 0)
	//	nLifePercent = pPlayer->m_nCurrentLife * UCHAR_MAX / pPlayer->m_nMaxLife;

	//if (pPlayer->m_nMaxMana > 0)
	//	nManaPercent = pPlayer->m_nCurrentMana * UCHAR_MAX / pPlayer->m_nMaxMana;

	//if (pPlayer->m_nMaxRage > 0)
	//	nRagePercent = pPlayer->m_nCurrentRage * UCHAR_MAX / pPlayer->m_nMaxRage;

	//bValueChanged |= (pPlayer->m_nLastSyncLocalTeamLifePercent != nLifePercent);
	//bValueChanged |= (pPlayer->m_nLastSyncLocalTeamManaPercent != nManaPercent);
	//bValueChanged |= (pPlayer->m_nLastSyncLocalTeamRagePercent != nRagePercent);

	//PROCESS_ERROR(bValueChanged);

	//pPlayer->m_nLastSyncLocalTeamLifePercent = nLifePercent;
	//pPlayer->m_nLastSyncLocalTeamManaPercent = nManaPercent;
	//pPlayer->m_nLastSyncLocalTeamRagePercent = nRagePercent;

	//Pak.byProtocolID     = s2c_sync_team_member_current_lmr;
	//Pak.dwMemberID       = pPlayer->m_dwID;
	//Pak.byLifePercent    = (BYTE)nLifePercent;
	//Pak.byManaPercent    = (BYTE)nManaPercent;
	//Pak.byRagePercent    = (BYTE)nRagePercent;

	//pTeam = g_cTeamServer.GetTeam(dwTeamID);
	//LOG_PROCESS_ERROR(pTeam);

	//for (int i = 0 ; i < pTeam->nGroupNum; i++)
	//{
	//	for (
	//		QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
	//		it != pTeam->MemberGroup[i].MemberList.end(); ++it
	//		)
	//	{
	//		KPlayer* pMember = g_cPlayerMgr.GetById(it->dwMemberID);

	//		if (it->dwMemberID == pPlayer->m_dwID)
	//		{
	//			it->nLifePercent   = nLifePercent;
	//			it->nManaPercent   = nManaPercent;
	//			it->nRagePercent   = nRagePercent;
	//			continue;
	//		}

	//		if (!pMember)
	//			continue;

	//		if (pMember->m_pScene != pPlayer->m_pScene)
	//			continue;

	//		Send(pMember->m_nConnIndex, &Pak, sizeof(Pak));
	//	}
	//}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QPlayerTeam::DoSyncTeamMemberMisc(DWORD dwTeamID, QTEAM_MEMBER_INFO* pMember)
{
	BOOL                        bResult     = false;
	BOOL                        bRetCode    = false;
	S2C_SYNC_TEAM_MEMBER_MISC   Pak;

	Pak.dwMemberID   = pMember->dwMemberID;
	//Pak.PortraitInfo = pMember->PortraitInfo;
	Pak.dwMapID      = pMember->dwMapID;
	Pak.dwForceID    = pMember->dwForceID;
	//Pak.byCamp       = (BYTE)pMember->eCamp;
	Pak.byLevel      = (BYTE)pMember->nLevel;
	Pak.byRoleType   = (BYTE)pMember->nRoleType;
	Pak.bDeathFlag   = pMember->bDeathFlag;

	TeamBroadcast(dwTeamID, &Pak, sizeof(Pak));

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QPlayerTeam::DoSyncTeamMemberPositionGlobal(DWORD dwTeamID, QTEAM_MEMBER_INFO* pMember)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	S2C_SYNC_TEAM_MEMBER_POSITION   Pak;

	Pak.dwMemberID      = pMember->dwMemberID;
	Pak.dwMapID         = pMember->dwMapID;
	Pak.nMapCopyIndex   = pMember->nMapCopyIndex;
	Pak.nPosX           = pMember->nPosX;
	Pak.nPosY           = pMember->nPosY;

	TeamBroadcast(dwTeamID, &Pak, sizeof(Pak));

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QPlayerTeam::DoSyncTeamMemberPositionLocal(KPlayer* pPlayer)
{
	BOOL                             bResult        = false;
	BOOL                             bRetCode       = false;
	QTeam*                           pTeam          = NULL;
	BOOL                             bValueChanged  = false;
	DWORD                            dwTeamID       = KD_BAD_ID;
	S2C_SYNC_TEAM_MEMBER_POSITION    Pak;

	//PROCESS_ERROR(pPlayer->m_pScene);

	//dwTeamID = pPlayer->GetCurrentTeamID();
	//PROCESS_ERROR(dwTeamID != KD_BAD_ID);

	//bValueChanged |= (pPlayer->m_pScene->m_dwMapID != pPlayer->m_dwLastSyncLocalTeamMemberMapID);
	//bValueChanged |= (pPlayer->m_pScene->m_nCopyIndex != pPlayer->m_nLastSyncLocalTeamMemberMapCopyIndex);
	//bValueChanged |= (pPlayer->m_nX != pPlayer->m_nLastSyncLocalTeamMemberPosX);
	//bValueChanged |= (pPlayer->m_nY != pPlayer->m_nLastSyncLocalTeamMemberPosY);

	//PROCESS_ERROR(bValueChanged);

	//pPlayer->m_dwLastSyncLocalTeamMemberMapID         = pPlayer->m_pScene->m_dwMapID;
	//pPlayer->m_nLastSyncLocalTeamMemberMapCopyIndex   = pPlayer->m_pScene->m_nCopyIndex;
	//pPlayer->m_nLastSyncLocalTeamMemberPosX           = pPlayer->m_nX;
	//pPlayer->m_nLastSyncLocalTeamMemberPosY           = pPlayer->m_nY;

	//Pak.byProtocolID   = s2c_sync_team_member_position;
	//Pak.dwMemberID     = pPlayer->m_dwID;
	//Pak.dwMapID        = pPlayer->m_pScene->m_dwMapID;
	//Pak.nMapCopyIndex  = pPlayer->m_pScene->m_nCopyIndex;
	//Pak.nPosX          = pPlayer->m_nX;
	//Pak.nPosY          = pPlayer->m_nY;

	//pTeam = g_cTeamServer.GetTeam(dwTeamID);
	//LOG_PROCESS_ERROR(pTeam);

	//for (int i = 0; i < pTeam->nGroupNum; i++)
	//{
	//	for (
	//		QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
	//		it != pTeam->MemberGroup[i].MemberList.end(); ++it
	//		)
	//	{
	//		KPlayer* pMember = g_cPlayerMgr.GetById(it->dwMemberID);

	//		if (it->dwMemberID == pPlayer->m_dwID)
	//		{
	//			it->dwMapID        = pPlayer->m_pScene->m_dwMapID;
	//			it->nMapCopyIndex  = pPlayer->m_pScene->m_nCopyIndex;
	//			it->nPosX          = pPlayer->m_nX;
	//			it->nPosY          = pPlayer->m_nY;
	//			continue;
	//		}

	//		if (!pMember)
	//			continue;

	//		if (pMember->m_pScene != pPlayer->m_pScene)
	//			continue;

	//		Send(pMember->m_nConnIndex, &Pak, sizeof(Pak));
	//	}
	//}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QPlayerTeam::DoTeamLevelUpRaidNotify(DWORD dwTeamID, int nGroupNum)
{
	BOOL                            bResult     = false;
	S2C_TEAM_LEVEL_UP_RAID_NOTIFY   Notify;

	Notify.dwTeamID     = dwTeamID;
	Notify.byGroupNum   = (BYTE)nGroupNum;

	TeamBroadcast(dwTeamID, &Notify, sizeof(Notify));

	bResult = true;
EXIT0:
	return bResult;
}

BOOL QPlayerTeam::OnLogin()
{
	DoLoginTeamSync(&m_rPlayer);
	return TRUE;
}


