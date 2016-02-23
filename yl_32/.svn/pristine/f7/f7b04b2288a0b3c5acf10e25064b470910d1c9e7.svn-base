
#ifdef GAME_CLIENT
#include "stdafx.h"
#include "onlinegameworld/kmoduleinterface.h"
#include "kmodulemanager.h"
#include "knetconnectplayer_client.h"
#include "kplayerbaseprocess_client.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kobjeventsetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KNetConnectPlayer g_cNetConnectPlayer;
// -------------------------------------------------------------------------
BOOL KNetConnectPlayer::ProcessEvent( INT nEventId, LPCVOID pData, INT nData )
{
	switch (nEventId)
	{
	case emKSOCKET_EVENT_CREATE:
		{
			QLogPrintf(LOG_DEBUG, "Connection Established.");
		}
		break;
	case emKSOCKET_EVENT_CLOSE:
		{
			g_cOrpgWorld.m_cEventCreator.FireEvent(
				emKOBJEVENTTYPE_PLAYER_LOGOUT,
				g_cOrpgWorld.m_dwClientPlayerId,
				0,
				0);
            if (g_cOrpgWorld.m_dwClientSceneId != 0)
			{
			    g_pSceneMgr->UnloadScene(g_cOrpgWorld.m_dwClientSceneId);
				g_cOrpgWorld.m_dwClientSceneId = 0;
			}
			g_cOrpgWorld.m_cEventCreator.FireEvent(emKOBJEVENTTYPE_GAMEWORLD_SHUTDOWN, 1, 0, 0);
		}
		break;
	case emKSOCKET_EVENT_LOST:
		{
			//QLogPrintf(LOG_DEBUG, "Lost Connection To Game Server!", );
			// 服务器连接已断开
			g_cOrpgWorld.m_cEventCreator.FireEvent(emKOBJEVENTTYPE_GAMEWORLD_LOST_CONNECT, 0, 0, 0);

			// need world uninit?
			if (g_cOrpgWorld.m_dwClientSceneId != 0)
			{
				g_pSceneMgr->UnloadScene(g_cOrpgWorld.m_dwClientSceneId);
				g_cOrpgWorld.m_dwClientSceneId = 0;
			}
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;
}


BOOL KNetConnectPlayer::ProcessPacket( LPCBYTE pData, UINT uDataLen )
{
	if (*pData == 0)
		return g_cPlayerBaseProcess.ProcessPacket(pData, uDataLen);
	IKModuleInterface* piModule = g_cModuleManager.GetModule(BYTE(*pData) - KD_MAX_BASE_PROTOCOL_HEADER);
	if (!piModule)
	{
		Q_Error("Un Recognize Prococol " << BYTE(*pData) << " " << BYTE(*(pData+ 1)) << " Size " << uDataLen);
		return FALSE;
	}
	piModule->ProcessProtocol(pData, uDataLen);
	return TRUE;
}

// -------------------------------------------------------------------------
#endif // #ifdef GAME_CLIENT
