/* -------------------------------------------------------------------------
//	文件名		：	kinstancescript_table.hpp
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
	{"GetMultiStartTime", LuaGetMultiStartTime, "得到开始多人副本时间", "o", "pPlayer;", ""},
	{"SetMultiStartTime", LuaSetMultiStartTime, "设置开始多人副本时间", "od", "pPlayer;开始挑战时间;", ""},
	{"GetMultiState", LuaGetMultiState, "得到当前多人副本状态", "o", "pPlayer;", ""},
	{"SetMultiState", LuaSetMultiState, "", "od", "pPlayer;闯关;", ""},
	{"GetMultiRushStar", LuaGetMultiRushStar, "得到多人副本评星", "o", "pPlayer;", ""},
	{"SetMultiRushStar", LuaSetMultiRushStar, "", "od", "pPlayer;评星;", ""},
	{"GetMultiRushMapId", LuaGetMultiRushMapId, "得到多人副本地图id", "o", "pPlayer;", ""},
	{"SetMultiRushMapId", LuaSetMultiRushMapId, "", "od", "pPlayer;评星;", ""},
	{"IsMultiMapPassed", LuaIsMultiMapPassed, "是否通关了mapId的多人副本", "od", "pPlayer;mapId;", ""},
	{"SetMultiMapPassed", LuaSetMultiMapPassed, "设置mapId的多人副本已通关状态", "od", "pPlayer;mapId;", ""},
#endif // #ifdef GAME_SERVER
};
