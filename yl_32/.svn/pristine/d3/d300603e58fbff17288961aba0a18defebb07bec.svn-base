
#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/koverloadextension.h"
#include "onlinegamemodule/ai/kaivm.h" // TODO

#include "onlinegamemodule/findpath/kplayerfindpath_i.h"

#ifdef GAME_SERVER
#include "onlinegameworld/kplayermgr.h"
#include "kplayerbaseprocess.h"
#endif // GAME_SERVER

#ifdef GAME_CLIENT
#include "onlinegameworld/kplayermanager_client.h"
#include "../onlinegamemodule/kplayerteammanager_client.h"
#include "kplayerbaseprocess_client.h"
#endif // GAME_CLIENT

#include "onlinegameworld/knpcmgr.h"
#include "onlinegameworld/kdoodadmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -----------------------------------------
#pragma warning(disable : 4355)
#ifdef GAME_SERVER
KPlayer::KPlayer() : KCharacter(m_cPlayerServer, KD_PLAYER_ID_MASK, emKPLAYER_TASK_MAX), m_cPlayerServer(*this)
#else
KPlayer::KPlayer() : KCharacter(m_cPlayerClient, KD_PLAYER_ID_MASK, emKPLAYER_TASK_MAX), m_cPlayerClient(*this)
#endif
{
	m_pLuna = new KLuaPlayer(this);
	m_cCtrlSet.CreateCtrls(*this, KD_PLAYER_ID_MASK);
	m_eGameState = emPLAYERSTATE_BEGIN;

#ifdef GAME_SERVER
	m_pDoodad = NULL;
#endif

	SetDelayTalkObjId(0);
}
#pragma warning(default : 4355)

KPlayer::~KPlayer()
{
	SAFE_DELETE(m_pLuna);
}

BOOL KPlayer::Init(INT nPlayerIndex, const QString& strName)
{
	m_eGameState = emPLAYERSTATE_BEGIN;

	m_nIndex = nPlayerIndex;
	m_strName = strName;
	memset(&m_sMoveCtrl, 0, sizeof(m_sMoveCtrl));

	m_dwTeamId = KD_BAD_ID;
	m_dwSystemTeamId = KD_BAD_ID;
	m_dwKinId = KD_BAD_ID;
	m_dwKinMasterId = KD_BAD_ID;
	m_byIsKinMaster = KD_BAD_ID;
	m_nTimer = 0;
	m_nThreatTime = PLAYER_THREAT_TIME;

	BOOL bRet = KCharacter::Init();
	QCONFIRM_RET_FALSE(bRet);

	m_mapDoodadIds.clear();

//	m_cCtrlSet.InitCtrls();

	SetDelayTalkObjId(0);

#ifdef GAME_SERVER
	m_cPlayerServer.m_nBanishTime = 0;
	m_cPlayerServer.m_pDelayedTeleport = NULL;
	m_cPlayerServer.m_bOnThisGs = FALSE;
#endif // GAME_SERVER
	return TRUE;
}

BOOL KPlayer::Uninit()
{
	BOOL bRet = KCharacter::Uninit();
	QCONFIRM(bRet);
	m_eGameState = emPLAYERSTATE_BEGIN;
	m_nIndex = 0;

//	m_cCtrlSet.UnInitCtrls();

	SetDelayTalkObjId(0);

	return TRUE;
}

// 临时方案
#ifdef GAME_CLIENT
BOOL KPlayer::ClientGoTo(INT nX, INT nY, INT nZ)
{
	//玩家在指定的逻辑帧外改变目标，取消延迟场景对象会话
	this->OnTargetChange();

	if (!CanMove())
		return FALSE;

	if ( ! m_cPlayerClient.DoGoTo(nX, nY, nZ, m_nMoveMode))
		return FALSE; 	// 被协议禁止

	//m_nMoveSpeed = GetMoveSpeed();

	GoTo(nX, nY, nZ);

	return TRUE;
}

