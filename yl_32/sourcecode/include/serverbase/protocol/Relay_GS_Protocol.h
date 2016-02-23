
#pragma once

#include "ProtocolBasic.h"
#include "config/kplayerbasedbdef.h"
#include "serverbase/StatDataDef.h"
#include "serverbase/KinDef.h"
#include "config/MailDef.h"
#include "serverbase/RelationDef.h"
#include "config/AuctionDef.h"
#include "onlinegamemodule/kescortdef.h"


#define REGISTER_EXTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
	m_nProtocolSize[ProtocolID] = ProtocolSize;}						

#define REGISTER_INTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
	m_uProtocolSize[ProtocolID] = ProtocolSize;}

#define PQ_PARAM_SYNC_COUNT 8
#define MAX_ROLE_DATA_PAK_SIZE  (1024 * 60)
#define QUIZ_QUESTION_COUNT 30
#define QUIZ_LIST_COUNT 10

// 上行协议定义
enum KS2R_PROTOCOL
{
    s2r_protocol_begin,

	s2r_handshake_request,
    s2r_ping_signal,
    s2r_update_performance,

	s2r_create_map_respond,

    s2r_player_login_respond,
	s2r_search_map_request,
	s2r_load_map_request,

    s2r_transfer_player_request,
    s2r_transfer_player_respond,
    s2r_confirm_player_login_request,
	s2r_player_leave_gs,

	s2r_update_fellowship,

    // ------------- 组队相关协议 --------------------------------
	s2r_invite_player_join_team_request,
	s2r_invite_player_join_team_respond,
    s2r_apply_join_team_request,
    s2r_apply_join_team_respond,
    s2r_team_del_member_request,
    s2r_team_change_authority_request,
    s2r_team_set_loot_mode_request,
	s2r_team_set_roll_quality_request,
	s2r_team_set_formation_leader_request,
    s2r_team_disband_request,
    s2r_sync_team_member_max_lmr,
    s2r_sync_team_member_current_lmr,
    s2r_sync_team_member_misc,
    s2r_sync_team_member_position,
    s2r_team_set_mark_request,
    s2r_team_level_up_raid_request,
	s2r_team_change_member_group_request,
	s2r_create_team_request,

    // ---------------- 聊天相关 --------------------------------------
    s2r_chat_message, 
    s2r_player_chat_error, 

    // ---------------- 好友相关 --------------------------------------
    s2r_apply_fellowship_data_request, 
    s2r_update_fellowship_data, 
    s2r_add_fellowship_request, 
    s2r_get_fellowship_name_request, 
    s2r_apply_fellowship_player_level_and_forceid, 
    s2r_add_fellowship_notify, 

    // ---------------- 邮件系统 --------------------------------------
    s2r_send_mail_request,
    s2r_send_global_mail_request,
    s2r_get_maillist_request,
    s2r_query_mail_content,
    s2r_acquire_mail_money_request,
    s2r_acquire_mail_item_request,
    s2r_give_mail_money_to_player,
    s2r_give_mail_item_to_player,
    s2r_set_mail_read,
    s2r_delete_mail,
    s2r_return_mail,
    
    // ---------------- 副本相关 --------------------------------------
    s2r_save_scene_player_list,
    s2r_save_scene_data,
    s2r_reset_map_copy_request,
    s2r_player_enter_scene_notify,

    // ---------------- 交通相关 --------------------------------------
    s2r_sync_road_track_force,
    
    // ---------------- 角色操作 --------------------------------------
    s2r_load_role_data_request,
	s2r_load_delay_role_data_request,

    s2r_change_role_level_request,
    s2r_change_role_forceid_request,

    s2r_send_gm_message,
    s2r_send_gm_chn,

    s2r_send_gm_command_gs,
	s2r_send_gm_command_gc,

	s2r_remote_lua_call,

    s2r_join_battle_field_queue_request,
    s2r_leave_battle_field_queue_request,

    s2r_accept_join_battle_field,
    s2r_get_battle_field_list,

    s2r_leave_battle_field_request,

    s2r_apply_create_pq_request, 
    s2r_apply_delete_pq_request, 
    s2r_apply_change_pq_value_request, 
    
    s2r_add_camp_score,
    s2r_apply_set_camp_request,

    s2r_sync_role_data,
    s2r_save_role_data,
	
	s2r_create_delay_role_data,
	s2r_save_delay_role_data,

    // ---------------- 家族相关 --------------------------------------
    s2r_apply_tong_roster_request,
    s2r_apply_tong_info_request,
    s2r_apply_tong_repertory_page_request,
    s2r_apply_create_tong_request,
    s2r_invite_player_join_tong_request,
    s2r_invite_player_join_tong_respond,
    s2r_apply_kick_out_tong_member_request,
    s2r_modify_tong_info_request,
    s2r_modify_tong_schema_request,
    s2r_apply_quit_tong_request,
    s2r_change_tong_member_group_request,
    s2r_change_tong_master_request,
    s2r_change_tong_member_remark_request,
    s2r_get_tong_description_request,
    s2r_save_money_in_tong_request,
    s2r_pay_tong_salary_request,
    s2r_get_tong_salary_request,
    s2r_get_tong_salary_fail_respond,
    s2r_change_tong_camp_request,

    s2r_apply_open_tong_repertory_request,

    s2r_take_tong_repertory_item_request,
	s2r_take_tong_repertory_item_respond,
    s2r_put_tong_repertory_item_request,
	s2r_put_tong_repertory_item_respond,
    s2r_exchange_tong_repertory_item_pos_request,
    s2r_stack_tong_repertory_item_request,
	s2r_remove_tong_repertory_item_request,
	s2r_get_back_item_from_repertory_request,

	s2r_get_freejoin_tong_list_request,
	s2r_apply_join_tong_request,
	s2r_set_tong_freejoin_request,
	s2r_set_tong_activity_opentime_request,

	s2r_get_tong_rank_info_request,

	// -----------------排行榜相关--------------------------------------
	s2r_get_rank_idx_request,
	s2r_get_rank_id_request,
	s2r_swap_rank_request,
	s2r_modify_rank_value_request,

	// -----------------修仙相关--------------------------------------
	s2r_start_escort_request,
	s2r_refresh_escorts_request,
	s2r_escort_remove_watch_request,
	s2r_take_escort_award_request,
	s2r_rob_player_escort_request,
	s2r_del_rob_escort_request,
	s2r_take_rob_award_request,
	s2r_speedup_escort_request,
	s2r_rescue_player_request,

	// -----------------好友相关--------------------------------------
	s2r_relation_add_relation_request,					// 添加关系
	s2r_relation_del_relation_request,					// 删除关系
	s2r_relation_add_relation_favor_request,			// 增加好感度
	s2r_relation_pre_add_relation_favor_request,		// 预增加好感度（确保下一次增加好感度成功）
	s2r_relation_act_add_favor_request,					// 实际增加好感度（和预加好感度对应）
	s2r_relation_cancel_pre_add_favor_request,			// 取消预增加好感度
	s2r_relation_search_teacher_request,				// 搜索可拜为师父的在线玩家
	s2r_relation_search_student_request,				// 搜索可收为弟子的在线玩家
	s2r_relation_set_training_option_request,			// 修改"允许拜师/允许收徒"选项
	s2r_relation_get_training_option_request,			// 获取"允许拜师/允许收徒"选项
	s2r_relation_add_teacher_request,					// 拜师申请
	s2r_relation_add_student_request,					// 收徒申请
	s2r_relation_add_teacher_respond,					// 答复拜师申请
	s2r_relation_add_student_respond,					// 答复收徒申请
	s2r_relation_training_graduate_request,				// 出师
	s2r_relation_training_checkin_request,				// 弟子报到
	s2r_relation_training_view_info_request,			// 请求查看申请拜师/收徒者的信息
	s2r_relation_training_player_info_request,			// 指定玩家的信息
	s2r_relation_training_left_checking_time_request,	// 查看自己或者自己当前弟子的剩余报到时间
	s2r_relation_training_make_friend_request,			// 通过师徒系统成为好友
	s2r_relation_training_apply_request,				// 拜师
	s2r_relation_training_apply_failed_respond,			// 拜师失败
	s2r_relation_add_bind_coin_request,					// 密友绑定积分
	s2r_relation_apply_gain_coin_request,				// 申请领取绑定金币
	s2r_relation_gain_ib_coin_request,					// 领取绑定金币
	s2r_relation_cancel_gain_coin_request,				// 取消领取金币

    // ---------------- 寄卖行相关 --------------------------------------
    s2r_auction_lookup_request,
    s2r_auction_bid_request,
    s2r_auction_sell_request,
    s2r_auction_cancel_request,
    
	// ---------------------   PVE竞技场相关   -----------------------------
	s2r_load_arena_record_request,
	s2r_add_arena_record_request,

	// ---------------------   服务器各种记录   -----------------------------
	s2r_get_server_record_request,
	s2r_add_kinbattle_record_request,
	s2r_set_kinbattle_winnerId_request,
	s2r_set_last_winkin_request,
	s2r_set_bank_invest_request,
	s2r_buy_all_purchase_request,


	// -----------------------------------------
    s2r_achievement_global_announce_request,

    s2r_designation_global_announce_request,

