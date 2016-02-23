
#pragma once

#ifdef GAME_SERVER
class KCharacter;
class KPlayerProtocolProcessor;
// -------------------------------------------------------------------------
class KCharacterServer
{
public:
	KCharacterServer(KCharacter& rCharacter) : m_rcCharacter(rCharacter) {};
	// 同步单个变量到周围玩家
	BOOL SyncVarAround(WORD wVarGroup, WORD wVarId);
	// 广播到附近玩家 (bIncludeHimself是否广播到他自己的客户端)
	BOOL BroadcastNearby(LPCVOID pbyData, SIZE_T uSize,
		BOOL bIncludeHimself, KPlayerProtocolProcessor* pProtocolSender = NULL);
	// 广播到本地图玩家 (bIncludeHimself是否广播到他自己的客户端)
	BOOL BroadcastScene(LPCBYTE pbyData, SIZE_T uSize,
		BOOL bIncludeHimself, KPlayerProtocolProcessor* pProtocolSender = NULL);

	// 广播到本服所有人 不包含此玩家pPlayerToExclude
	static BOOL BroadcastThisServer(LPCBYTE pbyData, SIZE_T uSize, 
		KCharacter* pPlayerToExclude, KPlayerProtocolProcessor* pProtocolSender = NULL);

	// 根据同步属性同步变量改变
	BOOL SyncVarChange(WORD wVarGroup, WORD wVarId);
	VOID SetVisible(BOOL bVisible);
	VOID SetSkillHidden(BOOL bHidden);

	BOOL SyncActionAround(INT nActionId, INT nPendantId);
private:
	virtual BOOL DoSyncVarSelf(WORD wVarGroup, WORD wVarId) { return TRUE; }
protected:
	KCharacter& m_rcCharacter;
};

// -------------------------------------------------------------------------
#endif // #ifdef GAME_SERVER
