
#ifdef GAME_SERVER
#include "stdafx.h"
#include "kplayerbaseprocess.h"
#include "onlinegameworld/kprotocol.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kdoodadmgr.h"
#include "../onlinegameworld/KRelayClient.h"
#include "onlinegameworld/kdoodad.h"
#include "onlinegameworld/scene_def.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/synchelper.h"
#include "onlinegameworld/koverloadextension.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/knpcmgr.h"
#include "../onlinegamemodule/item2/kplayeritem.h"
#include "kvarsetting.h"
#include "../nsworld/kcharacterhelper.h" // todo: revive应该移到world层character上

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KPlayerBaseProcess g_cPlayerBaseProcess;
// -------------------------------------------------------------------------
KPlayerBaseProcess::KPlayerBaseProcess()
{
	m_aProcessFunc[emKPLBASE_PTC_C2S_LOGIN] = NULL;
	m_aProcessFunc[emKPLBASE_PTC_C2S_SYNCEND] = &KPlayerBaseProcess::PtcSyncEnd;

	m_aProcessFunc[emKPLBASE_PTC_C2S_PING] = &KPlayerBaseProcess::OnPing;
	m_aProcessFunc[emKPLBASE_PTC_C2S_JUMP] = &KPlayerBaseProcess::OnJump;
	m_aProcessFunc[emKPLBASE_PTC_C2S_MOVE] = &KPlayerBaseProcess::OnMove;
	m_aProcessFunc[emKPLBASE_PTC_C2S_MOVE_CTRL] = &KPlayerBaseProcess::OnMoveCtrl;
	m_aProcessFunc[emKPLBASE_PTC_C2S_DIALOG_DOODAD] = &KPlayerBaseProcess::OnDialogDoodad;
	m_aProcessFunc[emKPLBASE_PTC_C2S_REQUEST_OFFLINE] = &KPlayerBaseProcess::OnRequestOffline;
	m_aProcessFunc[emKPLBASE_PTC_C2S_CALL_SERVER_SCRIPT] = &KPlayerBaseProcess::OnCallServerScript;
	m_aProcessFunc[emKPLBASE_PTC_C2S_APPLYNPCTALK] = &KPlayerBaseProcess::OnNpcTalk;
	m_aProcessFunc[emKPLBASE_PTC_C2S_APPLY_SYNC_NEIGHBOR] = &KPlayerBaseProcess::OnApplySyncNeighbor;
	m_aProcessFunc[emKPLBASE_PTC_C2S_APPLY_SET_VAR] = &KPlayerBaseProcess::OnApplySetVar;
	m_aProcessFunc[emKPLBASE_PTC_C2S_APPLY_REVIVE] = &KPlayerBaseProcess::OnApplyRevive;
	m_aProcessFunc[emPLAYERBASE_C2S_DIALOG_SELECT] = &KPlayerBaseProcess::OnDialogSelect;
	m_aProcessFunc[emPLAYERBASE_C2S_PICKUP_DOODAD] = &KPlayerBaseProcess::OnPickUpDoodad;
	m_aProcessFunc[emPLAYERBASE_C2S_GET_DELAYDATA] = &KPlayerBaseProcess::OnGetDelayDataRequest;
	m_aProcessFunc[emPLAYERBASE_C2S_ACTION_AROUND] = &KPlayerBaseProcess::OnPlayActionRequest;
	m_aProcessFunc[emPLAYERBASE_C2S_PROCESS_DAILY_TASK] = &KPlayerBaseProcess::OnPlayProcessDailyTask;
}

BOOL KPlayerBaseProcess::ProcessProtocol( INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen )
{
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer && pbData);
	PLAYER_GAME_STATE eGameState = pPlayer->GetGameState();
	switch (eGameState)
	{
	case emPLAYERSTATE_BEGIN:
		ProcessBeginning(pPlayer, pbData, uDataLen);
		break;
	case gsPlaying:
		ProcessPlaying(pPlayer, pbData, uDataLen);
		break;
	case gsWaitForConnect:
	case gsWaitForPermit:
	case gsWaitForRoleData:
	case gsWaitForLoginLoading:
	case gsWaitForNewMapLoading:
	case gsSearchMap:
	case gsWaitForTransmissionSave:
	case gsWaitForTransmissionGuid:
	case gsDeleting:
		break;
	default:
		QLogPrintf(LOG_ERR, "Unknown Game State [%d] for %s", eGameState, pPlayer->GetName());
		break;
	}
	return TRUE;
}

BOOL KPlayerBaseProcess::ProcessBeginning( KPlayer* pPlayer, LPCBYTE pData, UINT uDataLen )
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pData;
	// 只允许KC2SSYNC_END协议上来
	if (pHeader->byProtocol != emKPLBASE_PTC_C2S_SYNCEND)
	{
		QLogPrintf(LOG_ERR, "Invalid Protocol [%d,%d]", pHeader->byProtocolFamily, pHeader->byProtocol);
	}
	QCONFIRM_RET_FALSE(pHeader->byProtocol == emKPLBASE_PTC_C2S_SYNCEND);
	PtcSyncEnd(pPlayer, pData, uDataLen);
	//pPlayer->SetGameState(gsPlaying);
	return TRUE;
}

BOOL KPlayerBaseProcess::ProcessPlaying( KPlayer* pPlayer, LPCBYTE pData, UINT uDataLen )
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pData;
	QCONFIRM_RET_FALSE(m_aProcessFunc[pHeader->byProtocol]);
	return (this->*m_aProcessFunc[pHeader->byProtocol])(pPlayer, pData, uDataLen);
}

BOOL KPlayerBaseProcess::ProcessExchanging( KPlayer* pPlayer, LPCBYTE pData, UINT uDataLen )
{
	return TRUE;
}

BOOL KPlayerBaseProcess::PtcSyncEnd( KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen )
{
	INT nCtrlId = -1;
	KCharacterCtrlBase* pCtrlBase = NULL;

	while (NULL != (pCtrlBase = pPlayer->TraverseNextCtrl(nCtrlId)))
	{
		pCtrlBase->OnClientReady();
	}

	return TRUE;
}

