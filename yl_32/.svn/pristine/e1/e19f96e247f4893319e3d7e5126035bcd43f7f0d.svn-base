#include "stdafx.h"
#include "battlezoneslave.h"
#include "GameCenter.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/korpgworld.h"
#include "globalserverslave.h"

BattleZoneSlave g_BZoneSlave;

BattleZoneSlave::BattleZoneSlave(void) : GlobalServerSlaveProtocolHandler<BattleZoneSlave>(m2r_bzone_protocol_begin, m2r_bzone_protocol_end)
{
#if 0
	m_piSocketStream = NULL;
	m_bSendErrorFlag = FALSE;
	m_nPingCycle = 0;
	m_nLastSendPacketTime = 0;
	m_nWorldIndex = 0;

	memset(m_ProcessProtocolFuns, 0, sizeof(m_ProcessProtocolFuns));
	memset(m_nProtocolSize, 0, sizeof(m_nProtocolSize));

	REGISTER_EXTERNAL_FUNC(m2r_handshake_respond,  &BattleZoneSlave::OnHandshakeRespond, sizeof(M2R_HANDSHAKE_RESPOND));
	REGISTER_EXTERNAL_FUNC(m2r_bzone_transfer_respond,  &BattleZoneSlave::OnBZoneTransferRespond, sizeof(M2R_TRANSFER_PLAYER_RESPOND));
	REGISTER_EXTERNAL_FUNC(m2r_bzone_return_request,  &BattleZoneSlave::OnBZoneReturnRequest, sizeof(M2R_BZONE_RETURN_REQUEST));
#endif
}

BattleZoneSlave::~BattleZoneSlave(void)
{
}

void BattleZoneSlave::OnBZoneTransferRequest()
{
	// check permission
	// Add Role
	// Set Role State BZoneTransferWait
	// OnPlayerLoginRequest
}

void BattleZoneSlave::OnConfirmLoginRequest()
{
	// LoadRoleData
	// SetRoleState ?
}

void BattleZoneSlave::OnRoleDataLoaded()
{
	// set role state 
	// BZone DoConfirmLoginRespond
}

void BattleZoneSlave::OnConfirmLoginRespond()
{
// got role data
	// ServerHandler::DoConfirmLoginRespond
	// ServerHandler::DoLoadRoleData
}

void BattleZoneSlave::OnSaveRole()
{
	// check role state
	// if not roaming state
	// send kick account
	// else
	// saverole 存完返回结果给源服？
}

BOOL BattleZoneSlave::DoKickAccount()
{
	// SendGlobal
	return FALSE;
}

void BattleZoneSlave::OnKickAccount()
{
	// Gs::DoKickAccountNotify
}

void BattleZoneSlave::OnBZoneReturnHomeRequest()
{
	// DoBZoneReturnHomeRespond
}

BOOL BattleZoneSlave::DoBZoneReturnHomeRespond()
{
	return FALSE;
}

void BattleZoneSlave::OnBZoneReturnHomeRespond()
{

	// do change server result
	// ServerHandler OnPlayerLogin
}

BOOL BattleZoneSlave::Init()
{
	BOOL bResult = FALSE;

	bResult = g_GlobalServerSlave.RegisterProtocolHandler(this);

	//RegisterProtocolProcessFunction(m2r_handshake_respond,  &BattleZoneSlave::OnHandshakeRespond, sizeof(M2R_HANDSHAKE_RESPOND));
	RegisterProtocolProcessFunction(m2r_bzone_transfer_respond,  &BattleZoneSlave::OnBZoneTransferRespond, sizeof(M2R_TRANSFER_PLAYER_RESPOND));
	RegisterProtocolProcessFunction(m2r_bzone_return_request,  &BattleZoneSlave::OnBZoneReturnRequest, sizeof(M2R_BZONE_RETURN_REQUEST));
#if 0
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	struct timeval TimeVal = {8, 0};
	QSocketConnector  Connector;

	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_SLAVE)
	{
		bResult = TRUE;
		goto EXIT0;
	}

	m_piSocketStream = Connector.Connect(g_pGameCenter->m_Settings.m_szGlobalServerAddr, 
		g_pGameCenter->m_Settings.m_nGlobalServerPort);
	LOG_PROCESS_ERROR(m_piSocketStream);

	bRetCode = m_piSocketStream->SetTimeout(&TimeVal);
	LOG_PROCESS_ERROR(bRetCode);

	m_nPingCycle = g_pGameCenter->m_Settings.m_nGlobalServerPingInterval;

	bRetCode = DoHandshakeRequest();
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	if (!bResult)
	{
		SAFE_RELEASE(m_piSocketStream);
	}

	if(g_pGameCenter->m_Settings.m_nGlobalServerMode == emRUNMODE_SLAVE)
	{
		QLogPrintf(
			LOG_INFO, "Connect to MasterCR %s:%d ... [%s]",
			g_pGameCenter->m_Settings.m_szGlobalServerAddr, 
			g_pGameCenter->m_Settings.m_nGlobalServerPort, 
			bResult ? "OK" : "Failed"
			);
	}
