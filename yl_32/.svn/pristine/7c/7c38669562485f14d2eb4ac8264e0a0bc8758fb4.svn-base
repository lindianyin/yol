#pragma once

class KCharacter;

class LuaPlayerTitle : public QLunaBase
{
private:
	KCharacter& m_rcCharacter;

public:
	explicit LuaPlayerTitle(KCharacter& rcCharacter) : m_rcCharacter(rcCharacter){}
	~LuaPlayerTitle(){}

	static INT Register2Player(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaPlayerTitle);

	INT LuaAddTitleToCharacter(QLuaScript& sc);
	INT LuaRemoveTitleToCharacter(QLuaScript& sc);
	INT LuaSnycTitle(QLuaScript& sc);
	INT LuaClearTitle(QLuaScript& sc);

};