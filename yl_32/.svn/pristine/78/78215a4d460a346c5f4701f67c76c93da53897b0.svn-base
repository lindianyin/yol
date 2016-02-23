
#include "stdafx.h"
#include "onlinegamemodule/ai/kluaaiaction.h"
#include "onlinegamemodule/ai/kaimanager.h"
#include "onlinegamemodule/ai/kluaailogic.h"
#include "config/kconfigfile.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KAIManager g_sAIManager;

template<>
IKModuleInterface* GetModuleInterFace<emKMODULE_AI>()
{
	return &g_sAIManager;
}

KAIManager::KAIManager() : 
KModuleBase(emKMODULE_AI),
m_bLogAIRuntimeStat(FALSE)
{}

BOOL KAIManager::PreInit()
{
    BOOL        bResult     = false;
    BOOL        bRetCode    = false;
    IIniFile*   piIniFile   = NULL;

    RegisterActionFunctions();

#ifdef GAME_SERVER
    QLogPrintf(LOG_DEBUG, "[AI] loading ... ...");

    bRetCode = LoadAITabFile();
    LOG_PROCESS_ERROR(bRetCode);

    QLogPrintf(LOG_DEBUG, "[AI] %u AI loaded !", m_AITable.size());

    memset(m_ActionRunTimeStatisticsArray, 0, sizeof(m_ActionRunTimeStatisticsArray));
    m_NpcAIRunTimeStatisticsMap.clear();

    m_bLogAIRuntimeStat = 0;

    piIniFile = g_OpenIniFile(KDF_SERVER_CFG);
    LOG_PROCESS_ERROR(piIniFile);

    bRetCode = piIniFile->GetInteger("AI", "LogAIRuntimeStat", 0, &m_bLogAIRuntimeStat);
    //LOG_PROCESS_ERROR(bRetCode);
#endif

    bResult = true;
EXIT0:
    SAFE_RELEASE(piIniFile);
    return bResult;
}

BOOL AIRunTimeStatisticsComp(const std::pair<DWORD, KAI_RUNTIME_STATISTICS_NODE>& crLeft, const std::pair<DWORD, KAI_RUNTIME_STATISTICS_NODE>& crRight)
{
    if (crLeft.second.ullTime == crRight.second.ullTime)
        return crLeft.second.uCount < crRight.second.uCount;

    return crLeft.second.ullTime > crRight.second.ullTime;
}

BOOL KAIManager::LogAIRuntimeStat()
{
    BOOL        bResult         = false;
	time_t      nTime           = KSysService::Time(NULL);
    FILE*       pNpcStatFile    = NULL;
    FILE*       pActionStatFile = NULL;
    char        szFileName[_MAX_PATH];
    struct tm   tmNow; 
    std::map<DWORD, KAI_RUNTIME_STATISTICS_NODE>::iterator  it;
    std::map<DWORD, KAI_RUNTIME_STATISTICS_NODE>::iterator  itEnd;

    localtime_r(&nTime, &tmNow);

    if (!pNpcStatFile)
    {
        snprintf(
            szFileName, sizeof(szFileName), 
            "AIRuntimeStat/%d%2.2d%2.2d-%2.2d%2.2d%2.2d_Npc.tab",
            tmNow.tm_year + 1900,
            tmNow.tm_mon + 1,
            tmNow.tm_mday,
            tmNow.tm_hour,
            tmNow.tm_min,
            tmNow.tm_sec
        );
        szFileName[sizeof(szFileName) - 1] = '\0';

#ifdef unix
		mkdir("AIRuntimeStat", 0777);
#else
		mkdir("AIRuntimeStat");
#endif // unix

        pNpcStatFile = fopen(szFileName, "w");
        PROCESS_ERROR(pNpcStatFile);

        fprintf(pNpcStatFile, "NpcTemp\tTime\tCount\t\n");
    }

    for (it = m_NpcAIRunTimeStatisticsMap.begin(), itEnd = m_NpcAIRunTimeStatisticsMap.end(); it != itEnd; ++it)
        fprintf(pNpcStatFile, "%u\t%llu\t%u\n", it->first, it->second.ullTime, it->second.uCount);

    if (!pActionStatFile)
    {
        snprintf(
            szFileName, sizeof(szFileName), 
            "AIRuntimeStat/%d%2.2d%2.2d-%2.2d%2.2d%2.2d_Action.tab",
            tmNow.tm_year + 1900,
            tmNow.tm_mon + 1,
            tmNow.tm_mday,
            tmNow.tm_hour,
            tmNow.tm_min,
            tmNow.tm_sec
        );
        szFileName[sizeof(szFileName) - 1] = '\0';

#ifdef unix
		mkdir("AIRuntimeStat", 0777);
#else
		mkdir("AIRuntimeStat");
#endif // unix

        pActionStatFile = fopen(szFileName, "w");
        PROCESS_ERROR(pActionStatFile);

        fprintf(pActionStatFile, "Action\tTime\tCount\t\n");
    }

    for (int i = 0; i < sizeof(m_ActionRunTimeStatisticsArray) / sizeof(m_ActionRunTimeStatisticsArray[0]); i++)
        fprintf(pActionStatFile, "%d\t%llu\t%u\n", i, m_ActionRunTimeStatisticsArray[i].ullTime, m_ActionRunTimeStatisticsArray[i].uCount);

    bResult = true;
EXIT0:
    if (pNpcStatFile)
    {
        fclose(pNpcStatFile);
        pNpcStatFile = NULL;
    }

    if (pActionStatFile)
    {
        fclose(pActionStatFile);
        pActionStatFile = NULL;
    }

    return bResult;
}

