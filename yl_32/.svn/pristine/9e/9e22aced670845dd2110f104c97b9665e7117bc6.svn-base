

#include "stdafx.h"
#include "GameCenterMonitor.h"
#include "GameCenter.h"
#include "fseye/fseye_protocol.h"

#define REGISTER_GCEYES_FUNCTION(id, func)                      \
    do                                                          \
    {                                                           \
        m_ProtocolFunctions[id] = &KGameCenterEyes::func;       \
    } while(false)

KGameCenterEyes::KGameCenterEyes()
{
}

KGameCenterEyes::~KGameCenterEyes()
{
}

BOOL KGameCenterEyes::Init()
{
    BOOL bResult            = false;
    BOOL bRetCode           = false;
    BOOL bStreamInitFlag    = false;

    assert(g_pGameCenter);

    bRetCode = m_Stream.Initialize(emKGUARDSERVER_FIRSTID + emKGUARDSERVER_COORDINATOR, 200 * 1024);
    LOG_PROCESS_ERROR(bRetCode);
    bStreamInitFlag = true;

    memset(m_ProtocolFunctions, 0, sizeof(m_ProtocolFunctions));

    m_nNextPingTime             = 0;
    m_nNextSendPlayerTime       = 0;
    m_nNextSendCustomInfoTime   = 0;
    m_fFluxTotal                = 0.0f;
    m_fMainLoopIdle             = 0.0f;
    m_fRoleDBIdle               = 0.0f;
    m_fMiscDBIdle               = 0.0f;

    REGISTER_GCEYES_FUNCTION(e2l_getpid_def, OnE2LGetPid);
    REGISTER_GCEYES_FUNCTION(e2l_exit_def, OnE2LStop);
    REGISTER_GCEYES_FUNCTION(e2l_header_def, OnE2LProtocol);
    REGISTER_GCEYES_FUNCTION(g2l_header_def, OnG2LProtocol);

    bResult = true;
EXIT0:
    if (!bResult && bStreamInitFlag)
    {
        m_Stream.UnInitialize();
        bStreamInitFlag = false;
    }
    return bResult;
}

void KGameCenterEyes::UnInit()
{
    m_Stream.UnInitialize();
}

void KGameCenterEyes::Activate()
{
    size_t              nDataLen            = 0;
	char*               pData               = NULL;
    BYTE                byProtocol          = 0;
    PROTOCOL_FUNCTION   pfnProtocolFunction = NULL;
    
    assert(g_pGameCenter);

    while (true)
    {
        pData = m_Stream.GetPack(nDataLen);

        if (pData == NULL)
        {
            break;
        }
        
        assert(nDataLen > 0);
        
        byProtocol = *(BYTE*)pData;
        
        pfnProtocolFunction = m_ProtocolFunctions[byProtocol];
        if (pfnProtocolFunction == NULL)
        {
            m_Stream.PopPack();
            continue;
        }

        (this->*pfnProtocolFunction)((BYTE*)pData, nDataLen);
        m_Stream.PopPack();
    }

    if (g_pGameCenter->m_nTimeNow >= m_nNextPingTime)
    {
        DoPing();
        m_nNextPingTime = g_pGameCenter->m_nTimeNow + 20;
    }

    if (g_pGameCenter->m_nTimeNow >= m_nNextSendPlayerTime)
    {
        DoSendPlayerCount();
        m_nNextSendPlayerTime = g_pGameCenter->m_nTimeNow + 3;
    }

    if (g_pGameCenter->m_nTimeNow >= m_nNextSendCustomInfoTime)
    {
        DoSendCustomInfo();
        m_nNextSendCustomInfoTime = g_pGameCenter->m_nTimeNow + 2;
    }
}

void KGameCenterEyes::OnE2LStop(BYTE* pbyData, size_t uDataLen)
{
    QLogPrintf(LOG_INFO, "Receive quit signal from kingeyes !\n");
    g_pGameCenter->Quit();
}

void KGameCenterEyes::OnE2LGetPid(BYTE* pbyData, size_t uDataLen)
{
    l2e_getpid  sMsg;

	sMsg.wProtocol = l2e_getpid_def;
	sMsg.wServer   = 0;
	sMsg.nPid	   = getpid();
	
    m_Stream.PutPack(&sMsg, sizeof(sMsg));
    
    return;
}

