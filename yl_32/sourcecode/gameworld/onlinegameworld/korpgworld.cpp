/* -------------------------------------------------------------------------
//	文件名		：	korpgworld.cpp
//	创建者		：	xiewen
//	创建时间	：	2009-10-23 10:11:52
//	功能描述	：	游戏世界
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include <string>
#include "onlinegameworld/korpgworld.h"
#include "scene/kscenemgr.h"
#include "onlinegameworld/synchelper.h"
#include "config/kconfigfile.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/knpcmgr.h"
#include "onlinegameworld/kdoodadmgr.h"

#include "onlinegameworld/kprotocolsize.h"
#include "kvarsetting.h"
#include "onlinegamebase/ktimermgr.h"

#include "../onlinegamemodule/buffer/buffermanager.h"

#ifdef GAME_CLIENT
#include "onlinegameworld/kplayermanager_client.h"
#include "kplayerbaseprocess_client.h"
#endif // GAME_CLIENT
#ifdef GAME_SERVER
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kregeneration_mgr.h"
#endif // GAME_SERVER
#include "onlinegamebase/ikbadwordfilter.h"
#include "onlinegamebase/performancestat.h"
#include "../onlinegamemodule/record/RecordRelayClient.h"
#include "serverbase/RecordDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KOrpgWorld		g_cOrpgWorld;
QLogFile* g_pScriptLogFile = NULL;

#define GM_NAME_FILE "\\setting\\gmlist.txt"
// -------------------------------------------------------------------------


KOrpgWorld::KOrpgWorld(void)
: m_dwLogicFrames(0)
{
	m_nServerId = 0;
#ifdef GAME_SERVER
	m_bDevMode = FALSE;
	m_nIdleFrame = 0;
#endif // GAME_SERVER
}

KOrpgWorld::~KOrpgWorld(void)
{
}

BOOL KOrpgWorld::Activate()
{
#ifdef GAME_SERVER
	std::vector<DWORD> vPendingUnloadScene = m_vDelayUnloadScene;
	BEGIN_STAT("KOrpgWorld::Activate.UnloadScene()");
	m_vDelayUnloadScene.clear();
	for (std::vector<DWORD>::iterator it = vPendingUnloadScene.begin(); it != vPendingUnloadScene.end(); ++it)
	{
		UnloadScene(*it);
	}
	END_STAT();

	//场景物件重生激活,必需放到场景激活前面
	KRegenerationMgr::GetInstance().Activate();

#endif // GAME_SERVER

	BEGIN_STAT("KOrpgWorld::Activate.g_cTimerMgr.Breath()");
	g_cTimerMgr.Breath();
	END_STAT();

	// 地图场景处理
	BEGIN_STAT("KOrpgWorld::Activate.g_SceneMgr.Activate()");
	g_SceneMgr.Activate();
	END_STAT();

	// 处理玩家游戏状态
	BEGIN_STAT("KOrpgWorld::Activate.g_cPlayerMgr.Activate()");
	g_cPlayerMgr.Activate();
	END_STAT();

	BEGIN_STAT("KOrpgWorld::Activate.g_cDoodadMgr.Activate()");
	g_cDoodadMgr.Activate();
	END_STAT();
	
	

#ifdef GAME_CLIENT
#define KD_PING_INTERVAL 5
	{
		static DWORD timeLastPing = 0;
		DWORD dwTimeNow = KSysService::Time(NULL);
		if (dwTimeNow >= timeLastPing + KD_PING_INTERVAL)
		{
			g_cPlayerBaseProcess.DoPing();
			timeLastPing = dwTimeNow;
		}
	}
	CheckApplySyncNeighbor();
#endif // GAME_CLIENT

EXIT0:
	m_dwLogicFrames++;
	return TRUE;
}

BOOL KOrpgWorld::Init()
{
	BOOL bRet = FALSE;

	m_bDebugSelf = FALSE;

#ifdef GAME_CLIENT
	m_dwClientPlayerId = 0;
	m_dwClientSceneId = 0;
	m_nPing = 0;
	m_dwServerTime = 0;
#endif // GAME_CLIENT

	KPTC_SIZE::Init(); // 网络协议大小初始化
	// g_cBadwordFilter.Init();
	// g_cBadwordFilter.LoadFilterFile(KDF_BAD_WORDS);
//	g_pBadwordFilter->Init();
//	g_pBadwordFilter->LoadFilterFile(KDF_BAD_WORDS);

	g_cScriptManager.SetMe(NULL);
	g_cScriptManager.SetHim(NULL);
	g_cScriptManager.SetIt(NULL);

#ifdef GAME_SERVER
	g_cCharacterVarSetting.LoadBaseVardef();
	g_cCharacterVarSetting.LoadTaskDef(KDF_CHARACTER_VAR_DEF);
	LoadGmList();
#endif // GAME_SERVER

	bRet = m_cEventCreator.Init();
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cNpcMgr.Init();
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cDoodadMgr.Init();
	QCONFIRM_RET_FALSE(bRet);

	QCONFIRM_RET_FALSE(g_pSceneMgr);
	bRet = g_pSceneMgr->Init();
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cPlayerMgr.Init();
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cTimerMgr.Init();	// 为Lua提供服务
	QCONFIRM_RET_FALSE(bRet);

	bRet = InitServerStartTime();
	QCONFIRM_RET_FALSE(bRet);

	return TRUE;
}

BOOL KOrpgWorld::InitServerStartTime()
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;

	bRetCode = RecordRelayClient::DoGetServerRecordRequest(emRecordServerStartTime);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KOrpgWorld::UnInit()
{
	g_cTimerMgr.UnInit();

#ifdef GAME_SERVER
	KRegenerationMgr::GetInstance().Clear();
#endif // GAME_SERVER

	g_SceneMgr.Uninit();

	g_cDoodadMgr.Uninit();
	g_cNpcMgr.Uninit();
	g_cPlayerMgr.Uninit();
	// g_cBadwordFilter.Uninit();
//	g_pBadwordFilter->Uninit();

#ifdef GAME_SERVER
	m_vDelayUnloadScene.clear();
#endif
#ifdef GAME_CLIENT
	m_dwClientPlayerId = 0;
	m_dwClientSceneId = 0;
	while (!m_vApplySyncNeighbor.empty())
		m_vApplySyncNeighbor.pop();
#endif

	g_cScriptManager.SetMe(NULL);
	g_cScriptManager.SetHim(NULL);
	g_cScriptManager.SetIt(NULL);

	return TRUE;
}

BOOL KOrpgWorld::UnloadScene(INT nMapId, INT nMapCopyIndex/* = 0*/)
{
	QCONFIRM_RET_FALSE(g_pSceneMgr);

#ifdef GAME_SERVER
	Q_Printl("KOrpgWorld::UnloadScene [" << nMapId << ","<<nMapCopyIndex<<"] & expel players.");

	IKScene* pScene = g_pSceneMgr->GetScene(nMapId, nMapCopyIndex);
	QCONFIRM_RET_FALSE(pScene && "No such scene!");

	// 将此场景中的玩家赶走
	TKTraverseHelper<KPlayer> cTravHelper;
	pScene->TraverseAllPlayers(cTravHelper);
	for(std::vector<KPlayer*>::iterator it = cTravHelper.m_vObjList.begin();
		it != cTravHelper.m_vObjList.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		QCONFIRM_CONTINUE(pPlayer);

		pPlayer->m_cPlayerServer.ReturnToLastCity();
		if (pPlayer->GetGameState() != gsSearchMap) 
		{
			Q_Error("Unable to send [" << pPlayer->GetName() << "] back to city, kickout...");
			pPlayer->m_cPlayerServer.KickOut(emKICK_PLAYER_REASON_BUG_CHECK);
		}
	}
#endif // GAME_SERVER

	return g_pSceneMgr->UnloadScene(nMapId, nMapCopyIndex);
}

