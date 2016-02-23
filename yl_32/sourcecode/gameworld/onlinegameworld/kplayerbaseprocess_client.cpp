
#ifdef GAME_CLIENT
#include "stdafx.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "kplayerbaseprocess_client.h"
#include "onlinegameworld/kplayermanager_client.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/knpcmgr.h"
#include "onlinegameworld/kcharactermgr.h"
#include "onlinegameworld/kdoodad.h"
#include "onlinegameworld/kdoodadmgr.h"
#include "ksocketinterface.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/findpath/kplayerfindpath_i.h"
#include "knetconnectplayer_client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KPlayerBaseProcess g_cPlayerBaseProcess;
// -------------------------------------------------------------------------
KPlayerBaseProcess::KPlayerBaseProcess()
{
	m_aProcessFunc[emPLAYERBASE_S2C_NONE] = NULL;
	m_aProcessFunc[emPLAYERBASE_S2C_PLAYERDATA] = &KPlayerBaseProcess::PtcPlayerData;

	m_aProcessFunc[emPLAYERBASE_S2C_PING] = &KPlayerBaseProcess::OnPing;
	m_aProcessFunc[emKPBP_S2C_FRAME_SIGNAL] = &KPlayerBaseProcess::OnSyncFrameSignal;
	m_aProcessFunc[emPLAYERBASE_S2C_CHANGE_SERVER] = &KPlayerBaseProcess::OnChangeServer;

	m_aProcessFunc[emKPBP_S2C_SYNC_SELF] = &KPlayerBaseProcess::OnSyncSelf;
	m_aProcessFunc[emKPBP_S2C_SYNC_LEAVE] = &KPlayerBaseProcess::OnLeaveScene;
	m_aProcessFunc[emKPBP_S2C_SYNC_JUMP] = &KPlayerBaseProcess::OnSyncJump;
	m_aProcessFunc[emKPBP_S2C_SYNC_JUMP_TO] = &KPlayerBaseProcess::OnSyncJumpTo;
	m_aProcessFunc[emKPBP_S2C_SYNC_MOVE] = &KPlayerBaseProcess::OnSyncMove;
	m_aProcessFunc[emKPBP_S2C_SYNC_FACE_DIRECTION] = &KPlayerBaseProcess::OnSyncFaceDirection;
	m_aProcessFunc[emKPBP_S2C_SYNC_PLAYER] = &KPlayerBaseProcess::OnSyncPlayer;
	m_aProcessFunc[emKPBP_S2C_SYNC_NPC] = &KPlayerBaseProcess::OnSyncNpc;
	m_aProcessFunc[emKPBP_S2C_SYNC_DOODAD] = &KPlayerBaseProcess::OnSyncDoodad;
	m_aProcessFunc[emKPBP_S2C_SYNC_ADJUST_MOVE] = &KPlayerBaseProcess::OnSyncAdjustMove;
	m_aProcessFunc[emKPBP_S2C_SYNC_TELEPORT] = &KPlayerBaseProcess::OnSyncTeleport;
	m_aProcessFunc[emKPBP_S2C_SYNC_VAR_SELF] = &KPlayerBaseProcess::OnSyncVarSelf;
	m_aProcessFunc[emKPBP_S2C_SYNC_VAR_AROUND] = &KPlayerBaseProcess::OnSyncVarAround;
	m_aProcessFunc[emKPBP_S2C_RESPONSE_OFFLINE] = &KPlayerBaseProcess::OnResponseOffline;
	m_aProcessFunc[emKPLBASE_PTC_S2C_CALL_CLIENT_SCRIPT] = &KPlayerBaseProcess::OnCallClientCall;
	m_aProcessFunc[emKPBP_S2C_SYNC_DOODAD_STATE] = &KPlayerBaseProcess::OnSyncDoodadState;
	m_aProcessFunc[emKPBP_S2C_SYNC_DOODAD_EXTRA] = &KPlayerBaseProcess::OnSyncDoodadExtra;

	m_aProcessFunc[emKPBP_S2C_SYNC_VISIBLE] = &KPlayerBaseProcess::OnSyncVisible;
}

