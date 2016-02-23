/* -------------------------------------------------------------------------
//	文件名		：	KRegion_NS.cpp
//	创建者		：	xiewen
//	创建时间	：	2009-9-23 16:47:54
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kscene.h"
#include "onlinegameworld/ksceneobject.h"
#include "onlinegameworld/kmath.h"
#include "kterraindata.h"
#include "kregion.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegamebase/performancestat.h"

#ifdef GAME_CLIENT
#include "onlinegameworld/kplayermanager_client.h"
#endif // GAME_SERVER

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

struct KRegionCleaner
{
	BOOL operator()(QNode* pNode);
};
BOOL KRegionCleaner::operator()( QNode* pNode )
{
	SceneObjNode* pSceneObjNode = (SceneObjNode*)pNode;
	QCONFIRM_RET_FALSE(pSceneObjNode);

	KSceneObject* pScnObj = pSceneObjNode->m_pSceneObject;
	QCONFIRM_RET_FALSE(pScnObj);

	IKScene* pScene = pScnObj->GetScene();
	assert(pScene);

	QLogPrintf(LOG_DEBUG, "[0x%x][%s] [%d][%s] Removing",
			pScnObj->m_dwId, pScnObj->GetName(), pScene->GetId(), pScene->GetName());

	return pScnObj->OnRemove();
}

struct KRegionActivator
{
	BOOL operator()(QNode* pNode);
};

BOOL KRegionActivator::operator()(QNode* pNode)
{
	SceneObjNode* pSceneObjNode = (SceneObjNode*)pNode;
	QCONFIRM_RET_FALSE(pSceneObjNode);

	KSceneObject* pSceneObj = pSceneObjNode->m_pSceneObject;
	QCONFIRM_RET_FALSE(pSceneObj);

	// 防止重复激活（比如从当前region移动到下一个还没激活过的region，
	// 可能会被那个region再激活一次）
	if (pSceneObj->CheckGameLoop())
		pSceneObj->Activate();

	return TRUE;
}

template<class TList, class TTraverseFunc>
VOID ListTraverse(TList& rtList, TTraverseFunc& rfnTraverse)
{
	QNode*  pNode   = NULL;

	pNode = rtList.GetHead();
	while (pNode != NULL)
	{
		QNode*  pNext = pNode->GetNext();

		// KRegionActivator遍历时可能刚好造成下一个节点被移除
		if (!pNode->IsLinked())
			break;

		rfnTraverse(pNode);

		pNode = pNext;
	}
}


// -------------------------------------------------------------------------

KRegion::KRegion(void)
{
	m_pScene = NULL;
	m_pScriptList = NULL;
}

KRegion::~KRegion(void)
{
}

BOOL KRegion::Init(KScene* pScene, int nRegionX, int nRegionY)
{
	BOOL           bResult       = false;
	KTerrainScene* pTerrainScene = NULL;

	PROCESS_ERROR(pScene);

	LOG_PROCESS_ERROR(pScene);
	LOG_PROCESS_ERROR(nRegionX >= 0);
	LOG_PROCESS_ERROR(nRegionY >= 0);

	LOG_PROCESS_ERROR(m_cNpcList.IsEmpty());
	LOG_PROCESS_ERROR(m_cPlayerList.IsEmpty());
	LOG_PROCESS_ERROR(m_cDoodadList.IsEmpty());

	m_pScene                = pScene;
	m_nRegionX              = nRegionX;
	m_nRegionY              = nRegionY;
	m_nLeftCell             = nRegionX * REGION_GRID_WIDTH;
	m_nBottomCell           = nRegionY * REGION_GRID_HEIGHT;
	m_nLeft                 = m_nLeftCell * CELL_LENGTH;
	m_nBottom               = m_nBottomCell * CELL_LENGTH;
	m_dwActiveFrame          = 0;

	//memset(m_Cells, 0, sizeof(m_Cells));

	pTerrainScene = pScene->m_pTerrainScene;
	m_pCells = pTerrainScene->GetRegionTerrainData(nRegionX, nRegionY);
	m_pScriptList = pTerrainScene->GetRegionScriptData(nRegionX, nRegionY);

	bResult = true;
EXIT0:
	return bResult;
}

void KRegion::UnInit()
{
	m_pScene        = NULL;
	m_pCells        = NULL;
	m_pScriptList = NULL;
}

void KRegion::Activate(void)
{
	// 激活本区域所有npc、player、doodad
	KRegionActivator cRegionActivatorFunc;

	BEGIN_STAT("KOrpgWorld::KRegion.m_cNpcList.Activate()");
	ListTraverse(m_cNpcList, cRegionActivatorFunc);
	END_STAT();
	BEGIN_STAT("KOrpgWorld::KRegion.m_cDoodadList.Activate()");
	ListTraverse(m_cDoodadList, cRegionActivatorFunc);
	END_STAT();
	BEGIN_STAT("KOrpgWorld::KRegion.m_cPlayerList.Activate()");
	ListTraverse(m_cPlayerList, cRegionActivatorFunc);
	END_STAT();
}

BOOL KRegion::LoadTerrain()
{
	BOOL            bResult             = false;
	BOOL            bRetCode            = false;
	IFile*          piFile              = NULL;
	IMemBlock*     	piBuffer            = NULL;
	BYTE*           pbyBuffer           = NULL;
	size_t          uFileSize           = 0;
	size_t          uReadBytes          = 0;
	size_t          uLeftBytes          = 0;
	BYTE*           pbyOffset           = NULL;
	KRegionHeader*  pFileHeader         = NULL;
	char            szFileName[MAX_PATH];

	LOG_PROCESS_ERROR(m_pScene);

	snprintf(szFileName,
		countof(szFileName) - 1,
		"%s/v_%03d/%03d%s", 
		m_pScene->m_szPath, m_nRegionY, m_nRegionX, KD_REGION_FILE_EXT);
	szFileName[sizeof(szFileName) - 1] = '\0';

	piFile = g_OpenFile(szFileName);
	LOG_PROCESS_ERROR(piFile);
	//QLogPrintf(LOG_INFO,"..szFileName=%s,m_nRegionY=%d,m_nRegionX=%d",szFileName,m_nRegionY,m_nRegionX);

	uFileSize = piFile->Size();
	LOG_PROCESS_ERROR(uFileSize > 0);

	piBuffer = QCreateMemBlock((unsigned)uFileSize);
	LOG_PROCESS_ERROR(piBuffer);

	pbyBuffer = (BYTE*)piBuffer->GetData();
	LOG_PROCESS_ERROR(pbyBuffer);

	uReadBytes = piFile->Read(pbyBuffer, (unsigned long)uFileSize);
	LOG_PROCESS_ERROR(uReadBytes == uFileSize);

	SAFE_RELEASE(piFile);

	pbyOffset  = pbyBuffer;
	uLeftBytes = uReadBytes;

	LOG_PROCESS_ERROR(uLeftBytes >= sizeof(KRegionHeader));
	pFileHeader = (KRegionHeader*)pbyOffset;

	pbyOffset  += sizeof(KRegionHeader);
	uLeftBytes -= sizeof(KRegionHeader);

	LOG_PROCESS_ERROR(pFileHeader->nRegionX == m_nRegionX);
	LOG_PROCESS_ERROR(pFileHeader->nRegionY == m_nRegionY);

	if (pFileHeader->nVersion == eTerrainDataVersionCurrent)
	{
		bRetCode = LoadTerrainBufferV2(pbyOffset, uLeftBytes);
		LOG_PROCESS_ERROR(bRetCode);
	}
	//else if (pFileHeader->nVersion == eTerrainDataVersion6)
	//{
	//    bRetCode = LoadTerrainBufferV6(pbyOffset, uLeftBytes);
	//    LOG_PROCESS_ERROR(bRetCode);
	//}
	else
	{
		QLogPrintf(LOG_ERR, "Unsupported map version (%d) at file: (%s)\n", pFileHeader->nVersion, szFileName);
		goto EXIT0;
	}

	bResult = true;
EXIT0:
	SAFE_RELEASE(piBuffer);
	SAFE_RELEASE(piFile);
	return bResult;
}

BOOL KRegion::LoadTerrainBufferV2(const BYTE* pbyData, size_t uDataLen)
{
	BOOL            bResult                 = false;
	BOOL            bRetCode                = false;
	size_t          uLeftBytes              = uDataLen;
	const BYTE*     pbyOffset               = pbyData;
	size_t          uBaseCellInfoSize       = sizeof(KCell::KBaseInfo) + sizeof(WORD);
	KCell*          pAllocNormalCell        = NULL;
	KDynamicCell*   pAllocDynamicCell       = NULL;
	int             nExtNormalCellCount     = 0;
	size_t          uExtNormalCellDataSize  = sizeof(BYTE) * 2 + sizeof(KCell::KBaseInfo) + sizeof(WORD) * 2;
	int             nExtDynamicCellCount    = 0;
	size_t          uExtDynamicCellDataSize = sizeof(BYTE) * 2 + sizeof(KCell::KBaseInfo) + sizeof(WORD) * 2 + sizeof(WORD);

	LOG_PROCESS_ERROR(uLeftBytes >= uBaseCellInfoSize * REGION_GRID_WIDTH * REGION_GRID_HEIGHT);
	uLeftBytes -= uBaseCellInfoSize * REGION_GRID_WIDTH * REGION_GRID_HEIGHT;

	for (int nCellY = 0; nCellY < REGION_GRID_HEIGHT; nCellY++)
	{
		for (int nCellX = 0; nCellX < REGION_GRID_WIDTH; nCellX++)
		{
			KCell*              pCell       = GetLowestObstacle(nCellX, nCellY);
			KCell::KBaseInfo*   pBaseInfo   = (KCell::KBaseInfo*)pbyOffset;

			pCell->m_BaseInfo           = *pBaseInfo;
			pCell->m_BaseInfo.dwDynamic = 0; 
			pCell->m_wLowLayer          = 0;

			pbyOffset += sizeof(KCell::KBaseInfo);

			pCell->m_wHighLayer = *(WORD*)pbyOffset;
			pbyOffset += sizeof(WORD);
		}
	}

	// -------- 读取静态扩展Cell---------------------------------

	LOG_PROCESS_ERROR(uLeftBytes >= sizeof(int));
	uLeftBytes -= sizeof(int);

	nExtNormalCellCount = *(int*)pbyOffset;
	pbyOffset += sizeof(int);

	LOG_PROCESS_ERROR(nExtNormalCellCount >= 0);
	LOG_PROCESS_ERROR(uLeftBytes >= nExtNormalCellCount * uExtNormalCellDataSize);
	uLeftBytes -= nExtNormalCellCount * uExtNormalCellDataSize;

	for (int nIndex = 0; nIndex < nExtNormalCellCount; nIndex++)
	{
		int                 nCellX      = 0;
		int                 nCellY      = 0;
		KCell::KBaseInfo*   pBaseInfo   = NULL;

		//pAllocNormalCell = KMemory::New<KCell>();
		pAllocNormalCell = KCellHelper::NewCell();
		LOG_PROCESS_ERROR(pAllocNormalCell);

		nCellX = *(BYTE*)pbyOffset;
		pbyOffset += sizeof(BYTE);

		nCellY = *(BYTE*)pbyOffset;
		pbyOffset += sizeof(BYTE);

		pBaseInfo = (KCell::KBaseInfo*)pbyOffset;
		pbyOffset += sizeof(KCell::KBaseInfo);

		pAllocNormalCell->m_BaseInfo = *pBaseInfo;
		pAllocNormalCell->m_BaseInfo.dwDynamic = 0;

		pAllocNormalCell->m_wHighLayer = *(WORD*)pbyOffset;
		pbyOffset += sizeof(WORD);

		pAllocNormalCell->m_wLowLayer = *(WORD*)pbyOffset;
		pbyOffset += sizeof(WORD);

		bRetCode = AddObstacle(nCellX, nCellY, pAllocNormalCell);
		LOG_PROCESS_ERROR(bRetCode);

		pAllocNormalCell = NULL;
	}

	// -------- 读取动态扩展Cell---------------------------------

	LOG_PROCESS_ERROR(uLeftBytes >= sizeof(int));
	uLeftBytes -= sizeof(int);

	nExtDynamicCellCount = *(int*)pbyOffset;
	pbyOffset += sizeof(int);

	LOG_PROCESS_ERROR(nExtDynamicCellCount >= 0);
	LOG_PROCESS_ERROR(uLeftBytes >= nExtDynamicCellCount * uExtDynamicCellDataSize);
	uLeftBytes -= nExtDynamicCellCount * uExtDynamicCellDataSize;

	for (int nIndex = 0; nIndex < nExtDynamicCellCount; nIndex++)
	{
		int                 nCellX      = 0;
		int                 nCellY      = 0;
		KCell::KBaseInfo*   pBaseInfo   = NULL;

		//pAllocDynamicCell = KMemory::New<KDynamicCell>();
		pAllocDynamicCell = KCellHelper::NewDynamicCell();
		LOG_PROCESS_ERROR(pAllocDynamicCell);

		nCellX = *(BYTE*)pbyOffset;
		pbyOffset += sizeof(BYTE);

		nCellY = *(BYTE*)pbyOffset;
		pbyOffset += sizeof(BYTE);

		pBaseInfo = (KCell::KBaseInfo*)pbyOffset;
		pbyOffset += sizeof(KCell::KBaseInfo);

		pAllocDynamicCell->m_BaseInfo = *pBaseInfo;
		pAllocDynamicCell->m_BaseInfo.dwDynamic = 1;

		pAllocDynamicCell->m_wHighLayer = *(WORD*)pbyOffset;
		pbyOffset += sizeof(WORD);

		pAllocDynamicCell->m_wLowLayer = *(WORD*)pbyOffset;
		pbyOffset += sizeof(WORD);

		pAllocDynamicCell->m_wGroupIndex = *(WORD*)pbyOffset;
		pbyOffset += sizeof(WORD);

		bRetCode = AddObstacle(nCellX, nCellY, pAllocDynamicCell);
		LOG_PROCESS_ERROR(bRetCode);

		pAllocDynamicCell = NULL;
	}


	if (uLeftBytes >= SCRIPT_DATA_SIZE)
	{
		memcpy(m_pScriptList, pbyOffset, SCRIPT_DATA_SIZE);
		pbyOffset  += SCRIPT_DATA_SIZE;
		uLeftBytes -= SCRIPT_DATA_SIZE;
	}
	LOG_PROCESS_ERROR(uLeftBytes == 0);

	bResult = true;
EXIT0:
	SAFE_DELETE(pAllocNormalCell);
	SAFE_DELETE(pAllocDynamicCell);
	return bResult;
}

void KRegion::SetActive()
{ 
	m_dwActiveFrame = g_cOrpgWorld.m_dwLogicFrames; 
}

BOOL KRegion::IsActive()
{
#ifdef GAME_SERVER
	return g_cOrpgWorld.m_dwLogicFrames < m_dwActiveFrame + GAME_FPS * 3;
#else
	return ((UINT64)g_cOrpgWorld.m_dwLogicFrames < (UINT64)m_dwActiveFrame + GAME_FPS / 2);
#endif
}

BOOL KRegion::AddObstacle(int nXCell, int nYCell, KCell* pCell)
{
	BOOL bFound = false;
	KCell* pInsertPos = NULL;

	LOG_PROCESS_ERROR(nXCell >= 0);
	LOG_PROCESS_ERROR(nXCell < REGION_GRID_WIDTH);
	LOG_PROCESS_ERROR(nYCell >= 0);
	LOG_PROCESS_ERROR(nYCell < REGION_GRID_HEIGHT);
	LOG_PROCESS_ERROR(pCell);

	pInsertPos = GetLowestObstacle(nXCell, nYCell);

	//查找插入的位置
	while (!bFound)
	{
		LOG_PROCESS_ERROR(pCell->m_wLowLayer >= pInsertPos->m_wHighLayer);

		if (pInsertPos->m_pNext)
		{
			if (pCell->m_wLowLayer >= pInsertPos->m_wHighLayer &&
				pCell->m_wHighLayer <= pInsertPos->m_pNext->m_wLowLayer)
			{
				bFound = true;
			}
			else
			{
				pInsertPos = pInsertPos->m_pNext;
			}
		}
		else
		{			
			bFound = true;
		}
	}

	//插入链表
	pCell->m_pNext = pInsertPos->m_pNext;
	pInsertPos->m_pNext = pCell;

	return true;

EXIT0:

	return false;
}

BOOL KRegion::AddObstacle(int nXCell, int nYCell, const KCell& crCellInfo, int nDynamicObstacleGroupIndex)
{
	BOOL   bResult              = false;
	KCell* pCellNode            = NULL;
	KCell* pLastLowerCell       = NULL;
	KCell* pPreNode             = NULL;
	KCell* pNewCell             = NULL;
	DWORD  dwDynamicCellFlag    = 0;

	LOG_PROCESS_ERROR(nXCell >= 0);
	LOG_PROCESS_ERROR(nXCell < REGION_GRID_WIDTH);
	LOG_PROCESS_ERROR(nYCell >= 0);
	LOG_PROCESS_ERROR(nYCell < REGION_GRID_HEIGHT);

	pCellNode = GetLowestObstacle(nXCell, nYCell);

	// 找到最后一个低面<=新Cell的节点,将新Cell插入到它的后面
	if (crCellInfo.m_BaseInfo.dwFaceUp)
	{
		// 对于朝上的Cell,尽量放在上面
		while (pCellNode)
		{
			if (pCellNode->m_wLowLayer > crCellInfo.m_wLowLayer)
				break;
			pLastLowerCell = pCellNode;
			pCellNode = pCellNode->m_pNext;
		}
	}
	else
	{
		// 对于朝下的Cell,尽量放在下面
		while (pCellNode)
		{
			if (pCellNode->m_wLowLayer >= crCellInfo.m_wLowLayer)
				break;
			pLastLowerCell = pCellNode;
			pCellNode = pCellNode->m_pNext;
		}
	}

	// 因为至少有个[0, 0]的基本Cell,所以不可能为空
	LOG_PROCESS_ERROR(pLastLowerCell);

	if (nDynamicObstacleGroupIndex > INVALID_OBSTACLE_GROUP && nDynamicObstacleGroupIndex < MAX_OBSTACLE_GROUP)
	{
		//KDynamicCell* pDynamicCell = KMemory::New<KDynamicCell>();
		KDynamicCell* pDynamicCell = new KDynamicCell;
		LOG_PROCESS_ERROR(pDynamicCell);

		pDynamicCell->m_wGroupIndex = (WORD)nDynamicObstacleGroupIndex;
		dwDynamicCellFlag = 1;
		pNewCell = pDynamicCell;
	}
	else
	{
		//pNewCell = KMemory::New<KCell>();
		pNewCell = new KCell;
		LOG_PROCESS_ERROR(pNewCell);
	}

	pNewCell->m_wLowLayer   = crCellInfo.m_wLowLayer;
	pNewCell->m_wHighLayer  = crCellInfo.m_wHighLayer;
	pNewCell->m_BaseInfo    = crCellInfo.m_BaseInfo;

	pNewCell->m_BaseInfo.dwDynamic = dwDynamicCellFlag;

	pNewCell->m_pNext       = pLastLowerCell->m_pNext;
	pLastLowerCell->m_pNext = pNewCell;

	bResult = true;
EXIT0:
	return bResult;
}


KCell* KRegion::RemoveObstacle(int nXCell, int nYCell, int nZ)
{
	BOOL    bFound      = false;
	KCell*  pCell       = NULL;
	KCell*  pRemovePos  = NULL;

	LOG_PROCESS_ERROR(nXCell >= 0);
	LOG_PROCESS_ERROR(nXCell < REGION_GRID_WIDTH);
	LOG_PROCESS_ERROR(nYCell >= 0);
	LOG_PROCESS_ERROR(nYCell < REGION_GRID_HEIGHT);

	pRemovePos = GetLowestObstacle(nXCell, nYCell);

	while (!bFound)
	{
		PROCESS_ERROR(pRemovePos->m_pNext);
		if ((nZ / POINT_PER_ALTITUDE) <= pRemovePos->m_pNext->m_wHighLayer)
		{
			PROCESS_ERROR((nZ / POINT_PER_ALTITUDE) >= pRemovePos->m_pNext->m_wLowLayer);
			bFound = true;
		}
		else
		{
			pRemovePos = pRemovePos->m_pNext;
		}
	}

	PROCESS_ERROR(pRemovePos->m_pNext);

	pCell = pRemovePos->m_pNext;
	pRemovePos->m_pNext = pRemovePos->m_pNext->m_pNext;

	return pCell;

EXIT0:

	return NULL;
}


BOOL KRegion::GetBaseCellInfo(int nXCell, int nYCell, KCell* pCell)
{
	BOOL    bResult     = false;
	KCell*  pBaseCell   = NULL;

	LOG_PROCESS_ERROR(nXCell >= 0 && nXCell < REGION_GRID_WIDTH);
	LOG_PROCESS_ERROR(nYCell >= 0 && nYCell < REGION_GRID_HEIGHT);

	pBaseCell = GetLowestObstacle(nXCell, nYCell);
	LOG_PROCESS_ERROR(pBaseCell);

	*pCell = *pBaseCell;

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KRegion::SetBaseCellInfo(int nXCell, int nYCell, KCell& rCell)
{
	BOOL    bResult = false;
	KCell*  pCell   = NULL;

	LOG_PROCESS_ERROR(nXCell >= 0 && nXCell < REGION_GRID_WIDTH);
	LOG_PROCESS_ERROR(nYCell >= 0 && nYCell < REGION_GRID_HEIGHT);

	pCell = GetLowestObstacle(nXCell, nYCell);

	pCell->m_wLowLayer  = rCell.m_wLowLayer;
	pCell->m_wHighLayer = rCell.m_wHighLayer;
	pCell->m_BaseInfo   = rCell.m_BaseInfo;

	bResult = true;
EXIT0:
	return bResult;
}


BOOL KRegion::DeleteAllObject(void)
{
	BOOL bRetCode = FALSE;

	KRegionCleaner cCleaner;

	// 清空Player/Npc/Doodad列表
	ListTraverse(m_cPlayerList, cCleaner);
	ListTraverse(m_cDoodadList, cCleaner);
	ListTraverse(m_cNpcList, cCleaner);

	bRetCode = TRUE;
//EXIT0:
	return bRetCode;
}

BOOL KRegion::DeleteAllNpc(void)
{
	BOOL bRetCode = FALSE;

	KRegionCleaner cCleaner;

	// 清空Player/Npc/Doodad列表
	ListTraverse(m_cNpcList, cCleaner);

	bRetCode = TRUE;
	//EXIT0:
	return bRetCode;
}

BOOL KRegion::DeleteAllDoodad(void)
{
	BOOL bRetCode = FALSE;

	KRegionCleaner cCleaner;

	// 清空Player/Npc/Doodad列表
	ListTraverse(m_cDoodadList, cCleaner);

	bRetCode = TRUE;
	//EXIT0:
	return bRetCode;
}

BOOL KRegion::DeleteAllCell(void)
{
	BOOL bResult = false;

	for (int nXCell = 0; nXCell < REGION_GRID_WIDTH; nXCell++)
	{
		for (int nYCell = 0; nYCell < REGION_GRID_HEIGHT; nYCell++)
		{
			KCell* pBaseCell = GetLowestObstacle(nXCell, nYCell);
			KCell* pCell     = NULL;

			LOG_PROCESS_ERROR(pBaseCell);

			pCell = pBaseCell->m_pNext;

			while (pCell)
			{
				KCell* pNext = pCell->m_pNext;

				SAFE_DELETE(pCell);
				//if (pCell->m_BaseInfo.dwDynamic)
				//{
					//KMemory::Delete((KDynamicCell*)pCell);
				//}
				//else
				//{
					//KMemory::Delete(pCell);
				//}

				pCell = pNext;
			}

			pBaseCell->m_pNext = NULL;
		}
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KRegion::AddSceneObject(QList& rcList, KSceneObject* pSceneObj)
{
	BOOL    bResult     = FALSE;
	BOOL    bRetCode    = FALSE;
	KCell*  pCell       = NULL;
	INT     nRelativeZ  = 0;

	LOG_PROCESS_ERROR(pSceneObj);

	pCell = GetProximalCellForObj(
		pSceneObj->m_nXCell, pSceneObj->m_nYCell, pSceneObj->m_nZ, 
		pSceneObj->m_nHeight, FALSE, TRUE, 
		&nRelativeZ
		);
	LOG_PROCESS_ERROR(pCell);

	rcList.AddTail(&pSceneObj->m_cRegionNode);
	pSceneObj->m_pRegion = this;

	pSceneObj->SetCell(pCell);

	pSceneObj->m_nZ = pCell->m_wHighLayer * POINT_PER_ALTITUDE + nRelativeZ;
	pSceneObj->m_nDestX = pSceneObj->m_nX;
	pSceneObj->m_nDestY = pSceneObj->m_nY;
	pSceneObj->m_nDestZ = pSceneObj->m_nZ;
	pSceneObj->m_dwBroadcastFrame = 0;

#ifdef GAME_SERVER
	SetActive();
#else
	m_dwActiveFrame = -1; // this trick alleviates de-sync in rare occasions[9/19/2011 simon]
#endif

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRegion::RemoveSceneObject(KSceneObject* pSceneObj)
{
	QCONFIRM_RET_FALSE(pSceneObj->m_pCell);

	pSceneObj->SetCell(NULL);

	PCSTR pszNickName = pSceneObj->GetNickName();
	if (pszNickName && *pszNickName)
	{
		m_pScene->RemoveNickName(pszNickName);
		pSceneObj->m_strNickName.Clear();
	}

	if (pSceneObj->m_pRegion)
		pSceneObj->m_cRegionNode.Remove();

	pSceneObj->m_pRegion = NULL;
	pSceneObj->m_pLastRegion = NULL;
	pSceneObj->m_pScene = NULL;

	return TRUE;
}

KCell* KRegion::GetLowestObstacle(int nXCell, int nYCell)
{
	QCONFIRM_RET_NULL(nXCell >= 0);
	QCONFIRM_RET_NULL(nXCell < REGION_GRID_WIDTH);
	QCONFIRM_RET_NULL(nYCell >= 0);
	QCONFIRM_RET_NULL(nYCell < REGION_GRID_HEIGHT);

	return m_pCells + REGION_GRID_WIDTH * nYCell + nXCell;
}

KCell* KRegion::GetProximalCellForObj(
									  int nXCell, int nYCell, int nFootPosZ, int nHeight, 
									  BOOL bIgnoreDynamicSwitch,
									  BOOL bIgnoreBlock, 
									  int* pnRelativeZ
									  )
{
	KCell*  pResult         = NULL;
	KCell*  pPossibleCell   = NULL;
	KCell*  pCellNode       = NULL;
	int     nRelativeZ      = 0;
	int     nAbsolutePosZ   = 0;

	LOG_PROCESS_ERROR(pnRelativeZ);
	LOG_PROCESS_ERROR(nXCell >= 0);
	LOG_PROCESS_ERROR(nXCell < REGION_GRID_WIDTH);
	LOG_PROCESS_ERROR(nYCell >= 0);
	LOG_PROCESS_ERROR(nYCell < REGION_GRID_HEIGHT);

	pCellNode = GetLowestObstacle(nXCell, nYCell);
	while (pCellNode)
	{
		int    nTempPosZ     = 0;
		BOOL   bStandable    = false;

		if (pCellNode->m_BaseInfo.dwCellType == ctWater)
		{
			if (bIgnoreBlock || !pCellNode->m_BaseInfo.dwBlockCharacter)
			{
				KCell* pNextRealCell = NULL;
				KCell* pIter         = GetNextCell(pCellNode, bIgnoreDynamicSwitch);

				// 找到下一个非水的Cell,看与这个Cell之间的空间够不够
				while (pIter)
				{
					if (pIter->m_BaseInfo.dwCellType != ctWater)
					{
						pNextRealCell = pIter;
						break;
					}
					pIter = GetNextCell(pIter, bIgnoreDynamicSwitch);
				}

				if (pNextRealCell)
				{
					if ((pNextRealCell->m_wLowLayer - pCellNode->m_wLowLayer) * POINT_PER_ALTITUDE >= nHeight)
					{
						bStandable = true;

						if (nFootPosZ >= pNextRealCell->m_wLowLayer * POINT_PER_ALTITUDE - nHeight)
						{
							nTempPosZ = pNextRealCell->m_wLowLayer * POINT_PER_ALTITUDE - nHeight;
						}
						else if (nFootPosZ < pCellNode->m_wLowLayer * POINT_PER_ALTITUDE)
						{
							nTempPosZ = pCellNode->m_wLowLayer * POINT_PER_ALTITUDE;
						}
						else
						{
							nTempPosZ = nFootPosZ;
						}
					}
				}
				else
				{
					bStandable = true;

					if (nFootPosZ > pCellNode->m_wLowLayer * POINT_PER_ALTITUDE)
					{
						nTempPosZ = nFootPosZ;
					}
					else
					{
						nTempPosZ = pCellNode->m_wLowLayer * POINT_PER_ALTITUDE;
					}                        
				}
			}
		}
		else
		{
			if (bIgnoreBlock || !pCellNode->m_BaseInfo.dwBlockCharacter)
			{
				KCell* pNextRealCell = NULL;
				KCell* pIter         = GetNextCell(pCellNode, bIgnoreDynamicSwitch);

				// 找到下一个非水的Cell,看与这个Cell之间的空间够不够
				while (pIter)
				{
					if (pIter->m_BaseInfo.dwCellType != ctWater)
					{
						pNextRealCell = pIter;
						break;
					}
					pIter = GetNextCell(pIter, bIgnoreDynamicSwitch);
				}

				if (pNextRealCell)
				{
					if ((pNextRealCell->m_wLowLayer - pCellNode->m_wHighLayer) * POINT_PER_ALTITUDE >= nHeight)
					{
						bStandable = true;

						if (nFootPosZ >= pNextRealCell->m_wLowLayer * POINT_PER_ALTITUDE - nHeight)
						{
							nTempPosZ = pNextRealCell->m_wLowLayer * POINT_PER_ALTITUDE - nHeight;
						}
						else if (nFootPosZ < pCellNode->m_wHighLayer * POINT_PER_ALTITUDE)
						{
							nTempPosZ = pCellNode->m_wHighLayer * POINT_PER_ALTITUDE;
						}
						else
						{
							nTempPosZ = nFootPosZ;
						}
					}
				}
				else
				{
					bStandable = true;

					if (nFootPosZ > pCellNode->m_wHighLayer * POINT_PER_ALTITUDE)
					{
						nTempPosZ = nFootPosZ;
					}
					else
					{
						nTempPosZ = pCellNode->m_wHighLayer * POINT_PER_ALTITUDE;
					}                        
				}
			}
		}

		if (bStandable)
		{
			if (pPossibleCell)
			{
				// 如果之前已经找到Cell了,则比较看相比是否更接近nFootZ
				if (abs(nTempPosZ - nFootPosZ) <= abs(nAbsolutePosZ - nFootPosZ))
				{
					pPossibleCell = pCellNode;
					nAbsolutePosZ = nTempPosZ;
				}
				else
				{
					// 由于Cell没有交错的,所以,如果前面已经找到其他Cell,而这个Cell离nFootZ更远,那么后面其他的Cell也就没必要找了
					break;
				}
			}
			else
			{
				pPossibleCell = pCellNode;
				nAbsolutePosZ = nTempPosZ;
			}
		}

		pCellNode = GetNextCell(pCellNode, bIgnoreDynamicSwitch);
	}


	PROCESS_ERROR(pPossibleCell);

	nRelativeZ      = nAbsolutePosZ - pPossibleCell->m_wHighLayer * POINT_PER_ALTITUDE;
	//printf("<PossibleCell>%d ~ %d, %f. ", pPossibleCell->m_wLowLayer, pPossibleCell->m_wHighLayer, pPossibleCell->m_wHighLayer * POINT_PER_ALTITUDE);

	pResult         = pPossibleCell;
	*pnRelativeZ    = nRelativeZ;
EXIT0:
	return pResult;
}

KCell* KRegion::GetNextCell(KCell* pCell, BOOL bIgnoreDynamicSwitch)
{
	KCell*  pCellNode     = NULL;

	pCellNode = pCell->m_pNext;

	while (pCellNode && pCellNode->m_BaseInfo.dwDynamic)
	{
		// 忽略动态障碍时,动态障碍一律视为disable
		if (!bIgnoreDynamicSwitch)
		{
			KDynamicCell* pDynamicCell   = (KDynamicCell*)pCellNode;
			BOOL bDynamicSwitch = m_pScene->GetDynamicObstacleState(pDynamicCell->m_wGroupIndex);
			if (bDynamicSwitch)
			{
				return pCellNode;
			}
		}

		pCellNode = pCellNode->m_pNext;
	}

	return pCellNode;
}

KCell* KRegion::GetPreCell(int nCellX, int nCellY, KCell* pCell)
{
	KCell* pCellNode = GetLowestObstacle(nCellX, nCellY);

	while (pCellNode && pCellNode->m_pNext != pCell)
	{
		pCellNode = pCellNode->m_pNext;
	}

	if (pCellNode && pCellNode->m_BaseInfo.dwDynamic)
	{
		KDynamicCell* pDynamicCell = (KDynamicCell*)pCellNode;

		if (!m_pScene->GetDynamicObstacleState(pDynamicCell->m_wGroupIndex))
		{
			return GetPreCell(nCellX, nCellY, pCellNode);
		}
	}

	return pCellNode;
}

// 获取障碍
KCell* KRegion::GetObstacle(int nXCell, int nYCell, int nZ)
{
	KCell* pCell = GetLowestObstacle(nXCell, nYCell);

	LOG_PROCESS_ERROR(nXCell >= 0);
	LOG_PROCESS_ERROR(nXCell < REGION_GRID_WIDTH);
	LOG_PROCESS_ERROR(nYCell >= 0);
	LOG_PROCESS_ERROR(nYCell < REGION_GRID_HEIGHT);

	while (pCell)
	{
		if (nZ >= pCell->m_wLowLayer * POINT_PER_ALTITUDE && nZ <= pCell->m_wHighLayer * POINT_PER_ALTITUDE)
		{
			break;
		}
		pCell = pCell->m_pNext;
	}
EXIT0:
	return pCell;
}

KCell* KRegion::GetAdaptedCellForObj(
									 int nXCell, int nYCell, int nFootPosZ, int nClimb, 
									 int nHeight, BOOL bIgnoreDynamicSwitch,
									 int* pnRelativeZ, int *pnObstacleDirection
									 )
{
	KCell*  pResult         = NULL;
	KCell*  pPossibleCell   = NULL;
	int     nFootPosMin     = INT_MAX;
	int     nRelativeZ      = 0;

	LOG_PROCESS_ERROR(nXCell >= 0);
	LOG_PROCESS_ERROR(nXCell < REGION_GRID_WIDTH);
	LOG_PROCESS_ERROR(nYCell >= 0);
	LOG_PROCESS_ERROR(nYCell < REGION_GRID_HEIGHT);
	LOG_PROCESS_ERROR(pnRelativeZ);

	for (
		KCell* pCellNode = GetLowestObstacle(nXCell, nYCell); 
		pCellNode != NULL;
	pCellNode = GetNextCell(pCellNode, bIgnoreDynamicSwitch)
		)
	{
		if (pCellNode->m_BaseInfo.dwCellType == ctWater)
		{
			if (pCellNode->m_wLowLayer * POINT_PER_ALTITUDE > nFootPosZ + nClimb)
				break;

			if (pCellNode->m_BaseInfo.dwBlockCharacter)
				continue;

			KCell* pNextRealCell = NULL;
			KCell* pIter         = GetNextCell(pCellNode, bIgnoreDynamicSwitch);

			// 找到下一个非水的Cell,看与这个Cell之间的空间够不够
			while (pIter)
			{
				if (pIter->m_BaseInfo.dwCellType != ctWater)
				{
					pNextRealCell = pIter;
					break;
				}
				pIter = GetNextCell(pIter, bIgnoreDynamicSwitch);
			}

			if (pNextRealCell)
			{
				if (pNextRealCell->m_wLowLayer * POINT_PER_ALTITUDE < nFootPosZ + nHeight / 2)
					continue;

				if ((pNextRealCell->m_wLowLayer - pCellNode->m_wLowLayer) * POINT_PER_ALTITUDE < nHeight)
					continue;
			}

			pPossibleCell = pCellNode;

			if (pCellNode->m_wLowLayer * POINT_PER_ALTITUDE < nFootPosZ)
				nRelativeZ = nFootPosZ - pCellNode->m_wHighLayer * POINT_PER_ALTITUDE;
			else
				nRelativeZ = (pCellNode->m_wLowLayer - pCellNode->m_wHighLayer) * POINT_PER_ALTITUDE;
		}
		else
		{
			if (pCellNode->m_wHighLayer * POINT_PER_ALTITUDE > nFootPosZ + nClimb)
			{
				if (pCellNode->m_wLowLayer * POINT_PER_ALTITUDE < nFootPosZ + nHeight && pnObstacleDirection)
					*pnObstacleDirection = pCellNode->GetBarrierDirection();
				break;
			}

			if (pCellNode->m_BaseInfo.dwBlockCharacter)
				continue;

			KCell* pNextRealCell = NULL;
			KCell* pIter         = GetNextCell(pCellNode, bIgnoreDynamicSwitch);

			// 找到下一个非水的Cell,看与这个Cell之间的空间够不够
			while (pIter)
			{
				if (pIter->m_BaseInfo.dwCellType != ctWater)
				{
					pNextRealCell = pIter;
					break;
				}
				pIter = GetNextCell(pIter, bIgnoreDynamicSwitch);
			}

			if (pNextRealCell)
			{
				if (pNextRealCell->m_wLowLayer * POINT_PER_ALTITUDE < nFootPosZ + nHeight / 2)  // Can't go down
					continue;

				if ((pNextRealCell->m_wLowLayer - pCellNode->m_wHighLayer) * POINT_PER_ALTITUDE < nHeight)   // Not enough space
					continue;
			}

			if (pCellNode->m_wHighLayer * POINT_PER_ALTITUDE >= nFootPosMin)    // Go the lowest possible cell
				break;

			nFootPosMin = pCellNode->m_wHighLayer * POINT_PER_ALTITUDE;
			pPossibleCell = pCellNode;

			if (pCellNode->m_wHighLayer * POINT_PER_ALTITUDE < nFootPosZ)
				nRelativeZ  = nFootPosZ - pCellNode->m_wHighLayer * POINT_PER_ALTITUDE;
			else
				nRelativeZ = 0;
		}
	}

	PROCESS_ERROR(pPossibleCell);

	pResult         = pPossibleCell;
	*pnRelativeZ    = nRelativeZ;
EXIT0:
	return pResult;
}

BOOL KRegion::CheckObstacle(int nXCell, int nYCell, int nLayer)
{
	BOOL    bFound = false;
	KCell*  pCell  = NULL;

	PROCESS_SUCCESS(nXCell < 0);
	PROCESS_SUCCESS(nXCell >= REGION_GRID_WIDTH);
	PROCESS_SUCCESS(nYCell < 0);
	PROCESS_SUCCESS(nYCell >= REGION_GRID_HEIGHT);

	pCell = GetLowestObstacle(nXCell, nYCell);
	while (pCell)
	{
		if (nLayer >= pCell->m_wLowLayer)
		{
			if (nLayer < pCell->m_wHighLayer)
				return true;
			else
				pCell = pCell->m_pNext;
		}
		else
		{
			return false;
		}
	}

//EXIT0:
	return false;

EXIT1:
	// 非法区域全算障碍
	return true;
}

BOOL KRegion::AddPlayer( KSceneObject* pSceneObj )
{
	return AddSceneObject(m_cPlayerList, pSceneObj);
}

BOOL KRegion::RemovePlayer( KSceneObject* pSceneObj )
{
	return RemoveSceneObject(pSceneObj);
}

BOOL KRegion::AddNpc( KSceneObject* pSceneObj )
{
	return AddSceneObject(m_cNpcList, pSceneObj);
}

BOOL KRegion::RemoveNpc( KSceneObject* pSceneObj )
{
	return RemoveSceneObject(pSceneObj);
}

BOOL KRegion::AddDoodad( KSceneObject* pSceneObj )
{
	return AddSceneObject(m_cDoodadList, pSceneObj);
}

BOOL KRegion::RemoveDoodad( KSceneObject* pSceneObj )
{
	return RemoveSceneObject(pSceneObj);
}

BOOL KRegion::TraverseNpc( IKTraverseHelper& rcHelper )
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	SceneObjNode* pNode = (SceneObjNode*)m_cNpcList.GetHead();
	while (pNode != NULL)
	{
		SceneObjNode* pNextNode = (SceneObjNode*)pNode->GetNext();;

		bRetCode = rcHelper.OnTraverse(pNode->m_pSceneObject);
		PROCESS_ERROR(bRetCode);

		pNode = pNextNode;
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KRegion::TraversePlayer( IKTraverseHelper& rcHelper )
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	SceneObjNode* pNode = (SceneObjNode*)m_cPlayerList.GetHead();
	while (pNode != NULL)
	{
		SceneObjNode* pNextNode = (SceneObjNode*)pNode->GetNext();;

		bRetCode = rcHelper.OnTraverse(pNode->m_pSceneObject);
		LOG_PROCESS_ERROR(bRetCode);

		pNode = pNextNode;
	}

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KRegion::TraverseCharacter(IKTraverseHelper& rcHelper)
{
    LOG_PROCESS_ERROR(TraversePlayer(rcHelper));
	LOG_PROCESS_ERROR(TraverseNpc(rcHelper));

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL KRegion::TraverseDoodad( IKTraverseHelper& rcHelper )
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	SceneObjNode* pNode = (SceneObjNode*)m_cDoodadList.GetHead();
	while (pNode != NULL)
	{
		SceneObjNode* pNextNode = (SceneObjNode*)pNode->GetNext();;

		bRetCode = rcHelper.OnTraverse(pNode->m_pSceneObject);
		PROCESS_ERROR(bRetCode);

		pNode = pNextNode;
	}

	bResult = true;
EXIT0:
	return bResult;
}
