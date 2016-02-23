#include "stdafx.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluaplayer.h"
#include "luaplayertitle.h"
#include "playertitle.h"
#include "titlemanager.h"

INT LuaPlayerTitle::Register2Player(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	PlayerTitle* pPlayerTitle = pPlayer->GetCtrl<PlayerTitle>();
	QCONFIRM_RET_FALSE(pPlayerTitle);
	sc.PushObj(pPlayerTitle->GetScriptInterface());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerTitle
// @LuaApiName		: AddTitleToCharacter
// @Description		: 添加称号
// @ReturnCode		: 
// @ArgumentFlag	: dddd
// @ArgumentComment	: 获取途径
// @ArgumentComment	: 数值
// @ArgumentComment	: 类型
// @ArgumentComment	: 已经过了多久
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerTitle::LuaAddTitleToCharacter(QLuaScript& sc)
{
	INT nGetWay = sc.GetInt(1);
	INT nLevel = sc.GetInt(2);
	INT nType = sc.GetInt(3);
	DWORD dwPassTime = sc.GetInt(4);

	BOOL bOk = g_cTitleManager.AddTitleToCharacter(m_rcCharacter.GetId(), nGetWay, nLevel, nType, dwPassTime);

	sc.PushNumber(bOk);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerTitle
// @LuaApiName		: RemoveTitleToCharacter
// @Description		: 删除称号
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: 获取途径
// @ArgumentComment	: 数值
// @ArgumentComment	: 类型
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerTitle::LuaRemoveTitleToCharacter(QLuaScript& sc)
{
	INT nGetWay = sc.GetInt(1);
	INT nLevel = sc.GetInt(2);
	INT nType = sc.GetInt(3);

	BOOL bOk = g_cTitleManager.RemoveTitleToCharacter(m_rcCharacter.GetId(), nGetWay, nLevel, nType);

	sc.PushNumber(bOk);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerTitle
// @LuaApiName		: SnycTitle
// @Description		: 同步称号
// @ReturnCode		: 
// @ArgumentFlag	:
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerTitle::LuaSnycTitle(QLuaScript& sc)
{
	PlayerTitle* pPlayerTitle = m_rcCharacter.GetCtrl<PlayerTitle>();

	BOOL bOk = pPlayerTitle->SyncTitle();

	sc.PushNumber(bOk);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerTitle
// @LuaApiName		: ClearTitle
// @Description		: 清除称号
// @ReturnCode		: 
// @ArgumentFlag	
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerTitle::LuaClearTitle(QLuaScript& sc)
{
	PlayerTitle* pPlayerTitle = m_rcCharacter.GetCtrl<PlayerTitle>();

	BOOL bOk = pPlayerTitle->ClearSavaData();

	sc.PushNumber(bOk);

	return 1;
}




#include "luaplayertitle_table.hpp"