BOOL KPlayerBaseProcess::OnMove( KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen )
{
	KC2S_MOVE* pMove = (KC2S_MOVE*)pbData;
	INT nPreviousMoveState = pPlayer->m_eMoveState;
	INT nPrevX = 0, nPrevY = 0, nPrevZ = 0;
	pPlayer->GetPosition(nPrevX, nPrevY, nPrevZ);

	QCONFIRM_RET_FALSE(pPlayer->CanMove());

	// 为保证速度改变不会引起MoveAdjust，速度的改变由客户端发起，即服务端改变DelaySpeed，客户端确认并重发goto协议
   if (pMove->nSpeed > 0)
   {
	   INT nIndex = pMove->nSpeed >> 16 & 0xffff;
	   if (pPlayer->m_SpeedCheckArr.size() > nIndex)
	   {
		   pPlayer->SetSpeed(pPlayer->m_SpeedCheckArr[nIndex]);
		   //pPlayer->m_SpeedCheckArr.erase(pPlayer->m_SpeedCheckArr.begin(), pPlayer->m_SpeedCheckArr.begin() + nIndex + 1);
		   if (pPlayer->m_SpeedCheckArr.size() == nIndex + 1) // 最后一个
		   {
               pPlayer->SetSpeedDelaySet(0);
			   pPlayer->m_SpeedCheckArr.clear();
		   }
	   }
   }

	DWORD dwCurrentVirtualFrame = pPlayer->m_cPlayerServer.m_dwVirtualFrame;

	BOOL bAdjust = false;
	KMOVE_CRITICAL_PARAM pParam;
	pParam.byMoveState = pMove->nMoveState;
	pParam.nX = pMove->nX;
	pParam.nY = pMove->nY;
	pParam.nZ = pMove->nZ;
	bAdjust = pPlayer->m_cPlayerServer.CheckMoveAdjust(pMove->dwFrame, pParam);

	IKScene* pScene = pPlayer->GetScene();
	QCONFIRM_RET_FALSE(pScene);
	INT nMapId = pScene->GetTemplateId();

	// 优化：排除上一个场景的移动包
	if (pMove->nMapId == nMapId /*|| pMove->nMoveCount == 1 || pPlayer->m_sMoveCtrl.nMoveCount != 0*/)
	{
		if (pMove->nMoveState == cmsOnKnockedBack) // 击退
		{
			INT nX = 0, nY = 0, nZ = 0;
			pPlayer->GetPosition(nX, nY, nZ);
			INT nDirection = g_GetDirection(nX, nY, pMove->nDestX, pMove->nDestY);
			pPlayer->SetMoveState(cmsOnKnockedBack);
		    pPlayer->KnockTo(pMove->nDestX, pMove->nDestY, pMove->nDestZ, nDirection);
		}
		else if (pMove->nMoveState == cmsOnPull)
		{
			INT nX = 0, nY = 0, nZ = 0;
			pPlayer->GetPosition(nX, nY, nZ);
			INT nDirection = g_GetDirection(nX, nY, pMove->nDestX, pMove->nDestY);
			pPlayer->SetMoveState(cmsOnPull);
			pPlayer->Pull(pMove->nDestX, pMove->nDestY, pMove->nDestZ, nDirection);
		}
		else if (pMove->nMoveState == cmsOnHalt)
		{
			pPlayer->SetMoveMode(pMove->nMoveMode);
			pPlayer->SetMoveState(cmsOnHalt);
			pPlayer->GoTo(pMove->nDestX, pMove->nDestY, pMove->nDestZ);
		}
		else
		{
			if(pPlayer->GetMoveState() != cmsOnDeath)
			{
				pPlayer->SetMoveMode(pMove->nMoveMode);
				pPlayer->SetMoveState(cmsOnRun);
				pPlayer->GoTo(pMove->nDestX, pMove->nDestY, pMove->nDestZ);
			}
		}
	}
	pPlayer->m_sMoveCtrl.nMoveCount = pMove->nMoveCount;

	//pPlayer->m_sMoveCtrl.nDestX  = pMove->nDestX;
	//pPlayer->m_sMoveCtrl.nDestY       = pMove->nDestY;
	//pPlayer->m_sMoveCtrl.nDestZ       = pMove->nDestZ;
	//pPlayer->m_sMoveCtrl.nMoveState = pMove->nMoveState;

	//// 防止客户端传入非法参数导致同步出错(取位域)
	//MAKE_IN_RANGE(pPlayer->m_MoveCtrl.nDirection8, 0, 7);
	//MAKE_IN_RANGE(pPlayer->m_MoveCtrl.nTurn, -1, 1);

	//QLogPrintf(
	//    LOG_DEBUG, "[move] ctrl: frame(%d), bMove(%d), bWalk(%d), nDirection8(%d), nTurn(%d)\n",
	//    pPlayer->m_dwVirtualFrame, pMoveCtrl->bMove, pMoveCtrl->bWalk, 
	//    pMoveCtrl->byDirection8, pMoveCtrl->nTurn
	//);

	if (bAdjust)
	{
		INT nX = 0, nY = 0, nZ = 0;
		pPlayer->GetPosition(nX, nY, nZ);

		printf("[MoveAdjust] S[PreviouseState:%d- ThisMoveState%d],client position[%d,%d,%d] server before position[%d,%d,%d]server after position[%d,%d,%d],F[S%d C%d :%d]\n",
			nPreviousMoveState,
			pMove->nMoveState,
			pMove->nX, pMove->nY, pMove->nZ,
			nPrevX, nPrevY, nPrevZ,
			nX, nY, nZ,
			INT(dwCurrentVirtualFrame),
			INT(pMove->dwFrame),
			INT(dwCurrentVirtualFrame - pMove->dwFrame));

		DoAdjustPlayerMove(pPlayer);
	}

	//KFollowCtrl* pFollowCtrl = pPlayer->GetCtrl<KFollowCtrl>();
	//ASSERT(pFollowCtrl);
	//pFollowCtrl->OnMove();

	return TRUE;
}

