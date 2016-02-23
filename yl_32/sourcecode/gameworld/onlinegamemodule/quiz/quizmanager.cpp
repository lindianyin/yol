#include "stdafx.h"
#include "quizmanager.h"
#include "quizsetting.h"
#include "playerquiz.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamebase/ksysservice.h"

KQuizManager g_cQuizManager;
KQuizManager::KQuizManager()
{

}

KQuizManager::~KQuizManager()
{

}

BOOL KQuizManager::Init()
{
	m_nCurrentQuizId = 0;
	m_nIssueId = 0;
	m_vecQuestion.reserve(QUESTION_COUNT);
	return TRUE;
}

BOOL KQuizManager::UnInit()
{
	return TRUE;
}

BOOL KQuizManager::Breath()
{	
	// 每秒检测一次
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
		return TRUE;

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction( "Quiz", "NotifyQuizIssueAll", 0, "d", m_nCurrentQuizId);

	return TRUE;
}

VOID KQuizManager::SetCurrentQuizId(INT nCurrentQuizId)
{
	m_nCurrentQuizId = MIN(nCurrentQuizId, QUESTION_COUNT);
}

INT KQuizManager::GetCurrentQuizId()
{ 
	return m_nCurrentQuizId; 
}

VOID KQuizManager::SetIssueId(INT nIssueId)
{
	m_nIssueId = nIssueId;
}

INT KQuizManager::GetIssueId()
{
	return m_nIssueId;
}

VOID KQuizManager::SetVecQuestion(INT nQuestion[])
{
	m_vecQuestion.clear();
	for (INT i = 0; i < QUESTION_COUNT; ++i)
	{
		m_vecQuestion.push_back(nQuestion[i]);
	}
}

INT KQuizManager::GetCurrentIssueValue()
{

	if (m_nCurrentQuizId > 0 && m_nCurrentQuizId <= (INT)m_vecQuestion.size())
	{
		return m_vecQuestion[m_nCurrentQuizId-1];
	}
	return 1;
}

VOID KQuizManager::SetRank(R2S_QUIZ_NAME_LIST_RESPOND* pRank)
{

	//先排序
	INT nCount = pRank->nCount;
	for (INT i = 0; i < nCount; ++i)
	{
		for(INT j=nCount -1 ;j > i; j--)
		{
			if(pRank->arrInfo[j].nIntegral > pRank->arrInfo[j-1].nIntegral)
			{
				QUIZ_RANK_INFO temp = pRank->arrInfo[j];
				pRank->arrInfo[j] = pRank->arrInfo[j-1];
				pRank->arrInfo[j-1] = temp;
			}
		}
	}

	for (INT i = 0; i < RANK_COUNT; ++i)
	{
		if (i < pRank->nCount)
		{
			m_Rank.arrInfo[i].dwRoldId = pRank->arrInfo[i].dwRoldId;
			m_Rank.arrInfo[i].nIntegral = pRank->arrInfo[i].nIntegral;
			memcpy(m_Rank.arrInfo[i].szName, pRank->arrInfo[i].szName, sizeof(m_Rank.arrInfo[i].szName));
		}
		else
		{
			m_Rank.arrInfo[i].dwRoldId = 0;
			m_Rank.arrInfo[i].nIntegral = 0;
			//memcpy(m_Rank.arrInfo[i].szName, pRank->arrInfo[i].szName, sizeof(m_Rank.arrInfo[i].szName));
		}
	}
}

INT KQuizManager::GetRank(DWORD dwRoldId)
{
	INT nRank = 0;
	for (INT i = 0; i < RANK_COUNT; ++i)
	{
		if (dwRoldId == m_Rank.arrInfo[i].dwRoldId)
		{
			nRank = i+1;
			break;
		}

	}
	return nRank;
}