	// ---------------- 跨服战场相关 --------------------------------------
	s2r_bzone_transfer_request,
	s2r_bzone_return_request,
	s2r_bzone_get_data_request,
	s2r_get_multi_award_request,
	s2r_get_crosssv_exdata_request,

	// ---------------- 跨服组队相关 --------------------------------------
	s2r_create_overall_team_request,		// 创建跨服队伍
	s2r_join_overall_team_request,
	s2r_leave_overall_team_request,
	s2r_member_prepare_request,
	s2r_get_player_teaminfo_request,
	s2r_kick_overall_player_request,
	s2r_start_mission_request,
	s2r_member_countdown_request,
	s2r_broadcast_overallteam_msg,
	s2r_check_online_leave_request,

    // ---------------- 统计数据相关 ------------------------------------
    s2r_update_stat_data_request,
    s2r_query_stat_id_request,
    s2r_map_copy_keep_player,

    s2r_change_ext_point_request,
    s2r_set_charge_flag_request,
    s2r_active_present_code_request,

    s2r_apex_protocol,
    
    s2r_report_farmer_player_request,

    s2r_game_card_sell_request,
    s2r_game_card_buy_request,
    s2r_game_card_lookup_request,
    s2r_game_card_cancel_request,

    s2r_apply_mentor_data,
    s2r_update_max_apprentice_num,
    s2r_add_mentor_value_request,

    s2r_transfer_log,
    
	//答题相关
	s2r_quiz_questions_request,
	s2r_quiz_sumit_score_request,
	s2r_quiz_name_list_request,

	//支付相关
	s2r_pay_order_request,
	s2r_pay_order_finish_request,

	s2r_delete_battle_map_request,

	//执行插入的GM指令
	s2r_GM_send_award_success_request,
	s2r_Login_Find_Sql,

	//Log同中心服
	s2r_log_info_save_request,
 
    s2r_protocol_end
};

// 下行协议定义
enum KR2S_PROTOCOL
{
	r2s_protocol_begin = 0,

	r2s_handshake_respond,
	r2s_bzone_handshake_respond,
    r2s_quit_notify,
	r2s_create_map_notify,
    r2s_finish_create_map_notify,
	r2s_delete_map_notify,
	//6
    
    r2s_player_login_request,
	r2s_search_map_respond,
	r2s_load_map_respond,
    r2s_transfer_player_request,
    r2s_transfer_player_respond,
	r2s_bzone_transfer_player_respond,
	r2s_bzone_return_respond,
    r2s_confirm_player_login_respond,
	r2s_get_multi_award_respond,
	r2s_get_crosssv_exdata_respond,

	r2s_kick_account_notify,
	//17
	
    // ------------- 组队相关协议 --------------------------------	
	r2s_invite_player_join_team_request,
    r2s_apply_join_team_request,
    r2s_team_create_notify,
    r2s_team_add_member_notify,
    r2s_team_del_member_notify,
    r2s_team_change_authority_notify,
    r2s_team_set_loot_mode_notify,
    r2s_team_disband_notify,
    r2s_sync_team_member_online_flag,
    r2s_sync_team_member_max_lmr,
    r2s_sync_team_member_current_lmr,
    r2s_sync_team_member_misc,
    r2s_sync_team_member_position,
    r2s_party_message_notify,
    r2s_team_set_formation_leader_notify,
    r2s_team_set_mark_respond,
    r2s_team_camp_change,
    r2s_team_level_up_raid_notify,
    r2s_team_change_member_group_notify,
	//36
	
    // ---------------- 聊天相关 --------------------------------------
    r2s_player_chat_error,
    r2s_player_chat_whisper_success_respond, 
    r2s_chat_message,

    // ---------------- 好友相关 --------------------------------------
    r2s_apply_fellowship_data_respond,
    r2s_add_fellowship_respond, 
    r2s_get_fellowship_name_respond, 
    r2s_sync_fellowship_mapid, 
    r2s_sync_fellowship_player_level, 
    r2s_sync_fellowship_player_forceid, 
    r2s_add_fellowship_notify,
	//46
	
    // ---------------- 邮件系统 --------------------------------------
    r2s_send_mail_respond,
    r2s_get_maillist_respond,
    r2s_sync_mail_content,
    r2s_give_mail_money_to_player,
    r2s_give_mail_item_to_player,
    r2s_new_mail_notify,
    r2s_mail_general_respond,
    // ----------------------------------------------------------------

	r2s_sync_mid_map_mark,
    
    // ---------------- 副本相关 ---------------------------------------
    r2s_map_copy_do_clear_player_prepare,
    r2s_map_copy_do_clear_player,
    r2s_set_map_copy_owner,

    // ---------------- 交通相关 ---------------------------------------
    r2s_sync_road_track_info,
    
    // --------------------- 角色操作相关 -----------------------------
    r2s_save_role_data_respond,
    
    r2s_sync_role_data,

	//60
	
    r2s_load_role_data,
	r2s_delay_load_role_data,

	r2s_gm_command,

	r2s_remote_lua_call,

    r2s_join_battle_field_queue_respond,
    r2s_leave_battle_field_queue_respond,

    r2s_battle_field_notify,

    r2s_sync_battle_field_list,
    r2s_accept_join_battle_field_respond,

    r2s_leave_battle_field_respond,

    r2s_sync_pq,
    r2s_delete_pq,
    r2s_pq_value_change,
    r2s_pq_finish,
    
    r2s_sync_camp_info,
    r2s_apply_set_camp_respond,

    // ---------------------   家族相关   -----------------------------
    r2s_sync_tong_member_info,
    r2s_delete_tong_member_notify,
    r2s_apply_tong_info_respond,
    r2s_update_tong_client_data_version,

    r2s_invite_player_join_tong_request,

    r2s_change_player_tong_notify,
    r2s_tong_broadcast_message,
    r2s_get_tong_description_respond,
    r2s_sync_tong_online_message,

    r2s_apply_open_tong_repertpry_respond,
	r2s_sync_apply_join_tong_list,
    r2s_sync_tong_repertory_page_respond,

    r2s_take_tong_repertory_item_respond,
	r2s_put_tong_repertory_item_respond,
    r2s_put_tong_repertory_item_fail_respond,
    r2s_stack_tong_repertory_item_fail_respond,

    r2s_get_tong_salary_respond,

    r2s_sync_tong_history_respond,

	r2s_kin_message_notify,
	r2s_get_back_item_from_repertory_respond,

	r2s_get_freejoin_tong_list_respond,
	r2s_apply_join_tong_respond,
	r2s_sync_kin_opentimes,

	r2s_sync_tong_rank_info,

	// ---------------------  排行榜相关 ---------------------------------
	r2s_get_rank_idx_respond,
	r2s_get_rank_id_respond,
	r2s_swap_rank_respond,

	// ---------------------  修仙相关 ---------------------------------
	r2s_sync_escort_members,
	r2s_sync_escort_state,
	r2s_take_escort_award_respond,
	r2s_sync_rob_escort_members,
	r2s_update_escort_state,
	r2s_rescue_player_respond,
	r2s_take_robescort_award_respond,

	// ---------------------  好友相关 ---------------------------------
	r2s_relation_start_sync_list_respond,				// 告诉客户端要开始同步了
	r2s_relation_sync_list_respond,						//  同步好友列表
	r2s_relation_online_notify,							// 通知好友自己上线/下线
	r2s_relation_del_relation_respond,					// 删除关系
	r2s_relation_friend_info_respond,					// 同步好友信息（等级、门派、亲密度）
	r2s_relation_notify_respond,						// 通知关系建立
	r2s_relation_delete_player_respond,					// 对方删号通知
	r2s_relation_pre_add_favor_respond,					// 预增加好感度结果
	r2s_relation_training_error_respond,				// 师徒错误通知
	r2s_relation_search_teacher_respond,				// 可拜为师父的在线玩家搜索结果
	r2s_relation_search_student_respond,				// 可收为弟子的在线玩家搜索结果
	r2s_relation_add_teacher_respond,					// 转发拜师申请
	r2s_relation_add_student_respond,					// 转发收徒申请
	r2s_relation_training_option_respond,				// 获取拜师/收徒选项
	r2s_relation_training_view_info_respond,			// 查看申请拜师/收徒者的信息
	r2s_relation_training_player_info_respond,			// 指定玩家的信息
	r2s_relation_myself_left_checkin_time_respond,		// 自己的剩余报到时间
	r2s_relation_student_left_checkin_time_respond,		// 自己当前弟子的剩余报到时间
	r2s_relation_add_training_respond,					// 拜师
	r2s_relation_apply_ib_coin_result_respond,			// 申请IB积分结果反馈
	r2s_relation_gain_ib_coin_result_respond,			// 领取结果反馈
	r2s_relation_award_coin_by_friend_favor_respond,	// 根据亲密度奖励


    // ---------------------   寄卖行相关   -----------------------------
    r2s_auction_lookup_respond,
    r2s_auction_bid_respond,
    r2s_auction_sell_respond,
    r2s_auction_cancel_respond,
    r2s_auction_message_notify,

	// ---------------------   服务器各种记录   -----------------------------
	r2s_get_server_record_respond,
	r2s_get_all_purchase_buy_respond,

	// ---------------------   跨服组队相关   -----------------------------
	r2s_sync_overall_teams,
	r2s_sync_overall_teamops_result,
	r2s_sync_player_overall_team,
	r2s_member_countdown_notify,
	r2s_check_member_online_respond,
	r2s_multi_trans_request,


