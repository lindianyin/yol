
#pragma once

#include "Protocol/KG_CSLogServerProtocal.h"

// -------------------------------------------------------------------------

enum KE_LOG_LEVEL
{
	emLOG_LEVEL_NOMAL = 0,              // 普通
	emLOG_LEVEL_COUNT,
};

enum KE_PLAYERLOG_TYPE
{
	emKPLAYERLOG_TYPE_APPRAISAL = 35,   //评价
	emKPLAYERLOG_TYPE_SHOP,
	emKPLAYERLOG_TYPE_COUNT,
};

class KLogWritter
{
	enum
	{
		emKSUBTYPE_MAXCOUNT = 32,	// 日志最大子类别
		emKCOMMENT_MAXLEN = 4096,	// 注释最大长度
	};
public:
	KLogWritter();
	~KLogWritter();
	static KLogWritter *GetInstance() { return &ms_logWritter; }

	VOID LogSever_PlayerLog(			
		LPCSTR				pszAccount,
		LPCSTR				pszRole,
		KE_PLAYERLOG_TYPE	nType,
		LPCSTR              pszTarget,
		LPCSTR				pszComment = NULL);

	VOID LogSever_Login(LPCSTR pszGatewayName);

	VOID LogSever_DataAnalysisLog(INT32 nLogType, LPCSTR pszComment = NULL);

	VOID LogSever_DataAnalysisLogV(INT32 nLogType, ...);

	VOID DoLogin();

	BOOL StrConcat( LPSTR pszLog, va_list vl);

private:
	static KLogWritter ms_logWritter;
	static BOOL bIsLogin;
	CHAR				m_szBuff[32 * 1024];
	ULONG				m_nIdentity;		// 日志流水号
};