#endif
	return bResult;
}

BOOL BattleZoneSlave::UnInit()
{
#if 0
	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_SLAVE)
	{
		return TRUE;
	}

	return TRUE;
#endif
	return TRUE;
}

VOID BattleZoneSlave::Activate()
{
	#if 0
	ProcessPackage();
	#endif
}

BOOL BattleZoneSlave::Send(IMemBlock* piBuffer)
{
	return g_GlobalServerSlave.Send(piBuffer);
#if 0
	BOOL bResult  = FALSE;
	INT nRetCode = FALSE;

	assert(piBuffer);
	LOG_PROCESS_ERROR(m_piSocketStream);

	nRetCode = m_piSocketStream->Send(piBuffer);
	LOG_PROCESS_ERROR(nRetCode == 1);

	m_nLastSendPacketTime = KSysService::GameTime(NULL);
	bResult = TRUE;

EXIT0:
	if (!bResult)
		m_bSendErrorFlag = TRUE;

	return bResult;
#endif
}

#if 0
BOOL BattleZoneSlave::DoHandshakeRequest()
{
	BOOL                    bResult         = false;
	BOOL                    bRetCode        = false;
	IMemBlock*             piPackage       = NULL;
	R2M_HANDSHAKE_REQUEST*  pHandshake      = NULL;

	piPackage = QCreateMemBlock((unsigned)sizeof(S2R_HANDSHAKE_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pHandshake = (R2M_HANDSHAKE_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pHandshake);

	pHandshake->wProtocolID = r2m_handshake_request;
	pHandshake->nGameVersion  = KD_PROTOCOL_VERSION;
	pHandshake->nServerTime  = KSysService::GameTime(NULL);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}


BOOL BattleZoneSlave::OnHandshakeRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                    bRetCode   = FALSE;
	M2R_HANDSHAKE_RESPOND*  pHandshake = (M2R_HANDSHAKE_RESPOND*)pbyData;
	IMemBlock*             piSendBuffer    = NULL;
	R2S_BZONE_HANDSHAKE_RESPOND* pNotify = NULL;

	m_nWorldIndex = pHandshake->nWorldIndex;
	g_pGameCenter->m_nBaseTime = pHandshake->nBaseTime;
	g_pGameCenter->m_nWorkLoop = pHandshake->nStartFrame;
	g_pGameCenter->m_bDevMode = pHandshake->nDevMode;

	if (pHandshake->nDevMode)
	{
		QConsoleHelper::DoInfoColor();
		QLogPrintf(LOG_INFO, "Running DevMode, GM Command OPEN", pHandshake->nWorldIndex);
		QConsoleHelper::RestoreColor();
	}

	QLogPrintf(LOG_INFO, "Set world index = %d\n", pHandshake->nWorldIndex);

	piSendBuffer = QCreateMemBlock(sizeof(R2S_BZONE_HANDSHAKE_RESPOND));
	LOG_PROCESS_ERROR(piSendBuffer);

	pNotify = (R2S_BZONE_HANDSHAKE_RESPOND*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pNotify);

	pNotify->nBaseTime = g_pGameCenter->m_nBaseTime;
	pNotify->nDevMode = g_pGameCenter->m_bDevMode;
	pNotify->nStartFrame = g_pGameCenter->m_nWorkLoop;

	g_pGameCenter->m_GameServer.Broadcast(piSendBuffer);

EXIT0:
	return TRUE;
}

BOOL BattleZoneSlave::DoPingSignal()
{
	BOOL						bResult				= FALSE;
	BOOL						bRetCode			= FALSE;
	IMemBlock*                 piSendBuffer        = NULL;
	R2M_PING_SIGNAL*			pPingSingal			= NULL;

	piSendBuffer = QCreateMemBlock(sizeof(R2M_PING_SIGNAL));
	LOG_PROCESS_ERROR(piSendBuffer);

	pPingSingal = (R2M_PING_SIGNAL*)piSendBuffer->GetData();
	LOG_PROCESS_ERROR(pPingSingal);

	pPingSingal->wProtocolID       = r2m_ping_signal;
	pPingSingal->dwReserved        = 0;

	bRetCode = Send(piSendBuffer);
	LOG_PROCESS_ERROR(bRetCode);   

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piSendBuffer);
	return bResult;
}

