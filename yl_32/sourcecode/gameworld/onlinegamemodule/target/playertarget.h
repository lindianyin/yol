#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "luaplayertarget.h"


struct TargetSaveData
{
	INT nDay;		//第几天
	INT nId;		//id， 为0表示是完成当天的总奖励
	INT nFinish;	//是否完成
	INT nGive;		//是否发奖励
	INT nCount;		//次数
};

class PlayerTarget : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_TARGET	};

	explicit PlayerTarget(KCharacter& rcPlayer);
	~PlayerTarget();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// 读盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 存盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	LuaPlayerTarget* GetScriptInterface() CONST;

	BOOL DoTarget(INT nDay, INT nId);

	BOOL SyncTarget();

	BOOL GetAward(INT nDay, INT nId);

	BOOL SendAllFinish();
private:
	TargetSaveData* GetTargetSaveData(INT nDay, INT nId);

private:
	KPlayer&						m_rcPlayer;

	std::vector<TargetSaveData>		m_vecSaveData;

	LuaPlayerTarget* m_pLunaObj;
};