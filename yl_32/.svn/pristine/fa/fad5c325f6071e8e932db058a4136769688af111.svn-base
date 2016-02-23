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
	{"StartMopTower", LuaStartMopTower, "开始扫塔", "?d", "nPlayerId/pPlayer:角色Id或者对象;扫塔需要时间;", ""},
	{"StopMopTower", LuaStopMopTower, "点击停止扫塔", "?d", "nPlayerId/pPlayer:角色Id或者对象;是否有扫完一层;", ""},
	{"GetSpeedUp", LuaGetSpeedUp, "得到是否在加速", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"GetLeftMopTime", LuaGetLeftMopTime, "得到扫荡剩余时间", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"GetMopStartTime", LuaGetMopStartTime, "得到扫荡开始时间", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"FinishMop", LuaFinishMop, "直接完成扫荡", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"SpeedUpMop", LuaSpeedUpMop, "加速扫荡", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"SyncTowerRank", LuaSyncTowerRank, "同步排行榜", "?", "", ""},
#endif // #ifdef GAME_SERVER
};
