

#ifdef GAME_SERVER

#include "stdafx.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kplayerserver.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kdoodad.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "kvarsetting.h"
#include "kplayerbaseprocess.h"
#include "onlinegameworld/synchelper.h"
#include "config/kplayerbasedbdef.h"
#include "kplayerdbmanager.h"
#include "onlinegameworld/koverloadextension.h"
#include "knetconnectplayer.h"

#include "serverbase/protocol/GatewayRelayProtocol.h"
#include "kgamecenterprocess.h"
#include "KRelayClient.h"
#include "scene/kscene.h"
#include "scene/kscenemgr.h"
#include "../onlinegamemodule/item2/kplayeritem.h"
#include "onlinegamemodule/characterskill.h"
#include "../onlinegamemodule/kin/KinModule.h"
#include "../nsworld/kcharacterhelper.h"
#include "../onlinegamemodule/pet/kpetctrl.h"
#include "../onlinegamemodule/pet/kridectrl.h"
#include "../onlinegamemodule/pet/kpet.h"
#include "Runtime/TypeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KPlayerServer::KPlayerServer( KPlayer& rcPlayer ) :KCharacterServer(rcPlayer), m_rcPlayer(rcPlayer)
{
	m_dwVirtualFrame = 0;
	m_dwRecordCount = 0;

	m_timeLastPing = 0;
	m_timeLastSave = 0;
	m_dwLoginIp = 0;
	m_timeLogin = 0;
	m_nAwardLimitFlag = 0;
	m_dwCreateTime = 0;
	m_bSetSavePosition = FALSE;
}

VOID KPlayerServer::SavePlayerBaseValue(KSAVE_DATA& sSaveData, KPlayer* pPlayer)
{
	sSaveData.nCurrentLife = pPlayer->GetCurrentLife();
	sSaveData.nCurrentMana = pPlayer->GetCurrentMana();
	sSaveData.nCurrentPower = pPlayer->GetCurrentPower();
}

VOID KPlayerServer::LoadPlayerBaseValue(KSAVE_DATA* pSaveData, KPlayer& rcPlayer)
{
	rcPlayer.SetCurrentLife(pSaveData->nCurrentLife);
	rcPlayer.SetCurrentMana(pSaveData->nCurrentMana);
	rcPlayer.SetCurrentPower(pSaveData->nCurrentPower);
}

BOOL KPlayerServer::LoginServer2(IMemBlock* piRoleData)
{
	BOOL bRet = FALSE;
	time_t timeNow = KSysService::Time(NULL);
	KROLE_DATA* pRoleData = NULL;
	DB_PLAYER_BASE_DATA* pBaseInfo = NULL;
	const KDB_ROLE_BLOCK_HEADER* pBaseBlockHeader = NULL;
	SIZE_T uBaseInfoLen = 0;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	KSAVE_DATA sSaveData;

	USHORT uPort = 0;
	g_piSocketServer->GetRemoteAddress(g_cNetConnectPlayer.GetConnIndex(m_rcPlayer.GetPlayerIndex()), m_dwLoginIp, uPort);

	m_dwVirtualFrame = g_cOrpgWorld.m_dwLogicFrames;
	m_dwRecordCount = 0;
	m_timeLastPing = timeNow;

	if (piRoleData)
	{
		pRoleData = (KROLE_DATA*)piRoleData->GetData();
		QCONFIRM_RET_FALSE(pRoleData);
		pBaseInfo = GET_DB_PLAYER_BASE_DATA(pRoleData);
		QCONFIRM_RET_FALSE(pBaseInfo);
		const KDB_ROLE_BLOCK_HEADER* pBaseBlockHeader = GET_ROLE_BLOCK_HEADER(pRoleData, emKDRBT_BASE_INFO);
		QCONFIRM_RET_FALSE(pBaseBlockHeader);
		uBaseInfoLen = pBaseBlockHeader->wDataLen;
	}

	bRet = g_cPlayerDbMgr.LoadBaseInfo2(m_rcPlayer, pBaseInfo, uBaseInfoLen);
	QCONFIRM_RET_FALSE(bRet);

	SavePlayerBaseValue(sSaveData, &m_rcPlayer);

	// 记录最后一次登录的位置
	SaveLastEntry();

	// 设置下战斗状态信息
	IKScene* pScene = m_rcPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene);
	CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(pScene->GetTemplateId());
	QCONFIRM_RET_FALSE(pCfg);
	m_rcPlayer.SetIsFightMap(pCfg->bFightMap);

	SyncSelfVar(); // 同步自身基本信息和变量数据
	SyncSelfScene(); // 同步自身场景

	// 加载角色数据
	if (piRoleData)
	{
		m_rcPlayer.SetFirstTimeLogin(FALSE);
		bRet = g_cPlayerDbMgr.OnPlayerDbLoad(m_rcPlayer, *pRoleData, piRoleData->GetSize());
		QCONFIRM_RET_FALSE(bRet);
	}
	else
	{// 新角色
		m_rcPlayer.SetFirstTimeLogin(TRUE);
	}

	LoadPlayerBaseValue(&sSaveData, m_rcPlayer);

	// 触发OnLogin
	INT nCtrlId = -1;
	KCharacterCtrlBase* pCtrlBase = NULL;
	while (NULL != (pCtrlBase = m_rcPlayer.TraverseNextCtrl(nCtrlId)))
		pCtrlBase->OnLogin();


	g_cCharacterHelper.CalcFightScore(m_rcPlayer);

	{
		cSafeScript->CallTableFunction("Scene", "OnEnterScene", 0, "d",
			m_rcPlayer.GetId());
	}

	m_rcPlayer.SetGameState(gsPlaying);

	m_rcPlayer.FireEvent(emKOBJEVENTTYPE_PLAYER_LOGIN, m_rcPlayer.GetId(), 0, 0);

	m_rcPlayer.SetLastLoginTime(timeNow);

    m_rcPlayer.m_cPlayerServer.SyncChangeRegion();

	if (!piRoleData)  // kk 新角色， 脚本onfirstlogin放到最后执行， 在task OnDbLoad之后 (否则OnDbLoad会重复添加任务1)
	{
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		if (cSafeScript->GetGlobal("Player"))
		{
			cSafeScript->CallTableFunction("Player", "OnFirstLogin", 1, "o", (QLunaBase*)m_rcPlayer.GetScriptInterface());
		}
	}
	//上线发奖励
	LoginFindSql(m_rcPlayer.GetId()); 
	return TRUE;
}

BOOL KPlayerServer::LogoutServer()
{
	m_rcPlayer.FireEvent(emKOBJEVENTTYPE_PLAYER_LOGOUT, m_rcPlayer.GetId(), 0, 0);
	m_rcPlayer.RawSetLastLogoutTime(KSysService::Time(NULL));
	// 触发OnLogout
	INT nCtrlId = -1;
	KCharacterCtrlBase* pCtrlBase = NULL;
	while (NULL != (pCtrlBase = m_rcPlayer.TraverseNextCtrl(nCtrlId)))
		pCtrlBase->OnLogout();

	// 存档
	if (m_rcPlayer.GetGameState() == gsPlaying ||
		m_rcPlayer.GetGameState() == gsDeleting)
	{
		if (!m_bSetSavePosition) // 优先使用外部设置的保存位置
		    SavePosition();
		g_RelayClient.SaveRoleData(&m_rcPlayer);
		g_RelayClient.DoSaveDelayRoleData(&m_rcPlayer);
	}

	// 通知周围玩家
	g_cPlayerBaseProcess.DoSyncLeave(m_rcPlayer);

	m_bSetSavePosition = FALSE;

	Q_Printl("Player[" << m_rcPlayer.m_dwId << "] " << m_rcPlayer.GetName() << " 已离开游戏");
	m_strAccount.Clear();
	return TRUE;
}

