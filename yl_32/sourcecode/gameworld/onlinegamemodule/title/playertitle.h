#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "luaplayertitle.h"

struct TitleSaveData
{
	INT nTitleId;
	INT nCount;
	BOOL bOwn;
	BOOL bUse;
	DWORD dwGetTime;
};

class PlayerTitle : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_TITLE	};

	explicit PlayerTitle(KCharacter& rcPlayer);
	~PlayerTitle();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// ∂¡≈Ã
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// ¥Ê≈Ã
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	LuaPlayerTitle* GetScriptInterface() CONST;


	BOOL RemoveTitleToCharacter(INT nGetWay, INT nLevel, INT nType);

	BOOL AddTitleToCharacter1(INT nGetWay, INT nLevel, INT nType, DWORD dwPassTime);

	BOOL AddTitleToCharacter2(INT nGetWay, INT nLevel, INT nType, DWORD dwPassTime);

	BOOL SyncTitle();

	BOOL SendAddTitle(INT nTitleId);

	BOOL SetTitleCarray_1(INT nOldValue, INT nNewValue);

	BOOL SetTitleCarray_2(INT nOldValue, INT nNewValue);

	BOOL SetTitleCarray_3(INT nOldValue, INT nNewValue);


	//test
	BOOL ClearSavaData();

private:
	KPlayer&						m_rcPlayer;

	LuaPlayerTitle* m_pLunaObj;

	std::map<INT, TitleSaveData>	m_mapTitleSaveData;
};