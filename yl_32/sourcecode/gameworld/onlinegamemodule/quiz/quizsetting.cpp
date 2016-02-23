#include "stdafx.h"
#include "quizsetting.h"

#define QUIZ_TABLE_FILE                 "\\setting\\quiz\\quiz.txt"
#define QUIZ_ISSUE_QUESTION_FILE        "\\setting\\quiz\\quiz_issue_question.txt"

QuizSetting g_cQuizSetting;

QuizSetting::QuizSetting(void)
{
}

QuizSetting::~QuizSetting(void)
{
}

QuizTemplate* QuizSetting::GetQuizTemplate(INT nQuizId)
{
	if (m_mapQuizTemplates.find(nQuizId) == m_mapQuizTemplates.end())
		return NULL;  // 不存在

	return m_mapQuizTemplates[nQuizId];
}


QuizIssueTemplate* QuizSetting::GetQuizIssueTemplate(INT nQuizId)
{
	if (m_mapQuizIssueTemplates.find(nQuizId) == m_mapQuizIssueTemplates.end())
		return NULL;  // 不存在

	return m_mapQuizIssueTemplates[nQuizId];
}

BOOL QuizSetting::Init()
{
	ReadQuizQuetion();
	ReadQuizIssueQuestion();

	return TRUE;
}

BOOL QuizSetting::ReadQuizQuetion()
{
	m_mapQuizTemplates.clear();
	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(QUIZ_TABLE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 3; i <= nFileHeight; i++)  // 第3行
	{
		QuizTemplate* quizTemplate = new QuizTemplate();
		tabFile->GetInteger(i, "QuizId", 0, &quizTemplate->nQuizId);
		tabFile->GetString(i, "Question", "", quizTemplate->szQuestion, countof(quizTemplate->szQuestion));
		tabFile->GetString(i, "CorrectAnswer", "", quizTemplate->szCorrectAnswer, countof(quizTemplate->szCorrectAnswer));
		tabFile->GetString(i, "Answer2", "", quizTemplate->szAnswer2, countof(quizTemplate->szAnswer2));
		tabFile->GetString(i, "Answer3", "", quizTemplate->szAnswer3, countof(quizTemplate->szAnswer3));
		tabFile->GetString(i, "Answer4", "", quizTemplate->szAnswer4, countof(quizTemplate->szAnswer4));

		m_mapQuizTemplates[quizTemplate->nQuizId] = quizTemplate;
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QuizSetting::ReadQuizIssueQuestion()
{
	m_mapQuizIssueTemplates.clear();
	INT nFileHeight = 0;
	ITabFile* tabFile = g_OpenTabFile(QUIZ_ISSUE_QUESTION_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nFileHeight = tabFile->GetHeight();

	for (INT i = 3; i <= nFileHeight; i++)  // 第3行
	{
		QuizIssueTemplate* quizTemplate = new QuizIssueTemplate();
		tabFile->GetInteger(i, "QuizId", 0, &quizTemplate->nQuizId);
		tabFile->GetInteger(i, "Difficulty", 0, &quizTemplate->nDifficulty);
		tabFile->GetInteger(i, "CorrectAnswer", 0, &quizTemplate->nCorrectAnswer);
		tabFile->GetString(i, "Question", "", quizTemplate->szQuestion, countof(quizTemplate->szQuestion));
		tabFile->GetString(i, "Answer1", "", quizTemplate->szAnswer1, countof(quizTemplate->szAnswer1));
		tabFile->GetString(i, "Answer2", "", quizTemplate->szAnswer2, countof(quizTemplate->szAnswer2));
		tabFile->GetString(i, "Answer3", "", quizTemplate->szAnswer3, countof(quizTemplate->szAnswer3));
		tabFile->GetString(i, "Answer4", "", quizTemplate->szAnswer4, countof(quizTemplate->szAnswer4));

		m_mapQuizIssueTemplates[quizTemplate->nQuizId] = quizTemplate;
	}

	return TRUE;

EXIT0:
	return FALSE;
}

BOOL QuizSetting::UnInit()
{
	for (map<INT, QuizTemplate*>::iterator it = m_mapQuizTemplates.begin();it != m_mapQuizTemplates.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	m_mapQuizTemplates.clear();

	for (map<INT, QuizIssueTemplate*>::iterator it = m_mapQuizIssueTemplates.begin();it != m_mapQuizIssueTemplates.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	m_mapQuizIssueTemplates.clear();

	return TRUE;
}

INT QuizSetting::GetIssueCount()
{
	return m_mapQuizIssueTemplates.size();
}