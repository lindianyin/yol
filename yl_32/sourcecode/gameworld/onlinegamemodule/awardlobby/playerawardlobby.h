#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "luaplayerawardlobby.h"
#include "awardlobbydef.h"

struct AwardLobbyActivity
{
	INT nId;
	INT nAward;
};

class PlayerAwardLobby : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_AWARD_LOBBY	};

	explicit PlayerAwardLobby(KCharacter& rcPlayer);
	~PlayerAwardLobby();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// 读盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 存盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	LuaPlayerAwardLobby* GetScriptInterface() CONST;

private:

	KPlayer&						m_rcPlayer;
	LuaPlayerAwardLobby* m_pLunaObj;

private:
	INT m_nSignedIn;						// 每位表示一天是否已经签到
	TIME_T m_dwLastSignedInTime;				//上次签到时间

	INT m_nLoginDays;						//连续登陆天数
	INT m_nMaxLoginDays;					//当前最大连续登陆天数
	TIME_T m_dwLastLoginTime;				//上次改变登陆天数时间
	std::map<INT, AwardLobbyActivity> m_mapActivity;


public:
	BOOL PlayerSignedIn(INT nDay);

	BOOL IfHasSignedIn(INT nDay);

	INT GetSignedInCount();

	VOID SetLoginDays();

	BOOL DoFinishActivity(INT nId, INT nAwardId);

	BOOL DoFinishRankActivity(INT nId, INT nAwardId);

	BOOL DoRecharge(INT nCoins);

	BOOL SyncSignedInInfo();

	BOOL SyncSevenDays();

	BOOL SyncActivity();
};