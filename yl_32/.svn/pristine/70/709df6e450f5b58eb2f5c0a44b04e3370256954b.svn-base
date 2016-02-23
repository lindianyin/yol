#include "stdafx.h"
#include "KRelayClient.h"

#if defined(GAME_SERVER)
#include "config/kplayerbasedbdef.h"

#include "onlinegameworld/kplayer.h"
#include "config/kconfigfile.h"
#include "onlinegameworld/KRecorderFactory.h"
#include "onlinegameworld/ikgameserver.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/korpgworld.h"
#include "scene/kscenemgr.h"
#include "kplayerdbmanager.h"
#include "kplayerbaseprocess.h"
#include "knetconnectplayer.h"
#include "scene/kscene.h"
#include "../onlinegamemodule/qteamgchandler.h"
#include "onlinegameworld/synchelper.h"
#include "../onlinegamemodule/kin/KinRelayClient.h"
#include "../onlinegamemodule/rank/RankRelayClient.h"
#include "../onlinegamemodule/crosssv/kcrosssvctrl.h"
#include "../onlinegamemodule/arena/ArenaRelayClient.h"
#include "../onlinegamemodule/relation/RelationModule.h"
#include "../onlinegamemodule/mail2/MailRelayClient.h"
#include "../onlinegamemodule/item2/kplayeritem.h"
#include "../onlinegamemodule/instance/kinstancectrl.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "../onlinegamemodule/arena/arenamodule.h"
#include "../onlinegamemodule/escort/kescortmodule.h"
#include "../onlinegamemodule/arena/ArenaHelper.h"
#include "../onlinegamemodule/quiz/QuizRelayClient.h"
#include "../onlinegamemodule/escort/kescortrelayclient.h"
#include "../onlinegamemodule/overallteam/koverallrelayclient.h"
#include "../onlinegamemodule/record/RecordRelayClient.h"
#include "../onlinegamemodule/rank/krankmanager.h"
#include "../onlinegamemodule/activity/kactivitymanager.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "../onlinegamemodule/vip/playervip.h"
#include "../onlinegamemodule/awardlobby/playerawardlobby.h"
#include "../onlinegamemodule/award/kawardmanager.h"
#include "../onlinegamemodule/award/awarddef.h"


// ------------------------------------------------------------------------------
#define REGISTER_MODULE_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ModuleProtocolFuns[ProtocolID] = FuncName;					\
	m_uProtocolSize[ProtocolID] = ProtocolSize;}

#ifdef WIN32
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")
#include "../onlinegamemodule/arena/arenamodule.h"

SIZE_T _GetUsedMemSize()
{
	INT						nRetCode	= 0;
	SIZE_T					uMemUsed	= 0;
	PROCESS_MEMORY_COUNTERS	memCounters;

	ZeroStruct(memCounters);

	nRetCode = GetProcessMemoryInfo(INVALID_HANDLE_VALUE, &memCounters, sizeof(memCounters));
	LOG_PROCESS_ERROR(nRetCode);

	uMemUsed = memCounters.WorkingSetSize;
EXIT0:
	return uMemUsed;
}
#else // WIN32
SIZE_T _GetUsedMemSize()
{
	INT nRetCode = 0;
	static INT nPageSize = 0;
	SIZE_T uMemUsed = 0;
	FILE* pFile = NULL;
	CHAR pBuffer[MAX_PATH] = {0};

	if (nPageSize == 0)
		nPageSize = getpagesize();

	LOG_PROCESS_ERROR(nPageSize);

	pFile = fopen("/proc/self/statm", "r");
	PROCESS_ERROR(pFile); // 有的系统没加载procfs，不报错

	nRetCode = fread(pBuffer, 1, sizeof(pBuffer), pFile);
	LOG_PROCESS_ERROR(nRetCode);

	nRetCode = sscanf(pBuffer, "%lu", &uMemUsed);
	LOG_PROCESS_ERROR(nRetCode);

	uMemUsed *= nPageSize;

EXIT0:
	if (pFile)
		fclose(pFile);

	return uMemUsed;
}
#endif


KRelayClient g_RelayClient;
// ------------------------------------------------------------------------------

