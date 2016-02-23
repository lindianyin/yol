/* -------------------------------------------------------------------------
//	文件名		：	ktaskscript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"SendAccept", LuaSendAccept, "向服务端/客户端发送接受任务请求", "odd[d]", "pPlayer;nTaskId:主任务ID;nReferId:引用子任务ID;bAccept:是否接受;", "1、成功；nil、失败"},
	{"Dump", LuaDump, "输出任务变量信息", "o", "pPlayer;", ""},
	{"SendAward", LuaSendAward, "向服务端/客户端发送任务奖励请求", "odd[d]", "pPlayer;nTaskId:主任务ID;nReferId:引用子任务ID;nSelIdx:所选项的索引;", "1、成功；nil、失败"},

#ifdef GAME_SERVER
	{"SendRefresh", LuaSendRefresh, "向客户端发送刷新要求", "odd[d]", "pPlayer;nTaskId:主任务ID;nReferId:引用子任务ID;nTaskGroupID:该任务保存的TaskGroup;", "1、成功；nil、失败"},
	{"StartProgressTimer", LuaStartProgressTimer, "开启一个计时器，并设置客户端的进度条", "ods", "pPlayer;间隔时间;描述文字;", "无"},
	{"StepStart", LuaStepStart, "通知客户端目标开始", "oddd", "玩家对象;任务Id;子任务Id;步骤数;", "无"},
	{"SetBlackSky", LuaSetBlackSky, "通知客户端开启或结束黑屏模式", "od", "pPlayer;是否开启黑屏模式;", "无"},
#endif // #ifdef GAME_SERVER
};