	// ---------------------   PVE竞技场相关   -----------------------------
	r2s_load_arena_record_respond,

    r2s_achievement_global_announce_respond,

    r2s_designation_global_announce_respond,

    r2s_sync_global_system_value,

    r2s_query_stat_id_respond,
    
    r2s_change_ext_point_respond,
    r2s_set_charge_flag_respond,
    r2s_sync_zone_charge_flag,
    r2s_active_present_code_respond,

    r2s_apex_protocol,

    r2s_set_farmer_limit_flag_request,

    r2s_game_card_sell_respond,
    r2s_game_card_buy_respond,
    r2s_game_card_lookup_respond,
    r2s_game_card_cancel_respond,

    r2s_sync_mentor_data,
    r2s_delete_mentor_record,
    r2s_update_mentor_record,
    r2s_seek_mentor_yell,
    r2s_seek_apprentice_yell,

	//答题相关
	r2s_quiz_qustions_respond,
	r2s_quiz_name_list_respond,

	//支付相关
	r2s_pay_order_respond,

	//执行插入的GM指令
	r2s_GM_send_award,

	r2s_protocol_end
};



#pragma	pack(1)

// 上行协议结构定义

struct S2R_HANDSHAKE_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    int     nGameWorldLowerVersion;
    int     nGameWorldUpperVersion;
    time_t  nServerTime;
};

struct S2R_PING_SIGNAL : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwReserved;
};

struct S2R_UPDATE_PERFORMANCE : INTERNAL_PROTOCOL_HEADER
{
    int     nIdleFrame;
    size_t  uMemory;
    int     nPlayerCount;
    int     nConnectionCount;
    UINT   uNetworkFlux;
};

struct S2R_DELETE_BATTLE_MAP : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwMapId;
	DWORD dwMapCopyIndex;
};

struct S2R_CREATE_MAP_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwMapID;			
	int		    nMapCopyIndex;		
	BOOL		bResult;			
};

// GameServer对登录请求的应答消息 
struct S2R_PLAYER_LOGIN_RESPOND : IDENTITY_HEADER
{
	DWORD		dwPlayerID;
	BOOL		bPermit;
	GUID		Guid;
	DWORD       dwGSAddr;//游戏服ip地址
	int			nGSPort; //游戏服端口
};

struct S2R_CONFIRM_PLAYER_LOGIN_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
};

struct S2R_PLAYER_LEAVE_GS : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
};

struct S2R_SEARCH_MAP_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
	DWORD		dwMapID;
    int         nMapCopyIndex;
    int         nPosX;
    int         nPosY;
    int         nPosZ;
	BOOL		bOnThisGs;
};

struct S2R_LOAD_MAP_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwMapID;
	DWORD		dwMapOwnerId;
	DWORD		dwParam1;
	BOOL        bToThisServer; // 是否在本服创建地图
};

// 玩家跨服务器,源服务器发出跨服请求(附带角色基本数据)
struct S2R_TRANSFER_PLAYER_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD           dwRoleID;
    BOOL            bChargeFlag;
	tagExtPointInfo	ExtPointInfo;			// 可用的附送点
    BOOL            bExtPointLock;
    int             nLastExtPointIndex;
    int             nLastExtPointValue;
    time_t          nEndTimeOfFee;
	int             nCoin;
	int             nBattleFieldSide;
    KRoleBaseInfo   RoleBaseInfo;
};

// 玩家跨服返回原服务器
struct S2R_BZONE_RETURN_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwRoleID;
	INT				nExValue;
	KRoleBaseInfo   RoleBaseInfo;

	DWORD uExDataLen;
	BYTE byExData[0];
};

struct S2R_GET_MULTI_AWARD_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
};

struct S2R_GET_CROSSSV_EXDATA_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
};

// 玩家跨服务器,源服务器发出跨服请求(附带角色基本数据)
struct S2R_BZONE_TRANSFER_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwRoleID;
	KRoleBaseInfo   RoleBaseInfo;
	INT				nPlayerIdx;

	DWORD			uRoleDataLen;
	BYTE			byRoleData[0];
};

// 玩家跨服时,目标服务器的响应消息
struct S2R_TRANSFER_PLAYER_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
	BOOL		bSucceed;
	DWORD	    dwAddress;
	WORD		wPort;
	GUID		Guid;
};

// 聊天
struct S2R_CHAT_MESSAGE : INTERNAL_PROTOCOL_HEADER
{
	BYTE  byMsgType;
	BYTE  byShownType;    // 在客户端显示方式
	DWORD dwSenderId;
	char  szSender[_NAME_LEN];
	DWORD dwReceiverId;
	char  szReceiver[_NAME_LEN];
	BYTE  byTalkData[0];
};

struct S2R_PLAYER_CHAT_ERROR : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwReceiverId;
	int   nErrorCode;
};

// <--------------------- 副本相关 ---------------------------------------
struct S2R_SAVE_SCENE_PLAYER_LIST : INTERNAL_PROTOCOL_HEADER
{
	DWORD    dwMapID;
    int      nMapCopyIndex;
    int      nPlayerCount;
    DWORD    dwPlayerList[0];
};

struct S2R_SAVE_SCENE_DATA : INTERNAL_PROTOCOL_HEADER
{
	DWORD    dwMapID;
    int      nMapCopyIndex;
    size_t   uDataLen;
    BYTE     byData[0];
};

struct S2R_PLAYER_ENTER_SCENE_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
    DWORD    dwPlayerID;
    DWORD    dwMapID;
    int      nMapCopyIndex;
};

struct S2R_RESET_MAP_COPY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD    dwPlayerID;
	DWORD    dwMapID;
	int      nMapCopyIndex;
};

struct S2R_LOAD_ROLE_DATA_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD   dwPlayerID;
};

struct S2R_LOAD_DELAY_ROLE_DATA_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	BYTE	byModuleId;	// 请求模块id
	DWORD	dwSrcId;
	DWORD   dwPlayerID;
	CHAR	szRoleName[MAX_NAME_LEN];
};

struct S2R_CHANGE_ROLE_LEVEL_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    BYTE  byLevel;
};

struct S2R_CHANGE_ROLE_FORCEID_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    BYTE  byForceID;
};

// ---------------------- 
struct S2R_SEND_GM_MESSAGE : INTERNAL_PROTOCOL_HEADER 
{
    char    szGmName[_NAME_LEN];
    char    szMessage[0];
};

struct S2R_SEND_GM_CHN : INTERNAL_PROTOCOL_HEADER 
{
    char    szAccount[_NAME_LEN];
    char    szRole[_NAME_LEN];
    char    szMessage[0];
};

struct S2R_SEND_GM_COMMAND_GS : INTERNAL_PROTOCOL_HEADER
{
    char    szRoleName[_NAME_LEN];
    char    szCommand[0];
};

struct S2R_SEND_GM_COMMAND_GC : INTERNAL_PROTOCOL_HEADER
{
    char    szCommand[0];
};

struct S2R_REMOTE_LUA_CALL :  INTERNAL_PROTOCOL_HEADER
{
	size_t uParamLen;
	BYTE   byParam[0];
};


struct S2R_CHANGE_EXT_POINT_REQUEST : INTERNAL_PROTOCOL_HEADER 
{
    DWORD   dwPlayerID;
    WORD    wExtPointIndex;
    WORD    wChangeValue;
};

struct S2R_SYNC_ROLE_DATA : INTERNAL_PROTOCOL_HEADER
{
    DWORD   dwRoleID;
    size_t  uOffset;
    BYTE    byData[0];
};

struct S2R_SAVE_ROLE_DATA : INTERNAL_PROTOCOL_HEADER
{
    DWORD           dwRoleID;
	int             nUserValue;
	int             nUserValue2;
	size_t          uRoleDataLen;
    KRoleBaseInfo   BaseInfo;
};

struct S2R_CREATE_DELAY_ROLE_DATA: INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwRoleID;
	KDelayRoleData	DelayRoleData;
};

struct S2R_SAVE_DELAY_ROLE_DATA: INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwRoleID;
	KDelayRoleData	DelayRoleData;
};

struct S2R_UPDATE_STAT_DATA_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    int               	nCount;
    KSTAT_DATA_MODIFY 	Values[0];
};

struct S2R_QUERY_STAT_ID_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    char            	szName[STAT_DATA_NAME_LEN];
};

struct S2R_MAP_COPY_KEEP_PLAYER : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwMapID;
    int   nCopyIndex;
    DWORD dwPlayerID;
};

// ---------------------- 家族相关 ------------------------------------------
struct S2R_APPLY_TONG_ROSTER_REQUEST : INTERNAL_PROTOCOL_HEADER 
{
	DWORD   dwPlayerID;
	int     nClientUpdateCount;
	int     nLastUpdateFrame;
	int     nMemberCount;
	DWORD   dwMemberList[0];
};

struct S2R_APPLY_TONG_INFO_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	int     nClientUpdateCount;
	int     nLastUpdateFrame;
};

struct S2R_APPLY_TONG_REPERTORY_PAGE_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byPageIndex;
	int     nLastUpdateCounter;
};

struct S2R_APPLY_CREATE_TONG_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	char  szTongName[_NAME_LEN];
	BYTE  byCamp;
	BYTE  byTemplateIndex;
};

