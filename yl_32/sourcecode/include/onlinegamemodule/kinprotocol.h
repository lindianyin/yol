
#pragma once

#include "serverbase/KinDef.h"
#include "onlinegameworld/kprotocol.h"

#pragma pack(1)


enum 
{
	s2c_tong_begin = 0,

	s2c_sync_tong_member_info,
	s2c_delete_tong_member_notify,
	s2c_apply_tong_info_respond,
	s2c_update_tong_client_data_version,
	s2c_sync_apply_join_tong_list,          // 同步请求入族列表
	s2c_sync_activity_opentimes,            // 同步活动时间

	s2c_sync_tong_repertory_page_respond,

	s2c_invite_player_join_tong_request,
	s2c_change_player_tong_notify,
	s2c_tong_broadcast_message,
	s2c_get_tong_description_respond,
	s2c_broadcast_player_change_tong,
	s2c_sync_tong_online_message,
	s2c_apply_open_tong_repertory_respond,
	s2c_sync_tong_history_respond,
	s2c_kin_message_notify,
	s2c_get_freejoin_tong_list_respond,

	s2c_tong_end
};

enum 
{
	c2s_tong_begin = 0,

	c2s_update_tong_roster_request,			//更新家族花名册
	c2s_apply_tong_info_request,			//请求家族信息
	c2s_apply_tong_repertory_page_request,  //请求家族仓库
	c2s_invite_player_join_tong_request,    //邀请玩家加入家族
	c2s_invite_player_join_tong_respond,    //
	c2s_apply_kick_out_tong_member_request, //申请踢人
	c2s_modify_tong_info_request,           //修改家族信息
	c2s_modify_tong_schema_request,         
	c2s_apply_quit_tong_request,

	c2s_change_tong_member_group_request,
	c2s_change_tong_master_request,			//请求改变家族族长
	c2s_change_tong_member_remark_request,

	c2s_get_tong_description_request,
	c2s_save_money_in_tong_request,
	c2s_pay_tong_salary_request,
	c2s_get_tong_salary_request,

	c2s_apply_open_tong_repertory_request,
	c2s_take_tong_repertory_item_request,
	c2s_put_tong_repertory_item_request,
	c2s_exchange_tong_repertory_item_pos_request,
	c2s_stack_tong_repertory_item_request,
	c2s_remove_repertory_item_request,
	c2s_get_back_item_from_repertory_request,

	c2s_get_freejoin_tong_list_request,
	c2s_apply_join_tong_request,
	c2s_set_tong_freejoin_request,

	c2s_set_kin_activity_opentime,           // 设置家族活动开启时间

	c2s_tong_end
};

struct S2C_SYNC_TONG_MEMBER_INFO : KPTC_HEADER<s2c_sync_tong_member_info>
{
	KTongMemberInfo TongMemberInfo;
};

struct S2C_DELETE_TONG_MEMBER_NOTIFY : KPTC_HEADER<s2c_delete_tong_member_notify>
{
	DWORD dwMemberID;
};

struct S2C_APPLY_TONG_INFO_RESPOND : KLENPTC_HEADER<s2c_apply_tong_info_respond>
{
	BYTE    byRespondType;
	BYTE    byData[0];
};

struct S2C_UPDATE_TONG_CLIENT_DATA_VERSION : KPTC_HEADER<s2c_update_tong_client_data_version>
{
	BYTE    byFrameType;
	int     nUpdateCount;
};

struct S2C_SYNC_APPLY_JOIN_TONG_LIST : KLENPTC_HEADER<s2c_sync_apply_join_tong_list>
{
	DWORD   dwApplyerCount;
	BYTE    byData[0];
};

struct S2C_SYNC_ACTIVITY_OPENTIMS : KLENPTC_HEADER<s2c_sync_activity_opentimes>
{
	INT    nCount;
	BYTE   byData[0];
};

struct S2C_SYNC_TONG_REPERTORY_PAGE_RESPOND : KLENPTC_HEADER<s2c_sync_tong_repertory_page_respond>
{
	BYTE    byPageIndex;
	BYTE    byUpdateItemCount;
	BYTE    byInvailedItemCount;
	int     nServerCounter;
	BYTE    byGetRight;
	BYTE    byPutRight;
	BYTE    byData[0];
};

struct S2C_SYNC_TONG_REPERTORY_PAGE_FRAME : KPTC_HEADER<0>
{
	BYTE    byPageIndex;
	int     nServerFrame;
};

