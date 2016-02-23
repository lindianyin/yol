#pragma once

class UnityPolicyServer
{
public:
	UnityPolicyServer(void);
	~UnityPolicyServer(void);
	INT Init(void);
	INT Uninit();
	INT Activate(void);

private:
	BOOL bInitSuccess;
};