struct S2R_INVITE_PLAYER_JOIN_TONG_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	char  szNewMemberName[_NAME_LEN];
};

struct S2R_INVITE_PLAYER_JOIN_TONG_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwInviterID;
	DWORD dwTongID;
	BYTE  byAccept;
};

struct S2R_APPLY_KICK_OUT_TONG_MEMBER : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwMemberID;
};

struct S2R_MODIFY_TONG_INFO_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byInfoType;
	WORD    wDataLen;
	BYTE    byData[0];
};

struct S2R_MODIFY_TONG_SCHEMA_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byGroupIndex;
	BYTE    byModifyType;
	BYTE    byData[0];
};

struct S2R_GET_TONG_RANK_INFO_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwTongId;
	INT nRankId;
};

struct S2R_APPLY_QUIT_TONG_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
};

struct S2R_CHANGE_TONG_MEMBER_GROUP_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwTargetMemberID;
	BYTE  byTargetGroup;
};

struct S2R_CHANGE_TONG_MASTER_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwNewMasterID;
};

struct S2R_CHANGE_TONG_MEMBER_REMARK_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwTargetMemberID;
	char  szRemark[_NAME_LEN];
};

struct S2R_GET_TONG_DESCRIPTION_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwTongID;
};

struct S2R_SAVE_MONEY_IN_TONG_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	DWORD   dwPealNum;
	DWORD   dwJadeiteNum;
	DWORD   dwAgateNum;			//改成捐的银两
	DWORD   dwDiamondNum;		//改成捐的元宝
	DWORD   dwContributionAdd; // 增加贡献度
};

struct S2R_PAY_TONG_SALARY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	int     nTotalSalary;
};

struct S2R_GET_TONG_SALARY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
};

struct S2R_GET_TONG_SALARY_FAIL_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	DWORD   dwTongID;
	int     nSilver;
};

struct S2R_CHANGE_TONG_CAMP_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	BYTE  byCamp;
};

struct S2R_APPLY_OPEN_TONG_REPERTORY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
};

struct S2R_TAKE_TONG_REPERTORY_ITEM_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byPageIndex;
	BYTE    byRepertoryPos;
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	int     nLastUpdateCounter;
};

struct S2R_TAKE_TONG_REPERTORY_ITEM_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwTongID;
	BYTE  byPageIndex;
	BYTE  byPagePos;
	BOOL  bSuccess;
};

struct S2R_PUT_TONG_REPERTORY_ITEM_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	BYTE    byPageIndex;
	BYTE    byPagePos;
	DWORD   dwItemIdx;
	BYTE    byItemDataLen;
	BYTE    byItemData[0];
};

struct S2R_PUT_TONG_REPERTORY_ITEM_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byPageIndex;
	BYTE    byPagePos;
	BOOL    bFindItem;
	BYTE    byItemDataLen;
	BYTE    byItemData[0];
};

struct S2R_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    bySrcPageIndex;
	BYTE    byDstPageIndex;
	BYTE    bySrcPagePos;
	BYTE    byDstPagePos;
	int     nSrcUpdateCounter;
	int     nDstUpdateCounter;
};

struct S2R_STACK_TONG_REPERTORY_ITEM_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	DWORD   dwTongID;
	BYTE    byPageIndex;
	BYTE    byPagePos;
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	BYTE    byTabType;
	WORD    wTabIndex;
	WORD    wStackNum;
	WORD    wMaxStackNum;
	int     nLastUpdateCounter;
};

struct S2R_REMOVE_TONG_REPERTORY_ITEM_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD  dwPlayerID;
	BYTE   byPageIndex;
	BYTE   byPagePos;
	INT    nLastUpdateCounter;
};

struct S2R_SET_TONG_ACTIVITY_OPENTIME_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerId;
	DWORD dwTongId;
	DWORD dwTongActivityIndex;
	DWORD dwTongActivityOpenTime;
};

struct S2R_GET_BACK_ITEM_FROM_REPERTORY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD  dwPlayerID;
	DWORD  dwTongID;
	BYTE   byBoxIndex;
	BYTE   byBoxPos;
	GUID   sGuid;
	INT    nLastUpdateCounter;
};

struct S2R_GET_FREEJOIN_TONG_LIST_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD	dwPlayerID;
	INT		nLastUpdateCounter;
};

struct S2R_APPLY_JOIN_TONG_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwTongID;
};

struct S2R_SET_TONG_FREEJOIN_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD	dwPlayerID;
	DWORD	dwTongID;
	BOOL	bValue;
};

// ---------------------- 排行榜相关 ------------------------------------------
struct S2R_GET_RANK_IDX_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	BYTE byRankId;
	DWORD dwSrcId;
	DWORD dwId;
};

struct S2R_GET_RANK_ID_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	BYTE byRankId;
	DWORD dwSrcId;
	INT	 nRankIdx;
};

struct S2R_SWAP_RANK_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	BYTE byRankId;
	DWORD dwSrcId;
	DWORD dwDstId;
};

struct S2R_MODIFY_RANK_VALUE_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	BYTE byRankId;
	DWORD dwId;
	INT nValue;
	DWORD dwModifyTime;
	BOOL bIfRoll;
};

// ---------------------- 修仙相关 ------------------------------------------
struct S2R_START_ESCORT_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	DWORD dwProtectRoleId;
	INT nEscortLevel;
};

struct S2R_REFRESH_ESCORTS_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
};

struct S2R_TAKE_ESCORT_AWARD_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
};

struct S2R_ROB_PLAYER_ESCORT_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	INT nRobPlayerId;
};

struct S2R_DEL_ROB_ESCORT_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	INT nRobPlayerId;
};

struct S2R_TAKE_ROB_AWARD_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	INT nRobPlayerId;
	BYTE byEscortLevel;
};

struct S2R_REMOVE_ESCORT_WATCH_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
};

struct S2R_SPEED_UP_ESCORT : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	BYTE bySpeed;	// 1加速 0直接完成
};

struct S2R_RESCUE_PLAYER_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	INT nRescuePlayerId;
};

// -------------- 邮件系统相关　------------------------------->

struct S2R_SEND_MAIL_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	BYTE            byRequestID;
	char            szReceiverName[_NAME_LEN];
	BYTE            byData[0];                      // KMail
};

struct S2R_SEND_GLOBAL_MAIL_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	int             nLifeTime;
	BYTE            byData[0];                      // KMail
};

struct S2R_GET_MAILLIST_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	DWORD           dwStartID;
};

struct S2R_QUERY_MAIL_CONTENT : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	DWORD           dwMailID;
	BYTE            byMailType;
};

struct S2R_ACQUIRE_MAIL_MONEY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	DWORD           dwMailID;
};

struct S2R_ACQUIRE_MAIL_ITEM_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	DWORD           dwMailID;
	BYTE            byIndex;
};

struct S2R_GIVE_MAIL_MONEY_TO_PLAYER_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	DWORD           dwMailID;
	BYTE            byResult;
};

struct S2R_GIVE_MAIL_ITEM_TO_PLAYER_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	DWORD           dwMailID;
	BYTE            byIndex;
	BYTE            byResult;
};

struct S2R_SET_MAIL_READ : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	DWORD           dwMailID;
};

struct S2R_DELETE_MAIL : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	DWORD           dwMailID;
};

struct S2R_RETURN_MAIL : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	DWORD           dwMailID;
};

// ---------------------- 拍卖行 ------------------------------------------
struct S2R_AUCTION_LOOKUP_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	BYTE                    byRequestID;
	DWORD                   dwPlayerID;
	KAUCTION_LOOKUP_PARAM   Param;
};

struct S2R_AUCTION_BID_REQUEST : INTERNAL_PROTOCOL_HEADER 
{
	DWORD   dwBidderID;
	DWORD   dwSaleID;
	DWORD   dwCRC;
	int     nPrice;
};

struct S2R_AUCTION_SELL_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwSellerID;
	char    szSaleName[SALE_NAME_LEN];
	int     nAucGenre;
	int     nAucSub;
	int     nRequireLevel;
	int     nQuality;
	int     nStartPrice;
	int     nBuyItNowPrice;
	int     nStackNum;
	int     nCustodyCharges;
	int     nLeftTime;
	size_t  uItemDataLen;
	BYTE    byItemData[0];
};

struct S2R_AUCTION_CANCEL_REQUEST : INTERNAL_PROTOCOL_HEADER 
{
	DWORD   dwPlayerID;
	DWORD   dwSaleID;
};

//----------------------------------好友相关-----------------------------------
/**********************************************************
 * Description	: 添加玩家关系
 * Protocol		: s2r_relation_add_relation_request
**********************************************************/
struct S2R_RELATION_ADD_RELATION_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	BYTE	nType;					// 玩家关系类型
	BYTE	nAppRole;				// 申请者角色（1为主位关系，0为次位关系）
	INT		nAppID;					// 申请玩家ID
	CHAR	szRelation[_NAME_LEN];	// 关系玩家名字
};

/**********************************************************
 * Description	: 删除关系
 * Protocol		: s2r_relation_del_relation_request
**********************************************************/
struct S2R_RELATION_DEL_RELATION_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	BYTE	nType;					// 玩家关系类型
	BYTE	nAppRole;				// 申请者角色（1为主位关系，0为次位关系）
	INT		nAppID;					// 申请玩家ID
	CHAR	szRelation[_NAME_LEN];	// 关系玩家名字
};

