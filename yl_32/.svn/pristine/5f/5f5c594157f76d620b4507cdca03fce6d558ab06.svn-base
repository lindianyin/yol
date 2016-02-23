#include "stdafx.h"
#include "QuizRelayClient.h"
#include "../onlinegameworld/KRelayClient.h"
#include "onlinegameworld/kplayermgr.h"
#include "playerquiz.h"
#include "quizprotocol.h"
#include "quizmanager.h"

QuizRelayClient::QuizRelayClient()
{

}

QuizRelayClient::~QuizRelayClient()
{

}

VOID QuizRelayClient::DoNameListRequest(INT nRequest)
{
	BOOL						bRetCode = FALSE;
	S2R_QUIZ_NAME_LIST_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_QUIZ_NAME_LIST_REQUEST));
	pRequest = (S2R_QUIZ_NAME_LIST_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_quiz_name_list_request;
	pRequest->nRequest = nRequest;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}

VOID QuizRelayClient::DoQuestionListRequest(INT nIssueId, INT nMaxQuestionCount)
{
	BOOL						bRetCode			= FALSE;
	S2R_QUIZ_QUESTION_REQUEST*	pRequest			= NULL;
	IMemBlock*					piPackage			= NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_QUIZ_QUESTION_REQUEST));
	pRequest = (S2R_QUIZ_QUESTION_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_quiz_questions_request;
	pRequest->nIssueId = nIssueId;
	pRequest->nMaxQuestionCount = nMaxQuestionCount;
	//pRequest->nQuestionCount = QUESTION_COUNT;

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}

VOID QuizRelayClient::DoSumitScoreRequest(DWORD dwRoldId, INT nIntegral,CONST CHAR* szName)
{
	BOOL						bRetCode = FALSE;
	S2R_QUIZ_SUMIT_INTEGRAL_REQUEST*	pRequest = NULL;
	IMemBlock*					piPackage = NULL;

	piPackage = QCreateMemBlock(sizeof(S2R_QUIZ_SUMIT_INTEGRAL_REQUEST));
	pRequest = (S2R_QUIZ_SUMIT_INTEGRAL_REQUEST*)piPackage->GetData();
	LOG_PROCESS_ERROR(pRequest);

	pRequest->wProtocolID = s2r_quiz_sumit_score_request;
	pRequest->dwRoldId = dwRoldId;
	pRequest->nIntegral = nIntegral;
	memcpy(pRequest->szName,szName,sizeof(CHAR)*MAX_NAME_LEN);

	bRetCode = Send(piPackage);
	LOG_PROCESS_ERROR(bRetCode);

EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}

VOID QuizRelayClient::OnNameListRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_QUIZ_NAME_LIST_RESPOND*		pRespond		= NULL;
	IMemBlock*						piPackage		= NULL;
	std::vector<KPlayer*>			vPlayers;
	QUIZ_S2C_ISSUE_RANKING* pIssueRank				= NULL;
	pRespond = (R2S_QUIZ_NAME_LIST_RESPOND*)pbyData;
	LOG_PROCESS_ERROR(pRespond);

	piPackage = QCreateMemBlock(sizeof(QUIZ_S2C_ISSUE_RANKING)+sizeof(QUIZ_ISSUE_RANK_INFO)*pRespond->nCount);
	pIssueRank = (QUIZ_S2C_ISSUE_RANKING*)piPackage->GetData();
	LOG_PROCESS_ERROR(pIssueRank);

	g_cQuizManager.SetRank(pRespond);

	pIssueRank->nCount = pRespond->nCount;
	for (int i = 0; i < pIssueRank->nCount; ++i)
	{
		pIssueRank->arrInfo[i].nIntegral = pRespond->arrInfo[i].nIntegral;
		memcpy(pIssueRank->arrInfo[i].szRoleName, pRespond->arrInfo[i].szName,sizeof(pIssueRank->arrInfo[i].szRoleName));
	}
	g_cPlayerMgr.GetAllPlayers(vPlayers);
	for (std::vector<KPlayer*>::iterator it = vPlayers.begin(); it != vPlayers.end(); ++it)
	{
		KPlayer* pPlayer = *it;
		PlayerQuiz *pPlayerQuiz = pPlayer->GetCtrl<PlayerQuiz>();
		pPlayerQuiz->SendIssueRanking(pIssueRank);
	}

EXIT0:
	SAFE_RELEASE(piPackage);
	return;
}

VOID QuizRelayClient::OnQuestionListRespond(BYTE* pbyData, size_t uDataLen)
{
	R2S_QUIZ_QUESTION_RESPOND*		pRespond		= NULL;

	pRespond = (R2S_QUIZ_QUESTION_RESPOND*)pbyData;
	LOG_PROCESS_ERROR(pRespond);
	g_cQuizManager.SetVecQuestion(pRespond->nQuestion);

EXIT0:
	return;
}


BOOL QuizRelayClient::Send( IMemBlock* piBuffer )
{
	return g_RelayClient.Send(piBuffer);
}