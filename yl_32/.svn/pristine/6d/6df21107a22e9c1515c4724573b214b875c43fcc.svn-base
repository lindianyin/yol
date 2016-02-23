/* -------------------------------------------------------------------------
//	文件名		：	kmailprotocol.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011-5-16 10:44:00
//	功能描述	：	邮件协议结构
// -----------------------------------------------------------------------*/
#ifndef __KMAILPROTOCOL_H__
#define __KMAILPROTOCOL_H__

#include "onlinegameworld/kprotocol.h"
#include "gclogicbase/kgclogicbaseprotocol.h"
//#include "onlinegamemodule/item/kitemdatadef.h"
#include "gcmodule/mail/kmail_datadef.h"
#pragma pack(push, 1)

/**********************************************************
 * Description	: 邮件协议
 * Protocol		: GameCenter=>GameServer
**********************************************************/
enum KE_MAIL_G2S_PROTOTYPE
{
	emKMAIL_G2S_PROTO_LOADOK,						// 邮件加载完成
	emKMAIL_G2S_PROTO_FETCHINFO,					// 邮件简要信息
	emKMAIL_G2S_PROTO_FETCHMAIL,					// 邮件正文和附件
	emKMAIL_G2S_PROTO_FETCHITEM,					// 附件中的物品
	emKMAIL_G2S_PROTO_ADDMAIL,						// 通知邮箱中增加邮件
	emKMAIL_G2S_PROTO_DELMAIL,						// 通知邮箱中删除邮件
	emKMAIL_G2S_PROTO_CHECKRECEIVER,				// 邮件收件人检测结果
};

/**********************************************************
 * Description	: 邮件协议
 * Protocol		: GameServer=>GameCenter
**********************************************************/
enum KE_MAIL_S2G_PROTOTYPE
{
	emKMAIL_S2G_PROTO_RESERVE = emKGCBASE_PTC_COUNT,// 保留给LogicBase用
	emKMAIL_S2G_PROTO_PLAYER_LOGIN,					// 玩家登陆游戏
	emKMAIL_S2G_PROTO_PLAYER_LOGOUT,				// 玩家登出游戏
	emKMAIL_S2G_PROTO_FETCHINFO,					// 收取邮件简要信息
	emKMAIL_S2G_PROTO_FETCHCONDINFO,				// 获取系统邮件简要信息列表
	emKMAIL_S2G_PROTO_FETCHMAIL,					// 获取邮件内容
	emKMAIL_S2G_PROTO_DELMAIL,						// 删除邮件
	emKMAIL_S2G_PROTO_FETCHITEM,					// 获取附件中的物品
	emKMAIL_S2G_PROTO_DELETEITEM,					// 删除附件中的物品
	emKMAIL_S2G_PROTO_SENDMAIL,						// 发送邮件
	emKMAIL_S2G_PROTO_SENDCONDMAIL,					// 发送系统邮件（发送给符合指定条件的玩家）
	emKMAIL_S2G_PROTO_CHECKRECEIVER,				// 检测收件人是否合法
};

/**********************************************************
 * Description	: 邮件协议
 * Protocol		: GameServer=>GameClient
**********************************************************/
enum KE_MAIL_S2C_PROTOTYPE
{
	emKMAIL_S2C_PROTO_LOADOK = 1,					// 邮件加载完成
	emKMAIL_S2C_PROTO_FETCHINFO,					// 邮件简要信息
	emKMAIL_S2C_PROTO_FETCHCONDINFO,				// 条件群组邮件
	emKMAIL_S2C_PROTO_FETCHMAIL,					// 邮件正文
	emKMAIL_S2C_PROTO_SENDRESULT,					// 邮件发送结果
	emKMAIL_S2C_PROTO_NEWMAIL,						// 新邮件通知
	emKMAIL_S2C_PROTO_FETCHITEMOK,					// 成功获取邮件附件
};

