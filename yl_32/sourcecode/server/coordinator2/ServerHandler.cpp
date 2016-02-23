#include "stdafx.h"
#include "Core/QGUID.h"
#include "ServerHandler.h"
#include "GameCenter.h"
#include "RoleDB.h"

#include "team/teammodule_gc.h"
#include "onlinegamemodule/kteamprotocol.h"
#include "onlinegamebase/kscriptmanager.h"
#include "kin/kinhandler.h"
#include "rank/krankhandler.h"
#include "arena/ArenaServerHandler.h"
#include "record/krecordserverhandler.h"
#include "relation/relationmanager.h"
#include "quiz/QuizServerHandler.h"
#include "gcmodule/chat/kchanneldef.h"
#include "onlinegamebase/ksysservice.h"
#include "battlezone/battlezonemaster.h"
#include "battlezone/battlezoneslave.h"
#include "battlezone/battlezone.h"
#include "overallqueue/kqueuehandler.h"
#include "escort/kescorthandler.h"
#include "GMSendAwardDB.h"

#define MAP_OPERATION_OVERTIME	120	// 地图创建或者删除的超时时间（单位：秒）

#define REGISTER_MODULE_FUNC(ProtocolID, FuncName, ProtocolSize)	\
	{m_ModuleProtocolFuns[ProtocolID] = FuncName;					\
	m_uProtocolSize[ProtocolID] = ProtocolSize;}

