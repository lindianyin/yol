
#pragma once

#include "serverbase/RelationDef.h"
#include "onlinegameworld/kprotocol.h"

#pragma pack(1)

// GameClient=>GameServer
enum KERELATION_C2S_PROTOTYPE
{
	emKRELATION_C2S_PROTO_ADDRELATION = 1,				// 添加关系
	emKRELATION_C2S_PROTO_DELRELATION,					// 删除关系
	emKRELATION_C2S_PROTO_REQUESTTEACHER,				// 拜师申请
	emKRELATION_C2S_PROTO_REQUESTSTUDENT,				// 收徒申请
	emKRELATION_C2S_PROTO_SEARCHTEACHER,				// 搜索可拜为师父的在线玩家
	emKRELATION_C2S_PROTO_SEARCHSTUDENT,				// 搜索可收为弟子的在线玩家
	emKRELATION_C2S_PROTO_SETTRAININGOPTION,			// 修改"允许拜师/允许收徒"选项
	emKRELATION_C2S_PROTO_GETTRAININGOPTION,			// 获取"允许拜师/允许收徒"选项
	emKRELATION_C2S_PROTO_REPLYTEACHERREQ,				// 答复拜师申请
	emKRELATION_C2S_PROTO_REPLYSTUDENTREQ,				// 答复收徒申请
	emKRELATION_C2S_PROTO_TRAININGVIEWINFO,				// 查看对方资料
	emKRELATION_C2S_PROTO_TRAININGLEFTCHECKINTIME,		// 查看自己或者自己当前弟子的剩余报到时间
	emKRELATION_C2S_PROTO_TRAININGMAKEFRIEND,			// 通过师徒系统成为好友
	emKRELATION_C2S_PROTO_TRAININGAPPLY,				// 拜师
	emKRELATION_C2S_PROTO_WISHTEACHERCOMEHERE,			// 返回徒弟意思，是否希望师傅过来
	emKRELATION_C2S_END,
};


// GameServer=>GameClient
enum KERELATION_S2C_PROTOTYPE
{
	emKRELATION_S2C_PROTO_SYNCRELATION = 1,				// 同步玩家关系
	emKRELATION_S2C_PROTO_START_SYNCRELATION,			//	开始同步了
	emKRELATION_S2C_PROTO_DELRELATION,					// 删除玩家关系
	emKRELATION_S2C_PROTO_FRIENDINFO,					// 同步好友信息
	emKRELATION_S2C_PROTO_RELATIONLISTLOADED,			// 通知关系列表加载完成
	emKRELATION_S2C_PROTO_DELFAVOR,						// 删除好感度
	emKRELATION_S2C_PROTO_NOTIFYONLINE,					// 通知好友上下线
	emKRELATION_S2C_PROTO_NOTIFYRELATION,				// 通知关系建立
	emKRELATION_S2C_PROTO_DELETEPLAYER,					// 关系玩家删号通知
	emKRELATION_S2C_PROTO_SEARCHTEACHER,				// 搜索可以拜为师父的在线玩家的结果
	emKRELATION_S2C_PROTO_SEARCHSTUDENT,				// 搜索可以收为弟子的在线玩家的结果
	emKRELATION_S2C_PROTO_APPLYINGTEACHER,				// 通知有人申请拜师
	emKRELATION_S2C_PROTO_APPLYINGSTUDENT,				// 通知有人申请收徒
	emKRELATION_S2C_PROTO_TRAININGOPTION,				// 拜师/收徒选项
	emKRELATION_S2C_PROTO_MYSELFLEFTCHECKINTIME,		// 自己的剩余报到时间
	emKRELATION_S2C_PROTO_STUDENTLEFTCHECKINTIME,		// 自己当前弟子的剩余报到时间
	emKRELATION_S2C_PROTO_TRAININGAPPLY,				// 拜师
};


struct KRELATION_PROTO_C2S_ADDRELATION : public KPTC_HEADER<emKRELATION_C2S_PROTO_ADDRELATION>
{
	BYTE	nType;							// 关系类型
	CHAR	szDstPlayer[_NAME_LEN];			// 目标玩家名字
};

struct KRELATION_PROTO_C2S_DELRELATION : public KPTC_HEADER<emKRELATION_C2S_PROTO_DELRELATION>
{
	BYTE	nType;							// 关系类型
	CHAR	szRelation[_NAME_LEN];			// 关系玩家名字
};

