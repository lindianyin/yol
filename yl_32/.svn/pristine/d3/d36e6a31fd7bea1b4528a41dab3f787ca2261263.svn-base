/* -------------------------------------------------------------------------
//	文件名		：	kluaplayerpurse_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaPlayerPurse)
	REGISTER_LUA_FUNC(GetCurrency, "获取某珍币数量", "d", "珍币类型;", "数量值")
	REGISTER_LUA_FUNC(GetMoney, "获取某类型钱数量", "d", "钱类型;", "钱数量")
	REGISTER_LUA_FUNC(GetCoin, "获取金币", "", "", "数量")
	REGISTER_LUA_FUNC(GetCurrencyList, "获取玩家货币种类和数量", "", "", "{{nCurrencyType, nCurrency},...}")
	REGISTER_LUA_FUNC(GetMoneyList, "获取钱种类和数量", "", "", "{{nMoneyType, nMoney},....}")

#ifdef GAME_SERVER
	REGISTER_LUA_FUNC(RegistCurrencyType, "注册珍币类型", "d", "珍币类型;", "返回值")
	REGISTER_LUA_FUNC(AddCurrency, "往玩家身上加珍币", "dd", "货币id;货币数量;", "加珍币返回值")
	REGISTER_LUA_FUNC(RegistMoneyType, "注册钱类型", "d", "钱类型;", "返回结果")
	REGISTER_LUA_FUNC(AddMoney, "给玩家加钱", "dd", "货币类型id;钱;", "加钱返回值")
	REGISTER_LUA_FUNC(LogMoney, "给玩家加钱", "ddd", "类型枚举;货币类型id;钱;", "加钱返回值")
	REGISTER_LUA_FUNC(AddCoin, "给玩家加金币（金币由paysys同步，这个接口不起作用）", "d", "金币;", "加金币结果")
#endif // #ifdef GAME_SERVER
DEFINE_LUA_CLASS_END()