KRelayClient::KRelayClient(void) : m_ProtocolStat("r2s", r2s_protocol_end)
{
    m_piSocketStream        = NULL;
    m_bSendErrorFlag        = false;
    m_bQuiting              = false;
    m_bSyncDataOver     = false;
    m_nNextQuitingSaveTime  = 0;
	m_nPingCycle            = 0;
	m_nLastSendPacketTime   = 0;

    m_dwSyncRoleID          = KD_BAD_ID;
    m_pbySyncRoleBuffer     = NULL;
    m_uSyncRoleOffset       = 0;

    m_pbySaveRoleBuffer     = NULL;

    memset(m_ProcessProtocolFuns, 0, sizeof(m_ProcessProtocolFuns));
	memset(m_uProtocolSize, 0, sizeof(m_uProtocolSize));
	memset(m_ModuleProtocolFuns, 0, sizeof(m_ProcessProtocolFuns));

    // ------------------ 协议注册 --------------------------------------------------------------------
    REGISTER_INTERNAL_FUNC(r2s_handshake_respond, &KRelayClient::OnHandshakeRespond, sizeof(R2S_HANDSHAKE_RESPOND));
	REGISTER_INTERNAL_FUNC(r2s_bzone_handshake_respond, &KRelayClient::OnBZoneHandshakeRespond, sizeof(R2S_BZONE_HANDSHAKE_RESPOND));
    REGISTER_INTERNAL_FUNC(r2s_quit_notify, &KRelayClient::OnQuitNotify, sizeof(R2S_QUIT_NOTIFY));
    REGISTER_INTERNAL_FUNC(r2s_confirm_player_login_respond, &KRelayClient::OnConfirmPlayerLoginRespond, sizeof(R2S_CONFIRM_PLAYER_LOGIN_RESPOND));

    REGISTER_INTERNAL_FUNC(r2s_create_map_notify, &KRelayClient::OnCreateMapNotify, sizeof(R2S_CREATE_MAP_NOTIFY));
    REGISTER_INTERNAL_FUNC(r2s_finish_create_map_notify, &KRelayClient::OnFinishCreateMapNotify, sizeof(R2S_FINISH_CREATE_MAP_NOTIFY));
    REGISTER_INTERNAL_FUNC(r2s_delete_map_notify, &KRelayClient::OnDeleteMapNotify, sizeof(R2S_DELETE_MAP_NOTIFY));

    REGISTER_INTERNAL_FUNC(r2s_player_login_request, &KRelayClient::OnPlayerLoginRequest, sizeof(R2S_PLAYER_LOGIN_REQUEST));
	REGISTER_INTERNAL_FUNC(r2s_search_map_respond, &KRelayClient::OnSearchMapRespond, sizeof(R2S_SEARCH_MAP_RESPOND));
	REGISTER_INTERNAL_FUNC(r2s_load_map_respond, &KRelayClient::OnLoadMapRespond, sizeof(R2S_LOAD_MAP_RESPOND));
    REGISTER_INTERNAL_FUNC(r2s_transfer_player_request, &KRelayClient::OnTransferPlayerRequest, sizeof(R2S_TRANSFER_PLAYER_REQUEST));
    REGISTER_INTERNAL_FUNC(r2s_transfer_player_respond, &KRelayClient::OnTransferPlayerRespond, sizeof(R2S_TRANSFER_PLAYER_RESPOND));
	REGISTER_INTERNAL_FUNC(r2s_bzone_transfer_player_respond, &KRelayClient::OnBZoneTransferPlayerRespond, sizeof(R2S_BZONE_TRANSFER_PLAYER_RESPOND));
	REGISTER_INTERNAL_FUNC(r2s_bzone_return_respond, &KRelayClient::OnBZoneReturnRespond, sizeof(R2S_BZONE_RETURN_RESPOND));
	REGISTER_INTERNAL_FUNC(r2s_get_multi_award_respond, &KRelayClient::OnGetMultiAwardRespond, sizeof(R2S_GET_MULTI_AWARD_RESPOND));
	REGISTER_INTERNAL_FUNC(r2s_get_crosssv_exdata_respond, &KRelayClient::OnGetCrossSvExDataRespond, sizeof(R2S_GET_CROSSSV_EXDATA_RESPOND));
	

    REGISTER_INTERNAL_FUNC(r2s_kick_account_notify, &KRelayClient::OnKickAccountNotify, sizeof(R2S_KICK_ACCOUNT_NOTIFY));

    REGISTER_INTERNAL_FUNC(r2s_map_copy_do_clear_player_prepare, &KRelayClient::OnMapCopyDoClearPlayerPrepare, sizeof(R2S_MAP_COPY_DO_CLEAR_PLAYER_PREPARE));
    REGISTER_INTERNAL_FUNC(r2s_map_copy_do_clear_player, &KRelayClient::OnMapCopyDoClearPlayer, sizeof(R2S_MAP_COPY_DO_CLEAR_PLAYER));
    REGISTER_INTERNAL_FUNC(r2s_set_map_copy_owner, &KRelayClient::OnSetMapCopyOwner, sizeof(R2S_SET_MAP_COPY_OWNER));

    REGISTER_INTERNAL_FUNC(r2s_save_role_data_respond, &KRelayClient::OnSaveRoleDataRespond, sizeof(R2S_SAVE_ROLE_DATA_RESPOND));
    REGISTER_INTERNAL_FUNC(r2s_sync_role_data, &KRelayClient::OnSyncRoleData, sizeof(R2S_SYNC_ROLE_DATA));
    REGISTER_INTERNAL_FUNC(r2s_load_role_data, &KRelayClient::OnLoadRoleData, sizeof(R2S_LOAD_ROLE_DATA));

	REGISTER_INTERNAL_FUNC(r2s_delay_load_role_data, &KRelayClient::OnDelayLoadRoleData, sizeof(R2S_DELAY_LOAD_ROLE_DATA));

	//REGISTER_INTERNAL_FUNC(r2s_gm_command, &KRelayClient::OnGmCommand, sizeof(R2S_GM_COMMAND));
	REGISTER_INTERNAL_FUNC(r2s_remote_lua_call, &KRelayClient::OnRemoteLuaCall, sizeof(R2S_REMOTE_LUA_CALL));

    //REGISTER_INTERNAL_FUNC(r2s_change_ext_point_respond, &KRelayClient::OnChangeExtPointRespond, sizeof(R2S_CHANGE_EXT_POINT_RESPOND));

	// ------------------------ Team --------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_invite_player_join_team_request, &QTeamGCHandler::OnInvitePlayerJoinTeamRequest, sizeof(R2S_INVITE_PLAYER_JOIN_TEAM_REQUEST));
	REGISTER_MODULE_FUNC(r2s_apply_join_team_request, &QTeamGCHandler::OnApplyJoinTeamRequest, sizeof(R2S_APPLY_JOIN_TEAM_REQUEST));
	REGISTER_MODULE_FUNC(r2s_team_create_notify, &QTeamGCHandler::OnTeamCreateNotify, sizeof(R2S_TEAM_CREATE_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_team_add_member_notify, &QTeamGCHandler::OnTeamAddMemberNotify, sizeof(R2S_TEAM_ADD_MEMBER_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_team_del_member_notify, &QTeamGCHandler::OnTeamDelMemberNotify, sizeof(R2S_TEAM_DEL_MEMBER_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_team_disband_notify, &QTeamGCHandler::OnTeamDisbandNotify, sizeof(R2S_TEAM_DISBAND_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_team_change_authority_notify, &QTeamGCHandler::OnTeamChangeAuthorityNotify, sizeof(R2S_TEAM_CHANGE_AUTHORITY_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_team_set_loot_mode_notify, &QTeamGCHandler::OnTeamSetLootModeNotify, sizeof(R2S_TEAM_SET_LOOT_MODE_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_party_message_notify, &QTeamGCHandler::OnTeamMessageNotify, sizeof(R2S_PARTY_MESSAGE_NOTIFY));
	//REGISTER_MODULE_FUNC(r2s_team_set_formation_leader_notify, &QTeamGCHandler::OnSetFormationLeaderNotify, sizeof(R2S_TEAM_SET_FORMATION_LEADER_NOTIFY));

	REGISTER_MODULE_FUNC(r2s_sync_team_member_online_flag, &QTeamGCHandler::OnSyncTeamMemberOnlineFlag, sizeof(R2S_SYNC_TEAM_MEMBER_ONLINE_FLAG));
	REGISTER_MODULE_FUNC(r2s_sync_team_member_max_lmr, &QTeamGCHandler::OnTeamSyncMemberMaxLMR, sizeof(R2S_SYNC_TEAM_MEMBER_MAX_LMR));
	REGISTER_MODULE_FUNC(r2s_sync_team_member_current_lmr, &QTeamGCHandler::OnTeamSyncMemberCurrentLMR, sizeof(R2S_SYNC_TEAM_MEMBER_CURRENT_LMR));
	REGISTER_MODULE_FUNC(r2s_sync_team_member_misc, &QTeamGCHandler::OnTeamSyncMemberMisc, sizeof(R2S_SYNC_TEAM_MEMBER_MISC));
	REGISTER_MODULE_FUNC(r2s_sync_team_member_position, &QTeamGCHandler::OnTeamSyncMemberPosition, sizeof(R2S_SYNC_TEAM_MEMBER_POSITION));
	//REGISTER_MODULE_FUNC(r2s_team_set_mark_respond, &QTeamGCHandler::OnTeamSetMarkRespond, sizeof(R2S_TEAM_SET_MARK_RESPOND));
	//REGISTER_MODULE_FUNC(r2s_team_camp_change, &QTeamGCHandler::OnTeamCampChange, sizeof(R2S_TEAM_CAMP_CHANGE));
	REGISTER_MODULE_FUNC(r2s_team_level_up_raid_notify, &QTeamGCHandler::OnTeamLevelUpRaidNotify, sizeof(R2S_TEAM_LEVEL_UP_RAID_NOTIFY));
	//REGISTER_MODULE_FUNC(r2s_team_change_member_group_notify, &QTeamGCHandler::OnTeamChangeMemberGroupNotify, sizeof(R2S_TEAM_CHANGE_MEMBER_GROUP_NOTIFY));
	// -------------------------------------------------------------------------------------------

	// ------------------------ Kin --------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_sync_tong_member_info, &KinRelayClient::OnSyncTongMemberInfo, sizeof(R2S_SYNC_TONG_MEMBER_INFO));
	REGISTER_MODULE_FUNC(r2s_delete_tong_member_notify, &KinRelayClient::OnDeleteTongMemberNotify, sizeof(R2S_DELETE_TONG_MEMBER_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_apply_tong_info_respond, &KinRelayClient::OnApplyTongInfoRespond, sizeof(R2S_APPLY_TONG_INFO_RESPOND));
	REGISTER_MODULE_FUNC(r2s_update_tong_client_data_version, &KinRelayClient::OnUpdateTongClientDataVersion, sizeof(R2S_UPDATE_TONG_CLIENT_DATA_VERSION));

	REGISTER_MODULE_FUNC(r2s_sync_tong_repertory_page_respond, &KinRelayClient::OnSyncTongRepertoryPageRespond, sizeof(R2S_SYNC_TONG_REPERTORY_PAGE_RESPOND));
	REGISTER_MODULE_FUNC(r2s_sync_apply_join_tong_list, &KinRelayClient::OnSyncTongApplyJoinList, sizeof(R2S_SYNC_TONG_APPLY_JOIN_LIST));
	REGISTER_MODULE_FUNC(r2s_sync_kin_opentimes, &KinRelayClient::OnSyncTongActivityOpenTimes, sizeof(R2S_SYNC_KIN_ACTIVITY_OPENTIMES));

	REGISTER_MODULE_FUNC(r2s_invite_player_join_tong_request, &KinRelayClient::OnInvitePlayerJoinTongRequest, sizeof(R2S_INVITE_PLAYER_JOIN_TONG_REQUEST));
	REGISTER_MODULE_FUNC(r2s_change_player_tong_notify, &KinRelayClient::OnChangePlayerTongNotify, sizeof(R2S_CHANGE_PLAYER_TONG_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_tong_broadcast_message, &KinRelayClient::OnBroadcastTongMessage, sizeof(R2S_TONG_BROADCAST_MESSAGE));
	REGISTER_MODULE_FUNC(r2s_get_tong_description_respond, &KinRelayClient::OnGetTongDescriptionRespond, sizeof(R2S_GET_TONG_DESCRIPTION_RESPOND));
	REGISTER_MODULE_FUNC(r2s_sync_tong_online_message, &KinRelayClient::OnSyncTongOnlineMessage, sizeof(R2S_SYNC_TONG_ONLINE_MESSAGE));
	//REGISTER_MODULE_FUNC(, &KinRelayClient::OnTakeTongRepertoryItemToPosRespond, sizeof());
	REGISTER_MODULE_FUNC(r2s_take_tong_repertory_item_respond, &KinRelayClient::OnTakeTongRepertoryItemRespond, sizeof(R2S_TAKE_TONG_REPERTORY_ITEM_RESPOND));
	//REGISTER_MODULE_FUNC(r2s_put_tong_repertory_item_fail_respond, &KinRelayClient::, sizeof(R2S_PUT_TONG_REPERTORY_ITEM_FAIL_RESPOND));
	//REGISTER_MODULE_FUNC(, &KinRelayClient::OnApplyStackTongRepertoryItemRespond(BYTE* pbyData, size_t uDataLen);
	REGISTER_MODULE_FUNC(r2s_apply_open_tong_repertpry_respond, &KinRelayClient::OnApplyOpenTongRepertoryRespond, sizeof(R2S_APPLY_OPEN_TONG_REPERTPRY_RESPOND));
	REGISTER_MODULE_FUNC(r2s_get_tong_salary_respond, &KinRelayClient::OnGetTongSalaryRespond, sizeof(R2S_GET_TONG_SALARY_RESPOND));
	REGISTER_MODULE_FUNC(r2s_sync_tong_history_respond, &KinRelayClient::OnSyncTongHistoryRespond, sizeof(R2S_SYNC_TONG_HISTORY_RESPOND));
	REGISTER_MODULE_FUNC(r2s_kin_message_notify, &KinRelayClient::OnKinMessageNotify, sizeof(R2S_KIN_MESSAGE_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_put_tong_repertory_item_respond, &KinRelayClient::OnPutTongRepertoryItemRespond, sizeof(R2S_PUT_TONG_REPERTORY_ITEM_RESPOND));
	REGISTER_MODULE_FUNC(r2s_get_back_item_from_repertory_respond, &KinRelayClient::OnGetBackItemFromRepertoryRespond, sizeof(R2S_GET_BACK_ITEM_FROM_REPERTORY_RESPOND));
	REGISTER_MODULE_FUNC(r2s_get_freejoin_tong_list_respond, &KinRelayClient::OnGetFreeJoinTongListRespond, sizeof(R2S_GET_FREEJOIN_TONG_LIST_RESPOND));
	REGISTER_MODULE_FUNC(r2s_sync_tong_rank_info, &KinRelayClient::OnGetTongRankInfoRespond, sizeof(R2S_SYNC_TONG_RANK_INFO));

	// ------------------------ 排行榜 --------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_get_rank_idx_respond, &RankRelayClient::OnGetRankIdxRespond, sizeof(R2S_GET_RANK_IDX_RESPOND));
	REGISTER_MODULE_FUNC(r2s_get_rank_id_respond, &RankRelayClient::OnGetRankIdRespond, sizeof(R2S_GET_RANK_ID_RESPOND));
	REGISTER_MODULE_FUNC(r2s_swap_rank_respond, &RankRelayClient::OnSwapRankRespond, sizeof(R2S_SWAP_RANK_RESPOND));

	// ------------------------ 竞技场 --------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_load_arena_record_respond, &ArenaRelayClient::OnLoadRecordRespond, sizeof(R2S_LOAD_ARENA_RECORD_RESPOND));

	// ------------------------ 修仙 --------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_sync_escort_members, &KEscortRelayClient::OnSyncEscortMembers, sizeof(R2S_SYNC_ESCORT_MEMBERS));
	REGISTER_MODULE_FUNC(r2s_sync_escort_state, &KEscortRelayClient::OnSyncEscortState, sizeof(R2S_SYNC_ESCORT_STATE));
	REGISTER_MODULE_FUNC(r2s_take_escort_award_respond, &KEscortRelayClient::OnTakeEscortAwardRespond, sizeof(R2S_TAKE_ESCORT_AWARD_RESPOND));
	REGISTER_MODULE_FUNC(r2s_sync_rob_escort_members, &KEscortRelayClient::OnSyncRobEscortMembers, sizeof(R2S_SYNC_PLAYER_ROB_ESCORTS));
	REGISTER_MODULE_FUNC(r2s_update_escort_state, &KEscortRelayClient::OnUpdateEscortState, sizeof(R2S_UPDATE_ESCORT_STATE));
	REGISTER_MODULE_FUNC(r2s_rescue_player_respond, &KEscortRelayClient::OnRescuePlayerRespond, sizeof(R2S_RESCUE_PLAYER_RESPOND));
	REGISTER_MODULE_FUNC(r2s_take_robescort_award_respond, &KEscortRelayClient::OnTakeRobAwardRespond, sizeof(R2S_TAKE_ROBESCORT_AWARD_RESPOND));


	// ------------------------ 全服记录模块 --------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_get_server_record_respond, &RecordRelayClient::OnGetServerRecordRespond, sizeof(R2S_GET_SERVER_RECORD_RESPOND));
	REGISTER_MODULE_FUNC(r2s_get_all_purchase_buy_respond, &RecordRelayClient::OnGetAllPurchaseRespond, sizeof(R2S_BUY_ALL_PURCHASE_REQUEST));

	// ------------------------ 跨服组队 --------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_sync_overall_teams, &KOverallRelayClient::OnSyncOverallTeamList, sizeof(R2S_SYNC_OVERALL_TEAM));
	REGISTER_MODULE_FUNC(r2s_sync_overall_teamops_result, &KOverallRelayClient::OnSyncOverallTeamOpsResult, sizeof(R2S_OVERALL_TEAM_OPS_RESULT));
	REGISTER_MODULE_FUNC(r2s_sync_player_overall_team, &KOverallRelayClient::OnSynPlayerOverallTeam, sizeof(R2S_SYNC_PLAYER_OVERALL_TEAM));
	REGISTER_MODULE_FUNC(r2s_member_countdown_notify, &KOverallRelayClient::OnMemberCountDownNotify, sizeof(R2S_MEMBER_COUNTDOWN_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_check_member_online_respond, &KOverallRelayClient::OnCheckMemberOnlineRespond, sizeof(R2S_CHECK_MEMBER_ONLINE_RESPOND));
	REGISTER_MODULE_FUNC(r2s_multi_trans_request, &KOverallRelayClient::OnMultiTransRequest, sizeof(R2S_MULTI_TRANS_REQUEST));

	// ----------------------- 好友关系 -----------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_relation_start_sync_list_respond, &QRelationModule::R2SOnStartSyncRelation, sizeof(R2S_RELATION_SYNC_LIST_RESPOND));
	REGISTER_MODULE_FUNC(r2s_relation_sync_list_respond, &QRelationModule::R2SOnSyncRelation, sizeof(R2S_RELATION_SYNC_LIST_RESPOND));
	REGISTER_MODULE_FUNC(r2s_relation_del_relation_respond, &QRelationModule::R2SOnDelRelation, sizeof(R2S_RELATION_DEL_RELATION_RESPOND));
	REGISTER_MODULE_FUNC(r2s_relation_friend_info_respond, &QRelationModule::R2SOnFriendInfo, sizeof(R2S_RELATION_FRIEND_INFO_RESPOND));
	REGISTER_MODULE_FUNC(r2s_relation_online_notify, &QRelationModule::R2SOnNotifyOnline, sizeof(R2S_RELATION_ONLINE_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_relation_notify_respond, &QRelationModule::R2SOnNotifyRelation, sizeof(R2S_RELATION_NOTIFY_RESPOND));
// 	REGISTER_MODULE_FUNC(r2s_relation_delete_player_respond, &QRelationModule::R2SOnDeletePlayer, sizeof(R2S_RELATION_DEL_RELATION_RESPOND));
// 	REGISTER_MODULE_FUNC(r2s_relation_training_error_respond, &QRelationModule::R2SOnTrainingError, sizeof(KRELATION_PROTO_G2S_TRAININGERROR));
// 	REGISTER_MODULE_FUNC(r2s_relation_search_teacher_respond, &QRelationModule::R2SOnSearchTeacher, sizeof(KRELATION_PROTO_G2S_SEARCHTEACHER));
// 	REGISTER_MODULE_FUNC(r2s_relation_search_student_respond, &QRelationModule::R2SOnSearchStudent, sizeof(KRELATION_PROTO_G2S_SEARCHSTUDENT));
// 	REGISTER_MODULE_FUNC(r2s_relation_training_option_respond, &QRelationModule::R2SOnTrainingOption, sizeof(KRELATION_PROTO_G2S_TRAININGOPTION));
// 	REGISTER_MODULE_FUNC(r2s_relation_applying_teacher_respond, &QRelationModule::R2SOnApplyingTeacher, sizeof(KRELATION_PROTO_G2S_TRAININGAPPLY));
// 	REGISTER_MODULE_FUNC(r2s_relation_applying_student_respond, &QRelationModule::R2SOnApplyingStudent, sizeof(KRELATION_PROTO_G2S_TRAININGAPPLY));
// 	REGISTER_MODULE_FUNC(r2s_relation_training_view_info_respond, &QRelationModule::R2SOnTrainingViewInfo, sizeof(KRELATION_PROTO_G2S_TRAININGVIEWINFO));
// 	REGISTER_MODULE_FUNC(r2s_relation_training_player_info_respond, &QRelationModule::R2SOnTrainingRoleInfo, sizeof(KRELATION_PROTO_G2S_PLAYERINFO));
// 	REGISTER_MODULE_FUNC(r2s_relation_training_apply_respond, &QRelationModule::R2SOnApplyForTeacher, sizeof(KRELATION_PROTO_G2S_TRAININGAPPLYFORTEACHER));
// 	REGISTER_MODULE_FUNC(r2s_relation_result_apply_ib_coin_respond, &QRelationModule::R2SOnResultApplyIbCoin, sizeof(KRELATION_PROTO_G2S_RESULTAPPLYIBCOIN));
// 	REGISTER_MODULE_FUNC(r2s_relation_result_gain_ib_coin_respond, &QRelationModule::R2SOnResultGainIbCoin, sizeof(KRELATION_PROTO_S2G_ADDBINDCOIN));
// 	REGISTER_MODULE_FUNC(r2s_relation_award_coin_by_friend_favor_respond, &QRelationModule::R2SOnProcessAwardFriendFavor, sizeof(KRELATION_PROTO_G2S_AWARDBYFRIENDFAVOR));

	// ------------------------邮件相关-------------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_send_mail_respond, &MailRelayClient::OnSendMailRespond, sizeof(R2S_SEND_MAIL_RESPOND));
	REGISTER_MODULE_FUNC(r2s_get_maillist_respond, &MailRelayClient::OnGetMailListRespond, sizeof(R2S_GET_MAILLIST_RESPOND));
	REGISTER_MODULE_FUNC(r2s_sync_mail_content, &MailRelayClient::OnSyncMailContent, sizeof(R2S_SYNC_MAIL_CONTENT));
	REGISTER_MODULE_FUNC(r2s_give_mail_money_to_player, &MailRelayClient::OnGiveMailMoneyToPlayer, sizeof(R2S_GIVE_MAIL_MONEY_TO_PLAYER));
	REGISTER_MODULE_FUNC(r2s_give_mail_item_to_player, &MailRelayClient::OnGiveMailItemToPlayer, sizeof(R2S_GIVE_MAIL_ITEM_TO_PLAYER));
	REGISTER_MODULE_FUNC(r2s_new_mail_notify, &MailRelayClient::OnNewMailNotify, sizeof(R2S_NEW_MAIL_NOTIFY));
	REGISTER_MODULE_FUNC(r2s_mail_general_respond, &MailRelayClient::OnMailGeneralRespond, sizeof(R2S_MAIL_GENERAL_RESPOND));

	REGISTER_INTERNAL_FUNC(r2s_player_chat_error, &KRelayClient::OnPlayerChatError, sizeof(R2S_PLAYER_CHAT_ERROR));
	REGISTER_INTERNAL_FUNC(r2s_player_chat_whisper_success_respond, &KRelayClient::OnPlayerChatWhisperSuccessRespond, sizeof(R2S_PLAYER_CHAT_WHISPER_SUCCESS_RESPOND));
	REGISTER_INTERNAL_FUNC(r2s_chat_message, &KRelayClient::OnChatMessage, sizeof(R2S_CHAT_MESSAGE));


	//-----------------------------------------答题相关------------------------------------------------------------------
	REGISTER_MODULE_FUNC(r2s_quiz_name_list_respond, &QuizRelayClient::OnNameListRespond, sizeof(R2S_QUIZ_NAME_LIST_RESPOND));
	REGISTER_MODULE_FUNC(r2s_quiz_qustions_respond, &QuizRelayClient::OnQuestionListRespond, sizeof(R2S_QUIZ_QUESTION_RESPOND));

	REGISTER_INTERNAL_FUNC(r2s_pay_order_respond, &KRelayClient::OnPayOrderRespond, sizeof(R2S_PAY_ORDER_RESPOND));
    //AutoCode:注册协议

	REGISTER_INTERNAL_FUNC(r2s_GM_send_award, &KRelayClient::SendGMCMDAward, sizeof(R2S_GM_SEND_AWARD));
}

BOOL KRelayClient::Init(/*IRecorderFactory* piFactory*/)
{
    BOOL               bResult          = false;
    BOOL               bRetCode         = false;
    IIniFile*	       piIniFile        = NULL;
	int                nRelayPort       = 0;
	int                nPingCycle       = 0;
    ISocketClient*     piSocketClient   = NULL;
    char               szRelayAddr[16];
	struct timeval     TimeVal;
	QSocketConnector  Connector;
    
    //assert(piFactory);

    m_pbySyncRoleBuffer = new BYTE[MAX_ROLE_DATA_SIZE];
    LOG_PROCESS_ERROR(m_pbySyncRoleBuffer);

    m_uSyncRoleOffset = 0;
    m_dwSyncRoleID    = KD_BAD_ID;

    m_pbySaveRoleBuffer = new BYTE[MAX_ROLE_DATA_SIZE];
    LOG_PROCESS_ERROR(m_pbySaveRoleBuffer);

    piIniFile = g_OpenIniFile(KDF_SERVER_CFG);
	LOG_PROCESS_ERROR(piIniFile);

    piIniFile->GetString("Coordinator", "IP", "127.0.0.1", szRelayAddr, 16);
	piIniFile->GetInteger("Coordinator", "Port", 5003, &nRelayPort);
	piIniFile->GetInteger("Coordinator", "PingInterval", 20, &nPingCycle);
    
	m_piSocketStream = Connector.Connect(szRelayAddr, nRelayPort);
	LOG_PROCESS_ERROR(m_piSocketStream);

	TimeVal.tv_sec  = 8;
	TimeVal.tv_usec = 0;

	bRetCode = m_piSocketStream->SetTimeout(&TimeVal);
	LOG_PROCESS_ERROR(bRetCode);
    
    m_bSendErrorFlag    = false;
	m_nPingCycle        = nPingCycle;
    m_nWorldIndex       = 0;

    m_nUpTraffic      = 0;
    m_nDownTraffic    = 0;
    m_fUpTraffic      = 0.0f;
    m_fDownTraffic    = 0.0f;

    bRetCode = DoHandshakeRequest();
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        SAFE_RELEASE(m_piSocketStream);
        SAFE_DELETE_ARRAY(m_pbySaveRoleBuffer);
        SAFE_DELETE_ARRAY(m_pbySyncRoleBuffer);
    }

    QLogPrintf(
        LOG_INFO, "Connect to coordinator %s:%d ... [%s]",
        szRelayAddr, nRelayPort, bResult ? "OK" : "Failed"
    );
    
    SAFE_RELEASE(piSocketClient);
    SAFE_RELEASE(piIniFile);
    return bResult;
}

void KRelayClient::UnInit(void)
{
    SAFE_RELEASE(m_piSocketStream);
    SAFE_DELETE_ARRAY(m_pbySaveRoleBuffer);
    SAFE_DELETE_ARRAY(m_pbySyncRoleBuffer);
}

struct KSearchPlayerForExitingSave
{
    KSearchPlayerForExitingSave()
    {
        m_pPlayer = NULL;
    }

    BOOL operator()(DWORD dwID, KPlayer* pPlayer)
    {
        assert(pPlayer);
        if (pPlayer->GetPlayerIndex() >= 0)
        {
            m_pPlayer = pPlayer;
            return false;
        }

        return true;
    }

    KPlayer* m_pPlayer;
};

void KRelayClient::Activate()
{
    if (m_bQuiting)
	{
		IKGameServer* piGameServer = g_GetGameServerInterface();
		piGameServer->ShutDown();
    }

    if (g_cOrpgWorld.m_dwLogicFrames % GAME_FPS == 0)
	{
		DoUpdatePerformance();

        m_fUpTraffic    = m_fUpTraffic * 0.9f + (float)m_nUpTraffic * 0.1f;
        m_fDownTraffic  = m_fDownTraffic * 0.9f + (float)m_nDownTraffic * 0.1f;

        m_nUpTraffic    = 0;
        m_nDownTraffic  = 0;
    }
}

BOOL KRelayClient::ProcessPackage()
{
    BOOL                    bResult             = false;
	int                     nRetCode            = false;
    IMemBlock*              piBuffer            = NULL;
    int                     nConnectionAlive    = true;

    assert(m_piSocketStream);

    if (m_bSendErrorFlag)
    {
        nConnectionAlive = false;
        m_bSendErrorFlag = false;
        goto EXIT0;
    }

	while (true)
	{
	    const struct timeval        TimeVal     = {0, 0};
        INTERNAL_PROTOCOL_HEADER*   pHeader     = NULL;
        size_t                      uPakSize    = 0;
        PROCESS_PROTOCOL_FUNC       pFunc       = NULL;
		INT64						uTSC		= 0;
		int							nProtocol	= 0;

		if (KSysService::GameTime(NULL) - m_nLastSendPacketTime > m_nPingCycle)
		{
			DoPingSignal();
		}

	    nRetCode = m_piSocketStream->CheckCanRecv(&TimeVal);
        if (nRetCode == -1)
        {
            nConnectionAlive = false;
            goto EXIT0;
        }
        if (nRetCode == 0)
        {
            break;
        }

        LOG_PROCESS_ERROR(nRetCode == 1);

        SAFE_RELEASE(piBuffer);

		nRetCode = m_piSocketStream->Recv(&piBuffer);
        if (nRetCode == -1)
        {
            nConnectionAlive = false;
            goto EXIT0;
        }
		LOG_PROCESS_ERROR(nRetCode == 1);

		pHeader = (INTERNAL_PROTOCOL_HEADER*)piBuffer->GetData();
        LOG_PROCESS_ERROR(pHeader);
          
        LOG_PROCESS_ERROR(pHeader->wProtocolID < r2s_protocol_end);
        
        uPakSize = piBuffer->GetSize();
        LOG_PROCESS_ERROR(uPakSize >= m_uProtocolSize[pHeader->wProtocolID]);

        m_nDownTraffic += (int)uPakSize;

		if (m_ModuleProtocolFuns[pHeader->wProtocolID])
		{
			m_ModuleProtocolFuns[pHeader->wProtocolID]((BYTE*)pHeader, uPakSize);
		}
		else
		{
			pFunc = m_ProcessProtocolFuns[pHeader->wProtocolID];
			LOG_PROCESS_ERROR(pFunc);

			uTSC = RDTSC();
			nProtocol = pHeader->wProtocolID;
			(this->*pFunc)((BYTE*)pHeader, uPakSize);
			m_ProtocolStat.Record(nProtocol, uPakSize, RDTSC() - uTSC);
		}
	}
     
    bResult = true;
EXIT0:
    if (m_piSocketStream && !nConnectionAlive)
    {
        QLogPrintf(LOG_DEBUG, "[Relay] LastPakTime = %u\n", m_nLastSendPacketTime);
		QLogPrintf(LOG_DEBUG, "[Relay] CurrentTime = %u\n", KSysService::GameTime(NULL));

		QLogPrintf(LOG_ERR, "Coordinator lost, shutdown !\n");

		IKGameServer* piGameServer = g_GetGameServerInterface();
		piGameServer->ShutDown();
    }
    SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KRelayClient::SaveRoleData(KPlayer* pPlayer)
{
    BOOL            bResult      = false;
    BOOL            bRetCode     = false;
    SIZE_T          uRoleDataLen = 0;
    BYTE*           pbyPos       = m_pbySaveRoleBuffer;
    BYTE*           pbyTail      = NULL;

	PROCESS_ERROR(!IS_BZONE_PLAYER(pPlayer->GetId()));

    bRetCode = g_cPlayerDbMgr.SaveRole(*pPlayer, *(KROLE_DATA*)m_pbySaveRoleBuffer, MAX_ROLE_DATA_SIZE, uRoleDataLen);
    LOG_PROCESS_ERROR(bRetCode);

    pbyTail = m_pbySaveRoleBuffer + uRoleDataLen;

    while (pbyPos < pbyTail)
    {
		size_t uSubPakSize = MIN(pbyTail - pbyPos, MAX_ROLE_DATA_PAK_SIZE);

        bRetCode = DoSyncRoleData(pPlayer->GetId(), pbyPos, pbyPos - m_pbySaveRoleBuffer, uSubPakSize);
        LOG_PROCESS_ERROR(bRetCode);

        pbyPos += uSubPakSize;
    }

    bRetCode = DoSaveRoleData(pPlayer, uRoleDataLen);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KRelayClient::Send(IMemBlock* piBuffer)
{
    BOOL    bResult  = false;
    int     nRetCode = false;

    assert(piBuffer);
    LOG_PROCESS_ERROR(m_piSocketStream);

	nRetCode = m_piSocketStream->Send(piBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

    m_nLastSendPacketTime = KSysService::GameTime(NULL);

    m_nUpTraffic += piBuffer->GetSize();

    bResult = true;
EXIT0:
    if (!bResult)
        m_bSendErrorFlag = true;

	return bResult;
}

void KRelayClient::OnHandshakeRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                    bRetCode   = false;
	R2S_HANDSHAKE_RESPOND*  pHandshake = (R2S_HANDSHAKE_RESPOND*)pbyData;

    //assert(g_pWorld->m_piRecorderTools);

    //g_pWorld->m_nBaseTime   = pHandshake->nBaseTime;
    //g_pWorld->m_dwStartTime = g_pWorld->m_piRecorderTools->GetTickCount();;
    //g_pWorld->m_nStartLoop  = pHandshake->nStartFrame;
    //g_pWorld->m_nGameLoop   = pHandshake->nStartFrame;

    m_nWorldIndex = pHandshake->nWorldIndex;
	g_cOrpgWorld.m_bDevMode = pHandshake->nDevMode;

	if (pHandshake->nDevMode)
	{
		QConsoleHelper::DoInfoColor();
		QLogPrintf(LOG_INFO, "Running DevMode, GM Command OPEN", pHandshake->nWorldIndex);
		QConsoleHelper::RestoreColor();
	}

	QLogPrintf(LOG_INFO, "Set world index = %d\n", pHandshake->nWorldIndex);

EXIT0:
	return;
}

void KRelayClient::OnBZoneHandshakeRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_BZONE_HANDSHAKE_RESPOND*  pHandshake = (R2S_BZONE_HANDSHAKE_RESPOND*)pbyData;

	g_cOrpgWorld.m_bDevMode = pHandshake->nDevMode;

EXIT0:
	return;
}

void KRelayClient::OnQuitNotify(BYTE* pbyData, size_t uDataLen)
{
    QLogPrintf(LOG_INFO, "Quit signal from coordinator !");
    PrepareToQuit();
}

void KRelayClient::OnCreateMapNotify(BYTE* pbyData, size_t uDataLen)
{
    BOOL                    bResult                 = false;
	BOOL                    bRetCode                = false;
	R2S_CREATE_MAP_NOTIFY*  pNotify                 = (R2S_CREATE_MAP_NOTIFY*)pbyData;
    KScene*                 pScene                  = NULL;

	pScene = g_SceneMgr.LoadScene(pNotify->dwMapID, pNotify->nMapCopyIndex);
	LOG_PROCESS_ERROR(pScene);

	// Scene Data 未加载

	g_RelayClient.DoCreateMapRespond(pNotify->dwMapID, pNotify->nMapCopyIndex, true);

    bResult = true;
EXIT0:
    if (!bResult)
    {
	    DoCreateMapRespond(pNotify->dwMapID, pNotify->nMapCopyIndex, false);
    }
    return;
}

void KRelayClient::OnLoadMapRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                    bResult                 = false;
	BOOL                    bRetCode                = false;
	R2S_LOAD_MAP_RESPOND*  	pNotify                 = (R2S_LOAD_MAP_RESPOND*)pbyData;
	KScene*                 pScene                  = NULL;
	ScriptSafe				cSafeScript				= g_cScriptManager.GetSafeScript();
	CONST KSCENE_CONFIG* 	pSceneCfg = NULL;

	if (pNotify->nCode != smrcSuccess)
	{
		QLogPrintf(LOG_ERR, "LoadMap Failed %d map %d", pNotify->nCode, pNotify->dwMapID);
		goto EXIT0;
	}
	QLogPrintf(LOG_INFO, "LoadMap map %d param %d", pNotify->dwMapID, pNotify->nParam1);

	pScene = g_SceneMgr.LoadScene(pNotify->dwMapID, pNotify->nMapCopyIndex);
	LOG_PROCESS_ERROR(pScene);

	g_RelayClient.DoCreateMapRespond(pNotify->dwMapID, pNotify->nMapCopyIndex, true);

	pSceneCfg = g_pSceneMgr->GetSceneInfo(pNotify->dwMapID);
	LOG_PROCESS_ERROR(pSceneCfg);

	cSafeScript->CallTableFunction("Scene", "OnLoadDynamicScene", 0, "uuuu",
		pNotify->dwMapID, DYNAMIC_SCENE_ID(pNotify->dwMapID, pNotify->nMapCopyIndex), pSceneCfg->nType, pNotify->nParam1);

	bResult = true;
EXIT0:
	return;
}

void KRelayClient::OnFinishCreateMapNotify(BYTE* pbyData, size_t uDataLen)
{
    R2S_FINISH_CREATE_MAP_NOTIFY* pNotify = (R2S_FINISH_CREATE_MAP_NOTIFY*)pbyData;
    KPlayer*                      pPlayer = NULL;

    pPlayer = g_cPlayerMgr.GetById(pNotify->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

    pPlayer->m_cPlayerServer.DoMessageNotify(ectSwitchMapErrorCode, smrcCreateMapFinished);

EXIT0:
    return;
}

void KRelayClient::OnDeleteMapNotify(BYTE* pbyData, size_t uDataLen)
{
    R2S_DELETE_MAP_NOTIFY* pNotify  = (R2S_DELETE_MAP_NOTIFY*)pbyData;
    KScene*                pScene   = NULL;
	BOOL				   bRetCode = false; 

    pScene = (KScene*)g_SceneMgr.GetScene(pNotify->dwMapID, pNotify->nMapCopyIndex);
    LOG_PROCESS_ERROR(pScene);

    QLogPrintf(LOG_INFO, "Delete map %s(%u, %d) !\n", pScene->m_szName, pNotify->dwMapID, pNotify->nMapCopyIndex);

	g_pSceneMgr->UnloadScene(pNotify->dwMapID, pNotify->nMapCopyIndex);

EXIT0:
	return;
}

void KRelayClient::OnSearchMapRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bRetCode            = false;
	R2S_SEARCH_MAP_RESPOND*     pRespond            = (R2S_SEARCH_MAP_RESPOND*)pbyData;
    KPlayer*                    pPlayer             = NULL;
    KScene*                     pScene              = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsSearchMap);

    if (pRespond->nResultCode != smrcSuccess)
    {
        pPlayer->SetGameState(gsPlaying);
        //pPlayer->m_nRecordCount     = 0;
        
		if (pPlayer->m_eMoveState != cmsOnDeath)
        {
            //pPlayer->m_nCurrentTrack = 0;
            pPlayer->StopMoving();
        }

        pPlayer->m_cPlayerServer.DoMessageNotify(ectSwitchMapErrorCode, pRespond->nResultCode);

        goto EXIT0;
    }

	if (pPlayer->m_eMoveState != cmsOnDeath)
		pPlayer->StopMove(false);

	pPlayer->m_cPlayerServer.ClearRecordPosition();

	if (pRespond->bSwitchServer)
	{
        // 将角色数据存盘,位置保存为跨服的目标点
        pPlayer->m_cPlayerServer.m_SavePosition.nX              = pRespond->nPosX;
        pPlayer->m_cPlayerServer.m_SavePosition.nY              = pRespond->nPosY;
        pPlayer->m_cPlayerServer.m_SavePosition.nZ              = pRespond->nPosZ;
        pPlayer->m_cPlayerServer.m_SavePosition.dwMapID         = pRespond->dwMapID;
        pPlayer->m_cPlayerServer.m_SavePosition.nMapCopyIndex   = pRespond->nMapCopyIndex;
        pPlayer->m_cPlayerServer.m_SavePosition.nFaceDirection  = pPlayer->m_nFaceDirection;
		pPlayer->SetGameState(gsWaitForTransmissionSave);

	    bRetCode = SaveRoleData(pPlayer);
	    LOG_PROCESS_ERROR(bRetCode);

		bRetCode = DoSaveDelayRoleData(pPlayer);
		LOG_PROCESS_ERROR(bRetCode);
	}
    else
	{
        int nMaxX = 0;
        int nMaxY = 0;
		IKScene* pCurrentScene = NULL;

		pScene = (KScene*)g_SceneMgr.GetScene(pRespond->dwMapID, pRespond->nMapCopyIndex);
		LOG_PROCESS_ERROR(pScene);

        nMaxX = pScene->GetRegionWidth() * REGION_GRID_WIDTH * CELL_LENGTH - 1;
        nMaxY = pScene->GetRegionHeight() * REGION_GRID_HEIGHT * CELL_LENGTH - 1;

        MAKE_IN_RANGE(pRespond->nPosX, 0, nMaxX);
        MAKE_IN_RANGE(pRespond->nPosY, 0, nMaxY);
        MAKE_IN_RANGE(pRespond->nPosZ, 0, MAX_Z_POINT);

        pPlayer->m_cPlayerServer.m_SavePosition.nX              = pRespond->nPosX;
        pPlayer->m_cPlayerServer.m_SavePosition.nY              = pRespond->nPosY;
        pPlayer->m_cPlayerServer.m_SavePosition.nZ              = pRespond->nPosZ;
        pPlayer->m_cPlayerServer.m_SavePosition.dwMapID         = pRespond->dwMapID;
        pPlayer->m_cPlayerServer.m_SavePosition.nMapCopyIndex   = pRespond->nMapCopyIndex;
		g_cPlayerBaseProcess.DoSyncLeave(*pPlayer, TRUE);

		pCurrentScene = pPlayer->GetScene();
		if (pCurrentScene)
			pCurrentScene->RemovePlayer(pPlayer);

		// 设置下战斗状态信息
		{
			CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(pScene->GetTemplateId());
			LOG_PROCESS_ERROR(pCfg);
			pPlayer->SetIsFightMap(pCfg->bFightMap);
		}

		bRetCode = pScene->AddPlayer(pPlayer, pRespond->nPosX, pRespond->nPosY, pRespond->nPosZ);
		LOG_PROCESS_ERROR(bRetCode);

		pPlayer->m_cPlayerServer.SyncSelfScene();
		pPlayer->m_cPlayerServer.SyncChangeRegion();
		
		cSafeScript->CallTableFunction("Scene", "OnEnterScene", 0, "d",
			pPlayer->GetId()); 

		pPlayer->SetGameState(gsPlaying);
	}

EXIT0:
	QLogPrintf(LOG_DEBUG, "[%d][%s] SearchMapRespond %d ChangeServer %d map %d,%d",
		pRespond->dwPlayerID, pPlayer ? pPlayer->GetName() : "(null)",
		pRespond->nResultCode,
		pRespond->bSwitchServer, pRespond->dwMapID, pRespond->nMapCopyIndex);
	return;
}

void KRelayClient::OnTransferPlayerRequest(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	R2S_TRANSFER_PLAYER_REQUEST*    pRequest    = (R2S_TRANSFER_PLAYER_REQUEST*)pbyData;
	KPlayer*                        pPlayer     = NULL;
    GUID                            Guid        = {0, 0, 0, 0};

    PROCESS_ERROR(!m_bQuiting);
    //PROCESS_ERROR(m_bSyncDataOver);

    pPlayer = g_cPlayerMgr.GetById(pRequest->dwRoleID);
    LOG_PROCESS_ERROR(pPlayer == NULL);

	pPlayer = g_cPlayerMgr.Add2(pRequest->szRoleName, pRequest->dwRoleID);
	LOG_PROCESS_ERROR(pPlayer);

    bRetCode = pPlayer->m_cPlayerServer.LoadBaseInfo(&pRequest->BaseInfo, uDataLen);
    LOG_PROCESS_ERROR(bRetCode);

    //pPlayer->m_bChargeFlag          = pRequest->bChargeFlag;
    //pPlayer->m_ExtPointInfo         = pRequest->ExtPointInfo;
    //pPlayer->m_bExtPointLock        = pRequest->bExtPointLock;
    //pPlayer->m_nLastExtPointIndex   = pRequest->nLastExtPointIndex;
    //pPlayer->m_nLastExtPointValue   = pRequest->nLastExtPointValue;
    //pPlayer->m_nEndTimeOfFee        = pRequest->nEndTimeOfFee;
    pPlayer->m_cPlayerServer.SetAccountCoin(pRequest->nCoin);
    pPlayer->m_dwSystemTeamId       = pRequest->dwSystemTeamID;
    pPlayer->m_dwTeamId             = pRequest->dwTeamID;
	pPlayer->m_nTimer               = 0;
	pPlayer->SetGameState(gsWaitForConnect);
    //pPlayer->m_nBattleFieldSide     = pRequest->nBattleFieldSide;
    pPlayer->m_dwKinId             = pRequest->dwTongID;
	pPlayer->m_dwKinMasterId		= pRequest->dwKinMasterId;
    //pPlayer->m_bFarmerLimit         = pRequest->byFarmerLimit;

	pPlayer->m_cPlayerServer.m_strAccount = pRequest->szAccount;

    //assert(g_pWorld->m_piRecorderTools);

	bRetCode = QGenerateGUID(&Guid);
	LOG_PROCESS_ERROR(bRetCode);

    pPlayer->m_cPlayerServer.m_Guid = Guid;

    bResult = true;
EXIT0:
	DoTransferPlayerRespond(pRequest->dwRoleID, bResult, Guid);

    if (!bResult)
    {
        if (pPlayer)
        {
			DoPlayerLeaveGS(pPlayer->GetId());

			if (pPlayer->GetScene())
			{
				pPlayer->GetScene()->RemovePlayer(pPlayer);
			}

			g_cPlayerMgr.RemoveById(pPlayer->GetId());

			//pPlayer->m_cPlayerServer.KickOut(emKICK_PLAYER_REASON_BUG_CHECK);
        }
    }

	return;
}

void KRelayClient::OnTransferPlayerRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bRetCode    = false;
	R2S_TRANSFER_PLAYER_RESPOND*    pRespond    = (R2S_TRANSFER_PLAYER_RESPOND*)pbyData;
    KPlayer*                        pPlayer     = NULL;

    // 跨服中断线就可能找不到
	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	PROCESS_ERROR(pPlayer);

    LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsWaitForTransmissionGuid);

    if (!pRespond->bSucceed)
    {
        QLogPrintf(LOG_INFO, "Switch GS failed, shutdown(%d)\n", pPlayer->GetPlayerIndex());

		g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
        goto EXIT0;
    }

	g_cPlayerBaseProcess.DoSwitchGameServer(pPlayer, pRespond->Guid, pRespond->dwAddress, pRespond->wPort);
	QLogPrintf(LOG_INFO, "OnTransferPlayerRespond %s @ %s:%d allowed",
		pPlayer->GetName(), inet_ntoa(*(struct in_addr*)&pRespond->dwAddress), pRespond->wPort);

	pPlayer->SetGameState(gsDeleting);

EXIT0:
	return;
}

void KRelayClient::OnBZoneTransferPlayerRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bRetCode    = false;
	R2S_BZONE_TRANSFER_PLAYER_RESPOND*    pRespond    = (R2S_BZONE_TRANSFER_PLAYER_RESPOND*)pbyData;
	KPlayer*                        pPlayer     = NULL;

	// 跨服中断线就可能找不到
	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsWaitForTransmissionGuid);

	if (!pRespond->bSucceed)
	{
		pPlayer->SetGameState(gsPlaying);

		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, pPlayer->GetId(), "DEBUG", "跨服服务器人数已满，请稍后再试", emQCHAT_PANEL);
		/*
		QLogPrintf(LOG_INFO, "Switch GS failed, shutdown(%d)\n", pPlayer->GetPlayerIndex());

		g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
		*/
		goto EXIT0;
	}

	QLogPrintf(LOG_INFO, "OnTransferPlayerRespond %s @ %s:%d allowed",
		pPlayer->GetName(), inet_ntoa(*(struct in_addr*)&pRespond->dwAddress), pRespond->wPort);

	g_cPlayerBaseProcess.DoSwitchBZoneGameServer(pPlayer, pRespond->dwDstPlayerID, pRespond->Guid, pRespond->dwAddress, pRespond->wPort);

	pPlayer->SetGameState(gsDeleting);

EXIT0:
	return;
}

void KRelayClient::OnBZoneReturnRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bRetCode    = false;
	R2S_BZONE_RETURN_RESPOND*		pRespond    = (R2S_BZONE_RETURN_RESPOND*)pbyData;
	KPlayer*                        pPlayer     = NULL;

	// 跨服中断线就可能找不到
	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsWaitForTransmissionGuid);

	if (!pRespond->bSucceed)
	{
		QLogPrintf(LOG_INFO, "Switch GS failed, shutdown(%d)\n", pPlayer->GetPlayerIndex());

		g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
		goto EXIT0;
	}

	g_cPlayerBaseProcess.DoBZoneReturnGameServer(pPlayer, pRespond->dwDstPlayerID, pRespond->Guid, pRespond->dwAddress, pRespond->wPort);
	QLogPrintf(LOG_INFO, "OnTransferPlayerRespond %s @ %s:%d allowed",
		pPlayer->GetName(), inet_ntoa(*(struct in_addr*)&pRespond->dwAddress), pRespond->wPort);

	pPlayer->SetGameState(gsDeleting);

EXIT0:
	return;
}

void KRelayClient::OnGetMultiAwardRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bRetCode    = false;
	R2S_GET_MULTI_AWARD_RESPOND*		pRespond    = (R2S_GET_MULTI_AWARD_RESPOND*)pbyData;
	KPlayer*                        pPlayer     = NULL;
	KInstanceCtrl* pInstanceCtrl = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwRoleId);
	PROCESS_ERROR(pPlayer);

	PROCESS_ERROR(pRespond->nStar > 0);

	pInstanceCtrl = pPlayer->GetCtrl<KInstanceCtrl>();
	LOG_PROCESS_ERROR(pInstanceCtrl);

	pInstanceCtrl->SetMultiMapPassed(pRespond->nMapId);

	bRetCode = cSafeScript->CallTableFunction("MultiInstance", "AddMultiAward", 0, "odd", pPlayer->GetScriptInterface(), pRespond->nMapId, pRespond->nStar);

EXIT0:
	return;
}

void KRelayClient::OnGetCrossSvExDataRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bRetCode    = false;
	R2S_GET_CROSSSV_EXDATA_RESPOND*		pRespond    = (R2S_GET_CROSSSV_EXDATA_RESPOND*)pbyData;
	KPlayer*                        pPlayer     = NULL;
	KCrossSvCtrl* pCrossSvCtrl = NULL;

	pPlayer = g_cPlayerMgr.GetById(pRespond->dwRoleId);
	PROCESS_ERROR(pPlayer);

	pCrossSvCtrl = pPlayer->GetCtrl<KCrossSvCtrl>();
	LOG_PROCESS_ERROR(pCrossSvCtrl);

	bRetCode = pCrossSvCtrl->LoadCrossSvDatas(pRespond->byExData, pRespond->uExDataLen);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}


void KRelayClient::OnPlayerLoginRequest(BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bRetCode            = false;
	BOOL                        bResult             = false;
	KPlayer*                    pPlayer             = NULL;
    R2S_PLAYER_LOGIN_REQUEST*   pRequest            = (R2S_PLAYER_LOGIN_REQUEST*)pbyData;
    GUID                        guid                = {0, 0, 0, 0};

    PROCESS_ERROR(!m_bQuiting);

	pPlayer = g_cPlayerMgr.Add2(pRequest->szRoleName, pRequest->dwRoleID);
	LOG_PROCESS_ERROR(pPlayer);

    bRetCode = pPlayer->m_cPlayerServer.LoadBaseInfo(&pRequest->BaseInfo, uDataLen);
    LOG_PROCESS_ERROR(bRetCode);

    //pPlayer->m_bChargeFlag      = (BOOL)pRequest->nChargeFlag;
    //pPlayer->m_ExtPointInfo     = pRequest->ExtPointInfo;
    //pPlayer->m_nEndTimeOfFee    = pRequest->nEndTimeOfFee;
    pPlayer->m_cPlayerServer.SetAccountCoin(pRequest->nCoin);
    pPlayer->m_dwSystemTeamId   = pRequest->dwSystemTeamID;
    pPlayer->m_dwTeamId         = pRequest->dwTeamID;
    pPlayer->m_dwKinId         = pRequest->dwTongID;
	pPlayer->m_dwKinMasterId  = pRequest->dwKinMasterId;
    //pPlayer->m_bFarmerLimit     = pRequest->byFarmerLimit;
    pPlayer->m_nTimer           = 0;
    //pPlayer->m_nBattleFieldSide = pRequest->nBattleSide;
	pPlayer->SetGameState(gsWaitForConnect);

	pPlayer->m_cPlayerServer.m_strAccount = pRequest->szAccount;

    //assert(g_pWorld->m_piRecorderTools);

	bRetCode = QGenerateGUID(&guid);
    LOG_PROCESS_ERROR(bRetCode);

    pPlayer->m_cPlayerServer.m_Guid = guid;

    //g_pWorld->m_StatDataServer.UpdateClientLoginPermit();

	bResult = true;
EXIT0:
	DoPlayerLoginRespond(pRequest->dwRoleID, bResult, guid, pRequest->dwPacketIdentity);

	QLogPrintf(LOG_DEBUG, "LoginRequest [%d][%s] result %d",
		pRequest->dwRoleID,
		pPlayer ? pPlayer->m_cPlayerServer.GetAccount() : "",
		bResult
		);

    if (!bResult)
    {
        if (pPlayer)
        {
			g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
            //g_pWorld->DelPlayer(pPlayer);
            pPlayer = NULL;
        }
    }

	return;
}

void KRelayClient::OnConfirmPlayerLoginRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_CONFIRM_PLAYER_LOGIN_RESPOND*   pRespond = (R2S_CONFIRM_PLAYER_LOGIN_RESPOND*)pbyData;
    KPlayer*                            pPlayer  = NULL;

	// 登陆期间，客户端断线，就有可能在这里找不到
	pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pPlayer);

	LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsWaitForPermit);

    //g_LogClient.LogPlayerLogin(pPlayer->m_szAccount, pPlayer->m_szName, pPlayer->m_szClientIP);

    if (!pRespond->bPermit)
    {
        QLogPrintf(LOG_INFO, "Login denied by coordinator, shutdown(%d)\n", pPlayer->GetPlayerIndex());

		g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
        goto EXIT0;
    }

	DoLoadRoleDataRequest(pRespond->dwPlayerID);

	pPlayer->SetGameState(gsWaitForRoleData);
    pPlayer->m_nTimer       = 0;
    //pPlayer->m_nRecordCount = 0;

EXIT0:
	return;
}

void KRelayClient::OnKickAccountNotify(BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bRetCode    = false;
	R2S_KICK_ACCOUNT_NOTIFY*    pNotify     = (R2S_KICK_ACCOUNT_NOTIFY*)pbyData;
    KPlayer*                    pPlayer     = NULL;

	pPlayer = g_cPlayerMgr.GetById(pNotify->dwPlayerID);
	PROCESS_ERROR(pPlayer);

	//g_PlayerServer.DoAccountKickout(pPlayer->GetPlayerIndex());

    //if (pPlayer->GetPlayerIndex() >= 0)
    {
        QLogPrintf(LOG_INFO, "Kick account from coordinator, shutdown(%d)\n", pPlayer->GetPlayerIndex());

		pPlayer->m_cPlayerServer.KickOut(emKICK_PLAYER_REASON_NETWORK);
    }

EXIT0:
	return;
}

