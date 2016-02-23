#include "stdafx.h"
#include "kscene.h"
#include "kregion.h"
#include "onlinegameworld/kmath.h"
#include "kterraindata.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kobjeventsetting.h"
#include "onlinegameworld/kdoodadmgr.h"
#include "KRelayClient.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegamemodule/ai/kaivm.h"
#include "onlinegamemodule/ai/kaictrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KScene::KScene()
: m_dwId(KD_BAD_ID)
, m_pTerrainScene(NULL)
{

}

KScene::~KScene()
{
	Uninit();
}

BOOL KScene::Init(CONST KSCENE_CONFIG& rCfg)
{
    BOOL bResult    = FALSE;
    BOOL bRetCode   = FALSE;

	m_dwTemplateId = 0;
    //m_nBroadcastRegion  = 1;
    //m_dwBanSkillMask    = 0;
	m_nCopyIndex = 0;
	m_dwTemplateId = 0;

#ifdef GAME_SERVER
    m_dwOwnerID = KD_BAD_ID;
	m_bSaved = FALSE;
	m_nDeleteTime = 2 * 60 * 60 * GAME_FPS; // 回收时间2小时
	m_dwCreationTime = g_cOrpgWorld.m_dwLogicFrames;

	m_AIPatrolGroupManager.Init(this);
#endif

	memset(m_RegionList, 0, sizeof(m_RegionList));
	m_nRegionWidth		= 0;
	m_nRegionHeight	    = 0;
    m_nTimeStamp        = 0;
	m_dwScriptID		= 0;
    m_pPartitionTable   = NULL;

    m_DynamicObstacleSwitchTable.reset();

	// 初始化地图名
	//m_nBroadcastRegion = rCfg.nBroadcastRange == 1 ? 0 : rCfg.nBroadcastRange;//规避广播区域太大导致卡的问题
	m_nBroadcastRegion = rCfg.nBroadcastRange;
	
	m_dwTemplateId = rCfg.dwTemplateId;
	QStrCpyLen(m_szClass, rCfg.szClass, sizeof(m_szClass));
	QStrCpyLen(m_szName, rCfg.szName, sizeof(m_szName));
	QStrCpyLen(m_szPath, rCfg.szPath, sizeof(m_szPath));
	m_bCanPK = rCfg.bCanPK;
	m_nBattleSceneId = rCfg.nBattleSceneId;

    bResult = TRUE;
//EXIT0:
	return bResult;
}

BOOL KScene::Uninit()
{
	m_vNpcCfg.clear();
	m_vTrapCfg.clear();
	m_mapNickNames.clear();

	DeleteAllRegion();

#ifdef GAME_SERVER
	m_AIPatrolGroupManager.UnInit();
#endif

	SAFE_RELEASE(m_pTerrainScene);
	return TRUE;
}

BOOL KScene::Activate(VOID)
{
	DWORD dwLogicFrame = g_cOrpgWorld.m_dwLogicFrames;
	for (INT nX = m_nRegionWidth - 1; nX >= 0; --nX)
	{
		for (INT nY = m_nRegionHeight - 1; nY >= 0; --nY)
		{
		    KRegion* pRegion = m_RegionList[nX][nY];

			if (pRegion == NULL)
				continue;

			#ifdef GAME_SERVER
			if (pRegion->IsActive(dwLogicFrame))
			{
				pRegion->Activate();
			}
			#else
			pRegion->Activate();
			if (!pRegion->IsActive())
			{
				// 不真正删除region，只清掉sobj
				pRegion->DeleteAllObject();

				//DelRegion(pRegion->m_nRegionX, pRegion->m_nRegionY);
			}
			#endif 
	    }
    }

#ifdef GAME_SERVER
	//if (IS_DYNAMIC_SCENE(m_dwId) && dwLogicFrame > m_dwCreationTime + m_nDeleteTime)
	//{
	//	// recycle this copy
	//	g_cOrpgWorld.m_cEventCreator.FireEvent(emKOBJEVENTTYPE_SCENE_RECYCLE, m_dwId, 0, 0);
	//}
#endif 

    return TRUE;
}

BOOL KScene::DelRegion(INT nRegionX, INT nRegionY)
{
	BOOL        bResult     = FALSE;
	BOOL        bRetCode    = FALSE;
	KRegion*    pRegion     = NULL;

	LOG_PROCESS_ERROR(nRegionX >= 0 && nRegionX < MAX_REGION_WIDTH);
	LOG_PROCESS_ERROR(nRegionY >= 0 && nRegionY < MAX_REGION_HEIGHT);

	pRegion = m_RegionList[nRegionX][nRegionY];
	PROCESS_ERROR(pRegion);

	bRetCode = pRegion->DeleteAllObject();
	LOG_PROCESS_ERROR(bRetCode);

//#if !defined(GAME_SERVER)
 //   // 服务端的Cell由于共享障碍的原因,另外管理
	//bRetCode = pRegion->DeleteAllCell();
	//LOG_PROCESS_ERROR(bRetCode);
//#endif

	m_RegionList[nRegionX][nRegionY] = NULL;

    //KMemory::Delete(pRegion);
	delete pRegion;

    bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KScene::DeleteAllObstacle(VOID)
{
	BOOL bRetCode = FALSE;

	for (INT nWidth = 0; nWidth < m_nRegionWidth; nWidth++)
	{
		for (INT nHeight = 0; nHeight < m_nRegionHeight; nHeight++)
		{
			KRegion* pRegion = GetRegion(nWidth, nHeight);
			if (pRegion)
			{
				bRetCode = pRegion->DeleteAllCell();
				LOG_CHECK_ERROR(bRetCode);
			}
		}
	}

	return TRUE;
}


BOOL KScene::DeleteAllRegion(VOID)
{
	BOOL bRetCode = FALSE;

	for (INT nWidth = 0; nWidth < m_nRegionWidth; nWidth++)
	{
		for (INT nHeight = 0; nHeight < m_nRegionHeight; nHeight++)
		{
			DelRegion(nWidth, nHeight);
		}
	}

	m_nRegionWidth = 0;
	m_nRegionHeight = 0;

	return TRUE;
}


#ifdef GAME_SERVER

BOOL KScene::LoadTrap()
{
	BOOL bResult = FALSE;
	ITabFile* piFile = NULL;
	CHAR szFileName[MAX_PATH]/* = {0}*/;
	
	INT nLines = 0;
	snprintf(szFileName, sizeof(szFileName), "%s/%s", m_szPath, KD_SCENE_TRAP_FILE);
	szFileName[sizeof(szFileName) - 1] = '\0';

	// 没有Trap文件
	piFile = g_OpenTabFile(szFileName);
	PROCESS_ERROR(piFile);

	nLines = piFile->GetHeight();

	for (INT n = 2; n <= nLines; ++n)
	{
		INT nTmp = 0;
		KTRAP_INFO sTrapInfo;

		bResult = piFile->GetInteger(n, "Id", KD_BAD_ID, &nTmp);
		sTrapInfo.dwId = nTmp;

		bResult = piFile->GetInteger(n, "TargetSceneId", KD_BAD_ID, &nTmp);
		sTrapInfo.dwTargetSceneId = nTmp;

		bResult = piFile->GetInteger(n, "TargetX", 0, &sTrapInfo.nTargetX);
		bResult = piFile->GetInteger(n, "TargetY", 0, &sTrapInfo.nTargetY);
		bResult = piFile->GetInteger(n, "TargetZ", 0, &sTrapInfo.nTargetZ);
		bResult = piFile->GetInteger(n, "ToleranceZ", 0, &sTrapInfo.nToleranceZ);

		bResult = piFile->GetString(n, "Class", "", sTrapInfo.szClass, countof(sTrapInfo.szClass));

		m_vTrapCfg.push_back(sTrapInfo);
	}

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piFile);
	return bResult;
}

