#pragma once

#include "ProtocolBasic.h"
#include "onlinegameworld/kprotocol.h"

#pragma pack(push, 1)

enum
{
	c2s_send_mail_request = 0,
	c2s_get_maillist_request,        //获得邮件列表
	c2s_query_mail_content,          //查询邮件内容
	c2s_acquire_mail_money_request,  //请求邮件金钱
	c2s_acquire_mail_item_request,   //请求邮件物品
	c2s_set_mail_read,               //设置邮件已读
	c2s_delete_mail,                 //删除邮件
	c2s_return_mail,
	c2s_mail_end,
};

enum
{
	s2c_send_mail_respond = 0,
	s2c_get_mail_list_respond,
	s2c_new_mail_notify,
	s2c_sync_mail_content,
	s2c_mail_general_respond,
	s2c_mail_end,
};

// c2s
// ==========================================================================

struct C2S_SEND_MAIL_REQUEST : KLENPTC_HEADER<c2s_send_mail_request>
{
	DWORD       dwNpcID;
	BYTE        byRequestID;
	char        szDstName[_NAME_LEN];
	char        szTitle[_NAME_LEN];
	int         nMoney;
	BYTE        byBox[KMAIL_MAX_ITEM_COUNT];        // 邮寄物品所在的包裹
	BYTE        byX[KMAIL_MAX_ITEM_COUNT];          // 邮寄物品所在包裹里的位置
	int         nItemPrice[KMAIL_MAX_ITEM_COUNT];   // 货到付款的价格
	WORD        wTextLen;     
	char        szText[0];                          // 信件文本内容
};

struct C2S_GET_MAILLIST_REQUEST : KPTC_HEADER<c2s_get_maillist_request>
{
	DWORD dwStartID;
};

struct C2S_QUERY_MAIL_CONTENT : KPTC_HEADER<c2s_query_mail_content>
{
	DWORD   dwNpcID;
	DWORD   dwMailID;
	BYTE    byMailType;
};

struct C2S_ACQUIRE_MAIL_MONEY_REQUEST : KPTC_HEADER<c2s_acquire_mail_money_request>
{
	DWORD       dwMailID;
};

struct C2S_ACQUIRE_MAIL_ITEM_REQUEST : KPTC_HEADER<c2s_acquire_mail_item_request>
{
	DWORD       dwMailID;
	BYTE        byIndex;
};

struct C2S_SET_MAIL_READ : KPTC_HEADER<c2s_set_mail_read>
{
	DWORD   dwMailID;
};

struct C2S_DELETE_MAIL : KPTC_HEADER<c2s_delete_mail>
{
	DWORD   dwMailID;
};

struct C2S_RETURN_MAIL : KPTC_HEADER<c2s_return_mail>
{
	DWORD   dwMailID;
};

// s2c
// ============================================================================

struct S2C_SEND_MAIL_RESPOND : KPTC_HEADER<s2c_send_mail_respond>
{
	BYTE            byRespondID;
	BYTE            byResult;
};

struct S2C_GET_MAIL_LIST_RESPOND : KLENPTC_HEADER<s2c_get_mail_list_respond>
{
	BYTE            byMailCount;
	KMailListInfo   MailListInfo[0];
};

struct S2C_NEW_MAIL_NOTIFY : KPTC_HEADER<s2c_new_mail_notify>
{
	KMailListInfo   NewMailListInfo;
};

struct S2C_SYNC_MAIL_CONTENT : KLENPTC_HEADER<s2c_sync_mail_content>
{
	DWORD           dwMailID;
	BYTE            byResult;   // Success, mail not found, data not ready.
	BYTE            byData[0];
};

struct S2C_MAIL_GENERAL_RESPOND : KPTC_HEADER<s2c_mail_general_respond>
{
	DWORD           dwMailID;
	BYTE            byResult;   // Success, mail not found, data not ready.
};

#pragma pack(pop)