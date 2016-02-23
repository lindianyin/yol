#include "stdafx.h"
#include "config/gamedef.h"
#include "onlinegamemodule/ai/kluaaiaction.h"
#include "onlinegamemodule/ai/kaivm.h"

QLuaConst LUA_AI_THREAT_TYPE[] =
{
	{"INVALID",                         thtInvalid},

	{"MAIN_THREAT",                     thtMainThreat},
	{"BASE_THREAT",                     thtBaseThreat},
	{"DAMAGE_THREAT",                   thtDamageThreat},
	{"THERAPY_THREAT",                  thtTherapyThreat},
	{"SPECIAL_THREAT",                  thtSpecialThreat},

	{"TOTAL",                           thtTotal},
	{NULL, 0}
};

QLuaConst LUA_AI_TARGET_TYPE[] =
{
	{"INVALID",                         aittInvalid},

	{"SELECT_TARGET",                   aittSelectTarget},
	{"SKILL_TARGET",                    aittSkillTarget},

	{"LAST_SELECT_TARGET",              aittLastSelectTarget},
	{"LAST_SKILL_TARGET",               aittLastSkillTarget},
	{"ATTACK_TARGET",                   aittAttackTarget},

	{"CUSTOM_TARGET_1",                 aittCustomTarget1},
	{"CUSTOM_TARGET_2",                 aittCustomTarget2},
	{"CUSTOM_TARGET_3",                 aittCustomTarget3},
	{"CUSTOM_TARGET_4",                 aittCustomTarget4},

	{"TOTAL",                           aittTotal},
	{NULL, 0}
};

QLuaConst LUA_AI_FILTER_TYPE[] =
{
	{"INVALID",                         airtInvalid},

	{"BASE",                            airtBaseTarget},
	{"RANGE",                           airtRangeTarget},
	{"NEAREST",                         airtNearestTarget},

	{"TOTAL",                           airtTotal},
	{NULL, 0}
};

QLuaConst LUA_AI_ACTION_KEY[] = 
{
	{"INVALID",                                 eakInvalid},

	{"SET_STATE",                               eakSetState},
	{"SET_PRIMARY_TIMER",                       eakSetPrimaryTimer},
	{"SET_SECONDARY_TIMER",                     eakSetSecondaryTimer},
	{"RANDOM_BI_BRANCH",                        eakRandomBiBranch},
	{"RANDOM_TRI_BRANCH",                       eakRandomTriBranch},
	{"STAND",                                   eakStand},

	{"SEARCH_ENEMY",                            eakSearchEnemy},
	{"ADD_TARGET_TO_THREAT_LIST",               eakAddTargetToThreatList},
	{"SELECT_FIRST_THREAT",                     eakSelectFirstThreat},
	{"IS_IN_FIGHT",                             eakIsInFight},
	{"KEEP_RANGE",                              eakKeepRange},
	{"CAST_SKILL",                              eakCastSkill},
	{"SET_SYSTEM_SHEILD",                       eakSetSystemShield},
	{"RECORD_RETURN_POSITION",                  eakRecordReturnPosition},
	{"RETURN",                                  eakReturn},
	{"ENTER_PATROL_PATH",                       eakEnterPatrolPath},
	{"LEAVE_PATROL_PATH",                       eakLeavePatrolPath},
	{"DO_PATROL",                               eakPatrol},
	{"RECORD_ORIGIN_POSITION",                  eakRecordOriginPosition},
	{"KEEP_ORIGIN_DIRECTION",                   eakKeepOriginDirection},
	{"FACE_TARGET",                             eakFaceTarget},
	{"WANDER",                                  eakWander},
	{"SEARCH_PLAYER",                           eakSearchPlayer},
	{"SELECT_EVENT_SRC",                        eakSelectEventSrc},
	{"NPC_OPEN_DIALOG_WINDOW",                  eakNpcOpenDialogWindow},
	{"NPC_SAY_TO_TARGET",                       eakNpcSayToTarget},
	{"CHECK_LIFE",                              eakCheckLife},
	{"NPC_INIT_SKILL_CD",                       eakNpcInitSkillCD},
	{"NPC_STANDARD_SKILL_SELECTOR",             eakNpcStandardSkillSelector},
	{"NPC_KEEP_SKILL_CAST_RANGE",               eakNpcKeepSkillCastRange},
	{"NPC_CAST_SELECT_SKILL",                   eakNpcCastSelectSkill},
	{"DO_ANIMATION",                            eakDoAnimation},
	{"GROUP_REVIVE",                            eakGroupRevive},
	{"IS_TARGET_IN_RANGE",                      eakIsTargetInRange},
	{"CLEAN_BUFF_BY_FUNCTION",                  eakCleanBuffByFunction},
	{"NEED_ESCAPE",                             eakNeedEscape},
	{"PARAM_COMPARE",                           eakParamCompare},
	{"SELECT_STATE",                            eakSelectState},
	{"RANDOM_RUN",                              eakRandomRun},
	{"CLEAN_DEBUFF",                            eakCleanDebuff},
	{"SEARCH_MULTI_PLAYER_ADD_THREAT_LIST",     eakSearchMultiPlayerAddThreatList},
	{"CHECK_SKILL_FAILED_COUNTER",              eakCheckSkillFailedCounter},
	{"NPC_SET_SEC_TIMER_BY_SKILL_ANI",          eakNpcSetSecTimerBySkillAni},
	{"NPC_SET_SEC_TIMER_BY_SKILL_REST",         eakNpcSetSecTimerBySkillRest},   
	{"IS_TARGET_EXIST",                         eakIsTargetExist},
	{"IS_TARGE_IN_WATER",                       eakIsTargeInWater},
	{"NPC_CLEAR_SELECT_SKILL_CD",               eakNpcClearSelectSkillCD},
	{"NPC_RESET_SELECT_SKILL_CD",               eakNpcResetSelectSkillCD},
	{"IS_NEED_KEEP_RANGE",                      eakIsNeedKeepRange},
	{"SET_TERTIARY_TIMER",                      eakSetTertiaryTimer},
	{"SEARCH_PLAYER_WITHOUT_ANY_AFFECT",        eakSearchPlayerWithoutAnyAffect},
	{"CLEAR_THREAT_FOR_TARGET",                 eakClearThreatForTarget},
	{"SELECT_NTH_THREAT",                       eakSelectNthThreat},
	{"FILTER_THREAT",                           eakFilterThreat},
	{"CHANGE_TARGET",                           eakChangeTarget},
	{"SET_TARGET",                              eakSetTarget},
	{"CLEAR_TOTAL_DAMAGE_VALUE",                eakClearTotalDamageValue},
	{"TOTAL_DAMAGE_VALUE_COMPARE",              eakTotalDamageValueCompare},
	{"SEARCH_BUSTUP_PLAYER_ADD_THREAT_LIST",    eakSearchBustupPlayerAddThreatList},
	{"COPY_THREATLIST_TO_TARGETGROUP",          eakCopyThreatListToTargetGroup},
	{"EXTRACTION_TARGETGROUP",                  eakExtractionTargetGroup},
	{"RETURN_POSITION_DISTANCE_COMPARE",        eakReturnPositionDistanceCompare},
	{"IS_OTACTION_STATE",                       eakIsOTActionState},
	{"SEARCH_NPC",                              eakSearchNpc},

	{"TOTAL",                                   eakTotal},
	{NULL, 0}
};

