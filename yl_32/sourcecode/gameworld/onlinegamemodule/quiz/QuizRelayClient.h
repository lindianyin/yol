#ifndef _QUIZ_RELAYCLIENT_H__
#define _QUIZ_RELAYCLIENT_H__

class QuizRelayClient
{
public:
	QuizRelayClient();
	~QuizRelayClient();

public:

	//请求随机题目
	static VOID DoQuestionListRequest(INT nIssueId, INT nMaxQuestionCount);

	//提交分数
	static VOID DoSumitScoreRequest(DWORD dwRoldId, INT nIntegral, CONST CHAR* szName);

	//请求榜单s
	static VOID DoNameListRequest(INT nRequest);
	
	//返回前十名榜单
	static VOID OnNameListRespond(BYTE* pbyData, size_t uDataLen);

	//返回题目序号
	static VOID OnQuestionListRespond(BYTE* pbyData, size_t uDataLen);

private:
	static inline BOOL Send(IMemBlock* piBuffer);
};
#endif