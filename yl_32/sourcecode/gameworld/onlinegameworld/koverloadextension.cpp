
#include "stdafx.h"
#include "config/kcharacterdef.h"
#include "config/kplayerdef.h"
#include "onlinegameworld/koverloadextension.h"
#include "onlinegameworld/kplayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
namespace KNSBASE_EXT
{
	// Npc≥ı ºªØ ±
	BOOL (*DoNpcInit)(KNpc& rcNpc) = NULL;

	BOOL (*DoingProtocolFilter[cmsTotal])(KPlayer& rcPlayer, LPCBYTE pbyData, UINT uDataLen) = { 0 };
};


// -------------------------------------------------------------------------
