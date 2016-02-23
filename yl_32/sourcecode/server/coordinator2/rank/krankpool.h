/* -------------------------------------------------------------------------
//	文件名		：	krankpool.h
//	创建者		：	zhaoyu
//	创建时间	：	2013/08/10
//	功能描述	：	排序模块
//
// -----------------------------------------------------------------------*/
#ifndef __KRANKPOOL_H__
#define __KRANKPOOL_H__

#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// -------------------------------------------------------------------------

#pragma	pack(1)
struct KRankDBHead
{
	WORD wCurRankSize;
	WORD wMaxRankSize;
};
#pragma	pack()

struct KRankInfo 
{
	DWORD dwValue;
	DWORD dwId;
	DWORD dwModifyTime;
};

#define RANK_OUTDATE_TIME (7 * 3600 * 24)			// 7天超时时间

class KRankPool
{
public:
	KRankPool(void);
	~KRankPool(void);
	KRankPool(CONST KRankPool&);

public:
	BOOL Init(INT nMaxCount);
	BOOL UnInit();

	VOID SetId(INT nId) { m_nId = nId; }
	INT GetId() { return m_nId; }

	VOID ModifyRankValue(DWORD dwId, DWORD dwValue, DWORD dwModifyTime = 0, BOOL bIfRoll = TRUE,INT RankID = 0,BYTE byRankId = 0);
	VOID EraseRankValue(DWORD dwId);				//删除排行榜信息
	BOOL SwapRank(DWORD dwId1, DWORD dwId2);

	INT GetIdByIdx(INT nIdx);
	INT GetIdxById(DWORD dwId);
	DWORD GetValueById(DWORD dwId);
	DWORD GetValueByIdx(INT nIdx);
	DWORD GetLastModifyTime(INT nIdx);

	BOOL Save(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize);
	BOOL Load(BYTE* pbyData, size_t uDataLen);

	size_t GetSaveSize();

	VOID Test();
	VOID Sort();

	std::map<DWORD, INT> m_mapId2Idx;
	std::map<DWORD, DWORD> m_mapId2Value;

	

private:

	KRankInfo* m_pRank;


	INT m_nMaxRank;
	INT m_nIdx;

	INT m_nId;
private:
	KRankInfo* GetRankByIdx(INT nIdx);

	VOID AddRankValue(DWORD dwId, DWORD dwValue, DWORD dwModifyTime = 0, BOOL bIfRoll = TRUE);

	VOID SwapRankInfo(INT nLeftIdx, INT nRightIdx);

	VOID RollForward(INT nStartIdx);
	VOID RollBackward(INT nStartIdx);

	VOID CreateIdxMap();

	BOOL IsOutDate(KRankInfo* pInfo);
};


// -------------------------------------------------------------------------

#endif /* __KRANKPOOL_H__ */