BOOL KPlayerBaseProcess::OnMoveCtrl( KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen )
{
	KC2S_MOVE_CTRL* pMove = (KC2S_MOVE_CTRL*)pbData;
	printf("[MoveCtrl] %d,%d,%d D%d M%d, W%d\n",
		pMove->nX, pMove->nY, pMove->nZ, pMove->nDirection, pMove->nMove, pMove->nWalk);
	INT nPreviousMoveState = pPlayer->m_eMoveState;
	INT nPrevX = 0, nPrevY = 0, nPrevZ = 0;
	pPlayer->GetPosition(nPrevX, nPrevY, nPrevZ);

	QCONFIRM_RET_FALSE(pPlayer->CanMove());

	BOOL bAdjust = FALSE;
	KMOVE_CRITICAL_PARAM pParam;
	pParam.byMoveState = pMove->nMoveState;
	pParam.nX = pMove->nX;
	pParam.nY = pMove->nY;
	pParam.nZ = pMove->nZ;
	bAdjust = pPlayer->m_cPlayerServer.CheckMoveAdjust(pMove->dwFrame, pParam);

	//pPlayer->m_sMoveCtrl.nDestX  = pMove->nDestX;
	//pPlayer->m_sMoveCtrl.nDestY       = pMove->nDestY;
	//pPlayer->m_sMoveCtrl.nDestZ       = pMove->nDestZ;
	pPlayer->m_sMoveCtrl.nMoveState = pMove->nMoveState;
	pPlayer->m_sMoveCtrl.bMove = pMove->nMove;
	pPlayer->m_sMoveCtrl.bWalk = pMove->nWalk;
	pPlayer->m_sMoveCtrl.nDirection = pMove->nDirection;
	pPlayer->m_sMoveCtrl.nMoveCount = pMove->nMoveCount;

	if ( ! pMove->nMove)
		pPlayer->StopMoving();
	//pPlayer->GoTo(pMove->nDestX, pMove->nDestY, pMove->nDestZ);

	//// 防止客户端传入非法参数导致同步出错(取位域)
	//MAKE_IN_RANGE(pPlayer->m_MoveCtrl.nDirection8, 0, 7);
	//MAKE_IN_RANGE(pPlayer->m_MoveCtrl.nTurn, -1, 1);

	if (bAdjust)
	{
		INT nX = 0, nY = 0, nZ = 0;
		pPlayer->GetPosition(nX, nY, nZ);

		printf("[MoveAdjust] S[%d-%d],[%d,%d,%d][%d,%d,%d],F[S%u C%u :%u]\n",
			nPreviousMoveState,
			pMove->nMoveState,
			nPrevX, nPrevY, nPrevZ,
			nX, nY, nZ,
			pPlayer->m_cPlayerServer.m_dwVirtualFrame,
			pMove->dwFrame,
			pPlayer->m_cPlayerServer.m_dwVirtualFrame - pMove->dwFrame);

		DoAdjustPlayerMove(pPlayer);
	}

	//KFollowCtrl* pFollowCtrl = pPlayer->GetCtrl<KFollowCtrl>();
	//ASSERT(pFollowCtrl);
	//pFollowCtrl->OnMove();

	return TRUE;
}

BOOL KPlayerBaseProcess::OnRequestOffline(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	KS2C_RESPONSE_OFFLINE	offline;
	return g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), offline);
}

BOOL KPlayerBaseProcess::OnPing( KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen )
{
	KC2S_PING* pPing = (KC2S_PING*)pbData;
	QCONFIRM_RET_FALSE(pPing);
	pPlayer->m_cPlayerServer.m_timeLastPing = KSysService::Time(NULL);

	KS2C_PING sPingBack;
	sPingBack.dwClientTime = pPing->dwClientTime;
	g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sPingBack);
	return TRUE;
}

BOOL KPlayerBaseProcess::OnJump( KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen )
{
	KC2S_JUMP* pJumpParam = (KC2S_JUMP*)pbData;
	INT nPreviousMoveState = pPlayer->m_eMoveState;
	INT nPrevX = 0, nPrevY = 0, nPrevZ = 0;
	pPlayer->GetPosition(nPrevX, nPrevY, nPrevZ);

	BOOL bAdjust = FALSE;
	KMOVE_CRITICAL_PARAM pParam;
	pParam.byMoveState = pJumpParam->nMoveState;
	pParam.nX = pJumpParam->nX;
	pParam.nY = pJumpParam->nY;
	pParam.nZ = pJumpParam->nZ;
	bAdjust = pPlayer->m_cPlayerServer.CheckMoveAdjust(pJumpParam->dwFrame, pParam);

	pPlayer->Jump();

	if (bAdjust)
	{
		INT nX = 0, nY = 0, nZ = 0;
		pPlayer->GetPosition(nX, nY, nZ);

		if (g_cOrpgWorld.m_bDebugSelf)
		{
			printf("[MoveAdjust] S[%d-%d],[%d,%d,%d][%d,%d,%d],F[S%u C%u :%u]\n",
				nPreviousMoveState,
				pJumpParam->nMoveState,
				nPrevX, nPrevY, nPrevZ,
				nX, nY, nZ,
				pPlayer->m_cPlayerServer.m_dwVirtualFrame,
				pJumpParam->dwFrame,
				pPlayer->m_cPlayerServer.m_dwVirtualFrame - pJumpParam->dwFrame);
		}

		DoAdjustPlayerMove(pPlayer);
	}
	//if ( ! bAdjust && g_cOrpgWorld.m_dwLogicFrames != pJumpParam->dwFrame)
	//	printf(".");

	return TRUE;
}

BOOL KPlayerBaseProcess::OnDialogDoodad( KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen )
{
	KC2S_DIALOG_DOODAD* pDialog = (KC2S_DIALOG_DOODAD*)pbData;
	QCONFIRM_RET_FALSE(pPlayer);
	QCONFIRM_RET_FALSE(pDialog);
	return pPlayer->ApplyOpenDoodad(pDialog->dwDoodadId);
}

