

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	// 客户端/服务端0长度不能编译通过，放个没用的进来
	{"...", NULL, "", "", "", ""},

#ifdef GAME_SERVER
	{"CheckReceiver", LuaCheckReceiver, "检测收件人是否合法", "dd?", "待发邮件索引;带发邮件类型;收件人列表;", ""},
	{"SendMail", LuaSendMail, "发送邮件", "dd?ss[??]", "邮件图标Id;邮件类型;接收者名字列表;邮件标题;邮件内容;邮件附件列表;邮件接收条件;", ""},
#endif // #ifdef GAME_SERVER
};
