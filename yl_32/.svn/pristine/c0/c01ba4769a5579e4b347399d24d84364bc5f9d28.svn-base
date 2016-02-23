#pragma once

class KCharacter;

class LuaPlayerVip : public QLunaBase
{
private:
	KCharacter& m_rcCharacter;

public:
	explicit LuaPlayerVip(KCharacter& rcCharacter) : m_rcCharacter(rcCharacter){}
	~LuaPlayerVip(){}

	static INT Register2Player(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaPlayerVip);

	INT LuaRefreshBuyCount(QLuaScript& sc);

	INT LuaDoBuyCount(QLuaScript& sc);

	INT LuaCanBuyCount(QLuaScript& sc);

	INT LuaIsOpen(QLuaScript& sc);

	INT LuaGetRate(QLuaScript& sc);

	INT LuaSetFreeState(QLuaScript& sc);

	INT LuaGetNowBuyCount(QLuaScript& sc);

	INT LuaAddVipExpAndMoney(QLuaScript& sc);
};