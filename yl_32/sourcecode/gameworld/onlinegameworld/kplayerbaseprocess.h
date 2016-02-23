
#pragma once

#ifdef GAME_SERVER

#include "onlinegameworld/kbaseprotocol.h"

class KPlayer;
class KCharacter;
class KSceneObject;
class ScriptSafe;

// -------------------------------------------------------------------------
class KPlayerBaseProcess
{
public:
	KPlayerBaseProcess();
	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

	// -------------------------------Do Protocol Sending Begin-------------------------------
	BOOL DoSyncJump(KSceneObject* pObj, BOOL bSyncSelf = FALSE);
	BOOL DoSyncMove(KSceneObject* pObj, BOOL bSyncSelf = FALSE);
	BOOL DoSyncJumpTo( KSceneObject* pObj, INT nX, INT nY, INT nZ);
	BOOL DoSyncFaceDirection(KSceneObject* pObj, INT nFaceDirection, BOOL bSyncSelf = FALSE);
	BOOL DoMessageNotify(KPlayer* pPlayer, INT nModule, INT nMsgId, const PVOID pAdditional = NULL, UINT uDataLen = 0);
	BOOL DoPickUpDoodad(KPlayer* pPlayer, DWORD dwDoodadId, DWORD bOnScene = TRUE);

	BOOL DoAdjustPlayerMove(KPlayer* pPlayer);
	BOOL DoSyncFrame(KPlayer* pPlayer);
	BOOL DoSyncTeleport(KSceneObject* pObj, INT nX, INT nY, INT nZ);
	BOOL DoSyncLeave(KSceneObject& rSceneObj, BOOL bSyncSelf = FALSE);
	BOOL DoSyncLeave(KPlayer* pPlayer, DWORD dwCharacterId);

	BOOL DoSyncVisible(KPlayer* pPlayer, DWORD dwCharacterId, BOOL bVisible); // 隐身时发给己方的
	BOOL DoSyncHeadText(KPlayer* pPlayer, DWORD dwCharacterId, INT nType, INT nDamage);
	BOOL BroadcastHeadText( KSceneObject& rSceneObj, DWORD dwCharacterId, INT nType, INT nNumber);
	BOOL DoSyncBlackMsg(KPlayer* pPlayer, LPCSTR pszMessage);
	BOOL DoSyncDegree(KPlayer* pPlayer, LPCSTR pszModuleName, INT nCurDegree, INT nMaxDegree);
	BOOL DoSwitchGameServer(KPlayer* pPlayer, GUID& Guid, DWORD dwIPAddr, WORD wPort);
	BOOL DoSwitchBZoneGameServer( KPlayer* pPlayer, DWORD dwDstPlayerID, GUID& Guid, DWORD dwIPAddr, WORD wPort);
	BOOL DoBZoneReturnGameServer( KPlayer* pPlayer, DWORD dwDstPlayerID, GUID& Guid, DWORD dwIPAddr, WORD wPort );
	// -------------------------------Do Protocol Sending End---------------------------------

	LPCSTR GetScriptFunStat(std::string& rstrBuffer);
	void S2CFunStat( QLuaScript& cScript, UINT uDataLen );
private:
	BOOL ProcessBeginning(KPlayer* pPlayer, LPCBYTE pData, UINT uDataLen); // 玩家登录状态
	BOOL ProcessPlaying(KPlayer* pPlayer, LPCBYTE pData, UINT uDataLen); // 玩家游戏状态
	BOOL ProcessExchanging(KPlayer* pPlayer, LPCBYTE pData, UINT uDataLen); // 玩家跨服状态

	// -------------------------------On Receiving Protocols Begin----------------------------
	BOOL PtcSyncEnd(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);

	BOOL OnPing(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnMove(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnMoveCtrl(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnRequestOffline(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnJump(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnDialogDoodad(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnCallServerScript(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen );

	void C2SFunStat( ScriptSafe& cSafeScript, UINT uDataLen );

	//NPC会话
	BOOL OnNpcTalk( KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen );
	BOOL OnApplySyncNeighbor(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnApplySetVar(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnApplyRevive(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnDialogSelect(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnPickUpDoodad(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);


	BOOL OnGetDelayDataRequest(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnPlayActionRequest(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);


	BOOL OnPlayProcessDailyTask(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);

	
	// -------------------------------On Receiving Protocols End------------------------------

private:
	// 协议处理函数数组
	BOOL (KPlayerBaseProcess::*m_aProcessFunc[emKPLBASE_PTC_C2S_MAX])(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	BOOL OnGetDailyTaskList(KPlayer* pPlayer, LPCBYTE pbData, UINT uDataLen);
	INT  GetDtaskRandLevel();


	struct KSCRIPT_FUN_STAT
	{
		UINT uTimes; // 次数
		UINT uBytes; // 包大小

		KSCRIPT_FUN_STAT() { uTimes = 0; uBytes = 0; }
	};
	std::map<std::string, KSCRIPT_FUN_STAT> m_mapC2SCallStatics; // c2s函数被调次数/流量统计
	std::map<std::string, KSCRIPT_FUN_STAT> m_mapS2CCallStatics; // CallClientScript次数/流量统计
};
extern KPlayerBaseProcess g_cPlayerBaseProcess;

// -------------------------------------------------------------------------
#endif
