/* -------------------------------------------------------------------------
//	文件名		：	relationscriptfuns_table.hpp
//	创建者		：	LuaScriptTool
//	功能描述	：	本文件由VisualStudio宏生成，请不要手动修改！！
//	使用方法	：	在相应的CPP文件末端，include此文件
//
// -----------------------------------------------------------------------*/

CONST SCRIPT_FUNCTION arFunction[]	=  
{
	{"CheckIsLoadEnd", LuaCheckIsLoadEnd, "检查角色数据是否加载完成", "d", "nAppId 玩家姓名;", "TRUE 玩家数据加载完成，FALSE 还没有加载完成"},
	{"CheckCreateDepRep", LuaCheckCreateDepRep, "检查创建关系的依赖和冲突关系", "dddd", "nType 关系类型;nRole 是否是主位;nAppId 申请玩家id;nDstId 对方id;", "BOOL"},
	{"CanCreateRelation", LuaCanCreateRelation, "检查具体的某种关系类型是否可以建立", "dddd", "nType 要添加的关系类型;nRole: 申请者角色（1为主位关系，0为次位关系）;nAppId 玩家ID;nDstId 玩家ID;", "BOOL 检查结果"},
	{"CreateRelation", LuaCreateRelation, "不做检查，直接创建人际关系", "ddd[d]", "nType 要添加的关系类型;nAppId 玩家ID;nDstId 玩家ID;bIsMaster 默认为1，表示是否是关系的主位 1主位 0次位;", "BOOL  创建关系的结果"},
	{"CheckDelDepRep", LuaCheckDelDepRep, "检查删除关系的依赖和冲突关系", "dddd", "nType 关系类型;nRole 是否是主位;nAppId 申请玩家id;nDstId 对方id;", "BOOL"},
	{"DelRelation", LuaDelRelation, "不做检查，直接删除指定的人际关系", "ddd[d]", "nType 要删除的关系类型;nAppId 玩家ID;nDstId 关系玩家ID;bIsMaster 默认为1，表示是否是关系的主位 1主位 0次位;", "BOOL  创建关系的结果"},
	{"ResetLimtWhenCrossDay", LuaResetLimtWhenCrossDay, "检查加亲密度的日期，跨天则清除每日上限", "dd", "nAppId 申请玩家id;nDstId 对方玩家id;", ""},
	{"AddFriendFavor", LuaAddFriendFavor, "增加亲密度", "ddd[dd]", "nAppId 申请者id;nDstId 对方玩家id;nFavor 要增加的亲密度;nMethod 途径 0正常途径 1通过ib道具 默认是0;bByHand 是否手工添加，不判断每日上限（0表示系统添加1表示手工添加）;", "BOOL 增加好友亲密度是否成功"},
	{"SetFriendTalkTime", LuaSetFriendTalkTime, "设置最近一次聊天时间", "ddd", "nAppId 申请者id;nDstId 对方玩家id;dwTime;", ""},
	{"GetRoleName", LuaGetRoleName, "获取角色名字", "d", "nAppId 申请者id;", ""},
	{"SyncRelation", LuaSyncRelation, "同步所有关系", "d", "nAppId 申请者id;", ""},
	{"SyncFriendInfo", LuaSyncFriendInfo, "在增加亲密度后同步好友信息", "dd", "nAppId 申请方玩家id;nDstId 对方玩家id;", "BOOL"},
	{"GetCloseFriendTimeInfo", LuaGetCloseFriendTimeInfo, "获取密友（单向和双向密友）关系是否快要到期", "d", "nAppPlayerId	玩家id;", "{{nPlayerId, nTime, nType}, {nPlayerId, nTime, nType}, ...}"},
	{"DelOverTimeRelation", LuaDelOverTimeRelation, "删除一年到期的密友，师徒和介绍人关系", "dddd", "nType			要删除的关系类型;nMasterId		主位玩家id;nTargetId		次位玩家id;bIsMaster		角色是否是主位;", "BOOL			是否删除成功"},
	{"GetOneRelationCount", LuaGetOneRelationCount, "获取指定的某种人际关系的数量", "dd[d]", "nPlayerId		玩家id;nType			关系类型;bAsMaster		是否是主位玩家;", "INT			数量"},
	{"HasRelation", LuaHasRelation, "检查是否存在指定的人际关系", "ddd[d]", "nAppId	主位id;nDstId	次位id;nType		关系类型;bAsMaster	主次位;", "BOOL"},
	{"GetFriendFavor", LuaGetFriendFavor, "获取指定玩家的亲密度", "dd", "nPlayerId1	第一个玩家id;nPlayerId2	第二个玩家id;", "nFavor"},
};