BOOL BattleZoneSlave::ProcessPackage()
{
	BOOL bResult = FALSE;
	INT nRetCode = FALSE;
	INT nConnectionAlive = TRUE;
	IMemBlock* piBuffer = NULL;

	if(g_pGameCenter->m_Settings.m_nGlobalServerMode != emRUNMODE_SLAVE)
	{
		bResult = TRUE;
		goto EXIT0;
	}

	assert(m_piSocketStream);

	while (TRUE)
	{
		const struct timeval        TimeVal     = {0, 0};
		INTERNAL_PROTOCOL_HEADER*   pHeader     = NULL;
		size_t                      uPakSize    = 0;
		PROCESS_PROTOCOL_FUNC       pFunc       = NULL;

		if (KSysService::GameTime(NULL) - m_nLastSendPacketTime > m_nPingCycle)
		{
			DoPingSignal();
		}

		nRetCode = m_piSocketStream->CheckCanRecv(&TimeVal);
		if (nRetCode == -1)
		{
			nConnectionAlive = FALSE;
			goto EXIT0;
		}
		if (nRetCode == 0)
		{
			break;
		}

		LOG_PROCESS_ERROR(nRetCode == 1);

		SAFE_RELEASE(piBuffer);

		nRetCode = m_piSocketStream->Recv(&piBuffer);
		if (nRetCode == -1)
		{
			nConnectionAlive = FALSE;
			goto EXIT0;
		}
		LOG_PROCESS_ERROR(nRetCode == 1);

		pHeader = (INTERNAL_PROTOCOL_HEADER*)piBuffer->GetData();
		LOG_PROCESS_ERROR(pHeader);

		LOG_PROCESS_ERROR(pHeader->wProtocolID < m2r_protocol_end);

		uPakSize = piBuffer->GetSize();
		LOG_PROCESS_ERROR(uPakSize >= m_nProtocolSize[pHeader->wProtocolID]);

		
		pFunc = m_ProcessProtocolFuns[pHeader->wProtocolID];
		LOG_PROCESS_ERROR(pFunc);

		(this->*pFunc)((BYTE*)pHeader, uPakSize);
	}

	bResult = TRUE;

EXIT0:
	if (m_piSocketStream && !nConnectionAlive)
	{
		QLogPrintf(LOG_DEBUG, "[SlaveCR] LastPakTime = %u\n", m_nLastSendPacketTime);
		QLogPrintf(LOG_DEBUG, "[SlaveCR] CurrentTime = %u\n", KSysService::GameTime(NULL));

		QLogPrintf(LOG_ERR, "Coordinator lost, shutdown !\n");

		//TODO zhaoyu:shutdown?
	}
	SAFE_RELEASE(piBuffer);
	return bResult;
}
#endif

BOOL BattleZoneSlave::DoBZoneTransferRequest(KRole* pRole, INT nPlayerIdx, KRoleBaseInfo* pBaseInfo, BYTE *byRoleData, DWORD uRoleDataLen)
{
	BOOL bRetCode = FALSE;
	IMemBlock* piPackage = NULL;
	R2M_BZONE_TRANSFER_REQUEST* pTransferPackage = NULL;

	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(pBaseInfo);

	piPackage = QCreateMemBlock(sizeof(R2M_BZONE_TRANSFER_REQUEST) + uRoleDataLen);
	LOG_PROCESS_ERROR(piPackage);

	pTransferPackage = (R2M_BZONE_TRANSFER_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pTransferPackage);

	pTransferPackage->wProtocolID       = r2m_bzone_transfer_request;
	pTransferPackage->dwRoleID          = pRole->GetId();

	strncpy(pTransferPackage->szRoleName, pRole->m_szName, sizeof(pTransferPackage->szRoleName));
	pTransferPackage->szRoleName[sizeof(pTransferPackage->szRoleName) - 1] = '\0';

	strncpy(pTransferPackage->szAccount, pRole->m_pszAccount, sizeof(pTransferPackage->szAccount));
	pTransferPackage->szAccount[sizeof(pTransferPackage->szAccount) - 1] = '\0';

	memcpy(&(pTransferPackage->RoleBaseInfo), pBaseInfo, sizeof(KRoleBaseInfo));
	pTransferPackage->dwSourceRoleID        = pRole->GetId();
	pTransferPackage->nPlayerIdx = nPlayerIdx;

	memcpy(pTransferPackage->byData, byRoleData, uRoleDataLen);
	pTransferPackage->uRoleDataLen = uRoleDataLen;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bRetCode;
}

