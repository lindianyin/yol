#include "stdafx.h"
#include "kactivitymanager.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "../record/RecordRelayClient.h"
#include "serverbase/RecordDef.h"
#include "onlinegameworld/korpgworld.h"
#include "../onlinegameworld/KRelayClient.h"
#include "Runtime/TypeDef.h"

KActivityManager g_cActivityManager;

KActivityManager::KActivityManager()
{

}

KActivityManager::~KActivityManager()
{

}

BOOL KActivityManager::Init()
{
	PROCESS_ERROR(g_cOrpgWorld.m_nServerMode != emRUNMODE_MASTER);

	m_dwKinBattleKingNpcId = 0;
	m_dwKinBattleWinnerId = 0;
	memset(&m_sLastWinKinInfo, 0, sizeof(KKinBattleRecord));

	// 初始化三界争霸信息
	LoadKinBattleInfo();

EXIT0:
	return TRUE;
}

BOOL KActivityManager::UnInit()
{
	return TRUE;
}

BOOL KActivityManager::InitKinBattleWinnerNpc()
{
	BOOL bResult = FALSE;

	RecordRelayClient::DoGetServerRecordRequest(emRecordKinBattleWinnerId);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityManager::InitLastWinKinInfo()
{
	BOOL bResult = FALSE;

	RecordRelayClient::DoGetServerRecordRequest(emRecordLastWinKinInfo);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityManager::InitKinBattleRecords()
{
	BOOL bResult = FALSE;

	RecordRelayClient::DoGetServerRecordRequest(emRecordKinBattle);

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KActivityManager::LoadKinBattleInfo()
{
	// 初始化三界神皇雕像
	InitKinBattleWinnerNpc();

	// 初始化最近一次家族战赢家记录
	InitLastWinKinInfo();

	// 初始化三界神皇记录
	InitKinBattleRecords();
}

BOOL KActivityManager::OnGetKinBattleWinnerIdRespond(BYTE* pRecordData, size_t uRecordDataLen)
{
	BOOL		bResult = FALSE;

	LOG_PROCESS_ERROR(uRecordDataLen == sizeof(DWORD));

	memcpy(&m_dwKinBattleWinnerId, pRecordData, sizeof(DWORD));

	if(m_dwKinBattleKingNpcId)
	{
		g_cOrpgWorld.RemoveNpc(m_dwKinBattleKingNpcId);
	}

	if(m_dwKinBattleWinnerId && IS_PLAYER(m_dwKinBattleWinnerId))
	{
		g_RelayClient.DoLoadDelayRoleDataRequest(emMODULE_ACTIVITY, 0, m_dwKinBattleWinnerId);
	}
	else
	{
		// 加载默认的神皇npc（虚位以待）
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunction("KinBattle", "LoadDefaultWinner", 0, "");
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityManager::OnGetLastWinKinInfoRespond(BYTE* pRecordData, size_t uRecordDataLen)
{
	BOOL		bResult = FALSE;

	LOG_PROCESS_ERROR(uRecordDataLen == sizeof(KKinBattleRecord));

	memcpy(&m_sLastWinKinInfo, pRecordData, sizeof(KKinBattleRecord));

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityManager::OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	KDelayRoleData sDelayData;
	KNpc* pNpc = NULL;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	memset(&sDelayData, 0, sizeof(KDelayRoleData));
	LOG_PROCESS_ERROR(piBuffer->GetSize() == sizeof(KDelayRoleData));
	memcpy(&sDelayData, piBuffer->GetData(), piBuffer->GetSize());
	pNpc =  g_cOrpgWorld.AddNpc(KIN_BATTLE_WINNER_NPC);
	LOG_PROCESS_ERROR(pNpc);
	bRetCode = SetKinBattleNpcWithDelayData(pNpc, &sDelayData);
	LOG_PROCESS_ERROR(bRetCode);

	m_dwKinBattleKingNpcId = pNpc->GetId();

	cSafeScript->CallTableFunction("KinBattle", "OnLoadWinnerComplete", 0, "o", pNpc->GetScriptInterface());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityManager::SetKinBattleNpcWithDelayData(KNpc* pNpc, KDelayRoleData* pData)
{
	BOOL bResult = FALSE;
	KDelayBaseData* pBaseData = &(pData->sBaseRoleData);
	LOG_PROCESS_ERROR(pBaseData);

	pNpc->SetSex(pBaseData->bySex);

	pNpc->SetMaxLife(pBaseData->nMaxLife);
	pNpc->SetCurrentLife(pBaseData->nMaxLife);
	//外观
	pNpc->SetHatRepresentId(pBaseData->wHatRepresentId);
	pNpc->SetBodyRepresentId(pBaseData->wBodyRepresentId);
	pNpc->SetLeftWeaponRepresentId(pBaseData->wLeftWeaponRepresentId);
	pNpc->SetLeftShoulderRepresentId(pBaseData->wLeftShoulderRepresentId);
	pNpc->SetRightWeaponRepresentId(pBaseData->wRightWeaponRepresentId);
	pNpc->SetRightShoulerRepresentId(pBaseData->wRightShoulerRepresentId);
	pNpc->SetRideRepresentId(pBaseData->wRideRepresentId);
	pNpc->SetHeadPendantRepresentId(pBaseData->wHeadPendantRepresentId);
	pNpc->SetBottomPendantRepresentId(pBaseData->wBottomPendantRepresentId);
	pNpc->SetBackPendantRepresentId(pBaseData->wBackPendantRepresentId);
	pNpc->SetPetRepresentId(pBaseData->wPetRepresentId);

	pNpc->SetFightScore(pBaseData->dwFightScore);

	CHAR szNewName[1024];
	ZeroMemory(szNewName,sizeof(szNewName));
	sprintf(szNewName, LANG_011, pBaseData->m_szRoleName, pBaseData->m_szTongName);
	pNpc->SetStrName(szNewName);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityManager::OnGetKinBattleRecordRespond(BYTE* pRecordData, size_t uRecordDataLen)
{
	BOOL		bResult = FALSE;
	BYTE*		pBuffer = pRecordData;
	size_t		uPakSize = uRecordDataLen;

	m_vecKinBattleRecord.clear();

	while(uPakSize >= sizeof(KKinBattleRecord))
	{
		KKinBattleRecord sRecord;
		memcpy((VOID*)&sRecord, pBuffer, sizeof(KKinBattleRecord));
		m_vecKinBattleRecord.push_back(sRecord);
		pBuffer += sizeof(KKinBattleRecord);
		uPakSize -= sizeof(KKinBattleRecord);
	}

	LOG_PROCESS_ERROR(uPakSize == 0);

	bResult = TRUE;
EXIT0:
	return bResult;
}

KKinBattleRecord* KActivityManager::GetKinBattleRecord(INT nIdx)
{
	KKinBattleRecord* pRecord = NULL;
	LOG_PROCESS_ERROR(nIdx >= 0 && nIdx < m_vecKinBattleRecord.size());

	pRecord = &m_vecKinBattleRecord[nIdx];

EXIT0:
	return pRecord;
}