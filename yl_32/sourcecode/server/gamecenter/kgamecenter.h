
#ifndef __KGAMECENTER_H__
#define __KGAMECENTER_H__

// -------------------------------------------------------------------------


class KGameCenter
{
public:
	KGameCenter(void);
	~KGameCenter(void);

	INT Init();
	INT Run();
	INT Uninit();
	INT Shutdown();
private:
	INT OnSyncModuleInfo(LPCVOID pData, size_t uSize);
	INT SendModuleInfoRequest(ISocketStream* piSocket);
private:
	BOOL m_bRunning;
};

extern KGameCenter g_cGameCenter;

// -------------------------------------------------------------------------

#endif /* __KGAMECENTER_H__ */
