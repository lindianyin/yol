#include "stdafx.h"
#include "QuizServerHandler.h"
#include "GameCenter.h"
#include "../rank/krankpool.h"
#include "QuizManager.h"

QuizServerHandler::QuizServerHandler()
{

}

QuizServerHandler::~QuizServerHandler()
{

}

VOID QuizServerHandler::OnNameListRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	INT									nRankIdx	= 0;
	S2R_QUIZ_NAME_LIST_REQUEST*			pRequest	= (S2R_QUIZ_NAME_LIST_REQUEST*)pbyData;

	LOG_PROCESS_ERROR(pRequest);
	DoNameListRespond(nConnIndex);

EXIT0:
	return;
}

VOID QuizServerHandler::OnQuestionListRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	INT									nRankIdx	= 0;
	S2R_QUIZ_QUESTION_REQUEST*			pRequest	= (S2R_QUIZ_QUESTION_REQUEST*)pbyData;
	LOG_PROCESS_ERROR(pRequest);
	DoQuestionListRespond(nConnIndex, pRequest->nIssueId, pRequest->nMaxQuestionCount);


EXIT0:
	return;
}


VOID QuizServerHandler::OnSubmitIntegralRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex)
{
	BOOL								bRetCode	= FALSE;
	INT									nRankIdx	= 0;
	S2R_QUIZ_SUMIT_INTEGRAL_REQUEST*	pRequest	= (S2R_QUIZ_SUMIT_INTEGRAL_REQUEST*)pbyData;

	LOG_PROCESS_ERROR(pRequest);

	g_pGameCenter->m_QuizManager.SetMpRoleIntegral(pRequest->dwRoldId, *pRequest);
	g_pGameCenter->m_QuizManager.ModifyRank(pRequest->dwRoldId, pRequest->nIntegral);

EXIT0:
	return;
}

VOID QuizServerHandler::DoNameListRespond(INT nConnIndex)
{
	BOOL							bRetCode = FALSE;
	BOOL							bResult	 = FALSE;
	R2S_QUIZ_NAME_LIST_RESPOND*		pRespond = NULL;
	IMemBlock*						piPackage = NULL;
	KRankPool&						pRankPool = g_pGameCenter->m_QuizManager.GetRankPool();
	QuizManager						quizManager = g_pGameCenter->m_QuizManager;

	INT nCount = 0;
	for (INT i = 0; i < QUIZ_LIST_COUNT; ++i)
	{
		if (pRankPool.GetIdByIdx(i) <= 0) break;
			nCount++;
	}

	piPackage = QCreateMemBlock(sizeof(R2S_QUIZ_NAME_LIST_RESPOND)+sizeof(QUIZ_RANK_INFO)*nCount);
	pRespond = (R2S_QUIZ_NAME_LIST_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_quiz_name_list_respond;
	pRespond->nCount = nCount;
	for (INT i = 0; i < nCount; ++i)
	{
		pRespond->arrInfo[i].dwRoldId = pRankPool.GetIdByIdx(i);
		pRespond->arrInfo[i].nIntegral = pRankPool.GetValueByIdx(i);
		QStrCpyLen(pRespond->arrInfo[i].szName, g_pGameCenter->m_QuizManager.GetRoleNameById(pRespond->arrInfo[i].dwRoldId), sizeof(pRespond->arrInfo[i].szName));
	}
	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}

VOID QuizServerHandler::DoQuestionListRespond(INT nConnIndex, INT nIssue, INT nMaxQuestionCount)
{
	BOOL							bRetCode = FALSE;
	BOOL							bResult	 = FALSE;
	R2S_QUIZ_QUESTION_RESPOND*		pRespond = NULL;
	IMemBlock*						piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(R2S_QUIZ_QUESTION_RESPOND));
	pRespond = (R2S_QUIZ_QUESTION_RESPOND*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRespond);

	pRespond->wProtocolID = r2s_quiz_qustions_respond;
	INT	arrIssue[QUESTION_COUNT];
	g_pGameCenter->m_QuizManager.DoQuestionListRespond(nIssue, nMaxQuestionCount, arrIssue);
	for (INT i = 0; i < QUESTION_COUNT; ++i)
	{
		pRespond->nQuestion[i] = arrIssue[i];
	}
	bRetCode = Send(nConnIndex, piPackage);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}

BOOL QuizServerHandler::Send( int nConnIndex, IMemBlock* piBuffer )
{
	return g_pGameCenter->m_GameServer.Send(nConnIndex, piBuffer);
}
