/* -------------------------------------------------------------------------
//	文件名		：	ktowerscript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	// 客户端/服务端0长度不能编译通过，放个没用的进来
	{"...", NULL, "", "", "", ""},

#ifdef GAME_SERVER
	{"RefreshEscortPlayers", LuaRefreshEscortPlayers, "", "d", "nPlayerId;", ""},
	{"ClearEscortData", LuaClearEscortData, "", "d", "nPlayerId;", ""},
	{"RemoveEscortWatch", LuaRemoveEscortWatch, "", "d", "nPlayerId;", ""},
	{"SpeedUpEscort", LuaSpeedUpEscort, "", "dd", "nPlayerId;bSpeed;", ""},
	{"SetLastRefreshTime", LuaSetLastRefreshTime, "", "?d", "nPlayerId/pPlayer:角色Id或者对象;dwTime;", ""},
	{"GetLastRefreshTime", LuaGetLastRefreshTime, "", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	
#endif // #ifdef GAME_SERVER
};
