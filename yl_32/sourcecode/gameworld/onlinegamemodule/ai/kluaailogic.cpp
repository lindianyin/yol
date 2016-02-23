
#include "stdafx.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/knpc.h"
#include "onlinegamemodule/ai/kluaaistate.h"
#include "onlinegamemodule/ai/kaivm.h"
#include "onlinegamemodule/ai/kaimanager.h"
#include "onlinegamemodule/ai/kluaailogic.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KAILogic::KAILogic()
{
    m_nAIType       = 0;
    m_dwScriptID    = 0;
    m_nInitState    = 0;
}

BOOL KAILogic::Setup(int nType, DWORD dwScriptID)
{
    BOOL      bResult       = FALSE;
    int       nLuaTopIndex  = 0;
	BOOL      bRetCode      = FALSE;
	LPCSTR		pszScriptName = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

    // 为了支持reload,这个函数可能被重新调用,所以先clear();
    m_ActionTable.clear();
    m_StateTable.clear();
    m_UserActionTable.clear();

    m_nAIType       = nType;
    m_dwScriptID    = dwScriptID;
    m_nInitState    = 0;

	pszScriptName = g_sAIManager.GetScriptNameById(dwScriptID);
	LOG_PROCESS_ERROR(pszScriptName);

	bRetCode = g_cScriptManager.LoadScript(pszScriptName, TRUE);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = cSafeScript->CallTableFunction("AI","Setup", 0, "o", (QLunaBase*)this);
	LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    return bResult;
}

KAIAction* KAILogic::GetAction(int nActionID)
{
    KAI_ACTION_TABLE::iterator it = m_ActionTable.find(nActionID);

    if (it != m_ActionTable.end())
    {
        return &it->second;
    }

    return NULL;
}

KAIState* KAILogic::GetState(int nState)
{
    KAI_STATE_TABLE::iterator it = m_StateTable.find(nState);

    if (it != m_StateTable.end())
    {
        return &it->second;
    }

    return NULL;
}

int KAILogic::GetInitState()
{
    return m_nInitState;
}

DWORD KAILogic::GetScriptID()
{
    return m_dwScriptID;
}