void KRelayClient::OnPlayerChatWhisperSuccessRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                                        bRetCode    = false;
	R2S_PLAYER_CHAT_WHISPER_SUCCESS_RESPOND*    pPak        = (R2S_PLAYER_CHAT_WHISPER_SUCCESS_RESPOND*)pbyData;

EXIT0:
	return;
}

void KRelayClient::OnPlayerChatError(BYTE* pbyData, size_t uDataLen)
{
    BOOL                     bRetCode         = false;
    KPlayer*                 pPlayer          = NULL;
    int                      nPlayerConnIndex = -1;
    R2S_PLAYER_CHAT_ERROR*   pPak             = (R2S_PLAYER_CHAT_ERROR*)pbyData;

    pPlayer = g_cPlayerMgr.GetById(pPak->dwReceiverId);
    LOG_PROCESS_ERROR(pPlayer);

    nPlayerConnIndex = pPlayer->GetPlayerIndex();
    LOG_PROCESS_ERROR(nPlayerConnIndex >= 0);

    bRetCode = pPlayer->m_cPlayerServer.DoMessageNotify(ectTalkErrorCode, pPak->nErrorCode);
    LOG_PROCESS_ERROR(bRetCode);

EXIT0:
    return;
}

void KRelayClient::OnChatMessage(BYTE* pbyData, size_t uDataLen)
{
	R2S_CHAT_MESSAGE*			pTalkMsg			= (R2S_CHAT_MESSAGE*)pbyData;

	g_ChatChannelManager.RequestTransferToClient(pTalkMsg->byMsgType, 
		pTalkMsg->dwSenderId,
		pTalkMsg->dwReceiverId,
		pTalkMsg->szSender, 
		(LPCTSTR)pTalkMsg->byTalkData, pTalkMsg->byShownType);
    
    return;
}

struct KClearPlayerPrepareFunc
{
    int m_nDelayTime;
    int m_nType;

    BOOL operator () (KPlayer* pPlayer) const
    {
        pPlayer->m_cPlayerServer.DoMessageNotify(ectBanishNotifyCode, m_nType, &m_nDelayTime, sizeof(m_nDelayTime));

        return true;
    }
};

void KRelayClient::OnMapCopyDoClearPlayerPrepare(BYTE* pbyData, size_t uDataLen)
{   
    KScene*			                        pScene          = NULL;
    R2S_MAP_COPY_DO_CLEAR_PLAYER_PREPARE*   pPrepareClear   = (R2S_MAP_COPY_DO_CLEAR_PLAYER_PREPARE*)pbyData;
    KClearPlayerPrepareFunc                 Func;

    Func.m_nDelayTime = pPrepareClear->nDelayTime;
    Func.m_nType      = pPrepareClear->nType;

    //pScene = g_pWorld->GetScene(pPrepareClear->dwMapID, pPrepareClear->nMapCopyIndex);
    //LOG_PROCESS_ERROR(pScene);

    //pScene->TraversePlayer(Func);

EXIT0:
    return;
}

void KRelayClient::OnMapCopyDoClearPlayer(BYTE* pbyData, size_t uDataLen)
{
	R2S_MAP_COPY_DO_CLEAR_PLAYER*   pClear    = (R2S_MAP_COPY_DO_CLEAR_PLAYER*)pbyData;
	TKTraverseHelper<KPlayer> cTravHelper;

	IKScene* pScene = g_pSceneMgr->GetScene(pClear->dwMapID, pClear->nMapCopyIndex);
	LOG_PROCESS_ERROR(pScene);

	pScene->TraverseAllPlayers(cTravHelper);
	for(std::vector<KPlayer*>::iterator it = cTravHelper.m_vObjList.begin();
		it != cTravHelper.m_vObjList.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		LOG_PROCESS_ERROR(pPlayer);

		if (pPlayer->GetGameState() == gsPlaying) // 只处理正常状态下的玩家
		{
			pPlayer->m_cPlayerServer.ReturnToLastCity();
			if (pPlayer->GetGameState() != gsSearchMap)
			{
				Q_Error("Unable to send [" << pPlayer->GetName() << "] back to city, kickout...");
				pPlayer->m_cPlayerServer.KickOut(emKICK_PLAYER_REASON_BUG_CHECK);
			}
		}
		
	}

EXIT0:
	QLogPrintf(LOG_DEBUG, "MapCopyDoClearPlayer %d index %d", pClear->dwMapID, pClear->nMapCopyIndex);
    return;
}

void KRelayClient::OnSetMapCopyOwner(BYTE* pbyData, size_t uDataLen)
{
    R2S_SET_MAP_COPY_OWNER* pSetOwner = (R2S_SET_MAP_COPY_OWNER*)pbyData;
    KScene*                 pScene    = NULL;

    pScene = (KScene*)g_SceneMgr.GetScene(pSetOwner->dwMapID, pSetOwner->nMapCopyIndex);
    LOG_PROCESS_ERROR(pScene);

    pScene->m_dwOwnerID = pSetOwner->dwOwnerID;

EXIT0:
    return;
}

void KRelayClient::OnSaveRoleDataRespond(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bRetCode        = false;
    int                             nStateForSaving = 0;
    KPlayer*                        pPlayer         = NULL;
    R2S_SAVE_ROLE_DATA_RESPOND*     pRespond        = (R2S_SAVE_ROLE_DATA_RESPOND*)pbyData;

    m_nNextQuitingSaveTime = 0;

    pPlayer = g_cPlayerMgr.GetById(pRespond->dwPlayerID);
    if (pPlayer != NULL &&  pRespond->nUserValue == gsWaitForTransmissionSave)
    {
        LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsWaitForTransmissionSave);
        
        if (!pRespond->bSucceed)
        {
            QLogPrintf(
                LOG_ERR, "Transmission save failed: ID = %u, Name = %s\n",
                pPlayer->GetId(), pPlayer->GetName()
            );

			g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
            goto EXIT0;
        }

        g_RelayClient.DoTransferPlayerRequest(pPlayer);
        pPlayer->SetGameState(gsWaitForTransmissionGuid);
    }
	else if (pPlayer != NULL &&  pRespond->nUserValue == gsWaitForBZoneTransferSave)
	{
		LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsWaitForBZoneTransferSave);

		if (!pRespond->bSucceed)
		{
			QLogPrintf(
				LOG_ERR, "BZoneTransfer save failed: ID = %u, Name = %s\n",
				pPlayer->GetId(), pPlayer->GetName()
				);

			g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
			goto EXIT0;
		}

		g_RelayClient.DoBZoneTransferRequest(pPlayer, pRespond->uRoleDataLen, pRespond->nUserValue2);
		pPlayer->SetGameState(gsWaitForTransmissionGuid);
	}

EXIT0:
	QLogPrintf(LOG_DEBUG, "SaveRoleRespond [%d] %s", pRespond->dwPlayerID, pRespond->bSucceed ? "succeeded" : "failed");
    return;
}

void KRelayClient::OnSyncRoleData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                bResult         = false;
    BOOL                bRetCode        = false;
    R2S_SYNC_ROLE_DATA* pSyncData       = (R2S_SYNC_ROLE_DATA*)pbyData;
    BYTE*               pbyOffset       = m_pbySyncRoleBuffer + m_uSyncRoleOffset;
    size_t              uRoleDataLen    = uDataLen - sizeof(R2S_SYNC_ROLE_DATA);

    LOG_PROCESS_ERROR(pSyncData->uOffset == m_uSyncRoleOffset);
    LOG_PROCESS_ERROR(MAX_ROLE_DATA_SIZE - m_uSyncRoleOffset >= uRoleDataLen);
    LOG_PROCESS_ERROR(pSyncData->uOffset == 0 || pSyncData->dwRoleID == m_dwSyncRoleID);

    memcpy(pbyOffset, pSyncData->byData, uRoleDataLen);

    m_uSyncRoleOffset += uRoleDataLen;
    m_dwSyncRoleID     = pSyncData->dwRoleID;

    bResult = true;    
EXIT0:
    if(!bResult)
    {
        m_dwSyncRoleID    = KD_BAD_ID;
        m_uSyncRoleOffset = 0;
    }
    return;
}

void KRelayClient::OnLoadRoleData(BYTE* pbyData, size_t uDataLen)
{
    BOOL                bResult   = false;
    BOOL                bRetCode  = false;
    KPlayer*            pPlayer   = NULL;
    R2S_LOAD_ROLE_DATA* pNotify   = (R2S_LOAD_ROLE_DATA*)pbyData;
    IKScene*             pScene    = NULL;
	IMemBlock* piBuffer = NULL;

    LOG_PROCESS_ERROR(pNotify->uRoleDataLen == m_uSyncRoleOffset);
    LOG_PROCESS_ERROR(pNotify->dwRoleID == m_dwSyncRoleID || m_dwSyncRoleID == KD_BAD_ID);

    pPlayer = g_cPlayerMgr.GetById(pNotify->dwRoleID);
    LOG_PROCESS_ERROR(pPlayer);

    LOG_PROCESS_ERROR(pNotify->bSucceed);

    // 注意: 由于取角色数据是异步的,所以,有可能回包到达时已经是玩家断线后的下一次登陆了
    LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsWaitForRoleData);

	// 新角色无数据
	if (pNotify->uRoleDataLen)
	{
		piBuffer = QCreateMemBlock(pNotify->uRoleDataLen);
		LOG_PROCESS_ERROR(piBuffer);

		memcpy(piBuffer->GetData(), m_pbySyncRoleBuffer, pNotify->uRoleDataLen);
	}

	bResult = pPlayer->m_cPlayerServer.LoginServer2(piBuffer);
	LOG_PROCESS_ERROR(bResult);

    bResult = true;
EXIT0:
    m_uSyncRoleOffset = 0;
    m_dwSyncRoleID    = KD_BAD_ID;
	SAFE_RELEASE(piBuffer);

    // 这里失败不调用detach是因为玩家可能是第二次登录,玩家状态是waitforconnect
    return;
}

void KRelayClient::OnPayOrderRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_PAY_ORDER_RESPOND*		pRespond		= NULL;
	KPlayer* pPlayer = NULL;
	KPlayerPurse* pPlayerPurse = NULL;
	KE_PURSE_RESULT eResult;
	PlayerVip* pPlayerVip = NULL;
	INT nExp = 0;
	PlayerAwardLobby* pPlayerAwardLobby;

	pRespond = (R2S_PAY_ORDER_RESPOND*)pbyData;
	LOG_PROCESS_ERROR(pRespond);
	pPlayer = g_cPlayerMgr.GetById(pRespond->dwRoleId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	LOG_PROCESS_ERROR(pPlayerPurse);
	pPlayerPurse->AddMoney(EM_MONEY_COIN, pRespond->dwGold, &eResult);

	pPlayerVip = pPlayer->GetCtrl<PlayerVip>();
	LOG_PROCESS_ERROR(pPlayerVip);
	nExp = pPlayerVip->GetExp();
	if (nExp == 0)
		g_cAwardMgr.AddPlayerAward(pPlayer,"FirstRecharge", 1);
	else
		g_cAwardMgr.AddPlayerAward(pPlayer,"FirstRecharge", 2);
	
	pPlayerVip->AddExp(pRespond->dwGold);
	pPlayerAwardLobby = pPlayer->GetCtrl<PlayerAwardLobby>();
	LOG_PROCESS_ERROR(pPlayerAwardLobby);
	pPlayerAwardLobby->DoRecharge(pRespond->dwGold);

	DoPayOrderFinishRequest(pRespond->dwRoleId, pRespond->szOrderno);

EXIT0:
	return;
}


//支付完成发奖励
void KRelayClient::OnPayFinish(DWORD dwRoleId, INT nType,DWORD dwGold)
{
	KPlayer* pPlayer = NULL;
	KPlayerPurse* pPlayerPurse = NULL;
	PlayerVip* pPlayerVip = NULL;
	PlayerAwardLobby* pPlayerAwardLobby;
	pPlayer = g_cPlayerMgr.GetById(dwRoleId);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerPurse = pPlayer->GetCtrl<KPlayerPurse>();
	LOG_PROCESS_ERROR(pPlayerPurse);
	pPlayerVip = pPlayer->GetCtrl<PlayerVip>();
	LOG_PROCESS_ERROR(pPlayerVip);
	LOG_PROCESS_ERROR(nType == 1 ||  nType == 2);
	g_cAwardMgr.AddPlayerAward(pPlayer,"FirstRecharge", nType);
	pPlayerAwardLobby = pPlayer->GetCtrl<PlayerAwardLobby>();
	LOG_PROCESS_ERROR(pPlayerAwardLobby);
	pPlayerAwardLobby->DoRecharge(dwGold);
EXIT0:
	return;
}





