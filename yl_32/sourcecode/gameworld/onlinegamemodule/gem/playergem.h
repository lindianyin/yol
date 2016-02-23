#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "luaplayergem.h"
#include "gemdef.h"

struct GemSaveData
{
	INT nCount;
};

class PlayerGem : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_GEM	};

	explicit PlayerGem(KCharacter& rcPlayer);
	~PlayerGem();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// ∂¡≈Ã
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// ¥Ê≈Ã
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	LuaPlayerGem* GetScriptInterface() CONST;

private:

	KPlayer&						m_rcPlayer;
	LuaPlayerGem* m_pLunaObj;

	std::map<KITEM_INDEX, GemSaveData> m_mapSaveData;

	BOOL SyncGemInfo();

public:

	BOOL OnUse(INT nGenre,INT nDetailType, INT nParticular, INT nLevel);
	BOOL AddAttribute(KITEM_INDEX &sIndex, INT nCount);
};