BOOL KPlayer::ClientJump()
{
	//玩家在指定的逻辑帧外改变目标，取消延迟场景对象会话
	this->OnTargetChange();

	if (!CanMove())
		return FALSE;

	if ( ! m_cPlayerClient.DoJump())
		return FALSE; 	// 被协议禁止

	KSceneObject::Jump();

	return TRUE;
}
#endif // GAME_CLIENT

#ifdef GAME_SERVER
BOOL KPlayer::LogicGoTo(INT nX, INT nY, INT nZ)
{
	QCONFIRM_RET_FALSE(m_eMoveState != cmsOnDeath);
	QCONFIRM_RET_FALSE(m_eMoveState != cmsOnSkill);

	KSceneObject::GoTo(nX, nY, nZ);

	if (!m_nMoveMode)
	{
		SetMoveState(cmsOnWalk);
		m_nVelocityXY = KD_BASE_WALK_SPEED * VELOCITY_ZOOM_COEFFICIENT;
	}

	this->OnTargetChange();

	FireEvent(emKOBJEVENTTYPE_CHARACTER_MOVE, m_dwId, 0, 0);
	return TRUE;
}
#endif // GAME_SERVER

BOOL KPlayer::GoTo( INT nX, INT nY, INT nZ, BOOL bSyncSelf /*= FALSE*/ )
{
	QCONFIRM_RET_FALSE(m_eMoveState != cmsOnDeath);
	QCONFIRM_RET_FALSE(m_eMoveState != cmsOnSkill);

	if (GetIsFix() > 0)
	{
		if (GetIsFix() == 2 && m_eMoveState == cmsOnHalt)
		{
            //QLogPrintf(LOG_INFO, "正式定身");
			// 定身，2变1
			SetIsFix(1);
		}
		else if (GetIsFix() == 1)
		{
			// 定身中，返回
			QLogPrintf(LOG_INFO, "角色定身中不能移动");
			return FALSE;
		}
	}
	if (GetIsStun() > 0)
	{
		if (GetIsStun() == 2 && m_eMoveState == cmsOnHalt)
		{
			// 定身，2变1
			SetIsStun(1);
		}
		else if (GetIsStun() == 1)
		{
			// 定身中，返回
			QLogPrintf(LOG_INFO, "角色晕眩中不能移动");
			return FALSE;
		}
	}

	KSceneObject::GoTo(nX, nY, nZ);

	if (!m_nMoveMode)
	{
		SetMoveState(cmsOnWalk);
		m_nVelocityXY = KD_BASE_WALK_SPEED * VELOCITY_ZOOM_COEFFICIENT;
	}

	//玩家在指定的逻辑帧外改变目标，取消延迟场景对象会话
	this->OnTargetChange();

#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncMove(this, bSyncSelf);
#endif // GAME_SERVER
	FireEvent(emKOBJEVENTTYPE_CHARACTER_MOVE, m_dwId, 0, 0);
	return TRUE;
}

BOOL KPlayer::KnockTo(INT nX, INT nY, INT nZ, INT nDirection)
{
	QCONFIRM_RET_FALSE(m_eMoveState != cmsOnDeath);

	KSceneObject::KnockTo(nX, nY, nZ, nDirection);
	/*
	if (!m_nMoveMode)
	{
		SetMoveState(emKMOVESTATE_KNOCK);
		m_nVelocityXY = nSpeed * VELOCITY_ZOOM_COEFFICIENT;
	}
	*/

	//玩家在指定的逻辑帧外改变目标，取消延迟场景对象会话
	this->OnTargetChange();

#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncMove(this, FALSE);
#endif // GAME_SERVER
	FireEvent(emKOBJEVENTTYPE_CHARACTER_KNOCK, m_dwId, 0, 0);
	return TRUE;
}

BOOL KPlayer::Pull(INT nX, INT nY, INT nZ, INT nDirection)
{
	QCONFIRM_RET_FALSE(m_eMoveState != cmsOnDeath);

	KSceneObject::Pull(nX, nY, nZ, nDirection);

	//玩家在指定的逻辑帧外改变目标，取消延迟场景对象会话
	this->OnTargetChange();

#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncMove(this, FALSE);
#endif // GAME_SERVER
	FireEvent(emKOBJEVENTTYPE_CHARACTER_PULL, m_dwId, 0, 0);
	return TRUE;
}



