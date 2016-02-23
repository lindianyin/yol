

#include "stdafx.h"
#include "kpursedata.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "kluaplayerpurse.h"
//#include "GameCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
INT KLuaPlayerPurse::RegisterToPlayer(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLuaPlayer = static_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_NULL(NULL != pLuaPlayer);

	KPlayer* pPlayer = pLuaPlayer->GetHim();
	QCONFIRM_RET_NULL(NULL != pPlayer);

	KPlayerPurse* pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	sc.PushObj(pPlayerPurse->GetScriptInterface());
	return 1;
}

#ifdef GAME_SERVER

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: RegistCurrencyType
// @Description		: 注册珍币类型
// @ReturnCode		: 返回值
// @ArgumentFlag	: d
// @ArgumentComment	: 珍币类型
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaRegistCurrencyType(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	BYTE byCurrencyType = sc.GetInt(1);
	
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	sc.PushNumber(m_pPlayerPurse->RegistCurrencyType(byCurrencyType));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: AddCurrency
// @Description		: 往玩家身上加珍币
// @ReturnCode		: 加珍币返回值
// @ArgumentFlag	: dd
// @ArgumentComment	: 货币id
// @ArgumentComment	: 货币数量
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaAddCurrency(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	BYTE  byCurrencyID = sc.GetInt(1);
	INT   nCurrency    = sc.GetInt(2);
	
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	m_pPlayerPurse->AddCurrency(byCurrencyID, nCurrency, &nResult);
	sc.PushNumber(nResult);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: RegistMoneyType
// @Description		: 注册钱类型
// @ReturnCode		: 返回结果
// @ArgumentFlag	: d
// @ArgumentComment	: 钱类型
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaRegistMoneyType(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	BYTE byMoneyType = sc.GetInt(1);
	sc.PushNumber(m_pPlayerPurse->RegistMoneyType(byMoneyType));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: AddMoney
// @Description		: 给玩家加钱
// @ReturnCode		: 加钱返回值
// @ArgumentFlag	: dd
// @ArgumentComment	: 货币类型id
// @ArgumentComment	: 钱
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaAddMoney(QLuaScript& sc)
{
	QLogPrintf(LOG_INFO,"..KLuaPlayerPurse::LuaAddMoney");
	ASSERT(NULL != m_pPlayerPurse);
	BYTE  byMoneyType = (BYTE)sc.GetInt(1);
	INT   nMoney      = sc.GetInt(2);
	BOOL  isTinyTermal = m_pPlayerPurse->IsTinyTerminal();
	long long llAdd = 0;
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	m_pPlayerPurse->AddMoney(byMoneyType, nMoney, &nResult);
	if(byMoneyType ==  EM_MONEY_YLENERGY && isTinyTermal && nMoney)
	{
		llAdd = (long long)nMoney * 1.05 - nMoney;
		m_pPlayerPurse->AddMoney(byMoneyType, (INT)llAdd, &nResult);
	}
	
	//printf("sc.GetTopIndex():%d",sc.GetTopIndex());
	//if(sc.GetTopIndex() > 2)
	//{
	//	INT   nActionType = sc.GetInt(3);
	//	m_pPlayerPurse->LogMoney(byMoneyType, nMoney,nActionType, &nResult);//记录消耗的钱 
	//}
	sc.PushNumber(nResult);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: LogMoney
// @Description		: 记录玩家加钱
// @ReturnCode		: 加钱返回值
// @ArgumentFlag	: dd
// @ArgumentComment	: 货币类型id
// @ArgumentComment	: 钱
// @LuaMarkEnd
// -------------------------------------------------------------------------
//ldy:
INT KLuaPlayerPurse::LuaLogMoney(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	BYTE  byMoneyType = (BYTE)sc.GetInt(1);
	INT   nMoney      = sc.GetInt(2);
	INT   nActionType = sc.GetInt(3);
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	if(sc.GetTopIndex() >= 4)
	{
		char* szStr = (char*)sc.GetStr(4);
		m_pPlayerPurse->LogMoney(byMoneyType, nMoney,nActionType, &nResult,0,szStr);//记录消耗的钱
	}
	else
	{
		m_pPlayerPurse->LogMoney(byMoneyType, nMoney,nActionType, &nResult);//记录消耗的钱
	}
	sc.PushNumber(nResult);
	return 1;
}




// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: AddCoin
// @Description		: 给玩家加金币（金币由paysys同步，这个接口不起作用）
// @ReturnCode		: 加金币结果
// @ArgumentFlag	: d
// @ArgumentComment	: 金币
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaAddCoin(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	INT nCoin = sc.GetInt(1);
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	m_pPlayerPurse->AddCoin(nCoin, &nResult);
	sc.PushNumber(nResult);
	return 1;
}


#endif

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: GetCurrency
// @Description		: 获取某珍币数量
// @ReturnCode		: 数量值
// @ArgumentFlag	: d
// @ArgumentComment	: 珍币类型
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaGetCurrency(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	BYTE byCurrencyID = sc.GetInt(1);
	sc.PushNumber(m_pPlayerPurse->GetCurrency(byCurrencyID));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: GetMoney
// @Description		: 获取某类型钱数量
// @ReturnCode		: 钱数量
// @ArgumentFlag	: d
// @ArgumentComment	: 钱类型
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaGetMoney(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	BYTE byMoneyType = (BYTE)sc.GetInt(1);
	sc.PushNumber(m_pPlayerPurse->GetMoney(byMoneyType));
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: GetCoin
// @Description		: 获取金币
// @ReturnCode		: 数量
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaGetCoin(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	sc.PushNumber(m_pPlayerPurse->GetCoin());
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: GetCurrencyList
// @Description		: 获取玩家货币种类和数量
// @ReturnCode		: {{nCurrencyType, nCurrency},...}
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaGetCurrencyList(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	KPurseData::MPCURRENCYS* pCurrencyList = m_pPlayerPurse->GetPurseData()->GetCurrencyList();
	ASSERT(NULL != pCurrencyList);	
	
	if (0 == pCurrencyList->size())
	{
		sc.PushNull();
		return 1;
	}
	
	sc.PushTable();
	KPurseData::MPCURRENCYS::iterator it = pCurrencyList->begin();	
	for (UINT i = 1; it != pCurrencyList->end(); it++)
	{
		sc.PushTable();
		sc.PushNumber(it->first); 
		sc.SetTableField("nCurrencyType");
		sc.PushNumber(it->second);
		sc.SetTableField("nCurrency");
		sc.SetTableIndex(i++);
	}
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayerPurse
// @LuaApiName		: GetMoneyList
// @Description		: 获取钱种类和数量
// @ReturnCode		: {{nMoneyType, nMoney},....}
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayerPurse::LuaGetMoneyList(QLuaScript& sc)
{
	ASSERT(NULL != m_pPlayerPurse);
	KPurseData::MPMONEYS* pMoneyList = m_pPlayerPurse->GetPurseData()->GetMoneyList();
	QCONFIRM_RET_NULL(NULL != pMoneyList);

	if (0 == pMoneyList->size())
	{
		sc.PushNull();
		return 1;
	}

	sc.PushTable();
	KPurseData::MPCURRENCYS::iterator it = pMoneyList->begin();	
	for (UINT i = 1; it != pMoneyList->end(); it++)
	{
		sc.PushTable();
		sc.PushNumber(it->first); 
		sc.SetTableField("nMoneyType");
		sc.PushNumber(it->second);
		sc.SetTableField("nMoney");
		sc.SetTableIndex(i++);
	}
	return 1;
}

#include "kluaplayerpurse_table.hpp"
// -------------------------------------------------------------------------
