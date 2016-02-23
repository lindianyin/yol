

#include "stdafx.h"
#include "krankpool.h"

#include "serverbase/RankDef.h"
#include "KRole.h"

#include "GameCenter.h"
#include "onlinegamebase/ksysservice.h"
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
KRankPool::KRankPool(void)
{
	m_pRank = NULL;
}

KRankPool::~KRankPool(void)
{
}

// 拷贝构造函数
KRankPool::KRankPool(CONST KRankPool& sPool)
{
	m_nIdx = 0;
	m_nMaxRank = 0;
	m_pRank = NULL;
}

BOOL KRankPool::Init(INT nMaxCount)
{
	if(!m_pRank)
	{
		m_nIdx = 0;

		m_nMaxRank = nMaxCount;
		m_pRank = new KRankInfo[nMaxCount];
		
		memset(m_pRank, 0, sizeof(KRankInfo) * nMaxCount);
	}
EXIT0:
	return TRUE;
}

BOOL KRankPool::UnInit()
{
	SAFE_DELETE_ARRAY(m_pRank);
	m_pRank = NULL;
	m_mapId2Idx.clear();
	m_mapId2Value.clear();

	return TRUE;
}

KRankInfo* KRankPool::GetRankByIdx(INT nIdx)
{
	KRankInfo* pInfo = NULL;
	PROCESS_ERROR(nIdx >= 0 && nIdx <= m_nIdx);

	pInfo = &m_pRank[nIdx];

EXIT0:
	return pInfo;
}

VOID KRankPool::SwapRankInfo(INT nLeftIdx, INT nRightIdx)
{
	DWORD dwTmpId;
	DWORD dwTmpValue;
	DWORD dwTmpModifyTime;

	KRankInfo* pLeft = GetRankByIdx(nLeftIdx);
	KRankInfo* pRight = GetRankByIdx(nRightIdx);

	LOG_PROCESS_ERROR(pLeft && pRight);

	dwTmpId = pLeft->dwId;
	dwTmpValue = pLeft->dwValue;
	dwTmpModifyTime = pLeft->dwModifyTime;

	pLeft->dwId = pRight->dwId;
	pLeft->dwValue = pRight->dwValue;
	pLeft->dwModifyTime = pRight->dwModifyTime;

	pRight->dwId = dwTmpId;
	pRight->dwValue = dwTmpValue;
	pRight->dwModifyTime = dwTmpModifyTime;

EXIT0:
	return;
}