BOOL KScene::LoadNpc()
{
	BOOL bResult = FALSE;
	ITabFile* piFile = NULL;
	CHAR szFileName[MAX_PATH]/* = {0}*/;
	INT nLines = 0;
	snprintf(szFileName, sizeof(szFileName), "%s/%s", m_szPath, KD_SCENE_NPC_FILE);
	szFileName[sizeof(szFileName) - 1] = '\0';

	// 没有Npc文件
	piFile = g_OpenTabFile(szFileName);
	PROCESS_ERROR(piFile);

	nLines = piFile->GetHeight();

	// 第一行是中英文字段头描述
	for (INT n = 2; n <= nLines; ++n)
	{
		INT nTmp = 0;
		KNPC_INFO sNpcInfo;

		bResult = piFile->GetInteger(n, "TemplateId", KD_BAD_ID, &nTmp);
		sNpcInfo.dwTemplateId = (DWORD)nTmp;

		bResult = piFile->GetInteger(n, "X", 0, &sNpcInfo.nX);
		bResult = piFile->GetInteger(n, "Y", 0, &sNpcInfo.nY);
		bResult = piFile->GetInteger(n, "Z", 0, &sNpcInfo.nZ);
		bResult = piFile->GetInteger(n, "FaceDirection", 0, &sNpcInfo.nFaceDirection);
		piFile->GetString(n, "NickName", "", sNpcInfo.szNickName, sizeof(sNpcInfo.szNickName));

		INT32	nRegenerationTime = 0;
		bResult = piFile->GetInteger(n, "RegenerationTime", 0, &nRegenerationTime);

		m_vNpcCfg.push_back(sNpcInfo);

		// add to gameworld
		KSceneObject* pNpc = (KSceneObject*)g_cOrpgWorld.AddNpc(sNpcInfo.dwTemplateId);
		LOG_PROCESS_ERROR(pNpc);

		pNpc->m_nFaceDirection = sNpcInfo.nFaceDirection;
		pNpc->m_nDirectionXY = sNpcInfo.nFaceDirection;

		bResult = this->AddNpc(pNpc, sNpcInfo.nX, sNpcInfo.nY, sNpcInfo.nZ);
		LOG_PROCESS_ERROR(bResult);

		if (sNpcInfo.szNickName[0])
			pNpc->SetNickName(sNpcInfo.szNickName);

		if (nRegenerationTime > 0)
		{
			//设置重生信息
			KREGENERATION_INFO regeneration_info;
			//regeneration_info.dwTemplateId = sNpcInfo.dwTemplateId;
			regeneration_info.nGenerationTime = nRegenerationTime * GAME_FPS;//将配置的秒转换成帧数
			regeneration_info.nX = sNpcInfo.nX;
			regeneration_info.nY = sNpcInfo.nY;
			regeneration_info.nZ = sNpcInfo.nZ;
			regeneration_info.nFaceDirection = sNpcInfo.nFaceDirection;
			pNpc->SetRegenerationInfo(regeneration_info);
		}
	}

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piFile);
	return bResult;
}

BOOL KScene::LoadDoodad()
{
	BOOL bResult = FALSE;
	ITabFile* piFile = NULL;
	KDoodad* pstDoodad = NULL;
	CHAR szFileName[MAX_PATH]/* = {0}*/;
	INT nLines = 0;
	snprintf(szFileName, sizeof(szFileName), "%s/%s", m_szPath, KD_SCENE_DOODAD_FILE);
	szFileName[sizeof(szFileName) - 1] = '\0';

	// 没有Npc文件
	piFile = g_OpenTabFile(szFileName);
	PROCESS_ERROR(piFile);

	nLines = piFile->GetHeight();

	// 前两行是中英文字段头描述
	for (INT n = 2; n <= nLines; ++n)
	{
		INT nTmp = 0;
		KDOODAD_INFO sDoodadInfo;

		bResult = piFile->GetInteger(n, "TemplateId", KD_BAD_ID, &nTmp);
		sDoodadInfo.dwTemplateId = (DWORD)nTmp;

		bResult = piFile->GetInteger(n, "X", 0, &sDoodadInfo.nX);
		bResult = piFile->GetInteger(n, "Y", 0, &sDoodadInfo.nY);
		bResult = piFile->GetInteger(n, "Z", 0, &sDoodadInfo.nZ);
		bResult = piFile->GetInteger(n, "FaceDirection", 0, &sDoodadInfo.nFaceDirection);

		m_vDoodadCfg.push_back(sDoodadInfo);

		/*
		KSceneObject* pDoodad = (KSceneObject*)g_cOrpgWorld.AddDoodad(sDoodadInfo.dwTemplateId);
		LOG_PROCESS_ERROR(pDoodad);

		bResult = this->AddDoodad(pDoodad, sDoodadInfo.nX, sDoodadInfo.nY, sDoodadInfo.nZ);
		LOG_PROCESS_ERROR(bResult);

		pDoodad->m_nFaceDirection = sDoodadInfo.nFaceDirection;
		pDoodad->m_nDirectionXY = sDoodadInfo.nFaceDirection;
		*/
		LOG_PROCESS_ERROR(pstDoodad = g_cDoodadMgr.CreateDoodad(sDoodadInfo.dwTemplateId));
		LOG_PROCESS_ERROR(g_cDoodadMgr.AddToScene(pstDoodad, m_dwId, sDoodadInfo.nX, sDoodadInfo.nY, sDoodadInfo.nZ));
		pstDoodad = NULL;
		//LOG_PROCESS_ERROR(g_cOrpgWorld.AddDoodadToScene(sDoodadInfo.dwTemplateId, m_dwId, sDoodadInfo.nX, sDoodadInfo.nY, sDoodadInfo.nZ, sDoodadInfo.nFaceDirection));
	}

	bResult = TRUE;
EXIT0:
	SAFE_RELEASE(piFile);
	if (pstDoodad)
	{
		g_cDoodadMgr.RemoveFromScene(pstDoodad->m_dwId);
		g_cDoodadMgr.DestroyDoodad(pstDoodad->m_dwId);
	}
	return bResult;
}

#endif // GAME_SERVER

BOOL KScene::Load()
{
	BOOL            bResult         = FALSE;
	BOOL            bRetCode        = FALSE;
	KRegion*        pRegion         = NULL;
	BOOL            bRegionInitFlag = FALSE;

	LOG_PROCESS_ERROR(m_pTerrainScene);
	//LOG_PROCESS_ERROR(m_pPartitionTable);

	bRetCode = LoadBaseInfo();
	LOG_PROCESS_ERROR(bRetCode);

	if (!m_pTerrainScene->m_bLoadFlag)
	{
		bRetCode = m_pTerrainScene->CreateTerrain(m_nRegionWidth, m_nRegionHeight);
		LOG_PROCESS_ERROR(bRetCode);

		//LoadPartitionTable();
	}

	for (INT nY = 0; nY < m_nRegionHeight; nY++)
	{
		for (INT nX = 0; nX < m_nRegionWidth; nX++)
		{
			pRegion = new KRegion;
			LOG_PROCESS_ERROR(pRegion);

			bRetCode = pRegion->Init(this, nX, nY);
			LOG_PROCESS_ERROR(bRetCode);
			bRegionInitFlag = TRUE;

			if (!m_pTerrainScene->m_bLoadFlag)
			{
				bRetCode = pRegion->LoadTerrain();
				LOG_PROCESS_ERROR(bRetCode);
			}

			bRetCode = SetRegion(nX, nY, pRegion);
			LOG_PROCESS_ERROR(bRetCode);

			pRegion->SetActive();

			pRegion            = NULL;
			bRegionInitFlag    = FALSE;
		}
	}

	// 设置载入标记
	m_pTerrainScene->m_bLoadFlag = true;

	bResult = TRUE;
EXIT0:
	if (bRegionInitFlag)
	{
		pRegion->UnInit();
		bRegionInitFlag = FALSE;
	}
	SAFE_DELETE(pRegion);
	return bResult;
}

static const char szMapBaseInfoSection[]	= "BaseInfo";
static const char szTimeStamp[]             = "TimeStamp";
static const char szRegionCountX[]          = "RegionCountX";
static const char szRegionCountY[]          = "RegionCountY";

BOOL KScene::LoadBaseInfo()
{
    BOOL        bResult                 = FALSE;
    INT         nRetCode                = 0;
    IIniFile*   piFile                  = NULL;
    char        szFileName[MAX_PATH];

	snprintf(szFileName, sizeof(szFileName), "%s/%s.scn", m_szPath, m_szClass);
    szFileName[sizeof(szFileName) - 1] = '\0';

	piFile = g_OpenIniFile(szFileName);
	if ( ! piFile)
		QLogPrintf(LOG_ERR, "无法找到场景 [%s]", szFileName);
    LOG_PROCESS_ERROR(piFile);

    nRetCode = piFile->GetStruct(szMapBaseInfoSection, szTimeStamp, &m_nTimeStamp, sizeof(m_nTimeStamp));
    //LOG_PROCESS_ERROR(nRetCode == 1);

	nRetCode = piFile->GetInteger(szMapBaseInfoSection, szRegionCountX, 0, &m_nRegionWidth);
	LOG_PROCESS_ERROR(nRetCode == 1);

	nRetCode = piFile->GetInteger(szMapBaseInfoSection, szRegionCountY, 0, &m_nRegionHeight);
	LOG_PROCESS_ERROR(nRetCode == 1);

	LOG_PROCESS_ERROR(m_nRegionWidth > 0 && m_nRegionWidth < MAX_REGION_WIDTH);
	LOG_PROCESS_ERROR(m_nRegionHeight > 0 && m_nRegionHeight < MAX_REGION_HEIGHT);

    bResult = TRUE;
EXIT0:
    SAFE_RELEASE(piFile);
    return bResult;
}

