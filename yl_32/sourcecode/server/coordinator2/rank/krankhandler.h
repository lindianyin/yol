
#pragma once

#include "serverbase/protocol/Relay_GS_Protocol.h"

class KRankHandler
{
public:
	static VOID OnGetRankIdxRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static VOID OnGetRankIdRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static VOID OnSwapRankRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static VOID OnModifyRankValueRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static BOOL DoGetRankIdxRespond(INT nConnIndex, INT nRankId, DWORD dwSrcId, DWORD dwId, INT nRankIdx);
	static BOOL DoGetRankIdRespond(INT nConnIndex, INT nRankId, DWORD dwSrcId, DWORD dwId, INT nRankIdx, DWORD dwValue = 0, DWORD dwLastModifyTime = 0);
	static BOOL DoSwapRankRespond(INT nConnIndex, INT nRankId, DWORD dwSrcId, INT nSrcRank, DWORD dwDestId, INT nDestRank, BOOL bSucceed);

public:
	static inline BOOL Send(int nConnIndex, IMemBlock* piBuffer);
};
