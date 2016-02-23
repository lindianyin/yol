
#include "stdafx.h"
#include "bufcommand.h"
#include "onlinegamebase/kscriptmanager.h"


CONST CHAR* szConditionTableName = "BufferCondition";
CONST CHAR* szMagicEffectTableName = "BufferMagicEffect";
CONST CHAR* szTempValueName = "BufferCondition.nResult";
CONST CHAR* szResultRetFunctionName = "GetResult";
CONST CHAR* szOrOperator = "|";
CONST CHAR* szAndOperator = "&";
CONST CHAR* szNotOperator = "!";
CONST CHAR* szOrOperatorReplace = " or ";	//一定要带上空格，要不lua里会无法识别
CONST CHAR* szAndOperatorReplace = " and ";
CONST CHAR* szNotOperatorReplace = " not ";
CONST size_t nNotOperatorLength = strlen(szNotOperator);
CONST size_t nAndOperatorLength = strlen(szAndOperator);
CONST size_t nOrOperatorLength  = strlen(szOrOperator);



BufCommand::BufCommand()
{
	m_szFunctionName = "";
	m_szCommandScript = "";
}

BufCommand::~BufCommand()
{

}

BufCommand& BufCommand::operator=(const BufCommand& command)
{
	m_vecParams = command.m_vecParams;
	m_szCommandScript = command.m_szCommandScript;
	m_szFunctionName = command.m_szFunctionName;
	return *this;
}

BOOL BufCommand::Init(string commandStr)
{
#if 0
	INT  nParamStartIndex = 0;
	CHAR cCurrChar;

	LOG_PROCESS_ERROR(!commandStr.empty());

	for (INT i = 0; i < (int)commandStr.size(); i++)
	{
	    cCurrChar = commandStr[i];

		if (cCurrChar == '(')
		{
		    nParamStartIndex = i + 1;
			break;
		}

		m_szFunctionName.push_back(cCurrChar);
	}

	LOG_PROCESS_ERROR(nParamStartIndex != 0);

	cCurrChar = commandStr[nParamStartIndex];

	// 解析参数值
	CHAR szParamStr[2];
	szParamStr[1] = '\0';
	while (cCurrChar != ')' && cCurrChar != '\0')
	{
		if (cCurrChar == ',') // 逗号跳过
		{
		    cCurrChar = commandStr[++nParamStartIndex];
		}

	    szParamStr[0] = cCurrChar;
	    INT param = atoi(szParamStr);
		m_vecParams.push_back(param);

		cCurrChar = commandStr[++nParamStartIndex];
	}
#else
    m_szCommandScript = m_szCommandScript + szTempValueName + " = " + commandStr;
#endif
	return TRUE;
}

BOOL BufCommand::DoScript(INT nResult)
{
	BOOL bRet = FALSE;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	QCONFIRM_RET_FALSE(!m_szCommandScript.empty());

	cSafeScript->DoBuffer(m_szCommandScript.c_str());

	if (nResult > 0)
	{

		bRet = CallTableFunction(szConditionTableName, szResultRetFunctionName, nResult);
	}

	return bRet;
}

BOOL BufCommand::CallTableFunction(CONST CHAR* szTableName,CONST CHAR* szFunctionName, INT nResult)
{
	INT nRet = 0;
	BOOL bRet = FALSE;
	INT nParamNum = (INT)m_vecParams.size();
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	QCONFIRM_RET_FALSE(szTableName);
	QCONFIRM_RET_FALSE(szFunctionName);

	if (nParamNum > 0)
		bRet = cSafeScript->CallTableFunction(szTableName, szFunctionName, nResult, "*d", &m_vecParams);
	else
		bRet = cSafeScript->CallTableFunction(szTableName, szFunctionName, nResult, "");

	QCONFIRM_RET_FALSE(bRet);

	if (nResult)
		nRet = cSafeScript->GetInt(-1);

	if(!nRet)
 		return FALSE;

	return TRUE;
}

BOOL BufConditionCommand::Init(string szCommand)
{
	ReplaceCommand(szCommand);
	return BufCommand::Init(szCommand);
}

