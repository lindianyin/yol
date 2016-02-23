/* -------------------------------------------------------------------------
//	文件名		：	synchelper.cpp
//	创建者		：	simon
//	创建时间	：	2009/12/19 22:16:07
//	功能描述	：	同步功能集
//
// -----------------------------------------------------------------------*/
#ifdef GAME_SERVER

#include "stdafx.h"
#include "onlinegameworld/synchelper.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kdoodad.h"
#include "onlinegameworld/kplayerserver.h"
#include "kplayerbaseprocess.h"
#include "onlinegameworld/scene_def.h"
#include "onlinegameworld/korpgworld.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
BYTE g_abySyncPackBuffer[KD_SYNC_PACK_MAX] = {0}; // 全局共享的同步包发送缓存


////////////////////////////////////////////Synchronization Helper Function Objects//////////////////////////////
KBroadcastFunc::KBroadcastFunc()
{
	m_pvData = NULL;
	m_uSize = 0;
	m_nMaxSyncCount = KD_NEARBY_OBJ_SYNC_MAX;
	m_pExcept = NULL;
	m_pCenterObject = NULL;
	m_nRange = 0;
	m_pProcessor = g_pBaseSender; // 默认为base协议发送器
}

BOOL KBroadcastFunc::OnTraverse(KSceneObject* pObj)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	KPlayer* pPlayer = (KPlayer*)pObj;

	PROCESS_SUCCESS(m_pCenterObject && m_pCenterObject != pObj
		&& !m_pCenterObject->IsVisible());

	LOG_PROCESS_ERROR(m_pProcessor);
	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR( IS_PLAYER(pPlayer->m_dwId) );

	PROCESS_SUCCESS(pPlayer == m_pExcept);

	if (m_pCenterObject && IS_DOODAD(m_pCenterObject->GetId()))
	{
		KDoodad* pDoodad = (KDoodad*)m_pCenterObject;
		PROCESS_SUCCESS(pDoodad->m_dwOwnerId != KD_BAD_ID && !pDoodad->IsPlayerVisible(pPlayer));
	}

	if (m_nRange > 0 && m_pCenterObject && m_pCenterObject->GetScene())
	{
		bRetCode = m_pCenterObject->IsNearby(pPlayer, m_nRange * REGION_GRID_WIDTH * LOGICAL_CELL_CM_LENGTH );
		//QLogPrintf(LOG_INFO,"..m_nRange=%d,bRetCode=%d",m_nRange,bRetCode);
		PROCESS_SUCCESS(!bRetCode);
	}

	// 如果玩家和centerobject不在同一个场景则不用同步
	PROCESS_SUCCESS(m_pCenterObject && pPlayer->GetScene() != m_pCenterObject->GetScene());

	LOG_PROCESS_ERROR(m_nMaxSyncCount-- > 0);

	//if (m_pCenterObject)
	//	printf("[0x%X] -> [%d],", m_pCenterObject->m_dwId, pPlayer->m_dwId);
	//else
	//	printf(" -> [%d],", pPlayer->m_dwId);
	
	PROCESS_ERROR(pPlayer->GetPlayerIndex() > 0);//可能被释放了
	bResult = m_pProcessor->SendUnknownData(pPlayer->GetPlayerIndex(), (LPSTR)m_pvData, m_uSize);
	if (!bResult)
	{
		QLogPrintf(LOG_ERR,
			"KBroadcastFunc SendVarLenData Failed! Receiver [%u][%d][%s]", 
			pPlayer->GetId(), pPlayer->GetPlayerIndex(), pPlayer->GetName());
	}

EXIT1:
	bResult = true;
EXIT0:
	return bResult;
}

KBroadcastPlayerFunc::KBroadcastPlayerFunc()
{
	m_pvData = NULL;
	m_uSize = 0;
	m_nMaxSyncCount = KD_NEARBY_OBJ_SYNC_MAX;
	m_pExcept = NULL;
	m_pCenterObject = NULL;
	m_nRange = 0;
	m_pProcessor = g_pBaseSender; // 默认为base协议发送器
}


