#include "stdafx.h"
#include "GatewayAgency.h"
#include "GameCenter.h"
#include "onlinegamebase/ksysservice.h"

#define REGISTER_G2R_FUNCTION(id, func, data)               \
    do                                                      \
    {                                                       \
        m_ProtocolFunctions[id] = &KGatewayAgency::func;    \
        m_uPakSize[id]          = sizeof(data);             \
    } while(false)

KGatewayAgency::KGatewayAgency()
{
     m_piSocket         = NULL;
     m_nLastPingTime    = 0;
     m_nPingCycle       = 30;
     m_bQuiting         = false;
}

KGatewayAgency::~KGatewayAgency()
{
}

BOOL KGatewayAgency::Init()
{
    BOOL                bResult         = false;
    BOOL                bRetCode        = false;
    timeval             TimeoutValue    = {0, 0};
    int                 nOpenPortFlag   = false;
    
    m_nPingCycle = g_pGameCenter->m_Settings.m_nGatewayPingCycle;

    bRetCode = m_Acceptor.Open(g_pGameCenter->m_Settings.m_szGatewayIP, g_pGameCenter->m_Settings.m_nGatewayPort);
    LOG_PROCESS_ERROR(bRetCode);
    nOpenPortFlag = true;

    m_Acceptor.SetTimeout(&TimeoutValue);   

    memset(m_ProtocolFunctions, 0, sizeof(m_ProtocolFunctions));
    memset(m_uPakSize, 0, sizeof(m_uPakSize));

    REGISTER_G2R_FUNCTION(g2r_ping_signal, OnPingSignal, G2R_PING_SIGNAL);
    REGISTER_G2R_FUNCTION(g2r_query_account_state, OnQueryAccountState, G2R_QUERY_ACCOUNT_STATE);
    REGISTER_G2R_FUNCTION(g2r_query_role_list, OnQueryRoleList, G2R_QUERY_ROLE_LIST);
    REGISTER_G2R_FUNCTION(g2r_query_newbie_maps, OnQueryNewbieMaps, G2R_QUERY_NEWBIE_MAPS);
    REGISTER_G2R_FUNCTION(g2r_kick_account_notify, OnKickAccountNotify, G2R_KICK_ACCOUNT_NOTIFY);
	REGISTER_G2R_FUNCTION(g2r_random_name_request, OnRandomNameRequest, G2R_RANDOM_NAME_REQUEST);
    REGISTER_G2R_FUNCTION(g2r_create_role_request, OnCreateRoleRequest, G2R_CREATE_ROLE_REQUEST);
    REGISTER_G2R_FUNCTION(g2r_delete_role_request, OnDeleteRoleRequest, G2R_DELETE_ROLE_REQUEST);
    REGISTER_G2R_FUNCTION(g2r_game_login_request, OnGameLoginRequest, G2R_GAME_LOGIN_REQUEST);
    REGISTER_G2R_FUNCTION(g2r_change_ext_point_respond, OnChangeExtPointRespond, G2R_CHANGE_EXT_POINT_RESPOND);
    REGISTER_G2R_FUNCTION(g2r_set_charge_flag_respond, OnSetChargeFlagRespond, G2R_SET_CHARGE_FLAG_RESPOND);
    REGISTER_G2R_FUNCTION(g2r_get_zone_charge_flag_notify, OnGetZoneChargeFlagNotify, G2R_GET_ZONE_CHARGE_FLAG_NOTIFY);
    REGISTER_G2R_FUNCTION(g2r_active_present_code_respond, OnActivePresentCodeRespond, G2R_ACTIVE_PRESENT_CODE_RESPOND);
    REGISTER_G2R_FUNCTION(g2r_freeze_coin_respond, OnFreezeCoinRespond, G2R_FREEZE_COIN_RESPOND);
    REGISTER_G2R_FUNCTION(g2r_account_exchange_respond, OnAccountExchangeRespond, G2R_ACCOUNT_EXCHANGE_RESPOND);
    REGISTER_G2R_FUNCTION(g2r_rename_request, OnRenameRequest, G2R_RENAME_REQUEST);

    bResult = true;
EXIT0:
    if (!bResult)
    {
        if (nOpenPortFlag)
        {
            m_Acceptor.Close();
            nOpenPortFlag = false;
        }
    }
    return bResult;
}

void KGatewayAgency::UnInit()
{
    SAFE_RELEASE(m_piSocket);
    m_Acceptor.Close();
}

void KGatewayAgency::Activate()
{
    ProcessAcceptor();

    ProcessPackage();

    CheckTimeout();
}

BOOL KGatewayAgency::ProcessAcceptor()
{
    BOOL                bResult                     = false;
    BOOL                bRetCode                    = false;
    ISocketStream*   piSocket                    = NULL;
    timeval             IOTimeout                   = {5, 0};
    struct in_addr      RemoteAddr                  = { 0 };
    u_short             wRemotePortNet              = 0;
    u_short             wRemotePortHost             = 0;
    char*               pszRetString                = NULL;
    int                 nLowerProtocolVersion       = 0;
    int                 nUpperProtocolVersion       = 0;

    PROCESS_ERROR(m_piSocket == NULL);

    piSocket = m_Acceptor.Accept();
    PROCESS_ERROR(piSocket);

    piSocket->SetTimeout(&IOTimeout);

    piSocket->GetRemoteAddress(&RemoteAddr, &wRemotePortNet);

    wRemotePortHost = ntohs(wRemotePortNet);

    pszRetString = inet_ntoa(RemoteAddr);
    LOG_PROCESS_ERROR(pszRetString);

    m_piSocket = piSocket;
    m_piSocket->AddRef();
    
    m_nLastPingTime = g_pGameCenter->m_nTimeNow;

    bRetCode = g_pGameCenter->m_GameServer.GetGameWorldProtocolVersion(
        &nLowerProtocolVersion, &nUpperProtocolVersion
    );
    if (bRetCode)
    {        
        DoSetGameWorldVersion(nLowerProtocolVersion, nUpperProtocolVersion);
    }

    QLogPrintf(LOG_INFO, "Gateway connected from %s:%u\n", pszRetString, wRemotePortHost);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSocket);
    return bResult;
}

