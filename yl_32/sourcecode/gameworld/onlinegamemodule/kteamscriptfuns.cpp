
#include "stdafx.h"

#include "onlinegamebase/kscriptmanager.h"
//#include "kteammanager.h"
//#ifdef GAME_SERVER
//#include "kplayerteammanager_gs.h"
//#else
//#include "kplayerteammanager_client.h"
//#endif

#include "onlinegameworld/kplayermgr.h"
#include "qteamserver.h"
#include "qteamgchandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KTeamScript_NameSpace
{

#ifdef GAME_SERVER

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: GetTeamMemberList
// @Description		: 获取队伍成员列表
// @ReturnCode		: 成员ID数组，成员数量
// @ArgumentFlag	: d
// @ArgumentComment	: 队伍ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetTeamMemberList(QLuaScript& cScript)
{
	if (cScript.GetTopIndex() < 1)
		return 0;

	INT nTeamID = cScript.GetInt(1);
	QTeam* pTeam = g_cTeamServer.GetTeam(nTeamID);

	cScript.PushTable();
	INT nIdx = 0;
	if(pTeam)
	{
		for (int i = 0; i < pTeam->nGroupNum; i++)
		{
			for (
				QTEAM_MEMBER_LIST::iterator it = pTeam->MemberGroup[i].MemberList.begin();
				it != pTeam->MemberGroup[i].MemberList.end(); ++it
				)
			{
				cScript.PushNumber(it->dwMemberID);
				cScript.SetTableIndex(++nIdx);
			}
		}
	}
	
	cScript.PushNumber(nIdx);
	return 2;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: GetTeamLeader
// @Description		: 获取队长Id
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 队伍ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetTeamLeader(QLuaScript& cScript)
{
	if (cScript.GetTopIndex() < 1)
		return 0;

	INT nTeamID = cScript.GetInt(1);
	QTeam* pTeam = g_cTeamServer.GetTeam(nTeamID);
	QCONFIRM_RET_NULL(pTeam);

	cScript.PushNumber(pTeam->dwAuthority[tatLeader]);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: CreateTeam
// @Description		: 自己创建一个队伍
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 角色Id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCreateTeam(QLuaScript& cScript)
{
	INT nPlayerId = cScript.GetInt(1);
	KPlayer* pPlayer = g_cPlayerMgr.GetById(nPlayerId);
	QCONFIRM_RET_NULL(pPlayer);
	QTeamGCHandler::DoCreateTeamRequest(pPlayer->GetId(), pPlayer->GetId(), 1);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: DisbandTeam
// @Description		: 解散队伍
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 队长id
// @ArgumentComment	: 队伍id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaDisbandTeam(QLuaScript& cScript)
{
	DWORD dwTeamLeaderId = cScript.GetInt(1);
	INT nTeamId = cScript.GetInt(2);
	
	QTeamGCHandler::DoTeamDisbandRequest(dwTeamLeaderId, nTeamId);

	return 0;
}

#endif //#ifdef GAME_SERVER

#include "kteamscriptfuns_table.hpp"
} // namespace KTeamScript_NameSpace

BOOL g_GCRegisterTeamScriptFuns()
{
	return g_cScriptManager.RegisterTableFuncs("KTeam", KTeamScript_NameSpace::arFunction, countof(KTeamScript_NameSpace::arFunction));
}