BOOL KBroadcastPlayerFunc::OnTraverse(KSceneObject* pObj)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	KPlayer* pPlayer = (KPlayer*)pObj;
	if(m_pCenterObject && m_pCenterObject != pObj)
	{
		PROCESS_SUCCESS(!m_pCenterObject->IsVisible());
		if(IS_PLAYER(m_pCenterObject->GetId()))
		{
			KCharacter* pCharacter = (KCharacter*)m_pCenterObject;
			PROCESS_SUCCESS(pCharacter->IsSkillHidden(pPlayer));
		}
	}
	//QLogPrintf(LOG_INFO,"..m_nRange=%d",m_nRange);
	LOG_PROCESS_ERROR(m_pProcessor);
	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(IS_PLAYER(pPlayer->m_dwId) );

	PROCESS_SUCCESS(pPlayer == m_pExcept);

	if (m_pCenterObject && IS_DOODAD(m_pCenterObject->GetId()))
	{
		KDoodad* pDoodad = (KDoodad*)m_pCenterObject;
		PROCESS_SUCCESS(pDoodad->m_dwOwnerId != KD_BAD_ID && !pDoodad->IsPlayerVisible(pPlayer));
	}

	if (m_nRange > 0 && m_pCenterObject && m_pCenterObject->GetScene())
	{
		bRetCode = m_pCenterObject->IsNearby(pPlayer, m_nRange * REGION_GRID_WIDTH * LOGICAL_CELL_CM_LENGTH);
		//QLogPrintf(LOG_INFO,"..m_nRange=%d,bRetCode=%d",m_nRange,bRetCode);
		PROCESS_SUCCESS(!bRetCode);
		
	}
	
	// 如果玩家和centerobject不在同一个场景则不用同步
	PROCESS_SUCCESS(m_pCenterObject && pPlayer->GetScene() != m_pCenterObject->GetScene());

	LOG_PROCESS_ERROR(m_nMaxSyncCount-- > 0);

	//if (m_pCenterObject)
	//	printf("[0x%X] -> [%d],", m_pCenterObject->m_dwId, pPlayer->m_dwId);
	//else
	//	printf(" -> [%d],", pPlayer->m_dwId);

	bResult = m_pProcessor->SendUnknownData(pPlayer->GetPlayerIndex(), (LPSTR)m_pvData, m_uSize);
	if ( ! bResult)
	{
		QLogPrintf(LOG_ERR,
			"KBroadcastFunc SendVarLenData Failed! Receiver [%u][%d][%s]", 
			pPlayer->GetId(), pPlayer->GetPlayerIndex(), pPlayer->GetName());
	}

EXIT1:
	bResult = true;
EXIT0:
	return bResult;
}

