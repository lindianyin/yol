#include "stdafx.h"
#include "kawardmanager.h"
#include "kawardctrl.h"
#include "awarddef.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/kplayer.h"

KAwardManager g_cAwardMgr;

KAwardManager::KAwardManager()
{

}

KAwardManager::~KAwardManager()
{
	UnInit();
}

BOOL KAwardManager::Init()
{
	m_cAwardPool.Init(AWARD_POOL_MAX_COUNT);

	m_cAwardIdList.Init(AWARD_POOL_MAX_COUNT);

	return TRUE;
}

BOOL KAwardManager::UnInit()
{
	m_cAwardPool.Uninit();

	return TRUE;
}

INT KAwardManager::AddAward(DWORD dwId /*= 0*/)
{
	BOOL bRet = FALSE;
	INT nIndex = 0;
	KAward* pAward = NULL;

	dwId = m_cAwardIdList.FindFree();

	nIndex = m_cAwardPool.Add(dwId);

	m_cAwardIdList.Use(dwId);

	pAward = m_cAwardPool.GetByIndex(nIndex);
	LOG_PROCESS_ERROR(pAward);

	pAward->SetIndex(nIndex);

EXIT0:
	return nIndex;
}

KAward* KAwardManager::GetAwardByIndex(INT nIndex)
{
	return m_cAwardPool.GetByIndex(nIndex);
}

KAward* KAwardManager::GetAwardById(DWORD dwId)
{
	return m_cAwardPool.GetById(dwId);
}

KAward* KAwardManager::AddPlayerAward(KPlayer* pPlayer, LPCSTR szModuleName, INT nLevel, INT nTimes /* = 1 */, INT nAwardPercent /* = 100 */, BOOL bGiveExp /* = TRUE */)
{
	INT nIndex = 0;
	KAward* pAward = NULL;
	KAwardCtrl* pAwardCtrl = NULL;

	nIndex = AddAward();

	pAward = GetAwardByIndex(nIndex);
	LOG_PROCESS_ERROR(pAward); 

	LOG_PROCESS_ERROR(pAward->Init(pPlayer, szModuleName, nLevel, nTimes, nAwardPercent));

	pAward->m_bGiveExp = bGiveExp;

	pAwardCtrl = pPlayer->GetCtrl<KAwardCtrl>();
	LOG_PROCESS_ERROR(pAwardCtrl);

	pAwardCtrl->Add(pAward, pAward->IsSync());

EXIT0:
	return pAward;
}

BOOL KAwardManager::RemoveAward(DWORD dwId)
{
	KAward* pAward = g_cAwardMgr.GetAwardById(dwId);
	QCONFIRM_RET_FALSE(pAward);

	QCONFIRM_RET_FALSE(m_cAwardIdList.Free(dwId));

	return m_cAwardPool.Remove(dwId);
}