//#pragma once
//
//class LuaPlayerArena
//{
//public:
//	LuaPlayerArena(void);
//	~LuaPlayerArena(void);
//};
#pragma once

class KCharacter;

class LuaPlayerArena : public QLunaBase
{
private:
	KCharacter& m_rcCharacter;

public:
	explicit LuaPlayerArena(KCharacter& rcCharacter) : m_rcCharacter(rcCharacter){}
	~LuaPlayerArena(){}

	static INT Register2Player(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaPlayerArena);

	INT LuaStartArena(QLuaScript& sc);

	INT LuaLoadArenaPlayers(QLuaScript& sc);

	INT LuaLoadFrontArenaPlayers(QLuaScript& sc);

	INT LuaChallenge(QLuaScript& sc);

	INT LuaSyncArenaInfo(QLuaScript& sc);

	INT LuaGetCDTime(QLuaScript& sc);

	INT LuaSetCDTime(QLuaScript& sc);

	INT LuaGetWinCombo(QLuaScript& sc);

	INT LuaSetWinCombo(QLuaScript& sc);

	INT LuaSetTakeAwardTime(QLuaScript& sc);

	INT LuaGetTakeComboAward(QLuaScript& sc);

	INT LuaSetTakeComboAward(QLuaScript& sc);

	INT LuaClearData(QLuaScript& sc);

	INT LuaResetData(QLuaScript& sc);
};
