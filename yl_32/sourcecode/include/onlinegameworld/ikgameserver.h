
#pragma once
// -------------------------------------------------------------------------
class IKNsBaseRequire;
class IKGameServer
{
public:
	virtual BOOL	Init(IKNsBaseRequire* piNsBaseRequire) = 0;
	virtual BOOL	StartUp() = 0;
	virtual BOOL	ShutDown() = 0;
	virtual BOOL	Loop() = 0;
	virtual BOOL	UnInit() = 0;
	virtual BOOL	DoCommand(LPCSTR szCommand) = 0;
};
extern IKGameServer* g_GetGameServerInterface();

// -------------------------------------------------------------------------