BOOL KPlayerBaseProcess::OnCallServerScript(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen )
{
	QCONFIRM_RET_FALSE(pbData && uDataLen);
	BOOL bResult = FALSE;
	KC2S_SCRIPT_REMOTECALL* pProtocol = (KC2S_SCRIPT_REMOTECALL*)pbData;
	QCONFIRM_RET_FALSE(uDataLen > sizeof(KC2S_SCRIPT_REMOTECALL));

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	if (cSafeScript->LoadBuffer2Value(pProtocol->arBuffer, uDataLen - sizeof(KC2S_SCRIPT_REMOTECALL)))
	{
		QCONFIRM_RET_FALSE(cSafeScript->IsTable(-1));

		C2SFunStat(cSafeScript, uDataLen); // 统计

		g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());

		bResult = cSafeScript->CallTableFunction("c2s", "OnClientCall", 0,
			"ov",
			pPlayer->GetScriptInterface(),
			cSafeScript.GetBeginTopIndex() + 1
			);

		g_cScriptManager.SetMe(NULL);
	}
	return bResult;
}

BOOL KPlayerBaseProcess::DoSyncMove( KSceneObject* pObj, BOOL bSyncSelf/* = FALSE*/ )
{
	KS2C_SYNC_MOVE sSync;

	pObj->GetPosition(sSync.nX, sSync.nY, sSync.nZ);
	pObj->GetDestination(sSync.nDestX, sSync.nDestY, sSync.nDestZ);

	sSync.dwId = pObj->m_dwId;
	sSync.nMoveState = pObj->GetMoveState();
	sSync.nMoveMode = pObj->MoveMode();

	// broadcast
	KBroadcastPlayerFunc cBroadcastor;
	//cBroadcastor.m_nRange = 1; // by ldy 
	
	if ( ! bSyncSelf)
		cBroadcastor.m_pExcept = pObj;
	cBroadcastor.m_pCenterObject = pObj;
	cBroadcastor.m_pvData = &sSync;
	cBroadcastor.m_uSize = sizeof(sSync);

	IKScene* pScene = pObj->GetScene();
	QCONFIRM_RET_FALSE(pScene);

	//printf("player[%d] move, told ", pObj->m_dwId);
	pScene->TraverseNearbyPlayers(cBroadcastor, pObj); // 向自己周围玩家广播
	//printf("\n");

	return TRUE;
}

BOOL KPlayerBaseProcess::DoSyncTeleport( KSceneObject* pObj, INT nX, INT nY, INT nZ )
{
	KS2C_SYNC_TELEPORT sSync;
	sSync.dwId = pObj->m_dwId;
	sSync.nX = nX;
	sSync.nY = nY;
	sSync.nZ = nZ;

	// broadcast
	KBroadcastFunc cBroadcastor;
	cBroadcastor.m_pCenterObject = pObj;
	cBroadcastor.m_pvData = &sSync;
	cBroadcastor.m_uSize = sizeof(sSync);
	//cBroadcastor.m_nRange = 1; //by ldy 限制广播范围

	IKScene* pScene = pObj->GetScene();
	QCONFIRM_RET_FALSE(pScene);

	//printf("player[%d] move, told ", pPlayer->m_dwId);
	pScene->TraverseNearbyPlayers(cBroadcastor, pObj); // 向自己周围玩家广播
	//printf("\n");

	return TRUE;
}

BOOL KPlayerBaseProcess::DoSyncJump( KSceneObject* pObj, BOOL bSyncSelf/* = FALSE*/ )
{
	KS2C_SYNC_JUMP sSync;
	//sSync.nMoveState = ; // TODO
	sSync.nJumpCount = 0; // TODO:
	sSync.dwId = pObj->m_dwId;

	// broadcast
	KBroadcastFunc cBroadcastor;
	if ( ! bSyncSelf)
		cBroadcastor.m_pExcept = pObj;
	cBroadcastor.m_pCenterObject = pObj;
	cBroadcastor.m_pvData = &sSync;
	cBroadcastor.m_uSize = sizeof(sSync);
	//cBroadcastor.m_nRange = 1; //by ldy 限制广播范围

	IKScene* pScene = pObj->GetScene();
	QCONFIRM_RET_FALSE(pScene);

	//printf("player[%d] jump, told ", pObj->m_dwId);
	pScene->TraverseNearbyPlayers(cBroadcastor, pObj); // 向自己周围玩家广播
	//printf("\n");

	return TRUE;
}

BOOL KPlayerBaseProcess::DoSyncJumpTo( KSceneObject* pObj, INT nX, INT nY, INT nZ)
{
	KS2C_SYNC_JUMP_TO sSync;
	sSync.nDestX = nX;
	sSync.nDestY = nY;
	sSync.nDestZ = nZ;
	pObj->GetPosition(sSync.nX, sSync.nY, sSync.nZ);
	sSync.dwId = pObj->m_dwId;

	// broadcast
	KBroadcastFunc cBroadcastor;
	cBroadcastor.m_pCenterObject = pObj;
	cBroadcastor.m_pvData = &sSync;
	cBroadcastor.m_uSize = sizeof(sSync);
	//cBroadcastor.m_nRange = 1; //by ldy 限制广播范围

	IKScene* pScene = pObj->GetScene();
	QCONFIRM_RET_FALSE(pScene);

	pScene->TraverseNearbyPlayers(cBroadcastor, pObj);

	return TRUE;
}

