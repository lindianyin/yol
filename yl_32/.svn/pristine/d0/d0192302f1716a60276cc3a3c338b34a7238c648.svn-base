
#pragma once

class IKNsBaseRequire;
class IKGameClient
{
public:
	virtual BOOL	Init(IKNsBaseRequire* piNsBaseRequire) = 0;
	virtual BOOL	UnInit() = 0;
	virtual BOOL	Connect(DWORD dwIpAddress, USHORT uPort, const GUID* pGuid, BOOL bHidden) = 0;
	virtual BOOL	DisConnect() = 0;
	virtual BOOL	Loop() = 0;
	virtual BOOL	IsReady() = 0;
	virtual BOOL	ProcessCommand(PCSTR pszCommand) = 0;
};
IKGameClient* g_GetGameClientInterface();

// -------------------------------------------------------------------------