BOOL BattleZoneSlave::OnBZoneTransferRespond(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bResult     = FALSE;
	BOOL                            bRetCode    = FALSE;
	IMemBlock*                     piPackage   = NULL;
	M2R_TRANSFER_PLAYER_RESPOND*  pRespond		= (M2R_TRANSFER_PLAYER_RESPOND*)pbyData;
	const GUID                      cGuid       = {0};
	KRole*                          pRole       = NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	//pRespond->nSourceServerId = nConnIndex;

	bRetCode = g_pGameCenter->m_GameServer.DoBZoneTransferPlayerRespond(
		pRole->m_nConnIndex, pRespond->dwDstPlayerID, pRole->m_dwPlayerID, pRespond->bSucceed, 
		pRespond->dwAddress, pRespond->wPort, &pRespond->Guid
		);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL BattleZoneSlave::OnBZoneReturnRequest(BYTE* pbyData, size_t uDataLen)
{
	BOOL                            bRetCode            = FALSE;
	INT								nResult				= 0;
	KRole*							pRole				= NULL;
	M2R_BZONE_RETURN_REQUEST*    pRequest            = (M2R_BZONE_RETURN_REQUEST*)pbyData;
	const KMapCopy*                 pcDestMapCopy       = NULL;
	KRoleBaseInfo&					rRoleBaseInfo		= pRequest->RoleBaseInfo;
	int                         nMapId       = rRoleBaseInfo.LastEntry.dwMapID;
	int                         nMapCopyIndex       = rRoleBaseInfo.LastEntry.nMapCopyIndex;
	KMapInfo*                   pMapInfo            = NULL;
	KACCOUNT_LOGIN_INFO         AccInfo;
	KCrossSvData				sCrossSvData;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRequest->dwRoleID);
	LOG_PROCESS_ERROR(pRole);

	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(nMapId);
	PROCESS_ERROR_RET_CODE(pMapInfo != NULL, smrcFailed);

	bRetCode = g_pGameCenter->m_MapManager.IncreaseMapCopyPlayerCount(nMapId, nMapCopyIndex);
	LOG_PROCESS_ERROR(bRetCode);

	pcDestMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(nMapId, nMapCopyIndex);
	if (pcDestMapCopy == NULL || pcDestMapCopy->m_eState != eMapStateNormal)
	{
		g_pGameCenter->m_RoleManager.DelRole(pRequest->dwRoleID);

		QLogPrintf(LOG_ERR, "BZoneTransfer map %d %d not found", nMapId, nMapCopyIndex);

		goto EXIT0;
	}

	// 设置master上的角色id
	pRole->SetSourcePlayerId(pRequest->dwSourceRoleID);

	SetMultiFinishInfo(pRequest->dwRoleID, rRoleBaseInfo.CurrentPos.dwMapID, pRequest->nExValue);

	// 从中心服传回来的额外数据
	if(pRequest->uExDataLen > 0)
	{
		sCrossSvData.uExDataLen = pRequest->uExDataLen;
		memcpy(sCrossSvData.byExData, pRequest->byExData, pRequest->uExDataLen);
		m_mapCrossSvDatas[pRequest->dwRoleID] = sCrossSvData;
	}
	
	pRole->m_nDestConnIndex     = pcDestMapCopy->m_nConnIndex;
	pRole->m_dwDestMapID        = nMapId;
	pRole->m_nDestMapCopyIndex  = nMapCopyIndex;

	// 改变登录地图
	rRoleBaseInfo.CurrentPos.dwMapID = nMapId;
	rRoleBaseInfo.CurrentPos.nMapCopyIndex = nMapCopyIndex;
	rRoleBaseInfo.CurrentPos.byFaceDirection = 0;
	rRoleBaseInfo.CurrentPos.nX = rRoleBaseInfo.LastEntry.nX;
	rRoleBaseInfo.CurrentPos.nY = rRoleBaseInfo.LastEntry.nY;
	rRoleBaseInfo.CurrentPos.nZ = rRoleBaseInfo.LastEntry.nZ;

	bRetCode = g_pGameCenter->m_GameServer.DoPlayerLoginRequest(
		pcDestMapCopy->m_nConnIndex,
		AccInfo, pRequest->dwRoleID, rRoleBaseInfo);
	LOG_PROCESS_ERROR(bRetCode);

	pRole->SetRoleState(rsLG_WaitForLogin);

	nResult = TRUE;
EXIT0:
	return nResult;
}