BOOL KGatewayAgency::ProcessPackage()
{
    BOOL                    bResult             = false;
    int                     nRetCode            = 0;
    IMemBlock*             piRecvPackage       = NULL;
    size_t                  uBufferSize         = 0;

    while (m_piSocket)
    {
        timeval                 TimeoutValue        = {0, 0};
        PROTOCOL_FUNCTION       pfnProtocolFunction = NULL;
        KGR_PROTOCOL_HEADER*    pProtocolHeader     = NULL;
        int                     nProtocol           = 0;

        nRetCode = m_piSocket->CheckCanRecv(&TimeoutValue);
        if (nRetCode == 0)
            break;

        LOG_PROCESS_ERROR(nRetCode == 1);

        SAFE_RELEASE(piRecvPackage);

        nRetCode = m_piSocket->Recv(&piRecvPackage);
        LOG_PROCESS_ERROR(nRetCode == 1);

        LOG_PROCESS_ERROR(piRecvPackage);

        m_nLastPingTime = g_pGameCenter->m_nTimeNow;

        uBufferSize = (size_t)piRecvPackage->GetSize();
        LOG_PROCESS_ERROR(uBufferSize >= sizeof(KGR_PROTOCOL_HEADER));

        pProtocolHeader = (KGR_PROTOCOL_HEADER*)piRecvPackage->GetData();
        LOG_PROCESS_ERROR(pProtocolHeader);
        
        nProtocol = pProtocolHeader->byProtocol;
        LOG_PROCESS_ERROR(nProtocol > g2r_protocol_begin);
        LOG_PROCESS_ERROR(nProtocol < g2r_protocol_total);

        LOG_PROCESS_ERROR(uBufferSize >= m_uPakSize[nProtocol]);

        pfnProtocolFunction = m_ProtocolFunctions[nProtocol];
        LOG_PROCESS_ERROR(pfnProtocolFunction);

        (this->*pfnProtocolFunction)((BYTE*)pProtocolHeader, uBufferSize);
    }

    bResult = true;
EXIT0:
    if (!bResult)
    {
        if (m_piSocket)
        {
            QLogPrintf(LOG_INFO, "[Gateway] Connection lost !\n");
        }

        SAFE_RELEASE(m_piSocket);
    }
    SAFE_RELEASE(piRecvPackage);
    return bResult;
}

BOOL KGatewayAgency::CheckTimeout()
{
    DWORD   dwTimeNow = 0;

    PROCESS_ERROR(m_piSocket);

    if (g_pGameCenter->m_nTimeNow > m_nLastPingTime + m_nPingCycle)
    {
        QLogPrintf(LOG_INFO, "[Gateway] connection timeout !\n");
        
        SAFE_RELEASE(m_piSocket);
    }

EXIT0:
    return true;
}

BOOL KGatewayAgency::DoSetGameWorldVersion(int nLowerVersion, int nUpperVersion)
{
    BOOL                                bResult          = false;
    BOOL                                bRetCode         = false;
    R2G_SET_GAME_WORLD_VERSION*         pSetVersion      = NULL;
    IMemBlock*                         piSendBuffer     = NULL;

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2G_SET_GAME_WORLD_VERSION));
    LOG_PROCESS_ERROR(piSendBuffer);

    pSetVersion = (R2G_SET_GAME_WORLD_VERSION*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pSetVersion);

    pSetVersion->byProtocol         = r2g_set_game_world_version;
    pSetVersion->nLowerVersion      = nLowerVersion;
    pSetVersion->nUpperVersion      = nUpperVersion;

    bRetCode = Send(piSendBuffer);
    PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}