KGameServer::KGameServer(void)
{
    m_nLastConnIndex                    = 1;
    m_bGameWorldProtocolValidFlag       = false;
    m_nGameWorldLowerProtocolVersion    = 0;
    m_nGameWorldUpperProtocolVersion    = 0;
    m_nConnectionTimeout                = 0;
    m_bQuiting                          = false;

    memset(m_GSList, 0, sizeof(m_GSList));
    memset(m_ProcessProtocolFuns, 0, sizeof(m_ProcessProtocolFuns));
	memset(m_uProtocolSize, 0, sizeof(m_uProtocolSize));

	memset(m_ModuleProtocolFuns, 0, sizeof(m_ModuleProtocolFuns));

	REGISTER_INTERNAL_FUNC(s2r_handshake_request,  &KGameServer::OnHandshakeRequest, sizeof(S2R_HANDSHAKE_REQUEST));
    REGISTER_INTERNAL_FUNC(s2r_ping_signal, &KGameServer::OnPingSignal, sizeof(S2R_PING_SIGNAL));
    REGISTER_INTERNAL_FUNC(s2r_update_performance, &KGameServer::OnUpdatePerformance, sizeof(S2R_UPDATE_PERFORMANCE));

    REGISTER_INTERNAL_FUNC(s2r_create_map_respond, &KGameServer::OnCreateMapRespond, sizeof(S2R_CREATE_MAP_RESPOND));

	REGISTER_INTERNAL_FUNC(s2r_player_login_respond, &KGameServer::OnPlayerLoginRespond, sizeof(S2R_PLAYER_LOGIN_RESPOND));
	REGISTER_INTERNAL_FUNC(s2r_confirm_player_login_request, &KGameServer::OnConfirmPlayerLoginRequest, sizeof(S2R_CONFIRM_PLAYER_LOGIN_REQUEST));

	REGISTER_INTERNAL_FUNC(s2r_player_leave_gs, &KGameServer::OnPlayerLeaveGS, sizeof(S2R_PLAYER_LEAVE_GS));

	REGISTER_INTERNAL_FUNC(s2r_search_map_request, &KGameServer::OnSearchMapRequest, sizeof(S2R_SEARCH_MAP_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_load_map_request, &KGameServer::OnLoadMapRequest, sizeof(S2R_LOAD_MAP_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_delete_battle_map_request, &KGameServer::OnDeleteBattleMapRequest, sizeof(S2R_QUIZ_NAME_LIST_REQUEST));

	REGISTER_INTERNAL_FUNC(s2r_transfer_player_request, &KGameServer::OnTransferPlayerRequest, sizeof(S2R_TRANSFER_PLAYER_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_transfer_player_respond, &KGameServer::OnTransferPlayerRespond, sizeof(S2R_TRANSFER_PLAYER_RESPOND));

	REGISTER_INTERNAL_FUNC(s2r_pay_order_request, &KGameServer::OnPayOrderRequest, sizeof(S2R_PAY_ORDER_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_pay_order_finish_request, &KGameServer::OnPayOrderFinishRequest, sizeof(S2R_PAY_ORDER_FINISH_REQUEST));

	// ---------------- Team Module --------------------------------------------------------------------------------
	REGISTER_MODULE_FUNC(s2r_invite_player_join_team_request, &QCenterTeamModule::OnInvitePlayerJoinTeamRequest, sizeof(S2R_INVITE_PLAYER_JOIN_TEAM_REQUEST));
	REGISTER_MODULE_FUNC(s2r_invite_player_join_team_respond, &QCenterTeamModule::OnInvitePlayerJoinTeamRespond, sizeof(S2R_INVITE_PLAYER_JOIN_TEAM_RESPOND));
	REGISTER_MODULE_FUNC(s2r_apply_join_team_request, &QCenterTeamModule::OnApplyJoinTeamRequest, sizeof(S2R_APPLY_JOIN_TEAM_REQUEST));
	REGISTER_MODULE_FUNC(s2r_apply_join_team_respond, &QCenterTeamModule::OnApplyJoinTeamRespond, sizeof(S2R_APPLY_JOIN_TEAM_RESPOND));
	REGISTER_MODULE_FUNC(s2r_team_del_member_request, &QCenterTeamModule::OnTeamDelMemberRequest, sizeof(S2R_TEAM_DEL_MEMBER_REQUEST));
	REGISTER_MODULE_FUNC(s2r_team_change_authority_request, &QCenterTeamModule::OnTeamChangeAuthorityRequest, sizeof(S2R_TEAM_CHANGE_AUTHORITY_REQUEST));
	REGISTER_MODULE_FUNC(s2r_team_set_loot_mode_request, &QCenterTeamModule::OnTeamSetLootModeRequest, sizeof(S2R_TEAM_SET_LOOT_MODE_REQUEST));
	REGISTER_MODULE_FUNC(s2r_team_set_roll_quality_request, &QCenterTeamModule::OnTeamSetRollQualityRequest, sizeof(S2R_TEAM_SET_ROLL_QUALITY_REQUEST));
	REGISTER_MODULE_FUNC(s2r_team_set_formation_leader_request, &QCenterTeamModule::OnTeamSetFormationLeader, sizeof(S2R_TEAM_SET_FORMATION_LEADER_REQUEST));
	REGISTER_MODULE_FUNC(s2r_team_disband_request, &QCenterTeamModule::OnTeamDisbandRequest, sizeof(S2R_TEAM_DISBAND_REQUEST));

	REGISTER_MODULE_FUNC(s2r_sync_team_member_max_lmr, &QCenterTeamModule::OnTeamSyncMemberMaxLMR, sizeof(S2R_SYNC_TEAM_MEMBER_MAX_LMR));
	REGISTER_MODULE_FUNC(s2r_sync_team_member_current_lmr, &QCenterTeamModule::OnTeamSyncMemberCurrentLMR, sizeof(S2R_SYNC_TEAM_MEMBER_CURRENT_LMR));
	REGISTER_MODULE_FUNC(s2r_sync_team_member_misc, &QCenterTeamModule::OnTeamSyncMemberMisc, sizeof(S2R_SYNC_TEAM_MEMBER_MISC));
	REGISTER_MODULE_FUNC(s2r_sync_team_member_position, &QCenterTeamModule::OnTeamSyncMemberPosition, sizeof(S2R_SYNC_TEAM_MEMBER_POSITION));

	REGISTER_MODULE_FUNC(s2r_team_set_mark_request, &QCenterTeamModule::OnTeamSetMarkRequest, sizeof(S2R_TEAM_SET_MARK_REQUEST));
	REGISTER_MODULE_FUNC(s2r_team_level_up_raid_request, &QCenterTeamModule::OnTeamLevelRaidRequest, sizeof(S2R_TEAM_LEVEL_UP_RAID_REQUEST));
	REGISTER_MODULE_FUNC(s2r_team_change_member_group_request, &QCenterTeamModule::OnTeamChangeMemberGroupRequest, sizeof(S2R_TEAM_CHANGE_MEMBER_GROUP_REQUEST));
	REGISTER_MODULE_FUNC(s2r_create_team_request, &QCenterTeamModule::OnCreateTeamRequest, sizeof(S2R_CREATE_TEAM_REQUEST));
	// -------------------------------------------------------------------------------------------------------------

    REGISTER_INTERNAL_FUNC(s2r_save_scene_player_list, &KGameServer::OnSaveScenePlayerList, sizeof(S2R_SAVE_SCENE_PLAYER_LIST));
    REGISTER_INTERNAL_FUNC(s2r_save_scene_data, &KGameServer::OnSaveSceneData, sizeof(S2R_SAVE_SCENE_DATA));
    REGISTER_INTERNAL_FUNC(s2r_player_enter_scene_notify, &KGameServer::OnPlayerEnterSceneNotify, sizeof(S2R_PLAYER_ENTER_SCENE_NOTIFY));
	REGISTER_INTERNAL_FUNC(s2r_reset_map_copy_request, &KGameServer::OnResetMapCopyNotify, sizeof(S2R_RESET_MAP_COPY_REQUEST));

    REGISTER_INTERNAL_FUNC(s2r_load_role_data_request, &KGameServer::OnLoadRoleDataRequest, sizeof(S2R_LOAD_ROLE_DATA_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_load_delay_role_data_request, &KGameServer::OnLoadDelayRoleDataRequest, sizeof(S2R_LOAD_DELAY_ROLE_DATA_REQUEST));
	
    REGISTER_INTERNAL_FUNC(s2r_sync_role_data, &KGameServer::OnSyncRoleData, sizeof(S2R_SYNC_ROLE_DATA));
    REGISTER_INTERNAL_FUNC(s2r_save_role_data, &KGameServer::OnSaveRoleData, sizeof(S2R_SAVE_ROLE_DATA));

	REGISTER_INTERNAL_FUNC(s2r_create_delay_role_data, &KGameServer::OnCreateDelayRoleData, sizeof(S2R_CREATE_DELAY_ROLE_DATA));
	REGISTER_INTERNAL_FUNC(s2r_save_delay_role_data, &KGameServer::OnSaveDelayRoleData, sizeof(S2R_SAVE_DELAY_ROLE_DATA));

	REGISTER_INTERNAL_FUNC(s2r_remote_lua_call, &KGameServer::OnRemoteLuaCall, sizeof(S2R_REMOTE_LUA_CALL));

	REGISTER_INTERNAL_FUNC(s2r_chat_message, &KGameServer::OnChatMessage, sizeof(S2R_CHAT_MESSAGE));
	REGISTER_INTERNAL_FUNC(s2r_player_chat_error, &KGameServer::OnPlayerChatError, sizeof(S2R_PLAYER_CHAT_ERROR));

	// ---------------- Kin Module --------------------------------------------------------------------------------
	REGISTER_MODULE_FUNC(s2r_apply_tong_roster_request, &KinHandler::OnApplyTongRosterRequest, sizeof(S2R_APPLY_TONG_ROSTER_REQUEST));
	REGISTER_MODULE_FUNC(s2r_apply_tong_info_request, &KinHandler::OnApplyTongInfoRequest, sizeof(S2R_APPLY_TONG_INFO_REQUEST));
	REGISTER_MODULE_FUNC(s2r_apply_tong_repertory_page_request, &KinHandler::OnApplyRepertoryPageRequest, sizeof(S2R_APPLY_TONG_REPERTORY_PAGE_REQUEST));
	REGISTER_MODULE_FUNC(s2r_apply_create_tong_request, &KinHandler::OnApplyCreateTongRequest, sizeof(S2R_APPLY_CREATE_TONG_REQUEST));
	REGISTER_MODULE_FUNC(s2r_invite_player_join_tong_request, &KinHandler::OnInvitePlayerJoinTongRequest, sizeof(S2R_INVITE_PLAYER_JOIN_TONG_REQUEST));
	REGISTER_MODULE_FUNC(s2r_invite_player_join_tong_respond, &KinHandler::OnInvitePlayerJoinTongRespond, sizeof(S2R_INVITE_PLAYER_JOIN_TONG_RESPOND));
	REGISTER_MODULE_FUNC(s2r_apply_kick_out_tong_member_request, &KinHandler::OnApplyKickOutTongMemberRequest, sizeof(S2R_APPLY_KICK_OUT_TONG_MEMBER));
	REGISTER_MODULE_FUNC(s2r_modify_tong_info_request, &KinHandler::OnModifyTongInfoRequest, sizeof(S2R_MODIFY_TONG_INFO_REQUEST));
	REGISTER_MODULE_FUNC(s2r_modify_tong_schema_request, &KinHandler::OnModifyTongSchemaRequest, sizeof(S2R_MODIFY_TONG_SCHEMA_REQUEST));
	REGISTER_MODULE_FUNC(s2r_apply_quit_tong_request, &KinHandler::OnApplyQuitTongRequest, sizeof(S2R_APPLY_QUIT_TONG_REQUEST));
	REGISTER_MODULE_FUNC(s2r_change_tong_member_group_request, &KinHandler::OnChangeTongMemberGroupRequest, sizeof(S2R_CHANGE_TONG_MEMBER_GROUP_REQUEST));
	REGISTER_MODULE_FUNC(s2r_change_tong_master_request, &KinHandler::OnChangeTongMasterRequest, sizeof(S2R_CHANGE_TONG_MASTER_REQUEST));
	REGISTER_MODULE_FUNC(s2r_change_tong_member_remark_request, &KinHandler::OnChangeTongMemberRemarkRequest, sizeof(S2R_CHANGE_TONG_MEMBER_REMARK_REQUEST));
	REGISTER_MODULE_FUNC(s2r_get_tong_description_request, &KinHandler::OnGetTongDescriptionRequest, sizeof(S2R_GET_TONG_DESCRIPTION_REQUEST));
	REGISTER_MODULE_FUNC(s2r_save_money_in_tong_request, &KinHandler::OnSaveMoneyInTongRequest, sizeof(S2R_SAVE_MONEY_IN_TONG_REQUEST));
	REGISTER_MODULE_FUNC(s2r_pay_tong_salary_request, &KinHandler::OnPayTongSalaryRequest, sizeof(S2R_PAY_TONG_SALARY_REQUEST));
	REGISTER_MODULE_FUNC(s2r_get_tong_salary_request, &KinHandler::OnGetTongSalaryRequest, sizeof(S2R_GET_TONG_SALARY_REQUEST));
	REGISTER_MODULE_FUNC(s2r_get_tong_salary_fail_respond, &KinHandler::OnGetTongSalaryFailRespond, sizeof(S2R_GET_TONG_SALARY_FAIL_RESPOND));
	REGISTER_MODULE_FUNC(s2r_apply_open_tong_repertory_request, &KinHandler::OnApplyOpenTongRepertoryRequest, sizeof(S2R_APPLY_OPEN_TONG_REPERTORY_REQUEST));
	REGISTER_MODULE_FUNC(s2r_get_tong_rank_info_request, &KinHandler::OnGetTongRankInfoRequest, sizeof(S2R_GET_TONG_RANK_INFO_REQUEST));
	//REGISTER_MODULE_FUNC(s2r_change_tong_camp_request, &KinHandler::OnChangeTongCampRequest, sizeof(S2R_CHANGE_TONG_CAMP_REQUEST));

	//REGISTER_MODULE_FUNC(s2r_take_tong_repertory_item_to_pos_request, &KinHandler::OnTakeTongRepertoryItemToPosRequest, sizeof(S2R_TAKE_TONG_REPERTORY_ITEM_TO_POS_REQUEST));
	REGISTER_MODULE_FUNC(s2r_take_tong_repertory_item_request, &KinHandler::OnTakeTongRepertoryItemRequest, sizeof(S2R_TAKE_TONG_REPERTORY_ITEM_REQUEST)); 
	REGISTER_MODULE_FUNC(s2r_take_tong_repertory_item_respond, &KinHandler::OnTakeTongRepertoryItemRespond, sizeof(S2R_TAKE_TONG_REPERTORY_ITEM_RESPOND));

	//REGISTER_MODULE_FUNC(s2r_put_tong_repertory_item_to_pos_request, &KinHandler::OnPutTongRepertoryItemToPosRequest, sizeof(S2R_PUT_TONG_REPERTORY_ITEM_TO_POS_REQUEST));
	REGISTER_MODULE_FUNC(s2r_put_tong_repertory_item_request, &KinHandler::OnPutTongRepertoryItemRequest, sizeof(S2R_PUT_TONG_REPERTORY_ITEM_REQUEST));
	REGISTER_MODULE_FUNC(s2r_put_tong_repertory_item_respond, &KinHandler::OnPutTongRepertoryItemRespond, sizeof(S2R_PUT_TONG_REPERTORY_ITEM_RESPOND));

	//REGISTER_MODULE_FUNC(s2r_unlock_tong_repertory_grid, &KinHandler::OnUnlockTongRepertoryGrid, sizeof(S2R_UNLOCK_TONG_REPERTORY_GRID));
	REGISTER_MODULE_FUNC(s2r_exchange_tong_repertory_item_pos_request, &KinHandler::OnExchangeTongRepertoryItemPosRequest, sizeof(S2R_EXCHANGE_TONG_REPERTORY_ITEM_POS_REQUEST));
	//REGISTER_MODULE_FUNC(s2r_apply_stack_tong_repertory_item_request, &KinHandler::OnApplyStackTongRepertoryItemRequest, sizeof(S2R_APPLY_STACK_TONG_REPERTORY_ITEM_REQUEST));
	REGISTER_MODULE_FUNC(s2r_stack_tong_repertory_item_request, &KinHandler::OnStackTongRepertoryItemRequest, sizeof(S2R_STACK_TONG_REPERTORY_ITEM_REQUEST));
	REGISTER_MODULE_FUNC(s2r_remove_tong_repertory_item_request, &KinHandler::OnRemoveTongRepertoryItemRequest, sizeof(S2R_REMOVE_TONG_REPERTORY_ITEM_REQUEST));
	REGISTER_MODULE_FUNC(s2r_get_back_item_from_repertory_request, &KinHandler::OnGetBackItemFromRepertoryRequest, sizeof(S2R_GET_BACK_ITEM_FROM_REPERTORY_REQUEST));
	REGISTER_MODULE_FUNC(s2r_get_freejoin_tong_list_request, &KinHandler::OnGetFreeJoinTongListRequest, sizeof(S2R_GET_FREEJOIN_TONG_LIST_REQUEST));
	REGISTER_MODULE_FUNC(s2r_apply_join_tong_request, &KinHandler::OnApplyJoinTongRequest, sizeof(S2R_APPLY_JOIN_TONG_REQUEST));
	REGISTER_MODULE_FUNC(s2r_set_tong_freejoin_request, &KinHandler::OnSetTongFreeJoinRequest, sizeof(S2R_SET_TONG_FREEJOIN_REQUEST));
	REGISTER_MODULE_FUNC(s2r_set_tong_activity_opentime_request, &KinHandler::OnSetTongActivityOpenTimeRequest, sizeof(S2R_SET_TONG_ACTIVITY_OPENTIME_REQUEST));

	// ---------------- Rank Module --------------------------------------------------------------------------------
	REGISTER_MODULE_FUNC(s2r_get_rank_idx_request, &KRankHandler::OnGetRankIdxRequest, sizeof(S2R_GET_RANK_IDX_REQUEST));
	REGISTER_MODULE_FUNC(s2r_get_rank_id_request, &KRankHandler::OnGetRankIdRequest, sizeof(S2R_GET_RANK_ID_REQUEST));
	REGISTER_MODULE_FUNC(s2r_swap_rank_request, &KRankHandler::OnSwapRankRequest, sizeof(S2R_SWAP_RANK_REQUEST));
	REGISTER_MODULE_FUNC(s2r_modify_rank_value_request, &KRankHandler::OnModifyRankValueRequest, sizeof(S2R_MODIFY_RANK_VALUE_REQUEST));

	// ---------------- Escort Module --------------------------------------------------------------------------------
	REGISTER_MODULE_FUNC(s2r_start_escort_request, &KEscortHandler::OnStartEscortRequest, sizeof(S2R_START_ESCORT_REQUEST));
	REGISTER_MODULE_FUNC(s2r_refresh_escorts_request, &KEscortHandler::OnRefreshEscortsRequest, sizeof(S2R_REFRESH_ESCORTS_REQUEST));
	REGISTER_MODULE_FUNC(s2r_escort_remove_watch_request, &KEscortHandler::OnRemovePlayerFromWatchTable, sizeof(S2R_REMOVE_ESCORT_WATCH_REQUEST));
	REGISTER_MODULE_FUNC(s2r_take_escort_award_request, &KEscortHandler::OnTakeEscortAwardRequest, sizeof(S2R_TAKE_ESCORT_AWARD_REQUEST));
	REGISTER_MODULE_FUNC(s2r_rob_player_escort_request, &KEscortHandler::OnRobPlayerEscortRequest, sizeof(S2R_ROB_PLAYER_ESCORT_REQUEST));
	REGISTER_MODULE_FUNC(s2r_del_rob_escort_request, &KEscortHandler::OnDelRobEscortRequest, sizeof(S2R_DEL_ROB_ESCORT_REQUEST));
	REGISTER_MODULE_FUNC(s2r_take_rob_award_request, &KEscortHandler::OnTakeRobAwardRequest, sizeof(S2R_TAKE_ROB_AWARD_REQUEST));
	REGISTER_MODULE_FUNC(s2r_speedup_escort_request, &KEscortHandler::OnSpeedUpRequest, sizeof(S2R_SPEED_UP_ESCORT));
	REGISTER_MODULE_FUNC(s2r_rescue_player_request, &KEscortHandler::OnRescuePlayerRequest, sizeof(S2R_RESCUE_PLAYER_REQUEST));

	// ---------------- Arena Module --------------------------------------------------------------------------------
	REGISTER_MODULE_FUNC(s2r_load_arena_record_request, &KArenaHandler::OnLoadArenaRecordRequest, sizeof(S2R_LOAD_ARENA_RECORD_REQUEST));
	REGISTER_MODULE_FUNC(s2r_add_arena_record_request, &KArenaHandler::OnAddArenaRecordRequest, sizeof(S2R_ADD_ARENA_RECORD_REQUEST));

	// ---------------- Record Module --------------------------------------------------------------------------------
	REGISTER_MODULE_FUNC(s2r_get_server_record_request, &KRecordHandler::OnGetServerRecordRequest, sizeof(S2R_GET_SERVER_RECORD_REQUEST));
	REGISTER_MODULE_FUNC(s2r_add_kinbattle_record_request, &KRecordHandler::OnAddKinBattleRecordRequest, sizeof(S2R_ADD_KIN_BATTLE_RECORD_REQUEST));
	REGISTER_MODULE_FUNC(s2r_set_kinbattle_winnerId_request, &KRecordHandler::OnSetKinBattleWinnerIdRequest, sizeof(S2R_SET_KIN_BATTLE_WINNERID_REQUEST));
	REGISTER_MODULE_FUNC(s2r_set_last_winkin_request, &KRecordHandler::OnSetLastWinKinRequest, sizeof(S2R_SET_LAST_WIN_KIN_REQUEST));
	REGISTER_MODULE_FUNC(s2r_set_bank_invest_request, &KRecordHandler::OnSetBankInvestRequest, sizeof(S2R_SET_BANK_INVEST_REQUEST));
	REGISTER_MODULE_FUNC(s2r_buy_all_purchase_request, &KRecordHandler::OnSetBuyAllPurcaseRequest, sizeof(S2R_BUY_ALL_PURCHASE_REQUEST));

	// ---------------- OverallTeam Module --------------------------------------------------------------------------------
	REGISTER_MODULE_FUNC(s2r_create_overall_team_request, &KOverallQueueHandler::OnCreateOverallTeamRequest, sizeof(S2R_CREATE_OVERALL_TEAM_REQUEST));
	REGISTER_MODULE_FUNC(s2r_join_overall_team_request, &KOverallQueueHandler::OnJoinOverallTeamRequest, sizeof(S2R_JOIN_OVERALL_TEAM_REQUEST));
	REGISTER_MODULE_FUNC(s2r_leave_overall_team_request, &KOverallQueueHandler::OnLeaveOverallTeamRequest, sizeof(S2R_LEAVE_OVERALL_TEAM_REQUEST));
	REGISTER_MODULE_FUNC(s2r_member_prepare_request, &KOverallQueueHandler::OnMemberPrepareRequest, sizeof(S2R_MEMBER_PREPARE_REQUEST));
	REGISTER_MODULE_FUNC(s2r_get_player_teaminfo_request, &KOverallQueueHandler::OnGetPlayerTeamInfoRequest, sizeof(S2R_GET_PLAYER_TEAMINFO_REQUEST));
	REGISTER_MODULE_FUNC(s2r_kick_overall_player_request, &KOverallQueueHandler::OnKickOverllPlayerRequest, sizeof(S2R_KICK_OVERALL_PLAYER_REQUEST));
	REGISTER_MODULE_FUNC(s2r_start_mission_request, &KOverallQueueHandler::OnStartMissionRequest, sizeof(S2R_START_MISSION_REQUEST));
	REGISTER_MODULE_FUNC(s2r_member_countdown_request, &KOverallQueueHandler::OnMemberCountDownRequest, sizeof(S2R_MEMBER_COUNTDOWN_REQUEST));
	REGISTER_MODULE_FUNC(s2r_broadcast_overallteam_msg, &KOverallQueueHandler::OnBroadcastOverallteamMsg, sizeof(S2R_BROADCAST_OVERALLTEAM_MSG));
	REGISTER_MODULE_FUNC(s2r_check_online_leave_request, &KOverallQueueHandler::OnCheckOnlineLeaveRequest, sizeof(S2R_CHECK_OVERALL_ONLINE_REQUEST));

	// ---------------- Mail --------------------------------------------------------------------------------
	REGISTER_INTERNAL_FUNC(s2r_send_mail_request, &KGameServer::OnSendMailRequest, sizeof(S2R_SEND_MAIL_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_send_global_mail_request, &KGameServer::OnSendGlobalMailRequest, sizeof(S2R_SEND_GLOBAL_MAIL_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_get_maillist_request, &KGameServer::OnGetMailListRequest, sizeof(S2R_GET_MAILLIST_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_query_mail_content, &KGameServer::OnQueryMailContent, sizeof(S2R_QUERY_MAIL_CONTENT));
	REGISTER_INTERNAL_FUNC(s2r_acquire_mail_money_request, &KGameServer::OnAcquireMailMoneyRequest, sizeof(S2R_ACQUIRE_MAIL_MONEY_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_acquire_mail_item_request, &KGameServer::OnAcquireMailItemRequest, sizeof(S2R_ACQUIRE_MAIL_ITEM_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_give_mail_money_to_player, &KGameServer::OnGiveMailMoneyToPlayerRespond, sizeof(S2R_GIVE_MAIL_MONEY_TO_PLAYER_RESPOND));
	REGISTER_INTERNAL_FUNC(s2r_give_mail_item_to_player, &KGameServer::OnGiveMailItemToPlayerRespond, sizeof(S2R_GIVE_MAIL_ITEM_TO_PLAYER_RESPOND));
	REGISTER_INTERNAL_FUNC(s2r_set_mail_read, &KGameServer::OnSetMailRead, sizeof(S2R_SET_MAIL_READ));
	REGISTER_INTERNAL_FUNC(s2r_delete_mail, &KGameServer::OnDeleteMail, sizeof(S2R_DELETE_MAIL));
	REGISTER_INTERNAL_FUNC(s2r_return_mail, &KGameServer::OnReturnMail, sizeof(S2R_RETURN_MAIL));

	// ------------relation ---------------------------------------------------------------------------------------------
	REGISTER_MODULE_FUNC(s2r_relation_add_relation_favor_request, &KRelationManager_GC::Process_AddFavor, sizeof(S2R_RELATION_ADD_RELATION_FAVOR_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_search_teacher_request, &KRelationManager_GC::Process_SearchTeacher, sizeof(S2R_RELATION_SEARCH_PLAYER_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_search_student_request, &KRelationManager_GC::Process_SearchStudent, sizeof(S2R_RELATION_SEARCH_PLAYER_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_set_training_option_request, &KRelationManager_GC::Process_SetTrainingOption, sizeof(S2R_RELATION_SET_TRAINING_OPTION_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_get_training_option_request, &KRelationManager_GC::Process_GetTrainingOption, sizeof(S2R_RELATION_PLAYER_ID_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_add_teacher_request, &KRelationManager_GC::Process_RequestTeacher, sizeof(S2R_RELATION_ADD_TRAINING_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_add_student_request, &KRelationManager_GC::Process_RequestStudent, sizeof(S2R_RELATION_ADD_TRAINING_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_training_graduate_request, &KRelationManager_GC::Process_TrainingGraduate, sizeof(S2R_RELATION_TRAINING_GRADUATE_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_training_view_info_request, &KRelationManager_GC::Process_TrainingViewInfo, sizeof(S2R_RELATION_TRAINING_VIEW_INFO_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_training_player_info_request, &KRelationManager_GC::Process_TrainingPlayerInfo, sizeof(S2R_RELATION_TRAINING_PLAYER_INFO_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_training_make_friend_request, &KRelationManager_GC::Process_TrainingMakeFriend, sizeof(S2R_RELATION_TRAINING_MAKE_FRIEND_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_training_apply_request, &KRelationManager_GC::Process_TrainingApply, sizeof(S2R_RELATION_TRAINING_APPLY_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_add_bind_coin_request, &KRelationManager_GC::Process_AddCloseFriendBindCoin, sizeof(S2R_RELATION_ADD_BIND_COIN_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_apply_gain_coin_request, &KRelationManager_GC::Process_ApplyGainIbBackCoin, sizeof(S2R_RELATION_APPLY_GAIN_COIN_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_gain_ib_coin_request, &KRelationManager_GC::Process_GainIbBackCoin, sizeof(S2R_RELATION_GAIN_IB_COIN_REQUEST));
	REGISTER_MODULE_FUNC(s2r_relation_cancel_gain_coin_request, &KRelationManager_GC::Process_CancelGainCoin, sizeof(S2R_RELATION_APPLY_GAIN_COIN_REQUEST));

	// ------------battlezone ---------------------------------------------------------------------------------------------
	REGISTER_INTERNAL_FUNC(s2r_bzone_transfer_request, &KGameServer::OnBZoneTransferRequest, sizeof(S2R_BZONE_TRANSFER_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_bzone_return_request, &KGameServer::OnBZoneReturnRequest, sizeof(S2R_BZONE_RETURN_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_get_multi_award_request, &KGameServer::OnGetMultiAwardRequest, sizeof(S2R_GET_MULTI_AWARD_REQUEST));
	REGISTER_INTERNAL_FUNC(s2r_get_crosssv_exdata_request, &KGameServer::OnGetCrossSvExDataRequest, sizeof(S2R_GET_CROSSSV_EXDATA_REQUEST));


	// ------------PVE 竞技场 ---------------------------------------------------------------------------------------------


	//---------------------------------------答题-------------------------------------------------------------------------------------
	REGISTER_MODULE_FUNC(s2r_quiz_name_list_request, &QuizServerHandler::OnNameListRequest, sizeof(S2R_QUIZ_NAME_LIST_REQUEST));
	REGISTER_MODULE_FUNC(s2r_quiz_questions_request, &QuizServerHandler::OnQuestionListRequest, sizeof(S2R_QUIZ_QUESTION_REQUEST));
	REGISTER_MODULE_FUNC(s2r_quiz_sumit_score_request, &QuizServerHandler::OnSubmitIntegralRequest, sizeof(S2R_QUIZ_SUMIT_INTEGRAL_REQUEST));
	//AutoCode:注册协议
	//GM指令成功返回
	REGISTER_INTERNAL_FUNC(s2r_GM_send_award_success_request, &KGameServer::ChangeGMCMDMarkRequest, sizeof(R2S_GM_SEND_AWARD));
	//玩家刚登录查数据库
	REGISTER_INTERNAL_FUNC(s2r_Login_Find_Sql, &KGameServer::onLoginFindSqlRequest, sizeof(S2R_LOAD_ARENA_RECORD_REQUEST)); 
	//打印log
	REGISTER_INTERNAL_FUNC(s2r_log_info_save_request, &KGameServer::SyncInfoToSqlRequest, sizeof(S2R_LOG_INFO_SAVE_REQUEST)); 

}

BOOL KGameServer::Init()
{
    BOOL            bResult  = false;
	int             nRetCode = false;
    struct timeval  TimeVal  = {0, 0};

    nRetCode = m_SocketAcceptor.Open("", g_pGameCenter->m_Settings.m_nGameServerPort);
    LOG_PROCESS_ERROR(nRetCode);

    m_SocketAcceptor.SetTimeout(&TimeVal);

	m_nConnectionTimeout        = g_pGameCenter->m_Settings.m_nConnectionTimeout;
    m_dwNextLoginQueueID        = 1;

    bResult = true;
EXIT0:
	return bResult;
}

void KGameServer::UnInit()
{
    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        if (m_GSList[i].piSocket != NULL)
        {
            CloseConnection(m_GSList[i].nConnIndex);
        }
    }
    m_SocketAcceptor.Close();
}

void KGameServer::Activate()
{
    ProcessAcceptor();

    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        KGS_INFO* pGSInfo = &m_GSList[i];

        if (pGSInfo->piSocket == NULL)
            continue;

        if (m_bQuiting && !pGSInfo->bQuiting)
        {
            DoQuitNotify(pGSInfo->nConnIndex);
            pGSInfo->bQuiting = true;
        }

        ProcessConnection(pGSInfo->nConnIndex, pGSInfo->piSocket);
    }
}

BOOL KGameServer::IsAnyGSConnected()
{
    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        if (m_GSList[i].piSocket)
        {
            return true;
        }
    }

    return false;
}

KGS_INFO* KGameServer::GetGSInfoByConnectionIndex(int nConnIndex)
{
    if (nConnIndex == 0)
        return NULL;

    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        if (m_GSList[i].nConnIndex == nConnIndex)
        {
            return &m_GSList[i];
        }
    }

    return NULL;
}

int KGameServer::GetRandomGS()
{
    int nResult = 0;
    int nCount  = 0;
    int nGS[MAX_GS_COUNT];

    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        if (m_GSList[i].piSocket == NULL)
            continue;

        nGS[nCount++] = m_GSList[i].nConnIndex;
    }

    if (nCount > 0)
    {
        int nIndex = KSysService::Rand(nCount);

        assert(nIndex >= 0 && nIndex < nCount);

        nResult = nGS[nIndex];
    }

    return nResult;
}

int KGameServer::GetBestGSConnectionForCreateMap(KMapInfo* pMapInfo)
{
    int     nConnIndex      = 0;
    float   fPerformance    = 0.0f;

    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        if (m_GSList[i].piSocket == NULL)
            continue;

        // 内存使用超标的一票否决
        if ((int)(m_GSList[i].uMemory >> 20) > g_pGameCenter->m_Settings.m_nMemoryLimitForCreateMap)
            continue;

        // 考虑平均性能是为了避免瞬时性能过于乐观
        if (m_GSList[i].fAveragePerformance < g_pGameCenter->m_Settings.m_fAveragePerformanceLimitForCreateMap)
            continue;

        // 考虑瞬时性能是为了避免平均性能无法即时反映性能下降
        if (m_GSList[i].fImmediatePerformance < g_pGameCenter->m_Settings.m_fImmediatePerformanceLimitForCreateMap)
            continue;

        if (m_GSList[i].fAveragePerformance > fPerformance)
        {
            fPerformance = m_GSList[i].fAveragePerformance;
            nConnIndex   = m_GSList[i].nConnIndex;
        }
    }

    return nConnIndex;
}

BOOL KGameServer::IsPlayerLoginPerformanceLimit(int nConnIndex)
{
    BOOL        bResult = false;
    KGS_INFO*   pGSInfo = NULL;

    pGSInfo = GetGSInfoByConnectionIndex(nConnIndex);
    LOG_PROCESS_ERROR(pGSInfo);

    PROCESS_ERROR(pGSInfo->fAveragePerformance < g_pGameCenter->m_Settings.m_fAveragePerformanceLimitForNewPlayer);

    PROCESS_ERROR(pGSInfo->fImmediatePerformance < g_pGameCenter->m_Settings.m_fImmediatePerformanceLimitForNewPlayer);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KGameServer::ProcessRoleLogin(
    KACCOUNT_LOGIN_INFO* pAccInfo, DWORD dwRoleID, KRoleBaseInfo* pBaseInfo, DWORD dwQueueID
)
{
    int							nResult				    = false;
    int                         nRetCode                = false;
    unsigned                    uBufferSize             = 0;
    KRole*                      pRole                   = NULL;
    KMapManager*                pMapManager             = &g_pGameCenter->m_MapManager;                 
    KMapCopy*                   pMapCopy                = NULL;
    DWORD                       dwMapID                 = 0;
    int                         nMapCopyIndex           = 0;
    BOOL                        bMapPermit              = true;
    int                         nMaxPermitPlayerCount   = 0;
    KMapInfo*                   pMap                    = NULL;
    const GUID                  Guid                    = {0, 0, 0, 0};
    BOOL                        bGSPerformLimit         = false;
    BOOL                        bIsPlayerKeeped         = false;
    KGS_INFO*                   pGSInfo                 = NULL;

    assert(pAccInfo);
    assert(pBaseInfo);

    pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleID);
    LOG_PROCESS_ERROR(pRole);

    dwMapID         = pBaseInfo->CurrentPos.dwMapID;
    nMapCopyIndex   = pBaseInfo->CurrentPos.nMapCopyIndex;

    pMap = pMapManager->GetMapInfo(dwMapID);
    if (pMap == NULL)
    {
        QLogPrintf(
            LOG_ERR, "Invalid map ID %u, Role: %s\n", 
            dwMapID, pRole->m_szName
        );

        g_pGameCenter->m_Gateway.DoGameLoginRespond(
            pAccInfo->nIdentity, eGameLoginSystemMaintenance, pRole->m_dwPlayerID, INVALID_QUEUE_ID, 0, 0, Guid
        );

        goto EXIT1;
    }

    //if (pMap->m_nType == emtBattleField)
    //{
        //bMapPermit = g_pGameCenter->m_BattleFieldManager.CheckPermission(dwRoleID, dwMapID, nMapCopyIndex);
    //}

    pMapCopy = pMap->GetMapCopy(nMapCopyIndex);

    // 如果玩家所在新手村可能被运营时缩减,缩减后,找个人最少的地方塞进去
    // 这种情况很少,为了方便,就不找"人气较旺盛"的因素了
    if (pMapCopy == NULL && pMap->m_nType == emtBirthMap && nMapCopyIndex >= pMap->m_nSNGenerator)
    {
        KMapCopy*   pThisCopy       = NULL;
        int         nPlayerCount    = INT_MAX;

        for (int i = 1; i < pMap->m_nSNGenerator; i++)
        {
            pThisCopy = pMap->GetMapCopy(i);
            if (pThisCopy == NULL || pThisCopy->m_eState != eMapStateNormal)
                continue;

            if (pThisCopy->m_nPlayerCount < nPlayerCount)
            {
                pMapCopy      = pThisCopy;
                nMapCopyIndex = i;
                pBaseInfo->CurrentPos.nMapCopyIndex = i;

                nPlayerCount  = pThisCopy->m_nPlayerCount;
            }
        }
    }
    
    if (pMapCopy)
    {
        bIsPlayerKeeped = pMapCopy->IsPlayerKeeped(dwRoleID);
    }

    // 副本和战场无法进入则进入上一入口，上一入口需是NromalMap
    if (pMapCopy == NULL || (!bMapPermit) || (pMapCopy->m_nPlayerCount >= pMap->m_nMaxPlayerCount && !bIsPlayerKeeped))
    {
        if (pMap->m_nType == emtDungeon || pMap->m_nType == emtBattleField)
        {
            dwMapID         = pBaseInfo->LastEntry.dwMapID;
            nMapCopyIndex   = pBaseInfo->LastEntry.nMapCopyIndex;

            pMap = pMapManager->GetMapInfo(dwMapID);
            if (pMap == NULL || (pMap->m_nType != emtNormalMap && pMap->m_nType != emtBirthMap))
            {
                g_pGameCenter->m_Gateway.DoGameLoginRespond(
                    pAccInfo->nIdentity, eGameLoginSystemMaintenance, pRole->m_dwPlayerID, INVALID_QUEUE_ID, 0, 0, Guid
                );
                goto EXIT1;
            }
            pBaseInfo->CurrentPos = pBaseInfo->LastEntry;
            pMapCopy = pMapManager->GetMapCopy(dwMapID, nMapCopyIndex);
        }
    }

    // 需要加载的，只可能是动态加载的正常场景 新手村 和 离线的副本
    if (pMapCopy == NULL || pMapCopy->m_eState == eMapStateOffline)
    {
        int     nGSConnIndex    = 0;
        time_t  nCreateTime     = time(NULL);
        KRole*  pTeamLeader    = NULL;

        if (pRole->m_dwTeamID != KD_BAD_ID)
        {
            KTeam* pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pRole->m_dwTeamID);
            LOG_PROCESS_ERROR(pTeam);

            pTeamLeader = g_pGameCenter->m_RoleManager.GetRole(pTeam->dwAuthority[tatLeader]);
            LOG_PROCESS_ERROR(pTeamLeader);
        }

        nRetCode = pMapManager->IsMapCopyInPreloadGroup(dwMapID, nMapCopyIndex);
        if (nRetCode)
        {
            // 该副本不能被动态加载
            g_pGameCenter->m_Gateway.DoGameLoginRespond(
                pAccInfo->nIdentity, eGameLoginSystemMaintenance, pRole->m_dwPlayerID, INVALID_QUEUE_ID, 0, 0, Guid
            );
            goto EXIT1;
        }

        nRetCode = pMapManager->IsMapCopyCountLimit(pMap);
        if (nRetCode)
        {
            // 无法创建副本: 副本数超过限制
            g_pGameCenter->m_Gateway.DoGameLoginRespond(
                pAccInfo->nIdentity, eGameLoginOverload, pRole->m_dwPlayerID, INVALID_QUEUE_ID, 0, 0, Guid
            );
            goto EXIT1;
        }

        nGSConnIndex = g_pGameCenter->m_GameServer.GetBestGSConnectionForCreateMap(pMap);

		
        if (nGSConnIndex == 0)
        {
            g_pGameCenter->m_Gateway.DoGameLoginRespond(
                pAccInfo->nIdentity, eGameLoginOverload, pRole->m_dwPlayerID, INVALID_QUEUE_ID, 0, 0, Guid
            );
            goto EXIT1;
        }

        if (pMapCopy)
        {
            pMapCopy->m_nConnIndex = nGSConnIndex;
        }
        else
        {
            pMapCopy = pMap->CreateMapCopy(nMapCopyIndex, nCreateTime, 0, nGSConnIndex, true);
            LOG_PROCESS_ERROR(pMapCopy);
        }

        DoCreateMapNotify(nGSConnIndex, dwMapID, nMapCopyIndex);

        pMapCopy->m_eState = eMapStateCreating;
        
        nRetCode = g_pGameCenter->m_MapManager.SetMapCopyOwner(
            dwMapID, nMapCopyIndex, pTeamLeader ? pTeamLeader->m_dwPlayerID : dwRoleID
        );
        LOG_PROCESS_ERROR(nRetCode);
    }

    pMapCopy->m_nLastTryLoginTime  = g_pGameCenter->m_nTimeNow;

    bIsPlayerKeeped = pMapCopy->IsPlayerKeeped(dwRoleID);

    // 如果副本需要排队,玩家又没有在指定队伍排队,则重新排队
    if (pMapCopy->m_dwQueueID != INVALID_QUEUE_ID)
    {
        if (dwQueueID != pMapCopy->m_dwQueueID && !bIsPlayerKeeped)
        {
            if (dwQueueID != INVALID_QUEUE_ID)
            {
                QLogPrintf(LOG_DEBUG, "Incorrect QueueID : Player QID(%u), MapCopy QID(%u)", dwQueueID, pMapCopy->m_dwQueueID);
            }

            g_pGameCenter->m_Gateway.DoGameLoginRespond(
                pAccInfo->nIdentity, eGameLoginQueueWait, pRole->m_dwPlayerID, pMapCopy->m_dwQueueID, 0, 0, Guid
            );
            goto EXIT1;
        }
    }

    // 如果副本正在创建,副本人数已满,目标GS性能不足 或者 副本需要预排队, 则暂时不让新玩家登陆,排队等待
    nMaxPermitPlayerCount = pMap->m_nMaxPlayerCount;
    if (pMap->m_nLoginIntervalTime > 0)
    {
        int nPlayerLimit = 
            (int)(g_pGameCenter->m_nTimeNow - pMapCopy->m_nCreateTime) / pMap->m_nLoginIntervalTime + 
            pMap->m_nPreQueuePlayerCount;

//        if (nPlayerLimit < nMaxPermitPlayerCount)
//        {
//           nMaxPermitPlayerCount = nPlayerLimit;
//        }
    }

    bGSPerformLimit = IsPlayerLoginPerformanceLimit(pMapCopy->m_nConnIndex);
    if (
        (pMapCopy->m_eState == eMapStateCreating) ||
        (pMapCopy->m_nPlayerCount >= nMaxPermitPlayerCount && !bIsPlayerKeeped) ||
        bGSPerformLimit
    )
    {
        if (pMapCopy->m_dwQueueID == INVALID_QUEUE_ID)
        {
            pMapCopy->m_dwQueueID = m_dwNextLoginQueueID++;

            QLogPrintf(LOG_DEBUG, "New QueueID = %u, MapID(%u), MapCopyID(%d)", pMapCopy->m_dwQueueID, pMap->m_dwMapID, pMapCopy->m_nCopyIndex);
        }

        g_pGameCenter->m_Gateway.DoGameLoginRespond(
            pAccInfo->nIdentity, eGameLoginQueueWait, pRole->m_dwPlayerID, pMapCopy->m_dwQueueID, 0, 0, Guid
        );
        goto EXIT1;
    }

    if (bIsPlayerKeeped)
    {
        pMapCopy->UnRegisterKeepPlayer(dwRoleID);
    }

    if (pMapCopy->m_eState != eMapStateNormal)
    {
        // 副本可能在其他状态,比如正被重置的过程中,这个算了,不好处理,直接不让进得了
        g_pGameCenter->m_Gateway.DoGameLoginRespond(
            pAccInfo->nIdentity, eGameLoginSystemMaintenance, pRole->m_dwPlayerID, INVALID_QUEUE_ID, 0, 0, Guid
        );
        goto EXIT1;
    }

    // 在登录被拒绝的时候,需要把人数减回来;
    nRetCode = g_pGameCenter->m_MapManager.IncreaseMapCopyPlayerCount(dwMapID, nMapCopyIndex);
    LOG_PROCESS_ERROR(nRetCode);

    pRole->m_nDestConnIndex     = pMapCopy->m_nConnIndex;
    pRole->m_dwDestMapID        = dwMapID;
    pRole->m_nDestMapCopyIndex  = nMapCopyIndex;

    g_pGameCenter->m_GameServer.DoPlayerLoginRequest(
        pMapCopy->m_nConnIndex, *pAccInfo, dwRoleID, *pBaseInfo
    );

    pRole->SetRoleState(rsLG_WaitForPermit);

EXIT1:
    nResult = true;
EXIT0:
    return nResult;
}

BOOL KGameServer::GetGameWorldProtocolVersion(int* pnGameWorldLowerProtocolVersion, int* pnGameWorldUpperProtocolVersion)
{
    BOOL bResult = false;

    assert(pnGameWorldLowerProtocolVersion);
    assert(pnGameWorldUpperProtocolVersion);

    PROCESS_ERROR(m_bGameWorldProtocolValidFlag);

    *pnGameWorldLowerProtocolVersion = m_nGameWorldLowerProtocolVersion;
    *pnGameWorldUpperProtocolVersion = m_nGameWorldUpperProtocolVersion;

    bResult = true;
EXIT0:
    return bResult;
}

size_t KGameServer::GetFluxTotal()
{
    KGS_INFO*   pGS             = NULL;
    size_t      uTotalFlux      = 0;

    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        pGS = &m_GSList[i];

        if (pGS->piSocket == NULL)
            continue;

        uTotalFlux   += pGS->uNetworkFlux;
    }

    return uTotalFlux;
}

BOOL KGameServer::SendGlobalSysMsg(const char cszMsg[])
{
    BOOL                     bResult            = false;
    BOOL                     bRetCode           = false;
    //S2R_TALK_MESSAGE*        pUpMsg             = NULL;
    //size_t                   uTalkDataSize      = 0;
    //IMemBlock*              piSendBuffer       = NULL;
    //R2S_TALK_MESSAGE*        pDownMsg           = NULL;

    //uTalkDataSize = 1 + strlen(cszMsg) + 1;

 //   piSendBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_TALK_MESSAGE) + uTalkDataSize));
 //   LOG_PROCESS_ERROR(piSendBuffer);

 //   pDownMsg = (R2S_TALK_MESSAGE*)piSendBuffer->GetData();
 //   LOG_PROCESS_ERROR(pDownMsg);
 //   
 //   pDownMsg->wProtocolID      = r2s_talk_message;
 //   pDownMsg->byMsgType        = trGlobalSys;
	//pDownMsg->dwTalkerID       = KD_BAD_ID;
 //   pDownMsg->dwReceiverID     = KD_BAD_ID;

 //   pDownMsg->szTalker[0]      = '\0';
 //   pDownMsg->szReceiver[0]    = '\0';

 //   pDownMsg->byTalkData[0]    = tdbText;

 //   strcpy((char*)pDownMsg->byTalkData + 1, cszMsg);

 //   Broadcast(piSendBuffer);

//    bResult = true;
//EXIT0:
//    SAFE_RELEASE(piSendBuffer);
    return bResult;
}


BOOL KGameServer::LoadRoleData(int nConnIndex, DWORD dwPlayerID, BOOL bSucceed, BYTE* pbyRoleData, size_t uRoleDataLen)
{
    BOOL            bResult     = false;
    BOOL            bRetCode    = false;
    BYTE*           pbyPos      = pbyRoleData;
    BYTE*           pbyTail     = pbyRoleData + uRoleDataLen;

    while (pbyPos < pbyTail)
    {
        size_t uPackSize = MIN(pbyTail - pbyPos, MAX_ROLE_DATA_PAK_SIZE);

        bRetCode = DoSyncRoleData(nConnIndex, dwPlayerID, pbyPos, pbyPos - pbyRoleData, uPackSize);
        LOG_PROCESS_ERROR(bRetCode);

        pbyPos += uPackSize;
    }

    bRetCode = DoLoadRoleData(nConnIndex, dwPlayerID, bSucceed, uRoleDataLen);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

void KGameServer::ProcessAcceptor()
{
    int                 nWorldIndex     = 0;
	ISocketStream*   piSocket        = NULL;
    struct timeval      TimeVal         = {3, 0};
    KGS_INFO*           pGSInfo         = NULL;
    struct in_addr      RemoteAddr      = { 0 };
    u_short             wRemotePortNet  = 0;
    u_short             wRemotePortHost = 0;
    char*               pszRetString    = NULL;

    piSocket = m_SocketAcceptor.Accept();
    PROCESS_ERROR(piSocket);

    PROCESS_ERROR(!m_bQuiting);
    
    piSocket->SetTimeout(&TimeVal);

    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        if (m_GSList[i].piSocket == NULL)
        {
            nWorldIndex = i + 1;
            pGSInfo     = &m_GSList[i];
            break;
        }
    }

    PROCESS_ERROR(pGSInfo);

    pGSInfo->piSocket = piSocket;
    pGSInfo->piSocket->AddRef();

    pGSInfo->nConnIndex             = m_nLastConnIndex++;
    pGSInfo->nWorldIndex            = nWorldIndex;
    pGSInfo->fImmediatePerformance  = 0.0f;
    pGSInfo->fAveragePerformance    = 0.0f;
    pGSInfo->uMemory                = 0;
    pGSInfo->nPlayerCount           = 0;
    pGSInfo->nConnectionCount       = 0;
    pGSInfo->uNetworkFlux           = 0;
    pGSInfo->nLastRecvPacketTime    = g_pGameCenter->m_nTimeNow;

    piSocket->GetRemoteAddress(&RemoteAddr, &wRemotePortNet);
    wRemotePortHost = ntohs(wRemotePortNet);

    pszRetString = inet_ntoa(RemoteAddr);
    LOG_PROCESS_ERROR(pszRetString);

	OnGSConnected(pGSInfo->nConnIndex);

    QLogPrintf(
        LOG_INFO, "GS %d connected from %s:%u, WorldIndex = %d\n", 
        pGSInfo->nConnIndex, pszRetString, wRemotePortHost, nWorldIndex
    );

EXIT0:
    SAFE_RELEASE(piSocket);
    return;
}

VOID KGameServer::OnGSConnected(int nConnIndex)
{
	g_pGameCenter->m_TongManager.OnGSConnected(nConnIndex);
}


BOOL KGameServer::ProcessConnection(int nConnIndex, ISocketStream* piSocket)
{
    BOOL                        bResult         = false;
    int                         nRetCode        = 0;
    size_t                      uPakSize        = 0;
    IMemBlock*                 piPackage       = NULL;
    time_t                      nCurrentTime    = g_pGameCenter->m_nTimeNow;
	KGS_INFO*                   pGSInfo         = NULL;
    int                         nIdleTime       = 0;

    assert(piSocket);
    // 在协议响应函数中,可能将Socket关闭,所以这里先AddRef
    piSocket->AddRef();

	pGSInfo = GetGSInfoByConnectionIndex(nConnIndex);
	LOG_PROCESS_ERROR(pGSInfo);

    while (true)
    {
        struct timeval              TimeVal         = {0, 0};
        INTERNAL_PROTOCOL_HEADER*   pHeader         = NULL;
		PROCESS_PROTOCOL_FUNC       ProcessFunc     = NULL;
		MODULE_PROTOCOL_FUNC		ModuleProcessFunc     = NULL;

        nRetCode = piSocket->CheckCanRecv(&TimeVal);
        if (nRetCode == 0)
            break;

        PROCESS_ERROR(nRetCode == 1);
        
        SAFE_RELEASE(piPackage);

        nRetCode = piSocket->Recv(&piPackage);
        PROCESS_ERROR(nRetCode == 1);
        
        LOG_PROCESS_ERROR(piPackage);

	    pGSInfo->nLastRecvPacketTime = nCurrentTime;

	    pHeader = (INTERNAL_PROTOCOL_HEADER*)piPackage->GetData();
        LOG_PROCESS_ERROR(pHeader);

		if(pHeader->wProtocolID <= s2r_protocol_begin
			|| pHeader->wProtocolID >= s2r_protocol_end)
		{
			QLogPrintf(LOG_ERR,"..pHeader->wProtocolID=%d,piPackage->GetSize()=%d",(int)pHeader->wProtocolID,(int)piPackage->GetSize()); 
		}
		
        LOG_PROCESS_ERROR(pHeader->wProtocolID > s2r_protocol_begin);
	    LOG_PROCESS_ERROR(pHeader->wProtocolID < s2r_protocol_end);

        uPakSize = piPackage->GetSize();
        LOG_PROCESS_ERROR(uPakSize >= m_uProtocolSize[pHeader->wProtocolID]);

		ModuleProcessFunc = m_ModuleProtocolFuns[pHeader->wProtocolID];
		if (ModuleProcessFunc)
		{
			ModuleProcessFunc((BYTE*)(pHeader), uPakSize, nConnIndex);
		}
		else
		{
			ProcessFunc = m_ProcessProtocolFuns[pHeader->wProtocolID];
			LOG_PROCESS_ERROR(ProcessFunc);

			(this->*ProcessFunc)((BYTE*)(pHeader), uPakSize, nConnIndex);
		}
    }
    
    nIdleTime = (int)(nCurrentTime - pGSInfo->nLastRecvPacketTime);
	if (nIdleTime > m_nConnectionTimeout)
    {
        QLogPrintf(LOG_DEBUG, "[GS%d] LastPakTime = %u\n", nConnIndex, pGSInfo->nLastRecvPacketTime);
        QLogPrintf(LOG_DEBUG, "[GS%d] CurrentTime = %u\n", nConnIndex, nCurrentTime);

        QLogPrintf(LOG_ERR, "GS %d timeout(%d)\n", nConnIndex, nIdleTime);
        goto EXIT0;
    }

    bResult = true;
EXIT0:

    SAFE_RELEASE(piSocket);

    if (!bResult)
    {
        CloseConnection(nConnIndex);
    }

    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KGameServer::CloseConnection(int nConnIndex)
{
    BOOL      bResult   = false;
    KGS_INFO* pGSInfo   = GetGSInfoByConnectionIndex(nConnIndex);
    int       nGSCount  = 0;

    LOG_PROCESS_ERROR(pGSInfo);

    QLogPrintf(LOG_INFO, "GS %d disconnected !\n", nConnIndex);

    g_pGameCenter->m_RoleManager.OnGSCrash(nConnIndex);	

    g_pGameCenter->m_MapManager.OnGameServerDestroy(nConnIndex);

    SAFE_RELEASE(pGSInfo->piSocket);

    pGSInfo->nConnIndex = 0;

    pGSInfo->uRoleDataOffset = 0;

    // 如果所有的GS都断开了,则设置协议版本号为"未设置"
    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        if (m_GSList[i].piSocket != NULL)
        {
            nGSCount++;
        }
    }

    if (nGSCount == 0)
    {
        g_pGameCenter->m_Gateway.DoSetGameWorldVersion(0, 0);
        m_bGameWorldProtocolValidFlag = false;
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KGameServer::Send(int nConnIndex, IMemBlock* piBuffer)
{
    BOOL        bResult     = false;
    int         nRetCode    = false;
    KGS_INFO*   pGSInfo     = NULL;

    PROCESS_SUCCESS(nConnIndex == 0);

    pGSInfo = GetGSInfoByConnectionIndex(nConnIndex);
    LOG_PROCESS_ERROR(pGSInfo);    

    LOG_PROCESS_ERROR(pGSInfo->piSocket);    

    nRetCode = pGSInfo->piSocket->Send(piBuffer);
    LOG_PROCESS_ERROR(nRetCode == 1);

EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

void KGameServer::Broadcast(IMemBlock* piBuffer, int nExceptConnIndex/* = 0*/)
{
    for (int i = 0; i < MAX_GS_COUNT; i++)
    {
        KGS_INFO* pGSInfo = &m_GSList[i];

        if (pGSInfo->piSocket == NULL)
            continue;

        if (pGSInfo->nConnIndex == nExceptConnIndex)
            continue;

        pGSInfo->piSocket->Send(piBuffer);
    }
}

BOOL KGameServer::CallGSConnectScript(int nConnIndex)
{
    BOOL        bResult             = false;
    BOOL        bRetCode            = false;
    int         nIndex              = 0;
    DWORD       dwScriptID          = 0;
    const char* pszFunction         = "OnGSConnect";
//
//    dwScriptID = g_FileNameHash(MAIN_SCRIPT_FILE);
//
//    g_pGameCenter->m_ScriptManager.SafeCallBegin(&nIndex);
//
//    bRetCode = g_pGameCenter->m_ScriptManager.IsScriptExist(dwScriptID);
//    PROCESS_ERROR(bRetCode);
//
//    bRetCode = g_pGameCenter->m_ScriptManager.IsFuncExist(dwScriptID, pszFunction);
//    PROCESS_ERROR(bRetCode);
//
//    g_pGameCenter->m_ScriptManager.Push(nConnIndex);
//
//    g_pGameCenter->m_ScriptManager.Call(dwScriptID, pszFunction, 0);
//
//    bResult = true;
//EXIT0:
//    g_pGameCenter->m_ScriptManager.SafeCallEnd(nIndex);
    return bResult;
}

void KGameServer::OnHandshakeRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    BOOL                    bResult         = false;
    int                     nRetCode        = false;
    S2R_HANDSHAKE_REQUEST*  pHandshake      = (S2R_HANDSHAKE_REQUEST*)pbyData;
    int                     nServerTimeDiff = 0;

	QLogPrintf(LOG_INFO, "KGameServer::OnHandshakeRequest nConnIndex=%d\n", nConnIndex);

    if (m_bGameWorldProtocolValidFlag)
    {
        LOG_PROCESS_ERROR(pHandshake->nGameWorldLowerVersion == m_nGameWorldLowerProtocolVersion);
        LOG_PROCESS_ERROR(pHandshake->nGameWorldUpperVersion == m_nGameWorldUpperProtocolVersion);
    }
    else
    {
        m_nGameWorldLowerProtocolVersion = pHandshake->nGameWorldLowerVersion;
        m_nGameWorldUpperProtocolVersion = pHandshake->nGameWorldUpperVersion;

        g_pGameCenter->m_Gateway.DoSetGameWorldVersion(m_nGameWorldLowerProtocolVersion, m_nGameWorldUpperProtocolVersion);

        m_bGameWorldProtocolValidFlag = true;
    }

    nServerTimeDiff = (int)(pHandshake->nServerTime - g_pGameCenter->m_nTimeNow);

    if (abs(nServerTimeDiff) > 1)
    {
        QLogPrintf(LOG_WARNING, "GS %d Time difference is %d !\n", nConnIndex, nServerTimeDiff);
    }

    DoHandshakeRespond(nConnIndex);

    //DoSyncZoneChargeFlag(nConnIndex, g_pGameCenter->m_nZoneChargeFlag);

    //DoSyncCampInfo(nConnIndex);

    //DoSyncGlobalSystemValue(nConnIndex);

    //CallGSConnectScript(nConnIndex);

    g_pGameCenter->m_TeamCenter.SyncNewServer(nConnIndex);

    g_pGameCenter->m_MapManager.OnGameServerCreate(nConnIndex);

    //g_pGameCenter->m_PQCenter.OnGameServerCreate(nConnIndex);

    //g_pGameCenter->m_RoadCenter.SyncNewServer(nConnIndex);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        CloseConnection(nConnIndex);
    }
	return;
}

void KGameServer::OnPingSignal(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	//QLogPrintf(LOG_INFO, "KGameServer::OnPingSignal nConnIndex=%d\n", nConnIndex);
}

void KGameServer::OnUpdatePerformance(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    KGS_INFO*                       pGSInfo             = NULL;
    S2R_UPDATE_PERFORMANCE*         pPerformanceData    = (S2R_UPDATE_PERFORMANCE*)pbyData;

	//QLogPrintf(LOG_INFO, "KGameServer::OnUpdatePerformance nConnIndex=%d\n", nConnIndex);

    pGSInfo = GetGSInfoByConnectionIndex(nConnIndex);
    LOG_PROCESS_ERROR(pGSInfo);

    pGSInfo->fImmediatePerformance = 
        (pGSInfo->fImmediatePerformance * 60.0f + pPerformanceData->nIdleFrame * 40.0f) / 100.0f;

    pGSInfo->fAveragePerformance   =
        (pGSInfo->fAveragePerformance * 99.0f + pPerformanceData->nIdleFrame * 1.0f) / 100.0f;

    pGSInfo->uMemory               = pPerformanceData->uMemory;
    pGSInfo->nPlayerCount          = pPerformanceData->nPlayerCount;
    pGSInfo->nConnectionCount      = pPerformanceData->nConnectionCount;
    pGSInfo->uNetworkFlux          = pPerformanceData->uNetworkFlux;

EXIT0:
    return;
}

void KGameServer::OnPlayerLoginRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                        bRetCode    = false;
	S2R_PLAYER_LOGIN_RESPOND*   pRespond    = (S2R_PLAYER_LOGIN_RESPOND*)pbyData;
	KRole*                      pRole       = NULL;

	QLogPrintf(LOG_INFO, "KGameServer::OnPlayerLoginRespond nConnIndex=%d dwPlayerID=%d\n", nConnIndex, pRespond->dwPlayerID);
    
	if (IS_BZONE_PLAYER(pRespond->dwPlayerID))
	{
		OnBZonePlayerLoginRespond(pbyData, uDataLen, nConnIndex);
		goto EXIT0;
	}

    pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	if (IS_BZONE_PLAYER(pRole->GetSourcePlayerId()))
	{
		OnBZoneSlaveLoginRespond(pbyData, uDataLen, nConnIndex);
		goto EXIT0;
	}

	LOG_PROCESS_ERROR(pRole->GetRoleState() == rsLG_WaitForPermit);

	if (pRespond->bPermit)
	{
        g_pGameCenter->m_Gateway.DoGameLoginRespond(
            pRespond->dwPacketIdentity, eGameLoginSucceed, pRole->m_dwPlayerID, INVALID_QUEUE_ID,
            pRespond->dwGSAddr, pRespond->nGSPort,  pRespond->Guid
        );

		pRole->SetRoleState(rsLG_WaitForLogin);
	}
	else
	{
		g_pGameCenter->m_Gateway.DoGameLoginRespond(
            pRespond->dwPacketIdentity, eGameLoginUnknownError, pRole->m_dwPlayerID, INVALID_QUEUE_ID,
            pRespond->dwGSAddr, pRespond->nGSPort, pRespond->Guid
        );
        
        bRetCode = g_pGameCenter->m_MapManager.DecreaseMapCopyPlayerCount(pRole->m_dwDestMapID, pRole->m_nDestMapCopyIndex);
        LOG_PROCESS_ERROR(bRetCode);

		pRole->SetRoleState(rsOffline);
	}

EXIT0:
	return;
}

void KGameServer::OnBZonePlayerLoginRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                        bRetCode    = false;
	S2R_PLAYER_LOGIN_RESPOND*   pRespond    = (S2R_PLAYER_LOGIN_RESPOND*)pbyData;
	KRole*                      pRole       = NULL;

	QLogPrintf(LOG_INFO, "KGameServer::OnBZonePlayerLoginRespond nConnIndex=%d dwPlayerID=%d\n", nConnIndex, pRespond->dwPlayerID);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	//LOG_PROCESS_ERROR(pRole->GetRoleState() == rsBZ_WaitForPermit);

	if (pRespond->bPermit)
	{
		pRole->m_nConnIndex         = nConnIndex;
		pRole->m_dwMapID            = pRole->m_dwDestMapID;
		pRole->m_nMapCopyIndex      = pRole->m_nDestMapCopyIndex;

		g_BZoneMaster.DoBZoneTransferRespond(pRespond->dwPlayerID,
			pRespond->dwPacketIdentity, TRUE, pRole->GetSourcePlayerId(), INVALID_QUEUE_ID,
			pRespond->dwGSAddr, pRespond->nGSPort,  pRespond->Guid
			);
	}
	else
	{
		g_BZoneMaster.DoBZoneTransferRespond(pRespond->dwPlayerID,
			pRespond->dwPacketIdentity, FALSE, pRole->GetSourcePlayerId(), INVALID_QUEUE_ID,
			pRespond->dwGSAddr, pRespond->nGSPort, pRespond->Guid
			);

		bRetCode = g_pGameCenter->m_MapManager.DecreaseMapCopyPlayerCount(pRole->m_dwDestMapID, pRole->m_nDestMapCopyIndex);
		LOG_PROCESS_ERROR(bRetCode);
	}

EXIT0:
	return;
}

void KGameServer::OnBZoneSlaveLoginRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                        bRetCode    = false;
	S2R_PLAYER_LOGIN_RESPOND*   pRespond    = (S2R_PLAYER_LOGIN_RESPOND*)pbyData;
	KRole*                      pRole       = NULL;

	QLogPrintf(LOG_INFO, "KGameServer::OnBZoneSlaveLoginRespond nConnIndex=%d dwPlayerID=%d\n", nConnIndex, pRespond->dwPlayerID);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);
	
	if (pRespond->bPermit)
	{
		g_BZoneSlave.DoBZoneReturnRespond(pRespond->dwPlayerID,
			pRespond->dwPacketIdentity, TRUE, pRole->GetSourcePlayerId(), INVALID_QUEUE_ID,
			pRespond->dwGSAddr, pRespond->nGSPort,  pRespond->Guid
			);
	}
	else
	{
		g_BZoneSlave.DoBZoneReturnRespond(pRespond->dwPlayerID,
			pRespond->dwPacketIdentity, TRUE, pRole->GetSourcePlayerId(), INVALID_QUEUE_ID,
			pRespond->dwGSAddr, pRespond->nGSPort,  pRespond->Guid
			);

		/*
		bRetCode = g_pGameCenter->m_MapManager.DecreaseMapCopyPlayerCount(pRole->m_dwDestMapID, pRole->m_nDestMapCopyIndex);
		LOG_PROCESS_ERROR(bRetCode);
		*/
	}

	pRole->SetSourcePlayerId(0);

EXIT0:
	return;
}

void KGameServer::OnCreateMapRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode            = false;
	S2R_CREATE_MAP_RESPOND*         pRespond            = (S2R_CREATE_MAP_RESPOND*)pbyData;
    KMapInfo*                       pMapInfo            = NULL;
    KMapCopy*                       pMapCopy            = NULL;
    KRole*                          pRole               = NULL;
    KROLE_STATE                     eRoleState          = rsInvalid;
    DWORD                           dwOwnerID           = KD_BAD_ID;

	QLogPrintf(LOG_INFO, "KGameServer::OnCreateMapRespond nConnIndex=%d dwMapID=%d nMapCopyIndex=%d\n", nConnIndex, pRespond->dwMapID, pRespond->nMapCopyIndex);

    pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pRespond->dwMapID);
    LOG_PROCESS_ERROR(pMapInfo);

    pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pRespond->dwMapID, pRespond->nMapCopyIndex);
    LOG_PROCESS_ERROR(pMapCopy);

    //QLogPrintf(
    //   LOG_INFO, "Create map %s(%.2u, %d) on GS %d %s !\n", 
    //   pMapInfo->m_szName, pRespond->dwMapID, pRespond->nMapCopyIndex, nConnIndex, 
    //   pRespond->bResult ? "OK" : "Failed"
    //);

    assert(pMapCopy->m_eState == eMapStateCreating);

    // 切换地图状态,处理等待在这张地图上的所有玩家

	if (pRespond->bResult)
	{
        pMapCopy->m_eState = eMapStateNormal;

        g_pGameCenter->m_StatDataManager.UpdateCreateMap(pMapInfo);

        // 创建场景绑定的PQ
        //bRetCode = g_pGameCenter->m_PQCenter.OnSceneCreate(pRespond->dwMapID, pRespond->nMapCopyIndex);
        //LOG_CHECK_ERROR(bRetCode);

        for (KCREATE_MAP_CG_CALLBACK_LIST::const_iterator it = pMapCopy->m_CGCallbackList.begin(); it != pMapCopy->m_CGCallbackList.end(); ++it)
        {
            pRole = g_pGameCenter->m_RoleManager.GetRole(it->dwPlayerID);
            LOG_PROCESS_ERROR(pRole);

            eRoleState = pRole->GetRoleState();
            LOG_PROCESS_ERROR(eRoleState == rsCG_WaitForCreateMap || eRoleState == rsOnline);
            
            if (eRoleState == rsOnline)
            {
                // 通知玩家地图已加载好，可以进来了
                g_pGameCenter->m_GameServer.DoFinishCreateMapNotify(pRole->m_nConnIndex, pRole->m_dwPlayerID);
                continue;
            }

            // 注意,这里不见得就是跨服,确切的说应该是跨地图等待创建目标地图
            if (pRole->m_nConnIndex == nConnIndex)
            {
                bRetCode = g_pGameCenter->m_MapManager.DecreaseMapCopyPlayerCount(pRole->m_dwMapID, pRole->m_nMapCopyIndex);
                LOG_PROCESS_ERROR(bRetCode);

                pRole->m_dwMapID        = pRespond->dwMapID;
                pRole->m_nMapCopyIndex  = pRespond->nMapCopyIndex;

                bRetCode = g_pGameCenter->m_MapManager.IncreaseMapCopyPlayerCount(pRespond->dwMapID, pRespond->nMapCopyIndex);
                LOG_PROCESS_ERROR(bRetCode);

                //g_pGameCenter->m_GameServer.DoSyncFellowshipMapID(pRole->m_dwPlayerID, pRole->m_dwMapID);

                // 玩家就在本服务器上,那么, 直接切换回"Online"即可
                DoSearchMapRespond(
                    pRole->m_nConnIndex, it->dwPlayerID, smrcSuccess, false, 
                    pRespond->dwMapID, pRespond->nMapCopyIndex,
                    it->nX, it->nY, it->nZ
                );
                pRole->SetRoleState(rsOnline);
            }
            else
            {
                // 玩家并不在本服务器上,那么等待源服务器传角色数据过来(跨服)
                DoSearchMapRespond(
                    pRole->m_nConnIndex, it->dwPlayerID, smrcSuccess, true, 
                    pRespond->dwMapID, pRespond->nMapCopyIndex,
                    it->nX, it->nY, it->nZ
                );

                pRole->SetRoleState(rsCG_WaitForPlayerData);

                pRole->m_dwDestMapID        = pRespond->dwMapID;
                pRole->m_nDestMapCopyIndex  = pRespond->nMapCopyIndex;
                pRole->m_nDestConnIndex     = nConnIndex;

                bRetCode = g_pGameCenter->m_MapManager.IncreaseMapCopyPlayerCount(pRespond->dwMapID, pRespond->nMapCopyIndex);
                LOG_PROCESS_ERROR(bRetCode);
            }
        }

        pMapCopy->ClearCGCallbackPlayer();
	}
    else
    {  
        for (KCREATE_MAP_CG_CALLBACK_LIST::const_iterator it = pMapCopy->m_CGCallbackList.begin(); it != pMapCopy->m_CGCallbackList.end(); ++it)
        {
            pRole = g_pGameCenter->m_RoleManager.GetRole(it->dwPlayerID);
            if (!pRole)
                continue;

            eRoleState = pRole->GetRoleState();

            LOG_PROCESS_ERROR(eRoleState == rsCG_WaitForCreateMap || eRoleState == rsOnline);
            // 通知玩家,前方路途不通,稍后再来
            DoSearchMapRespond(
                pRole->m_nConnIndex, it->dwPlayerID, smrcFailed, false, 
                pRespond->dwMapID, pRespond->nMapCopyIndex,
                it->nX, it->nY, it->nZ
            );
            pRole->SetRoleState(rsOnline);
        }

        pMapCopy->ClearCGCallbackPlayer();

        if (pMapCopy->m_piSceneData == NULL)
        {
            pMapInfo->DeleteMapCopy(pRespond->nMapCopyIndex);
        }
        else
        {
            pMapCopy->m_eState      = eMapStateOffline;
            pMapCopy->m_nConnIndex  = 0;
        }
    }

EXIT0:
	return;
}

#define DO_SEARCH_MAP_FAILED_RETURN(C, Result)                      \
    do                                                              \
    {																\
        if (!(C))                                                   \
        {                                                           \
			QLogPrintf(LOG_INFO,"DO_SEARCH_MAP_FAILED_RETURN C=%s Result=%d",#C,Result);\
			DoSearchMapRespond(                                     \
                nConnIndex, pRequest->dwPlayerID, Result, false,    \
                pRequest->dwMapID, nMapCopyIndex,                   \
                pRequest->nPosX, pRequest->nPosY, pRequest->nPosZ   \
            );                                                      \
            goto EXIT0;                                             \
        }                                                           \
    } while (false)


void KGameServer::OnSearchMapRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    BOOL                        bResult             = false;
    BOOL                        bRetCode            = false;
	S2R_SEARCH_MAP_REQUEST*     pRequest            = (S2R_SEARCH_MAP_REQUEST*)pbyData;
    KROLE_STATE                 eRoleState          = rsInvalid;
	int                         nMapCopyIndex       = pRequest->nMapCopyIndex;
	KRole*                      pRole               = NULL;
    KMapInfo*                   pMapInfo            = NULL;
    KMapCopy*                   pMapCopy            = NULL;
    KRole*                      pTeamLeader         = NULL;

	QLogPrintf(LOG_INFO, "KGameServer::OnSearchMapRequest nConnIndex=%d dwPlayerID=%d dwMapID=%d nMapCopyIndex=%d bOnThisGs=%d\n", 
		nConnIndex, pRequest->dwPlayerID, pRequest->dwMapID, pRequest->nMapCopyIndex, pRequest->bOnThisGs);

    DO_SEARCH_MAP_FAILED_RETURN(pRequest->nMapCopyIndex >= 0, smrcFailed);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

    if (pRole->GetAutoTeamId() != KD_BAD_ID)
    {
        KTeam* pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pRole->GetAutoTeamId());
        LOG_PROCESS_ERROR(pTeam);

        pTeamLeader = g_pGameCenter->m_RoleManager.GetRole(pTeam->dwAuthority[tatLeader]);

		//临时解决组队bug导致无法切场景，此时pTeam对象中已无pRole成员,bug范围已确定在TeamCenter.cpp中。
		//需要找到最终原因并消除以下if{}块，恢复LOG_PROCESS_ERROR
		//以下为临时解决方案,以便玩家可以继续正常游戏 
		//@wangyue 2015-12-22 14:30:05
		if (pTeamLeader == NULL)
		{
			int team_id = pRole->GetAutoTeamId();
			if (pRole->m_dwSystemTeamID)
			{
				pRole->m_dwSystemTeamID = KD_BAD_ID;
			}
			else
			{
				pRole->m_dwTeamID = KD_BAD_ID;
			}
			QLogPrintf(LOG_INFO, "focre fix team && role when team_id=%d, role_id=%d\n", team_id, pRole->m_dwPlayerID);
			pTeam = NULL;
		}
		//LOG_PROCESS_ERROR(pTeamLeader);
    }

	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pRequest->dwMapID);
    DO_SEARCH_MAP_FAILED_RETURN(pMapInfo != NULL, smrcFailed);

    eRoleState  = pRole->GetRoleState();
	DO_SEARCH_MAP_FAILED_RETURN(eRoleState == rsOnline, smrcFailed);

    DO_SEARCH_MAP_FAILED_RETURN(pRole->m_nConnIndex == nConnIndex, smrcFailed);

    switch (pMapInfo->m_nType)
    {
    case emtBirthMap:
        DO_SEARCH_MAP_FAILED_RETURN(nMapCopyIndex < pMapInfo->m_nSNGenerator, smrcFailed);

        if (nMapCopyIndex == 0)
        {
            nMapCopyIndex = 1;
        }
    
        pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pRequest->dwMapID, nMapCopyIndex);
        DO_SEARCH_MAP_FAILED_RETURN(pMapCopy, smrcFailed);
        DO_SEARCH_MAP_FAILED_RETURN(pMapCopy->m_eState == eMapStateNormal, smrcFailed);
    	break;

    case emtNormalMap:
        nMapCopyIndex = 1;
        
        pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pRequest->dwMapID, nMapCopyIndex);
        if (!pMapCopy)
        {
            // 不在预加载表里的才可动态加载
            bRetCode = g_pGameCenter->m_MapManager.IsMapCopyInPreloadGroup(pRequest->dwMapID, nMapCopyIndex);
            DO_SEARCH_MAP_FAILED_RETURN(!bRetCode, smrcGameMaintenance);
        }
    	break;

    case emtDungeon:
        if (nMapCopyIndex == 0)
        {
            nMapCopyIndex = g_pGameCenter->m_MapManager.FindPlayerCopyIndex(pRole, pTeamLeader, pRequest->dwMapID);
        }

        if (nMapCopyIndex == 0)
        {
            UINT64    nNextCopyIndex = 0;

            bRetCode = g_pGameCenter->m_piSequence->GenerateID(pMapInfo->m_szSequenceName, 16, &nNextCopyIndex);
            LOG_PROCESS_ERROR(bRetCode);

            nMapCopyIndex = (int)nNextCopyIndex;
            pMapInfo->m_nSNGenerator++;
        }

        pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pRequest->dwMapID, nMapCopyIndex);
        break;

    case emtBattleField:
        // 战场不处理nMapCopyIndex
        pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pRequest->dwMapID, nMapCopyIndex);
        break;

    default:
        assert(false);
        break;
    }

    // 到这里,MapCopyIndex已经找到了
    if (pMapCopy == NULL || pMapCopy->m_eState == eMapStateOffline)
    {
        // 只有动态加载的NormalScene, Dungeon 和 BattleField能进入这里 
        int nDestConnIndex  = 0;

        bRetCode = g_pGameCenter->m_MapManager.IsMapCopyCountLimit(pMapInfo);
        DO_SEARCH_MAP_FAILED_RETURN(!bRetCode, smrcMapCopyCountLimit);

		if(pRequest->bOnThisGs)
		{
			nDestConnIndex = pRole->m_nConnIndex;
		}
		else
		{
			nDestConnIndex = g_pGameCenter->m_GameServer.GetBestGSConnectionForCreateMap(pMapInfo);
		}
		
        DO_SEARCH_MAP_FAILED_RETURN(nDestConnIndex != 0, smrcGameOverLoaded);
        
        bRetCode = pMapInfo->CheckPlayerEnterNewCopyTimes(pRequest->dwPlayerID, nMapCopyIndex);  
        DO_SEARCH_MAP_FAILED_RETURN(bRetCode, smrcEnterNewCopyTooManyTimes);

        if (pMapCopy != NULL)
        {
            pMapCopy->m_nConnIndex = nDestConnIndex;
        }
        else
        {
            pMapCopy = pMapInfo->CreateMapCopy(nMapCopyIndex, g_pGameCenter->m_nTimeNow, 0, nDestConnIndex, true);
		    LOG_PROCESS_ERROR(pMapCopy);
        }
        
        DoCreateMapNotify(nDestConnIndex, pRequest->dwMapID, nMapCopyIndex);

        pMapCopy->m_eState = eMapStateCreating;
        
        if (pMapInfo->m_nType == emtDungeon)
        {
            // 副本才有Owner
            bRetCode = g_pGameCenter->m_MapManager.SetMapCopyOwner(
                pRequest->dwMapID, nMapCopyIndex, 
                pTeamLeader ? pTeamLeader->m_dwPlayerID : pRequest->dwPlayerID
            );
            LOG_PROCESS_ERROR(bRetCode);
        }

        bRetCode = pMapCopy->RegisterCGCallbackPlayer(
            pRequest->dwPlayerID,
            pRequest->nPosX, pRequest->nPosY, pRequest->nPosZ
        );
        LOG_PROCESS_ERROR(bRetCode);

        pRole->m_nDestConnIndex     = nDestConnIndex;
        pRole->m_dwDestMapID        = pRequest->dwMapID;
        pRole->m_nDestMapCopyIndex  = nMapCopyIndex;

		pRole->SetRoleState(rsCG_WaitForCreateMap);

        goto EXIT0;
    }

    if (pMapCopy->m_eState == eMapStateCreating)
    {
        int nWaitPlayerCount = (int)pMapCopy->m_CGCallbackList.size();
        DO_SEARCH_MAP_FAILED_RETURN(nWaitPlayerCount < pMapInfo->m_nMaxPlayerCount, smrcGameOverLoaded);
        
        bRetCode = pMapInfo->CheckPlayerEnterNewCopyTimes(pRequest->dwPlayerID, nMapCopyIndex);  
        DO_SEARCH_MAP_FAILED_RETURN(bRetCode, smrcEnterNewCopyTooManyTimes);

        bRetCode = pMapCopy->RegisterCGCallbackPlayer(
            pRequest->dwPlayerID,
            pRequest->nPosX, pRequest->nPosY, pRequest->nPosZ
        );
        LOG_PROCESS_ERROR(bRetCode);

        pRole->m_nDestConnIndex     = pMapCopy->m_nConnIndex;
        pRole->m_dwDestMapID        = pRequest->dwMapID;
        pRole->m_nDestMapCopyIndex  = nMapCopyIndex;

		pRole->SetRoleState(rsCG_WaitForCreateMap);
        
        goto EXIT0;
    }

    DO_SEARCH_MAP_FAILED_RETURN(pMapCopy->m_eState == eMapStateNormal, smrcFailed);

    DO_SEARCH_MAP_FAILED_RETURN(pMapCopy->m_nPlayerCount < pMapInfo->m_nMaxPlayerCount, smrcGameOverLoaded);
    
    bRetCode = pMapInfo->CheckPlayerEnterNewCopyTimes(pRequest->dwPlayerID, nMapCopyIndex);  
    DO_SEARCH_MAP_FAILED_RETURN(bRetCode, smrcEnterNewCopyTooManyTimes);

    // 找到了，看是不是要跨服务器
    if (pMapCopy->m_nConnIndex == nConnIndex)
    {        
	    // 不跨服务器, 通知GS换地图
        DoSearchMapRespond(
            nConnIndex, pRequest->dwPlayerID, smrcSuccess, false, 
            pRequest->dwMapID, nMapCopyIndex,
            pRequest->nPosX, pRequest->nPosY, pRequest->nPosZ
        );
        
        bRetCode = g_pGameCenter->m_MapManager.DecreaseMapCopyPlayerCount(pRole->m_dwMapID, pRole->m_nMapCopyIndex);
        LOG_PROCESS_ERROR(bRetCode);

	    pRole->m_dwMapID        = pRequest->dwMapID;
	    pRole->m_nMapCopyIndex  = nMapCopyIndex;

        //g_pGameCenter->m_GameServer.DoSyncFellowshipMapID(pRole->m_dwPlayerID, pRole->m_dwMapID);
        
        bRetCode = g_pGameCenter->m_MapManager.IncreaseMapCopyPlayerCount(pRequest->dwMapID, nMapCopyIndex);
        LOG_PROCESS_ERROR(bRetCode);
    }
    else
    {
        // 进入跨服流程
	    DoSearchMapRespond(
            nConnIndex, pRequest->dwPlayerID, smrcSuccess, true, 
            pRequest->dwMapID, nMapCopyIndex,
            pRequest->nPosX, pRequest->nPosY, pRequest->nPosZ
        );  

	    pRole->SetRoleState(rsCG_WaitForPlayerData);

        pRole->m_nDestConnIndex     = pMapCopy->m_nConnIndex;
        pRole->m_dwDestMapID        = pRequest->dwMapID;
        pRole->m_nDestMapCopyIndex  = nMapCopyIndex;
        
        bRetCode = g_pGameCenter->m_MapManager.IncreaseMapCopyPlayerCount(pRequest->dwMapID, nMapCopyIndex);
        LOG_PROCESS_ERROR(bRetCode);
    }

EXIT0:
	return;
}
#undef DO_SEARCH_MAP_FAILED_RETURN

#define DO_LOAD_MAP_FAILED_RETURN(C, Result)                       \
	do                                                              \
	{                                                               \
		if (!(C))                                                   \
		{                                                           \
			DoLoadMapRespond(                                     \
				nConnIndex, pRequest->dwMapID,                 \
				nMapCopyIndex, Result, pRequest->dwParam1         \
				);                                                      \
			goto EXIT0;                                             \
		}                                                           \
	} while (false)

void KGameServer::OnDeleteBattleMapRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                        bResult             = false;
	BOOL                        bRetCode            = false;
	S2R_DELETE_BATTLE_MAP*      pRequest            = (S2R_DELETE_BATTLE_MAP*)pbyData;
	KMapInfo*                   pMapInfo            = NULL;
	KMapCopy*                   pMapCopy            = NULL;

	QLogPrintf(LOG_INFO, "KGameServer::OnDeleteBattleMapRequest nConnIndex=%d dwMapId=%d dwMapCopyIndex=%d\n",
		nConnIndex, pRequest->dwMapId, pRequest->dwMapCopyIndex);

	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pRequest->dwMapId);
	LOG_PROCESS_ERROR(pMapInfo);

    pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pRequest->dwMapId, pRequest->dwMapCopyIndex);
	LOG_PROCESS_ERROR(pMapCopy);

	PROCESS_ERROR(pMapCopy->m_eState != eMapStateUnloading);
	LOG_PROCESS_ERROR(pMapInfo->m_nType == emtBattleField);

	g_pGameCenter->m_GameServer.DoMapCopyDoClearPlayer(pMapCopy->m_nConnIndex, pRequest->dwMapId, pRequest->dwMapCopyIndex);

	pMapCopy->m_eState = eMapStateUnloading;

EXIT0:;
}

//加载地图
void KGameServer::OnLoadMapRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                        bResult             = false;
	BOOL                        bRetCode            = false;
	S2R_LOAD_MAP_REQUEST*       pRequest            = (S2R_LOAD_MAP_REQUEST*)pbyData;
	KMapInfo*                   pMapInfo            = NULL;
	KMapCopy*                   pMapCopy            = NULL;
	INT							nMapCopyIndex		= 0;
	UINT64						nNextCopyIndex		= 0;

	QLogPrintf(LOG_INFO, "KGameServer::OnLoadMapRequest nConnIndex=%d dwMapID=%d dwMapOwnerId=%d dwParam1=%d bToThisServer=%d\n",
		nConnIndex, pRequest->dwMapID, pRequest->dwMapOwnerId, pRequest->dwParam1, pRequest->bToThisServer);

	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pRequest->dwMapID);
	DO_LOAD_MAP_FAILED_RETURN(pMapInfo != NULL, smrcFailed);

	bRetCode = g_pGameCenter->m_MapManager.IsMapCopyCountLimit(pMapInfo);
	DO_LOAD_MAP_FAILED_RETURN(!bRetCode, smrcMapCopyCountLimit);

	switch (pMapInfo->m_nType)
	{
	case emtBirthMap: // 新手村
		nMapCopyIndex = pMapInfo->m_nSNGenerator++;
		break;

	case emtDungeon:// 任务副本
		{
			KRole*			pRole			= NULL;
			KRole*			pTeamLeader		= NULL;

			pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwMapOwnerId);

			if (pRole && pRole->GetAutoTeamId() != KD_BAD_ID)
			{
				KTeam* pTeam = g_pGameCenter->m_TeamCenter.GetTeam(pRole->GetAutoTeamId());
				LOG_PROCESS_ERROR(pTeam);

				pTeamLeader = g_pGameCenter->m_RoleManager.GetRole(pTeam->dwAuthority[tatLeader]);
			}

			if (pRole != NULL)
			{
				nMapCopyIndex = g_pGameCenter->m_MapManager.FindPlayerCopyIndex(pRole, pTeamLeader, pRequest->dwMapID);
			}
			DO_LOAD_MAP_FAILED_RETURN(nMapCopyIndex == 0, smrcNotAllowed); // 不允许再创建一个同样的副本
		}

		bRetCode = g_pGameCenter->m_piSequence->GenerateID(pMapInfo->m_szSequenceName, 16, &nNextCopyIndex);
		LOG_PROCESS_ERROR(bRetCode);

		nMapCopyIndex = (int)nNextCopyIndex;
		pMapInfo->m_nSNGenerator++;

		break;

	case emtNormalMap:// 正常场景
		// 普通场景只加载一个
		DO_LOAD_MAP_FAILED_RETURN(pMapInfo->m_nSNGenerator == 1, smrcNotAllowed);
		DO_LOAD_MAP_FAILED_RETURN(pMapInfo->m_nSNGenerator <= pMapInfo->m_nMaxCopyCount, smrcNotAllowed);

		pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pRequest->dwMapID, pMapInfo->m_nSNGenerator);
		DO_LOAD_MAP_FAILED_RETURN(!pMapCopy, smrcNotAllowed);

		// 不在预加载表里的才可动态加载
		bRetCode = g_pGameCenter->m_MapManager.IsMapCopyInPreloadGroup(pRequest->dwMapID, pMapInfo->m_nSNGenerator);
		DO_LOAD_MAP_FAILED_RETURN(!bRetCode, smrcNotAllowed);

		nMapCopyIndex = 1;
		break;

	case emtBattleField:  // 战场允许动态加载// 战场
		//DO_LOAD_MAP_FAILED_RETURN(FALSE, smrcNotAllowed);
		nMapCopyIndex = pRequest->dwParam1;
		break;

	default:
		assert(false);
		break;
	}

	if (!pRequest->bToThisServer)
	{
        INT nBestConnIndex = GetBestGSConnectionForCreateMap(pMapInfo);  // 战场是活动动态分配的地图，必须考虑性能
		if (nBestConnIndex != 0) // 如果找不到，就返回请求所在服
		{
            nConnIndex = nBestConnIndex;
		}
	}

	pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pRequest->dwMapID, nMapCopyIndex);
	DO_LOAD_MAP_FAILED_RETURN(!pMapCopy, smrcFailed);

	pMapCopy = pMapInfo->CreateMapCopy(nMapCopyIndex, g_pGameCenter->m_nTimeNow, 0, nConnIndex, true);
	LOG_PROCESS_ERROR(pMapCopy);

	DoLoadMapRespond(nConnIndex, pRequest->dwMapID, nMapCopyIndex, smrcSuccess, pRequest->dwParam1);

	pMapCopy->m_eState = eMapStateCreating;

	if (pMapInfo->m_nType == emtDungeon)
	{
		g_pGameCenter->m_MapManager.SetMapCopyOwner(pRequest->dwMapID, nMapCopyIndex, pRequest->dwMapOwnerId);
	}

