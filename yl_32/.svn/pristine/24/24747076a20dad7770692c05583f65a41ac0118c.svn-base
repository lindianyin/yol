#pragma once

class KCharacter;

class LuaPlayerOpense : public QLunaBase
{
private:
	KCharacter& m_rcCharacter;

public:
	explicit LuaPlayerOpense(KCharacter& rcCharacter) : m_rcCharacter(rcCharacter){}
	~LuaPlayerOpense(){}

	static INT Register2Player(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaPlayerOpense);

	INT LuaDoOpense(QLuaScript& sc);
};