BOOL KScene::ValidateRegions(INT nRegionX, INT nRegionY)
{
    BOOL            bResult             = FALSE;
    BOOL            bRetCode            = FALSE;
    INT             nLeft               = nRegionX - m_nBroadcastRegion;
    INT             nRight              = nRegionX + m_nBroadcastRegion;
    INT             nBottom             = nRegionY - m_nBroadcastRegion;
    INT             nTop                = nRegionY + m_nBroadcastRegion;
    KRegion*        pAllocRegion        = NULL;
    BOOL            bAllocRegionInit    = FALSE;

	for (INT nX = nLeft; nX <= nRight; nX++)
	{
		for (INT nY = nBottom; nY <= nTop; nY++)
		{
            if (nX < 0 || nX >= m_nRegionWidth)
                continue;

            if (nY < 0 || nY >= m_nRegionHeight)
                continue;
			
            KRegion* pRegion = GetRegion(nX, nY);
            if (pRegion != NULL)
            {
                pRegion->SetActive();
                continue;
            }

			//pAllocRegion = KMemory::New<KRegion>();
			pAllocRegion = new KRegion;
		    LOG_PROCESS_ERROR(pAllocRegion);

		    bRetCode = pAllocRegion->Init(this, nX, nY);
		    LOG_PROCESS_ERROR(bRetCode);
            bAllocRegionInit = TRUE;

		    bRetCode = pAllocRegion->LoadTerrain();
		    LOG_PROCESS_ERROR(bRetCode);

		    bRetCode = SetRegion(nX, nY, pAllocRegion);
		    LOG_PROCESS_ERROR(bRetCode);
            
            pAllocRegion->SetActive();

            pAllocRegion        = NULL;
            bAllocRegionInit    = FALSE;
		}
	}

    bResult = TRUE;
EXIT0:
    if (bAllocRegionInit)
    {
        pAllocRegion->UnInit();
        bAllocRegionInit = FALSE;
    }

	SAFE_DELETE(pAllocRegion);
    //if (pAllocRegion)
    //{
        //KMemory::Delete(pAllocRegion);
    //}
    return bResult;
}

BOOL KScene::GetObstacle(INT nXCell, INT nYCell, INT nZ, KCell::KBaseInfo& rBaseInfo, 
						 WORD& rwLowLayer, WORD& rwHighLayer)
{
	BOOL bRetCode = FALSE;

	DWORD dwRegionX = nXCell / REGION_GRID_WIDTH;
	DWORD dwRegionY = nYCell / REGION_GRID_HEIGHT;
    KCell* pCell = NULL;

	KRegion* pRegion = GetRegion(dwRegionX, dwRegionY);
	LOG_PROCESS_ERROR(pRegion);

	nXCell = nXCell % REGION_GRID_WIDTH;
	nYCell = nYCell % REGION_GRID_HEIGHT;

	pCell = pRegion->GetObstacle(nXCell, nYCell, nZ);
	LOG_PROCESS_ERROR(pCell);

	memcpy(&rBaseInfo, &(pCell->m_BaseInfo), sizeof(KCell::KBaseInfo));
	rwLowLayer = pCell->m_wLowLayer;
	rwHighLayer = pCell->m_wHighLayer;

	return TRUE;

EXIT0:

	return FALSE;
}

BOOL KScene::CheckObstacle(INT nXCell, INT nYCell, INT nLayer)
{
	BOOL bRetCode = FALSE;

	DWORD dwRegionX = nXCell / REGION_GRID_WIDTH;
	DWORD dwRegionY = nYCell / REGION_GRID_HEIGHT;

	KRegion* pRegion = GetRegion(dwRegionX, dwRegionY);
	PROCESS_SUCCESS(!pRegion);

	nXCell = nXCell % REGION_GRID_WIDTH;
	nYCell = nYCell % REGION_GRID_HEIGHT;

	return pRegion->CheckObstacle(nXCell, nYCell, nLayer);

EXIT1: 
	return TRUE;
}

BOOL KScene::IsPointInSceneSpace(INT nX, INT nY, INT nZ)
{
     if (nX < 0 || nX >= CELL_LENGTH * REGION_GRID_WIDTH * m_nRegionWidth)
         return FALSE;

     if (nY < 0 || nY >= CELL_LENGTH * REGION_GRID_HEIGHT * m_nRegionHeight)
         return FALSE;

     if (nZ < 0 || nZ > MAX_Z_POINT)
         return FALSE;

     return TRUE;
}

INT KScene::GetRegionWidth(VOID)
{
	return m_nRegionWidth;
}

INT KScene::GetRegionHeight(VOID)
{
	return m_nRegionHeight;
}

BOOL KScene::SetRegion(INT nRegionX, INT nRegionY, KRegion* pRegion)
{
	QCONFIRM_RET_FALSE(pRegion);
	QCONFIRM_RET_FALSE(nRegionX < MAX_REGION_WIDTH);
	QCONFIRM_RET_FALSE(nRegionY < MAX_REGION_HEIGHT);
    QCONFIRM_RET_FALSE(m_RegionList[nRegionX][nRegionY] == NULL);

	m_RegionList[nRegionX][nRegionY] = pRegion;

	return TRUE;
}

KRegion* KScene::GetRegion(INT nRegionX, INT nRegionY)
{
	if (nRegionX >= 0 && nRegionX < m_nRegionWidth && nRegionY >= 0 && nRegionY < m_nRegionHeight)
    {
		return m_RegionList[nRegionX][nRegionY];
    }
	return NULL;
}

VOID KScene::DeleteAllObjects()
{
    for (INT nY = 0; nY < m_nRegionHeight; nY++)
    {
        for (INT nX = 0; nX < m_nRegionWidth; nX++)
        {
            KRegion* pRegion = GetRegion(nX, nY);
            if (pRegion)
            {
                pRegion->DeleteAllObject();
            }
        }
    }
}

VOID KScene::DeleteAllNpc()
{
	for (INT nY = 0; nY < m_nRegionHeight; nY++)
	{
		for (INT nX = 0; nX < m_nRegionWidth; nX++)
		{
			KRegion* pRegion = GetRegion(nX, nY);
			if (pRegion)
			{
				pRegion->DeleteAllNpc();
			}
		}
	}
}

BOOL KScene::SetDynamicObstacleState(INT nGroupIndex, BOOL bState)
{
    if (nGroupIndex > INVALID_OBSTACLE_GROUP && nGroupIndex < MAX_OBSTACLE_GROUP)
    {
        m_DynamicObstacleSwitchTable[nGroupIndex] = (bState != 0);
        return TRUE;
    }

    return FALSE;
}

BOOL KScene::GetDynamicObstacleState(INT nGroupIndex)
{
    if (nGroupIndex > INVALID_OBSTACLE_GROUP && nGroupIndex < MAX_OBSTACLE_GROUP)
    {
        return m_DynamicObstacleSwitchTable[nGroupIndex];
    }
    return FALSE;
}

BOOL KScene::PreprocessCalculateData()
{
    BOOL bResult    = FALSE;
    BOOL bRetCode   = FALSE;

    bRetCode = ReviseCalculateGradient();
    LOG_PROCESS_ERROR(bRetCode);

    bRetCode = CalculateObstacleRange();
    LOG_PROCESS_ERROR(bRetCode);

    bResult = TRUE;
EXIT0:
    return bResult;
}

