/* -------------------------------------------------------------------------
//	文件名		：	luaplayerquiz_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(LuaPlayerQuiz)
	REGISTER_LUA_FUNC(CallQuiz, "开始答题, 传入一个题目编号", "d", "问题ID;", "")
	REGISTER_LUA_FUNC(SendClientResult, "传递答题结果", "dd", "答题是否对;是否关闭客户端面板;", "")
	REGISTER_LUA_FUNC(GetIsJoinIssue, "获取是否正在答题", "", "", "")
	REGISTER_LUA_FUNC(GetPlayerRank, "获取答题排名", "", "", "")
	REGISTER_LUA_FUNC(SendIssueStart, "通知客户端答题开始", "dddd", "发题剩余时间;给答案初始元宝;当前免费剩余次数;免费最大次数;", "")
	REGISTER_LUA_FUNC(SendIssueQuestion, "通知客户端答题开始", "ddddd", "答题剩余时间;给答案初始元宝;给答案递增元宝;当前免费剩余次数;免费最大次数;", "")
	REGISTER_LUA_FUNC(SendIssueResult, "发送结果", "d", "发题剩余时间;", "")
	REGISTER_LUA_FUNC(SumitIntegral, "提交积分到CR", "d", "玩家积分;", "")
	REGISTER_LUA_FUNC(SetIssueDouble, "设置玩家双倍", "d", "是否双倍;", "")
	REGISTER_LUA_FUNC(SetIssueCorrect, "设置玩家是否正确", "d", "是否正确;", "")
	REGISTER_LUA_FUNC(AddVipDoubleCount, "增加当前vip 双倍次数", "d", "增量;", "")
	REGISTER_LUA_FUNC(DoClientSpirit, "玩家直接给答案", "dd", "题目编号;答案序号;", "")
	REGISTER_LUA_FUNC(GetIssueAnswerInfo, "获取玩家答题信息", "", "", "")
DEFINE_LUA_CLASS_END()
