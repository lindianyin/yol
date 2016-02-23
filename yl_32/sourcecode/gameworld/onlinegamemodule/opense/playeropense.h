#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "luaplayeropense.h"

#define KDF_OPEN_CFG			"/setting/opense/open.ini"

#pragma	pack(1)
struct OpenseSaveData
{
	INT nDay;		//第几天
	INT nId;		//id， 为0表示是完成当天的总奖励
	INT nFinish;	//是否完成
	INT nGive;		//是否发奖励
	INT nCount;		//次数
};
#pragma pack()


class PlayerOpense : public KCharacterCtrlBase
{
public:
	enum
	{
		emKCtrlId = emKCHARACTER_CTRL_OPENSE
	};

	explicit PlayerOpense(KCharacter& rcPlayer);
	~PlayerOpense();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// 读盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 存盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	LuaPlayerOpense* GetScriptInterface() CONST;

	BOOL DoOpense(INT nDay, INT nId);

	BOOL SyncOpense();

	BOOL GetAward(INT nDay, INT nId);

	BOOL SendAllFinish();
private:
	OpenseSaveData* GetOpenseSaveData(INT nDay, INT nId);

private:
	KPlayer&						m_rcPlayer;

	std::vector<OpenseSaveData>		m_vecSaveData;

	LuaPlayerOpense* m_pLunaObj;
};