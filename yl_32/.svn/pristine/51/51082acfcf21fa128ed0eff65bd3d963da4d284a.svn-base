
#ifdef GAME_SERVER
#include "stdafx.h"
#include "onlinegameworld/kgcmodulebase.h"
#include "kmodulemanager.h"
#include "kgamecenterprocess.h"
#include "gclogicbase/kgclogicbaseprotocol.h"
#include "onlinegameworld/kplayer.h"
#include "serverbase/kgcmoduledef.h"
#include "onlinegameworld/korpgworld.h"
#include "gclogicbase/kgcplayercommon.h"
#include "onlinegameworld/kplayermgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KGameCenterProcess g_cGameCenterProcess;

#define KD_PING_GAMECENTER (20 * 1000)

static INT _s_anGcByteVarMapping[256] = {0};
static INT _s_anGcWordVarMapping[256] = {0};
static INT _s_anGcDWordVarMapping[256] = {0};
static BYTE _s_abyBuffer[USHRT_MAX] = {0};
// -------------------------------------------------------------------------

BOOL KGameCenterProcess::Init( INT nMaxConnectId )
{
	m_uLastPing = 0;

	for (INT i = 0; i < countof(_s_anGcByteVarMapping); ++i)
		_s_anGcByteVarMapping[i] = -1;
	for (INT i = 0; i < countof(_s_anGcWordVarMapping); ++i)
		_s_anGcWordVarMapping[i] = -1;
	for (INT i = 0; i < countof(_s_anGcDWordVarMapping); ++i)
		_s_anGcDWordVarMapping[i] = -1;

	//从GS的角色变量到GC角色变量的映射表
	_s_anGcByteVarMapping[emKCHARACTER_TASK_SEX] = emKGCPLAYER_TASK_BYTE_SEX;

	_s_anGcWordVarMapping[emKCHARACTER_TASK_LEVEL] = emKGCPLAYER_TASK_WORD_LEVEL;
	_s_anGcWordVarMapping[emKCHARACTER_TASK_PRIMARY_FACTION] = emKGCPLAYER_TASK_WORD_PRIMARY_FACTION;

	_s_anGcDWordVarMapping[emKCHARACTER_TASK_RANK] = emKGCPLAYER_TASK_DWORD_RANK;

	return TRUE;
}

BOOL KGameCenterProcess::ProcessEvent( INT nConnectId, INT nEventId, LPCVOID pData, INT nData )
{
	return TRUE;
}

BOOL KGameCenterProcess::ProcessPacket( INT nConnectId, LPCBYTE pData, UINT uDataLen )
{
	QCONFIRM_RET_FALSE(pData && uDataLen > 1);
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pData;
	BYTE byModuleId = pHeader->byProtocolFamily;
	KGcModuleBase* pModule = static_cast<KGcModuleBase*>(g_cModuleManager.GetGcModule(byModuleId));
	QCONFIRM_RET_FALSE(pModule);
	return pModule->ProcessGcProtocol(pData, uDataLen);
}

BOOL KGameCenterProcess::Activate()
{
	UINT uNow = KSysService::GetTickCount();
	DoPing(uNow);	// 定时Ping
	return TRUE;
}

BOOL KGameCenterProcess::DoPing( UINT uNow )
{
	if (uNow - m_uLastPing > KD_PING_GAMECENTER)
	{
		KPTC_HEADER_BASE cPing(emKGCBASE_PTC_PING);
		cPing.byProtocolFamily = emKGCMODULE_LOGICBASE;
		//g_piGcConnect->Send(-1, 0, &cPing, sizeof(cPing));
		m_uLastPing = uNow;
	}
	return TRUE;
}

BOOL KGameCenterProcess::OnPlayerSave(KPlayer& rPlayer)
{
	KGCPLAYER_SAVE_TRIGGER sPack;
	sPack.byFamiliy = emKGCMODULE_LOGICBASE;
	sPack.byProtocol = emKGCBASE_PTC_PLAYER_SAVE_TRIGGER;
	sPack.dwRoleId = rPlayer.GetId();

	//g_piGcConnect->Send(-1, 0, &sPack, sizeof(sPack));
	return TRUE;
}

