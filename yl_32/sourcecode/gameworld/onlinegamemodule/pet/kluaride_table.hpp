/* -------------------------------------------------------------------------
//	文件名		：	kluaride_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaRide)
	REGISTER_LUA_DWORD_READONLY(Id, "坐骑Id")
	REGISTER_LUA_DWORD_READONLY(OwnerId, "坐骑主人Id")
	REGISTER_LUA_INTEGER_READONLY(Genre, "坐骑G")
	REGISTER_LUA_INTEGER_READONLY(Detail, "坐骑D")
	REGISTER_LUA_INTEGER_READONLY(Particular, "坐骑P")
	REGISTER_LUA_INTEGER_READONLY(Level, "坐骑Level")
	REGISTER_LUA_INTEGER_READONLY(Hungry, "饥饿度")
	REGISTER_LUA_INTEGER_READONLY(State, "坐骑状态")
	REGISTER_LUA_INTEGER_READONLY(MagicRate, "当前幻化果数量")
	REGISTER_LUA_STRING_READONLY(Name, "坐骑名字")

#ifdef GAME_SERVER
	REGISTER_LUA_FUNC(SetMagicRate, "设置当前幻化果数量", "", "", "")
	REGISTER_LUA_FUNC(Sync, "同步坐骑数据到客户端", "", "", "1成功")
	REGISTER_LUA_FUNC(AddHungry, "增加饱食度", "d", "nHungry	;", "1成功")
	REGISTER_LUA_FUNC(SetState, "设置坐骑状态", "d", "nState	;", "1成功")
	REGISTER_LUA_FUNC(Regenerate, "重新生成一只坐骑", "dddd", "nGenre	;nDetail	;nParticular;nLevel;", "成功返回1")
#endif // #ifdef GAME_SERVER
DEFINE_LUA_CLASS_END()
