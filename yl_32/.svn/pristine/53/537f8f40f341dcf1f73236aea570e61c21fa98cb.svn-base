
#ifdef GAME_SERVER
#include "stdafx.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/kcharacterserver.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/synchelper.h"
#include "kvarsetting.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "kplayerbaseprocess.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
BOOL KCharacterServer::SyncVarAround( WORD wVarGroup, WORD wVarId )
{
	BOOL 		bResult 		= FALSE;
	KS2C_SYNC_VAR_AROUND sSync;
	IKScene*	pScene 			= NULL;
	DWORD 		dwSceneId     	= 0;
	BOOL 		bNpc			= FALSE;
	KNpc		*pNpc          	= FALSE;
	DWORD		dwTemplateId 	=  0;
	sSync.dwId = m_rcCharacter.GetId();
	sSync.byVarGroup = (BYTE)wVarGroup;
	sSync.byVarId = (BYTE)wVarId;
	sSync.dwVarValue = (DWORD)m_rcCharacter.GetVar(wVarGroup, wVarId);
	pScene = m_rcCharacter.GetScene();
	PROCESS_ERROR(pScene);
	dwSceneId = pScene->GetId();
	bNpc = IS_NPC(m_rcCharacter.GetId());
	//by ldy 判读蛮荒之地里面的boss才同步血量
	if((dwSceneId >= 353 && dwSceneId <= 356) && bNpc)
	{
		pNpc = (KNpc*)&m_rcCharacter;
		dwTemplateId = pNpc->GetTemplateId();
		PROCESS_ERROR(dwTemplateId >= 2807 && dwTemplateId <= 2813);
	}
	bResult = BroadcastNearby(&sSync, sizeof(sSync), FALSE);
	bResult = TRUE;
EXIT0:
	return bResult;
}


// 广播到附近玩家
BOOL KCharacterServer::BroadcastNearby(
	LPCVOID pbyData, SIZE_T uSize, BOOL bIncludeHimself, KPlayerProtocolProcessor* pProtocolSender /*= NULL*/ )
{
	QCONFIRM_RET_FALSE(pbyData && uSize > 0);

	// broadcast
	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = pbyData;
	cBroadcaster.m_uSize = uSize;
	//cBroadcaster.m_nRange = 1; //by ldy 设置广播范围

	if ( ! bIncludeHimself)
		cBroadcaster.m_pExcept = &m_rcCharacter;
	if (pProtocolSender)
		cBroadcaster.m_pProcessor = pProtocolSender;

	//HWD_MODIFY 在初始化期间有些数据需要同步，此时可能尚无场景
	IKScene* pScene = m_rcCharacter.GetScene();
	if (NULL  == pScene)
		return FALSE;

	return pScene->TraverseNearbyPlayers(cBroadcaster, &m_rcCharacter); // 向自己周围玩家广播
}

BOOL KCharacterServer::BroadcastScene( LPCBYTE pbyData, SIZE_T uSize, BOOL bIncludeHimself, KPlayerProtocolProcessor* pProtocolSender /*= NULL*/ )
{
	QCONFIRM_RET_FALSE(pbyData && uSize > 0);

	// broadcast
	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = pbyData;
	cBroadcaster.m_uSize = uSize;
	cBroadcaster.m_nMaxSyncCount = INT_MAX;

	if ( ! bIncludeHimself)
		cBroadcaster.m_pExcept = &m_rcCharacter;
	if (pProtocolSender)
		cBroadcaster.m_pProcessor = pProtocolSender;

	IKScene* pScene = m_rcCharacter.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	return pScene->TraverseAllPlayers(cBroadcaster);
}

BOOL KCharacterServer::BroadcastThisServer(
	LPCBYTE pbyData, SIZE_T uSize, KCharacter* pPlayerToExclude,
	KPlayerProtocolProcessor* pProtocolSender /*= NULL*/
)
{
	BOOL bRet = TRUE;
	QCONFIRM_RET_FALSE(pbyData && uSize > 0);

	if ( ! pProtocolSender)
		pProtocolSender = g_pBaseSender;

	// 如果玩家不在任何场景里，不必向他同步?
	//IKScene* pScene = g_pSceneMgr->FirstScene();
	//while (pScene)
	//{
	//	IKScene* pNextScene = g_pSceneMgr->NextScene();
	//	// broadcast
	//	KBroadcastFunc cBroadcaster;
	//	cBroadcaster.m_pvData = pbyData;
	//	cBroadcaster.m_uSize = uSize;
	//	cBroadcaster.m_pExcept = pPlayerToExclude;

	//	if (pProtocolSender)
	//		cBroadcaster.m_pProcessor = pProtocolSender;

	//	bRet &= pScene->TraverseAllPlayers(cBroadcaster);

	//	pScene = pNextScene;
	//}

	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = pbyData;
	cBroadcaster.m_uSize = uSize;
	cBroadcaster.m_pExcept = pPlayerToExclude;
	cBroadcaster.m_nMaxSyncCount = INT_MAX;

	if (pProtocolSender)
		cBroadcaster.m_pProcessor = pProtocolSender;

	std::vector<KPlayer*> vPlayers;
	g_cPlayerMgr.GetAllPlayers(vPlayers);
	for (std::vector<KPlayer*>::iterator it = vPlayers.begin(); it != vPlayers.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		bRet &= cBroadcaster.OnTraverse(pPlayer);
	}

	return bRet;
}

