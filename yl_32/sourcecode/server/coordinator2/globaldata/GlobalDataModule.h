
#pragma once

#include <map>
#include "serverbase/kgcmoduleinterface.h"
#include "gclogicbase/kgclogicextinterface.h"
#include "gcmodule/globaldata/kglobaldataconfig.h"

extern "C"
{
	GCMODULE_API IKGC_Module* CreateModuleInstance(IKGcQuery* piQuery);
};

extern IKGcQuery *g_pGcQuery;


// -------------------------------------------------------------------------
class KGC_GlobalDataModule : public IKGC_Module
{
public:
	KGC_GlobalDataModule();

public:
	virtual LPCSTR GetModuleName() { return "GlobalData"; };
	virtual BOOL Loop() { return TRUE; };
	virtual BOOL Breath();
	virtual BOOL OnInit();
	virtual BOOL OnStartUp();
	virtual BOOL OnUninit() { return TRUE; };

public:
	BOOL GetIntValue(WORD wGroupId, WORD wKey, INT &rnValue);
	BOOL SetIntValue(WORD wGroupId, WORD wKey, INT nValue, INT nConnectId);
	BOOL AddIntValue(WORD wGroupId, WORD wKey, INT nValue, INT nConnectId);

	BOOL GetBinaryValue(WORD wGroupId, WORD wKey, BYTE abValue[], WORD &rwLength);
	BOOL SetBinaryValue(WORD wGroupId, WORD wKey, BYTE abValue[], WORD wLength, INT nConnectId);
	BOOL DeleteBinaryValue(WORD wGroupId, WORD wKey, INT nConnectId);

	BOOL BatchSyncIntData(INT nConnectId);
	BOOL BatchSyncBinData(INT nConnectId);
	
	BOOL SaveIntGroup(INT nGroupId);
	BOOL OnSaveIntGroup(INT nGroupId);
	BOOL SaveBinGroup(INT nGroupId);
	BOOL OnSaveBinGroup(INT nGroupId);

private:
	INT_DATA_GROUP_MAP m_mapIntDataGroup;
	BINARY_DATA_GROUP_MAP m_mapBinaryDataGroup;
};

extern KGC_GlobalDataModule g_cGlobalDataModule;
extern IKGcServer *g_pGcServer;
extern IKGcDatabase* g_pDatabase;

