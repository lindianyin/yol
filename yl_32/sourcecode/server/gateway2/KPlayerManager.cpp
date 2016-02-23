#include "stdafx.h"
#include "Gateway.h"
#include "Protocol/AccountLoginDef.h"
#include "protocol/GatewayClientProtocol.h"
#include "protocol/GatewayDef.h"
#include "kldapclient.h"

using namespace std;

#define REGISTER_C2G_PROTOCOL(Protocol, Func, DataStruct)       \
	do                                                          \
{                                                           \
	m_PakProcessor[Protocol]    = &KPlayerManager::Func;    \
	m_uPakSize[Protocol]        = sizeof(DataStruct);       \
} while (false)

#define ACCOUNT_WHITE_LIST_FILE "whitelist.txt"
#define MAX_WAIT_ACCEPT     2048
#define MAX_SOCKET_EVENT    (1024 + MAX_WAIT_ACCEPT)
#define	SOCKET_SEND_BUFFER_SIZE	2 * 1024
#define	SOCKET_RECV_BUFFER_SIZE	1 * 1024

KPlayerManager::KPlayerManager()
{
	m_pPaysysAgency         = NULL;
	m_pRelayAgency          = NULL;
	m_pSockerServer         = NULL;
	m_pSocketEvents         = NULL;
	m_pGateway              = NULL;
	m_pSndaAgency           = NULL;
	m_nNextPlayerIndex      = 0;
	m_bRecycleConnections   = false;
	m_nTimeNow              = 0;
	m_nNextPingCheck        = 0;
	m_nMaxPlayer            = 0;
	m_nPingCycle            = 30;
	m_eAccountVerifier = emACCOUNT_VERIFIER_PAYSYS;
	m_nTotalConnections = 0;

	memset(m_PakProcessor, 0, sizeof(m_PakProcessor));
	memset(m_uPakSize, 0, sizeof(m_uPakSize));

	REGISTER_C2G_PROTOCOL(c2g_ping_signal, OnPingSignal, C2G_PING_SIGNAL);
	REGISTER_C2G_PROTOCOL(c2g_handshake_request, OnHandshakeRequest, C2G_HANDSHAKE_REQUEST);
	REGISTER_C2G_PROTOCOL(c2g_account_verify_request, OnAccountVerifyRequest, C2G_ACCOUNT_VERIFY_REQUEST);
	REGISTER_C2G_PROTOCOL(c2g_snda_token_verify_request, OnSndaTokenVerifyRequest, C2G_SNDA_TOKEN_VERIFY_REQUEST);
	REGISTER_C2G_PROTOCOL(c2g_mibao_verify_request, OnMibaoVerifyRequest, C2G_MIBAO_VERIFY_REQUEST);
	REGISTER_C2G_PROTOCOL(c2g_query_newbie_maps, OnQueryNewbieMaps, C2G_QUERY_NEWBIE_MAPS);
	REGISTER_C2G_PROTOCOL(c2g_create_role_request, OnCreateRoleRequest, C2G_CREATE_ROLE_REQUEST);
	REGISTER_C2G_PROTOCOL(c2g_random_playername_request, OnRandomPlayernameRequest, C2G_RANDOM_PLAYERNAME_REQUEST);
	REGISTER_C2G_PROTOCOL(c2g_delete_role_request, OnDeleteRoleRequest, C2G_DELETE_ROLE_REQUEST);
	REGISTER_C2G_PROTOCOL(c2g_game_login_request, OnGameLoginRequest, C2G_GAME_LOGIN_REQUEST);
	REGISTER_C2G_PROTOCOL(c2g_giveup_queue_request, OnGiveupQueueRequest, C2G_GIVEUP_QUEUE_REQUEST);
	REGISTER_C2G_PROTOCOL(c2g_rename_request, OnRenameRequest, C2G_RENAME_REQUEST);
}

BOOL KPlayerManager::Init(KE_ACCOUNT_VERIFIER eAccountVerifier)
{
	BOOL                        bResult                = false;
	BOOL                        bRetCode               = false;
	BOOL                        nSocketServerInitFlag  = false;
	IIniFile*                   piIniFile              = NULL;
	int                         nPort                  = 0;
	char                        szIP[_NAME_LEN];

	m_pSocketEvents = new QSOCKET_EVENT[MAX_SOCKET_EVENT];
	LOG_PROCESS_ERROR(m_pSocketEvents);

	m_pSockerServer = new QSocketServerAcceptor();
	LOG_PROCESS_ERROR(m_pSockerServer);

	piIniFile = g_OpenIniFile(GATEWAY_CONFIG_FILE);
	LOG_PROCESS_ERROR(piIniFile);

	bRetCode = piIniFile->GetInteger("Player", "MaxPlayer", 8192, &m_nMaxPlayer);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piIniFile->GetString("Player", "IP", "", szIP, (unsigned int)sizeof(szIP));
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piIniFile->GetInteger("Player", "Port", 0, &nPort);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = piIniFile->GetInteger("Player", "PingCycle", 20, &m_nPingCycle);
	LOG_PROCESS_ERROR(bRetCode);

	m_eAccountVerifier = eAccountVerifier;
	if (m_eAccountVerifier & emACCOUNT_VERIFIER_WHITE_LIST)
	{
		bRetCode = _LoadWhiteList();
		LOG_PROCESS_ERROR(bRetCode);
	}

	bRetCode = m_pSockerServer->Init(
		szIP, nPort, MAX_WAIT_ACCEPT, 
		SOCKET_RECV_BUFFER_SIZE, SOCKET_SEND_BUFFER_SIZE, ENCODE_DECODE_NONE/*ENCODE_DECODE*/, NULL
		);
	LOG_PROCESS_ERROR(bRetCode);
	nSocketServerInitFlag = true;

	bResult = true;
EXIT0:

	QLogPrintf(LOG_INFO, "Start service at %s:%d ... ... [%s]\n", szIP, nPort, bResult ? "OK" : "Failed");

	if (!bResult)
	{
		if (nSocketServerInitFlag)
		{
			m_pSockerServer->UnInit(NULL);
			nSocketServerInitFlag = false;
		}
		SAFE_DELETE(m_pSockerServer);
		SAFE_DELETE_ARRAY(m_pSocketEvents);
	}
	SAFE_RELEASE(piIniFile);
	return bResult;
}