QLuaConst LUA_AI_EVENT[] =
{
	{"INVALID",                         aevInvalid},

	{"ON_PRIMARY_TIMER",                aevOnPrimaryTimer},
	{"ON_SECONDARY_TIMER",              aevOnSecondaryTimer},
	{"ON_TERTIARY_TIMER",               aevOnTertiaryTimer},
	{"ON_PATH_SUCCESS",                 aevOnPathSuccess},
	{"ON_PATH_FAILED",                  aevOnPathFailed},
	{"ON_DIALOG",                       aevOnDialog},
	{"ON_ACTION",                       aevOnAction},
	{"ON_ATTACKED",                     aevOnAttacked},
	{"ON_SELF_KILLED",                  aevOnSelfKilled},
	{"ON_TARGET_KILLED",                aevOnTargetKilled},
	{"ON_IN_FEAR",                      aevOnInFear},
	{"ON_OUT_FEAR",                     aevOnOutFear},
	{"ON_BEGIN_PREPARE",                aevOnBeginPrepare},
	{"ON_BREAK_PREPARE",                aevOnBreakPerpare},
	{"ON_CAST_SKILL",                   aevOnCastSkill},
	{"ON_CAST_SKILL_FAILED",            aevOnCastSkillFailed},
	{"ON_OUT_OF_BATTLE_ZONE",           aevOnOutOfBattleZone},

	{"TOTAL",                           aevTotal},
	{NULL, 0}
};




QLuaConstList g_LuaConstList[] =
{
    {"AI_ACTION",                   LUA_AI_ACTION_KEY},
    {"AI_EVENT",                    LUA_AI_EVENT},
    {"AI_THREAT_TYPE",              LUA_AI_THREAT_TYPE},
    {"AI_TARGET_TYPE",              LUA_AI_TARGET_TYPE},
    {"AI_FILTER_TYPE",              LUA_AI_FILTER_TYPE},

    {NULL,                          0}
};