/**********************************************************
 * Description	: 
 * Protocol		: s2r_relation_add_relation_favor_request			增加亲密度
 * Protocol		: s2r_relation_act_add_favor_request			实际加亲密度（和预增加亲密度对应）
 * Protocol		: s2r_relation_cancel_pre_add_favor_request	取消预增加亲密度
**********************************************************/
struct S2R_RELATION_ADD_RELATION_FAVOR_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;				// 玩家ID
	CHAR	szRelation[_NAME_LEN];	// 关系玩家名字
	BYTE	nMethod;				// 增加亲密度的途径，0: 非IB，1：IB道具
	INT		nFavor;					// 要增加的亲密度
};

struct S2R_RELATION_ADD_BIND_COIN_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT	nPlayerId;		// 玩家id
	INT nCoin;			// 玩家花费金币数量
	CHAR szCloseFriend[_NAME_LEN]; // 密友名字
};

struct S2R_RELATION_APPLY_GAIN_COIN_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
};

struct S2R_RELATION_GAIN_IB_COIN_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	CHAR szTarget[_NAME_LEN];
};

/**********************************************************
 * Description	: 预增加亲密度（确保下一次增加亲密度成功）
 * Protocol		: s2r_relation_pre_add_relation_favor_request
**********************************************************/
struct S2R_RELATION_PRE_ADD_RELATION_FAVOR_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;				// 玩家ID
	CHAR	szRelation[_NAME_LEN];	// 关系玩家名字
	BYTE	nMethod;				// 增加好感度的途径，0: 非IB，1：IB道具
	INT		nFavor;					// 要增加的好感度
	INT		nIdentity;				// 自定义标识（用于回调）
};

/**********************************************************
 * Description	: 
 * Protocol		: 搜索可拜为师父的在线玩家 s2r_relation_search_teacher_request
 * Protocol		: 搜索可收为弟子的在线玩家 s2r_relation_search_student_request
**********************************************************/
struct S2R_RELATION_SEARCH_PLAYER_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;				// 发起搜索的玩家ID
//	INT		nFirstID;				// 搜索的起始玩家ID
};

/**********************************************************
 * Description	: 修改"允许拜师/允许收徒"选项
 * Protocol		: s2r_relation_set_training_option_request
**********************************************************/
struct S2R_RELATION_SET_TRAINING_OPTION_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;				// 玩家ID
	BYTE	byTeacherOrStudent;		// 1表示是允许拜师选项，0表示是允许收徒选项
	BYTE	byOption;				// 1为允许，0为不允许
};

/**********************************************************
 * Description	: 修改"允许拜师/允许收徒"选项
 * Protocol		: s2r_relation_add_teacher_request	拜师申请
 * Protocol		: s2r_relation_add_student_request	收徒申请
**********************************************************/
struct S2R_RELATION_ADD_TRAINING_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;				// 申请玩家ID
	CHAR	szTarget[_NAME_LEN];	// 目标角色名字
};

/**********************************************************
 * Description	:
 * Protocol		: s2r_relation_add_teacher_respond 答复拜师申请
 * Protocol		: s2r_relation_add_student_respond 答复收徒申请
**********************************************************/
struct S2R_RELATION_ADD_TRAINING_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	BYTE	byAnswer;					// 1表示接受申请，0表示拒绝申请
	INT		nPlayerID;					// 收到申请的玩家ID
	CHAR	szApplicant[_NAME_LEN];		// 申请者角色名
};

/**********************************************************
 * Description	: 出师
 * Protocol		: s2r_relation_training_graduate_request
**********************************************************/
struct S2R_RELATION_TRAINING_GRADUATE_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	CHAR	szStudent[_NAME_LEN];		// 弟子角色名
	CHAR	szTeacher[_NAME_LEN];		// 师父角色名
};

/**********************************************************
* Description	: 
* Protocol		: s2r_relation_training_checkin_request		弟子报到
* Protocol		: s2r_relation_get_training_option_request	获取拜师/收徒选项
**********************************************************/
struct S2R_RELATION_PLAYER_ID_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
};

/**********************************************************
* Description	: 查看申请拜师/收徒者的信息
* Protocol		: s2r_relation_training_view_info_request
**********************************************************/
struct S2R_RELATION_TRAINING_VIEW_INFO_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
	CHAR	szApplicant[_NAME_LEN];
};

/**********************************************************
* Description	: 指定玩家的信息
* Protocol		: s2r_relation_training_player_info_request
**********************************************************/
struct S2R_RELATION_TRAINING_PLAYER_INFO_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT					nViewerID;		// 查看者ID
	KPLAYER_EQUIPVIEW	sPlayerInfo;	// 玩家信息
};

//s2r_relation_training_left_checking_time_request
/**********************************************************
* Description	: 查看自己或者自己当前弟子的剩余报到时间
* Protocol		: s2r_relation_training_left_checking_time_request
**********************************************************/
struct S2R_RELATION_TRAINING_LEFT_CHECKING_TIME_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
	BYTE	bySelfView;		// 1：查看自己的剩余报到时间；0：查看自己当前弟子的剩余报到时间
};

/**********************************************************
* Description	: 通过师徒系统成为好友
* Protocol		: s2r_relation_training_make_friend_request
**********************************************************/
struct S2R_RELATION_TRAINING_MAKE_FRIEND_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
	CHAR	szRelation[_NAME_LEN];
};

/**********************************************************
* Description	: 拜师
* Protocol		: s2r_relation_training_apply_request
**********************************************************/
struct S2R_RELATION_TRAINING_APPLY_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
	CHAR	szTeacher[_NAME_LEN];
};

//----------------------------答题相关-------------------------------------------------
struct S2R_QUIZ_QUESTION_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nIssueId;
	//INT nQuestionCount;
	INT nMaxQuestionCount;
};

struct S2R_QUIZ_SUMIT_INTEGRAL_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoldId;
	INT nIntegral;
	CHAR szName[_NAME_LEN];
};

struct S2R_QUIZ_NAME_LIST_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nRequest;
};

struct S2R_PAY_ORDER_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
};

struct S2R_PAY_ORDER_FINISH_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
	CHAR szOrderno[64];
};


// ---------------------- 下行协议结构定义 ---------------------------------------

struct R2S_HANDSHAKE_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	int     nWorldIndex;
    time_t  nBaseTime;
	int     nStartFrame;
	int     nDevMode;
};

struct R2S_BZONE_HANDSHAKE_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	time_t  nBaseTime;
	int     nStartFrame;
	int     nDevMode;
};

struct R2S_QUIT_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
    int nNothing;
};

// 地图管理相关协议结构
struct R2S_CREATE_MAP_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwMapID;		
	int		    nMapCopyIndex;
    size_t      uDataLen;
    BYTE        byData[0];
};

struct R2S_LOAD_MAP_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	int		    nCode;
	DWORD		dwMapID;		
	int		    nMapCopyIndex;
	int			nParam1;
};

struct R2S_FINISH_CREATE_MAP_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
};

struct R2S_DELETE_MAP_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwMapID;				
	int		    nMapCopyIndex;
};

// 玩家登录请求
struct R2S_PLAYER_LOGIN_REQUEST : IDENTITY_HEADER
{
    DWORD           dwRoleID;
    char            szRoleName[_NAME_LEN];
    char            szAccount[_NAME_LEN];
    int             nChargeFlag;            // 收费状态
	tagExtPointInfo	ExtPointInfo;			// 可用的附送点
    time_t          nEndTimeOfFee;          // 收费总截止时间
    int             nCoin;                  // 金币数
    DWORD           dwSystemTeamID;
    DWORD           dwTeamID;
    int             nBattleSide;
    DWORD           dwTongID;
	DWORD			dwKinMasterId;
	BYTE            byFarmerLimit;
	KRoleBaseInfo   BaseInfo;
};

struct R2S_CONFIRM_PLAYER_LOGIN_RESPOND : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    BOOL  bPermit;
};

struct R2S_SEARCH_MAP_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
	int         nResultCode;
    BOOL        bSwitchServer;
	DWORD		dwMapID;
	int		    nMapCopyIndex;
    int         nPosX;
    int         nPosY;
    int         nPosZ;
    int         nBattleFieldSide;
};

// 玩家跨服务器,GameCenter转发源服务器发出的跨服请求(附带角色基本数据)
struct R2S_TRANSFER_PLAYER_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD           dwRoleID;
    char            szRoleName[_NAME_LEN];
    char            szAccount[_NAME_LEN];
    BOOL            bChargeFlag;
	tagExtPointInfo	ExtPointInfo;
    BOOL            bExtPointLock;
    int             nLastExtPointIndex;
    int             nLastExtPointValue;
    time_t          nEndTimeOfFee;
    int             nCoin;
    DWORD           dwSystemTeamID;
    DWORD           dwTeamID;
    int             nBattleFieldSide;
    DWORD           dwTongID;
	DWORD			dwKinMasterId;
	BYTE            byFarmerLimit;
	KRoleBaseInfo   BaseInfo;
};

// 玩家跨服时,GameCenter转发目标服务器的响应消息
struct R2S_TRANSFER_PLAYER_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
	BOOL		bSucceed;
	DWORD	    dwAddress;
	WORD		wPort;
	GUID		Guid;
};

