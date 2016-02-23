#ifndef __ITEMENV_H__
#define __ITEMENV_H__

#ifdef GAME_SERVER
#include "itemenv_i.h"

class KPlayer;

// 角色相关的物件环境
class KItemEnvPlayer : public IItemEnv
{
protected:
	KPlayer *m_pPlayer;
	CHAR	 m_szBuff[1024];
public:
	KItemEnvPlayer(KPlayer *pPlayer) : m_pPlayer(pPlayer) 
	{
		//ASSERT(pPlayer); 
	}
	virtual LPCSTR GetEnvMsg();
	LPCSTR GetAccount();
	LPCSTR GetRole();
};

// 交易相关的物件环境
template <INT nPorotocol>
class KItemEnvAuction : public KItemEnvPlayer
{
public:
	KItemEnvAuction(KPlayer *pPlayer) : KItemEnvPlayer(pPlayer) {}
	virtual LPCSTR GetEnvMsg()
	{
		snprintf(
			m_szBuff, sizeof(m_szBuff) - 1,
			"AuctionProtocol:%d, Account:%s, Role:%s",
			nPorotocol,
			GetAccount(),
			GetRole());
		m_szBuff[sizeof(m_szBuff) - 1] = 0;
		return m_szBuff;
	}
};

// 摆摊相关的物件环境
class KItemEnvBuySell : public KItemEnvPlayer
{
	INT m_nCurrencyType;
public:
	KItemEnvBuySell(KPlayer *pPlayer) : KItemEnvPlayer(pPlayer), m_nCurrencyType(0)
	{
		ASSERT(pPlayer); 
	}
	VOID CurrencyType(INT nCurrencyType)
	{
		m_nCurrencyType = nCurrencyType;
	}
	virtual LPCSTR GetEnvMsg()
	{
		snprintf(
			m_szBuff, sizeof(m_szBuff) - 1,
			"CurrencyType:%d, Account:%s, Role:%s",
			m_nCurrencyType,
			GetAccount(),
			GetRole());
		m_szBuff[sizeof(m_szBuff) - 1] = 0;
		return m_szBuff;
	}
};

class KItemEnvCustom : public KItemEnvPlayer
{
	LPCSTR m_pszPrefix;
public:
	KItemEnvCustom(KPlayer *pPlayer) : KItemEnvPlayer(pPlayer), m_pszPrefix(NULL)
	{
	}
	VOID Prefix(LPCSTR pszPrefix)
	{
		m_pszPrefix = pszPrefix;
	}
	virtual LPCSTR GetEnvMsg()
	{
		snprintf(
			m_szBuff, sizeof(m_szBuff) - 1,
			"%s Account:%s, Role:%s",
			m_pszPrefix,
			GetAccount(),
			GetRole());
		m_szBuff[sizeof(m_szBuff) - 1] = 0;
		return m_szBuff;
	}
};

#endif //_SERVER
#endif //__ITEMENV_H__