BOOL KScene::ReviseCalculateGradient()
{
    BOOL        bResult             = FALSE;    
    KCell*      pLowestCell         = NULL;
	KRegion*    pProcessRegion      = NULL;
    KCell*      pProcessCell        = NULL;
    INT         nProcessPointX      = 0;
    INT         nProcessPointY      = 0;
    INT         nLowerPointX        = 0;
    INT         nLowerPointY        = 0;
    INT         nLowerCellX         = 0;
    INT         nLowerCellY         = 0;
	KRegion*    pLowerRegion        = NULL;
    KCell*      pLowerCell          = NULL;
    INT         nGradientDirection  = 0;
    INT         nSceneMaxPointX     = 0;
    INT         nSceneMaxPointY     = 0;
    INT         nRelativeHeight     = 0;
    INT         nDistanceXY         = 0;
    INT         nGradientDegree     = 0;
    INT         nOriginalGradient   = 0;

    nSceneMaxPointX = m_nRegionWidth * REGION_GRID_WIDTH * CELL_LENGTH;
    nSceneMaxPointY = m_nRegionHeight * REGION_GRID_HEIGHT * CELL_LENGTH;

	for (INT nYCell = 0; nYCell < m_nRegionHeight * REGION_GRID_HEIGHT; nYCell++)
	{
		for (INT nXCell = 0; nXCell < m_nRegionWidth * REGION_GRID_WIDTH; nXCell++)
		{
            pProcessRegion = GetRegion(nXCell / REGION_GRID_WIDTH, nYCell / REGION_GRID_HEIGHT);
			LOG_PROCESS_ERROR(pProcessRegion);

			pLowestCell = pProcessRegion->GetLowestObstacle(
                nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT
            );
            LOG_PROCESS_ERROR(pLowestCell);

            for (pProcessCell = pLowestCell; pProcessCell != NULL; pProcessCell = pProcessCell->m_pNext)
            {
				if (pProcessCell->m_BaseInfo.dwCellType == ctWater)
                    continue;

                nOriginalGradient = pProcessCell->GetGradientDegree();
                if (!nOriginalGradient)
                    continue;

                nGradientDirection = pProcessCell->GetGradientDirection();
                
                nLowerPointX = CELL_LENGTH * nXCell + CELL_LENGTH / 2 + 
                    CELL_LENGTH * g_Cos(nGradientDirection) / SIN_COS_NUMBER;

                nLowerPointY = CELL_LENGTH * nYCell + CELL_LENGTH / 2 + 
                    CELL_LENGTH * g_Sin(nGradientDirection) / SIN_COS_NUMBER;

                if (nLowerPointX < 0 || nLowerPointX >= nSceneMaxPointX)
                    continue;

                if (nLowerPointY < 0 || nLowerPointY >= nSceneMaxPointY)
                    continue;

                nLowerCellX = nLowerPointX / CELL_LENGTH;
                nLowerCellY = nLowerPointY / CELL_LENGTH;

                nLowerPointX = nLowerCellX * CELL_LENGTH + CELL_LENGTH / 2;
                nLowerPointY = nLowerCellY * CELL_LENGTH + CELL_LENGTH / 2;

                pLowerRegion = GetRegionByPoint(nLowerPointX, nLowerPointY);
                LOG_PROCESS_ERROR(pLowerRegion);

                pLowerCell = pLowerRegion->GetAdaptedCellForObj(
                    nLowerCellX % REGION_GRID_WIDTH, nLowerCellY % REGION_GRID_HEIGHT,
                    pProcessCell->m_wHighLayer * POINT_PER_ALTITUDE,
                    0, 0, FALSE,
                    &nRelativeHeight, NULL
                );
                if (!pLowerCell)
                {
                    pProcessCell->SetGradientDegree(0);
                    continue;
                }

                if (pLowerCell->m_BaseInfo.dwCellType == ctWater)
                {
                    nRelativeHeight += (pLowerCell->m_wHighLayer - pLowerCell->m_wLowLayer) * POINT_PER_ALTITUDE;
                }

                nProcessPointX = nXCell * CELL_LENGTH + CELL_LENGTH / 2;
                nProcessPointY = nYCell * CELL_LENGTH + CELL_LENGTH / 2;

                nDistanceXY = (INT)sqrt(
                    (float)(nLowerPointX - nProcessPointX) * (nLowerPointX - nProcessPointX) +
                    (float)(nLowerPointY - nProcessPointY) * (nLowerPointY - nProcessPointY)
                );

                nGradientDegree = g_GetDirection(
                    0, 0, XYPOINT_TO_ZPOINT(nDistanceXY), nRelativeHeight
                );

                MAKE_IN_RANGE(nGradientDegree, 0, DIRECTION_COUNT / 4 - 1);
                
                pProcessCell->SetGradientDegree(nGradientDegree);
            }
        }
    }

    bResult = TRUE;
EXIT0:
    return bResult;
}

