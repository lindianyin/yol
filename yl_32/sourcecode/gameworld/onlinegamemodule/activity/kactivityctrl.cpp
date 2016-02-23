#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "kactivityctrl.h"
#include "kactivitymodule.h"
#include "record/RecordRelayClient.h"
#include "serverbase/RecordDef.h"
#include "kactivitymanager.h"

KActivityCtrl::KActivityCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KActivityCtrl::~KActivityCtrl()
{

}

BOOL KActivityCtrl::Init()
{
	ResetActivityInfo();

	return TRUE;
}

BOOL KActivityCtrl::ResetActivityInfo()
{
	memset(&sActivityInfo, 0, sizeof(sActivityInfo));

	return TRUE;
}

BOOL KActivityCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	memcpy(pBuffer, &sActivityInfo, sizeof(sActivityInfo));
	uUsedSize = sizeof(sActivityInfo);

	return TRUE;
}

BOOL KActivityCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bResult = FALSE;

	memcpy(&sActivityInfo, pData, sizeof(sActivityInfo));

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityCtrl::SyncActivityTime(INT nActivityIdx, INT nBaseActivityIdx, DWORD dwOpenTime, DWORD dwCloseTime)
{
	BOOL									bResult		= FALSE;
	BOOL									bRetCode	= FALSE;
	KS2C_SYNC_ACTIVITY_TIME					sSync;

	sSync.nActivityIdx = nActivityIdx;
	sSync.nBaseActivityIdx = nBaseActivityIdx;

	sSync.dwOpenTime = dwOpenTime;
	sSync.dwCloseTime = dwCloseTime;

	bRetCode = g_cActivityModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityCtrl::SyncKinBattleRecord()
{
	INT										nIdx		= 0;
	IMemBlock*								piMem		= NULL;
	BOOL									bResult		= FALSE;
	BOOL									bRetCode	= FALSE;
	KS2C_SYNC_KINBATTLE_RECORD*				pSync		= NULL;
	size_t									uPakSize	= 0;
	std::vector<KKinBattleRecord>::iterator	it;
	INT										nRecordsNum = g_cActivityManager.GetKinBattleRecordsNum();

	PROCESS_ERROR(nRecordsNum > 0);

	uPakSize = sizeof(KS2C_SYNC_KINBATTLE_RECORD) + nRecordsNum * sizeof(KKinBattleRecord);
	piMem = QCreateMemBlock(uPakSize);
	LOG_PROCESS_ERROR(piMem);
	pSync = (KS2C_SYNC_KINBATTLE_RECORD*)piMem->GetData();

	pSync->byProtocol = s2c_sync_kinbattle_records;
	pSync->nRecordNums = (INT)nRecordsNum;
	for(INT i = 0; i < nRecordsNum; ++i)
	{
		KKinBattleRecord* pRecord = g_cActivityManager.GetKinBattleRecord(i);
		LOG_PROCESS_ERROR(pRecord);
		memcpy((VOID*)&(pSync->arrRecords[nIdx++]), pRecord, sizeof(KKinBattleRecord));
	}

	bRetCode = g_cActivityModule.SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, uPakSize);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL KActivityCtrl::SyncKinBattleWinnerId()
{
	BOOL									bResult		= FALSE;
	BOOL									bRetCode	= FALSE;
	KS2C_SYNC_KINBATTLE_WINNERID			sSync;

	sSync.dwWinnerId = g_cActivityManager.GetKinBattleWinnerId();

	bRetCode = g_cActivityModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityCtrl::SyncLastWinKinInfo()
{

	BOOL									bResult		= FALSE;
	BOOL									bRetCode	= FALSE;
	KS2C_SYNC_LAST_WINKIN_INFO				sSync;

	memcpy(&sSync.sRecord, &g_cActivityManager.m_sLastWinKinInfo, sizeof(KKinBattleRecord));

	bRetCode = g_cActivityModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityCtrl::OnLogin()
{
	BOOL bResult = FALSE;

	SyncLastWinKinInfo();

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KActivityCtrl::SyncKinBattleJoinNum(INT nJoinNum, INT nHasJoined)
{
	BOOL									bResult		= FALSE;
	BOOL									bRetCode	= FALSE;
	KS2C_SYNC_KINBATTLE_JOIN_NUM			sSync;

	sSync.nJoinNum = nJoinNum;
	sSync.nHasJoined = nHasJoined;

	bRetCode = g_cActivityModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}