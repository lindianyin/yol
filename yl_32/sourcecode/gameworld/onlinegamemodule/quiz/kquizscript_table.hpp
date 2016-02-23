/* -------------------------------------------------------------------------
//	文件名		：	kquizscript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	// 客户端/服务端0长度不能编译通过，放个没用的进来
	{"...", NULL, "", "", "", ""},

#ifdef GAME_SERVER
	{"SetQuizIssueCurrentId", LuaSetQuizIssueCurrentId, "设置答题编号", "d", "答题编号;", ""},
	{"NameListRequest", LuaNameListRequest, "请求榜单", "d", "请求;", ""},
	{"DoIssueStart", LuaDoIssueStart, "请求问题列表", "", "", ""},
#endif // #ifdef GAME_SERVER
};