void KGameCenterEyes::OnG2LProtocol(BYTE* pbyData, size_t uDataLen)
{
	KSubProtocolHead*   pHead   = (KSubProtocolHead*)pbyData;

	if (pHead->wSubProtocol == emKPROTOCOL_G2L_PIDREQ)
	{
        KPROTOCOL_L2G_PIDRET sRet;
        
        LOG_PROCESS_ERROR(uDataLen == sizeof(KSubProtocolHead));
		
        sRet.wProtocol		= l2g_header_def;
		sRet.wSubProtocol	= emKPROTOCOL_L2G_PIDRET;
		sRet.nPid			= getpid();
		sRet.wServer		= 0;
		m_Stream.PutPack(&sRet, sizeof(sRet));
	}

EXIT0:
    return;
}

void KGameCenterEyes::OnE2LProtocol(BYTE* pbyData, size_t uDataLen)
{
    KSubProtocolHead*   pHead   = (KSubProtocolHead*)pbyData;

    //if (pHead->wSubProtocol == e2l_gmcmd_gs_def)
    //    OnE2LGmCommandGS(pbyData, uDataLen);
    //else if (pHead->wSubProtocol == e2l_gmcmd_gc_def)
    //    OnE2LGmCommandGC(pbyData, uDataLen);
    //else if (pHead->wSubProtocol == e2l_remote_lua_call_def)
    //    OnE2LRemoteLuaCall(pbyData, uDataLen);
}
//
//void KGameCenterEyes::OnE2LGmCommandGS(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL            bRetCode    = false;
//    e2l_gmcmd_gs*   pGmCmdGs    = (e2l_gmcmd_gs*)pbyData;
//    KRole*          pRole       = NULL;
//    
//    LOG_PROCESS_ERROR(uDataLen >= sizeof(e2l_gmcmd_gs));
//
//    pGmCmdGs->szGmName[sizeof(pGmCmdGs->szGmName) - 1] = '\0';
//    pGmCmdGs->szRoleName[sizeof(pGmCmdGs->szRoleName) - 1] = '\0';
//
//    pRole = g_pGameCenter->m_RoleManager.GetRole(pGmCmdGs->szRoleName);
//    if (!pRole)
//    {
//        DoSendGmMessage(pGmCmdGs->szGmName, "PLAYER_NOT_FOUND");
//        goto EXIT0;
//    }
//
//    bRetCode = pRole->IsOnline();
//    if (!bRetCode)
//    {
//        DoSendGmMessage(pGmCmdGs->szGmName, "PLAYER_NOT_ONLINE");
//        goto EXIT0;
//    }
//
//    pGmCmdGs->szCommand[uDataLen - sizeof(e2l_gmcmd_gs) - 1] = '\0';
//
//    bRetCode = g_pGameCenter->m_GameServer.DoGmCommand(
//        pRole->m_nConnIndex, pRole->m_dwPlayerID, 
//        pGmCmdGs->szGmName, pGmCmdGs->szCommand
//    );
//    LOG_PROCESS_ERROR(bRetCode);
//
//EXIT0:
//    return;
//}
//
//void KGameCenterEyes::OnE2LGmCommandGC(BYTE* pbyData, size_t uDataLen)
//{
//    BOOL            bRetCode    = false;
//    e2l_gmcmd_gc*   pGmCmdGc    = (e2l_gmcmd_gc*)pbyData;
//
//    LOG_PROCESS_ERROR(uDataLen > sizeof(e2l_gmcmd_gc));
//
//    pGmCmdGc->szGmName[sizeof(pGmCmdGc->szGmName) - 1] = '\0';
//    pGmCmdGc->szCommand[uDataLen - sizeof(e2l_gmcmd_gc) - 1] = '\0';
//
//    QLogPrintf(LOG_INFO, "GMC[%s]: %s\n", pGmCmdGc->szGmName, pGmCmdGc->szCommand);
//
//    bRetCode = g_pGameCenter->m_ScriptManager.ExecuteGMCScript(pGmCmdGc->szGmName, pGmCmdGc->szCommand);
//    if (!bRetCode)
//    {
//        DoSendGmMessage(pGmCmdGc->szGmName, "SCRIPT_ERROR");
//        goto EXIT0;
//    }
//
//EXIT0:
//    return;
//}
//
//void KGameCenterEyes::OnE2LRemoteLuaCall(BYTE* pbyData, size_t uDataLen)
//{
//    e2l_remote_lua_call* pLuaCall = (e2l_remote_lua_call*)pbyData;
//
//    LOG_PROCESS_ERROR(uDataLen == sizeof(e2l_remote_lua_call) + pLuaCall->uParamLen);
//
//    pLuaCall->szFunction[sizeof(pLuaCall->szFunction) - 1] = '\0';
//
//    g_pGameCenter->m_RemoteScript.Accept(
//        0, pLuaCall->szFunction, pLuaCall->byParam, pLuaCall->uParamLen, false
//    );
//
//EXIT0:
//    return;
//}

