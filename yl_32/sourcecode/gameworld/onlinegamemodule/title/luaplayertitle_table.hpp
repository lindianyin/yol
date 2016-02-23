/* -------------------------------------------------------------------------
//	文件名		：	luaplayertitle_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(LuaPlayerTitle)
	REGISTER_LUA_FUNC(AddTitleToCharacter, "添加称号", "dddd", "获取途径;数值;类型;已经过了多久;", "")
	REGISTER_LUA_FUNC(RemoveTitleToCharacter, "删除称号", "ddd", "获取途径;数值;类型;", "")
	REGISTER_LUA_FUNC(SnycTitle, "同步称号", "", "", "")
	REGISTER_LUA_FUNC(ClearTitle, "清除称号", "", "", "")
DEFINE_LUA_CLASS_END()
