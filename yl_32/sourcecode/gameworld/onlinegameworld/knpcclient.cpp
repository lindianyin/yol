
#ifdef GAME_CLIENT
#include "stdafx.h"
#include "onlinegameworld/knpcclient.h"
#include "onlinegameworld/knpc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KNpcClient::KNpcClient( KNpc& rNpc ) : m_rcNpc(rNpc), KCharacterClient(rNpc)
{
}


// -------------------------------------------------------------------------
#endif // #ifdef GAME_CLIENT

