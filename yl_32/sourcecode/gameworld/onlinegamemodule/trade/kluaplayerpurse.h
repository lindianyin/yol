
#pragma once

// -------------------------------------------------------------------------
class KPlayerPurse;

class KLuaPlayerPurse : public QLunaBase
{
public:
	explicit KLuaPlayerPurse(KPlayerPurse* pPlayerPurse) : m_pPlayerPurse(pPlayerPurse)
	{}
	~KLuaPlayerPurse() {}

	static INT RegisterToPlayer(QLuaScript& sc, QLunaBase* pLunaObj);

	// 脚本接口定义
	DECLARE_LUA_CLASS(KLuaPlayerPurse);

	//珍币
	INT LuaGetCurrency(QLuaScript& sc);	
	
	//钱
	INT LuaGetMoney(QLuaScript& sc);
	
	//金币	
	INT LuaGetCoin(QLuaScript& sc);
	
	//珍币列表
	INT LuaGetCurrencyList(QLuaScript& sc);

	//钱列表
	INT LuaGetMoneyList(QLuaScript& sc);

#ifdef GAME_SERVER
	//珍币
	INT LuaRegistCurrencyType(QLuaScript& sc);
	INT LuaAddCurrency(QLuaScript& sc);

	//钱
	INT LuaRegistMoneyType(QLuaScript& sc);
	INT LuaAddMoney(QLuaScript& sc);
	INT LuaLogMoney(QLuaScript& sc);

	//金币
	INT LuaAddCoin(QLuaScript& sc);

#endif

private:
	KPlayerPurse* m_pPlayerPurse;
};


