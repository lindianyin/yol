#pragma once

#include "stdafx.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"

class RecordRelayClient
{
public:
	RecordRelayClient(void);
	~RecordRelayClient(void);

public:
	static BOOL DoGetServerRecordRequest(INT nRecordId);

	static BOOL DoAddKinBattleRecordRequest(DWORD dwRoleId);

	static BOOL DoSetKinBattleWinnerIdRequest(DWORD dwWinnerId);

	static BOOL DoSetLastWinKinRequest(DWORD dwWinnerId);

	static BOOL DoSetBankInvestRequest(INT nCoins, DWORD dwRoleId);

	static BOOL DoBuyAllPurchaseRequest( S2R_BUY_ALL_PURCHASE_REQUEST &sRequest);

	static VOID OnGetServerRecordRespond(BYTE* pbyData, size_t uDataLen);

	static VOID OnGetAllPurchaseRespond(BYTE* pbyData, size_t uDataLen);

private:
	static inline BOOL Send(IMemBlock* piBuffer);
};