BOOL BattleZoneSlave::DoBZoneReturnRespond(DWORD dwRoleId, int nPlayerIndex, int nRespondCode, DWORD dwRoleSourceID, 
											DWORD dwQueueID, int nLoginIP, int nLoginPort, GUID Guid)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piPackage   = NULL;
	R2M_BZONE_RETURN_RESPOND*    pRespond    = NULL;
	const GUID                      cGuid       = {0};
	INT nConnIndex								= 0;

	piPackage = QCreateMemBlock(sizeof(R2M_BZONE_RETURN_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespond = (R2M_BZONE_RETURN_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID        = r2m_bzone_return_respond;
	pRespond->dwDstPlayerID = dwRoleId;
	pRespond->bSucceed        = (BYTE)nRespondCode;
	pRespond->dwPlayerID      = dwRoleSourceID;
	pRespond->dwAddress          = nLoginIP;
	pRespond->wPort         = nLoginPort;
	pRespond->Guid          = Guid;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

VOID BattleZoneSlave::SetMultiFinishInfo(DWORD dwRoleId, INT nMapId, INT nStar)
{
	KMULTI_FINISH_INFO info;

	std::map<DWORD, KMULTI_FINISH_INFO>::iterator it = m_mapFinishInfos.find(dwRoleId);
	LOG_PROCESS_ERROR(it == m_mapFinishInfos.end());

	info.nMapId = nMapId;
	info.nStar = nStar;

	m_mapFinishInfos[dwRoleId] = info;

EXIT0:
	return;
}

VOID BattleZoneSlave::RemoveMultiFinishInfo(DWORD dwRoleId)
{
	std::map<DWORD, KMULTI_FINISH_INFO>::iterator it = m_mapFinishInfos.find(dwRoleId);
	LOG_PROCESS_ERROR(it != m_mapFinishInfos.end());

	m_mapFinishInfos.erase(it);

EXIT0:
	return;
}

VOID BattleZoneSlave::OnGetMultiAwardRequest(DWORD dwRoleId)
{
	KRole* pRole = NULL;
	BOOL bRetCode = FALSE;
	std::map<DWORD, KMULTI_FINISH_INFO>::iterator it = m_mapFinishInfos.find(dwRoleId);
	PROCESS_ERROR(it != m_mapFinishInfos.end());

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleId);
	LOG_PROCESS_ERROR(pRole);

	bRetCode = g_pGameCenter->m_GameServer.DoGetMultiAwardRespond(pRole->m_nConnIndex, dwRoleId, it->second.nMapId, it->second.nStar);
	LOG_PROCESS_ERROR(bRetCode);

	m_mapFinishInfos.erase(it);

EXIT0:
	return;
}

VOID BattleZoneSlave::OnGetCrossSvExDataRequest(DWORD dwRoleId)
{
	KRole* pRole = NULL;
	BOOL bRetCode = FALSE;
	std::map<DWORD, KCrossSvData>::iterator it = m_mapCrossSvDatas.find(dwRoleId);
	PROCESS_ERROR(it != m_mapCrossSvDatas.end());

	pRole = g_pGameCenter->m_RoleManager.GetRole(dwRoleId);
	LOG_PROCESS_ERROR(pRole);

	bRetCode = g_pGameCenter->m_GameServer.DoGetCrossSvExDataRespond(pRole->m_nConnIndex, dwRoleId, it->second.byExData, it->second.uExDataLen);
	LOG_PROCESS_ERROR(bRetCode);

	m_mapCrossSvDatas.erase(it);

EXIT0:
	return;
}

BOOL BattleZoneSlave::GetCrossSvData(DWORD dwRoleId, IMemBlock** ppiRoleData)
{
	BOOL bResult = FALSE;
	DWORD uDataLen = 0;
	IMemBlock* piPackage   = NULL;
	VOID *pExData = NULL;
	std::map<DWORD, KCrossSvData>::iterator it = m_mapCrossSvDatas.find(dwRoleId);

	assert(ppiRoleData);
	LOG_PROCESS_ERROR(it != m_mapCrossSvDatas.end());

	uDataLen = it->second.uExDataLen;
	piPackage = QCreateMemBlock(it->second.uExDataLen);
	LOG_PROCESS_ERROR(piPackage);

	pExData = piPackage->GetData();
	LOG_PROCESS_ERROR(pExData);

	memcpy(pExData, it->second.byExData, uDataLen);

	*ppiRoleData = piPackage;
	(*ppiRoleData)->AddRef();

	bResult = TRUE;

EXIT0:
	return bResult;
}