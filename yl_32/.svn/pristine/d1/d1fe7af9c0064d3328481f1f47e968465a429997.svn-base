
#include "stdafx.h"
#include "onlinegamemodule/ai/kluaaistate.h"
#include "onlinegamemodule/ai/kluaaiaction.h"
#include "onlinegamemodule/ai/kaimanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KAIActionHandle KAIState::GetEventHandler(int nEvent)
{
    KAIActionHandle Result;

    Result.nAIActionID  = KAI_ACTION_ID_NONE;
    Result.pAIAction    = NULL;

    for (int i = 0; i < (int)m_EventHandleVector.size(); i++)
    {
        if (m_EventHandleVector[i].nEvent == nEvent)
        {
            if (!m_EventHandleVector[i].ActionKey.pAIAction)
            {
                KAIAction* pAction = NULL;
                pAction = m_pAILogic->GetAction(m_EventHandleVector[i].ActionKey.nAIActionID);
                LOG_PROCESS_ERROR(pAction);

                m_EventHandleVector[i].ActionKey.pAIAction = pAction;
            }

            Result = m_EventHandleVector[i].ActionKey;
            break;
        }
    }

EXIT0:
    return Result;
}

void KAIState::DebugStateInfo()
{
    for (int i = 0; i < (int)m_EventHandleVector.size(); i++)
    {
        int nEvent = m_EventHandleVector[i].nEvent;
        int nAction = m_EventHandleVector[i].ActionKey.nAIActionID;

        QLogPrintf(LOG_DEBUG, "[AI] CurrentState Have AIEvent<%d>, Next Action is %d\n", nEvent, nAction);
    }

    return;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KAIState
// @LuaApiName		: HandleEvent
// @Description		: 事件响应
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 事件ID
// @ArgumentComment	: ActionID
// @LuaMarkEnd
// -------------------------------------------------------------------------
int KAIState::LuaHandleEvent(QLuaScript& sc)
{
    int                 nRetCode    = 0;
    int                 nEvent      = 0;
    int                 nAction     = 0;
    KAI_EVENT_HANDLE    Handle;

    nRetCode = sc.GetTopIndex();
    LOG_PROCESS_ERROR(nRetCode == 2);

    nEvent  = sc.GetInt(1);
    nAction = sc.GetInt(2);

    LOG_PROCESS_ERROR(nAction > KAI_ACTION_ID_NONE);

    for (int i = 0; i < (int)m_EventHandleVector.size(); i++)
    {
        if (m_EventHandleVector[i].nEvent == nEvent)
        {
            QLogPrintf(
                LOG_ERR, "[AI] Duplicated event handler(%d, %d) <--> (%d, %d)\n", 
                nEvent, nAction, m_EventHandleVector[i].nEvent, m_EventHandleVector[i].ActionKey.nAIActionID
            );
            goto EXIT0;
        }
    }

    Handle.nEvent = nEvent;

    Handle.ActionKey.nAIActionID = nAction;
    Handle.ActionKey.pAIAction = NULL;

    m_EventHandleVector.push_back(Handle);

EXIT0:
    return 0;
}

#include "kluaaistate_table.hpp"
//--------------------------------------------------------------------------------