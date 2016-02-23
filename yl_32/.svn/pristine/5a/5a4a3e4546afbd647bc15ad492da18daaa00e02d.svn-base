
#pragma once

#include "onlinegamebase/kscriptmanager.h"

// -------------------------------------------------------------------------
class KPlayer;
class KTaskCtrl;

class KLuaTask : public QLunaBase
{
public:
	explicit  KLuaTask(KPlayer& rPlayer);
	~KLuaTask() {}	

	static INT Register2Character(QLuaScript& sc, QLunaBase* pLunaObj);

	// 脚本接口定义
	DECLARE_LUA_CLASS(KLuaTask);
	INT		LuaGetTask(QLuaScript& sc);
	INT		LuaSetTask(QLuaScript& sc);
	INT		LuaDirectSetTask(QLuaScript& sc);	// 调试用
	INT		LuaGetTaskBit(QLuaScript& sc);
	INT		LuaSetTaskBit(QLuaScript& sc);
	INT		LuaClearTaskGroup(QLuaScript& sc);
	INT		LuaGetAllTask(QLuaScript& sc);
	INT		LuaSendAccept(QLuaScript& sc);
	INT		LuaSendAward(QLuaScript& sc);
	INT LuaGetTaskStr(QLuaScript& sc);
	INT LuaSetTaskStr(QLuaScript& sc);

#ifdef GAME_SERVER
	INT LuaSendRefresh(QLuaScript& sc);
	INT LuaStartProgressTimer(QLuaScript& sc);
	INT LuaStepStart(QLuaScript& sc);
	INT LuaSyncTaskGroup(QLuaScript& sc);
#else
#endif

private:
	KPlayer&	m_rcPlayer;
	KTaskCtrl*	m_pTaskCtrl;
};

