/* -------------------------------------------------------------------------
//	文件名		：	KLuaCharacter.cpp
//	创建者		：	simon
//	创建时间	：	2009/11/21 18:29:17
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/knpcmgr.h"
#include "onlinegameworld/kdoodadmgr.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kcharactermgr.h"
#include "onlinegameworld/ksceneobject.h"
#include "../onlinegameworld/scene/kscenemgr.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#ifdef GAME_SERVER
#include "onlinegameworld/kplayermgr.h"
#include "../onlinegamemodule/buffer/buffermanager.h"
#include "../onlinegamemodule/fightskill/fightskillmanager.h"
#include "../onlinegamemodule/fightskill/fightskillmodule.h"
#include "onlinegamemodule/ai/kaictrl.h"
#else
#include "onlinegameworld/kplayermanager_client.h"
#endif // GAME_SERVER
#include "../onlinegamemodule/ai/ThreatList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------


KCharacter* KLuaCharacter::GetHim(QLuaScript& sc, INT nIndex)
{
	KLuaCharacter* pLuaCharacter = dynamic_cast<KLuaCharacter*>(GetCObj(sc.GetLuaState(), nIndex));
	if (pLuaCharacter)
		return pLuaCharacter->m_pMe;

	return NULL;
}

BOOL KLuaCharacter::ClearTempTable()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	return ClearScriptTempTable(cSafeScript->GetLuaState(), s_szClassName, m_pMe->m_dwId);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: Id
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaCharacter::getId(VOID) CONST
{
	return m_pMe->m_dwId;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: Visible
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::getVisible(VOID) CONST
{
	return m_pMe->IsVisible();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: SceneId
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaCharacter::getSceneId(VOID) CONST
{
	DWORD dwSceneId = KD_BAD_ID;
	IKScene* pScene = m_pMe->GetScene();
	if (pScene)
		dwSceneId = pScene->GetId();
	return dwSceneId;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: SceneTemplateId
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaCharacter::getSceneTemplateId(VOID) CONST
{
	DWORD dwId = KD_BAD_ID;
	IKScene* pScene = m_pMe->GetScene();
	if (pScene)
		dwId = pScene->GetTemplateId();
	return dwId;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: FaceDirection
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::getFaceDirection(VOID) CONST
{
	return m_pMe->GetFaceDirection();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: Name
// @Description		: 玩家名称
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaCharacter::getName() CONST
{
	return m_pMe->GetName();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: KinId
// @Description		: 玩家家族Id
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaCharacter::getKinId() CONST
{
	return 0/*m_pMe->GetKinId()*/;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsNpc
// @Description		: 判断一个Character是否是一个Character
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsNpc(QLuaScript& sc)
{
	sc.PushNumber(IS_NPC(m_pMe->GetId()));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsDead
// @Description		: 判断是否死亡
// @ReturnCode		: 1 死亡状态 nil 未死亡
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsDead(QLuaScript& sc)
{
	if (m_pMe->GetMoveState() == cmsOnDeath)
		sc.PushNumber(1);
	else
		sc.PushNull();

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsPlayer
// @Description		: 判断一个Character是否是一个Player
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsPlayer(QLuaScript& sc)
{
	sc.PushNumber(IS_PLAYER(m_pMe->GetId()));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsBZonePlayer
// @Description		: 判断一个Character是否是一个跨服Player
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsBZonePlayer(QLuaScript& sc)
{
	sc.PushNumber(IS_BZONE_PLAYER(m_pMe->GetId()));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetVar
// @Description		: 获取角色变量
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: wVarGroup
// @ArgumentComment	: wVarId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaGetVar(QLuaScript& sc)
{
	QCONFIRM_RET_FALSE(sc.GetTopIndex() >= 2);
	WORD wVarGroup = sc.GetInt(1);
	WORD wVarId = sc.GetInt(2);
	INT dwValue = m_pMe->GetVar(wVarGroup, wVarId);
	sc.PushNumber(dwValue);

	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: SetVar
// @Description		: 设置角色变量
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: wVarGroup
// @ArgumentComment	: wVarId
// @ArgumentComment	: dwVarValue
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaSetVar(QLuaScript &sc)
{
	QCONFIRM_RET_FALSE(sc.GetTopIndex() >= 3);
	WORD wVarGroup = sc.GetInt(1);
	WORD wVarId = sc.GetInt(2);
	DWORD dwVarValue = sc.GetInt(3);
	m_pMe->SetVar(wVarGroup, wVarId, dwVarValue);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: RawSetVar
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: wVarGroup
// @ArgumentComment	: wVarId
// @ArgumentComment	: dwVarValue
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaRawSetVar(QLuaScript &sc)
{
	QCONFIRM_RET_FALSE(sc.GetTopIndex() >= 3);
	WORD wVarGroup = sc.GetInt(1);
	WORD wVarId = sc.GetInt(2);
	DWORD dwVarValue = sc.GetInt(3);
	m_pMe->RawSetVar(wVarGroup, wVarId, dwVarValue);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetPosition
// @Description		: 获取逻辑位置
// @ReturnCode		: { dwSceneId, nX, nY, nZ }
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaCharacter::LuaGetPosition(QLuaScript& sc)
{
	INT nX = 0, nY = 0, nZ = 0;
	IKScene* pScene = m_pMe->GetScene();
	if (pScene)
	{
		DWORD dwSceneId = pScene->GetId();
		m_pMe->GetPosition(nX, nY, nZ);

		sc.PushTable();
		sc.PushNumber(dwSceneId);
		sc.SetTableField("dwSceneId");
		sc.PushNumber(nX);
		sc.SetTableField("nX");
		sc.PushNumber(nY);
		sc.SetTableField("nY");	
		sc.PushNumber(nZ);
		sc.SetTableField("nZ");
		return 1;
	}
	return 0;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetWorldPos
// @Description		: 获取逻辑位置，兼容某些旧脚本
// @ReturnCode		: dwSceneId, nX, nY, nZ
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaCharacter::LuaGetWorldPos(QLuaScript& sc)
{
	INT nX = 0, nY = 0, nZ = 0;
	IKScene* pScene = m_pMe->GetScene();
	if (pScene)
	{
		DWORD dwSceneId = pScene->GetId();
		m_pMe->GetPosition(nX, nY, nZ);

		sc.PushNumber(dwSceneId);
		sc.PushNumber(nX);
		sc.PushNumber(nY);
		sc.PushNumber(nZ);
		return 4;
	}
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetDestination
// @Description		: 获取目标点位置
// @ReturnCode		: { dwSceneId, nX, nY, nZ }
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaCharacter::LuaGetDestination(QLuaScript& sc)
{
	INT nX = 0, nY = 0, nZ = 0;
	IKScene* pScene = m_pMe->GetScene();
	if (pScene)
	{
		DWORD dwSceneId = pScene->GetId();
		m_pMe->GetDestination(nX, nY, nZ);

		sc.PushTable();
		sc.PushNumber(nX);
		sc.SetTableField("nX");
		sc.PushNumber(nY);
		sc.SetTableField("nY");	
		sc.PushNumber(nZ);
		sc.SetTableField("nZ");
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: Jump
// @Description		: 跳跃
// @ReturnCode		: 
// @ArgumentFlag	: [d]
// @ArgumentComment	: 是否同步给他自己（默认为1，用于服务端主动控制玩家移动）
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaCharacter::LuaJump(QLuaScript& sc)
{
	BOOL bSyncSelf = TRUE;
	if (sc.GetTopIndex() >= 1)
		bSyncSelf = sc.GetInt(1);
	m_pMe->Jump(bSyncSelf);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: GoTo
// @Description		: 使角色走向目标点
// @ReturnCode		: 
// @ArgumentFlag	: ddd[d]
// @ArgumentComment	: x
// @ArgumentComment	: y
// @ArgumentComment	: z
// @ArgumentComment	: 是否同步给他自己（默认为0，服务端主动控制玩家移动时用1）
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaCharacter::LuaGoTo(QLuaScript& sc)
{
	BOOL bSyncSelf = TRUE;
	if (sc.GetTopIndex() >= 4)
		bSyncSelf = sc.GetInt(4);
	INT nX = sc.GetInt(1), nY = sc.GetInt(2), nZ = sc.GetInt(3);

	return m_pMe->GoTo(nX, nY, nZ, bSyncSelf);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: StopMove
// @Description		: 停止移动
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaCharacter::LuaStopMove(QLuaScript& sc)
{
	sc.PushNumber(m_pMe->StopMove(TRUE));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: AddExp
// @Description		: 增加经验
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增加经验值
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaCharacter::LuaAddExp(QLuaScript& sc)
{
	INT nExp = sc.GetInt(1);
	INT nBuffExp = 0;
	if(m_pMe->GetTinyTerminal() && nExp > 0)
	{
		//nBuffExp = (nExp * 105) / 100 -  nExp; 
		nBuffExp = (INT)(nExp * 1.05) -  nExp;
	}
	
	if (m_pMe->GetLevel()<=(MAX_PLAYER_LEVEL-1))
	{
		m_pMe->AddExp(nExp);
		if(nBuffExp > 0)
		{
			m_pMe->AddExp(nBuffExp);
		}
	}
	else
	{
	//by ldy
//		DWORD nLvUpNeedExp = g_cCharacterHelper.GetNextLvExp(MAX_PLAYER_LEVEL);
//		if (m_pMe->GetExp() +	nExp < nLvUpNeedExp)//经验卡在升级经验少一点
//		{
//			m_pMe->AddExp(nExp);
//		}
//		else
//		{
//			m_pMe->AddExp(nLvUpNeedExp - m_pMe->GetExp() - 1);
//		}
		
	}
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: ChangeFaceDirection
// @Description		: 改变朝向
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nFaceDirection
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaCharacter::LuaChangeFaceDirection(QLuaScript& sc)
{
	INT nFaceDirection = sc.GetInt(1);	
	return m_pMe->ChangeFaceDirection(nFaceDirection);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetCharacterTempTable
// @Description		: 获得临时table
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaGetCharacterTempTable(QLuaScript& sc)
{
	if (!GetScriptTempTable(sc.GetLuaState(), s_szClassName, m_pMe->m_dwId))
	{
		_ASSERT(FALSE);
		return 0;
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsNearBy
// @Description		: 判断目标是否在Character附近
// @ReturnCode		: 1 在附近 0 不在
// @ArgumentFlag	: d[d]
// @ArgumentComment	: dwSceneObjectId 目标player/npc/doodad的Id
// @ArgumentComment	: nDistance 距离(厘米)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsNearBy(QLuaScript& sc)
{
	KSceneObject* pSceneObj = NULL;
	DWORD dwId = (DWORD)sc.GetInt(1);
	if (IS_NPC(dwId))
		pSceneObj = g_cNpcMgr.GetById(dwId);
	else if (IS_PLAYER(dwId))
		pSceneObj = g_cPlayerMgr.GetById(dwId);
	else if (IS_DOODAD(dwId))
		pSceneObj = g_cDoodadMgr.GetById(dwId);

	if (pSceneObj)
	{
		if (sc.GetTopIndex() >= 2)
		{
			INT nDistance = sc.GetInt(2);
			sc.PushNumber(m_pMe->IsNearby(pSceneObj, nDistance));
		}
		else
		{
			sc.PushNumber(m_pMe->IsNearby(pSceneObj));
		}
	}
	else
	{
		QLogPrintf(LOG_ERR, "[KLuaCharacter::LuaIsNearBy] Invalid SceneObjId 0x%X", dwId);
		sc.PushNumber(0);
	}
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsNearByIn2D
// @Description		: XY平面判断目标是否在Character附近
// @ReturnCode		: 1 在附近 0 不在
// @ArgumentFlag	: dd
// @ArgumentComment	: dwSceneObjectId 目标player/npc/doodad的Id
// @ArgumentComment	: nDistance 距离(厘米)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsNearByIn2D( QLuaScript& sc )
{
	KSceneObject* pSceneObj = NULL;
	DWORD dwId = (DWORD)sc.GetInt(1);
	if (IS_NPC(dwId))
		pSceneObj = g_cNpcMgr.GetById(dwId);
	else if (IS_PLAYER(dwId))
		pSceneObj = g_cPlayerMgr.GetById(dwId);
	else if (IS_DOODAD(dwId))
		pSceneObj = g_cDoodadMgr.GetById(dwId);

	if (pSceneObj)
	{

		INT nDistance = sc.GetInt(2);
		sc.PushNumber(m_pMe->IsNearbyIn2D(pSceneObj, nDistance));
	}
	else
	{
		QLogPrintf(LOG_ERR, "[KLuaCharacter::LuaIsNearByIn2D] Invalid SceneObjId 0x%X", dwId);
		sc.PushNumber(0);
	}
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsInDynamicScene
// @Description		: 判断是否在副本中
// @ReturnCode		: TRUE FALSE
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsInDynamicScene(QLuaScript& sc)
{
	BOOL bInDynamicScene = FALSE;
//#ifdef GAME_SERVER
//	IKScene* pScene = m_pMe->GetScene();
//	if (pScene && IS_DYNAMIC_SCENE(pScene->GetId()))
//		bInDynamicScene = TRUE;
//#else
//	KPlayer* pMe = g_cOrpgWorld.GetClientPlayer();
//	if (m_pMe == pMe)
//		bInDynamicScene = pMe->m_cPlayerClient.DynamicSceneFlag();
//#endif // GAME_SERVER
	sc.PushNumber(bInDynamicScene);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsOnFierce
// @Description		: 判断是否处于狂暴状态
// @ReturnCode		: 1 狂暴 0 正常
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsOnFierce(QLuaScript& sc)
{
	INT nResult = 0;
	/*if (m_pHim->GetFierceStatic() > 0)
	{
		nResult = 1;
	}*/ // zzx
	sc.PushNumber(nResult);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: KilledBy
// @Description		: 直接杀死角色
// @ReturnCode		: 
// @ArgumentFlag	: [o]
// @ArgumentComment	: pKiller
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaKilledBy(QLuaScript& sc)
{
	m_pMe->SetMoveState(cmsOnDeath);
	return 0;
}

#ifdef GAME_SERVER
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: SetVisible
// @Description		: 隐藏角色
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 1显示 0隐藏
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaSetVisible(QLuaScript& sc)
{
	m_pMe->m_cCharacterServer.SetVisible(sc.GetInt(1));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: SetSkillHidden
// @Description		: 隐藏角色
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 1隐身 0显身
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaSetSkillHidden(QLuaScript& sc)
{
	m_pMe->m_cCharacterServer.SetSkillHidden(sc.GetInt(1));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: SetMoveState
// @Description		: 设定角色移动状态
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nState
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaSetMoveState(QLuaScript& sc)
{
	KE_MOVE_STATE nState = (KE_MOVE_STATE)(sc.GetInt(1));
	m_pMe->SetMoveState(nState);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetMoveState
// @Description		: 获取角色移动状态
// @ReturnCode		: d
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaGetMoveState(QLuaScript& sc)
{
	sc.PushNumber((INT)m_pMe->GetMoveState());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: SetFaceDirection
// @Description		: 设定角色朝向
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 朝向0-360
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaSetFaceDirection(QLuaScript& sc)
{
	m_pMe->m_nFaceDirection = sc.GetInt(1);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetNearByNpcInSector
// @Description		: 获取扇形范围内NPC
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: nRadius 半径
// @ArgumentComment	: nAngle 角度（一半）
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaGetNearByNpcInSector(QLuaScript& sc)
{
	INT nRadius = sc.GetInt(1);
	INT nAngle = sc.GetInt(2);
	DWORD pNpcList[20];
	ZeroMemory(pNpcList, sizeof(pNpcList));
	INT nRet = m_pMe->GetNearByNpcInSector(nRadius, nAngle, 20, pNpcList);
	sc.PushTable();
	for (INT i = 0; i < nRet; i++)
	{
		sc.PushNumber(pNpcList[i]);
		sc.SetTableIndex(i + 1);
	}
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetNearByPlayerInSector
// @Description		: 获取扇形范围内玩家
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: nRadius 半径
// @ArgumentComment	: nAngle 角度（一半）
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaGetNearByPlayerInSector(QLuaScript& sc)
{
	INT nRadius = sc.GetInt(1);
	INT nAngle = sc.GetInt(2);
	DWORD pdwPlayerList[20];
	ZeroMemory(pdwPlayerList, sizeof(pdwPlayerList));
	INT nRet = m_pMe->GetNearByPlayerInSector(nRadius, nAngle, 20, pdwPlayerList);
	sc.PushTable();
	for (INT i = 0; i < nRet; i++)
	{
		sc.PushNumber(pdwPlayerList[i]);
		sc.SetTableIndex(i + 1);
	}
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetFrontPosition
// @Description		: 获取目标前方距离X的点
// @ReturnCode		: 
// @ArgumentFlag	: d[d]
// @ArgumentComment	: nDistance 距离(单位m)
// @ArgumentComment	: nDirection 方向
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaGetFrontPosition(QLuaScript& sc)
{
	INT nDistance = sc.GetInt(1);
	INT nDirection = m_pMe->GetFaceDirection();
	if(sc.IsNumber(2))
	{
		nDirection = sc.GetInt(2);
	}
	nDistance *= CELL_LENGTH * 2;
	INT nX = 0, nY = 0, nZ = 0;
	m_pMe->GetFrontPosition(nDistance, nDirection, nX, nY, nZ);
	sc.PushTable();
	sc.PushNumber(nX);
	sc.SetTableField("nX");
	sc.PushNumber(nY);
	sc.SetTableField("nY");
	sc.PushNumber(nZ);
	sc.SetTableField("nZ");

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetDirection
// @Description		: 获取目标与（nX,nY）的点的距离
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: nX
// @ArgumentComment	: nY
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaGetDirection(QLuaScript& sc)
{
	INT nMeX = 0, nMeY = 0, nMeZ = 0;
	INT nHimX = sc.GetInt(1);
	INT nHimY = sc.GetInt(2);
	m_pMe->GetPosition(nMeX, nMeY, nMeZ);
	INT nDirection = g_GetDirection(nHimX, nHimY, nMeX, nMeY);
	sc.PushNumber(nDirection);
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: AddSkillTest
// @Description		: 加技能，测试
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nSkillId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaAddSkillTest(QLuaScript& sc)
{
	INT nSkillId = sc.GetInt(1);
	g_FightSkillManager.AddSkill(m_pMe->GetId(),nSkillId);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaCharacter
// @LuaApiName		: CheckTrigger
// @Description		: 加BUFF，测试
// @ReturnCode		: 
// @ArgumentFlag	: s[oo]
// @ArgumentComment	: szEvent
// @ArgumentComment	: EventSender
// @ArgumentComment	: EventReceive
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaCheckTrigger(QLuaScript& sc)
{
	KCharacter* pEventSender = NULL;
	KCharacter* pEventReceive = NULL;
	KLuaCharacter* pLuaCharacter = NULL;
	LPCSTR szEvent = sc.GetStr(1);

	if(sc.GetTopIndex() >= 2)
	{
		pLuaCharacter = dynamic_cast<KLuaCharacter*>(sc.GetObj(2));
		QCONFIRM_RET_NULL(pLuaCharacter);
		pEventSender = pLuaCharacter->m_pMe;
	}
	if(sc.GetTopIndex() >= 3)
	{
		pLuaCharacter = dynamic_cast<KLuaCharacter*>(sc.GetObj(3));
		QCONFIRM_RET_NULL(pLuaCharacter);
		pEventReceive = pLuaCharacter->m_pMe;
	}
	
	g_cBufferMgr.CheckCharacterBuffTrigger(m_pMe->GetId(), szEvent, pEventSender, pEventReceive);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsInRecRange
// @Description		: 是否在区域范围内
// @ReturnCode		: bool
// @ArgumentFlag	: dddd
// @ArgumentComment	: 最小X
// @ArgumentComment	: 最大X
// @ArgumentComment	: 最小Y
// @ArgumentComment	: 最大Y
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsInRecRange(QLuaScript &sc)
{
	INT nXmin = sc.GetInt(1);
	INT nXmax = sc.GetInt(2);
	INT nYmin = sc.GetInt(3);
	INT nYmax = sc.GetInt(4);

	BOOL bAnswer = false;
	INT nXAim = 0;
	INT nYAim = 0;
	INT nZAim = 0;

	m_pMe->GetPosition(nXAim, nYAim, nZAim);

	bAnswer = g_InRecRange(nXmin, nXmax, nYmin, nYmax, nXAim, nYAim);
	sc.PushNumber(bAnswer);

	return 1;
//EXIT0:
//	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsInCircle
// @Description		: 是否在区域范围内
// @ReturnCode		: bool
// @ArgumentFlag	: dddd
// @ArgumentComment	: 区域中心X
// @ArgumentComment	: 区域中心Y
// @ArgumentComment	: 区域中心Z
// @ArgumentComment	: 区域半径R
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsInCircle(QLuaScript &sc)
{
	INT nX = sc.GetInt(1);
	INT nY = sc.GetInt(2);
	INT nZ = sc.GetInt(3);
	INT nR = sc.GetInt(3);
	nR *= CELL_LENGTH * 2;

	BOOL bAnswer = false;
	INT nXAim = 0;
	INT nYAim = 0;
	INT nZAim = 0;

	m_pMe->GetPosition(nXAim, nYAim, nZAim);

	bAnswer = g_InRange(nXAim, nYAim, nZAim, nX, nY, nZ, nR);
	sc.PushNumber(bAnswer);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsBack
// @Description		: 是否在区域范围内
// @ReturnCode		: bool
// @ArgumentFlag	: d
// @ArgumentComment	: AimID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaIsBack(QLuaScript &sc)
{
	INT nAimID = sc.GetInt(1);

	BOOL bAnswer = false;
	KCharacter* pAim = 0;
	INT nXAim = 0;
	INT nYAim = 0;
	INT nZAim = 0;
	INT nXSrc = 0;
	INT nYSrc = 0;
	INT nZSrc = 0;

	INT nMeDirection = m_pMe->GetFaceDirection();
	INT nToAimDirection = 0;
	INT nDeltaAngle = 0;
	BOOL isback = false;

	pAim = g_cCharacterMgr.GetById(nAimID);
	LOG_PROCESS_ERROR(pAim);

	m_pMe->GetPosition(nXSrc, nYSrc, nZSrc);
	pAim->GetPosition(nXAim, nYAim, nZAim);

	nToAimDirection = g_GetDirection(nXSrc, nYSrc, nXAim, nYAim);
	nDeltaAngle = abs(nMeDirection - nToAimDirection);

	//[0, 90) || (270,360)在身后
	if( (0 <= nDeltaAngle && nDeltaAngle < 90) || (270 < nDeltaAngle || nDeltaAngle < 360) )
	{
		isback = true;
	}

	sc.PushNumber(isback);

	return 1;
EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetEnemyInDis
// @Description		: 返回区域范围内敌人列表
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nRadius
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaGetEnemyInDis(QLuaScript &sc)
{
	KCharacter* pCharacter = m_pMe;
	QCONFIRM_RET_FALSE(pCharacter);

	INT nRadius = sc.GetInt(1);
	QCONFIRM_RET_FALSE(nRadius > 0);
	nRadius = nRadius * CELL_LENGTH * 2;

	KScene* pScene = (KScene*)g_pSceneMgr->GetScene(pCharacter->GetSceneId());
	QCONFIRM_RET_FALSE(pScene);

	INT nMapId = pScene->GetTemplateId();
	CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(nMapId);
	QCONFIRM_RET_NULL(pCfg);
	// 非战斗地图
	if(!pCfg->bFightMap)
	{
		return 0;
	}

	INT nPosX = 0;
	INT nPosY = 0;
	INT nPosZ = 0;
	pCharacter->GetPosition(nPosX, nPosY, nPosZ);

	TKTraverseHelper<KCharacter> m_cTravHelper;
	pScene->TraverseNearbyPlayers(m_cTravHelper, nPosX, nPosY);
	pScene->TraverseNearbyNpcs(m_cTravHelper, nPosX, nPosY);

	INT nIdx = 1;
	sc.PushTable();
	const std::vector<KCharacter*>& rtRegionList = m_cTravHelper.m_vObjList;

	for (std::vector<KCharacter*>::const_iterator iter = rtRegionList.begin();
		iter != rtRegionList.end();
		++iter)
	{
		KCharacter* pTarget = *iter;
		QCONFIRM_RET_FALSE(pTarget);
		if(pTarget != pCharacter && pTarget->GetCurrentCamp() != 0 && pTarget->GetCurrentCamp() != pCharacter->GetCurrentCamp())
		{
			INT nX, nY, nZ;
			pTarget->GetPosition(nX,nY,nZ);
			INT nDistance = g_GetDistance2(nX, nY, nPosX, nPosY);

			if (nRadius * nRadius >= nDistance)
			{
				sc.PushNumber(pTarget->GetId());
				sc.SetTableIndex(nIdx++);
			}
		}
	}

	return 1;
}


#endif // GAME_SERVER

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: CopyAITarget
// @Description		: 复制AI目标
// @ReturnCode		: 1成功 0失败
// @ArgumentFlag	: dd
// @ArgumentComment	: dwDestId
// @ArgumentComment	: nTargetType
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaCopyAITarget(QLuaScript &sc)
{
	BOOL        bResult         = false;
	DWORD       dwDstID         = 0;
	int         nTargetType     = aittInvalid;
	KCharacter* pDstCharacter   = NULL;

	dwDstID     = (DWORD)sc.GetInt(1);
	nTargetType = (int)sc.GetInt(2);
	LOG_PROCESS_ERROR(dwDstID != m_pMe->m_dwId);
	LOG_PROCESS_ERROR(IS_NPC(dwDstID));
	LOG_PROCESS_ERROR(nTargetType > aittInvalid && nTargetType < aittTotal);

	pDstCharacter = g_cNpcMgr.GetById(dwDstID);
	LOG_PROCESS_ERROR(pDstCharacter);
	PROCESS_ERROR(pDstCharacter->m_pScene);

	switch (nTargetType)
	{
	case aittSelectTarget:
		pDstCharacter->m_SelectTarget.SetTarget(m_pMe->m_SelectTarget.m_eTargetType, m_pMe->m_SelectTarget.m_dwID);
		break;
	case aittSkillTarget:
		pDstCharacter->m_SelectTarget.SetTarget(m_pMe->m_SkillTarget.m_eTargetType, m_pMe->m_SkillTarget.m_dwID);
		break;
	default:
		LOG_PROCESS_ERROR(nTargetType >= 0);
		pDstCharacter->m_AIData.ChoiceTarget[nTargetType].SetTarget(
			m_pMe->m_AIData.ChoiceTarget[nTargetType].m_eTargetType, 
			m_pMe->m_AIData.ChoiceTarget[nTargetType].m_dwID
			);
		break;
	}

	bResult = true;
EXIT0:
	sc.PushNumber(bResult);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: GetTarget
// @Description		: 获取目标
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaGetTarget(QLuaScript &sc)
{
	BOOL            bRetCode        = false;
	TARGET_TYPE     eTargetType     = ttNoTarget;
	KSceneObject*   pSceneObject    = NULL;
	DWORD           dwTargetID      = 0;

	bRetCode = m_pMe->m_SelectTarget.GetTarget(&pSceneObject);
	PROCESS_ERROR(bRetCode);

	PROCESS_ERROR(pSceneObject && pSceneObject->m_pScene);

	eTargetType = m_pMe->m_SelectTarget.GetTargetType();
	dwTargetID  = pSceneObject->m_dwId;

EXIT0:
	sc.PushNumber(eTargetType);
	sc.PushNumber(dwTargetID);
	return 2;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: CalcFightScore
// @Description		: 重新计算角色战斗力
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaCalcFightScore(QLuaScript &sc)
{
	g_cCharacterHelper.CalcFightScore(*m_pMe);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: SetTarget
// @Description		: 设置目标
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaSetTarget(QLuaScript &sc)
{
	BOOL        bRetCode        = false;
	DWORD       dwID            = 0;
	TARGET_TYPE eTargetType     = ttInvalid;

	eTargetType = (TARGET_TYPE)sc.GetInt(1);
	dwID = (DWORD)sc.GetInt(2);

	PROCESS_ERROR(eTargetType != m_pMe->m_SelectTarget.m_eTargetType || dwID != m_pMe->m_SelectTarget.m_dwID);

	if (m_pMe->m_SelectTarget.m_eTargetType == ttNoTarget && eTargetType == ttNoTarget)
	{
		goto EXIT0;
	}

	bRetCode = m_pMe->SelectTarget(eTargetType, dwID);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: ModifyThreat
// @Description		: 设置威胁值
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: dwCharacterId
// @ArgumentComment	: nThreat
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaCharacter::LuaModifyThreat(QLuaScript &sc)
{
	BOOL        bResult         = false;
	BOOL        bRetCode        = false;
	KCharacter* pCharacter      = NULL;
	DWORD       dwCharacterID   = 0;
	int         nThreat         = 0;

	dwCharacterID = sc.GetInt(1);
	nThreat = sc.GetInt(2);

	pCharacter = g_cCharacterMgr.GetById(dwCharacterID);
	PROCESS_ERROR(pCharacter);
	PROCESS_ERROR(pCharacter->m_pScene);

	bRetCode = m_pMe->m_SimpThreatList.ModifyThreat(thtMainThreat, pCharacter, nThreat);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	sc.PushNumber(bResult);
	return 1;
}

#include "kluacharacter_table.hpp"
