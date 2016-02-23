#include "stdafx.h"
#include "battlezone/battlezonemaster.h"
#include "GameCenter.h"
#include "globalservermaster.h"
#include "overallqueue/kqueuemaster.h"

BattleZoneMaster g_BZoneMaster;

BattleZoneMaster::BattleZoneMaster(void)
: GlobalServerMasterProtocolHandler<BattleZoneMaster>(r2m_bzone_protocol_begin, r2m_bzone_protocol_end)
{

}

BattleZoneMaster::~BattleZoneMaster(void)
{
}


BOOL BattleZoneMaster::Init()
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;

	bRetCode = LoadMultiMapPos();
	LOG_PROCESS_ERROR(bRetCode);

    g_GlobalServerMaster.RegisterProtocolHandler(this);

	RegisterProtocolProcessFunction(r2m_bzone_transfer_request,  &BattleZoneMaster::OnBZoneTransferRequest, sizeof(R2M_BZONE_TRANSFER_REQUEST));
	RegisterProtocolProcessFunction(r2m_bzone_return_respond,  &BattleZoneMaster::OnBZoneReturnRespond, sizeof(R2M_BZONE_RETURN_RESPOND));

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL BattleZoneMaster::UnInit()
{
	return TRUE;
}

VOID BattleZoneMaster::Activate()
{

}

BOOL BattleZoneMaster::Send(int nConnIndex, IMemBlock* piBuffer)
{
	BOOL bResult = FALSE;

	bResult = g_GlobalServerMaster.Send(nConnIndex, piBuffer);
EXIT0:
	return bResult;
}

BOOL BattleZoneMaster::OnBZoneTransferRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bRetCode            = FALSE;
	INT								nResult				= 0;
	R2M_BZONE_TRANSFER_REQUEST*    pRequest				= (R2M_BZONE_TRANSFER_REQUEST*)pbyData;
	KRole*                          pRole               = NULL;
	const KMapCopy*                 pcDestMapCopy       = NULL;
	KRoleBaseInfo&					rRoleBaseInfo		= pRequest->RoleBaseInfo;
	int                         nMapId					= 0;
	int                         nMapCopyIndex			= 1;
	DWORD						dwTeamId				= 0;
	KMapInfo*                   pMapInfo				= NULL;
	KRole*                      pTeamLeader				= NULL;
	KTeam*						pTeam					= NULL;
	KMULTI_MAP_POS				sPos;
	KACCOUNT_LOGIN_INFO         AccInfo;
	KRoleData sRoleData;
	GUID							guid                = {0, 0, 0, 0};

	static INT nRoleIdGen = 0;
	DWORD dwRoleId = ++nRoleIdGen | BZONE_PLAYER_ID_MASK;
	//DWORD dwRoleId = pRequest->dwRoleID | BZONE_PLAYER_ID_MASK;

	m_mapRoleSlave[dwRoleId] = nConnIndex;

	assert(pRequest->uRoleDataLen < 100000);

    INT nFightScore = *((INT*)rRoleBaseInfo.byReserved);
	// Add Roaming Role (temporary role)
	pRole = g_pGameCenter->m_RoleManager.AddRole(pRequest->szAccount,
		pRequest->szRoleName,
		dwRoleId,
		rRoleBaseInfo.byLevel,
		rRoleBaseInfo.cRoleType,
		rRoleBaseInfo.byFaction,
		nFightScore,
		rRoleBaseInfo.nLastSaveTime,
		rRoleBaseInfo.nCreateTime
		);
	LOG_PROCESS_ERROR(pRole);

	pRole->SetSourcePlayerId(pRequest->dwRoleID);

	// 加入队伍
	dwTeamId = g_QueueMaster.GetInstanceTeamId(nConnIndex, pRequest->dwSourceRoleID);
	LOG_PROCESS_ERROR(dwTeamId != KD_BAD_ID);

	pTeam = g_pGameCenter->m_TeamCenter.GetTeam(dwTeamId);
	LOG_PROCESS_ERROR(pTeam);

	g_pGameCenter->m_TeamCenter.AddMember(dwTeamId, dwRoleId, 1);
	g_pGameCenter->m_TeamCenter.ProcessTeam(dwTeamId, pTeam);

	// 加入地图
	nMapId = g_QueueMaster.GetInstanceMapId(nConnIndex, pRequest->dwSourceRoleID);
	nMapCopyIndex = g_QueueMaster.GetInstanceMapCopyIdx(nConnIndex, pRequest->dwSourceRoleID);

	pMapInfo = g_pGameCenter->m_MapManager.GetMapInfo(nMapId);
	PROCESS_ERROR_RET_CODE(pMapInfo != NULL, smrcFailed);

	pcDestMapCopy = g_pGameCenter->m_MapManager.GetMapCopy(nMapId, nMapCopyIndex);
	if (pcDestMapCopy == NULL/* || pcDestMapCopy->m_eState != eMapStateNormal*/)
	{
		g_pGameCenter->m_RoleManager.DelRole(dwRoleId);

		QLogPrintf(LOG_ERR, "BZoneTransfer map %d %d not found", nMapId, nMapCopyIndex);

		goto EXIT0;
	}

	bRetCode = g_pGameCenter->m_MapManager.IncreaseMapCopyPlayerCount(nMapId, nMapCopyIndex);
	LOG_PROCESS_ERROR(bRetCode);

	pRole->m_nDestConnIndex     = pcDestMapCopy->m_nConnIndex;
	pRole->m_dwDestMapID        = nMapId;
	pRole->m_nDestMapCopyIndex  = nMapCopyIndex;

	// 改变登录地图
	rRoleBaseInfo.CurrentPos.dwMapID = nMapId;
	rRoleBaseInfo.CurrentPos.nMapCopyIndex = nMapCopyIndex;
	rRoleBaseInfo.CurrentPos.byFaceDirection = 0;
	LOG_PROCESS_ERROR(GetMultiPos(nMapId, sPos));
	rRoleBaseInfo.CurrentPos.nX = sPos.nX;
	rRoleBaseInfo.CurrentPos.nY = sPos.nY;
	rRoleBaseInfo.CurrentPos.nZ = sPos.nZ;

	sRoleData.uRoleDataLen = pRequest->uRoleDataLen;
	memcpy(sRoleData.RoleData, pRequest->byData, pRequest->uRoleDataLen);
	m_mapRoleData[dwRoleId] = sRoleData;

	/*
	AccInfo.nChargeFlag             = pRequest->nChargeFlag;
	AccInfo.ExtPointInfo            = pRequest->ExtPointInfo;
	AccInfo.dwEndTimeOfFee          = pRequest->dwEndTimeOfFee;
	AccInfo.dwCoin                  = pRequest->dwCoin;
	AccInfo.dwLimitPlayTimeFlag     = pRequest->dwLimitPlayTimeFlag;
	AccInfo.dwLimitOnlineSecond     = pRequest->dwLimitOnlineSecond;
	AccInfo.dwLimitOfflineSecond    = pRequest->dwLimitOfflineSecond;
	*/
	AccInfo.nIdentity               = pRequest->nPlayerIdx;

	//pRole->SetRoleState(rsBZ_WaitForPermit);

	bRetCode = g_pGameCenter->m_GameServer.DoPlayerLoginRequest(
		pcDestMapCopy->m_nConnIndex,
		AccInfo, pRole->GetId(), rRoleBaseInfo);
	LOG_PROCESS_ERROR(bRetCode);

	pRole->SetRoleState(rsLG_WaitForLogin);

	nResult = TRUE;