void KPlayerManager::UnInit()
{
	for (KPlayerTable::iterator it = m_PlayerTable.begin(); it != m_PlayerTable.end(); ++it)
	{
		KPlayerAgency* pPlayer = &it->second;

		SAFE_RELEASE(pPlayer->piSocket);

		for (
			KROLE_LIST::iterator RoleIter = pPlayer->RoleList.begin(); 
			RoleIter != pPlayer->RoleList.end(); ++RoleIter
			)
		{
			SAFE_RELEASE(RoleIter->second);
		}
	}

	m_pSockerServer->UnInit(NULL);

	SAFE_DELETE(m_pSockerServer);
	SAFE_DELETE_ARRAY(m_pSocketEvents);
}

void KPlayerManager::Activate()
{
	int nRetCode = 0;

	m_nTimeNow = time(NULL);

	while (true)
	{
		int                 nEventCount     = 0;
		QSOCKET_EVENT*    pSocketEvent    = NULL;
		QSOCKET_EVENT*    pSocketEventEnd = NULL;

		nRetCode = m_pSockerServer->Wait(MAX_SOCKET_EVENT, m_pSocketEvents, &nEventCount);
		LOG_PROCESS_ERROR(nRetCode);

		if (nEventCount == 0)
			break;

		pSocketEventEnd = m_pSocketEvents + nEventCount;
		for (pSocketEvent = m_pSocketEvents; pSocketEvent < pSocketEventEnd; pSocketEvent++)
		{
			if (pSocketEvent->uEventFlag & QSOCKET_EVENT_ACCEPT)
			{
				ProcessNewConnection(pSocketEvent->piSocket);
				SAFE_RELEASE(pSocketEvent->piSocket);
				continue;
			}

			if (!(pSocketEvent->uEventFlag & QSOCKET_EVENT_IN))
			{
				QLogPrintf(LOG_DEBUG, "Unexpected socket event: %u", pSocketEvent->uEventFlag);
				SAFE_RELEASE(pSocketEvent->piSocket);
				continue;
			}

			ProcessPackage(pSocketEvent->piSocket);

			SAFE_RELEASE(pSocketEvent->piSocket);
		}
	}

	CheckConnectionTimeout();

	RecycleConnections();

EXIT0:
	return;
}

void KPlayerManager::UnlockAccount(KPlayerAgency* pPlayer)
{
	m_LockedAccountTable.erase(pPlayer->szAccount);
}

void KPlayerManager::KickoutAccount(const char* pszAccountName)
{
	KLockedAccountTable::iterator it;
	KPlayerAgency*  pPlayer = NULL;

	assert(pszAccountName);

	it = m_LockedAccountTable.find(pszAccountName);
	PROCESS_ERROR(it != m_LockedAccountTable.end());

	pPlayer = GetPlayer(it->second);
	PROCESS_ERROR(pPlayer);

	QLogPrintf(LOG_INFO, "Account kickout: %d(%s, %d)\n", pPlayer->nIndex, pPlayer->szAccount, pPlayer->nState);

	Disconnect(pPlayer);

EXIT0:
	return;
}