BOOL KCharacterServer::SyncVarChange( WORD wVarGroup, WORD wVarId )
{
	if (IS_PLAYER(m_rcCharacter.GetId()) && g_cCharacterVarSetting.CheckSyncSelf(wVarGroup, wVarId))
		DoSyncVarSelf(wVarGroup, wVarId);
	if (g_cCharacterVarSetting.CheckSyncOther(wVarGroup, wVarId))
		SyncVarAround(wVarGroup, wVarId);
	return TRUE;
}

VOID KCharacterServer::SetVisible( BOOL bVisible )
{
	// 显身 --> 隐身
	if (m_rcCharacter.m_bVisible && ! bVisible)
	{
		if (IS_PLAYER(m_rcCharacter.GetId()))
		{
			KPlayer& rPlayer = static_cast<KPlayer&>(m_rcCharacter);
			g_cPlayerBaseProcess.DoSyncVisible(&rPlayer, m_rcCharacter.GetId(), bVisible);
		}
		g_cPlayerBaseProcess.DoSyncLeave(m_rcCharacter);
	}
	// 隐身 --> 显身
	if (bVisible && ! m_rcCharacter.m_bVisible)
	{
		IKScene* pScene = m_rcCharacter.GetScene();
		QCONFIRM(pScene);
		if (pScene)
		{
			TKTraverseHelper<KPlayer> cTravHelper;
			pScene->TraverseNearbyPlayers(cTravHelper, &m_rcCharacter);
			if (IS_PLAYER(m_rcCharacter.GetId()))
			{
				KPlayer& rPlayer = static_cast<KPlayer&>(m_rcCharacter);
				g_cPlayerBaseProcess.DoSyncVisible(&rPlayer, m_rcCharacter.GetId(), bVisible);
				rPlayer.m_cPlayerServer.SyncToNeighborPlayers(cTravHelper.m_vObjList);
			}
			else if (IS_NPC(m_rcCharacter.GetId()))
			{
				KNpc& rNpc = static_cast<KNpc&>(m_rcCharacter);
				rNpc.m_cNpcServer.SyncToNeighborPlayers(cTravHelper.m_vObjList);
			}
			else
			{
				Q_Error("Unexpected SObj [" << m_rcCharacter.GetId() << "][" << m_rcCharacter.GetName() << "]");
			}
		}
	}
	m_rcCharacter.SetVisible(bVisible);
}

// 设置是否隐身，1：隐身
VOID KCharacterServer::SetSkillHidden( BOOL bHidden )
{
	IKScene* pScene = m_rcCharacter.GetScene();
	QCONFIRM(pScene);
	BOOL bHiddenFlag = (!bHidden && m_rcCharacter.m_bSkillHidden);

	m_rcCharacter.SetSkillHidden(bHidden);

	TKTraverseHelper<KPlayer> cTravHelper;
	pScene->TraverseNearbyPlayers(cTravHelper, &m_rcCharacter);

	for (std::vector<KPlayer*>::iterator it = cTravHelper.m_vObjList.begin(); it != cTravHelper.m_vObjList.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		QCONFIRM_CONTINUE(pPlayer);

		// 反隐
		if(m_rcCharacter.GetIsVisiontoPlayer())
			continue;

		if(g_GetRelation(pPlayer, &m_rcCharacter) == sortEnemy && bHidden)
			g_cPlayerBaseProcess.DoSyncLeave(pPlayer, m_rcCharacter.GetId());
		else
			g_cPlayerBaseProcess.DoSyncVisible(pPlayer, m_rcCharacter.GetId(), !bHidden);
	}

	if (bHiddenFlag)
	{
		IKScene* pScene = m_rcCharacter.GetScene();
		if (pScene)
		{
			TKTraverseHelper<KPlayer> cTravHelper;
			pScene->TraverseNearbyPlayers(cTravHelper, &m_rcCharacter);
			if (IS_PLAYER(m_rcCharacter.GetId()))
			{
				KPlayer& rPlayer = static_cast<KPlayer&>(m_rcCharacter);
				rPlayer.m_cPlayerServer.SyncToNeighborPlayers(cTravHelper.m_vObjList);
			}
		}
	}
}

// 同步做动作给周围玩家
BOOL KCharacterServer::SyncActionAround(INT nActionId, INT nPendantId)
{
	KS2C_SYNC_ACTION_AROUND sSync;

	sSync.dwCharacterId = m_rcCharacter.GetId();
	sSync.byActionId = (BYTE)nActionId;
	sSync.nPendantId = nPendantId;

	return BroadcastNearby(&sSync, sizeof(sSync), FALSE);
}

// -------------------------------------------------------------------------
#endif // #ifdef GAME_SERVER