BOOL KPlayerBaseProcess::ProcessPacket( LPCBYTE pbData, UINT uDataLen )
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pbData;
	QCONFIRM_RET_FALSE(pbData && *pbData < emKPLBASE_PTC_S2C_MAX);
	if (!m_aProcessFunc[pHeader->byProtocol])
		return FALSE;
	return (this->*m_aProcessFunc[pHeader->byProtocol])(pbData, uDataLen);
}

BOOL KPlayerBaseProcess::PtcPlayerData( LPCBYTE pData, UINT uDataLen )
{
	KS2C_PLAYERDATA* pSData = (KS2C_PLAYERDATA* )pData;
	// CheckSize
	QCONFIRM_RET_FALSE(_offsetof(KS2C_PLAYERDATA, sVarSync.aMapVarSet) + sizeof(KMAP_VAR_VALUE) *
		(pSData->sVarSync.wMapVarNum) == uDataLen);

	Q_Printl("Received Player Data. Client Player Rolename: " << pSData->szPlayerName <<
		" Id:" << pSData->dwPlayerId);

	g_cOrpgWorld.m_dwClientPlayerId = pSData->dwPlayerId;
	g_cOrpgWorld.m_dwLogicFrames = pSData->dwLogicFrames;
	g_cOrpgWorld.m_nServerId = pSData->nServerId;

    InitStartTimeEvaluation(&g_cOrpgWorld.m_StartTimeEvaluation);
    g_cOrpgWorld.m_nStartLoop = g_cOrpgWorld.m_dwLogicFrames;
    g_cOrpgWorld.m_nGSGameLoop = g_cOrpgWorld.m_dwLogicFrames;
    g_cOrpgWorld.m_dwStartTime = timeGetTime();
	g_cOrpgWorld.m_dwServerTime = pSData->dwServerTime;

	KPlayer* pPlayer = g_cOrpgWorld.AddPlayer(pSData->szPlayerName, pSData->dwPlayerId);
	QCONFIRM_RET_FALSE(pPlayer);
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());

	pPlayer->SetKinId(0);
	pPlayer->SetKinName("");
	pPlayer->SetKinFigure(0);
	pPlayer->m_cPlayerClient.LoadSyncVarSelf(pSData->sVarSync);

	g_cOrpgWorld.m_cEventCreator.FireEvent(emKOBJEVENTTYPE_PLAYER_LOGIN, g_cOrpgWorld.m_dwClientPlayerId, 0, 0);		// by houxuan, 玩家登陆事件

	// 通知服务端已完成加载
	DoSyncEnd();

	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncSelf( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_SELF* pSData = (KS2C_SYNC_SELF* )pData;

	g_cOrpgWorld.m_dwClientSceneId = pSData->dwSceneId;

	KPlayer* pPlayer = g_cPlayerMgr.GetById(g_cOrpgWorld.m_dwClientPlayerId);
	QCONFIRM_RET_FALSE(pPlayer);
	
	// 为了加载地图时，加快3D地图的加载所作的优化
	pPlayer->m_nX = pSData->nX;
	pPlayer->m_nY = pSData->nY;
	pPlayer->m_nZ = pSData->nZ;
	pPlayer->m_nFaceDirection = pSData->nDirectionXY;

	IKScene* pScene = g_cOrpgWorld.LoadScene(pSData->dwSceneId);
	QCONFIRM_RET_FALSE(pScene);

	//modify by wdb
	const KSCENE_CONFIG* pSceneCfg = g_pSceneMgr->GetSceneInfo(g_cOrpgWorld.m_dwClientSceneId);
	QCONFIRM_RET_FALSE(pSceneCfg);

	CHAR szNavPath[MAX_PATH] = {0};	
	snprintf(szNavPath,
			 countof(szNavPath) - 1,
			 "%s/%s%s",
			 pSceneCfg->szPath,
			 pSceneCfg->szClass,
			 KD_SCENE_NAVFILE_EXT);

	IKPlayerFindPath* pIKPlayerFindPath = GetPlayerFindPath(pPlayer);
	QCONFIRM_RET_FALSE(pIKPlayerFindPath);
	BOOL bLoadRet = pIKPlayerFindPath->LoadNavPath(szNavPath);
	if (!bLoadRet)
	{
		QLogPrintf(LOG_INFO, "Load Scene Name:%s Navigate Path File %s Failed!\n", pSceneCfg->szClass, szNavPath);
	}

	pScene->AddPlayer(pPlayer, pSData->nX, pSData->nY, pSData->nZ);
	pPlayer->m_cPlayerClient.DynamicSceneFlag(pSData->nDynamicScene);
	pPlayer->m_nDirectionXY = pSData->nDirectionXY;
	pPlayer->m_nFaceDirection = pSData->nDirectionXY;
	pPlayer->m_sMoveCtrl.nMoveCount = 0;

	pPlayer->FireEvent(emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, pPlayer->GetId(), FALSE, 0);

	return TRUE;
}

