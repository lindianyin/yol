/* -------------------------------------------------------------------------
//	文件名		：	KinScript_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"CreateTongRequest", LuaCreateTongRequest, "创建家族", "dsd", "dwPlayerId作族长;szTongName字符串作家族名字;nTemplateIndex为创建模板索引;", "创建成功：1，创建失败：0"},
	{"InvitePlayerJoinTongRequest", LuaInvitePlayerJoinTongRequest, "邀请玩家", "ds", "dwPlayerId;szMemberName;", "无"},
	{"InvitePlayerJoinTongRespond", LuaInvitePlayerJoinTongRespond, "邀请", "ds", "dwPlayerId;szMemberName;", "无"},
	{"ApplyKickOutTongMemberRequest", LuaApplyKickOutTongMemberRequest, "踢人", "dd", "dwPlayerId;nMemberId;", "无"},
	{"ApplyTongRosterRequest", LuaApplyTongRosterRequest, "花名册", "ddd", "dwPlayerId;nLastUpdateFrame;dwMemberList;", "无"},
	{"ApplyTongInfoRequest", LuaApplyTongInfoRequest, "ApplyTongInfo", "dd", "dwPlayerId;nLastUpdateFrame;", "无"},
	{"ApplyRepertoryPageRequest", LuaApplyRepertoryPageRequest, "仓库相关", "d", "dwPlayerId;", "无"},
	{"ModifyTongSchemaRequest", LuaModifyTongSchemaRequest, "ModifyTongSchema", "d", "dwPlayerId;", "无"},
	{"ApplyQuitTongRequest", LuaApplyQuitTongRequest, "退出家族", "d", "dwPlayerId;", "无"},
	{"GetKinLevelByPlayerId", LuaGetKinLevelByPlayerId, "根据玩家id得到所在家族等级", "d", "dwPlayerId;", "无"},
	{"SetApplyerState", LuaSetApplyerState, "允许或拒绝申请者入族", "dddd", "dwOperatorId;dwApplyerId;dwTongId;dwPlayerId;", "无"},
	{"UpgradeKin", LuaUpgradeKin, "升级家族", "d", "dwPlayerId;", "无"},
	{"ChangeTongMemberGroupRequest", LuaChangeTongMemberGroupRequest, "更改家族成员组", "ddd", "dwPlayerId;dwTargetMemberId;nTargetGroup;", "无"},
	{"ChangeTongMasterRequest", LuaChangeTongMasterRequest, "移交族长", "dd", "dwPlayerId;dwNewMasterId;", "无"},
	{"GetKinActivityOpenTimes", LuaGetKinActivityOpenTimes, "得到所有家族活动的开启时间", "", "", "无"},
	{"ChangeTongMemberRemarkRequest", LuaChangeTongMemberRemarkRequest, "更改家族成员remark", "dds", "dwPlayerId;dwTargetMemberId;szRemark;", "无"},
	{"GetTongDescriptionRequest", LuaGetTongDescriptionRequest, "取得家庭描述", "d", "dwTongId;", "描述字串"},
	{"GetCurrentTime", LuaGetCurrentTime, "得到当前时间", "", "", ""},
	{"GetPlayerConnectIndex", LuaGetPlayerConnectIndex, "取得玩家连接的gs索引", "d", "dwPlayerId;", ""},
	{"GetTongMembers", LuaGetTongMembers, "取得成员信息", "d", "dwPlayerId;", ""},
	{"ResetTongMemberDailyContribution", LuaResetTongMemberDailyContribution, "重置家族成员每日贡献度", "d", "dwPlayerId;", ""},
	{"GetTongMember", LuaGetTongMember, "取得成员id列表", "d", "dwTongId;", ""},
	{"SaveMoneyInTongRequest", LuaSaveMoneyInTongRequest, "存钱", "ddddd", "dwPlayerId;dwPealNum;dwJadeiteNum;dwAgateNum;dwDiamondNum;", "无"},
	{"PayTongSalaryRequest", LuaPayTongSalaryRequest, "薪水相关", "dd", "dwPlayerId;nTotalSalary;", "无"},
	{"GetTongSalaryRequest", LuaGetTongSalaryRequest, "获取薪水", "d", "dwPlayerId;", "无"},
	{"GetOnlineAndHaveKinPlayerIds", LuaGetOnlineAndHaveKinPlayerIds, "获取所有在线且有家族的玩家id", "", "", "无"},
	{"GetTongSalaryFailRespond", LuaGetTongSalaryFailRespond, "获取工资失败", "ddd", "dwTongId;dwPlayerId;nSalary;", "无"},
	{"ApplyOpenTongRepertoryRequest", LuaApplyOpenTongRepertoryRequest, "打开仓库", "d", "dwPlayerId;", "无"},
	{"ChangeTongCampRequest", LuaChangeTongCampRequest, "改变家族Camp", "d", "dwPlayerId;", "无"},
	{"TakeTongRepertoryItemToPosRequest", LuaTakeTongRepertoryItemToPosRequest, "无", "d", "dwPlayerId;", "无"},
	{"TakeTongRepertoryItemRequest", LuaTakeTongRepertoryItemRequest, "无", "d", "dwPlayerId;", "无"},
	{"TakeTongRepertoryItemRespond", LuaTakeTongRepertoryItemRespond, "无", "d", "dwPlayerId;", "无"},
	{"PutTongRepertoryItemToPosRequest", LuaPutTongRepertoryItemToPosRequest, "无", "d", "dwPlayerId;", "无"},
	{"PutTongRepertoryItemRequest", LuaPutTongRepertoryItemRequest, "无", "d", "dwPlayerId;", "无"},
	{"PutTongRepertoryItemRespond", LuaPutTongRepertoryItemRespond, "无", "d", "dwPlayerId;", "无"},
	{"UnlockTongRepertoryGridRequest", LuaUnlockTongRepertoryGridRequest, "无", "d", "dwPlayerId;", "无"},
	{"ExchangeTongRepertoryItemPosRequest", LuaExchangeTongRepertoryItemPosRequest, "无", "d", "dwPlayerId;", "无"},
	{"ApplyStackTongRepertoryItemRequest", LuaApplyStackTongRepertoryItemRequest, "无", "d", "dwPlayerId;", "无"},
	{"StackTongRepertoryItemRequest", LuaStackTongRepertoryItemRequest, "无", "d", "dwPlayerId;", "无"},
	{"SetKinRepertoryEnable", LuaSetKinRepertoryEnable, "打开关闭家族仓库功能", "d", "1表示开启，0表示关闭;", "无"},
	{"SetKinRentEnable", LuaSetKinRentEnable, "打开关闭家族仓库租借功能", "d", "1表示开启，0表示关闭;", "无"},
	{"SetKinRentPeriod", LuaSetKinRentPeriod, "设置装备出租时间，以秒为单位", "d", "时间;", "无"},
	{"ImpeachKinMaster", LuaImpeachKinMaster, "弹劾族长成功，将族长移交给其他人", "dd", "弹劾人ID;族长连续不在线时间;", "d"},
};
