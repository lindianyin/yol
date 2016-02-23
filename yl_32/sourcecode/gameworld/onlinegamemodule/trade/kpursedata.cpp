
#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "kpursemodule.h"
#include "kpursedata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
VOID KPurseData::Clear()
{
	m_mpCurrencys.clear();
	m_mpMoneys.clear();
	m_nJbCoin = 0;
}

//货币
BOOL KPurseData::RegistCurrency(BYTE byCurrencyId)
{
	MPCURRENCYS::iterator iter = m_mpCurrencys.find(byCurrencyId);
	if (iter == m_mpCurrencys.end())
		m_mpCurrencys.insert(MPCURRENCYS::value_type(byCurrencyId, 0));
	return TRUE;
}

BOOL KPurseData::SetCurrency(BYTE byCurrencyId, INT nCurrency, KE_PURSE_RESULT* pResult)
{
	ASSERT(pResult);
	MPCURRENCYS::iterator iter = m_mpCurrencys.find(byCurrencyId);
	if (iter == m_mpCurrencys.end())
	{	
		*pResult = emKPURSE_E_CURRENCYTYPE_NOTEXIST;
		return FALSE;
	}
	INT nPlayerMaxCarryCurrency = GetPurseModule().GetMaxCarryCurrency();
	if (nCurrency > nPlayerMaxCarryCurrency)
	{
		m_mpCurrencys[byCurrencyId] = nPlayerMaxCarryCurrency;
		*pResult = emKPURSE_E_CURRENCYADD_OVERLIMIT;
		return TRUE;	//只要加了, 就返回true
	}
	if (nCurrency < INT_MIN) 
	{
		*pResult = emKPURSE_E_CURRENCYADD_OVERLIMIT;
		return FALSE;
	}
	if (nCurrency < 0)
	{
		*pResult = emKPURSE_E_CURRENCYCOST_NOENOUGH;
		return FALSE;
	}
	
	*pResult = emKPURSE_S_SUCCESS;
	m_mpCurrencys[byCurrencyId] = nCurrency;
	return TRUE;
}

INT  KPurseData::GetCurrency(BYTE byCurrencyId)
{
	MPCURRENCYS::iterator it = m_mpCurrencys.find(byCurrencyId);
	if (it == m_mpCurrencys.end())
	{
		Q_Error("[Purse]Player not Registe Currency Type: " << byCurrencyId << "\n");
		return 0;
	}
	return it->second;
}

//钱
BOOL KPurseData::RegistMoney(BYTE byMoneyType)
{
	MPMONEYS::iterator it = m_mpMoneys.find(byMoneyType);
	if (it == m_mpMoneys.end())
		m_mpMoneys.insert(MPMONEYS::value_type(byMoneyType, 0));
	
	return TRUE;
}

INT  KPurseData::GetMoney(BYTE byMoneyType)
{
	MPMONEYS::iterator it = m_mpMoneys.find(byMoneyType);
	if (it == m_mpMoneys.end())
	{	
		Q_Error("[Purse]Player not Registe Money Type: " << byMoneyType << "\n");
		return 0;
	}	
	return it->second;
}

BOOL KPurseData::SetMoney(BYTE byMoneyType, INT nMoney, KE_PURSE_RESULT* pResult)
{
	ASSERT(pResult);
	MPMONEYS::iterator it = m_mpMoneys.find(byMoneyType);
	if (it == m_mpMoneys.end())
	{
		*pResult = emKPURSE_E_MONEYTYPE_NOTEXIST;
		return FALSE;
	}	
	if (nMoney > INT_MAX || nMoney < INT_MIN)
	{
		*pResult = emKPURSE_E_MONEYADD_OVERLIMIT;
		return FALSE;
	}
	if (nMoney < 0)
	{
		*pResult = emKPURSE_E_MONEYCOST_NOENOUGH;
		return FALSE;
	}
	m_mpMoneys[byMoneyType] = nMoney;
	return TRUE;
}

BOOL KPurseData::SetCoin(INT nCoin, KE_PURSE_RESULT* pResult)
{
	ASSERT(pResult);
	if (nCoin > INT_MAX)
	{
		*pResult = emKPURSE_E_COINADD_OVERLIMIT;
		return FALSE;
	}
	if (nCoin < 0)
	{
		*pResult = emKPURSE_E_COINCOST_NOENOUGH;			
		return FALSE;
	}
	m_nJbCoin = nCoin;
	return TRUE;
}

KPurseData::MPCURRENCYS* KPurseData::GetCurrencyList()
{
	return &m_mpCurrencys;
}

KPurseData::MPMONEYS* KPurseData::GetMoneyList()
{
	return &m_mpMoneys;
}

// -------------------------------------------------------------------------
