#include "stdafx.h"
#include "quizmodule.h"
#include "quizprotocol.h"
#include "quizsetting.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegameworld/kplayer.h"
#include "playerquiz.h"
#include "quizmanager.h"

QuizModule g_QuizModule;

template<>
IKModuleInterface* GetModuleInterFace<emMODULE_QUIZ>()
{
	return &g_QuizModule;
}

QuizModule::QuizModule() : KModuleBase(emMODULE_QUIZ)
{

}

QuizModule::~QuizModule()
{
}

BOOL QuizModule::OnInit()
{
	g_cQuizSetting.Init();
	return TRUE;
}

BOOL QuizModule::InitProtocol()
{
	//任务答题
	KD_PTC_LENGTNED_SIZE_S2C(PTC_S2C_QUESTION);
	KD_PTC_FIXED_SIZE_S2C(PTC_S2C_RESULT);
	KD_PTC_FIXED_SIZE_C2S(PTC_C2S_ANSWER);

	//活动答题
	KD_PTC_LENGTNED_SIZE_S2C(QUIZ_S2C_ISSUE_QUESTION);
	KD_PTC_FIXED_SIZE_S2C(QUIZ_S2C_ISSUE_RESULT);
	KD_PTC_FIXED_SIZE_S2C(QUIZ_S2C_ISSUE_START);
	KD_PTC_LENGTNED_SIZE_S2C(QUIZ_S2C_ISSUE_RANKING);
	KD_PTC_FIXED_SIZE_S2C(QUIZ_S2C_ISSUE_SPIRIT);
	KD_PTC_FIXED_SIZE_C2S(QUIZ_C2S_ISSUE_ANSWER);
	KD_PTC_FIXED_SIZE_C2S(QUIZ_C2S_ISSUE_JOIN);


	return TRUE;
}

BOOL QuizModule::OnUnInit()
{
	g_cQuizSetting.UnInit();
	return TRUE;
}

BOOL QuizModule::Breath()
{
	g_cQuizManager.Breath();
	return TRUE;
}


BOOL QuizModule::ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen)
{
	QCONFIRM_RET_FALSE(pbData);

	KPTC_HEADER_BASE *pData = (KPTC_HEADER_BASE*)pbData;
	PlayerQuiz* pPlayerQuiz = NULL;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	LOG_PROCESS_ERROR(pPlayer);
	pPlayerQuiz = pPlayer->GetCtrl<PlayerQuiz>();
	LOG_PROCESS_ERROR(pPlayerQuiz);
	switch (pData->byProtocol)
	{
	case emC2S_ANSWER:
		{
			// 做出答题选择
			PTC_C2S_ANSWER* ptcAnswer = (PTC_C2S_ANSWER*)pData;
			pPlayerQuiz->DoChoice(ptcAnswer->nChoice);
		}
		break;

	case emC2S_ISSUE_JOIN:
		{
			QUIZ_C2S_ISSUE_JOIN* ptcJoin = (QUIZ_C2S_ISSUE_JOIN*)pData;
			pPlayerQuiz->DoJoinIssue(ptcJoin->nJoin);
		}
		break;

	case emC2S_ISSUE_ANSWER:
		{
			QUIZ_C2S_ISSUE_ANSWER* ptcAnswer = (QUIZ_C2S_ISSUE_ANSWER*)pData;
			pPlayerQuiz->DoClientAnswer(ptcAnswer->nAnswer, ptcAnswer->nChoice, ptcAnswer->byDouble, ptcAnswer->byGiveAnswer);
		}
		break;
	}
	return TRUE;

EXIT0:
	return FALSE;
}