struct R2S_BZONE_TRANSFER_PLAYER_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwDstPlayerID;
	DWORD		dwPlayerID;
	BOOL		bSucceed;
	DWORD	    dwAddress;
	WORD		wPort;
	GUID		Guid;
};

struct R2S_BZONE_RETURN_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwDstPlayerID;
	DWORD		dwPlayerID;
	BOOL		bSucceed;
	DWORD	    dwAddress;
	WORD		wPort;
	GUID		Guid;
};

struct R2S_KICK_ACCOUNT_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
	DWORD       dwPlayerID;
};

struct R2S_GET_MULTI_AWARD_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
	INT nMapId;
	INT nStar;
};

struct R2S_GET_CROSSSV_EXDATA_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
	DWORD uExDataLen;
	BYTE byExData[0];
};

// -------------- 聊天相关协议结构定义 ------------------------
struct R2S_PLAYER_CHAT_ERROR : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwReceiverId;
    int   nErrorCode;
};

struct R2S_PLAYER_CHAT_WHISPER_SUCCESS_RESPOND : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwTalkerId;
    DWORD dwReceiverId;
};

struct R2S_CHAT_MESSAGE : INTERNAL_PROTOCOL_HEADER
{
    BYTE  byMsgType;
	BYTE  byShownType;    // 在客户端显示方式
    DWORD dwSenderId;
    char  szSender[_NAME_LEN];
    DWORD dwReceiverId;
    char  szReceiver[_NAME_LEN];
    BYTE  byTalkData[0];
};

// <--------------------- 副本相关 ---------------------------------------
struct R2S_MAP_COPY_DO_CLEAR_PLAYER_PREPARE : INTERNAL_PROTOCOL_HEADER
{
    DWORD    dwMapID;
    int      nMapCopyIndex;
    int      nType;
    int      nDelayTime;
};

struct R2S_MAP_COPY_DO_CLEAR_PLAYER : INTERNAL_PROTOCOL_HEADER
{
    DWORD    dwMapID;
    int      nMapCopyIndex;
};

struct R2S_SET_MAP_COPY_OWNER : INTERNAL_PROTOCOL_HEADER
{
    DWORD    dwMapID;
    int      nMapCopyIndex;
    DWORD    dwOwnerID;
};

// --------------------- 角色操作相关 -----------------------------

struct R2S_SAVE_ROLE_DATA_RESPOND : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    BOOL  bSucceed;
	DWORD uRoleDataLen;
	int   nUserValue;
	int   nUserValue2;
};

struct R2S_GM_COMMAND : INTERNAL_PROTOCOL_HEADER
{
    DWORD   dwPlayerID;
    char    szGmName[_NAME_LEN];
    char	szGmCommand[0];
};

struct R2S_REMOTE_LUA_CALL :  INTERNAL_PROTOCOL_HEADER
{
	size_t uParamLen;
	BYTE   byParam[0];
};


struct R2S_SYNC_GLOBAL_SYSTEM_VALUE : INTERNAL_PROTOCOL_HEADER 
{
    int     nGameWorldStartTime;
    int     nMaxPlayerLevel;
    int     nGameWorldChargeTime;
    int     nGameOperator;
    int     nChargeMode;
};

struct R2S_SYNC_ROLE_DATA : INTERNAL_PROTOCOL_HEADER
{
    DWORD   dwRoleID;
    size_t  uOffset;
    BYTE    byData[0];
};

struct R2S_LOAD_ROLE_DATA : INTERNAL_PROTOCOL_HEADER
{
    DWORD   dwRoleID;
    BOOL    bSucceed;
    size_t  uRoleDataLen;
    BYTE    byRoleData[0];
};

struct R2S_DELAY_LOAD_ROLE_DATA : INTERNAL_PROTOCOL_HEADER
{
	BYTE byModuleId;
	DWORD dwRoleId;
	DWORD dwSrcId;
	BOOL bSucceed;
	size_t  uDelayDataLen;
	BYTE    byDelayData[0];
};

struct R2S_QUERY_STAT_ID_RESPOND : INTERNAL_PROTOCOL_HEADER
{
    char    szName[STAT_DATA_NAME_LEN];
    int     nID;
};

struct R2S_CHANGE_EXT_POINT_RESPOND : INTERNAL_PROTOCOL_HEADER 
{
    DWORD   dwPlayerID;
    BOOL    bSucceed;
};

// ---------------------   家族相关   -------------------------------

struct R2S_SYNC_TONG_MEMBER_INFO : INTERNAL_PROTOCOL_HEADER
{
	DWORD           dwPlayerID;
	KTongMemberInfo MemberInfo;
};

struct R2S_DELETE_TONG_MEMBER_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	DWORD   dwMemberID;
};

struct R2S_APPLY_TONG_INFO_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byInfoType;
	BYTE    byData[0];
};

struct R2S_UPDATE_TONG_CLIENT_DATA_VERSION : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byCountType;
	int     nUpdateCount;
};

struct R2S_INVITE_PLAYER_JOIN_TONG_REQUEST: INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	DWORD   dwInviterID;
	DWORD   dwTongID;
	BYTE    byTongCamp;
	char    szInviterName[_NAME_LEN];
	char    szTongName[_NAME_LEN];
};

struct R2S_CHANGE_PLAYER_TONG_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	DWORD   dwTongID;
	DWORD	dwKinMasterId;
	BYTE    byReason;
	char    szTongName[_NAME_LEN];
};

struct R2S_TONG_BROADCAST_MESSAGE : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byCode;
	BYTE    byData[0];
};

struct R2S_GET_TONG_DESCRIPTION_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	DWORD   dwTongID;
	BYTE    byTongLevel;
	char    szTongName[_NAME_LEN];
	DWORD	dwMasterID;
	//DWORD	TodayContribute;//今日捐献
};

struct R2S_SYNC_TONG_ONLINE_MESSAGE : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	BYTE  byDataLen;
	char  szOnlineMessage[0];
};

struct R2S_SYNC_TONG_APPLY_JOIN_LIST: INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerId;
	DWORD   dwCount;
	BYTE    byData[0];
};

struct R2S_SYNC_TONG_REPERTORY_PAGE_RESPOND: INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byPageIndex;
	BYTE    byUpdateItemCount;
	BYTE    byInvailedItemCount;
	int     nServerCounter;
	BYTE    byGetRight;
	BYTE    byPutRight;
	BYTE    byData[0];
};

struct R2S_TAKE_TONG_REPERTORY_ITEM_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	DWORD   dwPutItemPlayerID;
	time_t  tRentPeriod;
	DWORD   dwTongID;
	BYTE    byPageIndex;
	BYTE    byPagePos;
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	BYTE    byItemDataLen;
	BYTE    byItemData[0];
};

struct R2S_PUT_TONG_REPERTORY_ITEM_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwTongID;
	DWORD dwItemID;
	BOOL  bFindItem;
	BYTE  byBoxIndex;
	BYTE  byBoxPos;
	BYTE  byPageIndex;
	BYTE  byPagePos;
};

struct R2S_PUT_TONG_REPERTORY_ITEM_FAIL_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	BYTE    byDataLen;
	BYTE    byItemData[0];
};

struct R2S_SYNC_KIN_ACTIVITY_OPENTIMES : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerId;
	INT   nCount;
	BYTE  byOpenTimesData[0];
};

struct R2S_STACK_TONG_REPERTORY_ITEM_FAIL_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byBoxIndex;
	BYTE    byBoxPos;
	BYTE    byTabType;
	WORD    wTabIndex;
	WORD    wStackNum;
};

struct R2S_APPLY_OPEN_TONG_REPERTPRY_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	char    szTongName[_NAME_LEN];
	BYTE    byRepertoryPageNum;
};

struct R2S_GET_TONG_SALARY_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	DWORD   dwTongID;
	int     nSilver;
};

struct R2S_SYNC_TONG_RANK_INFO : INTERNAL_PROTOCOL_HEADER
{
	INT nRankId;
	DWORD dwTongId;
	CHAR szTongName[_NAME_LEN];
	CHAR szMasterName[_NAME_LEN];
	INT nTongMemberNum;
};

struct R2S_SYNC_TONG_HISTORY_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD   dwPlayerID;
	BYTE    byType;
	DWORD   dwStartIndex;
	BYTE    byCount;
	BYTE    byData[0];
};

struct R2S_KIN_MESSAGE_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	int   nCode;
	BYTE  byData[0];
};

struct R2S_GET_BACK_ITEM_FROM_REPERTORY_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD  dwPlayerID;
	BYTE   byBoxIndex;
	BYTE   byBoxPos;
	BYTE   bySuccess;
};

struct R2S_GET_FREEJOIN_TONG_LIST_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD	dwPlayerID;
	INT		byTongNum;
	INT		nUpdateCounter;
	BYTE	byData[0];
};

struct R2S_APPLY_JOIN_TONG_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	BYTE  byResult;
};

// -------------- 排行榜相关　------------------------------->
struct R2S_GET_RANK_IDX_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	INT nRankId;
	DWORD dwSrcId;
	DWORD dwId;
	INT nRankIdx;
};

struct R2S_GET_RANK_ID_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	INT nRankId;
	INT nRankIdx;
	DWORD dwSrcId;
	DWORD dwId;
	DWORD dwRankValue;
	DWORD dwLastModifyTime;
};

