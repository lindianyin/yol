/* -------------------------------------------------------------------------
//	文件名		：	luaplayerawardlobby_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(LuaPlayerAwardLobby)
	REGISTER_LUA_FUNC(DoFinishActivity, "完成活动", "dd", "id;awardid;", "")
	REGISTER_LUA_FUNC(DoSignIn, "完成活动", "d", "日期天数;", "")
	REGISTER_LUA_FUNC(DoGetBankIncomeTest, "完成活动", "ds", "id;日期;", "")
	REGISTER_LUA_FUNC(DoFinishRankActivity, "完成冲榜活动", "dd", "id;awardid;", "")
DEFINE_LUA_CLASS_END()
