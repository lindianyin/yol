#ifndef __KACTIVITYMANAGER_H__
#define __KACTIVITYMANAGER_H__

#include "stdafx.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "activitydef.h"
#include "config/kplayerbasedbdef.h"
#include "onlinegameworld/knpc.h"

#define KIN_BATTLE_WINNER_NPC 99998

class KActivityManager
{
public:
	KActivityManager();
	~KActivityManager();

public:
	BOOL Init();
	BOOL UnInit();

public:
	VOID LoadKinBattleInfo();

	BOOL OnGetKinBattleWinnerIdRespond(BYTE* pRecordData, size_t uRecordDataLen);

	BOOL OnGetLastWinKinInfoRespond(BYTE* pRecordData, size_t uRecordDataLen);

	BOOL OnGetKinBattleRecordRespond(BYTE* pRecordData, size_t uRecordDataLen);

	BOOL OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId);

	DWORD GetKinBattleWinnerId() { return m_dwKinBattleWinnerId; }

	VOID SetKinBattleWinNpcId(DWORD dwNpcId) { m_dwKinBattleKingNpcId = dwNpcId; }

private:
	BOOL InitKinBattleWinnerNpc();

	BOOL InitLastWinKinInfo();

	BOOL InitKinBattleRecords();

	BOOL SetKinBattleNpcWithDelayData(KNpc* pNpc, KDelayRoleData* pData);

private:
	DWORD m_dwKinBattleWinnerId;
	DWORD m_dwKinBattleKingNpcId;			// 三界神皇npcid

public:
	// 三界争霸榜单
	std::vector<KKinBattleRecord> m_vecKinBattleRecord;

	KKinBattleRecord m_sLastWinKinInfo;		// 最近一次赢得三界争霸家族信息

public:
	INT GetKinBattleRecordsNum() { return m_vecKinBattleRecord.size(); }

	KKinBattleRecord* GetKinBattleRecord(INT nIdx);
};

extern KActivityManager g_cActivityManager;

#endif