//将配置表里的字符串进行转义
VOID BufConditionCommand::ReplaceCommand(string& szCommand)
{
	if (szCommand.find(szNotOperator) == 0)	//如果表达式开头就是！，先进行替换
	{
		szCommand.insert(nNotOperatorLength,":");
		szCommand.insert(nNotOperatorLength,szConditionTableName);
		szCommand.insert(nNotOperatorLength,szNotOperatorReplace);
		szCommand.erase(0,nNotOperatorLength);
	}
	else
	{
		szCommand.insert(0,":");
		szCommand.insert(0,szConditionTableName);
	}
	size_t andFound = szCommand.find(szAndOperator);
	while( andFound != string::npos)
	{
		if(szCommand[andFound + nAndOperatorLength] == '(')
		{
			szCommand.insert(andFound + nAndOperatorLength,szAndOperatorReplace);
			szCommand.erase(andFound,nAndOperatorLength);
			andFound = szCommand.find(szAndOperator,andFound);
			continue;
		}
		szCommand.insert(andFound + nAndOperatorLength,":");
		szCommand.insert(andFound + nAndOperatorLength,szConditionTableName);
		szCommand.insert(andFound + nAndOperatorLength,szAndOperatorReplace);
		szCommand.erase(andFound,nAndOperatorLength);
		andFound = szCommand.find(szAndOperator);
	}
	size_t orFound = szCommand.find(szOrOperator);
	while( orFound != string::npos)
	{
		if(szCommand[orFound + nOrOperatorLength] == '(')
		{
			szCommand.insert(orFound + nOrOperatorLength,szOrOperatorReplace);
			szCommand.erase(orFound,nOrOperatorLength);
			orFound = szCommand.find(szOrOperator,orFound);
			continue;
		}
		szCommand.insert(orFound + nOrOperatorLength,":");
		szCommand.insert(orFound + nOrOperatorLength,szConditionTableName);
		szCommand.insert(orFound + nOrOperatorLength,szOrOperatorReplace);
		szCommand.erase(orFound,nOrOperatorLength);
		orFound = szCommand.find(szOrOperator);
	}
	size_t notFound = szCommand.find(szNotOperator);
	while( notFound != string::npos)
	{
		if(szCommand[notFound + nNotOperatorLength] == '(')
		{
			szCommand.insert(notFound + nNotOperatorLength,szNotOperatorReplace);
			szCommand.erase(notFound,nNotOperatorLength);
			notFound = szCommand.find(szNotOperator,notFound);
			continue;
		}
		szCommand.insert(notFound + nNotOperatorLength,":");
		szCommand.insert(notFound + nNotOperatorLength,szConditionTableName);
		szCommand.insert(notFound + nNotOperatorLength,szNotOperatorReplace);
		szCommand.erase(notFound,nNotOperatorLength);
		notFound = szCommand.find(szNotOperator);
	}
}


BOOL BufConditionCommand::Excecute()
{
	BOOL bRet = FALSE;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	QCONFIRM_RET_FALSE(!m_szCommandScript.empty());

	cSafeScript->DoBuffer(m_szCommandScript.c_str());

	bRet = CallTableFunction(szConditionTableName, szResultRetFunctionName, 1);

	return bRet;
}


VOID BufMagicEffectCommand::ReplaceCommand(string& szCommand)
{
	INT nStartPos = 0;
	INT nEndPos = 0;
	m_szReverseCommand =  m_szReverseCommand + ":de_" + szCommand;
	m_szReverseCommand =  szMagicEffectTableName + m_szReverseCommand;
	nStartPos = m_szReverseCommand.find('(');
	nEndPos = m_szReverseCommand.find(')');
	m_szReverseCommand.erase(nStartPos + 1, nEndPos - nStartPos - 1);

	szCommand = ":" + szCommand;
	szCommand = szMagicEffectTableName + szCommand;
}


BOOL BufMagicEffectCommand::Init(string szCommand)
{
	m_szReverseCommand = "";
	ReplaceCommand(szCommand);
	return BufCommand::Init(szCommand);
}

BOOL BufMagicEffectCommand::Excecute(INT nBufferTemplateId, INT nSkillLevel)
{
	CHAR szTempStr[20];
	INT nEndPos = -1;
	INT nStartPos = -1;
	string szCommand = m_szCommandScript;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	QCONFIRM_RET_FALSE(!m_szCommandScript.empty());

	nStartPos = szCommand.find('(');
	nEndPos = szCommand.find(')');
	if(nEndPos > 0)
	{
		if(nEndPos - nStartPos > 1)
		{
			::sprintf(szTempStr, ", %d , %d", nBufferTemplateId, nSkillLevel);
		}
		else
		{
			::sprintf(szTempStr, "%d , %d", nBufferTemplateId, nSkillLevel);
		}
		szCommand.insert(nEndPos, szTempStr);
	}
	
	// Tme_damagelife_v

	cSafeScript->DoBuffer(szCommand.c_str());

	return TRUE;
}

//在解除buff的时候进行调用
BOOL BufMagicEffectCommand::ExcecuteReverse(INT nBufferTemplateId)
{
	CHAR szTempStr[20];
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	string szReverseCommand = m_szReverseCommand;
	INT nEndPos = szReverseCommand.find(')');

	if(nEndPos > 0)
	{
		::sprintf(szTempStr, "%d", nBufferTemplateId);
		szReverseCommand.insert(nEndPos, szTempStr);
	}

	return cSafeScript->DoBuffer(szReverseCommand.c_str());
}