struct S2C_INVITE_PLAYER_JOIN_TONG_REQUEST : KPTC_HEADER<s2c_invite_player_join_tong_request>
{
	DWORD dwPlayerID;
	DWORD dwInviterID;
	DWORD dwTongID;
	char  szInviterName[_NAME_LEN];
	char  szTongName[_NAME_LEN];
};

struct S2C_CHANGE_PLAYER_TONG_NOTIFY : KPTC_HEADER<s2c_change_player_tong_notify>
{
	DWORD   dwTongID;
	BYTE    byReason;
	char    szTongName[_NAME_LEN];
};

struct S2C_TONG_BROADCAST_MESSAGE : KLENPTC_HEADER<s2c_tong_broadcast_message>
{
	BYTE byCode;
	BYTE byData[0];
};

struct S2C_GET_TONG_DESCRIPTION_RESPOND : KPTC_HEADER<s2c_get_tong_description_respond>
{
	DWORD dwTongID;
	char  szTongName[_NAME_LEN];
	DWORD dwMastarID;
};

struct S2C_BROADCAST_PLAYER_CHANGE_TONG : KPTC_HEADER<s2c_broadcast_player_change_tong>
{
	DWORD dwPlayerID;
	DWORD dwTongID;
};

struct S2C_SYNC_TONG_ONLINE_MESSAGE : KLENPTC_HEADER<s2c_sync_tong_online_message>
{
	char    szOnlineMessage[0];
};

struct S2C_APPLY_OPEN_TONG_REPERTORY_RESPOND : KPTC_HEADER<s2c_apply_open_tong_repertory_respond>
{
	char    szTongName[_NAME_LEN];
	BYTE    byRepertoryPageNum;
};

struct S2C_SYNC_TONG_HISTORY_RESPOND : KLENPTC_HEADER<s2c_sync_tong_history_respond>
{
	BYTE    byType;
	DWORD   dwStartIndex;
	BYTE    byCount;
	BYTE    byData[0];
};

struct S2C_KIN_MESSAGE_NOTIFY : KLENPTC_HEADER<s2c_kin_message_notify>
{
	BYTE byCode;
	BYTE byData[0];
};

struct S2C_GET_FREEJOIN_TONG_LIST_RESPOND : KLENPTC_HEADER<s2c_get_freejoin_tong_list_respond>
{
	BYTE byTongNum;
	INT  nUpdateCounter;
	BYTE byData[0];
};

// -------------------------------------------------------------------------

struct C2S_APPLY_TONG_ROSTER_REQUEST : KLENPTC_HEADER<c2s_update_tong_roster_request>
{
	int     nClientUpdateCount;
	int     nMemberCount;
	DWORD   dwMemberList[0];
};

struct C2S_APPLY_TONG_INFO_REQUEST : KPTC_HEADER<c2s_apply_tong_info_request>
{
	int     nLastUpdateCount;
};

struct C2S_APPLY_TONG_REPERTORY_PAGE_REQUEST : KPTC_HEADER<c2s_apply_tong_repertory_page_request>
{
	BYTE    byPageIndex;
	int     nLastUpdateCounter;
};

struct C2S_INVITE_PLAYER_JOIN_TONG_REQUEST : KPTC_HEADER<c2s_invite_player_join_tong_request>
{
	char    szNewMemberName[_NAME_LEN];
};

struct C2S_INVITE_PLAYER_JOIN_TONG_RESPOND : KPTC_HEADER<c2s_invite_player_join_tong_respond>
{
	DWORD   dwInviterID;
	DWORD   dwTongID;
	BYTE    byAccept;
};

struct C2S_APPLY_KICK_OUT_TONG_MEMBER_REQUEST : KPTC_HEADER<c2s_apply_kick_out_tong_member_request>
{
	DWORD dwMemberID;
};

struct C2S_MODIFY_TONG_INFO_REQUEST : KLENPTC_HEADER<c2s_modify_tong_info_request>
{
	BYTE byInfoType;
	WORD wDataLen;
	BYTE byData[0];
};

struct C2S_MODIFY_TONG_SCHEMA_REQUEST : KLENPTC_HEADER<c2s_modify_tong_schema_request>
{
	BYTE byGroupIndex;
	BYTE byModifyType;
	BYTE byData[0];
};

