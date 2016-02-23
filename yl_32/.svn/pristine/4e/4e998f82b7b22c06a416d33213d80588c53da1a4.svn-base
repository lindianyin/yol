
#pragma once

#ifdef GAME_CLIENT
// -------------------------------------------------------------------------
class KCharacter;
struct KCHARACTER_VAR_SYNC_SELF;
struct KCHARACTER_VAR_SYNC_OTHER;
// -------------------------------------------------------------------------
class KCharacterClient
{
public:
	KCharacterClient(KCharacter& rCharacter) : m_cCharacter(rCharacter) {};
	// 同步加载变量
	BOOL LoadSyncVarSelf(KCHARACTER_VAR_SYNC_SELF& sSyncVar);
	BOOL LoadSyncVarOther(KCHARACTER_VAR_SYNC_OTHER& sSyncVar);
protected:
	KCharacter& m_cCharacter;
};

// -------------------------------------------------------------------------
#endif // #ifdef GAME_CLIENT

