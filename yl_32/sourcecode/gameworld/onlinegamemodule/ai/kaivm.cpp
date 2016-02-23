
#include "stdafx.h"
#include "onlinegamemodule/ai/kluaaistate.h"
#include "onlinegamemodule/ai/kaivm.h"
#include "onlinegamemodule/ai/kaimanager.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegamemodule/ai/kaictrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KAIVM::KAIVM(KCharacter& rcCharacter) : KCharacterCtrlBase(rcCharacter)
{
    m_pOwner                = NULL;
    m_nAIType               = 0;
    m_pState                = NULL;
    m_pAILogic              = NULL;
    m_nStateID              = 0;
    m_nActionID             = 0;
    m_nPrimaryTimerFrame    = 0;
    m_nSecondaryTimerFrame  = 0;
    m_nTertiaryTimerFrame   = 0;
    m_nCurrentEvent         = aevInvalid;
    m_nDebugCount           = 0;
    m_nPendingEvent         = aevInvalid;
    m_nPendingEventParam    = 0;
    m_ullRunTimeStatistic   = 0;
}

BOOL KAIVM::Setup(KCharacter* pCharacter, int nAIType)
{
    BOOL        bResult     = false;
    BOOL        bRetCode    = false;
    KAILogic*   pAILogic    = NULL;
    int         nInitState  = 0;
    
    assert(pCharacter);

    // 允许动态的切换AI类型(便于调试),但是不允许在一个Action流程中切换
    PROCESS_ERROR(m_nCurrentEvent == 0);

    m_pOwner = pCharacter;

    if (nAIType == 0)
    {
        // 清除原有AI类型
        m_pAILogic  = NULL;
        goto EXIT1;
    }

    pAILogic = g_sAIManager.GetAILogic(nAIType);
    if (!pAILogic)
    {
        QLogPrintf(LOG_ERR, "[AI] Setup AIVM failed, AIType %d is invalid.", nAIType);
        goto EXIT0;
    }

    m_nDebugCount   = 0;
    m_nAIType   = nAIType;
    m_pAILogic  = pAILogic;
    nInitState  = m_pAILogic->GetInitState();

    bRetCode = SetState(nInitState);
    LOG_PROCESS_ERROR(bRetCode);

    //m_nPrimaryTimerFrame    = 0;
    m_nSecondaryTimerFrame  = 0;
    m_nTertiaryTimerFrame   = 0;
    m_nCurrentEvent         = 0;

    //FireEvent(aevOnPrimaryTimer, m_pOwner->m_dwId, g_cOrpgWorld.m_dwLogicFrames);
	SetPrimaryTimer(SYNC_CHARACTER_DELAY + 1); // 同步角色后触发首次PrimaryTimer

EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

INT KAIVM::GetAIType()
{
    return m_nAIType;
}


#define MAX_ACTION_CALL  64

void KAIVM::FireEvent(int nEvent, DWORD dwEventSrc, int nEventParam)
{
    int             nEventBlock     = m_nCurrentEvent;
    //int             nAction         = KAI_ACTION_BRANCH_ERROR;
    int             nCallCount      = 0;
    DWORD           dwScriptID      = ERROR_ID;
    unsigned long   ulPrevTickCount = 0;
    unsigned long   ulPostTickCount = 0;
    KAIActionHandle ActionKey;

    assert(nEvent > 0);

    PROCESS_ERROR(m_pAILogic);

    dwScriptID = m_pAILogic->GetScriptID();

    assert(m_pOwner);
    assert(m_pState);

    if (g_sAIManager.m_bLogAIRuntimeStat)
        ulPrevTickCount = GetTickCount();

    // 触发的事件预先判断在当前状态是否有托管
    ActionKey = m_pState->GetEventHandler(nEvent);
    PROCESS_SUCCESS(ActionKey.nAIActionID == KAI_ACTION_ID_NONE);

    if (m_nDebugCount > 0)
        QLogPrintf(LOG_DEBUG, "[AI] <%u> On event: %d, src: %u, param: %d\n", m_pOwner->m_dwId, nEvent, dwEventSrc, nEventParam);

    if (nEventBlock != aevInvalid)
    {
        // 重复事件替代
        if (m_nPendingEvent == aevInvalid || m_nPendingEvent == nEvent)
        {
            m_nPendingEvent = nEvent;
            m_dwPendingEventSrc = dwEventSrc;
            m_nPendingEventParam = nEventParam;
        }
        else
        {
            QLogPrintf(
                LOG_ERR, "[AI] Event %d blocked by %d, PendingEvent %d, ai type %d, npc name is %s.\n", 
                nEvent, nEventBlock, m_nPendingEvent, m_nAIType, m_pOwner->GetName()
            );
        }
        goto EXIT0;
    }

    m_nCurrentEvent = nEvent;

    m_pOwner->m_AIData.dwAIEventSrc         = dwEventSrc;
    m_pOwner->m_AIData.nAIEventParam        = nEventParam;
    m_pOwner->m_AIData.nTargetGroupCount    = 0;
    memset(m_pOwner->m_AIData.TargetGroup, 0, sizeof(m_pOwner->m_AIData.TargetGroup));

    while (ActionKey.nAIActionID != KAI_ACTION_ID_NONE)
    {
        KAIActionHandle NextActionKey;

        if (nCallCount > MAX_ACTION_CALL)
        {
            // 死循环检测
            QLogPrintf(
                LOG_ERR, "[AI] Action call up to limit(%d), ai type = %d, event = %d, action = %d\n", 
                MAX_ACTION_CALL, m_nAIType, nEvent, ActionKey.nAIActionID
            );
            break;
        }

        if (m_nDebugCount > 0)
        {
			ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

			if (!cSafeScript->CallTableFunction("AI", "DebugOnCallAction", 0, "dd", ActionKey.nAIActionID, m_nAIType))
            {
                QLogPrintf(LOG_DEBUG, "[AI] <%u> Call action %d\n", m_pOwner->m_dwId, ActionKey.nAIActionID);
            }
        }
		
        NextActionKey = m_pAILogic->CallAction(m_pOwner, ActionKey);
        if (NextActionKey.nAIActionID == KAI_ACTION_ID_ERROR)
        {
            QLogPrintf(
                LOG_ERR, "[AI] Action call return error, AI type = %d, event = %d, action = %d, character id = %u, name:%s, pos(%d, %d, %d)\n", 
                m_nAIType, nEvent, ActionKey.nAIActionID, m_pOwner->m_dwId, m_pOwner->GetName(), m_pOwner->m_nX, m_pOwner->m_nY, m_pOwner->m_nZ
            );

            m_pAILogic = NULL;  // 出错的时候就把AI清掉，防止Log不断刷屏。
            break;
        }

        if (m_nDebugCount > 0)
            m_nDebugCount--;

        ActionKey = NextActionKey;
        m_nActionID = ActionKey.nAIActionID;
        nCallCount++;
    }

    m_pOwner->m_AIData.dwAIEventSrc = ERROR_ID;

EXIT1:
EXIT0:
    m_nCurrentEvent = nEventBlock;

    if (g_sAIManager.m_bLogAIRuntimeStat)
    {
        ulPostTickCount = GetTickCount();
        m_ullRunTimeStatistic += (ulPostTickCount - ulPrevTickCount);
    }

    return;
}

#undef MAX_ACTION_CALL

BOOL KAIVM::Active()
{
	PROCESS_ERROR(IS_NPC(m_rcCharacter.m_dwId));
	PROCESS_ERROR(m_rcCharacter.GetMoveState() != cmsOnDeath);

	assert(m_pOwner);

    if (m_nPendingEvent != aevInvalid)
    {
        int nEvent = m_nPendingEvent;
        m_nPendingEvent = aevInvalid;

        FireEvent(nEvent, m_dwPendingEventSrc, m_nPendingEventParam);
    }

    if (m_nPrimaryTimerFrame != 0 && g_cOrpgWorld.m_dwLogicFrames >= m_nPrimaryTimerFrame)
    {
        m_nPrimaryTimerFrame = 0;
        FireEvent(aevOnPrimaryTimer, m_pOwner->m_dwId, g_cOrpgWorld.m_dwLogicFrames);
    }

    if (m_nSecondaryTimerFrame != 0 && g_cOrpgWorld.m_dwLogicFrames >= m_nSecondaryTimerFrame)
    {
        m_nSecondaryTimerFrame = 0;
        FireEvent(aevOnSecondaryTimer, m_pOwner->m_dwId, g_cOrpgWorld.m_dwLogicFrames);
    }

    if (m_nTertiaryTimerFrame != 0 && g_cOrpgWorld.m_dwLogicFrames >= m_nTertiaryTimerFrame)
    {
        m_nTertiaryTimerFrame = 0;
        FireEvent(aevOnTertiaryTimer, m_pOwner->m_dwId, g_cOrpgWorld.m_dwLogicFrames);
    }
EXIT0:
	return TRUE;
}

BOOL KAIVM::SetState(int nState)
{
    BOOL        bResult = false;
    KAIState*   pState  = NULL;

    assert(m_pAILogic);

    pState = m_pAILogic->GetState(nState);
    LOG_PROCESS_ERROR(pState);

    m_pState    = pState;
    m_nStateID  = nState;

    bResult = true;
EXIT0:
    return bResult;
}

void KAIVM::SetPrimaryTimer(int nFrame)
{
    LOG_PROCESS_ERROR(nFrame > 0);

    m_nPrimaryTimerFrame = g_cOrpgWorld.m_dwLogicFrames + nFrame;
EXIT0:
    return;
}

void KAIVM::SetSecondaryTimer(int nFrame)
{
    LOG_PROCESS_ERROR(nFrame > 0);

    m_nSecondaryTimerFrame = g_cOrpgWorld.m_dwLogicFrames + nFrame;
EXIT0:
    return;
}

void KAIVM::SetTertiaryTimer(int nFrame)
{
    LOG_PROCESS_ERROR(nFrame > 0);

    m_nTertiaryTimerFrame = g_cOrpgWorld.m_dwLogicFrames + nFrame;
EXIT0:
    return;
}

void KAIVM::AddupTotalDamageValue(int nDeltaDamageValue)
{
    assert(m_pOwner);

    m_pOwner->m_AIData.nAITotalDamageValue += nDeltaDamageValue;
    return;
}

void KAIVM::DebugAICurrentStateInfo()
{
    assert(m_pState);

    QLogPrintf(LOG_DEBUG, "[AI] DebugAI : AIType is %d, Current StateID is %d, ActionID is %d.\n", m_nAIType, m_nStateID, m_nActionID);
    QLogPrintf(LOG_DEBUG, "[AI] DebugAI : nGameLoop is %u\n", g_cOrpgWorld.m_dwLogicFrames);
    QLogPrintf(LOG_DEBUG, "[AI] DebugAI : PrimaryTimer is %d\n", m_nPrimaryTimerFrame);
    QLogPrintf(LOG_DEBUG, "[AI] DebugAI : SecondaryTimer is %d\n", m_nSecondaryTimerFrame);
    QLogPrintf(LOG_DEBUG, "[AI] DebugAI : TertiaryTimer is %d\n", m_nTertiaryTimerFrame);
    m_pState->DebugStateInfo();

    return;
}