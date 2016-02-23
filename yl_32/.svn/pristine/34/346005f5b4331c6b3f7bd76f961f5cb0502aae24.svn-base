
#pragma once

// -------------------------------------------------------------------------

interface IKPlayerFindPathClient
{
	virtual BOOL LoadNavPath(const char* szFileName)   PURE;
	virtual BOOL StartAutoPath(INT nX, INT nY, INT nZ, BOOL bStopNearby) PURE;
	virtual BOOL EndAutoPath(BOOL bFinish) PURE;
};

interface IKPlayerFindPathServer
{
	virtual BOOL GoTo(INT nMapId, INT nX, INT nY, INT nZ) PURE;
};

#ifdef GAME_CLIENT
	typedef IKPlayerFindPathClient IKPlayerFindPath;
#else
	typedef IKPlayerFindPathServer IKPlayerFindPath;
#endif


IKPlayerFindPath* GetPlayerFindPath(KPlayer* pPlayer);


