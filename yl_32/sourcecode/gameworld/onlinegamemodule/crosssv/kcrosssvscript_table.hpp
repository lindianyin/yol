/* -------------------------------------------------------------------------
//	文件名		：	kcrosssvscript_table.hpp
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

	{"ClearDropItems", LuaClearDropItems, "清空记录掉落道具", "?", "nPlayerId/pPlayer:角色Id或者对象;", ""},
	{"RecordDropItems", LuaRecordDropItems, "记录掉落道具", "?ddddd", "nPlayerId/pPlayer:角色Id或者对象;g;d;p;l;amount;", ""},
	
#endif // #ifdef GAME_SERVER
};
