
#pragma once

#include "serverbase/protocol/Relay_GS_Protocol.h"

class KArenaHandler
{
public:
	static BOOL DoLoadRecordRespond(INT nConnIndex, DWORD dwRoleId, BYTE* pRecordData, size_t uRecordDataLen);

	static VOID OnLoadArenaRecordRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	static VOID OnAddArenaRecordRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

private:
	static inline BOOL Send(int nConnIndex, IMemBlock* piBuffer);
};
