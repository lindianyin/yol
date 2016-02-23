
#pragma once

#ifdef GAME_CLIENT
#include "onlinegameworld/kprotocol.h"
#include "onlinegameworld/kbaseprotocol.h"
// -------------------------------------------------------------------------
class KPlayerBaseProcess
{
public:
	KPlayerBaseProcess();
	BOOL ProcessPacket(LPCBYTE pData, UINT uDataLen);
	// -------------------------------Do Protocol Sending Begin-------------------------------
	BOOL DoSyncEnd();
	BOOL DoPing();
	BOOL DoApplySyncNeighbor(DWORD dwId);
	BOOL DoApplySetVar(DWORD dwGroupId, DWORD dwVarId, DWORD dwValue);
	// -------------------------------Do Protocol Sending End---------------------------------
private:
	// -------------------------------On Receiving Protocols Begin----------------------------
	BOOL PtcPlayerData(LPCBYTE pData, UINT uDataLen);

	BOOL OnPing(LPCBYTE pData, UINT uDataLen); // ping回包
	BOOL OnSyncFrameSignal(LPCBYTE pData, UINT uDataLen);
	BOOL OnChangeServer( LPCBYTE pData, UINT uDataLen );

	BOOL OnSyncSelf( LPCBYTE pData, UINT uDataLen );
	BOOL OnLeaveScene(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncJump(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncMove(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncJumpTo( LPCBYTE pData, UINT uDataLen );
	BOOL OnSyncFaceDirection(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncPlayer(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncNpc( LPCBYTE pData, UINT uDataLen );
	BOOL OnSyncNpcBorn(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncDoodad( LPCBYTE pData, UINT uDataLen );
	BOOL OnSyncDoodadState( LPCBYTE pData, UINT uDataLen );
	BOOL OnSyncDoodadExtra( LPCBYTE pData, UINT uDataLen );
	BOOL OnSyncAdjustMove(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncTeleport(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncVarSelf(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncVarAround(LPCBYTE pData, UINT uDataLen);
	BOOL OnResponseOffline(LPCBYTE pData, UINT uDataLen);
	BOOL OnCallClientCall(LPCBYTE pData, UINT uDataLen);
	BOOL OnSyncVisible(LPCBYTE pData, UINT uDataLen); // 隐身后发给己方
	// -------------------------------On Receiving Protocols End------------------------------

private:
	BOOL (KPlayerBaseProcess::*m_aProcessFunc[emKPLBASE_PTC_S2C_MAX])(LPCBYTE pbData, UINT uDataLen);
};

extern KPlayerBaseProcess g_cPlayerBaseProcess;
// -------------------------------------------------------------------------
#endif  // GAME_CLIENT
