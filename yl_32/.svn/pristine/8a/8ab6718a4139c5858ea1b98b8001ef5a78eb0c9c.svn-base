
/* -------------------------------------------------------------------------
//	文件名		：	quizmanager.h
//	创建者		：	zhengbozhou
//	创建时间	：	2013-12-11
//	功能描述	：  
// -------------------------------------------------------------------------*/

#ifndef __KQUIZMANAGER_H__
#define __KQUIZMANAGER_H__
#include "stdafx.h"
#include "onlinegameworld/korpgworld.h"
#include "quizprotocol.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#define QUESTION_COUNT 30
#define RANK_COUNT 10

struct KQUIZ_ISSUE_RANK
{
	QUIZ_RANK_INFO arrInfo[10];
};

class KQuizManager
{
public:
	KQuizManager();
	~KQuizManager();

public:
	BOOL Init();
	BOOL UnInit();
	BOOL Breath();

private:
	INT m_nIssueId;															//答题id，每次活动+1
	INT m_nCurrentQuizId;													// 正在做的题目 1-30
	std::vector<INT> m_vecQuestion;											//本次答题问题编号
	KQUIZ_ISSUE_RANK m_Rank;

public:
	VOID SetCurrentQuizId(INT nCurrentQuizId);
	INT GetCurrentQuizId();

	VOID SetIssueId(INT nIssueId);
	INT GetIssueId();

	VOID SetVecQuestion(INT nQuestion[]);
	INT GetCurrentIssueValue();

	VOID SetRank(R2S_QUIZ_NAME_LIST_RESPOND* pRank);
	INT GetRank(DWORD dwRoldId);

	BOOL SendRankAward(KPlayer *pPlayer, INT nRank);
};

extern KQuizManager g_cQuizManager;
extern KOrpgWorld g_cOrpgWorld;
#endif