
#pragma once

#include <map>
#include <set>
#include "config/gamedef.h"
#include "RoleDB.h"
#include "MapInfo.h"

class KMapManager
{
public:
	KMapManager();

	BOOL Init();
    void UnInit();

    void Activate();

	KMapInfo* GetMapInfo(DWORD dwMapID);

	KMapCopy* GetMapCopy(DWORD dwMapID, int nMapCopyIndex);

	void    OnGameServerCreate(int nConnIndex);
    void    OnGameServerDestroy(int nConnIndex);

    // 在预加载表里面的副本是不能根据玩家请求动态加载的,通过这个函数检查
    BOOL    IsMapCopyInPreloadGroup(DWORD dwMapID, int nCopySN);
    
    BOOL    ResetAllMap(DWORD dwPlayerID, std::vector<DWORD>& vecFailedMap);
    BOOL    ResetMap(DWORD dwPlayerID, DWORD dwMapID, int nMapCopyIndex);
    BOOL    RefreshMap(DWORD dwMapID);
    // 地图副本信息相关操作
    BOOL    IncreaseMapCopyPlayerCount(DWORD dwMapID, int nCopyIndex); // ++PlayerCount;
    BOOL    DecreaseMapCopyPlayerCount(DWORD dwMapID, int nCopyIndex); // --PlayerCount;
    BOOL    SetMapCopyOwner(DWORD dwMapID, int nCopyIndex, DWORD dwOwnerID);
    BOOL    IsMapCopyCountLimit(KMapInfo* pMapInfo);
    
    // 副本进入规则1, 2, 3, 4: 按规则查找副本,见文档说明
    int     FindPlayerCopyIndex(KRole* pRole, KRole* pLeader, DWORD dwMapID);

    void    OnPlayerEnterMapCopy(DWORD dwPlayerID, DWORD dwMapID, int nCopyIndex);

    int     GetPlayerOnlineTotal() { return m_nPlayerOnlineTotal; }

    void    GetMapList(std::vector<DWORD>& rRetMap);
    
    void    ResetPlayerEnterNewCopyTime();
private:
    BOOL    LoadMapList();
    BOOL    LoadMapGroup();
    
private:

    typedef std::map<DWORD, KMapInfo, std::less<DWORD> >	KMAP_TABLE;
	KMAP_TABLE		m_MapTable;

    struct KMAP_GROUP_ITEM
    {
        DWORD dwMapID;
        int   nCopySN;
    };

    struct KMapGroup 
    {
        int                             nConnIndex; // 0表示空闲
        std::vector<KMAP_GROUP_ITEM>    MapTable;
    };

    KMapGroup       m_GroupTable[MAX_GS_COUNT];

    int    m_nPlayerOnlineTotal;
};


