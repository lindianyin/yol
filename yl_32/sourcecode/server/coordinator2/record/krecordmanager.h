#pragma once
#include "config/kplayerbasedbdef.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"

// 最多保留20条记录
#define MAX_RECORD_NUM 20

#define MAX_BANK_RECORD_NUM (30)

class KRecordManager
{
public:
	KRecordManager(void);
	~KRecordManager(void);

	BOOL Init();
	void UnInit();

private:
	DWORD m_dwKinBattleWinnerId;

	// 家族战记录
	typedef std::vector<KKinBattleRecord> KIN_BATTLE_RECORD_LIST;
	KIN_BATTLE_RECORD_LIST m_vecKinBattleRecords;

	// 最近一次赢的家族记录
	KKinBattleRecord m_sLastWinKinInfo;

	//钱庄投资记录
	std::vector<KBankInvestRecord> m_vecBankInvestRecords;

	//全服限购商品购买记录
	std::vector<KBuyAllPurchaseRecord> m_vecBuyAllPurchaseRecords;

	// 开服时间
	DWORD m_dwServerStartTime;
	
	//开服数据
	DWORD m_openServerData[7];

	//
	//DWORD m_loop;

	//DWORD m_time;

	//struct tm * mp_timeNow;

public:
	VOID SaveAll();

	BOOL OnLoadRecordFromDB(INT nRecordId, BYTE* pData, size_t uDataLen);

	BOOL AddKinBattleRecord(DWORD dwRoleId);

	BOOL SetLastWinKinInfo(DWORD dwRoleId);

	BOOL OnGetServerRecordRequest(INT nConnIdx, INT nRecordId);

	VOID SetKinBattleWinnerId(DWORD dwWinnerId) { m_dwKinBattleWinnerId = dwWinnerId; }

	VOID SetServerStartTime();

	BOOL SetBankInvestRecord(INT nCoins, DWORD dwRoleId);

	BOOL SetBuyAllPurchaseRequest(S2R_BUY_ALL_PURCHASE_REQUEST &sRequest);


	BOOL OnGetBankInvestRecordBroadcast();

	BOOL OnBuyAllPurchaseRecordBroadcast();

	BOOL OnBuyAllPurchaseRecordRespond(INT nConnIndex);

	BOOL OnAllPurchaseRespond(INT nConnIndex, INT nShopId, INT nCategoryId, INT nGoodsId, INT nBuycount, DWORD dwRoleId);

	
	DWORD GetOpenServerTime();
	DWORD * GetOpenServerData();
private:
	BOOL LoadRecords();

	VOID SaveKinBattleRecord();

	VOID SaveKinBattleWinnerId();

	VOID SaveServerStartTime();

	VOID SaveOpenServerData();
	

	VOID SaveLastWinKinInfo();

	VOID SaveBankInvestRecord();

	VOID SaveBuyAllPurcharseRecord();

	BOOL LoadKinBattleRecord(BYTE* pData, size_t uDataLen);

	BOOL LoadKinBattleWinnerId(BYTE* pData, size_t uDataLen);

	BOOL LoadServerStartTime(BYTE* pData, size_t uDataLen);
	
	BOOL LoadOpenServerData(BYTE* pData, size_t uDataLen);

	BOOL LoadLastWinKinInfo(BYTE* pData, size_t uDataLen);

	BOOL LoadBankInvestRecord(BYTE* pData, size_t uDataLen);

	BOOL LoadBuyAllPurchaseRecord(BYTE* pData, size_t uDataLen);

	BOOL OnGetKinBattleRecordRespond(INT nConnIndex);

	BOOL OnGetKinBattleWinnerIdRespond(INT nConnIndex);

	BOOL OnGetServerStartTimeRespond(INT nConnIndex);

	BOOL OnGetLastWinKinInfoRespond(INT nConnIndex);

public:
	BOOL ResetBuyPurchases();
	
};
