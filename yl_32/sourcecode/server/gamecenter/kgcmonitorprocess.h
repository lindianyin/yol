
#ifndef __KGCMONITORPROCESS_H__
#define __KGCMONITORPROCESS_H__

// -------------------------------------------------------------------------
#include "ksocketclient.h" 

class KGcMonitorProcess : public IKSocketEventProcessor_Client
{
public:
	virtual BOOL ProcessEvent(INT nEventId, LPCVOID pData, INT nData);
	virtual BOOL ProcessPacket(LPCBYTE pData, UINT uDataLen);

	BOOL OnModuleLoaded(BYTE byModuleId, USHORT uPortGs);
private:
	BOOL OnLoadModule(LPCBYTE pData, UINT uDataLen);
	BOOL OnUpdateModuleInfo(LPCBYTE pData, UINT uDataLen);
};

extern KGcMonitorProcess g_cGcMonitorProcess;
// -------------------------------------------------------------------------

#endif /* __KGCMONITORPROCESS_H__ */
