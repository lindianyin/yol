#ifndef _QUIZ_SERVERHANDLER_H__
#define _QUIZ_SERVERHANDLER_H__

#include "serverbase/protocol/Relay_GS_Protocol.h"

class QuizServerHandler
{
public:
	QuizServerHandler();
	~QuizServerHandler();

public:
	//玩家提交分数
	static VOID OnSubmitIntegralRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	//请求榜单
	static VOID OnNameListRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	//题目序号
	static VOID OnQuestionListRequest(BYTE* pbyData, size_t uDataLen, int nConnIndex);

	//返回榜单
	static VOID DoNameListRespond(INT nConnIndex);

	//返回题目
	static VOID DoQuestionListRespond(INT nConnIndex, INT nIssue, INT nMaxQuestionCount);

private:
	static inline BOOL Send(int nConnIndex, IMemBlock* piBuffer);
};

#endif