EXIT0:
	return;
}
#undef DO_LOAD_MAP_FAILED_RETURN

void KGameServer::OnTransferPlayerRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode            = false;
	S2R_TRANSFER_PLAYER_REQUEST*    pRequest            = (S2R_TRANSFER_PLAYER_REQUEST*)pbyData;
	KRole*                          pRole               = NULL;
    KROLE_STATE                     eRoleState          = rsInvalid;
    const KMapCopy*                 pcDestMapCopy       = NULL;
    IMemBlock*                     piPackage           = NULL;
    R2S_TRANSFER_PLAYER_REQUEST*    pTransferPackage    = NULL;

	QLogPrintf(LOG_INFO, "KGameServer::OnTransferPlayerRequest nConnIndex=%d dwRoleID=%d \n", nConnIndex, pRequest->dwRoleID);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRoleID);
	LOG_PROCESS_ERROR(pRole);

    eRoleState = pRole->GetRoleState();
	LOG_PROCESS_ERROR(eRoleState == rsCG_WaitForPlayerData);
	LOG_PROCESS_ERROR(pRole->m_nConnIndex == nConnIndex);

    // "找不到目标地图"在极少数情况下还是可能发生的,比如目标GS宕机
    // 即使找到了,理论上,还有可能是宕机了后再启动起来的,所以下一步还要核对一下连接号
    pcDestMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(
        pRequest->RoleBaseInfo.CurrentPos.dwMapID, 
        pRequest->RoleBaseInfo.CurrentPos.nMapCopyIndex
    );
    if (pcDestMapCopy == NULL || pcDestMapCopy->m_nConnIndex != pRole->m_nDestConnIndex)
    {
        DoTransferPlayerRespond(nConnIndex, pRole->m_dwPlayerID, false);

        pRole->m_dwDestMapID        = 0;
        pRole->m_nDestMapCopyIndex  = 0;
        pRole->SetRoleState(rsOnline);

        goto EXIT0;
    }

    LOG_PROCESS_ERROR(pcDestMapCopy->m_eState == eMapStateNormal);

	pRole->SetRoleState(rsCG_WaitForGuid);

    piPackage = QCreateMemBlock(sizeof(R2S_TRANSFER_PLAYER_REQUEST));
    LOG_PROCESS_ERROR(piPackage);

    pTransferPackage = (R2S_TRANSFER_PLAYER_REQUEST*)piPackage->GetData();
    LOG_PROCESS_ERROR(pTransferPackage);
    
    pTransferPackage->wProtocolID       = r2s_transfer_player_request;
    pTransferPackage->dwRoleID          = pRequest->dwRoleID;
    
    strncpy(pTransferPackage->szRoleName, pRole->m_szName, sizeof(pTransferPackage->szRoleName));
    pTransferPackage->szRoleName[sizeof(pTransferPackage->szRoleName) - 1] = '\0';

    strncpy(pTransferPackage->szAccount, pRole->m_pszAccount, sizeof(pTransferPackage->szAccount));
    pTransferPackage->szAccount[sizeof(pTransferPackage->szAccount) - 1] = '\0';

    pTransferPackage->bChargeFlag           = pRequest->bChargeFlag;
    pTransferPackage->ExtPointInfo          = pRequest->ExtPointInfo;
    pTransferPackage->bExtPointLock         = pRequest->bExtPointLock;
    pTransferPackage->nLastExtPointIndex    = pRequest->nLastExtPointIndex;
    pTransferPackage->nLastExtPointValue    = pRequest->nLastExtPointValue;
    pTransferPackage->nEndTimeOfFee         = pRequest->nEndTimeOfFee;
    pTransferPackage->nCoin                 = pRequest->nCoin;
    pTransferPackage->dwSystemTeamID        = pRole->m_dwSystemTeamID;
    pTransferPackage->dwTeamID              = pRole->m_dwTeamID;
    pTransferPackage->BaseInfo              = pRequest->RoleBaseInfo;
    pTransferPackage->nBattleFieldSide      = pRequest->nBattleFieldSide;
    pTransferPackage->dwTongID              = pRole->m_dwTongID;

	pTransferPackage->dwKinMasterId			= 0;
	if(pRole->m_dwTongID > 0)
	{
		KTong* pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
		LOG_PROCESS_ERROR(pTong);
		pTransferPackage->dwKinMasterId = pTong->m_dwMaster;
	}
    
    //pTransferPackage->byFarmerLimit         = 
    //    (BYTE)g_pGameCenter->m_AntiFarmerManager.CheckFarmerLimit(pRole->m_pszAccount);

    bRetCode = Send(pcDestMapCopy->m_nConnIndex, piPackage);
    LOG_PROCESS_ERROR(bRetCode);