BOOL KOrpgWorld::RemovePlayer( DWORD dwId )
{
	return g_cPlayerMgr.RemoveById(dwId);
}

KNpc* KOrpgWorld::AddNpc(DWORD dwTemplateId, DWORD dwId /*= KD_BAD_ID*/ )
{
	KNpc* pNpc = NULL;
	INT nIndex = g_cNpcMgr.Add(dwTemplateId, dwId);

	pNpc = g_cNpcMgr.GetByIndex(nIndex);
	QCONFIRM_RET_NULL(pNpc);

	return pNpc;
}

BOOL KOrpgWorld::RemoveNpc( DWORD dwId, BOOL bRegenerate)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;

	// 删除npc加的对地buff
	bRetCode = g_cBufferMgr.RemoveGroundBufferByCasterId(dwId);
	LOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_cNpcMgr.Remove(dwId, bRegenerate);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	return bResult;
}

#ifdef GAME_SERVER
// 临时GM判断
std::map<std::string , BOOL > g_mapGmAccount;
BOOL KOrpgWorld::IsGm( KPlayer* pPlayer )
{
	// 开发模式（DevMode）下，都是GM
	if (m_bDevMode)
		return TRUE;

	QCONFIRM_RET_FALSE(pPlayer);

	std::map<std::string , BOOL>::iterator it = 
		g_mapGmAccount.find((LPCSTR)pPlayer->m_cPlayerServer.GetAccount());
	return it != g_mapGmAccount.end();

	return TRUE;
}

