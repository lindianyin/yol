#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kcharactermgr.h"
#include "kawardctrl.h"
#include "kawardmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KAwardScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetAwardData
	// @Description		: 根据奖励id得到奖励数据
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: 奖励id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetAwardData(QLuaScript &sc)
	{
		KAward* pAward = g_cAwardMgr.GetAwardById(sc.GetInt(1));
		QCONFIRM_RET_FALSE(pAward);

		sc.PushTable();

		sc.PushString(pAward->GetModuleName());
		sc.SetTableField("szModuleName");

		sc.PushNumber(pAward->GetLevel());
		sc.SetTableField("nLevel");

		sc.PushNumber(pAward->GetSilver());
		sc.SetTableField("nSilver");

		sc.PushNumber(pAward->GetEnergy());
		sc.SetTableField("nEnergy");

		sc.PushNumber(pAward->GetFightSpirit());
		sc.SetTableField("nFightingSpirit");

		sc.PushNumber(pAward->GetExp());
		sc.SetTableField("nExp");

		sc.PushNumber(pAward->m_bGiveExp);
		sc.SetTableField("bGiveExp");

		sc.PushNumber(pAward->GetPrestige());
		sc.SetTableField("nPrestige");

		sc.PushTable();
		for(INT i = 0; i < pAward->GetItemNum(); ++i)
		{
			sc.PushTable();

			CONST KAwardItem* pItem = pAward->GetAwardItem(i);

			sc.PushNumber(pItem->nGenre);
			sc.SetTableField("nGenre");

			sc.PushNumber(pItem->nDetail);
			sc.SetTableField("nDetailType");

			sc.PushNumber(pItem->nParticular);
			sc.SetTableField("nParticular");

			sc.PushNumber(pItem->nLevel);
			sc.SetTableField("nLevel");

			sc.PushNumber(pItem->nAmount);
			sc.SetTableField("nAmount");

			sc.PushNumber(pItem->nTimeOut);
			sc.SetTableField("nTimeOut");

			sc.SetTableIndex(i + 1);
		}

		sc.SetTableField("pItem");

		return 1;

	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetAssetAwardData
	// @Description		: 根据奖励模板id得到奖励
	// @ReturnCode		: 
	// @ArgumentFlag	: sd
	// @ArgumentComment	: 奖励模块名称
	// @ArgumentComment	: 奖励模块level
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetAssetAwardData(QLuaScript &sc)
	{
		QString strModuleName(sc.GetStr(1));
		INT nLevel = sc.GetInt(2);
		DWORD dwExp = 0;
		CONST QAssetAward* pAssetAward = g_cAwardSetting.RequestAssetAward(strModuleName, nLevel);
		QCONFIRM_RET_FALSE(pAssetAward);

		sc.PushTable();

		sc.PushNumber(pAssetAward->nSilver);
		sc.SetTableField("nSilver");

		sc.PushNumber(pAssetAward->nEnergy);
		sc.SetTableField("nEnergy");

		sc.PushNumber(pAssetAward->nFightingSpirit);
		sc.SetTableField("nFightingSpirit");

		sc.PushNumber(pAssetAward->nExpAwardId);
		sc.SetTableField("nExpAwardId");

		sc.PushTable();
		for(INT i = 0; i < ASSET_AWARD_MAX_ITEM_NUM; ++i)
		{
			if(pAssetAward->arrAwardItem[i].nRate == 0)
				continue;

			KITEM_INDEX sIndex = pAssetAward->arrAwardItem[i].sIndex;

			sc.PushTable();

			sc.PushNumber(sIndex.nGenre);
			sc.SetTableField("nGenre");

			sc.PushNumber(sIndex.nDetailType);
			sc.SetTableField("nDetailType");

			sc.PushNumber(sIndex.nParticular);
			sc.SetTableField("nParticular");

			sc.PushNumber(sIndex.nLevel);
			sc.SetTableField("nLevel");

			sc.PushNumber(pAssetAward->arrAwardItem[i].nAmountMin);
			sc.SetTableField("nAmountMin");

			sc.PushNumber(pAssetAward->arrAwardItem[i].nAmountMax);
			sc.SetTableField("nAmountMax");

			sc.PushNumber(pAssetAward->arrAwardItem[i].nTimeOut);
			sc.SetTableField("nTimeOut");

			if(pAssetAward->arrAwardItem[i].nAmountMin == pAssetAward->arrAwardItem[i].nAmountMax)
			{
				sc.PushNumber(pAssetAward->arrAwardItem[i].nAmountMin);
				sc.SetTableField("nAmount");
			}

			sc.SetTableIndex(i + 1);
		}

		sc.SetTableField("pItem");

		return 1;

	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetLevelNumOfStage
	// @Description		: 得到某个stage的level数量
	// @ReturnCode		: 
	// @ArgumentFlag	: sd
	// @ArgumentComment	: 奖励模块名称
	// @ArgumentComment	: 奖励模块Stage名称
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetLevelNumOfStage(QLuaScript &sc)
	{
		QString strModuleName(sc.GetStr(1));
		INT nStage = sc.GetInt(2);
		INT nLevelNum = g_cAwardSetting.GetLevelNumOfStage(strModuleName, nStage);
		sc.PushNumber(nLevelNum);

		return 1;

	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetNumericalAwardData
	// @Description		: 根据奖励模板id得到奖励
	// @ReturnCode		: 
	// @ArgumentFlag	: sdd
	// @ArgumentComment	: 奖励模块名称
	// @ArgumentComment	: 奖励模块Stage名称
	// @ArgumentComment	: 奖励模块level
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetNumericalAwardData(QLuaScript &sc)
	{
		QString strModuleName(sc.GetStr(1));
		INT nStage = sc.GetInt(2);
		INT nLevel = sc.GetInt(3);
		CONST QNumericalAward* pAssetAward = g_cAwardSetting.RequestNumericalAward(strModuleName, nStage, nLevel);
		QCONFIRM_RET_FALSE(pAssetAward);

		sc.PushTable();

		sc.PushNumber(pAssetAward->nLife);
		sc.SetTableField("nLife");

		sc.PushNumber(pAssetAward->nAbility);
		sc.SetTableField("nAbility");

		sc.PushNumber(pAssetAward->nAttack);
		sc.SetTableField("nAttack");

		sc.PushNumber(pAssetAward->nDefense);
		sc.SetTableField("nDefense");
		
		sc.PushNumber(pAssetAward->nHit);
		sc.SetTableField("nHit");

		sc.PushNumber(pAssetAward->nDodge);
		sc.SetTableField("nDodge");

		sc.PushNumber(pAssetAward->nCrit);
		sc.SetTableField("nCrit");

		sc.PushNumber(pAssetAward->nResist);
		sc.SetTableField("nResist");

		sc.PushNumber(pAssetAward->nCost);
		sc.SetTableField("nCost");

		return 1;

	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddPlayerAward
	// @Description		: 角色获得一条奖励
	// @ReturnCode		: 
	// @ArgumentFlag	: osd[ddd]
	// @ArgumentComment	: pPlayer
	// @ArgumentComment	: 奖励模块名称
	// @ArgumentComment	: 奖励等级
	// @ArgumentComment	: 奖励次数
	// @ArgumentComment	: 奖励百分比
	// @ArgumentComment	: 是否有经验奖励
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddPlayerAward(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KAward* pAward = NULL;
		LPCSTR szModuleName = sc.GetStr(2);
		INT nLevel = sc.GetInt(3);
		INT nAwardPercent = 100;
		INT nTimes = 1;
		BOOL bGiveExp = TRUE;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		LOG_PROCESS_ERROR(pPlayer);

		if (sc.GetTopIndex() >= 4)
		{
			nTimes = sc.GetInt(4);
		}

		if (sc.GetTopIndex() >= 5)
		{
			nAwardPercent = sc.GetInt(5);
		}

		if(sc.GetTopIndex() >= 6)
		{
			bGiveExp = sc.GetInt(6);
		}

		pAward = g_cAwardMgr.AddPlayerAward(pPlayer, szModuleName, nLevel, nTimes, nAwardPercent, bGiveExp);
		LOG_PROCESS_ERROR(pAward);

	EXIT0:
		sc.PushNumber(pAward->GetId());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddRankAward
	// @Description		: 获得一条排名奖励
	// @ReturnCode		: 
	// @ArgumentFlag	: osdd
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: 奖励模块名称
	// @ArgumentComment	: 奖励等级
	// @ArgumentComment	: 排名
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddRankAward(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KAward* pAward = NULL;
		KAwardCtrl* pAwardCtrl = NULL;
		LPCSTR szModuleName = sc.GetStr(2);
		INT nLevel = sc.GetInt(3);
		INT nRankIdx = sc.GetInt(4);

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();

		LOG_PROCESS_ERROR(pPlayer);

		pAwardCtrl = pPlayer->GetCtrl<KAwardCtrl>();
		LOG_PROCESS_ERROR(pAwardCtrl);

		pAward = g_cAwardMgr.AddPlayerAward(pPlayer, szModuleName, nLevel);
		LOG_PROCESS_ERROR(pAward);

		pAward->SetRankIdx(nRankIdx);

		LOG_PROCESS_ERROR(pAwardCtrl->SyncAwardInfo(pAward));
	EXIT0:
		sc.PushNumber(pAward->GetId());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: DelAward
	// @Description		: 删除奖励
	// @ReturnCode		: 
	// @ArgumentFlag	: ?[d]
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: 奖励id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaDelAward(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		DWORD dwAwardId = 0;
		KAwardCtrl* pAwardCtrl = NULL;

		if (sc.IsNumber(1))
		{
			INT nPlayerId = sc.GetInt(1);
			QCONFIRM_RET_FALSE(IS_PLAYER(nPlayerId));
			pPlayer = (KPlayer*)g_cCharacterMgr.GetById(nPlayerId);
		}
		else
		{
			KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
			if (pLuaPlayer)
				pPlayer = pLuaPlayer->GetHim();
		}
		QCONFIRM_RET_FALSE(pPlayer);

		if(sc.IsNumber(2))
		{
			dwAwardId = sc.GetInt(2);
		}

		pAwardCtrl = pPlayer->GetCtrl<KAwardCtrl>();
		QCONFIRM_RET_FALSE(pAwardCtrl);

		pAwardCtrl->DelAward(dwAwardId);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetPlayerAwards
	// @Description		: 得到奖励Id列表，此函数必须和使用其id的函数在同一个lua函数里使用，Id不是永久的
	// @ReturnCode		: 
	// @ArgumentFlag	: ?[s]
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: 奖励id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPlayerAwards(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KAwardCtrl* pAwardCtrl = NULL;
		LPCSTR szModuleName = NULL;
		std::vector<DWORD> arrAwards;
		INT nIdx = 0;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		QCONFIRM_RET_FALSE(pPlayer);

		if(sc.IsString(2))
		{
			szModuleName = sc.GetStr(2);
		}

		pAwardCtrl = pPlayer->GetCtrl<KAwardCtrl>();
		QCONFIRM_RET_FALSE(pAwardCtrl);

		pAwardCtrl->GetPlayerAwards(arrAwards, szModuleName);

		sc.PushTable();
		for(std::vector<DWORD>::iterator it = arrAwards.begin(); it != arrAwards.end(); ++it)
		{
			sc.PushNumber(*it);
			sc.SetTableIndex(++nIdx);
		}

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: TakeAllAward
	// @Description		: 领取所有奖励
	// @ReturnCode		: 
	// @ArgumentFlag	: o[s]
	// @ArgumentComment	: 角色对象
	// @ArgumentComment	: 奖励模块名称
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaTakeAllAward(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		KAwardCtrl* pAwardCtrl = NULL;
		LPCSTR szModuleName = NULL;

		KLuaPlayer *pLuaPlayer = dynamic_cast<KLuaPlayer *>(sc.GetObj(1));
		if (pLuaPlayer)
			pPlayer = pLuaPlayer->GetHim();
		QCONFIRM_RET_FALSE(pPlayer);

		if(sc.IsString(2))
		{
			szModuleName = sc.GetStr(2);
		}

		pAwardCtrl = pPlayer->GetCtrl<KAwardCtrl>();
		QCONFIRM_RET_FALSE(pAwardCtrl);

		pAwardCtrl->TakeAllAward(szModuleName);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: HasAssetAward
	// @Description		: 判断是否有某等级的奖励
	// @ReturnCode		: 
	// @ArgumentFlag	: sd
	// @ArgumentComment	: 奖励模块名称
	// @ArgumentComment	: 奖励模块nLevel
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaHasAssetAward(QLuaScript &sc)
	{
		LPCTSTR szModuleName = sc.GetStr(1);
		INT nLevel = sc.GetInt(2);
		sc.PushNumber(g_cAwardSetting.HasAssetAward(szModuleName, nLevel));

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetExpByLevel
	// @Description		: 根据经验奖励id和等级得到经验值
	// @ReturnCode		: 
	// @ArgumentFlag	: dd
	// @ArgumentComment	: nExpAwardId
	// @ArgumentComment	: nNextLevel
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetExpByLevel(QLuaScript &sc)
	{
		INT nExpAwardId = sc.GetInt(1);
		INT nNextLevel = sc.GetInt(2);

		DWORD dwExp = g_cAwardSetting.GetExpByLevel(nExpAwardId, nNextLevel);
		sc.PushNumber(dwExp);

		return 1;
	}

	#include "kawardscript_table.hpp"
}

BOOL g_RegisterAwardScript()
{
	return g_cScriptManager.RegisterTableFuncs("KAward", KAwardScript::arFunction, countof(KAwardScript::arFunction));
}