#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kcharactermgr.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "kpet.h"
#include "kpetmanager.h"
#include "kpetctrl.h"
#include "kridectrl.h"
#include "kluapet.h"
#include "kluaride.h"
#include "../onlinegamemodule/bladestate/kbladectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

namespace KPetScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddPlayerPet
	// @Description		: 给指定角色一只新的宠物
	// @ReturnCode		: 成功返回宠物对象，失败返回nil
	// @ArgumentFlag	: ?d[d]
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: nType:宠物表现id
	// @ArgumentComment	: nCharacter
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddPlayerPet(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		INT nType = sc.GetInt(2);
		//INT nQuality = sc.GetInt(3);
		INT nCharacter = 0;
		if (sc.IsNumber(3))
		{
			nCharacter = sc.GetInt(3);
		}

		KPet* pPet = g_cPetMgr.AddPlayerPet(pPlayer, nType, nCharacter);
		if(pPet)
		{
			sc.PushObj((QLunaBase*)pPet->GetScriptInterface());
			return 1;
		}

		return 0;
	}
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetPetById
	// @Description		: 通过Id得到Pet
	// @ReturnCode		: o
	// @ArgumentFlag	: d
	// @ArgumentComment	: PetId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPetById(QLuaScript& sc)
	{
		DWORD dwId = (DWORD)sc.GetInt(1);

		KPet* pPet = g_cPetMgr.GetPetById(dwId);
		if (!pPet)
			return 0;

		sc.PushObj((QLunaBase*)pPet->GetScriptInterface());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetPetByTypeAndNature
	// @Description		: 通过表现id和法系得到Pet
	// @ReturnCode		: o
	// @ArgumentFlag	: dd
	// @ArgumentComment	: 表现id
	// @ArgumentComment	: 法系
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPetByTypeAndNature(QLuaScript& sc)
	{
		INT nType = sc.GetInt(1);
		INT nNature = sc.GetInt(2);

		KPet* pPet = g_cPetMgr.GetPetByTypeAndNature(nType, nNature);
		if (!pPet)
			return 0;

		sc.PushObj((QLunaBase*)pPet->GetScriptInterface());

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: FreePet
	// @Description		: 放生宠物
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: PetId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaFreePet(QLuaScript& sc)
	{
		DWORD dwId = (DWORD)sc.GetInt(1);

		KPet* pPet = g_cPetMgr.GetPetById(dwId);
		if (!pPet)
			return 0;

		KPlayer* pPlayer = (KPlayer*)g_cCharacterMgr.GetById(pPet->GetOwnerId());
		QCONFIRM_RET_NULL(pPlayer);

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_NULL(pPetCtrl);

		pPetCtrl->FreePet(dwId);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetPlayerPetList
	// @Description		: 得到角色宠物id列表
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPlayerPetList(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_NULL(pPetCtrl);

		INT nCount = pPetCtrl->GetPetsCount();
		sc.PushTable();
		for(INT i = 0; i < nCount; ++i)
		{
			KPet* pPet = pPetCtrl->GetPetByIdx(i);
			QCONFIRM_RET_NULL(pPet);
			sc.PushNumber(pPet->GetId());
			sc.SetTableIndex(i+1);
		}

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetPlayerPetMaxPower
	// @Description		: 得到所有宠物中最大战斗力
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPlayerPetMaxPower(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_NULL(pPetCtrl);

		//INT nCount = pPetCtrl->GetPetsCount();
		INT nMaxPower = pPetCtrl->GetMaxPower();
		sc.PushNumber(nMaxPower);
		/*sc.PushTable();
		for (INT i = 0; i < nCount; ++i)
		{
			KPet* pPet = pPetCtrl->GetPetByIdx(i);
			QCONFIRM_RET_NULL(pPet);
			sc.PushNumber(pPet->GetId());
			sc.SetTableIndex(i + 1);
		}*/

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetFightPet
	// @Description		: 得到玩家当前正在出战的宠物
	// @ReturnCode		: 成功返回宠物对象，失败返回nil
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetFightPet(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);
		KPet* pPet = pPetCtrl->GetCurrentFightPet();
		if(pPet)
		{
			sc.PushObj((QLunaBase*)pPet->GetScriptInterface());
			return 1;
		}

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddPlayerRide
	// @Description		: 给指定角色一只新的坐骑
	// @ReturnCode		: 成功返回坐骑对象，失败返回nil
	// @ArgumentFlag	: ?dddd[dd]
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: nGenre
	// @ArgumentComment	: nDetail
	// @ArgumentComment	: nParticular
	// @ArgumentComment	: nLevel
	// @ArgumentComment	: nHungry
	// @ArgumentComment	: nMagicRate
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddPlayerRide(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		INT nGenre = sc.GetInt(2);
		INT nDetail = sc.GetInt(3);
		INT nParticular = sc.GetInt(4);
		INT nLevel = sc.GetInt(5);
		INT nHungry = KD_RIDE_HUNGRY_MAX;
		INT nMagicRate = 0;

		if(sc.IsNumber(6))
			nHungry = sc.GetInt(6);
		if(sc.IsNumber(7))
			nMagicRate = sc.GetInt(7);

		KRide* pRide = g_cPetMgr.AddPlayerRide(pPlayer, nGenre, nDetail, nParticular, nLevel, nHungry, nMagicRate);
		if(pRide)
		{
			sc.PushObj((QLunaBase*)pRide->GetScriptInterface());
			return 1;
		}

		return 0;
	}


	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: TakeOffRide
	// @Description		: 卸下人物坐骑
	// @ReturnCode		: 成功返回1
	// @ArgumentFlag	: ?d
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: 坐骑ID
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaTakeOffRide(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		DWORD dwRideId = sc.GetInt(2);
		KRide* pRide = g_cPetMgr.GetRideById(dwRideId);
		QCONFIRM_RET_FALSE(pRide);

		KRideCtrl* pRideCtrl = pPlayer->GetCtrl<KRideCtrl>();
		QCONFIRM_RET_FALSE(pRideCtrl);

		BOOL bRet = pRideCtrl->TakeOffRide(pRide);

		g_cCharacterHelper.CalcFightScore(*pPlayer);

		sc.PushNumber(bRet);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetRide
	// @Description		: 得到玩家的当前坐骑
	// @ReturnCode		: 成功返回坐骑对象，失败返回nil
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetRide(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KRideCtrl* pRideCtrl = pPlayer->GetCtrl<KRideCtrl>();
		QCONFIRM_RET_FALSE(pRideCtrl);
		KRide* pRide = pRideCtrl->GetCurrentRide();
		if(pRide)
		{
			sc.PushObj((QLunaBase*)pRide->GetScriptInterface());
			return 1;
		}
		
		return 0;
	}

// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetPetNpcIdByType
	// @Description		: 获取宠物对应的NPC编号
	// @ReturnCode		: 返回INT
	// @ArgumentFlag	: d
	// @ArgumentComment	: nType 宠物ID
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPetNpcIdByType(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
		INT nPetType = sc.GetInt(1);
		INT nNpcId = g_cPetSetting.GetPetNpcIdByType(nPetType);
		sc.PushNumber(nNpcId);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetPetIslandRefreshRate
	// @Description		: 获取宠物岛刷新成功率
	// @ReturnCode		: 成功返回成功率，失败返回nil
	// @ArgumentFlag	: ?d
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: nQuality指定品质
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPetIslandRefreshRate(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		INT nQuality = sc.GetInt(2);

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);

		sc.PushNumber(pPetCtrl->m_arrPetRefreshRate[nQuality - 1]);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SetPetIslandRefreshRate
	// @Description		: 设置宠物岛刷新成功率
	// @ReturnCode		: 成功返回1，失败返回0
	// @ArgumentFlag	: ?dd
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: nQuality指定品质
	// @ArgumentComment	: nValue指定值
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSetPetIslandRefreshRate(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		INT nQuality = sc.GetInt(2);

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);

		pPetCtrl->m_arrPetRefreshRate[nQuality - 1] = sc.GetInt(3);

		pPetCtrl->SyncPetIslandRefreshState(0); // 同步

		sc.PushNumber(1);

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetPetIslandRefreshCount
	// @Description		: 获取宠物岛刷新过的次数
	// @ReturnCode		: 成功返回刷新次数，失败返回nil
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPetIslandRefreshCount(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		INT nQuality = sc.GetInt(2);

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);

		sc.PushNumber(pPetCtrl->m_nPetIslandRefreshCount);

		return 1;
	}
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: UpgradePetSkill
	// @Description		: 升级神仙技能
	// @ReturnCode		: 成功返回1,失败返回0
	// @ArgumentFlag	: dd
	// @ArgumentComment	: 神仙Id
	// @ArgumentComment	: 神仙技能Id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaUpgradePetSkill(QLuaScript &sc)
	{
		INT nPetId = sc.GetInt(1);
		KPet* pPet = g_cPetMgr.GetPetById((DWORD)nPetId);
		QCONFIRM_RET_FALSE(pPet);

		INT nSkillId = sc.GetInt(2);
        BOOL bRet = pPet->UpgradeSkill(nSkillId);
		sc.PushNumber(bRet);

		return 1;
	}
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddPetIslandRefreshCount
	// @Description		: 增加宠物岛刷新过的次数+1
	// @ReturnCode		: 成功返回刷新次数，失败返回nil
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddPetIslandRefreshCount(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);
		pPetCtrl->m_nPetIslandRefreshCount++;

		sc.PushNumber(pPetCtrl->m_nPetIslandRefreshCount);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RecordPetIslandCurrentInfo
	// @Description		: 记录此次召唤神仙信息
	// @ReturnCode		: 
	// @ArgumentFlag	: ?dddd
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: 当前刷出的索引
	// @ArgumentComment	: 品质
	// @ArgumentComment	: 法系
	// @ArgumentComment	: 是否在战斗状态
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRecordPetIslandCurrentInfo(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);

		KPetCtrl::LastPetIslandInfo cPetInfo = pPetCtrl->GetLastPetIslandInfo();

		// 负值表示保留原来的值
		INT nValue = sc.GetInt(2);
		cPetInfo.wLastPetIndex = nValue >= 0 ? nValue : cPetInfo.wLastPetIndex;

		nValue = sc.GetInt(3);
		cPetInfo.wLastPetQuality = nValue >= 0 ? (WORD)nValue : cPetInfo.wLastPetQuality;

		nValue = sc.GetInt(4);
		cPetInfo.wLastPetNature = nValue >= 0 ? (WORD)nValue : cPetInfo.wLastPetNature;

		nValue = sc.GetInt(5);
		cPetInfo.wLastFightState = nValue >= 0 ? (WORD)nValue : cPetInfo.wLastFightState;

        pPetCtrl->m_nPetIslandRefreshCount = cPetInfo.wLastPetIndex;
		pPetCtrl->SyncPetIslandRefreshState(cPetInfo.wLastPetQuality);
		pPetCtrl->SetLastPetIslandInfo(cPetInfo);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetPetIslandLastInfo
	// @Description		: 记录此次召唤神仙信息
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPetIslandLastInfo(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);

		CONST KPetCtrl::LastPetIslandInfo& cPetInfo = pPetCtrl->GetLastPetIslandInfo();
		sc.PushTable();

		sc.PushNumber(cPetInfo.wLastPetIndex);
		sc.SetTableField("nCurrentLuckyValue");

		sc.PushNumber(cPetInfo.wLastPetQuality);
		sc.SetTableField("nCurrentQuality");
		
		sc.PushNumber(cPetInfo.wLastPetNature);
		sc.SetTableField("nLastNature");

		sc.PushNumber(cPetInfo.wLastFightState);
		sc.SetTableField("nLastFightState");

		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: SendRefreshPetInfo
	// @Description		: 记录身上宠物的状态
	// @ReturnCode		: 
	// @ArgumentFlag	: ??
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @ArgumentComment	: 刷出的神仙卡牌信息
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaSendRefreshPetInfo(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);

		QCONFIRM_RET_FALSE(sc.IsTable(2));
	
	    QPET_CARD cCard;
		std::vector<QPET_CARD> cPetCards;
		INT nIndex = 0;
		for (INT i = 1; i <= 10; i++)
		{
			sc.GetTableIndex(2, i * 2 - 1);
			nIndex = sc.GetTopIndex();
			cCard.nPetId = sc.GetInt(nIndex);

			sc.GetTableIndex(2, i * 2);
			nIndex = sc.GetTopIndex();
			cCard.nPetNature = sc.GetInt(nIndex);

			cPetCards.push_back(cCard);
		}

		pPetCtrl->SyncPetCardInfo(cPetCards);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RecordPetsLastState
	// @Description		: 记录身上宠物的状态
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRecordPetsLastState(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);

		pPetCtrl->RecordPetsLastState();

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: ResumePetsLastState
	// @Description		: 恢复之前身上宠物状态
	// @ReturnCode		: 
	// @ArgumentFlag	: ?
	// @ArgumentComment	: nPlayerId/pPlayer:角色Id或者对象
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaResumePetsLastState(QLuaScript &sc)
	{
		KPlayer* pPlayer = NULL;
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

		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);

		pPetCtrl->ResumePetsLastState();

		return 0;
	}

	#include "kpetscript_table.hpp"
}

BOOL g_RegisterPetScript()
{
	return g_cScriptManager.RegisterTableFuncs("KPet", KPetScript::arFunction, countof(KPetScript::arFunction));
}