/**********************************************************
 * Description	: 邮件协议
 * Protocol		: GameClient=>GameServer
**********************************************************/
enum KE_MAIL_C2S_PROTOTYPE
{
	emKMAIL_C2S_PROTO_FETCHINFO = 1,				// 获取邮件简要信息列表
	emKMAIL_C2S_PROTO_FETCHCONDINFO,				// 获取系统邮件简要信息列表
	emKMAIL_C2S_PROTO_FETCHMAIL,					// 获取邮件详细信息
	emKMAIL_C2S_PROTO_FETCHITEM,					// 获得附件中的物品
	emKMAIL_C2S_PROTO_DELMAIL,						// 删除邮件
	emKMAIL_C2S_PROTO_SENDMAIL,						// 发送邮件
};
/**********************************************************
 * Description	: 邮件错误信息
**********************************************************/
enum KE_MAIL_ERROR
{
	emKMAIL_ERR_BOXFULL = 1,			// 邮箱已满
	emKMAIL_ERR_RECEIVER,				// 收件人错误, 收件人不正确或是收件与发件人相同
	emKMAIL_ERR_MONEY,					// 金钱不足
	emKMAIL_ERR_ITEM,					// 物品错误
	emKMAIL_ERR_FORBID,					// 禁止发送
};

/**********************************************************
 * Description	: 玩家邮件加载完成
 * Protocol		: emKMAIL_G2S_PROTO_LOADOK
**********************************************************/
struct KMAIL_G2S_PROTO_LOADOK : KPTC_HEADER<emKMAIL_G2S_PROTO_LOADOK>
{
	DWORD	dwPlayerId;		// 玩家ID
	BYTE	byShowNotify;	// 是否消息提示
};

struct KMAIL_PROTO_INFO
{
	INT64					nMailID;									// 邮件ID
	BYTE					nType;										// 类型
	BYTE					nStat;										// 状态
	CHAR					szSender[_NAME_LEN];						// 发送者
	CHAR					szReceiver[_NAME_LEN];						// 接收者
	CHAR					szCaption[KD_MAIL_MAX_CAPTION_LENGTH];		// 标题
	INT						nSendTime;									// 发送时间
	INT						nIconId;									// 邮件图标ID
};

/**********************************************************
 * Description	: 邮件简要信息
 * Protocol		: emKMAIL_G2S_PROTO_FETCHINFO
**********************************************************/
struct KMAIL_G2S_PROTO_FETCHINFO : KLENPTC_HEADER<emKMAIL_G2S_PROTO_FETCHINFO>
{
	DWORD				dwPlayerId;		// 玩家ID
	INT					nCurrentTime;	// 当前时间(time_t)
	WORD				wCount;			// 邮件数量
	KMAIL_PROTO_INFO	aryMails[0];	// 邮件信息
};

/**********************************************************
 * Description	: 新邮件通知
 * Protocol		: emKMAIL_G2S_PROTO_ADDMAIL			  		
**********************************************************/
struct KMAIL_G2S_PROTO_ADDMAIL : KPTC_HEADER<emKMAIL_G2S_PROTO_ADDMAIL>
{
	DWORD		dwPlayerId;		// 玩家ID
	INT64		nMailID;		// 邮件ID
};

/**********************************************************
 * Description	: 通知邮箱中删除邮件
 * Protocol		: emKMAIL_G2S_PROTO_DELMAIL		
**********************************************************/
struct KMAIL_G2S_PROTO_DELMAIL : KPTC_HEADER<emKMAIL_G2S_PROTO_DELMAIL>
{
	DWORD		dwPlayerId;		// 玩家ID
	INT64		nMailID;		// 邮件ID
};

/**********************************************************
 * Description	: 邮件正文和附件（变长协议）
 * Protocol		: emKMAIL_G2S_PROTO_FETCHMAIL
**********************************************************/
struct KMAIL_G2S_PROTO_FETCHMAIL : KLENPTC_HEADER<emKMAIL_G2S_PROTO_FETCHMAIL>
{
	DWORD		dwPlayerId;				// 玩家ID
	INT64		nMailID;				// 邮件ID
	INT			nIconId;				// 邮件图标ID
	INT			nType;					// 邮件类型
	WORD		wContentLen;			// 正文长度
	BYTE		nItemCount;				// 附件物品数量
	KMAIL_ITEM_DATA	aryItem[0];			// 附带物品
};

