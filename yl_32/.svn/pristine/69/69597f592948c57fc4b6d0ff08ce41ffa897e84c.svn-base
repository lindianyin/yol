#pragma once

class KEscortRelayClient
{
public:
	KEscortRelayClient(void);
	~KEscortRelayClient(void);

	static BOOL DoStartEscortRequest(INT nPlayerId, INT nProtectRoleId, INT nEscortLevel);

	static BOOL DoRefreshPlayerEscorts(INT nPlayerId);

	static BOOL DoTakeEscortAwardRequest(INT nPlayerId);

	static BOOL DoRobPlayerEscortRequest(INT nPlayerId, INT nRobPlayerId);

	static BOOL DoDelRobEscortRequest(INT nPlayerId, INT nRobPlayerId);

	static BOOL DoTakeRobAwardRequest(INT nPlayerId, INT nRobPlayerId, BYTE byEscortLevel);

	static BOOL DoRemoveEscortWatchRequest(INT nPlayerId);

	static BOOL DoSpeedEscortRequest(INT nPlayerId, BOOL bSpeedUp);

	static BOOL DoRescuePlayerRequest(INT nPlayerId, INT nRescuePlayerId);


	static VOID OnSyncEscortMembers(BYTE* pbyData, size_t uDataLen);

	static VOID OnUpdateEscortState(BYTE* pbyData, size_t uDataLen);

	static VOID OnSyncRobEscortMembers(BYTE* pbyData, size_t uDataLen);

	static VOID OnSyncEscortState(BYTE* pbyData, size_t uDataLen);

	static VOID OnTakeEscortAwardRespond(BYTE* pbyData, size_t uDataLen);

	static VOID OnRescuePlayerRespond(BYTE* pbyData, size_t uDataLen);

	static VOID OnTakeRobAwardRespond(BYTE* pbyData, size_t uDataLen);


	static inline BOOL Send(IMemBlock* piBuffer);
};
