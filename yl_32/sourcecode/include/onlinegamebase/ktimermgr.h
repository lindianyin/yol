
#pragma once

// -------------------------------------------------------------------------


class KTimerMgr
{
public:
	KTimerMgr(void);
	~KTimerMgr(void);

	BOOL Breath();
	BOOL Init();
	BOOL UnInit();
	UINT AddScriptTimePoint(UINT uFrame);
	DWORD	GetCurTime() { return m_dwCurrentTime; }
public:
	typedef std::set<UINT> KScriptTimerSet;		// 脚本注册时间点信息

private:
	KScriptTimerSet m_setScriptTimer;			// 脚本注册时间点
	UINT			m_uNextScriptTime;			// 下次触发脚本时间
	DWORD			m_dwCurrentTime;

};

extern KTimerMgr g_cTimerMgr;