BOOL KPlayerManager::ProcessNewConnection(ISocketStream* piSocket)
{
	BOOL            bResult         = false;
	int             nRetCode        = 0;
	u_short         uRemotePort     = 0;
	const char*     pcszIP          = NULL;
	KPlayerAgency*  pPlayer         = NULL;
	struct in_addr  RemoteIP;
	std::pair<KPlayerTable::iterator, BOOL> InsRet;

	assert(piSocket);

	m_nTotalConnections++;
	PROCESS_ERROR(m_PlayerTable.size() < (size_t)m_nMaxPlayer);

	nRetCode = piSocket->GetRemoteAddress(&RemoteIP, &uRemotePort);
	LOG_PROCESS_ERROR(nRetCode);

	pcszIP = inet_ntoa(RemoteIP);
	LOG_PROCESS_ERROR(pcszIP);

	for (UINT i = 0; i < UINT_MAX; ++i)
	{
		KPlayerTable::iterator it = m_PlayerTable.find(m_nNextPlayerIndex);
		if (it == m_PlayerTable.end())
		{
			break;
		}
		m_nNextPlayerIndex++;
	}

	InsRet = m_PlayerTable.insert(std::make_pair(m_nNextPlayerIndex, KPlayerAgency()));
	LOG_PROCESS_ERROR(InsRet.second);

	pPlayer = &InsRet.first->second;

	pPlayer->nIndex             = m_nNextPlayerIndex;
	pPlayer->dwQueueID          = INVALID_QUEUE_ID;
	pPlayer->piSocket           = piSocket;
	pPlayer->nState             = easWaitForHandshake;
	pPlayer->nLastPingTime      = m_nTimeNow;

	pPlayer->szAccount[0]       = '\0';
	pPlayer->szPsw[0]           = '\0';
	pPlayer->szAct[0]           = '\0';
	pPlayer->szSndaToken[0]     = '\0';
	pPlayer->szIDCard[0]        = '\0';
	pPlayer->nServerId = 0;
	pPlayer->nZoneId = 0;

	pPlayer->piSocket->AddRef();

	nRetCode = piSocket->SetUserData((void*)(ptrdiff_t)m_nNextPlayerIndex);
	LOG_PROCESS_ERROR(nRetCode);

	QLogPrintf(LOG_INFO, "New connection from %s:%u, index = %d\n", pcszIP, uRemotePort, m_nNextPlayerIndex);

	m_nNextPlayerIndex++;

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KPlayerManager::ProcessPackage(ISocketStream* piSocket)
{
	BOOL                    bResult         = false;
	int                     nRetCode        = 0;
	int                     nPlayerIndex    = 0;
	KPlayerAgency*          pPlayer         = NULL;
	IMemBlock*              piBuffer        = NULL;
	BYTE*                   pbyData         = NULL;
	size_t                  uDataLen        = 0;
	KGC_PROTOCOL_HEADER*    pHeader         = NULL;
	PC2G_PROTOCOL_FUNC      ProtocolFunc    = NULL;

	assert(piSocket);

	nPlayerIndex = (int)(ptrdiff_t)(piSocket->GetUserData());

	pPlayer = GetPlayer(nPlayerIndex);
	LOG_PROCESS_ERROR(pPlayer);

	while (true)
	{
		SAFE_RELEASE(piBuffer);

		nRetCode = piSocket->Recv(&piBuffer);
		if (nRetCode == -2)
		{
			break;
		}

		if (nRetCode == -1)
		{
			QLogPrintf(LOG_INFO, "Connection lost: %d(%s, %d)\n", nPlayerIndex, pPlayer->szAccount, pPlayer->nState);
			Disconnect(pPlayer);
			break;
		}

		LOG_PROCESS_ERROR(piBuffer);

		pbyData = (BYTE*)piBuffer->GetData();
		LOG_PROCESS_ERROR(pbyData);

		uDataLen = piBuffer->GetSize();
		LOG_PROCESS_ERROR(uDataLen >= sizeof(KGC_PROTOCOL_HEADER));

		pHeader = (KGC_PROTOCOL_HEADER*)pbyData;
		LOG_PROCESS_ERROR(pHeader->byProtocol > c2g_protocol_begin);
		LOG_PROCESS_ERROR(pHeader->byProtocol < c2g_protocol_total);

		ProtocolFunc = m_PakProcessor[pHeader->byProtocol];
		LOG_PROCESS_ERROR(ProtocolFunc);

		LOG_PROCESS_ERROR(uDataLen >= m_uPakSize[pHeader->byProtocol]);

		(this->*ProtocolFunc)(pPlayer, pbyData, uDataLen);
	}

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

void KPlayerManager::CheckConnectionTimeout()
{
	PROCESS_ERROR(m_nTimeNow > m_nNextPingCheck);
	m_nNextPingCheck = m_nTimeNow + 8;

	for (KPlayerTable::iterator it = m_PlayerTable.begin(); it != m_PlayerTable.end(); ++it)
	{
		KPlayerAgency* pPlayer = &it->second;

		if (m_nTimeNow > pPlayer->nLastPingTime + m_nPingCycle)
		{
			struct tm* tmLocal = localtime(&m_nTimeNow);
			QLogPrintf(LOG_INFO, "Connection timeout: %d(%s, %d) last ping %s\n",
				it->first, pPlayer->szAccount, pPlayer->nState,
				asctime(tmLocal));
			Disconnect(pPlayer);
		}
	}

EXIT0:
	return;
}

void KPlayerManager::RecycleConnections()
{
	PROCESS_ERROR(m_bRecycleConnections);
	m_bRecycleConnections = false;

	for (KPlayerTable::iterator it = m_PlayerTable.begin(); it != m_PlayerTable.end(); NULL)
	{
		KPlayerAgency* pPlayer = &(it->second);

		if (pPlayer->nState != easInvalid)
		{
			++it;
			continue;
		}

		for (
			KROLE_LIST::iterator RoleIter = pPlayer->RoleList.begin(); 
			RoleIter != pPlayer->RoleList.end(); ++RoleIter
			)
		{
			SAFE_RELEASE(RoleIter->second);
		}

		SAFE_RELEASE(pPlayer->piSocket);
		m_PlayerTable.erase(it++);
	}

EXIT0:
	return;
}

void KPlayerManager::Disconnect(KPlayerAgency* pPlayer)
{
	int             nRetCode        = 0;
	KPlayerQueue*   pPlayerQueue    = NULL;

	assert(pPlayer);

	switch (pPlayer->nState)
	{
	case easInvalid:
	case easWaitForHandshake:
	case easWaitForVerifyInfo:
		break;

	case easQueryAccountState:
	case easWaitForSDOAVerify:
		UnlockAccount(pPlayer);
		break;

	case easWaitForPaysysVerify:
		UnlockAccount(pPlayer);
		if (!pPlayer->bAccountInGame && (m_eAccountVerifier & emACCOUNT_VERIFIER_PAYSYS))
		{
			m_pPaysysAgency->DoAccountLogoutNotify(pPlayer->szAccount);
		}
		break;

	case easWaitForMibaoPassword:
	case easWaitForMibaoVerify:
	case easWaitForRoleList:
	case easWaitForPlayerOperation:
	case easWaitForNewbieMapRespond:
	case easWaitForCreateRoleRespond:
	case easWaitForDeleteRoleRespond:
	case easWaitForRenameRespond:
		UnlockAccount(pPlayer);
		if (m_eAccountVerifier & emACCOUNT_VERIFIER_PAYSYS)
			m_pPaysysAgency->DoAccountLogoutNotify(pPlayer->szAccount);
		break;

	case easWaitForLoginPermission:
		if (pPlayer->dwQueueID != INVALID_QUEUE_ID)
		{
			pPlayerQueue = m_pQueueManager->GetQueue(pPlayer->dwQueueID);
			LOG_PROCESS_ERROR(pPlayerQueue);

			nRetCode = pPlayerQueue->Dequeue(pPlayer->nIndex);
			LOG_PROCESS_ERROR(nRetCode);

			pPlayer->dwQueueID = INVALID_QUEUE_ID;
		}
		UnlockAccount(pPlayer);
		if (m_eAccountVerifier & emACCOUNT_VERIFIER_PAYSYS)
			m_pPaysysAgency->DoAccountLogoutNotify(pPlayer->szAccount);
		break;

	case easQueueWaiting:
		assert(pPlayer->dwQueueID != INVALID_QUEUE_ID);

		pPlayerQueue = m_pQueueManager->GetQueue(pPlayer->dwQueueID);
		LOG_PROCESS_ERROR(pPlayerQueue);

		nRetCode = pPlayerQueue->Dequeue(pPlayer->nIndex);
		LOG_PROCESS_ERROR(nRetCode);

		pPlayer->dwQueueID = INVALID_QUEUE_ID;

		UnlockAccount(pPlayer);
		if (m_eAccountVerifier & emACCOUNT_VERIFIER_PAYSYS)
			m_pPaysysAgency->DoAccountLogoutNotify(pPlayer->szAccount);
		break;

	case easWaitForDisconnect:
		break;

	default:
		QLogPrintf(LOG_ERR, "Unexpected player state %d\n", pPlayer->nState);
		assert(false);
		break;
	}

EXIT0:
	m_bRecycleConnections = true;
	pPlayer->nState = easInvalid;
	return;
}

BOOL KPlayerManager::DoPingRespond(KPlayerAgency* pPlayer, DWORD dwTime)
{
	BOOL                bResult     = false;
	int                 nRetCode    = 0;
	IMemBlock*         piBuffer    = NULL;
	G2C_PING_RESPOND*   pRespond    = NULL;

	piBuffer = QCreateMemBlock(sizeof(G2C_PING_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (G2C_PING_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol = g2c_ping_respond;
	pRespond->dwTime     = dwTime;

	nRetCode = pPlayer->piSocket->Send(piBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KPlayerManager::DoHandshakeRespond(KPlayerAgency* pPlayer, int nCode)
{
	BOOL                    bResult     = false;
	int                     nRetCode    = 0;
	IMemBlock*             piBuffer    = NULL;
	G2C_HANDSHAKE_RESPOND*  pRespond    = NULL;

	piBuffer = QCreateMemBlock((unsigned)sizeof(G2C_HANDSHAKE_RESPOND));
	LOG_PROCESS_ERROR(piBuffer);

	pRespond = (G2C_HANDSHAKE_RESPOND*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol        = g2c_handshake_respond;
	pRespond->byCode            = (BYTE)nCode;
	pRespond->bZoneChargeFlag   = m_pPaysysAgency->m_bZoneChargeFlag;
	pRespond->byLdapAuth = (m_eAccountVerifier & emACCOUNT_VERIFIER_LDAP);

	nRetCode = pPlayer->piSocket->Send(piBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	return bResult;
}

BOOL KPlayerManager::DoAccountLockedNotify(KPlayerAgency* pPlayer)
{
	BOOL                        bResult         = false;
	int                         nRetCode        = 0;
	G2C_ACCOUNT_LOCKED_NOTIFY*  pNotify         = NULL;
	IMemBlock*                 piSendBuffer    = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(G2C_ACCOUNT_LOCKED_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (G2C_ACCOUNT_LOCKED_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->byProtocol = g2c_account_locked_notify;
	pNotify->byNothing  = 0;

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoAccountVerifyRespond(KPlayerAgency* pPlayer, int nRespondCode)
{
	BOOL                            bResult         = false;
	int                             nRetCode        = 0;
	IMemBlock*                     piSendBuffer    = NULL;
	G2C_ACCOUNT_VERIFY_RESPOND*     pRespond        = NULL;

	assert(pPlayer);

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(G2C_ACCOUNT_VERIFY_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_ACCOUNT_VERIFY_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol            = g2c_account_verify_respond;
	pRespond->nCode                 = nRespondCode;
	pRespond->byMibaoMode           = pPlayer->byMibaoMode;
	pRespond->bChargeFlag           = pPlayer->nChargeFlag;
	pRespond->dwEndOfDayTime        = pPlayer->dwEndOfDayTime;
	pRespond->dwLeftTimeOfPoint     = pPlayer->dwLeftTimeOfPoint;
	pRespond->dwLeftTimeOfDay       = pPlayer->dwLeftTimeOfDay;
	pRespond->dwEndTimeOfFee        = pPlayer->dwEndTimeOfFee;
	pRespond->nLoginTime            = (time_t)pPlayer->dwLoginTime;
	pRespond->nLastLoginTime        = (time_t)pPlayer->dwLastLoginTime;
	pRespond->dwLastLoginIP         = pPlayer->dwLastLoginIP;
	pRespond->nZoneID               = m_pPaysysAgency->m_nZoneID;
	pRespond->dwLimitPlayTimeFlag   = pPlayer->dwLimitPlayTimeFlag;

	pRespond->szMatrixPosition[0] = '\0';
	//if (pPlayer->byMibaoMode == PASSPODMODE_MATRIX)
	//{
	//	strncpy(pRespond->szMatrixPosition, pPlayer->szMatrixPosition, sizeof(pRespond->szMatrixPosition));
	//	pRespond->szMatrixPosition[sizeof(pRespond->szMatrixPosition) - 1] = '\0';
	//}

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoMibaoVerifyRespond(KPlayerAgency* pPlayer, int nRespondCode)
{
	BOOL                        bResult         = false;
	int                         nRetCode        = 0;
	IMemBlock*                 piSendBuffer    = NULL;
	G2C_MIBAO_VERIFY_RESPOND*   pRespond        = NULL;

	assert(pPlayer);

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(G2C_MIBAO_VERIFY_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_MIBAO_VERIFY_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol        = g2c_mibao_verify_respond;
	pRespond->nCode             = nRespondCode;

	strncpy(pRespond->szMatrixPosition, pPlayer->szMatrixPosition, sizeof(pRespond->szMatrixPosition));
	pRespond->szMatrixPosition[sizeof(pRespond->szMatrixPosition) - 1] = '\0';

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoKickAccountNotify(KPlayerAgency* pPlayer)
{
	BOOL                        bResult         = false;
	int                         nRetCode        = 0;
	G2C_KICK_ACCOUNT_NOTIFY*    pNotify         = NULL;
	IMemBlock*                 piSendBuffer    = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(G2C_KICK_ACCOUNT_NOTIFY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (G2C_KICK_ACCOUNT_NOTIFY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->byProtocol = g2c_kick_account_notify;
	pNotify->byNothing  = 0;

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

// 随机名字到客户端
BOOL KPlayerManager::DoSyncRandomPlayerName(
									KPlayerAgency* pPlayer, CHAR* szPlayerName, INT nSex
									)
{
	BOOL bResult = FALSE;
	INT                     nRetCode        = 0;
	IMemBlock*             piSendBuffer    = NULL;
	G2C_RANDOM_PLAYERNAME *pSync = NULL;

	piSendBuffer = QCreateMemBlock(sizeof(G2C_RANDOM_PLAYERNAME));
	LOG_PROCESS_ERROR(piSendBuffer);

	pSync = (G2C_RANDOM_PLAYERNAME*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pSync);
	pSync->byProtocol  = g2c_random_playername;

	pSync->nSex = nSex; // 性别
	memcpy(pSync->szPlayerName, szPlayerName, _NAME_LEN); // string cpy

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);
	bResult = TRUE;

EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoSyncRoleList(
									KPlayerAgency* pPlayer, int nRoleIndex, int nRoleCount, BYTE* pbyRoleData, size_t uRoleDataLen
									)
{
	BOOL                    bResult         = false;
	int                     nRetCode        = 0;
	IMemBlock*             piSendBuffer    = NULL;
	G2C_SYNC_ROLE_LIST*     pRespond        = NULL;
	size_t                  uDataLen        = sizeof(G2C_SYNC_ROLE_LIST);

	if (pbyRoleData != NULL)
	{
		uDataLen += uRoleDataLen;
	}

	piSendBuffer = QCreateMemBlock((unsigned)uDataLen);
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_SYNC_ROLE_LIST*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol  = g2c_sync_role_list;
	pRespond->nRoleIndex  = nRoleIndex;
	pRespond->nRoleCount  = nRoleCount;

	if (pbyRoleData != NULL)
	{
		memcpy(pRespond->byData, pbyRoleData, uRoleDataLen);
	}

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoSyncNewbieMaps(KPlayerAgency* pPlayer, DWORD dwMapID, KNEWBIE_MAP_COPY_INFO CopyInfo[], int nCopyCount)
{
	BOOL                    bResult      = false;
	int                     nRetCode     = 0;
	IMemBlock*             piSendBuffer = NULL;
	G2C_SYNC_NEWBIE_MAPS*   pRespond     = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)(sizeof(G2C_SYNC_NEWBIE_MAPS) + sizeof(KNEWBIE_MAP_COPY_INFO) * nCopyCount));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_SYNC_NEWBIE_MAPS*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol = g2c_sync_newbie_maps;
	pRespond->dwMapID    = dwMapID;
	pRespond->nCopyCount = nCopyCount;

	memcpy(pRespond->CopyInfo, CopyInfo, sizeof(KNEWBIE_MAP_COPY_INFO) * nCopyCount);

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoCreateRoleRespond(KPlayerAgency* pPlayer, BYTE byCode, DWORD dwRoleID, BYTE* pbyData, size_t uDataLen)
{
	BOOL                        bResult      = false;
	int                         nRetCode     = 0;
	G2C_CREATE_ROLE_RESPOND*    pRespond     = NULL;
	IMemBlock*                 piSendBuffer = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)(sizeof(G2C_CREATE_ROLE_RESPOND) + uDataLen));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_CREATE_ROLE_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol    = g2c_create_role_respond;
	pRespond->byCode        = byCode;
	pRespond->dwRoleID      = dwRoleID;

	if (uDataLen > 0)
	{
		memcpy(pRespond->byData, pbyData, uDataLen);
	}

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoDeleteRoleRespond(KPlayerAgency* pPlayer, BYTE byCode, DWORD dwRoleID, time_t nDeleteTime)
{
	BOOL                        bResult      = false;
	int                         nRetCode     = 0;
	G2C_DELETE_ROLE_RESPOND*    pRespond     = NULL;
	IMemBlock*                 piSendBuffer = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(G2C_DELETE_ROLE_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_DELETE_ROLE_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol    = g2c_delete_role_respond;
	pRespond->byCode        = byCode;
	pRespond->dwRoleID      = dwRoleID;
	pRespond->nDeleteTime   = nDeleteTime;

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}


//将游戏服的ip地址和端口发送给客户端
BOOL KPlayerManager::DoGameLoginRespond(
										KPlayerAgency* pPlayer, BYTE byCode,
										DWORD dwRoleID, DWORD dwIP, 
										int nPort, const GUID& crGuid
										)
{
	QLogPrintf(LOG_INFO,"..KPlayerManager::DoGameLoginRespond dwRoleID=%d dwIP=%d nPort=%d",dwRoleID,dwIP,nPort);
	BOOL                bResult      = false;
	int                 nRetCode     = 0;
	G2C_SYNC_LOGIN_KEY* pRespond     = NULL;
	IMemBlock*         piSendBuffer = NULL;
	QIniFile iniIPCfg;
	
	piSendBuffer = QCreateMemBlock((unsigned)sizeof(G2C_SYNC_LOGIN_KEY));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_SYNC_LOGIN_KEY*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol = g2c_sync_login_key;
	pRespond->byCode     = byCode;
	pRespond->dwRoleID   = dwRoleID;
	pRespond->dwIP       = dwIP;
	pRespond->nPort      = nPort;
	pRespond->guid       = crGuid;
	
	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoSyncQueueState(KPlayerAgency* pPlayer, int nPosition)
{
	BOOL                    bResult         = false;
	int                     nRetCode        = 0;
	G2C_SYNC_QUEUE_STATE*   pNotify         = NULL;
	IMemBlock*             piSendBuffer    = NULL;

	assert(pPlayer);

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(G2C_SYNC_QUEUE_STATE));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (G2C_SYNC_QUEUE_STATE*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->byProtocol = g2c_sync_queue_state;
	pNotify->nPosition  = nPosition;

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoGiveupQueueRespond(KPlayerAgency* pPlayer)
{
	BOOL                        bResult         = false;
	int                         nRetCode        = 0;
	G2C_GIVEUP_QUEUE_RESPOND*   pRespond        = NULL;
	IMemBlock*                 piSendBuffer    = NULL;

	assert(pPlayer);

	piSendBuffer = QCreateMemBlock((unsigned)sizeof(G2C_GIVEUP_QUEUE_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_GIVEUP_QUEUE_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol    = g2c_giveup_queue_respond;
	pRespond->byNothing     = 0;

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoRenameRespond(KPlayerAgency* pPlayer, BYTE byCode, DWORD dwRoleID, BYTE* pbyData, size_t uDataLen)
{
	BOOL                    bResult         = false;
	int                     nRetCode        = 0;
	G2C_RENAME_RESPOND*     pRespond        = NULL;
	IMemBlock*             piSendBuffer    = NULL;

	piSendBuffer = QCreateMemBlock((unsigned)(sizeof(G2C_RENAME_RESPOND) + uDataLen));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_RENAME_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol    = g2c_rename_respond;
	pRespond->byCode        = byCode;
	pRespond->dwRoleID      = dwRoleID;

	if (pbyData)
	{
		memcpy(pRespond->byData, pbyData, uDataLen);
	}

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL KPlayerManager::DoSndaVerifyRespond(KPlayerAgency* pPlayer, int nRespondCode)
{
	BOOL                        bResult         = false;
	int                         nRetCode        = 0;
	G2C_SNDA_VERIFY_RESPOND*    pRespond        = NULL;
	IMemBlock*                 piSendBuffer    = NULL;

	assert(pPlayer);

	piSendBuffer = QCreateMemBlock((unsigned)(sizeof(G2C_SNDA_VERIFY_RESPOND)));
	LOG_PROCESS_ERROR(piSendBuffer);

	pRespond = (G2C_SNDA_VERIFY_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->byProtocol    = g2c_snda_verify_respond;
	pRespond->nRespondCode  = nRespondCode;

	nRetCode = pPlayer->piSocket->Send(piSendBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

void KPlayerManager::OnPingSignal(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	C2G_PING_SIGNAL* pPing = (C2G_PING_SIGNAL*)pbyData;

	pPlayer->nLastPingTime = m_nTimeNow;

	DoPingRespond(pPlayer, pPing->dwTime);
}

void KPlayerManager::OnHandshakeRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bRetCode                = false;
	C2G_HANDSHAKE_REQUEST*          pHandshake              = (C2G_HANDSHAKE_REQUEST*)pbyData;
	int                             nGameWorldLowerVersion  = 0;
	int                             nGameWorldUpperVersion  = 0;

	LOG_PROCESS_ERROR(pPlayer->nState == easWaitForHandshake);

	if (pHandshake->nGatewayVersion != GATEWAY_VERSION)
	{
		DoHandshakeRespond(pPlayer, ghcGatewayVersionError);
		pPlayer->nState = easWaitForDisconnect;
		goto EXIT0;
	}

	bRetCode = m_pRelayAgency->GetGameWorldVersion(&nGameWorldLowerVersion, &nGameWorldUpperVersion);
	if (!bRetCode)
	{
		DoHandshakeRespond(pPlayer, ghcGameWorldMaintenance);
		pPlayer->nState = easWaitForDisconnect;
		goto EXIT0;
	}

	if (pHandshake->nGameWorldVersion < nGameWorldLowerVersion ||
		pHandshake->nGameWorldVersion > nGameWorldUpperVersion)
	{
		DoHandshakeRespond(pPlayer, ghcGameWorldVersionError);
		pPlayer->nState = easWaitForDisconnect;
		goto EXIT0;
	}

	if (m_eAccountVerifier & emACCOUNT_VERIFIER_PAYSYS)
		bRetCode = m_pPaysysAgency->IsAvailable();
	if (!bRetCode)
	{
		DoHandshakeRespond(pPlayer, ghcAccountSystemLost);
		pPlayer->nState = easWaitForDisconnect;
		goto EXIT0;
	}

	DoHandshakeRespond(pPlayer, ghcHandshakeSucceed);

	pPlayer->nState = easWaitForVerifyInfo;

EXIT0:
	return;
}

void KPlayerManager::OnAccountVerifyRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	C2G_ACCOUNT_VERIFY_REQUEST*     pRequest = (C2G_ACCOUNT_VERIFY_REQUEST*)pbyData;
	KLockedAccountTable::iterator   it;

	pRequest->szAcc[sizeof(pRequest->szAcc) - 1]         = '\0';
	pRequest->szPsw[sizeof(pRequest->szPsw) - 1]         = '\0';
	pRequest->szAct[sizeof(pRequest->szAct) - 1]         = '\0';

	LOG_PROCESS_ERROR(pPlayer->nState == easWaitForVerifyInfo);
	LOG_PROCESS_ERROR(!m_pSndaAgency->m_bOpen);

	it = m_LockedAccountTable.find(pRequest->szAcc);
	if (it != m_LockedAccountTable.end())
	{
		DoAccountLockedNotify(pPlayer);
		pPlayer->nState = easWaitForDisconnect;
		goto EXIT0;
	}
	else
	{
		m_LockedAccountTable.insert(std::make_pair(pRequest->szAcc, pPlayer->nIndex));
	}

	strncpy(pPlayer->szAccount, pRequest->szAcc, sizeof(pPlayer->szAccount));
	pPlayer->szAccount[sizeof(pPlayer->szAccount) - 1] = '\0';

	strncpy(pPlayer->szPsw, pRequest->szPsw, sizeof(pPlayer->szPsw));
	pPlayer->szPsw[sizeof(pPlayer->szPsw) - 1] = '\0';

	strncpy(pPlayer->szAct, pRequest->szAct, sizeof(pPlayer->szAct));
	pPlayer->szAct[sizeof(pPlayer->szAct) - 1] = '\0';

	pPlayer->nZoneId = pRequest->nZoneId;
	pPlayer->nServerId = pRequest->nServerId;

	m_pRelayAgency->DoQueryAccountState(pPlayer);

	pPlayer->nState = easQueryAccountState;

EXIT0:
	return;
}

void KPlayerManager::OnSndaTokenVerifyRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	C2G_SNDA_TOKEN_VERIFY_REQUEST*  pRequest = (C2G_SNDA_TOKEN_VERIFY_REQUEST*)pbyData;
	KLockedAccountTable::iterator   it;

	pRequest->szSndaID[sizeof(pRequest->szSndaID) - 1]  = '\0';
	pRequest->szToken[sizeof(pRequest->szToken) - 1]    = '\0';

	LOG_PROCESS_ERROR(pPlayer->nState == easWaitForVerifyInfo);
	LOG_PROCESS_ERROR(m_pSndaAgency->m_bOpen);

	it = m_LockedAccountTable.find(pRequest->szSndaID);
	if (it != m_LockedAccountTable.end())
	{
		DoAccountLockedNotify(pPlayer);
		pPlayer->nState = easWaitForDisconnect;
		goto EXIT0;
	}
	else
	{
		m_LockedAccountTable.insert(std::make_pair(pRequest->szSndaID, pPlayer->nIndex));
	}

	strncpy(pPlayer->szAccount, pRequest->szSndaID, sizeof(pPlayer->szAccount));
	pPlayer->szAccount[sizeof(pPlayer->szAccount) - 1] = '\0';

	pPlayer->szPsw[0] = '\0';
	pPlayer->szAct[0] = '\0';

	strncpy(pPlayer->szSndaToken, pRequest->szToken, sizeof(pPlayer->szSndaToken));
	pPlayer->szSndaToken[sizeof(pPlayer->szSndaToken) - 1] = '\0';

	m_pRelayAgency->DoQueryAccountState(pPlayer);

	pPlayer->nState = easQueryAccountState;

EXIT0:
	return;
}

void KPlayerManager::OnMibaoVerifyRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	C2G_MIBAO_VERIFY_REQUEST*   pRequest = (C2G_MIBAO_VERIFY_REQUEST*)pbyData;

	LOG_PROCESS_ERROR(pPlayer->nState == easWaitForMibaoPassword);

	m_pPaysysAgency->DoMibaoVerifyRequest(pPlayer, pRequest->szMibaoPassword);

	pPlayer->dwVerifyPostTime    = GetTickCount();
	pPlayer->nState              = easWaitForMibaoVerify;

EXIT0:
	return;
}

void KPlayerManager::OnQueryNewbieMaps(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	LOG_PROCESS_ERROR(pPlayer->nState == easWaitForPlayerOperation);

	m_pRelayAgency->DoQueryNewbieMaps(pPlayer->nIndex);

	pPlayer->nState = easWaitForNewbieMapRespond;
EXIT0:
	return;
}

void KPlayerManager::OnRandomPlayernameRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	C2G_RANDOM_PLAYERNAME_REQUEST*    pRequest        = (C2G_RANDOM_PLAYERNAME_REQUEST*)pbyData;

	m_pRelayAgency->DoRandomNameRequest(pPlayer->nIndex, pRequest->nSex);

EXIT0:
	return;
}
void KPlayerManager::OnCreateRoleRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	C2G_CREATE_ROLE_REQUEST*    pRequest        = (C2G_CREATE_ROLE_REQUEST*)pbyData;
	size_t                      uExtendDataLen  = 0;

	LOG_PROCESS_ERROR(pPlayer->nState == easWaitForPlayerOperation);

	uExtendDataLen = uDataLen - sizeof(C2G_CREATE_ROLE_REQUEST);
	LOG_PROCESS_ERROR(uExtendDataLen > 0);

	m_pRelayAgency->DoCreateRoleRequest(pPlayer->nIndex, pPlayer->szAccount, pRequest->byData, uExtendDataLen);

	pPlayer->nState = easWaitForCreateRoleRespond;

EXIT0:
	return;
}

void KPlayerManager::OnDeleteRoleRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	C2G_DELETE_ROLE_REQUEST*    pRequest = (C2G_DELETE_ROLE_REQUEST*)pbyData;
	KROLE_LIST::const_iterator  it;

	LOG_PROCESS_ERROR(pPlayer->nState == easWaitForPlayerOperation);

	it = pPlayer->RoleList.find(pRequest->dwRoleID);
	LOG_PROCESS_ERROR(it != pPlayer->RoleList.end());

	m_pRelayAgency->DoDeleteRoleRequest(pPlayer, pRequest->dwRoleID);

	pPlayer->nState = easWaitForDeleteRoleRespond;

EXIT0:
	return;
}

void KPlayerManager::OnGameLoginRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	C2G_GAME_LOGIN_REQUEST*     pRequest        = (C2G_GAME_LOGIN_REQUEST*)pbyData;
	BYTE*                       pbyRoleData     = NULL;
	size_t                      uRoleDataLen    = 0;
	KROLE_LIST::const_iterator  it;

	LOG_PROCESS_ERROR(pPlayer->nState == easWaitForPlayerOperation);

	it = pPlayer->RoleList.find(pRequest->dwRoleID);
	LOG_PROCESS_ERROR(it != pPlayer->RoleList.end());

	pbyRoleData = (BYTE*)(it->second->GetData());
	LOG_PROCESS_ERROR(pbyRoleData);

	uRoleDataLen = it->second->GetSize();

	m_pRelayAgency->DoGameLoginRequest(pPlayer, pRequest->dwRoleID, pbyRoleData, uRoleDataLen);

	pPlayer->nState = easWaitForLoginPermission;

EXIT0:
	return;
}

void KPlayerManager::OnGiveupQueueRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	int                         nRetCode        = 0;
	KPlayerQueue*               pPlayerQueue    = NULL;

	PROCESS_ERROR(pPlayer->nState == easQueueWaiting);

	pPlayerQueue = m_pQueueManager->GetQueue(pPlayer->dwQueueID);
	LOG_PROCESS_ERROR(pPlayerQueue);

	nRetCode = pPlayerQueue->Dequeue(pPlayer->nIndex);
	LOG_PROCESS_ERROR(nRetCode);

	pPlayer->dwQueueID = INVALID_QUEUE_ID;

	DoGiveupQueueRespond(pPlayer);

	pPlayer->nState = easWaitForPlayerOperation;

EXIT0:
	return;
}

void KPlayerManager::OnRenameRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen)
{
	C2G_RENAME_REQUEST*     pRenameRequest  = (C2G_RENAME_REQUEST*)pbyData;
	KROLE_LIST::iterator    itFind;

	PROCESS_ERROR(pPlayer->nState == easWaitForPlayerOperation);

	itFind = pPlayer->RoleList.find(pRenameRequest->dwRoleID);
	LOG_PROCESS_ERROR(itFind != pPlayer->RoleList.end());

	m_pRelayAgency->DoRenameRequest(
		pPlayer->nIndex, pRenameRequest->dwRoleID, pRenameRequest->szNewName
		);

	pPlayer->nState = easWaitForRenameRespond;
EXIT0:
	return;
}

BOOL KPlayerManager::_LoadWhiteList()
{
	INT nRetCode = 0;
	INT nLines = 0;
	QTabFile cTabFile;
	CHAR szAccount[LOGIN_USER_ACCOUNT_MAX_LEN] = "";
	CHAR szPassword[LOGIN_USER_PASSWORD_MAX_LEN] = "";

	QLogPrintf(LOG_DEBUG, "Loading whitelist...");

	nRetCode = cTabFile.Load(ACCOUNT_WHITE_LIST_FILE);
	LOG_PROCESS_ERROR(nRetCode);

	m_mapWhiteList.clear();

	nLines = cTabFile.GetHeight();
	// 第一行是英文字段头描述
	for (INT n = 2; n <= nLines; ++n)
	{
		nRetCode = cTabFile.GetString(n, "Account", "", szAccount, countof(szAccount));
		LOG_PROCESS_ERROR(nRetCode && "Account column not found");

		nRetCode = cTabFile.GetString(n, "Password", "", szPassword, countof(szPassword));

		nRetCode = m_mapWhiteList.insert(std::make_pair(szAccount, szPassword)).second;
		if (!nRetCode)
			QLogPrintf(LOG_WARNING, "duplicate accounts [%s]", szAccount);
	}
	nRetCode = TRUE;
EXIT0:
	return nRetCode;
}

BOOL KPlayerManager::SmartAccountVerify( KPlayerAgency* pPlayer )
{
	INT nRetCode = FALSE;

	if (m_eAccountVerifier & emACCOUNT_VERIFIER_WHITE_LIST)
	{
		std::map<std::string, std::string>::iterator it = m_mapWhiteList.find(pPlayer->szAccount);
		LOG_PROCESS_ERROR(it != m_mapWhiteList.end() && "Account not in whitelist");
		// whitelist里密码为空的让账户服务器验证
		if (it->second != "")
		{
			PROCESS_SUCCESS(it->second == pPlayer->szPsw); // 密码正确
			LOG_PROCESS_ERROR(!"Password not match with whitelist");
		}
	}
	if (m_eAccountVerifier & emACCOUNT_VERIFIER_LDAP)
	{
		nRetCode = KLdapClient::Inst().VerifyAccount(pPlayer->szAccount, pPlayer->szAct); // 临时借用szAct传送域密码
		PROCESS_ERROR(nRetCode);
	}
	if (m_eAccountVerifier & emACCOUNT_VERIFIER_PAYSYS)
	{
		if (m_pSndaAgency->m_bOpen)
		{
			pPlayer->nState = easWaitForSDOAVerify;
			m_pSndaAgency->DoTokenVerifyRequest(pPlayer);
		}
		else
		{
			pPlayer->dwVerifyPostTime = GetTickCount();

			pPlayer->nState = easWaitForPaysysVerify;
			m_pPaysysAgency->DoAccountVerifyRequest(pPlayer);
		}
	}
EXIT1:
	nRetCode = TRUE;
EXIT0:
	if ( ! (m_eAccountVerifier & emACCOUNT_VERIFIER_PAYSYS))
	{
		//pPlayer->byMibaoMode = PASSPODMODE_UNBIND;
		INT nCode = nRetCode ? ACTION_SUCCESS : E_ACCOUNT_OR_PASSWORD;

		DoAccountVerifyRespond(pPlayer, nCode);

		if (nCode == ACTION_SUCCESS)
		{
			if (pPlayer->bAccountInGame)
			{
				m_pRelayAgency->DoKickAccountNotify(pPlayer);
				DoKickAccountNotify(pPlayer);
				UnlockAccount(pPlayer);
				pPlayer->nState = easWaitForDisconnect;
			}
			else
			{
				m_pRelayAgency->DoQueryRoleList(pPlayer);
				pPlayer->nState = easWaitForRoleList;
			}
		}
	}
	return nRetCode;
}
