/* -------------------------------------------------------------------------
//	文件名		：	kgcglobaldatascript_table.hpp
//	创建者		：	JXLuaMapTableMaker
//	功能描述	：	本文件由VS2005 Macro自动生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
#ifdef GAME_CENTER
	{"SetIntValue", LuaSetIntValue, "设置一个全局整形变量", "dd", "Key;Value;", "d"},
	{"AddIntValue", LuaAddIntValue, "累加一个全局整形变量", "dd", "Key;Value;", "d"},
	{"GetIntValue", LuaGetIntValue, "获得一个全局整形变量", "d", "Key;", ""},
	{"SetStrValue", LuaSetStrValue, "", "ds", "Key;Value;", ""},
	{"GetStrValue", LuaGetStrValue, "", "d", "Key;", ""},
	{"DelStrValue", LuaDelStrValue, "", "d", "Key;", ""},
#endif // #ifdef GAME_CENTER
};
