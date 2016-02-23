/* -------------------------------------------------------------------------
//	文件名		：	kactivityscript_table.hpp
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
	{"SyncActivityTime", LuaSyncActivityTime, "同步活动时间", "odddd", "pPlayer;nActivityIdx;nBaseActivityIdx;dwOpenTime;dwCloseTime;", ""},
	{"SetLastEnterZBTTime", LuaSetLastEnterZBTTime, "得到上次进入战宝堂的时间", "od", "pPlayer;time;", ""},
	{"GetLastEnterZBTTime", LuaGetLastEnterZBTTime, "得到上次进入战宝堂的时间", "o", "pPlayer;", ""},
	{"GetLastMPJJRank", LuaGetLastMPJJRank, "活动最近门派竞技排名", "o", "pPlayer;", ""},
	{"SetLastMPJJRank", LuaSetLastMPJJRank, "设置最近门派竞技排名", "od", "pPlayer;rank;", ""},
	{"GetKinBattleWinTimes", LuaGetKinBattleWinTimes, "得到三界争霸连胜场数", "o", "pPlayer;", ""},
	{"SetKinBattleWinTimes", LuaSetKinBattleWinTimes, "设置三界争霸连胜场数", "od", "pPlayer;times;", ""},
	{"AddKinBattleRecord", LuaAddKinBattleRecord, "新增一条三界神皇记录", "d", "dwRoleId;", ""},
	{"SetKinBattleWinnerId", LuaSetKinBattleWinnerId, "设置三界神皇id", "d", "dwWinnerId;", ""},
	{"SetLastWinKinInfo", LuaSetLastWinKinInfo, "设置最近一次赢得三界争霸家族信息", "d", "dwWinnerId;", ""},
	{"SetKinBattleWinNpcId", LuaSetKinBattleWinNpcId, "设置凌霄殿立的三界神皇的npcid", "d", "dwNpcId;", ""},
	{"SyncKinBattleJoinNum", LuaSyncKinBattleJoinNum, "发送当前三界争霸参赛人数", "ddd", "dwRoleId;nJoinNum;nHasJoined;", ""},
	{"ReloadKinBattleInfo", LuaReloadKinBattleInfo, "重新加载三界争霸信息", "", "", ""},
	{"GetKinBattleWinnerId", LuaGetKinBattleWinnerId, "得到本届三界神皇id", "", "", ""},
#endif // #ifdef GAME_SERVER
};