BOOL KPlayerServer::SyncChangeRegion()
{
	IKScene* pScene = m_rcPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	KSyncRegionPlayerFunc cSyncRegionPlayerFunc;
	cSyncRegionPlayerFunc.m_pThisPlayer = &m_rcPlayer;

	// 获取周围的新玩家发给自己
	// Tips: The synchronization mechanism used in 9-Sky, as well as in Sword Online III, 
	// is implemented by means of "Lazy-Synchronization". A Player checks neighboring
	// regions for objs(Player/NPC/Doodad) only when he enters the region/scene for the first time, 
	// and he is responsible to broadcast his arrival to nearby players. Thus, If he 
	// doesn't  move at all, no more Sync message's sent by him.
	pScene->FindNewNeighborPlayers(cSyncRegionPlayerFunc, &m_rcPlayer);

	// doodad、npc
	KSyncRegionNpcFunc cSyncRegionNpcFunc;
	cSyncRegionNpcFunc.m_pThisPlayer = &m_rcPlayer;
	pScene->FindNewNeighborNpcs(cSyncRegionNpcFunc, &m_rcPlayer);

	KSyncRegionDoodadFunc cSyncRegionDoodadFunc;
	cSyncRegionDoodadFunc.m_pThisPlayer = &m_rcPlayer;
	pScene->FindNewNeighborDoodads(cSyncRegionDoodadFunc, &m_rcPlayer);

	// finally, broadcast to new neighbors
	if (m_rcPlayer.IsVisible() && cSyncRegionPlayerFunc.m_vPlayerList.size() > 0)
	{
		SyncToNeighborPlayers(cSyncRegionPlayerFunc.m_vPlayerList);
	}

	// 同步npc的离开
	KSyncRegionNpcLeaveFunc cSyncLeaveNpcs;
	cSyncLeaveNpcs.m_pThisPlayer = &m_rcPlayer;
	pScene->FindOldNeighborNpcs(cSyncLeaveNpcs, &m_rcPlayer);

	// 同步doodad的离开
	KSyncRegionDoodadLeaveFunc cSyncLeaveDoodads;
	cSyncLeaveDoodads.m_pThisPlayer = &m_rcPlayer;
	pScene->FindOldNeighborDoodads(cSyncLeaveDoodads, &m_rcPlayer);

	// 同步玩家的离开
	KSyncRegionPlayerLeaveFunc cSyncLeavePlayers;
	cSyncLeavePlayers.m_pThisPlayer = &m_rcPlayer;
	pScene->FindOldNeighborPlayers(cSyncLeavePlayers, &m_rcPlayer);

	// 把自己的离开同步给已不在自己同步范围内的玩家
	if (cSyncLeavePlayers.m_vPlayerList.size() > 0)
	{
	    SyncToOldNeighborPlayers(cSyncLeavePlayers.m_vPlayerList);   
	}

	if (m_rcPlayer.m_dwBroadcastFrame == 0)
		m_rcPlayer.m_dwBroadcastFrame = g_cOrpgWorld.m_dwLogicFrames;

	return TRUE;
}

BOOL KPlayerServer::FillSyncVar( KCHARACTER_VAR_SYNC_OTHER& rsVarSync, SIZE_T uInLen, SIZE_T& ruOutLen )
{
	BOOL bRet = FALSE;
	ruOutLen = 0;

	// 程序内置变量
	for (UINT i = 0; i < countof(csByteVarSyncOther); ++i)
		rsVarSync.aByVarSet[i] = m_rcPlayer.m_cContainer.m_cByArrayValue.GetValue(csByteVarSyncOther[i]);
	for (UINT i = 0; i < countof(csWordVarSyncOther); ++i)
		rsVarSync.aWVarSet[i] = m_rcPlayer.m_cContainer.m_cWArrayValue.GetValue(csWordVarSyncOther[i]);
	for (UINT i = 0; i < countof(csDWordVarSyncOther); ++i)
		rsVarSync.aDwVarSet[i] = m_rcPlayer.m_cContainer.m_cDwArrayValue.GetValue(csDWordVarSyncOther[i]);
	// 自动同步的扩展变量
	const KVarSetting::VEC_VARKEY& vecSyncVarKey = g_cCharacterVarSetting.GetSyncOtherTask();
	// 检查假设vecSyncVarKey的变量全部要设置时发送缓冲区是否足够
	if (vecSyncVarKey.size() * sizeof(rsVarSync.aMapVarSet[0]) < uInLen)
	{
		rsVarSync.wMapVarNum = 0;
		KVarSetting::VEC_VARKEY::const_iterator it;
		for (it = vecSyncVarKey.begin(); it != vecSyncVarKey.end(); ++it)
		{
			DWORD dwValue = m_rcPlayer.GetVar(it->byVarGroup, it->byVarId);
			if (dwValue)
			{
				rsVarSync.aMapVarSet[rsVarSync.wMapVarNum].byVarGroup = it->byVarGroup;
				rsVarSync.aMapVarSet[rsVarSync.wMapVarNum].byVarId = it->byVarId;
				rsVarSync.aMapVarSet[rsVarSync.wMapVarNum].dwVarValue = dwValue;
				++rsVarSync.wMapVarNum;
				ruOutLen += sizeof(rsVarSync.aMapVarSet[0]);
			}
		}

		bRet = TRUE;
	}
	else
	{
		Q_Error("Too much data to synchronize!");
	}

	return bRet;
}

BOOL KPlayerServer::FillSyncVar( KCHARACTER_VAR_SYNC_SELF& rsVarSync, SIZE_T uInLen, SIZE_T& ruOutLen )
{
	BOOL bRet = FALSE;
	ruOutLen = 0;

	// 程序内置变量
	for (UINT i = 0; i < countof(csByteVarSyncSelf); ++i)
		rsVarSync.aByVarSet[i] = m_rcPlayer.m_cContainer.m_cByArrayValue.GetValue(csByteVarSyncSelf[i]);
	for (UINT i = 0; i < countof(csWordVarSyncSelf); ++i)
		rsVarSync.aWVarSet[i] = m_rcPlayer.m_cContainer.m_cWArrayValue.GetValue(csWordVarSyncSelf[i]);
	for (UINT i = 0; i < countof(csDWordVarSyncSelf); ++i)
		rsVarSync.aDwVarSet[i] = m_rcPlayer.m_cContainer.m_cDwArrayValue.GetValue(csDWordVarSyncSelf[i]);
	// 自动同步的扩展变量
	const KVarSetting::VEC_VARKEY& vecSyncVarKey = g_cCharacterVarSetting.GetSyncSelfTask();
	// 检查假设vecSyncVarKey的变量全部要设置时发送缓冲区是否足够
	if (vecSyncVarKey.size() * sizeof(rsVarSync.aMapVarSet[0]) < uInLen)
	{
		rsVarSync.wMapVarNum = 0;
		KVarSetting::VEC_VARKEY::const_iterator it;
		for (it = vecSyncVarKey.begin(); it != vecSyncVarKey.end(); ++it)
		{
			DWORD dwValue = m_rcPlayer.GetVar(it->byVarGroup, it->byVarId);
			if (dwValue)
			{
				rsVarSync.aMapVarSet[rsVarSync.wMapVarNum].byVarGroup = it->byVarGroup;
				rsVarSync.aMapVarSet[rsVarSync.wMapVarNum].byVarId = it->byVarId;
				rsVarSync.aMapVarSet[rsVarSync.wMapVarNum].dwVarValue = dwValue;
				++rsVarSync.wMapVarNum;
				ruOutLen += sizeof(rsVarSync.aMapVarSet[0]);
			}
		}

		bRet = TRUE;
	}
	else
	{
		Q_Error("Too much data to synchronize!");
	}

	return bRet;
}

