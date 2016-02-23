
#pragma once

// #include "engine/Luna.h"
#include "kluaaiaction.h"

class KCharacter;
class KAILogic;

class KAIState : public QLunaBase
{
public:
    KAIState(KAILogic* pAILogic) { assert(pAILogic); m_pAILogic = pAILogic; };

    KAIActionHandle GetEventHandler(INT nEvent);

private:
    struct KAI_EVENT_HANDLE  
    {
        INT             nEvent;
        KAIActionHandle ActionKey;
    };

    std::vector<KAI_EVENT_HANDLE>   m_EventHandleVector;    // Event handles
    KAILogic*                       m_pAILogic;

public:
    DECLARE_LUA_CLASS(KAIState);
    INT LuaHandleEvent(QLuaScript& sc);
    VOID DebugStateInfo();
};