BOOL KRelayClient::DoPayOrderRequest(DWORD dwRoleID)
{
	BOOL						bRetCode = FALSE;
	S2R_PAY_ORDER_REQUEST*		pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_PAY_ORDER_REQUEST));
	pRequest = (S2R_PAY_ORDER_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_pay_order_request;
	pRequest->dwRoleId = dwRoleID;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bRetCode;
}

BOOL KRelayClient::DoPayOrderFinishRequest(DWORD dwRoleID, CHAR* szOrderno)
{
	BOOL								bRetCode = FALSE;
	S2R_PAY_ORDER_FINISH_REQUEST*		pRequest = NULL;
	IMemBlock*							piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_PAY_ORDER_FINISH_REQUEST));
	pRequest = (S2R_PAY_ORDER_FINISH_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_pay_order_finish_request;
	pRequest->dwRoleId = dwRoleID;
	strncpy(pRequest->szOrderno, szOrderno, 64);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bRetCode;
}


void KRelayClient::OnDelayLoadRoleData(BYTE* pbyData, size_t uDataLen)
{
	BOOL                bResult   = false;
	R2S_DELAY_LOAD_ROLE_DATA* pNotify   = (R2S_DELAY_LOAD_ROLE_DATA*)pbyData;
	IMemBlock* piBuffer = NULL;
	KPlayer* pPlayer = NULL;

	LOG_PROCESS_ERROR(pNotify->bSucceed);
	
	piBuffer = QCreateMemBlock(pNotify->uDelayDataLen);
	LOG_PROCESS_ERROR(piBuffer);

	memcpy(piBuffer->GetData(), pNotify->byDelayData, pNotify->uDelayDataLen);
	
	switch(pNotify->byModuleId)
	{
	case emKMODULE_NONE:
		pPlayer = g_cPlayerMgr.GetById(pNotify->dwSrcId);
		LOG_PROCESS_ERROR(pPlayer);
		pPlayer->m_cPlayerServer.OnDelayLoadRespond(piBuffer, pNotify->dwSrcId, pNotify->dwRoleId);
		break;
	case emMODULE_ARENA:
		g_ArenaModule.OnDelayLoadRespond(piBuffer, pNotify->dwSrcId, pNotify->dwRoleId);
		break;
	case emMODULE_RANK:
		g_cRankManager.OnDelayLoadRespond(piBuffer, pNotify->dwSrcId, pNotify->dwRoleId);
		break;
	case emMODULE_ESCORT:
		g_cEscortModule.OnDelayLoadRespond(piBuffer, pNotify->dwSrcId, pNotify->dwRoleId);
		break;
	case emMODULE_ACTIVITY:
		g_cActivityManager.OnDelayLoadRespond(piBuffer, pNotify->dwSrcId, pNotify->dwRoleId);
		break;
	case MODULE_OFFLINE_FIGHT:
		g_cArenaHelper.OnDelayLoadRespond(piBuffer, pNotify->dwSrcId, pNotify->dwRoleId);
		break;
	default:
		ASSERT(FALSE);
	}

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);

	return;
}

void KRelayClient::OnRemoteLuaCall(BYTE* pbyData, size_t uDataLen)
{
	BOOL				bRetCode  = FALSE;
	R2S_REMOTE_LUA_CALL* pLuaCall = (R2S_REMOTE_LUA_CALL*)pbyData;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	LOG_PROCESS_ERROR(uDataLen == sizeof(R2S_REMOTE_LUA_CALL) + pLuaCall->uParamLen);

	bRetCode = cSafeScript->LoadBuffer2Value(pLuaCall->byParam, pLuaCall->uParamLen);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = cSafeScript->CallTableFunction("ServerEvent", "OnGlobalExecute", 0,
		"v", cSafeScript.GetBeginTopIndex() + 1
		);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	return;
}

//AutoCode:-处理协议函数结束-

/************************************************************************/

