
#pragma once

#ifdef GAME_SERVER

#include "onlinegamebase/keyes.h"
#include <vector>
// -------------------------------------------------------------------------

class KGameserverEyes : public KEyes
{
public:
	KGameserverEyes();
	~KGameserverEyes();

	virtual int Init(int nKey);
	virtual int Uninit();
	virtual int Active();
	virtual int OnProtocal(BYTE byProtocal, BYTE* pData, size_t uDataLen);

public:
	int SetPingInterval(unsigned int uIntervalSec);
	int SetSendCustomInfoInterval(unsigned int uIntervalSec);
	int SetSendPlayerCountInterval(unsigned int uIntervalSec);
	int SetSendMapInfoInterval(unsigned int uIntervalSec);

private:
	int SendPlayerCount(int nOnline);
	//int SendMapInfo(l2e_update_map_list::KMapDesc* pMapInfo, int nCount);
	int OnE2LCommonProtocol(BYTE* pData, size_t uDataLen);
	int OnG2LCommonProtocol(BYTE* pData, size_t uDataLen);
	int OnE2LStopProtocol(BYTE* pData, size_t uDataLen);

	int DoSendMapInfo(DWORD dwFrames);
	int DoSendCustomInfo(DWORD dwFrames);
	int DoSendPlayerCount(DWORD dwFrames);
	int DoPing(DWORD dwFrames);

	int GetMapDescInfoInC();
	int GetCustomInfoInScript(std::string& rStrInfo);
	int GetIntranetStatus(std::string& strCustomInfo);

	VOID ExecuteGmCmd(e2l_gmcmd* pCmd, LPCSTR pszCmd);
	BOOL ExecuteMappedGMCmd(e2l_gmmapcmd* pCmd);
private:
	DWORD m_dwPingInterval; /* frames */
	DWORD m_dwSendCustomInfoInterval;
	DWORD m_dwSendPlayerCountInterval;
	DWORD m_dwSendMapInfoInterval;

	DWORD m_dwNextPingFrame;
	DWORD m_dwNextSendCustomInfoFrame;
	DWORD m_dwNextSendPlayerCountFrame;
	DWORD m_dwNextSendMapInfoFrame;

	float       m_fLastPerformance;
	float       m_fLastClientFlux;
	char        m_szSelfName[PATH_MAX];

	//l2e_update_map_list::KMapDesc* m_pMapDesc;
	int m_nMapDesc;
	int m_nCurMapDesc;
};

extern KGameserverEyes g_cGameserverEyes;
// -------------------------------------------------------------------------

#endif /* GAME_SERVER */