struct R2S_SWAP_RANK_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	INT nRankId;
	char  szSrcPlayerName[_NAME_LEN];
	INT   nSrcRank;
	DWORD dwDestId;
	INT   nDestRank;
	BOOL bSucceed;
};

// -------------- 修仙相关　------------------------------->
struct R2S_SYNC_ESCORT_MEMBERS : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	INT nMembersNum;		// 当前修仙人数
	KPlayerEscort aryEscorts[0];
};

struct R2S_SYNC_ESCORT_STATE : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	INT nProtecterId;
	BYTE byEscortLevel;
	BYTE byState;
};

struct R2S_SYNC_PLAYER_ROB_ESCORTS : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	INT nRobNum;
	KRobEscort aryRobEscorts[0];
};

struct R2S_TAKE_ESCORT_AWARD_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	BYTE byEscortState;
	BYTE byEscortLevel;
	BYTE bySuccess;
};

struct R2S_UPDATE_ESCORT_STATE : INTERNAL_PROTOCOL_HEADER
{
	INT nSrcId;
	INT nPlayerId;
	BYTE byEscortState;
	INT nLeftTime;
	DWORD dwRoberId;
	DWORD dwRoberFightscore;
	CHAR szRoberName[_NAME_LEN];
};

struct R2S_RESCUE_PLAYER_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	CHAR szPlayerName[_NAME_LEN];
	INT nRescuePlayerId;
	BOOL bCanResue;
};

struct R2S_TAKE_ROBESCORT_AWARD_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	BYTE byEscortLevel;
	BYTE bySuccess;
};

// -------------- 邮件系统相关　------------------------------->
struct R2S_SYNC_MAIL_COUNT_INFO : INTERNAL_PROTOCOL_HEADER
{
	DWORD               dwPlayerID;
	int                 nReadCount;
	int                 nTotalCount;
};

struct R2S_SEND_MAIL_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	BYTE                byRespondID;
	DWORD               dwSrcID;
	BYTE                byResult;
	char                szReceiver[_NAME_LEN];
	BYTE                byData[0];        // KMail
};

struct R2S_GET_MAILLIST_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD               dwPlayerID;
	int                 nMailCount;
	KMailListInfo       MailList[0];
};

struct R2S_SYNC_MAIL_CONTENT : INTERNAL_PROTOCOL_HEADER
{
	DWORD               dwPlayerID;
	DWORD               dwMailID;
	BYTE                byResult;
	BYTE                byData[0];//R2S_SYNC_MAIL_CONTENT
};

struct R2S_GIVE_MAIL_MONEY_TO_PLAYER : INTERNAL_PROTOCOL_HEADER
{
	DWORD               dwPlayerID;
	DWORD               dwMailID;
	BYTE                byMailType;
	int                 nMoney;
	int             	nSilver;
	int            	    nEnergy;	
	char                szSender[_NAME_LEN];
};

struct R2S_GIVE_MAIL_ITEM_TO_PLAYER : INTERNAL_PROTOCOL_HEADER
{
	DWORD               dwPlayerID;
	DWORD               dwMailID;
	BYTE                byMailType;
	BYTE                byResult;
	BYTE                byItemIndex;
	int                 nItemPrice;
	BYTE                byItemLen;
	char                szSender[_NAME_LEN];
	BYTE                byData[0];
};

struct R2S_NEW_MAIL_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
	DWORD               dwPlayerID;
	KMailListInfo       NewMailListInfo;
};

struct R2S_MAIL_GENERAL_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	DWORD               dwPlayerID;
	DWORD               dwMailID;
	BYTE                byRespond;
};

// ---------------------   拍卖行   -------------------------------

struct R2S_AUCTION_LOOKUP_RESPOND : INTERNAL_PROTOCOL_HEADER
{
	BYTE                byRespondID;
	DWORD               dwPlayerID;
	BYTE                byCode;
	KAUCTION_PAGE_HEAD  Page;
};

struct R2S_AUCTION_BID_RESPOND : INTERNAL_PROTOCOL_HEADER 
{
	DWORD   dwPlayerID;
	BYTE    byCode;
	int     nBidPrice;
};

struct R2S_AUCTION_SELL_RESPOND : INTERNAL_PROTOCOL_HEADER 
{
	DWORD   dwPlayerID;
	BYTE    byCode;
	int     nStartPrice;
	int     nBuyItNowPrice;
	int     nCustodyCharges;
	size_t  uItemDataLen;
	BYTE    byItemData[0];
};

struct R2S_AUCTION_CANCEL_RESPOND : INTERNAL_PROTOCOL_HEADER 
{
	DWORD   dwPlayerID;
	BYTE    byCode;
};

struct R2S_AUCTION_MESSAGE_NOTIFY : INTERNAL_PROTOCOL_HEADER 
{
	DWORD   dwPlayerID;
	BYTE    byCode;
	char    szSaleName[SALE_NAME_LEN];
	int     nPrice;
};

// ------------------------- 好友关系相关 -----------------------------
/**********************************************************
 * Description	: 同步好友列表 
 * Protocol		: r2s_relation_sync_list_respond 和 emKRELATION_G2S_PROTO_START_SYNCLIST 要开始同步了
**********************************************************/
struct R2S_RELATION_SYNC_LIST_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT				nPlayerID;			// 玩家ID
	BYTE			nType;				// 关系类型
	BYTE			nRole;				// 角色，0为主位玩家，1为次位玩家
	BYTE			nCount;				// 关系数量
	BYTE			byNotifyClient;		// 是否通知客户端
	BYTE			byCreate;			// 是否是新创建关系
	KRELATION_INFO	aryInfo[0];
};

/**********************************************************
 * Description	: 通知好友上线/下线
 * Protocol		: r2s_relation_online_notify
**********************************************************/
struct R2S_RELATION_ONLINE_NOTIFY : public INTERNAL_PROTOCOL_HEADER
{
	BYTE	nType;				// 关系类型
	BYTE	nRole;				// 角色，0为主位玩家，1为次位玩家
	BYTE	nCount;				// 关系数量
	BYTE	nOnline;			// 上/下线，1：上线，0：下线
	CHAR	szName[_NAME_LEN];	// 上下线玩家名字
	INT		aryRelationID[0];	// 关系玩家的ID
};

/**********************************************************
 * Description	: 
 * Protocol		: r2s_relation_add_teacher_respond	拜师申请
 * Protocol		: r2s_relation_add_student_respond	收徒申请
**********************************************************/
struct R2S_RELATION_ADD_TEACHER_STUDENT_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
	CHAR	szApplicant[_NAME_LEN];	// 申请者角色名
};

/**********************************************************
 * Description	: 预增加好感度结果通知
 * Protocol		: r2s_relation_pre_add_favor_respond
**********************************************************/
struct R2S_RELATION_PRE_ADD_FAVOR_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	BYTE	nResult;
	INT		nIdentity;				// 自定义标识（用于回调）
};

/**********************************************************
 * Description	: 通知关系建立
 * Protocol		: r2s_relation_notify_respond
**********************************************************/
struct R2S_RELATION_NOTIFY_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	BYTE	nType;					// 关系类型
	BYTE	nRole;					// PlayerID对应的角色(1：主位，0：次位)
	INT		nPlayerID;				// 玩家ID
	CHAR	szRelation[_NAME_LEN];	// 关系玩家名字
};

/**********************************************************
 * Description	: 玩家关系
 * Protocol		: 删除关系	emKRELATION_G2S_PROTO_DELRELATION
 * Protocol		: 删号通知	emKRELATION_G2S_PROTO_DELETEPLAYER
**********************************************************/
struct R2S_RELATION_DEL_RELATION_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;				// 玩家ID
	BYTE	nType;					// 玩家关系类型
	BYTE	nRelationRole;			// 关系玩家角色（1为主位关系，0为次位关系）
	CHAR	szRelation[_NAME_LEN];	// 关系玩家名字
};

/**********************************************************
 * Description	: 同步好友信息（等级、门派、亲密度）
 * Protocol		: R2S_RELATION_FRIEND_INFO_RESPOND
**********************************************************/
struct R2S_RELATION_FRIEND_INFO_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT							nPlayerID;		// 玩家ID
	BYTE						nCount;			// 好友数量
	KRELATION_PROTO_FRIENDINFO	aryInfo[0];		// 好友信息数组
};


/**********************************************************
 * Description	: 师徒错误通知
 * Protocol		: r2s_relation_training_error_respond
**********************************************************/
struct R2S_RELATION_TRAINING_ERROR_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
	BYTE	byError;		// 见KE_RELATION_TRAINING_ERROR
};

/**********************************************************
 * Description	: 可拜为师父的在线玩家搜索结果
 * Protocol		: r2s_relation_search_teacher_respond
**********************************************************/
struct R2S_RELATION_SEARCH_TEACHER_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT					nPlayerID;		// 发起搜索的玩家ID
//	INT					nLastID;		// 本次搜索结果的最大玩家ID
	WORD				wCount;			// 搜索结果数目
	KRELATION_TEACHER	aryResult[0];	// 搜索结果
};

