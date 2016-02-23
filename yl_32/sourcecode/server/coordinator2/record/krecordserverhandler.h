
#pragma once

#include "serverbase/protocol/Relay_GS_Protocol.h"

class KRecordHandler
{
public:
	static VOID OnGetServerRecordRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnAddKinBattleRecordRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnSetKinBattleWinnerIdRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnSetLastWinKinRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnSetBankInvestRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnSetBuyAllPurcaseRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static BOOL DoGetRecordRespond(INT nConnIndex, INT nRecordId, BYTE* pRecordData, size_t uRecordDataLen);

	static BOOL DoGetRecordBroadcast(INT nRecordId, BYTE* pRecordData, size_t uRecordDataLen);

	static BOOL DoGetAllPurchaseRespond(INT nConnIndex, INT nShopId, INT nCategoryId, INT nGoodsId, INT nCount,INT nBuycount, DWORD dwRoleId);

private:
	static inline BOOL Send(int nConnIndex, IMemBlock* piBuffer);

	static inline BOOL Broadcast( IMemBlock* piBuffer );
};