EXIT0:
    SAFE_RELEASE(piPackage);
	return;
}

void KGameServer::OnTransferPlayerRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode    = false;
	S2R_TRANSFER_PLAYER_RESPOND*    pRespond    = (S2R_TRANSFER_PLAYER_RESPOND*)pbyData;
	KRole*                          pRole       = NULL;
    const KMapCopy*                 pcMapCopy   = NULL;
    KROLE_STATE                     eRoleState  = rsInvalid; 

	QLogPrintf(LOG_INFO, "KGameServer::OnTransferPlayerRespond nConnIndex=%d dwPlayerID=%d \n", nConnIndex, pRespond->dwPlayerID);

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

    eRoleState = pRole->GetRoleState(); 

	if (pRespond->bSucceed)
	{
		switch (eRoleState)
		{
		case rsCG_WaitForGuid:
			pRole->SetRoleState(rsCG_WaitForEnter_Leave);
			break;

		case rsCG_WaitForDestTimeout:
            // 等会儿,GS会发来玩家的LeaveGS消息,然后再调pRole->OnLeaveGS(nConnIndex);
			break;

		default:
			assert(false);
		}
	}
	else
	{
        // 跨服被拒绝后,要把目标地图的人数减下来(之前 SearchMap的时候加的)
        bRetCode = g_pGameCenter->m_MapManager.DecreaseMapCopyPlayerCount(pRole->m_dwDestMapID, pRole->m_nDestMapCopyIndex);
        LOG_PROCESS_ERROR(bRetCode);

        pRole->m_nDestConnIndex     = 0;
        pRole->m_dwDestMapID        = 0;
        pRole->m_nDestMapCopyIndex  = 0;

		switch (eRoleState)
		{
		case rsCG_WaitForGuid:
			pRole->SetRoleState(rsOnline);
			break;

		case rsCG_WaitForDestTimeout:
			pRole->OnLeaveGS(nConnIndex);
			break;

		default:
			assert(false);
		}
	}

    DoTransferPlayerRespond(
        pRole->m_nConnIndex, pRole->m_dwPlayerID, pRespond->bSucceed, 
        pRespond->dwAddress, pRespond->wPort, &pRespond->Guid
    );

