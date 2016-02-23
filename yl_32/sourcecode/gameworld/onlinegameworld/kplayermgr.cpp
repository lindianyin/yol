
#ifdef GAME_SERVER
#include "stdafx.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/krequireinterface.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/synchelper.h"
#include "kplayerdbmanager.h"
#include "KRelayClient.h"
#include "knetconnectplayer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KD_BATCH_PROCESS_PLAYER_MAX 100
#define KD_PLAYER_SAVE_INTERVAL  180
#define KD_CLIENT_CONN_TIME_OUT  200

KPlayerMgr g_cPlayerMgr;
// -------------------------------------------------------------------------
KPlayerMgr::KPlayerMgr()
{
	m_timeActivate = 0;
}

BOOL KPlayerMgr::Init( )
{
	const KENV_CONFIG* pConfig = g_piNsBaseRequire->GetEnvConfig();
	QCONFIRM_RET_FALSE(pConfig);

	// 多分配一个，Player索引从1到nMaxPlayer，索引0保留
	m_cObjMgr.Init(pConfig->nMaxPlayer);
	m_mapNameToPlayer.clear();

	return TRUE;
}

BOOL KPlayerMgr::Uninit()
{
	m_cObjMgr.Uninit();
	m_mapNameToPlayer.clear();
	return TRUE;
}

KPlayer* KPlayerMgr::GetByIndex( INT nPlayerIndex )
{
	return m_cObjMgr.GetByIndex(nPlayerIndex);
}

KPlayer* KPlayerMgr::GetById( DWORD dwId )
{
	return m_cObjMgr.GetById(dwId);
}