BOOL KOrpgWorld::LoadGmList()
{
	QTabFile cTabFile;
	BOOL bRet = cTabFile.Load(GM_NAME_FILE);
	g_mapGmAccount.clear();
	if (bRet)
	{
		INT nLines = cTabFile.GetHeight();

		// 第一行是英文字段头描述
		for (INT n = 2; n <= nLines; ++n)
		{
			CHAR szBuf[MAX_NAME_LEN] = "";

			bRet = cTabFile.GetString(n, "Account", "", szBuf, countof(szBuf));
			QCONFIRM(bRet && "gm list file incorrect.");

			QCONFIRM(g_mapGmAccount.insert(std::make_pair(szBuf, TRUE)).second &&
				"duplicate gm accounts");
		}
	}
	else
	{
		Q_Printl("gm list not exist");
	}
	return TRUE;
}

BOOL KOrpgWorld::UnloadSceneDelay( DWORD dwId )
{
	m_vDelayUnloadScene.push_back(dwId);
	return TRUE;
}

BOOL KOrpgWorld::OnGetServerStartTimeRespond(BYTE* pRecordData, size_t uRecordDataLen)
{
	BOOL		bResult = FALSE;

	LOG_PROCESS_ERROR(uRecordDataLen == sizeof(DWORD));

	memcpy(&m_dwServerStartTime, pRecordData, sizeof(DWORD));
	QLogPrintf(LOG_INFO,"record OnGetServerStartTimeRespond %d",m_dwServerStartTime);
	bResult = TRUE;
EXIT0:
	return bResult;
}

#endif // GAME_SERVER

#ifdef GAME_CLIENT
KPlayer* KOrpgWorld::GetClientPlayer() const
{
	return g_cPlayerMgr.GetById(m_dwClientPlayerId);
}

VOID KOrpgWorld::CheckApplySyncNeighbor()
{
	if ( ! m_vApplySyncNeighbor.empty())
	{
		KSceneObject* pSceneObj = NULL;
		DWORD& dwSceneObjectId = m_vApplySyncNeighbor.front();

		if (IS_DOODAD(dwSceneObjectId))
			pSceneObj = g_cDoodadMgr.GetById(dwSceneObjectId);
		else if (IS_NPC(dwSceneObjectId))
			pSceneObj = g_cNpcMgr.GetById(dwSceneObjectId);
		else if (IS_PLAYER(dwSceneObjectId))
			pSceneObj = g_cPlayerMgr.GetById(dwSceneObjectId);
		else
			QLogPrintf(LOG_ERR, "CheckApplySyncNeighbor unknown id 0x%x", dwSceneObjectId);

		if (pSceneObj == NULL)
			g_cPlayerBaseProcess.DoApplySyncNeighbor(dwSceneObjectId);

		m_vApplySyncNeighbor.pop();
	}
}

void InitStartTimeEvaluation(KStartTimeEvaluation* pStartTimeEvaluation)
{
    pStartTimeEvaluation->nFirstEvaluationGameLoop = 0;
    pStartTimeEvaluation->dwFirstStartTime = 0;
    pStartTimeEvaluation->dwMinFirstStartTime = 0;
    pStartTimeEvaluation->lfFirstStartTime = 0.0;
    pStartTimeEvaluation->lfNetStartTime = 0.0;
}

