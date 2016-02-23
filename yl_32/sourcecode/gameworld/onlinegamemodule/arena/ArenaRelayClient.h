#pragma once

#include "arenadef.h"

class ArenaRelayClient
{
public:
	ArenaRelayClient(void);
	~ArenaRelayClient(void);

public:
	static BOOL DoLoadRecordRequest(DWORD dwRoleId);
	static VOID DoAddRecordRequest(DWORD dwRoleId, BOOL bAttack, BOOL bFightResult, LPCSTR szName, INT nTargetIdx);

	static VOID OnLoadRecordRespond(BYTE* pbyData, size_t uDataLen);

private:
	static inline BOOL Send(IMemBlock* piBuffer);
};