// 随机名字
BOOL KGatewayAgency::DoSyncRandomName(int nPlayerIndex, CHAR szRandomName[], INT nSex)
{
	BOOL                        bResult             = false;
	BOOL                        bRetCode            = false;
	BOOL                        bRename             = false;
	IMemBlock*                 piSendBuffer        = NULL;
	size_t                      uSendBufferSize     = 0;
	R2G_SYNC_RANDOM_NAME*         pRespond            = NULL;

	uSendBufferSize = sizeof(R2G_SYNC_RANDOM_NAME);

	piSendBuffer = QCreateMemBlock((unsigned)uSendBufferSize);
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (R2G_SYNC_RANDOM_NAME*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol    = r2g_sync_random_name;
	pRespond->nPlayerIndex = nPlayerIndex;
	pRespond->nSex = nSex;
	memcpy(pRespond->szRandomName, szRandomName, _NAME_LEN);
	//pRespond->szRandomName  = szRandomName;

	bRetCode = Send(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KGatewayAgency::DoSyncRoleList(
    int nPlayerIndex, int nRoleIndex, int nRoleCount, DWORD dwRoleID, KRoleBaseInfo* pBaseInfo
)
{
    BOOL                        bResult             = false;
    BOOL                        bRetCode            = false;
    BOOL                        bRename             = false;
    IMemBlock*                 piSendBuffer        = NULL;
    size_t                      uSendBufferSize     = 0;
    R2G_SYNC_ROLE_LIST*         pRespond            = NULL;

    uSendBufferSize = sizeof(R2G_SYNC_ROLE_LIST);
    if (pBaseInfo != NULL)
    {
        uSendBufferSize += sizeof(KROLE_LIST_INFO);
    }

    piSendBuffer = QCreateMemBlock((unsigned)uSendBufferSize);
    LOG_PROCESS_ERROR(piSendBuffer);

    pRespond = (R2G_SYNC_ROLE_LIST*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pRespond);

    pRespond->byProtocol    = r2g_sync_role_list;
    pRespond->nPlayerIndex  = nPlayerIndex;
    pRespond->nRoleIndex    = nRoleIndex;
    pRespond->nRoleCount    = nRoleCount;
    pRespond->dwRoleID      = KD_BAD_ID;

    if (pBaseInfo != NULL)
    {
        KRole*              pRole       = NULL;
        KROLE_LIST_INFO*    pListInfo   = (KROLE_LIST_INFO*)pRespond->byData;
        
        pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleID);
        LOG_PROCESS_ERROR(pRole);
        
        pRespond->dwRoleID      = dwRoleID;
        
        bRename = g_pGameCenter->m_RoleManager.IsCanRename(dwRoleID);
        pListInfo->byCanRename = (BYTE)bRename;

        pListInfo->BaseInfo    = *pBaseInfo;
        pListInfo->dwPlayerID  = dwRoleID;
        
        strncpy(pListInfo->szRoleName, pRole->m_szName, sizeof(pListInfo->szRoleName));
        pListInfo->szRoleName[sizeof(pListInfo->szRoleName) - 1] = '\0';
        
        strncpy(pListInfo->szAccount, pRole->m_pszAccount, sizeof(pListInfo->szAccount));
        pListInfo->szAccount[sizeof(pListInfo->szAccount) - 1] = '\0';

        pListInfo->nFreezeTime = g_pGameCenter->m_RoleManager.GetRoleFreezeTime(dwRoleID);
        pListInfo->nDeleteTime = g_pGameCenter->m_RoleManager.GetRoleDeleteTime(dwRoleID);
    }

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoSyncNewbieMaps(int nPlayerIndex)
{
    BOOL                        bResult         = false;
    BOOL                        bRetCode        = false;
    const QBORNPLACE_MAP_TABLE*  pBornMaps  = NULL;
    IMemBlock*                 piSendBuffer    = NULL;
    R2G_SYNC_NEWBIE_MAPS*       pRespond        = NULL;

    pBornMaps = g_pGameCenter->m_BornPlaceList.GetBornPlaceTable();
    LOG_PROCESS_ERROR(pBornMaps);

    for (QBORNPLACE_MAP_TABLE::const_iterator it = pBornMaps->begin(); it != pBornMaps->end(); ++it)
    {
        KNEWBIE_MAP_COPY_INFO*  pCopyInfo    = NULL;
        KMapInfo*               pMap         = NULL;
        int                     nCopyCount   = 0;

        pMap = g_pGameCenter->m_MapManager.GetMapInfo(*it);
        LOG_PROCESS_ERROR(pMap);

        nCopyCount = (int)pMap->m_CopyTable.size();
        LOG_PROCESS_ERROR(nCopyCount <= UCHAR_MAX);

        SAFE_RELEASE(piSendBuffer);

        piSendBuffer = QCreateMemBlock((unsigned)(sizeof(R2G_SYNC_NEWBIE_MAPS) + sizeof(KNEWBIE_MAP_COPY_INFO) * nCopyCount));
        LOG_PROCESS_ERROR(piSendBuffer);

        pRespond = (R2G_SYNC_NEWBIE_MAPS*)piSendBuffer->GetData();
        LOG_PROCESS_ERROR(pRespond);

        pRespond->byProtocol    = r2g_sync_newbie_maps;
        pRespond->nPlayerIndex  = nPlayerIndex;
        pRespond->dwMapID       = *it;
        pRespond->nCopyCount    = nCopyCount;

        pCopyInfo = (KNEWBIE_MAP_COPY_INFO*)pRespond->CopyInfo;

        for (KMAP_COPY_TABLE::iterator CopyIter = pMap->m_CopyTable.begin(); CopyIter != pMap->m_CopyTable.end(); ++CopyIter)
        {
            int nPlayerCount = CopyIter->second.m_nPlayerCount;
            int nMaxPlayer   = pMap->m_nMaxPlayerCount;

            LOG_PROCESS_ERROR(nMaxPlayer > 0);

            pCopyInfo->nCopyIndex  = CopyIter->first;
            pCopyInfo->nLoadFactor = nPlayerCount * 100 / nMaxPlayer;

            pCopyInfo++;
        }

        bRetCode = Send(piSendBuffer);
        LOG_PROCESS_ERROR(bRetCode);
    }


    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoCreateRoleRespond(
    BYTE byCode, unsigned long ulIdentity, KROLE_LIST_INFO* pRoleListInfo
)
{
    BOOL                        bResult             = false;
    BOOL                        bRetCode            = false;
    IMemBlock*                 piSendBuffer        = NULL;
    unsigned                    uSendBufferSize     = 0;
    R2G_CREATE_ROLE_RESPOND*    pRespond            = NULL;

    uSendBufferSize = sizeof(R2G_CREATE_ROLE_RESPOND);

    if (byCode == eCreateRoleSucceed)
    {
        uSendBufferSize += sizeof(KROLE_LIST_INFO); 
    }

    piSendBuffer = QCreateMemBlock(uSendBufferSize);
    LOG_PROCESS_ERROR(piSendBuffer);

    pRespond = (R2G_CREATE_ROLE_RESPOND*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pRespond);

    pRespond->byProtocol    = r2g_create_role_respond;
    pRespond->nPlayerIndex  = (int)ulIdentity;
    pRespond->byCode        = byCode;
    pRespond->dwRoleID      = pRoleListInfo ? pRoleListInfo->dwPlayerID : KD_BAD_ID;

    if (byCode == eCreateRoleSucceed)
    {
        memcpy(pRespond->byData, pRoleListInfo, sizeof(KROLE_LIST_INFO));
    }

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoDeleteRoleRespond(int nPlayerIndex, DWORD dwRoleID, int nCode, time_t nDeleteTime)
{
    BOOL                        bResult            = false;
    BOOL                        bRetCode           = false;
    IMemBlock*                 piSendBuffer       = NULL;
    R2G_DELETE_ROLE_RESPOND*    pDeleteRoleRespond = NULL;

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2G_DELETE_ROLE_RESPOND));
    LOG_PROCESS_ERROR(piSendBuffer);

    pDeleteRoleRespond = (R2G_DELETE_ROLE_RESPOND*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pDeleteRoleRespond);

    pDeleteRoleRespond->byProtocol      = r2g_delete_role_respond;
    pDeleteRoleRespond->nPlayerIndex    = nPlayerIndex;
    pDeleteRoleRespond->byCode          = (BYTE)nCode;
    pDeleteRoleRespond->dwRoleID        = dwRoleID;
    pDeleteRoleRespond->nDeleteTime     = nDeleteTime;

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoGameLoginRespond(
    int nPlayerIndex, int nRespondCode, DWORD dwRoleID, DWORD dwQueueID, int nLoginIP, int nLoginPort, GUID Guid
)
{
    BOOL                    bResult         = false;
    BOOL                    bRetCode        = false;
    R2G_GAME_LOGIN_RESPOND* pRespond        = NULL;
    IMemBlock*             piSendBuffer    = NULL;

    piSendBuffer = QCreateMemBlock(sizeof(R2G_GAME_LOGIN_RESPOND));
    LOG_PROCESS_ERROR(piSendBuffer);

    pRespond = (R2G_GAME_LOGIN_RESPOND*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pRespond);

    pRespond->byProtocol    = r2g_game_login_respond;
    pRespond->nPlayerIndex  = nPlayerIndex;
    pRespond->byCode        = (BYTE)nRespondCode;
    pRespond->dwRoleID      = dwRoleID;
    pRespond->dwQueueID     = dwQueueID;
    pRespond->dwIP          = nLoginIP;
    pRespond->nPort         = nLoginPort;
    pRespond->guid          = Guid;

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoPlayerLeaveGame(const char cszAccountName[])
{
    BOOL                    bResult         = false;
    BOOL                    bRetCode        = false;
    IMemBlock*             piSendBuffer    = NULL;
    R2G_PLAYER_LEAVE_GAME*  pNotify         = NULL;

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2G_PLAYER_LEAVE_GAME));
    LOG_PROCESS_ERROR(piSendBuffer);

    pNotify = (R2G_PLAYER_LEAVE_GAME*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pNotify);

    pNotify->byProtocol = r2g_player_leave_game;

    strncpy(pNotify->szAccount, cszAccountName, sizeof(pNotify->szAccount));
    pNotify->szAccount[sizeof(pNotify->szAccount) - 1] = '\0';

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoChangeExtPointRequest(const char* pszAccountName, DWORD dwRoleID, WORD wExtPointIndex, WORD wChangeValue)
{
    BOOL                            bResult  = false;
    BOOL                            bRetCode = false;
    IMemBlock*                     piBuffer = NULL;
    R2G_CHANGE_EXT_POINT_REQUEST*   pRequest = NULL;

    assert(pszAccountName);

    piBuffer = QCreateMemBlock((unsigned)sizeof(R2G_CHANGE_EXT_POINT_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (R2G_CHANGE_EXT_POINT_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->byProtocol = r2g_change_ext_point_request;

    strncpy(pRequest->szAccount, pszAccountName, sizeof(pRequest->szAccount));
    pRequest->szAccount[sizeof(pRequest->szAccount) - 1] = '\0';

    pRequest->dwRoleID          = dwRoleID;
    pRequest->wExtPointIndex    = wExtPointIndex;
    pRequest->wChangeValue      = wChangeValue;

    bRetCode = Send(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoSetChargeFlagRequest(const char* pszAccountName, DWORD dwRoleID, DWORD dwLastLoginIP, int nChargeFlag)
{
    BOOL                            bResult  = false;
    BOOL                            bRetCode = false;
    IMemBlock*                     piBuffer = NULL;
    R2G_SET_CHARGE_FLAG_REQUEST*    pRequest = NULL;

    assert(pszAccountName);

    piBuffer = QCreateMemBlock((unsigned)sizeof(R2G_SET_CHARGE_FLAG_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (R2G_SET_CHARGE_FLAG_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->byProtocol = r2g_set_charge_flag_request;

    strncpy(pRequest->szAccount, pszAccountName, sizeof(pRequest->szAccount));
    pRequest->szAccount[sizeof(pRequest->szAccount) - 1] = '\0';

    pRequest->dwRoleID      = dwRoleID;
    pRequest->dwLastLoginIP = dwLastLoginIP;
    pRequest->nChargeFlag   = nChargeFlag;

    bRetCode = Send(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoActivePresentCodeRequest(const char* pszAccountName, DWORD dwRoleID, const char* pszPresentCode, DWORD dwActiveIP)
{
    BOOL                                bResult  = false;
    BOOL                                bRetCode = false;
    IMemBlock*                         piBuffer = NULL;
    R2G_ACTIVE_PRESENT_CODE_REQUEST*    pRequest = NULL;

    assert(pszAccountName);
    assert(pszPresentCode);

    piBuffer = QCreateMemBlock((unsigned)sizeof(R2G_ACTIVE_PRESENT_CODE_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (R2G_ACTIVE_PRESENT_CODE_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->byProtocol = r2g_active_present_code_request;

    strncpy(pRequest->szAccount, pszAccountName, sizeof(pRequest->szAccount));
    pRequest->szAccount[sizeof(pRequest->szAccount) - 1] = '\0';

    strncpy(pRequest->szPresentCode, pszPresentCode, sizeof(pRequest->szPresentCode));
    pRequest->szPresentCode[sizeof(pRequest->szPresentCode) - 1] = '\0';

    pRequest->dwRoleID      = dwRoleID;
    pRequest->dwActiveIP    = dwActiveIP;

    bRetCode = Send(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoFreezeCoinRequest(DWORD dwRequestID, const char* pszAccountName, BOOL bFreezeFlag, int nCoin)
{
    BOOL                        bResult  = false;
    BOOL                        bRetCode = false;
    IMemBlock*                 piBuffer = NULL;
    R2G_FREEZE_COIN_REQUEST*    pRequest = NULL;

    assert(pszAccountName);

    piBuffer = QCreateMemBlock((unsigned)sizeof(R2G_FREEZE_COIN_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (R2G_FREEZE_COIN_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->byProtocol    = r2g_freeze_coin_request;
    pRequest->dwRequestID   = dwRequestID;

    strncpy(pRequest->szAccount, pszAccountName, sizeof(pRequest->szAccount));
    pRequest->szAccount[sizeof(pRequest->szAccount) - 1] = '\0';

    pRequest->bFreezeFlag   = bFreezeFlag;
    pRequest->nCoin         = nCoin;

    bRetCode = Send(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoAccountExchangeRequest(
    DWORD dwRequestID, const char* pszSrcAccount, const char* pszDstAccount, DWORD dwExchangeType,
    DWORD dwDstAmount, DWORD dwSrcAmount
)
{
    BOOL                            bResult  = false;
    BOOL                            bRetCode = false;
    IMemBlock*                     piBuffer = NULL;
    R2G_ACCOUNT_EXCHANGE_REQUEST*   pRequest = NULL;

    assert(pszSrcAccount);
    assert(pszDstAccount);

    piBuffer = QCreateMemBlock((unsigned)sizeof(R2G_ACCOUNT_EXCHANGE_REQUEST));
    LOG_PROCESS_ERROR(piBuffer);

    pRequest = (R2G_ACCOUNT_EXCHANGE_REQUEST*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRequest);

    pRequest->byProtocol    = r2g_account_exchange_request;
    pRequest->dwRequestID   = dwRequestID;

    strncpy(pRequest->szSrcAccount, pszSrcAccount, sizeof(pRequest->szSrcAccount));
    pRequest->szSrcAccount[sizeof(pRequest->szSrcAccount) - 1] = '\0';

    strncpy(pRequest->szDstAccount, pszDstAccount, sizeof(pRequest->szDstAccount));
    pRequest->szDstAccount[sizeof(pRequest->szDstAccount) - 1] = '\0';

    pRequest->dwExchangeType    = dwExchangeType;
    pRequest->dwDstAmount       = dwDstAmount;
    pRequest->dwSrcAmount       = dwSrcAmount;

    bRetCode = Send(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KGatewayAgency::DoRenameRespond(BYTE byCode, int nPlayerIndex, DWORD dwRoleID, KROLE_LIST_INFO* pRoleListInfo)
{
    BOOL                bResult         = false;
    BOOL                bRetCode        = false;
    IMemBlock*         piBuffer        = NULL;
    R2G_RENAME_RESPOND* pRenameRespond  = NULL;
    size_t              uSendSize       = 0;

    uSendSize = sizeof(R2G_RENAME_RESPOND);

    if (pRoleListInfo)
    {
        uSendSize += sizeof(KROLE_LIST_INFO);
    }

    piBuffer = QCreateMemBlock((unsigned)uSendSize);
    LOG_PROCESS_ERROR(piBuffer);

    pRenameRespond = (R2G_RENAME_RESPOND*)piBuffer->GetData();
    LOG_PROCESS_ERROR(pRenameRespond);

    pRenameRespond->byProtocol      = r2g_rename_respond;
    pRenameRespond->nPlayerIndex    = nPlayerIndex;
    pRenameRespond->byCode          = byCode;
    pRenameRespond->dwRoleID        = dwRoleID;

    if (pRoleListInfo)
    {
        pRoleListInfo->nDeleteTime  = g_pGameCenter->m_RoleManager.GetRoleDeleteTime(dwRoleID);
        pRoleListInfo->nFreezeTime  = g_pGameCenter->m_RoleManager.GetRoleFreezeTime(dwRoleID);
        pRoleListInfo->byCanRename  = (BYTE)g_pGameCenter->m_RoleManager.IsCanRename(dwRoleID);

        memcpy(pRenameRespond->byData, pRoleListInfo, sizeof(KROLE_LIST_INFO));
    }

    bRetCode = Send(piBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piBuffer);
    return bResult;
}

BOOL KGatewayAgency::Send(IMemBlock* piBuffer)
{
    BOOL bResult  = false;
    int  nRetCode = false;

    assert(piBuffer);

    LOG_PROCESS_ERROR(m_piSocket);

    nRetCode = m_piSocket->Send(piBuffer);
    LOG_PROCESS_ERROR(nRetCode == 1);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KGatewayAgency::OnPingSignal(BYTE* pbyData, size_t uDataLen)
{
    BOOL                bResult         = false;
    BOOL                bRetCode        = false;
    G2R_PING_SIGNAL*    pPing           = (G2R_PING_SIGNAL*)pbyData;
    IMemBlock*         piSendBuffer    = NULL;
    R2G_PING_SIGNAL*    pPingRespond    = NULL;

    piSendBuffer = QCreateMemBlock((unsigned)sizeof(R2G_PING_SIGNAL));
    LOG_PROCESS_ERROR(piSendBuffer);

    pPingRespond = (R2G_PING_SIGNAL*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pPingRespond);

    pPingRespond->byProtocol    = r2g_ping_signal;
    pPingRespond->dwTime        = pPing->dwTime;

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGatewayAgency::OnQueryAccountState(BYTE* pbyData, size_t uDataLen)
{
    BOOL                         bResult             = false;
    BOOL                         bRetCode            = false;
    IMemBlock*                  piSendBuffer        = NULL;
    KRole*                       pOnlineRole         = NULL;
    G2R_QUERY_ACCOUNT_STATE*     pQueryAccountState  = (G2R_QUERY_ACCOUNT_STATE*)pbyData;
    R2G_SYNC_ACCOUNT_STATE*      pSyncAccountState   = NULL;

    pQueryAccountState->szAccount[sizeof(pQueryAccountState->szAccount) - 1] = '\0';

    pOnlineRole = g_pGameCenter->m_RoleManager.GetOnlineRoleForAccount(pQueryAccountState->szAccount);

    piSendBuffer = QCreateMemBlock(sizeof(R2G_SYNC_ACCOUNT_STATE));
    LOG_PROCESS_ERROR(piSendBuffer);

    pSyncAccountState = (R2G_SYNC_ACCOUNT_STATE*)piSendBuffer->GetData();
    LOG_PROCESS_ERROR(pSyncAccountState);

    pSyncAccountState->byProtocol   = r2g_sync_account_state;
    pSyncAccountState->nPlayerIndex = pQueryAccountState->nPlayerIndex;
    pSyncAccountState->bOnline      = (pOnlineRole != NULL);

    bRetCode = Send(piSendBuffer);
    LOG_PROCESS_ERROR(bRetCode);

    bResult = true;
EXIT0:
    SAFE_RELEASE(piSendBuffer);
    return bResult;
}

BOOL KGatewayAgency::OnQueryRoleList(BYTE* pbyData, size_t uDataLen)
{
    BOOL                   bResult         = false;
    G2R_QUERY_ROLE_LIST*   pQuery          = (G2R_QUERY_ROLE_LIST*)pbyData;
    KAccount*              pAccount        = NULL;
    int                    nRoleCount      = 0;

    pQuery->szAccount[sizeof(pQuery->szAccount) - 1] = '\0';

    pAccount = g_pGameCenter->m_RoleManager.GetAccount(pQuery->szAccount);
    if (pAccount == NULL)
    {
        DoSyncRoleList(pQuery->nPlayerIndex, 0, 0, 0, NULL);

		// 随机名字 尚未有角色，那么传递协议，随机中文名, 随机性别
		INT nSex = g_Random(2); // 0 or 1 
		CHAR randomName[_NAME_LEN] = "";
		g_pGameCenter->m_QRandomName.GetRandomName(randomName, nSex);
		g_pGameCenter->m_Gateway.DoSyncRandomName(pQuery->nPlayerIndex, randomName, nSex);
        goto EXIT0;
    }

    nRoleCount = (int)pAccount->vRoleList.size();
    if (nRoleCount == 0)
    {
        DoSyncRoleList(pQuery->nPlayerIndex, 0, 0, 0, NULL);
		INT nSex = g_Random(2);
		CHAR randomName[_NAME_LEN] = "";
		g_pGameCenter->m_QRandomName.GetRandomName(randomName, nSex);
		g_pGameCenter->m_Gateway.DoSyncRandomName(pQuery->nPlayerIndex, randomName, nSex);
        goto EXIT0;
    }

    for (int i = 0; i < nRoleCount; i++)
    {
        KRole* pRole = pAccount->vRoleList[i];

        assert(pRole);

        g_pGameCenter->m_RoleDB.DoQueryRoleBaseData(
            pRole->m_dwPlayerID, pQuery->nPlayerIndex, i + 1, nRoleCount
        );
    }

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KGatewayAgency::OnQueryNewbieMaps(BYTE* pbyData, size_t uDataLen)
{
    G2R_QUERY_NEWBIE_MAPS*  pRequest        = (G2R_QUERY_NEWBIE_MAPS*)pbyData;

    DoSyncNewbieMaps(pRequest->nPlayerIndex);

    return true;
}

BOOL KGatewayAgency::OnKickAccountNotify(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult         = false;
    G2R_KICK_ACCOUNT_NOTIFY*    pNotify         = (G2R_KICK_ACCOUNT_NOTIFY*)pbyData;
    KRole*                      pOnlineRole     = NULL;

    pNotify->szAccount[sizeof(pNotify->szAccount) - 1] = '\0';

    pOnlineRole = g_pGameCenter->m_RoleManager.GetOnlineRoleForAccount(pNotify->szAccount);
    LOG_PROCESS_ERROR(pOnlineRole);

    g_pGameCenter->m_GameServer.DoKickAccountNotify(pOnlineRole->m_dwPlayerID);

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KGatewayAgency::OnRandomNameRequest(BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bResult         = false;
	G2R_RANDOM_NAME_REQUEST*    pSync         = (G2R_RANDOM_NAME_REQUEST*)pbyData;

	CHAR randomName[_NAME_LEN] = "";
	g_pGameCenter->m_QRandomName.GetRandomName(randomName, pSync->nSex);
	g_pGameCenter->m_Gateway.DoSyncRandomName(pSync->nPlayerIndex, randomName, pSync->nSex);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KGatewayAgency::OnCreateRoleRequest(BYTE* pbyData, size_t uDataLen)
{
    BOOL                                bResult             = false;
    int                                 nRetCode            = 0;
    G2R_CREATE_ROLE_REQUEST*            pRequest            = (G2R_CREATE_ROLE_REQUEST*)pbyData;
    KCREATE_ROLE_PARAM*                 pCreateRoleParam    = NULL;
    KMapInfo*                           pMap                = NULL;
    int                                 nMapCopyIndex       = 0;
    const KMapCopy*                     pcMapCopy           = NULL;
    KRole*                              pRole               = NULL;
    KDB_ROLE_GENERATOR_PARAM            RoleCreateParam;
    QBornPointInfo                      BornPointInfo;
    int                                 nRoleNameLen;

    pRequest->szAccount[sizeof(pRequest->szAccount) - 1] = '\0';

    LOG_PROCESS_ERROR(uDataLen == sizeof(G2R_CREATE_ROLE_REQUEST) + sizeof(KCREATE_ROLE_PARAM));

    pCreateRoleParam = (KCREATE_ROLE_PARAM*)pRequest->byData;

    pCreateRoleParam->szRoleName[sizeof(pCreateRoleParam->szRoleName) - 1] = '\0';

    nRoleNameLen = (int)strlen(pCreateRoleParam->szRoleName);
    if (nRoleNameLen > g_pGameCenter->m_Settings.m_nMaxRoleNameLength)
    {
        g_pGameCenter->m_Gateway.DoCreateRoleRespond(
            eCreateRoleNameTooLong, pRequest->nPlayerIndex, NULL
        );
        goto EXIT1;
    }

    if (nRoleNameLen < g_pGameCenter->m_Settings.m_nMinRoleNameLength)
    {
        g_pGameCenter->m_Gateway.DoCreateRoleRespond(
            eCreateRoleNameTooShort, pRequest->nPlayerIndex, NULL
        );
        goto EXIT1;
    }

//    nRetCode = g_pBadwordFilter->HasBadword(pCreateRoleParam->szRoleName);
//    if (nRetCode)
//    {
//        g_pGameCenter->m_Gateway.DoCreateRoleRespond(
//            eCreateRoleInvalidRoleName, pRequest->nPlayerIndex, NULL
//        );
//        goto EXIT1;
//    }
//by ldy;
//	nRetCode = KSysService::ValidateStringGBK(pCreateRoleParam->szRoleName);
//	if (!nRetCode)
//	{
//		g_pGameCenter->m_Gateway.DoCreateRoleRespond(
//			eCreateRoleInvalidRoleName, pRequest->nPlayerIndex, NULL
//			);
//		goto EXIT1;
//	}
    
// 名字重复
//    pRole = g_pGameCenter->m_RoleManager.GetRole(pCreateRoleParam->szRoleName);
//   if (pRole != NULL)
//    {
//        g_pGameCenter->m_Gateway.DoCreateRoleRespond(
//            eCreateRoleNameAlreadyExist, pRequest->nPlayerIndex, NULL
//        );
//        goto EXIT1;
//   }

    // 检查Role type
    LOG_PROCESS_ERROR(/*pCreateRoleParam->cRoleType > rtInvalid &&*/ pCreateRoleParam->cRoleType < emROLETYPE_TOTAL);

    // 检查MapID是不是新手村
    pMap = g_pGameCenter->m_MapManager.GetMapInfo(pCreateRoleParam->dwMapID);
    LOG_PROCESS_ERROR(pMap && pMap->m_nType == emtBirthMap);

    nMapCopyIndex = pCreateRoleParam->nMapCopyIndex;

    // 副本ID为0表示服务器自动选择副本
    if (nMapCopyIndex == 0)
    {
        typedef std::vector<int> KMapCopyTable;
        int             nPlayerCount = -1;
        KMapCopyTable   vMapCopys;

        for (int nIndex = 1; nIndex < pMap->m_nSNGenerator; nIndex++)
        {
            pcMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pCreateRoleParam->dwMapID, nIndex);

            if (pcMapCopy == NULL || pcMapCopy->m_eState != eMapStateNormal)
            {
                continue;
            }

            vMapCopys.push_back(nIndex);

            // 选一个不需要预排队的 人数最大的 新手村
            if (pcMapCopy->m_nPlayerCount > nPlayerCount && pcMapCopy->m_nPlayerCount < pMap->m_nPreQueuePlayerCount)
            {
                nMapCopyIndex     = nIndex;
                nPlayerCount      = pcMapCopy->m_nPlayerCount;
            }
        }

        // 如果所有新手村都需要排队, 则随机挑选一个
        if (nPlayerCount == -1 && vMapCopys.size() > 0)
        {
			int nIndex = (int)(KSysService::Rand() % vMapCopys.size());

            nMapCopyIndex = vMapCopys[nIndex];
        }
    }

    pcMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(pCreateRoleParam->dwMapID, nMapCopyIndex);
    if (pcMapCopy == NULL || pcMapCopy->m_eState != eMapStateNormal)
    {
        // 找不到合适的新手村副本,不能创建角色
        DoCreateRoleRespond(eCreateRoleUnableToCreate, pRequest->nPlayerIndex, NULL);
		QLogPrintf(LOG_ERR,"..eCreateRoleUnableToCreate pRequest->nPlayerIndex=%d pCreateRoleParam->dwMapID=%d nMapCopyIndex=%d",pRequest->nPlayerIndex,pCreateRoleParam->dwMapID,nMapCopyIndex);
        goto EXIT1;
    }

    // 获取对应的出身点坐标
    nRetCode = g_pGameCenter->m_BornPlaceList.GetBornPointInfo(
        &BornPointInfo, pCreateRoleParam->dwMapID, pCreateRoleParam->cRoleType
    );
    LOG_PROCESS_ERROR(nRetCode);

    RoleCreateParam.nX             = BornPointInfo.nPosX;
    RoleCreateParam.nY             = BornPointInfo.nPosY;
    RoleCreateParam.nZ             = BornPointInfo.nPosZ;
    RoleCreateParam.byDirection    = (BYTE)BornPointInfo.nDirection;
    RoleCreateParam.UserParam      = *pCreateRoleParam;

    RoleCreateParam.UserParam.nMapCopyIndex  = nMapCopyIndex;

    strncpy(RoleCreateParam.szAccountName, pRequest->szAccount, sizeof(RoleCreateParam.szAccountName));
    RoleCreateParam.szAccountName[sizeof(RoleCreateParam.szAccountName) - 1] = '\0';

    g_pGameCenter->m_RoleDB.DoCreateRole(&RoleCreateParam, pRequest->nPlayerIndex);

	
EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KGatewayAgency::OnDeleteRoleRequest(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult             = false;
    BOOL                        bRetCode            = false;
    int                         nRetCode            = 0;
    KRole*                      pRole               = NULL;
    KTong*                      pTong               = NULL; 
    G2R_DELETE_ROLE_REQUEST*    pDeleteRoleRequest  = (G2R_DELETE_ROLE_REQUEST*)pbyData;
    time_t                      nEndTime            = 0;
    time_t                      nFreezeTime         = 0;
    int                         nCode               = eDeleteRoleUnknownError;

    pRole = g_pGameCenter->m_RoleManager.GetRole(pDeleteRoleRequest->dwRoleID);
    LOG_PROCESS_ERROR(pRole);

    pTong = g_pGameCenter->m_TongManager.GetTong(pRole->m_dwTongID);
    if (pTong && pTong->m_dwMaster == pRole->m_dwPlayerID)
    {
        nCode = eDeleteRoleTongMaster;
        goto EXIT0;
    }

    nFreezeTime = g_pGameCenter->m_RoleManager.GetRoleFreezeTime(pRole->m_dwPlayerID);
    if (nFreezeTime > g_pGameCenter->m_nTimeNow)
    {
        nCode = eDeleteRoleFreezeRole;
        goto EXIT0;
    }

    QLogPrintf(LOG_DEBUG, "Apply delete role %s.", pRole->m_szName);

    if (pRole->m_byLevel < g_pGameCenter->m_Settings.m_nDeleteProtectLevel)
    {
        bRetCode = g_pGameCenter->m_RoleManager.DelRole(pDeleteRoleRequest->dwRoleID);
        PROCESS_ERROR(bRetCode);

        nCode = eDeleteRoleSucceed;
    }
    else
    {
        nEndTime = g_pGameCenter->m_nTimeNow + g_pGameCenter->m_Settings.m_nDeleteDelayTime;

        g_pGameCenter->m_RoleManager.RegisterDelRole(pDeleteRoleRequest->dwRoleID, nEndTime);

        g_pGameCenter->m_MiscDB.DoUpdateRoleDeleteList(pDeleteRoleRequest->dwRoleID, nEndTime);

        nCode = eDeleteRoleDelay;
    }

    bResult = true;
EXIT0:
    DoDeleteRoleRespond(pDeleteRoleRequest->nPlayerIndex, pDeleteRoleRequest->dwRoleID, nCode, nEndTime);
    return bResult;
}

BOOL KGatewayAgency::OnGameLoginRequest(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult         = false;
    BOOL                        bRetCode        = false;
    G2R_GAME_LOGIN_REQUEST*     pRequest        = (G2R_GAME_LOGIN_REQUEST*)pbyData;
    time_t                      nFreezeTime     = 0;
    int                         nNameLen        = 0;
    KRole*                      pRole           = NULL;
    KROLE_LIST_INFO*            pRoleListInfo   = NULL;
    KACCOUNT_LOGIN_INFO         AccInfo;

    if (m_bQuiting)
    {
        g_pGameCenter->m_Gateway.DoGameLoginRespond(
            pRequest->nPlayerIndex, eGameLoginSystemMaintenance, pRequest->dwRoleID, INVALID_QUEUE_ID, 0, 0, GUID()
        );
        goto EXIT0;
    }

    nFreezeTime = g_pGameCenter->m_RoleManager.GetRoleFreezeTime(pRequest->dwRoleID);
    if (nFreezeTime > g_pGameCenter->m_nTimeNow)
    {
        g_pGameCenter->m_Gateway.DoGameLoginRespond(
            pRequest->nPlayerIndex, eGameLoginRoleFreeze, pRequest->dwRoleID, INVALID_QUEUE_ID, 0, 0, GUID()
        );
        goto EXIT0;
    }

    g_pGameCenter->m_RoleManager.UnregisterDelRole(pRequest->dwRoleID);

    pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRoleID);
    LOG_PROCESS_ERROR(pRole);

    LOG_PROCESS_ERROR(pRole->GetRoleState() == rsOffline);

    AccInfo.nChargeFlag             = pRequest->nChargeFlag;
    AccInfo.ExtPointInfo            = pRequest->ExtPointInfo;
    AccInfo.dwEndTimeOfFee          = pRequest->dwEndTimeOfFee;
    AccInfo.dwCoin                  = pRequest->dwCoin;
    AccInfo.dwLimitPlayTimeFlag     = pRequest->dwLimitPlayTimeFlag;
    AccInfo.dwLimitOnlineSecond     = pRequest->dwLimitOnlineSecond;
    AccInfo.dwLimitOfflineSecond    = pRequest->dwLimitOfflineSecond;
    AccInfo.nIdentity               = pRequest->nPlayerIndex;
    
    pRoleListInfo = (KROLE_LIST_INFO*)pRequest->byData;
    
    assert(pRoleListInfo);

    g_pGameCenter->m_GameServer.ProcessRoleLogin(
        &AccInfo, pRoleListInfo->dwPlayerID, &pRoleListInfo->BaseInfo, pRequest->dwQueueID
    );

	//ldy:统计登录
	g_pGameCenter->m_LogServerAgency.LogAction(PLAYER_ACTION_LOGIN,pRole,0,"LogRoleLogin");


    bResult = true;
EXIT0:
    return bResult;
}

BOOL KGatewayAgency::OnChangeExtPointRespond(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult     = false;
//    G2R_CHANGE_EXT_POINT_RESPOND*   pRespond    = (G2R_CHANGE_EXT_POINT_RESPOND*)pbyData;
//    KRole*                          pRole       = NULL;
//
//    pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwRoleID);
//    LOG_PROCESS_ERROR(pRole);
//
//    QLogPrintf(
//        LOG_INFO, "Change ExtPoint %s: Acc = %s, Role = %s, ID = %u",
//        pRespond->bSucceed ? "succeed" : "failed", 
//        pRole->m_pszAccount, pRole->m_szName, pRole->m_dwPlayerID
//    );
//
//    g_pGameCenter->m_GameServer.DoChangeExtPointRespond(
//        pRole->m_nConnIndex, pRole->m_dwPlayerID, pRespond->bSucceed
//    );
//
//    bResult = true;
//EXIT0:
    return bResult;
}

BOOL KGatewayAgency::OnSetChargeFlagRespond(BYTE* pbyData, size_t uDataLen)
{
    BOOL                            bResult     = false;
//    G2R_SET_CHARGE_FLAG_RESPOND*    pRespond    = (G2R_SET_CHARGE_FLAG_RESPOND*)pbyData;
//    KRole*                          pRole       = NULL;
//
//    pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwRoleID);
//    LOG_PROCESS_ERROR(pRole);
//
//    if (!pRespond->bSucceed)
//    {
//        QLogPrintf(
//            LOG_ERR, "Set ChargeFlag failed! Acc = %s, Role = %s",
//            pRole->m_pszAccount, pRole->m_szName
//        );
//        goto EXIT0;
//    }
//
//    g_pGameCenter->m_GameServer.DoSetChargeFlagRespond(
//        pRole->m_nConnIndex, pRole->m_dwPlayerID, pRespond->nChargeFlag, pRespond->ExtPointInfo,
//        pRespond->dwEndTimeOfFee
//    );
//
//    bResult = true;
//EXIT0:
    return bResult;
}

BOOL KGatewayAgency::OnGetZoneChargeFlagNotify(BYTE* pbyData, size_t uDataLen)
{
    //G2R_GET_ZONE_CHARGE_FLAG_NOTIFY*    pNotify = (G2R_GET_ZONE_CHARGE_FLAG_NOTIFY*)pbyData;

    //g_pGameCenter->m_nZoneChargeFlag = pNotify->nZoneChargeFlag;

    //g_pGameCenter->m_GameServer.DoSyncZoneChargeFlag(-1, pNotify->nZoneChargeFlag);

    //if (pNotify->nZoneChargeFlag && g_pGameCenter->m_GlobalSystemValueManager.m_GameWorldChargeTime.nValue == 0)
    //{
    //    g_pGameCenter->m_GlobalSystemValueManager.m_GameWorldChargeTime.nValue = (int)time(NULL);
    //    g_pGameCenter->m_GlobalSystemValueManager.m_GameWorldChargeTime.bUpdate = true;
    //    g_pGameCenter->m_GlobalSystemValueManager.m_bNeedSyncToServer = true;
    //}

    return true;
}

BOOL KGatewayAgency::OnActivePresentCodeRespond(BYTE* pbyData, size_t uDataLen)
{
    BOOL                                bResult     = false;
//    G2R_ACTIVE_PRESENT_CODE_RESPOND*    pRespond    = (G2R_ACTIVE_PRESENT_CODE_RESPOND*)pbyData;
//    KRole*                              pRole       = NULL;
//
//    pRespond->szPresentCode[sizeof(pRespond->szPresentCode) - 1] = '\0';
//
//    pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwRoleID);
//    LOG_PROCESS_ERROR(pRole);
//
//    QLogPrintf(
//        LOG_INFO, "Active PresentCode %s: Acc = %s, Role = %s, ID = %u, PresentCode = %s",
//        pRespond->bSucceed ? "succeed" : "failed", 
//        pRole->m_pszAccount, pRole->m_szName, pRole->m_dwPlayerID, pRespond->szPresentCode
//    );
//
//    g_pGameCenter->m_GameServer.DoActivePresentCodeRespond(
//        pRole->m_nConnIndex, pRole->m_dwPlayerID, pRespond->bSucceed,
//        pRespond->szPresentCode, pRespond->dwPresentType
//    );
//
//    bResult = true;
//EXIT0:
    return bResult;
}

BOOL KGatewayAgency::OnFreezeCoinRespond(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult     = false;
    G2R_FREEZE_COIN_RESPOND*    pRespond    = (G2R_FREEZE_COIN_RESPOND*)pbyData;

    //if (pRespond->bFreezeFlag)
    //{
    //    g_pGameCenter->m_GameCardManager.OnPaysysFreezeCoinRespond(pRespond->bSucceed, pRespond->dwRequestID);
    //}
    //else
    //{
    //    KRole* pRole = NULL;

    //    pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwRequestID);
    //    LOG_PROCESS_ERROR(pRole);

    //    g_pGameCenter->m_LogServerAgency.LogUnfreezeCoin(pRole, pRespond->nCoin, pRespond->bSucceed ? "succeed" : "failed");

    //    g_pGameCenter->m_GameServer.DoGameCardCancelRespond(pRespond->dwRequestID, (BYTE)(pRespond->bSucceed ? gorSucceed : gorUnFreezeCoinFailed));
    //}

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KGatewayAgency::OnAccountExchangeRespond(BYTE* pbyData, size_t uDataLen)
{
    G2R_ACCOUNT_EXCHANGE_RESPOND* pRespond = (G2R_ACCOUNT_EXCHANGE_RESPOND*)pbyData;

    //g_pGameCenter->m_GameCardManager.OnPaysysAccountExchangeRespond(pRespond->bSucceed, pRespond->dwRequestID);

    return true;
}

BOOL KGatewayAgency::OnRenameRequest(BYTE* pbyData, size_t uDataLen)
{
    BOOL                    bResult         = false;
    BOOL                    bRetCode        = false;
    G2R_RENAME_REQUEST*     pRenameRequest  = (G2R_RENAME_REQUEST*)pbyData;
    KRole*                  pRole           = NULL;
    size_t                  uParamDataLen   = 0;
    int                     nNewNameLen     = 0;

    bRetCode = g_pGameCenter->m_RoleManager.IsCanRename(pRenameRequest->dwRoleID);
    PROCESS_ERROR(bRetCode);

    pRenameRequest->szNewName[sizeof(pRenameRequest->szNewName) - 1] = '\0';

    nNewNameLen = (int)strlen(pRenameRequest->szNewName);
    if (nNewNameLen > g_pGameCenter->m_Settings.m_nMaxRoleNameLength)
    {
        DoRenameRespond(
            eRenameNewNameTooLong, pRenameRequest->nPlayerIndex,
            pRenameRequest->dwRoleID, NULL
        );
        goto EXIT0;
    }

    if (nNewNameLen < g_pGameCenter->m_Settings.m_nMinRoleNameLength)
    {
        DoRenameRespond(
            eRenameNewNameTooShort, pRenameRequest->nPlayerIndex,
            pRenameRequest->dwRoleID, NULL
        );
        goto EXIT0;
    }

    //bRetCode = g_pGameCenter->m_piTextFilter->Check(pRenameRequest->szNewName); // TODO
    //if (!bRetCode)
    //{
    //    DoRenameRespond(
    //        eRenameNewNameError, pRenameRequest->nPlayerIndex,
    //        pRenameRequest->dwRoleID, NULL
    //    );
    //    goto EXIT0;
    //}

    pRole = g_pGameCenter->m_RoleManager.GetRole(pRenameRequest->szNewName);
    if (pRole)
    {
        DoRenameRespond(
            eRenameNameAlreadyExist, pRenameRequest->nPlayerIndex, 
            pRenameRequest->dwRoleID, NULL
        );
        goto EXIT0;
    }

    pRole = g_pGameCenter->m_RoleManager.GetRole(pRenameRequest->dwRoleID);
    if (!pRole)
    {
        DoRenameRespond(
            eRenameUnknownError, pRenameRequest->nPlayerIndex, 
            pRenameRequest->dwRoleID, NULL
        );
        goto EXIT0;
    }

    LOG_PROCESS_ERROR(pRole->GetRoleState() == rsOffline);

    g_pGameCenter->m_RoleDB.DoRename(
        pRenameRequest->dwRoleID, pRenameRequest->nPlayerIndex, pRenameRequest->szNewName
    );

    bResult = true;
EXIT0:
    return bResult;
}
