
#include "stdafx.h"
#include "ktaskmodule.h"
#include "onlinegamemodule/ktaskctrl.h"
#include "ktasksetting.h"

#ifdef GAME_SERVER
#include "onlinegameworld/kplayermgr.h"
#else
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kplayermanager_client.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------任务--------------------------
#define F_TASK_SETTING_PLAYER			"setting/task/player_task_def.txt"

// -------------------------------------------------------------------------

KTaskModule g_sTaskModule;

template<>
IKModuleInterface* GetModuleInterFace<emKMODULE_TASK>()
{
	return &g_sTaskModule;
}

KTaskModule::KTaskModule() : KModuleBase(emKMODULE_TASK)
{}

BOOL KTaskModule::OnInit()
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	BOOL bOK = FALSE;
	bOK = g_PlayerTaskSetting.LoadTaskDef(F_TASK_SETTING_PLAYER);
	CHECK_REPORT(bOK);

	bOK = cSafeScript->CallTableFunction("Task", "OnInit", 1, "");

EXIT0:
	return bOK;
}

BOOL KTaskModule::OnUnInit()
{
	return TRUE;
}

BOOL KTaskModule::Breath()
{
	return TRUE;
}

#ifdef GAME_SERVER
BOOL KTaskModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
#else
BOOL KTaskModule::ProcessProtocol(LPCBYTE pbData, UINT uDataLen)
#endif
{
	ASSERT(pbData != NULL);
	ASSERT(uDataLen > 0);

	// 当前玩家	
#ifdef GAME_SERVER
	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
#else
	KPlayer* pPlayer = g_cPlayerMgr.GetById(KD_CLIENT_PLAYER_ID);
#endif
	QCONFIRM_RET_FALSE(pPlayer);

	KTaskCtrl* pPlayerTask = pPlayer->GetCtrl<KTaskCtrl>();
	QCONFIRM_RET_FALSE(pPlayerTask);

	pPlayerTask->ProcessProtocol(pbData, uDataLen);
	return TRUE;
}

BOOL KTaskModule::InitProtocol()
{
	KD_PTC_LENGTNED_SIZE_S2C(S2C_SYNC_TASKGROUP);
	KD_PTC_FIXED_SIZE_S2C(TASK_SYNC_EVENT);
	KD_PTC_FIXED_SIZE_S2C(TASK_SYNC_TASK);

	KD_PTC_FIXED_SIZE_C2S(TASK_SYNC_EVENT);
	KD_PTC_FIXED_SIZE_C2S(TASK_SYNC_TASK);
	return true;
}

// -------------------------------------------------------------------------
