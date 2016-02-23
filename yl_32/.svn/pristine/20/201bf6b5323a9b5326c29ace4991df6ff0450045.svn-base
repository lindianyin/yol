/* -------------------------------------------------------------------------
//	文件名		：	kgclogicbasescript.cpp
//	创建者		：	simon
//	创建时间	：	2010/12/31 11:25:28
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "GameCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------

namespace KGcLogicBaseScriptNameSpace
{
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GlobalExecute
// @Description		: 远程让所有GameServer调用脚本指令
// @ReturnCode		: 发送成功：1，发送失败：0
// @ArgumentFlag	: {s}[d]
// @ArgumentComment	: arg:要调用的函数，随后跟着调用参数，Table打包
// @ArgumentComment	: [可选] 指定gs
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGlobalExecute(QLuaScript& sc)
{
	INT nRetCode = 0;
	INT nServerConnIdx = -1;
	static BYTE byBuff[60000];

	ZeroStruct(byBuff);

	if (sc.GetTopIndex() >= 2)
		nServerConnIdx = sc.GetInt(2);

	nRetCode = sc.SaveValue2Buffer(byBuff, sizeof(byBuff), 1);
	LOG_PROCESS_ERROR(nRetCode > 0);

	if (nServerConnIdx == 0)
	{
		sc.OutPutErrMsgF("params error!!!nServerConnIdx\n");
	}

	nRetCode = g_pGameCenter->m_GameServer.DoRemoteLuaCall(nServerConnIdx, byBuff, nRetCode);
	LOG_PROCESS_ERROR(nRetCode);

EXIT0:
	sc.PushNumber(nRetCode);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: CreateSystemTeam
// @Description		: 创建系统队伍并加入指定角色
// @ReturnCode		: SystemTeamId
// @ArgumentFlag	: d[dddd]
// @ArgumentComment	: 一个或更多玩家Id 作为队员
// @ArgumentComment	: 同上
// @ArgumentComment	: 同上
// @ArgumentComment	: 同上
// @ArgumentComment	: 同上
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCreateSystemTeam(QLuaScript& sc)
{
	INT nPlayerCount = sc.GetTopIndex();
	DWORD dwTeamId = 0;
	KTeam* pTeam = NULL;

	dwTeamId = g_pGameCenter->m_TeamCenter.CreateTeam(TRUE);
	LOG_PROCESS_ERROR(dwTeamId);
 
	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(dwTeamId);
	LOG_PROCESS_ERROR(pTeam);

	for (INT i = 1; i <= nPlayerCount; ++i)
	{
		DWORD dwPlayerId = sc.GetInt(i);
		KRole*  pRole   = NULL;

		pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
		QCONFIRM_CONTINUE(pRole);

		if (pRole->m_dwSystemTeamID != KD_BAD_ID)
		{
			g_pGameCenter->m_TeamCenter.DelMember(pRole->m_dwSystemTeamID, pRole->m_dwPlayerID);
		}

		if (pRole->GetTeamId() != KD_BAD_ID)
		{
			g_pGameCenter->m_TeamCenter.DelMember(pRole->GetTeamId(), pRole->m_dwPlayerID);
		}

		g_pGameCenter->m_TeamCenter.AddMember(dwTeamId, dwPlayerId, 1);
		g_pGameCenter->m_TeamCenter.ProcessTeam(dwTeamId, pTeam);
	}

EXIT0:
	sc.PushNumber(dwTeamId);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetMapPlayerCount
// @Description		: 得到地图上玩家个数
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 地图id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetMapPlayerCount(QLuaScript& sc)
{
	INT nPlayerCount = 0;
	INT nMapId = sc.GetInt(1);
	KMapInfo* pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(nMapId);
	LOG_PROCESS_ERROR(pMapInfo);

	nPlayerCount = pMapInfo->m_nPlayerCount;

EXIT0:
	sc.PushNumber(nPlayerCount);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetPlayerIdByName
// @Description		: 通过名字获取玩家ID
// @ReturnCode		: playerId
// @ArgumentFlag	: s
// @ArgumentComment	: 玩家名字
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetPlayerIdByName(QLuaScript& sc)
{
	LPCSTR	pcszName = sc.GetStr(1);
	DWORD	dwPlayerId = 0;
	KRole*	pRole = g_pGameCenter->m_RoleManager.GetRole(pcszName);
	if (pRole)
	{
		dwPlayerId = pRole->m_dwPlayerID;
	}

	sc.PushNumber(dwPlayerId);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetRoleInfoById
// @Description		: 通过id获取玩家信息
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 玩家id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetRoleInfoById(QLuaScript& sc)
{
	INT     nPlayerId = sc.GetInt(1);
	DWORD	dwPlayerId = 0;
	KRole*	pRole = g_pGameCenter->m_RoleManager.GetRole(nPlayerId);
	if (pRole)
	{
		sc.PushTable();

		sc.PushString(pRole->getAccount());
		sc.SetTableField("szAccount");

		sc.PushNumber(pRole->getLevel());
		sc.SetTableField("nLevel");

		sc.PushString(pRole->getName());
		sc.SetTableField("szName");

		sc.PushNumber(pRole->m_nFightScore);
		sc.SetTableField("nFightScore");

		return 1;
	}
	
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetRankIdByIdx
// @Description		: 得到某个排行榜排行第几的id
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 排行榜索引
// @ArgumentComment	: 第几名(start from 0)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetRankIdByIdx(QLuaScript& sc)
{
	INT nRankId = sc.GetInt(1);
	INT nRankIdx = sc.GetInt(2);
	DWORD dwId = 0;

	KRankPool* pRankPool = g_pGameCenter->m_RankManager.GetRankPool(nRankId);
	LOG_PROCESS_ERROR(pRankPool);

	dwId = pRankPool->GetIdByIdx(nRankIdx);

	sc.PushNumber(dwId);
EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: IsPlayerOnLine
// @Description		: 验证玩家是否在线
// @ReturnCode		: 在线1不在线0
// @ArgumentFlag	: d
// @ArgumentComment	: 玩家ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaIsPlayerOnLine(QLuaScript& sc)
{
	INT bOnLine = 0;
	DWORD dwPlayerID = sc.GetInt(1);
	KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerID);
	if (pRole && pRole->IsOnline())
		bOnLine = 1;

	sc.PushNumber(bOnLine);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: Quit
// @Description		: 服务器正常退出
// @ReturnCode		: 无
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaQuit(QLuaScript& sc)
{
	g_pGameCenter->Quit();
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SaveAll
// @Description		: CR存盘
// @ReturnCode		: 无
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSaveAll(QLuaScript& sc)
{
	g_pGameCenter->OnRegularSaveProcess();
	return 0;
}


#include "gclogicbasescript_table.hpp"
};

BOOL g_RegisterGcLogicBaseScriptFun()
{
	return g_cScriptManager.RegisterTableFuncs("_G",
		KGcLogicBaseScriptNameSpace::arFunction, countof(KGcLogicBaseScriptNameSpace::arFunction));
}