struct KRELATION_PROTO_C2S_TRAININGREQUEST_TEACHER : public KPTC_HEADER<emKRELATION_C2S_PROTO_REQUESTTEACHER>
{
	CHAR	szTarget[_NAME_LEN];		// 要拜师/收徒的目标角色名
};

struct KRELATION_PROTO_C2S_TRAININGREQUEST_STUDENT : public KPTC_HEADER<emKRELATION_C2S_PROTO_REQUESTSTUDENT>
{
	CHAR	szTarget[_NAME_LEN];		// 要拜师/收徒的目标角色名
};

struct KRELATION_PROTO_C2S_TRAININGAPPLY : public KPTC_HEADER<emKRELATION_C2S_PROTO_TRAININGAPPLY>
{
	CHAR szTeacher[_NAME_LEN];		// 要拜师的师父名字
};

struct KRELATION_PROTO_C2S_TRAININGSEARCH_TEACHER : public KPTC_HEADER<emKRELATION_C2S_PROTO_SEARCHTEACHER>
{
	INT		nFirstID;					// 起始玩家（用来指定搜索位置）
};

struct KRELATION_PROTO_C2S_TRAININGSEARCH_STUDENT : public KPTC_HEADER<emKRELATION_C2S_PROTO_SEARCHSTUDENT>
{
	INT		nFirstID;					// 起始玩家（用来指定搜索位置）
};

struct KRELATION_PROT_C2S_SETTRAININGOPTION : public KPTC_HEADER<emKRELATION_C2S_PROTO_SETTRAININGOPTION>
{
	BYTE	byTeacherOrStudent;		// 1为拜师，0为收徒
	BYTE	byOption;				// 1为允许，0为不允许
};

struct KRELATION_PROT_C2S_GETTRAININGOPTION : public KPTC_HEADER<emKRELATION_C2S_PROTO_GETTRAININGOPTION>
{

};

// struct KRELATION_PROTO_C2S_REPLYTRAININGREQ : public KPTC_HEADER<emKRELATION_C2S_PROTO_REPLYTEACHERREQ>
// {
// 	CHAR	szApplicant[_NAME_LEN];		// 申请者角色名
// 	BYTE	byAnswer;					// 1：同意，0：拒绝
// };

struct KRELATION_PROTO_C2S_REPLYTRAININGREQ : public KPTC_HEADER<emKRELATION_C2S_PROTO_REPLYSTUDENTREQ>
{
	CHAR	szApplicant[_NAME_LEN];		// 申请者角色名
	BYTE	byAnswer;					// 1：同意，0：拒绝
};

struct KRELATION_PROTO_C2S_TRAININGVIEWINFO : public KPTC_HEADER<emKRELATION_C2S_PROTO_TRAININGVIEWINFO>
{
	CHAR	szApplicant[_NAME_LEN];		// 申请者角色名
};

struct KRELATION_PROTO_C2S_TRAININGLEFTCHECKINTIME : public KPTC_HEADER<emKRELATION_C2S_PROTO_TRAININGLEFTCHECKINTIME>
{
	BYTE bySelfView;		// 1：查看自己的剩余报到时间；0：查看自己当前弟子的剩余报到时间
};

struct KRELATION_PROTO_C2S_TRAININGMAKEFRIEND : public KPTC_HEADER<emKRELATION_C2S_PROTO_TRAININGMAKEFRIEND>
{
	CHAR	szRelation[_NAME_LEN];
};

struct KRELATION_PROTO_C2S_WISHTHEACHERCOMEHERE : public KPTC_HEADER<emKRELATION_C2S_PROTO_WISHTEACHERCOMEHERE>
{
	BYTE byHopeTeacherComeHere;
};

// ==================================================================================================

struct KRELATION_PROTO_S2C_SYNCRELATION : public KLENPTC_HEADER<emKRELATION_S2C_PROTO_SYNCRELATION>
{
	BYTE			nType;				// 关系类型
	BYTE			nRole;				// 关系角色：1为主位玩家，0为次位玩家
	BYTE			nCount;				// 关系数量
	BYTE			byCreate;			// 是否新建关系
	KRELATION_INFO	aryInf[0];			// 关系玩家
};

struct KRELATION_PROTO_S2C_START_SYNCRELATION : public KLENPTC_HEADER<emKRELATION_S2C_PROTO_START_SYNCRELATION>
{
	BYTE			nType;				// 关系类型
	BYTE			nRole;				// 关系角色：1为主位玩家，0为次位玩家
	BYTE			nCount;				// 关系数量
	KRELATION_INFO	aryInf[0];			// 关系玩家
};