BOOL KPlayerBaseProcess::DoSyncFaceDirection( KSceneObject* pObj, INT nFaceDirection, BOOL bSyncSelf/* = FALSE*/ )
{
	KS2C_SYNC_FACE_DIRECTION sSync;
	sSync.dwId = pObj->m_dwId;
	sSync.nFaceDirection = nFaceDirection;

	// broadcast
	KBroadcastFunc cBroadcastor;
	if ( ! bSyncSelf)
		cBroadcastor.m_pExcept = pObj;
	cBroadcastor.m_pCenterObject = pObj;
	cBroadcastor.m_pvData = &sSync;
	cBroadcastor.m_uSize = sizeof(sSync);
	//cBroadcastor.m_nRange = 1;

	IKScene* pScene = pObj->GetScene();
	QCONFIRM_RET_FALSE(pScene);

	pScene->TraverseNearbyPlayers(cBroadcastor, pObj); // 向自己周围玩家广播

	return TRUE;
}

BOOL KPlayerBaseProcess::DoAdjustPlayerMove( KPlayer* pPlayer )
{
	BOOL                        bResult     = false;
	BOOL                        bRetCode    = false;
	KS2C_SYNC_ADJUST_MOVE sPak;

	LOG_PROCESS_ERROR(pPlayer->m_nDirectionXY >= 0 && pPlayer->m_nDirectionXY < DIRECTION_COUNT);
	LOG_PROCESS_ERROR(pPlayer->m_nFaceDirection >= 0 && pPlayer->m_nFaceDirection < DIRECTION_COUNT);
	LOG_PROCESS_ERROR(pPlayer->m_nCurrentGravity >= 0 && pPlayer->m_nCurrentGravity <= MAX_GRAVITY);

	sPak.dwVirtualFrame = pPlayer->m_cPlayerServer.m_dwVirtualFrame;
	sPak.nVelocityXY = pPlayer->m_nVelocityXY;
	sPak.nDirectionXY = pPlayer->m_nDirectionXY;
	sPak.nMoveState = pPlayer->m_eMoveState;
	sPak.nMoveCount = pPlayer->m_sMoveCtrl.nMoveCount;
	pPlayer->GetPosition(sPak.nX, sPak.nY, sPak.nZ);
	pPlayer->GetDestination(sPak.nDestX, sPak.nDestY, sPak.nDestZ);

	//QLogPrintf(
	//    LOG_DEBUG, "[MoveAdjust] <state %d, %d, %d, %d>", 
	//   /* Pak.nMoveCount, sPak.dwVirtualFrame,*/
	//    pPlayer->m_eMoveState, sPak.nX, sPak.nY, sPak.nZ
	//);
	PROCESS_ERROR(pPlayer->GetPlayerIndex() > 0);//可能被释放了
	bRetCode = g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sPak);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KPlayerBaseProcess::DoSyncFrame( KPlayer* pPlayer )
{
	KS2C_FRAME_SIGNAL sSignal;
	sSignal.dwLogicFrames = g_cOrpgWorld.m_dwLogicFrames;

	g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sSignal);
	return TRUE;
}

BOOL KPlayerBaseProcess::DoSyncVisible(KPlayer* pPlayer, DWORD dwCharacterId, BOOL bVisible)
{
	KS2C_SYNC_VISIBLE sPack;
	sPack.dwId = dwCharacterId;
	sPack.byVisible = (BYTE)bVisible;

	g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sPack);
	return TRUE;
}

BOOL KPlayerBaseProcess::DoSyncHeadText(KPlayer* pPlayer, DWORD dwCharacterId, INT nType, INT nNumber)
{
	KS2C_SYNC_HEAD_TEXT sSync;
	sSync.dwCharacterId = dwCharacterId;
	sSync.nType = nType;
	sSync.nDamage = nNumber;

	g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sSync);
	return TRUE;
}

BOOL KPlayerBaseProcess::BroadcastHeadText( KSceneObject& rSceneObj, DWORD dwCharacterId, INT nType, INT nNumber)
{
	KS2C_SYNC_HEAD_TEXT sSync;
	sSync.dwCharacterId = dwCharacterId;
	sSync.nType = nType;
	sSync.nDamage = nNumber;

	// broadcast
	KBroadcastFunc cBroadcastor;
	cBroadcastor.m_pCenterObject = &rSceneObj;
	cBroadcastor.m_pvData = &sSync;
	cBroadcastor.m_uSize = sizeof(sSync);
	//cBroadcastor.m_nRange = 1;

	IKScene* pScene = rSceneObj.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	pScene->TraverseNearbyPlayers(cBroadcastor, &rSceneObj); // 向周围玩家广播

	return TRUE;
}

BOOL KPlayerBaseProcess::DoSyncBlackMsg(KPlayer* pPlayer, LPCSTR pszMessage)
{
	static BYTE _sbyBuffer[USHRT_MAX];
	KS2C_BLACK_MSG* pSync = (KS2C_BLACK_MSG*)_sbyBuffer;
	*pSync = KS2C_BLACK_MSG();

	INT nLen = strlen(pszMessage);
	strncpy((CHAR*)pSync->arBuffer, pszMessage, nLen);
	pSync->arBuffer[nLen] = 0;

	g_pBaseSender->SendLenData(pPlayer->GetPlayerIndex(), *pSync, sizeof(*pSync) + nLen);
	return 0;
}

BOOL KPlayerBaseProcess::DoSyncDegree(KPlayer* pPlayer, LPCSTR pszModuleName, INT nCurDegree, INT nMaxDegree)
{
	KS2C_SYNC_DEGREE sSync;

	//memcpy(sSync.szModuleName, pszModuleName, MAX_NAME_LEN);
	snprintf(sSync.szModuleName,sizeof(sSync.szModuleName),pszModuleName);
	sSync.nCurDegree = nCurDegree;
	sSync.nMaxDegree = nMaxDegree;

	return g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sSync);
}

BOOL KPlayerBaseProcess::DoSyncLeave(KPlayer* pPlayer, DWORD dwCharacterId)
{
	KS2C_SYNC_LEAVE sSync;
	sSync.dwId = dwCharacterId;

	g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sSync);
	return TRUE;
}

