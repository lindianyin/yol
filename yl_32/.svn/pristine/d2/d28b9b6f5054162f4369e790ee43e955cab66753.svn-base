/* -------------------------------------------------------------------------
//	文件名		：	kstagescript_table.hpp
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
	{"StartMopStage", LuaStartMopStage, "开始扫荡关卡", "od", "pPlayer;nTotalTime;", ""},
	{"GetRushStartTime", LuaGetRushStartTime, "得到开始挑战关卡时间", "o", "pPlayer;", ""},
	{"SetRushStartTime", LuaSetRushStartTime, "设置开始挑战关卡时间", "od", "pPlayer;开始挑战时间;", ""},
	{"GetRushLayer", LuaGetRushLayer, "得到当前挑战完的关卡数", "o", "pPlayer;", ""},
	{"SetRushLayer", LuaSetRushLayer, "设置当前挑战完的关卡数", "od", "pPlayer;关卡数;", ""},
	{"GetRushDifficulty", LuaGetRushDifficulty, "得到当前挑战的关卡难度", "o", "pPlayer;", ""},
	{"SetRushDifficulty", LuaSetRushDifficulty, "设置当前挑战关卡难度", "od", "pPlayer;关卡难度;", ""},
	{"SetStageStar", LuaSetStageStar, "设置挑战星级", "odd", "pPlayer;难度;星级;", ""},
	{"GetStageStar", LuaGetStageStar, "得到挑战星级", "od", "pPlayer;难度;", ""},
	{"GetLeftMopTime", LuaGetLeftMopTime, "得到剩余扫荡时间", "o", "pPlayer;", ""},
	{"StopMopStage", LuaStopMopStage, "点击停止扫荡关卡", "?d", "nPlayerId/pPlayer:角色Id或者对象;是否有扫完一层;", ""},
	{"FinishMop", LuaFinishMop, "直接完成扫荡", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"GetMopStage", LuaGetMopStage, "得到当前正在扫荡的关卡难度", "o", "pPlayer;", ""},
	{"GetMopState", LuaGetMopState, "得到当前扫荡状态", "o", "pPlayer;", ""},
	{"SetMopStage", LuaSetMopStage, "设置当前正在扫荡的关卡难度", "od", "pPlayer;关卡难度;", ""},
	{"GetRushState", LuaGetRushState, "得到当前闯关状态", "o", "pPlayer;", ""},
	{"SetRushState", LuaSetRushState, "", "od", "pPlayer;闯关;", ""},
	{"SpeedUpMop", LuaSpeedUpMop, "", "?", "pPlayer;", ""},
	{"GetSpeedUp", LuaGetSpeedUp, "", "?", "pPlayer;", ""},
#endif // #ifdef GAME_SERVER
};