void KGameCenterEyes::DoPing()
{
	KSubProtocolHead    sPing;

	sPing.wProtocol		= l2e_header_def;
	sPing.wSubProtocol	= l2e_ping_def;
	sPing.wServer		= 0;
	
    m_Stream.PutPack(&sPing, sizeof(sPing));
}

void KGameCenterEyes::DoSendPlayerCount()
{
	char                szBuff[sizeof(l2e_PlayerCount) + sizeof(int) * emKGUARD_PLAYERCOUNT_COUNT];
	l2e_PlayerCount*    pProtocol = (l2e_PlayerCount*)szBuff;

	pProtocol->wProtocol	= l2e_header_def;
	pProtocol->wServer		= 0;
	pProtocol->wSubProtocol = l2e_PlayerCount_def;
	pProtocol->uCount		= emKGUARD_PLAYERCOUNT_COUNT;

	int* pnCount = (int*)(pProtocol + 1);
	for (int i = 0; i < emKGUARD_PLAYERCOUNT_COUNT; i++)
	{
		pnCount[i] = -1;
	}

	pnCount[emKGUARD_PLAYERCOUNT_TOTOAL] = g_pGameCenter->m_MapManager.GetPlayerOnlineTotal();

	m_Stream.PutPack(szBuff, sizeof(szBuff));
}

#define MAX_CUSTOM_INFO_PAK 1024

