#ifndef _KRELATIONDEF_H_
#define _KRELATIONDEF_H_


#pragma	pack(1)

struct KRELATION_INFO
{
	CHAR	szRelation[_NAME_LEN];	// 关系玩家名字
	BYTE	nOnline;				// 是否在线，1为在线，0为不在线
	BYTE	nSex;					// 性别
	BYTE	nLevel;					// 等级
	DWORD	nPlayerId;				// 关系玩家id
};

struct KRELATION_PROTO_FRIENDINFO
{
	CHAR	szRelation[_NAME_LEN];	// 关系玩家名字
	INT		nFavor;					// 好感度
	INT		nPortrait;				// 头像
	BYTE	nLevel;					// 等级
	BYTE	nSex;					// 性别
	BYTE	nKinFriend;			// 家族好友
	DWORD	dwLastTalkTime;			// 上次聊天时间
};

struct KRELATION_STUDENT
{
	CHAR		szName[_NAME_LEN];	// 角色名
	CHAR		szTong[_NAME_LEN];	// 帮会
	BYTE		bySex;				// 性别
	BYTE		byFaction;			// 门派
	INT			nLevel;				// 等级
	INT			nPlayerID;			// 玩家ID
};

struct KRELATION_TEACHER : public KRELATION_STUDENT
{
	WORD		wCurStudentCount;	// 当前弟子数
	WORD		wHisStudentCount;	// 出师弟子数
	WORD		wAllStudentCount;	// 弟子总数（包括解除师徒关系的弟子数目）
};

struct KRELATION_LEFTCHECKINTIME
{
	CHAR	szStudent[_NAME_LEN];	// 弟子角色名
	INT		nLeftSeconds;			// 剩余报到时间（秒），-1表示自己不是未出师弟子，0表示报到时间超时
};

#pragma pack()

#endif
