

#include "stdafx.h"
#include "onlinegameworld/kcharactermgr.h"

#include "onlinegameworld/knpcmgr.h"

#ifdef GAME_SERVER
	#include "onlinegameworld/kplayermgr.h"
#else
	#include "onlinegameworld/kplayermanager_client.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KCharacterMgr g_cCharacterMgr;

// -------------------------------------------------------------------------
KCharacter* KCharacterMgr::GetById(DWORD dwId)
{
	if (IS_PLAYER(dwId))
	{
		return g_cPlayerMgr.GetById(dwId);
	}
	else if (IS_NPC(dwId))
	{
		return g_cNpcMgr.GetById(dwId);
	}
	Q_Error("Wrong Id Type! ID: " << dwId);
	return NULL;
}


// -------------------------------------------------------------------------
