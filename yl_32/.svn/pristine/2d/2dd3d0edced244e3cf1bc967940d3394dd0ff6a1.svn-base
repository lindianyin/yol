#include "stdafx.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kluanpc.h"
#include "onlinegameworld/knpcmgr.h"
#include "onlinegameworld/koverloadextension.h"
#include "kplayerbaseprocess.h"

#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/synchelper.h"
#include "config/kplayerdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegamebase/kscriptmanager.h"
#include "scene/kscene.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegamemodule/bufferdef.h"
#include "onlinegamebase/performancestat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
#pragma warning(disable : 4355)
#ifdef GAME_SERVER
KNpc::KNpc() : m_cNpcServer(*this), KCharacter(m_cNpcServer, KD_NPC_ID_MASK, emKCHARACTER_TASK_MAX)
#else
KNpc::KNpc() : m_cNpcClient(*this), KCharacter(m_cNpcClient, KD_NPC_ID_MASK, emKCHARACTER_TASK_MAX)
#endif
{
	m_pLuna = new KLuaNpc(this);
	m_cCtrlSet.CreateCtrls(*this, KD_NPC_ID_MASK);
	m_pTemplate = NULL;
}
#pragma warning(default : 4355)

KNpc::~KNpc()
{
	SAFE_DELETE(m_pLuna);
}


BOOL KNpc::Init( INT nIndex,  CONST KNPC_TEMPLATE& rsTemplate )
{
	QCONFIRM_RET_FALSE(KCharacter::Init());
	m_nIndex = nIndex;
	m_dwTemplateId = rsTemplate.dwTemplateId;
	SetRepresentId(rsTemplate.nRepresentId);
	//strncpy(m_szName, rsTemplate.szClassName, countof(m_szName));
	m_strName = rsTemplate.strDisplayName;
	m_strTitle = rsTemplate.strTitle;
	m_strClassName = rsTemplate.strClassName;

	m_nTouchRange = rsTemplate.nTouchRange;
	m_nSelectable = rsTemplate.nSelectable;
	m_nDropListId = rsTemplate.nDropListId1;
	m_nDropTimes = rsTemplate.nDropTimes1;
	m_byKind = rsTemplate.byKind;

// 	m_strAIClassName = rsTemplate.strAIClassName;
// 	m_strAiParam1 = rsTemplate.strAiParam1;

	m_nNpcTeamIndex		        = -1;
	m_nNpcTeamID		        = rsTemplate.nThreatLinkId;
	m_nSkillCommomCD	        = 0;
	memset(m_nSkillCastFrame, 0, sizeof(m_nSkillCastFrame));
	m_nSkillSelectIndex         = -1;
	m_nDisappearFrame = -1;

	m_nThreatTime				= rsTemplate.nThreatTime;

	m_bDeleted = FALSE;

	m_pTemplate = &rsTemplate;

	if (KNSBASE_EXT::DoNpcInit)
	{
		(*KNSBASE_EXT::DoNpcInit)(*this);
	}
	return TRUE;
}

BOOL KNpc::Uninit()
{
	KCharacter::Uninit();

	m_bDeleted = TRUE;

	return TRUE;
}

BOOL KNpc::ChangeRegion(KRegion* pDestRegion)
{
	// 在瞬移切换region的时候，必须马上同步给目标region的玩家
	KSceneObject::ChangeRegion(pDestRegion);

	m_cNpcServer.SyncChangeRegion();

	return TRUE;
}


BOOL KNpc::Activate()
{
	BEGIN_STAT("KOrpgWorld::KRegion::KNpc.KCharacter::Activate()");
	if ( ! KCharacter::Activate())
		return FALSE;  // 可能这帧已经激活过
	END_STAT();

	BEGIN_STAT("KOrpgWorld::KRegion::KNpc.KCharacter::ProcessMove()");
	KCharacter::ProcessMove(); // 注意激活顺序
	END_STAT();

#ifdef GAME_SERVER
	//m_cNpcServer.SyncChangeRegion();

	if(g_cOrpgWorld.m_dwLogicFrames == m_nDisappearFrame)
		g_cNpcMgr.Remove(m_dwId, TRUE);

#endif // GAME_SERVER

	return TRUE;
}

BOOL KNpc::OnRemove()
{
	if (m_pScene && m_nNpcTeamID)
	{
		m_pScene->m_ThreatLink.AddMember(m_nNpcTeamID, this);
	}

	return g_cNpcMgr.Remove(m_dwId, FALSE);
}

BOOL KNpc::GoTo( INT nX, INT nY, INT nZ, BOOL bSyncSelf /*= FALSE*/ )
{
	if (GetIsFix() > 0 || GetIsStun() > 0)
	{
		return FALSE;
	}

	KSceneObject::GoTo(nX, nY, nZ);

#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncMove(this, bSyncSelf);
#endif // GAME_SERVER
	FireEvent(emKOBJEVENTTYPE_CHARACTER_MOVE, m_dwId, 0, 0);

	return TRUE;
}

BOOL KNpc::KnockTo(INT nX, INT nY, INT nZ, INT nDirection)
{
	KSceneObject::KnockTo(nX, nY, nZ, nDirection);

#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncMove(this, TRUE);
#endif // GAME_SERVER
	FireEvent(emKOBJEVENTTYPE_CHARACTER_KNOCK, m_dwId, 0, 0);

	return TRUE;
}

BOOL KNpc::Pull(INT nX, INT nY, INT nZ, INT nDirection)
{
	KSceneObject::Pull(nX, nY, nZ, nDirection);

#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncMove(this, TRUE);
#endif // GAME_SERVER
	FireEvent(emKOBJEVENTTYPE_CHARACTER_PULL, m_dwId, 0, 0);

	return TRUE;
}

BOOL KNpc::StopMove(BOOL bSyncSelf /*= FALSE*/)
{
	KSceneObject::StopMoving();
#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncMove(this, bSyncSelf);
#endif // GAME_SERVER
	return TRUE;
}
BOOL KNpc::Jump(BOOL bSyncSelf/* = FALSE*/)
{
	KSceneObject::Jump();

#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncJump(this);
#endif // GAME_SERVER

	return TRUE;
}

BOOL KNpc::SetDisappearFrame(INT nDelayFrame)
{
	m_nDisappearFrame = nDelayFrame + g_cOrpgWorld.m_dwLogicFrames;
	return TRUE;
}

BOOL KNpc::IsIgnoreDebuff(INT nBuffType)
{
	QCONFIRM_RET_FALSE(nBuffType >= emNoneEffectBuff && nBuffType <= emNiceBuff);

	for(INT i = 0; i < MAX_NPC_IGNORE_DEBUFF; ++i)
	{
		if(m_pTemplate->nIgnoreDebuff[i] == nBuffType)
		{
			return TRUE;
		}
	}

	return FALSE;
}


// -------------------------------------------------------------------------
