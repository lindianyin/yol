/* -------------------------------------------------------------------------
//	文件名		：	kawardscript_table.hpp
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
	{"GetAwardData", LuaGetAwardData, "根据奖励id得到奖励数据", "d", "奖励id;", ""},
	{"GetAssetAwardData", LuaGetAssetAwardData, "根据奖励模板id得到奖励", "sd", "奖励模块名称;奖励模块level;", ""},
	{"GetLevelNumOfStage", LuaGetLevelNumOfStage, "得到某个stage的level数量", "sd", "奖励模块名称;奖励模块Stage名称;", ""},
	{"GetNumericalAwardData", LuaGetNumericalAwardData, "根据奖励模板id得到奖励", "sdd", "奖励模块名称;奖励模块Stage名称;奖励模块level;", ""},
	{"AddPlayerAward", LuaAddPlayerAward, "角色获得一条奖励", "osd[ddd]", "pPlayer;奖励模块名称;奖励等级;奖励次数;奖励百分比;是否有经验奖励;", ""},
	{"AddRankAward", LuaAddRankAward, "获得一条排名奖励", "osdd", "pPlayer;奖励模块名称;奖励等级;排名;", ""},
	{"DelAward", LuaDelAward, "删除奖励", "?[d]", "nPlayerId/pPlayer:角色Id或者对象;奖励id;", ""},
	{"GetPlayerAwards", LuaGetPlayerAwards, "得到奖励Id列表，此函数必须和使用其id的函数在同一个lua函数里使用，Id不是永久的", "?[s]", "nPlayerId/pPlayer:角色Id或者对象;奖励id;", ""},
	{"TakeAllAward", LuaTakeAllAward, "领取所有奖励", "o[s]", "角色对象;奖励模块名称;", ""},
	{"HasAssetAward", LuaHasAssetAward, "判断是否有某等级的奖励", "sd", "奖励模块名称;奖励模块level;", ""},
	{"GetExpByLevel", LuaGetExpByLevel, "根据经验奖励id和等级得到经验值", "dd", "nExpAwardId;nNextLevel;", ""},
#endif // #ifdef GAME_SERVER
};
