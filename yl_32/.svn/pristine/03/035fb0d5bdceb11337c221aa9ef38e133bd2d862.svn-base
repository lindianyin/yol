#include "stdafx.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluaplayer.h"
#include "luaplayerartifact.h"
#include "playerartifact.h"
#include "artifactmanager.h"

INT LuaPlayerArtifact::Register2Player(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	PlayerArtifact* pPlayerArtifact= pPlayer->GetCtrl<PlayerArtifact>();
	QCONFIRM_RET_FALSE(pPlayerArtifact);
	sc.PushObj(pPlayerArtifact->GetScriptInterface());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArtifact
// @LuaApiName		: AddPiece
// @Description		: 加碎片
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: id
// @ArgumentComment	: 数量
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArtifact::LuaAddPiece(QLuaScript& sc)
{
	INT nId = sc.GetInt(1);
	INT nCount = sc.GetInt(2);
	BOOL bOk = FALSE;
	PlayerArtifact* pPlayerArtifact = m_rcCharacter.GetCtrl<PlayerArtifact>();
	LOG_PROCESS_ERROR(pPlayerArtifact);

	bOk = pPlayerArtifact->AddPiece(nId, nCount);
	sc.PushNumber(bOk);

EXIT0:
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArtifact
// @LuaApiName		: UsePiece
// @Description		: 加碎片
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: id
// @ArgumentComment	: 数量
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArtifact::LuaUsePiece(QLuaScript& sc)
{
	INT nId = sc.GetInt(1);
	INT nCount = sc.GetInt(2);
	BOOL bOk = FALSE;
	PlayerArtifact* pPlayerArtifact = m_rcCharacter.GetCtrl<PlayerArtifact>();
	LOG_PROCESS_ERROR(pPlayerArtifact);

	bOk = pPlayerArtifact->UsePiece(nId, nCount);
	sc.PushNumber(bOk);

EXIT0:
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaGetArtifactNumByLevel
// @LuaApiName		: GetArtifactNumByLevel
// @Description		: 获取某一等级以上神器数量
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 等级
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArtifact::LuaGetArtifactNumByLevel(QLuaScript& sc)
{
	INT nLevel = sc.GetInt(1);
	INT nCount = FALSE;
	PlayerArtifact* pPlayerArtifact = m_rcCharacter.GetCtrl<PlayerArtifact>();
	LOG_PROCESS_ERROR(pPlayerArtifact);

	nCount = pPlayerArtifact->GetArtifactNumByLevel(nLevel);
	sc.PushNumber(nCount);

EXIT0:
	return 1;
}

#include "luaplayerartifact_table.hpp"