/* -------------------------------------------------------------------------
//	文件名		：	kmail_datadef.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011-5-16 15:34:00
//	功能描述	：	邮件数据类型
// -----------------------------------------------------------------------*/
#ifndef __KMAIL_DATADEF_H__
#define __KMAIL_DATADEF_H__

#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)

#define KD_MAIL_MAX_CONTENT_LENGTH      512			// 最大正文长度
#define KD_MAIL_MAX_CAPTION_LENGTH		64			// 最大标题长度
#define KD_MAIL_MAX_RECEIVER_COUNT		10			// 收件人最大个数
#define KD_MAIL_MAX_ITEM_COUNT			8			// 最大携带的物品数量
#define KD_MAIL_MAX_COND_LENGTH			256			// 条件最大长度
#define KD_MAIL_MAX_SIZE				2 * 1024	// 邮件最大字节数（包括标题、内容、附件）
#define KD_MAIL_MAX_COMMON_COUNT		20			// 邮箱中玩家邮件达到此数目，不在接收玩家邮件（暂时不考虑退信）

struct KItemData_Common
{
	BYTE byData[1];
};

// 邮件类型
enum KE_MAIL_TYPE
{
	emKMAIL_TYPE_SYSTEM,				// 系统发送的邮件
	emKMAIL_TYPE_PLAYER,				// 玩家发送的邮件
	emKMAIL_TYPE_DELIVERY,				// 急件（如果玩家在线，直接推送）
	
	emKMAIL_TYPE_COUNT,					// 类型总数
};

// 邮件状态
enum KE_MAIL_STAT
{
	emKMAIL_STAT_UNREAD,		// 未读
	emKMAIL_STAT_READED,		// 已读
	emKAMIL_STAT_COUNT
};

// 日期时间
struct KMAIL_DATETIME
{
	INT		nYear;
	INT		nMonth;
	INT		nDay;
	INT		nHour;
	INT		nMinute;
	INT		nSecond;
};

// 可修改的邮件结构
struct KMAIL_MODINFO
{
	INT64			nMailID;
	INT				nStat;
	LPCSTR			pMailData;
	INT				nMailSize;
	INT				nCondLen;
	CHAR			szCond[KD_MAIL_MAX_COND_LENGTH];
};

// 邮件结构
struct KMAIL_INFO : public KMAIL_MODINFO
{
	INT				nType;
	DWORD			dwSenderId;
	DWORD			dwReceiverId;
	CHAR			szSender[32];
	KMAIL_DATETIME	sSendTime;
};

// 邮件的数据库记录结构
struct KMAIL_DBINFO
{
	WORD	wVersion;			// 版本号
	WORD	wSize;				// 邮件记录大小
	INT		nIconId;			// 邮件图标ID
	WORD	wCaptionOffset;		// 标题偏移位置
	WORD	wContentOffset;		// 正文偏移位置
	WORD	wAttachOffet;		// 附件偏移位置
};

// 邮件附件信息数据库记录结构
struct KMAIL_DBINFO_ATTACHMENT
{
	WORD				wCount;				// 附件数量
	KItemData_Common	aryItems[0];		// 附件物品
};

// 获取邮件内容时的附件数据结构
struct KMAIL_ITEM_DATA
{
	INT	nItemIndex;				// 附件索引
	KItemData_Common sItem;		// 附件数据
};

// 邮件玩家过滤条件头
struct KMAIL_CONDHEAD
{
	BYTE	byType;
};

// 邮件过滤条件类型
enum KE_MAILCOND_TYPE
{
	emKMAILCOND_TYPE_SEXLEVEL = 1,		// 级别性别
	emKMAILCOND_TYPE_SENDTIME = 2,		// 发送时间
};

// 邮件过滤条件的操作
enum KE_MAILCOND_OPERATION
{
	emKMAILCOND_OP_IGNORE		= 0,		// 忽略
	emKMAILCOND_OP_GREATER		= 1,		// 大于
	emKMAILCOND_OP_GREATEREQUAL = 2,		// 大于等于
	emKMAILCOND_OP_LESS			= 3,		// 小于
	emKMAILCOND_OP_LESSEQUAL	= 4,		// 小于等于
	emKMAILCOND_OP_EQUAL		= 5,		// 等于
	emKMAILCOND_OP_NOTEQUAL		= 6,		// 不等于
};

// 条件类型：emKMAILCOND_SEXLEVEL
struct KMAILCOND_SEXLEVEL : public KMAIL_CONDHEAD
{
	BYTE	bySex;					// 性别，1为女，2为男，3为无论男女
	INT		nLevel1;				// 级别条件1
	BYTE	byLevel1Op;				// 级别条件1操作，见KE_MAILCOND_OPERATION
	INT		nLevel2;				// 级别条件2
	BYTE	byLevel2Op;				// 级别条件2操作，见KE_MAILCOND_OPERATION
};

// 条件类型：emKMAILCOND_TYPE_SENDTIME
struct KMAILCOND_SENDTIME : public KMAIL_CONDHEAD
{
	INT nSendTime;					// 邮件发送基准时间
	INT	nDeltaTime;					// 时间间隔
	BYTE byTimeOp;					// 发送时间条件操作，见KE_MAILCOND_OPERATION
};


#pragma pack(pop)
#endif //__KMAIL_DATADEF_H__
