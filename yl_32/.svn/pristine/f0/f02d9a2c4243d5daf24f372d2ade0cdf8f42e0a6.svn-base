#pragma once

#include "stdafx.h"
#include "DBBase.h"
#include "ProtocolBasic.h"
#include "config/kplayerbasedbdef.h"

struct openseSav
{ 
	DWORD da;
	DWORD mar;
};

class OpenseSave
{
public:
	OpenseSave();
	~OpenseSave();

	BOOL                Init();
	void                UnInit();

	void				Activate();
	BOOL				connect();
	BOOL				FindTable(const char* search); 

	BOOL				OnLogin(DWORD playerID);
	BOOL				DoQuery(const char cszSQL[]);
	BOOL				DoCommand(LPCSTR pszCommand);
	BOOL				R2S_SendCMD(DWORD id,INT nConnIndex, DWORD dwRoleId, char* GMCMD);
	BOOL				Send( int nConnIndex, IMemBlock* piBuffer );
	void				ChangeGMCMDMark(BYTE* pbyData, size_t uDataLen, int nConnIndex);
	void 				signalRun(int nDay);
	std::map<INT, openseSav> mapOpenseSave;
	void 				AutoGiveOpenServerReward();
private:
	MYSQL*              m_pDBGMCMD;
	char				m_szSQL[1024];
	QLock				m_RespondGMCMD;
};