// 交换两个id的排名
BOOL KRankPool::SwapRank(DWORD dwId1, DWORD dwId2)
{
	BOOL bResult = FALSE;
	INT nIdx1 = GetIdxById(dwId1);
	INT nIdx2 = GetIdxById(dwId2);
	LOG_PROCESS_ERROR(nIdx1 >= 0 && nIdx2 >= 0);

	m_mapId2Idx[dwId1] = nIdx2;
	m_mapId2Idx[dwId2] = nIdx1;

	SwapRankInfo(nIdx1, nIdx2);

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankPool::IsOutDate(KRankInfo* pInfo)
{
	BOOL bOutDate = FALSE;
	time_t tNow = time(NULL);

	if(pInfo->dwModifyTime > 0 && tNow - pInfo->dwModifyTime > RANK_OUTDATE_TIME)
	{
		bOutDate = TRUE;
	}

	return bOutDate;
}

// 向前找到合适的位置
VOID KRankPool::RollForward(INT nStartIdx)
{
	KRankInfo* pLeft = NULL;
	KRankInfo* pRight = NULL;

	ASSERT(nStartIdx >= 0 && nStartIdx <= m_nIdx);

	pRight = GetRankByIdx(nStartIdx);
	m_mapId2Idx[pRight->dwId] = nStartIdx;

	for(INT i = nStartIdx; i > 0; --i)
	{
		pRight = GetRankByIdx(i);
		for(INT j = i - 1; j >= 0; --j)
		{
			pLeft = GetRankByIdx(j);
			if(pRight->dwValue > pLeft->dwValue || IsOutDate(pLeft))
			{
				m_mapId2Idx[pLeft->dwId] = i;
				m_mapId2Idx[pRight->dwId] = j;

				SwapRankInfo(i, j);
				break;
			}
		}
	}
}

// 向后找到合适的位置
VOID KRankPool::RollBackward(INT nStartIdx)
{
	KRankInfo* pLeft = NULL;
	KRankInfo* pRight = NULL;

	ASSERT(nStartIdx >= 0 && nStartIdx <= m_nIdx);

	pLeft = GetRankByIdx(nStartIdx);
	m_mapId2Idx[pLeft->dwId] = nStartIdx;

	for(INT i = nStartIdx; i < m_nIdx - 1; ++i)
	{
		pLeft = GetRankByIdx(i);
		for(INT j = i + 1; j < m_nIdx; ++j)
		{
			pRight = GetRankByIdx(j);
			if(pLeft->dwValue < pRight->dwValue)
			{
				m_mapId2Idx[pRight->dwId] = i;
				m_mapId2Idx[pLeft->dwId] = j;

				SwapRankInfo(i, j);
				break;
			}
		}
	}
}

VOID KRankPool::AddRankValue(DWORD dwId, DWORD dwValue, DWORD dwModifyTime /* = 0 */, BOOL bIfRoll /* = TRUE */)
{
	INT nRightIdx = 0;
	KRankInfo* pInfo = NULL;

	PROCESS_ERROR(m_nIdx < m_nMaxRank);

	m_mapId2Value[dwId] = dwValue;

	pInfo = GetRankByIdx(m_nIdx);

	pInfo->dwId = dwId;
	pInfo->dwValue = dwValue;
	pInfo->dwModifyTime = dwModifyTime;

	if(bIfRoll)
	{
		RollForward(m_nIdx);
	}
	else
	{
		m_mapId2Idx[dwId] = m_nIdx;
	}

	++m_nIdx;

EXIT0:
	return;
}
VOID KRankPool::EraseRankValue(DWORD dwId)
{
	std::map<DWORD, INT>::iterator it;
	it = m_mapId2Idx.find(dwId);
	if(it != m_mapId2Idx.end())
	{
		m_mapId2Idx.erase(it);
	}
	std::map<DWORD, DWORD>::iterator it1;
	it1 = m_mapId2Value.find(dwId);
	if (it1 != m_mapId2Value.end())
	{
		m_mapId2Value.erase(it1);
	}

EXIT0:
	return;
}

VOID KRankPool::ModifyRankValue(DWORD dwId, DWORD dwValue, DWORD dwModifyTime /* = 0 */, BOOL bIfRoll /* = TRUE */,INT RankID /*= 0*/,BYTE byRankID/*  = 0*/)
{
	INT nIdx = 0;
	INT nDelta = 0;
	KRankInfo* pInfo = NULL;
	KRole* pRole = NULL;
	INT nLevel = 0;
	std::map<DWORD, INT>::iterator it;
	std::map<DWORD, DWORD>::iterator it_value;
	it = m_mapId2Idx.find(dwId);
	//int nDay = ((time(NULL)) + 8*60*60) / 86400 - ((g_pGameCenter->m_RecordManager.GetOpenServerTime()) + 8*60*60) / 86400 + 1;
	int nDay = KSysService::GetDayDiff(g_pGameCenter->m_RecordManager.GetOpenServerTime(),time(NULL)) + 1;
	//家族的排行榜: dwId = 家族的ID
	if(	RankID != emRANK_TONG_FIGHTSCORE  && RankID != emRANK_TONG_SALARY)
	{
		pRole = g_pGameCenter->m_RoleManager.GetRole(dwId);
		LOG_PROCESS_ERROR(pRole);
		nLevel = pRole->GetLevel();
		switch(RankID)
		{
		case emRANK_ROLE_LEVEL_OPEN_SERVER:
			{
				if (dwValue < 45 || nDay > 1)
				{
					goto EXIT0;
				}
				break;
			}
		case emRANK_RIDE_LV:
			{
				if (dwValue < 4 || nDay > 2 )
				{
					goto EXIT0;
				}
				break;
			}
		case emRANK_AREAN_OS:
			{
				if (nLevel < 45 || nDay > 3)
				{
					goto EXIT0;
				}
				break;
			}
		case emRANK_PET_LV_OS:
			{
				if (dwValue < 80000 || nDay > 4)
				{
					goto EXIT0;
				}
				break;
			}
		case emRANK_LG_OS:
			{
				if (dwValue < 8 || nDay > 5)
					{
						goto EXIT0;
					}
					break;
			}
		case emRANK_TOWER:
			{
				if (dwValue < 30 || nDay > 6)
					{
						goto EXIT0;
					}
					break;
			}
		case emRANK_ROLE_FIGSHTSCORE_OS:
			{
				if (dwValue < 30000 || nDay > 7 )
				{
					goto EXIT0;
				}
				break;
			}
		default:
			break;
		}
	}
	
	// 排行满了的情况
	if(it == m_mapId2Idx.end() && m_nIdx >= m_nMaxRank)
	{
		DWORD dwLastId = GetIdByIdx(m_nIdx - 1);
		DWORD dwLastValue = GetValueByIdx(m_nIdx - 1);

		// 跟最后一名对比
		if(dwValue <= dwLastValue && bIfRoll)
		{
			goto EXIT0;
		}
		else
		{
			it = m_mapId2Idx.find(dwLastId);
			if(it != m_mapId2Idx.end())
			{
				m_mapId2Idx.erase(it);
			}

			it_value = m_mapId2Value.find(dwLastId);
			if(it_value != m_mapId2Value.end())
			{
				m_mapId2Value.erase(it_value);
			}

			m_mapId2Value[dwId] = dwValue;

			pInfo = GetRankByIdx(m_nIdx - 1);

			pInfo->dwId = dwId;
			pInfo->dwValue = dwValue;
			pInfo->dwModifyTime = dwModifyTime;

			if(bIfRoll)
			{
				RollForward(m_nIdx - 1);
			}
			
			goto EXIT0;
		}
	}

	it = m_mapId2Idx.find(dwId);
	if(it == m_mapId2Idx.end())
	{
		AddRankValue(dwId, dwValue, dwModifyTime, bIfRoll);
		goto EXIT0;
	}

	nIdx = it->second;

	pInfo = GetRankByIdx(nIdx);
	LOG_PROCESS_ERROR(pInfo);

	m_mapId2Value[dwId] = dwValue;

	nDelta = dwValue - pInfo->dwValue;

	pInfo->dwValue = dwValue;
	pInfo->dwModifyTime = dwModifyTime;
	
	if(bIfRoll)
	{
		if(nDelta > 0)
		{
			RollForward(nIdx);
		}
		else
		{
			RollBackward(nIdx);
		}
	}
EXIT0:
	return;
}

// 根据排名获得id
INT KRankPool::GetIdByIdx(INT nIdx)
{
	DWORD dwId = 0;
	KRankInfo* pInfo = NULL;

	PROCESS_ERROR(nIdx >= 0 && nIdx < m_nIdx);

	pInfo = GetRankByIdx(nIdx);
	dwId = pInfo->dwId;

EXIT0:
	return dwId;
}

// 根据id获得排名
INT KRankPool::GetIdxById(DWORD dwId)
{
	INT nIdx = -1;
	std::map<DWORD, INT>::iterator it = m_mapId2Idx.find(dwId);
	if(it != m_mapId2Idx.end())
	{
		nIdx = it->second;
	}

EXIT0:
	return nIdx;
}

// 根据id找到排名值
DWORD KRankPool::GetValueById(DWORD dwId)
{
	DWORD dwValue = 0;
	std::map<DWORD, DWORD>::iterator it = m_mapId2Value.find(dwId);
	if(it != m_mapId2Value.end())
	{
		dwValue = it->second;
	}

EXIT0:
	return dwValue;
}

// 根据排名找到排名值
DWORD KRankPool::GetValueByIdx(INT nIdx)
{
	DWORD dwValue = 0;
	KRankInfo* pInfo = GetRankByIdx(nIdx);
	PROCESS_ERROR(pInfo);
	dwValue = pInfo->dwValue;

EXIT0:
	return dwValue;
}

DWORD KRankPool::GetLastModifyTime(INT nIdx)
{
	DWORD dwModifyTime = 0;
	KRankInfo* pInfo = GetRankByIdx(nIdx);
	PROCESS_ERROR(pInfo);
	dwModifyTime = pInfo->dwModifyTime;

EXIT0:
	return dwModifyTime;
}

VOID KRankPool::Test()
{
	INT arrId[5] = {102, 105, 101, 103, 110};
	INT arrValue[5] = {6000, 256, 343, 1000, 9999};
	for(INT i = 0; i < 5; ++i)
	{
		ModifyRankValue(arrId[i], arrValue[i]);
	}

	ModifyRankValue(110, 5000);

	SwapRank(102, 110);

	return;
}



BOOL KRankPool::Save(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize)
{
	BOOL bResult = FALSE;
	KRankDBHead* pHead = NULL;
	size_t uLeftSize = uBufferSize;
	BYTE* pbyOffset = pbyBuffer;

	ASSERT(pbyBuffer);
	ASSERT(uLeftSize > sizeof(KRankDBHead));

	pHead = (KRankDBHead*)pbyOffset;
	pHead->wCurRankSize = m_nIdx + 1;
	pHead->wMaxRankSize = m_nMaxRank;

	if(pHead->wCurRankSize > m_nMaxRank)
	{
		pHead->wCurRankSize = m_nMaxRank;
	}

	pbyOffset += sizeof(KRankDBHead);

	ASSERT(uBufferSize >= sizeof(KRankInfo) * pHead->wCurRankSize);
	memcpy(pbyOffset, (BYTE*)m_pRank, sizeof(KRankInfo) * pHead->wCurRankSize);

	pbyOffset += sizeof(KRankInfo) * pHead->wCurRankSize;

	*puUsedSize = pbyOffset - pbyBuffer;

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRankPool::Load(BYTE* pbyData, size_t uDataLen)
{
	BOOL bResult = FALSE;
	BYTE* pbyOffset = pbyData;
	KRankDBHead* pHead = NULL;
	size_t uLeftLen = uDataLen;

	ASSERT(pbyData);
	ASSERT(uLeftLen > 0);

	ASSERT(uLeftLen >= sizeof(KRankDBHead));

	pHead = (KRankDBHead*)pbyData;
	m_nIdx = pHead->wCurRankSize - 1;
	m_nMaxRank = pHead->wMaxRankSize;

	pbyOffset += sizeof(KRankDBHead);
	uLeftLen -= sizeof(KRankDBHead);

	ASSERT(uLeftLen >= sizeof(KRankInfo) * pHead->wCurRankSize);

	LOG_PROCESS_ERROR(pHead->wMaxRankSize >= pHead->wCurRankSize);

	m_pRank = new KRankInfo[pHead->wMaxRankSize];
	memcpy(m_pRank, pbyOffset, sizeof(KRankInfo) * pHead->wCurRankSize);

	pbyOffset += sizeof(KRankInfo) * pHead->wCurRankSize;

	CreateIdxMap();

	bResult = TRUE;
EXIT0:
	return bResult;
}

VOID KRankPool::CreateIdxMap()
{
	m_mapId2Idx.clear();
	m_mapId2Value.clear();

	for(INT i = 0; i < m_nIdx; ++i)
	{
		KRankInfo* pInfo = &m_pRank[i];
		m_mapId2Idx[pInfo->dwId] = i;
		m_mapId2Value[pInfo->dwId] = pInfo->dwValue;
	}
}

size_t KRankPool::GetSaveSize()
{
	size_t uSize = 0;
	uSize += sizeof(KRankDBHead);

	uSize += sizeof(KRankInfo) * (m_nIdx + 1);

	return uSize;
}