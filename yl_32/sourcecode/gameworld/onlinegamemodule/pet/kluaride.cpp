
#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "kluaride.h"
#include "kride.h"
#include "kridectrl.h"
#include "onlinegameworld/kplayermgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: Id
// @Description		: 坐骑Id
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaRide::getId(VOID) CONST
{
	return	m_pRide->GetId();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: OwnerId
// @Description		: 坐骑主人Id
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaRide::getOwnerId(VOID) CONST
{
	return	m_pRide->GetOwnerId();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: Genre
// @Description		: 坐骑G
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::getGenre(VOID) CONST
{
	return	m_pRide->GetGenre();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: Detail
// @Description		: 坐骑D
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::getDetail(VOID) CONST
{
	return	m_pRide->GetDetail();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: Particular
// @Description		: 坐骑P
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::getParticular(VOID) CONST
{
	return	m_pRide->GetParticular();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: Level
// @Description		: 坐骑Level
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::getLevel(VOID) CONST
{
	return	m_pRide->GetLevel();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: Hungry
// @Description		: 饥饿度
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::getHungry(VOID) CONST
{
	return	m_pRide->GetHungry();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: State
// @Description		: 坐骑状态
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::getState(VOID) CONST
{
	return	m_pRide->GetState();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: MagicRate
// @Description		: 当前幻化果数量
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::getMagicRate(VOID) CONST
{
	return	m_pRide->GetMagicRate();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaRide
// @LuaApiName		: Name
// @Description		: 坐骑名字
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaRide::getName(VOID) CONST
{
	KITEM_INDEX sIndex = {m_pRide->GetGenre(), m_pRide->GetDetail(), m_pRide->GetParticular(), m_pRide->GetLevel()};

	KRideTemplate* pTemplate = g_cPetSetting.GetRideTemplate(sIndex);
	QCONFIRM_RET_FALSE(pTemplate);

	return	pTemplate->m_szName;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaRide
// @LuaApiName		: SetMagicRate
// @Description		: 设置当前幻化果数量
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::LuaSetMagicRate(QLuaScript& sc)
{
	INT nRate = sc.GetInt(1);

	BOOL bRet = m_pRide->SetMagicRate(nRate);

	sc.PushNumber(bRet);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaRide
// @LuaApiName		: Sync
// @Description		: 同步坐骑数据到客户端
// @ReturnCode		: 1成功
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::LuaSync(QLuaScript& sc)
{
	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_pRide->GetOwnerId());
	QCONFIRM_RET_FALSE(pPlayer);

	KRideCtrl* pRideCtrl = pPlayer->GetCtrl<KRideCtrl>();
	QCONFIRM_RET_FALSE(pRideCtrl);

	BOOL nRet = pRideCtrl->SyncRideInfo(m_pRide);

	sc.PushNumber(nRet);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaRide
// @LuaApiName		: AddHungry
// @Description		: 增加饱食度
// @ReturnCode		: 1成功
// @ArgumentFlag	: d
// @ArgumentComment	: nHungry	
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::LuaAddHungry(QLuaScript& sc)
{
	INT nHungry = sc.GetInt(1);

	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_pRide->GetOwnerId());
	QCONFIRM_RET_FALSE(pPlayer);

	KRideCtrl* pRideCtrl = pPlayer->GetCtrl<KRideCtrl>();
	QCONFIRM_RET_FALSE(pRideCtrl);

	BOOL nRet = pRideCtrl->AddHungry(m_pRide, nHungry);

	sc.PushNumber(nRet);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaRide
// @LuaApiName		: SetState
// @Description		: 设置坐骑状态
// @ReturnCode		: 1成功
// @ArgumentFlag	: d
// @ArgumentComment	: nState	
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::LuaSetState(QLuaScript& sc)
{
	INT nState = sc.GetInt(1);

	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_pRide->GetOwnerId());
	QCONFIRM_RET_FALSE(pPlayer);

	KRideCtrl* pRideCtrl = pPlayer->GetCtrl<KRideCtrl>();
	QCONFIRM_RET_FALSE(pRideCtrl);

	BOOL nRet = pRideCtrl->ChangeRideState(m_pRide, nState);

	sc.PushNumber(nRet);
	return	1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaRide
// @LuaApiName		: Regenerate
// @Description		: 重新生成一只坐骑
// @ReturnCode		: 成功返回1
// @ArgumentFlag	: dddd
// @ArgumentComment	: nGenre	
// @ArgumentComment	: nDetail	
// @ArgumentComment	: nParticular
// @ArgumentComment	: nLevel
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaRide::LuaRegenerate(QLuaScript& sc)
{
	INT nGenre			= sc.GetInt(1);
	INT nDetail		= sc.GetInt(2);
	INT nParticular	= sc.GetInt(3);
	INT nLevel			= sc.GetInt(4);

	BOOL bRet = m_pRide->Regenerate(
		nGenre,
		nDetail,
		nParticular,
		nLevel);

	sc.PushNumber(bRet);
	return	1;
}

#include "kluaride_table.hpp"