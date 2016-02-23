/* -------------------------------------------------------------------------
//	文件名		：	kluadoodad.cpp
//	创建者		：	simon
//	创建时间	：	2010/3/15 9:36:40
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegameworld/kdoodad.h"
#include "onlinegameworld/kluadoodad.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------


KDoodad* KLuaDoodad::GetIt(QLuaScript& sc, INT nIndex)
{
	KLuaDoodad* pLuaDoodad = dynamic_cast<KLuaDoodad*>(GetCObj(sc.GetLuaState(), nIndex));
	if (pLuaDoodad)
		return pLuaDoodad->m_pIt;

	return NULL;
}

BOOL KLuaDoodad::ClearTempTable()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	return ClearScriptTempTable(cSafeScript->GetLuaState(), s_szClassName, m_pIt->m_dwId);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: Id
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaDoodad::getId(VOID) CONST
{
	return m_pIt->m_dwId;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: TemplateId
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaDoodad::getTemplateId(VOID) CONST
{
	return m_pIt->m_dwTemplateId;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: SceneId
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaDoodad::getSceneId(VOID) CONST
{
	DWORD dwSceneId = KD_BAD_ID;
	IKScene* pScene = m_pIt->GetScene();
	if (pScene)
		dwSceneId = pScene->GetId();
	return dwSceneId;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: FaceDirection
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaDoodad::getFaceDirection(VOID) CONST
{
	return m_pIt->GetFaceDirection();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: Name
// @Description		: Doodad名称
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaDoodad::getName() CONST
{
	return m_pIt->GetName();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: State
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaDoodad::getState(VOID) CONST
{
	return m_pIt->m_eState;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: GetPosition
// @Description		: 获取逻辑3D坐标
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaDoodad::LuaGetPosition(QLuaScript& sc)
{
	INT nX = 0, nY = 0, nZ = 0;
	m_pIt->GetPosition(nX, nY, nZ);

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
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: SetPosition
// @Description		: 设置逻辑坐标(无同步)
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: x
// @ArgumentComment	: y
// @ArgumentComment	: z
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaDoodad::LuaSetPosition(QLuaScript& sc)
{
	INT nX = sc.GetInt(1), nY = sc.GetInt(2), nZ = sc.GetInt(3);

	BOOL bRet = m_pIt->SetPosition(nX, nY, nZ);

	return sc.PushNumber(bRet);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: SetOwnerId
// @Description		: 设置OwnerId
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: OwnerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaDoodad::LuaSetOwnerId(QLuaScript& sc)
{
	INT nOwnerId = sc.GetInt(1);

	m_pIt->SetOwnerId(nOwnerId);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaDoodad
// @LuaApiName		: GetTempTable
// @Description		: 获得临时table
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaDoodad::LuaGetTempTable(QLuaScript& sc)
{
	if (!GetScriptTempTable(sc.GetLuaState(), s_szClassName, m_pIt->m_dwId))
	{
		_ASSERT(FALSE);
		return 0;
	}

	return 1;
}

#ifdef GAME_SERVER
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaDoodad
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
INT	KLuaDoodad::LuaTeleportTo(QLuaScript& sc)
{
	INT nSceneId = sc.GetInt(1);
	INT nX = sc.GetInt(2), nY = sc.GetInt(3), nZ = sc.GetInt(4);

	BOOL bRet = m_pIt->m_cDoodadServer.TeleportTo((DWORD)nSceneId, nX, nY, nZ);

	sc.PushNumber(bRet);

	return 1;
}

#endif // GAME_SERVER

// -------------------------------------------------------------------------
#include "kluadoodad_table.hpp"
