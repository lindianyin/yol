#pragma once

class KCharacter;

class LuaPlayerTarget : public QLunaBase
{
private:
	KCharacter& m_rcCharacter;

public:
	explicit LuaPlayerTarget(KCharacter& rcCharacter) : m_rcCharacter(rcCharacter){}
	~LuaPlayerTarget(){}

	static INT Register2Player(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaPlayerTarget);

	INT LuaDoTarget(QLuaScript& sc);
};