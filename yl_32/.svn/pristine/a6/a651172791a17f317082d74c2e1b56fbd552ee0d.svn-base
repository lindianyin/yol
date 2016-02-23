#include "stdafx.h"
#include "ktowerctrl.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "ktowerprotocol.h"
#include "ktowermodule.h"
#include "onlinegamebase/ksysservice.h"

KTowerCtrl::KTowerCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KTowerCtrl::~KTowerCtrl()
{

}

BOOL KTowerCtrl::Init()
{
	ResetMopInfo();

	return TRUE;
}

BOOL KTowerCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	memcpy(pBuffer, &sMopInfo, sizeof(sMopInfo));
	uUsedSize += sizeof(sMopInfo);
	pBuffer += sizeof(sMopInfo);

	return TRUE;
}

BOOL KTowerCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bResult = FALSE;
	ASSERT(uSize == sizeof(sMopInfo));
	memcpy(&sMopInfo, pData, sizeof(sMopInfo));

	LOG_PROCESS_ERROR(SyncTowerInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KTowerCtrl::Active()
{
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
	{
		goto EXIT0;
	}

	PROCESS_ERROR(sMopInfo.nState == emMopStateDoing);

	if(sMopInfo.nLeftTime > 0)
	{
		--sMopInfo.nLeftTime;
		if(sMopInfo.nLeftTime <= 0)
		{
			OnFinishMop();
		}
	}

EXIT0:
	return TRUE;
}

BOOL KTowerCtrl::OnFinishMop()
{
	BOOL bResult = FALSE;
	sMopInfo.nState = emMopStateDone;
	// 已经扫荡到最高层了
	sMopInfo.nLeftTime = 0;

	LOG_PROCESS_ERROR(SyncTowerInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KTowerCtrl::StartMopTower(INT nMopTime)
{
	BOOL bResult = FALSE;
	time_t tNow = KSysService::Time(NULL);

	LOG_PROCESS_ERROR(sMopInfo.nState == emMopStateNone);

	sMopInfo.nState = emMopStateDoing;
	sMopInfo.nLeftTime = nMopTime;
	sMopInfo.dwStartMopTime = (DWORD)tNow;

	LOG_PROCESS_ERROR(SyncTowerInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KTowerCtrl::StopMopTower(INT nCurLayer)
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(sMopInfo.nState == emMopStateDoing);

	if(nCurLayer)
	{
		sMopInfo.nState = emMopStateDone;
	}
	else
	{
		// 连一层都没扫完
		sMopInfo.nState = emMopStateNone;
	}

	m_rcPlayer.SetMopState(0); // 清空扫荡状态
	LOG_PROCESS_ERROR(SyncTowerInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KTowerCtrl::OnSpeedUp()
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(sMopInfo.nState == emMopStateDoing);

	sMopInfo.nLeftTime = sMopInfo.nLeftTime / 2;
	sMopInfo.bSpeedUp = TRUE;

	LOG_PROCESS_ERROR(SyncTowerInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KTowerCtrl::OnTakeAward()
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(sMopInfo.nState == emMopStateDone);

	ResetMopInfo();

	LOG_PROCESS_ERROR(SyncTowerInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KTowerCtrl::ResetMopInfo()
{
	memset(&sMopInfo, 0, sizeof(sMopInfo));
}

BOOL KTowerCtrl::SyncTowerInfo()
{
	KS2C_SYNC_MOP_INFO sSync;
	sSync.bSpeedUp = sMopInfo.bSpeedUp;
	sSync.nLeftTime = sMopInfo.nLeftTime;
	sSync.nState = sMopInfo.nState;

	return g_cTowerModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}