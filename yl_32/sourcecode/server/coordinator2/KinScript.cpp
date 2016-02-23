/* -------------------------------------------------------------------------
//	文件名		：	KinScript.cpp
//	创建者		：	liyun
//	创建时间	：	2012/11/10 14:35:06
//	功能描述	：	临时接口
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
// @LuaApiName		: CreateTongRequest
// @Description		: 创建家族
// @ReturnCode		: 创建成功：1，创建失败：0
// @ArgumentFlag	: dsd
// @ArgumentComment	: dwPlayerId作族长
// @ArgumentComment	: szTongName字符串作家族名字
// @ArgumentComment	: nTemplateIndex为创建模板索引
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaCreateTongRequest(QLuaScript& sc)
{
	DWORD dwMasterId = sc.GetInt(1);
	LPCSTR cszTongName = sc.GetStr(2);
	INT nTemplateIndex = sc.GetInt(3);
	BYTE byCamp        = (BYTE)sc.GetInt(4);

	BOOL bRet = g_pGameCenter->m_TongManager.CreateTong(dwMasterId, cszTongName, nTemplateIndex, byCamp);
	sc.PushNumber(bRet ? 1 : 0);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: InvitePlayerJoinTongRequest
// @Description		: 邀请玩家
// @ReturnCode		: 无
// @ArgumentFlag	: ds
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: szMemberName
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaInvitePlayerJoinTongRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	LPCSTR cszMemberName = sc.GetStr(2);
	// g_pGameCenter->m_TongManager.ProcessInvitePlayerJoinTongRequest(dwPlayerId, ()cszMemberName);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: InvitePlayerJoinTongRespond
// @Description		: 邀请
// @ReturnCode		: 无
// @ArgumentFlag	: ds
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: szMemberName
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaInvitePlayerJoinTongRespond(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	LPCSTR cszMemberName = sc.GetStr(2);
	// g_pGameCenter->m_TongManager.ProcessInvitePlayerJoinTongRespond(dwPlayerId, cszMemberName);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ApplyKickOutTongMemberRequest
// @Description		: 踢人
// @ReturnCode		: 无
// @ArgumentFlag	: dd
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: nMemberId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaApplyKickOutTongMemberRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	DWORD dwMemberId = sc.GetInt(2);

	g_pGameCenter->m_TongManager.ProcessKickOutTongMemberRequest(dwPlayerId, dwMemberId);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ApplyTongRosterRequest
// @Description		: 花名册
// @ReturnCode		: 无
// @ArgumentFlag	: ddd
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: nLastUpdateFrame
// @ArgumentComment	: dwMemberList
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaApplyTongRosterRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	INT nLastUpdateFrame = sc.GetInt(2);
	INT nMemberCount = sc.GetTopIndex();
	// todo 要用到动态DWORD数组

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ApplyTongInfoRequest
// @Description		: ApplyTongInfo
// @ReturnCode		: 无
// @ArgumentFlag	: dd
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: nLastUpdateFrame
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaApplyTongInfoRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	INT nLastUpdateFrame = sc.GetInt(2);
	g_pGameCenter->m_TongManager.ProcessApplyTongInfoRequest(dwPlayerId, nLastUpdateFrame);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ApplyRepertoryPageRequest
// @Description		: 仓库相关
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaApplyRepertoryPageRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ModifyTongSchemaRequest
// @Description		: ModifyTongSchema
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaModifyTongSchemaRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ApplyQuitTongRequest
// @Description		: 退出家族
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaApplyQuitTongRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	g_pGameCenter->m_TongManager.ProcessQuitRequest(dwPlayerId);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetKinLevelByPlayerId
// @Description		: 根据玩家id得到所在家族等级
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetKinLevelByPlayerId(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	KTong* pTong = NULL;
	KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);
	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	sc.PushNumber(pTong->m_nLevel);
EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SetApplyerState
// @Description		: 允许或拒绝申请者入族
// @ReturnCode		: 无
// @ArgumentFlag	: dddd
// @ArgumentComment	: dwOperatorId
// @ArgumentComment	: dwApplyerId
// @ArgumentComment	: dwTongId
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSetApplyerState(QLuaScript& sc)
{
	DWORD dwOperatorId = sc.GetInt(1);
    DWORD dwApplyerId = sc.GetInt(2);
	DWORD dwTongId = sc.GetInt(3);
    INT nAccept = sc.GetInt(4);

	BOOL bRet = g_pGameCenter->m_TongManager.SetApplyerState(dwOperatorId, dwApplyerId, dwTongId, nAccept == 1);
    sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: UpgradeKin
// @Description		: 升级家族
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaUpgradeKin(QLuaScript& sc)
{
	QTongUpgradeNeed cUpgradeNeed;
	KTong* pTong = NULL;
	DWORD dwPlayerId = sc.GetInt(1);
	DWORD dwPealNum = 0;
	DWORD dwJadeiteNum = 0;
	DWORD dwAgateNum = 0;
	DWORD dwDiamondNum = 0;
	KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);
	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	PROCESS_ERROR(pTong->m_nLevel < TONG_MAX_LEVEL);
	PROCESS_ERROR(pTong->m_dwMaster == dwPlayerId); // 只有族长才能执行升级操作
	
	cUpgradeNeed = g_pGameCenter->m_TongManager.GetTongUpgradeNeed(pTong->m_nLevel + 1);
	dwPealNum = cUpgradeNeed.nPealNeed;
	dwJadeiteNum = cUpgradeNeed.nJadeiteNeed;
	dwAgateNum = cUpgradeNeed.nAgateNeed;
	dwDiamondNum = cUpgradeNeed.nDiamondNeed;

	if (pTong->m_dwPearlNum < dwPealNum || pTong->m_dwJadeiteNum < dwJadeiteNum || pTong->m_dwAgateNum < dwAgateNum || pTong->m_dwDiamondNum < dwDiamondNum)
	{
		sc.PushNumber(0);
        goto EXIT0;
	}

	pTong->m_dwPearlNum -= dwPealNum;
	pTong->m_dwJadeiteNum -= dwJadeiteNum;
	pTong->m_dwAgateNum -= dwAgateNum;
	pTong->m_dwDiamondNum -= dwDiamondNum;

	pTong->m_nLevel++;
	pTong->m_nMaxMemberCount += 5; // 每升一级，帮会人数上限加5人

	sc.PushNumber(1);

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ChangeTongMemberGroupRequest
// @Description		: 更改家族成员组
// @ReturnCode		: 无
// @ArgumentFlag	: ddd
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: dwTargetMemberId
// @ArgumentComment	: nTargetGroup
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaChangeTongMemberGroupRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	DWORD dwTargetMemberId = sc.GetInt(2);
	INT nTargetGroup = sc.GetInt(3);
	g_pGameCenter->m_TongManager.ProcessChangeMemberGroupRequest(dwPlayerId, dwTargetMemberId, nTargetGroup);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ChangeTongMasterRequest
// @Description		: 移交族长
// @ReturnCode		: 无
// @ArgumentFlag	: dd
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: dwNewMasterId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaChangeTongMasterRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	DWORD dwNewMasterId = sc.GetInt(2);
	g_pGameCenter->m_TongManager.ProcessChangeMasterRequest(dwPlayerId, dwNewMasterId, FALSE);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetKinActivityOpenTimes
// @Description		: 得到所有家族活动的信息
// @ReturnCode		: 无
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetKinActivityOpenTimes(QLuaScript& sc)
{
	DWORD dwKinId = sc.GetInt(1);
	std::map<DWORD, SyncKinActivityInfo>::iterator it;
	KTong* pTong = g_pGameCenter->m_TongManager.GetTong(dwKinId);
	LOG_PROCESS_ERROR(pTong);
	
	sc.PushTable();
	for (it = pTong->m_ActivityOpenTimeMap.begin(); it != pTong->m_ActivityOpenTimeMap.end(); ++it)
	{
		sc.PushTable();
		SyncKinActivityInfo& actInfo = it->second;
        sc.PushNumber(actInfo.dwActivityOpenTime);
		sc.SetTableField("dwActivityOpenTime");
		sc.PushNumber(actInfo.bySignUp);
		sc.SetTableField("bySignUp");

		sc.SetTableIndex(it->first);
	}
	
EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ChangeTongMemberRemarkRequest
// @Description		: 更改家族成员remark
// @ReturnCode		: 无
// @ArgumentFlag	: dds
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: dwTargetMemberId
// @ArgumentComment	: szRemark
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaChangeTongMemberRemarkRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	DWORD dwTargetMemberId = sc.GetInt(2);
	LPCSTR cszRemark = sc.GetStr(3);
	// g_pGameCenter->m_TongManager.ProcessChangeTongMemberRemarkRequest(dwPlayerId, dwTargetMemberId, cszRemark);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetTongDescriptionRequest
// @Description		: 取得家庭描述
// @ReturnCode		: 描述字串
// @ArgumentFlag	: d
// @ArgumentComment	: dwTongId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetTongDescriptionRequest(QLuaScript& sc)
{
	// 仿照函数DoGetTongDescriptionRepond()实现
	DWORD dwTongId = sc.GetInt(1);
	KTong* pTong = g_pGameCenter->m_TongManager.GetTong(dwTongId);
	sc.PushString(pTong->m_szName);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetCurrentTime
// @Description		: 得到当前时间
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetCurrentTime(QLuaScript& sc)
{
	sc.PushNumber(g_pGameCenter->m_nTimeNow);

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetPlayerConnectIndex
// @Description		: 取得玩家连接的gs索引
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetPlayerConnectIndex(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	KRole* pRole = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);

	sc.PushNumber(pRole->m_nConnIndex);

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetTongMembers
// @Description		: 取得成员id列表
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: dwTongId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetTongMembers(QLuaScript& sc)
{
	KTong* pTong = NULL;
	std::vector<DWORD> vecMembers;
	DWORD dwTongId = sc.GetInt(1);
	INT nIdx = 1;

	pTong = g_pGameCenter->m_TongManager.GetTong(dwTongId);
	LOG_PROCESS_ERROR(pTong);

	pTong->GetMembers(vecMembers);

	sc.PushTable();

	for(std::vector<DWORD>::iterator it = vecMembers.begin(); it != vecMembers.end(); ++it)
	{
		sc.PushNumber(*it);
		sc.SetTableIndex(nIdx++);
	}

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ResetTongMemberDailyContribution
// @Description		: 重置家族成员每日贡献度
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaResetTongMemberDailyContribution(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	KTong* pTong = NULL;
	KRole* pRole = NULL;
	KTongMember* pMember = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);
	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);
	pMember = pTong->GetMember(dwPlayerId);
	LOG_PROCESS_ERROR(pTong);

	pMember->nDailyContribution = 0;

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetTongMember
// @Description		: 取得成员信息
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetTongMember(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	KTong* pTong = NULL;
	KRole* pRole = NULL;
	KTongMember* pMember = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);
	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);
	pMember = pTong->GetMember(dwPlayerId);
	LOG_PROCESS_ERROR(pTong);

	sc.PushTable();

	sc.PushNumber(pMember->bIsOnline);
	sc.SetTableField("bIsOnline");

	sc.PushNumber(pMember->dwForceID);
	sc.SetTableField("dwForceID");

	sc.PushNumber(pMember->dwID);
	sc.SetTableField("dwID");

	sc.PushNumber(pMember->dwMapID);
	sc.SetTableField("dwMapID");

	sc.PushNumber(pMember->nDailyContribution);
	sc.SetTableField("nDailyContribution");

	sc.PushNumber(pMember->nGroupID);
	sc.SetTableField("nGroupID");

	sc.PushNumber(pMember->nJoinTime);
	sc.SetTableField("nJoinTime");

	sc.PushNumber(pMember->nLastOfflineTime);
	sc.SetTableField("nLastOfflineTime");

	sc.PushNumber(pMember->nLastUpdateCount);
	sc.SetTableField("nLastUpdateCount");

	sc.PushNumber(pMember->nLastUpdateFrame);
	sc.SetTableField("nLastUpdateFrame");

	sc.PushNumber(pMember->nLevel);
	sc.SetTableField("nLevel");

	sc.PushNumber(pMember->dwFightScore);
	sc.SetTableField("nFightScore");

	sc.PushNumber(pMember->nSalary);
	sc.SetTableField("nSalary");

	sc.PushString(pMember->szRemark);
	sc.SetTableField("szRemark");

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SaveMoneyInTongRequest
// @Description		: 存钱
// @ReturnCode		: 无
// @ArgumentFlag	: ddddd
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: dwPealNum
// @ArgumentComment	: dwJadeiteNum
// @ArgumentComment	: dwAgateNum
// @ArgumentComment	: dwDiamondNum
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSaveMoneyInTongRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	DWORD dwPealNum = (DWORD)sc.GetInt(2);
	DWORD dwJadeiteNum = (DWORD)sc.GetInt(3);
	DWORD dwAgateNum = (DWORD)sc.GetInt(4);
	DWORD dwDiamondNum = (DWORD)sc.GetInt(5);

	KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);
	g_pGameCenter->m_TongManager.ProcessSaveMoneyInTong(pRole, dwPealNum, dwJadeiteNum, dwAgateNum, dwDiamondNum, 0);

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: PayTongSalaryRequest
// @Description		: 薪水相关
// @ReturnCode		: 无
// @ArgumentFlag	: dd
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: nTotalSalary
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPayTongSalaryRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	INT nTotalSalary = sc.GetInt(2);
	KRole* pRole = NULL;
	KTong* pTong = NULL;
	BOOL bRetCode = FALSE;
	LOG_PROCESS_ERROR(nTotalSalary > 0);

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(pRole->m_dwTongID);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	bRetCode = pTong->PaySalary(dwPlayerId, nTotalSalary);
	if (bRetCode)
	{
		// g_pGameCenter->m_TongManager.SendTongEventNotify(nConnIndex, dwPlayerId, tenPaySalarySuccess, pTong->m_nFund);
	}
	else
	{
		// g_pGameCenter->m_TongManager.SendTongEventNotify(nConnIndex, dwPlayerId, tenPaySalaryFailedError);
	}

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetTongSalaryRequest
// @Description		: 获取薪水
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetTongSalaryRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);
	INT nSalarySilver = 0;
	KTong* pTong = NULL;
	KTongMember* pMember = NULL;
	KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(pRole->m_dwTongID);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	pMember = pTong->GetMember(dwPlayerId);
	LOG_PROCESS_ERROR(pMember);

	nSalarySilver = pMember->nSalary;

EXIT0:
	sc.PushNumber(nSalarySilver);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetOnlineAndHaveKinPlayerIds
// @Description		: 获取所有在线且有家族的玩家id
// @ReturnCode		: 无
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetOnlineAndHaveKinPlayerIds(QLuaScript& sc)
{
	std::vector<DWORD> vecRoleIds;
	g_pGameCenter->m_RoleManager.GetRoleIds(vecRoleIds);

	sc.PushTable();
    for (INT i = 0; i < (INT)vecRoleIds.size(); i++)
	{
		KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(vecRoleIds[i]);
		LOG_PROCESS_ERROR(pRole);

		if (pRole->IsOnline())
		{
			KTong* pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
			if (pTong != NULL)
			{
				sc.PushNumber(pRole->m_dwPlayerID);
				sc.SetTableIndex(i + 1);
			}
		}
	}

EXIT0:
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetTongSalaryFailRespond
// @Description		: 获取工资失败
// @ReturnCode		: 无
// @ArgumentFlag	: ddd
// @ArgumentComment	: dwTongId
// @ArgumentComment	: dwPlayerId
// @ArgumentComment	: nSalary
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetTongSalaryFailRespond(QLuaScript& sc)
{
	DWORD dwTongId = sc.GetInt(1);
	DWORD dwPlayerId = sc.GetInt(2);
	INT nSalary = sc.GetInt(3);

	// todo 
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ApplyOpenTongRepertoryRequest
// @Description		: 打开仓库
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaApplyOpenTongRepertoryRequest(QLuaScript& sc)
{
	DWORD dwPlayerId = sc.GetInt(1);

	KRole* pRole = NULL;
	KTong* pTong = NULL;
	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	// todo 返回名

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ChangeTongCampRequest
// @Description		: 改变家族Camp
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaChangeTongCampRequest(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: TakeTongRepertoryItemToPosRequest
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaTakeTongRepertoryItemToPosRequest(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: TakeTongRepertoryItemRequest
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaTakeTongRepertoryItemRequest(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: TakeTongRepertoryItemRespond
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaTakeTongRepertoryItemRespond(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: PutTongRepertoryItemToPosRequest
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPutTongRepertoryItemToPosRequest(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: PutTongRepertoryItemRequest
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPutTongRepertoryItemRequest(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: PutTongRepertoryItemRespond
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPutTongRepertoryItemRespond(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: UnlockTongRepertoryGridRequest
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaUnlockTongRepertoryGridRequest(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ExchangeTongRepertoryItemPosRequest
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaExchangeTongRepertoryItemPosRequest(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ApplyStackTongRepertoryItemRequest
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaApplyStackTongRepertoryItemRequest(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: StackTongRepertoryItemRequest
// @Description		: 无
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: dwPlayerId
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaStackTongRepertoryItemRequest(QLuaScript& sc)
{
	// todo
	DWORD dwPlayerId = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SetKinRepertoryEnable
// @Description		: 打开关闭家族仓库功能
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: 1表示开启，0表示关闭
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSetKinRepertoryEnable(QLuaScript& sc)
{
	KTongRepertory::GetRentConfig().s_bKinRepertoryEnable = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SetKinRentEnable
// @Description		: 打开关闭家族仓库租借功能
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: 1表示开启，0表示关闭
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSetKinRentEnable(QLuaScript& sc)
{
	KTongRepertory::GetRentConfig().s_bKinRepertoryRentEnable = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SetKinRentPeriod
// @Description		: 设置装备出租时间，以秒为单位
// @ReturnCode		: 无
// @ArgumentFlag	: d
// @ArgumentComment	: 时间
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSetKinRentPeriod(QLuaScript& sc)
{
	KTongRepertory::GetRentConfig().s_tRentPeriod = sc.GetInt(1);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ImpeachKinMaster
// @Description		: 弹劾族长成功，将族长移交给其他人
// @ReturnCode		: d
// @ArgumentFlag	: dd
// @ArgumentComment	: 弹劾人ID
// @ArgumentComment	: 族长连续不在线时间
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaImpeachKinMaster(QLuaScript& sc)
{
	BOOL			bRetCode		= FALSE;
	DWORD			dwPlayerId		= 0;
	time_t			dwOfflineTime	= 0;
	DWORD			dwNewMaster		= 0;
	KRole*			pRole			= NULL;
	KTong*			pTong			= NULL;
	KTongMember*	pMasterMem		= NULL;
	KTong::KROSTER::iterator it;

	dwPlayerId = sc.GetInt(1);
	LOG_PROCESS_ERROR(dwPlayerId);

	dwOfflineTime = sc.GetInt(2);
	LOG_PROCESS_ERROR(dwOfflineTime > 0);

	// 取所有家族成员
	pRole = g_pGameCenter->m_RoleManager.GetRole(dwPlayerId);
	LOG_PROCESS_ERROR(pRole);

	pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
	LOG_PROCESS_ERROR(pTong);

	LOG_PROCESS_ERROR(pTong->m_dwMaster != dwPlayerId);

	pMasterMem = pTong->GetMember(pTong->m_dwMaster);
	LOG_PROCESS_ERROR(pMasterMem);
	PROCESS_ERROR(!pMasterMem->bIsOnline); //族长应该不在线
	PROCESS_ERROR(g_pGameCenter->m_nTimeNow - pMasterMem->nLastOfflineTime >= dwOfflineTime); // 连续不在线超过一定时间方可弹劾成功

	// 找在线精英
	/*for (it = pTong->m_Roster.begin(); it != pTong->m_Roster.end(); ++it)
	{
		if (it->second.nGroupID == ELITE_GROUP_INDEX && it->second.bIsOnline)
		{
			dwNewMaster = it->first;
			break;
		}
	}

	if (dwNewMaster == 0)
	{
		// 找在线普通成员
		for (it = pTong->m_Roster.begin(); it != pTong->m_Roster.end(); ++it)
		{
			if (it->first != pTong->m_dwMaster)
			{
				if (it->second.nGroupID == DEFAULT_GROUP_INDEX && it->second.bIsOnline)
				{
					dwNewMaster = it->first;
					break;
				}
			}
		}
	}*/

	bRetCode = FALSE;
	dwNewMaster = dwPlayerId;//发起弹劾者取代成为新的族长
	if (dwNewMaster)
	{
		g_pGameCenter->m_TongManager.ProcessChangeMasterRequest(pTong->m_dwMaster, dwNewMaster, TRUE); 
		bRetCode = TRUE;
	}

EXIT0:
	sc.PushNumber(bRetCode ? 1 : 0);
	return 1;
}

#include "KinScript_table.hpp"
};

BOOL g_RegisterKinScriptFun()
{
	return g_cScriptManager.RegisterTableFuncs("_G",
		KGcLogicBaseScriptNameSpace::arFunction, countof(KGcLogicBaseScriptNameSpace::arFunction));
}
