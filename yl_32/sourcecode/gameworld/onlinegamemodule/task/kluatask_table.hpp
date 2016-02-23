/* -------------------------------------------------------------------------
//	文件名		：	kluatask_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaTask)
	REGISTER_LUA_FUNC(GetTask, "获取任务变量", "uu", "wGroupID:任务变量组ID;wTaskID:任务变量组内的子ID;", "成功-返回变量值，失败-返回nil")
	REGISTER_LUA_FUNC(SetTask, "设置任务变量", "uud[d]", "wGroupID:任务变量组ID;wTaskID:任务变量组内的子ID;nTaskValue:变量值;nSyncMode:同步模式(0:Auto, 1:Force, -1:No);", "无返回值")
	REGISTER_LUA_FUNC(DirectSetTask, "直接设置任务变量", "uud[d]", "wGroupID:任务变量组ID;wTaskID:任务变量组内的子ID;nTaskValue:变量值;nSyncMode:同步模式(0:Auto, 1:Force, -1:No);", "无返回值")
	REGISTER_LUA_FUNC(GetTaskBit, "将一组所有任务变量(65536个)的值按位排列(0-65536*32)，按位计算出TaskID(每32位为一个ID)，并获取对应变量值的位值", "uu", "wGroupID:任务变量组ID;dwBit:指定的位(范围：0-65536*32);", "成功-返回指定位的值，失败-返回nil")
	REGISTER_LUA_FUNC(SetTaskBit, "将一组所有任务变量(65536个)的值按位排列(0-65536*32)，按位计算出TaskID(每32位为一个ID)，并设置对应变量值的位值", "uud", "wGroupID:任务变量组ID;dwBit:指定的位(范围：0-65536*32);bValue:位值(0 or 1);", "无返回值")
	REGISTER_LUA_FUNC(ClearTaskGroup, "清除一组任务变量", "u[d]", "wGroupID:任务变量组ID;bSync:是否同步到客户端(服务端有效)(0 or 1);", "无返回值")
	REGISTER_LUA_FUNC(GetAllTask, "获取全部Task变量", "", "", "{[nTaskId]=nValue,[nTaskId]=nValue,...}")
	REGISTER_LUA_FUNC(SendAccept, "向服务端/客户端发送接受任务请求", "dd[d]", "nTaskId:主任务ID;nReferId:引用子任务ID;bAccept:是否接受;", "1、成功；nil、失败")
	REGISTER_LUA_FUNC(SendAward, "向服务端/客户端发送任务奖励请求", "dd[d]", "nTaskId:主任务ID;nReferId:引用子任务ID;nSelIdx:所选项的索引;", "1、成功；nil、失败")
	REGISTER_LUA_FUNC(GetTaskStr, "获取任务变量(每次使用4个任务变量,最长16个字符长度,角色名的长度)", "uu", "wGroupID:任务变量组ID;wTaskID:任务变量组内的子ID,需要4个子ID,此ID为起点ID;", "成功-返回变量值，失败-返回nil")
	REGISTER_LUA_FUNC(SetTaskStr, "设置任务变量(每次使用8个任务变量,最长32个字符长度)", "uus[d]", "wGroupID:任务变量组ID;wTaskID:任务变量组内的子ID,需要4个子ID,此ID为起点ID;szTaskValue:变量值(32个字符,16个汉字,角色名长度8个汉字,预留8个汉字越南版);;nSyncMode:同步模式(0:Auto, 1:Force, -1:No);", "无返回值")

#ifdef GAME_SERVER
	REGISTER_LUA_FUNC(SendRefresh, "向客户端发送刷新要求", "dd[d]", "nTaskId:主任务ID;nReferId:引用子任务ID;nTaskGroupID:该任务保存的TaskGroup;", "1、成功；nil、失败")
	REGISTER_LUA_FUNC(StartProgressTimer, "开启一个计时器，并设置客户端的进度条", "ods", "pPlayer;间隔时间;描述文字;", "无")
	REGISTER_LUA_FUNC(StepStart, "通知客户端目标开始", "ddd", "任务Id;子任务Id;步骤数;", "无")
	REGISTER_LUA_FUNC(SyncTaskGroup, "同步一组任务变量", "u", "任务变量组ID;", "成功：1, 失败：0")
#endif // #ifdef GAME_SERVER
DEFINE_LUA_CLASS_END()
