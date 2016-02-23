/* -------------------------------------------------------------------------
//	文件名		：	kluaplayermail_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

DEFINE_LUA_CLASS_BEGIN(KLuaPlayerMail)

#ifdef GAME_SERVER
	REGISTER_LUA_FUNC(OverSendLimit, "检测是否超过发送限制", "d", "玩家一天发送邮件的限制次数;", "超过发送限制-返回1，否则-返回0")
	REGISTER_LUA_FUNC(AddSendTime, "增加一次邮件发送次数", "", "", "")
	REGISTER_LUA_FUNC(CheckReceiver, "检测收件人是否合法", "dd?", "待发邮件索引;待发邮件类型;收件人列表;", "")
	REGISTER_LUA_FUNC(SendClientResult, "发送邮件结果", "d[ds]", "发送结果;失败类型;具体失败原因;", "")
	REGISTER_LUA_FUNC(SendMail2GC, "发送邮件", "dd?ss[?]", "邮件图标Id;邮件类型;接收者名字;邮件标题;邮件内容;邮件附件列表;", "")
#endif // #ifdef GAME_SERVER

#ifdef GAME_CLIENT
	REGISTER_LUA_FUNC(RequestMailList, "请求邮件列表", "d", "请求类型;", "")
	REGISTER_LUA_FUNC(RequestMail, "请求邮件具体内容", "s", "邮件Id;", "")
	REGISTER_LUA_FUNC(FetchItem, "请求获取附件", "sd", "邮件Id;附件索引;", "")
	REGISTER_LUA_FUNC(DeleteMail, "请求删除邮件", "s", "邮件Id;", "")
	REGISTER_LUA_FUNC(SendMail, "发送邮件", "dd?ss[?]", "邮件图标Id;邮件类型;接收者名字;邮件标题;邮件内容;邮件附件列表;", "")
#endif // #ifdef GAME_CLIENT
DEFINE_LUA_CLASS_END()