BOOL KSyncRegionPlayerFunc::OnTraverse(KSceneObject* pObj/*, INT nEnterFrame*/)
{
	BOOL bResult = FALSE;
	KPlayer* pPlayer = (KPlayer*)pObj;

	SIZE_T uSizeUsed = 0;
	SIZE_T uSizeBuffer = sizeof(g_abySyncPackBuffer) - _offsetof(KS2C_SYNC_PLAYER, sVarSync);
	KS2C_SYNC_PLAYER& sData = *(KS2C_SYNC_PLAYER* )g_abySyncPackBuffer;
	sData = KS2C_SYNC_PLAYER();

	PROCESS_SUCCESS(pPlayer == m_pThisPlayer);
	m_vPlayerList.push_back(pPlayer);

	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(m_pThisPlayer);
	if ( ! pPlayer->IsVisible())
	{
		PROCESS_SUCCESS(TRUE);
	}

	if(pPlayer->IsSkillHidden(m_pThisPlayer))
	{
		PROCESS_SUCCESS(TRUE);
	}

	



	
	// fill player basic info
	QStrCpyLen(sData.szName, pPlayer->GetName(), countof(sData.szName));
	sData.dwPlayerId = pPlayer->m_dwId;
	sData.nMoveState = pPlayer->m_eMoveState;
	sData.nVelocityXY = pPlayer->m_nVelocityXY;
	sData.nDirectionXY = pPlayer->m_nDirectionXY;
	sData.dwKinId = pPlayer->m_dwKinId;
	sData.nTeamId = pPlayer->GetTeamId();

	pPlayer->GetPosition(sData.nX, sData.nY, sData.nZ);
	pPlayer->GetDestination(sData.nDestX, sData.nDestY, sData.nDestZ);

	// fill variant sync
	bResult = pPlayer->m_cPlayerServer.FillSyncVar(sData.sVarSync, uSizeBuffer, uSizeUsed);
	LOG_PROCESS_ERROR(bResult && uSizeUsed <= uSizeBuffer);
	uSizeUsed += _offsetof(KS2C_SYNC_PLAYER, sVarSync.aMapVarSet);

	if (g_cOrpgWorld.m_bDebugSelf)
		printf("[%u] saw [%u],", m_pThisPlayer->m_dwId, pPlayer->m_dwId);
	bResult = g_pBaseSender->SendLenData(m_pThisPlayer->GetPlayerIndex(), sData, uSizeUsed);
	LOG_PROCESS_ERROR(bResult && "g_pBaseSender->SendLenData Failed!");
EXIT1:
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KSyncRegionNpcLeaveFunc::OnTraverse(KSceneObject* pObj)
{
	BOOL bResult = FALSE;
	KS2C_SYNC_LEAVE sSyncLeave;

	sSyncLeave.dwId = pObj->GetId();

	g_pBaseSender->SendData(m_pThisPlayer->GetPlayerIndex(), sSyncLeave);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KSyncRegionDoodadLeaveFunc::OnTraverse(KSceneObject* pObj)
{
	BOOL bResult = FALSE;
	KS2C_SYNC_LEAVE sSyncLeave;

	sSyncLeave.dwId = pObj->GetId();

	g_pBaseSender->SendData(m_pThisPlayer->GetPlayerIndex(), sSyncLeave);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KSyncRegionPlayerLeaveFunc::OnTraverse(KSceneObject* pObj)
{
	BOOL bResult = FALSE;
	KS2C_SYNC_LEAVE sSyncLeave;

	sSyncLeave.dwId = pObj->GetId();

	g_pBaseSender->SendData(m_pThisPlayer->GetPlayerIndex(), sSyncLeave);

	m_vPlayerList.push_back((KPlayer*)pObj);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KSyncRegionNpcFunc::OnTraverse(KSceneObject* pObj)
{
	BOOL bResult = FALSE;
	KNpc* pNpc = (KNpc*)pObj;
	KS2C_SYNC_NPC sSync;

	SIZE_T uSizeUsed = 0;
	SIZE_T uSizeBuffer = sizeof(g_abySyncPackBuffer) - _offsetof(KS2C_SYNC_NPC, sVarSync);
	KS2C_SYNC_NPC& sData = *(KS2C_SYNC_NPC* )g_abySyncPackBuffer;
	sData = KS2C_SYNC_NPC();

	LOG_PROCESS_ERROR((PVOID)pNpc != (PVOID)m_pThisPlayer); // Npc和Player列表弄混了？
	LOG_PROCESS_ERROR(pNpc);
	LOG_PROCESS_ERROR(m_pThisPlayer);

	// 如果玩家处于fork状态，那么NPC的数据不需要同步给该玩家
	PROCESS_ERROR(pNpc->GetScene() == m_pThisPlayer->GetScene());
	
	// fill npc basic info
	QStrCpyLen(sData.szName, pNpc->GetName(), sizeof(sData.szName));
	sData.dwTemplateId = pNpc->GetTemplateId();
	sData.dwId = pNpc->m_dwId;
	sData.nMoveState = pNpc->m_eMoveState;
	sData.nVelocityXY = pNpc->m_nVelocityXY;
	sData.nDirectionXY = pNpc->m_nDirectionXY;
	pNpc->GetPosition(sData.nX, sData.nY, sData.nZ);
	pNpc->GetDestination(sData.nDestX, sData.nDestY, sData.nDestZ);
	// fill variant sync
	bResult = pNpc->m_cNpcServer.FillSyncVar(sData.sVarSync, uSizeBuffer, uSizeUsed);
	LOG_PROCESS_ERROR(bResult && uSizeUsed <= uSizeBuffer);
	uSizeUsed += _offsetof(KS2C_SYNC_NPC, sVarSync.aMapVarSet);

	if (g_cOrpgWorld.m_bDebugSelf)
		printf("[%u] saw [0x%X] %d,", m_pThisPlayer->m_dwId, pNpc->m_dwId, g_cOrpgWorld.m_dwLogicFrames);

	bResult = g_pBaseSender->SendLenData(m_pThisPlayer->GetPlayerIndex(), sData, uSizeUsed);
	LOG_PROCESS_ERROR(bResult && "g_pBaseSender->SendLenData");

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KSyncRegionDoodadFunc::OnTraverse(KSceneObject* pObj)
{
	BOOL bResult = FALSE;
	KDoodad* pDoodad = (KDoodad*)pObj;
	KS2C_SYNC_DOODAD sSync;

	LOG_PROCESS_ERROR((PVOID)pDoodad != (PVOID)m_pThisPlayer);
	LOG_PROCESS_ERROR(pDoodad);
	LOG_PROCESS_ERROR(m_pThisPlayer);

	// 如果玩家处于fork状态，不需要给他同步doodad
	PROCESS_ERROR(pDoodad->GetScene() == m_pThisPlayer->GetScene());

	//PROCESS_ERROR(pDoodad->m_dwOwnerId == KD_BAD_ID || m_pThisPlayer->GetId() == pDoodad->m_dwOwnerId);
	PROCESS_ERROR(pDoodad->m_dwOwnerId == KD_BAD_ID || pDoodad->IsPlayerVisible(m_pThisPlayer));

	// fill basic info
	sSync.dwTemplateId = pDoodad->m_dwTemplateId;
	sSync.dwId = pDoodad->m_dwId;
	pDoodad->GetPosition(sSync.nX, sSync.nY, sSync.nZ);
	sSync.nGenre = pDoodad->m_nGenre;
	sSync.nDetail = pDoodad->m_nDetail;
	sSync.nParticular = pDoodad->m_nParticular;
	sSync.nLevel = pDoodad->m_nLevel;
	sSync.nAmount = pDoodad->m_nAmount;

	sSync.byAutoPickUp = 0;
	if(pDoodad->m_nVisibleMode == emDoodadVisibleSelf)
	{
		sSync.byAutoPickUp = 1;
	}

	/*
	sSync.nMoveState = pDoodad->m_eMoveState;
	sSync.nVelocityXY = pDoodad->m_nVelocityXY;
	sSync.nDirectionXY = pDoodad->m_nDirectionXY;
	pDoodad->GetDestination(sSync.nDestX, sSync.nDestY, sSync.nDestZ);
	sSync.nDoodadState = pDoodad->GetState();
	*/

	if (g_cOrpgWorld.m_bDebugSelf)
		printf("[%u] saw [0x%X],", m_pThisPlayer->m_dwId, pDoodad->m_dwId);

	bResult = g_pBaseSender->SendData(m_pThisPlayer->GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bResult && "g_pBaseSender->SendData");

	bResult = TRUE;
EXIT0:
	return bResult;
}
////////////////////////////////////////////Synchronization Helper Function Objects//////////////////////////////



// -------------------------------------------------------------------------

#endif // GAME_SERVER
