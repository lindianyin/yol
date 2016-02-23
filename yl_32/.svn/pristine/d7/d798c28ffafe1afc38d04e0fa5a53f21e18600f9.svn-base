

#include "stdafx.h"
#include "onlinegameworld/kluaplayer.h"
#include "kplayerfindpath.h"
#include "kluaplayerfindpath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

INT KLuaPlayerFindPath::RegisterToPlayer(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLuaPlayer = static_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_NULL(pLuaPlayer);	

	KPlayer* pPlayer = pLuaPlayer->GetHim();
	QCONFIRM_RET_NULL(pPlayer);	

	KPlayerFindPath* pPlayerFindPath = pPlayer->GetCtrl<KPlayerFindPath>();
	sc.PushObj(pPlayerFindPath->GetScriptInterface());
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerFindPath
// @LuaApiName		: StartAutoPath
// @Description		: 自动寻路
// @ReturnCode		: 1 成功
// @ArgumentFlag	: ddd[d]
// @ArgumentComment	: nX	目标点 nX
// @ArgumentComment	: nY	目标点 nY
// @ArgumentComment	: nZ	目标点 nZ
// @ArgumentComment	: bStopNearby	目标点 nZ
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerFindPath::LuaStartAutoPath(QLuaScript& sc)
{
	QCONFIRM_RET_NULL(m_pPlayerFindPath);
	INT nParamCount = sc.GetTopIndex();
	INT nMapId = sc.GetInt(1);
	INT nX = sc.GetInt(2);
	INT nY = sc.GetInt(3);
	INT nZ = sc.GetInt(4);
	BOOL bStopNearby = FALSE;

	sc.PushNumber(m_pPlayerFindPath->GoTo(nMapId, nX, nY, nZ));
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerFindPath
// @LuaApiName		: GetNavPath
// @Description		: 获取导航路径
// @ReturnCode		: {{nX, nY, nZ}, {}, }
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerFindPath::LuaGetNavPath(QLuaScript& sc)
{
#ifdef GAME_CLIENT
	QCONFIRM_RET_NULL(m_pPlayerFindPath);
	size_t uNavPathLen = m_pPlayerFindPath->GetNavPathLen();	
	float* pNavPath = m_pPlayerFindPath->GetNavPath();
	
	sc.PushTable();

	for (size_t i = 0; i < uNavPathLen; i++)
	{
		QCONFIRM_RET_NULL(pNavPath);
		sc.PushTable();
		INT nX = (INT)pNavPath[i * 3 + 0];
		INT nY = (INT)pNavPath[i * 3 + 1];
		INT nZ = (INT)pNavPath[i * 3 + 2];

		sc.PushNumber(nX);
		sc.SetTableField("nX");
		sc.PushNumber(nY);
		sc.SetTableField("nY");
		sc.PushNumber(nZ);
		sc.SetTableField("nZ");
		sc.SetTableIndex(i + 1);
	}
	return 1;
#else
	ASSERT(FALSE);
	sc.PushNull();
	return 0;
#endif
}

#include "kluaplayerfindpath_table.hpp"
// -------------------------------------------------------------------------
