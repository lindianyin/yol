/* -------------------------------------------------------------------------
//	文件名		：	kluadoodad_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaDoodad)
	REGISTER_LUA_DWORD_READONLY(Id, "")
	REGISTER_LUA_DWORD_READONLY(TemplateId, "")
	REGISTER_LUA_DWORD_READONLY(SceneId, "")
	REGISTER_LUA_INTEGER_READONLY(FaceDirection, "")
	REGISTER_LUA_STRING_READONLY(Name, "Doodad名称")
	REGISTER_LUA_INTEGER_READONLY(State, "")
	REGISTER_LUA_FUNC(GetPosition, "获取逻辑3D坐标", "", "", "")
	REGISTER_LUA_FUNC(SetPosition, "设置逻辑坐标(无同步)", "ddd", "x;y;z;", "")
	REGISTER_LUA_FUNC(SetOwnerId, "设置OwnerId", "d", "OwnerId;", "")
	REGISTER_LUA_FUNC(GetTempTable, "获得临时table", "", "", "")

#ifdef GAME_SERVER
	REGISTER_LUA_FUNC(TeleportTo, "瞬移", "dddd", "SceneId;x;y;z;", "")
#endif // #ifdef GAME_SERVER
DEFINE_LUA_CLASS_END()
