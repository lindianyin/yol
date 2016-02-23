#pragma once

#include "rankdef.h"

class RankRelayClient
{
public:
	RankRelayClient(void);
	~RankRelayClient(void);

	static void OnGetRankIdxRespond(BYTE* pbyData, size_t uDataLen);
	static void OnGetRankIdRespond(BYTE* pbyData, size_t uDataLen);
	static void OnSwapRankRespond(BYTE* pbyData, size_t uDataLen);


	static BOOL DoGetRankIdxRequest(INT nRankId, DWORD dwSrcId, DWORD dwId);
	static BOOL DoGetRankIdRequest(INT nRankId, DWORD dwSrcId, INT nRankIdx);
	static BOOL DoSwapRankRequest(INT nRankId, DWORD dwSrcId, DWORD dwDstId);
	static BOOL DoModifyRankValueRequest(INT nRankId, DWORD dwId, INT nValue, DWORD dwModifyTime = 0, BOOL bIfRoll = TRUE);

	//RankRelayClient::DoGetRankIdRequest(emRANK_ARENA, 1);

	static inline BOOL Send(IMemBlock* piBuffer);
};