EXIT0:
	return;
}


void KGameServer::OnConfirmPlayerLoginRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                                bRetCode        = false;
	S2R_CONFIRM_PLAYER_LOGIN_REQUEST*   pRequest        = (S2R_CONFIRM_PLAYER_LOGIN_REQUEST*)pbyData;
	KRole*                              pRole           = NULL;
    KMapInfo*                           pMapInfo        = NULL;
    KMapCopy*                           pMapCopy        = NULL;

	QLogPrintf(LOG_INFO, "KGameServer::OnConfirmPlayerLoginRequest nConnIndex=%d dwPlayerID=%d \n", nConnIndex, pRequest->dwPlayerID);
    
    pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
    LOG_PROCESS_ERROR(pRole);

	switch (pRole->GetRoleState())
	{
	case rsLG_WaitForLogin:
		LOG_PROCESS_ERROR(pRole->m_nDestConnIndex == nConnIndex);

        pRole->m_nConnIndex         = nConnIndex;
        pRole->m_dwMapID            = pRole->m_dwDestMapID;
        pRole->m_nMapCopyIndex      = pRole->m_nDestMapCopyIndex;

        pRole->m_nDestConnIndex     = 0;
        pRole->m_dwDestMapID        = 0;
        pRole->m_nDestMapCopyIndex  = 0;

        //g_pGameCenter->m_GameServer.DoSyncFellowshipMapID(pRole->m_dwPlayerID, pRole->m_dwMapID);

		//g_pGameCenter->m_ApexProxy.DoRoleLogin(pRole->m_dwPlayerID);

        g_pGameCenter->m_TongManager.OnPlayerEnterGame(pRole);

		g_pGameCenter->m_MailManager.PostRequestLoadMailCount(pRole->m_dwPlayerID);

		pRole->SetRoleState(rsOnline);

	    DoConfirmPlayerLoginRespond(nConnIndex, pRequest->dwPlayerID, true);

        pRole->CallLoginScript();

		// temp added by liyun 
		g_pGameCenter->m_MiscDB.DoLoadRelationData(pRole->m_dwPlayerID);
		g_pGameCenter->m_RelationManager.NotifyOnline(pRole->m_dwPlayerID, TRUE);
        break;

	case rsCG_WaitForDestEnter:
		LOG_PROCESS_ERROR(pRole->m_nDestConnIndex == nConnIndex);

        pRole->m_dwMapID            = pRole->m_dwDestMapID;
        pRole->m_nMapCopyIndex      = pRole->m_nDestMapCopyIndex;
        pRole->m_nConnIndex         = pRole->m_nDestConnIndex;

        //g_pGameCenter->m_ApexProxy.DoEntryServer(pRole->m_dwPlayerID, pRole->m_nConnIndex);

        pRole->m_nDestConnIndex     = 0;
        pRole->m_dwDestMapID        = 0;
        pRole->m_nDestMapCopyIndex  = 0;
        
        //g_pGameCenter->m_GameServer.DoSyncFellowshipMapID(pRole->m_dwPlayerID, pRole->m_dwMapID);
        pRole->UpdateMapCopyOwnerForTeamLeader();

		pRole->SetRoleState(rsOnline);

		DoConfirmPlayerLoginRespond(nConnIndex, pRequest->dwPlayerID, true);
		break;

	case rsCG_WaitForEnter_Leave:
		LOG_PROCESS_ERROR(pRole->m_nDestConnIndex == nConnIndex);
		pRole->SetRoleState(rsCG_WaitForSrcLeave);
		break;

	case rsBZ_WaitForLogin:
		LOG_PROCESS_ERROR(pRole->m_nDestConnIndex == nConnIndex);

		pRole->m_dwMapID            = pRole->m_dwDestMapID;
		pRole->m_nMapCopyIndex      = pRole->m_nDestMapCopyIndex;
		pRole->m_nConnIndex         = pRole->m_nDestConnIndex;

		//g_pGameCenter->m_ApexProxy.DoEntryServer(pRole->m_dwPlayerID, pRole->m_nConnIndex);

		pRole->m_nDestConnIndex     = 0;
		pRole->m_dwDestMapID        = 0;
		pRole->m_nDestMapCopyIndex  = 0;

		//g_pGameCenter->m_GameServer.DoSyncFellowshipMapID(pRole->m_dwPlayerID, pRole->m_dwMapID);
		pRole->UpdateMapCopyOwnerForTeamLeader();

		pRole->SetRoleState(rsOnline);

		DoConfirmPlayerLoginRespond(nConnIndex, pRequest->dwPlayerID, true);

		g_BZone.DoLoadRoleDataRequest(pRequest->dwPlayerID);
		break;

	default:
        assert(false);
	}
    