BOOL KPlayerBaseProcess::DoSyncLeave( KSceneObject& rSceneObj, BOOL bSyncSelf /*= FALSE*/ )
{
	KS2C_SYNC_LEAVE sSync;
	sSync.dwId = rSceneObj.GetId();
	// broadcast
	KBroadcastFunc cBroadcastor;
	cBroadcastor.m_pCenterObject = &rSceneObj;
	cBroadcastor.m_pvData = &sSync;
	cBroadcastor.m_uSize = sizeof(sSync);
	//cBroadcastor.m_nRange = 1; //by ldy 限制广播范围

	if ( ! bSyncSelf)
		cBroadcastor.m_pExcept = &rSceneObj;

	IKScene* pScene = rSceneObj.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	//printf("[%d] leaving, told:", pPlayer->m_dwId);
	pScene->TraverseNearbyPlayers(cBroadcastor, &rSceneObj); // 向自己周围玩家广播
	//printf("\n");

	return TRUE;
}

BOOL KPlayerBaseProcess::OnNpcTalk( KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen )
{
	KC2S_APPLYNPCTALK* pMsg = (KC2S_APPLYNPCTALK*)pbData;
	QCONFIRM_RET_FALSE(pMsg);
	QCONFIRM_RET_FALSE(pPlayer);
	QCONFIRM_RET_FALSE(uDataLen == sizeof(KC2S_APPLYNPCTALK));

	return pPlayer->ApplyNpcTalk(pMsg->dwNpcId);
}

BOOL KPlayerBaseProcess::OnApplySyncNeighbor(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	ASSERT(pbData && uDataLen);
	ASSERT(pPlayer);

	KC2S_APPLY_SYNC_NEIGHBOR* pMsg = (KC2S_APPLY_SYNC_NEIGHBOR*)pbData;

	std::vector<KPlayer*> vPlayer;
	vPlayer.push_back(pPlayer);

	if (IS_NPC(pMsg->dwId))
	{
		KNpc* pTarget = g_cNpcMgr.GetById(pMsg->dwId);
		QCONFIRM_RET_FALSE(pTarget);
		if (pTarget->IsVisible() && pTarget->IsWithinSyncRange(pPlayer))
			pTarget->m_cNpcServer.SyncToNeighborPlayers(vPlayer);
	}
	else if (IS_PLAYER(pMsg->dwId))
	{
		KPlayer* pTarget = g_cPlayerMgr.GetById(pMsg->dwId);
		QCONFIRM_RET_FALSE(pTarget);
		if (pTarget->IsVisible() && pTarget->IsWithinSyncRange(pPlayer))
			pTarget->m_cPlayerServer.SyncToNeighborPlayers(vPlayer);
	}
	else
	{
		QCONFIRM_RET_FALSE(!"[OnApplySyncNeighbor] not supported type");
	}

	return TRUE;
}

BOOL KPlayerBaseProcess::OnApplySetVar(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	ASSERT(pbData && uDataLen);
	ASSERT(pPlayer);

	KC2S_APPLY_SET_VAR* pMsg = (KC2S_APPLY_SET_VAR*)pbData;

	QCONFIRM_RET_FALSE(pMsg->wVarGroupId <= UCHAR_MAX);
	QCONFIRM_RET_FALSE(pMsg->wVarId <= UCHAR_MAX);

	QCONFIRM_RET_FALSE(g_cCharacterVarSetting.CheckClientSet(pMsg->wVarGroupId, pMsg->wVarId));

	pPlayer->SetVar(pMsg->wVarGroupId, pMsg->wVarId, pMsg->dwValue);

	return TRUE;
}

BOOL KPlayerBaseProcess::OnApplyRevive(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	ASSERT(pbData && uDataLen);
	ASSERT(pPlayer);

	KC2S_APPLY_REVIVE* pMsg = (KC2S_APPLY_REVIVE*)pbData;
	
	if (pPlayer->GetMoveState() != cmsOnDeath)
	{
		return TRUE;
	}

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction("Player", "CheckRevive", 1, "od", pPlayer->GetScriptInterface(), pMsg->nChoice);
	if (!cSafeScript->GetBool(-1))
		return FALSE;

	g_cCharacterHelper.Revive(*pPlayer, 1000, 1000, pMsg->nChoice);
	
	return TRUE;
}

BOOL KPlayerBaseProcess::OnDialogSelect(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	ASSERT(pbData && uDataLen);
	ASSERT(pPlayer);

	KC2S_DIALOG_SELECT* pMsg = (KC2S_DIALOG_SELECT*)pbData;

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());  // Pass "Me",     by KK
	cSafeScript->CallTableFunction("Dialog", "c2s_OnSelect", 0, "od", pPlayer->GetScriptInterface(), pMsg->bySelect);
	g_cScriptManager.SetMe(NULL);
	return TRUE;
}

BOOL KPlayerBaseProcess::OnPickUpDoodad(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	ASSERT(pbData && uDataLen);
	ASSERT(pPlayer);

	KC2S_PICKUP_DOODAD* pSync = (KC2S_PICKUP_DOODAD*)pbData;

	DoPickUpDoodad(pPlayer, pSync->dwId);

	return TRUE;
}

BOOL KPlayerBaseProcess::DoPickUpDoodad(KPlayer* pPlayer, DWORD dwDoodadId, DWORD bOnScene /* = TRUE */)
{
	KDoodad* pDoodad = g_cDoodadMgr.GetById(dwDoodadId);
	if(!pDoodad)
	{
		return FALSE;
	}

	if(bOnScene)
	{
		IKScene* pScene = pDoodad->GetScene();
		QCONFIRM_RET_FALSE(pScene);

		QCONFIRM_RET_FALSE(pDoodad->IsPlayerVisible(pPlayer));
	}

	INT nGenre = pDoodad->m_nGenre;
	INT nDetail = pDoodad->m_nDetail;
	INT nParticular = pDoodad->m_nParticular;
	INT nLevel = pDoodad->m_nLevel;
	INT nAmount = pDoodad->m_nAmount;

	KPlayerItem* pPlayerItem = pPlayer->GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	BOOL bRetCode = FALSE;

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)pPlayer->GetScriptInterface());
	cSafeScript->CallTableFunction("Item", "PickUpItem", 1, "ddddd", nGenre, nDetail, nParticular, nLevel, nAmount);
	bRetCode = cSafeScript->GetInt(-1);

	if(bRetCode == 1)
	{
		if(bOnScene)
		{
			g_cDoodadMgr.RemoveDoodad(pDoodad->GetId());
		}
		else
		{
			g_cDoodadMgr.RemoveDoodadNoScene(pDoodad->GetId());
		}
	}

	return TRUE;
}

