
#pragma once

// -------------------------------------------------------------------------
#include "onlinegamemodule/trade/kplayerpurse_i.h"

class KPurseData
{
public:
	typedef std::map<BYTE, INT>  MPCURRENCYS;	//珍币: 材料货币
	typedef std::map<BYTE, INT>  MPMONEYS;		//钱: 绑金，银两, 游龙真气等

	KPurseData() : m_nJbCoin(0) 
	{
		m_mpCurrencys.clear();
		ASSERT(0 == m_mpCurrencys.size());
		m_mpMoneys.clear();
		ASSERT(0 == m_mpMoneys.size());
	}

public:
	
	VOID Clear();

	// 材料货币
	BOOL RegistCurrency(BYTE byCurrencyId);
	BOOL SetCurrency(BYTE byCurrencyId, INT nCurrency, KE_PURSE_RESULT* pResult);
	INT  GetCurrency(BYTE byCurrencyId);	

	// 普通游戏币, 包括游龙真气
	BOOL RegistMoney(BYTE byMoneyType);	
	BOOL SetMoney(BYTE byMoneyType, INT nMoney, KE_PURSE_RESULT* pResult);
	INT  GetMoney(BYTE byMoneyType);
	
	//金币不存盘，由bishop同步过来, 比较特殊的钱
	INT  GetCoin() { return m_nJbCoin; }
	BOOL SetCoin(INT nCoin, KE_PURSE_RESULT* pResult);
	
	MPCURRENCYS* GetCurrencyList();
	MPMONEYS*    GetMoneyList();

private:	
	MPCURRENCYS	m_mpCurrencys;	
	MPMONEYS	m_mpMoneys;	
	INT			m_nJbCoin;
};

// -------------------------------------------------------------------------