BOOL KAIManager::OnUnInit()
{
    for (KAI_TABLE::iterator it = m_AITable.begin(); it != m_AITable.end(); ++it)
    {
        if (it->second.pLogic)
        {
			SAFE_DELETE(it->second.pLogic);
        }
    }

    m_AITable.clear();

#ifdef GAME_SERVER
    if (m_bLogAIRuntimeStat)
        LogAIRuntimeStat();
#endif
	return TRUE;
}

BOOL KAIManager::LoadAITabFile()
{
    BOOL        bResult                 = false;
    int         nRetCode                = false;
    ITabFile*   piAITabFile             = NULL;
    char        szFilePath[]            = SETTING_DIR"/ai/ai_type.txt";
    int         nHeight                 = 0;
    std::pair<KAI_TABLE::iterator, bool> RetPair;

    piAITabFile = g_OpenTabFile(szFilePath);
    if (!piAITabFile)
    {
        QLogPrintf(LOG_ERR, "[AI] Unable to open table file \"%s\"\n", szFilePath);
        goto EXIT0;
    }

    nHeight = piAITabFile->GetHeight();
    LOG_PROCESS_ERROR(nHeight > 1);

    for (int nRow = 2; nRow <= nHeight; nRow++)
    {
        int                 nAIType         = 0;
        DWORD               dwScriptID      = 0;
        char                szScriptPath[MAX_PATH];
        KAIInfo             AIInfo;

        nRetCode = piAITabFile->GetInteger(nRow, "AIType", 0, &nAIType);
        LOG_PROCESS_ERROR(nRetCode);
        LOG_PROCESS_ERROR(nAIType >= 0);

        nRetCode = piAITabFile->GetString(nRow, "ScriptFile", "", szScriptPath, sizeof(szScriptPath));
		LOG_PROCESS_ERROR(nRetCode);

        dwScriptID = g_FileNameHash(szScriptPath);
        LOG_PROCESS_ERROR(dwScriptID);

		AIInfo.strScriptFile = szScriptPath;
        AIInfo.dwScriptID = dwScriptID;
        AIInfo.pLogic     = NULL;

        m_AITable[nAIType] = AIInfo;
    }

#ifdef GAME_SERVER
	// ÔÝÊ±ÓÃtrue ´úÌæ !g_pWorld->m_bFastBootMode
    if (true)
    {
        KAI_TABLE::iterator it = m_AITable.begin();
        int nCount = (int)m_AITable.size();
        int nIndex = 1;

        for (;it != m_AITable.end(); ++it, ++nIndex)
        {
#if (defined(_MSC_VER) || defined(__ICL))   //WIN32
			cprintf( "******************************** Setup AI scripts : %d/%d\r", nIndex, nCount);
#endif
            it->second.pLogic   = CreateAI(it->first, it->second.dwScriptID);
        }
#if (defined(_MSC_VER) || defined(__ICL))   //WIN32
        cprintf("\n");
#endif
    }
#endif

    bResult = true;
EXIT0:
    if (!bResult)
    {
        m_AITable.clear();
    }
    SAFE_RELEASE(piAITabFile);
    return bResult;
}