BOOL KPlayerBaseProcess::OnGetDelayDataRequest(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	KC2S_GET_DELAYDATA* pSync = (KC2S_GET_DELAYDATA*)pbData;

	return g_RelayClient.DoLoadDelayRoleDataRequest(0, pSync->dwSrcId, pSync->dwRoleId, pSync->szRoleName);
}

BOOL KPlayerBaseProcess::OnPlayActionRequest(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	KC2S_ACTION_AROUND* pSync = (KC2S_ACTION_AROUND*)pbData;

	// 直接转发
	return pPlayer->m_cPlayerServer.SyncActionAround(pSync->byActionId, pSync->pendantId);
}

BOOL KPlayerBaseProcess::OnPlayProcessDailyTask(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	KC2S_PROCESS_DAILY_TASK* pReq = (KC2S_PROCESS_DAILY_TASK*)pbData;
	QLogPrintf(LOG_INFO, "****************处理日常任务pReq->byActionId=%d",pReq->byActionId);
	switch(pReq->byActionId)
	{
	case KE_PLAYERBASE_C2S_DAILDY_TASK_GET_TASK_LIST:
		OnGetDailyTaskList(pPlayer, pbData, uDataLen);
		break;
	default:
		ASSERT(FALSE);
	}
	return TRUE;
}

BOOL KPlayerBaseProcess::OnGetDailyTaskList(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen)
{
	QLogPrintf(LOG_INFO, "****************处理日常任务 获得日常任务列表");
	BOOL					bResult 			= false;
	BOOL                    bRetCode            = false;

	DWORD dwDtask1 = pPlayer->GetDtask1();
	DWORD dwDtask2 = pPlayer->GetDtask2();
	DWORD dwDtask3 = pPlayer->GetDtask3();
	DWORD dwDtask4 = pPlayer->GetDtask4();
	DWORD dwTimeNow = KSysService::Time(NULL);
	

	DWORD dwDtaskRefreshTime = pPlayer->GetDtaskRefreshTime();
	BOOL isCanRefresh = FALSE;
	if(dwTimeNow - dwDtaskRefreshTime > 20 * 60)
	{
		isCanRefresh = TRUE;
	}

	DTASK_DETAIL dtDtask1 = *(DTASK_DETAIL*)&dwDtask1;
	DTASK_DETAIL dtDtask2 = *(DTASK_DETAIL*)&dwDtask2;
	DTASK_DETAIL dtDtask3 = *(DTASK_DETAIL*)&dwDtask3;
	DTASK_DETAIL dtDtask4 = *(DTASK_DETAIL*)&dwDtask4;


	if(dwDtask1 == 0)
	{
		dtDtask1.byTaskLevel = (BYTE)GetDtaskRandLevel();
		dtDtask1.byTaskId = 1;//随机任务id
		dtDtask1.byTaskStatus = DTASK_PRE_ACCEPT;
		dtDtask1.byTaskTimes = 1;
		dtDtask1.byTaskTimes = 0;
	}

	if(dwDtask2 == 0)
	{
		dtDtask2.byTaskLevel = (BYTE)GetDtaskRandLevel();
		dtDtask2.byTaskId = 2;//随机任务id
		dtDtask2.byTaskStatus = DTASK_PRE_ACCEPT;
		dtDtask2.byTaskTimes = 1;
		dtDtask2.byTaskTimes = 0;
	}

	if(dwDtask3 == 0)
	{
		dtDtask3.byTaskLevel = (BYTE)GetDtaskRandLevel();
		dtDtask3.byTaskId = 3;//随机任务id
		dtDtask3.byTaskStatus = DTASK_PRE_ACCEPT;
		dtDtask3.byTaskTimes = 1;
		dtDtask3.byTaskTimes = 0;
	}


	if(dwDtask4 == 0)
	{
		dtDtask4.byTaskLevel = (BYTE)GetDtaskRandLevel();
		dtDtask4.byTaskId = 4;//随机任务id
		dtDtask4.byTaskStatus = DTASK_PRE_ACCEPT;
		dtDtask4.byTaskTimes = 1;
		dtDtask4.byTaskTimes = 0;
	}


	KS2C_SYNC_DAILY_TASK_LIST list;
	ZeroMemory(&list, sizeof(KS2C_SYNC_DAILY_TASK_LIST));

	list.dwDailyTaskList[0].dtaskDetail = dtDtask1;
	list.dwDailyTaskList[1].dtaskDetail = dtDtask2;
	list.dwDailyTaskList[2].dtaskDetail = dtDtask3;
	list.dwDailyTaskList[3].dtaskDetail = dtDtask4;


	if(isCanRefresh)
	{
		pPlayer->SetDtaskRefreshTime(dwTimeNow);
		list.dwLastTimeRefleshTime = dwTimeNow;
	}

	int nBegin = 10;
	DWORD dwRefresh = pPlayer->GetDtaskRefreshTime();
	int nDay = ((dwRefresh + 3600 * 8) / (3600 * 24)) - ((KSysService::Time(NULL) + 3600*8) / (3600 * 24));
	
	if(nDay != 0)
	{
		pPlayer->SetDtaskUsedTimes(0);
		pPlayer->SetDtaskRefreshTime(dwTimeNow);
	}

	list.dwLastDtaskTimes = 10  -  pPlayer->GetDtaskUsedTimes()  +   pPlayer->GetVipLevel();
	
	
	bRetCode = g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), list);
	LOG_PROCESS_ERROR(bRetCode);
	bResult = true;
EXIT0:
	return bResult;
}

