#pragma once

class KCharacter;

class LuaPlayerGem : public QLunaBase
{
private:
	KCharacter& m_rcCharacter;

public:
	explicit LuaPlayerGem(KCharacter& rcCharacter) : m_rcCharacter(rcCharacter){}
	~LuaPlayerGem(){}

	static INT Register2Player(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaPlayerGem);
	INT LuaOnUse(QLuaScript& sc);
};