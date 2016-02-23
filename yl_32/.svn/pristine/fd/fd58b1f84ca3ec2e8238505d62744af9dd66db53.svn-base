#include "stdafx.h"
#include "battlezonedef.h"
#include "serverbase/protocol/RelayMasterProtocol.h"
#include "KRole.h"
#include "battlezone.h"
#include "GameCenter.h"


BattleZone g_BZone;


BattleZone::BattleZone(void)
{
}

BattleZone::~BattleZone(void)
{
}

bool BattleZone::Init(void)
{
	// read config
	// load zone record

	m_eRunMode = emRUNMODE_STAND_ALONE;

	return false;
}

BOOL BattleZone::SendGlobal(IMemBlock* piBuffer)
{
	if (m_eRunMode == emRUNMODE_STAND_ALONE || m_eRunMode == emRUNMODE_MASTER)
	{
		OnReceivedProtocol();
	}
	else
	{
		m_pMasterSocket->Send(NULL);
	}
	return FALSE;
}

BOOL BattleZone::DoConfirmPlayerLoginRequest()
{
	// SendGlobal
	// set role state ?
	return FALSE;
}

BOOL BattleZone::DoConfirmPlayerLoginRespond()
{
	// SendGlobal
	return FALSE;
}

BOOL BattleZone::DoSaveRoleData()
{

	// SendGlobal
	return FALSE;
}

void BattleZone::OnReceivedProtocol()
{

}

BOOL BattleZone::DoLoadRoleDataRequest( DWORD dwRoleID )
{
	BOOL                        bResult         = false;
	BOOL                        bRetCode        = false;
	IMemBlock*                 piPackage       = NULL;
	R2M_LOAD_ROLE_DATA_REQUEST* pLoadRoleData   = NULL;

	piPackage = QCreateMemBlock(sizeof(R2M_LOAD_ROLE_DATA_REQUEST));
	LOG_PROCESS_ERROR(piPackage);

	pLoadRoleData = (R2M_LOAD_ROLE_DATA_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pLoadRoleData);

	pLoadRoleData->wProtocolID  = r2m_load_role_data_request;
	pLoadRoleData->dwPlayerID   = dwRoleID;

	bRetCode = SendGlobal(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

void BattleZone::OnLoadRoleDataRequest(INT nConnIndex)
{
	S2R_LOAD_ROLE_DATA_REQUEST* pLoadRole = NULL; /* = (S2R_LOAD_ROLE_DATA_REQUEST*)pbyData;*/

	g_pGameCenter->m_RoleDB.DoLoadRoleData(nConnIndex, pLoadRole->dwPlayerID);
}	

BOOL BattleZone::LoadRoleData(int nSourceServerId, DWORD dwRoleID, BOOL bSuccess, BYTE* pbyData, size_t uDataLen)
{
	BOOL                bResult     = false;
	BOOL                bRetCode    = false;
	IMemBlock*         piPackage   = NULL;
	R2M_SYNC_ROLE_DATA* pSyncData   = NULL;

	piPackage = QCreateMemBlock((unsigned)(sizeof(R2M_SYNC_ROLE_DATA) + uDataLen));
	LOG_PROCESS_ERROR(piPackage);

	pSyncData = (R2M_SYNC_ROLE_DATA*)piPackage->GetData();
	LOG_PROCESS_ERROR(pSyncData);

	pSyncData->wProtocolID  = r2s_sync_role_data;
	pSyncData->dwRoleID     = dwRoleID;
	pSyncData->bSuccess		= bSuccess;
	pSyncData->dwPacketIdentity = nSourceServerId;

	if (pbyData)
	{
		memcpy(pSyncData->byData, pbyData, uDataLen);
	}

	bRetCode = SendGlobal(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

void BattleZone::OnLoadRoleData(int nConnIndex, void* pData, size_t uDataLen)
{
	BOOL                bResult     = false;
	BOOL                bRetCode    = false;
	IMemBlock*         piPackage   = NULL;
	R2M_SYNC_ROLE_DATA* pSyncData   = (R2M_SYNC_ROLE_DATA*)pData;
	KRole*				pRole		= NULL;

	pRole = g_pGameCenter->m_RoleManager.GetRole(pSyncData->dwRoleID);
	LOG_PROCESS_ERROR(pRole);

	bRetCode = g_pGameCenter->m_GameServer.LoadRoleData(
		pRole->m_dwPlayerID, pSyncData->dwRoleID, pSyncData->bSuccess, pSyncData->byData, uDataLen - sizeof(*pSyncData)
		);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;
EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}
