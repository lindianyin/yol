
#pragma once


#include "kluaaistate.h"
#include "kluaaiaction.h"
#include "onlinegameworld/kluacharacter.h"

class KAIState;
class KCharacter;

class KAILogic : public QLunaBase
{
public:
    KAILogic();

    BOOL            Setup(int nType, DWORD dwScriptID);

    KAIAction*      GetAction(int nActionID);
    KAIState*       GetState(int nState);
    int             GetInitState();
    DWORD           GetScriptID();

    KAIActionHandle CallAction(KCharacter* pCharacter, KAIActionHandle ActionKey);

private:
    int     m_nAIType;      // AI typeµÄID
    DWORD   m_dwScriptID;
    int     m_nInitState;

    typedef std::map<int, KAIAction>  KAI_ACTION_TABLE;
    KAI_ACTION_TABLE      m_ActionTable;

    typedef std::map<int, KAIState> KAI_STATE_TABLE;
    KAI_STATE_TABLE       m_StateTable;

    typedef std::map<int, std::string> KUSER_ACTION_TABLE;
    KUSER_ACTION_TABLE    m_UserActionTable;

public:
    DECLARE_LUA_CLASS(KAILogic);

	INT	getAIType(VOID) CONST;

    int LuaNewAction(QLuaScript& sc);
    int LuaNewState(QLuaScript& sc);
    int LuaSetInitState(QLuaScript& sc);
    int LuaRegisterUserAction(QLuaScript& sc);
};

