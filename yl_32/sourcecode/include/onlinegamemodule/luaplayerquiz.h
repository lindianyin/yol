#pragma once

class KCharacter;

class LuaPlayerQuiz : public QLunaBase
{
private:
	KCharacter& m_rcCharacter;

public:
	explicit LuaPlayerQuiz(KCharacter& rcCharacter) : m_rcCharacter(rcCharacter){}
	~LuaPlayerQuiz(){}

	static INT Register2Player(QLuaScript& sc, QLunaBase* pLunaObj);

	DECLARE_LUA_CLASS(LuaPlayerQuiz);

	INT LuaCallQuiz(QLuaScript& sc);
	INT LuaSendClientResult(QLuaScript& sc);

	INT LuaGetIsJoinIssue(QLuaScript& sc);
	INT LuaSendIssueStart(QLuaScript& sc);
	INT LuaSendIssueQuestion(QLuaScript& sc);
	INT LuaSendIssueResult(QLuaScript& sc);
	INT LuaSumitIntegral(QLuaScript& sc);
	INT LuaGetPlayerRank(QLuaScript& sc);
	INT LuaSetIssueDouble(QLuaScript& sc);
	INT LuaSetIssueCorrect(QLuaScript& sc);
	INT LuaAddVipDoubleCount(QLuaScript& sc);
	INT LuaDoClientSpirit(QLuaScript& sc);
	INT LuaGetIssueAnswerInfo(QLuaScript& sc);
};