BOOL KGameCenterProcess::OnPlayerVarChange(KPlayer& rPlayer, WORD wVarGroup, WORD wVarId, INT nValue)
{
	INT nVarId = -1;
	if (wVarGroup == emKVARGROUP_CHARACTER_BYTE)
		nVarId = _s_anGcByteVarMapping[wVarId];
	if (wVarGroup == emKVARGROUP_CHARACTER_WORD)
		nVarId = _s_anGcWordVarMapping[wVarId];
	if (wVarGroup == emKVARGROUP_CHARACTER_DWORD)
		nVarId = _s_anGcDWordVarMapping[wVarId];

	if (nVarId >= 0)
	{
		KGCPLAYER_VAR_CHANGE sPack;
		sPack.byFamiliy = emKGCMODULE_LOGICBASE;
		sPack.byProtocol = emKGCBASE_PTC_PLAYER_VAR_CHANGE;
		sPack.dwRoleId = rPlayer.GetId();
		sPack.wGroupId = wVarGroup;
		sPack.wVarId = (WORD)nVarId;
		sPack.nVal = nValue;

		//g_piGcConnect->Send(-1, 0, &sPack, sizeof(sPack));
	}

	return TRUE;
}

BOOL KGameCenterProcess::DoSendServerInfo(INT nConnectId)
{
	KGC_SERVER_INFO sPack;
	sPack.byFamiliy = emKGCMODULE_LOGICBASE;
	sPack.byProtocol = emKGCBASE_PTC_SERVER_INFO;
	sPack.nServerId = g_cOrpgWorld.m_nServerId;

	//g_piGcConnect->Send(nConnectId, 0, &sPack, sizeof(sPack));
	return TRUE;
}

BOOL KGameCenterProcess::DoResyncLoginPlayers( INT nConnectId )
{
	std::vector<KPlayer*> vPlayers;
	g_cPlayerMgr.GetAllPlayers(vPlayers);
	for (std::vector<KPlayer*>::iterator it = vPlayers.begin(); it != vPlayers.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		SyncLoginPlayer(nConnectId, pPlayer);
	}

	return TRUE;
}

void KGameCenterProcess::SyncLoginPlayer(INT nConnectId, KPlayer* pPlayer)
{
	KGCPLAYER_BASE_INFO* pPack = (KGCPLAYER_BASE_INFO*)_s_abyBuffer;
	pPack->byFamiliy = emKGCMODULE_LOGICBASE;
	pPack->byProtocol = emKGCBASE_PTC_PLAYER_LOGIN;

	pPack->dwPlayerId = pPlayer->GetId();
	QStrCpyLen(pPack->szName, pPlayer->GetName(), sizeof(pPack->szName));
	QStrCpyLen(pPack->szAccount, pPlayer->m_cPlayerServer.GetAccount(), sizeof(pPack->szAccount));

	BYTE& rbyByVarNum = pPack->abyVar[0];
	BYTE& rbyWVarNum = pPack->abyVar[1];
	BYTE& rbyDwVarNum = pPack->abyVar[2];
	PBYTE pBuffer = &pPack->abyVar[3];
	rbyByVarNum = 0;
	rbyWVarNum = 0;
	rbyDwVarNum = 0;

	for (INT i = 0; i < countof(_s_anGcByteVarMapping); ++i)
	{
		INT nGcVarId = _s_anGcByteVarMapping[i];
		if (nGcVarId >= 0)
		{
			rbyByVarNum++;
			*pBuffer++ = (BYTE)nGcVarId;
			*pBuffer++ = (BYTE)pPlayer->GetVar(emKVARGROUP_CHARACTER_BYTE, i);
		}
	}
	for (INT i = 0; i < countof(_s_anGcWordVarMapping); ++i)
	{
		INT nGcVarId = _s_anGcWordVarMapping[i];
		if (nGcVarId >= 0)
		{
			rbyWVarNum++;
			*pBuffer++ = (BYTE)nGcVarId;
			*(WORD*)pBuffer = pPlayer->GetVar(emKVARGROUP_CHARACTER_WORD, i);
			pBuffer += sizeof(WORD);
		}
	}
	for (INT i = 0; i < countof(_s_anGcDWordVarMapping); ++i)
	{
		INT nGcVarId = _s_anGcDWordVarMapping[i];
		if (nGcVarId >= 0)
		{
			rbyDwVarNum++;
			*pBuffer++ = (BYTE)nGcVarId;
			*(DWORD*)pBuffer = pPlayer->GetVar(emKVARGROUP_CHARACTER_DWORD, i);
			pBuffer += sizeof(DWORD);
		}
	}
	pPack->dwVarSize = pBuffer - pPack->abyVar;
	//g_piGcConnect->Send(nConnectId, 0, pPack, sizeof(*pPack) + pPack->dwVarSize);
}

// -------------------------------------------------------------------------
#endif // GAME_SERVER
