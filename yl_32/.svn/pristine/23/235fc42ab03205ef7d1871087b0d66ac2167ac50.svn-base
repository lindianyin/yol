
#ifdef GAME_CLIENT
#include "stdafx.h"
#include "onlinegameworld/kplayerclient.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "kvarsetting.h"
#include "kplayerbaseprocess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KPlayerClient::KPlayerClient( KPlayer& rcPlayer ) : KCharacterClient(rcPlayer), m_rcPlayer(rcPlayer)
{
	m_nDynamicSceneFlag = 0;
}

BOOL KPlayerClient::DoGoTo( INT nX, INT nY, INT nZ, INT nMoveMode )
{
	KC2S_MOVE sSync;
	sSync.nDestX = nX;
	sSync.nDestY = nY;
	sSync.nDestZ = nZ;
	sSync.dwFrame = g_cOrpgWorld.m_dwLogicFrames;
	m_rcPlayer.GetPosition(sSync.nX, sSync.nY, sSync.nZ);
	sSync.nMoveState = m_rcPlayer.m_eMoveState;
	sSync.nMoveMode = nMoveMode;
	sSync.nMoveCount = ++m_rcPlayer.m_sMoveCtrl.nMoveCount;

	return g_pBaseSender->SendData(sSync);
}

BOOL KPlayerClient::SwitchMoveMode(INT nRun)
{
	m_rcPlayer.SetMoveMode(nRun);
	if (m_rcPlayer.m_nVelocityXY)
	{
		INT nDestX = 0, nDestY = 0, nDestZ = 0;
		m_rcPlayer.GetDestination(nDestX, nDestY, nDestZ);
		m_rcPlayer.ClientGoTo(nDestX, nDestY, nDestZ);
	}
	return TRUE;
}

BOOL KPlayerClient::DoRequestOffline()
{
	KC2S_REQUESTOFFLINE closeConnection;
	return g_pBaseSender->SendData(closeConnection);
}

BOOL KPlayerClient::DoJump()
{
	KC2S_JUMP sSync;
	sSync.nMoveState = m_rcPlayer.m_eMoveState;
	m_rcPlayer.GetPosition(sSync.nX, sSync.nY, sSync.nZ);
	sSync.dwJumpCount = 0;
	sSync.dwFrame = g_cOrpgWorld.m_dwLogicFrames;

	return g_pBaseSender->SendData(sSync);
}

BOOL KPlayerClient::DoDialogDoodad(DWORD dwDoodadId)
{
	KC2S_DIALOG_DOODAD sData;
	sData.dwDoodadId = dwDoodadId;
	return g_pBaseSender->SendData(sData);
}

BOOL KPlayerClient::DoMoveCtrl( BOOL bMove, BOOL bWalk, INT nDirection )
{
	KC2S_MOVE_CTRL sData;
	m_rcPlayer.GetPosition(sData.nX, sData.nY, sData.nZ);
	sData.dwFrame = g_cOrpgWorld.m_dwLogicFrames;
	sData.nDirection = nDirection;
	sData.nMove = bMove;
	sData.nMoveState = m_rcPlayer.GetMoveState();
	sData.nWalk = bWalk;
	sData.nMoveCount = ++m_rcPlayer.m_sMoveCtrl.nMoveCount;

	return g_pBaseSender->SendData(sData);
}

BOOL KPlayerClient::MoveCtrl( BOOL bWalk, BOOL bForward, BOOL bBackward, BOOL bLeft, BOOL bRight, INT nCamDirection )
{
	INT nDestX = 0;
	INT nDestY = 0;
	if (bForward)
		nDestY += MOVE_DEST_RANGE;
	if (bBackward)
		nDestY -= MOVE_DEST_RANGE;
	if (bRight)
		nDestX += MOVE_DEST_RANGE;
	if (bLeft)
		nDestX -= MOVE_DEST_RANGE;

	BOOL bMove = nDestX || nDestY;
	INT nDirection = g_GetDirection(0, 0, nDestX, nDestY) + nCamDirection - DIRECTION_COUNT / 4;
	if (nDirection < 0)
		nDirection += DIRECTION_COUNT;
	if (nDirection >= DIRECTION_COUNT)
		nDirection -= DIRECTION_COUNT;

	BOOL bChanged = FALSE;
	bChanged |= (bMove != m_rcPlayer.m_sMoveCtrl.bMove);
	bChanged |= (bWalk != m_rcPlayer.m_sMoveCtrl.bWalk);
	bChanged |= (bMove && nDirection != m_rcPlayer.m_sMoveCtrl.nDirection);

	m_rcPlayer.m_sMoveCtrl.bMove = bMove;
	m_rcPlayer.m_sMoveCtrl.bWalk = bWalk;
	m_rcPlayer.m_sMoveCtrl.nDirection = nDirection;

	if (bChanged)
	{
		if ( ! bMove)
			m_rcPlayer.StopMoving();
		DoMoveCtrl(bMove, bWalk, nDirection);
	}

	return TRUE;
}

BOOL KPlayerClient::DoApplyNpcTalk( KPlayer& rPlayer, DWORD dwNpcId )
{
	QCONFIRM_RET_FALSE(g_pBaseSender);
	KC2S_APPLYNPCTALK cApplyTalk;
	cApplyTalk.dwNpcId = dwNpcId;
	return g_pBaseSender->SendData(cApplyTalk);
}

// -------------------------------------------------------------------------
#endif // #ifdef GAME_CLIENT
