
#pragma once

#include <time.h>
#include "ikbadwordfilter.h"

// 游戏帧数更新
class IKLoopCounter
{
public:
	virtual VOID Loop(DWORD& dwGameLoop) = 0;
};

// 时间服务
class IKSysService
{
public:
	virtual void			Release() = 0;
	virtual time_t			Time(time_t* pTimer) = 0;					// == time()
	virtual time_t			GameTime(time_t* pTimer) = 0;					// in-game time
	virtual void			SetGameTime(time_t* pTimer) = 0;					// in-game time
	virtual DWORD			GetTickCount() = 0;							// == GetTickCount()
	virtual DWORD			Rand(INT nMax) = 0;							// == rand()
	virtual DOUBLE			Random() = 0;
	virtual IKBadwordFilter*	CreateBadWordFilter() = 0;
	virtual INT             GetTimeDiffOfDay(time_t tBegin,time_t tEnd) = 0;
};

struct IKSocketAcceptorEvent // : IUnknown
{
	virtual INT OnStartWait() = 0;
	virtual INT OnFinishWait(INT nResult) = 0;
	virtual INT OnClientConnect(INT nConnectIdx) = 0;
	virtual INT OnClientDataCome(INT nConnectIdx) = 0;
	virtual INT OnClientDisConnect(INT nConnectIdx) = 0;
};

