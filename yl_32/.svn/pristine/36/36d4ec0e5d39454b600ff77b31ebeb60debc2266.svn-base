/* -------------------------------------------------------------------------
//	文件名		：	BufCommand.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-7
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __BUF_COMMAND_H__
#define __BUF_COMMAND_H__

#include "stdafx.h"
#include <string>
#include <vector>
#include "onlinegamemodule/bufferdef.h"

using namespace std;

class BufCommand
{
public:
	BufCommand();

	~BufCommand();

	virtual BOOL Init(string szCommandStr);

	BufCommand& operator=(const BufCommand& command);

protected:
	BOOL CallTableFunction(CONST CHAR* szTableName,CONST CHAR* szFunctionName, INT nResult);

	BOOL DoScript(INT nResult);

protected:
	vector<INT> m_vecParams;
	string m_szFunctionName;
	string m_szCommandScript;
};

// 条件判断
class BufConditionCommand : public BufCommand
{
public:
	virtual BOOL Init(string szCommand);
	BOOL Excecute();
	VOID ReplaceCommand(string& szCommand);
};

// 魔法效果
class BufMagicEffectCommand : public BufCommand
{
private:
	string m_szReverseCommand;
public:
	virtual BOOL Init(string szCommand);
	BOOL Excecute(INT nBufferTemplateId, INT nSkillLevel=1);
	BOOL ExcecuteReverse(INT nBufferTemplateId);	//执行逆运算
	VOID ReplaceCommand(string& szCommand);
};

#endif