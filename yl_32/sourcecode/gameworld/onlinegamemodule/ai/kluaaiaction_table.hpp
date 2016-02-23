/* -------------------------------------------------------------------------
//	文件名		：	kluaaiaction_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KAIAction)
	REGISTER_LUA_FUNC(SetParam, "设置参数", "", "", "")
	REGISTER_LUA_FUNC(GetParam, "获取参数设置", "", "", "")
	REGISTER_LUA_FUNC(SetBranch, "设置Action的分支", "d", "分支的Id表;", "")
DEFINE_LUA_CLASS_END()
