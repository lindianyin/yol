/* -------------------------------------------------------------------------
//	文件名		：	quizprotocol.h
//	创建者		：	kk
//	创建时间	：	2013-8-12 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#pragma once
#include "onlinegameworld/kprotocol.h"

#define QUIZ_MAX_ANSWERS_COUNT 4  // 最大支持的答案数量
#define QUIZ_MAX_RANKING_COUNT 10

#pragma pack(push, 1)
enum QUIZ_S2C_PROTOCOL
{
	emS2C_QUESTION,
	emS2C_RESULT, // 答题结果

	emS2C_ISSUE_QUESTION,						//问题内容
	emS2C_ISSUE_RESULT,							// 答题结果
	emS2C_ISSUE_START,							//活动开始
	emS2C_ISSUE_RANKING,						//当前排名
	emS2C_ISSUE_SPIRIT,							//直接给答案

	emS2C_PROTOCOL_COUNT,
};

enum QUIZ_C2S_PROTOCOL
{
	emC2S_ANSWER,

	emC2S_ISSUE_ANSWER,							//答题
	emC2S_ISSUE_JOIN,							//参加答题

	emC2S_PROTOCOL_COUNT,
};

//-----------------------任务答题-----------------------------------------------------------------
struct PTC_S2C_QUESTION : KLENPTC_HEADER<emS2C_QUESTION>
{
	CHAR szQuestions[MAX_SENTENCE_LENGTH]; // 问题内容
	CHAR szAnswers[]; // 包括多个问题内容,用<end>分隔开, 依序排列
};

struct PTC_C2S_ANSWER : KPTC_HEADER<emC2S_ANSWER>
{
	INT nChoice;  // 选择第几个
};

struct PTC_S2C_RESULT : KPTC_HEADER<emS2C_RESULT>
{
	BYTE byCorrected;  // 是否正确
	BYTE byEndQuiz;  // 是否结束答题
};

//-------------------------------活动答题----------------------------------------
struct QUIZ_S2C_ISSUE_QUESTION : KLENPTC_HEADER<emS2C_ISSUE_QUESTION>
{
	INT nQuestionId;						//第几题
	INT nQuestionCount;						//题目总数
	INT nCorrectAnswer;						//正确答案
	INT nTimeSpan;							//倒计时间隔
	INT nDoubleFreeTimes;					//剩余免费双倍次数
	INT nDoubleAllFreeTimes;				//总共免费双倍次数
	INT nDoubleVipTimes;					//剩余vip双倍次数
	INT nDoubleAllVipTimes;					//总共vip双倍次数
	INT nDoubleNeedCoins;					//双倍需要的元宝
	INT nSpiritTimes;						//直接给答案次数
	INT nSpiritMaxTimes;					//直接给答案最大次数
	INT nSpiritNeedCoins;					//直接给答案需要的元宝
	INT nAnswerChoice;						//当前选择
	BYTE bySpirit;							//是否直接给答案
	BYTE byDouble;							//是否双倍
	CHAR szQuestions[MAX_SENTENCE_LENGTH];	// 问题内容
	CHAR szAnswers[];						// 包括多个问题内容,用<end>分隔开, 依序排列
};

struct QUIZ_S2C_ISSUE_RESULT : KPTC_HEADER<emS2C_ISSUE_RESULT>
{
	INT nIntegral;							//当前积分
	INT nTimeSpan;							//发题倒计时
	BYTE byCorrected;						// 是否正确
	BYTE byEndQuiz;							// 是否结束答题
};

struct QUIZ_S2C_ISSUE_START : KPTC_HEADER<emS2C_ISSUE_START>
{
	INT nTimeSpan;							//剩余开始时间
	INT nDoubleFreeTimes;					//剩余免费双倍次数
	INT nDoubleAllFreeTimes;				//总共免费双倍次数
	INT nDoubleVipTimes;					//剩余vip双倍次数
	INT nDoubleAllVipTimes;					//总共vip双倍次数
	INT nDoubleNeedCoins;					//双倍需要的元宝
	INT nSpiritNeedCoins;					//直接给答案需要的元宝
};

struct QUIZ_S2C_ISSUE_SPIRIT : KPTC_HEADER<emS2C_ISSUE_SPIRIT>
{
	INT nNeedCoin;							//需要的元宝
	INT nSpiritCount;						//当前次数
};

struct QUIZ_ISSUE_RANK_INFO
{
	INT nIntegral;
	CHAR szRoleName[MAX_NAME_LEN];
};

struct QUIZ_S2C_ISSUE_RANKING : KLENPTC_HEADER<emS2C_ISSUE_RANKING>
{
	INT nCount;
	QUIZ_ISSUE_RANK_INFO arrInfo[0];
};


struct QUIZ_C2S_ISSUE_ANSWER : KPTC_HEADER<emC2S_ISSUE_ANSWER>
{
	INT nAnswer;							//第几题
	INT nChoice;							//选择第几个
	BYTE byDouble;							//是否双倍
	BYTE byGiveAnswer;						//是否直接给答案
};

struct QUIZ_C2S_ISSUE_JOIN : KPTC_HEADER<emC2S_ISSUE_JOIN>
{
	INT nJoin;							//参加答题
};

#pragma pack(pop)
