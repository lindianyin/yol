/* -------------------------------------------------------------------------
//	文件名		：	kterraindata.cpp
//	创建者		：	simon
//	创建时间	：	2009/12/13 16:39:16
//	功能描述	：	服务器副本共享地形障碍
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kscene.h"
#include "kregion.h"
#include "kterraindata.h"
#include "onlinegameworld/korpgworld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------

KTerrainScene::KTerrainScene()
{
    m_nRefCount     = 1;
    m_nRegionCountX = 0;
    m_nRegionCountY = 0;
    m_bLoadFlag     = false;
	m_dwRecentReleaseTime = 0;
	m_dwId = KD_BAD_ID;
    memset(m_RegionList, 0, sizeof(m_RegionList));
}

int KTerrainScene::AddRef()
{
    return ++m_nRefCount;
}

int KTerrainScene::Release()
{
    ASSERT(m_nRefCount > 0);

    m_nRefCount--;
	m_dwRecentReleaseTime = g_cOrpgWorld.m_dwLogicFrames;

    if (m_nRefCount == 0)
    {
        Clean();
        delete this; // 注意delete this后要立刻 return 否则死得难看!!!
		return 0;
    }
    return m_nRefCount;
}

KCell* KTerrainScene::GetRegionTerrainData(int nRegionX, int nRegionY)
{
    KCell* pResult  = NULL;
    KTerrainRegion* pRegion  = NULL;

    LOG_PROCESS_ERROR(nRegionX >= 0 && nRegionX < m_nRegionCountX);
    LOG_PROCESS_ERROR(nRegionY >= 0 && nRegionY < m_nRegionCountY);
    
    pRegion = m_RegionList[nRegionX][nRegionY];
    LOG_PROCESS_ERROR(pRegion);

    pResult = pRegion->Cells;
EXIT0:
    return pResult;
}

KRegionTrapScript* KTerrainScene::GetRegionScriptData(int nRegionX, int nRegionY)
{
    KRegionTrapScript* pResult = NULL;
    KTerrainRegion* pRegion = NULL;

    LOG_PROCESS_ERROR(nRegionX >= 0 && nRegionX < m_nRegionCountX);
    LOG_PROCESS_ERROR(nRegionY >= 0 && nRegionY < m_nRegionCountY);
    
    pRegion = m_RegionList[nRegionX][nRegionY];
    LOG_PROCESS_ERROR(pRegion);

    pResult = pRegion->sScriptList;
EXIT0:
    return pResult;
}

BOOL KTerrainScene::CreateTerrain(int nRegionCountX, int nRegionCountY)
{
    BOOL bResult    = false;
    BOOL bRetCode   = false;

    Clean();

    m_nRegionCountX     = nRegionCountX;
    m_nRegionCountY     = nRegionCountY;

    memset(m_RegionList, 0, sizeof(m_RegionList));

    //memset(m_PartitionTable, 0, sizeof(m_PartitionTable));

    for (int nY = 0; nY < nRegionCountY; nY++)
    {
        for (int nX = 0; nX < nRegionCountX; nX++)
        {
            KTerrainRegion* pRegion = new KTerrainRegion;
            LOG_PROCESS_ERROR(pRegion);

            memset(pRegion->Cells, 0, sizeof(pRegion->Cells));
            memset(pRegion->sScriptList, 0, sizeof(pRegion->sScriptList));

            m_RegionList[nX][nY] = pRegion;
        }
    }

    m_bLoadFlag = false;

    bResult = true;
EXIT0:
    if (!bResult)
    {
        Clean();
    }
    return bResult;
}

void KTerrainScene::Clean()
{
	for (int nY = 0; nY < m_nRegionCountY; nY++)
	{
		for (int nX = 0; nX < m_nRegionCountX; nX++)
		{
			KTerrainRegion* pRegion = m_RegionList[nX][nY];
			if (!pRegion)
			continue;

			for (int i = 0; i < REGION_GRID_WIDTH * REGION_GRID_HEIGHT; i++)
			{
				KCell* pCell = pRegion->Cells[i].m_pNext;

				while (pCell)
				{
					KCell* pNext = pCell->m_pNext;
		    
					KCellHelper::DeleteCell(pCell);

					pCell = pNext;
				}
			}
			delete pRegion;
		}
	}

	memset(m_RegionList, 0, sizeof(m_RegionList));
	m_bLoadFlag = FALSE;
}

BOOL KTerrainScene::CanUnloadNow()
{
	BOOL bRet = FALSE;
	ASSERT(m_nRefCount >= 1);
	PROCESS_ERROR(m_nRefCount == 1);

#ifdef GAME_SERVER
	PROCESS_ERROR(m_dwRecentReleaseTime > 0);
	PROCESS_ERROR(m_dwRecentReleaseTime + KD_SHARED_TERRAIN_LIFETIME > 
		g_cOrpgWorld.m_dwLogicFrames);
#endif // GAME_SERVER

	bRet = TRUE;
EXIT0:
	return bRet;
}