BOOL KRelayClient::DoHandshakeRequest()
{
    BOOL                    bResult         = false;
	BOOL                    bRetCode        = false;
	IMemBlock*             piPackage       = NULL;
    S2R_HANDSHAKE_REQUEST*  pHandshake      = NULL;

	piPackage = QCreateMemBlock((unsigned)sizeof(S2R_HANDSHAKE_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pHandshake = (S2R_HANDSHAKE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pHandshake);

	pHandshake->wProtocolID             = s2r_handshake_request;
    pHandshake->nGameWorldLowerVersion  = KD_PROTOCOL_VERSION;
    pHandshake->nGameWorldUpperVersion  = KD_PROTOCOL_VERSION_LOWER_LIMIT;

	pHandshake->nServerTime = KSysService::GameTime(NULL);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoPingSignal()
{
	BOOL						bResult				= false;
	BOOL						bRetCode			= false;
	IMemBlock*                 piSendBuffer        = NULL;
	S2R_PING_SIGNAL*			pPingSingal			= NULL;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_PING_SIGNAL));
	LOG_PROCESS_ERROR(piSendBuffer);

	pPingSingal = (S2R_PING_SIGNAL*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pPingSingal);

	pPingSingal->wProtocolID       = s2r_ping_signal;
	pPingSingal->dwReserved        = 0;

	bRetCode = Send(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);   

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KRelayClient::DoUpdatePerformance()
{
	BOOL						bResult				= false;
	BOOL						bRetCode			= false;
	IMemBlock*                 piSendBuffer        = NULL;
	S2R_UPDATE_PERFORMANCE*	    pPerform			= NULL;
	KSOCKET_KINGEYES_INFO		sSockInfo;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_UPDATE_PERFORMANCE));
	LOG_PROCESS_ERROR(piSendBuffer);

	pPerform = (S2R_UPDATE_PERFORMANCE*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pPerform);

	ZeroStruct(sSockInfo);
	g_piSocketServer->GetSocketInfo(&sSockInfo);

	pPerform->wProtocolID       = s2r_update_performance;
	pPerform->nIdleFrame        = g_cOrpgWorld.m_nIdleFrame;
    pPerform->nPlayerCount      = g_cPlayerMgr.GetPlayerCount();
    pPerform->nConnectionCount  = sSockInfo.dwCurrentConnection;
	pPerform->uNetworkFlux      = sSockInfo.dwSendBytesPerSecond;
	pPerform->uMemory           = _GetUsedMemSize();

	bRetCode = Send(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);   

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KRelayClient::DoPlayerLoginRespond(DWORD dwPlayerID, BOOL bPermit, GUID Guid, DWORD dwPacketIdentity)
{
    BOOL                        bResult     = false;
	BOOL                        bRetCode    = false;
	IMemBlock*                 piPackage   = NULL;
    S2R_PLAYER_LOGIN_RESPOND*   pRespond    = NULL;

    piPackage = QCreateMemBlock(sizeof(S2R_PLAYER_LOGIN_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

    pRespond = (S2R_PLAYER_LOGIN_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID       = s2r_player_login_respond;
	pRespond->dwPacketIdentity  = dwPacketIdentity;
	pRespond->bPermit           = bPermit;
	pRespond->dwPlayerID        = dwPlayerID;
	pRespond->Guid              = Guid;

	KHOST_ADDRESS sHost;
	g_piSocketServer->GetHostIP(sHost);
	pRespond->dwGSAddr          = inet_addr(sHost.szInternetIp);
	pRespond->nGSPort           = sHost.nListenPort;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoConfirmPlayerLoginRequest(DWORD dwPlayerID)
{
    BOOL                                bResult     = false;
	BOOL                                bRetCode    = false;
    IMemBlock*                         piPackage   = NULL;
    S2R_CONFIRM_PLAYER_LOGIN_REQUEST*   pRequest    = NULL;

    piPackage = QCreateMemBlock(sizeof(S2R_CONFIRM_PLAYER_LOGIN_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (S2R_CONFIRM_PLAYER_LOGIN_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID         = s2r_confirm_player_login_request;
	pRequest->dwPlayerID          = dwPlayerID;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoPlayerLeaveGS(DWORD dwPlayerID)
{
    BOOL                    bResult     = false;
	BOOL                    bRetCode    = false;
    IMemBlock*             piPackage   = NULL;
    S2R_PLAYER_LEAVE_GS*    pLeaveGS    = NULL;

    piPackage = QCreateMemBlock(sizeof(S2R_PLAYER_LEAVE_GS));
	LOG_PROCESS_ERROR(piPackage);

	pLeaveGS = (S2R_PLAYER_LEAVE_GS*)piPackage->GetData();
	LOG_PROCESS_ERROR(pLeaveGS);

	pLeaveGS->wProtocolID         = s2r_player_leave_gs;
	pLeaveGS->dwPlayerID          = dwPlayerID;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoCreateMapRespond(DWORD dwMapID, int nMapCopyIndex, BOOL bSucceed)
{
    BOOL                    bResult     = false;
	BOOL                    bRetCode    = false;
    IMemBlock*             piPackage   = NULL;
    S2R_CREATE_MAP_RESPOND* pRespond    = NULL;
    //KMapParams*             pMapParam   = NULL;

    //pMapParam = g_pWorld->m_Settings.m_MapListFile.GetMapParamByID(dwMapID);
    //LOG_PROCESS_ERROR(pMapParam);

    //QLogPrintf(
    //    LOG_INFO, "Create map %s(%.2u, %d) ... ... [%s]\n", 
    //    pMapParam->szMapName, dwMapID, nMapCopyIndex, bSucceed ? "OK" : "Failed"
    //);

	piPackage = QCreateMemBlock(sizeof(S2R_CREATE_MAP_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespond = (S2R_CREATE_MAP_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID        = s2r_create_map_respond;
	pRespond->dwMapID            = dwMapID;
	pRespond->nMapCopyIndex      = nMapCopyIndex;
	pRespond->bResult            = bSucceed;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoSearchMapRequest(DWORD dwPlayerID, DWORD dwMapID, int nMapCopyIndex, int nX, int nY, int nZ, BOOL bOnThisGs /* = FALSE */)
{
    BOOL                        bResult             = false;
	BOOL                        bRetCode            = false;
    IMemBlock*                 piSendBuffer        = NULL;
	S2R_SEARCH_MAP_REQUEST*     pSearchMapRequest   = NULL;

    piSendBuffer = QCreateMemBlock(sizeof(S2R_SEARCH_MAP_REQUEST));
    LOG_PROCESS_ERROR(piSendBuffer);

    pSearchMapRequest = (S2R_SEARCH_MAP_REQUEST*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pSearchMapRequest);

	pSearchMapRequest->wProtocolID       = s2r_search_map_request;
	pSearchMapRequest->dwPlayerID        = dwPlayerID;
	pSearchMapRequest->dwMapID           = dwMapID;
    pSearchMapRequest->nMapCopyIndex     = nMapCopyIndex;
    pSearchMapRequest->nPosX             = nX;
    pSearchMapRequest->nPosY             = nY;
    pSearchMapRequest->nPosZ             = nZ;
	pSearchMapRequest->bOnThisGs		 = bOnThisGs;

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KRelayClient::DoDeleteBattleMapRequest(DWORD dwMapID, DWORD dwMapCopyIndex)
{
	BOOL                       bResult             = false;
	BOOL                       bRetCode            = false;
	IMemBlock*                 piSendBuffer        = NULL;
	S2R_DELETE_BATTLE_MAP*	   pRequest			= NULL;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_DELETE_BATTLE_MAP));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRequest = (S2R_DELETE_BATTLE_MAP*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID       = s2r_delete_battle_map_request;
	pRequest->dwMapId           = dwMapID;
	pRequest->dwMapCopyIndex    = dwMapCopyIndex;

	bRetCode = Send(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KRelayClient::DoLoadMapRequest(DWORD dwMapID, DWORD dwMapOwnerId, DWORD dwParam1, BOOL bToThisServer)
{
	BOOL                        bResult             = false;
	BOOL                        bRetCode            = false;
	IMemBlock*                 piSendBuffer        = NULL;
	S2R_LOAD_MAP_REQUEST*		pRequest			= NULL;

	piSendBuffer = QCreateMemBlock(sizeof(S2R_LOAD_MAP_REQUEST));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRequest = (S2R_LOAD_MAP_REQUEST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID       = s2r_load_map_request;
	pRequest->dwMapID           = dwMapID;
	pRequest->dwMapOwnerId      = dwMapOwnerId;
	pRequest->dwParam1          = dwParam1;
	pRequest->bToThisServer     = bToThisServer;

	bRetCode = Send(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KRelayClient::DoTransferPlayerRequest(KPlayer* pPlayer)
{
    BOOL                            bResult         = false;
	BOOL                            bRetCode        = false;
	IMemBlock*                     piPackage       = NULL;
    S2R_TRANSFER_PLAYER_REQUEST*    pRequest        = NULL;
    size_t                          uUsedSize       = 0;

	piPackage = QCreateMemBlock(sizeof(S2R_TRANSFER_PLAYER_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (S2R_TRANSFER_PLAYER_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

    pRequest->wProtocolID           = s2r_transfer_player_request;
    pRequest->dwRoleID              = pPlayer->GetId();
    //pRequest->bChargeFlag           = pPlayer->m_bChargeFlag;
    //pRequest->ExtPointInfo          = pPlayer->m_ExtPointInfo;
    //pRequest->bExtPointLock         = pPlayer->m_bExtPointLock;
    //pRequest->nLastExtPointIndex    = pPlayer->m_nLastExtPointIndex;
    //pRequest->nLastExtPointValue    = pPlayer->m_nLastExtPointValue;
    //pRequest->nEndTimeOfFee         = pPlayer->m_nEndTimeOfFee;
    pRequest->nCoin                 = pPlayer->m_cPlayerServer.GetAccountCoin();
    //pRequest->nBattleFieldSide      = pPlayer->m_nBattleFieldSide;

    bRetCode  = pPlayer->m_cPlayerServer.SaveBaseInfo(&pRequest->RoleBaseInfo);
    LOG_PROCESS_ERROR(bRetCode);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoTransferPlayerRespond(DWORD dwPlayerID , BOOL bSucceed, GUID Guid)
{
    BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piPackage   = NULL;
    S2R_TRANSFER_PLAYER_RESPOND*    pRespond    = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_TRANSFER_PLAYER_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespond = (S2R_TRANSFER_PLAYER_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID       = s2r_transfer_player_respond;
	pRespond->dwPlayerID        = dwPlayerID;
	pRespond->bSucceed          = bSucceed;
	pRespond->Guid              = Guid;

	KHOST_ADDRESS sHost;
	g_piSocketServer->GetHostIP(sHost);
	pRespond->dwAddress         = inet_addr(sHost.szInternetIp);
	pRespond->wPort             = (WORD)sHost.nListenPort;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoChatMessage(
    int nMsgType, 
    DWORD dwSenderId, const char cszTalker[],
    DWORD dwReceiverID, const char cszReceiver[],
    size_t uDataLen, const BYTE*  pbyTalkData, BYTE byShownType
)
{
    BOOL                bResult         = false;
    BOOL                bRetCode        = false;
    IMemBlock*         piSendBuffer    = NULL;
    S2R_CHAT_MESSAGE*   pTalkMsg        = NULL;

    piSendBuffer = QCreateMemBlock((unsigned)(sizeof(S2R_CHAT_MESSAGE) + uDataLen));
    LOG_PROCESS_ERROR(piSendBuffer);

    pTalkMsg = (S2R_CHAT_MESSAGE*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pTalkMsg);

    pTalkMsg->wProtocolID       = s2r_chat_message;
    pTalkMsg->byMsgType         = (BYTE)nMsgType;
    pTalkMsg->dwSenderId        = dwSenderId;
    pTalkMsg->dwReceiverId      = dwReceiverID;
	pTalkMsg->byShownType       = byShownType;

    pTalkMsg->szSender[0] = '\0';
    if (cszTalker)
    {
        strncpy(pTalkMsg->szSender, cszTalker, sizeof(pTalkMsg->szSender));
        pTalkMsg->szSender[sizeof(pTalkMsg->szSender) - 1] = '\0';
    }

    pTalkMsg->szReceiver[0] = '\0';
    if (cszReceiver)
    {
        strncpy(pTalkMsg->szReceiver, cszReceiver, sizeof(pTalkMsg->szReceiver));
        pTalkMsg->szReceiver[sizeof(pTalkMsg->szReceiver) - 1] = '\0';
    }

    memcpy(pTalkMsg->byTalkData, pbyTalkData, uDataLen);

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KRelayClient::DoPlayerChatError(DWORD dwPlayerID, int nErrorCode)
{
    BOOL                    bResult         = false;
    BOOL                    bRetCode        = false;
    IMemBlock*             piSendBuffer    = NULL;
    S2R_PLAYER_CHAT_ERROR*  pPak            = NULL;

    piSendBuffer = QCreateMemBlock((unsigned)(sizeof(S2R_PLAYER_CHAT_ERROR)));
    LOG_PROCESS_ERROR(piSendBuffer);

    pPak = (S2R_PLAYER_CHAT_ERROR*)piSendBuffer->GetData();
    assert(pPak);

    pPak->wProtocolID   = s2r_player_chat_error;
    pPak->dwReceiverId  = dwPlayerID;
    pPak->nErrorCode    = nErrorCode;

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}


struct KFuncGetScenePlayer : public IKTraverseHelper
{
    std::vector<DWORD> m_vecPlayerID;

	BOOL OnTraverse(KSceneObject* pObj)
	{
		m_vecPlayerID.push_back(pObj->GetId());
		return true;
	}
};

BOOL KRelayClient::DoSaveScenePlayerList(KScene* pScene)
{
    BOOL                            bResult             = false;
    BOOL                            bRetCode            = false;
    IMemBlock*                     piPackage           = NULL;
    S2R_SAVE_SCENE_PLAYER_LIST*     pPlayerList         = NULL;
    int                             nPlayerCount        = 0;
    KFuncGetScenePlayer             GetScenePlayerFunc;

    pScene->TraverseAllPlayers(GetScenePlayerFunc);
    
    nPlayerCount = (int)GetScenePlayerFunc.m_vecPlayerID.size();
    PROCESS_SUCCESS(nPlayerCount == 0);
    
    piPackage = QCreateMemBlock((unsigned)sizeof(S2R_SAVE_SCENE_PLAYER_LIST) + sizeof(DWORD) * nPlayerCount);
	LOG_PROCESS_ERROR(piPackage);

    pPlayerList = (S2R_SAVE_SCENE_PLAYER_LIST*)piPackage->GetData();
    LOG_PROCESS_ERROR(pPlayerList);

    pPlayerList->wProtocolID   = s2r_save_scene_player_list;
    pPlayerList->dwMapID       = pScene->GetTemplateId();
    pPlayerList->nMapCopyIndex = GET_MAP_COPY_INDEX(pScene->GetId());
    pPlayerList->nPlayerCount  = nPlayerCount;

    memcpy(pPlayerList->dwPlayerList, &(GetScenePlayerFunc.m_vecPlayerID[0]), sizeof(DWORD) * nPlayerCount);

    bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT1:
    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoSaveSceneData(KScene* pScene)
{
    BOOL                    bResult         = false;
    BOOL                    bRetCode        = false;
//    size_t                  uDataLen        = 0;
//    IMemBlock*             piPackage       = NULL;
//    S2R_SAVE_SCENE_DATA*    pSceneData      = NULL;
//    BYTE*                   pbyOffset       = NULL;
//    size_t                  uLeftSize       = 0;
//    size_t                  uUsedSize       = 0;
//
//    uDataLen = sizeof(size_t) + SCENE_CUSTOM_DATA_SIZE + sizeof(size_t) + SCENE_PROGRESS_DATA_LEN;
//    
//    piPackage = QCreateMemBlock((unsigned)(sizeof(S2R_SAVE_SCENE_DATA) + uDataLen));
//	LOG_PROCESS_ERROR(piPackage);
//
//    pSceneData = (S2R_SAVE_SCENE_DATA*)piPackage->GetData();
//    LOG_PROCESS_ERROR(pSceneData);
//
//    pSceneData->wProtocolID   = s2r_save_scene_data;
//	pSceneData->dwMapID       = pScene->GetTemplateId();
//	pSceneData->nMapCopyIndex = GET_MAP_COPY_INDEX(pScene->GetId());
//    pSceneData->uDataLen      = uDataLen;
//
//    pbyOffset = pSceneData->byData;
//    uLeftSize = uDataLen;
//
//    *(DWORD*)pbyOffset = SCENE_PROGRESS_DATA_LEN;
//    pbyOffset += sizeof(DWORD);
//    uLeftSize -= sizeof(DWORD);
//    
//    bRetCode = pScene->m_ProgressData.Save(&uUsedSize, pbyOffset, uLeftSize);
//	LOG_PROCESS_ERROR(bRetCode);
//
//    pbyOffset += uUsedSize;
//    uLeftSize -= uUsedSize;
//
//    *(DWORD*)pbyOffset = SCENE_CUSTOM_DATA_SIZE;
//    pbyOffset += sizeof(DWORD);
//    uLeftSize -= sizeof(DWORD);
//
//	bRetCode = pScene->m_CustomData.Save(&uUsedSize, pbyOffset, uLeftSize);
//	LOG_PROCESS_ERROR(bRetCode);
//
//    pbyOffset += uUsedSize;
//    uLeftSize -= uUsedSize;
//
//    assert(uLeftSize == 0);
//
//    bRetCode = Send(piPackage);
//	LOG_PROCESS_ERROR(bRetCode);
//
//    bResult = true;
//EXIT0:
//    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoPlayerEnterSceneNotify(DWORD dwPlayerID, DWORD dwMapID, int nMapCopyIndex)
{
    BOOL                                bResult         = false;
    BOOL                                bRetCode        = false;
    IMemBlock*                         piPackage       = NULL;
    S2R_PLAYER_ENTER_SCENE_NOTIFY*      pNotify         = NULL;

    piPackage = QCreateMemBlock(sizeof(S2R_PLAYER_ENTER_SCENE_NOTIFY));
    LOG_PROCESS_ERROR(piPackage);

    pNotify = (S2R_PLAYER_ENTER_SCENE_NOTIFY*)piPackage->GetData();
    LOG_PROCESS_ERROR(pNotify);

    pNotify->wProtocolID   = s2r_player_enter_scene_notify;
    pNotify->dwPlayerID    = dwPlayerID;
    pNotify->dwMapID       = dwMapID;
    pNotify->nMapCopyIndex = nMapCopyIndex;

    bRetCode = Send(piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoResetMapCopyRequest(DWORD dwPlayerID, DWORD dwMapID, int nMapCopyIndex)
{
	BOOL                                bResult         = false;
	BOOL                                bRetCode        = false;
	IMemBlock*                         piPackage       = NULL;
	S2R_RESET_MAP_COPY_REQUEST*      pNotify         = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_RESET_MAP_COPY_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pNotify = (S2R_RESET_MAP_COPY_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->wProtocolID   = s2r_reset_map_copy_request;
	pNotify->dwPlayerID    = dwPlayerID;
	pNotify->dwMapID       = dwMapID;
	pNotify->nMapCopyIndex = nMapCopyIndex;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoLoadRoleDataRequest(DWORD dwRoleID)
{
    BOOL                        bResult         = false;
    BOOL                        bRetCode        = false;
    IMemBlock*                 piPackage       = NULL;
    S2R_LOAD_ROLE_DATA_REQUEST* pLoadRoleData   = NULL;

    piPackage = QCreateMemBlock(sizeof(S2R_LOAD_ROLE_DATA_REQUEST));
    LOG_PROCESS_ERROR(piPackage);

    pLoadRoleData               = (S2R_LOAD_ROLE_DATA_REQUEST*)piPackage->GetData();
    LOG_PROCESS_ERROR(pLoadRoleData);

    pLoadRoleData->wProtocolID  = s2r_load_role_data_request;
    pLoadRoleData->dwPlayerID   = dwRoleID;

    bRetCode = Send(piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoLoadDelayRoleDataRequest(INT nModuleId, DWORD dwSrcId, DWORD dwRoleID, LPCTSTR szRoleName /* = NULL */)
{
	BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	IMemBlock*                 piPackage       = NULL;
	S2R_LOAD_DELAY_ROLE_DATA_REQUEST* pLoadRoleData   = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_LOAD_DELAY_ROLE_DATA_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pLoadRoleData               = (S2R_LOAD_DELAY_ROLE_DATA_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pLoadRoleData);

	pLoadRoleData->wProtocolID  = s2r_load_delay_role_data_request;
	pLoadRoleData->byModuleId = (BYTE)nModuleId;
	pLoadRoleData->dwPlayerID   = dwRoleID;
	pLoadRoleData->dwSrcId = dwSrcId;
	memset(pLoadRoleData->szRoleName, 0, MAX_NAME_LEN);
	if(szRoleName != NULL)
	{
		strncpy(pLoadRoleData->szRoleName, szRoleName, MAX_NAME_LEN);
	}

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
	//QLogPrintf(LOG_ERR,"..KRelayClient::DoLoadDelayRoleDataRequest nModuleId=%d",nModuleId);
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoSyncRoleData(DWORD dwID, BYTE* pbyData, size_t uOffset, size_t uDataLen)
{
    BOOL                bResult         = false;
    BOOL                bRetCode        = false;
    IMemBlock*         piPackage       = NULL;
    S2R_SYNC_ROLE_DATA* pSyncRoleData   = NULL;

    piPackage = QCreateMemBlock((unsigned)(sizeof(S2R_SYNC_ROLE_DATA) + uDataLen));
    LOG_PROCESS_ERROR(piPackage);

    pSyncRoleData = (S2R_SYNC_ROLE_DATA*)piPackage->GetData();
    LOG_PROCESS_ERROR(pSyncRoleData);

    pSyncRoleData->wProtocolID  = s2r_sync_role_data;
    pSyncRoleData->dwRoleID     = dwID;
    pSyncRoleData->uOffset      = uOffset;

    memcpy(pSyncRoleData->byData, pbyData, uDataLen);

    bRetCode = Send(piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoSaveRoleData(KPlayer* pPlayer, size_t uRoleDataLen)
{
    BOOL                bResult       = false;
    BOOL                bRetCode      = false;
    IMemBlock*         piPackage     = NULL;
    S2R_SAVE_ROLE_DATA* pSaveRoleData = NULL;

    piPackage = QCreateMemBlock((unsigned)sizeof(S2R_SAVE_ROLE_DATA));
    LOG_PROCESS_ERROR(piPackage);

    pSaveRoleData = (S2R_SAVE_ROLE_DATA*)piPackage->GetData();
    LOG_PROCESS_ERROR(pSaveRoleData);

    pSaveRoleData->wProtocolID  = s2r_save_role_data;
    pSaveRoleData->dwRoleID     = pPlayer->GetId();
    pSaveRoleData->nUserValue   = pPlayer->GetGameState();
	pSaveRoleData->nUserValue2  = pPlayer->m_cPlayerServer.m_nTargetBZoneId;
    pSaveRoleData->uRoleDataLen = uRoleDataLen;

    bRetCode = pPlayer->m_cPlayerServer.SaveBaseInfo(&pSaveRoleData->BaseInfo);
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = Send(piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoCreateDelayRoleData(KPlayer* pPlayer)
{
	BOOL                bResult       = false;
	BOOL                bRetCode      = false;
	IMemBlock*         piPackage     = NULL;
	S2R_CREATE_DELAY_ROLE_DATA* pRoleData = NULL;

	piPackage = QCreateMemBlock((unsigned)sizeof(S2R_CREATE_DELAY_ROLE_DATA));
	LOG_PROCESS_ERROR(piPackage);

	pRoleData = (S2R_CREATE_DELAY_ROLE_DATA*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRoleData);

	pRoleData->wProtocolID  = s2r_create_delay_role_data;
	pRoleData->dwRoleID     = pPlayer->GetId();

	bRetCode = pPlayer->m_cPlayerServer.SaveDelayRoleData(&pRoleData->DelayRoleData);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoSaveDelayRoleData(KPlayer* pPlayer)
{
	BOOL                bResult       = false;
	BOOL                bRetCode      = false;
	IMemBlock*         piPackage     = NULL;
	S2R_SAVE_DELAY_ROLE_DATA* pRoleData = NULL;

	PROCESS_ERROR(!IS_BZONE_PLAYER(pPlayer->GetId()));

	piPackage = QCreateMemBlock((unsigned)sizeof(S2R_SAVE_DELAY_ROLE_DATA));
	LOG_PROCESS_ERROR(piPackage);

	pRoleData = (S2R_SAVE_DELAY_ROLE_DATA*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRoleData);

	pRoleData->wProtocolID  = s2r_save_delay_role_data;
	pRoleData->dwRoleID     = pPlayer->GetId();

	bRetCode = pPlayer->m_cPlayerServer.SaveDelayRoleData(&pRoleData->DelayRoleData);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoSendGmMessage(const char cszGmName[], const char cszMessage[])
{
    BOOL                    bResult     = false;
    BOOL                    bRetCode    = false;
    IMemBlock*             piPackage   = NULL;
    S2R_SEND_GM_MESSAGE*    pGmMessage  = NULL;
    size_t                  uStrLen     = 0;
    size_t                  uPakSize    = 0;

    assert(cszGmName);
    assert(cszMessage);

    uStrLen = strlen(cszMessage);
    uPakSize = sizeof(S2R_SEND_GM_MESSAGE) + uStrLen + sizeof('\0');

    piPackage = QCreateMemBlock((unsigned int)uPakSize);
    LOG_PROCESS_ERROR(piPackage);

    pGmMessage = (S2R_SEND_GM_MESSAGE*)piPackage->GetData();
    LOG_PROCESS_ERROR(pGmMessage);

    pGmMessage->wProtocolID = s2r_send_gm_message;
    
    strncpy(pGmMessage->szGmName, cszGmName, sizeof(pGmMessage->szGmName));
    pGmMessage->szGmName[sizeof(pGmMessage->szGmName) - sizeof('\0')] = '\0';

    strncpy(pGmMessage->szMessage, cszMessage, uStrLen + sizeof('\0'));
    pGmMessage->szMessage[uStrLen] = '\0';

    bRetCode = Send(piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoSendGmChn(const char cszAccount[], const char cszRole[], const char cszMessage[])
{
    BOOL                bResult     = false;
    BOOL                bRetCode    = false;
    IMemBlock*         piPackage   = NULL;
    S2R_SEND_GM_CHN*    pGmChn      = NULL;
    size_t              uStrLen     = 0;
    size_t              uPakSize    = 0;

    assert(cszAccount);
    assert(cszRole);
    assert(cszMessage);

    uStrLen = strlen(cszMessage);
    uPakSize = sizeof(S2R_SEND_GM_CHN) + uStrLen + sizeof('\0');

    piPackage = QCreateMemBlock((unsigned int)uPakSize);
    LOG_PROCESS_ERROR(piPackage);

    pGmChn = (S2R_SEND_GM_CHN*)piPackage->GetData();
    LOG_PROCESS_ERROR(pGmChn);

    pGmChn->wProtocolID = s2r_send_gm_chn;

    strncpy(pGmChn->szAccount, cszAccount, sizeof(pGmChn->szAccount));
    pGmChn->szAccount[sizeof(pGmChn->szAccount) - 1] = '\0';

    strncpy(pGmChn->szRole, cszRole, sizeof(pGmChn->szRole));
    pGmChn->szRole[sizeof(pGmChn->szRole) - 1] = '\0';

    strncpy(pGmChn->szMessage, cszMessage, uStrLen + 1);
    pGmChn->szMessage[uStrLen] = '\0';

    bRetCode = Send(piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoSendGMCommandGS(const char cszRoleName[], const char cszCommand[])
{
    BOOL                    bResult         = false;
    BOOL                    bRetCode        = false;
    IMemBlock*             piPackage       = NULL;
    S2R_SEND_GM_COMMAND_GS* pGMCommandGS    = NULL;
    size_t                  uStrLen         = 0;
    size_t                  uPakSize        = 0;

    assert(cszRoleName);
    assert(cszCommand);

    uStrLen     = strlen(cszCommand);
    uPakSize    = sizeof(S2R_SEND_GM_COMMAND_GS) + uStrLen + sizeof('\0');

    piPackage = QCreateMemBlock((unsigned int)uPakSize);
    LOG_PROCESS_ERROR(piPackage);

    pGMCommandGS = (S2R_SEND_GM_COMMAND_GS*)piPackage->GetData();
    LOG_PROCESS_ERROR(pGMCommandGS);

    pGMCommandGS->wProtocolID = s2r_send_gm_command_gs;

    strncpy(pGMCommandGS->szRoleName, cszRoleName, sizeof(pGMCommandGS->szRoleName));
    pGMCommandGS->szRoleName[sizeof(pGMCommandGS->szRoleName) - sizeof('\0')] = '\0';

    strncpy(pGMCommandGS->szCommand, cszCommand, uStrLen + sizeof('\0'));
    pGMCommandGS->szCommand[uStrLen] = '\0';

    bRetCode = Send(piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoSendGMCommandGC(const char cszCommand[])
{
    BOOL                    bResult      = false;
    BOOL                    bRetCode     = false;
    IMemBlock*             piPackage    = NULL;
    S2R_SEND_GM_COMMAND_GC* pGMCommandGC = NULL;
    size_t                  uStrLen      = 0;
    size_t                  uPakSize     = 0;

    assert(cszCommand);

    uStrLen     = strlen(cszCommand);
    uPakSize    = uStrLen + sizeof(S2R_SEND_GM_COMMAND_GC) + sizeof('\0');

    piPackage = QCreateMemBlock((unsigned int)uPakSize);
    LOG_PROCESS_ERROR(piPackage);

    pGMCommandGC = (S2R_SEND_GM_COMMAND_GC*)piPackage->GetData();
    LOG_PROCESS_ERROR(pGMCommandGC);

    pGMCommandGC->wProtocolID = s2r_send_gm_command_gc;

    strncpy(pGMCommandGC->szCommand, cszCommand, uStrLen + sizeof('\0'));
    pGMCommandGC->szCommand[uStrLen] = '\0';

    bRetCode = Send(piPackage);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piPackage);
    return bResult;
}

BOOL KRelayClient::DoRemoteLuaCall(BYTE* pbyParam, size_t uParamLen)
{
	BOOL                    bResult         = false;
	BOOL                    bRetCode        = false;
	IMemBlock*             piBuffer        = NULL;
	S2R_REMOTE_LUA_CALL*    pRequest        = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_REMOTE_LUA_CALL) + uParamLen);
	LOG_PROCESS_ERROR(piBuffer);

	pRequest = (S2R_REMOTE_LUA_CALL*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID   = s2r_remote_lua_call;
	pRequest->uParamLen     = uParamLen;

	memcpy(pRequest->byParam, pbyParam, uParamLen);

	bRetCode = Send(piBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

void KRelayClient::OnBZoneTransferNotify( BYTE* pbyData, size_t uDataLen )
{
// SaveRoleData 
	// player state savefor bzone transfer

}

BOOL KRelayClient::DoBZoneTransferRequest( KPlayer* pPlayer, DWORD uRoleDataLen, INT nBZoneId /*= 0*/ )
{
	BOOL                            bResult         = FALSE;
	BOOL                            bRetCode        = FALSE;
	IMemBlock*                     piPackage       = NULL;
	S2R_BZONE_TRANSFER_REQUEST*    pRequest        = NULL;
	size_t                          uUsedSize       = 0;
	SIZE_T							uDataLen		= 0;

	piPackage = QCreateMemBlock(sizeof(S2R_BZONE_TRANSFER_REQUEST) + uRoleDataLen);
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (S2R_BZONE_TRANSFER_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID           = s2r_bzone_transfer_request;
	pRequest->dwRoleID              = pPlayer->GetId();
	pRequest->nPlayerIdx = pPlayer->GetPlayerIndex();

	bRetCode  = pPlayer->m_cPlayerServer.SaveBaseInfo(&pRequest->RoleBaseInfo);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_cPlayerDbMgr.SaveRole(*pPlayer, *(KROLE_DATA*)pRequest->byRoleData, MAX_ROLE_DATA_SIZE, uDataLen);
	LOG_PROCESS_ERROR(bRetCode);
	LOG_PROCESS_ERROR(uDataLen == uRoleDataLen);

	pRequest->uRoleDataLen = uDataLen;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}


BOOL KRelayClient::DoBZoneReturnRequest(KPlayer* pPlayer, INT nMapId, INT nPosX, INT nPosY, INT nPosZ)
{
	BOOL                            bResult         = FALSE;
	BOOL                            bRetCode        = FALSE;
	IMemBlock*                     piPackage       = NULL;
	S2R_BZONE_RETURN_REQUEST*		pRequest        = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_BZONE_RETURN_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (S2R_BZONE_RETURN_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID           = s2r_bzone_return_request;
	pRequest->dwRoleID              = pPlayer->GetId();
	bRetCode  = pPlayer->m_cPlayerServer.SaveBaseInfo(&pRequest->RoleBaseInfo);
	LOG_PROCESS_ERROR(bRetCode);

	pRequest->RoleBaseInfo.CurrentPos.dwMapID = nMapId;
	pRequest->RoleBaseInfo.CurrentPos.nX = nPosX;
	pRequest->RoleBaseInfo.CurrentPos.nY = nPosY;
	pRequest->RoleBaseInfo.CurrentPos.nZ = nPosZ;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::ReturnToLastServer(KPlayer* pPlayer, INT nExValue)
{
	BOOL                            bResult         = FALSE;
	BOOL                            bRetCode        = FALSE;
	IMemBlock*                     piPackage       = NULL;
	S2R_BZONE_RETURN_REQUEST*		pRequest        = NULL;
	KCrossSvCtrl* pCrossSvCtrl = NULL;
	SIZE_T uUsedSize = 0;

	pCrossSvCtrl = pPlayer->GetCtrl<KCrossSvCtrl>();
	LOG_PROCESS_ERROR(pCrossSvCtrl);

	LOG_PROCESS_ERROR(pCrossSvCtrl->GetCrossSvDataSize(uUsedSize));

	piPackage = QCreateMemBlock(sizeof(S2R_BZONE_RETURN_REQUEST) + uUsedSize);
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (S2R_BZONE_RETURN_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID           = s2r_bzone_return_request;
	pRequest->dwRoleID              = pPlayer->GetId();
	pRequest->nExValue				= nExValue;

	bRetCode  = pPlayer->m_cPlayerServer.SaveBaseInfo(&pRequest->RoleBaseInfo);
	LOG_PROCESS_ERROR(bRetCode);

	pRequest->uExDataLen = uUsedSize;

	bRetCode = pCrossSvCtrl->SaveCrossSvDatas(pRequest->byExData, uUsedSize);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoGetMultiAwardRequest(KPlayer* pPlayer)
{
	BOOL                            bResult         = FALSE;
	BOOL                            bRetCode        = FALSE;
	IMemBlock*                     piPackage       = NULL;
	S2R_GET_MULTI_AWARD_REQUEST*		pRequest        = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_GET_MULTI_AWARD_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (S2R_GET_MULTI_AWARD_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID           = s2r_get_multi_award_request;
	pRequest->dwRoleId              = pPlayer->GetId();

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL KRelayClient::DoGetCrossSvExDataRequest(KPlayer* pPlayer)
{
	BOOL                            bResult         = FALSE;
	BOOL                            bRetCode        = FALSE;
	IMemBlock*                     piPackage       = NULL;
	S2R_GET_CROSSSV_EXDATA_REQUEST*		pRequest        = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_GET_CROSSSV_EXDATA_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pRequest = (S2R_GET_CROSSSV_EXDATA_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID           = s2r_get_crosssv_exdata_request;
	pRequest->dwRoleId              = pPlayer->GetId();

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

//BOOL KRelayClient::DoChangeExtPointRequest(DWORD dwPlayerID, WORD wExtPointIndex, WORD wChangeValue)
//{
//    BOOL                            bResult  = false;
//    BOOL                            bRetCode = false;
//    IMemBlock*                     piBuffer = NULL;
//    S2R_CHANGE_EXT_POINT_REQUEST*   pRequest = NULL;
//
//    piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_CHANGE_EXT_POINT_REQUEST));
//    LOG_PROCESS_ERROR(piBuffer);
//
//    pRequest = (S2R_CHANGE_EXT_POINT_REQUEST*)piBuffer->GetData();
//    LOG_PROCESS_ERROR(pRequest);
//
//    pRequest->wProtocolID       = s2r_change_ext_point_request;
//    pRequest->dwPlayerID        = dwPlayerID;
//    pRequest->wExtPointIndex    = wExtPointIndex;
//    pRequest->wChangeValue      = wChangeValue;
//
//    bRetCode = Send(piBuffer);
//    LOG_PROCESS_ERROR(bRetCode);
//
//    bResult = true;
//EXIT0:
//    SAFE_RELEASE(piBuffer);
//    return bResult;
//}

BOOL KRelayClient::DoMapCopyKeepPlayer(KPlayer* pPlayer)
{
    BOOL                                bResult  = false;
    BOOL                                bRetCode = false;
    IMemBlock*                         piBuffer = NULL;
    S2R_MAP_COPY_KEEP_PLAYER*           pRequest = NULL;
	KScene*								pScene   = NULL;

	pScene = (KScene*)pPlayer->GetScene();
    assert(pScene);

    piBuffer = QCreateMemBlock((unsigned)sizeof(S2R_MAP_COPY_KEEP_PLAYER));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (S2R_MAP_COPY_KEEP_PLAYER*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->wProtocolID = s2r_map_copy_keep_player;

    pRequest->dwMapID       = GET_MAP_ID(pScene->m_dwId);
    pRequest->nCopyIndex    = GET_MAP_COPY_INDEX(pScene->m_dwId);
    pRequest->dwPlayerID    = pPlayer->GetId();

    bRetCode = Send(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

VOID KRelayClient::SendGMCMDAward( BYTE* pbyData, size_t uDataLen )
{
	BOOL					bResult			= FALSE;
	R2S_GM_SEND_AWARD*		pRespond		= (R2S_GM_SEND_AWARD*)pbyData;
	LPCSTR					pszCommand		= (LPCSTR)pRespond->GMCMD;
	DWORD					id				= pRespond->id;
	DWORD					playerID		= pRespond->dwRoleId;
	ScriptSafe 				cSafeScript 	= g_cScriptManager.GetSafeScript();
	QLunaBase* 				pOldMe			= NULL;
	LPCSTR 					pszPrefix 		= GM_CMD_PREFIX_GAMESERVER;
	SIZE_T 					nPrefixLen 		= sizeof(GM_CMD_PREFIX_GAMESERVER) - 1;

	KPlayer* pSender = g_cPlayerMgr.GetById(playerID);
	LOG_PROCESS_ERROR(pSender);
	if (QStrCmpLen(pszCommand, pszPrefix, nPrefixLen))
	{
		pOldMe = g_cScriptManager.SetMe((QLunaBase*)pSender->GetScriptInterface());
		//LOG_PROCESS_ERROR(pOldMe);
		bResult = cSafeScript->DoBuffer(pszCommand + nPrefixLen);
		if (!bResult)
		{
			QLogPrintf(LOG_ERR, "%s\n", pszCommand + nPrefixLen);
			LOG_PROCESS_ERROR(bResult);
		}
		DoGMSendSuccess(id,playerID,(char*)pszCommand);
		QLogPrintf(LOG_INFO, "....[GM] [%s][%d] '%s'",
			pSender->m_cPlayerServer.GetAccount(),
			pSender->GetPlayerId(),
			pszCommand);
		g_cScriptManager.SetMe(pOldMe);
	}
	bResult = TRUE;
EXIT0:
	return;
}

BOOL KRelayClient::DoGMSendSuccess( DWORD id, DWORD dwRoleId, char* GMCMD )
{
	BOOL							bRetCode = FALSE;
	BOOL							bResult	 = FALSE;
	R2S_GM_SEND_AWARD*	pRespond = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_GM_SEND_AWARD));
	pRespond = (R2S_GM_SEND_AWARD*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = s2r_GM_send_award_success_request;
	pRespond->id		 = id;
	pRespond->dwRoleId = dwRoleId;
	strncpy(pRespond->GMCMD, GMCMD, sizeof(pRespond->GMCMD));

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}



//AutoCode:-发送协议函数结束-
#endif	// GAME_SERVER