KAIActionHandle KAILogic::CallAction(KCharacter* pCharacter, KAIActionHandle ActionKey)
{
    KAIActionHandle Result;
    KAIActionHandle NextAction;
    KAIAction*      pAction     = ActionKey.pAIAction;
    int             nActionKey  = 0;

    if (!pAction)
        pAction = GetAction(ActionKey.nAIActionID);

    if (pAction == NULL)
    {
        QLogPrintf(LOG_ERR, "[AI] Invalid action call(%d) in AI(%d)", ActionKey.nAIActionID, m_nAIType);
        goto EXIT0;
    }
	
    nActionKey = pAction->m_nKey;

    if (nActionKey < KAI_USER_ACTION)
    {
        int             nBranchIndex    = 0;
        unsigned long   ulPrevTickCount = 0;
        unsigned long   ulPostTickCount = 0;
        KAI_ACTION_FUNC PAction         = NULL;

        if (g_sAIManager.m_bLogAIRuntimeStat)
            ulPrevTickCount = GetTickCount();

        PAction = g_sAIManager.GetActionFunction(nActionKey);
        LOG_PROCESS_ERROR(PAction);

        nBranchIndex = (*PAction)(pCharacter, pAction);

        if (nBranchIndex > 0 && nBranchIndex <= KAI_ACTION_BRANCH_NUM)
        {
            int         nNextActionID   = pAction->m_nBranch[nBranchIndex - 1];
            KAIAction*  pNextAction     = pAction->m_pBranch[nBranchIndex - 1];

            if (!pNextAction)
            {
                pNextAction = GetAction(nNextActionID);
                pAction->m_pBranch[nBranchIndex - 1] = pNextAction;
            }

            NextAction.nAIActionID  = nNextActionID;
            NextAction.pAIAction    = pNextAction;
        }

        if (nBranchIndex == -1)
        {
            NextAction.nAIActionID = KAI_ACTION_ID_NONE;
            NextAction.pAIAction = NULL;
        }

        if (g_sAIManager.m_bLogAIRuntimeStat)
        {
            ulPostTickCount = GetTickCount();

            g_sAIManager.m_ActionRunTimeStatisticsArray[nActionKey].ullTime += ulPostTickCount - ulPrevTickCount;
            g_sAIManager.m_ActionRunTimeStatisticsArray[nActionKey].uCount++;
        }
    }
    else
    {
		int nBranchIndex = 0;
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		size_t uSeperatorIndex = std::string::npos;

        KUSER_ACTION_TABLE::iterator it = m_UserActionTable.find(nActionKey);
        LOG_PROCESS_ERROR(it != m_UserActionTable.end());

		uSeperatorIndex = it->second.find_first_of(':');

		if (std::string::npos != uSeperatorIndex)
		{
			std::string strTableName = it->second.substr(0, uSeperatorIndex);
			std::string strFuncName = it->second.substr(uSeperatorIndex + 1, it->second.length() - uSeperatorIndex - 1);

			cSafeScript->CallTableFunction(strTableName.c_str(), strFuncName.c_str(), 1, "oo",
				pCharacter->GetScriptInterface(), (QLunaBase*)this);
		}
		else
		{
			cSafeScript->CallTableFunction("AI", it->second.c_str(), 1, "oo", pCharacter->GetScriptInterface(), (QLunaBase*)this);
		}

		nBranchIndex = cSafeScript->GetInt(-1);

        if (nBranchIndex >= 1 && nBranchIndex <= KAI_ACTION_BRANCH_NUM)
        {
            int         nNextActionID   = pAction->m_nBranch[nBranchIndex - 1];
            KAIAction*  pNextAction     = pAction->m_pBranch[nBranchIndex - 1];

            if (!pNextAction)
            {
                pNextAction = GetAction(nNextActionID);
                pAction->m_pBranch[nBranchIndex - 1] = pNextAction;
            }

            NextAction.nAIActionID  = nNextActionID;
            NextAction.pAIAction    = pNextAction;
        }
    }

    Result = NextAction;
EXIT0:
    return Result;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KAILogic
// @LuaApiName		: AIType
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: INTEGER
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KAILogic::getAIType(VOID) CONST
{
	return m_nAIType;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KAILogic
// @LuaApiName		: NewAction
// @Description		: 建立一个新的Action
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: nActionId
// @ArgumentComment	: nActionKey
// @LuaMarkEnd
// -------------------------------------------------------------------------
int KAILogic::LuaNewAction(QLuaScript& sc)
{
    int         nResult     = 0;
    int         nRetCode    = 0;
    int         nActionID   = KAI_ACTION_ID_NONE;
    int         nActionKey  = eakInvalid;
    KAIAction*  pActionData = NULL;
    std::pair<KAI_ACTION_TABLE::iterator, bool> RetPair;

    nRetCode = sc.GetTopIndex();
    LOG_PROCESS_ERROR(nRetCode == 2);

    nActionID = sc.GetInt(1);
    LOG_PROCESS_ERROR(nActionID > KAI_ACTION_ID_NONE);

    nActionKey = sc.GetInt(2);
    LOG_PROCESS_ERROR(nActionKey > eakInvalid);

    if (nActionKey < KAI_USER_ACTION)
    {
        KAI_ACTION_FUNC PAction  = NULL;

        PAction = g_sAIManager.GetActionFunction(nActionKey);
        if (PAction == NULL)
        {
            QLogPrintf(LOG_ERR, "[AI] Unregistered sys action(ID: %d, Key: %d) in ai %d\n", nActionID, nActionKey, m_nAIType);
            goto EXIT0;
        }
    }
    else
    {
        KUSER_ACTION_TABLE::iterator it = m_UserActionTable.find(nActionKey);

        if (it == m_UserActionTable.end())
        {
            QLogPrintf(LOG_ERR, "[AI] Unregistered lua actionID: %d, Key: %d) in ai %d\n", nActionID, nActionKey, m_nAIType);
            goto EXIT0;
        }
    }

    RetPair = m_ActionTable.insert(std::make_pair(nActionID, KAIAction()));
    if (!RetPair.second)
    {
        QLogPrintf(LOG_ERR, "[AI] Duplicated AI action(%d) in ai %d\n", nActionID, m_nAIType);
        goto EXIT0;
    }

    pActionData = &(RetPair.first->second);

    pActionData->m_nKey = nActionKey;

	nResult = sc.PushObj(pActionData);
EXIT0:
    return nResult;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KAILogic
// @LuaApiName		: NewState
// @Description		: 建立一个新的状态结点
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nStateId 状态结点Id
// @LuaMarkEnd
// -------------------------------------------------------------------------
int KAILogic::LuaNewState(QLuaScript& sc)
{
    int                     nResult     = 0;
    int                     nRetCode    = 0;
    int                     nState      = 0;
    KAIState*               pState      = NULL;
    std::pair<KAI_STATE_TABLE::iterator, bool> RetPair;

    nRetCode = sc.GetTopIndex();
    LOG_PROCESS_ERROR(nRetCode == 1);

    nState = sc.GetInt(1);
    LOG_PROCESS_ERROR(nState > 0);

    RetPair = m_StateTable.insert(std::make_pair(nState, KAIState(this)));
    if (!RetPair.second)
    {
        QLogPrintf(LOG_ERR, "[AI] Duplicated AI state(%d) in ai type %d\n", nState, m_nAIType);
        goto EXIT0;
    }

    pState = &(RetPair.first->second);

	nResult = sc.PushObj(pState);
EXIT0:
    return nResult;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KAILogic
// @LuaApiName		: SetInitState
// @Description		: 设置初始状态结点
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: nInitState 初始化时候的状态结点ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
int KAILogic::LuaSetInitState(QLuaScript& sc)
{
    int         nRetCode    = 0;
    int         nInitState  = 0;
    KAIState*   pState      = NULL;

    nRetCode = sc.GetTopIndex();
    LOG_PROCESS_ERROR(nRetCode == 1);

    nInitState = sc.GetInt(1);
    LOG_PROCESS_ERROR(nInitState > 0);

    pState = GetState(nInitState);
    LOG_PROCESS_ERROR(pState);

    m_nInitState = nInitState;
EXIT0:
    return 0;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KAILogic
// @LuaApiName		: RegisterUserAction
// @Description		: 注册自定义的Action
// @ReturnCode		: 
// @ArgumentFlag	: ds
// @ArgumentComment	: nActionKey
// @ArgumentComment	: pszFunction 函数名称
// @LuaMarkEnd
// -------------------------------------------------------------------------
int KAILogic::LuaRegisterUserAction(QLuaScript& sc)
{
    int         nRetCode    = 0;
    int         nKey        = 0;
	const char* pszFunction = NULL;

    nRetCode = sc.GetTopIndex();
    LOG_PROCESS_ERROR(nRetCode == 2);

    nKey = sc.GetInt(1);
    LOG_PROCESS_ERROR(nKey >= KAI_USER_ACTION);

    pszFunction = sc.GetStr(2);
    LOG_PROCESS_ERROR(pszFunction);

	m_UserActionTable[nKey] = pszFunction;
EXIT0:
    return 0;
}

#include "kluaailogic_table.hpp"
//-----------------------------------------------------------------------