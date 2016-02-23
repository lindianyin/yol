#ifndef _QUIZ_MANAGER_H__
#define _QUIZ_MANAGER_H__

#include "../rank/krankpool.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"

#define QUESTION_COUNT 30

typedef std::map<DWORD, S2R_QUIZ_SUMIT_INTEGRAL_REQUEST> MP_RoleIntegral;
class QuizManager
{
public:
	QuizManager();
	~QuizManager();
	const static INT m_nRankCount = 10;
public:
	BOOL Init();
	BOOL UnInit();

	VOID SaveAll();

public:
	BOOL ModifyRank(DWORD dwRoldId, DWORD dwValue);

	VOID SetMpRoleIntegral(DWORD dwRoldId, S2R_QUIZ_SUMIT_INTEGRAL_REQUEST &pScore);

	KRankPool& GetRankPool(){ return m_RankPool; }

	CONST MP_RoleIntegral& GetRoleIntegralMap()CONST{ return m_mpRoleScore; }

	CHAR* GetRoleNameById(DWORD dwRoldId);

	VOID SetVecIssue(INT nMaxSize);

	VOID DoQuestionListRespond(INT nIssueId, INT nMaxIssueCount, INT arrIssue[]);

private:
	KRankPool m_RankPool;

	MP_RoleIntegral m_mpRoleScore;

	INT m_nIssueId;
	INT m_nIssueGroupId;

	std::vector<INT> m_vecIssue;
};

#endif