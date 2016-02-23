#include "stdafx.h"
#include "onlinegamemodule/kprogressbar.h"
#include "onlinegameworld/korpgworld.h"


BOOL KPlayerProgressBar::Init()
{
	m_dwFinishFrame = 0;
	return TRUE;
}

BOOL KPlayerProgressBar::UnInit()
{
	CancelProgressBar();
	return TRUE;
}

BOOL KPlayerProgressBar::CancelProgressBar()
{
	m_dwFinishFrame = 0;
	return TRUE;
}

INT KPlayerProgressBar::OnTimer()
{
	// 走完了进度条
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	QLunaBase* pOldMe = g_cScriptManager.SetMe((QLunaBase*)m_rcCharacter.GetScriptInterface());

	cSafeScript->CallTableFunction("ProgressBar", "OnProgressFull", 0, "u", m_rcCharacter.m_dwId);

	g_cScriptManager.SetMe(pOldMe);

	m_dwFinishFrame = 0;

	return 0;
}

BOOL KPlayerProgressBar::StartProgressBar(INT nIntervalFrame)
{
	ASSERT(nIntervalFrame > 0);

	m_dwFinishFrame = nIntervalFrame + g_cOrpgWorld.m_dwLogicFrames;

	return TRUE;
}

BOOL KPlayerProgressBar::Active()
{
	if (m_dwFinishFrame > 0)
	{
		if (g_cOrpgWorld.m_dwLogicFrames >= m_dwFinishFrame)
		{
			OnTimer();
		}
	}
	return TRUE;

}

