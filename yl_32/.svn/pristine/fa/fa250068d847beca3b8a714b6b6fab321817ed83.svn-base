/* -------------------------------------------------------------------------
//	文件名		：	kluanpc_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaNpc)
	REGISTER_LUA_DWORD_READONLY(Id, "")
	REGISTER_LUA_DWORD_READONLY(TemplateId, "")
	REGISTER_LUA_STRING_READONLY(DisplayName, "Npc显示名")
	REGISTER_LUA_STRING_READONLY(ClassName, "Npc类名")
	REGISTER_LUA_STRING(NickName, "Npc别名")
	REGISTER_LUA_FUNC(GetCombatGroup, "获取阵营", "", "", "d")
	REGISTER_LUA_FUNC(GetCombatState, "获取战斗状态", "", "", "d")
	REGISTER_LUA_FUNC(GetDeleteState, "获取npc是否已经准备被回收", "", "", "d")
	REGISTER_LUA_FUNC(CallClientScriptOfNearByPlayer, "对npc附近的人调用脚本", "", "", "")
	REGISTER_LUA_FUNC(GetThreatList, "npc仇恨列表", "", "", "{id, id...}")
	REGISTER_LUA_FUNC(ClearAllThreat, "清除仇恨列表和被仇恨表", "", "", "")
	REGISTER_LUA_FUNC(CopyThreatList, "复制仇恨列表到目标Npc", "d", "目标NpcId;", "")
	REGISTER_LUA_FUNC(SetPosition, "设置逻辑坐标(临时)", "ddd", "x;y;z;", "")
	REGISTER_LUA_STRING_READONLY(Title, "Npc Title")

#ifdef GAME_SERVER
	REGISTER_LUA_FUNC(TeleportTo, "瞬移", "dddd", "SceneId;x;y;z;", "")
	REGISTER_LUA_FUNC(SetTeamId, "设置分组Id", "d", "nNpcTeamId;", "")
#endif // #ifdef GAME_SERVER

#ifdef GAME_CLIENT
	REGISTER_LUA_FUNC(GetRepresentPosition, "获取表现3D坐标", "", "", "")
#endif // #ifdef GAME_CLIENT
DEFINE_LUA_CLASS_END()