struct C2S_APPLY_QUIT_TONG_REQUEST : KPTC_HEADER<c2s_apply_quit_tong_request>
{
};

struct C2S_CHANGE_TONG_MEMBER_GROUP_REQUEST : KPTC_HEADER<c2s_change_tong_member_group_request>
{
	DWORD dwTargetMemberID;
	BYTE  byTargetGroup;
};

struct C2S_CHANGE_TONG_MASTER_REQUEST : KPTC_HEADER<c2s_change_tong_master_request>
{
	DWORD dwNewMasterID;
};

struct C2S_CHANGE_TONG_MEMBER_REMARK_REQUEST : KPTC_HEADER<c2s_change_tong_member_remark_request>
{
	DWORD dwTargetMemberID;
	char  szRemark[_NAME_LEN];
};

struct C2S_GET_TONG_DESCRIPTION_REQUEST : KPTC_HEADER<c2s_get_tong_description_request>
{
	DWORD dwTongID;
};

struct C2S_SAVE_MONEY_IN_TONG_REQUEST : KPTC_HEADER<c2s_save_money_in_tong_request>
{
	DWORD dwPealNum;
	DWORD dwJadeiteNum;
	DWORD dwAgateNum;		//捐的银两
	DWORD dwDiamondNum;		//捐的元宝
};

struct C2S_PAY_TONG_SALARY_REQUEST : KPTC_HEADER<c2s_pay_tong_salary_request>
{
	int nTotalSalary;
};

struct C2S_GET_TONG_SALARY_REQUEST : KPTC_HEADER<c2s_get_tong_salary_request>
{

};

struct C2S_APPLY_OPEN_TONG_REPERTORY_REQUEST : KPTC_HEADER<c2s_apply_open_tong_repertory_request>
{
	DWORD dwNpcID;
};

struct C2S_TAKE_TONG_REPERTORY_ITEM_REQUEST : KPTC_HEADER<c2s_take_tong_repertory_item_request>
{
	BYTE    byPageIndex;
	BYTE    byPagePos;
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	int     nLastUpdateCounter;
};

struct C2S_PUT_TONG_REPERTORY_ITEM_REQUEST : KPTC_HEADER<c2s_put_tong_repertory_item_request>
{
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	BYTE    byPageIndex;
	BYTE    byPagePos;
};

struct C2S_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST : KPTC_HEADER<c2s_exchange_tong_repertory_item_pos_request>
{
	BYTE    bySrcPageIndex;
	BYTE    byDstPageIndex;
	BYTE    bySrcPagePos;
	BYTE    byDstPagePos;
	int     nSrcPageLastUpdateCounter;
	int     nDstPageLastUpdateCounter;
};

struct C2S_STACK_TONG_REPERTORY_ITEM_REQUEST : KPTC_HEADER<c2s_stack_tong_repertory_item_request>
{
	BYTE    byPageIndex;
	BYTE    byPagePos;
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	WORD    wStackNum;
	int     nLastUpdateCounter;
};

struct C2S_REMOVE_TONG_REPERTORY_ITEM_REQUEST : KPTC_HEADER<c2s_remove_repertory_item_request>
{
	BYTE    byPageIndex;
	BYTE    byPagePos;
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	int     nLastUpdateCounter;
};

struct C2S_GET_BACK_ITEM_FROM_REPERTORY_REQUEST : KPTC_HEADER<c2s_get_back_item_from_repertory_request>
{
	DWORD   dwTongID;
	DWORD   dwBoxPos;
	INT     nLastUpdateCounter;
};

struct C2S_GET_FREEJOIN_TONG_LIST_REQUEST : KPTC_HEADER<c2s_get_freejoin_tong_list_request>
{
	INT		nLastUpdateCounter;
};

struct C2S_APPLY_JOIN_TONG_REQUEST : KPTC_HEADER<c2s_apply_join_tong_request>
{
	DWORD	dwTongID;
};

struct C2S_SET_TONG_FREEJOIN_REQUEST : KPTC_HEADER<c2s_set_tong_freejoin_request>
{
	DWORD	dwTongID;
	BOOL	bFreeJoin;
};

struct C2S_SET_KIN_ACTIVITY_OPENTIME : KPTC_HEADER<c2s_set_kin_activity_opentime>
{
	DWORD dwKinID;
	DWORD dwKinActivityIndex;
	DWORD dwKinActivityOpenTime;
};


#pragma pack()
