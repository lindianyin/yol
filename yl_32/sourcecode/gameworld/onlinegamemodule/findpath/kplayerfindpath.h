
#pragma once

// -------------------------------------------------------------------------

#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kcharacterctrlbase.h"
#include "onlinegamemodule/findpath/kplayerfindpath_i.h"

class KLuaPlayerFindPath;

#ifdef GAME_CLIENT

#define MAX_NAVPATH_COUNT	256
struct KPLAYER_AUTOFINDPATH
{
	BOOL bAutoFindPath;	//玩家处于自动寻路阶段	
	INT nDestX;			//寻路目标位置
	INT nDestY;
	INT nDestZ;
	size_t nCurDestIndex; //当前阶段目标位置的索引
	KPLAYER_AUTOFINDPATH() : bAutoFindPath(FALSE), 
							 nDestX(0), 
							 nDestY(0), 
							 nDestZ(0),
							 nCurDestIndex(0)	{}
};

#endif

class KPlayerFindPath : public KCharacterCtrlBase, public IKPlayerFindPath
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_FINDPATH	};

	KPlayerFindPath(KCharacter& rCharacter);
	virtual ~KPlayerFindPath();

	KLuaPlayerFindPath* GetScriptInterface() { return m_pLuna; }

public:
	//重载框架
	BOOL Init();
	BOOL UnInit();
	BOOL Active();

	BOOL GoTo(INT nMapId, INT nX, INT nY, INT nZ);

#ifdef GAME_CLIENT

	//寻路接口
	BOOL LoadNavPath(const char* szFileName);
	BOOL StartAutoPath(INT nX, INT nY, INT nZ, BOOL bStopNearby);
	BOOL EndAutoPath(BOOL bFinish);

	//获取导航路径
	size_t GetNavPathLen() { return m_uNavPathLen; }
	float* GetNavPath()
	{ 
		if (m_uNavPathLen > 0)
			return &m_aNavPath[0];
		else
			return NULL;
	}

private:
	BOOL ProcessAutoPath();
	
	KPLAYER_AUTOFINDPATH m_cAutoPathState;
	float   m_aNavPath[MAX_NAVPATH_COUNT];
	size_t  m_uNavPathLen;
	BOOL    m_bStopNearby;

#endif	//#ifdef GAME_CLIENT

private:
	KPlayer& m_rPlayer;	
	KLuaPlayerFindPath* m_pLuna;
};


