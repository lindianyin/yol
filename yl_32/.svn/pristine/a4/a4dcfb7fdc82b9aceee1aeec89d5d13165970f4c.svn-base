/* -------------------------------------------------------------------------
//	文件名		：	kscenescript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"GetNameById", LuaGetNameById, "通过场景Id(模板)获得名字", "d", "场景Id;", ""},
	{"GetMapOwnerId", LuaGetMapOwnerId, "通过场景nSceneId获得场景OwnerId", "d", "场景Id;", ""},
	{"GetMapCopyIndex", LuaGetMapCopyIndex, "通过场景nSceneId获得场景CopyIndex", "d", "场景Id;", ""},
	{"Dump", LuaDump, "输出场景调试信息，可以当系统消息发给指定玩家", "[ou]", "pLuaPlayer 向此玩家发送sysmsg;dwSceneId 只输出此场景信息;", ""},
	{"GetPlayerNum", LuaGetPlayerNum, "获取场景内玩家人数", "d[d]", "dwSceneId 场景ID;nCopyIndex 副本序号;", ""},
	{"ResetMap", LuaResetMap, "重置场景", "dd", "nMapID;nSceneId;", ""},
	{"DeleteAllNpc", LuaDeleteAllNpc, "删除场景所有npc", "dd", "nMapID;nSceneId;", ""},
	{"GetNameByTemplateId", LuaGetNameByTemplateId, "通过场景的TemplateId获得名字", "d", "场景TemplateId;", ""},
	{"GetFilePathByTemplateId", LuaGetFilePathByTemplateId, "通过场景的TemplateId获得场景文件的存放路径", "d", "场景TemplateId;", ""},

#ifdef GAME_SERVER
	{"UnloadScene", LuaUnloadScene, "卸载地图(小心！若非必要请勿使用,会导致其中的玩家退出场景但仍留在游戏世界)", "d", "dwSceneId;", ""},
	{"UnloadBattleScene", LuaUnloadBattleScene, "卸载战斗地图", "d", "dwSceneId;", ""},
	{"LoadScene", LuaLoadScene, "载入地图 debug用", "dd[d]", "nSceneId;nMapOwnerId;nUserParam1 用户参数，加载完成返回;", "返回0：导入失败，返回1：导入成功"},
	{"GetSceneType", LuaGetSceneType, "得到地图Type", "d", "nMapId;", ""},
	{"GetSceneTypeName", LuaGetSceneTypeName, "得到地图SceneTypeName", "d", "nMapId;", ""},
	{"GetSceneFightState", LuaGetSceneFightState, "判断一个地图是否是可战斗地图", "d", "nMapId;", ""},
	{"GetSceneSetFightMode", LuaGetSceneSetFightMode, "判断一个地图是否是可战斗地图", "d", "nMapId;", ""},
	{"IsDynamicScene", LuaIsDynamicScene, "判断一个场景是否是动态场景", "d", "nSceneId;", "1 是动态场景 0 不是"},
	{"GetBattleSceneIdByTemplateId", LuaGetBattleSceneIdByTemplateId, "通过场景的TemplateId获得该场景默认使用的战斗场景Id", "d", "场景TemplateId;", ""},
	{"IsDynamicSceneLoad", LuaIsDynamicSceneLoad, "通过模版id查询动态场景是否已加载过", "d", "场景TemplateId;", "如存在"},
	{"GetDynamicSceneIdByTemplateId", LuaGetDynamicSceneIdByTemplateId, "通过模版id得到普通动态场景的id", "d", "场景TemplateId;", ""},
#endif // #ifdef GAME_SERVER
};