INT KPlayerMgr::Add(LPCSTR pszName, DWORD dwId /*= KD_BAD_ID*/ )
{
	INT nIndex = m_cObjMgr.Add(dwId);

	KPlayer* pPlayer = m_cObjMgr.GetByIndex(nIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	// TODO: 人物创建时应该先SyncToNeighborPlayers，否则周围玩家的SetVar会失效 kk

	BOOL bOk = pPlayer->Init(nIndex, pszName);
	QCONFIRM_RET_FALSE(bOk && "Player Init Failed!");

	// 名字到对象映射表
	m_mapNameToPlayer.insert(std::map<QString, KPlayer*>::value_type(pPlayer->GetStrName(), pPlayer));

	return nIndex;
}

KPlayer* KPlayerMgr::Add2(LPCSTR pszName, DWORD dwId /*= KD_BAD_ID*/ )
{
	INT nIndex = m_cObjMgr.Add(dwId);

	KPlayer* pPlayer = m_cObjMgr.GetByIndex(nIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	BOOL bOk = pPlayer->Init(nIndex, pszName);
	QCONFIRM_RET_FALSE(bOk && "Player Init Failed!");

	// 名字到对象映射表
	m_mapNameToPlayer.insert(std::map<QString, KPlayer*>::value_type(pPlayer->GetStrName(), pPlayer));

	return pPlayer;
}

BOOL KPlayerMgr::RemoveById( DWORD dwId )
{
	KPlayer* pPlayer = m_cObjMgr.GetById(dwId);
	QCONFIRM_RET_FALSE(pPlayer);

	// 擦除映射表
	m_mapNameToPlayer.erase(pPlayer->GetStrName());
	pPlayer->Uninit();

	return m_cObjMgr.Remove(dwId);
}

BOOL KPlayerMgr::RemoveByIndex( INT nIndex )
{
	KPlayer* pPlayer = m_cObjMgr.GetByIndex(nIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	// 擦除映射表
	m_mapNameToPlayer.erase(pPlayer->GetStrName());
	pPlayer->Uninit();

	return m_cObjMgr.RemoveByIndex(nIndex);
}

BOOL KPlayerMgr::Activate()
{
	static INT nPlayerIndex = 0;
	m_timeActivate = KSysService::GameTime(NULL);

	for (INT i = 0; i < KD_BATCH_PROCESS_PLAYER_MAX; ++i)
	{
		if (nPlayerIndex > m_cObjMgr.GetMaxCount())
		{
			nPlayerIndex = 0;
			break;
		}
		nPlayerIndex = m_cObjMgr.NextUsedIndex(nPlayerIndex);
		if (nPlayerIndex <= 0)
			break;

		KPlayer* pPlayer = m_cObjMgr.GetByIndex(nPlayerIndex);
		QCONFIRM_RET_FALSE(pPlayer);

		PLAYER_GAME_STATE eGameState = pPlayer->GetGameState();
		switch (eGameState)
		{
		case emPLAYERSTATE_BEGIN: // kick out
			break;
		case gsPlaying:
			OnRegularProcess(pPlayer);
			break;
		case gsWaitForConnect:
			// 等待登陆倒计时
			if (pPlayer->m_nTimer++ >= LOGIN_TIMEOUT)
			{
				QLogPrintf(
					LOG_INFO, "Login timeout: acc = %s, role = %s\n", 
					pPlayer->m_cPlayerServer.GetAccount(), pPlayer->GetName()
					);
				g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
			}
			break;

		case gsWaitForPermit:
		case gsWaitForRoleData:
			// 等待登陆倒计时
			if (pPlayer->m_nTimer++ >= LOGIN_TIMEOUT)
			{
				QLogPrintf(
					LOG_INFO, "Relay permission or query role data, kicking (%s, %d)\n", 
					pPlayer->GetName(), pPlayer->GetPlayerIndex()
					);
				g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
			}
			break;

		case gsWaitForLoginLoading:
		case gsWaitForNewMapLoading:
			break;

		case gsDeleting:
			// 掉线倒计时
			if (pPlayer->m_nTimer++ >= LOGOUT_WAITING_TIME/* || !pPlayer->m_bFightState*/)
			{
				//pPlayer->SavePosition();

				//// 这里之所以调RemovePlayer是为了在Save之前触发Trap(如果有的话),Trap可能会涉及存盘角色数据
				//g_pWorld->RemovePlayer(pPlayer);

				//g_RelayClient.SaveRoleData(pPlayer);

				//g_pWorld->m_FellowshipMgr.UnloadPlayerFellowship(pPlayer->m_dwID);

				g_cNetConnectPlayer.KickOutPlayer(pPlayer->GetPlayerIndex());
			}
			break;
		default:
			break;
		}
	}
	return TRUE;
}

KPlayer* KPlayerMgr::GetByName(const QString& strName )
{
	KPlayer* pPlayer = NULL;

	std::map<QString, KPlayer*>::const_iterator it = m_mapNameToPlayer.find(strName);
	if (it != m_mapNameToPlayer.end())
		pPlayer = it->second;

	return pPlayer;
}

BOOL KPlayerMgr::OnRegularProcess( KPlayer* pPlayer )
{
	// 踢掉无响应（ping）玩家
	if (m_timeActivate > pPlayer->m_cPlayerServer.m_timeLastPing + KD_CLIENT_CONN_TIME_OUT)
	{
		tm* tmLocal = localtime(&pPlayer->m_cPlayerServer.m_timeLastPing);
		Q_Printl("LongTimeNoPing [" << pPlayer->GetName() << "] Last Ping " << asctime(tmLocal));
		pPlayer->m_cPlayerServer.KickOut(emKICK_PLAYER_REASON_PING_CHECK);
	}
	// 定时存盘
	if (m_timeActivate > pPlayer->m_cPlayerServer.m_timeLastSave + KD_PLAYER_SAVE_INTERVAL)
	{
		pPlayer->m_cPlayerServer.SavePosition();
		g_RelayClient.SaveRoleData(pPlayer);

		// 存储离线数据
		g_RelayClient.DoSaveDelayRoleData(pPlayer);
	}
	return TRUE;
}

INT KPlayerMgr::GetAllPlayerId(std::vector<DWORD>* pvecPlayerId)
{
	INT nResult = 0;
	INT nPlayerIndex = 0;

	LOG_PROCESS_ERROR(pvecPlayerId && "[KPlayerMgrSys]:非法的参数");
	do 
	{
		nPlayerIndex = m_cObjMgr.NextUsedIndex(nPlayerIndex);
		if (nPlayerIndex > 0)
		{
			KPlayer* pPlayer = m_cObjMgr.GetByIndex(nPlayerIndex);
			LOG_PROCESS_ERROR(pPlayer && "[KPlayerMgrSys]:遍历到未知的玩家");
			pvecPlayerId->push_back(pPlayer->GetId());
		}

	} while ( 0 < nPlayerIndex);

	nResult = 1;
EXIT0:
	return nResult;
}

INT KPlayerMgr::GetAllPlayers(std::vector<KPlayer*>& rvPlayers)
{
	INT nResult = 0;
	INT nPlayerIndex = 0;

	while ((nPlayerIndex = m_cObjMgr.NextUsedIndex(nPlayerIndex)) > 0)
	{
		KPlayer* pPlayer = m_cObjMgr.GetByIndex(nPlayerIndex);
		if (pPlayer->GetGameState() == gsPlaying)
		{
			LOG_PROCESS_ERROR(pPlayer);
			rvPlayers.push_back(pPlayer);
		}
		else
		{
			//QLogPrintf(LOG_INFO, "KPlayerMgr::GetAllPlayers Cull %d %d", pPlayer->GetPlayerId(), pPlayer->GetGameState());
		}
	}

	nResult = 1;
EXIT0:
	return nResult;
}

// -------------------------------------------------------------------------
#endif // #ifdef GAME_SERVER
