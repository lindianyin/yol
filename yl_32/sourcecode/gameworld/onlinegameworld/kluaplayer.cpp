/* -------------------------------------------------------------------------
//	文件名		：	KLuaPlayer.cpp
//	创建者		：	simon
//	创建时间	：	2009/11/21 18:29:17
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/kluadoodad.h"
#include "onlinegameworld/kplayerclient.h"
#include "onlinegameworld/kdoodad.h"
#include "onlinegameworld/scene_def.h"
#include "../onlinegameworld/KRelayClient.h"

#include "onlinegamemodule/chat/kchatmodule.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegamemodule/kprogressbar.h"
#include "../gameworld/onlinegamemodule/qteamgchandler.h"
#include "../gameworld/onlinegamemodule/qteamserver.h"
#include "onlinegamemodule/qplayerteam.h"
#include "../onlinegamemodule/meditation/kmeditationctrl.h"
#include "../onlinegamemodule/bladestate/kbladectrl.h"

//////////////////////////////////////////////////////////////////////////
//TODO HWD 暂时把组队相关接口放在这里，待移走
#ifdef GAME_SERVER
#include "onlinegameworld/kplayermgr.h"
#else
#include "../onlinegamemodule/kplayerteammanager_client.h"

// pk相关，暂时放这里
#include "onlinegameworld/kplayermanager_client.h"
#include "kplayerbaseprocess_client.h"
#endif
//////////////////////////////////////////////////////////////////////////

#include "onlinegameworld/kbaseprotocol.h"
#include "kplayerbaseprocess.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------


KPlayer* KLuaPlayer::GetHim(QLuaScript& sc, INT nIndex)
{
	KLuaPlayer* pLuaPlayer = dynamic_cast<KLuaPlayer*>(GetCObj(sc.GetLuaState(), nIndex));
	if (pLuaPlayer)
		return pLuaPlayer->m_pMe;

	return NULL;
}

BOOL KLuaPlayer::ClearTempTable()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	return ClearScriptTempTable(cSafeScript->GetLuaState(), s_szClassName, m_pMe->GetId());
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: Id
// @Description		: 
// @Property(R/RW)	: R
// @PropertyType	: DWORD
// @LuaMarkEnd
// -------------------------------------------------------------------------
DWORD KLuaPlayer::getId(VOID) CONST
{
	return m_pMe->m_dwId;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: SetPosition
// @Description		: 设置逻辑坐标(瞬移应该调用TeleportTo，这个只准调试用，否则会造成同步问题)
// @ReturnCode		: 
// @ArgumentFlag	: ddd
// @ArgumentComment	: x
// @ArgumentComment	: y
// @ArgumentComment	: z
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaSetPosition(QLuaScript& sc)
{
	INT nX = sc.GetInt(1), nY = sc.GetInt(2), nZ = sc.GetInt(3);

	BOOL bRet = m_pMe->SetPosition(nX, nY, nZ);

	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: SysMsg
// @Description		: 向玩家发送一系统消息
// @ReturnCode		: 
// @ArgumentFlag	: s
// @ArgumentComment	: 消息内容
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSysMsg(QLuaScript& sc)
{
	KE_SYSTEMMSG_TYPE nType = emKSYSTEMMSG_NORMAL;
	if (sc.GetTopIndex() >= 2)
		nType = (KE_SYSTEMMSG_TYPE)sc.GetInt(2);

	g_ChatChannelManager.ProcessRespond(trLocalSys, nType, m_pMe->GetId(), "", sc.GetStr(1), emQCHAT_PANEL);

	return 0;
}

#ifdef GAME_SERVER
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: TeleportTo
// @Description		: 瞬移(传送)
// @ReturnCode		: 
// @ArgumentFlag	: dddd[dd]
// @ArgumentComment	: MapId
// @ArgumentComment	: x
// @ArgumentComment	: y
// @ArgumentComment	: z
// @ArgumentComment	: bOnThisGs(是否在本服加载地图)
// @ArgumentComment	: nMapIndex
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaTeleportTo(QLuaScript& sc)
{
	INT nMapId = sc.GetInt(1);
	INT nMapCopyIndex = 0;
	INT nX = 0, nY = 0, nZ = 0;
	INT nTeleportRightNow = 0;
	BOOL bOnThisGs = FALSE;

	nX = sc.GetInt(2);
	nY = sc.GetInt(3);
	nZ = sc.GetInt(4);

	nMapCopyIndex |= GET_MAP_COPY_INDEX(nMapId);
	nMapId = GET_MAP_ID(nMapId);

	if(sc.GetTopIndex() >= 5)
	{
		bOnThisGs = sc.GetInt(5);
	}

	if(sc.GetTopIndex() >= 6)
	{
		nMapCopyIndex = sc.GetInt(6);
	}

	if (sc.GetTopIndex() >= 7)
	{
		nTeleportRightNow = sc.GetInt(7); // 是否马上传送
	}

	m_pMe->m_cPlayerServer.m_bOnThisGs = bOnThisGs;

	BOOL bRet = FALSE;
	
	if (nTeleportRightNow == 0)
	{
        m_pMe->m_cPlayerServer.PrepareTeleportTo(nMapId, nMapCopyIndex, nX, nY, nZ);
	}
	else
	{
        m_pMe->m_cPlayerServer.TeleportTo2(nMapId, nMapCopyIndex, nX, nY, nZ);
	}

	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: SetPositionForSaving
// @Description		: 设置保存位置
// @ReturnCode		: 
// @ArgumentFlag	: dddd
// @ArgumentComment	: MapId 
// @ArgumentComment	: x
// @ArgumentComment	: y
// @ArgumentComment	: z
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaSetPositionForSaving(QLuaScript& sc)
{
	INT nMapId = sc.GetInt(1);

	INT nX = 0, nY = 0, nZ = 0;
	nX = sc.GetInt(2);
	nY = sc.GetInt(3);
	nZ = sc.GetInt(4);

	BOOL bRet = m_pMe->m_cPlayerServer.SetPositionForSaving((DWORD)nMapId, nX, nY, nZ);

	sc.PushNumber(bRet);

	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaCharacter
// @LuaApiName		: IsBZonePlayer
// @Description		: 是否是跨服过来的玩家
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaIsBZonePlayer(QLuaScript& sc)
{
	INT nX = 0, nY = 0, nZ = 0;
	DWORD dwId  = m_pMe->GetId();
	if (IS_BZONE_PLAYER(dwId))
	{
		sc.PushNumber(1);
	}
	else
	{
		sc.PushNumber(0);
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: BattleZoneTransfer
// @Description		: 传送到战场服务器
// @ReturnCode		: 
// @ArgumentFlag	: [ddd]
// @ArgumentComment	: nPosX
// @ArgumentComment	: nPosY
// @ArgumentComment	: nPosZ
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaBattleZoneTransfer(QLuaScript& sc)
{
	INT nPosX = 0, nPosY = 0, nPosZ = 0;
	INT nMapId = 0;
	INT nTopIndex = sc.GetTopIndex();

	if (nTopIndex >= 3)
	{
		nPosX = sc.GetInt(1);
		nPosY = sc.GetInt(2);
		nPosZ = sc.GetInt(3);
	}
	else
	{
		m_pMe->GetPosition(nPosX, nPosY, nPosZ);
	}

	BOOL bRet = m_pMe->m_cPlayerServer.TransferToBattleZone(nPosX, nPosY, nPosZ);

	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: DoMultiTransferRequest
// @Description		: 多人副本跨服传送
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaDoMultiTransferRequest(QLuaScript& sc)
{
	BOOL bRet = m_pMe->m_cPlayerServer.DoMultiTransferRequest();

	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: DoGetMultiAwardRequest
// @Description		: 跨服回来后请求奖励信息
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaDoGetMultiAwardRequest(QLuaScript& sc)
{
	g_RelayClient.DoGetMultiAwardRequest(m_pMe);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: DoGetCrossSvExDataRequest
// @Description		: 跨服回来后请求额外数据
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaDoGetCrossSvExDataRequest(QLuaScript& sc)
{
	g_RelayClient.DoGetCrossSvExDataRequest(m_pMe);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: BattleZoneReturn
// @Description		: 从战场传送回原服务器
// @ReturnCode		: 
// @ArgumentFlag	: [dddd]
// @ArgumentComment	: nMapId
// @ArgumentComment	: nPosX
// @ArgumentComment	: nPosY
// @ArgumentComment	: nPosZ
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaBattleZoneReturn(QLuaScript& sc)
{
	BOOL bRet = FALSE;
	INT nTopIndex = sc.GetTopIndex();
	INT nMapId = 0;
	INT nPosX = 0;
	INT nPosY = 0;
	INT nPosZ = 0;

	if (nTopIndex >= 3)
	{
		nMapId = sc.GetInt(1);
		nPosX = sc.GetInt(2);
		nPosY = sc.GetInt(3);
		nPosZ = sc.GetInt(4);
	}
	else
	{
		nMapId = GET_MAP_ID(m_pMe->GetSceneId());
		m_pMe->GetPosition(nPosX, nPosY, nPosZ);
	}
	

	QCONFIRM_RET_FALSE(IS_BZONE_PLAYER(m_pMe->GetId()));

	m_pMe->SetGameState(gsWaitForTransmissionGuid);

	bRet = g_RelayClient.DoBZoneReturnRequest(m_pMe, nMapId, nPosX, nPosY, nPosZ);

	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: ReturnToLastServer
// @Description		: 传送回原服务器
// @ReturnCode		: 
// @ArgumentFlag	: [d]
// @ArgumentComment	: nExValue
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT	KLuaPlayer::LuaReturnToLastServer(QLuaScript& sc)
{
	BOOL bRet = FALSE;
	INT nExValue = 0;
	if(sc.GetTopIndex() >= 1)
	{
		nExValue = sc.GetInt(1);
	}

	QCONFIRM_RET_FALSE(IS_BZONE_PLAYER(m_pMe->GetId()));

	m_pMe->SetGameState(gsWaitForTransmissionGuid);

	bRet = g_RelayClient.ReturnToLastServer(m_pMe, nExValue);

	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: Account
// @Description		: 玩家账户
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaPlayer::getAccount() CONST
{
	return m_pMe->m_cPlayerServer.GetAccount();
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: LoginIp
// @Description		: 玩家此次登陆IP
// @Property(R/RW)	: R
// @PropertyType	: STRING
// @LuaMarkEnd
// -------------------------------------------------------------------------
LPCSTR KLuaPlayer::getLoginIp() CONST
{
	return inet_ntoa(*(struct in_addr*)&m_pMe->m_cPlayerServer.m_dwLoginIp);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: CallClientScript
// @Description		: 远程调用客户端脚本
// @ReturnCode		: 发送成功：1，发送失败：nil
// @ArgumentFlag	: {s}
// @ArgumentComment	: szCall:要调用的函数。随后跟着调用参数。
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaCallClientScript(QLuaScript& sc)
{
	INT nParamCount	= 0;
	static BYTE _sbyBuffer[USHRT_MAX];
	KS2C_SCRIPT_REMOTECALL* pData = (KS2C_SCRIPT_REMOTECALL*)_sbyBuffer;
	*pData = KS2C_SCRIPT_REMOTECALL();

	INT nLen = sc.SaveValue2Buffer(pData->arBuffer, sizeof(_sbyBuffer) - sizeof(KS2C_SCRIPT_REMOTECALL), 1);
	LOG_PROCESS_ERROR(nLen > 0);	// 有可能是数据包过大

	g_cPlayerBaseProcess.S2CFunStat(sc, nLen); // 统计

	g_pBaseSender->SendLenData(m_pMe->GetPlayerIndex(), *pData,
		sizeof(KS2C_SCRIPT_REMOTECALL) + nLen);

	sc.PushNumber(1);
	nParamCount++;

EXIT0:
	return nParamCount;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: SendDialog
// @Description		: 发送Npc对话框
// @ReturnCode		: 
// @ArgumentFlag	: s
// @ArgumentComment	: 对话内容及选项
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSendDialog(QLuaScript& sc)
{
	static BYTE _sbyBuffer[USHRT_MAX];
	KS2C_SHOW_DIALOG* pSync = (KS2C_SHOW_DIALOG*)_sbyBuffer;
	*pSync = KS2C_SHOW_DIALOG();

	INT nLen = 0;
	LPCSTR pszString = sc.GetLStr(1, nLen);
	nLen++;

	QCONFIRM_RET_FALSE(nLen < sizeof(_sbyBuffer) - sizeof(*pSync));

	strncpy((CHAR*)pSync->arBuffer, pszString, nLen);
	pSync->arBuffer[nLen - 1] = 0;

	g_pBaseSender->SendLenData(m_pMe->GetPlayerIndex(), *pSync, sizeof(*pSync) + nLen);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: SendTalk
// @Description		: 发送剧情对话
// @ReturnCode		: 
// @ArgumentFlag	: s
// @ArgumentComment	: 对话内容
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSendTalk(QLuaScript& sc)
{
	static BYTE _sbyBuffer[USHRT_MAX];
	KS2C_FACE_TALK* pSync = (KS2C_FACE_TALK*)_sbyBuffer;
	*pSync = KS2C_FACE_TALK();

	INT nLen = 0;
	LPCSTR pszString = sc.GetLStr(1, nLen);
	nLen++;

	QCONFIRM_RET_FALSE(nLen < sizeof(_sbyBuffer) - sizeof(*pSync));

	strncpy((CHAR*)pSync->arBuffer, pszString, nLen);
	pSync->arBuffer[nLen - 1] = 0;

	g_pBaseSender->SendLenData(m_pMe->GetPlayerIndex(), *pSync, sizeof(*pSync) + nLen);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: SendBlackMsg
// @Description		: 发送黑条字幕
// @ReturnCode		: 
// @ArgumentFlag	: s
// @ArgumentComment	: 字幕内容
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSendBlackMsg(QLuaScript& sc)
{
	static BYTE _sbyBuffer[USHRT_MAX];
	KS2C_BLACK_MSG* pSync = (KS2C_BLACK_MSG*)_sbyBuffer;
	*pSync = KS2C_BLACK_MSG();

	INT nLen = 0;
	LPCSTR pszString = sc.GetLStr(1, nLen);
	nLen++;

	QCONFIRM_RET_FALSE(nLen < sizeof(_sbyBuffer) - sizeof(*pSync));

	strncpy((CHAR*)pSync->arBuffer, pszString, nLen);
	pSync->arBuffer[nLen - 1] = 0;

	g_pBaseSender->SendLenData(m_pMe->GetPlayerIndex(), *pSync, sizeof(*pSync) + nLen);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: SyncDegree
// @Description		: 发送次数
// @ReturnCode		: 
// @ArgumentFlag	: sdd
// @ArgumentComment	: 模块名称
// @ArgumentComment	: 当前次数
// @ArgumentComment	: 最大次数
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSyncDegree(QLuaScript& sc)
{
	LPCSTR pszName = sc.GetStr(1);
	INT nCurDegree = sc.GetInt(2);
	INT nMaxDegree = sc.GetInt(3);

	g_cPlayerBaseProcess.DoSyncDegree(m_pMe, pszName, nCurDegree, nMaxDegree);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: SendNoticeMsg
// @Description		: 发送公告字幕
// @ReturnCode		: 
// @ArgumentFlag	: s
// @ArgumentComment	: 字幕内容
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSendNoticeMsg(QLuaScript& sc)
{
	static BYTE _sbyBuffer[USHRT_MAX];
	KS2C_NOTICE_MSG* pSync = (KS2C_NOTICE_MSG*)_sbyBuffer;
	*pSync = KS2C_NOTICE_MSG();

	INT nLen = 0;
	LPCSTR pszString = sc.GetLStr(1, nLen);
	nLen++;

	QCONFIRM_RET_FALSE(nLen < sizeof(_sbyBuffer) - sizeof(*pSync));

	strncpy((CHAR*)pSync->arBuffer, pszString, nLen);
	pSync->arBuffer[nLen - 1] = 0;

	g_pBaseSender->SendLenData(m_pMe->GetPlayerIndex(), *pSync, sizeof(*pSync) + nLen);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: KickOut
// @Description		: 踢玩家下线，需指明原因
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaKickOut(QLuaScript& sc)
{
	INT nReason = emKICK_PLAYER_REASON_SCRIPT;
	if (sc.GetTopIndex() >= 1)
		nReason = sc.GetInt(-1);
	return m_pMe->m_cPlayerServer.KickOut((KEKICK_PLAYER_REASON)nReason);
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: ReturnToLastCity
// @Description		: 传回城市（最近去过的非副本场景）
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaReturnToLastCity(QLuaScript& sc)
{
	m_pMe->m_cPlayerServer.m_bOnThisGs = FALSE;
	INT nRetCode =  m_pMe->m_cPlayerServer.ReturnToLastCity();
	sc.PushNumber(nRetCode);
	return TRUE;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetLastSaveCity
// @Description		: 得到最近去过的城市非副本场景
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetLastSaveCity(QLuaScript& sc)
{
	sc.PushTable();

	INT nSceneId = DYNAMIC_SCENE_ID(m_pMe->m_cPlayerServer.m_LastEntry.dwMapID, m_pMe->m_cPlayerServer.m_LastEntry.nMapCopyIndex);
	sc.PushNumber(nSceneId);
	sc.SetTableField("nMapId");

	sc.PushNumber(m_pMe->m_cPlayerServer.m_LastEntry.nX);
	sc.SetTableField("nX");

	sc.PushNumber(m_pMe->m_cPlayerServer.m_LastEntry.nY);
	sc.SetTableField("nY");

	sc.PushNumber(m_pMe->m_cPlayerServer.m_LastEntry.nZ);
	sc.SetTableField("nZ");

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: IsGm
// @Description		: 是否GM（测试功能）
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaIsGm(QLuaScript& sc)
{
	sc.PushNumber(g_cOrpgWorld.IsGm(m_pMe));
	return TRUE;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: InviteJoinTeam
// @Description		: 邀请玩家加入队伍
// @ReturnCode		: 1 发送成功
// @ArgumentFlag	: ds
// @ArgumentComment	: nPlayerID:  玩家id
// @ArgumentComment	: pszName:  玩家名字
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaInviteJoinTeam(QLuaScript& sc)
{
	INT    nRetCode     = FALSE;
	INT    nParamCount  = 0;
	INT    nPlayerID    = sc.GetInt(1);
	LPCSTR pszName      = sc.GetStr(2);

	ASSERT(pszName);
	LOG_PROCESS_ERROR(pszName[0]);

	nRetCode = QTeamGCHandler::DoInviteJoinTeamRequest(nPlayerID, pszName);

	sc.PushNumber(nRetCode);
	nParamCount++;
EXIT0:
	return nParamCount;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: LeaveTeam
// @Description		: 离开队伍
// @ReturnCode		: 1 发送成功
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaLeaveTeam(QLuaScript& sc)
{
	INT nRetCode = FALSE;
	INT nTeamID = m_pMe->GetTeamId();
	INT nPlayerID = m_pMe->GetId();

	if(!nTeamID)
		return 0;

	nRetCode = QTeamGCHandler::DoTeamDelMemberRequest(nTeamID, nPlayerID, nPlayerID);
	sc.PushNumber(nRetCode);

	return 1;
}

#endif //#ifdef GAME_SERVER

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: IsCaptain
// @Description		: 是否为队长
// @ReturnCode		: 1:是， 0：不是
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaIsCaptain(QLuaScript& sc)
{
	INT nTeamID = m_pMe->GetTeamId();
	INT nIsCaptain	= 0;
	QTeam* pTeam = g_cTeamServer.GetTeam(nTeamID);
	QCONFIRM_RET_NULL(pTeam);
	if(pTeam->dwAuthority[tatLeader] == m_pMe->GetId())
		nIsCaptain = 1;

	sc.PushNumber(nIsCaptain);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetKinId
// @Description		: 获取家族Id
// @ReturnCode		: 返回家族Id
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetKinId(QLuaScript& sc)
{
	sc.PushNumber(m_pMe->GetKinId());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetKinMasterId
// @Description		: 获取家族族长Id
// @ReturnCode		: 返回家族族长Id
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetKinMasterId(QLuaScript& sc)
{
	sc.PushNumber(m_pMe->GetKinMasterId());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: IsKinMaster
// @Description		: 是否本家族族长
// @ReturnCode		: 返回0和1
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaIsKinMaster(QLuaScript& sc)
{
	sc.PushNumber(m_pMe->IsKinMaster());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetGameState
// @Description		: 获取玩家状态
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetGameState(QLuaScript& sc)
{
	sc.PushNumber(m_pMe->GetGameState());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetTeamId
// @Description		: 得到队伍ID
// @ReturnCode		: 返回队伍ID
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetTeamId(QLuaScript& sc)
{
	sc.PushNumber(m_pMe->GetTeamId());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetSysTeamId
// @Description		: 得到系统队伍ID
// @ReturnCode		: 返回系统队伍ID
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetSysTeamId(QLuaScript& sc)
{
	sc.PushNumber(m_pMe->GetSysTeamId());
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: SetTeamId
// @Description		: 设置队伍Id
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: bSystem，由gc传过来
// @ArgumentComment	: nTeamId，由gc传过来
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSetTeamId(QLuaScript& sc)
{
	BOOL bSystem = (sc.GetInt(1) != 0);
	INT nTeamId = sc.GetInt(2);
	m_pMe->SetPlayerTeamId(bSystem, nTeamId);
	return 1;
}




// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: KLuaPlayer
// @LuaApiName		: OnPayFinsh
// @Description		: 完成支付给奖励
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: nType
// @ArgumentComment	: nCoin
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaOnPayFinsh(QLuaScript& sc)
{
	INT nType = sc.GetInt(1);
	INT dwGold = sc.GetInt(2);
	DWORD dwRoleId = m_pMe->GetPlayerId();
	g_RelayClient.OnPayFinish(dwRoleId,nType,dwGold);
	return 0;
}


//////////////////////////////////////////////////////////////////////////

#ifdef GAME_SERVER

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetLastSaveTime
// @Description		: get last save time
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetLastSaveTime(QLuaScript& sc)
{
	time_t tLastSaveTime = m_pMe->m_cPlayerServer.m_timeLastSave;
	sc.PushNumber(tLastSaveTime);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetAccountCoin
// @Description		: Get Account Coin
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetAccountCoin(QLuaScript& sc)
{
	DWORD dwCoin = m_pMe->m_cPlayerServer.GetAccountCoin();
	sc.PushNumber(dwCoin);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetLimitPlayTimeFlag
// @Description		: 防沉迷标志位
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetLimitPlayTimeFlag(QLuaScript& sc)
{
	DWORD dwLimitPlayTimeFlag = m_pMe->m_cPlayerServer.GetLimitPlayTimeFlag();
	sc.PushNumber(dwLimitPlayTimeFlag);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetLimitOnlineSecond
// @Description		: 防沉迷累计在线时间
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetLimitOnlineSecond(QLuaScript& sc)
{
	DWORD dwLimitOnlineSecond = m_pMe->m_cPlayerServer.GetLimitOnlineSecond();
	sc.PushNumber(dwLimitOnlineSecond);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetLimitOfflineSecond
// @Description		: 防沉迷累计离线时间
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetLimitOfflineSecond(QLuaScript& sc)
{
	DWORD dwLimitOfflineSecond = m_pMe->m_cPlayerServer.GetLimitOfflineSecond();
	sc.PushNumber(dwLimitOfflineSecond);
	return 1;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetAwardFlag
// @Description		: 获取防沉迷状态下收益标志
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetAwardFlag(QLuaScript& sc)
{
	DWORD dwAwardLimitFlag = m_pMe->m_cPlayerServer.GetAwardLimitFlag();
	sc.PushNumber(dwAwardLimitFlag);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: SetAwardFlag
// @Description		: 设置防沉迷状态下收益标志
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 收益标志
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSetAwardFlag(QLuaScript& sc)
{
	INT nAwardLimitFlag = sc.GetInt(1);
	m_pMe->m_cPlayerServer.SetAwardLimitFlag(nAwardLimitFlag);
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: CancelProgressBar
// @Description		: 关闭进度条
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaCancelProgressBar(QLuaScript& sc)
{
	KPlayerProgressBar* pPlayerProgress = m_pMe->GetCtrl<KPlayerProgressBar>();
	assert(pPlayerProgress);
	pPlayerProgress->CancelProgressBar();
	return 0;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: StartProgressBar
// @Description		: 开启一个进度条
// @ReturnCode		: --
// @ArgumentFlag	: d
// @ArgumentComment	: 间隔逻辑帧
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaStartProgressBar(QLuaScript& sc)
{
	INT nIntervalFrame	= sc.GetInt(1);

	KPlayerProgressBar* pPlayerProgress = m_pMe->GetCtrl<KPlayerProgressBar>();
	assert(pPlayerProgress);

	BOOL bRet = pPlayerProgress->StartProgressBar(nIntervalFrame);
	sc.PushNumber(bRet);

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: ClientShowHeadText
// @Description		: 客户端冒头顶文字
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaClientShowHeadText(QLuaScript& sc)
{
	INT nCharacterId = sc.GetInt(1);
	INT nType = sc.GetInt(2);
	INT nNumber = sc.GetInt(3);

	g_cPlayerBaseProcess.DoSyncHeadText(m_pMe, (DWORD)nCharacterId, nType, nNumber);

	return 1;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: IsSwim
// @Description		: 是否在泡澡（测试功能）
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaIsSwim(QLuaScript& sc)
{
	sc.PushNumber(m_pMe->IsSwim());
	return TRUE;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: ResetMeditationData
// @Description		: 重置玩家打坐数据
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaResetMeditationData(QLuaScript& sc)
{
	KMeditationCtrl* pMeditationCtrl = m_pMe->GetCtrl<KMeditationCtrl>();
	QCONFIRM_RET_FALSE(pMeditationCtrl);
	pMeditationCtrl->ResetMeditationInfo();
	return TRUE;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: SetTakeMeditationOfflineAwardTime
// @Description		: 设置玩家领取离线打坐经验时间
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSetTakeMeditationOfflineAwardTime(QLuaScript& sc)
{
	INT nTime = sc.GetInt(1);
	KMeditationCtrl* pMeditationCtrl = m_pMe->GetCtrl<KMeditationCtrl>();
	QCONFIRM_RET_FALSE(pMeditationCtrl);
	pMeditationCtrl->SetTakeOfflineAwardTime((DWORD)nTime);
	return TRUE;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetTakeMeditationOfflineAwardTime
// @Description		: 获取玩家领取离线打坐经验时间
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetTakeMeditationOfflineAwardTime(QLuaScript& sc)
{
	KMeditationCtrl* pMeditationCtrl = m_pMe->GetCtrl<KMeditationCtrl>();
	QCONFIRM_RET_FALSE(pMeditationCtrl);
	sc.PushNumber(pMeditationCtrl->GetTakeOfflineAwardTime());
	return TRUE;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetLastMeditationOnlineTime
// @Description		: 获取玩家昨日打坐时间
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetLastMeditationOnlineTime(QLuaScript& sc)
{
	KMeditationCtrl* pMeditationCtrl = m_pMe->GetCtrl<KMeditationCtrl>();
	QCONFIRM_RET_FALSE(pMeditationCtrl);
	sc.PushNumber(pMeditationCtrl->GetLastMeditationOnlineTime());
	return TRUE;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetMeditationOnlineTime
// @Description		: 获取玩家今日在线打坐时间
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetMeditationOnlineTime(QLuaScript& sc)
{
	KMeditationCtrl* pMeditationCtrl = m_pMe->GetCtrl<KMeditationCtrl>();
	QCONFIRM_RET_FALSE(pMeditationCtrl);
	sc.PushNumber(pMeditationCtrl->GetMeditationOnlineTime());
	return TRUE;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: SetTrapInvalid
// @Description		: 设置trap点无效
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: trap点模板id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaSetTrapInvalid(QLuaScript& sc)
{
	INT nTrapTemplateId = sc.GetInt(1);
	m_pMe->SetTrapInvalid(nTrapTemplateId);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: RemoveTrapInvalid
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: trap点模板id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaRemoveTrapInvalid(QLuaScript& sc)
{
	INT nTrapTemplateId = sc.GetInt(1);
	m_pMe->RemoveTrapInvalid(nTrapTemplateId);

	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: IsTrapInvalid
// @Description		: 查看trap点是否失效
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: trap点模板id
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaIsTrapInvalid(QLuaScript& sc)
{
	INT nTrapTemplateId = sc.GetInt(1);
	sc.PushNumber(m_pMe->IsTrapInvalid(nTrapTemplateId));

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: KLuaPlayer
// @LuaApiName		: GetBladeInfo
// @Description		: 获取大神附加属性
// @ReturnCode		: { nLife, nMana, nAttack，nDef，nHit，nMiss，nCrit，nResist }，失败返回nil
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT KLuaPlayer::LuaGetBladeInfo(QLuaScript& sc)
{
	KBladeCtrl* pBladeCtrl = m_pMe->GetCtrl<KBladeCtrl>();
	if (pBladeCtrl == NULL)
		return 0;

	sc.PushTable();
	sc.PushNumber(pBladeCtrl->GetBladeAttr().nLife);
	sc.SetTableField("nLife");
	sc.PushNumber(pBladeCtrl->GetBladeAttr().nCrit);
	sc.SetTableField("nCrit");
	sc.PushNumber(pBladeCtrl->GetBladeAttr().nDef);
	sc.SetTableField("nDef");
	sc.PushNumber(pBladeCtrl->GetBladeAttr().nHit);
	sc.SetTableField("nHit");
	sc.PushNumber(pBladeCtrl->GetBladeAttr().nAttack);
	sc.SetTableField("nAttack");
	sc.PushNumber(pBladeCtrl->GetBladeAttr().nMana);
	sc.SetTableField("nMana");
	sc.PushNumber(pBladeCtrl->GetBladeAttr().nMiss);
	sc.SetTableField("nMiss");
	sc.PushNumber(pBladeCtrl->GetBladeAttr().nResist);
	sc.SetTableField("nResist");
	return	1;
}

#endif

#include "kluaplayer_table.hpp"
