#include "stdafx.h"
#include "kstagectrl.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "kstageprotocol.h"
#include "kstagemodule.h"

KStageCtrl::KStageCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KStageCtrl::~KStageCtrl()
{

}

BOOL KStageCtrl::Init()
{
	ResetMopInfo();
	ResetRushInfo();

	return TRUE;
}

BOOL KStageCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	memcpy(pBuffer, &sRushInfo, sizeof(sRushInfo));
	uUsedSize += sizeof(sRushInfo);
	pBuffer += sizeof(sRushInfo);

	memcpy(pBuffer, &sMopInfo, sizeof(sMopInfo));
	uUsedSize += sizeof(sMopInfo);
	pBuffer += sizeof(sMopInfo);

	return TRUE;
}

BOOL KStageCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bResult = FALSE;
	ASSERT(uSize == sizeof(sRushInfo) + sizeof(sMopInfo));

	LPCBYTE pBuffer = pData;
	memcpy(&sRushInfo, pBuffer, sizeof(sRushInfo));

	pBuffer += sizeof(sRushInfo);

	memcpy(&sMopInfo, pBuffer, sizeof(sMopInfo));

	SyncStageInfo();

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KStageCtrl::Active()
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

INT KStageCtrl::GetStageStar(INT nDifficulty)
{
	ASSERT(nDifficulty >= 1 && nDifficulty <= MAX_STAGE_DIFICULTY);
	return sRushInfo.aryStars[nDifficulty-1];
}

BOOL KStageCtrl::SetStageStar(INT nDifficulty, INT nStarLevel)
{
	ASSERT(nStarLevel >= 1 && nStarLevel <= MAX_STAGE_STAR);

	INT nStar = GetStageStar(nDifficulty);
	if(nStar < nStarLevel)
	{
		sRushInfo.aryStars[nDifficulty-1] = nStarLevel;
	}

	QCONFIRM_RET_FALSE(SyncStageInfo());

	return TRUE;
}

BOOL KStageCtrl::OnFinishMop()
{
	BOOL bResult = FALSE;
	sMopInfo.nState = emMopStateDone;
	// 已经扫荡到最高层了
	sMopInfo.nLeftTime = 0;

	LOG_PROCESS_ERROR(SyncStageInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KStageCtrl::StartMopStage(INT nMopTime)
{
	BOOL bResult = FALSE;

	LOG_PROCESS_ERROR(sMopInfo.nState == emMopStateNone);

	sMopInfo.nState = emMopStateDoing;
	sMopInfo.nLeftTime = nMopTime;

	LOG_PROCESS_ERROR(SyncStageInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KStageCtrl::StopMopStage(INT bFinish)
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(sMopInfo.nState == emMopStateDoing);

	if(bFinish)
	{
		sMopInfo.nState = emMopStateDone;
	}
	else
	{
		// 连一层都没扫完
		sMopInfo.nState = emMopStateNone;
	}

	LOG_PROCESS_ERROR(SyncStageInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KStageCtrl::OnTakeAward()
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(sMopInfo.nState == emMopStateDone);

	ResetMopInfo();

	LOG_PROCESS_ERROR(SyncStageInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KStageCtrl::OnSpeedUp()
{
	BOOL bResult = FALSE;
	LOG_PROCESS_ERROR(sMopInfo.nState == emMopStateDoing);

	sMopInfo.nLeftTime = sMopInfo.nLeftTime / 2;
	sMopInfo.bSpeedUp = TRUE;

	LOG_PROCESS_ERROR(SyncStageInfo());

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KStageCtrl::ResetRushInfo()
{
	memset(&sRushInfo, 0, sizeof(sRushInfo));
}

VOID KStageCtrl::ResetMopInfo()
{
	memset(&sMopInfo, 0, sizeof(sMopInfo));
}

BOOL KStageCtrl::SyncStageInfo()
{
	KS2C_SYNC_STAGE_INFO sSync;

	for(INT i = 0; i < MAX_STAGE_DIFICULTY; ++i)
	{
		sSync.aryStars[i] = sRushInfo.aryStars[i];
	}

	sSync.nCurMopStage = sMopInfo.nMopStage;
	sSync.nRushState = sRushInfo.nState;

	sSync.nLeftTime = sMopInfo.nLeftTime;
	sSync.nState = sMopInfo.nState;
	sSync.bySpeedUp = sMopInfo.bSpeedUp;
	
	return g_cStageModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}