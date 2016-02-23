/* -------------------------------------------------------------------------
//	文件名		：	kgc_relationscriptfuns.cpp
//	创建者		：	furuilei
//	创建时间	：	2009-8-21 14:45:07
//	功能描述	：	玩家人际关系脚本接口
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


namespace KGcRelationNameSpace
{

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: CheckIsLoadEnd
// @Description		: 检查角色数据是否加载完成
// @ReturnCode		: TRUE 玩家数据加载完成，FALSE 还没有加载完成
// @ArgumentFlag	: d
// @ArgumentComment	: nAppId 玩家姓名
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCheckIsLoadEnd(QLuaScript& sc)
{
	INT nAppId = sc.GetInt(1);
	//不檢查是否在線，不在線也可以添加或删除（zhengbozhou）
	BOOL bResult = TRUE;//g_pGameCenter->m_RelationManager.CheckIsLoadEnd(nAppId);
	sc.PushNumber(bResult);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: CheckCreateDepRep
// @Description		: 检查创建关系的依赖和冲突关系
// @ReturnCode		: BOOL
// @ArgumentFlag	: dddd
// @ArgumentComment	: nType 关系类型
// @ArgumentComment	: nRole 是否是主位
// @ArgumentComment	: nAppId 申请玩家id
// @ArgumentComment	: nDstId 对方id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCheckCreateDepRep(QLuaScript& sc)
{

	INT nType = sc.GetInt(1);
	INT nRole = sc.GetInt(2);
	INT nAppId = sc.GetInt(3);
	INT nDstId = sc.GetInt(4);
	BOOL bResult = g_pGameCenter->m_RelationManager.CheckCreateDepRep(nType, nRole, nAppId, nDstId);
	sc.PushNumber(bResult);
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: CanCreateRelation
// @Description		: 检查具体的某种关系类型是否可以建立
// @ReturnCode		: BOOL 检查结果
// @ArgumentFlag	: dddd
// @ArgumentComment	: nType 要添加的关系类型
// @ArgumentComment	: nRole: 申请者角色（1为主位关系，0为次位关系）
// @ArgumentComment	: nAppId 玩家ID
// @ArgumentComment	: nDstId 玩家ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCanCreateRelation(QLuaScript& sc)
{
	INT nType = sc.GetInt(1);
	INT nRole = sc.GetInt(2);
	INT nAppId = sc.GetInt(3);
	INT nDstId = sc.GetInt(4);

	BOOL bResult = g_pGameCenter->m_RelationManager.CanCreateRelation(nType, nRole, nAppId, nDstId);
	sc.PushNumber(bResult);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: CreateRelation
// @Description		: 不做检查，直接创建人际关系
// @ReturnCode		: BOOL  创建关系的结果
// @ArgumentFlag	: ddd[d]
// @ArgumentComment	: nType 要添加的关系类型
// @ArgumentComment	: nAppId 玩家ID
// @ArgumentComment	: nDstId 玩家ID
// @ArgumentComment	: bIsMaster 默认为1，表示是否是关系的主位 1主位 0次位
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCreateRelation(QLuaScript& sc)
{
	INT nParamCount = sc.GetTopIndex();
	INT nType = sc.GetInt(1);
	INT nAppId = sc.GetInt(2);
	INT nDstId = sc.GetInt(3);
	INT bIsMaster = TRUE;
	if (nParamCount >= 4)
	{
		bIsMaster = sc.GetInt(4);
	}

	BOOL bResult = g_pGameCenter->m_RelationManager.CreateRelation(nType, nAppId, nDstId, bIsMaster);
	sc.PushNumber(bResult);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: CheckDelDepRep
// @Description		: 检查删除关系的依赖和冲突关系
// @ReturnCode		: BOOL
// @ArgumentFlag	: dddd
// @ArgumentComment	: nType 关系类型
// @ArgumentComment	: nRole 是否是主位
// @ArgumentComment	: nAppId 申请玩家id
// @ArgumentComment	: nDstId 对方id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCheckDelDepRep(QLuaScript& sc)
{
	INT nType = sc.GetInt(1);
	INT nRole = sc.GetInt(2);
	INT nAppId = sc.GetInt(3);
	INT nDstId = sc.GetInt(4);

	BOOL bResult = g_pGameCenter->m_RelationManager.CheckDelDepRep(nType, nRole, nAppId, nDstId);
	sc.PushNumber(bResult);
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: DelRelation
// @Description		: 不做检查，直接删除指定的人际关系
// @ReturnCode		: BOOL  创建关系的结果
// @ArgumentFlag	: ddd[d]
// @ArgumentComment	: nType 要删除的关系类型
// @ArgumentComment	: nAppId 玩家ID
// @ArgumentComment	: nDstId 关系玩家ID
// @ArgumentComment	: bIsMaster 默认为1，表示是否是关系的主位 1主位 0次位
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaDelRelation(QLuaScript& sc)
{
	INT nParamCount = sc.GetTopIndex();
	INT nType = sc.GetInt(1);
	INT nAppId = sc.GetInt(2);
	INT nDstId = sc.GetInt(3);
	INT bIsMaster = TRUE;
	if (nParamCount >= 4)
	{
		bIsMaster = sc.GetInt(4);
	}

	BOOL bResult = g_pGameCenter->m_RelationManager.DelRelation(nType, nAppId, nDstId, bIsMaster);
	sc.PushNumber(bResult);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ResetLimtWhenCrossDay
// @Description		: 检查加亲密度的日期，跨天则清除每日上限
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: nAppId 申请玩家id
// @ArgumentComment	: nDstId 对方玩家id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaResetLimtWhenCrossDay(QLuaScript& sc)
{
	INT nAppId = sc.GetInt(1);
	INT nDstId = sc.GetInt(2);
	g_pGameCenter->m_RelationManager.ResetLimtWhenCrossDay(nAppId, nDstId);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: AddFriendFavor
// @Description		: 增加亲密度
// @ReturnCode		: BOOL 增加好友亲密度是否成功
// @ArgumentFlag	: ddd[dd]
// @ArgumentComment	: nAppId 申请者id
// @ArgumentComment	: nDstId 对方玩家id
// @ArgumentComment	: nFavor 要增加的亲密度
// @ArgumentComment	: nMethod 途径 0正常途径 1通过ib道具 默认是0
// @ArgumentComment	: bByHand 是否手工添加，不判断每日上限（0表示系统添加1表示手工添加）
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaAddFriendFavor(QLuaScript& sc)
{
	INT nParamCount = sc.GetTopIndex();
	INT nAppId = sc.GetInt(1);
	INT nDstId = sc.GetInt(2);
	INT nFavor = sc.GetInt(3);
	INT nMethod = 0;
	BOOL bByHand = FALSE;

	if (nParamCount >= 4)
	{
		nMethod = sc.GetInt(4);
	}
	if (nParamCount >= 5)
	{
		bByHand = sc.GetInt(5);
	}

	BOOL bResult = g_pGameCenter->m_RelationManager.AddFriendFavor(nAppId, nDstId, nFavor, nMethod, bByHand);
	sc.PushNumber(bResult);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SetFriendTalkTime
// @Description		: 设置最近一次聊天时间
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: nAppId 申请者id
// @ArgumentComment	: nDstId 对方玩家id
// @ArgumentComment	: dwTime
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSetFriendTalkTime(QLuaScript& sc)
{
	INT nAppId = sc.GetInt(1);
	INT nDstId = sc.GetInt(2);
	DWORD dwTime = sc.GetInt(3);

	BOOL bResult = g_pGameCenter->m_RelationManager.SetFriendTalkTime(nAppId, nDstId, dwTime);
	sc.PushNumber(bResult);

	return 1;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetRoleName
// @Description		: 获取角色名字
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nAppId 申请者id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetRoleName(QLuaScript& sc)
{
	INT nAppId = sc.GetInt(1);

	KRole *pRole = g_pGameCenter->m_RoleManager.GetRole(nAppId);
	if(pRole)
	{
		sc.PushString(pRole->GetPlayerName());
	}
	else
	{
		sc.PushNull();
	}
	return 1;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SyncRelation
// @Description		: 同步所有关系
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nAppId 申请者id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSyncRelation(QLuaScript& sc)
{
	INT nAppId = sc.GetInt(1);

	BOOL bResult = g_pGameCenter->m_RelationManager.SyncRelationList(nAppId);
	sc.PushNumber(bResult);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SyncFriendInfo
// @Description		: 在增加亲密度后同步好友信息
// @ReturnCode		: BOOL
// @ArgumentFlag	: dd
// @ArgumentComment	: nAppId 申请方玩家id
// @ArgumentComment	: nDstId 对方玩家id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSyncFriendInfo(QLuaScript& sc)
{
	INT nAppId = sc.GetInt(1);
	INT nDstId = sc.GetInt(2);
	BOOL bResult = g_pGameCenter->m_RelationManager.SyncFriendInfo(nAppId, nDstId);
	sc.PushNumber(bResult);
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetCloseFriendTimeInfo
// @Description		: 获取密友（单向和双向密友）关系是否快要到期
// @ReturnCode		: {{nPlayerId, nTime, nType}, {nPlayerId, nTime, nType}, ...}
// @ArgumentFlag	: d
// @ArgumentComment	: nAppPlayerId	玩家id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetCloseFriendTimeInfo(QLuaScript& sc)
{
	INT nAppPlayerId = sc.GetInt(1);
	std::vector<KRelationManager_GC::KTIMEINFO> vctTimeInfo;
	BOOL bRet = g_pGameCenter->m_RelationManager.GetCloseFriendTimeInfo(nAppPlayerId, vctTimeInfo);
	if (bRet)
	{
		sc.PushTable();
		std::vector<KRelationManager_GC::KTIMEINFO>::iterator itVctTimeInfo = vctTimeInfo.begin();
		INT nIndex = 0;
		for (; itVctTimeInfo != vctTimeInfo.end(); ++itVctTimeInfo)
		{
			sc.PushTable();
			sc.PushNumber(itVctTimeInfo->nPlayerId);
			sc.SetTableField("nPlayerId");
			sc.PushNumber(itVctTimeInfo->nRemanentTime);
			sc.SetTableField("nTime");
			sc.PushNumber(itVctTimeInfo->nType);
			sc.SetTableField("nType");
			sc.PushNumber(itVctTimeInfo->bAsMaster);
			sc.SetTableField("bAsMaster");

			sc.SetTableIndex(++nIndex);
		}
	}
	else
	{
		sc.PushNull();
	}
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: DelOverTimeRelation
// @Description		: 删除一年到期的密友，师徒和介绍人关系
// @ReturnCode		: BOOL			是否删除成功
// @ArgumentFlag	: dddd
// @ArgumentComment	: nType			要删除的关系类型
// @ArgumentComment	: nMasterId		主位玩家id
// @ArgumentComment	: nTargetId		次位玩家id
// @ArgumentComment	: bIsMaster		角色是否是主位
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaDelOverTimeRelation(QLuaScript& sc)
{
	INT nType = sc.GetInt(1);
	INT nMasterId = sc.GetInt(2);
	INT nTargetId = sc.GetInt(3);
	BOOL bIsMaster = sc.GetInt(4);
	BOOL bRet = g_pGameCenter->m_RelationManager.DelOverTimeRelation(nType, nMasterId, nTargetId, bIsMaster);
	sc.PushNumber(bRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetOneRelationCount
// @Description		: 获取指定的某种人际关系的数量
// @ReturnCode		: INT			数量
// @ArgumentFlag	: dd[d]
// @ArgumentComment	: nPlayerId		玩家id
// @ArgumentComment	: nType			关系类型
// @ArgumentComment	: bAsMaster		是否是主位玩家
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetOneRelationCount(QLuaScript& sc)
{
	INT nPlayerId = sc.GetInt(1);
	INT nType = sc.GetInt(2);
	BOOL bAsMaster = TRUE;
	if (sc.GetTopIndex() >= 3)
		bAsMaster = sc.GetInt(3);

	INT nCount = g_pGameCenter->m_RelationManager.GetOneRelationCount(nPlayerId, nType, bAsMaster);
	sc.PushNumber(nCount);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: HasRelation
// @Description		: 检查是否存在指定的人际关系
// @ReturnCode		: BOOL
// @ArgumentFlag	: ddd[d]
// @ArgumentComment	: nAppId	主位id
// @ArgumentComment	: nDstId	次位id
// @ArgumentComment	: nType		关系类型
// @ArgumentComment	: bAsMaster	主次位
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaHasRelation(QLuaScript& sc)
{
	INT nAppId = sc.GetInt(1);
	INT nDstId = sc.GetInt(2);
	INT nType = sc.GetInt(3);
	BOOL bAsMaster = TRUE;
	if (sc.GetTopIndex() >= 4)
		bAsMaster = sc.GetInt(4);

	BOOL bHasRelation = g_pGameCenter->m_RelationManager.HasRelation(nAppId, nDstId, nType, bAsMaster);
	sc.PushNumber(bHasRelation);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetFriendFavor
// @Description		: 获取指定玩家的亲密度
// @ReturnCode		: nFavor
// @ArgumentFlag	: dd
// @ArgumentComment	: nPlayerId1	第一个玩家id
// @ArgumentComment	: nPlayerId2	第二个玩家id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetFriendFavor(QLuaScript& sc)
{
	INT nPlayerId1 = sc.GetInt(1);
	INT nPlayerId2 = sc.GetInt(2);

	INT nFavor = g_pGameCenter->m_RelationManager.GetFriendFavor(nPlayerId1, nPlayerId2);
	sc.PushNumber(nFavor);
	return 1;
}

#include "relationscriptfuns_table.hpp"

};

BOOL g_RegisterRelationScriptFun()
{
	return g_cScriptManager.RegisterTableFuncs("_G",
		KGcRelationNameSpace::arFunction, countof(KGcRelationNameSpace::arFunction));
}

// -------------------------------------------------------------------------
