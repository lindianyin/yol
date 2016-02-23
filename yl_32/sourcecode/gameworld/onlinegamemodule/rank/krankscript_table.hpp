/* -------------------------------------------------------------------------
//	文件名		：	krankscript_table.hpp
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
	{"ModifyRankValue", LuaModifyRankValue, "", "ddd", "排行榜id（排行唯一依据）;nRankId;nValue;", ""},
	{"SetRank", LuaSetRank, "设置排名", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"RefreshRoleRank", LuaRefreshRoleRank, "刷新战斗力排行", "", "", ""},
	{"RefreshPetLvRank", LuaRefreshPetLvRank, "刷新宠物等级排行", "", "", ""},
	{"RefreshPetFSRank", LuaRefreshPetFSRank, "刷新宠物战斗力排行", "", "", ""},
	{"RefreshRideRank", LuaRefreshRideRank, "刷新坐骑排行", "", "", ""},
	{"RefreshMpjjRank", LuaRefreshMpjjRank, "刷新门派竞技排行", "", "", ""},
	{"RefreshLGRank", LuaRefreshLGRank, "刷新炼卦排行", "", "", ""},
	{"RefreshSmzcRank", LuaRefreshSmzcRank, "刷新神魔战场排行", "", "", ""},
	{"RefreshTongFSRank", LuaRefreshTongFSRank, "刷新家族战斗力排行", "", "", ""},
	{"RefreshTongSrRank", LuaRefreshTongSrRank, "刷新家族贡献度排行", "", "", ""},
	{"GetMyRank", LuaGetMyRank, "得到我的排名", "?d", "nPlayerId/pPlayer:角色Id或者对象;nRankId;", ""},
	{"GetLastTakeAwardTime", LuaGetLastTakeAwardTime, "得到上次领奖时间", "?d", "nPlayerId/pPlayer:角色Id或者对象;nRankId;", ""},
	{"SetLastTakeAwardTime", LuaSetLastTakeAwardTime, "设置上次领奖时间", "?dd", "nPlayerId/pPlayer:角色Id或者对象;nRankId;time;", ""},
	{"RefreshAllRank", LuaRefreshAllRank, "刷新全部排行榜", "", "", ""},
#endif // #ifdef GAME_SERVER
};
