
#pragma once

#include "serverbase/protocol/Relay_GS_Protocol.h"

class KEscortHandler
{
public:

	static VOID OnStartEscortRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnRefreshEscortsRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnRemovePlayerFromWatchTable(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnTakeEscortAwardRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnRobPlayerEscortRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnDelRobEscortRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnTakeRobAwardRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnSpeedUpRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	static VOID OnRescuePlayerRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);


	static BOOL DoSyncEscortMembers(INT nConnIndex, INT nPlayerId);

	static BOOL DoUpdateEscortState(INT nConnIndex, INT nSrcId, INT nUpdatePlayerId);

	static BOOL DoSyncRobEscortMembers(INT nConnIndex, INT nPlayerId);

	static BOOL DoSyncEscortState(INT nPlayerId);

	static BOOL DoTakeEscortAwardRespond(INT nPlayerId, BYTE byEscortState, BYTE byEscortLevel, BYTE bySuccess);

	static BOOL DoTakeRobAwardRespond(INT nPlayerId, BYTE byEscortLevel, BYTE bySuccess);

private:
	static inline BOOL Send(int nConnIndex, IMemBlock* piBuffer);

	static inline VOID Broadcast(IMemBlock* piBuffer);
};
