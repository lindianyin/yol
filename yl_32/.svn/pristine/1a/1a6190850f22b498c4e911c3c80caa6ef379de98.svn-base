
#ifdef GAME_CENTER

#ifndef __KGAMECENTEREYES_H__
#define __KGAMECENTEREYES_H__

#include "onlinegamebase/keyes.h"
// -------------------------------------------------------------------------

#define KD_EYES_MAX_TABLE_NAME 64
#define KD_EYES_MAX_FUNC_NAME 64

class KGameCenterEyes : public KEyes
{
public:
	static KGameCenterEyes& Inst() { static KGameCenterEyes ms_cThis; return ms_cThis; }

	virtual INT Init(INT nKey);
	virtual INT Uninit();
	virtual INT Active();
	virtual INT OnProtocal(BYTE byProtocal, BYTE* pData, size_t uDataLen);

private:
	KGameCenterEyes();

	INT SendPlayerCount(INT nOnline);
	INT OnE2LCommonProtocol(BYTE* pData, size_t uDataLen);
	INT OnG2LCommonProtocol(BYTE* pData, size_t uDataLen);
	INT OnE2LStopProtocol(BYTE* pData, size_t uDataLen);

	INT DoSendCustomInfo(DWORD dwFrames);
	INT DoSendPlayerCount(DWORD dwFrames);
	INT DoPing(DWORD dwFrames);

	INT GetCustomInfoInScript(std::string& rStrInfo);

	VOID ExecuteGmCmd(e2l_gmcmd* pCmd, LPCSTR pszCmd);
	BOOL ExecuteMappedGMCmd(e2l_gmmapcmd* pCmd);
private:
	DWORD m_dwPingInterval;
	DWORD m_dwSendCustomInfoInterval;
	DWORD m_dwSendPlayerCountInterval;

	DWORD m_dwNextPingFrame;
	DWORD m_dwNextSendCustomInfoFrame;
	DWORD m_dwNextSendPlayerCountFrame;
};

// -------------------------------------------------------------------------

#endif /* __KGAMECENTEREYES_H__ */

#endif /* GAME_CENTER */