/**********************************************************
 * Description	: 附件中的物品
 * Protocol		: emKMAIL_G2S_PROTO_FETCHITEM
**********************************************************/
struct KMAIL_G2S_PROTO_FETCHITEM : KPTC_HEADER<emKMAIL_G2S_PROTO_FETCHITEM>
{
	DWORD		dwPlayerId;				// 玩家ID
	INT64		nMailID;				// 邮件ID
	BYTE		nItemIdx;				// 物品索引
	BYTE        byMailType;				// 邮件的种类
	KItemData_Common	sItem;			// 物品数据
};

/**********************************************************
 * Description	: 检测收件人
 * Protocol		: emKMAIL_G2S_PROTO_CHECKRECEIVER			  		
**********************************************************/
struct KMAIL_G2S_PROTO_CHECKRECEIVER : KLENPTC_HEADER<emKMAIL_G2S_PROTO_CHECKRECEIVER>
{
	DWORD		dwPlayerId;				// 玩家ID
	INT			nMailIndex;				// 待发邮件索引
	INT			nResult;				// 检测结果
	INT			nFailReasonLen;			// 异常原因长度
	WORD		wReceiverCount;			// 收件人数量（如果nResult成功，返回收件人Id）
};

/**********************************************************
 * Description	: 玩家上线通知
 * Protocol		: emKMAIL_S2G_PROTO_PLAYER_LOGIN
**********************************************************/
struct KMAIL_S2G_PROTO_PLAYER_LOGIN : KPTC_HEADER<emKMAIL_S2G_PROTO_PLAYER_LOGIN>
{
	DWORD		dwPlayerId;		// 玩家ID
};

/**********************************************************
 * Description	: 玩家下线通知
 * Protocol		: emKMAIL_S2G_PROTO_PLAYER_LOGOUT
**********************************************************/
struct KMAIL_S2G_PROTO_PLAYER_LOGOUT : KPTC_HEADER<emKMAIL_S2G_PROTO_PLAYER_LOGOUT>
{
	DWORD		dwPlayerId;		// 玩家ID
};

/**********************************************************
 * Description	: 请求邮件简要信息
 * Protocol		: emKMAIL_S2G_PROTO_FETCHINFO
**********************************************************/
struct KMAIL_S2G_PROTO_FETCHINFO : KPTC_HEADER<emKMAIL_S2G_PROTO_FETCHINFO>
{
	DWORD		dwPlayerId;		// 玩家ID
};

/**********************************************************
 * Description	: 请求系统邮件简要信息
 * Protocol		: emKMAIL_S2G_PROTO_FETCHCONDINFO
**********************************************************/
struct KMAIL_S2G_PROTO_FETCHCONDINFO : KPTC_HEADER<emKMAIL_S2G_PROTO_FETCHCONDINFO>
{
	DWORD		dwPlayerId;		// 玩家ID
};

/**********************************************************
 * Description	: 请求邮件正文信息
 * Protocol		: emKMAIL_S2G_PROTO_FETCHMAIL
**********************************************************/
struct KMAIL_S2G_PROTO_FETCHMAIL : KPTC_HEADER<emKMAIL_S2G_PROTO_FETCHMAIL>
{
	DWORD	dwPlayerId;		// 玩家ID
	INT64	nMailID;		// 邮件ID
};

/**********************************************************
 * Description	: 删除邮件
 * Protocol		: emKMAIL_S2G_PROTO_DELMAIL
**********************************************************/
struct KMAIL_S2G_PROTO_DELMAIL : KPTC_HEADER<emKMAIL_S2G_PROTO_DELMAIL>
{
	DWORD	dwPlayerId;		// 玩家ID
	INT64	nMailID;		// 邮件ID
};

/**********************************************************
 * Description	: 获取附件中的物品
 * Protocol		: emKMAIL_S2G_PROTO_FETCHITEM
**********************************************************/
struct KMAIL_S2G_PROTO_FETCHITEM : KPTC_HEADER<emKMAIL_S2G_PROTO_FETCHITEM>
{
	DWORD	dwPlayerId;		// 玩家ID
	INT64	nMailID;		// 邮件ID
	BYTE	nItemIdx;		// 物品索引（从0开始）
};