BOOL KPlayerServer::SyncSelfScene()
{
	IKScene* pScene = m_rcPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	KS2C_SYNC_SELF sSync;
	sSync.dwLogicFrames = g_cOrpgWorld.m_dwLogicFrames;
	sSync.dwSceneId = pScene->GetTemplateId(); // 注意：传给客户端的是场景的模板Id
	//sSync.nMapCopyIndex = ;
	m_rcPlayer.GetPosition(sSync.nX, sSync.nY, sSync.nZ);
	sSync.nDirectionXY = m_rcPlayer.m_nDirectionXY;

	return g_pBaseSender->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KPlayerServer::SyncSelfVar()
{
	// 加载角色数据完毕，同步给玩家
	SIZE_T uSizeUsed = 0;
	SIZE_T uSizeBuffer = sizeof(g_abySyncPackBuffer) - _offsetof(KS2C_PLAYERDATA, sVarSync);
	KS2C_PLAYERDATA& sData = *(KS2C_PLAYERDATA* )g_abySyncPackBuffer;
	sData = KS2C_PLAYERDATA();

	sData.dwPlayerId = m_rcPlayer.m_dwId;
	sData.dwLogicFrames = g_cOrpgWorld.m_dwLogicFrames;
	sData.nServerId = g_cOrpgWorld.m_nServerId;
	QStrCpyLen(sData.szPlayerName, m_rcPlayer.GetName(), sizeof(sData.szPlayerName));
	sData.dwServerTime = KSysService::GameTime(NULL);
	sData.dwKinId = m_rcPlayer.m_dwKinId;
	sData.nTeamId = m_rcPlayer.GetTeamId();

	const KENV_CONFIG* pConfig = g_piNsBaseRequire->GetEnvConfig();
	sData.dwStartServerTime = pConfig->dwStartSeverTime;

	BOOL bRet = FillSyncVar(sData.sVarSync, uSizeBuffer, uSizeUsed);
	QCONFIRM_RET_FALSE(bRet);
	uSizeUsed += _offsetof(KS2C_PLAYERDATA, sVarSync.aMapVarSet);

	return g_pBaseSender->SendLenData(m_rcPlayer.GetPlayerIndex(), sData, uSizeUsed);
}

BOOL KPlayerServer::SetPositionForSaving(DWORD dwMapId, INT nX, INT nY, INT nZ)
{
	m_SavePosition.dwMapID = GET_MAP_ID(dwMapId);
	m_SavePosition.nMapCopyIndex = GET_MAP_COPY_INDEX(dwMapId);
	m_SavePosition.nX = nX;
	m_SavePosition.nY = nY;
	m_SavePosition.nZ = nZ;
	m_SavePosition.nFaceDirection  = m_rcPlayer.m_nFaceDirection;
	m_bSetSavePosition = TRUE;
	return TRUE;
}

BOOL KPlayerServer::TransferToBattleZone(INT nX, INT nY, INT nZ)
{
	BOOL                        bRetCode            = false;

	// 将角色数据存盘,位置保存为跨服的目标点
	m_SavePosition.nX              = nX;
	m_SavePosition.nY              = nY;
	m_SavePosition.nZ              = nZ;

	m_rcPlayer.SetGameState(gsWaitForBZoneTransferSave);
	//m_nTargetBZoneId = nServerId;

	bRetCode = g_RelayClient.SaveRoleData(&m_rcPlayer);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:
	QLogPrintf(LOG_DEBUG, "[%d][%s] ChangeGameServer",
		m_rcPlayer.GetId(), m_rcPlayer.GetName());

	return bRetCode;
}

BOOL KPlayerServer::DoMultiTransferRequest()
{
	BOOL bRetCode = false;

	m_rcPlayer.SetGameState(gsWaitForBZoneTransferSave);

	bRetCode = g_RelayClient.SaveRoleData(&m_rcPlayer);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = TRUE;
EXIT0:

	return bRetCode;
}

VOID KPlayerServer::ClearRecordPosition()
{
	m_dwRecordCount = 0;

	for(INT i = 0; i < MAX_POSITION_RECORD; ++i)
	{
		m_PositionRecord[i].bEffect = FALSE;
	}
}

BOOL KPlayerServer::RecordPosition()
{
	INT     nRecordIndex = m_dwVirtualFrame % MAX_POSITION_RECORD;

	m_PositionRecord[nRecordIndex].dwFrame                  = m_dwVirtualFrame;
	m_PositionRecord[nRecordIndex].eMoveState              = m_rcPlayer.m_eMoveState;
	m_rcPlayer.GetPosition(m_PositionRecord[nRecordIndex].nX,
		m_PositionRecord[nRecordIndex].nY, 
		m_PositionRecord[nRecordIndex].nZ);
	m_rcPlayer.GetDestination(m_PositionRecord[nRecordIndex].nDestX,
		m_PositionRecord[nRecordIndex].nDestY, 
		m_PositionRecord[nRecordIndex].nDestZ);
	m_PositionRecord[nRecordIndex].nVelocityXY             = m_rcPlayer.m_nVelocityXY;
	m_PositionRecord[nRecordIndex].nDirectionXY            = m_rcPlayer.m_nDirectionXY;
	m_PositionRecord[nRecordIndex].nVelocityZ              = m_rcPlayer.m_nVelocityZ;
	//m_PositionRecord[nRecordIndex].nConvergenceSpeed       = m_nConvergenceSpeed;
	m_PositionRecord[nRecordIndex].nCurrentGravity         = m_rcPlayer.m_nCurrentGravity;
	//m_PositionRecord[nRecordIndex].nJumpCount              = m_nJumpCount;	
	m_PositionRecord[nRecordIndex].nFaceDirection          = m_rcPlayer.m_nFaceDirection;
	//m_PositionRecord[nRecordIndex].nDivingCount            = m_nDivingCount;  
	//m_PositionRecord[nRecordIndex].bTryPathState           = m_bTryPathState;
	//m_PositionRecord[nRecordIndex].nTryPathSide            = m_nTryPathSide;
	//m_PositionRecord[nRecordIndex].bSlip                   = m_bSlip;
	//m_PositionRecord[nRecordIndex].nCurrentTrack           = m_nCurrentTrack;
	//m_PositionRecord[nRecordIndex].nFromNode               = m_nFromNode;
	//m_PositionRecord[nRecordIndex].nTargetCity             = m_nTargetCity;
	//m_PositionRecord[nRecordIndex].nMoveFrameCount         = m_nMoveFrameCounter;
	m_PositionRecord[nRecordIndex].bEffect					= TRUE;

	m_dwVirtualFrame++;

	if (m_dwRecordCount < MAX_POSITION_RECORD)
	{
		m_dwRecordCount++;
	}
	return TRUE;
}

BOOL KPlayerServer::ReversePosition(DWORD dwFrame)
{
	BOOL                bResult         = FALSE;
	BOOL                bRetCode        = FALSE;
	DWORD			dwReverseFrame   = 0;
	DWORD             dwRecordIndex    = 0;
	KPOSITION_RECORD*   pRecord         = NULL;

	LOG_PROCESS_ERROR(dwFrame > 0);

	dwReverseFrame = MIN(m_dwRecordCount, dwFrame);
	PROCESS_ERROR(dwReverseFrame > 0);

	dwRecordIndex = (m_dwVirtualFrame - dwReverseFrame) % MAX_POSITION_RECORD;

	pRecord = &m_PositionRecord[dwRecordIndex];

	PROCESS_ERROR(pRecord->bEffect);		// 已经不生效了

	LOG_PROCESS_ERROR(pRecord->dwFrame == m_dwVirtualFrame - dwReverseFrame);

	bRetCode = m_rcPlayer.SetPosition(pRecord->nX, pRecord->nY, pRecord->nZ);
	LOG_PROCESS_ERROR(bRetCode);

	LOG_PROCESS_ERROR(m_rcPlayer.GetScene());

	m_rcPlayer.m_eMoveState          =   pRecord->eMoveState;  
	m_rcPlayer.m_nDestX              =   pRecord->nDestX;
	m_rcPlayer.m_nDestY              =   pRecord->nDestY;
	m_rcPlayer.m_nVelocityXY         =   pRecord->nVelocityXY;
	m_rcPlayer.m_nDirectionXY        =   pRecord->nDirectionXY;
	m_rcPlayer.m_nVelocityZ          =   pRecord->nVelocityZ;
	//m_nConvergenceSpeed   =   pRecord->nConvergenceSpeed;
	m_rcPlayer.m_nCurrentGravity     =   pRecord->nCurrentGravity;
	//m_nJumpCount          =	  pRecord->nJumpCount;   
	m_rcPlayer.m_nFaceDirection      =   pRecord->nFaceDirection;   
	//m_bTryPathState       =   pRecord->bTryPathState;
	//m_nTryPathSide        =   pRecord->nTryPathSide;
	//m_bSlip               =   pRecord->bSlip;
	//m_nMoveFrameCounter   =   pRecord->nMoveFrameCount;

	m_dwVirtualFrame -= dwReverseFrame;

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerServer::CheckMoveAdjust(DWORD dwClientFrame, const KMOVE_CRITICAL_PARAM& crParam)
{
	BOOL bResult        = TRUE;
	INT nX = 0, nY = 0, nZ = 0;
	CharacterSkill* pCharacterSkill;
	DWORD dwLastVirtualFrame = m_dwVirtualFrame;

	// 可能是外挂，考虑断线封号
	LOG_PROCESS_ERROR(dwClientFrame <= g_cOrpgWorld.m_dwLogicFrames + GAME_FPS / 2);

	if (m_dwVirtualFrame > dwClientFrame)
	{
		if (g_cOrpgWorld.m_bDebugSelf)
			QLogPrintf(LOG_INFO, "[CheckMoveAdjust]服务器帧快于客户端帧，缓帧.角色ID:%d   Server: %d  Client: %d", m_rcPlayer.GetId(), m_dwVirtualFrame, dwClientFrame);

		ReversePosition(m_dwVirtualFrame - dwClientFrame);
	}
	else if (m_dwVirtualFrame < dwClientFrame)   // 客户端快一点, 技能追帧
	{
		DWORD dwGap = dwClientFrame - m_dwVirtualFrame;
		if (g_cOrpgWorld.m_bDebugSelf)
			QLogPrintf(LOG_INFO, "[CheckMoveAdjust]服务器帧慢于客户端帧，追帧.角色ID:%d   Server: %d  Client: %d", m_rcPlayer.GetId(), m_dwVirtualFrame, dwClientFrame);
		
		m_rcPlayer.ForwardPosition(dwClientFrame - m_dwVirtualFrame);

		pCharacterSkill = m_rcPlayer.GetCtrl<CharacterSkill>();
		pCharacterSkill->ForwardFrame(dwGap);
	}

	//PROCESS_ERROR(m_eMoveState == crParam.byMoveState);
	m_rcPlayer.GetPosition(nX, nY, nZ);

	if (g_cOrpgWorld.m_bDebugSelf && (nX != crParam.nX || nY != crParam.nY || nZ != crParam.nZ))
	{
		printf("[BadCoord] 服务端当前位置[%d,%d,%d] 客户端当前位置[%d,%d,%d], [服务器帧S%u-现客户端帧C%u :%u] 真客户端帧L%u\n",
			nX, nY, nZ,
			crParam.nX, crParam.nY, crParam.nZ,
			m_dwVirtualFrame,
			dwClientFrame,
			m_dwVirtualFrame - dwClientFrame,
			dwLastVirtualFrame);
		for (INT i = 0; i < MAX_POSITION_RECORD; ++i)
		{
			if (m_PositionRecord[i].dwFrame == dwLastVirtualFrame)
				printf("*");
			else if (m_PositionRecord[i].dwFrame == m_dwVirtualFrame)
				printf(">");
			else
				printf(" ");
			printf("帧F[%5u] 位置记录[%d,%d,%d]\n",
				m_PositionRecord[i].dwFrame,
				m_PositionRecord[i].nX, m_PositionRecord[i].nY, m_PositionRecord[i].nZ);
		}
	}
	PROCESS_ERROR(nX == crParam.nX);
	PROCESS_ERROR(nY == crParam.nY);
	PROCESS_ERROR(nZ == crParam.nZ);

	bResult = FALSE; // 返回FALSE表示无须修正
EXIT0:
	m_dwRecordCount  = 0;
	return bResult;
}

BOOL KPlayerServer::TeleportTo2(INT nMapId, INT nMapCopyIndex, INT nX, INT nY, INT nZ)
{
	BOOL bResult = FALSE;
	QCONFIRM_RET_FALSE(nX >= 0 && nY >= 0);

	QCONFIRM_RET_FALSE(m_rcPlayer.GetGameState() == gsPlaying);

	//by ldy  传送到蓬莱岛添加等级限制
	if(m_rcPlayer.GetLevel() < 51 && nMapId == 6){
		QLogPrintf(LOG_INFO,"the condition of telport to penglaidao is the role level more than 51 ");
		g_cPlayerBaseProcess.DoSyncBlackMsg(&m_rcPlayer, LANG_009);
		return FALSE;
	}

		//by ldy  传送到瑶池添加限制
	if(nMapId == 7){
		QLogPrintf(LOG_INFO,"the condition of telport to yaochi is not open ");
		g_cPlayerBaseProcess.DoSyncBlackMsg(&m_rcPlayer,LANG_035);
		return FALSE;
	}

	
	// 临时把传送Z值强制设为1048576，这是因为游龙地图数据没有高度，统一在0点，策划填表的时候没注意这一点引起很多问题，为了保证不出错这里强制设置，
	// TODO: 这是只为了游龙这个项目能及时上线的作法，之后如有其他项目用到这代码，必须去掉
	nZ = 1048576;

	IKScene* pTargetScene = g_pSceneMgr->GetScene(nMapId, nMapCopyIndex);

	IKScene* pCurrentScene = m_rcPlayer.GetScene();
	ASSERT(pCurrentScene);

	// 传送前先复活- -！
	if (m_rcPlayer.GetMoveState() == cmsOnDeath)
		g_cCharacterHelper.Revive(m_rcPlayer, 10, 10, 0);

	m_rcPlayer.PickUpAllDoodad();		// 传送前捡起所有场景掉落道具

	// 如果目标场景就是本场景 (包括场景Id填0的情况)
	if (0 == nMapId || (pCurrentScene == pTargetScene && pTargetScene))
	{
		m_rcPlayer.SetPosition(nX, nY, nZ);
		m_rcPlayer.StopMoving();

		g_cPlayerBaseProcess.DoSyncTeleport(&m_rcPlayer, nX, nY, nZ);
		// 清空追帧信息
		//ClearRecordPosition();
	}
	else
	{
		// 这里将原来的位置记录下来,Search map失败的话还要用这个记录来恢复原来的位置
		bResult = SavePosition();
		QCONFIRM_RET_FALSE(bResult);

		g_RelayClient.SaveRoleData(&m_rcPlayer);
		g_RelayClient.DoSearchMapRequest(m_rcPlayer.GetId(), nMapId, nMapCopyIndex, nX, nY, nZ, m_bOnThisGs);

		g_RelayClient.DoSaveDelayRoleData(&m_rcPlayer);

		//g_cPlayerBaseProcess.DoSyncLeave(m_rcPlayer, TRUE);

		//if (pCurrentScene)
		//	pCurrentScene->RemovePlayer(&m_rcPlayer);

		m_rcPlayer.SetGameState(gsSearchMap);
		m_nBanishTime = 0;
	}

	m_dwRecordCount = 0;

	m_dwVirtualFrame = g_cOrpgWorld.m_dwLogicFrames;

	return bResult;
}

BOOL KPlayerServer::SyncVarSelf( WORD wVarGroup, WORD wVarId )
{
	KS2C_SYNC_VAR_SELF sSync;
	sSync.byVarGroup = (BYTE)wVarGroup;
	sSync.byVarId = (BYTE)wVarId;
	sSync.dwVarValue = (DWORD)m_rcPlayer.GetVar(wVarGroup, wVarId);
	return g_pBaseSender->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KPlayerServer::SyncPullToSelf(INT nDestX, INT nDestY, INT nDestZ)
{
	KS2C_SYNC_MOVE sSync;

	sSync.nDestX = nDestX;
	sSync.nDestY = nDestY;
	sSync.nDestZ = nDestZ;

	sSync.dwId = m_rcPlayer.GetId();
	sSync.nMoveState = cmsOnPull;
	return g_pBaseSender->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KPlayerServer::SyncKnockToSelf(INT nDestX, INT nDestY, INT nDestZ)
{
	KS2C_SYNC_MOVE sSync;

	sSync.nDestX = nDestX;
	sSync.nDestY = nDestY;
	sSync.nDestZ = nDestZ;

	sSync.dwId = m_rcPlayer.GetId();
	sSync.nMoveState = cmsOnKnockedBack;
	return g_pBaseSender->SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KPlayerServer::DoSyncVarSelf( WORD wVarGroup, WORD wVarId )
{
	return SyncVarSelf(wVarGroup, wVarId);
}

//////////////////////////////////////////////////////////////////////////
SIZE_T KPlayerServer::SaveTaskVar( LPBYTE pBuffer, SIZE_T uBufSize )
{
	LPBYTE pEndBuffer = pBuffer + uBufSize;
	const KTArrayValue<BYTE>& rcByteArray = m_rcPlayer.m_cContainer.m_cByArrayValue;
	const KTArrayValue<WORD>& rcWordArray = m_rcPlayer.m_cContainer.m_cWArrayValue;
	const KTArrayValue<DWORD>& rcDwArray = m_rcPlayer.m_cContainer.m_cDwArrayValue;
	const std::map<DWORD, DWORD>& rcDynMap = m_rcPlayer.m_cContainer.m_mpDynValue;
	const KTArrayValue<DWORD>& rcRoleArray = m_rcPlayer.m_cRoleValue;
	LPDWORD pdwMapSaveCount = NULL;
	DWORD dwMapSaveCount = 0;

	QCONFIRM_RET(pBuffer + sizeof(WORD) * 3 + sizeof(DWORD) <= pEndBuffer, 0);

	// 先存储3种ArrayValue数量及MapValue数量（TODO:优化）
	QCONFIRM_RET(rcByteArray.Count() < UCHAR_MAX, 0);
	*(LPWORD)pBuffer = (WORD)rcByteArray.Count();
	pBuffer += sizeof(WORD);

	QCONFIRM_RET(rcWordArray.Count() < UCHAR_MAX, 0);
	*(LPWORD)pBuffer = (WORD)rcWordArray.Count();
	pBuffer += sizeof(WORD);

	QCONFIRM_RET(rcDwArray.Count() < UCHAR_MAX, 0);
	QCONFIRM_RET(emKCHARACTER_TASK_DWORD_SAVED_END <= rcDwArray.Count(), 0);
	*(LPWORD)pBuffer = (WORD)emKCHARACTER_TASK_DWORD_SAVED_END;
	pBuffer += sizeof(WORD);

	QCONFIRM_RET(rcDynMap.size() < UCHAR_MAX, 0);
	QCONFIRM_RET(dwMapSaveCount <= rcDynMap.size(), 0);
	pdwMapSaveCount = (LPDWORD)pBuffer;
	pBuffer += sizeof(DWORD);

	// 再存储KCharacter::m_cRoleValue容器数量
	QCONFIRM_RET(rcRoleArray.Count() < UCHAR_MAX, 0);
	QCONFIRM_RET(emKPLAYER_TASK_SAVED_END <= rcRoleArray.Count(), 0);
	*(LPWORD)pBuffer = (WORD)emKPLAYER_TASK_SAVED_END;
	pBuffer += sizeof(WORD);

	// Copy Byte Array Value
	SIZE_T uArraySize = sizeof(BYTE) * rcByteArray.Count();
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, 0);
	memcpy(pBuffer, rcByteArray.GetBuffer(), uArraySize);
	pBuffer += uArraySize;

	// Copy WORD Array Value
	uArraySize = sizeof(WORD) * rcWordArray.Count();
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, 0);
	memcpy(pBuffer, rcWordArray.GetBuffer(), uArraySize);
	pBuffer += uArraySize;

	// Copy DWORD Array Value
	uArraySize = sizeof(DWORD) * emKCHARACTER_TASK_DWORD_SAVED_END;
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, 0);
	memcpy(pBuffer, rcDwArray.GetBuffer(), uArraySize);
	pBuffer += uArraySize;

	// Save Map Value
	// MapValue的比较特殊，用总大小减去不存盘的(因为不存盘的在中间一段)
	// 先判断长度是否足够 TODO:压缩优化
	QCONFIRM_RET(pBuffer + rcDynMap.size() * sizeof(DWORD) * 2 <= pEndBuffer, 0);
	LPDWORD pdwBuffer = (LPDWORD)pBuffer;
	std::map<DWORD, DWORD>::const_iterator it;
	// 默认按key从小到大排列好的，如果不是，这样写就有问题
	for (it = rcDynMap.lower_bound(KD_VAR_KEY(emKVARGROUP_CHARACTER_MAP, 0));
		it != rcDynMap.lower_bound(
		KD_VAR_KEY(emKVARGROUP_CHARACTER_MAP, emKCHARACTER_TASK_MAP_SAVED_END));
		++it)
	{
		if (0 != it->second)
		{
			*pdwBuffer++ = it->first;
			*pdwBuffer++ = it->second;
			dwMapSaveCount++;
		}
	}
	for (it = rcDynMap.lower_bound(KD_VAR_KEY(KD_MIN_EXTERN_VARGROUP, 0));
		it != rcDynMap.end();
		++it)
	{
		if (0 != it->second)
		{
			*pdwBuffer++ = it->first;
			*pdwBuffer++ = it->second;
			dwMapSaveCount++;
		}
	}

	*pdwMapSaveCount = dwMapSaveCount;
	pBuffer = (LPBYTE)pdwBuffer;

	// 最后Copy
	uArraySize = sizeof(DWORD) * emKPLAYER_TASK_SAVED_END;
	QCONFIRM_RET(pBuffer + uArraySize <= pEndBuffer, 0);
	memcpy(pBuffer, rcRoleArray.GetBuffer(), uArraySize);
	pBuffer += uArraySize;

	return uBufSize - (pEndBuffer - pBuffer); // 返回已使用的缓冲区大小
}

BOOL KPlayerServer::LoadTaskVar( LPCBYTE pData, SIZE_T uDataLen )
{
	// byte/word/dword容器内变量个数 + map成员个数 + roleArray个数
	QCONFIRM_RET_FALSE( sizeof(WORD) * 3 + sizeof(DWORD) + sizeof(WORD) <= uDataLen);

	KTArrayValue<BYTE>& rcByteArray = m_rcPlayer.m_cContainer.m_cByArrayValue;
	KTArrayValue<WORD>& rcWordArray = m_rcPlayer.m_cContainer.m_cWArrayValue;
	KTArrayValue<DWORD>& rcDwArray = m_rcPlayer.m_cContainer.m_cDwArrayValue;
	std::map<DWORD, DWORD>& rcDynMap = m_rcPlayer.m_cContainer.m_mpDynValue;
	KTArrayValue<DWORD>& rcRoleArray = m_rcPlayer.m_cRoleValue;

	WORD wByArrayValueNum, wWArrayValueNum, wDwArrayValueNum, wRoleArrayValueNum;
	DWORD dwMapValueNum;
	wByArrayValueNum = *(LPWORD)pData;
	pData += sizeof(WORD);
	wWArrayValueNum = *(LPWORD)pData;
	pData += sizeof(WORD);
	wDwArrayValueNum = *(LPWORD)pData;
	pData += sizeof(WORD);
	dwMapValueNum = *(LPDWORD)pData;
	pData += sizeof(DWORD);
	wRoleArrayValueNum = *(LPWORD)pData;
	pData += sizeof(WORD);
	uDataLen -= sizeof(WORD) * 3 + sizeof(DWORD) + sizeof(WORD);
	// 检查数据大小是否合法
	QCONFIRM_RET_FALSE(wByArrayValueNum * sizeof(BYTE) + wWArrayValueNum * sizeof(WORD) + 
		wDwArrayValueNum * sizeof(DWORD) + dwMapValueNum * sizeof(DWORD) * 2 + 
		wRoleArrayValueNum * sizeof(DWORD) == uDataLen);
	// 检查ArrayValue Buffer是否足够
	QCONFIRM_RET_FALSE(wByArrayValueNum <= rcByteArray.Count() &&
		wWArrayValueNum <= rcWordArray.Count() && wDwArrayValueNum <= rcDwArray.Count() &&
		wRoleArrayValueNum <= rcRoleArray.Count());

	// Start copying...
	// byte/word/dword容器
	memcpy(rcByteArray.RawGet(), pData, wByArrayValueNum * sizeof(BYTE));
	pData += wByArrayValueNum * sizeof(BYTE);
	memcpy(rcWordArray.RawGet(), pData, wWArrayValueNum * sizeof(WORD));
	pData += wWArrayValueNum * sizeof(WORD);
	memcpy(rcDwArray.RawGet(), pData, wDwArrayValueNum * sizeof(DWORD));
	pData += wDwArrayValueNum * sizeof(DWORD);
	// map容器的
	LPCDWORD pdwData = (LPCDWORD)pData;
	for (UINT i = 0; i < dwMapValueNum; ++i)
	{
		DWORD dwVarKey = *pdwData++;
		DWORD dwVarValue = *pdwData++;

		ASSERT(0 != dwVarValue);
		QCONFIRM_RET_FALSE(KD_VAR_GROUP(dwVarKey) >= KD_MIN_EXTERN_VARGROUP ||
			KD_VAR_ID(dwVarKey) < emKCHARACTER_TASK_MAP_SAVED_END);
		rcDynMap.insert(std::map<DWORD, DWORD>::value_type(dwVarKey, dwVarValue));
	}
	// roleValue容器的
	memcpy(rcRoleArray.RawGet(), pData, wRoleArrayValueNum * sizeof(DWORD));
	pData += wRoleArrayValueNum * sizeof(DWORD);
	return TRUE;
}

BOOL KPlayerServer::KickOut(KEKICK_PLAYER_REASON eReason)
{
	Q_Printl("[KPlayerServer] kicking out [" << m_rcPlayer.GetId() << "][" << m_rcPlayer.GetPlayerIndex() <<
		"][" << m_rcPlayer.GetName() << "] Reason " << eReason);
	return g_cNetConnectPlayer.KickOutPlayer(m_rcPlayer.GetPlayerIndex());
}

// 储存上一次位置(普通地图里)
BOOL KPlayerServer::SaveLastEntry()
{
	IKScene* pScene = m_rcPlayer.GetScene();
	const KSCENE_CONFIG* pSceneSetting = g_SceneMgr.GetSceneInfo(m_SavePosition.dwMapID);
	if (pScene && pSceneSetting)
	{
		if(strcmp(pSceneSetting->szSceneType, "city") == 0)
		{
			m_LastEntry.nMapCopyIndex = GET_MAP_COPY_INDEX(pScene->GetId());
			m_LastEntry.dwMapID = pScene->GetTemplateId();
			m_rcPlayer.GetPosition(m_LastEntry.nX, m_LastEntry.nY, m_LastEntry.nZ);
			m_LastEntry.nFaceDirection = m_rcPlayer.GetFaceDirection();
		}
	}

	return TRUE;
}

BOOL KPlayerServer::ReturnToLastCity()
{
	QLogPrintf(LOG_DEBUG, "[%d][%s] ReturnToLastCity [%d][%d][%d,%d,%d]",
		m_rcPlayer.GetId(), m_rcPlayer.GetName(),
		m_LastEntry.dwMapID, m_LastEntry.nMapCopyIndex,
		m_LastEntry.nX, m_LastEntry.nY, m_LastEntry.nZ
		);

	if (m_rcPlayer.GetGameState() == gsPlaying) // 处于正常游戏状态才传送
	{
        TeleportTo2(m_LastEntry.dwMapID, m_LastEntry.nMapCopyIndex, m_LastEntry.nX, m_LastEntry.nY, m_LastEntry.nZ);
	}
	
	return TRUE;
}

BOOL KPlayerServer::SyncToOldNeighborPlayers(std::vector<KPlayer*>& vPlayers)
{
	KS2C_SYNC_LEAVE sSync;
	sSync.dwId = m_rcPlayer.GetId();

	for (std::vector<KPlayer*>::iterator it = vPlayers.begin(); it != vPlayers.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		QCONFIRM_CONTINUE(pPlayer);
		if (pPlayer == &m_rcPlayer)
			continue;

		if (pPlayer->GetScene() == m_rcPlayer.GetScene())
		{
			g_pBaseSender->SendData(pPlayer->GetPlayerIndex(), sSync);
		}
	}
	return TRUE;
}

BOOL KPlayerServer::SyncToNeighborPlayers( std::vector<KPlayer*>& vPlayers )
{
	SIZE_T uSizeUsed = 0;
	SIZE_T uSizeBuffer = sizeof(g_abySyncPackBuffer) - _offsetof(KS2C_SYNC_PLAYER, sVarSync);
	KS2C_SYNC_PLAYER& sData = *(KS2C_SYNC_PLAYER* )g_abySyncPackBuffer;
	sData = KS2C_SYNC_PLAYER();

	// fill player basic info
	QStrCpyLen(sData.szName, m_rcPlayer.GetName(), sizeof(sData.szName));
	sData.dwPlayerId = m_rcPlayer.m_dwId;
	sData.nMoveState = m_rcPlayer.m_eMoveState;
	sData.nVelocityXY = m_rcPlayer.m_nVelocityXY;
	sData.nDirectionXY = m_rcPlayer.m_nDirectionXY;
	sData.dwKinId = m_rcPlayer.m_dwKinId;
	sData.nTeamId = m_rcPlayer.GetTeamId();

	m_rcPlayer.GetPosition(sData.nX, sData.nY, sData.nZ);
	m_rcPlayer.GetDestination(sData.nDestX, sData.nDestY, sData.nDestZ);

	// fill variant sync
	BOOL bRet = FillSyncVar(sData.sVarSync, uSizeBuffer, uSizeUsed);
	QCONFIRM_RET_FALSE(bRet && uSizeBuffer > 0);
	uSizeUsed += _offsetof(KS2C_SYNC_PLAYER, sVarSync.aMapVarSet);

	for (std::vector<KPlayer*>::iterator it = vPlayers.begin(); it != vPlayers.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		QCONFIRM_CONTINUE(pPlayer);
		if (pPlayer == &m_rcPlayer)
			continue;

		if(m_rcPlayer.IsSkillHidden(pPlayer))
			continue;

		//if (!pPlayer->IsFork()) 战斗中的玩家的数据只需要同步给战斗中的， 不需要同步出去
		if (pPlayer->GetScene() == m_rcPlayer.GetScene())
		{
			g_pBaseSender->SendLenData(pPlayer->GetPlayerIndex(), sData, uSizeUsed);
		}
	}
	return TRUE;
}

BOOL KPlayerServer::DoMessageNotify( INT nModule, INT nMsgId, const void* pAdditional /*= 0*/, UINT uDataLen /*= 0*/)
{
	QLogPrintf(LOG_INFO,"..KPlayerServer::DoMessageNotify( INT nModule=%d, INT nMsgId=%d",nModule,nMsgId);
	return g_cPlayerBaseProcess.DoMessageNotify(&m_rcPlayer, nModule, nMsgId, (const PVOID)pAdditional, uDataLen);
}

BOOL KPlayerServer::LoadBaseInfo(KRoleBaseInfo* pBaseInfo, SIZE_T uSize )
{
	BOOL bRet = FALSE;
	int         nMaxX           = 0;
	int         nMaxY           = 0;
	KScene* pScene = NULL;

	pScene = (KScene*)g_SceneMgr.GetScene(pBaseInfo->CurrentPos.dwMapID, pBaseInfo->CurrentPos.nMapCopyIndex);
	QCONFIRM_RET_FALSE(pScene);

	nMaxX = pScene->GetRegionWidth() * REGION_GRID_WIDTH * CELL_LENGTH - 1;
	nMaxY = pScene->GetRegionHeight() * REGION_GRID_HEIGHT * CELL_LENGTH - 1;

	MAKE_IN_RANGE(pBaseInfo->CurrentPos.nX, 0, nMaxX);
	MAKE_IN_RANGE(pBaseInfo->CurrentPos.nY, 0, nMaxY);
	MAKE_IN_RANGE(pBaseInfo->CurrentPos.nZ, 0, MAX_Z_POINT);

	// 仅对第一次登录生效，以后登录会被LoadTaskVar覆盖
	m_rcPlayer.RawSetLevel(pBaseInfo->byLevel);			// 新建人物等级
	m_rcPlayer.RawSetOnlineTime(pBaseInfo->nTotalGameTime);
	m_rcPlayer.RawSetCurrentCamp(emKCHARACTER_CAMP_PLAYER);	// 玩家阵营
	m_rcPlayer.RawSetSex(pBaseInfo->cRoleType);
	m_rcPlayer.RawSetPrimaryFaction(pBaseInfo->byFaction); // 主职业(zzx)
	m_rcPlayer.RawSetCurrentRoute(1);	// 默认路线
	m_rcPlayer.RawSetPetCapacity(KD_MIN_PET_COUNT);	// 宠物栏数量

	m_rcPlayer.RawSetHatRepresentId(pBaseInfo->wRepresentId[emKPLAYER_REPRESENT_TYPE_HAIR]);
	m_rcPlayer.RawSetBodyRepresentId(pBaseInfo->wRepresentId[emKPLAYER_REPRESENT_TYPE_BODY]);

	// 玩家登录过程中,Load的时候,还没有真正的把玩家添加到场景中去,到客户端AckData的时候才会真正添加
	m_SavePosition.dwMapID          = pBaseInfo->CurrentPos.dwMapID;
	m_SavePosition.nMapCopyIndex    = pBaseInfo->CurrentPos.nMapCopyIndex;
	m_SavePosition.nX               = pBaseInfo->CurrentPos.nX;
	m_SavePosition.nY               = pBaseInfo->CurrentPos.nY;
	m_SavePosition.nZ               = pBaseInfo->CurrentPos.nZ;
	m_SavePosition.nFaceDirection   = pBaseInfo->CurrentPos.byFaceDirection;

	m_LastEntry.dwMapID             = pBaseInfo->LastEntry.dwMapID;
	m_LastEntry.nMapCopyIndex       = pBaseInfo->LastEntry.nMapCopyIndex;
	m_LastEntry.nX                  = pBaseInfo->LastEntry.nX;
	m_LastEntry.nY                  = pBaseInfo->LastEntry.nY;
	m_LastEntry.nZ                  = pBaseInfo->LastEntry.nZ;
	m_LastEntry.nFaceDirection      = pBaseInfo->LastEntry.byFaceDirection;

	m_rcPlayer.m_nFaceDirection		= pBaseInfo->CurrentPos.byFaceDirection;
	m_timeLastSave					= pBaseInfo->nLastSaveTime;
	m_rcPlayer.RawSetLastLoginTime(pBaseInfo->nLastLoginTime);
	m_timeLogin						= KSysService::Time(NULL);
	m_dwCreateTime					= pBaseInfo->nCreateTime;

	//memcpy(m_wRepresentId, pBaseInfo->wRepresentId, sizeof(m_wRepresentId));
	bRet = TRUE;
EXIT0:
	return bRet;
}

BOOL KPlayerServer::SaveBaseInfo(KRoleBaseInfo* pBaseInfo)
{
	BOOL    bResult         = false;

	pBaseInfo->byLevel                      = (BYTE)m_rcPlayer.GetLevel();
	pBaseInfo->nLastSaveTime                = KSysService::Time(NULL);
	pBaseInfo->CurrentPos.nX                = m_SavePosition.nX;
	pBaseInfo->CurrentPos.nY                = m_SavePosition.nY;
	pBaseInfo->CurrentPos.nZ                = m_SavePosition.nZ;
	pBaseInfo->CurrentPos.dwMapID           = m_SavePosition.dwMapID;
	pBaseInfo->CurrentPos.nMapCopyIndex     = m_SavePosition.nMapCopyIndex;
	pBaseInfo->CurrentPos.byFaceDirection   = (BYTE)m_rcPlayer.m_nFaceDirection;
	pBaseInfo->LastEntry.dwMapID            = m_LastEntry.dwMapID;
	pBaseInfo->LastEntry.nMapCopyIndex      = m_LastEntry.nMapCopyIndex;
	pBaseInfo->LastEntry.nX                 = m_LastEntry.nX;
	pBaseInfo->LastEntry.nY                 = m_LastEntry.nY;
	pBaseInfo->LastEntry.nZ                 = m_LastEntry.nZ;
	pBaseInfo->LastEntry.byFaceDirection    = (BYTE)m_LastEntry.nFaceDirection;
	pBaseInfo->nLastLoginTime               = m_timeLogin;
	pBaseInfo->nCreateTime                  = m_dwCreateTime;

	pBaseInfo->byFaction					= (BYTE)m_rcPlayer.GetPrimaryFaction();
	pBaseInfo->byCamp						= (BYTE)m_rcPlayer.GetCurrentCamp();
	pBaseInfo->cRoleType					= (char)m_rcPlayer.GetSex();
	pBaseInfo->nTotalGameTime				= m_rcPlayer.GetOnlineTime();
	INT* pFightScore =  (INT*)(pBaseInfo->byReserved);
    (*pFightScore) = m_rcPlayer.GetFightScore();   // 加上战斗力信息

	ZeroStruct(pBaseInfo->wRepresentId);

	pBaseInfo->wRepresentId[emKPLAYER_REPRESENT_TYPE_HAIR] = m_rcPlayer.GetHatRepresentId();
	pBaseInfo->wRepresentId[emKPLAYER_REPRESENT_TYPE_BODY] = m_rcPlayer.GetBodyRepresentId();

	//ZeroStruct(pBaseInfo->byReserved);

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KPlayerServer::SaveDelayPetData(KDelayRoleData* pDelayData)
{
	BOOL		bResult         = FALSE;
	KPetCtrl*	pPetCtrl		= NULL;

	ZeroMemory(pDelayData->aryPets, sizeof(pDelayData->aryPets));

	pPetCtrl = m_rcPlayer.GetCtrl<KPetCtrl>();
	LOG_PROCESS_ERROR(pPetCtrl);

	pPetCtrl->SavePetDelayData(pDelayData->aryPets);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerServer::SaveDelayRideData(KDelayRoleData* pDelayData)
{
	BOOL		bResult         = FALSE;
	KRideCtrl*	pRideCtrl		= NULL;

	ZeroMemory(&pDelayData->sRideData, sizeof(pDelayData->sRideData));

	pRideCtrl = m_rcPlayer.GetCtrl<KRideCtrl>();
	LOG_PROCESS_ERROR(pRideCtrl);

	pRideCtrl->SaveRideDelayData(&pDelayData->sRideData);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerServer::SaveDelaySkillData(KDelayRoleData* pDelayData)
{
	BOOL		bResult         = FALSE;
	CharacterSkill*	pCharSkill	= NULL;

	ZeroMemory(&pDelayData->sSkillData, sizeof(pDelayData->sSkillData));

	pCharSkill = m_rcPlayer.GetCtrl<CharacterSkill>();
	LOG_PROCESS_ERROR(pCharSkill);

	pCharSkill->SaveDelaySkillData(&pDelayData->sSkillData);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerServer::SaveDelayRoleData(KDelayRoleData* pDelayData)
{
	BOOL    bResult         = FALSE;

	// 保存基础数据
	SaveBaseDelayData(pDelayData);

	// 保存神仙
	SaveDelayPetData(pDelayData);

	// 保存装备
	SaveEquipDelayData(pDelayData);

	// 保存坐骑
	SaveDelayRideData(pDelayData);

	// 保存技能
	SaveDelaySkillData(pDelayData);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerServer::SaveBaseDelayData(KDelayRoleData* pDelayData)
{
	KDelayBaseData*		pBaseData = &(pDelayData->sBaseRoleData);

	INT srclen  = strlen(m_rcPlayer.GetName())+2;
	strncpy(pBaseData->m_szRoleName,m_rcPlayer.GetName(), srclen);
	pBaseData->m_szRoleName[srclen] = '\0';

	pBaseData->dwRoleId	= m_rcPlayer.m_dwId;
	pBaseData->bySex = (BYTE)m_rcPlayer.GetSex();
	pBaseData->byLevel = (BYTE)m_rcPlayer.GetLevel();
	pBaseData->dwExp = m_rcPlayer.GetExp();
	pBaseData->dwFightScore = m_rcPlayer.GetFightScore();
	
	pBaseData->dwTongId = m_rcPlayer.GetKinId();
	if(pBaseData->dwTongId > 0)
	{
		CONST QKinModule::PLAYER_KIN_INFO& crInfo = g_KinModule.GetPlayerKinInfo((DWORD)m_rcPlayer.GetId());
		srclen = strlen(crInfo.szKinName.c_str()) + 2;
		strncpy(pBaseData->m_szTongName, crInfo.szKinName.c_str() , srclen);
		pBaseData->m_szTongName[srclen] = '\0';
	}
	else
	{
		srclen = strlen(LANG_046) + 2;
		strncpy(pBaseData->m_szTongName, LANG_046, srclen);
		pBaseData->m_szTongName[srclen] = '\0';
	}
	pBaseData->nMaxLife = m_rcPlayer.GetMaxLife();
	pBaseData->nMaxMana = m_rcPlayer.GetMaxMana();
	pBaseData->nAttack = m_rcPlayer.GetAttack();
	pBaseData->nHit	= m_rcPlayer.GetHitCoefficient();
	pBaseData->nDodge = m_rcPlayer.GetDodge();
	pBaseData->nDef = m_rcPlayer.GetDef();
	pBaseData->nCritical = m_rcPlayer.GetCriticalValue();
	pBaseData->nResist = m_rcPlayer.GetResist();
	pBaseData->bySpeed = (BYTE)m_rcPlayer.GetSpeed();

	pBaseData->wHatRepresentId				 = (WORD)m_rcPlayer.GetHatRepresentId();
	pBaseData->wBodyRepresentId				 = (WORD)m_rcPlayer.GetBodyRepresentId();
	pBaseData->wLeftWeaponRepresentId		 = (WORD)m_rcPlayer.GetLeftWeaponRepresentId();
	pBaseData->wRightWeaponRepresentId		 = (WORD)m_rcPlayer.GetRightWeaponRepresentId();
	pBaseData->wBackPendantRepresentId		 = (WORD)m_rcPlayer.GetBackPendantRepresentId();
	pBaseData->wHeadPendantRepresentId		 = (WORD)m_rcPlayer.GetHeadPendantRepresentId();
	pBaseData->wBottomPendantRepresentId	 = (WORD)m_rcPlayer.GetBottomPendantRepresentId();
	pBaseData->wLeftShoulderRepresentId		 = (WORD)m_rcPlayer.GetLeftShoulderRepresentId();
	pBaseData->wRightShoulerRepresentId		 = (WORD)m_rcPlayer.GetRightShoulerRepresentId();
	pBaseData->wRideRepresentId				 = (WORD)m_rcPlayer.GetRideRepresentId();
	pBaseData->wPetRepresentId				 = (WORD)m_rcPlayer.GetPetRepresentId();

	pBaseData->nArenaRank					 = m_rcPlayer.GetArenaRank();
	pBaseData->nRelation					 = m_rcPlayer.GetRelation();
	pBaseData->nCamp						 = m_rcPlayer.GetCurrentCamp();
	pBaseData->nVipLevel = m_rcPlayer.GetVipLevel();
	return TRUE;
}

BOOL KPlayerServer::SaveEquipDelayData(KDelayRoleData* pDelayData)
{
	KPlayerItem* pPlayerItem = m_rcPlayer.GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	pPlayerItem->SaveEquipDelayData(pDelayData);

	return TRUE;
}

BOOL KPlayerServer::SavePosition()
{
	assert(m_rcPlayer.GetScene());
	//assert(m_rcPlayer.m_pCell);

	m_SavePosition.dwMapID         = m_rcPlayer.m_pScene->GetTemplateId();
	m_SavePosition.nMapCopyIndex   = GET_MAP_COPY_INDEX(m_rcPlayer.m_pScene->GetId());
	m_rcPlayer.GetPosition(m_SavePosition.nX, m_SavePosition.nY, m_SavePosition.nZ);
	m_SavePosition.nFaceDirection  = m_rcPlayer.m_nFaceDirection;

	return true;
}

BOOL KPlayerServer::PrepareTeleportTo( INT nMapId, INT nMapCopyIndex, INT nX, INT nY, INT nZ )
{
	m_cDelayedTeleport.dwMapID = nMapId;
	m_cDelayedTeleport.nMapCopyIndex = nMapCopyIndex;
	m_cDelayedTeleport.nX = nX;
	m_cDelayedTeleport.nY = nY;
	m_cDelayedTeleport.nZ = nZ;

	m_pDelayedTeleport = &m_cDelayedTeleport;
	return TRUE;
}

INT KPlayerServer::GetDelayPetsNum(KDelayRoleData* pData)
{
	INT petsNum = 0;

	for(INT i = 0; i < MAX_DELAY_PET_COUNT; ++i)
	{
		if(pData->aryPets[i].wType == 0)
			break;
		++petsNum;
	}
	return petsNum;
}

BOOL KPlayerServer::SyncDelayData(KPlayer* pPlayer, KDelayRoleData* pData)
{
	BOOL bResult = FALSE;
	IMemBlock* piMem = NULL;
	KS2C_SYNC_DELAYROLEDATA* pSync = NULL;
	INT nPetsNum = GetDelayPetsNum(pData);

	piMem = QCreateMemBlock(sizeof(KS2C_SYNC_DELAYROLEDATA) + sizeof(KDelayPetData) * nPetsNum);
	LOG_PROCESS_ERROR(piMem);

	pSync = (KS2C_SYNC_DELAYROLEDATA*)piMem->GetData();
	LOG_PROCESS_ERROR(pSync);

	pSync->byProtocol = emKPBP_S2C_SYNC_DELAYROLEDATA;

	memcpy(&(pSync->sBaseData), &(pData->sBaseRoleData), sizeof(KDelayBaseData));

	for(INT i = 0; i < MAX_DELAY_EQUIP_COUNT; ++i)
	{
		KDelayEquipData* pEquipData = &(pData->aryEquips[i]);
		LOG_PROCESS_ERROR(pEquipData);
		memcpy(&(pSync->arrEquips[i]), pEquipData, sizeof(KDelayEquipData));
	}

	memcpy(&pSync->sRideData, &pData->sRideData, sizeof(KDelayRideData));

	for(INT i = 0; i < MAX_DELAY_PET_COUNT; ++i)
	{
		KDelayPetData* pPetData = &(pData->aryPets[i]);
		LOG_PROCESS_ERROR(pPetData);
		if(pPetData->wType == 0)
			break;
		memcpy(&(pSync->arrPets[i]), pPetData, sizeof(KDelayPetData));
	}

	pSync->byPetCount = (BYTE)nPetsNum;

	bResult = g_pBaseSender->SendLenData(m_rcPlayer.GetPlayerIndex(), *pSync, piMem->GetSize());

EXIT0:
	SAFE_RELEASE(piMem);
	return bResult;
}

BOOL KPlayerServer::OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId)
{
	BOOL bResult = FALSE;
	KDelayRoleData* pDelayData = NULL;

	KPlayer* pPlayer = g_cPlayerMgr.GetById(dwSrcId);
	PROCESS_ERROR(pPlayer);

	pDelayData = (KDelayRoleData*)piBuffer->GetData();

	LOG_PROCESS_ERROR(SyncDelayData(pPlayer, pDelayData));

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerServer::LoginFindSql( DWORD playerID ) 
{
	BOOL						bResult = FALSE;
	BOOL						bRetCode = FALSE;
	S2R_LOAD_ARENA_RECORD_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_LOAD_ARENA_RECORD_REQUEST));
	pRequest = (S2R_LOAD_ARENA_RECORD_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_Login_Find_Sql;
	pRequest->dwRoleId = playerID;
 
	bRetCode = g_RelayClient.Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return bResult;
}

// -------------------------------------------------------------------------
#endif
