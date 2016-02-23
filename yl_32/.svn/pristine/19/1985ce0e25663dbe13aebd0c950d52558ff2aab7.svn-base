
#pragma once


#include "onlinegameworld/kcharacter.h"

#define KAI_ACTION_PARAM_NUM    5
#define KAI_ACTION_BRANCH_NUM   3


#define KAI_ACTION_ID_ERROR     (-1)
#define KAI_ACTION_ID_NONE      0



class KAIAction : public QLunaBase
{
public:
    KAIAction();

    INT         m_nKey;

    INT         m_nParam[KAI_ACTION_PARAM_NUM];
    INT         m_nBranch[KAI_ACTION_BRANCH_NUM];
    KAIAction*  m_pBranch[KAI_ACTION_BRANCH_NUM];

public:
    DECLARE_LUA_CLASS(KAIAction);

    INT LuaSetParam(QLuaScript& sc);
    INT LuaGetParam(QLuaScript& sc);

    INT LuaSetBranch(QLuaScript& sc);
};

struct KAIActionHandle 
{
    KAIActionHandle() : nAIActionID(KAI_ACTION_ID_ERROR), pAIAction(NULL) {};

    int         nAIActionID;
    KAIAction*  pAIAction;
};


enum KAI_FILTER_TYPE
{
    airtInvalid =-1,

    airtBaseTarget,
    airtRangeTarget,
    airtNearestTarget,

    airtTotal
};

enum KAI_ACTION_KEY
{
    eakInvalid = 0,

    eakSetState,
    eakSetPrimaryTimer,    
    eakSetSecondaryTimer,
    eakRandomBiBranch,
    eakRandomTriBranch,
    eakStand,

    eakSearchEnemy,
    eakAddTargetToThreatList,
    eakSelectFirstThreat,
    eakIsInFight,
    eakKeepRange,
    eakCastSkill,
    eakSetSystemShield,
    eakRecordReturnPosition,
    eakReturn,
    eakEnterPatrolPath,
    eakLeavePatrolPath,
    eakPatrol,
    eakRecordOriginPosition,
    eakKeepOriginDirection,
    eakFaceTarget,
    eakWander,
    eakSearchPlayer,
    eakSelectEventSrc,
    eakNpcOpenDialogWindow,
    eakNpcSayToTarget,
    eakCheckLife,
    eakNpcInitSkillCD,
    eakNpcStandardSkillSelector,
    eakNpcKeepSkillCastRange,
    eakNpcCastSelectSkill,
    eakDoAnimation,
    eakGroupRevive,
    eakIsTargetInRange,
    eakCleanBuffByFunction,
    eakNeedEscape,
    eakParamCompare,
    eakSelectState,
    eakRandomRun,
    eakCleanDebuff,
    eakSearchMultiPlayerAddThreatList,
    eakCheckSkillFailedCounter,
    eakNpcSetSecTimerBySkillAni,
    eakNpcSetSecTimerBySkillRest,
    eakIsTargetExist,
    eakIsTargeInWater,
    eakNpcClearSelectSkillCD,
    eakNpcResetSelectSkillCD,
    eakIsNeedKeepRange,
    eakSetTertiaryTimer,
    eakSearchPlayerWithoutAnyAffect,
    eakClearThreatForTarget,
    eakSelectNthThreat,
    eakFilterThreat,
    eakChangeTarget,
    eakSetTarget,
    eakClearTotalDamageValue,
    eakTotalDamageValueCompare,
    eakSearchBustupPlayerAddThreatList,
    eakCopyThreatListToTargetGroup,
    eakExtractionTargetGroup,
    eakReturnPositionDistanceCompare,
    eakIsOTActionState,
    eakSearchNpc,

    eakTotal
};

// 用户自定义 ai action key 从 KAI_USER_ACTION 开始
#define KAI_USER_ACTION 1000

typedef int (*KAI_ACTION_FUNC)(KCharacter* pCharacter, KAIAction* pActionData);