/**********************************************************
 * Description	: 删除附件中的物品
 * Protocol		: emKMAIL_S2G_PROTO_DELETEITEM
**********************************************************/
struct KMAIL_S2G_PROTO_DELETEITEM : KPTC_HEADER<emKMAIL_S2G_PROTO_DELETEITEM>
{
	DWORD	dwPlayerId;		// 玩家ID
	INT64	nMailID;		// 邮件ID
	BYTE	nItemIdx;		// 物品索引（从0开始）
};

/**********************************************************
 * Description	: 发送邮件
 * Protocol		: emKMAIL_S2G_PROTO_SENDMAIL
**********************************************************/
struct KMAIL_S2G_PROTO_SENDMAIL : KLENPTC_HEADER<emKMAIL_S2G_PROTO_SENDMAIL>
{
	DWORD		dwPlayerId;				// 发送人
	BYTE		nType;					// 邮件类型
	WORD		wReceiverCount;			// 收件人数量（收件人的Id）
	WORD		wCaptionLen;			// 标题长度
	WORD		wContentLen;			// 正文长度
	INT			nIconId;				// 邮件图标Id
	BYTE		nItemCount;				// 附带物品数量，每个物品数据结构为KItemData_Common
	BYTE		byCondLen;				// 条件长度
};

/**********************************************************
 * Description	: 发送全区系统邮件（发送给符合指定条件的玩家）
 * Protocol		: emKMAIL_S2G_PROTO_SENDCONDMAIL
**********************************************************/
struct KMAIL_S2G_PROTO_SENDCONDMAIL : KLENPTC_HEADER<emKMAIL_S2G_PROTO_SENDCONDMAIL>
{
	BYTE		byCondLen;				// 条件长度
	WORD		wCaptionLen;			// 标题长度
	WORD		wContentLen;			// 正文长度
	INT			nIconId;				// 邮件图标ID
	BYTE		nItemCount;				// 附带物品数量，每个物品数据结构为KItemData_Common
};

/**********************************************************
 * Description	: 检测收件人是否正确
 * Protocol		: emKMAIL_S2G_PROTO_CHECKRECEIVER
**********************************************************/
struct KMAIL_S2G_PROTO_CHECKRECEIVER : KLENPTC_HEADER<emKMAIL_S2G_PROTO_CHECKRECEIVER>
{
	DWORD dwPlayerId;		// 发件人Id
	INT	nMailIndex;			// 待发邮件索引
	INT nType;				// 待发邮件类型
	WORD wReceiverCount;	// 收件人数量（每个收件人名字占用_NAME_LEN字节长度）
};

/**********************************************************
* Description	: 邮件加载完成通知客户端
* Protocol		: emKMAIL_S2C_PROTO_LOADOK
**********************************************************/
struct KMAIL_S2C_MAILLOADOK : KPTC_HEADER<emKMAIL_S2C_PROTO_LOADOK>
{
	BYTE byShowMsg; // 是否有消息提示邮件加载完成
};

/**********************************************************
 * Description	: 获取邮件简要信息
 * Protocol		: emKMAIL_S2C_PROTO_FETCHINFO
**********************************************************/
struct KMAIL_S2C_PROTO_FETCHINFO : KLENPTC_HEADER<emKMAIL_S2C_PROTO_FETCHINFO>
{
	INT					nCurrentTime;	// 当前时间
	WORD				wCount;			// 邮件数量
	KMAIL_PROTO_INFO	aryMails[0];	// 邮件信息
};

/**********************************************************
 * Description	: 获取邮件正文（包括附件）
 * Protocol		: emKMAIL_S2C_PROTO_FETCHMAIL
**********************************************************/
struct KMAIL_S2C_PROTO_FETCHMAIL : KLENPTC_HEADER<emKMAIL_S2C_PROTO_FETCHMAIL>
{	
	INT64			nMailID;			// 邮件ID
	INT				nIconId;			// 邮件图标ID
	INT				nType;				// 邮件类型
	WORD			wContentLen;		// 正文长度
	BYTE			nItemCount;			// 附件物品数量
	KMAIL_ITEM_DATA	aryItem[0];			// 附件物品
};

