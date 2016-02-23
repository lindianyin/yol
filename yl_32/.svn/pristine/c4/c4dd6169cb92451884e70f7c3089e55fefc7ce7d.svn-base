#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kcharactermgr.h"
#include "onlinegameworld/kplayermgr.h"
#include "kactivityctrl.h"
#include "kactivitymanager.h"
#include "../record/RecordRelayClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KActivityScript
{

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SyncActivityTime
	// @Description		: 同步活动时间
	// @ReturnCode		: 
	// @ArgumentFlag	: odddd
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: nActivityIdx
	// @ArgumentComment	: nBaseActivityIdx
	// @ArgumentComment	: dwOpenTime
	// @ArgumentComment	: dwCloseTime
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSyncActivityTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KActivityCtrl* pActivityCtrl = NULL;
		INT nActivityIdx = sc.GetInt(2);
		INT nBaseActivityIdx = sc.GetInt(3);
		DWORD dwOpenTime = sc.GetInt(4);
		DWORD dwCloseTime = sc.GetInt(5);
		
		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		LOG_PROCESS_ERROR(pPlayer);

		pActivityCtrl = pPlayer->GetCtrl<KActivityCtrl>();
		LOG_PROCESS_ERROR(pActivityCtrl);

		pActivityCtrl->SyncActivityTime(nActivityIdx, nBaseActivityIdx, dwOpenTime, dwCloseTime);

EXIT0:
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetLastEnterZBTTime
	// @Description		: 设置上次进入战宝堂的时间
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: time
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetLastEnterZBTTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KActivityCtrl* pCtrl = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		pCtrl = pPlayer->GetCtrl<KActivityCtrl>();
		QCONFIRM_RET_FALSE(pCtrl);

		DWORD dwTime = sc.GetInt(2);

		pCtrl->sActivityInfo.dwLastEnterZBTTime = dwTime;

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetLastEnterZBTTime
	// @Description		: 得到上次进入战宝堂的时间
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetLastEnterZBTTime(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KActivityCtrl* pCtrl = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		pCtrl = pPlayer->GetCtrl<KActivityCtrl>();
		QCONFIRM_RET_FALSE(pCtrl);

		sc.PushNumber(pCtrl->sActivityInfo.dwLastEnterZBTTime);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetLastMPJJRank
	// @Description		: 活动最近门派竞技排名
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetLastMPJJRank(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KActivityCtrl* pCtrl = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		pCtrl = pPlayer->GetCtrl<KActivityCtrl>();
		QCONFIRM_RET_FALSE(pCtrl);

		sc.PushNumber(pCtrl->sActivityInfo.dwLastMPJJRank);

		return 1;
	}
	
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetLastMPJJRank
	// @Description		: 设置最近门派竞技排名
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: rank
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetLastMPJJRank(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KActivityCtrl* pCtrl = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		pCtrl = pPlayer->GetCtrl<KActivityCtrl>();
		QCONFIRM_RET_FALSE(pCtrl);


		DWORD dwRank = sc.GetInt(2);

		pCtrl->sActivityInfo.dwLastMPJJRank = dwRank;

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetKinBattleWinTimes
	// @Description		: 设置三界争霸连胜场数
	// @ReturnCode		: 
	// @ArgumentFlag	: od
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: times
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetKinBattleWinTimes(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KActivityCtrl* pCtrl = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		pCtrl = pPlayer->GetCtrl<KActivityCtrl>();
		QCONFIRM_RET_FALSE(pCtrl);

		BYTE byTimes = sc.GetInt(2);

		pCtrl->sActivityInfo.byKinBattleWinTimes = byTimes;

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetKinBattleWinTimes
	// @Description		: 得到三界争霸连胜场数
	// @ReturnCode		: 
	// @ArgumentFlag	: o
	// @ArgumentComment	: pPlayer
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetKinBattleWinTimes(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KActivityCtrl* pCtrl = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		QCONFIRM_RET_FALSE(pPlayer);

		pCtrl = pPlayer->GetCtrl<KActivityCtrl>();
		QCONFIRM_RET_FALSE(pCtrl);

		sc.PushNumber(pCtrl->sActivityInfo.byKinBattleWinTimes);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddKinBattleRecord
	// @Description		: 新增一条三界神皇记录
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: dwRoleId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddKinBattleRecord(QLuaScript &sc)
	{
		DWORD dwRoleId = sc.GetInt(1);

		RecordRelayClient::DoAddKinBattleRecordRequest(dwRoleId);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetKinBattleWinnerId
	// @Description		: 设置三界神皇id
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: dwWinnerId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetKinBattleWinnerId(QLuaScript &sc)
	{
		DWORD dwWinnerId = sc.GetInt(1);

		RecordRelayClient::DoSetKinBattleWinnerIdRequest(dwWinnerId);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetLastWinKinInfo
	// @Description		: 设置最近一次赢得三界争霸家族信息
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: dwWinnerId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetLastWinKinInfo(QLuaScript &sc)
	{
		DWORD dwWinnerId = sc.GetInt(1);

		RecordRelayClient::DoSetLastWinKinRequest(dwWinnerId);

		return 0;
	}


	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SyncKinBattleJoinNum
	// @Description		: 发送当前三界争霸参赛人数
	// @ReturnCode		: 
	// @ArgumentFlag	: ddd
	// @ArgumentComment	: dwRoleId
	// @ArgumentComment	: nJoinNum
	// @ArgumentComment	: nHasJoined
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSyncKinBattleJoinNum(QLuaScript &sc)
	{
		DWORD dwRoleId = sc.GetInt(1);
		INT nJoinNum = sc.GetInt(2);
		INT nHasJoined = sc.GetInt(3);

		KPlayer* pPlayer = NULL;
		KActivityCtrl* pActivityCtrl = NULL;

		pPlayer = g_cPlayerMgr.GetById(dwRoleId);
		LOG_PROCESS_ERROR(pPlayer);

		pActivityCtrl = pPlayer->GetCtrl<KActivityCtrl>();
		LOG_PROCESS_ERROR(pActivityCtrl);

		pActivityCtrl->SyncKinBattleJoinNum(nJoinNum, nHasJoined);

	EXIT0:
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: ReloadKinBattleInfo
	// @Description		: 重新加载三界争霸信息
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaReloadKinBattleInfo(QLuaScript &sc)
	{
		g_cActivityManager.LoadKinBattleInfo();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetKinBattleWinNpcId
	// @Description		: 设置凌霄殿立的三界神皇的npcid
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: dwNpcId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetKinBattleWinNpcId(QLuaScript &sc)
	{
		DWORD dwNpcId = sc.GetInt(1);
		g_cActivityManager.SetKinBattleWinNpcId(dwNpcId);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetKinBattleWinnerId
	// @Description		: 得到本届三界神皇id
	// @ReturnCode		: 
	// @ArgumentFlag	: 
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetKinBattleWinnerId(QLuaScript &sc)
	{
		sc.PushNumber(g_cActivityManager.GetKinBattleWinnerId());

		return 1;
	}

	#include "kactivityscript_table.hpp"
}

BOOL g_RegisterActivityScript()
{
	return g_cScriptManager.RegisterTableFuncs("KActivity", KActivityScript::arFunction, countof(KActivityScript::arFunction));
}