#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "luaplayerartifact.h"
#include "artifactdef.h"

#define JINGUBANG_MODULE_NAME "shenqi_jinkubang"
#define HOUYIGONG_MODULE_NAME "shenqi_houyigong"
#define YUJINPING_MODULE_NAME "shenqi_yujinping"
#define RULAIYIN_MODULE_NAME "shenqi_rulaiyin"

#define MAX_LEVEL (5)
#define START_LEVEL_ID (10000)

struct ArtifactSaveData
{
	INT nId;
	INT nLevel;
	INT nCount;			//已注入数量
	INT nCanLevelUp;	//是否可以升级
};

class PlayerArtifact : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_ARTIFACT	};

	explicit PlayerArtifact(KCharacter& rcPlayer);
	~PlayerArtifact();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// 读盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 存盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	LuaPlayerArtifact* GetScriptInterface() CONST;

private:

	KPlayer&						m_rcPlayer;
	LuaPlayerArtifact* m_pLunaObj;

	std::map<INT, ArtifactSaveData> m_mapSaveData;

public:
	//加碎片
	BOOL AddPiece(INT nId, INT nCount);

	BOOL UsePiece(INT nId, INT nCount);

	BOOL LevelUp(INT nId);

	INT GetArtifactNumByLevel(INT nLevel);

	BOOL AddAttributeOfPiece(INT nId, INT nMinValue, INT nMaxValue);

	BOOL AddAttributeOfLevel(INT nId, INT nMinLevel, INT nMaxLevel);

	BOOL SyncArtifact();

};