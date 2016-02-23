/* -------------------------------------------------------------------------
//	文件名		：	gclogicbasescript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"GlobalExecute", LuaGlobalExecute, "远程让所有GameServer调用脚本指令", "{s}[d]", "arg:要调用的函数，随后跟着调用参数，Table打包;[可选] 指定gs;", "发送成功：1，发送失败：0"},
	{"CreateSystemTeam", LuaCreateSystemTeam, "创建系统队伍并加入指定角色", "d[dddd]", "一个或更多玩家Id 作为队员;同上;同上;同上;同上;", "SystemTeamId"},
	{"GetPlayerIdByName", LuaGetPlayerIdByName, "通过名字获取玩家ID", "s", "玩家名字;", "playerId"},
	{"GetRoleInfoById", LuaGetRoleInfoById, "通过id获取玩家信息", "d", "玩家Id;", ""},
	{"GetMapPlayerCount", LuaGetMapPlayerCount, "得到地图上玩家个数", "d", "地图id;", "playerCount"},
	{"GetRankIdByIdx", LuaGetRankIdByIdx, "得到某个排行榜排行第几的id", "dd", "排行榜索引;第几名(start from 0);", ""},
	{"IsPlayerOnLine", LuaIsPlayerOnLine, "验证玩家是否在线", "d", "玩家ID;", "在线1不在线0"},
	{"Quit", LuaQuit, "服务器正常退出", "", "", "无"},
	{"SaveAll", LuaSaveAll, "CR存盘", "", "", "无"},
};
