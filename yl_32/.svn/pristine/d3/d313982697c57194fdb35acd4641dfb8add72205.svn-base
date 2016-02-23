#include "stdafx.h"
#include "QuizManager.h"
#include <algorithm>
#include "onlinegamebase/ksysservice.h"

QuizManager::QuizManager()
{

}

QuizManager::~QuizManager()
{

}

BOOL QuizManager::Init()
{
	m_RankPool.Init(m_nRankCount);
	m_nIssueId = -1;
	m_nIssueGroupId = -1;
	return TRUE;
}

BOOL QuizManager::UnInit()
{
	SaveAll();
	m_RankPool.UnInit();
	return TRUE;
}

VOID QuizManager::SaveAll()
{

}

BOOL QuizManager::ModifyRank(DWORD dwRoldId, DWORD dwValue)
{
	time_t tNow = KSysService::Time(NULL);
	m_RankPool.ModifyRankValue(dwRoldId, dwValue,tNow);
	return TRUE;
}

VOID QuizManager::SetMpRoleIntegral(DWORD dwRoldId, S2R_QUIZ_SUMIT_INTEGRAL_REQUEST &pScore)
{
	m_mpRoleScore[dwRoldId] = pScore;
}

CHAR* QuizManager::GetRoleNameById(DWORD dwRoldId)
{
	if (m_mpRoleScore.find(dwRoldId) != m_mpRoleScore.end())
		return m_mpRoleScore[dwRoldId].szName;
	return NULL;
}

VOID QuizManager::SetVecIssue(INT nMaxSize)
{
	m_vecIssue.clear();
	for (INT i = 1; i <= nMaxSize; ++i)
	{
		m_vecIssue.push_back(i);
	}
	std::srand(time(NULL));
	random_shuffle(m_vecIssue.begin(), m_vecIssue.end());
}

VOID QuizManager::DoQuestionListRespond(INT nIssueId, INT nMaxIssueCount, INT arrIssue[])
{
	m_RankPool.UnInit();
	m_RankPool.Init(m_nRankCount);
	if (nIssueId != m_nIssueId)
	{
		m_nIssueGroupId++;
		m_nIssueId = nIssueId;
	}

	if ((m_nIssueGroupId+1) * QUESTION_COUNT > m_vecIssue.size())
	{
		m_nIssueGroupId = 0;
		SetVecIssue(nMaxIssueCount);
	}

	for (INT i = 0; i < QUESTION_COUNT; ++i)
	{
		arrIssue[i] = m_vecIssue[m_nIssueGroupId*QUESTION_COUNT+i];
	}

}