BOOL KPlayer::StopMove(BOOL bSyncSelf /* = FALSE*/)
{
	KSceneObject::StopMoving();
#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncMove(this, bSyncSelf);
#endif // GAME_SERVER
	return TRUE;
}

BOOL KPlayer::Jump(BOOL bSyncSelf/* = FALSE*/)
{
	//玩家在指定的逻辑帧外改变目标，取消延迟场景对象会话
	this->OnTargetChange();

	KSceneObject::Jump();

#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncJump(this, bSyncSelf);
#endif // GAME_SERVER

	return TRUE;
}

BOOL KPlayer::JumpTo(INT nX, INT nY, INT nZ)
{
	//玩家在指定的逻辑帧外改变目标，取消延迟场景对象会话
	this->OnTargetChange();

	KSceneObject::JumpTo(nX, nY, nZ);

#ifdef GAME_SERVER
	g_cPlayerBaseProcess.DoSyncJumpTo(this, nX, nY, nZ);
#endif // GAME_SERVER

	return TRUE;
}

BOOL KPlayer::ChangeRegion(KRegion* pDestRegion)
{
	// 在瞬移切换region的时候，必须马上同步给目标region的玩家
	KSceneObject::ChangeRegion(pDestRegion);

	m_cPlayerServer.SyncChangeRegion();   // 同步新region的所有角色给自己

	return TRUE;
}

BOOL KPlayer::Activate()
{
	if ( ! KCharacter::Activate())
		return FALSE;  // 可能这帧已经激活过
	QCONFIRM_RET_FALSE(GetScene());

#ifdef GAME_SERVER
	m_cPlayerServer.RecordPosition();
	g_cPlayerBaseProcess.DoSyncFrame(this);
	QCONFIRM_RET_FALSE(GetScene());
#endif // GAME_SERVER

	MoveCtrl();
	QCONFIRM_RET_FALSE(GetScene());

	KCharacter::ProcessMove(); // 注意激活顺序不要搞乱
	QCONFIRM_RET_FALSE(GetScene());

#ifdef GAME_SERVER
	//m_cPlayerServer.SyncChangeRegion(); 
	//m_cCtrlSet.ActiveCtrls();

	// 放在最后，传送回造成离开场景
	if (m_cPlayerServer.m_pDelayedTeleport)
	{
		KROLE_POSITION sPos = *m_cPlayerServer.m_pDelayedTeleport;

		m_cPlayerServer.m_pDelayedTeleport = NULL;

		m_cPlayerServer.SaveLastEntry();

		m_cPlayerServer.TeleportTo2(sPos.dwMapID, sPos.nMapCopyIndex, sPos.nX, sPos.nY, sPos.nZ);
	}
#endif // GAME_SERVER
	return TRUE;
}

