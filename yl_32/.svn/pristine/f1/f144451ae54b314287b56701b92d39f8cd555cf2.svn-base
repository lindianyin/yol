/* -------------------------------------------------------------------------
//	文件名		：	kgamebasescript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"Sleep", LuaSleep, "Debug: 使该线程停止一段时间", "d", "dwMilliSeconds;", ""},
	{"WriteLog", LuaWriteLog, "写脚本日志", "ds", "日志重要等级 LOG_RECORD_REMIND_LEVEL;内容;", ""},
	{"WriteLogServer", LuaWriteLogServer, "写logserver日志", "ds", "日志类型;内容;", ""},
	{"GetPlayerByName", LuaGetPlayerByName, "通过角色名获得Player", "s", "szName;", "o"},
	{"GetPlayerById", LuaGetPlayerById, "通过Id获得Player", "d", "dwId;", "o"},
	{"SaveAllPlayers", LuaSaveAllPlayers, "存盘当前在线玩家", "", "", ""},
	{"SavePlayerDelayData", LuaSavePlayerDelayData, "存储玩家离线数据", "", "", ""},
	{"GetNpcById", LuaGetNpcById, "通过Id获得Npc", "d", "dwId;", "o"},
	{"GetNpcByNickName", LuaGetNpcByNickName, "通过别名获得Npc", "ds", "dwSceneId;szNickName;", "o"},
	{"GetDoodadByNickName", LuaGetDoodadByNickName, "通过别名获得Doodad", "ds", "dwSceneId;szNickName;", "o"},
	{"GetNameByTemplateId", LuaGetNameByTemplateId, "根据Npc的模板ID，返回NpcName", "d", "nNpcTemplateID:Npc的模板ID;", "成功返回npc名称，失败返回空"},
	{"GetNpcTitleByTemplateId", LuaGetNpcTitleByTemplateId, "根据Npc的模板ID，返回NpcTitle", "d", "Npc的模板ID;", "成功返回npc名称，失败返回空"},
	{"FindNpcWithTemplateId", LuaFindNpcWithTemplateId, "根据模板ID查找Npc", "ud", "dwSceneId 场景Id;nNpcTemplateId: Npc的模板Id;", "返回列表{o}"},
	{"GetDoodadNameByTemplateId", LuaGetDoodadNameByTemplateId, "根据Doodad的模板ID，返回DoodadName", "d", "nDoodadTemplateID:Doodad的模板ID;", "成功返回Doodad名称，失败返回空"},
	{"GetCharacterById", LuaGetCharacterById, "通过Id得到Character", "d", "CharacterId;", "o"},
	{"GetDoodadById", LuaGetDoodadById, "通过Id得到Doodad", "d", "DoodadId;", "o"},
	{"IsNpc", LuaIsNpc, "通过Id判断是否Npc", "d", "dwId;", "y/n"},
	{"IsPlayer", LuaIsPlayer, "通过Id判断是否Player", "d", "dwId;", "y/n"},
	{"GetTime", LuaGetTime, "获得当前时间（秒数）", "", "", "从1970年1月1日0时算起的秒数（UTC时区，即格林威治时间）"},
	{"EnableWorldDebug", LuaEnableWorldDebug, "允许/禁止场景等内部调试信息", "d", "1 允许 0 禁止;", ""},
	{"GetPlayerCount", LuaGetPlayerCount, "获得玩家数", "", "", "d"},
	{"GetNpcCount", LuaGetNpcCount, "获得Npc数", "", "", "d"},
	{"GetDoodadCount", LuaGetDoodadCount, "获得Doodad数", "", "", "d"},
	{"GetAllPlayers", LuaGetAllPlayers, "获取在线玩家快照（不要存储pPlayer，因为player对象会重用）", "", "", "*o"},
	{"GetServerId", LuaGetServerId, "获取服务器线Id", "", "", "d"},
	{"GetServerStartTime", LuaGetServerStartTime, "得到开服时间", "", "", "d"},
	

	
#ifdef GAME_SERVER
	{"CallNpc", LuaCallNpc, "召唤Npc", "ddddd[d]", "dwNpcTemplateId;dwSceneId;nX;nY;nZ;nFaceDirection 朝向（可选）;", ""},
	{"AddNpc", LuaAddNpc, "召唤Npc", "odddd[d]", "npc目标;dwSceneId;nX;nY;nZ;nFaceDirection 朝向（可选）;", ""},
	{"SetNpcRegenerationInfo", LuaSetNpcRegenerationInfo, "设置NPC的重生参数", "dddddddd[ds]", "dwNpcId;nX;nY;nZ;nFaceDirection;nRegenerationTime;nNormalAIType;nRelation;dwExValue;szExInfo;", "1/0"},
	{"RemoveNpc", LuaRemoveNpc, "删除Npc", "d[d]", "dwNpcId;bRegenerate;", ""},
	{"GetDistance", LuaGetDistance, "得到两点之间的距离的平方", "dddd", "X1;Y1;X2;Y2;", ""},
	{"StartOfflineFight", LuaStartOfflineFight, "", "dddddd", "dwStartPlayer;dwOfflinePlayer;dwStartRoleMapNpcId;dwStartPetMapNpcId;dwOfflineRoleMapNpcId;dwOfflinePetMapNpcId;", ""},
	{"GetPosition", LuaGetPosition, "根据方向和距离得到xyz点的目标点", "ddddd", "nX;nY;nZ;nDirection;nDistance;", ""},
	{"GetDirection", LuaGetDirection, "得到两个点的方向", "dddd", "nX1;nY1;nX2;nY2;", ""},
	{"CallDoodad", LuaCallDoodad, "创建Doodad", "dd???", "dwDoodadTemplateId;掉落发起者（一般是谁最后一击）;拥有得到掉落的资格玩家列表，会根据掉落发起者和掉落模式筛选出一个可见列表;table {dwSceneId = XXX, nX = XX, nY = XX, nZ = XX};table {nGenre = XXX, nDetail = XX, nParticular = XX, nLevel = XX, nAmount = XX};", ""},
	{"RemoveDoodad", LuaRemoveDoodad, "删除Doodad", "d", "dwDoodadId;", ""},
	{"GetNextLevelExp", LuaGetNextLevelExp, "", "d", "nLevel;", ""},
	{"Shutdown", LuaShutdown, "关闭服务器", "", "", ""},
	{"ReloadGmList", LuaReloadGmList, "重载本GS的GM白名单", "", "", ""},
	{"IsDevMode", LuaIsDevMode, "是否内部测试版", "", "", "d"},
	{"CenterExecute", LuaCenterExecute, "让Coordinator执行脚本指令", "{s}", "szCall：要执行函数及其他参数;", ""},
	{"GetIsSwimByPlayerId", LuaGetIsSwimByPlayerId, "是否在游泳状态", "d", "玩家id;", ""},
	{"LeaveOverallTeam", LuaLeaveOverallTeam, "离开副本大厅排队", "o", "pPlayer;", ""},
	{"CheckRemoveBladeState", LuaCheckRemoveBladeState, "取消大神状态", "o", "pPlayer;", ""},
#endif // #ifdef GAME_SERVER

#ifdef GAME_CLIENT
	{"GetPing", LuaGetPing, "获取ping包来回时间", "", "", "d"},
	{"GetClientPlayerId", LuaGetClientPlayerId, "获得客户端自己的Id号", "", "", "d"},
	{"Console", LuaConsole, "打开控制台输出", "", "", "d"},
	{"ShutDown", LuaShutDown, "关闭客户端连接", "", "", "d"},
	{"GetServerTime", LuaGetServerTime, "获取服务器时间（秒数，根据登录时服务器时间算出来）", "", "", "d"},
#endif // #ifdef GAME_CLIENT
};
