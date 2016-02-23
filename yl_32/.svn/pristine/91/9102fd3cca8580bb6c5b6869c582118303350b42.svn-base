
#pragma once

#include <map>
#include <string>

#include "kluaaiaction.h"
#include "kluaaistate.h"
#include "kluaailogic.h"
#include "onlinegameworld/kmodulebase.h"

struct KAI_RUNTIME_STATISTICS_NODE 
{
    KAI_RUNTIME_STATISTICS_NODE() { ullTime = 0; uCount = 0; };

    UINT64	        ullTime;
    unsigned int    uCount;
};

class KAIManager: public KModuleBase
{
public:
	KAIManager();
	virtual ~KAIManager(){}


    BOOL PreInit();
    BOOL OnUnInit();

    void RegisterActionFunctions();
    BOOL LoadAITabFile();
    BOOL ReloadAILogic(int nAIType);

    BOOL LogAIRuntimeStat();

    KAILogic*       GetAILogic(int nAIType);
    KAI_ACTION_FUNC GetActionFunction(int nKey);
	LPCSTR			GetScriptNameById(DWORD dwScriptId);

public:
    BOOL                                            m_bLogAIRuntimeStat;
    std::map<DWORD, KAI_RUNTIME_STATISTICS_NODE>    m_NpcAIRunTimeStatisticsMap;
    KAI_RUNTIME_STATISTICS_NODE                     m_ActionRunTimeStatisticsArray[eakTotal];

private:
    KAILogic* CreateAI(int nType, DWORD dwScriptID);

private:
    KAI_ACTION_FUNC m_ActionFunctionTable[eakTotal];

	struct KAIInfo
	{
		std::string       strScriptFile;
        DWORD       dwScriptID;
        KAILogic*   pLogic;
    };

    typedef std::map<int, KAIInfo> KAI_TABLE;
    KAI_TABLE       m_AITable;
};
extern KAIManager g_sAIManager;