BOOL KPlayer::ForwardPosition(int nFrame)
{
	BOOL bResult = FALSE;

	//printf("ForwardFrames[%d+%d]\n", m_dwVirtualFrame, nFrame);

	while (nFrame-- > 0)
	{
		//RunStateMachine();
		//PROCESS_ERROR(m_pScene);

		ProcessMove();        
		PROCESS_ERROR(GetScene());

		MoveCtrl();

#ifdef GAME_SERVER
		m_cPlayerServer.m_dwVirtualFrame++;
#endif
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayer::OnRemove()
{	
	return g_cPlayerMgr.RemoveById(m_dwId);
}

BOOL KPlayer::OnTrap( LPCSTR pszTrapScript )
{
	QCONFIRM_RET_FALSE(pszTrapScript);

	IKScene* pScene = GetScene();
	QCONFIRM_RET_FALSE(pScene);
	LPCSTR pszSceneClass = pScene->GetClass();
	if (pszSceneClass && pszTrapScript)
	{
		KCharacter::OnTrap(pszTrapScript);

		ScriptSafe cJxScriptSafe = g_cScriptManager.GetSafeScript();
		QLunaBase* pOldMe = g_cScriptManager.SetMe(m_pLuna);
		cJxScriptSafe->CallTableFunction("Scene", "OnTrap", 0, "ss", pszSceneClass, pszTrapScript);
		g_cScriptManager.SetMe(pOldMe);
	}
	return TRUE;
}

BOOL KPlayer::OnLeaveTrap( LPCSTR pszTrapScript )
{
	QCONFIRM_RET_FALSE(pszTrapScript);

	IKScene* pScene = GetScene();
	QCONFIRM_RET_FALSE(pScene);
	LPCSTR pszSceneClass = pScene->GetClass();
	if (pszSceneClass && pszTrapScript)
	{
		KCharacter::OnLeaveTrap(pszTrapScript);

		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
#ifdef GAME_SERVER
		QLunaBase* pOldMe = g_cScriptManager.SetMe(m_pLuna);
		cSafeScript->CallTableFunction("Scene", "OnLeaveTrap", 0, "ss", pszSceneClass, pszTrapScript);
		g_cScriptManager.SetMe(pOldMe);
#else
		cSafeScript->CallTableFunction("Scene", "OnLeaveTrap", 0, "ss", pszSceneClass, pszTrapScript);
#endif // GAME_SERVER
	}
	return TRUE;
}

void KPlayer::MoveCtrl()
{
	PROCESS_ERROR(m_pScene);

	if (m_sMoveCtrl.bMove)
	{
		INT nCurrentX = 0, nCurrentY = 0, nCurrentZ = 0;
		GetPosition(nCurrentX, nCurrentY, nCurrentZ);
		int nMoveDirection = m_sMoveCtrl.nDirection;
		int nDestX         = nCurrentX + MOVE_DEST_RANGE * g_Cos(nMoveDirection) / SIN_COS_NUMBER;
		int nDestY         = nCurrentY + MOVE_DEST_RANGE * g_Sin(nMoveDirection) / SIN_COS_NUMBER;

		GoTo(nDestX, nDestY, nCurrentZ);
	}

EXIT0:
	return;
}

VOID KPlayer::SetDelayTalkObjId( DWORD dwDelayTalkNpcId )
{
	m_dwDelayTalkObjId = dwDelayTalkNpcId;
	if (dwDelayTalkNpcId > 0)
	{
		m_dwSetDelayTalkObjIdFrame = g_cOrpgWorld.m_dwLogicFrames;
	}
	else
	{
		m_dwSetDelayTalkObjIdFrame = 0;
	}
}

BOOL KPlayer::ApplyNpcTalk( DWORD dwNpcId )
{
	//首先清除玩家身上已有的打开Obj对话框标记
	this->SetDelayTalkObjId(0);

	INT nHaveTalk = 0;  
	KNpc* pNpc = NULL;
	pNpc = g_cNpcMgr.GetById(dwNpcId);
	if (!pNpc)   
		return FALSE;
	if (pNpc->GetRelation() == 1)   // 1表示敌对关系 
		return FALSE;

	//如果会话对象距离自己过远，则设置走路，本次会话中止，延迟进行
	// 为了流畅性，禁掉距离判断
	/*if (this->CheckAndSetDelayOpen((KSceneObject*)pNpc, KD_MIN_SCENCE_OBJ_TALK_DISTANCE))
	{
		return TRUE;
	}*/

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
#ifdef GAME_SERVER
	QLunaBase* pOldHim = g_cScriptManager.SetHim((QLunaBase*)pNpc->GetScriptInterface());
	QLunaBase* pOldMe = g_cScriptManager.SetMe((QLunaBase*)this->GetScriptInterface());
#endif

	// 回调脚本，让脚本主动CALL LuaSayDialog;
	cSafeScript->CallTableFunction("Npc", "OnDialog", 1, "so", pNpc->GetClassName().CStr(), pNpc->GetScriptInterface());
	nHaveTalk = cSafeScript->GetInt(-1);

#ifdef GAME_SERVER
	pNpc->m_AIVM.FireEvent(aevOnDialog, m_dwId, 0);

	g_cScriptManager.SetMe(pOldMe);
	g_cScriptManager.SetHim(pOldHim);
#else
	QCONFIRM_RET_FALSE(g_pBaseSender);
	// 如果有客户端对话就不发服务端
	if (!nHaveTalk)
		return m_cPlayerClient.DoApplyNpcTalk(*this, dwNpcId);
#endif
	return TRUE;
}

BOOL KPlayer::CheckAndSetDelayOpen( KSceneObject* pScenceObj, INT32 nDistanceLimit /*= KD_MIN_SCENCE_OBJ_TALK_DISTANCE*/ )
{
	QCONFIRM_RET_FALSE(pScenceObj);
	QCONFIRM_RET_FALSE(nDistanceLimit > 0);

	const INT32 nDistance2Limit = nDistanceLimit * nDistanceLimit;
	//会话距离
	const INT32 N_TALK_DISTANCE = nDistanceLimit;

	//开始Obj对话之前检查自己与Obj的距离，距离超过限制则走过去再对话
	INT nObjX = 0;
	INT nObjY = 0;
	INT nObjZ = 0;
	pScenceObj->GetPosition(nObjX, nObjY, nObjZ);
	INT nPlayerX = 0;
	INT nPlayerY = 0;
	INT nPlayerZ = 0;
	this->GetPosition(nPlayerX, nPlayerY, nPlayerZ);
	INT32 nDistance2 = g_GetDistance2(nPlayerX, nPlayerY, nObjX, nObjY);

	//首先清除玩家身上已有的打开Obj对话框标记
	this->SetDelayTalkObjId(0);
	if ( nDistance2 > nDistance2Limit)
	{
		//以下通过定比分点坐标公式计算目标点
		FLOAT fDistance = (float)sqrt((float)nDistance2);
		INT32 nDelta = N_TALK_DISTANCE / 2;//走到距离检查返回近一些的位置

		QCONFIRM_RET_FALSE(N_TALK_DISTANCE > nDelta);//防止N_TALK_DISTANCE设置得过小
		QCONFIRM_RET_FALSE((fDistance - N_TALK_DISTANCE + nDelta) > 0);
		FLOAT fNamda = (FLOAT)(N_TALK_DISTANCE - nDelta) / (fDistance - N_TALK_DISTANCE + nDelta);

		INT32 nDestX = (INT32)((nObjX + fNamda * nPlayerX) / (1 + fNamda));
		INT32 nDestY = (INT32)((nObjY + fNamda * nPlayerY) / (1 + fNamda));
		INT32 nDestZ = MAX(nObjZ, nPlayerZ);

#ifdef GAME_CLIENT
		//设置延迟触发对象后 走近Obj
		this->SetDelayTalkObjId(pScenceObj->GetId());
		this->ClientGoTo(nDestX, nDestY, nDestZ);
#endif

#ifdef GAME_SERVER

		//设置延迟触发对象后 走近Obj
		// 临时改成导航接口
		this->SetDelayTalkObjId(pScenceObj->GetId());
		if (this->m_pScene != NULL)
		{
			IKPlayerFindPath* pFindPath = GetPlayerFindPath(this);
            QCONFIRM_RET_FALSE(pFindPath);
			pFindPath->GoTo(this->GetScene()->GetTemplateId(), nDestX, nDestY, nDestZ);
		}
		else
		{
		    this->GoTo(nDestX, nDestY, nDestZ, TRUE);
		}
#endif
		return TRUE;
	}

	return FALSE;
}

BOOL KPlayer::ApplyOpenDoodad( DWORD dwDoodadId )
{
	DWORD dwDialogRange = 0;

	//首先清除玩家身上已有的打开Obj对话框标记
	this->SetDelayTalkObjId(0);
	//如果会话对象距离自己过远，则设置走路，本次会话中止，延迟进行
	KDoodad* pDoodad = g_cDoodadMgr.GetById(dwDoodadId);
	QCONFIRM_RET_FALSE(pDoodad);

	//dwDialogRange = pDoodad->GetDialogRange();
	dwDialogRange = dwDialogRange == 0 ? DOODAD_TALK_DISTANCE : dwDialogRange;

	if (this->CheckAndSetDelayOpen((KSceneObject*)pDoodad, dwDialogRange))
	{
		return TRUE;
	}

#ifdef GAME_CLIENT
	return m_cPlayerClient.DoDialogDoodad(dwDoodadId);
#endif

#ifdef GAME_SERVER
	return pDoodad->DoDialog(this);
#endif

	return FALSE;
}

BOOL KPlayer::OnPlayerMoveStop()
{
	//有延迟会话信息
	DWORD dwDelayTalkObjId = this->GetDelayTalkObjId();
	if ( dwDelayTalkObjId > 0)
	{
		if (IS_NPC(dwDelayTalkObjId))
		{
			this->ApplyNpcTalk(dwDelayTalkObjId);
		} 
		else if (IS_DOODAD(dwDelayTalkObjId))
		{
			this->ApplyOpenDoodad(dwDelayTalkObjId);
		} 
	}

	return FALSE;
}

BOOL KPlayer::OnTargetChange()
{
	//玩家在设置DelayTalkNpcId的逻辑帧外点击事件，取消延迟跟随
	if (this->GetSetDelayTalkObjIdFrame() != g_cOrpgWorld.m_dwLogicFrames
		&& this->GetDelayTalkObjId() > 0)
	{
//#ifdef _DEBUG
//		printf("[DEBUG INFO]KPlayer::OnTargetChange(Player[%08X])，取消延迟场景对象[%08X]会话\n"
//			, this->GetId(),  this->GetDelayTalkObjId());
//#endif
		this->SetDelayTalkObjId(0);
		return TRUE;
	}

	return FALSE;
}

BOOL KPlayer::OnReachTarget()
{
	KCharacter::OnReachTarget();
	this->OnPlayerMoveStop();
	return TRUE;
}

BOOL KPlayer::CanMove()
{
	BOOL bResult = FALSE;

	PROCESS_ERROR(KCharacter::CanMove());
	//PROCESS_ERROR(!IsDisableOP(emKPLOS_OP_DOODAD_STATE));		// 操作Doodad时，禁止移动

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KPlayer::SetTrapInvalid(INT nTemplateId)
{
	m_mapTrapInvalid[nTemplateId] = 1;
}


VOID KPlayer::RemoveTrapInvalid(INT nTemplateId)
{
	std::map<INT, INT>::iterator it = m_mapTrapInvalid.find(nTemplateId);
	if(it != m_mapTrapInvalid.end())
	{
		m_mapTrapInvalid.erase(it);
	}
}

BOOL KPlayer::IsTrapInvalid(INT nTemplateId)
{
	std::map<INT, INT>::iterator it = m_mapTrapInvalid.find(nTemplateId);
	if(it != m_mapTrapInvalid.end())
	{
		return TRUE;
	}
	return FALSE;
}

VOID KPlayer::AddDropDoodadId(DWORD dwDoodadId)
{
	m_mapDoodadIds[dwDoodadId] = 1;
}

VOID KPlayer::RemoveDropDoodadId(DWORD dwDoodadId)
{
	std::map<DWORD, INT>::iterator it = m_mapDoodadIds.find(dwDoodadId);
	if(it != m_mapDoodadIds.end())
	{
		m_mapDoodadIds.erase(it);
	}
}

VOID KPlayer::PickUpAllDoodad()
{
	for(std::map<DWORD, INT>::iterator it = m_mapDoodadIds.begin(); it != m_mapDoodadIds.end(); ++it)
	{
		g_cPlayerBaseProcess.DoPickUpDoodad(this, it->first);
	}
	m_mapDoodadIds.clear();
}