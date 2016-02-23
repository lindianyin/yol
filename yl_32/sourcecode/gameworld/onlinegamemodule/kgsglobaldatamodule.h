
#pragma once

#ifdef GAME_SERVER

// -------------------------------------------------------------------------
#include "onlinegamemodule/kmoduledef.h"
#include "serverbase/kgcmoduledef.h"
#include "onlinegameworld/kgcmodulebase.h"
#include "gcmodule/globaldata/kglobaldataconfig.h"

class KGS_GlobalDataModule : public KGcModuleBase
{
public:
	KGS_GlobalDataModule() : KGcModuleBase(emKMODULE_GLOBALDATA, emKGCMODULE_GLOBALDATA) {};

public:
	BOOL SetIntValue(DWORD dwKey, INT nValue, BOOL bRequest = TRUE);
	BOOL AddIntValue(DWORD dwKey, INT nValue, BOOL bRequest = TRUE);
	BOOL GetIntValue(DWORD dwKey, INT &rnValue);
	BOOL SetBinaryValue(DWORD dwKey, BYTE abValue[], WORD wLength, BOOL bRequest = TRUE);
	BOOL DeleteBinaryValue(DWORD dwKey, BOOL bRequest = TRUE);
	BOOL GetBinaryValue(DWORD dwKey, BYTE abValue[], WORD &rwLength);

	BOOL ProcessGcProtocol(LPCBYTE pbData, UINT uDataLen);
private:
	BOOL ProcessSetIntValue(LPCBYTE pbData, UINT uDataLen);
	BOOL ProcessSetBinaryValue(LPCBYTE pbData, UINT uDataLen);
	BOOL ProcessDeleteBinaryValue(LPCBYTE pbData, UINT uDataLen);
	BOOL ProcessBatchIntValue(LPCBYTE pbData, UINT uDataLen);
	BOOL ProcessBatchBinValue(LPCBYTE pbData, UINT uDataLen);

private:
	FLAT_INT_DATA_MAP m_mapIntData;
	FLAT_BINARY_DATA_MAP m_mapBinaryData;
};

extern KGS_GlobalDataModule g_cGlobalDataModule;

// -------------------------------------------------------------------------
#endif

