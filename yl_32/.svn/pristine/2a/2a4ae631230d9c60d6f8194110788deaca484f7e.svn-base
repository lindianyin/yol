/* -------------------------------------------------------------------------
//	文件名		：	kluanpc.cpp
//	创建者		：	simon
//	创建时间	：	2009/12/3 17:10:13
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegameworld/kluanpc.h"
#include "scene/kscene.h"
#include "onlinegameworld/knpcmgr.h"
#include "kplayerbaseprocess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------

KNpc* KLuaNpc::GetHim(QLuaScript& sc, INT nIndex)
{
	KLuaNpc* pLuaNpc = dynamic_cast<KLuaNpc*>(GetCObj(sc.GetLuaState(), nIndex));
	if (pLuaNpc)
		return pLuaNpc->m_pNpc;

	return NULL;
}

BOOL KLuaNpc::ClearTempTable()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	return ClearScriptTempTable(cSafeScript->GetLuaState(), s_szClassName, m_pNpc->GetId());
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: Id
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaNpc::getId(VOID) CONST
{
	return m_pNpc->m_dwId;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: TemplateId
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaNpc::getTemplateId(VOID) CONST
{
	return m_pNpc->GetTemplateId();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: DisplayName
// @Description		: Npc显示名
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaNpc::getDisplayName() CONST
{
	return m_pNpc->GetName();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: ClassName
// @Description		: Npc类名
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaNpc::getClassName( VOID ) CONST
{
	return m_pNpc->GetClassName();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: NickName
// @Description		: Npc别名
// @Property(R/RW)	: RW
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaNpc::getNickName() CONST
{
	return m_pNpc->GetNickName();
}
VOID KLuaNpc::setNickName(LPCSTR pszNickName)
{
	m_pNpc->SetNickName(pszNickName);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: GetCombatGroup
// @Description		: 获取阵营
// @ReturnCode		: d
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaGetCombatGroup(QLuaScript& sc)
{
	sc.PushNumber(1);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: GetCombatState
// @Description		: 获取战斗状态
// @ReturnCode		: d
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaGetCombatState(QLuaScript& sc)
{
	sc.PushNumber(1);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: GetDeleteState
// @Description		: 获取npc是否已经准备被回收
// @ReturnCode		: d
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaGetDeleteState(QLuaScript& sc)
{
	sc.PushNumber(m_pNpc->m_bDeleted);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: CallClientScriptOfNearByPlayer
// @Description		: 对npc附近的人调用脚本
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaCallClientScriptOfNearByPlayer(QLuaScript& sc)
{
	static BYTE _sbyBuffer[USHRT_MAX];
	KS2C_SCRIPT_REMOTECALL* pData = (KS2C_SCRIPT_REMOTECALL*)_sbyBuffer;
	*pData = KS2C_SCRIPT_REMOTECALL();

	INT nLen = sc.SaveValue2Buffer(pData->arBuffer, sizeof(_sbyBuffer) - sizeof(KS2C_SCRIPT_REMOTECALL), 1);
	LOG_PROCESS_ERROR(nLen > 0);	// 有可能是数据包过大

	g_cPlayerBaseProcess.S2CFunStat(sc, nLen); // 统计

	m_pNpc->m_cNpcServer.BroadcastNearby(pData, sizeof(KS2C_SCRIPT_REMOTECALL) + nLen, FALSE);

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: GetThreatList
// @Description		: npc仇恨列表
// @ReturnCode		: {id, id...}
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaGetThreatList(QLuaScript& sc)
{
	INT nIdx = 1;
	const KSimpThreatNode*  cpThreatNode    = NULL;

	cpThreatNode = m_pNpc->m_SimpThreatList.GetFirstThreat(thtMainThreat);
	sc.PushTable();
	while(cpThreatNode != NULL)
	{
		QCONFIRM_RET_NULL(cpThreatNode->pCharacter);
		sc.PushNumber(cpThreatNode->pCharacter->GetId());
		sc.SetTableIndex(nIdx);
		cpThreatNode = m_pNpc->m_SimpThreatList.GetNextThreat(thtMainThreat, cpThreatNode);
		++nIdx;
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: ClearAllThreat
// @Description		: 清除仇恨列表和被仇恨表
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaClearAllThreat(QLuaScript& sc)
{
	BOOL    bRetCode        = false;

	bRetCode = m_pNpc->m_SimpThreatList.ClearAllThreat();
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = m_pNpc->m_SimpThreatList.ClearAllThreaten();
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: CopyThreatList
// @Description		: 复制仇恨列表到目标Npc
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 目标NpcId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaCopyThreatList(QLuaScript& sc)
{
	BOOL                    bResult         = false;
	DWORD                   dwDstID         = 0;
	KCharacter*             pDstCharacter   = NULL;
	const KSimpThreatNode*  cpThreatNode    = NULL;

	dwDstID = (DWORD)sc.GetInt(1);
	LOG_PROCESS_ERROR(dwDstID != m_pNpc->m_dwId);
	LOG_PROCESS_ERROR(IS_NPC(dwDstID));

	pDstCharacter = g_cNpcMgr.GetById(dwDstID);
	LOG_PROCESS_ERROR(pDstCharacter);
	PROCESS_ERROR(pDstCharacter->m_pScene);

	cpThreatNode = m_pNpc->m_SimpThreatList.GetFirstThreat(thtMainThreat);
	while (cpThreatNode)
	{
		pDstCharacter->m_SimpThreatList.ModifyThreat(thtMainThreat, cpThreatNode->pCharacter, 0);
		cpThreatNode = m_pNpc->m_SimpThreatList.GetNextThreat(thtMainThreat, cpThreatNode);
	}

	bResult = true;
EXIT0:
	sc.PushNumber(bResult);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: SetPosition
// @Description		: 设置逻辑坐标(临时)
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: x
// @ArgumentComment	: y
// @ArgumentComment	: z
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaSetPosition(QLuaScript& sc)
{
	INT nX = sc.GetInt(1), nY = sc.GetInt(2), nZ = sc.GetInt(3);

	BOOL bRet = m_pNpc->SetPosition(nX, nY, nZ);

	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaNpc
// @LuaApiName		: Title
// @Description		: Npc Title
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaNpc::getTitle() CONST
{
	return m_pNpc->GetTitle();
}

#ifdef GAME_SERVER
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaNpc
// @LuaApiName		: TeleportTo
// @Description		: 瞬移
// @ReturnCode		: 
// @ArgumentFlag	: dddd
// @ArgumentComment	: SceneId
// @ArgumentComment	: x
// @ArgumentComment	: y
// @ArgumentComment	: z
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaTeleportTo(QLuaScript& sc)
{
	INT nSceneId = sc.GetInt(1);
	INT nX = sc.GetInt(2), nY = sc.GetInt(3), nZ = sc.GetInt(4);

	BOOL bRet = m_pNpc->m_cNpcServer.TeleportTo((DWORD)nSceneId, nX, nY, nZ);

	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaNpc
// @LuaApiName		: SetTeamId
// @Description		: 设置分组Id
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nNpcTeamId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaSetTeamId(QLuaScript& sc)
{
	BOOL    bResult         = false;
	BOOL    bRetCode        = false;
	INT		nNpcTeamID		= sc.GetInt(1);

	LOG_PROCESS_ERROR(nNpcTeamID >= 0);

	PROCESS_SUCCESS(m_pNpc->m_nNpcTeamID == nNpcTeamID);

	if (m_pNpc->m_nNpcTeamID)
	{
		bRetCode = m_pNpc->m_pScene->m_ThreatLink.DelMember(m_pNpc->m_nNpcTeamID, m_pNpc);
		LOG_PROCESS_ERROR(bRetCode);
	}

	m_pNpc->m_nNpcTeamID = nNpcTeamID;

	if (m_pNpc->m_nNpcTeamID)
	{
		bRetCode = m_pNpc->m_pScene->m_ThreatLink.AddMember(m_pNpc->m_nNpcTeamID, m_pNpc);
		LOG_PROCESS_ERROR(bRetCode);
	}

EXIT1:
	bResult = true;
EXIT0:
	sc.PushNumber(bResult);
	return 1;
}

#endif // GAME_SERVER

#ifdef GAME_CLIENT
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: C
// @ClassName		: KLuaNpc
// @LuaApiName		: GetRepresentPosition
// @Description		: 获取表现3D坐标
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaNpc::LuaGetRepresentPosition(QLuaScript& sc)
{
	INT nX = 0, nY = 0, nZ = 0;
	m_pNpc->GetPosition(nX, nY, nZ);

	Logic3DToRepresent3D(nX, nY, nZ);

	sc.PushTable();
	sc.PushNumber(nX);
	sc.SetTableField("nX");
	sc.PushNumber(nY);
	sc.SetTableField("nY");	
	sc.PushNumber(nZ);
	sc.SetTableField("nZ");

	return 1;
}

#endif // GAME_CLIENT

#include "kluanpc_table.hpp"
