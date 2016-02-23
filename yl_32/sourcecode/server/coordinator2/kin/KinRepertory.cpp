#include "stdafx.h"
#include "onlinegamemodule/item2/itemdef.h"
#include "GameCenter.h"
#include "KinRepertory.h"

#define KTONG_REPERTORY_GRID_LOCK_TIME 600

KTongRepertory::KTongRepertory()
{
    m_nUpdateCounter = 0;
    memset(m_Page, 0, sizeof(m_Page));
}

BOOL KTongRepertory::Init()
{
    return true;
}

void KTongRepertory::UnInit()
{
    for (int i = 0 ; i < TONG_REPERTORY_PAGE_NUM; i++)
    {
        for (int j = 0 ; j < TONG_REPERTORY_PAGE_CAPACITY; j++)
            SAFE_DELETE(m_Page[i].Grids[j].pbyItemData);
    }
}

KTONG_REPERTORY_GRID* KTongRepertory::GetRepertoryGrid(int nPageIndex, int nPos)
{
    KTONG_REPERTORY_GRID*  pResult = NULL;

    LOG_PROCESS_ERROR(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    LOG_PROCESS_ERROR(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);

    pResult = &m_Page[nPageIndex].Grids[nPos];
EXIT0:
    return pResult;
}

// ----------------------获得物品相关----------------------------
// 获得道具数据，并对物品栏加锁
KTONG_REPERTORY_GRID* KTongRepertory::TakeItem(int nPageIndex, int nPos, int nClientUpdateCounter, DWORD dwTakePlayerId, time_t nTakeTime)
{
    KTONG_REPERTORY_GRID*   pResult = NULL;
    KTONG_REPERTORY_GRID*   pGrid   = NULL;

    assert(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    assert(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);

    LOG_PROCESS_ERROR(m_Page[nPageIndex].bEnable);
    
    pGrid = &m_Page[nPageIndex].Grids[nPos];
    PROCESS_ERROR(pGrid->pbyItemData);
    PROCESS_ERROR(pGrid->nLastUpdateCounter <= nClientUpdateCounter);
    
    PROCESS_ERROR(pGrid->nLockTimer < g_pGameCenter->m_nTimeNow);
    LOG_CHECK_ERROR(pGrid->nLockTimer == 0);
    pGrid->nLockTimer = g_pGameCenter->m_nTimeNow + KTONG_REPERTORY_GRID_LOCK_TIME;

    pResult = pGrid;
EXIT0:
    return pResult;
}

// 删除物品栏中的道具
BOOL KTongRepertory::FreeItem(int nPageIndex, int nPos)
{
    BOOL                    bResult = false;
    KTONG_REPERTORY_GRID*   pGrid   = NULL;

    assert(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    assert(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);

    pGrid = &m_Page[nPageIndex].Grids[nPos];

    LOG_PROCESS_ERROR(pGrid->pbyItemData);

    LOG_PROCESS_ERROR(pGrid->nLockTimer != 0);
    LOG_CHECK_ERROR(pGrid->nLockTimer >= g_pGameCenter->m_nTimeNow);
    pGrid->nLockTimer = 0;

    SAFE_DELETE(pGrid->pbyItemData);
	pGrid->byItemDataLen = 0;
	pGrid->pbyItemData = NULL;
	pGrid->dwPutPlayerId = KD_BAD_ID;
	pGrid->nPutTime = 0;
	pGrid->dwTakePlayerId = KD_BAD_ID;
	pGrid->nTakeTime = 0;
	pGrid->nExpireTime = 0;
	pGrid->bRemoveOnExpire = false;
    
    pGrid->nLastUpdateCounter = ++m_nUpdateCounter;

    bResult = true;
EXIT0:
    return bResult;
}

// 失败解锁物品栏中的道具
BOOL KTongRepertory::RecoverItem(int nPageIndex, int nPos)
{
    BOOL                    bResult = false;
    KTONG_REPERTORY_GRID*   pGrid   = NULL;

    assert(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    assert(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);

    pGrid = &m_Page[nPageIndex].Grids[nPos];
    LOG_PROCESS_ERROR(pGrid->pbyItemData);

    LOG_PROCESS_ERROR(pGrid->nLockTimer != 0);
    LOG_CHECK_ERROR(pGrid->nLockTimer >= g_pGameCenter->m_nTimeNow);
    pGrid->nLockTimer = 0;

    bResult = true;
EXIT0:
    return bResult;
}

// ----------------------放置物品相关------------------------------
// 尝试获得空的物品栏位，并加锁
BOOL KTongRepertory::ApplyEmptyGrid(int nPageIndex, int nPos)
{
    BOOL                    bResult = false;
    KTONG_REPERTORY_GRID*   pGrid   = NULL;

    assert(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    assert(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);
    
    LOG_PROCESS_ERROR(m_Page[nPageIndex].bEnable);

    pGrid = &m_Page[nPageIndex].Grids[nPos];
    PROCESS_ERROR(pGrid->pbyItemData == NULL);

    PROCESS_ERROR(pGrid->nLockTimer < g_pGameCenter->m_nTimeNow);
    LOG_CHECK_ERROR(pGrid->nLockTimer == 0);
    pGrid->nLockTimer = g_pGameCenter->m_nTimeNow + KTONG_REPERTORY_GRID_LOCK_TIME;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTongRepertory::UnlockGrid(int nPageIndex, int nPos)
{
    BOOL                    bResult = false;
    KTONG_REPERTORY_GRID*   pGrid   = NULL;

    assert(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    assert(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);
    
    pGrid = &m_Page[nPageIndex].Grids[nPos];
    LOG_PROCESS_ERROR(pGrid->nLockTimer != 0);
    LOG_CHECK_ERROR(pGrid->nLockTimer >= g_pGameCenter->m_nTimeNow);
    pGrid->nLockTimer = 0;

    bResult = true;
EXIT0:
    return bResult;
}


BOOL KTongRepertory::LockGrid( int nPageIndex, int nPos )
{
	BOOL                    bResult = false;
	KTONG_REPERTORY_GRID*   pGrid   = NULL;

	assert(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
	assert(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);

	pGrid = &m_Page[nPageIndex].Grids[nPos];
	pGrid->nLockTimer = g_pGameCenter->m_nTimeNow + KTONG_REPERTORY_GRID_LOCK_TIME;

	bResult = true;
EXIT0:
	return bResult;
}


// 在已被加锁的物品栏上放置道具
BOOL KTongRepertory::PutItem(int nPageIndex, int nPos, BYTE* pbyData, size_t uDataLen, DWORD dwPutPlayerId, time_t nPutTime)
{
    BOOL                    bResult     = false;
    KTONG_REPERTORY_GRID*   pGrid       = NULL;
    BYTE*                   pbyItemData = NULL;

    LOG_PROCESS_ERROR(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    LOG_PROCESS_ERROR(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);
    LOG_PROCESS_ERROR(m_Page[nPageIndex].bEnable);

    pGrid = &m_Page[nPageIndex].Grids[nPos];
    LOG_PROCESS_ERROR(pGrid->pbyItemData == NULL);

    LOG_PROCESS_ERROR(pGrid->nLockTimer >= g_pGameCenter->m_nTimeNow);
    
    LOG_PROCESS_ERROR(uDataLen <= UCHAR_MAX);

    pbyItemData = new BYTE[uDataLen]; // 放掉的时候是SAFE_DELETE中的delete
    LOG_PROCESS_ERROR(pbyItemData);

    pGrid->byItemDataLen = (BYTE)uDataLen;

    memcpy(pbyItemData, pbyData, uDataLen);
    
    pGrid->pbyItemData = pbyItemData;
    pbyItemData = NULL;

    pGrid->nLockTimer = 0;
    pGrid->nLastUpdateCounter = ++m_nUpdateCounter;

	LOG_PROCESS_ERROR(dwPutPlayerId != KD_BAD_ID);
	pGrid->dwPutPlayerId = dwPutPlayerId;
	LOG_PROCESS_ERROR(nPutTime > 0);
	pGrid->nPutTime = nPutTime;
    
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTongRepertory::FindEmptyGrid(int nPageIndex, int* pnRetEmptyPos)
{
    BOOL                    bResult         = false;
    BOOL                    bHaveEmptyGrid  = false;
    int                     nEmptyPos       = -1;
    KTONG_REPERTORY_PAGE*   pPage           = NULL;
    KTONG_REPERTORY_GRID*   pGrid           = NULL;

    assert(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    pPage = &m_Page[nPageIndex];

    LOG_PROCESS_ERROR(pPage->bEnable);

    for (int i = 0 ; i < TONG_REPERTORY_PAGE_CAPACITY; i++)
    {
        pGrid = &pPage->Grids[i];
        if (pGrid->nLockTimer >= g_pGameCenter->m_nTimeNow)
            continue;
        if (pGrid->pbyItemData != NULL)
            continue;

        LOG_CHECK_ERROR(pGrid->nLockTimer == 0);
        nEmptyPos = i;
        bHaveEmptyGrid = true;
        break;
    }

    if (bHaveEmptyGrid)
    {
        *pnRetEmptyPos = nEmptyPos;
        bResult = true;
    }

EXIT0:
    return bResult;
}

// --------------------置换物品位置-------------------------
BOOL KTongRepertory::ExchangeItemPos(
    int nSrcPageIndex, int nSrcPos, int nSrcClientCounter, 
    int nDstPageIndex, int nDstPos, int nDstClientCounter
)
{
    BOOL                    bResult = false;
    KTONG_REPERTORY_GRID*   pSrcGrid = NULL;
    KTONG_REPERTORY_GRID*   pDstGrid = NULL;

    assert(nSrcPageIndex >= 0 && nSrcPageIndex < TONG_REPERTORY_PAGE_NUM);
    assert(nDstPageIndex >= 0 && nDstPageIndex < TONG_REPERTORY_PAGE_NUM);
    assert(nSrcPos >= 0 && nSrcPos < TONG_REPERTORY_PAGE_CAPACITY);
    assert(nDstPos >= 0 && nDstPos < TONG_REPERTORY_PAGE_CAPACITY);

    LOG_PROCESS_ERROR(nSrcPageIndex != nDstPageIndex || nSrcPos != nDstPos);
    LOG_PROCESS_ERROR(m_Page[nSrcPageIndex].bEnable);
    LOG_PROCESS_ERROR(m_Page[nDstPageIndex].bEnable);

    pSrcGrid = &m_Page[nSrcPageIndex].Grids[nSrcPos];
    LOG_PROCESS_ERROR(pSrcGrid->nLastUpdateCounter <= nSrcClientCounter);
    pDstGrid = &m_Page[nDstPageIndex].Grids[nDstPos];
    LOG_PROCESS_ERROR(pDstGrid->nLastUpdateCounter <= nDstClientCounter);

    PROCESS_ERROR(pSrcGrid->nLockTimer < g_pGameCenter->m_nTimeNow);
    LOG_CHECK_ERROR(pSrcGrid->nLockTimer == 0);
    PROCESS_ERROR(pDstGrid->nLockTimer < g_pGameCenter->m_nTimeNow);
    LOG_CHECK_ERROR(pDstGrid->nLockTimer == 0);

    // 两个格子中至少一个有道具
    LOG_PROCESS_ERROR(pSrcGrid->pbyItemData || pDstGrid->pbyItemData);

	std::swap(pSrcGrid->dwTakePlayerId,   pDstGrid->dwTakePlayerId);
	std::swap(pSrcGrid->nTakeTime,        pDstGrid->nTakeTime);
	std::swap(pSrcGrid->dwPutPlayerId,    pDstGrid->dwPutPlayerId);
	std::swap(pSrcGrid->nPutTime,         pDstGrid->nPutTime);
	std::swap(pSrcGrid->nExpireTime,      pDstGrid->nExpireTime);
	std::swap(pSrcGrid->bRemoveOnExpire,  pDstGrid->bRemoveOnExpire);
	std::swap(pSrcGrid->byItemDataLen,    pDstGrid->byItemDataLen);
    std::swap(pSrcGrid->pbyItemData,      pDstGrid->pbyItemData);

    pSrcGrid->nLastUpdateCounter = ++m_nUpdateCounter;
    pDstGrid->nLastUpdateCounter = ++m_nUpdateCounter;

    bResult = true;
EXIT0:
    return bResult;
}

// --------------------叠加物品-------------------------
BOOL KTongRepertory::ApplyStackItem(
    int nPageIndex, int nPos, int nTabType, int nTabIndex, int nStakNum, int nMaxStackNum, int nClientCounter
)
{
    BOOL                    bResult         = false;
    BOOL                    bRetCode        = false;
    //TDBItemData*			pItemData       = NULL;
    //KTONG_REPERTORY_GRID*   pGrid           = NULL;

    //LOG_PROCESS_ERROR(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    //LOG_PROCESS_ERROR(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);

    //pGrid = &m_Page[nPageIndex].Grids[nPos];
    //LOG_PROCESS_ERROR(pGrid->nLastUpdateCounter <= nClientCounter);
    //PROCESS_ERROR(pGrid->nLockTimer < g_pGameCenter->m_nTimeNow);
    //LOG_CHECK_ERROR(pGrid->nLockTimer == 0);

    //LOG_PROCESS_ERROR(pGrid->byItemDataLen >= sizeof(TDBItemData));
    //pItemData = (TDBItemData*)pGrid->pbyItemData;
    //PROCESS_ERROR(pItemData);

    //PROCESS_ERROR(nTabType == pItemData->byTabType);
    //PROCESS_ERROR(nTabIndex == pItemData->wTabIndex);
    //PROCESS_ERROR(nStakNum + pItemData->wCurDur <= nMaxStackNum);

    //pGrid->nLockTimer = g_pGameCenter->m_nTimeNow + KTONG_REPERTORY_GRID_LOCK_TIME;

    //bResult = true;
EXIT0:
    return bResult;
}

BOOL KTongRepertory::StackItem(
    int nPageIndex, int nPos, int nTabType, int nTabIndex, int nStakNum, int nMaxStackNum
)
{
    BOOL                    bResult         = false;
    BOOL                    bRetCode        = false;
    //TDBItemData*			pItemData       = NULL;
    //KTONG_REPERTORY_GRID*   pGrid           = NULL;
    //int                     nTotalStackNum  = 0;

    //LOG_PROCESS_ERROR(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    //LOG_PROCESS_ERROR(nPos >= 0 && nPos < TONG_REPERTORY_PAGE_CAPACITY);

    //pGrid = &m_Page[nPageIndex].Grids[nPos];
    //PROCESS_ERROR(pGrid->nLockTimer >= g_pGameCenter->m_nTimeNow);    // 当前格子必须在之前的Apply操作中被锁定

    //PROCESS_ERROR(pGrid->byItemDataLen >= sizeof(TDBItemData));
    //pItemData = (TDBItemData*)pGrid->pbyItemData;
    //PROCESS_ERROR(pItemData);

    //LOG_PROCESS_ERROR(nTabType == pItemData->byTabType);
    //LOG_PROCESS_ERROR(nTabIndex == pItemData->wTabIndex);
    //
    //nTotalStackNum = nStakNum + pItemData->wStackCount;
    //LOG_PROCESS_ERROR(nTotalStackNum <= nMaxStackNum);

    //pItemData->wStackCount = (WORD)nTotalStackNum;
    //
    //pGrid->nLockTimer = 0;  // 操作完毕，解锁
    //pGrid->nLastUpdateCounter = ++m_nUpdateCounter;

    //bResult = true;
EXIT0:
    return bResult;
}

// --------------------同步物品-------------------------

// 同步一页中的变更的物品栏
BOOL KTongRepertory::GetSyncPageUpdateGridData(
    int nPageIndex, int nClientUpdateCounter, 
    BYTE* pbyBuffer, const size_t uBufferSize, int* pnCount, size_t* puUsedSize
)
{
    BOOL                    bResult         = false;
    size_t                  uItemDataSize   = 0;
    int                     nCount          = 0;
    size_t                  uLeftSize       = uBufferSize;
    BYTE*                   pbyOffset       = pbyBuffer;
    KTONG_ITEM_SYNC_DATA*   pSyncData       = NULL;
    KTONG_REPERTORY_GRID*   pGrid           = NULL;
    BYTE*                   pbyItemData     = NULL;
	KRole*                  pRole           = NULL;
    
    assert(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
    *pnCount = 0;

    for (int i = 0 ; i < TONG_REPERTORY_PAGE_CAPACITY; i++)
    {
        pGrid = &m_Page[nPageIndex].Grids[i];
        if (pGrid->nLastUpdateCounter <= nClientUpdateCounter)
            continue;

        pbyItemData = pGrid->pbyItemData;

        if (pbyItemData == NULL)
        {
            continue;
        }

        uItemDataSize = sizeof(KTONG_ITEM_SYNC_DATA) + (size_t)pGrid->byItemDataLen;

        LOG_PROCESS_ERROR(uLeftSize >= uItemDataSize);

        pSyncData = (KTONG_ITEM_SYNC_DATA*)pbyOffset;
        pSyncData->byPos             = (BYTE)i;
		pSyncData->byRent            = pGrid->dwTakePlayerId == KD_BAD_ID ? 0 : 1;
		pSyncData->byRemoveOnExpire  = pGrid->bRemoveOnExpire;
		pSyncData->nExpireTime       = pGrid->dwTakePlayerId == KD_BAD_ID ? 0 : pGrid->nExpireTime;
		memset(pSyncData->szRenterName, 0, sizeof(pSyncData->szRenterName));
		if (pSyncData->byRent)
		{
			pRole = g_pGameCenter->m_RoleManager.GetRole(pGrid->dwTakePlayerId);
			LOG_PROCESS_ERROR(pRole);
			strncpy(pSyncData->szRenterName, pRole->m_szName, sizeof(pSyncData->szRenterName));
		}
		memset(pSyncData->szPutterName, 0 , sizeof(pSyncData->szPutterName));
		if (pGrid->dwPutPlayerId)
		{
			pRole = g_pGameCenter->m_RoleManager.GetRole(pGrid->dwPutPlayerId);
			LOG_PROCESS_ERROR(pRole);
			strncpy(pSyncData->szPutterName, pRole->m_szName, sizeof(pRole->m_szName));
		}
        pSyncData->byDataLen = pGrid->byItemDataLen;
        memcpy(pSyncData->byData, pbyItemData, pGrid->byItemDataLen);

        nCount++;
        pbyOffset += uItemDataSize;
        uLeftSize -= uItemDataSize;
    }

    *pnCount    = nCount;
    *puUsedSize = uBufferSize - uLeftSize;

    bResult = true;
EXIT0:
    return bResult;
}

// 同步一页中的失效物品栏位
BOOL KTongRepertory::GetSyncPageFreeGridData(
    int nPageIndex, int nClientUpdateCounter, 
    BYTE* pbyBuffer, const size_t uBufferSize, int* pnCount, size_t* puUsedSize
)
{
    BOOL                    bResult     = false;
    KTONG_REPERTORY_GRID*   pGrid       = NULL;
    size_t                  uLeftSize   = uBufferSize;
    BYTE*                   pbyOffset   = pbyBuffer;
    int                     nCount      = 0;

    assert(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);

    *puUsedSize = 0;
    *pnCount    = 0;
    PROCESS_SUCCESS(nClientUpdateCounter == -1);

    for (int i = 0 ; i < TONG_REPERTORY_PAGE_CAPACITY; i++)
    {
        pGrid = &m_Page[nPageIndex].Grids[i];
        if (pGrid->nLastUpdateCounter <= nClientUpdateCounter)
            continue;

        if (!pGrid->pbyItemData)
        {
            LOG_PROCESS_ERROR(uLeftSize >= sizeof(BYTE));
            *pbyOffset = (BYTE)i;
            nCount++;

            pbyOffset++;
            uLeftSize--;
        }
    }

    *pnCount    = nCount;
    *puUsedSize = uBufferSize - uLeftSize;

EXIT1:
    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTongRepertory::Load(BYTE* pbyData, size_t uDataLen)
{
    BOOL                        bResult     = false;
    KTONG_REPERTORY_PAGE*       pPage       = NULL;
    KTONG_REPERTORY_GRID*       pGrid       = NULL;
    BYTE*                       pbyItemData = NULL;
    KTONG_REPERTORY_DB_PAGE*    pDBPage     = NULL;
    KTONG_REPERTORY_DB_ITEM*    pDBItem     = NULL;
    BYTE*                       pbyOffset   = pbyData;
    size_t                      uLeftSize   = uDataLen;
    size_t                      uItemDBSize = 0;
    int                         nCount      = 0;
    int                         nPageIndex  = 0;
    
    assert(pbyData);

    while (uLeftSize > 0)
    {
        LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_REPERTORY_DB_PAGE));
        pDBPage = (KTONG_REPERTORY_DB_PAGE*)pbyOffset;
        pbyOffset += sizeof(KTONG_REPERTORY_DB_PAGE);
        uLeftSize -= sizeof(KTONG_REPERTORY_DB_PAGE);

        nPageIndex = (int)pDBPage->byPageIndex;
        LOG_PROCESS_ERROR(nPageIndex >= 0 && nPageIndex < TONG_REPERTORY_PAGE_NUM);
        pPage = &m_Page[nPageIndex];

        LOG_PROCESS_ERROR(!pPage->bEnable);
        pPage->bEnable = true;

		nCount = pDBPage->uItemDalaLen;
        while (nCount--)
        {
            LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_REPERTORY_DB_ITEM));
            pDBItem = (KTONG_REPERTORY_DB_ITEM*)pbyOffset;

            uItemDBSize = sizeof(KTONG_REPERTORY_DB_ITEM) + pDBItem->byDataLen;
            LOG_PROCESS_ERROR(uLeftSize >= uItemDBSize);

            pbyOffset += uItemDBSize;
            uLeftSize -= uItemDBSize;

            LOG_PROCESS_ERROR(pDBItem->byPos < TONG_REPERTORY_PAGE_CAPACITY);
            pGrid = &pPage->Grids[pDBItem->byPos];
            LOG_PROCESS_ERROR(!pGrid->pbyItemData);

            pGrid->nLockTimer = 0;
            pGrid->nLastUpdateCounter = 0;
            pGrid->pbyItemData = (BYTE*)malloc(pDBItem->byDataLen);
            LOG_PROCESS_ERROR(pGrid->pbyItemData);

            pbyItemData = pGrid->pbyItemData;
            pGrid->byItemDataLen = pDBItem->byDataLen;
            memcpy(pbyItemData, pDBItem->byData, pDBItem->byDataLen);
        }
    }

    bResult = true;
EXIT0:
    if (!bResult)
    {
        for (int  i = 0 ; i < TONG_REPERTORY_PAGE_NUM; i++)
        {
            for (int j = 0 ; j < TONG_REPERTORY_PAGE_CAPACITY; j++)
                SAFE_DELETE(m_Page[i].Grids[j].pbyItemData);
        }
    }

    return bResult;
}

BOOL KTongRepertory::Save(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUseSize)
{
    BOOL                        bResult     = false;
    KTONG_REPERTORY_PAGE*       pPage       = NULL;
    KTONG_REPERTORY_GRID*       pGrid       = NULL;
    KTONG_REPERTORY_DB_PAGE*    pDBPage     = NULL;
    KTONG_REPERTORY_DB_ITEM*    pDBItem     = NULL;
    BYTE*                       pbyOffset   = pbyBuffer;
    size_t                      uLeftSize   = uBufferSize;
    size_t                      uItemSize   = 0;
    int                         nCount      = 0;

    assert(pbyBuffer);
    assert(puUseSize);

    for(int i = 0; i < TONG_REPERTORY_PAGE_NUM; i++)
    {
        pPage = &m_Page[i];

        if (!pPage->bEnable)
        {
            break;
        }

        LOG_PROCESS_ERROR(uLeftSize >= sizeof(KTONG_REPERTORY_DB_PAGE));
        pDBPage = (KTONG_REPERTORY_DB_PAGE*)pbyOffset;
        pbyOffset += sizeof(KTONG_REPERTORY_DB_PAGE);
        uLeftSize -= sizeof(KTONG_REPERTORY_DB_PAGE);

        pDBPage->byPageIndex = (BYTE)i;

        nCount = 0;
        for (int j = 0; j < TONG_REPERTORY_PAGE_CAPACITY; j++)
        {
            pGrid = &pPage->Grids[j];
            if (pGrid->pbyItemData == NULL)
            {
                continue;
            }

            uItemSize = sizeof(KTONG_REPERTORY_DB_ITEM) + pGrid->byItemDataLen;
            LOG_PROCESS_ERROR(uLeftSize >= uItemSize);

            pDBItem = (KTONG_REPERTORY_DB_ITEM*)pbyOffset;
            pbyOffset += uItemSize;
            uLeftSize -= uItemSize;

            pDBItem->byPos  = (BYTE)j;
			pDBItem->byDataLen = pGrid->byItemDataLen;
            memcpy(pDBItem->byData, pGrid->pbyItemData, pGrid->byItemDataLen);

            nCount++;
        }

		pDBPage->uItemDalaLen = (BYTE)nCount;
    }

    *puUseSize = pbyOffset - pbyBuffer;

    bResult = true;
EXIT0:
    return bResult;
}

BOOL KTongRepertory::FindItem( BYTE* pbyItemData, int* pnPageIndex /*= NULL*/, int* pnPos /*= NULL*/ )
{
	assert(pbyItemData);
	TDBItemData* pItemData = (TDBItemData*)pbyItemData;
	BOOL bResult = false;

	for (int i = 0; i < TONG_REPERTORY_PAGE_NUM; ++i)
	{
		KTONG_REPERTORY_PAGE* pPage = &m_Page[i];
		assert(pPage);
		if (pPage->bEnable)
		{
			for (int j = 0; j < TONG_REPERTORY_PAGE_CAPACITY; ++j)
			{
				KTONG_REPERTORY_GRID* pGrid = &pPage->Grids[j];
				assert(pGrid);
				TDBItemData* pGridItemData = (TDBItemData*)pGrid->pbyItemData;

				if (pGridItemData != NULL && pItemData->sGuid == pGridItemData->sGuid)
				{
					bResult = TRUE;
					if (pnPageIndex) *pnPageIndex = i;
					if (pnPos) *pnPos = j;
					break;
				}
			}

			if (bResult) 
			{
				break;
			}
		}
	}

	return bResult;
}

void KTongRepertory::CheckRentOnExpireItem()
{
	for (int i = 0; i < TONG_REPERTORY_PAGE_NUM; ++i)
	{
		KTONG_REPERTORY_PAGE* pPage = &m_Page[i];
		assert(pPage);
		if (pPage->bEnable)
		{
			for (int j = 0; j < TONG_REPERTORY_PAGE_CAPACITY; ++j)
			{
				KTONG_REPERTORY_GRID* pGrid = &pPage->Grids[j];
				if (pGrid->dwTakePlayerId != KD_BAD_ID && pGrid->nExpireTime < g_pGameCenter->m_nTimeNow)
				{
					if (pGrid->bRemoveOnExpire)
					{
						LockGrid(i, j);
						FreeItem(i, j);
					}
					else
					{
						pGrid->dwTakePlayerId = 0;
						pGrid->nTakeTime = 0;
						pGrid->nExpireTime = 0;
						pGrid->nLastUpdateCounter = ++m_nUpdateCounter;
					}
				}
			}
		}
	}
}
