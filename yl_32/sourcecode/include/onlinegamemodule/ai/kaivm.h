
#pragma once

#include "kluaaistate.h"
#include "kluaaiaction.h"
#include "onlinegameworld/kcharacter.h"

class KAIState;
class KAILogic;

enum KAI_EVENT
{
    aevInvalid  = 0,

    aevOnPrimaryTimer,
    aevOnSecondaryTimer,
    aevOnTertiaryTimer,
    aevOnPathSuccess,
    aevOnPathFailed,
    aevOnDialog,
    aevOnAction,
    aevOnAttacked,
    aevOnSelfKilled,
    aevOnTargetKilled,
    aevOnInFear,
    aevOnOutFear,
    aevOnBeginPrepare,
    aevOnBreakPerpare,
    aevOnCastSkill,
    aevOnCastSkillFailed,
    aevOnOutOfBattleZone,

    aevTotal,
};

// 用户自定义AI event从 KAI_USER_EVENT 开始
#define KAI_USER_EVENT 1000

class KAIVM : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emCHARACTER_CTRL_AI_VM	};

    KAIVM(KCharacter& rcCharacter);

	BOOL Init() { return TRUE; }
	BOOL UnInit() { return TRUE; }
	BOOL Active();

    BOOL Setup(KCharacter* pCharacter, int nAIType);

    INT  GetAIType();

    void FireEvent(int nEvent, DWORD dwEventSrc, int nEventParam);

    BOOL SetState(int nState);
    void SetPrimaryTimer(int nFrame);
    void SetSecondaryTimer(int nFrame);
    void SetTertiaryTimer(int nFrame);

    void AddupTotalDamageValue(int nDeltaDamageValue);

    void DebugAICurrentStateInfo();

public:
    int                 m_nDebugCount;
    UINT64				m_ullRunTimeStatistic;

private:
    KCharacter*         m_pOwner;
    INT                 m_nAIType;
    KAILogic*           m_pAILogic;
    KAIState*           m_pState;
    INT                 m_nStateID;
    INT                 m_nActionID;
    INT                 m_nPrimaryTimerFrame;
    int                 m_nSecondaryTimerFrame;
    int                 m_nTertiaryTimerFrame;
    INT                 m_nCurrentEvent; // 标示"正在某个Event的处理流程中"
    INT                 m_nPendingEvent;
    DWORD               m_dwPendingEventSrc;
    int                 m_nPendingEventParam;
};

#define m_AIVM GetCtrlRef<KAIVM>()
