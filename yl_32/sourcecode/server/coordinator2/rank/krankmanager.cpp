#include "stdafx.h"
#include "krankmanager.h"
#include "GameCenter.h"
#include "DBTools.h"
#include "serverbase/RankDef.h"

KRankManager::KRankManager()
{

}

BOOL KRankManager::Init()
{
    BOOL bResult    = false;

    bResult = true;
EXIT0:
    return bResult;
}

void KRankManager::UnInit()
{
	KRankPool* pRankPool = NULL;
	for(KRANK_TABLE::iterator it = m_RankTable.begin(); it != m_RankTable.end(); ++it)
	{
		pRankPool = &it->second;
		g_pGameCenter->m_MiscDB.DoSaveRankData(pRankPool->GetId(), pRankPool);

		it->second.UnInit();
	}

	m_RankTable.clear();
}

void KRankManager::Activate()
{   
    
}

BOOL KRankManager::CreateRankPool(INT nPoolId, INT nMaxCount)
{
	BOOL									bResult		= FALSE;
	KRankPool*								pRankPool	= NULL;
	std::pair<KRANK_TABLE::iterator, BOOL>	RankInsRet;

	ASSERT(nPoolId > emRANK_ID_BEGIN && nPoolId < emRANK_ID_END);

	RankInsRet = m_RankTable.insert(std::make_pair(nPoolId, KRankPool()));
	LOG_PROCESS_ERROR(RankInsRet.second);

	pRankPool = &RankInsRet.first->second;

	pRankPool->Init(nMaxCount);

	pRankPool->SetId(nPoolId);

	bResult = TRUE;
EXIT0:
	if (!bResult)
	{
		if (pRankPool)
			pRankPool->UnInit();

		m_RankTable.erase(nPoolId);
	}

	return bResult;
}

void KRankManager::SaveAll()
{
	for(KRANK_TABLE::iterator it = m_RankTable.begin(); it != m_RankTable.end(); ++it)
	{
		KRankPool* pRankPool = &it->second;

		g_pGameCenter->m_MiscDB.DoSaveRankData(it->first, pRankPool);
	}
}

BOOL KRankManager::LoadRankData(INT nRankId, BYTE* pbyData, size_t uRankDataLen)
{
	BOOL                                    bResult     = false;
	BOOL                                    bRetCode    = false;
	KRankPool*								pRankPool	= NULL;
	std::pair<KRANK_TABLE::iterator, bool>	InsertRet;

	ASSERT(pbyData);

	InsertRet = m_RankTable.insert(std::make_pair(nRankId, KRankPool()));
	LOG_PROCESS_ERROR(InsertRet.second);

	pRankPool = &InsertRet.first->second;

	bRetCode = pRankPool->Load(pbyData, uRankDataLen);
	LOG_PROCESS_ERROR(bRetCode);

	bResult = true;

EXIT0:
	if(!bResult)
	{
		pRankPool->UnInit();
		m_RankTable.erase(nRankId);
	}
	return bResult;
}

INT KRankManager::GetPoolMaxCount(INT nRankId)
{
	INT nMaxCount = RANK_POOL_MAX_COUNT;
	switch(nRankId)
	{
	case emRANK_ARENA:
		nMaxCount = emRANK_ARENA_MAX_COUNT;
		break;
	case emRANK_ROLE_FIGHTSCORE:
		nMaxCount = emRANK_MAX_COUNT;
		break;
	case emRANK_ROLE_LEVEL_OPEN_SERVER:
	case emRANK_TOWER:
		nMaxCount = 10;
		break;
	case emRANK_AREAN_OS:
		nMaxCount = 10;
		break;
	default:
		break;
	}

	return nMaxCount;
}

KRankPool* KRankManager::GetRankPool(INT nRankId)
{
	INT nMaxCount = 0;
	KRankPool* pRankPool = NULL;
	KRANK_TABLE::iterator it;

	ASSERT(nRankId > emRANK_ID_BEGIN && nRankId < emRANK_ID_END);

	it = m_RankTable.find(nRankId);
	if (it != m_RankTable.end())
	{
		pRankPool = &it->second;
	}
	else
	{
		nMaxCount = GetPoolMaxCount(nRankId);
		LOG_PROCESS_ERROR(CreateRankPool(nRankId, nMaxCount));
		pRankPool = GetRankPool(nRankId);
	}

EXIT0:
	return pRankPool;
}

BOOL KRankManager::ModifyRankValue(INT nRankId, DWORD dwId, DWORD dwValue, DWORD dwModifyTime /* = 0 */)
{
	BOOL bResult = FALSE;
	KRankPool* pRankPool = GetRankPool(nRankId);
	LOG_PROCESS_ERROR(pRankPool);

	pRankPool->ModifyRankValue(dwId, dwValue, dwModifyTime,TRUE,nRankId);

	bResult = TRUE;
EXIT0:
	return bResult;
}
BOOL KRankManager::EraseRankValue( INT nRankId, DWORD dwId )
{
	BOOL bResult = FALSE;
	KRankPool* pRankPool = GetRankPool(nRankId);
	LOG_PROCESS_ERROR(pRankPool);

	pRankPool->EraseRankValue(dwId);

	bResult = TRUE;
EXIT0:
	return bResult;
}
