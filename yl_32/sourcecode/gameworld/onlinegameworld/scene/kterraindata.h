/* -------------------------------------------------------------------------
//	文件名		：	kterraindata.h
//	创建者		：	simon
//	创建时间	：	2009/12/13 16:14:55
//	功能描述	：	variation of Sword Online III's implementation
//						for terrain sharing of Dynamic-Scenes
//
// -----------------------------------------------------------------------*/
#ifndef __KTERRAINDATA_H__
#define __KTERRAINDATA_H__

// -------------------------------------------------------------------------


#include "kcell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define MAX_PARTITION_COUNT  (1 << MAX_PARTITION_INDEX_BIT)

// 服务器上副本是共享地形障碍的，没有副本再用它以后多久再删除
#define KD_SHARED_TERRAIN_LIFETIME	(GAME_FPS * 300)

struct KCellHelper
{
	static KCell* NewCell() { return new KCell; }
	static KDynamicCell* NewDynamicCell() { return new KDynamicCell; }
	static void DeleteCell(KCell* pCell) { delete pCell; }
};

struct KTerrainRegion
{
    KCell Cells[REGION_GRID_WIDTH * REGION_GRID_HEIGHT];
    KRegionTrapScript sScriptList[SCRIPT_COUNT_PER_REGION];
};

#pragma pack(1)
struct KPartition
{
    DWORD		dwScriptID;
    DWORD		dwForbiddenRide     : 1;          // 禁止骑马
    DWORD       dwForbiddenPass     : 1;          // 禁止通行
    DWORD       dwForbiddenPK       : 1;          // 禁止PK
    DWORD       dwRevivePosition    : 3;          // 复活点索引号
};
#pragma pack()


class KTerrainScene
{
public:
	static	KTerrainScene* CreateInstance() { return new KTerrainScene; }

    INT	AddRef();
    INT	Release();
	BOOL	CanUnloadNow();

    KCell* GetRegionTerrainData(int nRegionX, int nRegionY);

    KRegionTrapScript* GetRegionScriptData(int nRegionX, int nRegionY);

    //KPartition* GetPartitionTable() { return m_PartitionTable; };

public:
    BOOL CreateTerrain(int nRegionCountX, int nRegionCountY);

	DWORD	m_dwId; // 其实是场景的模板号做Id
    BOOL	m_bLoadFlag;
	DWORD	m_dwRecentReleaseTime;	// 最近一次被释放的时间

private:
	KTerrainScene(); // 自己管理new/delete
	~KTerrainScene() {}

    void Clean();

private:

    int                          m_nRefCount;
    KTerrainRegion*	             m_RegionList[MAX_REGION_WIDTH][MAX_REGION_HEIGHT];
    int                          m_nRegionCountX;
    int                          m_nRegionCountY;
    //KPartition                   m_PartitionTable[MAX_PARTITION_COUNT];
};


// -------------------------------------------------------------------------

#endif /* __KTERRAINDATA_H__ */
