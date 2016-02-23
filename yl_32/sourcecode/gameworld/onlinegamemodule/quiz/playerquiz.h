/* -------------------------------------------------------------------------
//	文件名		：	playerquiz.h
//	创建者		：	kk
//	创建时间	：	2013-8-12 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegamemodule/luaplayerquiz.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "quizprotocol.h"

#define QUIZ_DOUBLE "quizDouble"
#define QUIZ_SPIRIT "quizSpirit"
#define PER_SPIRITE_NEED_COIN 5

struct KQuizSaveData
{
	INT nScore;					//当前积分
	INT nDoubleVipCount;		//vip双倍使用次数
	INT nGiveAnswerCount;		//直接给答案次数
};

// 一问一答
class PlayerQuiz : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_QUIZ	};

	PlayerQuiz(KCharacter& rcPlayer);
	~PlayerQuiz();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// 读盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 存盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	LuaPlayerQuiz* GetScriptInterface() CONST;

	BOOL CallQuiz(INT nQuizId);

	BOOL DoChoice(INT nChoice);

	BOOL SendClientResult(BOOL bCorrect, BOOL bEndQuiz=FALSE); // 告诉客户端是否答对, 默认不结束答题

public:

	BOOL DoJoinIssue(INT nJoin);

	BOOL DoClientAnswer(INT nIssueId, INT nAnswerChoice, BYTE byDouble, BYTE bySpirit);

	BOOL DoClientSpirit(INT nIssueId, INT nAnswerChoice);

	BOOL SendIssueStart(INT nTimeSpan, INT nInitSpiritCoin,INT nDoubleFreeTimes, INT nDoubleMaxFreeTimes);

	BOOL SendIssueQuestion(INT nTimeSpan, INT nInitSpiritCoin, INT nAddSpiritCoin,INT nDoubleFreeTimes, INT nDoubleMaxFreeTimes);

	BOOL SendIssueResult(INT nTimeSpan);

	BOOL SendIssueRanking(QUIZ_S2C_ISSUE_RANKING* issueRank);

	BOOL UpdateSaveDataNextDay();

	//BOOL DoSumitIntegral();
	BOOL GetIsJoin(){ return m_bJoin; }
	BOOL GetCorrect(){ return m_bCorrect; }
	BOOL GetDouble(){ return m_bDouble; }
	INT GetIntegral(){ return m_rcQuizSaveData.nScore; }
	SIZE_T GetAnswerTime(){ return m_tAnswerTime; }

	VOID SetIsCorrect(BOOL bCorrect){ m_bCorrect = bCorrect; }
	VOID SetIntegral(INT nIntegral){ m_rcQuizSaveData.nScore = nIntegral; }
	VOID AddVipDoubleCount(INT nCount){ m_rcQuizSaveData.nDoubleVipCount += nCount; }
	VOID SetDouble(BOOL bDouble) { m_bDouble = bDouble; }
private:
	KPlayer&		 m_rcPlayer;

	LuaPlayerQuiz* m_pLunaObj;

	INT m_nCurrentQuizId;  // 正在做的题目

	INT m_nCorrectQuizChoice; // 正确的选择
	
	BOOL SendClientQuestion(CHAR* szQuestion, CHAR* szAnswers); // 发送问题


private:
	BOOL						m_bCorrect;							//是否正确
	BOOL						m_bDouble;							//是否加倍
	BOOL						m_bSpirit;							//是否直接给答案
	SIZE_T						m_tAnswerTime;						//答题时间
	INT							m_nIssueId;							//当前比赛id
	INT							m_nCurIssue;						//当前题目
	INT							m_nAnswer;							//玩家选择的答案
	BOOL						m_bJoin;							//是否参加答题
	KQuizSaveData				m_rcQuizSaveData;


};
