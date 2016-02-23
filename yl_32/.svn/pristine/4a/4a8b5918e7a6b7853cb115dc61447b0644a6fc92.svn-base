/* -------------------------------------------------------------------------
//	文件名		：	kchatscript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"HasBadWords", LuaHasBadWords, "是否含有chat敏感词", "s", "输入文字;", ""},
	{"KillBadWords", LuaKillBadWords, "屏蔽chat敏感词", "s", "输入文字;", ""},

#ifdef GAME_SERVER
	{"SendMsg", LuaSendMsg, "", "dddss", "频道Id;发送者Id;接收对象;发送者Head;消息内容;", ""},
	{"BroadcastSysMsg", LuaBroadcastSysMsg, "向所有玩家发送系统消息", "s[dds]", "系统消息;系统消息类型;dwSenderId;szSenderName;", ""},
	{"BroadcastBlackMsg", LuaBroadcastBlackMsg, "向所有玩家发送黑条消息", "s", "系统消息;", ""},
	{"BroadcastKinNoticeMsg", LuaBroadcastKinNoticeMsg, "向所有家族玩家发送公告消息", "ds", "玩家id;系统消息;", ""},
	{"BroadcastNoticeMsg", LuaBroadcastNoticeMsg, "向所有玩家发送公告消息", "s", "系统消息;", ""},
	{"GetFreeWorldSpeakerCount", LuaGetFreeWorldSpeakerCount, "获取每日赠送线聊喇叭数", "", "", ""},
	{"SetFreeWorldSpeakerCount", LuaSetFreeWorldSpeakerCount, "设置每日赠送线聊喇叭数", "", "", ""},
	{"SetChatDisableAuth", LuaSetChatDisableAuth, "设置是否有禁言权限", "od", "pPlayer;0或1;", ""},
	{"GetChatDisableAuth", LuaGetChatDisableAuth, "是否有禁言权限", "o", "pPlayer;", ""},
	{"DisablePlayerChat", LuaDisablePlayerChat, "禁言", "o[dd]", "pPlayer;禁言频道;禁言时长", ""},
	{"SetChatDisableStartTime", LuaSetChatDisableStartTime, "设置开始禁别人言的时间", "o", "pPlayer;", ""},
	{"GetChatDisableStartTime", LuaGetChatDisableStartTime, "得到开始禁言的时间", "o", "pPlayer;", ""},
	{"ModifyLastBroadcastTime", LuaModifyLastBroadcastTime, "修改上次玩家个人公告时间", "odd", "pPlayer;nId;dwLastTime;", ""},
	{"GetLastBroadcastTime", LuaGetLastBroadcastTime, "得到上次玩家个人公告时间", "od", "pPlayer;nId;;", ""},
#endif // #ifdef GAME_SERVER
};