BOOL KAIManager::ReloadAILogic(int nAIType)
{
    BOOL                bResult                 = false;
    int                 nRetCode                = false;
    KAILogic*           pLogic                  = NULL;
    ITabFile*           piAITabFile             = NULL;
    char                szFilePath[]            = SETTING_DIR"/AIType.tab";
    int                 nHeight                 = 0;
    DWORD               dwScriptID              = 0;
    int                 nRow                    = 0;
    int                 nReadAIType             = 0;
    KAI_TABLE::iterator it;
    char                szScriptPath[MAX_PATH];

    piAITabFile = g_OpenTabFile(szFilePath);
    LOG_PROCESS_ERROR(piAITabFile);

    nHeight = piAITabFile->GetHeight();
    LOG_PROCESS_ERROR(nHeight > 1);

    for (nRow = 2; nRow <= nHeight; nRow++)
    {
        nRetCode = piAITabFile->GetInteger(nRow, "AIType", 0, &nReadAIType);
        LOG_PROCESS_ERROR(nRetCode == 1);

        if (nReadAIType != nAIType)
            continue;

        nRetCode = piAITabFile->GetString(nRow, "ScriptFile", "", szScriptPath, sizeof(szScriptPath));
        LOG_PROCESS_ERROR(nRetCode == 1);

        break;
    }

    LOG_PROCESS_ERROR(nRow <= nHeight);

    //nRetCode = g_pWorld->m_ScriptCenter.ReloadScript(szScriptPath);
    //LOG_PROCESS_ERROR(nRetCode);

    dwScriptID = g_FileNameHash(szScriptPath);
    LOG_PROCESS_ERROR(dwScriptID);

    it = m_AITable.find(nAIType);
    if (it == m_AITable.end() || it->second.pLogic == NULL)
	{
		m_AITable[nAIType].strScriptFile = szScriptPath;
        m_AITable[nAIType].dwScriptID   = dwScriptID;
        m_AITable[nAIType].pLogic       = CreateAI(nAIType, dwScriptID);
    }
    else
	{
		it->second.strScriptFile = szScriptPath;
        it->second.dwScriptID = dwScriptID;

        nRetCode = it->second.pLogic->Setup(nAIType, dwScriptID);
        LOG_PROCESS_ERROR(nRetCode);
    }

    bResult = true;
EXIT0:
    return bResult;
}

KAILogic* KAIManager::GetAILogic(int nAIType)
{
    KAILogic*           pLogic  = NULL;
    KAIInfo*            pInfo   = NULL;
    KAI_TABLE::iterator it      = m_AITable.find(nAIType);

    LOG_PROCESS_ERROR(it != m_AITable.end());

    pInfo = &it->second;

    if (!pInfo->pLogic)
    {
        pInfo->pLogic = CreateAI(nAIType, pInfo->dwScriptID);
    }

    pLogic = pInfo->pLogic;
EXIT0:
    return pLogic;
}

KAI_ACTION_FUNC KAIManager::GetActionFunction(int nKey)
{
    if (nKey > eakInvalid && nKey < eakTotal)
    {
        return m_ActionFunctionTable[nKey];
    }

    return NULL;
}

KAILogic* KAIManager::CreateAI(int nType, DWORD dwScriptID)
{
    KAILogic* pResult       = NULL;
    BOOL      bRetCode      = false;
    KAILogic* pAI           = NULL;

	pAI = new KAILogic();
    LOG_PROCESS_ERROR(pAI);

    bRetCode = pAI->Setup(nType, dwScriptID);
    LOG_PROCESS_ERROR(bRetCode);

    pResult = pAI;
EXIT0:
    if (!pResult)
    {
        if (pAI)
        {
			SAFE_DELETE(pAI);
        }

        QLogPrintf(LOG_ERR, "[AI] Setup AI failed, AIType: %d", nType);
    }
    return pResult;
}

LPCTSTR KAIManager::GetScriptNameById(DWORD dwScriptId)
{
	KAI_TABLE::iterator it;

	for(it = m_AITable.begin(); it != m_AITable.end(); ++it)
	{
		KAIInfo& rAIInfo = it->second;

		if(rAIInfo.dwScriptID == dwScriptId)
		{
			return rAIInfo.strScriptFile.c_str();
		}
	}

	return NULL;
}