BOOL KPlayerBaseProcess::OnPing(LPCBYTE pData, UINT uDataLen)
{
	KS2C_PING* pReceived = (KS2C_PING* )pData;
	DWORD dwDiffTime = timeGetTime() - pReceived->dwClientTime;

	g_cOrpgWorld.m_nPing = INT(dwDiffTime / 2); // 来回平均时间

	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncFrameSignal( LPCBYTE pData, UINT uDataLen )
{
	KS2C_FRAME_SIGNAL* pReceived = (KS2C_FRAME_SIGNAL* )pData;
    DWORD dwFrame = pReceived->dwLogicFrames;

	// houxuan, 修改下估算客户端启动时间的算法
    //UpdateStartTimeEvaluation(&g_cOrpgWorld.m_StartTimeEvaluation, dwFrame);
	UpdateStartTimeEvaluation2(&g_cOrpgWorld.m_StartTimeEvaluation, dwFrame);

	g_cOrpgWorld.m_nGSGameLoop = dwFrame;

	return TRUE;
}

BOOL KPlayerBaseProcess::OnChangeServer( LPCBYTE pData, UINT uDataLen )
{
	KS2C_CHANGE_SERVER* pReceived = (KS2C_CHANGE_SERVER* )pData;
	if (pReceived->nCode) // TODO: 临时方案
		return TRUE;

	printf("\n跨服: %s:%d\n", inet_ntoa(*(in_addr*)&pReceived->uServerIp), pReceived->wPort);

	extern IKSocketClientInterface* g_piSocketClient;
	g_piSocketClient->ShutDown();

	g_cOrpgWorld.UnInit();

	// TODO:是否隐身
	if (!g_piSocketClient->Connect(pReceived->uServerIp, pReceived->wPort, &pReceived->guid, FALSE))
	{
		g_cNetConnectPlayer.ProcessEvent(emKSOCKET_EVENT_LOST, 0, 0);
	}

	g_cOrpgWorld.Init();

	return TRUE;
}

BOOL KPlayerBaseProcess::OnLeaveScene( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_LEAVE* pReceived = (KS2C_SYNC_LEAVE*)pData;

	// get client scene
	IKScene* pScene = g_cOrpgWorld.GetScene(g_cOrpgWorld.m_dwClientSceneId);
	QCONFIRM_RET_FALSE(pScene);

	if (IS_NPC(pReceived->dwId))
	{
		KNpc* pNpc = g_cNpcMgr.GetById(pReceived->dwId);
		QCONFIRM_RET_FALSE(pNpc);
		g_cOrpgWorld.RemoveNpc(pNpc->m_dwId);
	}
	else if (IS_PLAYER(pReceived->dwId))
	{
		KPlayer* pPlayer = g_cPlayerMgr.GetById(pReceived->dwId);
		QCONFIRM_RET_FALSE(pPlayer);

		if (pReceived->dwId == g_cOrpgWorld.m_dwClientPlayerId)
		{
			pScene->RemovePlayer(pPlayer);
			// 是客户端玩家自己，卸载场景
			g_cOrpgWorld.UnloadScene(g_cOrpgWorld.m_dwClientSceneId);
		}
		else
		{
			g_cOrpgWorld.RemovePlayer(pPlayer->m_dwId);
		}
	}
	else if (IS_DOODAD(pReceived->dwId))
	{
		KDoodad* pDoodad = g_cDoodadMgr.GetById(pReceived->dwId);
		QCONFIRM_RET_FALSE(pDoodad);
		//g_cOrpgWorld.RemoveDoodad(pDoodad->m_dwId);
		g_cDoodadMgr.RemoveFromScene(pReceived->dwId);
		g_cDoodadMgr.DestroyDoodad(pReceived->dwId);
		//g_cOrpgWorld.RemoveFromScene(pDoodad->m_dwId);
	}
	else
	{
		Q_Error("Unknown SceneObject " << pReceived->dwId);
	}

	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncJump( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_JUMP* pReceived = (KS2C_SYNC_JUMP*)pData;

	KPlayer* pPlayer = g_cPlayerMgr.GetById(pReceived->dwId);
	QCONFIRM_RET_FALSE(pPlayer);

	pPlayer->Jump();

	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncJumpTo( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_JUMP_TO* pReceived = (KS2C_SYNC_JUMP_TO*)pData;

	KPlayer* pPlayer = g_cPlayerMgr.GetById(pReceived->dwId);
	QCONFIRM_RET_FALSE(pPlayer);

	pPlayer->SetPosition(pReceived->nX, pReceived->nY, pReceived->nZ);
	pPlayer->JumpTo(pReceived->nDestX, pReceived->nDestY, pReceived->nDestZ);

	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncMove( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_MOVE* pReceived = (KS2C_SYNC_MOVE*)pData;
	KCharacter* pSceneObj = NULL;
	KCharacter* pFollowTarget = NULL;
	INT nX = 0, nY = 0, nZ = 0;

	if (IS_PLAYER(pReceived->dwId) || IS_NPC(pReceived->dwId))
		pSceneObj = KCharacterMgr::GetById(pReceived->dwId);
	else
		QLogPrintf(LOG_LEVEL_WARNING, "OnSyncMove unknown 0x%x", pReceived->dwId);
	PROCESS_ERROR(pSceneObj);

	if (pSceneObj->GetFollowTarget() != KD_BAD_ID)
		pFollowTarget = KCharacterMgr::GetById(pSceneObj->GetFollowTarget());

	 // 在开始移动的时候同步位置
	if (!pFollowTarget && cmsOnStand == pSceneObj->GetMoveState())
	{
		pSceneObj->SetPosition(pReceived->nX, pReceived->nY, pReceived->nZ);
	}
	else
	{
		pSceneObj->GetPosition(nX, nY, nZ);
		if (g_GetDistance2(nX, nY, pReceived->nX, pReceived->nY) > KD_FOLLOW_DISTANCE2_MAX)
		{
			pSceneObj->SetPosition(pReceived->nX, pReceived->nY, pReceived->nZ);
		}
	}

	pSceneObj->SetMoveMode(pReceived->nMoveMode);
	pSceneObj->GoTo(pReceived->nDestX, pReceived->nDestY, pReceived->nDestZ);
	pSceneObj->SetMoveState((KE_MOVE_STATE)pReceived->nMoveState);

EXIT0:
	if (pReceived && !pSceneObj)
	{
		QLogPrintf(LOG_LEVEL_WARNING, "OnSyncMove [0x%x] not found, re-syncing...", pReceived->dwId);
		//DoApplySyncNeighbor(pReceived->dwId);
		g_cOrpgWorld.AddApplySyncNeighbor(pReceived->dwId);
	}

	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncFaceDirection( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_FACE_DIRECTION* pReceived = (KS2C_SYNC_FACE_DIRECTION*)pData;
	KSceneObject* pSceneObj = NULL;

	if (IS_PLAYER(pReceived->dwId))
	{
		pSceneObj = g_cPlayerMgr.GetById(pReceived->dwId);
	}
	else if (IS_NPC(pReceived->dwId))
	{
		pSceneObj = g_cNpcMgr.GetById(pReceived->dwId);
	}
	else if (IS_DOODAD(pReceived->dwId))
	{
		pSceneObj = g_cDoodadMgr.GetById(pReceived->dwId);
	}
	else
	{
		Q_Printl("Unknown SceneObject " << pReceived->dwId);
	}
	//QCONFIRM_RET_FALSE(pSceneObj);
	if (!pSceneObj)
	{
		QLogPrintf(LOG_LEVEL_WARNING, "OnSyncFaceDirection [0x%x] not found, re-syncing...", pReceived->dwId);
		g_cOrpgWorld.AddApplySyncNeighbor(pReceived->dwId);
		return TRUE;
	}

	if (pSceneObj->GetMoveState() == cmsOnStand)
	{
		pSceneObj->m_nFaceDirection = pReceived->nFaceDirection;
	} 
	else
	{
		pSceneObj->m_nStopedFaceDirection = pReceived->nFaceDirection;
	}
	
	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncTeleport( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_TELEPORT* pReceived = (KS2C_SYNC_TELEPORT*)pData;
	KSceneObject* pSceneObj = NULL;

	if (IS_PLAYER(pReceived->dwId))
	{
		pSceneObj = g_cPlayerMgr.GetById(pReceived->dwId);
	}
	else if (IS_NPC(pReceived->dwId))
	{
		pSceneObj = g_cNpcMgr.GetById(pReceived->dwId);
	}
	else if (IS_DOODAD(pReceived->dwId))
	{
		pSceneObj = g_cDoodadMgr.GetById(pReceived->dwId);
	}
	else
	{
		Q_Printl("Unknown SceneObject " << pReceived->dwId);
	}
	QCONFIRM_RET_FALSE(pSceneObj);

	pSceneObj->SetPosition(pReceived->nX, pReceived->nY, pReceived->nZ);
	pSceneObj->StopMoving();

	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncPlayer( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_PLAYER* pReceived = (KS2C_SYNC_PLAYER*)pData;
	// CheckSize
	QCONFIRM_RET_FALSE(_offsetof(KS2C_SYNC_PLAYER, sVarSync.aMapVarSet) + sizeof(KMAP_VAR_VALUE) *
		(pReceived->sVarSync.wMapVarNum) == uDataLen);

	// TODO: improve
	KPlayer* pPlayer = g_cPlayerMgr.GetById(pReceived->dwPlayerId);
	if (pPlayer)
	{
		pPlayer->m_cPlayerClient.LoadSyncVarOther(pReceived->sVarSync);
		pPlayer->SetPosition(pReceived->nX, pReceived->nY, pReceived->nZ);
		pPlayer->SetMoveState((KE_MOVE_STATE)pReceived->nMoveState);
		pPlayer->m_nVelocityXY = pReceived->nVelocityXY;
		pPlayer->m_nDestX = pReceived->nDestX;
		pPlayer->m_nDestY = pReceived->nDestY;
		pPlayer->m_nDestZ = pReceived->nDestZ;
		pPlayer->m_nFaceDirection = pReceived->nDirectionXY; // 暂时用速度方向代替
		pPlayer->SetKinName(pReceived->szKinName);
		pPlayer->SetKinId(pReceived->dwKinId);
		pPlayer->SetKinFigure(pReceived->dwKinFigure);

		QLogPrintf(LOG_LEVEL_WARNING, "[PlayerBaseProcess] %s :player_id=%d,already exists", __FUNCTION__, pReceived->dwPlayerId);
		return FALSE;
	}
	else
	{
		pPlayer = g_cOrpgWorld.AddPlayer(pReceived->szName, pReceived->dwPlayerId);
		QCONFIRM_RET_FALSE(pPlayer);

		pPlayer->m_cPlayerClient.LoadSyncVarOther(pReceived->sVarSync);

		// get client scene
		IKScene* pScene = g_pSceneMgr->FirstScene();
		QCONFIRM_RET_FALSE(pScene);

		pScene->AddPlayer(pPlayer, pReceived->nX, pReceived->nY, pReceived->nZ);
		pPlayer->SetMoveState((KE_MOVE_STATE)pReceived->nMoveState);
		pPlayer->m_nVelocityXY = pReceived->nVelocityXY;
		pPlayer->m_nDestX = pReceived->nDestX;
		pPlayer->m_nDestY = pReceived->nDestY;
		pPlayer->m_nDestZ = pReceived->nDestZ;
		pPlayer->m_nFaceDirection = pReceived->nDirectionXY; // 暂时用速度方向代替
		pPlayer->SetKinName(pReceived->szKinName);
		pPlayer->SetKinId(pReceived->dwKinId);
		pPlayer->SetKinFigure(pReceived->dwKinFigure);
		pPlayer->FireEvent(emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, pPlayer->GetId(), FALSE, 0);
		return TRUE;
	}

	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncNpc( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_NPC* pReceived = (KS2C_SYNC_NPC*)pData;
	// CheckSize
	QCONFIRM_RET_FALSE(_offsetof(KS2C_SYNC_NPC, sVarSync.aMapVarSet) + sizeof(KMAP_VAR_VALUE) *
		(pReceived->sVarSync.wMapVarNum) == uDataLen);

	KNpc* pOldNpc = g_cNpcMgr.GetById(pReceived->dwId); // tmp fix

	KNpc* pNpc = g_cOrpgWorld.AddNpc(pReceived->dwTemplateId, pReceived->dwId);
	QCONFIRM_RET_FALSE(pNpc);

	pNpc->m_cNpcClient.LoadSyncVarOther(pReceived->sVarSync);

	// get client scene
	IKScene* pScene = g_cOrpgWorld.GetScene(g_cOrpgWorld.m_dwClientSceneId);
	QCONFIRM_RET_FALSE(pScene);

	pScene->AddNpc(pNpc, pReceived->nX, pReceived->nY, pReceived->nZ);
	pNpc->SetMoveState((KE_MOVE_STATE)pReceived->nMoveState);
	pNpc->m_nVelocityXY = pReceived->nVelocityXY;
	pNpc->m_nDestX = pReceived->nDestX;
	pNpc->m_nDestY = pReceived->nDestY;
	pNpc->m_nDestZ = pReceived->nDestZ;
	pNpc->m_nFaceDirection = pReceived->nDirectionXY; // 暂时用速度方向代替

	if (!pOldNpc)
		pNpc->FireEvent(emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, pNpc->GetId(), TRUE, 0);
	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncNpcBorn(LPCBYTE pData, UINT uDataLen)
{
	KS2C_SYNC_NPC_BORN* pBornData = (KS2C_SYNC_NPC_BORN*)pData;
	KNpc* pNpc = g_cNpcMgr.GetById(pBornData->dwId);
	QCONFIRM_RET_FALSE(pNpc);
	pNpc->FireEvent(emKOBJEVENTTYPE_CHARACTER_BORN, pBornData->dwId, 0, 0);
	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncDoodad( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_DOODAD* pReceived = (KS2C_SYNC_DOODAD*)pData;
	KDoodad* pDoodad = NULL;

	// TODO: improve
	if (g_cDoodadMgr.GetById(pReceived->dwId))
		return FALSE;	

	//pDoodad = g_cOrpgWorld.AddDoodadToScene(pReceived->dwTemplateId, g_cOrpgWorld.m_dwClientSceneId, pReceived->nX, pReceived->nY, pReceived->nZ, pReceived->nDirectionXY, pReceived->dwId);
	pDoodad = g_cDoodadMgr.CreateDoodad(pReceived->dwTemplateId, pReceived->dwId);
	QCONFIRM_RET_FALSE(pDoodad);

	pDoodad->SetMoveState((KE_MOVE_STATE)pReceived->nMoveState);
	pDoodad->m_nVelocityXY = pReceived->nVelocityXY;
	pDoodad->m_nDestX = pReceived->nDestX;
	pDoodad->m_nDestY = pReceived->nDestY;
	pDoodad->m_nDestZ = pReceived->nDestZ;
	pDoodad->m_nFaceDirection = pReceived->nDirectionXY; // 暂时用速度方向代替
	pDoodad->SetState((KEDOODAD_STATE)pReceived->nDoodadState);
	g_cDoodadMgr.AddToScene(pDoodad, g_cOrpgWorld.m_dwClientSceneId, pReceived->nX, pReceived->nY, pReceived->nZ, pReceived->nDirectionXY);
	pDoodad->FireEvent(emKOBJEVENTTYPE_DOODAD_STATE_CHANGED, pReceived->dwPlayerId, pDoodad->GetId(), pReceived->nDoodadState);
	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncDoodadState( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_DOODAD_STATE* pReceived = (KS2C_SYNC_DOODAD_STATE*)pData;

	KDoodad* pDoodad = g_cDoodadMgr.GetById(pReceived->dwId);
	QCONFIRM_RET_FALSE(pDoodad);

	if (pReceived->nDoodadState != (INT)pDoodad->GetState())
	{
		pDoodad->SetState((KEDOODAD_STATE)pReceived->nDoodadState);

		pDoodad->FireEvent(emKOBJEVENTTYPE_DOODAD_STATE_CHANGED,
			pReceived->dwPlayerId,
			pDoodad->GetId(),
			pReceived->nDoodadState);

		// 因为要传递四个参数,现有机制不支持,使用CallFunction来告之脚本这个事情
		//pReceived->dwPlayerId
		//CallTableFunction();
	}
	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncDoodadExtra( LPCBYTE pData, UINT uDataLen )
{
	BOOL bResult = FALSE;
	KDoodad* pDoodad = NULL;
	KS2C_SYNC_DOODAD_EXTRA* pReceived = NULL;
	BOOL bBegin = FALSE;
	DWORD* pdwExtraHeader = NULL;
	BYTE* pstExtraData = NULL;
	
	PROCESS_ERROR(pReceived = (KS2C_SYNC_DOODAD_EXTRA*)pData);
	PROCESS_ERROR(pDoodad = g_cDoodadMgr.GetById(pReceived->dwDoodadId));

	PROCESS_ERROR(pDoodad->BeginSetExtra());
	bBegin = TRUE;

	//PROCESS_ERROR(pDoodad->ClearExtra());
	pdwExtraHeader = (DWORD*)pReceived->acData;
	pstExtraData = (BYTE*)(pReceived->acData + pReceived->dwExtraCount * sizeof(DWORD));
	for (DWORD dwI = 0; dwI < pReceived->dwExtraCount; dwI++)
	{
		KDoodadExtraField* pstExtraField = NULL;

		pstExtraField = (KDoodadExtraField*)pstExtraData;
		if (0 == pstExtraField->dwType)
		{
			PROCESS_ERROR(pDoodad->SetExtraNumber(pstExtraField->dwId, pstExtraField->adwData[0], FALSE));
		}
		else if (1 == pstExtraField->dwType)
		{
			PROCESS_ERROR(pDoodad->SetExtraString(pstExtraField->dwId, (CONST CHAR*)pstExtraField->adwData, FALSE));
		}

		pstExtraData += pdwExtraHeader[dwI];
	}

	PROCESS_ERROR(pDoodad->EndSetExtra(FALSE));
	bBegin = FALSE;
	/*事件*/

	bResult = TRUE;
EXIT0:
	if (bBegin)
	{
		if (pDoodad)
		{
			pDoodad->EndSetExtra(FALSE);
		}
	}
	return bResult;
}

BOOL KPlayerBaseProcess::OnSyncAdjustMove( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_ADJUST_MOVE* pReceived = (KS2C_SYNC_ADJUST_MOVE*)pData;

	//cprintf("Adjust: %d\n", pPak->nMoveCount);

	KPlayer* pPlayer = g_cPlayerMgr.GetById(g_cOrpgWorld.m_dwClientPlayerId);
	QCONFIRM_RET_FALSE(pPlayer);

	//if (pReceived->nMoveCount < pPlayer->m_sMoveCtrl.nMoveCount || pPlayer->m_pScene == NULL)
	//{
	//	QLogPrintf(
	//	    LOG_DEBUG, "[move] apply adjust(%d < %d)", 
	//	    pReceived->nMoveCount, pPlayer->m_sMoveCtrl.nMoveCount
	//	);

	//	DoApplyMoveAdjust();
	//	goto EXIT0;
	//}

	INT nX = 0, nY = 0, nZ = 0;
	pPlayer->GetPosition(nX, nY, nZ);

	{
		printf("[MoveAdjust] S[%d-%d],c[%d,%d,%d]s[%d,%d,%d],F[%d-%d] Diff[%d]\n",
			pPlayer->m_eMoveState,
			pReceived->nMoveState,
			nX, nY, nZ, 
			pReceived->nX, pReceived->nY, pReceived->nZ, 
			g_cOrpgWorld.m_dwLogicFrames, pReceived->dwVirtualFrame,
			g_cOrpgWorld.m_dwLogicFrames - pReceived->dwVirtualFrame);
	}

	pPlayer->SetPosition(pReceived->nX, pReceived->nY, pReceived->nZ);
	pPlayer->SetMoveState((KE_MOVE_STATE)pReceived->nMoveState);
	pPlayer->m_nVelocityXY = pReceived->nVelocityXY;
	pPlayer->m_nDirectionXY = pReceived->nDirectionXY;
	pPlayer->m_nDestX = pReceived->nDestX;
	pPlayer->m_nDestY = pReceived->nDestY;
	pPlayer->m_nDestZ = pReceived->nDestZ;

	if (pReceived->dwVirtualFrame < g_cOrpgWorld.m_dwLogicFrames)
	{
		pPlayer->ForwardPosition(g_cOrpgWorld.m_dwLogicFrames - pReceived->dwVirtualFrame);
	}

EXIT0:
	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncVarSelf( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_VAR_SELF* pReceived = (KS2C_SYNC_VAR_SELF*)pData;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(g_cOrpgWorld.m_dwClientPlayerId);
	QCONFIRM_RET_FALSE(pPlayer);
	pPlayer->SetVar(pReceived->byVarGroup, pReceived->byVarId, (INT)pReceived->dwVarValue);
	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncVarAround( LPCBYTE pData, UINT uDataLen )
{
	KS2C_SYNC_VAR_AROUND* pReceived = (KS2C_SYNC_VAR_AROUND*)pData;
	KCharacter* pCharacter = g_cCharacterMgr.GetById(pReceived->dwId);
	if (!pCharacter)
		return FALSE;
	pCharacter->SetVar(pReceived->byVarGroup, pReceived->byVarId, (INT)pReceived->dwVarValue);
	return TRUE;
}
BOOL KPlayerBaseProcess::OnResponseOffline(LPCBYTE pData, UINT uDataLen)
{
	extern IKSocketClientInterface* g_piSocketClient;
	g_cOrpgWorld.m_cEventCreator.FireEvent(emKOBJEVENTTYPE_PLAYER_LOGOUT, g_cOrpgWorld.m_dwClientPlayerId, 0, 0);
	return g_piSocketClient->ShutDown();
}

BOOL KPlayerBaseProcess::DoSyncEnd()
{
	KC2SSYNC_END sSync;
	//sSync.dwHwId = 0; // TODO: anti-hack
	g_pBaseSender->SendData(sSync);
	return TRUE;
}

BOOL KPlayerBaseProcess::DoPing()
{
	KC2S_PING sPack;
	sPack.dwClientTime = timeGetTime();
	g_pBaseSender->SendData(sPack);
	return TRUE;
}

BOOL KPlayerBaseProcess::DoApplySyncNeighbor(DWORD dwId)
{
	KC2S_APPLY_SYNC_NEIGHBOR sPack;
	sPack.dwId = dwId;
	g_pBaseSender->SendData(sPack);
	return TRUE;
}

BOOL KPlayerBaseProcess::DoApplySetVar(DWORD dwGroupId, DWORD dwVarId, DWORD dwValue)
{
	KC2S_APPLY_SET_VAR sPack;
	sPack.wVarGroupId = dwGroupId;
	sPack.wVarId = dwVarId;
	sPack.dwValue = dwValue;
	g_pBaseSender->SendData(sPack);
	return TRUE;
}

BOOL KPlayerBaseProcess::OnCallClientCall(LPCBYTE pData, UINT uDataLen)
{
	KS2C_SCRIPT_REMOTECALL* pMsg = (KS2C_SCRIPT_REMOTECALL*)pData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	if (cSafeScript->LoadBuffer2Value(pMsg->arBuffer, uDataLen - sizeof(KS2C_SCRIPT_REMOTECALL)))
	{
		cSafeScript->CallTableFunction("ClientEvent", "OnServerCall",
			0, "v", cSafeScript.GetBeginTopIndex() + 1);
	}
	return TRUE;
}

BOOL KPlayerBaseProcess::OnSyncVisible(LPCBYTE pData, UINT uDataLen)
{
	KS2C_SYNC_VISIBLE* pMsg = (KS2C_SYNC_VISIBLE*)pData;

	KCharacter* pCharacter = g_cCharacterMgr.GetById(pMsg->dwId);
	QCONFIRM_RET_FALSE(pCharacter);
	pCharacter->SetVisible(pMsg->byVisible);

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	cSafeScript->CallTableFunction("Represent", "SetCharacterSemiVisible", 0, "uu", pMsg->dwId, (DWORD)pMsg->byVisible);
	return TRUE;
}

#endif  // GAME_CLIENT
// -------------------------------------------------------------------------
