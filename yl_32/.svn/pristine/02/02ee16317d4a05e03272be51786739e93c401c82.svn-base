
#include "stdafx.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kprotocol.h"
#include "onlinegameworld/koverloadextension.h"
#include "onlinegameworld/kplayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
// An Example
BOOL FiltDoingDeath(KPlayer& rcPlayer, LPCBYTE pByData, UINT uLen)
{
	KPTC_HEADER_BASE* pPtc = (KPTC_HEADER_BASE* )pByData;

	return TRUE;
}




using namespace KNSBASE_EXT;
VOID g_SetDoingFilter()
{
	DoingProtocolFilter[cmsOnDeath] = FiltDoingDeath;
}
// -------------------------------------------------------------------------