BOOL UpdateStartTimeEvaluation(KStartTimeEvaluation* pStartTimeEvaluation, INT nFrame)
{
	DWORD dwTimeNow = 0;
    DWORD dwNewStartTime = 0;
    INT nNewStartTimeDelta = 0;
	DOUBLE lfStartTimeDelta = 0.0;
    DOUBLE lfTimeNow = 0.0;
    INT nGameLoop = 0;
    KPlayer* pPlayer = NULL;
    KE_MOVE_STATE eMoveState = cmsInvalid;
    BOOL bAdjust = FALSE;
	static UINT uIncreaseCount = 0;

    pPlayer = g_cOrpgWorld.GetClientPlayer();
    PROCESS_ERROR(pPlayer);
	
    nGameLoop = nFrame - g_cOrpgWorld.m_nStartLoop;

	dwTimeNow = timeGetTime();

    lfTimeNow = dwTimeNow;

	dwNewStartTime = (DWORD)(dwTimeNow - (DOUBLE)nGameLoop * 1000 / GAME_FPS);	

    if (pStartTimeEvaluation->dwFirstStartTime == 0)
    {
        pStartTimeEvaluation->nFirstEvaluationGameLoop = nGameLoop;
        pStartTimeEvaluation->lfFirstStartTime = lfTimeNow;
        pStartTimeEvaluation->dwFirstStartTime = dwTimeNow;
        pStartTimeEvaluation->dwMinFirstStartTime = dwNewStartTime;
    }

    // Plan A
    if (dwNewStartTime < pStartTimeEvaluation->dwMinFirstStartTime)
    {
        pStartTimeEvaluation->dwMinFirstStartTime = dwNewStartTime;
        bAdjust = TRUE;
    }

    eMoveState = pPlayer->m_eMoveState;

    if (nGameLoop - pStartTimeEvaluation->nFirstEvaluationGameLoop > GAME_FPS * 4 && eMoveState != KSceneObject::emKMOVESTATE_FLY)
    {
        // 前四秒或者飞行状态不使用插值修正

        bAdjust = FALSE;
		
        if (eMoveState == cmsOnStand)
        {
			// 待机时间太长，会出现角色像僵尸般的抖动，原因是客户端时间出现了误差，表现的插值时间，一直没落在逻辑帧缓冲内（或者说表现帧追上了逻辑帧）
			// 现在的做法是，让客户端每2个小时重设一次启动时间
			if (uIncreaseCount >= 2*GAME_FPS*60*60)
			{
				uIncreaseCount = 0;
				pStartTimeEvaluation->nFirstEvaluationGameLoop = nGameLoop;
				pStartTimeEvaluation->lfFirstStartTime = lfTimeNow;
				pStartTimeEvaluation->dwFirstStartTime = dwTimeNow;
				pStartTimeEvaluation->dwMinFirstStartTime = dwNewStartTime;
				pStartTimeEvaluation->lfNetStartTime = 0;
			}

            // Plan B
            lfStartTimeDelta = lfTimeNow - (DOUBLE)nGameLoop * 1000 / GAME_FPS - pStartTimeEvaluation->lfFirstStartTime;

            pStartTimeEvaluation->lfNetStartTime = pStartTimeEvaluation->lfNetStartTime * 0.9 + lfStartTimeDelta * 0.1;
            dwNewStartTime = (DWORD)pStartTimeEvaluation->lfNetStartTime + pStartTimeEvaluation->dwFirstStartTime;

            nNewStartTimeDelta = (INT)dwNewStartTime - (INT)pStartTimeEvaluation->dwMinFirstStartTime;

            if (g_cOrpgWorld.m_dwStartTime != dwNewStartTime)
            {
                if (nNewStartTimeDelta < 0)
                    dwNewStartTime = pStartTimeEvaluation->dwMinFirstStartTime;
                else if (nNewStartTimeDelta > 1000)
                    dwNewStartTime = pStartTimeEvaluation->dwMinFirstStartTime + 1000;

                bAdjust = TRUE;
            }
        }
    }

    if (bAdjust)
    {
        g_cOrpgWorld.m_dwStartTime = dwNewStartTime;
    }

	uIncreaseCount++;
EXIT0:
    return bAdjust;
}

BOOL UpdateStartTimeEvaluation2(KStartTimeEvaluation* pStartTimeEvaluation, INT nFrame)
{
	int nRetCode = false;
	int nResult = false;
	DWORD dwNewStartTime  = 0;
	double lfNewNetStartTime = 0.0;
	double lfTimeNow = 0.0;
	double lfNewNetStartTimeWeight = 0.0;
	int nGameLoop = 0;

	LOG_PROCESS_ERROR(pStartTimeEvaluation);

	nGameLoop = nFrame - g_cOrpgWorld.m_nStartLoop;

	lfTimeNow = timeGetTime();

	if (pStartTimeEvaluation->dwFirstStartTime == 0)
	{
		pStartTimeEvaluation->lfFirstStartTime = lfTimeNow;
		pStartTimeEvaluation->nFirstEvaluationGameLoop = 0;
		pStartTimeEvaluation->dwFirstStartTime = (DWORD)lfTimeNow;
	}

	lfNewNetStartTime = lfTimeNow - (double)nGameLoop * 1000 / GAME_FPS - pStartTimeEvaluation->lfFirstStartTime;

	if (pStartTimeEvaluation->nFirstEvaluationGameLoop++ < GAME_FPS * 2)
		lfNewNetStartTimeWeight = 0.5;
	else
		lfNewNetStartTimeWeight = 0.1;

	pStartTimeEvaluation->lfNetStartTime = pStartTimeEvaluation->lfNetStartTime * (1.0 - lfNewNetStartTimeWeight) + lfNewNetStartTime * lfNewNetStartTimeWeight;

	dwNewStartTime = (DWORD)pStartTimeEvaluation->lfNetStartTime + pStartTimeEvaluation->dwFirstStartTime;

	if (g_cOrpgWorld.m_dwStartTime != dwNewStartTime)
	{
		g_cOrpgWorld.m_dwStartTime = dwNewStartTime;
	}

	nResult = true;
EXIT0:
	return nResult;
}

#endif // GAME_CLIENT