/**********************************************************
 * Description	: 可收为弟子的在线玩家搜索结果
 * Protocol		: r2s_relation_search_student_respond
**********************************************************/
struct R2S_RELATION_SEARCH_STUDENT_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT					nPlayerID;		// 发起搜索的玩家ID
//	INT					nLastID;		// 本次搜索结果的最大玩家ID
	WORD				wCount;			// 搜索结果数目
	KRELATION_STUDENT	aryResult[0];	// 搜索结果
};

/**********************************************************
* Description	: 拜师/收徒选项
* Protocol		: r2s_relation_training_option_respond
**********************************************************/
struct R2S_RELATION_TRAINING_OPTION_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
	INT		nOption;		// 第1BIT置位表示允许拜师，第2个BIT置位表示允许收徒
};

/**********************************************************
* Description	: 查看申请拜师/收徒者的信息
* Protocol		: r2s_relation_training_view_info_respond
**********************************************************/
struct R2S_RELATION_TRAINING_VIEW_INFO_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT		nTargetID;		// 被查看者ID
	INT		nViewerID;		// 查看者ID
};

/**********************************************************
* Description	: 指定玩家的信息
* Protocol		: r2s_relation_training_player_info_respond
**********************************************************/
struct R2S_RELATION_TRAINING_PLAYER_INFO_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT					nViewerID;		// 查看者ID
	KPLAYER_EQUIPVIEW	sPlayerInfo;	// 玩家信息
};

/**********************************************************
* Description	: 自己的剩余报到时间
* Protocol		: r2s_relation_myself_left_checkin_time_respond
**********************************************************/
struct R2S_RELATION_MYSELF_LEFT_CHECKIN_TIME_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
	INT		nLeftSeconds;			// 剩余报到时间（秒），-1表示自己不是未出师弟子，0表示报到时间超时
};

/**********************************************************
* Description	: 自己当前弟子的剩余报到时间
* Protocol		: r2s_relation_student_left_checkin_time_respond
**********************************************************/
struct R2S_RELATION_STUDENT_LEFT_CHECKIN_TIME_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT							nPlayerID;
	WORD						wCount;
	KRELATION_LEFTCHECKINTIME	aLeftCheckInTime[0];
};

/**********************************************************
* Description	: 拜师
* Protocol		: r2s_relation_add_training_respond
**********************************************************/
struct R2S_RELATION_ADD_TRAINING_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerID;
	CHAR	szStudent[_NAME_LEN];		// 拜师的玩家名字
};

struct KTPRCLOSEELATION
{
	CHAR	szTargeter[_NAME_LEN];
	INT		nCoin;
};

struct R2S_RELATION_RESULT_APPLY_IB_COIN_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT		nPlayerId;
	WORD	wCount;
	BYTE	byFirst;
	KTPRCLOSEELATION	aCloseRelation[1];
};

/**********************************************************
* Description	: 根据亲密度等级，奖励绑定金币
* Protocol		: r2s_relation_award_coin_by_friend_favor_respond
**********************************************************/
struct R2S_RELATION_AWARD_COIN_BY_FRIEND_FAVOR_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerAppId;
	INT nPlayerDstId;

	BYTE byLeftCountApp;

	BYTE byGiveCountApp;

	BYTE byAwardApp;

	INT nLevel;
	INT nAwardCoin;
	BYTE byMaxNum;
};

// 竞技场
struct S2R_LOAD_ARENA_RECORD_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
};

struct S2R_ADD_ARENA_RECORD_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
	BOOL bAttack;	// 挑战还是被挑战
	BOOL bResult;	// 结果
	CHAR szTargetName[_NAME_LEN];	// 对方姓名
	INT	nTargetIdx;					// 目标排名
};

struct R2S_LOAD_ARENA_RECORD_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
	size_t uDataLen;
	BYTE byData[0];
};

struct R2S_GM_SEND_AWARD : public INTERNAL_PROTOCOL_HEADER
{
	DWORD	id;
	DWORD 	dwRoleId;
	CHAR  	GMCMD[1024];
};


struct S2R_LOG_INFO_SAVE_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nActionType;
	DWORD dwRoleID;
	INT number;
	char pszComment[100];
};
// 服务器记录
struct S2R_GET_SERVER_RECORD_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nRecordId;
};

struct S2R_ADD_KIN_BATTLE_RECORD_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
};

struct S2R_SET_KIN_BATTLE_WINNERID_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwWinnerId;
};

struct S2R_SET_LAST_WIN_KIN_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwWinnerId;
};

struct S2R_SET_BANK_INVEST_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nCoins;
	DWORD dwRoleId;
};

struct S2R_BUY_ALL_PURCHASE_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nShopId;
	INT nCategoryId;
	INT nGoodsId;		
	INT nCount;			
	DWORD dwRoleId;
	DWORD dwLimitTime;
	DWORD dwStartTime;
	INT nMaxCount;
};

struct R2S_GET_SERVER_RECORD_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT nRecordId;
	size_t uDataLen;
	BYTE byData[0];
};

struct R2S_BUY_ALL_PURCHASE_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nShopId;
	INT nCategoryId;
	INT nGoodsId;		
	INT nCount;	
	INT nBuyCount;
	DWORD dwRoleId;
	INT nMaxCount;
	//DWORD dwLimitTime;
	//DWORD dwStartTime;
};

//----------------------------答题相关-----------------------------------
struct R2S_QUIZ_QUESTION_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT nQuestion[QUIZ_QUESTION_COUNT];
};

struct QUIZ_RANK_INFO
{
	DWORD dwRoldId;
	INT nIntegral;
	CHAR szName[_NAME_LEN];
};

struct R2S_QUIZ_NAME_LIST_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	INT nCount;
	QUIZ_RANK_INFO arrInfo[0];
};

struct R2S_PAY_ORDER_RESPOND : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwRoleId;
	CHAR szOrderno[64];
	DWORD dwGold;
};

//----------------------------跨服组队相关-----------------------------------
struct S2R_CREATE_OVERALL_TEAM_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	INT nInstanceMapId;
	INT nRequireFightscore;
};

struct S2R_JOIN_OVERALL_TEAM_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nMemberId;
	INT nInstanceMapId;
	DWORD dwCombinedIdx;
};

struct S2R_LEAVE_OVERALL_TEAM_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwMemberCombinedIdx;
	DWORD dwLeaderCombinedIdx;
	BYTE byIsKick;
};

struct S2R_MEMBER_PREPARE_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nMemberId;
	DWORD dwLeaderCombinedIdx;
	BYTE byPrepare;
};

struct S2R_GET_PLAYER_TEAMINFO_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	DWORD dwLeaderCombinedIdx;
};

struct S2R_START_MISSION_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwLeaderCombinedIdx;
};

struct S2R_BROADCAST_OVERALLTEAM_MSG : public INTERNAL_PROTOCOL_HEADER
{
	INT nMapId;
	DWORD dwLeaderCombinedIdx;
	INT nRequireFightscore;
	CHAR szCreaterName[MAX_NAME_LEN];
};

struct S2R_MEMBER_COUNTDOWN_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwLeaderCombinedIdx;
};

struct S2R_KICK_OVERALL_PLAYER_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	DWORD dwLeaderCombinedIdx;
	DWORD dwMemberCombinedIdx;
};

struct S2R_CHECK_OVERALL_ONLINE_REQUEST : public INTERNAL_PROTOCOL_HEADER
{
	INT nPlayerId;
	INT nMapId;
	DWORD dwMemberCombinedIdx;
	DWORD dwLeaderCombinedIdx;
};

struct KTEAM_INFO
{
	CHAR szCreaterName[MAX_NAME_LEN];
	INT nRequireFightscore;
	DWORD dwLeaderCombinedIdx;
	BYTE byCurrentMemberNum;
};

struct R2S_SYNC_OVERALL_TEAM : public INTERNAL_PROTOCOL_HEADER
{
	INT nInstanceMapId;
	BYTE byTeamsNum;
	KTEAM_INFO arrTeams[0];
};

struct R2S_OVERALL_TEAM_OPS_RESULT : public INTERNAL_PROTOCOL_HEADER
{
	INT nMemberId;
	BYTE byTeamOps;
	BYTE byResult;
};

struct KRS_MEMBER_INFO
{
	CHAR szMemberName[MAX_NAME_LEN];
	BYTE byMemberLevel;
	BYTE byMemberSex;
	INT nMemberFightScore;
	DWORD dwMemberCombinedIdx;
	BYTE byPrepare;
};

struct R2S_SYNC_PLAYER_OVERALL_TEAM : IDENTITY_HEADER
{
	INT nPlayerId;
	INT nMapId;
	DWORD dwLeaderCombinedIdx;
	BYTE byMemberNum;
	INT nRequireFightscore;
	KRS_MEMBER_INFO arrMembers[0];
};

struct R2S_MEMBER_COUNTDOWN_NOTIFY : IDENTITY_HEADER
{
	INT nPlayerId;
};

struct R2S_CHECK_MEMBER_ONLINE_RESPOND : IDENTITY_HEADER
{
	INT nPlayerId;
	INT nMapId;
	DWORD dwMemberCombinedIdx;
	DWORD dwLeaderCombinedIdx;
	BYTE byOnline;
};

// 跨服请求
struct R2S_MULTI_TRANS_REQUEST : IDENTITY_HEADER
{
	INT nMapId;
	INT nMapCopyIdx;
	DWORD arrMembers[3];
};

#pragma pack()