void KGameCenterEyes::DoSendCustomInfo()
{
    l2e_update_custom_info* pInfo               = NULL;
    size_t                  uStrLen             = 0;
    IMemBlock*             piBuffer            = NULL;
    time_t                  nOperatingBegin     = 0;
    float                   fOperatingTime      = 0.0f;
    float                   fRunningTime        = 0.0f;
    int                     nOnlineTotal        = 0;
    int                     nRoleTotal          = 0;
    float                   fFluxTotal          = 0.0f;
    float                   fRoleSaveTimeCost   = 0.0f;
    float                   fRoleLoadTimeCost   = 0.0f;
    size_t                  uMemory             = 0;
    DWORD                   dwRoleDBIdle        = 0;
    DWORD                   dwMiscDBIdle        = 0;
    int                     nMiscDBSpeed        = 0;
    int                     nMailOperation      = 0;
    int                     nMailCacheFault     = 0;
    int                     nFarmerCount        = 0;
    int                     nMentorCount        = 0;
    
    assert(g_pGameCenter);

    //nOperatingBegin     = (time_t)g_pGameCenter->m_GlobalSystemValueManager.m_GameWorldStartTime.nValue;
    fOperatingTime      = (float)(g_pGameCenter->m_nTimeNow - nOperatingBegin) / 60.0f / 60.0f / 24.0f;
    fRunningTime        = g_pGameCenter->m_nWorkLoop / GAME_FPS / 3600.0f;
    nOnlineTotal        = g_pGameCenter->m_MapManager.GetPlayerOnlineTotal();
    nRoleTotal          = g_pGameCenter->m_RoleManager.GetRoleCountTotal();
    //uMemory             = KMemory::GetUsedSize();
    fFluxTotal          = (float)g_pGameCenter->m_GameServer.GetFluxTotal() / 1024.0f;
    fRoleSaveTimeCost   = g_pGameCenter->m_RoleDB.GetRoleSaveTimeCost();
    fRoleLoadTimeCost   = g_pGameCenter->m_RoleDB.GetRoleLoadTimeCost();
    dwRoleDBIdle        = g_pGameCenter->m_RoleDB.m_nDBThreadIdleLoop;
    dwMiscDBIdle        = g_pGameCenter->m_MiscDB.m_nDBThreadIdleLoop;
    nMiscDBSpeed        = (int)g_pGameCenter->m_MiscDB.GetOperationSpeed();
    nMailOperation      = g_pGameCenter->m_MailManager.GetCacheHitRatio();
    nMailCacheFault     = g_pGameCenter->m_MailManager.GetCacheMissRatio();
    //nFarmerCount        = g_pGameCenter->m_AntiFarmerManager.GetFarmerCount();
    //nMentorCount        = g_pGameCenter->m_MentorCenter.GetRelationCount();

    // 数值平滑处理
    m_fFluxTotal        = m_fFluxTotal * 0.9f + (float)fFluxTotal * 0.1f;
    m_fMainLoopIdle     = m_fMainLoopIdle * 0.9f + (float)g_pGameCenter->m_nIdleLoop * 0.1f; 
    m_fRoleDBIdle       = m_fRoleDBIdle * 0.9f + (float)dwRoleDBIdle * 0.1f;
    m_fMiscDBIdle       = m_fMiscDBIdle * 0.9f + (float)dwMiscDBIdle * 0.1f;

    piBuffer = QCreateMemBlock(MAX_CUSTOM_INFO_PAK);
    LOG_PROCESS_ERROR(piBuffer);

    pInfo = (l2e_update_custom_info*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pInfo);

    pInfo->wProtocol	= l2e_header_def;
    pInfo->wServer		= 0;
    pInfo->wSubProtocol = l2e_update_custom_info_def;

    uStrLen = snprintf(
        (char*)pInfo->byData, MAX_CUSTOM_INFO_PAK - sizeof(l2e_update_custom_info),
        "-------------------------------------\n"
        "Operating ... ... .. %.2f Days\n"
        "Running . ... ... .. %.2f H\n"
        "Online total ... ... %d\n"
        "Role total ... . ... %d\n"
        "Main loop idle .. .. %.1f\n"
        "RoleDB idle .. . ... %.1f\n"
        "MiscDB idle .. . ... %.1f\n"
        "MiscDB operation ... %d times/sec\n"
        "Memory ... . ... ... %.3f MB\n"
        "Flux total ... .. .. %.1f KB/sec\n"
        "RoleDB load ... . .. %.1f ms\n"
        "RoleDB save .. . ... %.1f ms\n"
        "Mail operation .. .. %d times/min\n"
        "Mail cache fault ... %d times/min\n"
        "Anti farmer .. ... . %d\n"
        "Mentor relation . .. %d\n"
        "-------------------------------------\n",
        fOperatingTime,
        fRunningTime,
        nOnlineTotal,
        nRoleTotal,
        m_fMainLoopIdle,
        m_fRoleDBIdle,
        m_fMiscDBIdle,
        nMiscDBSpeed,
        uMemory / 1024.0f / 1024.0f,
        m_fFluxTotal,
        fRoleLoadTimeCost,
        fRoleSaveTimeCost,
        nMailOperation,
        nMailCacheFault,
        nFarmerCount,
        nMentorCount
    );
    LOG_PROCESS_ERROR(uStrLen > 0);
    LOG_PROCESS_ERROR(uStrLen < MAX_CUSTOM_INFO_PAK - sizeof(l2e_update_custom_info));

    pInfo->uDataLen = uStrLen;

    m_Stream.PutPack(pInfo, sizeof(l2e_update_custom_info) + uStrLen);

EXIT0:
    SAFE_RELEASE(piBuffer);
    return;
}
//
//void KGameCenterEyes::DoSendGmMessage(const char cszGmName[], const char cszMessage[])
//{
//    l2e_send_gm_message*    pMessage    = NULL;
//    size_t                  uStrLen     = 0;
//    size_t                  uPakSize    = 0;
//    IMemBlock*             piBuffer    = NULL;
//
//    uStrLen = strlen(cszMessage);
//
//    uPakSize = sizeof(l2e_send_gm_message) + sizeof(char) * (uStrLen + 1);
//    piBuffer = QCreateMemBlock((unsigned int)uPakSize);
//    LOG_PROCESS_ERROR(piBuffer);
//
//    pMessage = (l2e_send_gm_message*)piBuffer->GetData();
//    LOG_PROCESS_ERROR(pMessage);
//
//    pMessage->wProtocol	    = l2e_send_gm_message_def;
//    pMessage->wServer	    = 0;
//
//    strncpy(pMessage->szGmName, cszGmName, sizeof(pMessage->szGmName));
//    pMessage->szGmName[sizeof(pMessage->szGmName) - 1] = '\0';
//
//    strncpy(pMessage->szMessage, cszMessage, uStrLen + 1);
//    pMessage->szMessage[uStrLen] = '\0';
//
//    m_Stream.PutPack(pMessage, uPakSize);
//
//EXIT0:
//    SAFE_RELEASE(piBuffer);
//    return;
//}
//
//void KGameCenterEyes::DoSendGmChn(ProtocolDef eType, const char cszAccount[], const char cszRole[], const char cszText[])
//{
//    l2e_gmchn*  pGmChn      = NULL;
//    size_t      uStrLen     = 0;
//    size_t      uPakSize    = 0;
//    IMemBlock* piBuffer    = NULL;
//    char*       pszText     = NULL;
//
//    LOG_PROCESS_ERROR(
//        eType == l2e_artificial_help_def || 
//        eType == l2e_report_trick_def   ||
//        eType == l2e_report_cheat_def
//    );
//
//    LOG_PROCESS_ERROR(cszAccount);
//    LOG_PROCESS_ERROR(cszRole);
//    LOG_PROCESS_ERROR(cszText);
//
//    uStrLen = strlen(cszText);
//
//    uPakSize = sizeof(l2e_gmchn) + sizeof(char) * (uStrLen + 1);
//    piBuffer = QCreateMemBlock((unsigned int)uPakSize);
//    LOG_PROCESS_ERROR(piBuffer);
//
//    pGmChn = (l2e_gmchn*)piBuffer->GetData();
//    LOG_PROCESS_ERROR(pGmChn);
//
//    pGmChn->wSubProtocol    = (WORD)eType;
//    pGmChn->wProtocol       = (WORD)l2e_gmchn_def;
//    pGmChn->wServer	        = 0;
//    pGmChn->nMsgLen         = (int)uStrLen + 1;
//
//    strncpy(pGmChn->szAccount, cszAccount, sizeof(pGmChn->szAccount));
//    pGmChn->szAccount[sizeof(pGmChn->szAccount) - 1] = '\0';
//
//    strncpy(pGmChn->szRole, cszRole, sizeof(pGmChn->szRole));
//    pGmChn->szRole[sizeof(pGmChn->szRole) - 1] = '\0';
//
//    pszText = (char*)pGmChn + sizeof(l2e_gmchn);
//
//    strncpy(pszText, cszText, uStrLen + 1);
//    pszText[uStrLen] = '\0';
//
//    m_Stream.PutPack(pGmChn, uPakSize);
//
//EXIT0:
//    SAFE_RELEASE(piBuffer);
//    return;
//}
//
//void KGameCenterEyes::DoSendReport(int nType, const char cszKey[], const char cszValue[])
//{
//    l2e_report* pReport     = NULL;
//    size_t      uStrLen     = 0;
//    size_t      uPakSize    = 0;
//    IMemBlock* piBuffer    = NULL;
//    char*       pszValue    = NULL;
//
//    uStrLen = strlen(cszValue);
//
//    uPakSize = sizeof(l2e_report) + sizeof(char) * (uStrLen + 1);
//    piBuffer = QCreateMemBlock((unsigned int)uPakSize);
//    LOG_PROCESS_ERROR(piBuffer);
//
//    pReport = (l2e_report*)piBuffer->GetData();
//    LOG_PROCESS_ERROR(pReport);
//
//    pReport->wProtocol      = l2e_header_def;
//    pReport->wServer	    = 0;
//    pReport->wSubProtocol   = l2e_report_def;
//    pReport->nType          = nType;
//    pReport->nValueLen      = (int)uStrLen;
//
//    strncpy(pReport->szKey, cszKey, sizeof(pReport->szKey));
//    pReport->szKey[sizeof(pReport->szKey) - 1] = '\0';
//
//    pszValue = (char*)pReport + sizeof(l2e_report);
//
//    strncpy(pszValue, cszValue, uStrLen + 1);
//    pszValue[uStrLen] = '\0';
//
//    m_Stream.PutPack(pReport, uPakSize);
//
//EXIT0:
//    SAFE_RELEASE(piBuffer);
//    return;
//}
//
//void KGameCenterEyes::DoSendPrizeInfo(const char szAccount[], const char szRoleName[], DWORD dwRoleID, const char szType[], int nValue)
//{
//    l2e_prize_info Info;
//
//    assert(szAccount);
//    assert(szRoleName);
//    assert(szType);
//
//    Info.wProtocol      = (WORD)l2e_header_def;
//    Info.wSubProtocol   = (WORD)l2e_prize_info_def;
//
//    strncpy(Info.szAccount, szAccount, sizeof(Info.szAccount));
//    Info.szAccount[sizeof(Info.szAccount) - 1] = '\0';
//
//    strncpy(Info.szRoleName, szRoleName, sizeof(Info.szRoleName));
//    Info.szRoleName[sizeof(Info.szRoleName) - 1] = '\0';
//
//    strncpy(Info.szType, szType, sizeof(Info.szType));
//    Info.szType[sizeof(Info.szType) - 1] = '\0';
//
//    Info.dwRoleID = dwRoleID;
//    Info.nValue = nValue;
//
//    m_Stream.PutPack(&Info, sizeof(Info));
//
//// EXIT0:
//    return;
//}