BOOL KScene::CalculateObstacleRange()
{
	INT nLastDistance = 0;
	for (INT nYCell = 0; nYCell < m_nRegionHeight * REGION_GRID_HEIGHT; nYCell++)
	{
		for (INT nXCell = 0; nXCell < m_nRegionWidth * REGION_GRID_WIDTH; nXCell++)
		{
			KRegion* pRegion = GetRegion(nXCell / REGION_GRID_WIDTH, nYCell / REGION_GRID_HEIGHT);
			LOG_PROCESS_ERROR(pRegion);

			KCell* pCell = pRegion->GetLowestObstacle(nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT);
			
			INT nDistance = (nLastDistance - 1) >= 0 ? nLastDistance : 0;
			while (nDistance < 64)
			{
				BOOL bIsObstacle = FALSE;

				for (INT nDestX = nXCell - nDistance; nDestX < nXCell + nDistance; nDestX++)
				{
					INT nDestY = nYCell - nDistance;
					KRegion* pDestRegion = GetRegion(nDestX / REGION_GRID_WIDTH, nDestY / REGION_GRID_HEIGHT);
					if (pDestRegion && nDestX >= 0 && nDestY >= 0)
					{
						KCell* pDestCell = pDestRegion->GetLowestObstacle(nDestX % REGION_GRID_WIDTH, nDestY % REGION_GRID_HEIGHT);

						if (pDestCell->m_wHighLayer > pCell->m_wHighLayer + 8 || pDestCell->m_pNext != NULL)
						{
							bIsObstacle = TRUE;
							break;
						}
					}

					nDestY = nYCell + nDistance;
					pDestRegion = GetRegion(nDestX / REGION_GRID_WIDTH, nDestY / REGION_GRID_HEIGHT);
					if (pDestRegion && nDestX >= 0 && nDestY >= 0)
					{
						KCell* pDestCell = pDestRegion->GetLowestObstacle(nDestX % REGION_GRID_WIDTH, nDestY % REGION_GRID_HEIGHT);

						if (pDestCell->m_wHighLayer > pCell->m_wHighLayer + 8 || pDestCell->m_pNext != NULL)
						{
							bIsObstacle = TRUE;
							break;
						}
					}
				}
				if (bIsObstacle)
					break;

				for (INT nDestY = nYCell - nDistance; nDestY < nYCell + nDistance; nDestY++)
				{
					INT nDestX = nXCell - nDistance;
					KRegion* pDestRegion = GetRegion(nDestX / REGION_GRID_WIDTH, nDestY / REGION_GRID_HEIGHT);
					if (pDestRegion && nDestX >= 0 && nDestY >= 0)
					{
						KCell* pDestCell = pDestRegion->GetLowestObstacle(nDestX % REGION_GRID_WIDTH, nDestY % REGION_GRID_HEIGHT);

						if (pDestCell->m_wHighLayer > pCell->m_wHighLayer + 8 || pDestCell->m_pNext != NULL)
						{
							bIsObstacle = TRUE;
							break;
						}
					}

					nDestX = nXCell + nDistance;
					pDestRegion = GetRegion(nDestX / REGION_GRID_WIDTH, nDestY / REGION_GRID_HEIGHT);
					if (pDestRegion && nDestX >= 0 && nDestY >= 0)
					{
						KCell* pDestCell = pDestRegion->GetLowestObstacle(nDestX % REGION_GRID_WIDTH, nDestY % REGION_GRID_HEIGHT);

						if (pDestCell->m_wHighLayer > pCell->m_wHighLayer + 8 || pDestCell->m_pNext != NULL)
						{
							bIsObstacle = TRUE;
							break;
						}
					}
				}
				if (bIsObstacle)
					break;

				nDistance++;
			}

			pCell->m_BaseInfo.dwNoObstacleRange = nDistance < 64 ? nDistance : 63;
			nLastDistance = nDistance;
		}
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL KScene::IsVisible(INT nSrcX, INT nSrcY, INT nSrcZ, INT nDstX, INT nDstY, INT nDstZ)
{
    BOOL        bResult             = FALSE;    // 默认有障碍
    KRegion*    pSrcRegion          = NULL;
    KCell*      pSrcCell            = NULL;
    INT         nSrcCellX           = 0;
    INT         nSrcCellY           = 0;
    DWORD       dwSrcRegionX        = 0;
    DWORD       dwSrcRegionY        = 0;
    INT         nDstCellX           = 0;   
    INT         nDstCellY           = 0;   
    DWORD       dwDstRegionX        = 0;
    DWORD       dwDstRegionY        = 0;
    KRegion*    pDstRegion          = NULL;
    KCell*      pDstCell            = NULL;
    INT         nNoObstacleRange    = 0;
    INT         nDeltaX             = 0;
    INT         nDeltaY             = 0;
    INT         nLastAltitude       = 0;
    INT         nProportionX        = 0;
    INT         nProportionY        = 0;
    INT         nProportionZ        = 0;
    KRegion*    pRegion             = NULL;
    KCell*      pCell               = NULL; 
    INT         nAltitude           = 0;
    INT         nXCell              = 0;
    INT         nYCell              = 0;
    INT         nZCell              = 0;
    DWORD       dwRegionX           = 0;
    DWORD       dwRegionY           = 0;
    DWORD       dwRegionZ           = 0;

    nSrcCellX       = nSrcX / CELL_LENGTH;
    nSrcCellY       = nSrcY / CELL_LENGTH;
	dwSrcRegionX    = nSrcCellX / REGION_GRID_WIDTH;
	dwSrcRegionY    = nSrcCellY / REGION_GRID_HEIGHT;

    pSrcRegion     = GetRegion(dwSrcRegionX, dwSrcRegionY);
    PROCESS_ERROR(pSrcRegion);

	pSrcCell = pSrcRegion->GetLowestObstacle(nSrcCellX % REGION_GRID_WIDTH, nSrcCellY % REGION_GRID_HEIGHT);
	PROCESS_ERROR(pSrcCell);

    nDstCellX       = nDstX / CELL_LENGTH;
    nDstCellY       = nDstY / CELL_LENGTH;
	dwDstRegionX    = nDstCellX / REGION_GRID_WIDTH;
	dwDstRegionY    = nDstCellY / REGION_GRID_HEIGHT;

    pDstRegion = GetRegion(dwDstRegionX, dwDstRegionY);
    PROCESS_ERROR(pDstRegion);

	pDstCell = pDstRegion->GetLowestObstacle(nDstCellX % REGION_GRID_WIDTH, nDstCellY % REGION_GRID_HEIGHT);
	PROCESS_ERROR(pDstCell);

	if (nSrcCellX == nDstCellX && nSrcCellY == nDstCellY)
	{
        // 同一个Cell
        goto EXIT1;
	}
  
	// 检测是否在无障碍范围内
	nNoObstacleRange = 
		pSrcCell->m_BaseInfo.dwNoObstacleRange > pDstCell->m_BaseInfo.dwNoObstacleRange ?
		pSrcCell->m_BaseInfo.dwNoObstacleRange : pDstCell->m_BaseInfo.dwNoObstacleRange;

   
    // 采用这个判断时会跳过地形因素造成的不可见,所以暂时去掉
    /*   
	if (abs(nSrcCellX - nDstCellX) < nNoObstacleRange && abs(nSrcCellY - nDstCellY) < nNoObstacleRange)
    {
        // 没有障碍
        goto EXIT0;
    }
    */

	//INT nDirection = g_GetDirection(nSrcCellX, nSrcCellY, nDstCellX, nDstCellY);

	//if ((nDirection < (DIRECTION_COUNT * 1 / 8) || nDirection > (DIRECTION_COUNT * 7 / 8)) ||
	//	(nDirection > (DIRECTION_COUNT * 3 / 8) && nDirection < (DIRECTION_COUNT * 5 / 8)))

    nDeltaX         = abs(nDstCellX - nSrcCellX);
    nDeltaY         = abs(nDstCellY - nSrcCellY);
	nLastAltitude   = nSrcZ;

    if (nDeltaX > nDeltaY)
	{
		//计算Y轴和Z轴上的比例

        LOG_PROCESS_ERROR(nDeltaX);

		nProportionY = (nDstCellY - nSrcCellY) * SIN_COS_NUMBER / (nDstCellX - nSrcCellX);
		nProportionZ = (nDstZ - nSrcZ) * SIN_COS_NUMBER / (nDstCellX - nSrcCellX);

		if ((nSrcCellX < nDstCellX) && (nSrcZ <= nDstZ))
		{
			for (nXCell = (nSrcCellX + 1); nXCell <= nDstCellX; nXCell++)
			{
				nYCell = nSrcCellY + (nXCell - nSrcCellX) * nProportionY / SIN_COS_NUMBER;

				dwRegionX = nXCell / REGION_GRID_WIDTH;
				dwRegionY = nYCell / REGION_GRID_HEIGHT;

				pRegion = GetRegion(dwRegionX, dwRegionY);
				PROCESS_ERROR(pRegion);
					
                pCell = pRegion->GetLowestObstacle(nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT);

				nAltitude = nSrcZ + (nXCell - nSrcCellX) * nProportionZ / SIN_COS_NUMBER;

				while (pCell)
				{
					PROCESS_ERROR(
                        pCell->m_wLowLayer >= nAltitude || 
                        pCell->m_wHighLayer <= nLastAltitude || 
                        pCell->m_BaseInfo.dwCellType == ctWater
                    );
				    pCell = pCell->m_pNext;
				}
				nLastAltitude = nAltitude;
			}
		}
        else if ((nSrcCellX < nDstCellX) && (nSrcZ > nDstZ))
        {
			for (nXCell = (nSrcCellX + 1); nXCell <= nDstCellX; nXCell++)
			{
				nYCell = nSrcCellY + (nXCell - nSrcCellX) * nProportionY / SIN_COS_NUMBER;

				dwRegionX = nXCell / REGION_GRID_WIDTH;
				dwRegionY = nYCell / REGION_GRID_HEIGHT;

				pRegion = GetRegion(dwRegionX, dwRegionY);
				PROCESS_ERROR(pRegion);
					
                pCell = pRegion->GetLowestObstacle(nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT);

                nAltitude = nSrcZ + (nXCell - nSrcCellX) * nProportionZ / SIN_COS_NUMBER;

				while (pCell)
				{
					PROCESS_ERROR(
                        pCell->m_wLowLayer >= nLastAltitude || 
                        pCell->m_wHighLayer <= nAltitude || 
                        pCell->m_BaseInfo.dwCellType == ctWater
                    );
				    pCell = pCell->m_pNext;
				}
				nLastAltitude = nAltitude;
			}
        }
		else if ((nSrcCellX > nDstCellX) && (nSrcZ <= nDstZ))
		{
			for (nXCell = (nSrcCellX - 1); nXCell >= nDstCellX; nXCell--)
			{
				nYCell = nSrcCellY + (nXCell - nSrcCellX) * nProportionY / SIN_COS_NUMBER;

				dwRegionX = nXCell / REGION_GRID_WIDTH;
				dwRegionY = nYCell / REGION_GRID_HEIGHT;

				pRegion = GetRegion(dwRegionX, dwRegionY);
                PROCESS_ERROR(pRegion);
					
                pCell = pRegion->GetLowestObstacle(nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT);

				nAltitude = nSrcZ + (nXCell - nSrcCellX) * nProportionZ / SIN_COS_NUMBER;

				while (pCell)
				{
					PROCESS_ERROR(
                        pCell->m_wLowLayer >= nAltitude || 
                        pCell->m_wHighLayer <= nLastAltitude || 
                        pCell->m_BaseInfo.dwCellType == ctWater
                    );
				    pCell = pCell->m_pNext;
				}
				nLastAltitude = nAltitude;				
			}
		}
        else if ((nSrcCellX > nDstCellX) && (nSrcZ > nDstZ))
        {
			for (nXCell = (nSrcCellX - 1); nXCell >= nDstCellX; nXCell--)
			{
				nYCell = nSrcCellY + (nXCell - nSrcCellX) * nProportionY / SIN_COS_NUMBER;

				dwRegionX = nXCell / REGION_GRID_WIDTH;
				dwRegionY = nYCell / REGION_GRID_HEIGHT;

				pRegion = GetRegion(dwRegionX, dwRegionY);
                PROCESS_ERROR(pRegion);
					
                pCell = pRegion->GetLowestObstacle(nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT);

				nAltitude = nSrcZ + (nXCell - nSrcCellX) * nProportionZ / SIN_COS_NUMBER;

				while (pCell)
				{
					PROCESS_ERROR(
                        pCell->m_wLowLayer >= nLastAltitude || 
                        pCell->m_wHighLayer <= nAltitude || 
                        pCell->m_BaseInfo.dwCellType == ctWater
                    );
				    pCell = pCell->m_pNext;
				}
				nLastAltitude = nAltitude;				
			}
        }
	}
	else
	{
		//计算X轴和Z轴的比例

        LOG_PROCESS_ERROR(nDeltaY);

		nProportionX = (nDstCellX - nSrcCellX) * SIN_COS_NUMBER / (nDstCellY - nSrcCellY);
		nProportionZ = (nDstZ - nSrcZ) * SIN_COS_NUMBER / (nDstCellY - nSrcCellY);

		if ((nSrcCellY < nDstCellY) && (nSrcZ <= nDstZ))
		{
			for (nYCell = (nSrcCellY + 1); nYCell <= nDstCellY; nYCell++)
			{
				nXCell = nSrcCellX + (nYCell - nSrcCellY) * nProportionX / SIN_COS_NUMBER;

				dwRegionX = nXCell / REGION_GRID_WIDTH;
				dwRegionY = nYCell / REGION_GRID_HEIGHT;

				pRegion = GetRegion(dwRegionX, dwRegionY);
                PROCESS_ERROR(pRegion);

				pCell = pRegion->GetLowestObstacle(nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT);

                nAltitude = nSrcZ + (nYCell - nSrcCellY) * nProportionZ / SIN_COS_NUMBER;

				while (pCell)
				{
					PROCESS_ERROR(
                        pCell->m_wLowLayer >= nAltitude || 
                        pCell->m_wHighLayer <= nLastAltitude || 
                        pCell->m_BaseInfo.dwCellType == ctWater
                    );
					pCell = pCell->m_pNext;
				}
				nLastAltitude = nAltitude;
			}
		}
        else if ((nSrcCellY < nDstCellY) && (nSrcZ > nDstZ))
        {
			for (nYCell = (nSrcCellY + 1); nYCell <= nDstCellY; nYCell++)
			{
				nXCell = nSrcCellX + (nYCell - nSrcCellY) * nProportionX / SIN_COS_NUMBER;

				dwRegionX = nXCell / REGION_GRID_WIDTH;
				dwRegionY = nYCell / REGION_GRID_HEIGHT;

				pRegion = GetRegion(dwRegionX, dwRegionY);
                PROCESS_ERROR(pRegion);

				pCell = pRegion->GetLowestObstacle(nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT);

				nAltitude = nSrcZ + (nYCell - nSrcCellY) * nProportionZ / SIN_COS_NUMBER;

				while (pCell)
				{
					PROCESS_ERROR(
                        pCell->m_wLowLayer >= nLastAltitude || 
                        pCell->m_wHighLayer <= nAltitude || 
                        pCell->m_BaseInfo.dwCellType == ctWater
                    );
					pCell = pCell->m_pNext;
				}
				nLastAltitude = nAltitude;
			}
        }
		else  if ((nSrcCellY > nDstCellY) && (nSrcZ <= nDstZ))
		{
			for (nYCell = (nSrcCellY - 1); nYCell >= nDstCellY; nYCell--)
			{
				nXCell = nSrcCellX + (nYCell - nSrcCellY) * nProportionX / SIN_COS_NUMBER;

				dwRegionX = nXCell / REGION_GRID_WIDTH;
				dwRegionY = nYCell / REGION_GRID_HEIGHT;

				pRegion = GetRegion(dwRegionX, dwRegionY);
                PROCESS_ERROR(pRegion);

				pCell = pRegion->GetLowestObstacle(nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT);

				nAltitude = nSrcZ + (nYCell - nSrcCellY) * nProportionZ / SIN_COS_NUMBER;

				while (pCell)
				{
					PROCESS_ERROR(
                        pCell->m_wLowLayer >= nAltitude || 
                        pCell->m_wHighLayer <= nLastAltitude ||
                        pCell->m_BaseInfo.dwCellType == ctWater
                    );
					pCell = pCell->m_pNext;
				}
				nLastAltitude = nAltitude;
			}
		}
        else if ((nSrcCellY > nDstCellY) && (nSrcZ > nDstZ))
        {
			for (nYCell = (nSrcCellY - 1); nYCell >= nDstCellY; nYCell--)
			{
				nXCell = nSrcCellX + (nYCell - nSrcCellY) * nProportionX / SIN_COS_NUMBER;

				dwRegionX = nXCell / REGION_GRID_WIDTH;
				dwRegionY = nYCell / REGION_GRID_HEIGHT;

				pRegion = GetRegion(dwRegionX, dwRegionY);
                PROCESS_ERROR(pRegion);

				pCell = pRegion->GetLowestObstacle(nXCell % REGION_GRID_WIDTH, nYCell % REGION_GRID_HEIGHT);

				nAltitude = nSrcZ + (nYCell - nSrcCellY) * nProportionZ / SIN_COS_NUMBER;

				while (pCell)
				{
					PROCESS_ERROR(
                        pCell->m_wLowLayer >= nLastAltitude || 
                        pCell->m_wHighLayer <= nAltitude ||
                        pCell->m_BaseInfo.dwCellType == ctWater
                    );
					pCell = pCell->m_pNext;
				}
				nLastAltitude = nAltitude;
			}
        }
	}    

EXIT1:
    bResult = TRUE;
EXIT0:
    return bResult;
}

BOOL KScene::AddPlayer( KSceneObject* pSceneObj, INT x, INT y, INT z )
{
	BOOL        bResult     = FALSE;
	KRegion*    pRegion     = NULL;
	KPlayer*  pPlayer = (KPlayer*)pSceneObj;

	pPlayer->m_pScene = this;

	pPlayer->m_nX = x;
	pPlayer->m_nY = y;
	pPlayer->m_nZ = z;
	pPlayer->m_nXCell = x / CELL_LENGTH % REGION_GRID_WIDTH;
	pPlayer->m_nYCell = y / CELL_LENGTH % REGION_GRID_HEIGHT;

	pRegion = GetRegionByPoint(pPlayer->m_nX, pPlayer->m_nY);
	LOG_PROCESS_ERROR(pRegion);

	bResult = pRegion->AddPlayer(pPlayer);
	LOG_PROCESS_ERROR(bResult);

#ifdef GAME_SERVER
	pPlayer->ResetRegionFrames(m_nBroadcastRegion);

	ValidateRegions(pRegion->m_nRegionX, pRegion->m_nRegionY);

	// 玩家禁止回滚到进入场景之前
	//pPlayer->m_nRecordCount = 0;
	//pPlayer->FireEvent(emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, pPlayer->GetId(), FALSE, 0);
#endif

	Q_Printl("[" << pPlayer->m_dwId << "][" << pPlayer->GetName() <<
		"] 已进入[" << m_dwId << "][" << m_szName << "]");

	g_RelayClient.DoPlayerEnterSceneNotify(pPlayer->m_dwId, m_dwTemplateId, GET_MAP_COPY_INDEX(m_dwId));

	bResult = TRUE;
EXIT0:
	if (!pRegion)
	{
		Q_Printl("[" << pPlayer->m_dwId << "][" << pPlayer->GetName() << "] Coordinate[" <<
			x << "," << y << ","  << z << "] Out Of Scene[" << m_dwId << "][" << m_szName << "]!");
	}
	return bResult;
}

BOOL KScene::RemovePlayer( KSceneObject* pPlayer )
{
	BOOL bResult = FALSE;
	KRegion* pRegion = pPlayer->m_pRegion;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

	LOG_PROCESS_ERROR(pRegion);
	cSafeScript->CallTableFunction("Scene", "OnLeaveScene", 0, "dd",
		pPlayer->GetId(), 0);

	bResult = pRegion->RemovePlayer(pPlayer);
	LOG_PROCESS_ERROR(bResult);

	pPlayer->m_pScene = NULL;

#ifdef GAME_SERVER
	// 临时屏蔽副本回收 [6/28/2010 simon]
//	if (m_dwId > 65536 && m_nPlayerCount <= 0)
//	{
//		// recycle this copy
//		g_cOrpgWorld.m_cEventCreator.FireEvent(emKOBJEVENTTYPE_SCENE_RECYCLE, m_dwId, 0, 0);
//	}
#endif // GAME_SERVER
#ifdef GAME_CLIENT
	QLogPrintf(LOG_DEBUG, "Id[0x%x][%s] left [%d][%s]",
		pPlayer->m_dwId, pPlayer->GetName(), m_dwId, m_szName);
#endif
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KScene::AddNpc( KSceneObject* pObj, INT x, INT y, INT z )
{
	BOOL		bResult		= FALSE;
	BOOL		bRetCode	= FALSE;
	KRegion*	pRegion		= NULL;
	KNpc*		pNpc		= (KNpc*)pObj;
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	
	pNpc->m_nX = x;
	pNpc->m_nY = y;
	pNpc->m_nZ = z;
	pNpc->m_nXCell = x / CELL_LENGTH % REGION_GRID_WIDTH;
	pNpc->m_nYCell = y / CELL_LENGTH % REGION_GRID_HEIGHT;

#ifdef GAME_SERVER
	pNpc->ResetRegionFrames(m_nBroadcastRegion);
#endif

	pRegion = GetRegionByPoint(pNpc->m_nX, pNpc->m_nY);
	LOG_PROCESS_ERROR(pRegion);

	bRetCode = pRegion->AddNpc(pNpc);
	LOG_PROCESS_ERROR(bRetCode);

	pNpc->m_pScene = this;

	// TODO 临时移植
	if (pNpc->m_AIData.nAIType)
	{
		pNpc->m_AIVM.Setup(pNpc, pNpc->m_AIData.nAIType);
		LOG_PROCESS_ERROR(bRetCode);
	}

	if (pNpc->m_nNpcTeamID)
	{
		bRetCode = m_ThreatLink.AddMember(pNpc->m_nNpcTeamID, pNpc);
		LOG_PROCESS_ERROR(bRetCode);
	}

	if (pNpc->GetKind() != NPC_KIND_FIGHT) // 战斗npc不刷信息,测试代码
	{
		//QLogPrintf(LOG_DEBUG,"Id[%u][%s] 已进入场景[%d] F%d",
		//	pNpc->m_dwId, pNpc->GetName(),
		//	m_dwId, g_cOrpgWorld.m_dwLogicFrames);
	}

	cSafeScript->CallTableFunction("Scene", "OnEnterScene", 0, "d", pNpc->GetId());

	pNpc->m_cNpcServer.SyncChangeRegion();

	bResult = TRUE;
EXIT0:
	if (!pRegion)
	{
		//Q_Printl(pNpc->GetName() << " Id[" << pNpc->m_dwId << "] Coordinate[" <<
		//	x << "," << y << ","  << z << "] Out Of Scene[" << m_dwId << "][" << m_szName << "]!");
	}
	return bResult;
}

BOOL KScene::RemoveNpc( KSceneObject* pNpc )
{
	BOOL        bRetCode = false;
	KRegion*    pRegion  = NULL;

	pRegion = pNpc->m_pRegion;
	QCONFIRM_RET_FALSE(pRegion);

	bRetCode = pRegion->RemoveNpc(pNpc);

	pNpc->m_pScene = NULL;

#ifdef GAME_CLIENT
	//QLogPrintf(LOG_DEBUG, "Id[0x%x][%s] left [%d][%s]", pNpc->m_dwId, pNpc->GetName(), m_dwId, m_szName);
#endif

	return bRetCode;
}

BOOL KScene::AddDoodad( KSceneObject* pDoodad, INT x, INT y, INT z )
{
	BOOL        bResult     = false;
	BOOL        bRetCode    = false;
	KRegion*    pRegion     = NULL;

	pDoodad->m_nX = x;
	pDoodad->m_nY = y;
	pDoodad->m_nZ = z;
	pDoodad->m_nXCell = x / CELL_LENGTH % REGION_GRID_WIDTH;
	pDoodad->m_nYCell = y / CELL_LENGTH % REGION_GRID_HEIGHT;

#ifdef GAME_SERVER
	pDoodad->ResetRegionFrames(m_nBroadcastRegion);
#endif

	pRegion = GetRegionByPoint(pDoodad->m_nX, pDoodad->m_nY);
	PROCESS_ERROR(pRegion);
	pDoodad->m_pScene = this;

	bRetCode = pRegion->AddDoodad(pDoodad);
	LOG_PROCESS_ERROR(bRetCode);

	//SetDynamicObstacleState(pDoodad->m_nObstacleGroup, true);

#ifdef GAME_CLIENT
	//Q_Printl("Id[0x" << std::hex  << pDoodad->m_dwId <<std::dec <<
	//	"][" << pDoodad->GetName() << "] 已进入场景[" << m_dwId << "]" << m_szName);
#endif //#ifdef GAME_CLIENT

	bResult = true;
EXIT0:
	if (!pRegion)
	{
		//Q_Printl(pDoodad->GetName() << " Id[" << pDoodad->m_dwId << "] Coordinate[" <<
		//	x << "," << y << ","  << z << "] Out Of Scene[" << m_dwId << "][" << m_szName << "]!");
	}
	return bResult;
}

BOOL KScene::RemoveDoodad( KSceneObject* pDoodad )
{
	BOOL        bResult    = false;
	BOOL        bRetCode   = false;
	KRegion*	pRegion		= NULL;

	pRegion = pDoodad->m_pRegion;
	LOG_PROCESS_ERROR(pRegion);

	bRetCode = pRegion->RemoveDoodad(pDoodad);
	LOG_PROCESS_ERROR(bRetCode);

	//SetDynamicObstacleState(pDoodad->m_nObstacleGroup, false);

	if (g_cOrpgWorld.m_bDebugSelf)
	{
		//QLogPrintf(LOG_DEBUG, "Id[0x%x][%s] left [%d][%s]",
		//	pDoodad->m_dwId, pDoodad->GetName(), m_dwId, m_szName);
	}

	bResult = true;
EXIT0:
	return bResult;
}

#ifdef GAME_SERVER

inline BOOL KScene::FindOldNeighbors( IKTraverseHelper& rcHelper,
									 KSceneObject* pCenterObj,
									 DWORD dwType)
{
	BOOL bRet = FALSE;
	INT nLeft = 0;
	INT nRight = 0;
	INT nBottom = 0;
	INT nTop = 0;
	INT nIndex = 0;
	KRegion* pCenterRegion = NULL;
	INT nLeaveRegionFrame   = 0;
	INT nRegionLeaveFrameCount = 0;
	INT* pLeaveFrame = NULL;

	QCONFIRM_RET_FALSE(pCenterObj);
	QCONFIRM_RET_FALSE(pCenterObj->m_pScene);
	pCenterRegion = pCenterObj->m_pLastRegion;
	if (pCenterRegion == NULL)
	{
	    return FALSE;
	}

	nLeft       = pCenterRegion->m_nRegionX - m_nBroadcastRegion;
	nRight      = pCenterRegion->m_nRegionX + m_nBroadcastRegion;
	nBottom     = pCenterRegion->m_nRegionY - m_nBroadcastRegion;
	nTop        = pCenterRegion->m_nRegionY + m_nBroadcastRegion;

	nRegionLeaveFrameCount = pCenterObj->m_vRegionLeaveFrameTable.size();
	pLeaveFrame = &pCenterObj->m_vRegionLeaveFrameTable[0];
	for (INT nX = nLeft; nX <= nRight; ++nX)
	{
		for (INT nY = nBottom; nY <= nTop; ++nY)
		{
			QCONFIRM_RET_FALSE(nIndex < nRegionLeaveFrameCount);
			nLeaveRegionFrame = *pLeaveFrame++; // optimization for DEBUG

			if (g_cOrpgWorld.m_dwLogicFrames - nLeaveRegionFrame != SYNC_CHARACTER_DELAY)
				continue;

			KRegion* pRegion = GetRegion(nX, nY);
			if (pRegion == NULL)
				continue;

			switch (dwType)
			{
			case KD_PLAYER_ID_MASK:
				bRet = pRegion->TraversePlayer(rcHelper);
				break;
			case KD_NPC_ID_MASK:
				bRet = pRegion->TraverseNpc(rcHelper);
				break;
			case KD_DOODAD_ID_MASK:
				bRet = pRegion->TraverseDoodad(rcHelper);
				break;
			default:
				//Q_Error("Invalid Scene Object Type " << dwType);
				break;
			}
		}
	}

	return bRet;
}

BOOL KScene::FindOldNeighborPlayers( IKTraverseHelper& rcHelper, KSceneObject* pCenterObj )
{
	return FindOldNeighbors(rcHelper, pCenterObj, KD_PLAYER_ID_MASK);
}

BOOL KScene::FindOldNeighborNpcs( IKTraverseHelper& rcHelper, KSceneObject* pCenterObj )
{
	return FindOldNeighbors(rcHelper, pCenterObj, KD_NPC_ID_MASK);
}

BOOL KScene::FindOldNeighborDoodads( IKTraverseHelper& rcHelper, KSceneObject* pCenterObj )
{
	return FindOldNeighbors(rcHelper, pCenterObj, KD_DOODAD_ID_MASK);
}

inline BOOL KScene::FindNewNeighbors( IKTraverseHelper& rcHelper,
									 KSceneObject* pCenterObj,
									 DWORD dwType)
{
	BOOL bRet = FALSE;
	INT nLeft = 0;
	INT nRight = 0;
	INT nBottom = 0;
	INT nTop = 0;
	INT nIndex = 0;
	KRegion* pCenterRegion = NULL;
	INT nEnterRegionFrame   = 0;
	INT nRegionEnterFrameCount = 0;
	INT* pEnterFrame = NULL;

	QCONFIRM_RET_FALSE(pCenterObj);
	QCONFIRM_RET_FALSE(pCenterObj->m_pScene);
	pCenterRegion = pCenterObj->m_pRegion;
	QCONFIRM_RET_FALSE(pCenterRegion);

	nLeft       = pCenterRegion->m_nRegionX - m_nBroadcastRegion;
	nRight      = pCenterRegion->m_nRegionX + m_nBroadcastRegion;
	nBottom     = pCenterRegion->m_nRegionY - m_nBroadcastRegion;
	nTop        = pCenterRegion->m_nRegionY + m_nBroadcastRegion;

	nRegionEnterFrameCount = pCenterObj->m_vRegionEnterFrameTable.size();
	pEnterFrame = &pCenterObj->m_vRegionEnterFrameTable[0];
	for (INT nX = nLeft; nX <= nRight; ++nX)
	{
		for (INT nY = nBottom; nY <= nTop; ++nY)
		{
			//QCONFIRM_RET_FALSE(nIndex < (INT)pCenterObj->m_vRegionEnterFrameTable.size());

			//nEnterRegionFrame = pCenterObj->m_vRegionEnterFrameTable[nIndex];
			//nIndex++;

			QCONFIRM_RET_FALSE(nIndex < nRegionEnterFrameCount);
			nEnterRegionFrame = *pEnterFrame++; // optimization for DEBUG

			if (g_cOrpgWorld.m_dwLogicFrames - nEnterRegionFrame != SYNC_CHARACTER_DELAY)
				continue;

			KRegion* pRegion = GetRegion(nX, nY);
			if (pRegion == NULL)
				continue;

			switch (dwType)
			{
			case KD_PLAYER_ID_MASK:
				bRet = pRegion->TraversePlayer(rcHelper);
				break;
			case KD_NPC_ID_MASK:
				bRet = pRegion->TraverseNpc(rcHelper);
				break;
			case KD_DOODAD_ID_MASK:
				bRet = pRegion->TraverseDoodad(rcHelper);
				break;
			default:
				//Q_Error("Invalid Scene Object Type " << dwType);
				break;
			}
		}
	}

	return bRet;
}

BOOL KScene::FindNewNeighborPlayers( IKTraverseHelper& rcHelper, KSceneObject* pCenterObj )
{
	return FindNewNeighbors(rcHelper, pCenterObj, KD_PLAYER_ID_MASK);
}

BOOL KScene::FindNewNeighborNpcs( IKTraverseHelper& rcHelper, KSceneObject* pCenterObj )
{
	return FindNewNeighbors(rcHelper, pCenterObj, KD_NPC_ID_MASK);
}

BOOL KScene::FindNewNeighborDoodads( IKTraverseHelper& rcHelper, KSceneObject* pCenterObj )
{
	return FindNewNeighbors(rcHelper, pCenterObj, KD_DOODAD_ID_MASK);
}

#endif // GAME_SERVER

BOOL KScene::TraverseAll( IKTraverseHelper& rcHelper, DWORD dwType )
{
	BOOL bRet = FALSE;
	for (INT nX = m_nRegionWidth - 1; nX >= 0; --nX)
	{
		KRegion** ppRegion = (KRegion**)m_RegionList[nX];
		for (INT nY = m_nRegionHeight - 1; nY >= 0; --nY)
		{
			KRegion* pRegion = ppRegion[nY];

			if (!pRegion)
				continue;

			switch (dwType)
			{
			case KD_PLAYER_ID_MASK:
				bRet = pRegion->TraversePlayer(rcHelper);
				break;
			case KD_NPC_ID_MASK:
				bRet = pRegion->TraverseNpc(rcHelper);
				break;
			case KD_DOODAD_ID_MASK:
				bRet = pRegion->TraverseDoodad(rcHelper);
				break;
			default:
				//Q_Error("Invalid Scene Object Type " << dwType);
				break;
			}
		}
	}

	return bRet;
}

BOOL KScene::TraverseNearbyPlayers( IKTraverseHelper& rcHelper, KSceneObject* pCenterObj )
{
	return TraverseNearbyObj(rcHelper, pCenterObj, KD_PLAYER_ID_MASK);
}

BOOL KScene::TraverseNearbyPlayers( IKTraverseHelper& rcHelper, INT nX, INT nY )
{
	return TraverseNearbyObj(rcHelper, nX, nY, KD_PLAYER_ID_MASK);
}

BOOL KScene::TraverseAllPlayers( IKTraverseHelper& rcHelper )
{
	return TraverseAll(rcHelper, KD_PLAYER_ID_MASK);
}

BOOL KScene::TraverseNearbyNpcs( IKTraverseHelper& rcHelper, KSceneObject* pCenterObj )
{
	return TraverseNearbyObj(rcHelper, pCenterObj, KD_NPC_ID_MASK);
}

BOOL KScene::TraverseNearbyNpcs( IKTraverseHelper& rcHelper, INT nX, INT nY )
{
	return TraverseNearbyObj(rcHelper, nX, nY, KD_NPC_ID_MASK);
}

BOOL KScene::TraverseAllNpcs( IKTraverseHelper& rcHelper )
{
	return TraverseAll(rcHelper, KD_NPC_ID_MASK);
}

BOOL KScene::TraverseNearbyDoodads( IKTraverseHelper& rcHelper, KSceneObject* pCenterObj )
{
	return TraverseNearbyObj(rcHelper, pCenterObj, KD_DOODAD_ID_MASK);
}

BOOL KScene::TraverseNearbyDoodads( IKTraverseHelper& rcHelper, INT nX, INT nY )
{
	return TraverseNearbyObj(rcHelper, nX, nY, KD_DOODAD_ID_MASK);
}

BOOL KScene::TraverseAllDoodads( IKTraverseHelper& rcHelper )
{
	return TraverseAll(rcHelper, KD_DOODAD_ID_MASK);
}

BOOL KScene::TraverseNearbyObj(IKTraverseHelper& rcHelper, INT nPointX, INT nPointY, DWORD dwType)
{
	BOOL bRet = FALSE;
	INT nLeft = 0;
	INT nRight = 0;
	INT nBottom = 0;
	INT nTop = 0;
	KRegion* pCenterRegion = GetRegionByPoint(nPointX, nPointY);
	if(NULL == pCenterRegion)
	{
		Q_Error("nPointX"<<nPointX<<"  nPointY " <<nPointY<< "  dwType" << dwType);
	}
	QCONFIRM_RET_FALSE(pCenterRegion);

	nLeft       = pCenterRegion->m_nRegionX - m_nBroadcastRegion;
	nRight      = pCenterRegion->m_nRegionX + m_nBroadcastRegion;
	nBottom     = pCenterRegion->m_nRegionY - m_nBroadcastRegion;
	nTop        = pCenterRegion->m_nRegionY + m_nBroadcastRegion;
	
	for (INT nX = nLeft; nX <= nRight; nX++)
	{
		for (INT nY = nBottom; nY <= nTop; nY++)
		{
			KRegion* pRegion = GetRegion(nX, nY);

			if (!pRegion)
				continue;

			switch (dwType)
			{
			case KD_PLAYER_ID_MASK:
				bRet = pRegion->TraversePlayer(rcHelper);
				break;
			case KD_NPC_ID_MASK:
				bRet = pRegion->TraverseNpc(rcHelper);
				break;
			case KD_DOODAD_ID_MASK:
				bRet = pRegion->TraverseDoodad(rcHelper);
				break;
			default:
				Q_Error("Invalid Scene Object Type " << dwType);
				break;
			}
		}
	}
	

	return bRet;
}

BOOL KScene::TraverseNearbyObj(IKTraverseHelper& rcHelper,
							   KSceneObject* pCenterObj,
							   DWORD dwType)
{
	BOOL bRet = FALSE;
	INT nLeft = 0;
	INT nRight = 0;
	INT nBottom = 0;
	INT nTop = 0;
	KRegion* pCenterRegion = NULL;

	QCONFIRM_RET_FALSE(pCenterObj);
	QCONFIRM_RET_FALSE(pCenterObj->m_pScene);
	pCenterRegion = pCenterObj->m_pRegion;
	QCONFIRM_RET_FALSE(pCenterRegion);

	nLeft       = pCenterRegion->m_nRegionX - m_nBroadcastRegion;
	nRight      = pCenterRegion->m_nRegionX + m_nBroadcastRegion;
	nBottom     = pCenterRegion->m_nRegionY - m_nBroadcastRegion;
	nTop        = pCenterRegion->m_nRegionY + m_nBroadcastRegion;

	for (INT nX = nLeft; nX <= nRight; nX++)
	{
		for (INT nY = nBottom; nY <= nTop; nY++)
		{
			KRegion* pRegion = GetRegion(nX, nY);

			if (!pRegion)
				continue;

			switch (dwType)
			{
			case KD_PLAYER_ID_MASK:
				bRet = pRegion->TraversePlayer(rcHelper);
				break;
			case KD_NPC_ID_MASK:
				bRet = pRegion->TraverseNpc(rcHelper);
				break;
			case KD_DOODAD_ID_MASK:
				bRet = pRegion->TraverseDoodad(rcHelper);
				break;
			default:
				//Q_Error("Invalid Scene Object Type " << dwType);
				break;
			}
		}
	}
	return bRet;
}

KSceneObject* KScene::GetByNickName( LPCSTR pszNickName )
{
	KSceneObject* pObj = NULL;
	QCONFIRM_RET_NULL(pszNickName);

	std::map<std::string, KSceneObject*>::iterator it = m_mapNickNames.find(pszNickName);
	if (it != m_mapNickNames.end())
	{
		pObj = it->second;
	}
	return pObj;
}

BOOL KScene::AddNickName( LPCSTR pszNickName, KSceneObject* pSceneObj )
{
	QCONFIRM_RET_NULL(pszNickName);
	QCONFIRM_RET_NULL(pSceneObj);

	std::pair<std::map<std::string, KSceneObject*>::iterator, bool> cResultPair =
		m_mapNickNames.insert(std::make_pair(pszNickName, pSceneObj));
	return cResultPair.second;
}

BOOL KScene::RemoveNickName(LPCSTR pszNickName)
{
	QCONFIRM_RET_NULL(pszNickName);
	m_mapNickNames.erase(pszNickName);
	return TRUE;
}

INT KScene::GetPlayerCount()
{
	INT nResult = 0;

	for (INT i = 0; i < m_nRegionWidth; ++i)
	{
		for (INT j = 0; j <= m_nRegionHeight; ++j)
		{
			KRegion* pRegion = m_RegionList[i][j];

			if (!pRegion)
				continue;
			nResult += pRegion->m_cPlayerList.GetNodeCount();
		}
	}
	return nResult;
}

INT KScene::GetNpcCount()
{
	INT nResult = 0;

	for (INT i = 0; i < m_nRegionWidth; ++i)
	{
		for (INT j = 0; j <= m_nRegionHeight; ++j)
		{
			KRegion* pRegion = m_RegionList[i][j];

			if (!pRegion)
				continue;
			nResult += pRegion->m_cNpcList.GetNodeCount();
		}
	}
	return nResult;
}

INT KScene::GetDoodadCount()
{
	INT nResult = 0;

	for (INT i = 0; i < m_nRegionWidth; ++i)
	{
		for (INT j = 0; j <= m_nRegionHeight; ++j)
		{
			KRegion* pRegion = m_RegionList[i][j];

			if (!pRegion)
				continue;
			nResult += pRegion->m_cDoodadList.GetNodeCount();
		}
	}
	return nResult;
}

INT KScene::GetRegionCount()
{
	INT nRegionCount = 0;

	for (INT i = 0; i < m_nRegionWidth; ++i)
	{
		for (INT j = 0; j <= m_nRegionHeight; ++j)
		{
			KRegion* pRegion = m_RegionList[i][j];

			if (!pRegion)
				continue;
			nRegionCount ++;
		}
	}
	return nRegionCount;
}