EXIT0:
	return;
}

void KGameServer::OnPlayerLeaveGS(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_PLAYER_LEAVE_GS*    pPlayerLeave    = (S2R_PLAYER_LEAVE_GS*)pbyData;
	KRole*                  pRole           = NULL;

	QLogPrintf(LOG_INFO, "KGameServer::OnPlayerLeaveGS nConnIndex=%d dwPlayerID=%d\n", nConnIndex, pPlayerLeave->dwPlayerID);

    pRole = g_pGameCenter->m_RoleManager.GetRole(pPlayerLeave->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

    pRole->OnLeaveGS(nConnIndex);

EXIT0:
	return;
}

BOOL KGameServer::KTALK_TONG_TRAVERSE::operator () (KTongMember* pMember)
{
    KRole* pReceiver = g_pGameCenter->m_RoleManager.GetRole(pMember->dwID);
    if (!pReceiver)
        return true;

    pDownMsg->dwReceiverId = pReceiver->m_dwPlayerID;
    pGameServer->Send(pReceiver->m_nConnIndex, piSendBuff);

    return true;
}

void KGameServer::OnChatMessage(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    BOOL                     bRetCode           = FALSE;
    S2R_CHAT_MESSAGE*        pUpMsg             = NULL;
    size_t                   uTalkDataSize      = 0;
    IMemBlock*              piSendBuffer        = NULL;
    R2S_CHAT_MESSAGE*        pDownMsg           = NULL;


    pUpMsg = (S2R_CHAT_MESSAGE*)pbyData;

	QLogPrintf(LOG_INFO, "KGameServer::OnChatMessage nConnIndex=%d byMsgType=%d\n", nConnIndex, pUpMsg->byMsgType);

    LOG_PROCESS_ERROR(pUpMsg);

    pUpMsg->szSender[sizeof(pUpMsg->szSender) - 1] = '\0';
    pUpMsg->szReceiver[sizeof(pUpMsg->szReceiver) - 1] = '\0';

    uTalkDataSize = uDataLen - sizeof(S2R_CHAT_MESSAGE);

    piSendBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_CHAT_MESSAGE) + uTalkDataSize));
    LOG_PROCESS_ERROR(piSendBuffer);

    pDownMsg = (R2S_CHAT_MESSAGE*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pDownMsg);
    
    pDownMsg->wProtocolID      = r2s_chat_message;
    pDownMsg->byMsgType        = pUpMsg->byMsgType;
	pDownMsg->dwSenderId       = pUpMsg->dwSenderId;
    pDownMsg->dwReceiverId     = pUpMsg->dwReceiverId;
	pDownMsg->byShownType      = pUpMsg->byShownType;

    strcpy(pDownMsg->szSender, pUpMsg->szSender);
    strcpy(pDownMsg->szReceiver, pUpMsg->szReceiver);

    memcpy(pDownMsg->byTalkData, pUpMsg->byTalkData, uTalkDataSize);

    switch(pUpMsg->byMsgType)
    {
    case trWhisper:
    case trFriends:
        {
            KRole* pReceiver = g_pGameCenter->m_RoleManager.GetRole(pUpMsg->szReceiver);
            KRole* pTalker = g_pGameCenter->m_RoleManager.GetRole(pUpMsg->dwSenderId);
            LOG_PROCESS_ERROR(pTalker);

            // 密聊时,上行协议的dwReceiverId是无效的,需要根据目标的名字查找到目标玩家的ID
            if (pReceiver == NULL)
            {
                if (pUpMsg->byMsgType != trFriends)
                    DoPlayerChatError(nConnIndex, pUpMsg->dwSenderId, TALK_ERROR_E_PLAYER_NOT_FOUND);

                goto EXIT0;
            }

            if (pReceiver->GetRoleState() != rsOnline)
            {
                if (pUpMsg->byMsgType != trFriends)
                {
                    DoPlayerChatError(nConnIndex, pUpMsg->dwSenderId, TALK_ERROR_E_PLAYER_OFFLINE);
                    //bRetCode = g_pGameCenter->m_MailManager.PostRequestSendMail(
                    //    0, pReceiver->m_dwPlayerID, 0, eMailType_PlayerMessage,
                    //    pTalker->m_szName, "Player Message Mail", 
                    //    0,0,0,
                    //    pDownMsg->byTalkData, uTalkDataSize,
                    //    NULL, 0, 0
                    //);
                }

                goto EXIT0;
            }
		
            pDownMsg->dwReceiverId = pReceiver->m_dwPlayerID;
			//黑名单处理
			if(g_pGameCenter->m_RelationManager.HasRelation(pDownMsg->dwReceiverId, pDownMsg->dwSenderId , emKPLAYERRELATION_TYPE_BLACKLIST))
			{
				ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
				cSafeScript->CallTableFunction("Relation", "NotifyAddedBlack_GC", 0, "ds", pDownMsg->dwSenderId, pReceiver->GetPlayerName());
			}
			else
			{
				Send(pReceiver->m_nConnIndex, piSendBuffer);
				DoPlayerChatWhisperSuccessRespond(nConnIndex, pUpMsg->dwSenderId, pReceiver->m_dwPlayerID);

				pDownMsg->dwReceiverId = pUpMsg->dwSenderId;
				strcpy(pDownMsg->szReceiver, pUpMsg->szSender);
				Send(pTalker->m_nConnIndex, piSendBuffer); // talk to self 
				
			}
        }
        break;

    case trTeam:
    case trRaid:
        {
            // 队聊时,要取对应的队伍ID
            KRole* pTalker = g_pGameCenter->m_RoleManager.GetRole(pUpMsg->dwSenderId);
            LOG_PROCESS_ERROR(pTalker);

            //PROCESS_ERROR(pTalker->m_dwTeamID == pUpMsg->dwReceiverId);

            pDownMsg->dwReceiverId = pTalker->m_dwTeamID;

            Broadcast(piSendBuffer);
        }
        break;

    case trBattleField:
        {
            KRole* pTalker = g_pGameCenter->m_RoleManager.GetRole(pUpMsg->dwSenderId);
            LOG_PROCESS_ERROR(pTalker);

            PROCESS_ERROR(pTalker->m_dwSystemTeamID == pUpMsg->dwReceiverId);

            pDownMsg->dwReceiverId = pTalker->m_dwSystemTeamID;

            Broadcast(piSendBuffer);
        }
        break;

    case trTong:
        {
            KRole*              pTalker = NULL;
            KTong*              pTong   = NULL;
            KTALK_TONG_TRAVERSE TalkTongTraverse;

			if (pUpMsg->byShownType == emQNOTICE_BOARD)
			{
				// 家族公告senderId为kinId
				pTong = g_pGameCenter->m_TongManager.GetTong(pUpMsg->dwSenderId);
				LOG_PROCESS_ERROR(pTong);
			}
			else
			{
				pTalker = g_pGameCenter->m_RoleManager.GetRole(pUpMsg->dwSenderId);
				LOG_PROCESS_ERROR(pTalker);

				if (!pTalker->m_dwTongID)
				{
					DoPlayerChatError(nConnIndex, pUpMsg->dwSenderId, TALK_ERROR_E_NOT_IN_TONG);
					goto EXIT0;
				}

				pTong = g_pGameCenter->m_TongManager.GetTong(pTalker->m_dwTongID);
				LOG_PROCESS_ERROR(pTong);

				if (!pTong->CanSpeak(pTalker->m_dwPlayerID))
				{
					DoPlayerChatError(nConnIndex, pUpMsg->dwSenderId, TALK_ERROR_E_TONG_CAN_NOT_SPEAK);
					goto EXIT0;
				}
			}

            TalkTongTraverse.pDownMsg    = pDownMsg;
            TalkTongTraverse.piSendBuff  = piSendBuffer;
            TalkTongTraverse.pGameServer = this;
            pTong->TraverseMember(TalkTongTraverse);
        }
        break;

    case trGlobalSys:
    case trWorld:
    case trForce:
    case trCamp:
    case trNpcSayToCamp:
        Broadcast(piSendBuffer);
        break;

	case trFaction:
		{
			// 频道为门派
			KRole* pTalker = g_pGameCenter->m_RoleManager.GetRole(pUpMsg->dwSenderId);
			LOG_PROCESS_ERROR(pTalker);

			pDownMsg->dwReceiverId = pTalker->GetPrimaryFaction();

			Broadcast(piSendBuffer);
		}
		break;

    case trMentor:
        {
            //KRole*                      pReceiver           = NULL;
            //int                         nMentorCount        = 0;
            //int                         nApprenticeCount    = 0;
            //DWORD                       dwMentorList[MAX_MENTOR_NUM];
            //DWORD                       dwApprenticeList[MAX_APPRENTICE_NUM];
            //
            //typedef std::set<DWORD, std::less<DWORD>, KMemory::KAllocator<DWORD> > KSameGateSet;
            //KSameGateSet           SameGateSet;
            //KSameGateSet::iterator itEnd;

            //nApprenticeCount = g_pGameCenter->m_MentorCenter.GetApprenticeList(pUpMsg->dwSenderId, dwApprenticeList);
            //for (int i = 0; i < nApprenticeCount; i++)
            //{
            //    SameGateSet.insert(dwApprenticeList[i]);
            //}

            //nMentorCount = g_pGameCenter->m_MentorCenter.GetMentorList(pUpMsg->dwSenderId, dwMentorList);
            //for (int i = 0; i < nMentorCount; i++)
            //{
            //    nApprenticeCount = g_pGameCenter->m_MentorCenter.GetApprenticeList(dwMentorList[i], dwApprenticeList);
            //    for (int j = 0; j < nApprenticeCount; j++)
            //    {
            //        SameGateSet.insert(dwApprenticeList[j]);
            //    }

            //    SameGateSet.insert(dwMentorList[i]);
            //}

            //SameGateSet.insert(pUpMsg->dwSenderId);

            //itEnd = SameGateSet.end();
            //for (KSameGateSet::iterator it = SameGateSet.begin(); it != itEnd; ++it)
            //{
            //    pReceiver = g_pGameCenter->m_RoleManager.GetRole(*it);

            //    if (pReceiver == NULL)
            //        continue;

            //    if (pReceiver->GetRoleState() != rsOnline)
            //        continue;

            //    pDownMsg->dwReceiverId = pReceiver->m_dwPlayerID;
            //    Send(pReceiver->m_nConnIndex, piSendBuffer);
            //}
        }
        break;

	case trGmMessage:
		{
			ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

			LPCSTR pszPrefix = GM_CMD_PREFIX_GAMECENTER;
			SIZE_T nPrefixLen = strlen(pszPrefix);

			if (QStrCmpLen((CHAR*)pUpMsg->byTalkData, pszPrefix, (INT)nPrefixLen))
			{
				// 在GS已经判断过GM权限
				cSafeScript->DoBuffer((CHAR*)pUpMsg->byTalkData + nPrefixLen);
			}
			else
			{
				QLogPrintf(LOG_WARNING, "Malformed gm command %s", pUpMsg->byTalkData);
			}
		}
		break;

    default:
		QLogPrintf(LOG_ERR, "..pUpMsg->byMsgType=%d", pUpMsg->byMsgType);
        LOG_PROCESS_ERROR(!"Unexcepted message type. ");
    }

EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return ;
}

void KGameServer::OnPlayerChatError(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    BOOL                    bRetCode    = false;
    S2R_PLAYER_CHAT_ERROR*  pPak        = NULL;
    KRole*                  pRole       = NULL;

    pPak = (S2R_PLAYER_CHAT_ERROR*)pbyData;
    LOG_PROCESS_ERROR(pPak);

	QLogPrintf(LOG_INFO, "KGameServer::OnPlayerChatError nConnIndex=%d dwReceiverId=%d nErrorCode=%d\n", nConnIndex, pPak->dwReceiverId, pPak->nErrorCode);

    LOG_PROCESS_ERROR(pPak->dwReceiverId != KD_BAD_ID);
    pRole = g_pGameCenter->m_RoleManager.GetRole(pPak->dwReceiverId);
    LOG_PROCESS_ERROR(pRole);

    bRetCode = DoPlayerChatError(pRole->m_nConnIndex, pPak->dwReceiverId, pPak->nErrorCode);
    PROCESS_ERROR(bRetCode);

EXIT0:
    return ;
}

void KGameServer::OnSaveScenePlayerList(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    BOOL                        bRetCode        = false;
    S2R_SAVE_SCENE_PLAYER_LIST* pRequest        = (S2R_SAVE_SCENE_PLAYER_LIST*)pbyData;
    KMapInfo*                   pMapInfo        = NULL;
    KMapCopy*                   pMapCopy        = NULL;

    LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_SAVE_SCENE_PLAYER_LIST) + sizeof(DWORD) * pRequest->nPlayerCount);
    
    pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pRequest->dwMapID);
    LOG_PROCESS_ERROR(pMapInfo && pMapInfo->m_nType == emtDungeon);
    
    pMapCopy = pMapInfo->GetMapCopy(pRequest->nMapCopyIndex);
    LOG_PROCESS_ERROR(pMapCopy);

    LOG_PROCESS_ERROR(pMapCopy->m_eState == eMapStateNormal);
    
    for (int i = 0; i < pRequest->nPlayerCount; i++)
    {
        pMapCopy->m_PlayerList.insert(pRequest->dwPlayerList[i]);
    }
    
	QLogPrintf(LOG_INFO, "KGameServer::OnSaveScenePlayerList nConnIndex=%d dwMapID=%d nMapCopyIndex=%d nPlayerCount=%d\n",
		nConnIndex, pRequest->dwMapID, pRequest->nMapCopyIndex, pRequest->nPlayerCount);

    pMapCopy->m_bIsChanged = true;

EXIT0:
    return;
}

void KGameServer::OnSaveSceneData(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    BOOL                    bRetCode        = false;
    S2R_SAVE_SCENE_DATA*    pData           = (S2R_SAVE_SCENE_DATA*)pbyData;
    IMemBlock*             piSceneData     = NULL;
    BYTE*                   pbySceneData    = NULL;
    size_t                  uSceneDataSize  = 0;
    KMapInfo*               pMapInfo        = NULL;
    KMapCopy*               pMapCopy        = NULL;

    LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_SAVE_SCENE_DATA) + pData->uDataLen);
    
    pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(pData->dwMapID);
    LOG_PROCESS_ERROR(pMapInfo);
    
    pMapCopy = pMapInfo->GetMapCopy(pData->nMapCopyIndex);
    LOG_PROCESS_ERROR(pMapCopy);

    LOG_PROCESS_ERROR(pMapCopy->m_eState == eMapStateNormal);

    LOG_PROCESS_ERROR(pData->uDataLen > 0);
    
    piSceneData = QCreateMemBlock((unsigned)pData->uDataLen);
    LOG_PROCESS_ERROR(piSceneData);

    pbySceneData = (BYTE*)piSceneData->GetData();
    LOG_PROCESS_ERROR(pbySceneData);

    memcpy(pbySceneData, pData->byData, pData->uDataLen);

    SAFE_RELEASE(pMapCopy->m_piSceneData);

    piSceneData->AddRef();
    pMapCopy->m_piSceneData = piSceneData;

    pMapCopy->m_bIsChanged = true;

	QLogPrintf(LOG_INFO, "KGameServer::OnSaveSceneData nConnIndex=%d dwMapID=%d nMapCopyIndex=%d \n", 
		nConnIndex, pData->dwMapID, pData->nMapCopyIndex);

EXIT0:
    SAFE_RELEASE(piSceneData);
    return;
}

void KGameServer::OnPlayerEnterSceneNotify(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    S2R_PLAYER_ENTER_SCENE_NOTIFY*   pNotify   = (S2R_PLAYER_ENTER_SCENE_NOTIFY*)pbyData;
    
	QLogPrintf(LOG_INFO, "KGameServer::OnPlayerEnterSceneNotify nConnIndex=%d dwPlayerID=%d dwMapID=%d nMapCopyIndex=%d \n",
		nConnIndex, pNotify->dwPlayerID, pNotify->dwMapID, pNotify->nMapCopyIndex);

    g_pGameCenter->m_MapManager.OnPlayerEnterMapCopy(
        pNotify->dwPlayerID, pNotify->dwMapID, pNotify->nMapCopyIndex
    );

    return;
}

void KGameServer::OnResetMapCopyNotify(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_RESET_MAP_COPY_REQUEST*   pNotify   = (S2R_RESET_MAP_COPY_REQUEST*)pbyData;

	QLogPrintf(LOG_INFO, "KGameServer::OnResetMapCopyNotify nConnIndex=%d dwPlayerID=%d dwMapID=%d nMapCopyIndex=%d \n",
		nConnIndex, pNotify->dwPlayerID, pNotify->dwMapID, pNotify->nMapCopyIndex);

	g_pGameCenter->m_MapManager.ResetMap(
		pNotify->dwPlayerID, pNotify->dwMapID, pNotify->nMapCopyIndex
		);

	return;
}

void KGameServer::OnLoadRoleDataRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    S2R_LOAD_ROLE_DATA_REQUEST* pLoadRole = (S2R_LOAD_ROLE_DATA_REQUEST*)pbyData;

	QLogPrintf(LOG_INFO, "KGameServer::OnLoadRoleDataRequest nConnIndex=%d dwPlayerID=%d\n", nConnIndex, pLoadRole->dwPlayerID);
    
    g_pGameCenter->m_RoleDB.DoLoadRoleData(nConnIndex, pLoadRole->dwPlayerID);

    return;
}

void KGameServer::OnLoadDelayRoleDataRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_LOAD_DELAY_ROLE_DATA_REQUEST* pLoadRole = (S2R_LOAD_DELAY_ROLE_DATA_REQUEST*)pbyData;
	if(!pLoadRole->dwPlayerID)
	{
		KRole* pRole = g_pGameCenter->m_RoleManager.GetRole(pLoadRole->szRoleName);
		if (pRole)
		{
			pLoadRole->dwPlayerID = pRole->GetId();
		}
	}
	LOG_PROCESS_ERROR(pLoadRole->dwPlayerID > 0);

	g_pGameCenter->m_RoleDB.DoDelayLoadRoleData(nConnIndex, pLoadRole->byModuleId, pLoadRole->dwPlayerID, pLoadRole->dwSrcId);

EXIT0:
	return;
}

void KGameServer::OnSyncRoleData(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    BOOL                bResult         = false;
    S2R_SYNC_ROLE_DATA* pSyncData       = (S2R_SYNC_ROLE_DATA*)pbyData;
    KGS_INFO*           pGSInfo         = NULL; 
    BYTE*               pbyOffset       = NULL;
    size_t              uRoleDataLen    = uDataLen - sizeof(S2R_SYNC_ROLE_DATA);

    pGSInfo = GetGSInfoByConnectionIndex(nConnIndex);
    LOG_PROCESS_ERROR(pGSInfo);

    LOG_PROCESS_ERROR(pGSInfo->uRoleDataOffset == pSyncData->uOffset);
    LOG_PROCESS_ERROR(sizeof(pGSInfo->byRoleDataBuffer) - pGSInfo->uRoleDataOffset >= uRoleDataLen);
    LOG_PROCESS_ERROR(pSyncData->uOffset == 0 || pSyncData->dwRoleID == pGSInfo->dwRoleID);

    pbyOffset = pGSInfo->byRoleDataBuffer + pGSInfo->uRoleDataOffset;
    
    memcpy(pbyOffset, pSyncData->byData, uRoleDataLen);

    pGSInfo->uRoleDataOffset += uRoleDataLen;
    pGSInfo->dwRoleID         = pSyncData->dwRoleID;

    bResult = true;
EXIT0:
    if(!bResult && pGSInfo)
    {
        pGSInfo->dwRoleID        = KD_BAD_ID;
        pGSInfo->uRoleDataOffset = 0;
    }
    return;
}

