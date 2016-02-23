/* -------------------------------------------------------------------------
//	文件名		：	kluaailogic_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KAILogic)
	REGISTER_LUA_INTEGER_READONLY(AIType, "")
	REGISTER_LUA_FUNC(NewAction, "建立一个新的Action", "dd", "nActionId;nActionKey;", "")
	REGISTER_LUA_FUNC(NewState, "建立一个新的状态结点", "d", "nStateId 状态结点Id;", "")
	REGISTER_LUA_FUNC(SetInitState, "设置初始状态结点", "d", "nInitState 初始化时候的状态结点ID;", "")
	REGISTER_LUA_FUNC(RegisterUserAction, "注册自定义的Action", "ds", "nActionKey;pszFunction 函数名称;", "")
DEFINE_LUA_CLASS_END()