/**********************************************************
 * Description	: 邮件发送结果通知（成功或者失败）
 * Protocol		: emKMAIL_S2C_PROTO_SENDRESULT
**********************************************************/
struct KMAIL_S2C_PROTO_SENDRESULT : KLENPTC_HEADER<emKMAIL_S2C_PROTO_SENDRESULT>
{	
	BYTE	nResult;					// 成功：１，失败：0
	BYTE	nReasonType;				// 失败原因类型，nResult为０时有效，见KE_MAIL_SENDRESULT
	INT		nFailReasonLen;				// 具体失败原因长度
};

/**********************************************************
 * Description	: 新邮件通知
 * Protocol		: emKMAIL_S2C_PROTO_NEWMAIL
**********************************************************/
struct KMAIL_S2C_PROTO_NEWMAIL : KPTC_HEADER<emKMAIL_S2C_PROTO_NEWMAIL>
{
};

/**********************************************************
 * Description	: 成功获取邮件附件
 * Protocol		: emKMAIL_S2C_PROTO_FETCHITEMOK
**********************************************************/
struct KMAIL_S2C_PROTO_FETCHITEMOK : KPTC_HEADER<emKMAIL_S2C_PROTO_FETCHITEMOK>
{
	INT64 nMailID;			// 邮件ID
	BYTE byItemIndex;		// 附件索引
};

/**********************************************************
 * Description	: 获取邮件列表信息
 * Protocol		: emKMAIL_C2S_PROTO_FETCHINFO
**********************************************************/
struct KMAIL_C2S_PROTO_FETCHINFO : KPTC_HEADER<emKMAIL_C2S_PROTO_FETCHINFO>
{
};

/**********************************************************
 * Description	: 获取系统邮件列表信息
 * Protocol		: emKMAIL_C2S_PROTO_FETCHCONDINFO
**********************************************************/
struct KMAIL_C2S_PROTO_FETCHCONDINFO : KPTC_HEADER<emKMAIL_C2S_PROTO_FETCHCONDINFO>
{
};

/**********************************************************
 * Description	: 获取邮件详细信息
 * Protocol		: emKMAIL_C2S_PROTO_FETCHMAIL
**********************************************************/
struct KMAIL_C2S_PROTO_FETCHMAIL : KPTC_HEADER<emKMAIL_C2S_PROTO_FETCHMAIL>
{	
	INT64		nMailID;		// 邮件ID
};

/**********************************************************
 * Description	: 获得附件中的物品
 * Protocol		: emKMAIL_C2S_PROTO_FETCHITEM
**********************************************************/
struct KMAIL_C2S_PROTO_FETCHITEM : KPTC_HEADER<emKMAIL_C2S_PROTO_FETCHITEM>
{
	INT64	nMailID;		// 邮件ID
	BYTE	nItemIndex;		// 物品索引
};

/**********************************************************
 * Description	: 删除邮件
 * Protocol		: emKMAIL_C2S_PROTO_DELMAIL
**********************************************************/
struct KMAIL_C2S_PROTO_DELMAIL : KPTC_HEADER<emKMAIL_C2S_PROTO_DELMAIL>
{	
	INT64		nMailID;		// 邮件ID
};

struct KMAIL_PROTO_MAIL
{
	INT					nIconId;										// 邮件图标Id
	INT					nType;											// 邮件类型
	CHAR				szCaption[KD_MAIL_MAX_CAPTION_LENGTH];			// 标题（可能含有二进制数据？）
	WORD				wContentLen;									// 正文长度
	BYTE				nItemCount;										// 附件物品数量
};

/**********************************************************
 * Description	: 发送邮件
 * Protocol		: emKMAIL_C2S_PROTO_SENDMAIL
**********************************************************/
struct KMAIL_C2S_PROTO_SENDMAIL : KLENPTC_HEADER<emKMAIL_C2S_PROTO_SENDMAIL>
{	
	WORD				wReceiverCount;			// 收件人数目（每个收件人占_NAME_LEN字节长度）
	KMAIL_PROTO_MAIL	sMail;					// 邮件
};

#pragma pack(pop)
#endif //__KMAILPROTOCOL_H__