void KGameServer::OnSaveRoleData(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    BOOL                bResult     = false;
    S2R_SAVE_ROLE_DATA* pSaveData   = (S2R_SAVE_ROLE_DATA*)pbyData;
    KGS_INFO*           pGSInfo     = NULL;
    KRole*              pRole       = NULL;
    IMemBlock*         piRoleData  = NULL;
    void*               pvRoleData  = NULL;

    pGSInfo = GetGSInfoByConnectionIndex(nConnIndex);
    LOG_PROCESS_ERROR(pGSInfo);

    LOG_PROCESS_ERROR(pSaveData->uRoleDataLen == pGSInfo->uRoleDataOffset);
    LOG_PROCESS_ERROR(pSaveData->dwRoleID == pGSInfo->dwRoleID);
    LOG_PROCESS_ERROR(pSaveData->uRoleDataLen > 0);

    pRole = g_pGameCenter->m_RoleManager.GetRole(pSaveData->dwRoleID);
    LOG_PROCESS_ERROR(pRole);

    pRole->m_nLastSaveTime = g_pGameCenter->m_nTimeNow;
	pRole->m_byLevel = pSaveData->BaseInfo.byLevel;
	pRole->m_nFightScore = *((INT*)pSaveData->BaseInfo.byReserved);

    piRoleData = QCreateMemBlock((unsigned)pSaveData->uRoleDataLen);
    LOG_PROCESS_ERROR(piRoleData);
    
    pvRoleData = piRoleData->GetData();
    LOG_PROCESS_ERROR(pvRoleData);

    memcpy(pvRoleData, pGSInfo->byRoleDataBuffer, pSaveData->uRoleDataLen);

    g_pGameCenter->m_RoleDB.DoSaveRoleData(
        nConnIndex, pSaveData->dwRoleID, pSaveData->BaseInfo, piRoleData, pSaveData->nUserValue
    );

	g_pGameCenter->m_RoleManager.OnSaveRoleData(pSaveData->dwRoleID, pSaveData->BaseInfo);

    DoSaveRoleDataRespond(nConnIndex, pSaveData->dwRoleID, true, pSaveData->uRoleDataLen, pSaveData->nUserValue, pSaveData->nUserValue2);

    bResult = true;
EXIT0:
    if (pGSInfo)
    {
        pGSInfo->uRoleDataOffset = 0;
        pGSInfo->dwRoleID        = KD_BAD_ID;
    }
    SAFE_RELEASE(piRoleData);
    return;
}

void KGameServer::OnCreateDelayRoleData(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                bResult     = false;
	S2R_CREATE_DELAY_ROLE_DATA* pSaveData   = (S2R_CREATE_DELAY_ROLE_DATA*)pbyData;
	KRole*              pRole       = NULL;
	IMemBlock*         piRoleData  = NULL;
	void*               pvRoleData  = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pSaveData->dwRoleID);
	LOG_PROCESS_ERROR(pRole);

	piRoleData = QCreateMemBlock(sizeof(KDelayRoleData));
	LOG_PROCESS_ERROR(piRoleData);

	pvRoleData = piRoleData->GetData();
	LOG_PROCESS_ERROR(pvRoleData);

	memcpy(pvRoleData, &(pSaveData->DelayRoleData), sizeof(KDelayRoleData));

	g_pGameCenter->m_RoleDB.DoCreateDelayRoleData(pSaveData->dwRoleID, piRoleData);
	bResult = true;

EXIT0:
	SAFE_RELEASE(piRoleData);
	return;
}

void KGameServer::OnSaveDelayRoleData(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                bResult     = false;
	S2R_SAVE_DELAY_ROLE_DATA* pSaveData   = (S2R_SAVE_DELAY_ROLE_DATA*)pbyData;
	KRole*              pRole       = NULL;
	IMemBlock*         piRoleData  = NULL;
	void*               pvRoleData  = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pSaveData->dwRoleID);
	LOG_PROCESS_ERROR(pRole);

	piRoleData = QCreateMemBlock(sizeof(KDelayRoleData));
	LOG_PROCESS_ERROR(piRoleData);

	pvRoleData = piRoleData->GetData();
	LOG_PROCESS_ERROR(pvRoleData);

	memcpy(pvRoleData, &(pSaveData->DelayRoleData), sizeof(KDelayRoleData));

	g_pGameCenter->m_RoleDB.DoSaveDelayRoleData(pSaveData->dwRoleID, piRoleData);
	bResult = true;

EXIT0:
	SAFE_RELEASE(piRoleData);
	return;
}


//
//void KGameServer::OnChangeRoleLevelRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
//{
//    S2R_CHANGE_ROLE_LEVEL_REQUEST*  pChangeRoleLevel = (S2R_CHANGE_ROLE_LEVEL_REQUEST*)pbyData;
//    KRole*                          pRole            = NULL;
//
//    pRole = g_pGameCenter->m_RoleManager.GetRole(pChangeRoleLevel->dwPlayerID);
//    LOG_PROCESS_ERROR(pRole);
//
//    pRole->m_byLevel = pChangeRoleLevel->byLevel;
//
//    DoSyncFellowshipPlayerLevel(pRole->m_dwPlayerID, pRole->m_byLevel, true);
//
//EXIT0:
//    return;
//}

void KGameServer::OnUpdateStatDataRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    S2R_UPDATE_STAT_DATA_REQUEST*   pRequest = (S2R_UPDATE_STAT_DATA_REQUEST*)pbyData;

    LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_UPDATE_STAT_DATA_REQUEST) + sizeof(KSTAT_DATA_MODIFY) * pRequest->nCount);

    g_pGameCenter->m_StatDataManager.OnGSUpdate(pRequest->Values, pRequest->nCount);

EXIT0:
    return;
}

void KGameServer::OnQueryStatIDRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    S2R_QUERY_STAT_ID_REQUEST*   pRequest = (S2R_QUERY_STAT_ID_REQUEST*)pbyData;
    int                          nNameID  = 0;

    nNameID = g_pGameCenter->m_StatDataManager.GetNameID(pRequest->szName);
    LOG_PROCESS_ERROR(nNameID != 0);

    DoQueryStatIDRespond(nConnIndex, pRequest->szName, nNameID);

EXIT0:
    return;
}

//void KGameServer::OnChangeExtPointRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
//{
//    BOOL                            bRetCode = false;
//    S2R_CHANGE_EXT_POINT_REQUEST*   pRequest = (S2R_CHANGE_EXT_POINT_REQUEST*)pbyData;
//    KRole*                          pRole    = NULL;
//
//    pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwPlayerID);
//    LOG_PROCESS_ERROR(pRole);
//
//    bRetCode = g_pGameCenter->m_Gateway.DoChangeExtPointRequest(
//        pRole->m_pszAccount, pRole->m_dwPlayerID, pRequest->wExtPointIndex, pRequest->wChangeValue
//    );
//    LOG_PROCESS_ERROR(bRetCode);
//
//EXIT0:
//    return;
//}

void KGameServer::OnMapCopyKeepPlayer(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
    S2R_MAP_COPY_KEEP_PLAYER* pKeepPlayer = (S2R_MAP_COPY_KEEP_PLAYER*)pbyData;
    KMapCopy*                 pMapCopy    = NULL;

    pMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pKeepPlayer->dwMapID, pKeepPlayer->nCopyIndex);
    LOG_PROCESS_ERROR(pMapCopy);

    pMapCopy->RegisterKeepPlayer(pKeepPlayer->dwPlayerID);

EXIT0:
    return;
}

void KGameServer::OnRemoteLuaCall(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL bRetCode  = FALSE;
	S2R_REMOTE_LUA_CALL* pLuaCall = (S2R_REMOTE_LUA_CALL*)pbyData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	LOG_PROCESS_ERROR(uDataLen == sizeof(S2R_REMOTE_LUA_CALL) + pLuaCall->uParamLen);

	bRetCode = cSafeScript->LoadBuffer2Value(pLuaCall->byParam, pLuaCall->uParamLen);
	LOG_PROCESS_ERROR(bRetCode);

	QLogPrintf(LOG_INFO, "KGameServer::OnRemoteLuaCall nConnIndex=%d byParam=%s\n", nConnIndex, pLuaCall->byParam);

	bRetCode = cSafeScript->CallTableFunction("GCEvent", "OnCenterExecute", 0,
		"v", cSafeScript.GetBeginTopIndex() + 1
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}



//----------------------------------支付相关-------------------------------------------
VOID KGameServer::OnPayOrderRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                    bRetCode			= false;
	S2R_PAY_ORDER_REQUEST*   pRequest			= (S2R_PAY_ORDER_REQUEST*)pbyData;
	PROCESS_ERROR(pRequest);

	g_pGameCenter->m_RoleDB.SendPlayerOrder(pRequest->dwRoleId);

EXIT0:
	return;
}
VOID KGameServer::OnPayOrderFinishRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                    bRetCode			= false;
	S2R_PAY_ORDER_FINISH_REQUEST*   pRequest	= (S2R_PAY_ORDER_FINISH_REQUEST*)pbyData;
	PROCESS_ERROR(pRequest);

	g_pGameCenter->m_RoleDB.DoOrderFromGS(pRequest->dwRoleId, pRequest->szOrderno);

EXIT0:
	return;
}


//AutoCode:-处理协议函数结束-

