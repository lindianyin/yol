/* -------------------------------------------------------------------------
//	文件名		：	nsbase/kdoodadserver.h
//	创建者		：	simon
//	创建时间	：	2010/3/15 9:38:38
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __NSBASE_KDOODADSERVER_H__
#define __NSBASE_KDOODADSERVER_H__

#ifdef GAME_SERVER
// -------------------------------------------------------------------------
class KDoodad;
class KPlayer;
class KPlayerProtocolProcessor;

// -------------------------------------------------------------------------

class KDoodadServer
{
public:
	KDoodadServer(KDoodad& rcDoodad);
	~KDoodadServer(void);

	BOOL SyncChangeRegion();
	BOOL SyncState();
	BOOL TeleportTo(DWORD dwSceneId, INT nX, INT nY, INT nZ);
	BOOL SyncExtra();

	// 广播到附近玩家
	BOOL BroadcastNearby(LPCVOID pbyData, SIZE_T uSize, KPlayerProtocolProcessor* pProtocolSender = NULL);

	BOOL SyncToOldNeighborPlayers(std::vector<KPlayer*>& vPlayers);
protected:
	KDoodad& m_rcDoodad;
};


#endif // GAME_SERVER

#endif /* __NSBASE_KDOODADSERVER_H__ */
