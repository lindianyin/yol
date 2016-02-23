#include "stdafx.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluaplayer.h"
#include "playerquiz.h"
#include "onlinegamemodule/luaplayerquiz.h"
#include "QuizRelayClient.h"
#include "quizmanager.h"

INT LuaPlayerQuiz::Register2Player(QLuaScript& sc, QLunaBase* pLunaObj)
{
	KLuaPlayer* pLunaPlayer = dynamic_cast<KLuaPlayer*>(pLunaObj);
	QCONFIRM_RET_FALSE(pLunaPlayer);
	KPlayer* pPlayer = pLunaPlayer->GetHim();
	QCONFIRM_RET_FALSE(pPlayer);

	PlayerQuiz* pPlayerQuiz = pPlayer->GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);
	sc.PushObj(pPlayerQuiz->GetScriptInterface());
	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: CallQuiz
// @Description		: 开始答题, 传入一个题目编号
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 问题ID
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaCallQuiz(QLuaScript& sc)
{
	INT quizId = sc.GetInt(1);

	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();

	BOOL bOk = pPlayerQuiz->CallQuiz(quizId);

	sc.PushNumber(bOk);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: SendClientResult
// @Description		: 传递答题结果
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 答题是否对
// @ArgumentComment	: 是否关闭客户端面板
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaSendClientResult(QLuaScript& sc)
{
	INT nRight = sc.GetInt(1);
	INT nEndQuiz = sc.GetInt(2);

	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();

	BOOL bOk = pPlayerQuiz->SendClientResult((BOOL)nRight, (BOOL)nEndQuiz);

	sc.PushNumber(bOk);

	return 1;
}


// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: GetIsJoinIssue
// @Description		: 获取是否正在答题
// @ArgumentFlag	:
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaGetIsJoinIssue(QLuaScript& sc)
{
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);

	sc.PushNumber(pPlayerQuiz->GetIsJoin());

	return 1;
}


// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: GetPlayerRank
// @Description		: 获取答题排名
// @ArgumentFlag	:
// @ReturnCode		: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaGetPlayerRank(QLuaScript& sc)
{
	sc.PushNumber(g_cQuizManager.GetRank(m_rcCharacter.GetId()));

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: SendIssueStart
// @Description		: 通知客户端答题开始
// @ReturnCode		: 
// @ArgumentFlag	: dddd
// @ArgumentComment	: 发题剩余时间
// @ArgumentComment	: 给答案初始元宝
// @ArgumentComment	: 当前免费剩余次数
// @ArgumentComment	: 免费最大次数
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaSendIssueStart(QLuaScript& sc)
{
	INT nTimeSpan = sc.GetInt(1);
	INT nInitSpiritCoin		= sc.GetInt(2);
	INT nDoubleFreeTimes	= sc.GetInt(3);
	INT nDoubleMaxFreeTimes = sc.GetInt(4);
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);

	BOOL bOk = pPlayerQuiz->SendIssueStart(nTimeSpan, nInitSpiritCoin, nDoubleFreeTimes, nDoubleMaxFreeTimes);

	sc.PushNumber(bOk);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: SendIssueQuestion
// @Description		: 通知客户端答题开始
// @ReturnCode		: 
// @ArgumentFlag	: ddddd
// @ArgumentComment	: 答题剩余时间
// @ArgumentComment	: 给答案初始元宝
// @ArgumentComment	: 给答案递增元宝
// @ArgumentComment	: 当前免费剩余次数
// @ArgumentComment	: 免费最大次数
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaSendIssueQuestion(QLuaScript& sc)
{
	INT nTimeSpan			= sc.GetInt(1);
	INT nInitSpiritCoin		= sc.GetInt(2);
	INT nAddSpiritCoin		= sc.GetInt(3);
	INT nDoubleFreeTimes	= sc.GetInt(4);
	INT nDoubleMaxFreeTimes = sc.GetInt(5);
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);

	//pPlayerQuiz->SetIsCorrect(FALSE);
	BOOL bOk = pPlayerQuiz->SendIssueQuestion(nTimeSpan, nInitSpiritCoin, nAddSpiritCoin, nDoubleFreeTimes, nDoubleMaxFreeTimes);

	sc.PushNumber(bOk);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: SendIssueResult
// @Description		: 发送结果
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 发题剩余时间
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaSendIssueResult(QLuaScript& sc)
{
	INT nTimeSpan = sc.GetInt(1);
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);

	BOOL bOk = pPlayerQuiz->SendIssueResult(nTimeSpan);

	sc.PushNumber(bOk);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: SumitIntegral
// @Description		: 提交积分到CR
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 玩家积分
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaSumitIntegral(QLuaScript& sc)
{
	INT nScore = sc.GetInt(1);
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);
	pPlayerQuiz->SetIntegral(nScore);
	QuizRelayClient::DoSumitScoreRequest(m_rcCharacter.GetId(), nScore, m_rcCharacter.GetName());

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: SetIssueDouble
// @Description		: 设置玩家双倍
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 是否双倍
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaSetIssueDouble(QLuaScript& sc)
{
	BOOL bDouble = (BOOL)sc.GetInt(1);
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);
	pPlayerQuiz->SetDouble(bDouble);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: SetIssueCorrect
// @Description		: 设置玩家是否正确
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 是否正确
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaSetIssueCorrect(QLuaScript& sc)
{
	BOOL bCorrect = (BOOL)sc.GetInt(1);
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);
	pPlayerQuiz->SetIsCorrect(bCorrect);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: AddVipDoubleCount
// @Description		: 增加当前vip 双倍次数
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 增量
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaAddVipDoubleCount(QLuaScript& sc)
{
	INT nCount = (BOOL)sc.GetInt(1);
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);
	pPlayerQuiz->AddVipDoubleCount(nCount);

	return 1;
}


// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: DoClientSpirit
// @Description		: 玩家直接给答案
// @ReturnCode		: 
// @ArgumentFlag	: dd
// @ArgumentComment	: 题目编号
// @ArgumentComment	: 答案序号
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaDoClientSpirit(QLuaScript& sc)
{
	INT nIssueId = sc.GetInt(1);
	INT nAnswerChoice = sc.GetInt(2);
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);
	pPlayerQuiz->DoClientSpirit(nIssueId, nAnswerChoice);

	return 1;
}

// ------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: LuaPlayerQuiz
// @LuaApiName		: GetIssueAnswerInfo
// @Description		: 获取玩家答题信息
// @ReturnCode		: 
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaPlayerQuiz::LuaGetIssueAnswerInfo(QLuaScript& sc)
{
	PlayerQuiz* pPlayerQuiz = m_rcCharacter.GetCtrl<PlayerQuiz>();
	QCONFIRM_RET_FALSE(pPlayerQuiz);
	sc.PushTable();

	sc.PushNumber(pPlayerQuiz->GetCorrect());
	sc.SetTableField("bCorrect");

	sc.PushNumber(pPlayerQuiz->GetDouble());
	sc.SetTableField("bDouble");

	sc.PushNumber(pPlayerQuiz->GetAnswerTime());
	sc.SetTableField("tTime");

	sc.PushNumber(pPlayerQuiz->GetIntegral());
	sc.SetTableField("nIntegral");

	return 1;
}



#include "luaplayerquiz_table.hpp"