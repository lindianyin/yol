#ifndef _KTONG_REPERTORY_H_
#define _KTONG_REPERTORY_H_

#include "KinDef.h"

// 单元格
struct KTONG_REPERTORY_GRID
{
    time_t    nLockTimer;
    int       nLastUpdateCounter;
    BYTE      byItemDataLen;
	BYTE*     pbyItemData;  

	// added by liyun
	DWORD     dwPutPlayerId;            // 装备的主人
	time_t	  nPutTime;	                // 放入仓库的时间
	DWORD     dwTakePlayerId;           // 拿走装备的人
	time_t    nTakeTime;                // 拿走时间
	time_t    nExpireTime;              // 到期时间
	BOOL      bRemoveOnExpire;          // 到期后删除
};

// 仓库页面
struct KTONG_REPERTORY_PAGE
{
    BOOL                    bEnable; 
    KTONG_REPERTORY_GRID    Grids[TONG_REPERTORY_PAGE_CAPACITY];
};

class KTongRepertory
{
public:
    KTongRepertory();
    BOOL Init();
    void UnInit();

    KTONG_REPERTORY_GRID* GetRepertoryGrid(int nPageIndex, int nPos);
    KTONG_REPERTORY_GRID* TakeItem(int nPageIndex, int nPos, int nClientCounter, DWORD dwTakerId, time_t nTakeTime);

    BOOL FreeItem(int nPageIndex, int nPos);

    BOOL RecoverItem(int nPageIndex, int nPos);

    BOOL ApplyEmptyGrid(int nPageIndex, int nPos);

    BOOL UnlockGrid(int nPageIndex, int nPos);

	BOOL LockGrid(int nPageIndex, int nPos);

    BOOL PutItem(int nPageIndex, int nPos, BYTE* pbyData, size_t uDataLen, DWORD dwPutPlayerId, time_t nPutTime);

    BOOL ExchangeItemPos(
        int nSrcPageIndex, int nSrcPos, int nSrcClientCounter, 
        int nDstPageIndex, int nDstPos, int nDstClientCounter
    );

    BOOL ApplyStackItem(
        int nPageIndex, int nPos, 
        int nTabType, int nTabIndex, int nStakNum, int nMaxStackNum, 
        int nClientCounter
    );

    BOOL StackItem(
        int nPageIndex, int nPos, int nTabType, int nTabIndex, int nStakNum, int nMaxStackNum
    );

    BOOL FindEmptyGrid(int nPageIndex, int* pnRetEmptyPos);

    BOOL GetSyncPageUpdateGridData(
        int nPageIndex, int nClientUpdateCounter, 
        BYTE* pbyBuffer, const size_t uBufferSize, int* pnCount, size_t* puUsedSize
    );

    BOOL GetSyncPageFreeGridData(
        int nPageIndex, int nClientUpdateCounter, 
        BYTE* pbyBuffer, const size_t uBufferSize, int* pnCount, size_t* puUsedSize
    );

    BOOL Load(BYTE* pbyData, size_t uDataLen);
    BOOL Save(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUseSize);

	BOOL FindItem(BYTE* pbyItemData, int* pnPageIndex = NULL, int* pnPos = NULL);
	
	void CheckRentOnExpireItem();

	static Kin_Repertory_Rent_Config& GetRentConfig()
	{
		static Kin_Repertory_Rent_Config s_config = {1, 1, 7 * 24 * 60 * 60};
		return s_config;
	}

public:
    int   m_nUpdateCounter;
    KTONG_REPERTORY_PAGE m_Page[TONG_REPERTORY_PAGE_NUM];
};

#endif // _KTONG_REPERTORY_H_