//获得随机等级
INT  KPlayerBaseProcess::GetDtaskRandLevel()
{
	DTASK_RAND  randTable[] = 
		{
			{1,30},
			{2,20},
			{3,20},
			{4,15},
			{5,10},
		};

	int nSum = 0;
	int nRand = 1 + rand() % (100-1+1);
	int nCount = sizeof(randTable) / sizeof(randTable[0]);
	int nIdex = -1;
	for (int i=0;i<nCount;i++)
	{
		nSum += randTable[i].nRate;
		if(nSum >= nRand)
		{
			nIdex = i;
		}
	}

	return randTable[nIdex].nLevel;
}


LPCSTR KPlayerBaseProcess::GetScriptFunStat( std::string& rstrBuffer )
{
	std::stringstream strmBuffer;
	for (std::map<std::string, KSCRIPT_FUN_STAT>::iterator it = m_mapC2SCallStatics.begin();
		it != m_mapC2SCallStatics.end();
		++it)
	{
		KSCRIPT_FUN_STAT& sScriptFunStat = it->second;
		strmBuffer << "[" << it->first << "] " << sScriptFunStat.uTimes << "/" << sScriptFunStat.uBytes << "\n";
	}
	strmBuffer << "s2c --------------------------------\n";
	for (std::map<std::string, KSCRIPT_FUN_STAT>::iterator it = m_mapS2CCallStatics.begin();
		it != m_mapS2CCallStatics.end();
		++it)
	{
		KSCRIPT_FUN_STAT& sScriptFunStat = it->second;
		strmBuffer << "[" << it->first << "] " << sScriptFunStat.uTimes << "/" << sScriptFunStat.uBytes << "\n";
	}
	rstrBuffer = strmBuffer.str();
	return rstrBuffer.c_str();
}

void KPlayerBaseProcess::C2SFunStat( ScriptSafe& cSafeScript, UINT uDataLen )
{
	cSafeScript->GetTableIndex(-1, 1); // 取栈顶表的第一个元素即c2sFun名字
	LPCSTR pszFuncName = cSafeScript->GetStr(-1);
	ASSERT(pszFuncName);

	KSCRIPT_FUN_STAT& sScriptFunStat = m_mapC2SCallStatics[pszFuncName];
	sScriptFunStat.uTimes++;
	sScriptFunStat.uBytes += uDataLen;
}

void KPlayerBaseProcess::S2CFunStat( QLuaScript& cScript, UINT uDataLen )
{
	cScript.GetTableIndex(-1, 1); // 取栈顶表的第一个元素即s2cFun名字
	LPCSTR pszFuncName = cScript.GetStr(-1);
	ASSERT(pszFuncName);

	KSCRIPT_FUN_STAT& sScriptFunStat = m_mapS2CCallStatics[pszFuncName];
	sScriptFunStat.uTimes++;
	sScriptFunStat.uBytes += uDataLen;
}

BOOL KPlayerBaseProcess::DoMessageNotify( KPlayer* pPlayer, INT nModule, INT nMsgId, const PVOID pAdditional, UINT uDataLen )
{
	BOOL                    bResult         = false;
	BOOL                    bRetCode        = false;
	S2C_MESSAGE_NOTIFY*     pMsg            = (S2C_MESSAGE_NOTIFY*)g_abySyncPackBuffer;
	size_t                  uPakSize        = sizeof(S2C_MESSAGE_NOTIFY) + uDataLen;

	LOG_PROCESS_ERROR(uPakSize <= sizeof(g_abySyncPackBuffer));

	pMsg->byProtocol = emPLAYERBASE_S2C_MESSAGE_NOTIFY;
	pMsg->byModule = (BYTE)nModule;
	pMsg->byMsgId = (BYTE)nMsgId;

	if (uDataLen > 0)
	{
		memcpy(pMsg->byData, pAdditional, uDataLen);
	}

	bRetCode = g_pBaseSender->SendLenData(pPlayer->GetPlayerIndex(), *pMsg, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KPlayerBaseProcess::DoSwitchGameServer( KPlayer* pPlayer, GUID& Guid, DWORD dwIPAddr, WORD wPort )
{
	BOOL                    bResult             = false;
	BOOL                    bRetCode            = false;
	KS2C_CHANGE_SERVER sNotify;

	sNotify.nCode = TRUE;
	sNotify.dwPlayerId = pPlayer->GetId();
	sNotify.uServerIp = dwIPAddr;
	sNotify.wPort = wPort;
	sNotify.guid = Guid;

	bRetCode = g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sNotify);
	PROCESS_ERROR(bRetCode);
	
	bResult = true;
EXIT0:
	return bResult;
}

BOOL KPlayerBaseProcess::DoSwitchBZoneGameServer( KPlayer* pPlayer, DWORD dwDstPlayerID, GUID& Guid, DWORD dwIPAddr, WORD wPort )
{
	BOOL                    bResult             = false;
	BOOL                    bRetCode            = false;
	KS2C_CHANGE_SERVER sNotify;

	sNotify.nCode = TRUE;
	sNotify.dwPlayerId = dwDstPlayerID | BZONE_PLAYER_ID_MASK;
	sNotify.uServerIp = dwIPAddr;
	sNotify.wPort = wPort;
	sNotify.guid = Guid;

	bRetCode = g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sNotify);
	PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KPlayerBaseProcess::DoBZoneReturnGameServer( KPlayer* pPlayer, DWORD dwDstPlayerID, GUID& Guid, DWORD dwIPAddr, WORD wPort )
{
	BOOL                    bResult             = false;
	BOOL                    bRetCode            = false;
	KS2C_CHANGE_SERVER sNotify;

	sNotify.nCode = TRUE;
	sNotify.dwPlayerId = dwDstPlayerID;
	sNotify.uServerIp = dwIPAddr;
	sNotify.wPort = wPort;
	sNotify.guid = Guid;

	bRetCode = g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sNotify);
	PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	return bResult;
}

// -------------------------------------------------------------------------
#endif