BOOL KGameServer::DoHandshakeRespond(int nConnIndex)
{
    BOOL                    bResult         = false;
	int                     nRetCode        = false;
	IMemBlock*             piPackage       = NULL;
    R2S_HANDSHAKE_RESPOND*  pRespond        = NULL;
    KGS_INFO*               pGSInfo         = NULL;

	QLogPrintf(LOG_INFO, "KGameServer::DoHandshakeRespond nConnIndex=%d \n", nConnIndex);

    pGSInfo = GetGSInfoByConnectionIndex(nConnIndex);
    LOG_PROCESS_ERROR(pGSInfo);
    
    piPackage = QCreateMemBlock((unsigned)sizeof(R2S_HANDSHAKE_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespond = (R2S_HANDSHAKE_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID        = r2s_handshake_respond;
    pRespond->nBaseTime          = g_pGameCenter->m_nBaseTime;
	pRespond->nWorldIndex        = pGSInfo->nWorldIndex;
    pRespond->nStartFrame        = g_pGameCenter->m_nWorkLoop;
	pRespond->nDevMode			= g_pGameCenter->m_bDevMode;

	nRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoQuitNotify(int nConnIndex)
{
    BOOL                    bResult         = false;
	int                     nRetCode        = false;
	IMemBlock*             piPackage       = NULL;
    R2S_QUIT_NOTIFY*        pNotify         = NULL;

    
    piPackage = QCreateMemBlock((unsigned)sizeof(R2S_QUIT_NOTIFY));
	LOG_PROCESS_ERROR(piPackage);

	pNotify = (R2S_QUIT_NOTIFY*)piPackage->GetData();
	LOG_PROCESS_ERROR(pNotify);

	QLogPrintf(LOG_INFO, "KGameServer::DoQuitNotify nConnIndex=%d nNothing=%d\n", nConnIndex, pNotify->nNothing);

	pNotify->wProtocolID        = r2s_quit_notify;
    pNotify->nNothing           = 0;

	nRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoPlayerLoginRequest(
    int nConnIndex, const KACCOUNT_LOGIN_INFO& crAccInfo, DWORD dwRoleID, const KRoleBaseInfo& crBaseInfo
)
{
    BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	IMemBlock*                 piPackage       = NULL;
    R2S_PLAYER_LOGIN_REQUEST*   pLoginRequest   = NULL;     
    KRole*                      pRole           = NULL;
    //int                         nBattleSide     = INVALID_BATTLE_SIDE;


    pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleID);
    LOG_PROCESS_ERROR(pRole);

    piPackage = QCreateMemBlock((unsigned)sizeof(R2S_PLAYER_LOGIN_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

    //nBattleSide = g_pGameCenter->m_BattleFieldManager.GetBattleFieldSide(
    //    pRole->m_dwDestMapID, pRole->m_nDestMapCopyIndex, pRole->m_dwPlayerID
    //);

	pLoginRequest = (R2S_PLAYER_LOGIN_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pLoginRequest);

	QLogPrintf(LOG_INFO, "KGameServer::DoQuitNotify nWorldIndex=%d dwRoleID=%d\n", nConnIndex, pLoginRequest->dwRoleID);

	pLoginRequest->wProtocolID          = r2s_player_login_request;
    pLoginRequest->dwRoleID             = pRole->m_dwPlayerID;

    strncpy(pLoginRequest->szRoleName, pRole->m_szName, sizeof(pLoginRequest->szRoleName));
    pLoginRequest->szRoleName[sizeof(pLoginRequest->szRoleName) - 1] = '\0';

    strncpy(pLoginRequest->szAccount, pRole->m_pszAccount, sizeof(pLoginRequest->szAccount));
    pLoginRequest->szAccount[sizeof(pLoginRequest->szAccount) - 1] = '\0';

	pLoginRequest->dwPacketIdentity     = (DWORD)crAccInfo.nIdentity;
    pLoginRequest->nChargeFlag          = crAccInfo.nChargeFlag;
	pLoginRequest->ExtPointInfo         = crAccInfo.ExtPointInfo;
    pLoginRequest->nEndTimeOfFee        = (time_t)crAccInfo.dwEndTimeOfFee;
    pLoginRequest->nCoin                = (int)crAccInfo.dwCoin;
    pLoginRequest->dwSystemTeamID       = pRole->m_dwSystemTeamID;
    pLoginRequest->dwTeamID             = pRole->m_dwTeamID;
    pLoginRequest->BaseInfo             = crBaseInfo;
    //pLoginRequest->nBattleSide          = nBattleSide;
    pLoginRequest->dwTongID             = pRole->m_dwTongID;
	if(pRole->m_dwTongID > 0)
	{
		KTong* pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
		LOG_PROCESS_ERROR(pTong);
		pLoginRequest->dwKinMasterId = pTong->m_dwMaster;
	}

    //pLoginRequest->byFarmerLimit        = 
    //    (BYTE)g_pGameCenter->m_AntiFarmerManager.CheckFarmerLimit(pRole->m_pszAccount);

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoConfirmPlayerLoginRespond(int nConnIndex, DWORD dwPlayerID, BOOL bPermit)
{
    BOOL                                bResult     = false;
    BOOL                                bRetCode    = false;
	IMemBlock*                         piPackage   = NULL;    
	R2S_CONFIRM_PLAYER_LOGIN_RESPOND*   pRespond    = NULL;

    
    piPackage = QCreateMemBlock((unsigned)sizeof(R2S_CONFIRM_PLAYER_LOGIN_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespond = (R2S_CONFIRM_PLAYER_LOGIN_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);
	QLogPrintf(LOG_INFO, "KGameServer::DoConfirmPlayerLoginRespond nConnIndex=%d dwPlayerID=%d bPermit=%d\n", nConnIndex, dwPlayerID, bPermit);

	pRespond->wProtocolID       = r2s_confirm_player_login_respond;
	pRespond->dwPlayerID        = dwPlayerID;
	pRespond->bPermit           = bPermit;

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoKickAccountNotify(DWORD dwPlayerID)
{
    BOOL                        bResult     = false;
	int                         nRetCode    = false;
	IMemBlock*                 piPackage   = NULL;
    R2S_KICK_ACCOUNT_NOTIFY*    pNotify     = NULL;

	piPackage = QCreateMemBlock((unsigned)sizeof(R2S_KICK_ACCOUNT_NOTIFY));
	PROCESS_ERROR(piPackage);

	pNotify = (R2S_KICK_ACCOUNT_NOTIFY*)piPackage->GetData();
	PROCESS_ERROR(pNotify);
	
	pNotify->wProtocolID        = r2s_kick_account_notify;
	pNotify->dwPlayerID         = dwPlayerID;

    Broadcast(piPackage);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoCreateMapNotify(int nConnIndex, DWORD dwMapID, int nMapCopyIndex)
{
    BOOL                    bResult         = false;
	int                     nRetCode        = false;
	IMemBlock*             piPackage       = NULL;
    R2S_CREATE_MAP_NOTIFY*  pCreateMap      = NULL;
    KMapInfo*               pMapInfo        = NULL;
    KMapCopy*               pMapCopy        = NULL;
    size_t                  uExtDataLen     = 0;
    BYTE*                   pbyExtData      = NULL;

    pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(dwMapID);
    LOG_PROCESS_ERROR(pMapInfo);

    pMapCopy = pMapInfo->GetMapCopy(nMapCopyIndex);
    LOG_PROCESS_ERROR(pMapCopy);

    if (pMapCopy->m_piSceneData)
    {
        pbyExtData  = (BYTE*)pMapCopy->m_piSceneData->GetData();
        LOG_PROCESS_ERROR(pbyExtData);
        uExtDataLen = pMapCopy->m_piSceneData->GetSize();
    }
    
    piPackage = QCreateMemBlock((unsigned)(sizeof(R2S_CREATE_MAP_NOTIFY) + uExtDataLen));
	LOG_PROCESS_ERROR(piPackage);

	pCreateMap = (R2S_CREATE_MAP_NOTIFY*)piPackage->GetData();
	LOG_PROCESS_ERROR(pCreateMap);

	pCreateMap->wProtocolID         = r2s_create_map_notify;
	pCreateMap->dwMapID             = dwMapID;
	pCreateMap->nMapCopyIndex       = nMapCopyIndex;
    pCreateMap->uDataLen            = uExtDataLen;

    if (uExtDataLen > 0)
    {
        memcpy(pCreateMap->byData, pbyExtData, uExtDataLen);
    }

	nRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(nRetCode);

    QLogPrintf(
        LOG_INFO, "Append map %s(%.2u, %d) to GS %d\n", 
        pMapInfo->m_szName, dwMapID, nMapCopyIndex, nConnIndex
    );

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoLoadMapRespond(int nConnIndex, DWORD dwMapID, int nMapCopyIndex, int nCode, int nParam1)
{
	BOOL                    bResult         = false;
	int                     nRetCode        = false;
	IMemBlock*             piPackage       = NULL;
	R2S_LOAD_MAP_RESPOND*  pRespond      = NULL;
	KMapInfo*               pMapInfo        = NULL;

	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(dwMapID);
	if(!pMapInfo)
	{
		printf("Can't Find MapInfo!!!!!!!!!!!!!!! :%d\n", dwMapID);
	}
	LOG_PROCESS_ERROR(pMapInfo);

	piPackage = QCreateMemBlock((unsigned)sizeof(R2S_LOAD_MAP_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespond = (R2S_LOAD_MAP_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID         = r2s_load_map_respond;
	pRespond->dwMapID             = dwMapID;
	pRespond->nMapCopyIndex       = nMapCopyIndex;
	pRespond->nCode				= nCode;
	pRespond->nParam1			= nParam1;

	nRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(nRetCode);

	//QLogPrintf(
	//	LOG_INFO, "Load map %s(%.2u, %d) GS %d %s\n", 
	//	pMapInfo->m_szName, dwMapID, nMapCopyIndex, nConnIndex,
	//	nCode == smrcSuccess ? "ok" : "failed"
	//	);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoFinishCreateMapNotify(int nConnIndex, DWORD dwPlayerID)
{
    BOOL                            bResult     = false;
    BOOL                            bRetCode    = false;
    IMemBlock*                     piPackage   = NULL;
    R2S_FINISH_CREATE_MAP_NOTIFY*   pPackage    = NULL;
    
    piPackage = QCreateMemBlock(sizeof(R2S_FINISH_CREATE_MAP_NOTIFY));
    LOG_PROCESS_ERROR(piPackage);

    pPackage = (R2S_FINISH_CREATE_MAP_NOTIFY*)piPackage->GetData();
    LOG_PROCESS_ERROR(pPackage);

    pPackage->wProtocolID   = r2s_finish_create_map_notify;
    pPackage->dwPlayerID    = dwPlayerID;

    bRetCode = Send(nConnIndex, piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoDeleteMapNotify(int nConnIndex, DWORD dwMapID, int nMapCopyIndex)
{
    BOOL                    bResult     = false;
	int                     nRetCode    = false;
    KMapInfo*               pMapInfo    = NULL;
	IMemBlock*             piPackage   = NULL;
    R2S_DELETE_MAP_NOTIFY*  pDeleteMap  = NULL;

    pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(dwMapID);
    LOG_PROCESS_ERROR(pMapInfo);

    QLogPrintf(LOG_INFO, "Delete map copy %s(%u, %d)\n", pMapInfo->m_szName, dwMapID, nMapCopyIndex);

	piPackage = QCreateMemBlock(sizeof(R2S_DELETE_MAP_NOTIFY));
	PROCESS_ERROR(piPackage);

	pDeleteMap = (R2S_DELETE_MAP_NOTIFY*)piPackage->GetData();
	PROCESS_ERROR(pDeleteMap);

	pDeleteMap->wProtocolID     = r2s_delete_map_notify;
	pDeleteMap->dwMapID         = dwMapID;
	pDeleteMap->nMapCopyIndex   = nMapCopyIndex;

	nRetCode = Send(nConnIndex, piPackage);
	PROCESS_ERROR(nRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoSearchMapRespond(
    int nConnIndex, DWORD dwPlayerID, SWITCH_MAP_RESULT_CODE eResult, BOOL bSwitchServer,
    DWORD dwDestMapID, int nDestMapCopyIndex,
    int nX, int nY, int nZ
)
{
    BOOL                            bResult             = false;
	BOOL                            bRetCode            = false;
	IMemBlock*                     piPackage           = NULL;
    R2S_SEARCH_MAP_RESPOND*         pRespondPak         = NULL;
    //int                             nBattleFieldSide    = INVALID_BATTLE_SIDE;
	QLogPrintf(LOG_INFO,"..DoSearchMapRespond eResult=%d bSwitchServer=%d dwDestMapID=%d nDestMapCopyIndex=%d"
							,eResult,bSwitchServer,dwDestMapID,nDestMapCopyIndex);
	piPackage = QCreateMemBlock(sizeof(R2S_SEARCH_MAP_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespondPak = (R2S_SEARCH_MAP_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespondPak);

    //nBattleFieldSide = g_pGameCenter->m_BattleFieldManager.GetBattleFieldSide(
    //    dwDestMapID, nDestMapCopyIndex, dwPlayerID
    //);

	pRespondPak->wProtocolID        = r2s_search_map_respond;
	pRespondPak->dwPlayerID         = dwPlayerID;
	pRespondPak->nResultCode        = eResult;
	pRespondPak->bSwitchServer      = bSwitchServer;
	pRespondPak->dwMapID            = dwDestMapID;
	pRespondPak->nMapCopyIndex      = nDestMapCopyIndex;
    pRespondPak->nPosX              = nX;
    pRespondPak->nPosY              = nY;
    pRespondPak->nPosZ              = nZ;
    //pRespondPak->nBattleFieldSide   = nBattleFieldSide;

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoTransferPlayerRespond(
    int nConnIndex, DWORD dwPlayerID, BOOL bSucceed, 
    DWORD dwAddress, int nPort, const GUID* pcGuid
)
{
    BOOL                            bResult     = false;
    BOOL                            bRetCode    = false;
    IMemBlock*                     piPackage   = NULL;
    R2S_TRANSFER_PLAYER_RESPOND*    pPackage    = NULL;
    const GUID                      cGuid       = {0};

    piPackage = QCreateMemBlock(sizeof(R2S_TRANSFER_PLAYER_RESPOND));
    LOG_PROCESS_ERROR(piPackage);

    pPackage = (R2S_TRANSFER_PLAYER_RESPOND*)piPackage->GetData();
    LOG_PROCESS_ERROR(pPackage);

    pPackage->wProtocolID        = r2s_transfer_player_respond;
    pPackage->dwPlayerID         = dwPlayerID;
    pPackage->bSucceed           = bSucceed;
    pPackage->dwAddress          = dwAddress;
    pPackage->wPort              = (WORD)nPort;
    pPackage->Guid               = pcGuid ? *pcGuid : cGuid;

    bRetCode = Send(nConnIndex, piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoBZoneTransferPlayerRespond(
	int nConnIndex, DWORD dwDstPlayerID, DWORD dwPlayerID, BOOL bSucceed, 
	DWORD dwAddress, int nPort, const GUID* pcGuid
	)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piPackage   = NULL;
	R2S_BZONE_TRANSFER_PLAYER_RESPOND*    pPackage    = NULL;
	const GUID                      cGuid       = {0};

	piPackage = QCreateMemBlock(sizeof(R2S_BZONE_TRANSFER_PLAYER_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pPackage = (R2S_BZONE_TRANSFER_PLAYER_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pPackage);

	pPackage->wProtocolID        = r2s_bzone_transfer_player_respond;
	pPackage->dwDstPlayerID		 = dwDstPlayerID;
	pPackage->dwPlayerID         = dwPlayerID;
	pPackage->bSucceed           = bSucceed;
	pPackage->dwAddress          = dwAddress;
	pPackage->wPort              = (WORD)nPort;
	pPackage->Guid               = pcGuid ? *pcGuid : cGuid;

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoBZoneReturnRespond(
	int nConnIndex, DWORD dwDstPlayerID, DWORD dwPlayerID, BOOL bSucceed, 
	DWORD dwAddress, int nPort, const GUID* pcGuid
	)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piPackage   = NULL;
	R2S_BZONE_RETURN_RESPOND*    pPackage    = NULL;
	const GUID                      cGuid       = {0};

	piPackage = QCreateMemBlock(sizeof(R2S_BZONE_RETURN_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pPackage = (R2S_BZONE_RETURN_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pPackage);

	pPackage->wProtocolID        = r2s_bzone_return_respond;
	pPackage->dwDstPlayerID		 = dwDstPlayerID;
	pPackage->dwPlayerID         = dwPlayerID;
	pPackage->bSucceed           = bSucceed;
	pPackage->dwAddress          = dwAddress;
	pPackage->wPort              = (WORD)nPort;
	pPackage->Guid               = pcGuid ? *pcGuid : cGuid;

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoGetMultiAwardRespond(INT nConIdx, DWORD dwRoleId, INT nMapId, INT nStar)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piPackage   = NULL;
	R2S_GET_MULTI_AWARD_RESPOND*    pPackage    = NULL;
	const GUID                      cGuid       = {0};

	piPackage = QCreateMemBlock(sizeof(R2S_GET_MULTI_AWARD_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pPackage = (R2S_GET_MULTI_AWARD_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pPackage);

	pPackage->wProtocolID        = r2s_get_multi_award_respond;
	pPackage->dwRoleId = dwRoleId;
	pPackage->nMapId = nMapId;
	pPackage->nStar = nStar;

	bRetCode = Send(nConIdx, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoGetCrossSvExDataRespond(INT nConIdx, DWORD dwRoleId, LPBYTE byExData, DWORD uExDataLen)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piPackage   = NULL;
	R2S_GET_CROSSSV_EXDATA_RESPOND*    pPackage    = NULL;
	const GUID                      cGuid       = {0};

	piPackage = QCreateMemBlock(sizeof(R2S_GET_CROSSSV_EXDATA_RESPOND) + uExDataLen);
	LOG_PROCESS_ERROR(piPackage);

	pPackage = (R2S_GET_CROSSSV_EXDATA_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pPackage);

	pPackage->wProtocolID        = r2s_get_crosssv_exdata_respond;
	pPackage->dwRoleId = dwRoleId;
	pPackage->uExDataLen = uExDataLen;
	memcpy(pPackage->byExData, byExData, uExDataLen);

	bRetCode = Send(nConIdx, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}


BOOL KGameServer::DoPlayerChatError(int nConnectionIndex, DWORD dwReceiverID, int nErrorCode)
{
    BOOL                    bResult             = false;
    BOOL                    bRetCode            = false;
    IMemBlock*             piSendBuffer        = NULL;
    R2S_PLAYER_CHAT_ERROR*  pPlayerTalkError    = NULL;

    LOG_PROCESS_ERROR(dwReceiverID != 0);

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_PLAYER_CHAT_ERROR));
    LOG_PROCESS_ERROR(piSendBuffer);

    pPlayerTalkError = (R2S_PLAYER_CHAT_ERROR *)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pPlayerTalkError);

    pPlayerTalkError->wProtocolID   = r2s_player_chat_error;
    pPlayerTalkError->dwReceiverId  = dwReceiverID;
    pPlayerTalkError->nErrorCode    = nErrorCode;

    bRetCode = Send(nConnectionIndex, piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = TRUE;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGameServer::DoPlayerChatWhisperSuccessRespond(int nConnectionIndex, DWORD dwTalkerID, DWORD dwReceiverID)
{
    BOOL                                        bResult                             = false;
    BOOL                                        bRetCode                            = false;
    IMemBlock*                                 piSendBuffer                        = NULL;
    R2S_PLAYER_CHAT_WHISPER_SUCCESS_RESPOND*    pPlayerTalkWhisperSuccessRespond    = NULL;

    LOG_PROCESS_ERROR(dwTalkerID);
    LOG_PROCESS_ERROR(dwReceiverID);

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_PLAYER_CHAT_WHISPER_SUCCESS_RESPOND));
    LOG_PROCESS_ERROR(piSendBuffer);

    pPlayerTalkWhisperSuccessRespond = (R2S_PLAYER_CHAT_WHISPER_SUCCESS_RESPOND *)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pPlayerTalkWhisperSuccessRespond);

    pPlayerTalkWhisperSuccessRespond->wProtocolID   = r2s_player_chat_whisper_success_respond;
    pPlayerTalkWhisperSuccessRespond->dwTalkerId    = dwTalkerID;
    pPlayerTalkWhisperSuccessRespond->dwReceiverId  = dwReceiverID;

    bRetCode = Send(nConnectionIndex, piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGameServer::DoMapCopyDoClearPlayerPrepare(int nConnIndex, DWORD dwMapID, int nMapCopyIndex, int nType, int nDaleyTime)
{
    BOOL                                    bResult         = false;
    BOOL                                    bRetCode        = false;
    IMemBlock*                             piSendBuffer    = NULL;
    R2S_MAP_COPY_DO_CLEAR_PLAYER_PREPARE*   pClearPrepare   = NULL;

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_MAP_COPY_DO_CLEAR_PLAYER_PREPARE));
    LOG_PROCESS_ERROR(piSendBuffer);

    pClearPrepare = (R2S_MAP_COPY_DO_CLEAR_PLAYER_PREPARE*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pClearPrepare);

    pClearPrepare->wProtocolID    = r2s_map_copy_do_clear_player_prepare;
    pClearPrepare->dwMapID        = dwMapID;
    pClearPrepare->nMapCopyIndex  = nMapCopyIndex;
    pClearPrepare->nType          = nType;
    pClearPrepare->nDelayTime     = nDaleyTime;

    bRetCode = Send(nConnIndex, piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGameServer::DoMapCopyDoClearPlayer(int nConnIndex, DWORD dwMapID, int nMapCopyIndex)
{
    BOOL                            bResult         = false;
    BOOL                            bRetCode        = false;
    IMemBlock*                     piSendBuffer    = NULL;
    R2S_MAP_COPY_DO_CLEAR_PLAYER*   pClear          = NULL;

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_MAP_COPY_DO_CLEAR_PLAYER));
    LOG_PROCESS_ERROR(piSendBuffer);

    pClear = (R2S_MAP_COPY_DO_CLEAR_PLAYER*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pClear);

    pClear->wProtocolID    = r2s_map_copy_do_clear_player;
    pClear->dwMapID        = dwMapID;
    pClear->nMapCopyIndex  = nMapCopyIndex;

    bRetCode = Send(nConnIndex, piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGameServer::DoSetMapCopyOwner(int nConnIndex, DWORD dwMapID, int nMapCopyIndex, DWORD dwOwnerID)
{
    BOOL                    bResult         = false;
    BOOL                    bRetCode        = false;
    IMemBlock*             piSendBuffer    = NULL;
    R2S_SET_MAP_COPY_OWNER* pSetOwner       = NULL;

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2S_SET_MAP_COPY_OWNER));
    LOG_PROCESS_ERROR(piSendBuffer);

    pSetOwner = (R2S_SET_MAP_COPY_OWNER*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pSetOwner);

    pSetOwner->wProtocolID      = r2s_set_map_copy_owner;
    pSetOwner->dwMapID          = dwMapID;
    pSetOwner->nMapCopyIndex    = nMapCopyIndex;
    pSetOwner->dwOwnerID        = dwOwnerID;

    bRetCode = Send(nConnIndex, piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);    

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGameServer::DoSaveRoleDataRespond(int nConnIndex, DWORD dwPlayerID, BOOL bSucceed, DWORD uRoleDataLen, int nUserValue, int nUserValue2)
{
    BOOL                        bResult         = false;
    BOOL                        bRetCode        = false;
    IMemBlock*                 piPackage       = NULL;
    R2S_SAVE_ROLE_DATA_RESPOND* pRespond        = NULL;

    piPackage = QCreateMemBlock((unsigned)(sizeof(R2S_SAVE_ROLE_DATA_RESPOND)));
    LOG_PROCESS_ERROR(piPackage);
    
    pRespond = (R2S_SAVE_ROLE_DATA_RESPOND*)piPackage->GetData();
    LOG_PROCESS_ERROR(pRespond);
    
    pRespond->wProtocolID   = r2s_save_role_data_respond;
    pRespond->dwPlayerID    = dwPlayerID;
    pRespond->bSucceed      = bSucceed;
	pRespond->uRoleDataLen = uRoleDataLen;
	pRespond->nUserValue    = nUserValue;
	pRespond->nUserValue2    = nUserValue2;

    bRetCode = Send(nConnIndex, piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KGameServer::DoSyncRoleData(int nConnIndex, DWORD dwRoleID, BYTE* pbyData, size_t uOffset, size_t uDataLen)
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = false;
    IMemBlock*         piPackage   = NULL;
    R2S_SYNC_ROLE_DATA* pSyncData   = NULL;

    piPackage = QCreateMemBlock((unsigned)(sizeof(R2S_SYNC_ROLE_DATA) + uDataLen));
    LOG_PROCESS_ERROR(piPackage);

    pSyncData = (R2S_SYNC_ROLE_DATA*)piPackage->GetData();
    LOG_PROCESS_ERROR(pSyncData);

    pSyncData->wProtocolID  = r2s_sync_role_data;
    pSyncData->dwRoleID     = dwRoleID;
    pSyncData->uOffset      = uOffset;

    if (pbyData)
    {
        memcpy(pSyncData->byData, pbyData, uDataLen);
    }

    bRetCode = Send(nConnIndex, piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KGameServer::DoLoadRoleData(int nConnIndex, DWORD dwRoleID, BOOL bSucceed, size_t uRoleDataLen)
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = false;
    IMemBlock*         piPackage   = NULL;
    R2S_LOAD_ROLE_DATA* pLoadData   = NULL;

    piPackage = QCreateMemBlock(sizeof(R2S_LOAD_ROLE_DATA));
    LOG_PROCESS_ERROR(piPackage);

    pLoadData = (R2S_LOAD_ROLE_DATA*)piPackage->GetData();
    LOG_PROCESS_ERROR(pLoadData);

    pLoadData->wProtocolID  = r2s_load_role_data;
    pLoadData->dwRoleID     = dwRoleID;
    pLoadData->bSucceed     = bSucceed;
    pLoadData->uRoleDataLen = uRoleDataLen;

    bRetCode = Send(nConnIndex, piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KGameServer::DoDelayLoadRoleData(int nConnIndex, INT nModuleId, DWORD dwPlayerID, DWORD dwSrcId, BOOL bSucceed, BYTE* pbyRoleData, size_t uDelayDataLen)
{
	BOOL                bResult     = false;
	BOOL                bRetCode    = false;
	IMemBlock*         piPackage   = NULL;
	R2S_DELAY_LOAD_ROLE_DATA* pDelayData   = NULL;
	KDelayRoleData* data = NULL; // zzx

	piPackage = QCreateMemBlock(sizeof(R2S_DELAY_LOAD_ROLE_DATA) + uDelayDataLen);
	LOG_PROCESS_ERROR(piPackage);

	pDelayData = (R2S_DELAY_LOAD_ROLE_DATA*)piPackage->GetData();
	LOG_PROCESS_ERROR(pDelayData);

	pDelayData->wProtocolID = r2s_delay_load_role_data;
	pDelayData->byModuleId = (BYTE)nModuleId;
	pDelayData->dwRoleId = dwPlayerID;
	pDelayData->dwSrcId = dwSrcId;
	pDelayData->bSucceed = bSucceed;
	pDelayData->uDelayDataLen = uDelayDataLen;

	data = (KDelayRoleData*)pbyRoleData;

	memcpy(pDelayData->byDelayData, pbyRoleData, uDelayDataLen);

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KGameServer::DoRemoteLuaCall(int nConnIndex, BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bResult         = false;
	BOOL                        bRetcode        = false;
	IMemBlock*                 piBuffer        = NULL;
	R2S_REMOTE_LUA_CALL*        pRemoteLua      = NULL;

	LOG_PROCESS_ERROR(nConnIndex != 0);

	piBuffer = QCreateMemBlock((unsigned)(sizeof(R2S_REMOTE_LUA_CALL) + uDataLen));
	LOG_PROCESS_ERROR(piBuffer);

	pRemoteLua = (R2S_REMOTE_LUA_CALL*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRemoteLua);

	pRemoteLua->wProtocolID   = r2s_remote_lua_call;

	pRemoteLua->uParamLen = uDataLen;
	memcpy(pRemoteLua->byParam, pbyData, uDataLen);

	if (nConnIndex == -1)
	{
		Broadcast(piBuffer, 0);
	}
	else
	{
		bRetcode = Send(nConnIndex, piBuffer);
		LOG_PROCESS_ERROR(bRetcode);
	}

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

void KGameServer::OnBZoneTransferRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL                            bRetCode            = FALSE;
	S2R_BZONE_TRANSFER_REQUEST*    pRequest            = (S2R_BZONE_TRANSFER_REQUEST*)pbyData;
	KRole*                          pRole               = NULL;
	KROLE_STATE                     eRoleState          = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRoleID);
	LOG_PROCESS_ERROR(pRole);

	eRoleState = pRole->GetRoleState();
	LOG_PROCESS_ERROR(eRoleState == rsOnline);
	LOG_PROCESS_ERROR(pRole->m_nConnIndex == nConnIndex);

	bRetCode = g_BZoneSlave.DoBZoneTransferRequest(pRole, pRequest->nPlayerIdx, 
		&pRequest->RoleBaseInfo, pRequest->byRoleData, pRequest->uRoleDataLen);

	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	if (bRetCode == FALSE)
	{
		DoTransferPlayerRespond(nConnIndex, pRole->m_dwPlayerID, false);
	}
}

VOID KGameServer::OnBZoneReturnRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL                            bRetCode            = FALSE;
	S2R_BZONE_RETURN_REQUEST*		pRequest            = (S2R_BZONE_RETURN_REQUEST*)pbyData;
	KRole*                          pRole               = NULL;
	KROLE_STATE                     eRoleState          = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRoleID);
	LOG_PROCESS_ERROR(pRole);

	eRoleState = pRole->GetRoleState();
	LOG_PROCESS_ERROR(eRoleState == rsOnline);
	LOG_PROCESS_ERROR(pRole->m_nConnIndex == nConnIndex);

	bRetCode = g_BZoneMaster.DoBZoneReturnRequest(pRole, &pRequest->RoleBaseInfo, pRequest->nExValue, pRequest->byExData, pRequest->uExDataLen);

	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	if (bRetCode == FALSE)
	{
		//DoTransferPlayerRespond(nConnIndex, pRole->m_dwPlayerID, false);
	}
}

VOID KGameServer::OnGetMultiAwardRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL                            bRetCode            = FALSE;
	S2R_GET_MULTI_AWARD_REQUEST*		pRequest            = (S2R_GET_MULTI_AWARD_REQUEST*)pbyData;
	KRole*                          pRole               = NULL;
	KROLE_STATE                     eRoleState          = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRoleId);
	LOG_PROCESS_ERROR(pRole);

	g_BZoneSlave.OnGetMultiAwardRequest(pRequest->dwRoleId);

EXIT0:
	return;
}

VOID KGameServer::OnGetCrossSvExDataRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	BOOL                            bRetCode            = FALSE;
	S2R_GET_CROSSSV_EXDATA_REQUEST*		pRequest            = (S2R_GET_CROSSSV_EXDATA_REQUEST*)pbyData;
	KRole*                          pRole               = NULL;
	KROLE_STATE                     eRoleState          = rsInvalid;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRoleId);
	LOG_PROCESS_ERROR(pRole);

	g_BZoneSlave.OnGetCrossSvExDataRequest(pRequest->dwRoleId);

EXIT0:
	return;
}

BOOL KGameServer::DoQueryStatIDRespond(int nConnIndex, const char szName[], int nID)
{
	BOOL                            bResult     = false;
	BOOL                            bRetcode    = false;
	IMemBlock*                     piBuffer	= NULL;
	R2S_QUERY_STAT_ID_RESPOND*      pRespond    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(R2S_QUERY_STAT_ID_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (R2S_QUERY_STAT_ID_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_query_stat_id_respond;
	pRespond->nID = nID;

	strncpy(pRespond->szName, szName, sizeof(pRespond->szName));
	pRespond->szName[sizeof(pRespond->szName) - 1] = '\0';

	bRetcode = Send(nConnIndex, piBuffer);
	LOG_PROCESS_ERROR(bRetcode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}


VOID KGameServer::ChangeGMCMDMarkRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	g_pGameCenter->m_GMSendAward.ChangeGMCMDMark(pbyData,uDataLen,nConnIndex);

EXIT0:
	return;
}

VOID KGameServer::onLoginFindSqlRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	S2R_LOAD_ARENA_RECORD_REQUEST*			pRequest            = (S2R_LOAD_ARENA_RECORD_REQUEST*)pbyData;
	BOOL             bResult         = false;
	BOOL             bRetCode        = false;
	int              nRetCode        = false;
	DWORD			 roleID		 = pRequest->dwRoleId;
	LOG_PROCESS_ERROR(roleID);
	g_pGameCenter->m_GMSendAward.OnLogin(roleID);
EXIT0:
	return;
}

VOID KGameServer::SyncInfoToSqlRequest( BYTE* pbyData, size_t uDataLen, int nConnIndex )
{
	S2R_LOG_INFO_SAVE_REQUEST*			pRequest            = (S2R_LOG_INFO_SAVE_REQUEST*)pbyData;
	INT nActionType = pRequest->nActionType;
	DWORD dwRoleID = pRequest->dwRoleID;
	INT number = pRequest->number;
	const char *pszComment = pRequest->pszComment;
	g_pGameCenter->m_LogServerAgency.LogConsumeAction(nActionType,dwRoleID,number,pszComment);
EXIT0:
	return;
}

//AutoCode:-发送协议函数结束-
