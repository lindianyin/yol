#pragma once

using namespace std;


struct QuizTemplate
{
	INT nQuizId;
	CHAR szQuestion[MAX_SENTENCE_LENGTH];
	CHAR szCorrectAnswer[MAX_SENTENCE_LENGTH];
	CHAR szAnswer2[MAX_SENTENCE_LENGTH];
	CHAR szAnswer3[MAX_SENTENCE_LENGTH];
	CHAR szAnswer4[MAX_SENTENCE_LENGTH];
};

struct QuizIssueTemplate
{
	INT nQuizId;
	INT nCorrectAnswer;
	INT nDifficulty;
	CHAR szQuestion[MAX_SENTENCE_LENGTH];
	CHAR szAnswer1[MAX_SENTENCE_LENGTH];
	CHAR szAnswer2[MAX_SENTENCE_LENGTH];
	CHAR szAnswer3[MAX_SENTENCE_LENGTH];
	CHAR szAnswer4[MAX_SENTENCE_LENGTH];
};

class QuizSetting
{

public:
	QuizSetting(void);
	~QuizSetting(void);
	
	BOOL Init();
	BOOL UnInit();
	QuizTemplate* GetQuizTemplate(INT nQuizId);
	QuizIssueTemplate* GetQuizIssueTemplate(INT nQuizId);

	INT GetIssueCount();

private:
	BOOL ReadQuizQuetion();
	BOOL ReadQuizIssueQuestion();

private:
	map<INT, QuizTemplate*> m_mapQuizTemplates;

	map<INT, QuizIssueTemplate*> m_mapQuizIssueTemplates;

};

extern QuizSetting g_cQuizSetting;