EXIT0:
	return nResult;
}

INT BattleZoneMaster::GetRoleSlaveIdx(DWORD dwRoleId)
{
	INT nConnIdx = 0;
	KROLE_SLAVEIDX_TABLE::iterator it = m_mapRoleSlave.find(dwRoleId);

	LOG_PROCESS_ERROR(it != m_mapRoleSlave.end());

	nConnIdx = it->second;

EXIT0:
	return nConnIdx;
}

BOOL BattleZoneMaster::GetRoleData(DWORD dwRoleID, IMemBlock** ppiRoleData)
{
	BOOL bResult = FALSE;
	DWORD uDataLen = 0;
	IMemBlock* piPackage   = NULL;
	VOID *pRoleData = NULL;
	KROLE_DATA_TABLE::iterator it = m_mapRoleData.find(dwRoleID);

	assert(ppiRoleData);
	LOG_PROCESS_ERROR(it != m_mapRoleData.end());

	uDataLen = it->second.uRoleDataLen;
	piPackage = QCreateMemBlock(it->second.uRoleDataLen);
	LOG_PROCESS_ERROR(piPackage);

	pRoleData = piPackage->GetData();
	LOG_PROCESS_ERROR(pRoleData);

	memcpy(pRoleData, it->second.RoleData, uDataLen);

	*ppiRoleData = piPackage;
	(*ppiRoleData)->AddRef();

	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL BattleZoneMaster::DoBZoneTransferRespond(DWORD dwRoleId, int nPlayerIndex, int nRespondCode, DWORD dwRoleSourceID, 
										DWORD dwQueueID, int nLoginIP, int nLoginPort, GUID Guid)
{
	BOOL                            bResult     = false;
	BOOL                            bRetCode    = false;
	IMemBlock*                     piPackage   = NULL;
	M2R_TRANSFER_PLAYER_RESPOND*    pRespond    = NULL;
	const GUID                      cGuid       = {0};
	INT nConnIndex								= 0;

	piPackage = QCreateMemBlock(sizeof(M2R_TRANSFER_PLAYER_RESPOND));
	LOG_PROCESS_ERROR(piPackage);

	pRespond = (M2R_TRANSFER_PLAYER_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID        = r2m_bzone_transfer_respond;
	pRespond->dwDstPlayerID = dwRoleId;
	pRespond->bSucceed        = (BYTE)nRespondCode;
	pRespond->dwPlayerID      = dwRoleSourceID;
	pRespond->dwAddress          = nLoginIP;
	pRespond->wPort         = nLoginPort;
	pRespond->Guid          = Guid;

	nConnIndex = GetRoleSlaveIdx(dwRoleId);

	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL BattleZoneMaster::DoBZoneReturnRequest(KRole* pRole, KRoleBaseInfo* pBaseInfo, INT nExValue, LPBYTE byExData, DWORD uExDataLen)
{
	INT nConnIdx = 0;
	BOOL bRetCode = FALSE;
	IMemBlock* piPackage = NULL;
	M2R_BZONE_RETURN_REQUEST* pReturnPackage = NULL;

	LOG_PROCESS_ERROR(pRole);
	LOG_PROCESS_ERROR(pBaseInfo);

	piPackage = QCreateMemBlock(sizeof(M2R_BZONE_RETURN_REQUEST) + uExDataLen);
	LOG_PROCESS_ERROR(piPackage);

	pReturnPackage = (M2R_BZONE_RETURN_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pReturnPackage);

	pReturnPackage->wProtocolID       = m2r_bzone_return_request;
	pReturnPackage->dwRoleID          = pRole->GetSourcePlayerId();
	pReturnPackage->dwSourceRoleID	  = pRole->GetId();
	pReturnPackage->nExValue		= nExValue;
	memcpy(&(pReturnPackage->RoleBaseInfo), pBaseInfo, sizeof(KRoleBaseInfo));

	memcpy(pReturnPackage->byExData, byExData, uExDataLen);
	pReturnPackage->uExDataLen = uExDataLen;

	nConnIdx = GetRoleSlaveIdx(pRole->GetId());

	bRetCode = Send(nConnIdx, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bRetCode;
}

BOOL BattleZoneMaster::OnBZoneReturnRespond(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL                            bResult     = FALSE;
	BOOL                            bRetCode    = FALSE;
	IMemBlock*                     piPackage   = NULL;
	R2M_BZONE_RETURN_RESPOND*		pRespond		= (R2M_BZONE_RETURN_RESPOND*)pbyData;
	const GUID                      cGuid       = {0};
	KRole*                          pRole       = NULL;
	KROLE_DATA_TABLE::iterator it;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pRespond->dwPlayerID);
	LOG_PROCESS_ERROR(pRole);

	//pRespond->nSourceServerId = nConnIndex;

	if(pRespond->bSucceed)
	{
		// 把人物数据从缓存里删掉
		it = m_mapRoleData.find(pRole->GetId());
		LOG_PROCESS_ERROR(it != m_mapRoleData.end());

		m_mapRoleData.erase(it);
	}

	bRetCode = g_pGameCenter->m_GameServer.DoBZoneReturnRespond(
		pRole->m_nConnIndex, pRespond->dwDstPlayerID, pRole->m_dwPlayerID, pRespond->bSucceed, 
		pRespond->dwAddress, pRespond->wPort, &pRespond->Guid
		);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

BOOL BattleZoneMaster::LoadMultiMapPos()
{
	BOOL bResult = FALSE;
	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(MULTI_MAP_POS_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 2; i <= nFileHeight; i++)
	{
		KMULTI_MAP_POS posInfo;
		tabFile->GetInteger(i, "MapId", 0, &posInfo.nMapId);
		tabFile->GetInteger(i, "X", 0, &posInfo.nX);
		tabFile->GetInteger(i, "Y", 0, &posInfo.nY);
		tabFile->GetInteger(i, "Z", 0, &posInfo.nZ);

		m_mapMultiPos[posInfo.nMapId] = posInfo;
	}

	bResult = TRUE;

EXIT0:
	return bResult;
}

BOOL BattleZoneMaster::GetMultiPos(INT nMapId, KMULTI_MAP_POS& sPos)
{
	BOOL bResult = FALSE;
	std::map<INT, KMULTI_MAP_POS>::iterator it = m_mapMultiPos.find(nMapId);
	LOG_PROCESS_ERROR(it != m_mapMultiPos.end());

	sPos = it->second;

	bResult = TRUE;
EXIT0:
	return bResult;
}