struct KRELATION_PROTO_S2C_DELRELATION : public KPTC_HEADER<emKRELATION_S2C_PROTO_DELRELATION>
{
	BYTE			nType;					// 关系类型
	BYTE			nRole;					// 角色，0为主位玩家，1为次位玩家
	CHAR			szRelation[_NAME_LEN];	// 关系玩家名字
};

struct KRELATION_PROTO_S2C_DEL_PLAYER : public KPTC_HEADER<emKRELATION_S2C_PROTO_DELETEPLAYER>
{
	BYTE			nType;					// 关系类型
	BYTE			nRole;					// 角色，0为主位玩家，1为次位玩家
	CHAR			szRelation[_NAME_LEN];	// 关系玩家名字
};

struct KRELATION_PROTO_S2C_DELFAVOR : public KPTC_HEADER<emKRELATION_S2C_PROTO_DELFAVOR>
{
	CHAR			szRelation[_NAME_LEN];	// 关系玩家名字
};

struct KRELATION_PROTO_S2C_FRIENDINFO : public KLENPTC_HEADER<emKRELATION_S2C_PROTO_FRIENDINFO>
{
	BYTE						nCount;			// 好友数量
	KRELATION_PROTO_FRIENDINFO	aryInfo[0];		// 好友信息数组
};

struct KRELATION_PROTO_S2C_NOTIFYONLINE : public KPTC_HEADER<emKRELATION_S2C_PROTO_NOTIFYONLINE>
{
	BYTE	nRelationType;					// 关系类型
	BYTE	nRole;							// 关系玩家角色，1：主位玩家，0：次位玩家
	BYTE	nOnline;						// 是否上线，1：上线，0：下线
	CHAR	szRelation[_NAME_LEN];			// 关系玩家名字
};

struct KRELATION_PROTO_S2C_NOTIFYRELATION : public KPTC_HEADER<emKRELATION_S2C_PROTO_NOTIFYRELATION>
{
	BYTE	nType;							// 关系类型
	BYTE	nRole;							// 客户端玩家的关系角色(1：主位，0：次位)
	CHAR	szRelation[_NAME_LEN];			// 关系玩家名字
};

struct KRELATION_PROTO_S2C_SEARCHTEACHER : public KLENPTC_HEADER<emKRELATION_S2C_PROTO_SEARCHTEACHER>
{
	WORD		wCount;
	KRELATION_TEACHER	aryResult[0];
};

struct KRELATION_PROTO_S2C_SEARCHSTUDENT : public KLENPTC_HEADER<emKRELATION_S2C_PROTO_SEARCHSTUDENT>
{
	WORD		wCount;
	INT			nLastPlayerId;
	KRELATION_STUDENT	aryResult[0];
};

struct KRELATION_PROTO_S2C_TRAININGAPPLY_TEACHER : public KPTC_HEADER<emKRELATION_S2C_PROTO_APPLYINGTEACHER>
{
	CHAR	szApplicant[_NAME_LEN];		// 申请拜师/收徒的角色名字
};

struct KRELATION_PROTO_S2C_TRAININGAPPLY_STUDENT : public KPTC_HEADER<emKRELATION_S2C_PROTO_APPLYINGSTUDENT>
{
	CHAR	szApplicant[_NAME_LEN];		// 申请拜师/收徒的角色名字
};

struct KRELATION_PROTO_S2C_TRAININGOPTION : public KPTC_HEADER<emKRELATION_S2C_PROTO_TRAININGOPTION>
{
	INT		nOption;		// 第1BIT置位表示允许拜师，第2个BIT置位表示允许收徒
};

struct KRELATION_PROTO_S2C_MYSELFLEFTCHECKINTIME : public KPTC_HEADER<emKRELATION_S2C_PROTO_MYSELFLEFTCHECKINTIME>
{
	INT		nLeftSeconds;	// 剩余报到时间（秒），-1表示自己不是未出师弟子，0表示报到时间超时
};

struct KRELATION_PROTO_S2C_SUTDENTLEFTCHECKINTIME : public KLENPTC_HEADER<emKRELATION_S2C_PROTO_STUDENTLEFTCHECKINTIME>
{
	WORD						wCount;
	KRELATION_LEFTCHECKINTIME	aLeftCheckInTime[0];
};

struct KRELATION_PROTO_S2C_TRAININGAPPLYFORTEACHER : public KPTC_HEADER<emKRELATION_S2C_PROTO_TRAININGAPPLY>
{
	CHAR	szApplicant[_NAME_LEN];	// 请求拜师的玩家名字
};

// ======================================================================================

#pragma pack()
