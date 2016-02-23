#include "stdafx.h"
#include "kmeditationctrl.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "kmeditationmodule.h"
#include "onlinegamebase/ksysservice.h"

KMeditationCtrl::KMeditationCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{

}

KMeditationCtrl::~KMeditationCtrl()
{

}

BOOL KMeditationCtrl::Init()
{
	m_nStandStateTotalTime = 0;
	memset(&sMeditationInfo, 0, sizeof(sMeditationInfo));
	
	return TRUE;
}

BOOL KMeditationCtrl::OnLogin()
{
	if (sMeditationInfo.dwTakeOfflineAwardTime == 0 || m_rcPlayer.GetLevel() < 22) // 未到开放等级之前需要持续更新,不然会出现今天是21级，明天升到22级时能领离线经验奖励的情况
	{
		sMeditationInfo.dwTakeOfflineAwardTime = KSysService::Time(NULL);
	}
	return TRUE;
}

BOOL KMeditationCtrl::OnLogout()
{
	m_rcPlayer.SetMeditationState(FALSE);
	return TRUE;
}

BOOL KMeditationCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	memcpy(pBuffer, &sMeditationInfo, sizeof(sMeditationInfo));
	uUsedSize += sizeof(sMeditationInfo);
	pBuffer += sizeof(sMeditationInfo);

	return TRUE;
}

BOOL KMeditationCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	BOOL bResult = FALSE;
	ASSERT(uSize == sizeof(sMeditationInfo));
	memcpy(&sMeditationInfo, pData, sizeof(sMeditationInfo));

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KMeditationCtrl::Active()
{
	//by ldy
	//return TRUE;
	
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	//关闭在线打坐面板
	if(m_rcPlayer.GetMeditationState() && 
	  (m_rcPlayer.GetMoveState() != cmsOnSit || m_rcPlayer.GetIsFightMap() > 0 || m_rcPlayer.IsSwim()))
	{
		m_rcPlayer.SetMeditationState(FALSE);
		cSafeScript->CallTableFunction("Meditation", "HideMeditationOnlinePanel", 0, "o", m_rcPlayer.GetScriptInterface());
	}

	//22级开放打坐
	if(m_rcPlayer.GetLevel() < 22 || g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
	{
		goto EXIT0;
	}

	//以下功能每秒中运行一次
	if (m_rcPlayer.GetMoveState() == cmsOnStand && m_rcPlayer.GetIsFightMap() == 0 &&  m_rcPlayer.IsSwim() == FALSE)
	{
		m_nStandStateTotalTime++;
		//超过30s进入打坐状态
		if (m_nStandStateTotalTime >= 30)
		{
			m_rcPlayer.SetMoveState(cmsOnSit);
			m_rcPlayer.SetMeditationState(TRUE);

			//重置脚本中的累计奖励
			cSafeScript->CallTableFunction("Meditation", "ResetOnlineTotalExp", 0, "o", m_rcPlayer.GetScriptInterface());
			//打开在线打坐面板
			cSafeScript->CallTableFunction("Meditation", "ShowMeditationOnlinePanel", 0, "o", m_rcPlayer.GetScriptInterface());
		}
	}
	else
	{
		m_nStandStateTotalTime = 0;
		if (m_rcPlayer.GetMoveState() == cmsOnSit)
		{
			sMeditationInfo.nMeditationTime++;
			//30s发一次奖励
			if(sMeditationInfo.nMeditationTime % 30 == 0)
			{
				//发奖励并更新一下客户端的在线打坐时间
				cSafeScript->CallTableFunction("Meditation", "GiveOnlineAward", 0, "o", m_rcPlayer.GetScriptInterface());
			}
		}
	}

EXIT0:
	return TRUE;
}


//0点重置
VOID KMeditationCtrl::ResetMeditationInfo()
{
	sMeditationInfo.nLastMeditationTime = sMeditationInfo.nMeditationTime;
	sMeditationInfo.nMeditationTime = 0;
}

VOID KMeditationCtrl::SetTakeOfflineAwardTime(DWORD nTime)
{
	sMeditationInfo.dwTakeOfflineAwardTime = nTime;
}

DWORD KMeditationCtrl::GetTakeOfflineAwardTime()
{
	return sMeditationInfo.dwTakeOfflineAwardTime;
}

INT KMeditationCtrl::GetLastMeditationOnlineTime()
{
	return sMeditationInfo.nLastMeditationTime;
}

INT KMeditationCtrl::GetMeditationOnlineTime()
{
	return sMeditationInfo.nMeditationTime;
}

