/* -------------------------------------------------------------------------
//	文件名		：	kteamscriptfuns_table.hpp
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
	{"GetTeamMemberList", LuaGetTeamMemberList, "获取队伍成员列表", "d", "队伍ID;", "成员ID数组，成员数量"},
	{"GetTeamLeader", LuaGetTeamLeader, "获取队长Id", "d", "队伍ID;", ""},
	{"CreateTeam", LuaCreateTeam, "自己创建一个队伍", "d", "角色Id;", ""},
	{"DisbandTeam", LuaDisbandTeam, "解散队伍", "dd", "队长id;队伍id;", ""},
#endif // #ifdef GAME_SERVER
};
