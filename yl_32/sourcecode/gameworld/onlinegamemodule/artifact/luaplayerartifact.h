#pragma once

class KCharacter;

class LuaPlayerArtifact : public QLunaBase
{
private:
	KCharacter& m_rcCharacter;

public:
	explicit LuaPlayerArtifact(KCharacter& rcCharacter) : m_rcCharacter(rcCharacter){}
	~LuaPlayerArtifact(){}

	static INT Register2Player(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaPlayerArtifact);

	INT LuaAddPiece(QLuaScript& sc);

	INT LuaUsePiece(QLuaScript& sc);

	INT LuaGetArtifactNumByLevel(QLuaScript& sc);
};