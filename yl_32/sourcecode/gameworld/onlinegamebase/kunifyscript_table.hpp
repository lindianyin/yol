/* -------------------------------------------------------------------------
//	文件名		：	kunifyscript_table.hpp
//	创建者		：	JXLuaMapTableMaker
//	功能描述	：	本文件由VS2005 Macro自动生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"IsNameWordPass", LuaIsNameWordPass, "检查名字中的单字是否符合要求", "s", "名字;", "1 or 0"},
	{"EnableLeakDectect", LuaEnableLeakDectect, "[Windows及Debug版下有效]开启/关闭内存泄露检测（主线程）", "d", "非0开启其他关闭;", ""},
	{"MathRandom", LuaMathRandom, "获得一个随机数", "[dd]", "如果无参数返回[0,1)，如果一个参数返回[1,max];如果两个参数返回[min,max];", "随机数"},
};
