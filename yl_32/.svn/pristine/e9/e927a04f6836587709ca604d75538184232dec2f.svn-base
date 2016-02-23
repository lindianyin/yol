#include "stdafx.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluaplayer.h"
#include "LuaPlayerArena.h"
#include "playerarena.h"
#include "rank/RankRelayClient.h"

INT LuaPlayerArena::Register2Player(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	PlayerArena* pPlayerArena = pPlayer->GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);
	sc.PushObj(pPlayerArena->GetScriptInterface());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: StartArena
// @Description		: 刚进入竞技场
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaStartArena(QLuaScript& sc)
{
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	pPlayerArena->StartArena();
	return 0;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: LoadArenaPlayers
// @Description		: 加载npc
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaLoadArenaPlayers(QLuaScript& sc)
{
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	pPlayerArena->DoLoadArenaPlayers();
	return 0;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: LoadFrontArenaPlayers
// @Description		: 加载前5名npc
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaLoadFrontArenaPlayers(QLuaScript& sc)
{
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	pPlayerArena->DoLoadFrontArenaPlayers();
	return 0;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: Challenge
// @Description		: 挑战
// @ArgumentFlag	: d
// @ArgumentComment	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaChallenge(QLuaScript& sc)
{
	DWORD dwNpcId = sc.GetInt(1);
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	pPlayerArena->OnFightRequest(dwNpcId);
	return 0;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: GetCDTime
// @Description		: 得到挑战CD时间
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaGetCDTime(QLuaScript& sc)
{
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	sc.PushNumber(pPlayerArena->GetCDTime());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: SetCDTime
// @Description		: 设置挑战CD时间
// @ArgumentFlag	: d
// @ArgumentComment	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaSetCDTime(QLuaScript& sc)
{
	INT nCDTime = sc.GetInt(1);
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	pPlayerArena->SetCDTime(nCDTime);

	return 0;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: SetTakeAwardTime
// @Description		: 设置领取排名奖励时间
// @ArgumentFlag	: d
// @ArgumentComment	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaSetTakeAwardTime(QLuaScript& sc)
{
	DWORD dwTime = sc.GetInt(1);
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	pPlayerArena->SetTakeAwardTime(dwTime);

	return 0;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: GetWinCombo
// @Description		: 得到连胜场次
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaGetWinCombo(QLuaScript& sc)
{
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	sc.PushNumber(pPlayerArena->GetWinCombo());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: SetWinCombo
// @Description		: 设置连胜场次
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaSetWinCombo(QLuaScript& sc)
{
	INT nCombo = sc.GetInt(1);

	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	pPlayerArena->SetWinCombo(nCombo);
	pPlayerArena->SyncArenaInfo();
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: GetTakeComboAward
// @Description		: 得到领取连胜奖励场次
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaGetTakeComboAward(QLuaScript& sc)
{
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	sc.PushNumber(pPlayerArena->GetTakeComboAward());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: SetTakeComboAward
// @Description		: 设置领取连胜奖励场次
// @ArgumentFlag	: d
// @ArgumentComment	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaSetTakeComboAward(QLuaScript& sc)
{
	INT nCombo = sc.GetInt(1);
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	pPlayerArena->SetTakeComboAward(nCombo);

	return 0;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: SyncArenaInfo
// @Description		: 发送竞技场信息
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaSyncArenaInfo(QLuaScript& sc)
{
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	pPlayerArena->SyncArenaInfo();
	return 0;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: ClearData
// @Description		: 清除竞技场数据
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaClearData(QLuaScript& sc)
{
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	pPlayerArena->ClearNpcs();
	return 0;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerArena
// @LuaApiName		: ResetData
// @Description		: 0点重置竞技场
// @ArgumentFlag	: 
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerArena::LuaResetData(QLuaScript& sc)
{
	PlayerArena* pPlayerArena = m_rcCharacter.GetCtrl<PlayerArena>();
	QCONFIRM_RET_FALSE(pPlayerArena);

	pPlayerArena->ResetData();
	return 0;
}

#include "LuaPlayerArena_table.hpp"